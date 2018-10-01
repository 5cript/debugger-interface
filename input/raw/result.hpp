#pragma once

#include "../../adapt.hpp"

namespace GdbInterface::RawData
{
    struct Result
    {
        std::string variable;

        // from the following, only one can be filled. This will later be post processed.
        std::string value;
        std::vector <Result> tupleValue;
        std::vector <Result> listValue;

        bool valueSet;
        bool tupleSet;
        bool listSet;
    };
}

BOOST_FUSION_ADAPT_STRUCT
(
    GdbInterface::RawData::Result,
    variable, value, tupleValue, listValue, valueSet, tupleSet, listSet
)
