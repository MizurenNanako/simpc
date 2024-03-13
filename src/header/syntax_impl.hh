#ifndef __SYNTAX_IMPL
#define __SYNTAX_IMPL

// precompiled header
#include "stdafx.hh"

// headers
#include "lexer.hh"

namespace simpc
{
    namespace syntactical
    {
        namespace impl
        {
            enum class _stmt_node_type;
            template<typename subiter_t>
            class _stmt_node_iterator;
            class _stmt_node;
            using _node_children_t = std::vector<std::unique_ptr<_stmt_node>>;
        } // namespace impl
        class ast_tree;
    } // namespace syntaxer
} // namespace simpc

enum class simpc::syntactical::impl::_stmt_node_type : int
{
    err = 0,
    translation_unit,
    external_declaration,
    function_definition,
    declaration,
    declaration_specifier,
    declarator,
    compound_statement,
    storage_class_specifier,
    type_specifier,
    type_qualifier,
    struct_or_union_specifier,
    struct_or_union,
    enum_specifier,
    typedef_name,
    identifier,
    struct_declaration,
    specifier_qualifier,
    struct_declarator_list,
    struct_declarator,
    constant_expression,
    parameter_type_list,
    pointer,
    direct_declarator,
    conditional_expression,
    logical_or_expression,
    logical_and_expression,
    inclusive_or_expression,
    exclusive_or_expression,
    and_expression,
    equality_expression,
    relational_expression,
    shift_expression,
    additive_expression,
    multiplicative_expression,
    cast_expression,
    unary_expression,
    postfix_expression,
    primary_expression,
    assignment_expression,
    expression,
    type_name,
    integer_constant,
    character_constant,
    floating_constant,
    enumeration_constant,
    constant,
    string,
    assignment_operator,
    unary_operator,
    abstract_declarator,
    direct_abstract_declarator,
    parameter_list,
    parameter_declaration,
    enumerator_list,
    enumerator,
    init_declarator,
    initializer,
    initializer_list,
    labeled_statement,
    expression_statement,
    selection_statement,
    iteration_statement,
    jump_statement,
    statement,
};

template<typename subiter_t>
class simpc::syntactical::impl::_stmt_node_iterator {
  private:
    subiter_t true_iter;

  public:
    using value_type        = _stmt_node;
    using pointer           = _stmt_node *;
    using reference         = _stmt_node &;
    using size_type         = size_t;
    using difference_type   = ptrdiff_t;
    using iterator_catagory = std::bidirectional_iterator_tag;
    _stmt_node_iterator(subiter_t &&vecitr) : true_iter{vecitr} {};
    ~_stmt_node_iterator() = default;
    inline auto operator++()
        -> _stmt_node_iterator<subiter_t> & { return ++true_iter, *this; }
    inline auto operator--()
        -> _stmt_node_iterator<subiter_t> & { return --true_iter, *this; }
    inline auto operator==(const _stmt_node_iterator<subiter_t> &rhs)
        -> bool { return true_iter == rhs.true_iter; }
    inline auto operator*() -> reference { return **true_iter; }
};

class simpc::syntactical::impl::_stmt_node {
  private:
    // the syntax type for this node.
    _stmt_node_type _type;
    // token holds full lexical imformation.
    lexical::token_t _token;
    // pointers to children.
    // vector always takes 3 size_t, unique_ptr takes 1 size_t.
    _node_children_t _children;

  public:
    _stmt_node(_stmt_node_type       type  = {_stmt_node_type::err},
               const lexical::token_t &token = {lexical::token_type::err, {}});
    _stmt_node(_stmt_node &) = delete;
    ~_stmt_node()            = default;
    inline auto type() const -> _stmt_node_type { return _type; }
    inline auto token() const -> const lexical::token_t & { return _token; }
    inline auto operator[](size_t index) -> _stmt_node & { return *(_children[index]); }
    inline auto operator[](size_t index) const -> _stmt_node & { return *(_children[index]); }
    inline auto at(size_t index) -> _stmt_node &
    {
        if (_children.at(index))
            return *(_children[index]);
        else throw(std::out_of_range{
            std::format("index: {}, at node with token {:n}",
                        index, _token.first)});
    }
    inline auto begin() -> _stmt_node_iterator<_node_children_t::iterator> { return _stmt_node_iterator{std::move(_children.begin())}; }
    inline auto cbegin() const -> _stmt_node_iterator<_node_children_t::const_iterator> { return _stmt_node_iterator{std::move(_children.cbegin())}; }
    inline auto rbegin() const -> _stmt_node_iterator<_node_children_t::const_reverse_iterator> { return _stmt_node_iterator{std::move(_children.rbegin())}; }
    inline auto crbegin() const -> _stmt_node_iterator<_node_children_t::const_reverse_iterator> { return _stmt_node_iterator{std::move(_children.crbegin())}; }
    inline auto end() -> _stmt_node_iterator<_node_children_t::iterator> { return _stmt_node_iterator{std::move(_children.end())}; }
    inline auto cend() const -> _stmt_node_iterator<_node_children_t::const_iterator> { return _stmt_node_iterator{std::move(_children.cend())}; }
    inline auto rend() -> _stmt_node_iterator<_node_children_t::reverse_iterator> { return _stmt_node_iterator{std::move(_children.rend())}; }
    inline auto crend() const -> _stmt_node_iterator<_node_children_t::const_reverse_iterator> { return _stmt_node_iterator{std::move(_children.crend())}; }
};

class simpc::syntactical::ast_tree {
  private:
    // root node must be a translation_unit
    impl::_stmt_node _root;

