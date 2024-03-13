#include "syntax_impl.hh"

namespace simpc::syntactical::impl
{
    _stmt_node::_stmt_node(_stmt_node_type type, const lexical::token_t &token)
        : _type{type}, _token{token} {}
} // namespace simpc::syntaxer::impl
