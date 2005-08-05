// file      : CCF/IDL2/SemanticGraph/Literals.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL2/SemanticGraph/Literals.hpp"

#include <ostream>

namespace CCF
{
  namespace IDL2
  {
    namespace SemanticGraph
    {
      // StringLiteral
      //
      //
      std::ostream&
      operator<< (std::ostream& o, StringLiteral const& lit)
      {
        return o << '\"' << lit.literal () << '\"';
      }
    }
  }
}
