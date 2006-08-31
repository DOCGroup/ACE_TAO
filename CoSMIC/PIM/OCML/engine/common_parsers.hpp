#ifndef COMMON_PARSERS_HPP
#define COMMON_PARSERS_HPP

#include <boost/spirit.hpp>
#include <boost/spirit/phoenix.hpp>

#include <string>

namespace common
{

  struct Word_Closure
    : public boost::spirit::closure<Word_Closure, std::string>
  {
    member1 value;
  };

  struct Word_Grammar
    : public boost::spirit::grammar<Word_Grammar, Word_Closure::context_t >
  {
    template <typename ScannerT>
    struct definition
    {
      definition(Word_Grammar const& self)
      {
        using namespace boost::spirit;
        using namespace phoenix;

        word = lexeme_d[
           ((alpha_p >> +(alnum_p | '_'))
           | (alpha_p | '_'))
            [
              self.value = construct_<std::string>(arg1, arg2)
            ]
          ]
          ;

        BOOST_SPIRIT_DEBUG_RULE(word);
      }

      boost::spirit::rule<ScannerT> word;

      const boost::spirit::rule<ScannerT>& start() const { return word; }
    };
  };

} // namespace common

#endif // WORD_HPP

