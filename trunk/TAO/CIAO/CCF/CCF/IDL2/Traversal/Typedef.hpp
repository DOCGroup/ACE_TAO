// file      : CCF/IDL2/Traversal/Typedef.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_TRAVERSAL_TYPEDEF_HPP
#define CCF_IDL2_TRAVERSAL_TYPEDEF_HPP

#include "CCF/IDL2/Traversal/Elements.hpp"
#include "CCF/IDL2/SyntaxTree/Typedef.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace Traversal
    {
      struct TypedefDeclBase : Traverser
      {
      protected:
        TypedefDeclBase ()
        {
          map (typeid (SyntaxTree::TypedefDecl), this);
        }

        virtual bool
        traverse (SyntaxTree::NodePtr const& n)
        {
          traverse (n->dynamic_type<SyntaxTree::TypedefDecl> ());
          return true;
        }

        virtual void
        traverse (SyntaxTree::TypedefDeclPtr const& n)
        {
          typedef_decl = n;

          SyntaxTree::NodePtr vt (n->virtual_type ());

          disp_.dispatch (vt);
        }

      protected:
        Dispatcher disp_;
        SyntaxTree::TypedefDeclPtr typedef_decl;
      };


      //
      //
      //
      template <typename T>
      struct TypedefDecl : virtual TypedefDeclBase, Traverser
      {
        typedef
        SyntaxTree::StrictPtr<T>
        NodePtr;

        TypedefDecl ()
        {
          disp_.map (typeid (T), this);
        }

        virtual bool
        traverse (SyntaxTree::NodePtr const& n)
        {
          //@@ gcc bug
          traverse (typedef_decl, n->template dynamic_type<T> ());
          return true;
        }

        virtual void
        traverse (SyntaxTree::TypedefDeclPtr const& td, NodePtr const& n)
        {
          delegate (n);
        }
      };
    }
  }
}

#endif  // CCF_IDL2_TRAVERSAL_TYPEDEF_HPP
