// file      : CCF/IDL2/Traversal/TypeId.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_TRAVERSAL_TYPE_ID_HPP
#define CCF_IDL2_TRAVERSAL_TYPE_ID_HPP

#include "CCF/IDL2/Traversal/Elements.hpp"
#include "CCF/IDL2/SyntaxTree/TypeId.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace Traversal
    {
      //
      //
      //
      struct TypeId : Traverser
      {
        typedef
        SyntaxTree::TypeIdPtr
        NodePtr;

        TypeId ()
        {
          map (typeid (SyntaxTree::TypeId), this);
        }

        virtual void
        traverse (SyntaxTree::NodePtr const& n)
        {
          traverse (n->dynamic_type<SyntaxTree::TypeId> ());
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
      struct TypePrefix : Traverser
      {
        typedef
        SyntaxTree::TypePrefixPtr
        NodePtr;

        TypePrefix ()
        {
          map (typeid (SyntaxTree::TypePrefix), this);
        }

        virtual void
        traverse (SyntaxTree::NodePtr const& n)
        {
          traverse (n->dynamic_type<SyntaxTree::TypePrefix> ());
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

#endif  // CCF_IDL2_TRAVERSAL_TYPE_ID_HPP
