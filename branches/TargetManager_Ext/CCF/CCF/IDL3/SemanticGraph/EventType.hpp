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

        EventType () // For virtual inheritance only.
        {
          type_info (static_type_info ());
        }

        virtual
        ~EventType () = 0;
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

        AbstractEventType (Path const& path, unsigned long line)
            : Node (path, line), AbstractValueType (path, line)
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

        ConcreteEventType (Path const& path, unsigned long line)
            : Node (path, line), ConcreteValueType (path, line)
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

        EventTypeFactory (Path const& path, unsigned long line)
            : Node (path, line),
              TwoWayOperation (path, line),
              ValueTypeFactory (path, line)
        {
          type_info (static_type_info ());
        }
      };
    }
  }
}

#endif  // CCF_IDL3_SEMANTIC_GRAPH_EVENT_TYPE_HPP
