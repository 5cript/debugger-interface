#pragma once

#include "out_of_band.hpp"
#include "result.hpp"

#include <string>
#include <optional>
#include <vector>

// NO FUNCTIONS HERE, UNLESS EXPORTED

namespace GdbInterface
{
    enum class AsyncRecordType
    {
        Exec = 0,
        Status,
        Notify
    };

    struct AsyncRecord : public OutOfBandRecord
    {
        std::optional <long long> token;
        AsyncRecordType type;
        std::string status;
        std::vector <Result> results;
    };
}
