// file      : CCF/IDL2/Traversal/Interface.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL2/Traversal/Interface.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace Traversal
    {
      // InterfaceDecl
      //
      //


      // InterfaceDef
      //
      //

      void InterfaceDef::
      traverse (NodePtr const& n)
      {
        if (!delegate (n))
        {
          pre (n);
          scope (n);
          post (n);
        }
      }

      void InterfaceDef::
      pre (NodePtr const&)
      {
      }

      void InterfaceDef::
      scope (NodePtr const& n)
      {
        delegate_scope (n);
      }

      void InterfaceDef::
      post (NodePtr const&)
      {
      }

      // AbstractInterfaceDecl
      //
      //

      // AbstractInterfaceDef
      //
      //

      void AbstractInterfaceDef::
      traverse (NodePtr const& n)
      {
        if (!delegate (n))
        {
          pre (n);
          scope (n);
          post (n);
        }
      }

      void AbstractInterfaceDef::
      pre (NodePtr const&)
      {
      }

      void AbstractInterfaceDef::
      scope (NodePtr const& n)
      {
        delegate_scope (n);
      }

      void AbstractInterfaceDef::
      post (NodePtr const&)
      {
      }

      // LocalInterfaceDecl
      //
      //

      // LocalInterfaceDef
      //
      //

      void LocalInterfaceDef::
      traverse (NodePtr const& n)
      {
        if (!delegate (n))
        {
          pre (n);
          scope (n);
          post (n);
        }
      }

      void LocalInterfaceDef::
      pre (NodePtr const&)
      {
      }

      void LocalInterfaceDef::
      scope (NodePtr const& n)
      {
        delegate_scope (n);
      }

      void LocalInterfaceDef::
      post (NodePtr const&)
      {
      }

      // UnconstrainedInterfaceDecl
      //
      //

      // UnconstrainedInterfaceDef
      //
      //

      void UnconstrainedInterfaceDef::
      traverse (NodePtr const& n)
      {
        if (!delegate (n))
        {
          pre (n);
          scope (n);
          post (n);
        }
      }

      void UnconstrainedInterfaceDef::
      pre (NodePtr const&)
      {
      }

      void UnconstrainedInterfaceDef::
      scope (NodePtr const& n)
      {
        delegate_scope (n);
      }

      void UnconstrainedInterfaceDef::
      post (NodePtr const&)
      {
      }
    }
  }
}
