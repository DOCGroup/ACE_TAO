// file      : CCF/IDL2/Traversal/Interface.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $id$

#ifndef CCF_IDL2_TRAVERSAL_INTERFACE_HPP
#define CCF_IDL2_TRAVERSAL_INTERFACE_HPP

#include "CCF/IDL2/Traversal/Elements.hpp"
#include "CCF/IDL2/SyntaxTree/Interface.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace Traversal
    {

      //
      //
      //
      struct InterfaceDecl : Traverser
      {
        typedef
        SyntaxTree::InterfaceDeclPtr
        NodePtr;

        InterfaceDecl ()
        {
          map (typeid (SyntaxTree::InterfaceDecl), this);
        }

        virtual void
        traverse (SyntaxTree::NodePtr const& n)
        {
          traverse (n->dynamic_type<SyntaxTree::InterfaceDecl> ());
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
      struct InterfaceDef : ScopeTraverser
      {
        typedef
        SyntaxTree::InterfaceDefPtr
        NodePtr;

        InterfaceDef ()
        {
          map (typeid (SyntaxTree::InterfaceDef), this);
        }

        virtual void
        traverse (SyntaxTree::NodePtr const& n)
        {
          traverse (n->dynamic_type<SyntaxTree::InterfaceDef> ());
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


      //
      //
      //
      struct AbstractInterfaceDecl : Traverser
      {
        typedef
        SyntaxTree::AbstractInterfaceDeclPtr
        NodePtr;

        AbstractInterfaceDecl ()
        {
          map (typeid (SyntaxTree::AbstractInterfaceDecl), this);
        }

        virtual void
        traverse (SyntaxTree::NodePtr const& n)
        {
          traverse (n->dynamic_type<SyntaxTree::AbstractInterfaceDecl> ());
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
      struct AbstractInterfaceDef : ScopeTraverser
      {
        typedef
        SyntaxTree::AbstractInterfaceDefPtr
        NodePtr;

        AbstractInterfaceDef ()
        {
          map (typeid (SyntaxTree::AbstractInterfaceDef), this);
        }

        virtual void
        traverse (SyntaxTree::NodePtr const& n)
        {
          traverse (n->dynamic_type<SyntaxTree::AbstractInterfaceDef> ());
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


      //
      //
      //
      struct LocalInterfaceDecl : Traverser
      {
        typedef
        SyntaxTree::LocalInterfaceDeclPtr
        NodePtr;

        LocalInterfaceDecl ()
        {
          map (typeid (SyntaxTree::LocalInterfaceDecl), this);
        }

        virtual void
        traverse (SyntaxTree::NodePtr const& n)
        {
          traverse (n->dynamic_type<SyntaxTree::LocalInterfaceDecl> ());
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
      struct LocalInterfaceDef : ScopeTraverser
      {
        typedef
        SyntaxTree::LocalInterfaceDefPtr
        NodePtr;

        LocalInterfaceDef ()
        {
          map (typeid (SyntaxTree::LocalInterfaceDef), this);
        }

        virtual void
        traverse (SyntaxTree::NodePtr const& n)
        {
          traverse (n->dynamic_type<SyntaxTree::LocalInterfaceDef> ());
        }

        virtual void
        traverse (NodePtr const& n);

        virtual void
        pre (NodePtr const&);

        virtual void
        scope (NodePtr const&);

        virtual void
        post (NodePtr const&);
      };


      //
      //
      //
      struct UnconstrainedInterfaceDecl : Traverser
      {
        typedef
        SyntaxTree::UnconstrainedInterfaceDeclPtr
        NodePtr;

        UnconstrainedInterfaceDecl ()
        {
          map (typeid (SyntaxTree::UnconstrainedInterfaceDecl), this);
        }

        virtual void
        traverse (SyntaxTree::NodePtr const& n)
        {
          traverse (n->dynamic_type<SyntaxTree::UnconstrainedInterfaceDecl> ());
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
      struct UnconstrainedInterfaceDef : ScopeTraverser
      {
        typedef
        SyntaxTree::UnconstrainedInterfaceDefPtr
        NodePtr;

        UnconstrainedInterfaceDef ()
        {
          map (typeid (SyntaxTree::UnconstrainedInterfaceDef), this);
        }

        virtual void
        traverse (SyntaxTree::NodePtr const& n)
        {
          traverse (n->dynamic_type<SyntaxTree::UnconstrainedInterfaceDef> ());
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

#endif  // CCF_IDL2_TRAVERSAL_INTERFACE_HPP
