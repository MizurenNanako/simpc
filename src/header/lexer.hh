#ifndef __LEXER
#define __LEXER

// precompiled header
#include "stdafx.hh"

// headers
#include "lexer_token_type.hh"

namespace simpc
{
    namespace lexical
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

        class lexer {
          private:
            std::deque<token_t>                          _lex_buffer;
            std::stack<tokenizer>                        _tokers;
            std::stack<std::string>                      _filenames;
            std::map<std::string, std::function<void()>> _replacements;

          public:
            /**
             * @brief Construct a new lexer object
             * to parse on input.
             * @param context The first input.
             */
            lexer(std::istream &context, std::string_view filename);
            lexer(lexer &) = delete;
            ~lexer()       = default;

            auto peek() -> token_t;
            auto get() -> token_t;
            auto register_macro(std::string_view     name,
                                std::vector<token_t> tokens) -> void;

            inline auto getpos() -> std::tuple<size_t, size_t, std::string>
            {
                auto &&[a, b] = _tokers.top().get_pos();
                return {a, b, _filenames.top()};
            }
            inline auto putback(token_t tok) -> void { _lex_buffer.emplace_back(tok); }
        };
    } // namespace lexer
} // namespace simpc

#endif // __LEXER
