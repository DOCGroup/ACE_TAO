// file      : CCF/CIDL/LexicalAnalyzer.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $id$

#ifndef CCF_CIDL_LEXICAL_ANALYZER_HPP
#define CCF_CIDL_LEXICAL_ANALYZER_HPP

#include "CCF/IDL3/LexicalAnalyzer.hpp"

namespace CCF
{
  namespace CIDL
  {
    class LexicalAnalyzer : public virtual IDL3::LexicalAnalyzer
    {
    public:
      LexicalAnalyzer (TokenStream<char>& is);
    };
  }
}

#endif  // CCF_CIDL_LEXICAL_ANALYZER_HPP
