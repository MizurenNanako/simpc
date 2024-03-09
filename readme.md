# Simple Compiler for C-

This project is a SYSU University *Compiler Principle Lecture* Group work.

This project should complete the implantation of a C- language compiler, covering most of the C language.

For the parser part, after long consideration and some attempts, I decided to separate the parsing logic and ast node generation.

All Copy constructors are marked deleted, until I implant them.

### REQUIREMENT

- **clang-format17** - Code formatting tool from clang project. (Should be accessible in `$PATH`.) **FORMATING IS NECESSARY!!!**
- **C++20 Standard** - If you're using gcc or mingw64, note that you will need gcc13 or higher for full C++20 support. See [Compiler Support](https://en.cppreference.com/w/cpp/compiler_support/20).
- **re2c v3.1** - To generate lexer DFA code. (Should be accessible in `$PATH`.)

### IMPLANTATION DETAILS

<details>
<summary>The parser stmt node meant to be implanted</summary>

```
1 [label="translation_unit"]
2 [label="external_declaration"]
3 [label="function_definition"]
4 [label="declaration"]
5 [label="declaration_specifier"]
6 [label="declarator"]
7 [label="compound_statement"]
8 [label="storage_class_specifier"]
9 [label="type_specifier"]
10 [label="type_qualifier"]
11 [label="struct_or_union_specifier"]
12 [label="struct_or_union"]
13 [label="enum_specifier"]
14 [label="typedef_name"]
15 [label="identifier"]
16 [label="struct_declaration"]
17 [label="specifier_qualifier"]
18 [label="struct_declarator_list"]
19 [label="struct_declarator"]
20 [label="constant_expression"]
21 [label="parameter_type_list"]
22 [label="pointer"]
23 [label="direct_declarator"]
24 [label="conditional_expression"]
25 [label="logical_or_expression"]
26 [label="logical_and_expression"]
27 [label="inclusive_or_expression"]
28 [label="exclusive_or_expression"]
29 [label="and_expression"]
30 [label="equality_expression"]
31 [label="relational_expression"]
32 [label="shift_expression"]
33 [label="additive_expression"]
34 [label="multiplicative_expression"]
35 [label="cast_expression"]
36 [label="unary_expression"]
37 [label="postfix_expression"]
38 [label="primary_expression"]
39 [label="assignment_expression"]
40 [label="expression"]
41 [label="type_name"]
42 [label="integer_constant"]
43 [label="character_constant"]
44 [label="floating_constant"]
45 [label="enumeration_constant"]
46 [label="constant"]
47 [label="string"]
48 [label="assignment_operator"]
49 [label="unary_operator"]
50 [label="abstract_declarator"]
51 [label="direct_abstract_declarator"]
52 [label="parameter_list"]
53 [label="parameter_declaration"]
54 [label="enumerator_list"]
55 [label="enumerator"]
56 [label="init_declarator"]
57 [label="initializer"]
58 [label="initializer_list"]
59 [label="labeled_statement"]
60 [label="expression_statement"]
61 [label="selection_statement"]
62 [label="iteration_statement"]
63 [label="jump_statement"]
64 [label="statement"]
```

</details>
<details><summary>The parser structure meant to be implanted</summary>

