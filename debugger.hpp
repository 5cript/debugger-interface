#pragma once

#include "gdb_run_arguments.hpp"
#include "lldb_run_arguments.hpp"

#include "dll.hpp"
#include "debugger_interface.hpp"
#include "commands/mi_command.hpp"

#include <variant>

namespace DebuggerInterface
{
    struct DebuggerImpl;

    struct UserDefinedArguments : CommonRunArguments
    {
        std::string commandline;
    };

    class DBG_DLL_EXPORT Debugger
    {
    public:
        /**
         *  Initialize everything. Does not call the process but memorizes it for the call.
         */
        Debugger(GdbRunArguments args);
        Debugger(LldbRunArguments args);
        Debugger(UserDefinedArguments args);
        ~Debugger();

        Debugger(Debugger const&) = delete;
        Debugger(Debugger&&) = default;
        Debugger& operator=(Debugger const&) = delete;
        Debugger& operator=(Debugger&&) = default;

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
        std::string constructCommand(GdbRunArguments const& args) const;
        std::string constructCommand(LldbRunArguments const& args) const;
        std::string constructCommand(UserDefinedArguments const& args) const;
        std::string constructCommand() const;

        void stdoutConsumer(std::string const& instream);
        void stderrConsumer(std::string const& instream);

    private:
        std::variant <
            GdbRunArguments,
            LldbRunArguments,
            UserDefinedArguments
        > args_;
        std::unique_ptr <DebuggerImpl> impl_;
    };
}
