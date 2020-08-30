#include "input.hpp"

namespace DebuggerInterface::Grammars
{
    using namespace TwistedSpirit;

    qi::rule <std::string::const_iterator> gdbFinalizer = qi::lit("(gdb)") >> linebreak;
}
