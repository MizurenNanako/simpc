#include "lexer.hh"

namespace simpc
{
    namespace lexer
    {
        tokenizer::tokenizer(std::istream &input) : _inputs{input} {}

        auto tokenizer::get_token() -> token_t
        try
        {
            // Pre-Define
            _marker    = _inputs.tellg();
            _ctxmarker = std::streampos{};
            _inputs.seekg(_marker);
            _inputs >> std::ws;

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
                    return _tokbuf.push_back(c), c;
                };
            auto skip =
                [this] { _inputs.get(); };
            auto backup =
                [this] { _marker = _inputs.tellg(); };
            auto restore =
                [this] {
                    _tokbuf.erase(_tokbuf.length()
                                  - static_cast<size_t>(_inputs.tellg() - _marker));
                    _inputs.seekg(_marker);
                };
            auto backupctx =
                [this] { _ctxmarker = _inputs.tellg(); };
            auto restorectx =
                [this] { _inputs.seekg(_ctxmarker); };
            auto restoretag =
                [this](std::streampos &tag) { _inputs.seekg(tag); };
            auto lessthan =
                [this](size_t len) { return false; };
            auto stagp =
                [this](std::streampos &tag) { tag = _inputs.tellg(); };
            auto stagn =
                [this](std::streampos &tag) { tag = std::streampos(); };
            auto shift =
                [this](size_t shift) { _inputs.seekg(shift, _inputs.cur); };
            auto shiftstag =
                [this](std::streampos &tag, std::streamoff shift) { tag += shift; };

            auto not_enough_input =
                [this] {
                    // throw NotEnoughtInputError(_lineno, _cols);
                    return false;
                };
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
            _tokbuf.pop_back();
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
