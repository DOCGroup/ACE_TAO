// file      : CCF/CIDL/LexicalAnalyzer.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/CIDL/LexicalAnalyzer.hpp"

namespace CCF
{
  namespace CIDL
  {
    LexicalAnalyzer::
    LexicalAnalyzer (CompilerElements::TokenStream<Char>& is)
        : IDL2::LexicalAnalyzer (is),
          IDL3::LexicalAnalyzer (is)
    {
      // Keywords (alphabetic order).

      keyword_table_.insert ("composition");
      keyword_table_.insert ("entity"     );
      keyword_table_.insert ("executor"   );
      keyword_table_.insert ("implements" );
      keyword_table_.insert ("process"    );
      keyword_table_.insert ("service"    );
      keyword_table_.insert ("session"    );
    }
  }
}
