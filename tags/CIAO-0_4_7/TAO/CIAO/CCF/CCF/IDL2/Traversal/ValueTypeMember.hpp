// file      : CCF/IDL2/Traversal/ValueTypeMember.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_TRAVERSAL_VALUE_TYPE_MEMBER_HPP
#define CCF_IDL2_TRAVERSAL_VALUE_TYPE_MEMBER_HPP

#include "CCF/IDL2/Traversal/Elements.hpp"

#include "CCF/IDL2/SemanticGraph/ValueTypeMember.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace Traversal
    {
      template <typename T>
      struct ValueTypeMemberTemplate : Node<T>
      {
        virtual void
        traverse (T&);

        virtual void
        pre (T&);

        virtual void
        belongs (T&, EdgeDispatcherBase&);

        virtual void
        belongs (T&);

        virtual void
        name (T&);

        virtual void
        post (T&);

        using Node<T>::edge_traverser;
      };

      typedef
      ValueTypeMemberTemplate<SemanticGraph::ValueTypeMember>
      ValueTypeMember;

      typedef
      ValueTypeMemberTemplate<SemanticGraph::ValueTypePrivateMember>
      ValueTypePrivateMember;

      typedef
      ValueTypeMemberTemplate<SemanticGraph::ValueTypePublicMember>
      ValueTypePublicMember;
    }
  }
}

#include "CCF/IDL2/Traversal/ValueTypeMember.tpp"

#endif  // CCF_IDL2_TRAVERSAL_VALUE_TYPE_MEMBER_HPP
