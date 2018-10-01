#pragma once

#include "out_of_band.hpp"

#include <string>

namespace GdbInterface
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
