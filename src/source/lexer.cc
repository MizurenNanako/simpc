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
            inline auto peek =
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
                    return _tokbuf.push_back(c), c;
                };
            inline auto skip =
                [this] { _inputs.get(); };
            inline auto backup =
                [&marker, this] { marker = _inputs.tellg(); };
            inline auto restore =
                [marker, this] {
                    _tokbuf.erase(_tokbuf.length()
                                  - static_cast<size_t>(_inputs.tellg() - marker));
                    _inputs.seekg(marker);
                };
            inline auto backupctx =
                [&ctxmarker, this] { ctxmarker = _inputs.tellg(); };
            inline auto restorectx =
                [ctxmarker, this] { _inputs.seekg(ctxmarker); };
            inline auto restoretag =
                [this](std::streampos &tag) { _inputs.seekg(tag); };
            inline auto lessthan =
                [limit, this](size_t len) { return limit - _inputs.tellg() < len; };
            inline auto stagp =
                [this](std::streampos &tag) { tag = _inputs.tellg(); };
            inline auto stagn =
                [this](std::streampos &tag) { tag = std::streampos(); };
            inline auto shift =
                [this](size_t shift) { _inputs.seekg(shift, _inputs.cur); };
            inline auto shiftstag =
                [this](std::streampos &tag, std::streamoff shift) { tag += shift; };

            inline auto not_enough_input =
                [this] { throw NotEnoughtInputError(_lineno, _cols); };
            inline auto unexpected =
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
