#pragma once

#include "../raw/input.hpp"

#include "out_of_band.hpp"
#include "result.hpp"

#include "twisted-spirit/core/parser_core.hpp"
#include "twisted-spirit/rules/space.hpp"

namespace DebuggerInterface::Grammars
{
    using namespace TwistedSpirit;

    extern qi::rule <std::string::const_iterator> gdbFinalizer;

    template GRAMMAR_TEMPLATE_SIGNATURE
    struct InputGrammar : qi::grammar <Iterator, RawData::Input()>
    {
        using grammar_result = RawData::Input;

        InputGrammar() : InputGrammar::base_type(main, "input")
        {
            using namespace common_usings;

            record =
                    qi::char_('^')
                >>  (
                            qi::lit("done")             [at_c <0> (qi::_val) = "done"]
                        |   qi::lit("running")          [at_c <0> (qi::_val) = "running"]
                        |   qi::lit("connected")        [at_c <0> (qi::_val) = "connected"]
                        |   qi::lit("error")            [at_c <0> (qi::_val) = "error"]
                        |   qi::lit("exit")             [at_c <0> (qi::_val) = "exit"]
                    )
                >> *(qi::char_(',') >> result [phoenix::push_back(at_c <1> (qi::_val), qi::_1)])
                >> linebreak
            ;

            main =
                   *outOfBand           [phoenix::push_back(at_c <0> (qi::_val), qi::_1)]
                >> -record              [at_c <1> (qi::_val) = qi::_1]
                >> -(
                           qi::lit("(gdb)")
                        >> *space
                        >> linebreak
                    )                   [at_c <2> (qi::_val) = true]
            ;
        }

        // Rules
        qi::rule <Iterator, grammar_result()> main;
        qi::rule <Iterator, RawData::ResultRecord()> record;

        // Grammars
        OutOfBandGrammar GRAMMAR_TEMPLATE_SIGNATURE_FORWARD outOfBand;
        ResultGrammar GRAMMAR_TEMPLATE_SIGNATURE_FORWARD result;
    };
}
