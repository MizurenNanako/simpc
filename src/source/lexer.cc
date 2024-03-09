#include "lexer.hh"

namespace simpc
{
    namespace lexer
    {
        tokenizer::tokenizer(std::istream &input)
            : _lineno{start_lineno},
              _cols{start_cols},
              _inputs{input} {}

        auto tokenizer::get_token() -> token_t
        {
            // Pre-Define
            _marker    = _inputs.tellg();
            _ctxmarker = std::streampos{};

            // remove prefix white space
            while (_inputs.peek() == ' ' || _inputs.peek() == '\t')
                _cols++, _inputs.get();

            // todo: Fix line number and col number for parsing.
            // todo: Fix backslash-return escape.
            auto peek = [this] {
                auto &&c = _inputs.peek();
                if (c == '\\') [[unlikely]]
                    if (auto cc = (_inputs.get(), _inputs.peek());
                        cc == '\n') [[unlikely]]
                    {
                        c = (_inputs.get(), _inputs.peek());
                        ++_lineno;
                        _cols = 0;
                    }
                    else {
                        c = cc;
                        _inputs.unget();
                    }
                return _tokbuf.push_back(c), c;
            };
            auto skip = [this] {
                ++_cols;
                if (_inputs.get() == '\n')
                {
                    ++_lineno;
                    _cols = start_cols;
                }
            };
            auto backup  = [this] { _marker = _inputs.tellg(); };
            auto restore = [this] {
                auto &&rlen = static_cast<size_t>(_inputs.tellg() - _marker);
                _tokbuf.erase(_tokbuf.length() - rlen);
                _cols -= rlen;
                _inputs.seekg(_marker);
            };
            auto backupctx  = [this] { _ctxmarker = _inputs.tellg(); };
            auto restorectx = [this] { _inputs.seekg(_ctxmarker); };
            auto restoretag =
                [this](std::streampos &tag) { _inputs.seekg(tag); };
            auto lessthan = [this](size_t len) { return false; };
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
#include "lexer.ccpart"

        finish:
            return {tok, {}};
        finish_with_info:
            switch (tok)
            {
                case token_type::preprocesser:
                    return {tok, _tokbuf.substr(1, _tokbuf.length() - 2)};
                case token_type::alt_preprocesser:
                    return {tok, _tokbuf.substr(2, _tokbuf.length() - 2)};
                case token_type::literial_char:
                case token_type::literial_string:
                    return {tok, _tokbuf.substr(1, _tokbuf.length() - 2)};
                case token_type::linecomment:
                    return {tok, _tokbuf.substr(2, _tokbuf.length() - 3)};
                case token_type::blockcomment:
                    return {tok, _tokbuf.substr(2, _tokbuf.length() - 2)};
                default:
                    return {tok, _tokbuf.substr(0, _tokbuf.length() - 1)};
            }
        }
    } // namespace lexer
} // namespace simpc
