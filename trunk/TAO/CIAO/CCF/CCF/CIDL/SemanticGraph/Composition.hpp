// file      : CCF/CIDL/SemanticGraph/Composition.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_CIDL_SEMANTIC_GRAPH_COMPOSITION_HPP
#define CCF_CIDL_SEMANTIC_GRAPH_COMPOSITION_HPP

#include "CCF/CIDL/SemanticGraph/Elements.hpp"

namespace CCF
{
  namespace CIDL
  {
    namespace SemanticGraph
    {
      class Composition : public virtual Scope
      {
      public:
        class Kind
        {
        public:
          static Kind const entity, process, service, session;

        public:
          operator char const* () const;

          bool
          operator== (Kind d) const
          {
            return v_ == d.v_;
          }

          bool
          operator!= (Kind d) const
          {
            return v_ != d.v_;
          }

        private:
          enum Value
          {
            entity_, process_, service_, session_
          } v_;

          Kind (Value v)
              : v_ (v)
          {
          }
        };

        virtual Kind
        kind () = 0;

        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        Composition ()
        {
          type_info (static_type_info ());
        }
      };

      class EntityComposition : public virtual Composition
      {
      public:
        virtual Kind
        kind ()
        {
          return Kind::entity;
        }

        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        EntityComposition ()
        {
          type_info (static_type_info ());
        }
      };

      class ProcessComposition : public virtual Composition
      {
      public:
        virtual Kind
        kind ()
        {
          return Kind::process;
        }

        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        ProcessComposition ()
        {
          type_info (static_type_info ());
        }
      };


      class ServiceComposition : public virtual Composition
      {
      public:
        virtual Kind
        kind ()
        {
          return Kind::service;
        }

        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        ServiceComposition ()
        {
          type_info (static_type_info ());
        }
      };


      class SessionComposition : public virtual Composition
      {
      public:
        virtual Kind
        kind ()
        {
          return Kind::session;
        }

        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        SessionComposition ()
        {
          type_info (static_type_info ());
        }
      };
    }
  }
}

#endif  // CCF_CIDL_SEMANTIC_GRAPH_COMPOSITION_HPP
