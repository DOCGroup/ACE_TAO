// file      : CCF/IDL2/Traversal/Struct.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_TRAVERSAL_STRUCT_HPP
#define CCF_IDL2_TRAVERSAL_STRUCT_HPP

#include "CCF/IDL2/Traversal/Elements.hpp"
#include "CCF/IDL2/SyntaxTree/Struct.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace Traversal
    {

      //
      //
      //
      struct StructDecl : Traverser
      {
        typedef
        SyntaxTree::StructDeclPtr
        NodePtr;

        StructDecl ()
        {
          map (typeid (SyntaxTree::StructDecl), this);
        }

        virtual bool
        traverse (SyntaxTree::NodePtr const& n)
        {
          traverse (n->dynamic_type<SyntaxTree::StructDecl> ());
	  return true;
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
      struct StructForwardDecl : Traverser
      {
        typedef
        SyntaxTree::StructForwardDeclPtr
        NodePtr;

        StructForwardDecl ()
        {
          map (typeid (SyntaxTree::StructForwardDecl), this);
        }

        virtual bool
        traverse (SyntaxTree::NodePtr const& n)
        {
          traverse (n->dynamic_type<SyntaxTree::StructForwardDecl> ());
	  return true;
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
      struct StructDef : ScopeTraverser
      {
        typedef
        SyntaxTree::StructDefPtr
        NodePtr;

        StructDef ()
        {
          map (typeid (SyntaxTree::StructDef), this);
        }

        virtual bool
        traverse (SyntaxTree::NodePtr const& n)
        {
          traverse (n->dynamic_type<SyntaxTree::StructDef> ());
          return true;
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

#endif  // CCF_IDL2_TRAVERSAL_STRUCT_HPP
