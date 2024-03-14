#ifndef __LEXER_TOKEN_TYPE
#define __LEXER_TOKEN_TYPE

// precompiled header
#include "stdafx.hh"

namespace simpc
{
    namespace lexical
    {
        enum class token_type;
    } // namespace lexer

} // namespace simpc

enum class simpc::lexical::token_type : int
{
// Meta
    err = 0,
    macro_param,     // not a lexcial symbol, just place holder.
    preprocesser,    // with # preserved
    alt_preprocesser,// supports %:

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
    op_ellipsis,   // ...
    newline,
    eof = EOF,
};

inline constexpr auto operator*(simpc::lexical::token_type t) -> const char *
{
    using tt = simpc::lexical::token_type;
    switch (t)
    {
        case tt::preprocesser:      return "[prep]";
        case tt::identifier:        return "[id]";
        case tt::literial_dec:      return "[dec]";
        case tt::literial_oct:      return "[oct]";
        case tt::literial_hex:      return "[hex]";
        case tt::literial_bin:      return "[bin]";
        case tt::literial_real:     return "[real]";
        case tt::literial_char:     return "[char]";
        case tt::literial_string:   return "[string]";
        case tt::linecomment:       return "[linecomment]";
        case tt::blockcomment:      return "[blockcomment]";
        case tt::zero:              return "0";
        case tt::LPAREN:            return "(";
        case tt::RPAREN:            return ")";
        case tt::LBRACK:            return "[";
        case tt::RBRACK:            return "]";
        case tt::LBRACE:            return "{";
        case tt::RBRACE:            return "}";
        case tt::kw_auto:           return "auto";
        case tt::kw_break:          return "break";
        case tt::kw_case:           return "case";
        case tt::kw_char:           return "char";
        case tt::kw_const:          return "const";
        case tt::kw_continue:       return "continue";
        case tt::kw_default:        return "default";
        case tt::kw_do:             return "do";
        case tt::kw_double:         return "double";
        case tt::kw_else:           return "else";
        case tt::kw_enum:           return "enum";
        case tt::kw_extern:         return "extern";
        case tt::kw_float:          return "float";
        case tt::kw_for:            return "for";
        case tt::kw_goto:           return "goto";
        case tt::kw_if:             return "if";
        case tt::kw_int:            return "int";
        case tt::kw_long:           return "long";
        case tt::kw_register:       return "register";
        case tt::kw_return:         return "return";
        case tt::kw_short:          return "short";
        case tt::kw_signed:         return "signed";
        case tt::kw_sizeof:         return "sizeof";
        case tt::kw_static:         return "static";
        case tt::kw_struct:         return "struct";
        case tt::kw_switch:         return "switch";
        case tt::kw_typedef:        return "typedef";
        case tt::kw_union:          return "union";
        case tt::kw_unsigned:       return "unsigned";
        case tt::kw_void:           return "void";
        case tt::kw_volatile:       return "volatile";
        case tt::kw_while:          return "while";
        case tt::kw_inline:         return "inline";
        case tt::kw_restrict:       return "restrict";
        case tt::kw__Bool:          return "_Bool";
        case tt::kw__Complex:       return "_Complex";
        case tt::kw__Imaginary:     return "_Imaginary";
        case tt::kw__Alignas:       return "_Alignas";
        case tt::kw__Alignof:       return "_Alignof";
        case tt::kw__Atomic:        return "_Atomic";
        case tt::kw__Generic:       return "_Generic";
        case tt::kw__Noreturn:      return "_Noreturn";
        case tt::kw__Static_assert: return "_Static_assert";
        case tt::kw__Thread_local:  return "_Thread_local";
        case tt::op_eq:             return "=";
        case tt::op_addeq:          return "+=";
        case tt::op_subeq:          return "-=";
        case tt::op_muleq:          return "*=";
        case tt::op_diveq:          return "/=";
        case tt::op_modeq:          return "%=";
        case tt::op_shleq:          return "<<=";
        case tt::op_shreq:          return ">>=";
        case tt::op_bitandeq:       return "&=";
        case tt::op_bitoreq:        return "|=";
        case tt::op_bitxoreq:       return "^=";
        case tt::op_inc:            return "++";
        case tt::op_dec:            return "--";
        case tt::op_compl:          return "~";
        case tt::op_add:            return "+";
        case tt::op_sub:            return "-";
        case tt::op_mul:            return "*";
        case tt::op_div:            return "/";
        case tt::op_mod:            return "%";
        case tt::op_shl:            return "<<";
        case tt::op_shr:            return ">>";
        case tt::op_bitand:         return "&";
        case tt::op_bitor:          return "|";
        case tt::op_bitxor:         return "^";
        case tt::op_not:            return "!";
        case tt::op_and:            return "&&";
        case tt::op_or:             return "||";
        case tt::op_eeq:            return "==";
        case tt::op_neq:            return "!=";
        case tt::op_lt:             return "<";
        case tt::op_gt:             return ">";
        case tt::op_leq:            return "<=";
        case tt::op_geq:            return ">=";
        case tt::op_dot:            return ".";
        case tt::op_to:             return "->";
        case tt::op_comma:          return ",";
        case tt::op_question:       return "?";
        case tt::op_colon:          return ":";
        case tt::op_semicolon:      return ";";
        case tt::op_ellipsis:       return "...";
        case tt::newline:           return "[newline]";
        default:                    return "[unknown]";
    }
}

inline auto operator<<(std::ostream &out, const simpc::lexical::token_type &t) -> std::ostream & { return out << *t; }

template<>
struct std::formatter<simpc::lexical::token_type, char> {
    bool name = false;

    template<class ParseContext>
    constexpr ParseContext::iterator parse(ParseContext &ctx)
    {
        auto it = ctx.begin();
        if (it == ctx.end()) return it;
        if (*it == 'n') name = ++it, true;
        else if (*it == 'i') ++it;
        if (*it != '}') throw std::format_error("Invalid format args for simpc::lexer::token_type");
        return it;
    }

    template<class FmtContext>
    inline FmtContext::iterator format(simpc::lexical::token_type s, FmtContext &ctx) const
    {
        return std::ranges::copy(
                   std::move(
                       name ? *s
                            : std::to_string(static_cast<int>(s))),
                   ctx.out())
            .out;
    }
};

#endif // __LEXER_TOKEN_TYPE
