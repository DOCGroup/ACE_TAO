// file      : CCF/IDL2/SemanticGraph/ValueType.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_SEMANTIC_GRAPH_VALUE_TYPE_HPP
#define CCF_IDL2_SEMANTIC_GRAPH_VALUE_TYPE_HPP

#include "CCF/IDL2/SemanticGraph/Elements.hpp"
#include "CCF/IDL2/SemanticGraph/Operation.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace SemanticGraph
    {
      //
      //
      //
      class ValueType : public virtual Type,
                        public virtual Scope
      {
      public:
        virtual bool
        complete () const
        {
          return true;
        }

        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        ValueType ()
        {
          type_info (static_type_info ());
        }

        using Type::add_edge_right;
        using Scope::add_edge_left;
      };


      //
      //
      //
      class AbstractValueType : public virtual ValueType
      {
      public:
        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        AbstractValueType ()
        {
          type_info (static_type_info ());
        }
      };

      // @@ TODO: value type member
      // @@ TODO: value type factory
      //

      //
      //
      //
      class ConcreteValueType : public virtual ValueType
      {
      public:
        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        ConcreteValueType ()
        {
          type_info (static_type_info ());
        }
      };


      //
      //
      //
      class ValueTypeFactory : public virtual TwoWayOperation
      {
      public:
        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        ValueTypeFactory ()
        {
          type_info (static_type_info ());
        }
      };
    }
  }
}

#endif  // CCF_IDL2_SEMANTIC_GRAPH_VALUE_TYPE_HPP
