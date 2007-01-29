// file      : CCF/IDL2/Traversal/Translation.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL2/Traversal/Translation.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace Traversal
    {
      // TranslationRegion
      //
      //
      void TranslationRegion::
      traverse (SemanticGraph::TranslationRegion& r)
      {
        iterate_and_traverse (r.contains_begin (),
                              r.contains_end (),
                              edge_traverser ());
      }


      // TranslationUnit
      //
      //
      void TranslationUnit::
      traverse (SemanticGraph::TranslationUnit& u)
      {
        iterate_and_traverse (u.contains_begin (),
                              u.contains_end (),
                              edge_traverser ());
      }
    }
  }
}
