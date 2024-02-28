#include "lexer.hh"

namespace simpc
{
    namespace lexer
    {
        tokenizer::tokenizer(std::istream &input)
            : _input_stream{input}
        {
        }

        auto tokenizer::get_token() -> token_t
        try
        {
            // Pre-Define
            auto ch   = char_type();
            auto _eat = [&ch, this] {
                ch = _input_stream.get();
                if (_input_stream.eof())
                {
                    _tokbuf.clear();
                    return false;
                }
                _tokbuf.push_back(ch);
                return true;
            };
            auto _vomit = [&ch, this] {
                _input_stream.unget();
                ch = _tokbuf.back();
                _tokbuf.pop_back();
            };

        start:
            // retrive a char from stream
            if (!_eat()) return {token_type::eof, {}};
            auto tok = token_type();

            // match keywords or identifier
            // todo: This will be a very impressive state machine
#include "lexer.ccpart"

        finish:
            return {tok, {}};
        finish_with_info:
            return {tok, _tokbuf};
        } catch (std::exception &e)
        {
            std::cerr << std::format("Unknown Exception from {}, what(): {}",
                                     __func__, e.what())
                      << std::endl;
            return {token_type::err, {}};
        }
    } // namespace lexer
} // namespace simpc
