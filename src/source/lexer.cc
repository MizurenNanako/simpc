#include "lexer.hh"

namespace simpc
{
    namespace lexer
    {
        tokenizer::tokenizer(std::istream &input)
            : _inputs{input} {}

        auto tokenizer::get_token() -> token_t
        try
        {
            // Pre-Define
            auto marker    = _inputs.tellg();
            auto ctxmarker = std::streampos{};
            _inputs.seekg(_inputs.end);
            const auto limit = _inputs.tellg();
            _inputs.seekg(marker);

            // todo: Update line number and col number while parsing.
            // todo: Handle white spaces.
            // todo: Handle backslash-return escape. [Done]
            auto peek =
                [this] {
                    auto &&c = _inputs.peek();
                    if (c == '\\') [[unlikely]]
                        if (auto cc = _inputs.get();
                            cc == '\n') [[unlikely]]
                            c = _inputs.peek();
                        else {
                            c = cc;
                            _inputs.unget();
                        }
                    return c;
                };
            auto skip =
                [this] { _inputs.get(); };
            auto backup =
                [&marker, this] { marker = _inputs.tellg(); };
            auto restore =
                [marker, this] { _inputs.seekg(marker); };
            auto backupctx =
                [&ctxmarker, this] { ctxmarker = _inputs.tellg(); };
            auto restorectx =
                [ctxmarker, this] { _inputs.seekg(ctxmarker); };
            auto restoretag =
                [this] {};
            auto lessthan =
                [limit, this](size_t len) { return limit - _inputs.tellg() < len; };
            auto stagp =
                [this](std::streampos &tag) { tag = _inputs.tellg(); };
            auto stagn =
                [this](std::streampos &tag) { tag = std::streampos(); };
            auto shift =
                [this](size_t shift) { _inputs.seekg(shift, _inputs.cur); };
            auto shiftstag =
                [this](std::streampos &tag, std::streamoff shift) { tag += shift; };

            auto not_enough_input =
                [this] { throw NotEnoughtInputError(_lineno, _cols); };
            auto unexpected =
                [this] { throw LexicalError(_lineno, _cols, _tokbuf); };
        start:
            _tokbuf.clear();
            // retrive a char from stream
            auto tok = token_type();

            // match keywords or identifier
            // todo: DFA correctness verification.
#include "lexer.ccpart"

            // todo: The DFA return is empty until correctness check passes...
            // This function currently has no functionality.
        finish:
            return {tok, {}};
        finish_with_info:
            return {tok, _tokbuf};
        } catch (std::exception &e)
        {
            std::cerr << std::format("Unknown Exception from {}, what(): {}",
                                     __func__, e.what())
                      << std::endl;
            return {token_type::err, {}};
        }
    } // namespace lexer
} // namespace simpc
