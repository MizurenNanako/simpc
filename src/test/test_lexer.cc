#include "lexer.hh"
#include "stdafx.hh"
using namespace std::literals;

auto getsym(simpc::lexer::token_type swc) -> const char *
{
    using tt = simpc::lexer::token_type;
    switch (swc)
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
        default:                    return "unknown";
    }
}

auto data = R"(#include <stdio.h>
#include "linking_list.h"

void pout(llist_int *list)
{
    for (int i = 0; i < list->length; ++i)
    {
        printf("index: %d\t data: %d\n", i, query_index_list_int(list, i));
    }
    puts("");
}

int main()
{
    llist_int *list = create_list_int(0);
    for (int i = 1; i < 10; ++i)
    {
        push_back_list_int(list, i);
    }
    pout(list);
    insert_index_list_int(list, 5, 999);
    insert_index_list_int(list, 3, 777);
    pout(list);
    delete_list_node_int(list, 8);
    delete_list_node_int(list, 8);
    pout(list);
    delete_list_int(list);
    return 0;
}
)"s;

auto main() -> int
{
    using simpc::lexer::token_type;
    auto ss    = std::istringstream{data};
    auto toker = simpc::lexer::tokenizer{ss};

    auto tok = simpc::lexer::token_t{};
    while ((tok = toker()).first != token_type::eof)
    // I am not handling EOF actually, this will be an exception.
    {
        if (tok.second.has_value())
            std::cout << std::format("typeid: {}\ttype: {}\tinfo: \"{}\"",
                                     (int) tok.first,
                                     getsym(tok.first),
                                     *tok.second)
                      << std::endl;
        else
            std::cout << std::format("typeid: {}\tsymbol: \"{}\"",
                                     (int) tok.first,
                                     getsym(tok.first))
                      << std::endl;
    }

    return 0;
}
