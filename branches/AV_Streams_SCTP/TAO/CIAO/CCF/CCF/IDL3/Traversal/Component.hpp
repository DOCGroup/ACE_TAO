// file      : CCF/IDL3/Traversal/Component.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL3_TRAVERSAL_COMPONENT_HPP
#define CCF_IDL3_TRAVERSAL_COMPONENT_HPP

#include "CCF/IDL2/Traversal/Elements.hpp"
#include "CCF/IDL3/SyntaxTree/Component.hpp"

namespace CCF
{
  namespace IDL3
  {
    namespace Traversal
    {

      //
      //
      //
      struct ComponentDecl : IDL2::Traversal::Traverser
      {
        typedef
        SyntaxTree::ComponentDeclPtr
        NodePtr;

        ComponentDecl ()
        {
          map (typeid (SyntaxTree::ComponentDecl), this);
        }

        virtual void
        traverse (SyntaxTree::NodePtr const& n)
        {
          traverse (n->dynamic_type<SyntaxTree::ComponentDecl> ());
        }

        virtual void
        traverse (NodePtr const& n)
        {
          delegate (n);
        }
      };


      //
      //
      //
      struct ComponentDef : IDL2::Traversal::ScopeTraverser
      {
        typedef
        SyntaxTree::ComponentDefPtr
        NodePtr;

        ComponentDef ()
        {
          map (typeid (SyntaxTree::ComponentDef), this);
        }

        virtual void
        traverse (SyntaxTree::NodePtr const& n)
        {
          traverse (n->dynamic_type<SyntaxTree::ComponentDef> ());
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

#endif  // CCF_IDL3_TRAVERSAL_COMPONENT_HPP
