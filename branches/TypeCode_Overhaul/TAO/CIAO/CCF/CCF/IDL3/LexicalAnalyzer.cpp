// file      : CCF/IDL3/LexicalAnalyzer.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL3/LexicalAnalyzer.hpp"

namespace CCF
{
  namespace IDL3
  {
    LexicalAnalyzer::
    LexicalAnalyzer (CompilerElements::TokenStream<Char>& is)
        : IDL2::LexicalAnalyzer (is)
    {
      // Keywords (alphabetic order).

      keyword_table_.insert ("component");
      keyword_table_.insert ("consumes" );
      keyword_table_.insert ("emits"    );
      keyword_table_.insert ("eventtype");
      keyword_table_.insert ("home"     );
      keyword_table_.insert ("finder"   );
      keyword_table_.insert ("manages"  );
      keyword_table_.insert ("multiple" );
      keyword_table_.insert ("provides" );
      keyword_table_.insert ("publishes");
      keyword_table_.insert ("uses"     );
    }
  }
}
