#pragma once

#include "out_of_band.hpp"

#include <string>

namespace DebuggerInterface
{
    enum class StreamRecordType
    {
        Console,
        Target,
        Log
    };

    struct StreamRecord : public OutOfBandRecord
    {
        StreamRecordType type;
        std::string data;
    };
}
