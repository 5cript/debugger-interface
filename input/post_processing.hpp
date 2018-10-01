#pragma once

#include "raw/input.hpp"

// Finals
#include "stream_record.hpp"
#include "async_record.hpp"
#include "value.hpp"
#include "tuple.hpp"
#include "list.hpp"
#include "response.hpp"

namespace GdbInterface::detail
{
    GdbInterface::StreamRecord convertStreamRecord(RawData::StreamRecord const& record);
    std::unique_ptr <GdbInterface::Value> makeValue(RawData::Result const& raw);
    GdbInterface::AsyncRecord convertAsyncRecord(RawData::OutOfBand const& record);
    Response postProcess(RawData::Input const& raw);
}
