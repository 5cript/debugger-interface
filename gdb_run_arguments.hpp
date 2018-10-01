#pragma once

#include <memory>
#include <optional>
#include <unordered_map>

namespace GdbInterface
{
    struct GdbRunArguments
    {
        /// What gdb to use.
        std::string gdbFullPath;

        /// The program to debug
        std::string program;

        /// The execution directory we are in.
        std::optional <std::string> directory = std::nullopt;

        /// A PID if we want to attach to a running process.
        std::optional <int> pid = std::nullopt;

        /// Arguments to pass to the inferior process (what we want to debug). Incompatible with pid, which is preferred.
        std::optional <std::string> args = std::nullopt;

        /// Pass a core dump.
        std::optional <std::string> core = std::nullopt;

        /// Source directory. (a root to lookup source files from).
        std::optional <std::string> sourceDirectory = std::nullopt;

        /// Pass a symbols file.
        std::optional <std::string> symbols = std::nullopt;

        /// gdb --readnow
        bool fullyReadSymbols = false;

        /// gdb --readnever
        bool neverReadSymbols = false;

        /// gdb --write
        bool write = false;

        /// do not print version number on startup.
        bool quiet = false;

        /// bool return-child-result. (dont temper with)
        bool returnChildResult = true;

        /// gdb --data-directory -D
        std::optional <std::string> gdbDataDirectory = std::nullopt;

        /// Specify environment variables with this.
        std::optional <std::unordered_map <std::string, std::string>> environment = std::nullopt;

        /// gdb --init-command
        std::optional <std::string> initCommandFile = std::nullopt;

        /// gdb --command
        std::optional <std::string> commandFile = std::nullopt;

        /// Do not read ~/.gdbinit
        bool ignoreHomeGdbInit = false;

        /// Do not read any .gdbinit.
        bool ignoreAllGdbInit = false;
    };
}

#ifdef BOOST_FUSION_ADAPT_STRUCT
BOOST_FUSION_ADAPT_STRUCT
(
    GdbInterface::GdbRunArguments,
    gdbFullPath, program, directory, pid, args, core, sourceDirectory, symbols,
    fullyReadSymbols, neverReadSymbols, write, quiet, returnChildResult, gdbDataDirectory, environment,
    initCommandFile, commandFile, ignoreHomeGdbInit, ignoreAllGdbInit
)
#endif // BOOST_FUSION_ADAPT_STRUCT
