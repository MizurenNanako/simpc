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
                auto ch = _input_stream.get();
                if (_input_stream.eof())
                {
                    _tokbuf.clear();
                    return false;
                }
                _tokbuf << ch;
                return true;
            };

        start:
            // retrive a char from stream
            if (!_eat()) return {token_type::eof, {}};
            auto tok = token_type();

            // Start the match tree
            // It will prove to be a mistake to hand write regex matching...
            switch (ch)
            {
            // todo
            }

        finish:
            return {tok, {}};
        finish_with_info:
            return {tok, _tokbuf.str()};
        } catch (std::exception &e)
        {
            std::cerr << std::format("Unknown Exception from {}, what(): {}",
                                     __func__, e.what())
                      << std::endl;
            return {token_type::err, {}};
        }
    } // namespace lexer
} // namespace simpc