```
<1> ::= {<2>}*
<2> ::= <3> | <4>
<3> ::= {<5>}* <6> {<4>}* <7>
<4> ::=  {<5>}+ {<56>}* <op_semicolon>
<5> ::= <8> | <9> | <10>
<6> ::= {<22>}? <23>
<7> ::= <LBRACE> {<4>}* {<64>}* <RBRACE>
<8> ::= <kw_auto> | <kw_register> | <kw_static> | <kw_extern> | <kw_typedef>
<9> ::= <kw_void> | <kw_char> | <kw_short> | <kw_int> | <kw_long> | <kw_float> | <kw_double> | <kw_signed> | <kw_unsigned> | <11> | <13> | <14>
<10> ::= <kw_const> | <kw_volatile>
<11> ::= <12> <15> <LBRACE> {<16>}+ <RBRACE> | <12> <LBRACE> {<16>}+ <RBRACE> | <12> <15>
<12> ::= <kw_struct> | <kw_union>
<13> ::= <kw_enum> <15> <LBRACE> <54> <RBRACE> | <kw_enum> <LBRACE> <54> <RBRACE> | <kw_enum> <15>
<14> ::= <15>
<16> ::= {<17>}* <18>
<17> ::= <9> | <10>
<18> ::= <19> | <18> <op_comma> <19>
<19> ::= <6> | <6> <op_colon> <20> | <op_colon> <20>
<20> ::= <24>
<21> ::= <52> | <52> <op_comma> <op_ellipsis>
<22> ::= <op_mul> {<10>}* {<22>}?
<23> ::= <15> | <LPAREN> <6> <RPAREN> | <23> <LBRACK> {<20>}? <RBRACK> | <23> <LPAREN> <21> <RPAREN> | <23> <LPAREN> {<15>}* <RPAREN>
<24> ::= <25> | <25> <op_question> <40> <op_colon> <24>
<25> ::= <26> | <25> <op_or> <26>
<26> ::= <27> | <26> <op_and> <27>
<27> ::= <28> | <27> <op_bitor> <28>
<28> ::= <29> | <28> <op_bitxor> <29>
<29> ::= <30> | <29> <op_bitand> <30>
<30> ::= <31> | <30> <op_eeq> <31> | <30> <op_neq> <31>
<31> ::= <32> | <31> <op_lt> <32> | <31> <op_gt> <32> | <31> <op_leq> <32> | <31> <op_geq> <32>
<32> ::= <33> | <32> <op_shl> <33> | <32> <op_shr> <33>
<33> ::= <34> | <33> <op_add> <34> | <33> <op_sub> <34>
<34> ::= <35> | <34> <op_mul> <35> | <34> <op_div> <35> | <34> <op_mod> <35>
<35> ::= <36> | <LPAREN> <41> <RPAREN> <35>
<36> ::= <37> | <op_inc> <36> | <op_dec> <36> | <49> <35> | <kw_sizeof> <36> | <kw_sizeof> <41>
<37> ::= <38> | <37> <LBRACK> <40> <RBRACK> | <37> <LPAREN> {<39>}* <RPAREN> | <37> <op_dot> <15> | <37> <op_to> <15> | <37> <op_inc> | <37> <op_dec>
<38> ::= <15> | <46> | <47> | <LPAREN> <40> <RPAREN>
<39> ::= <24> | <36> <48> <39>
<40> ::= <39> | <40> <op_comma> <39>
<41> ::= {<17>}+ {<50>}?
<46> ::= <42> | <43> | <44> | <45>
<48> ::= <op_eq> | <op_muleq> | <op_diveq> | <op_modeq> | <op_addeq> | op_subeq | <op_shleq> | <op_shreq> | <op_bitandeq> | <op_bitxoreq> | <op_bitoreq>
<49> ::= <op_bitand> | <op_mul> | <op_add> | <op_sub> | <op_compl> | <op_not>
<50> ::= <22> | <22> <51> | <51>
<51> ::=  <LPAREN> <50> <RPAREN> | {<51>}? <LBRACK> {<20>}? <RBRACK> | {<51>}? <LPAREN> {<21>}? <RPAREN>
<52> ::= <53> | <52> <op_comma> <53>
<53> ::= {<5>}+ <6> | {<5>}+ <50> | {<5>}+
<54> ::= <55> | <54> <op_comma> <55>
<55> ::= <15> | <15> <op_eq> <20>
<56> ::= <6> | <6> <op_eq> <57>
<57> ::= <39> | <LBRACE> <58> <RBRACE> | <LBRACE> <58> <op_comma> <RBRACE>
<58> ::= <57> | <58> <op_comma> <57>
<59> ::= <15> <op_colon> <64> | <kw_case> <20> <op_colon> <64> | <kw_default> <op_colon> <64>
<60> ::= {<40>}? <op_semicolon>
<61> ::= <kw_if> <LPAREN> <40> <RPAREN> <64> | <kw_if> <LPAREN> <40> <RPAREN> <64> <kw_else> <64> | <kw_switch> <LPAREN> <40> <RPAREN> <64>
<62> ::= <kw_while> <LPAREN> <40> <RPAREN> <64> | <kw_do> <64> <kw_while> <LPAREN> <40> <RPAREN> <op_semicolon> | <kw_for> <LPAREN> {<40>}? <op_semicolon> {<40>}? <op_semicolon> {<40>}? <RPAREN> <64>
<63> ::= <kw_goto> <15> <op_semicolon> | <kw_continue> <op_semicolon> | <kw_break> <op_semicolon> | <kw_return> {<40>}? <op_semicolon>
<64> ::= <59> | <60> | <7> | <61> | <62> | <63>
```

