#pragma once

#include "input/async_record.hpp"
#include "input/result_record.hpp"
#include "dll.hpp"

#include <string>

namespace DebuggerInterface
{
    struct ListenerInterface
    {
        virtual void onStdErr(std::string const& text) = 0;

        virtual void onResult(ResultRecord const& result) = 0;

        virtual void onExec(AsyncRecord const& record) = 0;
        virtual void onStatus(AsyncRecord const& record) = 0;
        virtual void onNotify(AsyncRecord const& record) = 0;

        // Stream Records
        virtual void onConsoleStream(std::string const& message) = 0;
        virtual void onTargetStream(std::string const& message) = 0;
        virtual void onLogStream(std::string const& message) = 0;

        // This is called when this library fails at some point
        virtual void onParserError(std::string const& message) = 0;

        virtual void onRawData(std::string const& raw) = 0;
        virtual void onPartialRemain(std::string const& remaninig, std::string const& subject) = 0;

        virtual ~ListenerInterface() = default;
    };

    struct PickyListener : ListenerInterface
    {
        void onStdErr(std::string const& text) override {}

        void onResult(ResultRecord const& result) override {}

        void onExec(AsyncRecord const& record) override {}
        void onStatus(AsyncRecord const& record) override {}
        void onNotify(AsyncRecord const& record) override {}

        // Stream Records
        void onConsoleStream(std::string const& message) override {}
        void onTargetStream(std::string const& message) override {}
        void onLogStream(std::string const& message) override {}

        // This is called when this library fails at some point
        void onParserError(std::string const& message) override {}

        void onRawData(std::string const& raw) override {}
        void onPartialRemain(std::string const& remaninig, std::string const& subject) {};
    };


}
