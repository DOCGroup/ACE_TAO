// file      : CCF/IDL2/LexicalAnalyzer.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_LEXICAL_ANALYZER_HPP
#define CCF_IDL2_LEXICAL_ANALYZER_HPP

#include <set>
#include <map>
#include <deque>
#include <locale>
#include <string>

#include "CCF/CompilerElements/TokenStream.hpp"
#include "CCF/CompilerElements/PreprocessorToken.hpp"
#include "CCF/IDL2/Token.hpp"

namespace CCF
{
  namespace IDL2
  {
    //@@ Call to get() after eof is illegal.
    //
    //

    class LexicalAnalyzer : public CompilerElements::TokenStream<TokenPtr>
    {
    public:
      virtual
      ~LexicalAnalyzer () throw () {}

    public:

      typedef
      CompilerElements::CPP::Token
      Char;

      typedef
      std::deque<Char>
      CharBuffer;

      LexicalAnalyzer (CompilerElements::TokenStream<Char>& is);

      virtual TokenPtr
      next ();

    protected:
      virtual Char
      skip_space (Char c);

      virtual TokenPtr
      identifier (Char c);

      virtual bool
      punctuation (Char c, TokenPtr& token);

      virtual bool
      operator_ (Char c, TokenPtr& token);

      virtual bool
      character_literal (Char c, TokenPtr& token);

      virtual bool
      string_literal (Char c, TokenPtr& token);

      virtual std::string
      string_literal_trailer ();

      virtual bool
      integer_literal (Char c, TokenPtr& token);

      // Literal scanners.
      //
      //
      class Format {};
      class Boundary {};

      std::pair<char, std::size_t>
      scan_char (char const* s);

      std::string
      scan_string (std::string const& s);

      unsigned long long
      scan_integer (std::string const& s, unsigned short base);

    protected:
      virtual Char
      get ();

      virtual Char
      peek ();

      virtual Char
      peek_more ();

      virtual void
      ret (Char const& c);

    protected:
      // Character utility functions.
      //
      bool
      is_alpha (char c) const
      {
        return std::isalpha (c, loc_);
      }

      bool
      is_oct_digit (char c) const
      {
        return std::isdigit (c, loc_) && c != '8' && c != '9';
      }

      bool
      is_dec_digit (char c) const
      {
        return std::isdigit (c, loc_);
      }

      bool
      is_hex_digit (char c) const
      {
        return std::isxdigit (c, loc_);
      }

      bool
      is_alnum (char c) const
      {
        return std::isalnum (c, loc_);
      }

      bool
      is_space (char c) const
      {
        return std::isspace (c, loc_);
      }

      bool
      is_eos (Char const& c) const
      {
        return c == Char::eos;
      }

      char
      to_upper (char c) const
      {
        return std::toupper (c, loc_);
      }

    protected:
      typedef
      std::set<std::string>
      KeywordTable;

      struct IdentifierTreeNode
      {
        typedef
        std::map<std::string, IdentifierTreeNode>
        PrefixMap;

        IdentifierTreeNode&
        operator[] (char const* key)
        {
          return map_[key];
        }

        PrefixMap map_;
      };

      typedef
      std::set<std::string>
      PunctuationTable;

      typedef
      std::set<std::string>
      OperatorTable;

    protected:
      bool
      read_simple_identifier (std::string& lexeme, CharBuffer& buf);

      bool
      traverse_identifier_tree (std::string& lexeme,
                                IdentifierTreeNode const& node);

    protected:
      std::locale loc_;

      CompilerElements::TokenStream<Char>& is_;

      CharBuffer ibuffer_;

      KeywordTable keyword_table_;
      IdentifierTreeNode identifier_tree_;
      PunctuationTable punctuation_table_;
      OperatorTable operator_table_;
    };
  }
}

#endif  // CCF_IDL2_LEXICAL_ANALYZER_HPP
