// file      : CCF/IDL3/Traversal/Home.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL3/Traversal/Home.hpp"

namespace CCF
{
  namespace IDL3
  {
    namespace Traversal
    {
      // HomeDef
      //
      //
      void HomeDef::
      traverse (NodePtr const& n)
      {
        if (!delegate (n))
        {
          pre (n);
          scope (n);
          post (n);
        }
      }

      void HomeDef::
      pre (NodePtr const&)
      {
      }

      void HomeDef::
      scope (NodePtr const& n)
      {
        delegate_scope (n);
      }

      void HomeDef::
      post (NodePtr const&)
      {
      }
    }
  }
}
