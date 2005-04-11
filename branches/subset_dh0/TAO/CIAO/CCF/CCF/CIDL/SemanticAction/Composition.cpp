// file      : CCF/CIDL/SemanticAction/Composition.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/CIDL/SemanticAction/Composition.hpp"

#include <ostream>

namespace CCF
{
  namespace CIDL
  {
    namespace SemanticAction
    {
      namespace
      {
        char* labels[] = {"entity", "process", "service", "session" };
      }

      std::ostream&
      operator<< (std::ostream& o, Composition::Category::Value v)
      {
        return o << labels[v];
      }
    }
  }
}
