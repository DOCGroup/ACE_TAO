// file      : CCF/CIDL/SemanticGraph/Executor.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_CIDL_SEMANTIC_GRAPH_EXECUTOR_HPP
#define CCF_CIDL_SEMANTIC_GRAPH_EXECUTOR_HPP

#include "CCF/IDL3/SemanticGraph/Component.hpp"
#include "CCF/IDL3/SemanticGraph/Home.hpp"
#include "CCF/CIDL/SemanticGraph/Elements.hpp"

namespace CCF
{
  namespace CIDL
  {
    namespace SemanticGraph
    {
      //
      //
      //
      class Executor : public virtual Type
      {
      public:
        Implements&
        implements () const
        {
          return *implements_;
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

        Executor ()
        {
          type_info (static_type_info ());
        }

        void
        add_edge_left (Implements& e)
        {
          implements_ = &e;
        }

      private:
        Implements* implements_;
      };


      //
      //
      //
      class ComponentExecutor : public virtual Executor
      {
      public:
        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        ComponentExecutor ()
        {
          type_info (static_type_info ());
        }
      };


      //
      //
      //
      class HomeExecutor : public virtual Executor
      {
      public:
        Manages&
        manages () const
        {
          return *manages_;
        }


        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        HomeExecutor ()
        {
          type_info (static_type_info ());
        }

        using Executor::add_edge_left;

        void
        add_edge_left (Manages& e)
        {
          manages_ = &e;
        }

      private:
        Manages* manages_;
      };

    }
  }
}

#endif  // CCF_CIDL_SEMANTIC_GRAPH_EXECUTOR_HPP
