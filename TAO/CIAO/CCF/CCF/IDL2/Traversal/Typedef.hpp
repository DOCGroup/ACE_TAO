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
      //
      //
      //
      template <typename T>
      struct TypedefDecl : Traverser
      {
        typedef
        SyntaxTree::TypedefDeclPtr
        NodePtr;

        TypedefDecl ()
        {
          map (typeid (SyntaxTree::TypedefDecl), this);
        }

        virtual bool
        traverse (SyntaxTree::NodePtr const& n)
        {
          //@@ gcc bug
          if (n->template is_a<T> ())
          {
            traverse (n);
            return true;
          }
          else
          {
            return false;
          }
        }

        virtual void
        traverse (NodePtr const& n)
        {
          delegate (n);
        }
      };
    }
  }
}

#endif  // CCF_IDL2_TRAVERSAL_TYPEDEF_HPP
