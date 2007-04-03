// file      : CCF/IDL2/SemanticGraph/ValueType.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_SEMANTIC_GRAPH_VALUE_TYPE_HPP
#define CCF_IDL2_SEMANTIC_GRAPH_VALUE_TYPE_HPP

#include "CCF/IDL2/SemanticGraph/Elements.hpp"
#include "CCF/IDL2/SemanticGraph/Operation.hpp"
#include "CCF/IDL2/SemanticGraph/Interface.hpp" // Supports

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
        typedef
        std::vector <Inherits*>
        Inherits_;

        typedef
        std::vector <Supports*>
        Supports_;

      public:
        typedef
        Inherits_::const_iterator
        InheritsIterator;

        InheritsIterator
        inherits_begin () const
        {
          return inherits_.begin ();
        }

        InheritsIterator
        inherits_end () const
        {
          return inherits_.end ();
        }

        typedef
        Supports_::const_iterator
        SupportsIterator;

        SupportsIterator
        supports_begin () const
        {
          return supports_.begin ();
        }

        SupportsIterator
        supports_end () const
        {
          return supports_.end ();
        }

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

        ValueType () // For virtual inheritance only.
        {
          type_info (static_type_info ());
        }

        virtual
        ~ValueType () = 0;

        using Type::add_edge_right;
        using Scope::add_edge_left;

        void
        add_edge_left (Inherits& e)
        {
          inherits_.push_back (&e);
        }

        void
        add_edge_right (Inherits&)
        {
        }

        void
        add_edge_left (Supports& e)
        {
          supports_.push_back (&e);
        }

      private:
        Inherits_ inherits_;
        Supports_ supports_;
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

        AbstractValueType (Path const& path, unsigned long line)
            : Node (path, line)
        {
          type_info (static_type_info ());
        }
      };


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

        ConcreteValueType (Path const& path, unsigned long line)
            : Node (path, line)
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

        ValueTypeFactory (Path const& path, unsigned long line)
            : Node (path, line), TwoWayOperation (path, line)
        {
          type_info (static_type_info ());
        }
      };
    }
  }
}

#endif  // CCF_IDL2_SEMANTIC_GRAPH_VALUE_TYPE_HPP
