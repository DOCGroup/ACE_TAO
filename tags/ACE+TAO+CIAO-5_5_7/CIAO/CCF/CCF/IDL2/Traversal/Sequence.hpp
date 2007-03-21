// file      : CCF/IDL2/Traversal/Sequence.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_TRAVERSAL_SEQUENCE_HPP
#define CCF_IDL2_TRAVERSAL_SEQUENCE_HPP

#include "CCF/IDL2/Traversal/Elements.hpp"

#include "CCF/IDL2/SemanticGraph/Sequence.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace Traversal
    {
      struct UnboundedSequence : Node<SemanticGraph::UnboundedSequence>
      {
        virtual void
        traverse (Type&);

        virtual void
        pre (Type&);

        virtual void
        arguments_with_type (Type&, EdgeDispatcherBase&);

        virtual void
        arguments_with_type (Type&);

        virtual void
        name (Type&);

        virtual void
        post (Type&);
      };

      struct BoundedSequence : Node<SemanticGraph::BoundedSequence>
      {
        virtual void
        traverse (Type&);

        virtual void
        pre (Type&);

        virtual void
        arguments_with_type (Type&, EdgeDispatcherBase&);

        virtual void
        arguments_with_type (Type&);

        virtual void
        arguments_with_value (Type&, EdgeDispatcherBase&);

        virtual void
        arguments_with_value (Type&);

        virtual void
        name (Type&);

        virtual void
        post (Type&);
      };
    }
  }
}

#endif  // CCF_IDL2_TRAVERSAL_SEQUENCE_HPP
