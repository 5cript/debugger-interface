#include "post_processing.hpp"

#include <iostream>

namespace DebuggerInterface::detail
{
//#####################################################################################################################
    DebuggerInterface::StreamRecord convertStreamRecord(RawData::StreamRecord const& record)
    {
        DebuggerInterface::StreamRecord result;
        switch (record.type)
        {
            case('~'): {result.type = StreamRecordType::Console; break;}
            case('@'): {result.type = StreamRecordType::Target; break;}
            case('&'): {result.type = StreamRecordType::Log; break;}
        }
        result.data = record.value;
        return result;
    }
//---------------------------------------------------------------------------------------------------------------------
    std::unique_ptr <DebuggerInterface::Value> makeValue(RawData::Result const& raw)
    {
        std::unique_ptr <Value> value;

        if (raw.valueSet)
            value.reset(new Const(raw.value));
        else if (raw.tupleSet)
        {
            value.reset(new Tuple);
            Tuple* tuple = static_cast <Tuple*> (value.get());
            for (auto const& i : raw.tupleValue)
            {
                auto res = makeValue(i);
                if (!isResult(res))
                    throw std::runtime_error("invalid value type in tuple");
                tuple->values.push_back(std::move(*static_cast <Result*> (res.get())));
            }
        }
        else if (raw.listSet)
        {
            value.reset(new List);
            List* list = static_cast <List*> (value.get());
            for (auto const& i : raw.listValue)
                list->values.push_back(makeValue(i));
        }

        if (!raw.variable.empty())
        {
            return std::make_unique <Result> (
                raw.variable,
                std::move(value)
            );
        }
        else
        {
            return value;
        }
    }
//---------------------------------------------------------------------------------------------------------------------
    DebuggerInterface::AsyncRecord convertAsyncRecord(RawData::OutOfBand const& record)
    {
        DebuggerInterface::AsyncRecord result;

        if (record.token)
            result.token = record.token.get();
        switch (record.type)
        {
            case('*'): {result.type = AsyncRecordType::Exec; break;}
            case('+'): {result.type = AsyncRecordType::Status; break;}
            case('='): {result.type = AsyncRecordType::Notify; break;}
        }
        result.status = record.asyncClass;

        for (auto const& raw : record.results)
        {
            result.results.emplace_back(
                raw.variable,
                makeValue(raw)
            );
        }

        return result;
    }
//---------------------------------------------------------------------------------------------------------------------
    Response postProcess(RawData::Input const& raw)
    {
        Response response;
        for (auto oobRecord : raw.outOfBand)
        {
            // Stream OOB Record
            if (oobRecord.streamRecord.wasSet)
                response.oobRecords.push_back(std::move(std::make_unique <StreamRecord> (convertStreamRecord(oobRecord.streamRecord))));
            else
                response.oobRecords.push_back(std::move(std::make_unique <AsyncRecord> (convertAsyncRecord(oobRecord))));
        }

        if (raw.resultRecord)
        {
            auto const& rawRecord = raw.resultRecord.get();

            ResultRecord record;
            if (rawRecord.status == "done")
                record.status = ResultStatus::Done;
            else if (rawRecord.status == "running")
                record.status = ResultStatus::Running;
            else if (rawRecord.status == "connected")
                record.status = ResultStatus::Connected;
            else if (rawRecord.status == "error")
                record.status = ResultStatus::Error;
            else if (rawRecord.status == "exit")
                record.status = ResultStatus::Exit;

            for (auto const& i : rawRecord.results)
            {
                auto res = makeValue(i);
                if (!isResult(res))
                    throw std::runtime_error("invalid value type in tuple");
                record.results.push_back(std::move(*static_cast <Result*> (res.get())));
            }
        }

        response.complete = raw.complete;

        return response;
    }
//#####################################################################################################################
}
