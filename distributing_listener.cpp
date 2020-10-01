#include "distributing_listener.hpp"

#include <iostream>

namespace DebuggerInterface
{
    #define DISTRIBUTE(FNAME, ...) \
        [&, this](){ \
        if (listeners_ == nullptr) \
        { \
            return; \
        } \
        for (auto const& i : *listeners_) \
        { \
            if (i != nullptr) \
                i-> FNAME (__VA_ARGS__); \
        }}()
//#####################################################################################################################
    DistributingListener::DistributingListener(std::vector <ListenerInterface*> const* listeners)
        : listeners_{listeners}
    {
    }
//---------------------------------------------------------------------------------------------------------------------s
    void DistributingListener::onStdErr(std::string const& text)
    {
        DISTRIBUTE(onStdErr, text);
    }
//---------------------------------------------------------------------------------------------------------------------
    void DistributingListener::onExec(AsyncRecord const& record)
    {
        DISTRIBUTE(onExec, record);
    }
//---------------------------------------------------------------------------------------------------------------------
    void DistributingListener::onStatus(AsyncRecord const& record)
    {
        DISTRIBUTE(onStatus, record);
    }
//---------------------------------------------------------------------------------------------------------------------
    void DistributingListener::onNotify(AsyncRecord const& record)
    {
        DISTRIBUTE(onNotify, record);
    }
//---------------------------------------------------------------------------------------------------------------------
    void DistributingListener::onConsoleStream(std::string const& message)
    {
        DISTRIBUTE(onConsoleStream, message);
    }
//---------------------------------------------------------------------------------------------------------------------
    void DistributingListener::onTargetStream(std::string const& message)
    {
        DISTRIBUTE(onTargetStream, message);
    }
//---------------------------------------------------------------------------------------------------------------------
    void DistributingListener::onLogStream(std::string const& message)
    {
        DISTRIBUTE(onLogStream, message);
    }
//---------------------------------------------------------------------------------------------------------------------
    void DistributingListener::onParserError(std::string const& message)
    {
        DISTRIBUTE(onParserError, message);
    }
//---------------------------------------------------------------------------------------------------------------------
    void DistributingListener::onRawData(std::string const& raw)
    {
        DISTRIBUTE(onRawData, raw);
    }
//---------------------------------------------------------------------------------------------------------------------
    void DistributingListener::onResult(ResultRecord const& result)
    {
        DISTRIBUTE(onResult, result);
    }
//---------------------------------------------------------------------------------------------------------------------
    void DistributingListener::onPartialRemain(std::string const& remaining, std::string const& subject)
    {
        DISTRIBUTE(onPartialRemain, remaining, subject);
    }
//#####################################################################################################################
}
