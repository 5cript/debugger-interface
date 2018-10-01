#pragma once

#include "value.hpp"
#include "result.hpp"

#include <vector>

namespace GdbInterface
{
    struct Tuple : public Value
    {
        std::vector <Result> values;
    };

    inline bool isTuple(std::unique_ptr <Value> const& value)
    {
        return dynamic_cast <Tuple*> (value.get()) != nullptr;
    }
}
