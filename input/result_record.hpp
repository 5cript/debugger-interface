#pragma once

#include "result.hpp"

#include <vector>

namespace DebuggerInterface
{
    enum class ResultStatus
    {
        Invalid,
        Done,
        Running,
        Connected,
        Error,
        Exit
    };

    struct ResultRecord
    {
        ResultRecord()
            : status{ResultStatus::Invalid}
            , results{}
        {
        }

        ResultRecord(ResultRecord const&) = delete;
        ResultRecord& operator=(ResultRecord const&) = delete;
        ResultRecord(ResultRecord&&) = default;
        ResultRecord& operator=(ResultRecord&&) = default;

        ResultStatus status;
        std::vector <Result> results;
    };
}
