// file      : CCF/IDL2/LexicalAnalyzer.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_LEXICAL_ANALYZER_HPP
#define CCF_IDL2_LEXICAL_ANALYZER_HPP

#include <set>
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

    protected:
      virtual int_type
      get ();

      virtual int_type
      peek ();

      virtual int_type
      peek_more ();

      char_type
      to_char_type (int_type i);

      virtual void
      cxx_comment (char_type c);

      virtual void
      c_comment (char_type c);

      virtual TokenPtr
      identifier (char_type c);

      virtual bool
      string_literal (char_type c, TokenPtr& token);

      virtual bool
      punctuation (char_type c, TokenPtr& token);

    protected:
      typedef
      std::set<std::string>
      KeywordTable;

      typedef
      std::set<std::string>
      PunctuationTable;

      std::locale loc_;

      TokenStream<char>& is_;

      KeywordTable keyword_table_;
      PunctuationTable punctuation_table_;

      // line numbering mechanism
      bool after_nl;
      unsigned long line_;

      // look ahead mechanism
      std::deque<int_type> buffer_;
    };
  }
}

#endif  // CCF_IDL2_LEXICAL_ANALYZER_HPP
