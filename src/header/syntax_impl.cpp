#include "syntax_impl.hh"

namespace simpc::syntaxer::impl
{
    _stmt_node::_stmt_node(_stmt_node_type type, const lexer::token_t &token)
        : _type{type}, _token{token} {}
} // namespace simpc::syntaxer::impl
