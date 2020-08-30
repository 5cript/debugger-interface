#pragma once

#include "../raw/out_of_band.hpp"
#include "result.hpp"

#include "twisted-spirit/core/parser_core.hpp"
#include "twisted-spirit/grammars/unescaped_string.hpp"
#include "twisted-spirit/rules/space.hpp"

namespace DebuggerInterface::Grammars
{
    using namespace TwistedSpirit;

    template GRAMMAR_TEMPLATE_SIGNATURE
    struct OutOfBandGrammar : qi::grammar <Iterator, RawData::OutOfBand()>
    {
        using grammar_result = RawData::OutOfBand;

        OutOfBandGrammar() : OutOfBandGrammar::base_type(main, "out-of-band-record")
        {
            using namespace common_usings;
            INSTALL_ERROR_HANDLER;
            INSTALL_WARNING_HANDLER;
            INSTALL_DEBUG_HANDLER;

            async_record =
                // token
                   -qi::long_long                   [at_c <0> (qi::_val) = qi::_1]

                // type (exec*, status+, notify=)
                >>  qi::char_("*+=")                [at_c <1> (qi::_val) = qi::_1]

                // class
                >>  async_class                     [at_c <2> (qi::_val) = qi::_1]

                // results
                >> *(qi::char_(',') >> result       [phoenix::push_back(at_c <3> (qi::_val), qi::_1)])
            ;

            stream_record =
                    qi::char_("~@&")                [at_c <0> (qi::_val) = qi::_1]
                //>  +(qi::char_ [phoenix::push_back(at_c <1> (qi::_val), qi::_1)] - qi::char_('\r'))
                >   c_string                        [at_c <1> (qi::_val) = qi::_1]
                >>  eps                             [at_c <2> (qi::_val) = true]
            ;

            async_class =
                +(qi::char_ [phoenix::push_back(qi::_val, qi::_1)] - qi::char_(",\r\n"))
            ;

            out_of_band_record =
                (       async_record                    [qi::_val = qi::_1]
                    |   stream_record                   [at_c <4> (qi::_val) = qi::_1]
                )
                >>  linebreak
            ;

            main = out_of_band_record;

            HANDLE_QI_ERROR(main, 1);
            HANDLE_QI_WARNING(main, 1);
        }

        // Rules
        qi::rule <Iterator, grammar_result()> main;

        qi::rule <Iterator, std::string()> async_class;

        qi::rule <Iterator, grammar_result()> async_record;
        qi::rule <Iterator, RawData::StreamRecord()> stream_record;
        qi::rule <Iterator, grammar_result()> out_of_band_record;

        // Grammars
        ResultGrammar GRAMMAR_TEMPLATE_SIGNATURE_FORWARD result;
        TwistedSpirit::unescaped_string_grammar GRAMMAR_TEMPLATE_SIGNATURE_FORWARD c_string;
    };
}
