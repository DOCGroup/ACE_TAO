// $Id$
#ifndef CCF_IDL2_LEXICAL_ANALYZER_HPP
#define CCF_IDL2_LEXICAL_ANALYZER_HPP

#include "MSVC_Pragmas.hpp"

#include "CCF/CompilerElements/Token.hpp"
#include "CCF/CompilerElements/TokenStream.hpp"

#include <map>
#include <deque>
#include <locale>
#include <cctype>
#include <string>
#include <istream>

namespace IDL2
{
  //@@ It seems to me that there should be a diagnistic information (file,
  //   line, start_pos, end_pos) supplied with each token. (Thus lexer
  //   will have to know about #include business. Or maybe not..?)
  //
  //@@ Would be nice to have a read-only symbol table (or at least keyword
  //   table). This way it would be easier to extend language (which from
  //   lexer's persective would ususally mean to add new keywords).

  class LexicalAnalyzer
  {
    //@@ fixme (should be at the end of class)
  protected:

    typedef CCF::TokenStream<char>::int_type int_type;
    typedef CCF::TokenStream<char>::char_type char_type;
    typedef CCF::TokenStream<char>::traits traits;

    typedef
    std::map<std::string, KeywordPtr>
    KeywordTable;

    typedef
    std::map<std::string, PunctuationPtr>
    PunctuationTable;


    std::locale loc_;
    KeywordTable keyword_table_;
    PunctuationTable punctuation_table_;
    CCF::TokenStream<char>& is_;

    // look ahead mechanism
    std::deque<int_type> buffer_;

    // line numbering mechanism
    bool after_nl;
    unsigned long line_;


  public:
    virtual
    ~LexicalAnalyzer () throw () {}

  public:
    class InternalError {};

    EndOfStreamPtr eos;

    // Keywords (alphabetic order)

    KeywordPtr abstract;
    KeywordPtr attribute;
    KeywordPtr factory;
    KeywordPtr in;
    KeywordPtr include;
    KeywordPtr inout;
    KeywordPtr interface;
    KeywordPtr local;
    KeywordPtr module;
    KeywordPtr out;
    KeywordPtr sinclude;
    KeywordPtr supports;

    // Punctuations (alphabetic group order)

    PunctuationPtr colon;
    PunctuationPtr comma;
    PunctuationPtr lbrace;
    PunctuationPtr rbrace;
    PunctuationPtr lparen;
    PunctuationPtr rparen;
    PunctuationPtr semi;

  public:
    LexicalAnalyzer (CCF::TokenStream<char>& is);

    virtual TokenPtr
    next ();

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
  };
}

#endif // CCF_IDL2_LEXICAL_ANALYZER_HPP
