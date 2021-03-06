#pragma once

#include "debugger_interface.hpp"

namespace DebuggerInterface
{
    class DistributingListener : public ListenerInterface
    {
    public:
        DistributingListener(std::vector <ListenerInterface*> const* listeners);
        DistributingListener() = default;

        void onResult(ResultRecord const& result) override;
        void onStdErr(std::string const& text) override;
        void onExec(AsyncRecord const& record) override;
        void onStatus(AsyncRecord const& record) override;
        void onNotify(AsyncRecord const& record) override;
        void onConsoleStream(std::string const& message) override;
        void onTargetStream(std::string const& message) override;
        void onLogStream(std::string const& message) override;
        void onParserError(std::string const& message) override;
        void onRawData(std::string const& raw) override;
        void onPartialRemain(std::string const& remain, std::string const& subject) override;

    private:
        std::vector <ListenerInterface*> const* listeners_;
    };
}
