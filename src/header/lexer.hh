#ifndef __LEXER
#define __LEXER

#include "lexer_token_type.hh"
#include "stdafx.hh"

namespace simpc
{
    namespace lexer
    {
        class tokenizer;
        class token_iterator;
        class token_iterator_sentinel {};

        using char_type    = char;
        using token_info_t = std::basic_string<char_type>;

        /// @brief Type of a lexical token.
        /// @brief Pair of token_type and optional info.
        using token_t = std::pair<token_type,
                                  std::optional<token_info_t>>;
        using _stream = std::basic_istream<char_type>;
        using _buffer = std::basic_string<char_type>;

        class NotEnoughtInputError : public std::runtime_error {
          public:
            NotEnoughtInputError(const size_t lineno, const size_t col)
                : runtime_error(
                    std::format("Not enough input on line: {}, col: {}",
                                lineno, col)) {}
            virtual ~NotEnoughtInputError() noexcept = default;
        };

        class LexicalError : public std::runtime_error {
          public:
            LexicalError(const size_t lineno, const size_t col,
                         const std::string_view errlex)
                : runtime_error(std::format(
                    "Lexical Error at line: {}, col: {}, error: \"{}\"",
                    lineno, col, errlex)) {}
            virtual ~LexicalError() noexcept = default;
        };

        class token_iterator {
          public:
            using value_type        = token_t;
            using difference_type   = void;
            using pointer           = token_t *;
            using reference         = token_t &;
            using iterator_category = std::forward_iterator_tag;

          private:
            tokenizer &_toker;
            token_t    _token{};

          public:
            token_iterator(tokenizer &t);
            ~token_iterator() = default;
            inline auto operator++() -> void;
            inline auto operator!=(token_iterator_sentinel) -> bool { return _token.first != token_type::eof; }
            inline auto operator*() -> token_t { return _token; }
        };

        class tokenizer {
          private:
            size_t _lineno; // Line number
            size_t _cols; // Column number

            _stream &_inputs;
            _buffer  _tokbuf; // token buffer

            std::streampos _marker;
            std::streampos _ctxmarker;

          public:
            tokenizer(std::istream &input = std::cin);
            tokenizer(tokenizer &) = delete;
            ~tokenizer()           = default;

            /// @brief Get token from bindded input stream.
            /// @return The token_type and info if any.
            auto        get_token() -> token_t;
            inline auto get_pos() -> std::pair<size_t, size_t> { return {_lineno, _cols}; }

            /// @brief Same as get_token but more elegent.
            /// @return The token_type and info if any.
            inline auto operator()() { return get_token(); };

            inline auto operator&() { return get_pos(); }

            inline auto begin() -> token_iterator { return token_iterator{*this}; }
            inline auto end() -> token_iterator_sentinel { return {}; };
        };

        inline token_iterator::token_iterator(tokenizer &t) : _toker{t}, _token{t.get_token()} {}
        inline auto token_iterator::operator++() -> void { _token = _toker.get_token(); }
    } // namespace lexer
} // namespace simpc

#endif // __LEXER
