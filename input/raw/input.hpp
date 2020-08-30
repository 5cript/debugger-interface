#pragma once

#include "result.hpp"
#include "out_of_band.hpp"

#include <vector>
#include <boost/optional.hpp>

namespace DebuggerInterface::RawData
{
    struct ResultRecord
    {
        std::string status;
        std::vector <Result> results;
    };

    struct Input
    {
        std::vector <OutOfBand> outOfBand;
        boost::optional <ResultRecord> resultRecord;
        bool complete = false;
    };
}

BOOST_FUSION_ADAPT_STRUCT
(
    DebuggerInterface::RawData::ResultRecord,
    status, results
)

BOOST_FUSION_ADAPT_STRUCT
(
    DebuggerInterface::RawData::Input,
    outOfBand, resultRecord, complete
)
