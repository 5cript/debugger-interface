#pragma once

#include "token.hpp"

#include <optional>

namespace DebuggerInterface
{
    struct Command
    {
        std::optional <Token> token;
    };
}
