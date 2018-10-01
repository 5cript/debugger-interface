#pragma once

#include <mplex/string/string.hpp>

namespace GdbInterface
{
    struct Operations
    {
        constexpr char const* test = "test";
    };

    template <typename OperationName>
    struct Operation
        : public OperationName
    {
    };
}
