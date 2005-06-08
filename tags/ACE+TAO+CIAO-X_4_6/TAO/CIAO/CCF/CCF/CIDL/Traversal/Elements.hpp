// file      : CCF/CIDL/Traversal/Elements.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_CIDL_TRAVERSAL_ELEMENTS_HPP
#define CCF_CIDL_TRAVERSAL_ELEMENTS_HPP

#include "CCF/IDL3/Traversal/Elements.hpp"

#include "CCF/CIDL/SemanticGraph/Elements.hpp"

namespace CCF
{
  namespace CIDL
  {
    namespace Traversal
    {
      using namespace IDL3::Traversal;

      //
      //
      //
      struct Implements : Edge<SemanticGraph::Implements>
      {
        virtual void
        traverse (Type& e)
        {
          node_traverser ().traverse (e.implementee ());
        }
      };

    }
  }
}

#endif  // CCF_CIDL_TRAVERSAL_ELEMENTS_HPP
