// file      : CCF/IDL3/Traversal/EventType.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $id$

#include "CCF/IDL3/Traversal/EventType.hpp"

namespace CCF
{
  namespace IDL3
  {
    namespace Traversal
    {
      // ConcreteEventTypeDef
      //
      //

      void ConcreteEventTypeDef::
      traverse (NodePtr const& n)
      {
        if (!delegate (n))
        {
          pre (n);
          scope (n);
          post (n);
        }
      }

      void ConcreteEventTypeDef::
      pre (NodePtr const&)
      {
      }

      void ConcreteEventTypeDef::
      scope (NodePtr const& n)
      {
        delegate_scope (n);
      }

      void ConcreteEventTypeDef::
      post (NodePtr const&)
      {
      }
    }
  }
}
