#ifndef __SYNTAXER
#define __SYNTAXER

// precompiled header
#include "stdafx.hh"

// headers
#include "lexer.hh"
#include "syntax_impl.hh"

namespace simpc
{
    namespace syntaxer
    {
        class NoChildrenError : std::logic_error {
          public:
            NoChildrenError(const lexer::token_t &_token)
                : logic_error{std::format(
                    "This node({:n}) has no children.",
                    _token.first)} {}
        };
        class SyntaxError : std::runtime_error {
          public:
            using runtime_error::runtime_error;
        };

        class parser {
          private:
            lexer::tokenizer &_toker;
            ast_tree          _ast;

          public:
            parser(lexer::tokenizer &toker);
            parser(parser &) = delete;
            ~parser()        = default;

            auto parse() -> void;
        };
    } // namespace syntaxer
} // namespace simpc

#endif // __SYNTAXER
