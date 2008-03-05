// file      : CCF/IDL2/Traversal/Member.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_TRAVERSAL_MEMBER_HPP
#define CCF_IDL2_TRAVERSAL_MEMBER_HPP

#include "CCF/IDL2/Traversal/Elements.hpp"

#include "CCF/IDL2/SemanticGraph/Member.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace Traversal
    {
      struct Member : Node<SemanticGraph::Member>
      {
        virtual void
        traverse (Type&);

        virtual void
        pre (Type&);

        virtual void
        belongs (Type&, EdgeDispatcherBase&);

        virtual void
        belongs (Type&);

        virtual void
        name (Type&);

        virtual void
        post (Type&);
      };
    }
  }
}

#endif  // CCF_IDL2_TRAVERSAL_MEMBER_HPP
