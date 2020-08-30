#pragma once

#include "command.hpp"
#include "option.hpp"

#include <mplex/fundamental/integral.hpp>
#include <mplex/functional/lift.hpp>
#include <mplex/string/combine.hpp>
#include <mplex/string/to_string.hpp>
#include <mplex/debug.hpp>

#include <type_traits>
#include <sstream>
#include <utility>
#include <optional>
#include <vector>
#include <string>

namespace DebuggerInterface
{
    namespace GdbMiCommands
    {
        MAKE_STRING(example, "example");
    }

    class MiCommand
        : public Command
    {
    private:
        std::optional <long long> token_;
        std::string operation_;
        OptionList options_;
        std::vector <std::string> parameters_;

    public:
        MiCommand() = default;
        MiCommand(std::string operation)
            : operation_{std::move(operation)}
        {
        }
        std::string operation() const
        {
            return operation_;
        }
        void operation(std::string const& operation)
        {
            operation_ = operation;
        }
        MiCommand& token(long long token) noexcept(noexcept(token_ = token))
        {
            token_ = token;
            return *this;
        }
        std::optional <long long> token() const noexcept
        {
            return token_;
        }
        MiCommand& option(std::string const& key, std::optional <std::string> const& value)
        {
            options_[key] = value;
            return *this;
        }
        MiCommand& param(std::string const& par)
        {
            parameters_.push_back(par);
            return *this;
        }
        std::vector <std::string> const& parameters() const
        {
            return parameters_;
        }
        std::optional <std::string> option(std::string const& key)
        {
            auto iter = options_.find(key);
            if (iter == std::end(options_))
                return {};
            return iter->second;
        }
        OptionList const& options() const
        {
            return options_;
        }
    };

    template <typename... Options>
    struct MiCommandOptions
    {

    };

    template <typename... Parameters>
    struct MiParameters
    {

    };

    struct MiNone {
        constexpr static auto value = 0;
    };

    template <
        typename OperationName,
        typename Options = MiNone,
        typename Token = MiNone,
        typename Parameters = MiNone
    >
    struct FixedMiCommand
        : public MiCommand
    {
        using operation = OperationName;
        using parameters = Parameters;
        using options = Options;
        using token = Token;
    };

    template <typename FixedMi>
    void synthesize()
    {
        using cmd = FixedMi;

        using token_str = mplex::lazy_if_vt <
            std::is_same <typename cmd::token, MiNone>::value,
            mplex::then_ <mplex::identity_functor, mplex::string<>>,
            mplex::else_ <mplex::lift <mplex::to_string>, typename cmd::token>
        >;

        using catenated = typename mplex::string_algorithm::combine_strings <
            token_str,
            mplex::string <' '>,
            typename cmd::operation
        >::type;

        PRINT_SINGLE(catenated) << "\n";
        std::cout << catenated::c_str << "\n";
    }

    template <typename Command>
    std::string synthesize(Command const& command)
    {
        std::stringstream sstr;
        if (command.token())
            sstr << command.token().value() << '-';
        sstr << command.operation();

        auto const& options = command.options();
        if (!options.empty())
        {
            sstr << ' ';
            for (auto const& opts : options)
            {
                sstr << '-' << opts.first;
                if (opts.second)
                    sstr << ' ' << opts.second.value();
            }
        }

        auto const& parameters = command.parameters();
        if (!parameters.empty())
        {
            sstr << " --";
            for (auto const& param : parameters)
            {
                sstr << ' ' << param;
            }
        }
        sstr << "\r\n";

        return sstr.str();
    }
}
