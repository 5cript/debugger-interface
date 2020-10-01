#include "debugger.hpp"

#include "process/process.hpp"
#include "input_parser.hpp"
#include "input/stream_record.hpp"
#include "input/async_record.hpp"
#include "distributing_listener.hpp"

#include <sstream>
#include <string>
#include <iostream>

using namespace std::string_literals;

namespace DebuggerInterface
{
//#####################################################################################################################
    struct DebuggerImpl
    {
        std::unique_ptr <Process> process;
        std::vector <ListenerInterface*> listeners;
        DistributingListener distributor;

        DebuggerImpl()
            : process{}
            , listeners{}
            , distributor{&listeners}
        {

        }
        ~DebuggerImpl() = default;
    };
//#####################################################################################################################
    Debugger::Debugger(GdbRunArguments args)
        : args_{std::move(args)}
        , impl_{new DebuggerImpl}
    {
    }
//---------------------------------------------------------------------------------------------------------------------
    Debugger::Debugger(LldbRunArguments args)
        : args_{std::move(args)}
        , impl_{new DebuggerImpl}
    {
    }
//---------------------------------------------------------------------------------------------------------------------
    Debugger::Debugger(UserDefinedArguments args)
        : args_{std::move(args)}
        , impl_{new DebuggerImpl}
    {

    }
//---------------------------------------------------------------------------------------------------------------------
    Debugger::~Debugger() = default;
//---------------------------------------------------------------------------------------------------------------------
    std::string Debugger::constructCommand(GdbRunArguments const& args) const
    {
        std::stringstream command;

        // gdb executable
        command << args.debuggerExecuteable <<  ' ';

        // options
        if (args.sourceDirectory)
            command << "--directory=" << args.sourceDirectory.value() << ' ';
        if (args.symbols)
            command << "--symbols=" << args.symbols.value() << ' ';
        if (args.fullyReadSymbols)
            command << "--readnow ";
        if (args.neverReadSymbols)
            command << "--readnever ";
        if (args.write)
            command << "--write ";

        if (args.initCommandFile)
            command << "--init-command=" << args.initCommandFile.value() << ' ';
        if (args.commandFile)
            command << "--command=" << args.commandFile.value() << ' ';
        if (args.ignoreHomeGdbInit)
            command << "--nh ";
        if (args.ignoreAllGdbInit)
            command << "--nx ";

        if (args.returnChildResult)
            command << "--return-child-result ";
        if (args.quiet)
            command << "-q ";
        if (args.gdbDataDirectory)
            command << "--data-directory=" << args.gdbDataDirectory.value() << ' ';

        // gdb mi
        command << "--interpreter=mi ";

        // args
        if (args.args)
            command << "--args ";

        // executable
        if (args.program)
            command << args.program.value() << ' ';

        if (args.args)
            command << args.args.value();
        else
        {
            // core dump or pid
            if (args.core)
                command << args.core.value();
            else if (args.pid)
                command << args.pid.value();
        }

        return command.str();
    }
//---------------------------------------------------------------------------------------------------------------------
    std::string Debugger::constructCommand(LldbRunArguments const& args) const
    {
        std::stringstream command;

        // gdb executable
        command << args.debuggerExecuteable <<  ' ';

        if (args.program)
            command << args.program.value() << ' ';

        return command.str();
    }
//---------------------------------------------------------------------------------------------------------------------
    std::string Debugger::constructCommand(UserDefinedArguments const& args) const
    {
        std::string com = args.debuggerExecuteable;
        if (args.program)
            com += " "s + args.program.value();
        com += " --interpreter=mi ";
        com += args.commandline;
        return com;
    }
//---------------------------------------------------------------------------------------------------------------------
    std::string Debugger::constructCommand() const
    {
        std::string res;
        std::visit([this, &res](auto const& args) {
            res = constructCommand(args);
        }, args_);
        return res;
    }
//---------------------------------------------------------------------------------------------------------------------
    void Debugger::start()
    {
        std::visit([this](auto const& args)
        {
            std::string path = args.directory ? args.directory.value() : "";

            std::cout << constructCommand() << "\n";

            if (args.environment)
            {
                impl_->process.reset(new Process{
                    constructCommand(),
                    path,
                    args.environment.value(),
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
        }, args_);
    }
//---------------------------------------------------------------------------------------------------------------------
    void Debugger::feed(std::string const& str)
    {
        stdoutConsumer(str);
    }
//---------------------------------------------------------------------------------------------------------------------
    void Debugger::stdoutConsumer(std::string const& instream)
    {
        auto& sender = impl_->distributor;
        sender.onRawData(instream);

        try
        {
            bool partialParse = false;
            int amountConsumed = 0;

            auto message = parse(instream, partialParse, amountConsumed);
            if (partialParse)
            {
                sender.onPartialRemain(instream.substr(amountConsumed, instream.size() - amountConsumed), instream);
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
    void Debugger::stderrConsumer(std::string const& instream)
    {
        impl_->distributor.onStdErr(instream);
    }
//---------------------------------------------------------------------------------------------------------------------
    void Debugger::registerListener(ListenerInterface* listener)
    {
        impl_->listeners.push_back(listener);
    }
//---------------------------------------------------------------------------------------------------------------------
    void Debugger::sendCommand(MiCommand const& command)
    {
        auto com = synthesize(command);
        std::cout << "synthesized command: " << com << "\n";
        impl_->process->write(com);
    }
//---------------------------------------------------------------------------------------------------------------------
    void Debugger::sendCommand(std::string const& command)
    {
        impl_->process->write(command);
    }
//---------------------------------------------------------------------------------------------------------------------
    void Debugger::stop()
    {
        sendCommand(MiCommand{"quit"});
    }
//---------------------------------------------------------------------------------------------------------------------
    long long Debugger::waitForProcess() const
    {
        return impl_->process->get_exit_status();
    }
//---------------------------------------------------------------------------------------------------------------------
    void Debugger::forceKill()
    {
        impl_->process->kill(true);
    }
//#####################################################################################################################
}
