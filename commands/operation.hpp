#pragma once

#include <mplex/string/string.hpp>

namespace DebuggerInterface
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
