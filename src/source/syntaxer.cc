#include "syntaxer.hh"

namespace simpc::syntactical
{
    parser::parser(lexical::tokenizer &toker) : _toker{toker} {}

    auto parser::parse() -> void
    {
        // todo: before parsing, we have to do preprocessor job
        // todo: thus we need a lexical stream i.e. a token buffer.
        // auto translation_unit = [this] {};
    }
} // namespace simpc::syntaxer
