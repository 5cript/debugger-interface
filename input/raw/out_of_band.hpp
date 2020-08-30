#pragma once

#include "result.hpp"
#include "../../adapt.hpp"

#include <boost/optional.hpp>

namespace DebuggerInterface::RawData
{
    struct StreamRecord
    {
        char type;
        std::string value;
        bool wasSet;
    };

    struct OutOfBand
    {
        boost::optional <long long> token;
        char type;
        std::string asyncClass;
        std::vector <Result> results;

        StreamRecord streamRecord;
    };
}

BOOST_FUSION_ADAPT_STRUCT
(
    DebuggerInterface::RawData::StreamRecord,
    type, value, wasSet
)

BOOST_FUSION_ADAPT_STRUCT
(
    DebuggerInterface::RawData::OutOfBand,
    token, type, asyncClass, results, streamRecord
)
