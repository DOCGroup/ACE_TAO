// file      : CCF/IDL2/Traversal/Member.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_TRAVERSAL_MEMBER_HPP
#define CCF_IDL2_TRAVERSAL_MEMBER_HPP

#include "CCF/IDL2/Traversal/Elements.hpp"
#include "CCF/IDL2/SyntaxTree/Member.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace Traversal
    {
      struct MemberDecl : Traverser
      {
        typedef
        SyntaxTree::MemberDeclPtr
        NodePtr;

        MemberDecl (Dispatcher* type = 0)
            : type_ (type)
        {
          map (typeid (SyntaxTree::MemberDecl), this);
        }

        virtual bool
        traverse (SyntaxTree::NodePtr const& n)
        {
          traverse (n->dynamic_type<SyntaxTree::MemberDecl> ());
	  return true;
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
    }
  }
}

#endif  // CCF_IDL2_TRAVERSAL_MEMBER_HPP
