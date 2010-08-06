// file      : CCF/IDL3/SemanticGraph/Home.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL3_SEMANTIC_GRAPH_HOME_HPP
#define CCF_IDL3_SEMANTIC_GRAPH_HOME_HPP

#include "CCF/IDL2/SemanticGraph/Operation.hpp"
#include "CCF/IDL3/SemanticGraph/Elements.hpp"
#include "CCF/IDL3/SemanticGraph/Component.hpp"

namespace CCF
{
  namespace IDL3
  {
    namespace SemanticGraph
    {
      //
      //
      //
      class Home : public virtual Type, public virtual Scope
      {
        typedef
        std::vector <Supports*>
        Supports_;

      public:
        Manages&
        manages () const
        {
          return *manages_;
        }

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

        Home (Path const& path, unsigned long line)
            : Node (path, line), inherits_ (0)
        {
          type_info (static_type_info ());
        }

        using Type::add_edge_right;
        using Scope::add_edge_left;

        void
        add_edge_left (Manages& e)
        {
          manages_ = &e;
        }

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

      private:
        Manages* manages_;
        Inherits* inherits_;
        Supports_ supports_;
      };


      //
      //
      //
      class HomeFactory : public virtual TwoWayOperation
      {
      public:
        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        HomeFactory (Path const& path, unsigned long line)
            : Node (path, line), TwoWayOperation (path, line)
        {
          type_info (static_type_info ());
        }
      };


      //
      //
      //
      class HomeFinder : public virtual TwoWayOperation
      {
      public:
        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        HomeFinder (Path const& path, unsigned long line)
            : Node (path, line), TwoWayOperation (path, line)
        {
          type_info (static_type_info ());
        }
      };
    }
  }
}

#endif  // CCF_IDL3_SEMANTIC_GRAPH_HOME_HPP
