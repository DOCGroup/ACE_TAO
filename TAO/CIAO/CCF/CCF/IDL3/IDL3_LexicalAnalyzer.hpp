// $Id$
#ifndef CCF_IDL3_LEXICAL_ANALYZER_HPP
#define CCF_IDL3_LEXICAL_ANALYZER_HPP

#include "MSVC_Pragmas.hpp"

#include "CCF/IDL2/IDL2_LexicalAnalyzer.hpp"

#include <string>

namespace IDL3
{
  class LexicalAnalyzer : public virtual IDL2::LexicalAnalyzer
  {
  public:
    virtual
    ~LexicalAnalyzer () throw () {}

  public:

    // Keywords (alphabetic order).

    KeywordPtr component;
    KeywordPtr consumes;
    KeywordPtr emits;
    KeywordPtr eventtype;
    KeywordPtr home;
    KeywordPtr manages;
    KeywordPtr provides;
    KeywordPtr publishes;
    KeywordPtr uses;

    LexicalAnalyzer (CCF::TokenStream<char>& is)
        : IDL2::LexicalAnalyzer (is),

          component (new Keyword),
          consumes  (new Keyword),
          emits     (new Keyword),
          eventtype (new Keyword),
          home      (new Keyword),
          manages   (new Keyword),
          provides  (new Keyword),
          publishes (new Keyword),
          uses      (new Keyword)
    {
      keyword_table_["component"] = component;
      keyword_table_["consumes" ] = consumes;
      keyword_table_["emits"    ] = emits;
      keyword_table_["eventtype"] = eventtype;
      keyword_table_["home"     ] = home;
      keyword_table_["manages"  ] = manages;
      keyword_table_["provides" ] = provides;
      keyword_table_["publishes"] = publishes;
      keyword_table_["uses"     ] = uses;
    }
  };
}

#endif // CCF_IDL3_LEXICAL_ANALYZER_HPP
