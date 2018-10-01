#include "input.hpp"

namespace GdbInterface::Grammars
{
    using namespace TwistedSpirit;

    qi::rule <std::string::const_iterator> gdbFinalizer = qi::lit("(gdb)") >> linebreak;
}
