#include "input_parser.hpp"

#include "twisted-spirit/core/parse.hpp"
#include "input/parser/input.hpp"
#include "input/post_processing.hpp"

#include <stdexcept>
#include <functional>

namespace GdbInterface
{
//#####################################################################################################################
    template <typename T>
    void print(T&& value)
    {
        std::function <void(RawData::Result const&)> printResult;
        printResult = [&](RawData::Result const& i)
        {
            std::cout << "," << i.variable;
            if (i.valueSet)
                std::cout << "=\"" << i.value << "\"";
            if (i.tupleSet)
            {
                std::cout << "={";
                for (auto const& res : i.tupleValue)
                    printResult(res);
                std::cout << "}";
            }
            if (i.listSet)
            {
                std::cout << "=[";
                for (auto const& res : i.listValue)
                    printResult(res);
                std::cout << "]";
            }
        };

        for (auto const& oband : value.outOfBand)
        {
            std::cout << oband.type << oband.asyncClass;
            for (auto const& i : oband.results)
            {
                printResult(i);
            }
        }

        std::cout << value.outOfBand.size() << "\n";
        std::cout << value.resultRecord.status << "\n";

        for (auto const& i : value.resultRecord.results)
        {
            printResult(i);
        }
    }
//#####################################################################################################################
    Response parse(std::string const& input, bool& partial)
    {
        TYPEDEF_GRAMMAR(Grammars::InputGrammar);

        auto [error, value] = TwistedSpirit::parse <grammar> (input);
        if (error == TwistedSpirit::ParsingResult::FAIL)
        {
            throw std::runtime_error("parsing failed");

        }
        else
        {
            if (error == TwistedSpirit::ParsingResult::PARTIAL)
                partial = true;
            else
                partial = false;
            return detail::postProcess(value);
        }
    }
//#####################################################################################################################
}
