// file      : CCF/IDL2/Parsing/Elements.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_PARSING_ELEMENTS_HPP
#define CCF_IDL2_PARSING_ELEMENTS_HPP

#include <string>

#include <boost/spirit.hpp>

#include "CCF/IDL2/Token.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace Parsing
    {
      using namespace boost::spirit;

      typedef
      TokenList::iterator
      Iterator;

      typedef
      scanner<Iterator, scanner_policies<> >
      Scanner;

      typedef
      rule<Scanner>
      Rule;


      //
      //
      //
      template <typename Type>
      struct IdentityParser : public parser <IdentityParser<Type> >
      {
        typedef
        IdentityParser
        self_t;

        IdentityParser (std::string lexeme)
            : lexeme_ (lexeme)
        {
        }

        typename parser_result<self_t, Scanner>::type
        parse(Scanner const& scan) const
        {
          if (!scan.at_end())
          {
            TokenPtr t = *scan;

            if(ReferenceCounting::strict_cast<Type> (t) != 0 &&
               lexeme_ == t->lexeme ())
            {
              Iterator save(scan.first);
              ++scan;
              return scan.create_match(1, t, save, scan.first);
            }
          }
          return scan.no_match();
        }

      private:
        std::string lexeme_;
      };

      typedef
      IdentityParser<Keyword>
      KeywordParser;

      typedef
      IdentityParser<Punctuation>
      PunctuationParser;

      typedef
      IdentityParser<Operator>
      OperatorParser;

      //
      //
      //
      template <typename Type>
      struct TypeParser : public parser <TypeParser<Type> >
      {
        typedef
        TypeParser
        self_t;

        typename parser_result<self_t, Scanner>::type
        parse(Scanner const& scan) const
        {
          if (!scan.at_end())
          {
            TokenPtr t = *scan;

            if(ReferenceCounting::strict_cast<Type> (t) != 0)
            {
              Iterator save(scan.first);
              ++scan;
              return scan.create_match(1, t, save, scan.first);
            }
          }
          return scan.no_match();
        }
      };

      typedef
      TypeParser<EndOfStream>
      EndOfStreamParser;

      typedef
      TypeParser<Identifier>
      IdentifierParser;

      typedef
      TypeParser<SimpleIdentifier>
      SimpleIdentifierParser;

      typedef
      TypeParser<ScopedIdentifier>
      ScopedIdentifierParser;

      // Literal parsers.
      //

      typedef
      TypeParser<BooleanLiteral>
      BooleanLiteralParser;

      typedef
      TypeParser<CharacterLiteral>
      CharacterLiteralParser;

      typedef
      TypeParser<IntegerLiteral>
      IntegerLiteralParser;

      typedef
      TypeParser<StringLiteral>
      StringLiteralParser;

      //
      //
      //
      inline bool
      parse (Iterator const& first_,
             Iterator const& last,
             Rule const& rule)
      {
        Iterator first = first_;
        Scanner scan(first, last);
        match<nil_t> hit = rule.parse(scan);

        bool result = parse_info<Iterator>(
          first, hit, hit && (first == last), hit.length()).full;

        return result;
      }
    }
  }
}

#endif  // CCF_IDL2_PARSING_ELEMENTS_HPP
