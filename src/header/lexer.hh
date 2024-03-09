#ifndef __LEXER
#define __LEXER

// precompiled header
#include "stdafx.hh"

// headers
#include "lexer_token_type.hh"

namespace simpc
{
    namespace lexer
    {
        /// @brief Occupies an input stream
         /// and drain inputs from that stream for each get_token call.
        /// No rewind and no buffers designed inside.
        class tokenizer;
        class tokenizer_internal_iterator;

        /// @brief Manage tokenizers, may opcupy multi files.
        /// return preprocessed token for each get_token call.
        /// Like a normal stream with tokens.
        class lexical_analyzer;
        class lexical_iterator;

        constexpr auto start_lineno = 1;
        constexpr auto start_cols   = 1;

        using char_type    = char;
        using token_info_t = std::basic_string<char_type>;

        /// @brief Type of a lexical token.
        /// Pair of token_type and optional info.
        using token_t = std::pair<token_type, std::optional<token_info_t>>;
        using _stream = std::basic_istream<char_type>;
        using _buffer = std::basic_string<char_type>;

        class BadFileError : public std::runtime_error {
          public:
            BadFileError(const std::string &filename)
                : runtime_error(
                    std::format("Failed opening file: {}",
                                filename)) {}
            virtual ~BadFileError() noexcept = default;
        };

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

        class tokenizer_internal_iterator {
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
            tokenizer_internal_iterator(tokenizer &t);
            ~tokenizer_internal_iterator() = default;
            inline auto operator++() -> tokenizer_internal_iterator &;
            inline auto operator!=(common_iterator_sentinel) -> bool { return _token.first != token_type::eof; }
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

            inline auto begin() -> tokenizer_internal_iterator { return tokenizer_internal_iterator{*this}; }
            inline auto end() -> common_iterator_sentinel { return {}; };
        };

        inline tokenizer_internal_iterator::tokenizer_internal_iterator(tokenizer &t) : _toker{t}, _token{t.get_token()} {}
        inline auto tokenizer_internal_iterator::operator++() -> tokenizer_internal_iterator & { return (_token = _toker.get_token()), *this; }

        class lexical_iterator {
          public:
            using value_type        = token_t;
            using difference_type   = void;
            using pointer           = token_t *;
            using reference         = token_t &;
            using iterator_category = std::forward_iterator_tag;

          private:
            tokenizer &_toker;

          public:
            lexical_iterator(tokenizer &t);
            ~lexical_iterator() = default;
            inline auto operator++() -> lexical_iterator &;
            inline auto operator!=(common_iterator_sentinel) -> bool;
            inline auto operator*() -> token_t;
        };

        class lexical_analyzer {
          private:
            std::deque<token_t>                            _buffer;
            std::vector<std::pair<std::string, tokenizer>> _toker_pool;
            // indicates the active toker in _toker_pool
            int _active_toker_index = 0;

          public:
            lexical_analyzer(const std::string &filename);
            lexical_analyzer(lexical_analyzer &) = delete;
            ~lexical_analyzer()                  = default;

            auto peek() -> token_t;
            auto get() -> token_t;
            auto unget() -> void;
            /// @brief Get parsing position
            /// @return the filename, line, col.
            inline auto pos() -> std::tuple<std::string_view, size_t, size_t>
            {
                auto &&tmp  = _toker_pool[_active_toker_index];
                auto &&ttmp = tmp.second.get_pos();
                return {tmp.first, ttmp.first, ttmp.second};
            }
            inline auto operator()() { return get(); };
            /// @brief Get parsing position
            /// @return the filename, line, col.
            inline auto operator&() { return pos(); }
        };
    } // namespace lexer
} // namespace simpc

#endif // __LEXER
