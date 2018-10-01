#pragma once

#include "gdb_run_arguments.hpp"

#include "dll.hpp"
#include "gdb_interface.hpp"
#include "commands/mi_command.hpp"

namespace GdbInterface
{
    struct GdbImpl;

    class GDBI_DLL_EXPORT Gdb
    {
    public:
        /**
         *  Initialize everything. Does not call the process but memorizes it for the call.
         */
        Gdb(GdbRunArguments args);
        ~Gdb();

        Gdb(Gdb const&) = delete;
        Gdb(Gdb&&) = default;
        Gdb& operator=(Gdb const&) = delete;
        Gdb& operator=(Gdb&&) = default;

        /**
         *  Send a custom command to gdb, that is not implemented directly.
         */
        void sendCommand(MiCommand const& command);

        /**
         *  Start the gdb debugger and start listening for input.
         */
        void start();

        /**
         *  Stop the gdb debugger gently via interface.
         */
        void stop();

        /**
         *  Kill the gdb process forcibly.
         */
        void forceKill();

        /**
         *  waits for gdb to terminate and return exit code.
         */
        long long waitForProcess() const;

        /**
         *  Register a listener that will receive all the output comming from gdb
         *  @param interface The ownership of this class is not aquired and lifetime has to exceed this class' lifetime.
         */
        void registerListener(ListenerInterface* listener);

    private:
        std::string constructCommand();

        void stdoutConsumer(std::string const& instream);
        void stderrConsumer(std::string const& instream);

    private:
        GdbRunArguments args_;
        std::unique_ptr <GdbImpl> impl_;
    };
}
