// $Id$
#ifndef CCF_CIDL_LEXICAL_ANALYZER_HPP
#define CCF_CIDL_LEXICAL_ANALYZER_HPP

#include "MSVC_Pragmas.hpp"

#include "CCF/IDL3/IDL3_LexicalAnalyzer.hpp"

#include <string>

namespace CIDL
{
  class LexicalAnalyzer : public virtual IDL3::LexicalAnalyzer
  {
  public:
    virtual
    ~LexicalAnalyzer () throw () {}

  public:

    // Keywords (alphabetic order).

    KeywordPtr composition;
    KeywordPtr entity;
    KeywordPtr executor;
    KeywordPtr implements;
    KeywordPtr process;
    KeywordPtr service;
    KeywordPtr session;



    LexicalAnalyzer (CCF::TokenStream<char>& is)
        : IDL2::LexicalAnalyzer (is),
          IDL3::LexicalAnalyzer (is),

          composition (new Keyword),
          entity      (new Keyword),
          executor    (new Keyword),
          implements  (new Keyword),
          process     (new Keyword),
          service     (new Keyword),
          session     (new Keyword)
    {

      keyword_table_["composition"] = composition;
      keyword_table_["entity"     ] = entity;
      keyword_table_["executor"   ] = executor;
      keyword_table_["implements" ] = implements;
      keyword_table_["process"    ] = process;
      keyword_table_["service"    ] = service;
      keyword_table_["session"    ] = session;
    }
  };
}

#endif // CCF_CIDL_LEXICAL_ANALYZER_HPP
