// file      : CCF/IDL3/Traversal/Component.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL3/Traversal/Component.hpp"

namespace CCF
{
  namespace IDL3
  {
    namespace Traversal
    {
      // ComponentDecl
      //
      //


      // ComponentDef
      //
      //

      void ComponentDef::
      traverse (NodePtr const& n)
      {
        if (!delegate (n))
        {
          pre (n);
          scope (n);
          post (n);
        }
      }

      void ComponentDef::
      pre (NodePtr const&)
      {
      }

      void ComponentDef::
      scope (NodePtr const& n)
      {
        delegate_scope (n);
      }

      void ComponentDef::
      post (NodePtr const&)
      {
      }
    }
  }
}
