#pragma once

#include "value.hpp"

#include <string>

namespace GdbInterface
{
    struct Result : public Value
    {
        Result() = default;
        Result(Result const&) = delete;
        Result(Result&&) = default;
        Result(std::string variable, std::unique_ptr <Value>&& value)
            : variable{std::move(variable)}
            , value(std::move(value))
        {}

        std::string variable;
        std::unique_ptr <Value> value;
    };

    inline bool isResult(std::unique_ptr <Value> const& value)
    {
        return dynamic_cast <Result*> (value.get()) != nullptr;
    }
}
