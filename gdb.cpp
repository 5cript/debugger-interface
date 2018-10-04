#include "gdb.hpp"

#include "process/process.hpp"
#include "input_parser.hpp"
#include "input/stream_record.hpp"
#include "input/async_record.hpp"
#include "distributing_listener.hpp"

#include <sstream>
#include <string>
#include <iostream>

namespace GdbInterface
{
//#####################################################################################################################
    struct GdbImpl
    {
        std::unique_ptr <Process> process;
        std::vector <ListenerInterface*> listeners;
        DistributingListener distributor;

        GdbImpl()
            : process{}
            , listeners{}
            , distributor{&listeners}
        {

        }
        ~GdbImpl() = default;
    };
//#####################################################################################################################
    Gdb::Gdb(GdbRunArguments args)
        : args_{std::move(args)}
        , impl_{new GdbImpl}
    {

    }
//---------------------------------------------------------------------------------------------------------------------
    Gdb::~Gdb() = default;
//---------------------------------------------------------------------------------------------------------------------
    std::string Gdb::constructCommand()
    {
        std::stringstream command;

        // gdb executable
        command << args_.gdbFullPath <<  ' ';

        // options
        if (args_.sourceDirectory)
            command << "--directory=" << args_.sourceDirectory.value() << ' ';
        if (args_.symbols)
            command << "--symbols=" << args_.symbols.value() << ' ';
        if (args_.fullyReadSymbols)
            command << "--readnow ";
        if (args_.neverReadSymbols)
            command << "--readnever ";
        if (args_.write)
            command << "--write ";

        if (args_.initCommandFile)
            command << "--init-command=" << args_.initCommandFile.value() << ' ';
        if (args_.commandFile)
            command << "--command=" << args_.commandFile.value() << ' ';
        if (args_.ignoreHomeGdbInit)
            command << "--nh ";
        if (args_.ignoreAllGdbInit)
            command << "--nx ";

        if (args_.returnChildResult)
            command << "--return-child-result ";
        if (args_.quiet)
            command << "-q ";
        if (args_.gdbDataDirectory)
            command << "--data-directory=" << args_.gdbDataDirectory.value() << ' ';

        // gdb mi
        command << "--interpreter=mi ";

        // args
        if (args_.args)
            command << "--args ";

        // executable
        command << args_.program << ' ';

        if (args_.args)
            command << args_.args.value();
        else
        {
            // core dump or pid
            if (args_.core)
                command << args_.core.value();
            else if (args_.pid)
                command << args_.pid.value();
        }

        return command.str();
    }
//---------------------------------------------------------------------------------------------------------------------
    void Gdb::start()
    {
        std::string path = args_.directory ? args_.directory.value() : "";

        if (args_.environment)
        {
            impl_->process.reset(new Process{
                constructCommand(),
                args_.environment.value(),
                path,
                [this](char const* c, auto count){stdoutConsumer(std::string{c, count});},
                [this](char const* c, auto count){stderrConsumer(std::string{c, count});},
                true
            });
        }
        else
        {
            impl_->process.reset(new Process{
                constructCommand(),
                path,
                [this](char const* c, auto count){stdoutConsumer(std::string{c, count});},
                [this](char const* c, auto count){stderrConsumer(std::string{c, count});},
                true
            });
        }
    }
//---------------------------------------------------------------------------------------------------------------------
    void Gdb::stdoutConsumer(std::string const& instream)
    {
        auto& sender = impl_->distributor;
        sender.onRawData(instream);

        try
        {
            bool partialParse = false;

            auto message = parse(instream, partialParse);
            if (partialParse)
            {
                sender.onParserError("message was parsed only partially, which is a hard error");
                return;
            }

            if (message.result)
                sender.onResult(message.result.value());

            for (auto const& i : message.oobRecords)
            {
                StreamRecord* srecord = dynamic_cast <StreamRecord*> (i.get());
                if (srecord)
                {
                    switch(srecord->type)
                    {
                        case(StreamRecordType::Console):
                        {
                            sender.onConsoleStream(srecord->data);
                            break;
                        }
                        case(StreamRecordType::Target):
                        {
                            sender.onTargetStream(srecord->data);
                            break;
                        }
                        case(StreamRecordType::Log):
                        {
                            sender.onLogStream(srecord->data);
                            break;
                        }
                    }
                }
                else
                {
                    AsyncRecord* arecord = dynamic_cast <AsyncRecord*> (i.get());
                    if (arecord)
                    {
                        switch(arecord->type)
                        {
                            case(AsyncRecordType::Exec):
                            {
                                sender.onExec(*arecord);
                                break;
                            }
                            case(AsyncRecordType::Notify):
                            {
                                sender.onNotify(*arecord);
                                break;
                            }
                            case(AsyncRecordType::Status):
                            {
                                sender.onStatus(*arecord);
                                break;
                            }
                        }
                    }
                    else
                    {
                        sender.onParserError("unknown record received?");
                    }
                }
            }
        }
        catch (std::exception const& exc)
        {
            sender.onParserError(exc.what());
        }
    }
//---------------------------------------------------------------------------------------------------------------------
    void Gdb::stderrConsumer(std::string const& instream)
    {
        impl_->distributor.onStdErr(instream);
    }
//---------------------------------------------------------------------------------------------------------------------
    void Gdb::registerListener(ListenerInterface* listener)
    {
        impl_->listeners.push_back(listener);
    }
//---------------------------------------------------------------------------------------------------------------------
    void Gdb::sendCommand(MiCommand const& command)
    {
        auto com = synthesize(command);
        impl_->process->write(com);
    }
//---------------------------------------------------------------------------------------------------------------------
    void Gdb::stop()
    {
        sendCommand(MiCommand{"-gdb-exit"});
    }
//---------------------------------------------------------------------------------------------------------------------
    long long Gdb::waitForProcess() const
    {
        return impl_->process->get_exit_status();
    }
//---------------------------------------------------------------------------------------------------------------------
    void Gdb::forceKill()
    {
        impl_->process->kill(true);
    }
//#####################################################################################################################
}
