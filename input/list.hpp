#pragma once

#include "value.hpp"

#include <vector>

namespace DebuggerInterface
{
    struct List : public Value
    {
        std::vector <std::unique_ptr <Value>> values;
    };

    inline bool isList(std::unique_ptr <Value> const& value)
    {
        return dynamic_cast <List*> (value.get()) != nullptr;
    }
}
