// file      : CCF/CIDL/Traversal/Composition.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/CIDL/Traversal/Composition.hpp"

namespace CCF
{
  namespace CIDL
  {
    namespace Traversal
    {
      // Composition
      //
      //
      void Composition::
      traverse (NodePtr const& n)
      {
        if (!delegate (n))
        {
          pre (n);
          scope (n);
          post (n);
        }
      }

      void Composition::
      pre (NodePtr const&)
      {
      }

      void Composition::
      scope (NodePtr const& n)
      {
        delegate_scope (n);
      }

      void Composition::
      post (NodePtr const&)
      {
      }
    }
  }
}
