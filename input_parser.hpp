#pragma once

#include "input/response.hpp"
#include "input/raw/input.hpp"

namespace DebuggerInterface
{
    namespace detail
    {
        Response postProcess(RawData::Input const& raw);
    }
    Response parse(std::string const& input, bool& partial);
}
