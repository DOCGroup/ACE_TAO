// file      : CCF/IDL2/Traversal/Struct.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL2/Traversal/Struct.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace Traversal
    {
      // StructDef
      //
      //

      void StructDef::
      traverse (NodePtr const& n)
      {
        if (!delegate (n))
        {
          pre (n);
          scope (n);
          post (n);
        }
      }

      void StructDef::
      pre (NodePtr const&)
      {
      }

      void StructDef::
      scope (NodePtr const& n)
      {
        delegate_scope (n);
      }

      void StructDef::
      post (NodePtr const&)
      {
      }
    }
  }
}