  public:
    ast_tree()           = default;
    ast_tree(ast_tree &) = delete;
    ~ast_tree()          = default;

    inline auto operator[](size_t index) -> impl::_stmt_node & { return _root[index]; }
    inline auto operator[](size_t index) const -> impl::_stmt_node & { return _root[index]; }
    inline auto at(size_t index) -> impl::_stmt_node & { return _root.at(index); }

};

inline constexpr auto operator*(simpc::syntactical::impl::_stmt_node_type t) -> const char *
{
    using tt = simpc::syntactical::impl::_stmt_node_type;
    switch (t)
    {
        case tt::err:                        return "{err}";
        case tt::translation_unit:           return "{translation_unit}";
        case tt::external_declaration:       return "{external_declaration}";
        case tt::function_definition:        return "{function_definition}";
        case tt::declaration:                return "{declaration}";
        case tt::declaration_specifier:      return "{declaration_specifier}";
        case tt::declarator:                 return "{declarator}";
        case tt::compound_statement:         return "{compound_statement}";
        case tt::storage_class_specifier:    return "{storage_class_specifier}";
        case tt::type_specifier:             return "{type_specifier}";
        case tt::type_qualifier:             return "{type_qualifier}";
        case tt::struct_or_union_specifier:  return "{struct_or_union_specifier}";
        case tt::struct_or_union:            return "{struct_or_union}";
        case tt::enum_specifier:             return "{enum_specifier}";
        case tt::typedef_name:               return "{typedef_name}";
        case tt::identifier:                 return "{identifier}";
        case tt::struct_declaration:         return "{struct_declaration}";
        case tt::specifier_qualifier:        return "{specifier_qualifier}";
        case tt::struct_declarator_list:     return "{struct_declarator_list}";
        case tt::struct_declarator:          return "{struct_declarator}";
        case tt::constant_expression:        return "{constant_expression}";
        case tt::parameter_type_list:        return "{parameter_type_list}";
        case tt::pointer:                    return "{pointer}";
        case tt::direct_declarator:          return "{direct_declarator}";
        case tt::conditional_expression:     return "{conditional_expression}";
        case tt::logical_or_expression:      return "{logical_or_expression}";
        case tt::logical_and_expression:     return "{logical_and_expression}";
        case tt::inclusive_or_expression:    return "{inclusive_or_expression}";
        case tt::exclusive_or_expression:    return "{exclusive_or_expression}";
        case tt::and_expression:             return "{and_expression}";
        case tt::equality_expression:        return "{equality_expression}";
        case tt::relational_expression:      return "{relational_expression}";
        case tt::shift_expression:           return "{shift_expression}";
        case tt::additive_expression:        return "{additive_expression}";
        case tt::multiplicative_expression:  return "{multiplicative_expression}";
        case tt::cast_expression:            return "{cast_expression}";
        case tt::unary_expression:           return "{unary_expression}";
        case tt::postfix_expression:         return "{postfix_expression}";
        case tt::primary_expression:         return "{primary_expression}";
        case tt::assignment_expression:      return "{assignment_expression}";
        case tt::expression:                 return "{expression}";
        case tt::type_name:                  return "{type_name}";
        case tt::integer_constant:           return "{integer_constant}";
        case tt::character_constant:         return "{character_constant}";
        case tt::floating_constant:          return "{floating_constant}";
        case tt::enumeration_constant:       return "{enumeration_constant}";
        case tt::constant:                   return "{constant}";
        case tt::string:                     return "{string}";
        case tt::assignment_operator:        return "{assignment_operator}";
        case tt::unary_operator:             return "{unary_operator}";
        case tt::abstract_declarator:        return "{abstract_declarator}";
        case tt::direct_abstract_declarator: return "{direct_abstract_declarator}";
        case tt::parameter_list:             return "{parameter_list}";
        case tt::parameter_declaration:      return "{parameter_declaration}";
        case tt::enumerator_list:            return "{enumerator_list}";
        case tt::enumerator:                 return "{enumerator}";
        case tt::init_declarator:            return "{init_declarator}";
        case tt::initializer:                return "{initializer}";
        case tt::initializer_list:           return "{initializer_list}";
        case tt::labeled_statement:          return "{labeled_statement}";
        case tt::expression_statement:       return "{expression_statement}";
        case tt::selection_statement:        return "{selection_statement}";
        case tt::iteration_statement:        return "{iteration_statement}";
        case tt::jump_statement:             return "{jump_statement}";
        case tt::statement:                  return "{statement}";
        default:                             return "[unknown]";
    }
}

inline auto operator<<(std::ostream &out, simpc::syntactical::impl::_stmt_node_type t) -> std::ostream & { return out << *t; }

template<>
struct std::formatter<simpc::syntactical::impl::_stmt_node_type, char> {
    bool name = false;

    template<class ParseContext>
    constexpr ParseContext::iterator parse(ParseContext &ctx)
    {
        auto it = ctx.begin();
        if (it == ctx.end()) return it;
        if (*it == 'n') name = ++it, true;
        else if (*it == 'i') ++it;
        if (*it != '}') throw std::format_error("Invalid format args for simpc::syntaxer::impl::_stmt_node_type");
        return it;
    }

    template<class FmtContext>
    inline FmtContext::iterator format(simpc::syntactical::impl::_stmt_node_type s, FmtContext &ctx) const
    {
        return std::ranges::copy(
                   std::move(
                       name ? *s
                            : std::to_string(static_cast<int>(s))),
                   ctx.out())
            .out;
    }
};

#endif // __SYNTAX_IMPL
