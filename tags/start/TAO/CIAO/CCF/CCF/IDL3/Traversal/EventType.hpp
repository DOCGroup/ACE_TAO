// file      : CCF/IDL3/Traversal/EventType.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $id$

#ifndef CCF_IDL3_TRAVERSAL_EVENT_TYPE_HPP
#define CCF_IDL3_TRAVERSAL_EVENT_TYPE_HPP

#include "CCF/IDL2/Traversal/Elements.hpp"
#include "CCF/IDL3/SyntaxTree/EventType.hpp"

namespace CCF
{
  namespace IDL3
  {
    namespace Traversal
    {
      //
      //
      //
      struct ConcreteEventTypeDef : IDL2::Traversal::ScopeTraverser
      {
        typedef
        SyntaxTree::ConcreteEventTypeDefPtr
        NodePtr;

        ConcreteEventTypeDef ()
        {
          map (typeid (SyntaxTree::ConcreteEventTypeDef), this);
        }

        virtual void
        traverse (SyntaxTree::NodePtr const& n)
        {
          traverse (n->dynamic_type<SyntaxTree::ConcreteEventTypeDef> ());
        }

        virtual void
        traverse (NodePtr const&);

        virtual void
        pre (NodePtr const&);

        virtual void
        scope (NodePtr const&);

        virtual void
        post (NodePtr const&);
      };
    }
  }
}

#endif  // CCF_IDL3_TRAVERSAL_EVENT_TYPE_HPP