</details>

<details>
<summary>The Origin BNF of C language (228 lines of definition)</summary>

```bnf
<translation-unit> ::= {<external-declaration>}*

<external-declaration> ::= <function-definition>
                         | <declaration>

<function-definition> ::= {<declaration-specifier>}* <declarator> {<declaration>}* <compound-statement>

<declaration-specifier> ::= <storage-class-specifier>
                          | <type-specifier>
                          | <type-qualifier>

<storage-class-specifier> ::= auto
                            | register
                            | static
                            | extern
                            | typedef

<type-specifier> ::= void
                   | char
                   | short
                   | int
                   | long
                   | float
                   | double
                   | signed
                   | unsigned
                   | <struct-or-union-specifier>
                   | <enum-specifier>
                   | <typedef-name>

<struct-or-union-specifier> ::= <struct-or-union> <identifier> { {<struct-declaration>}+ }
                              | <struct-or-union> { {<struct-declaration>}+ }
                              | <struct-or-union> <identifier>

<struct-or-union> ::= struct
                    | union

<struct-declaration> ::= {<specifier-qualifier>}* <struct-declarator-list>

<specifier-qualifier> ::= <type-specifier>
                        | <type-qualifier>

<struct-declarator-list> ::= <struct-declarator>
                           | <struct-declarator-list> , <struct-declarator>

<struct-declarator> ::= <declarator>
                      | <declarator> : <constant-expression>
                      | : <constant-expression>

<declarator> ::= {<pointer>}? <direct-declarator>

<pointer> ::= * {<type-qualifier>}* {<pointer>}?

<type-qualifier> ::= const
                   | volatile

<direct-declarator> ::= <identifier>
                      | ( <declarator> )
                      | <direct-declarator> [ {<constant-expression>}? ]
                      | <direct-declarator> ( <parameter-type-list> )
                      | <direct-declarator> ( {<identifier>}* )

<constant-expression> ::= <conditional-expression>

<conditional-expression> ::= <logical-or-expression>
                           | <logical-or-expression> ? <expression> : <conditional-expression>

<logical-or-expression> ::= <logical-and-expression>
                          | <logical-or-expression> || <logical-and-expression>

<logical-and-expression> ::= <inclusive-or-expression>
                           | <logical-and-expression> && <inclusive-or-expression>

<inclusive-or-expression> ::= <exclusive-or-expression>
                            | <inclusive-or-expression> | <exclusive-or-expression>

<exclusive-or-expression> ::= <and-expression>
                            | <exclusive-or-expression> ^ <and-expression>

<and-expression> ::= <equality-expression>
                   | <and-expression> & <equality-expression>

<equality-expression> ::= <relational-expression>
                        | <equality-expression> == <relational-expression>
                        | <equality-expression> != <relational-expression>

<relational-expression> ::= <shift-expression>
                          | <relational-expression> < <shift-expression>
                          | <relational-expression> > <shift-expression>
                          | <relational-expression> <= <shift-expression>
                          | <relational-expression> >= <shift-expression>

<shift-expression> ::= <additive-expression>
                     | <shift-expression> << <additive-expression>
                     | <shift-expression> >> <additive-expression>

<additive-expression> ::= <multiplicative-expression>
                        | <additive-expression> + <multiplicative-expression>
                        | <additive-expression> - <multiplicative-expression>

<multiplicative-expression> ::= <cast-expression>
                              | <multiplicative-expression> * <cast-expression>
                              | <multiplicative-expression> / <cast-expression>
                              | <multiplicative-expression> % <cast-expression>

<cast-expression> ::= <unary-expression>
                    | ( <type-name> ) <cast-expression>

<unary-expression> ::= <postfix-expression>
                     | ++ <unary-expression>
                     | -- <unary-expression>
                     | <unary-operator> <cast-expression>
                     | sizeof <unary-expression>
                     | sizeof <type-name>

<postfix-expression> ::= <primary-expression>
                       | <postfix-expression> [ <expression> ]
                       | <postfix-expression> ( {<assignment-expression>}* )
                       | <postfix-expression> . <identifier>
                       | <postfix-expression> -> <identifier>
                       | <postfix-expression> ++
                       | <postfix-expression> --

<primary-expression> ::= <identifier>
                       | <constant>
                       | <string>
                       | ( <expression> )

<constant> ::= <integer-constant>
             | <character-constant>
             | <floating-constant>
             | <enumeration-constant>

<expression> ::= <assignment-expression>
               | <expression> , <assignment-expression>

<assignment-expression> ::= <conditional-expression>
                          | <unary-expression> <assignment-operator> <assignment-expression>

<assignment-operator> ::= =
                        | *=
                        | /=
                        | %=
                        | +=
                        | -=
                        | <<=
                        | >>=
                        | &=
                        | ^=
                        | |=

<unary-operator> ::= &
                   | *
                   | +
                   | -
                   | ~
                   | !

<type-name> ::= {<specifier-qualifier>}+ {<abstract-declarator>}?

<parameter-type-list> ::= <parameter-list>
                        | <parameter-list> , ...

<parameter-list> ::= <parameter-declaration>
                   | <parameter-list> , <parameter-declaration>

<parameter-declaration> ::= {<declaration-specifier>}+ <declarator>
                          | {<declaration-specifier>}+ <abstract-declarator>
                          | {<declaration-specifier>}+

<abstract-declarator> ::= <pointer>
                        | <pointer> <direct-abstract-declarator>
                        | <direct-abstract-declarator>

<direct-abstract-declarator> ::=  ( <abstract-declarator> )
                               | {<direct-abstract-declarator>}? [ {<constant-expression>}? ]
                               | {<direct-abstract-declarator>}? ( {<parameter-type-list>}? )

<enum-specifier> ::= enum <identifier> { <enumerator-list> }
                   | enum { <enumerator-list> }
                   | enum <identifier>

<enumerator-list> ::= <enumerator>
                    | <enumerator-list> , <enumerator>

<enumerator> ::= <identifier>
               | <identifier> = <constant-expression>

<typedef-name> ::= <identifier>

<declaration> ::=  {<declaration-specifier>}+ {<init-declarator>}* ;

<init-declarator> ::= <declarator>
                    | <declarator> = <initializer>

<initializer> ::= <assignment-expression>
                | { <initializer-list> }
                | { <initializer-list> , }

<initializer-list> ::= <initializer>
                     | <initializer-list> , <initializer>

<compound-statement> ::= { {<declaration>}* {<statement>}* }

<statement> ::= <labeled-statement>
              | <expression-statement>
              | <compound-statement>
              | <selection-statement>
              | <iteration-statement>
              | <jump-statement>

<labeled-statement> ::= <identifier> : <statement>
                      | case <constant-expression> : <statement>
                      | default : <statement>

<expression-statement> ::= {<expression>}? ;

<selection-statement> ::= if ( <expression> ) <statement>
                        | if ( <expression> ) <statement> else <statement>
                        | switch ( <expression> ) <statement>

<iteration-statement> ::= while ( <expression> ) <statement>
                        | do <statement> while ( <expression> ) ;
                        | for ( {<expression>}? ; {<expression>}? ; {<expression>}? ) <statement>

<jump-statement> ::= goto <identifier> ;
                   | continue ;
                   | break ;
                   | return {<expression>}? ;
```

This grammar was adapted from Section A13 of The C programming language, 2nd edition, by Brian W. Kernighan and Dennis M. Ritchie,Prentice Hall, 1988.

</details>
