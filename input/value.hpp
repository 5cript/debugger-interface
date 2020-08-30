#pragma once

#include "result_forward.hpp"

#include <memory>
#include <string>

namespace DebuggerInterface
{
    struct Value
    {
        virtual ~Value() = default;
    };

    struct Const : public Value
    {
        Const() = default;
        Const(Const const&) = default;
        Const(Const&&) = default;
        Const& operator=(Const const&) = default;
        Const& operator=(Const&&) = default;
        Const(std::string data)
            : data{std::move(data)}
        {}

        std::string data;
    };

    inline bool isConst(std::unique_ptr <Value> const& value)
    {
        return dynamic_cast <Const*> (value.get()) != nullptr;
    }
}
