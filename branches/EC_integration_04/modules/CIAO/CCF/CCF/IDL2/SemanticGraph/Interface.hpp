// file      : CCF/IDL2/SemanticGraph/Interface.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_SEMANTIC_GRAPH_INTERFACE_HPP
#define CCF_IDL2_SEMANTIC_GRAPH_INTERFACE_HPP

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
      class Interface : public virtual Type, public virtual Scope
      {
        typedef
        std::vector <Inherits*>
        Inherits_;

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

        virtual bool
        complete () const
        {
          return true;
        }

        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        Interface () // For virtual inheritance only.
        {
          type_info (static_type_info ());
        }

        virtual
        ~Interface () = 0;

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

      private:
        Inherits_ inherits_;
      };


      //
      //
      //
      class Supports : public virtual IsA
      {
      public:
        virtual Type&
        supporter () const
        {
          return subject ();
        }

        virtual Type&
        supportee () const
        {
          return object ();
        }

        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        Supports ()
        {
          type_info (static_type_info ());
        }

        void
        set_right_node (Interface& n)
        {
          IsA::set_right_node (n);
        }
      };


      //
      //
      //
      class AbstractInterface : public virtual Interface
      {
      public:
        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        AbstractInterface (Path const& path, unsigned long line)
            : Node (path, line)
        {
          type_info (static_type_info ());
        }
      };


      //
      //
      //
      class LocalInterface : public virtual Interface
      {
      public:
        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        LocalInterface (Path const& path, unsigned long line)
            : Node (path, line)
        {
          type_info (static_type_info ());
        }
      };


      //
      //
      //
      class UnconstrainedInterface : public virtual Interface
      {
      public:
        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        UnconstrainedInterface (Path const& path, unsigned long line)
            : Node (path, line)
        {
          type_info (static_type_info ());
        }
      };
    }
  }
}

#endif  // CCF_IDL2_SEMANTIC_GRAPH_INTERFACE_HPP
