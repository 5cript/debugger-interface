#pragma once

#include <string>
#include <optional>
#include <unordered_map>

namespace DebuggerInterface
{
    struct CommonRunArguments
    {
        /// What gdb to use.
        std::string debuggerExecuteable;

        /// The program to debug
        std::optional <std::string> program = std::nullopt;

        /// The execution directory we are in.
        std::optional <std::string> directory = std::nullopt;

        /// Specify environment variables with this.
        std::optional <std::unordered_map <std::string, std::string>> environment = std::nullopt;
    };
}
