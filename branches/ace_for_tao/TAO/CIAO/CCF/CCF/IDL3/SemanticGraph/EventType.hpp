// file      : CCF/IDL3/SemanticGraph/EventType.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL3_SEMANTIC_GRAPH_EVENT_TYPE_HPP
#define CCF_IDL3_SEMANTIC_GRAPH_EVENT_TYPE_HPP

#include "CCF/IDL2/SemanticGraph/ValueType.hpp"
#include "CCF/IDL3/SemanticGraph/Elements.hpp"

namespace CCF
{
  namespace IDL3
  {
    namespace SemanticGraph
    {
      //
      //
      //
      class EventType : public virtual ValueType
      {
      public:
        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        EventType ()
        {
          type_info (static_type_info ());
        }
      };


      //
      //
      //
      class AbstractEventType : public virtual EventType,
                                public virtual AbstractValueType
      {
      public:
        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        AbstractEventType ()
        {
          type_info (static_type_info ());
        }
      };


      //
      //
      //
      class ConcreteEventType : public virtual EventType,
                                public virtual ConcreteValueType
      {
      public:
        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        ConcreteEventType ()
        {
          type_info (static_type_info ());
        }
      };

      //
      //
      //
      class EventTypeFactory : public virtual ValueTypeFactory
      {
      public:
        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        EventTypeFactory ()
        {
          type_info (static_type_info ());
        }
      };
    }
  }
}

#endif  // CCF_IDL3_SEMANTIC_GRAPH_EVENT_TYPE_HPP
