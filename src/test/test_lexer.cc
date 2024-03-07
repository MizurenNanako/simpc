#include "lexer.hh"
#include "stdafx.hh"
using namespace std::literals;

auto main() -> int
{
    using std::cerr, std::endl, std::format;
    auto ss    = std::ifstream{"../../src/test/lexer.in"};
    auto oo    = std::ofstream{"../../src/test/lexer.out"};
    auto toker = simpc::lexer::tokenizer{ss};

    for (auto [t, i] : toker)
    // Range loop stops at eof or \0
    {
        auto [line, col] = &toker;
        oo << format("[line: {:3}, col: {:3}] -> ", line, col);
        if (i) oo << format("typeid: {0:i}\ttype: {0:n}\tinfo: \"{1}\"", t, *i);
        else oo << format("typeid: {0:i}\tsymbol: \"{0:n}\"", t);
        if (t == simpc::lexer::token_type::newline) oo << endl;
        oo << endl;
    }

    return 0;
}
