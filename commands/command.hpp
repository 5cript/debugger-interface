#pragma once

#include "token.hpp"

#include <optional>

namespace GdbInterface
{
    struct Command
    {
        std::optional <Token> token;
    };
}
