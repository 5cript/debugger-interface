#pragma once

#include "../raw/out_of_band.hpp"

#include "twisted-spirit/core/parser_core.hpp"
#include "twisted-spirit/grammars/unescaped_string.hpp"

namespace DebuggerInterface::Grammars
{
    using namespace TwistedSpirit;

    template GRAMMAR_TEMPLATE_SIGNATURE
    struct ResultGrammar : qi::grammar <Iterator, RawData::Result()>
    {
        using grammar_result = RawData::Result;

        ResultGrammar() : ResultGrammar::base_type(main, "result")
        {
            using namespace common_usings;
            INSTALL_ERROR_HANDLER;
            INSTALL_WARNING_HANDLER;
            INSTALL_DEBUG_HANDLER;

            tuple =
                    qi::char_('{')
                >> (result [phoenix::push_back(_val, qi::_1)] % ',')
                >>  qi::char_('}')
            ;

            list =
                (
                        qi::lit("[]")
                    |   (
                                qi::char_('[')
                            >>  value [phoenix::push_back(qi::_val, qi::_1)]
                            >> *(',' >> value [phoenix::push_back(qi::_val, qi::_1)])
                            >>  qi::char_(']')
                        )
                    |   (
                                qi::char_('[')
                            >>  result [phoenix::push_back(qi::_val, qi::_1)]
                            >> *(',' >> result [phoenix::push_back(qi::_val, qi::_1)])
                            >>  qi::char_(']')
                        )

                )
            ;

            value =
                    (
                            c_string                [at_c <1> (qi::_val) = qi::_1, at_c<4> (qi::_val) = true]
                        |   tuple                   [at_c <2> (qi::_val) = qi::_1, at_c<5> (qi::_val) = true]
                        |   list                    [at_c <3> (qi::_val) = qi::_1, at_c<6> (qi::_val) = true]
                    )
            ;

            result =
                   +(qi::char_ - qi::char_('='))    [phoenix::push_back(at_c <0> (qi::_val), qi::_1)]
                >>  qi::char_('=')
                >>  (
                            c_string                [at_c <1> (qi::_val) = qi::_1, at_c<4> (qi::_val) = true]
                        |   tuple                   [at_c <2> (qi::_val) = qi::_1, at_c<5> (qi::_val) = true]
                        |   list                    [at_c <3> (qi::_val) = qi::_1, at_c<6> (qi::_val) = true]
                    )
            ;

            main = result;

            HANDLE_QI_ERROR(main, 1);
            HANDLE_QI_WARNING(main, 1);
        }

        // Rules
        qi::rule <Iterator, grammar_result()> main;

        qi::rule <Iterator, RawData::Result()> result;
        qi::rule <Iterator, RawData::Result()> value;

        qi::rule <Iterator, std::vector <RawData::Result>()> tuple;
        qi::rule <Iterator, std::vector <RawData::Result>()> list;

        // Grammars
        TwistedSpirit::unescaped_string_grammar GRAMMAR_TEMPLATE_SIGNATURE_FORWARD c_string;
    };
}
