// file      : CCF/IDL3/SemanticGraph/Component.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL3_SEMANTIC_GRAPH_COMPONENT_HPP
#define CCF_IDL3_SEMANTIC_GRAPH_COMPONENT_HPP

#include "CCF/IDL2/SemanticGraph/Interface.hpp"
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
      class Provider : public virtual Nameable, public virtual Instance
      {
      public:
        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        Provider (Path const& path, unsigned long line)
            : Node (path, line)
        {
          type_info (static_type_info ());
        }
      };


      //
      //
      //
      class User : public virtual Nameable, public virtual Instance
      {
      public:
        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        User () // For virtual inheritance only.
        {
          type_info (static_type_info ());
        }

        virtual
        ~User () = 0;
      };


      class MultiUser : public virtual User
      {
      public:
        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        MultiUser (Path const& path, unsigned long line)
            : Node (path, line)
        {
          type_info (static_type_info ());
        }
      };


      class SingleUser : public virtual User
      {
      public:
        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        SingleUser (Path const& path, unsigned long line)
            : Node (path, line)
        {
          type_info (static_type_info ());
        }
      };


      //
      //
      //
      class Publisher : public virtual Nameable, public virtual Instance
      {
      public:
        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        Publisher (Path const& path, unsigned long line)
            : Node (path, line)
        {
          type_info (static_type_info ());
        }
      };


      //
      //
      //
      class Emitter : public virtual Nameable, public virtual Instance
      {
      public:
        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        Emitter (Path const& path, unsigned long line)
            : Node (path, line)
        {
          type_info (static_type_info ());
        }
      };


      //
      //
      //
      class Consumer : public virtual Nameable, public virtual Instance
      {
      public:
        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        Consumer (Path const& path, unsigned long line)
            : Node (path, line)
        {
          type_info (static_type_info ());
        }
      };


      //
      //
      //
      class Component : public virtual Type, public virtual Scope
      {
        typedef
        std::vector <Supports*>
        Supports_;

      public:
        Inherits*
        inherits () const
        {
          return inherits_;
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

        virtual bool
        complete () const
        {
          return true;
        }

        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        Component (Path const& path, unsigned long line)
            : Node (path, line), inherits_ (0)
        {
          type_info (static_type_info ());
        }

        using Type::add_edge_right;
        using Scope::add_edge_left;

        void
        add_edge_left (Inherits& e)
        {
          inherits_ = &e;
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

        void
        add_edge_right (Manages&)
        {
        }

      private:
        Inherits* inherits_;
        Supports_ supports_;
      };
    }
  }
}

#endif  // CCF_IDL3_SEMANTIC_GRAPH_COMPONENT_HPP
