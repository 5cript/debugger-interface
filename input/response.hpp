#pragma once

#include "out_of_band.hpp"
#include "result_record.hpp"

#include <vector>
#include <optional>

namespace DebuggerInterface
{
    struct Response
    {
        std::vector <std::unique_ptr <OutOfBandRecord>> oobRecords;
        std::optional <ResultRecord> result;
        bool complete;
    };
}
