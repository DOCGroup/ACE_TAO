// file      : CCF/IDL2/Traversal/Member.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL2/Traversal/Member.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace Traversal
    {
      void MemberDecl::
      traverse (NodePtr const& n)
      {
        if (!delegate (n))
        {
          pre (n);
          type (n);
          post (n);
        }
      }


      void MemberDecl::
      pre (NodePtr const&)
      {
      }

      void MemberDecl::
      type (NodePtr const& n)
      {
        if (type_ != 0) type_->dispatch (n->type ());
        else dispatch (n->type ());
      }

      void MemberDecl::
      post (NodePtr const&)
      {
      }
    }
  }
}
