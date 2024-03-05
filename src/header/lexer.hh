#ifndef __LEXER
#define __LEXER

#include "stdafx.hh"

namespace simpc
{
    namespace lexer
    {
        enum class token_type;

        using char_type    = char;
        using token_info_t = std::basic_string<char_type>;

        /// @brief Type of a lexical token.
        /// @brief Pair of token_type and optional info.
        using token_t = std::pair<token_type,
                                  std::optional<token_info_t>>;
        using _stream = std::basic_istream<char_type>;
        using _buffer = std::basic_string<char_type>;

        class NotEnoughtInputError : protected std::runtime_error {
          public:
            NotEnoughtInputError(const size_t lineno, const size_t col)
                : runtime_error(
                    std::format("Not enough input on line: {}, col: {}",
                                lineno, col)) {}
            virtual ~NotEnoughtInputError() noexcept = default;
        };

        class LexicalError : protected std::runtime_error {
          public:
            LexicalError(const size_t lineno, const size_t col,
                         const std::string_view errlex)
                : runtime_error(std::format(
                    "Lexical Error at line: {}, col: {}, error: \"{}\"",
                    lineno, col, errlex)) {}
            virtual ~LexicalError() noexcept = default;
        };

        class tokenizer {
          private:
            _stream &_inputs;
            _buffer  _tokbuf; // token buffer

            size_t _lineno = 0; // Line number
            size_t _cols   = 0; // Column number

            std::streampos _marker;
            std::streampos _ctxmarker;

          public:
            tokenizer(std::istream &input = std::cin);
            tokenizer(tokenizer &) = delete;
            ~tokenizer()           = default;

            /// @brief Get token from bindded input stream.
            /// @return The token_type and info if any.
            auto        get_token() -> token_t;
            inline auto operator()() { return get_token(); };
        };
    } // namespace lexer
} // namespace simpc

enum class simpc::lexer::token_type : int
{
// Meta
    err = 0,
    preprocesser,    // with # preserved

// With Info
    identifier,      // [A-Za-z_$]+[A-Za-z0-9_$]*
    zero,            // 0
    // {isuffix} := ((u|U)?(l|L|ll|LL|z|Z)?|(l|L|ll|LL|z|Z)?(u|U)?)
    literial_dec,    // (+|-)?[1-9][0-9']*{isuffix}
    literial_oct,    // 0[0-9]*{isuffix}
    literial_hex,    // 0(x|X)[0-9A-Fa-f]*
    literial_bin,    // 0(b|B)[01]*
    // {fsuffix} := (f|F|l|L)
    // {significand} := {wholenumber}.{fraction}|.{fraction}|{wholenumber}.
    // {exponent} := (e|E|p|P)(+|-)?[0-9]+
    literial_real,   // {significand}{exponent}?{fsuffix}?
    // {cchar} := [^'\\\n]|\\'|\\"|\\?|\\\\|\\a|\\b|\\f|\\n|\\r|\\t|\\v|\\x[0-9A-Fa-f]+|\\u[0-9A-Fa-f]{4}
    literial_char,   // '{cchar}{1,2}'
    literial_string, // "{cchar}*"
    linecomment,         // Oh, who want this?
    blockcomment,        // another one

// Parenthesis
    LPAREN, // '('
    RPAREN, // ')'
    LBRACK, // '['
    RBRACK, // ']'
    LBRACE, // '{'
    RBRACE, // '}'

// Keywords
// (C98)
    kw_auto,
    kw_break,
    kw_case,
    kw_char,
    kw_const,
    kw_continue,
    kw_default,
    kw_do,
    kw_double,
    kw_else,
    kw_enum,
    kw_extern,
    kw_float,
    kw_for,
    kw_goto,
    kw_if,
    kw_int,
    kw_long,
    kw_register,
    kw_return,
    kw_short,
    kw_signed,
    kw_sizeof,
    kw_static,
    kw_struct,
    kw_switch,
    kw_typedef,
    kw_union,
    kw_unsigned,
    kw_void,
    kw_volatile,
    kw_while,
// (C99)
    kw_inline,
    kw_restrict,
    kw__Bool,
    kw__Complex,
    kw__Imaginary,
// (C11)
    kw__Alignas,
    kw__Alignof,
    kw__Atomic,
    kw__Generic,
    kw__Noreturn,
    kw__Static_assert,
    kw__Thread_local,

// Operators
// assignment
    op_eq,         // =
    op_addeq,      // +=
    op_subeq,      // -=
    op_muleq,      // *=
    op_diveq,      // /=
    op_modeq,      // %=
    op_shleq,      // <<=
    op_shreq,      // >>=
    op_bitandeq,   // &=
    op_bitoreq,    // |=
    op_bitxoreq,   // ^=
// inc/dec
    op_inc,        // ++
    op_dec,        // --
// arithmatic
    op_compl,      // ~
    op_add,        // +
    op_sub,        // -
    op_mul,        // *
    op_div,        // /
    op_mod,        // %
    op_shl,        // <<
    op_shr,        // >>
    op_bitand,     // &
    op_bitor,      // |
    op_bitxor,     // ^
// logical
    op_not,        // !
    op_and,        // &&
    op_or,         // ||
// comparison
    op_eeq,        // ==
    op_neq,        // !=
    op_lt,         // <
    op_gt,         // >
    op_leq,        // <=
    op_geq,        // >=
// member
    op_dot,        // .
    op_to,         // ->
// other
    op_comma,      // ,
    op_question,   // ?
    op_colon,      // :

    op_semicolon,  // ;
    newline,
    eof = EOF,
};

#endif // __LEXER
