// file      : CCF/IDL2/LexicalAnalyzer.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_LEXICAL_ANALYZER_HPP
#define CCF_IDL2_LEXICAL_ANALYZER_HPP

#include <set>
#include <map>
#include <deque>
#include <locale>
#include <cctype>
#include <string>
#include <istream>

#include "CCF/CompilerElements/TokenStream.hpp"

#include "CCF/IDL2/Token.hpp"

namespace CCF
{
  namespace IDL2
  {
    //@@ Call to get() after eof is illegal.
    //
    //

    class LexicalAnalyzer : public TokenStream<TokenPtr>
    {
    public:
      virtual
      ~LexicalAnalyzer () throw () {}

    public:
      LexicalAnalyzer (TokenStream<char>& is);

      virtual TokenPtr
      next ();

    protected:
      typedef
      TokenStream<char>::int_type
      int_type;

      typedef
      TokenStream<char>::char_type
      char_type;

      typedef
      TokenStream<char>::traits
      traits;

      struct Char
      {
        Char (int_type c, unsigned long line)
            : c_ (c), line_ (line)
        {
        }

      public:
        bool
        is_eof () const
        {
          return !traits::not_eof (c_);
        }

        bool
        is_alpha (std::locale const& l) const
        {
          return std::isalpha (char_ (), l);
        }

        bool
        is_alnum (std::locale const& l) const
        {
          return std::isalnum (char_ (), l);
        }

        bool
        is_space (std::locale const& l) const
        {
          return std::isspace (char_ (), l);
        }

      public:
        char_type
        char_ () const
        {
          return traits::to_char_type (c_);
        }

        unsigned long
        line () const
        {
          return line_;
        }

      public:
        friend bool
        operator== (Char const& a, char b)
        {
          return !a.is_eof () && a.char_ () == b;
        }

        friend bool
        operator== (char a, Char const& b)
        {
          return b == a;
        }

        friend bool
        operator!= (Char const& a, char b)
        {
          return a.is_eof () || a.char_ () != b;
        }

        friend bool
        operator!= (char a, Char const& b)
        {
          return b != a;
        }

      private:
        int_type c_;
        unsigned long line_;
      };


    protected:
      virtual Char
      get ();

      virtual Char
      peek ();

      virtual Char
      peek_more ();

      virtual void
      ret (Char const& c);

      /*
      char_type
      to_char_type (int_type i);
      */

      virtual void
      cxx_comment (Char c);

      virtual void
      c_comment (Char c);

      virtual Char
      skip_space (Char c);

      virtual TokenPtr
      identifier (Char c);

      virtual bool
      string_literal (Char c, TokenPtr& token);

      virtual bool
      punctuation (Char c, TokenPtr& token);

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
      std::deque<Char>
      CharBuffer;

    protected:

      Char
      get_from_stream ();

      bool
      read_simple_identifier (std::string& lexeme, CharBuffer& buf);

      bool
      traverse_identifier_tree (std::string& lexeme,
                                IdentifierTreeNode const& node);

    protected:
      std::locale loc_;

      TokenStream<char>& is_;

      KeywordTable keyword_table_;
      IdentifierTreeNode identifier_tree_;
      PunctuationTable punctuation_table_;

      // line numbering mechanism
      bool after_nl;
      unsigned long line_;

      // look ahead mechanism

      CharBuffer buffer_;
    };
  }
}

#endif  // CCF_IDL2_LEXICAL_ANALYZER_HPP
