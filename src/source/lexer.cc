#include "lexer.hh"

namespace simpc
{
    namespace lexical
    {
        tokenizer::tokenizer(std::istream    &input,
                             std::string_view filename)
            : _lineno{start_lineno},
              _cols{start_cols},
              _filename{filename},
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
                [this] { throw LexicalError(_lineno, _cols, _filename, {token_type::err, _tokbuf}); };
        start:
            _tokbuf.clear();

            // retrive a char from stream
            auto tok = token_type::err;

            // match keywords or identifier
#include "lexer.ccpart"

        finish:
            return {tok, {}};
        finish_with_info:
            switch (tok)
            {
                case token_type::literial_char:
                case token_type::literial_string:
                case token_type::preprocesser:
                    return {tok, _tokbuf.substr(1, _tokbuf.length() - 2)};
                case token_type::alt_preprocesser:
                case token_type::blockcomment:
                    return {tok, _tokbuf.substr(2, _tokbuf.length() - 2)};
                case token_type::linecomment:
                    return {tok, _tokbuf.substr(2, _tokbuf.length() - 3)};
                default:
                    return {tok, _tokbuf.substr(0, _tokbuf.length() - 1)};
            }
        }
        lexer::lexer(std::istream &context, std::string_view filename)
        {
            // push-in main input
            _tokers.emplace(context);
            _filenames.emplace(filename);
        }
        auto lexer::peek() -> token_t
        {
            if (_lex_buffer.empty())
            {
                // refill buffer
                while (true)
                {
                    auto &&tmp = _tokers.top().get_token();
                    if (is_one_of(tmp.first,
                                  token_type::blockcomment,
                                  token_type::linecomment))
                        // No reason to keep comment at least for now
                        continue;
                    else if (is_one_of(
                                 tmp.first,
                                 token_type::alt_preprocesser,
                                 token_type::preprocesser))
                    {
                        // todo: trigger preprocessor operation
                        // alt_prep and prep won't differ in this stage.
                        preprocess(tmp);
                        break;
                    }
                    else if (tmp.first == token_type::identifier) {
                        // todo: trigger macro check and expansion
                        break;
                    }
                    else if (tmp.first == token_type::eof) {
                        _lex_buffer.emplace_back(std::move(tmp));
                        break;
                    }
                    else
                        // get as much token at one time as possible
                        _lex_buffer.emplace_back(std::move(tmp));
                }
            }
            return _lex_buffer.front();
        }

        auto lexer::preprocess(const token_t &t) -> void
        {
            static constexpr auto report_error =
                [](auto &&tup, auto &&t) {
                    auto &&[l, c, n] = tup;
                    throw(LexicalError{l, c, n, t});
                };
            if (is_none_of(t, token_type::alt_preprocesser,
                           token_type::preprocesser))
                report_error(getpos(), t);

                // now t ensured to be prep instruction
            auto &&info = *t.second;
            if (info.starts_with("define "))
            {
                // register_macro
                // maybe: 1. single symbol define
                // maybe: 2. single with whitespace tail;
                // maybe: 3. name with replacement list.
                // maybe: 4. macro function with or without replacement list.
                auto sbuf  = std::stringstream{info};
                auto toker = tokenizer{sbuf};

                // all tokens can be get from toker, but the space between
                // first id and first paren have to be test manually.
                auto &&t0 = toker.get_token();
                // should be name
                if (t0.first != token_type::identifier) // wrong define
                    report_error(getpos(), t);

                auto lparenpos = info.find_first_of('(');
                if (lparenpos == std::string::npos
                    or info.at(lparenpos - 1) == ' ')
                {
                    // no paren appears means this is 1. 2. 3.
                    auto &&tn = std::vector<token_t>{};
                    for (auto &&tt : toker)
                        tn.emplace_back(std::move(tt));
                    register_macro(*t0.second, {}, std::move(tn));
                }
                else
                {
                    // should be 4.
                    if (toker.get_token().first != token_type::LPAREN)
                        // wrong symbol
                        report_error(getpos(), t);
                    // macro args
                    auto &&ta = std::vector<token_t>{};
                    while (true)
                    {
                        auto &&t1 = toker.get_token();
                        if (t1.first == token_type::RPAREN) [[unlikely]]
                            break; // empty
                        else if (t1.first == token_type::identifier) // arg
                        {
                            ta.emplace_back(std::move(t1));
                            auto &&t2 = toker.get_token();
                            if (t2.first == token_type::op_comma) // must be ,
                                continue;
                            else if (t2.first == token_type::RPAREN)
                                break;
                            else report_error(getpos(), t);
                        }
                        else report_error(getpos(), t);
                    }
                    // replacement list
                    auto &&tn = std::vector<token_t>{};
                    for (auto &&tt : toker)
                        tn.emplace_back(std::move(tt));
                    register_macro(*t0.second,
                                   std::move(ta),
                                   std::move(tn));
                }
            }
            else if (info.starts_with("include"))
            {
                // add_include
                auto is_system_header = false;

                // sbuf from existing string won't alloc buffer.
                auto sbuf = std::stringstream{info};
                sbuf.seekg(sbuf.beg + "include"sv.length());

                // skip spaces
                auto ch = sbuf.get();
                while (not sbuf.eof() and is_none_of(ch, '<', '\"'))
                    ch = sbuf.get();
                // should be at < or \"
                if (sbuf.eof()) report_error(getpos(), t);
                if (ch == '<') is_system_header = true;

                // get filename
                auto filename = std::string{20};
                ch            = sbuf.get();
                while (not sbuf.eof() and is_none_of(ch, '>', '\"'))
                {
                    filename.push_back(ch);
                    ch = sbuf.get();
                }
                // should be at \" or >
                if (sbuf.eof()) report_error(getpos(), t);
                if ((ch == '>' and not is_system_header)
                    or (ch == '\"' and is_system_header))
                    report_error(getpos(), t);

                // indeed filename, try to open it.
                auto &&fmgr = resources::filemanager::instance();
                // next token will from new input.
                add_include(fmgr.open_header(filename, is_system_header), filename);
            }
            else report_error(getpos(), t);
        }

        auto lexer::register_macro(std::string_view       name,
                                   std::vector<token_t> &&args,
                                   std::vector<token_t> &&tokens)
            -> void
        {
            // todo
            // _macros: Str -> (List[token_t] -> List[token_t])
            // _macro: List[token_t] -> List[token_t]
            // _macro: parameters |-> expanded_tokens
        }

        auto lexer::add_include(std::istream &&context, std::string_view filename) -> void
        {
            _tokers.emplace(context);
            _filenames.emplace(filename);
        }
    } // namespace lexical
} // namespace simpc
