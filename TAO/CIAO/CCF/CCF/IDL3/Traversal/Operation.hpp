// file      : CCF/IDL3/Traversal/Operation.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL3_TRAVERSAL_OPERATION_HPP
#define CCF_IDL3_TRAVERSAL_OPERATION_HPP

#include "CCF/IDL2/Traversal/Operation.hpp"
#include "CCF/IDL3/SyntaxTree/Operation.hpp"

namespace CCF
{
  namespace IDL3
  {
    namespace Traversal
    {
      //
      //
      //
      struct ProvidesDecl : IDL2::Traversal::Traverser
      {
        typedef
        SyntaxTree::ProvidesDeclPtr
        NodePtr;

        ProvidesDecl (Dispatcher* type = 0)
            : type_ (type)
        {
          map (typeid (SyntaxTree::ProvidesDecl), this);
        }

        virtual void
        traverse (SyntaxTree::NodePtr const& n)
        {
          traverse (n->dynamic_type<SyntaxTree::ProvidesDecl> ());
        }

        virtual void
        traverse (NodePtr const&);

        virtual void
        pre (NodePtr const&);

        virtual void
        type (NodePtr const&);

        virtual void
        post (NodePtr const&);

      private:
        Dispatcher* type_;
      };


      //
      //
      //
      struct UsesDecl : IDL2::Traversal::Traverser
      {
        typedef
        SyntaxTree::UsesDeclPtr
        NodePtr;

        UsesDecl (Dispatcher* type = 0)
            : type_ (type)
        {
          map (typeid (SyntaxTree::UsesDecl), this);
        }

        virtual void
        traverse (SyntaxTree::NodePtr const& n)
        {
          traverse (n->dynamic_type<SyntaxTree::UsesDecl> ());
        }

        virtual void
        traverse (NodePtr const&);

        virtual void
        pre (NodePtr const&);

        virtual void
        type (NodePtr const&);

        virtual void
        post (NodePtr const&);

      private:
        Dispatcher* type_;
      };


      //
      //
      //
      struct PublishesDecl : IDL2::Traversal::Traverser
      {
        typedef
        SyntaxTree::PublishesDeclPtr
        NodePtr;

        PublishesDecl (Dispatcher* type = 0)
            : type_ (type)
        {
          map (typeid (SyntaxTree::PublishesDecl), this);
        }

        virtual void
        traverse (SyntaxTree::NodePtr const& n)
        {
          traverse (n->dynamic_type<SyntaxTree::PublishesDecl> ());
        }

        virtual void
        traverse (NodePtr const&);

        virtual void
        pre (NodePtr const&);

        virtual void
        type (NodePtr const&);

        virtual void
        post (NodePtr const&);

      private:
        Dispatcher* type_;
      };


      //
      //
      //
      struct EmitsDecl : IDL2::Traversal::Traverser
      {
        typedef
        SyntaxTree::EmitsDeclPtr
        NodePtr;

        EmitsDecl (Dispatcher* type = 0)
            : type_ (type)
        {
          map (typeid (SyntaxTree::EmitsDecl), this);
        }

        virtual void
        traverse (SyntaxTree::NodePtr const& n)
        {
          traverse (n->dynamic_type<SyntaxTree::EmitsDecl> ());
        }

        virtual void
        traverse (NodePtr const&);

        virtual void
        pre (NodePtr const&);

        virtual void
        type (NodePtr const&);

        virtual void
        post (NodePtr const&);

      private:
        Dispatcher* type_;
      };


      //
      //
      //
      struct ConsumesDecl : IDL2::Traversal::Traverser
      {
        typedef
        SyntaxTree::ConsumesDeclPtr
        NodePtr;

        ConsumesDecl (Dispatcher* type = 0)
            : type_ (type)
        {
          map (typeid (SyntaxTree::ConsumesDecl), this);
        }

        virtual void
        traverse (SyntaxTree::NodePtr const& n)
        {
          traverse (n->dynamic_type<SyntaxTree::ConsumesDecl> ());
        }

        virtual void
        traverse (NodePtr const&);

        virtual void
        pre (NodePtr const&);

        virtual void
        type (NodePtr const&);

        virtual void
        post (NodePtr const&);

      private:
        Dispatcher* type_;
      };


      //
      //
      //
      struct HomeFactoryDecl : IDL2::Traversal::OperationTraverser
      {
        typedef
        SyntaxTree::HomeFactoryDeclPtr
        NodePtr;

        HomeFactoryDecl (Dispatcher* type = 0)
            : OperationTraverser (type)
        {
          map (typeid (SyntaxTree::HomeFactoryDecl), this);
        }

        virtual void
        traverse (SyntaxTree::NodePtr const& n)
        {
          traverse (n->dynamic_type<SyntaxTree::HomeFactoryDecl> ());
        }

        virtual void
        traverse (NodePtr const&);

        virtual void
        pre (NodePtr const&);

        virtual void
        type (NodePtr const&);

        virtual void
        name (NodePtr const&);

        virtual void
        parameters (NodePtr const&);

        virtual void
        post (NodePtr const&);

      private:
        Dispatcher* type_;
      };
    }
  }
}

#endif  // CCF_IDL3_TRAVERSAL_OPERATION_HPP
