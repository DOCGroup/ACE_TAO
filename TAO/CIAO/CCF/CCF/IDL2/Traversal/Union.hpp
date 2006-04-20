// file      : CCF/IDL2/Traversal/Union.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_TRAVERSAL_UNION_HPP
#define CCF_IDL2_TRAVERSAL_UNION_HPP

#include "CCF/IDL2/Traversal/Elements.hpp"
#include "CCF/IDL2/SemanticGraph/Union.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace Traversal
    {
      //
      //
      //
      struct UnionMember : Node<SemanticGraph::UnionMember>
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


      //
      //
      //
      struct Union : ScopeTemplate<SemanticGraph::Union>
      {
        virtual void
        traverse (Type&);

        virtual void
        pre (Type&);

        virtual void
        name (Type&);

        virtual void
        post (Type&);
      };
    }
  }
}

#endif  // CCF_IDL2_TRAVERSAL_UNION_HPP
