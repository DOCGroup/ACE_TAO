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
          return traverse (n->dynamic_type<SyntaxTree::TypedefDecl> ());
        }

        virtual bool
        traverse (SyntaxTree::TypedefDeclPtr const& n)
        {
          typedef_decl = n;

          SyntaxTree::NodePtr vt (n->virtual_type ());

          return disp_.dispatch (vt);
        }

      protected:
        Dispatcher disp_;
        SyntaxTree::TypedefDeclPtr typedef_decl;
      };


      //
      //
      //
      template <typename T>
      struct TypedefDecl : virtual TypedefDeclBase
      {
        typedef
        SyntaxTree::StrictPtr<T>
        NodePtr;

        TypedefDecl ()
            : thunk_ (this, typedef_decl)
        {
          disp_.map (typeid (T), &thunk_);
        }

        virtual void
        traverse (SyntaxTree::TypedefDeclPtr const& td, NodePtr const& n)
        {
          delegate (td);
        }

      private:

        template <typename X>
        struct TypedefDeclThunk : Traverser
        {
          TypedefDeclThunk (TypedefDecl<X>* t,
                            SyntaxTree::TypedefDeclPtr const& d)
              : t_ (t),
                d_ (d)
          {
          }

          virtual bool
          traverse (SyntaxTree::NodePtr const& n)
          {
            //@@ gcc bug
            t_->traverse (d_, n->template dynamic_type<X> ());
            return true;
          }

        private:
          TypedefDecl<X>* t_;
          SyntaxTree::TypedefDeclPtr const& d_;
        };

        TypedefDeclThunk<T> thunk_;
      };
    }
  }
}

#endif  // CCF_IDL2_TRAVERSAL_TYPEDEF_HPP
