// file      : CCF/IDL2/Traversal/Sequence.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL2/Traversal/Sequence.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace Traversal
    {
      // SequenceDecl
      //
      //
      void SequenceDecl::
      traverse (NodePtr const& n)
      {
        if (!delegate (n))
        {
          pre (n);
          type (n);
          post (n);
        }
      }

      void SequenceDecl::
      pre (NodePtr const&)
      {
      }

      void SequenceDecl::
      type (NodePtr const& n)
      {
        if (type_ != 0) type_->dispatch (n->type ());
        else dispatch (n->type ());
      }

      void SequenceDecl::
      post (NodePtr const&)
      {
      }


      // UnboundedSequenceDecl
      //
      //
      void UnboundedSequenceDecl::
      traverse (NodePtr const& n)
      {
        if (!delegate (n))
        {
          pre (n);
          type (n);
          post (n);
        }
      }

      void UnboundedSequenceDecl::
      pre (NodePtr const&)
      {
      }

      void UnboundedSequenceDecl::
      type (NodePtr const& n)
      {
        if (type_ != 0) type_->dispatch (n->type ());
        else dispatch (n->type ());
      }

      void UnboundedSequenceDecl::
      post (NodePtr const&)
      {
      }
    }
  }
}
