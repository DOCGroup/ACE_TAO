// file      : CCF/IDL3/Traversal/EventType.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL3_TRAVERSAL_EVENT_TYPE_HPP
#define CCF_IDL3_TRAVERSAL_EVENT_TYPE_HPP

#include "CCF/IDL3/SemanticGraph/EventType.hpp"

#include "CCF/IDL2/Traversal/Operation.hpp"
#include "CCF/IDL3/Traversal/Elements.hpp"

namespace CCF
{
  namespace IDL3
  {
    namespace Traversal
    {
      template <typename T>
      struct EventTypeTemplate : ScopeTemplate<T>
      {
        virtual void
        traverse (T&);

        virtual void
        pre (T&);

        virtual void
        name (T&);

        virtual void
        inherits (T&, EdgeDispatcherBase&);

        virtual void
        inherits (T&);

        virtual void
        inherits_pre (T&);

        virtual void
        inherits_post (T&);

        virtual void
        inherits_none (T&);

        virtual void
        supports (T&, EdgeDispatcherBase&);

        virtual void
        supports (T&);

        virtual void
        supports_pre (T&);

        virtual void
        supports_post (T&);

        virtual void
        supports_none (T&);

        virtual void
        post (T&);

        virtual void
        comma (T&);
      };


      typedef
      EventTypeTemplate<SemanticGraph::EventType>
      EventType;

      typedef
      EventTypeTemplate<SemanticGraph::AbstractEventType>
      AbstractEventType;

      typedef
      EventTypeTemplate<SemanticGraph::ConcreteEventType>
      ConcreteEventType;

      typedef
      OperationTemplate<SemanticGraph::EventTypeFactory>
      EventTypeFactory;
    }
  }
}

#include "CCF/IDL3/Traversal/EventType.tpp"

#endif  // CCF_IDL3_TRAVERSAL_EVENT_TYPE_HPP
