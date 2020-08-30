#pragma once

#include "raw/input.hpp"

// Finals
#include "stream_record.hpp"
#include "async_record.hpp"
#include "value.hpp"
#include "tuple.hpp"
#include "list.hpp"
#include "response.hpp"

namespace DebuggerInterface::detail
{
    DebuggerInterface::StreamRecord convertStreamRecord(RawData::StreamRecord const& record);
    std::unique_ptr <DebuggerInterface::Value> makeValue(RawData::Result const& raw);
    DebuggerInterface::AsyncRecord convertAsyncRecord(RawData::OutOfBand const& record);
    Response postProcess(RawData::Input const& raw);
}
