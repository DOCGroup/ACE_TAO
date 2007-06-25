// file      : CCF/IDL2/SemanticGraph/Exception.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_SEMANTIC_GRAPH_EXCEPTION_HPP
#define CCF_IDL2_SEMANTIC_GRAPH_EXCEPTION_HPP

#include "CCF/IDL2/SemanticGraph/Elements.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace SemanticGraph
    {
      class Raises;

      class Exception : public virtual Type, public virtual Scope
      {
      public:
        virtual bool
        complete () const
        {
          return defined ();
        }

        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        Exception (Path const& path, unsigned long line)
            : Node (path, line)
        {
          type_info (static_type_info ());
        }

        using Type::add_edge_right;
        using Scope::add_edge_left;

        void
        add_edge_right (Raises&)
        {
        }
      };
    }
  }
}

#endif  // CCF_IDL2_SEMANTIC_GRAPH_EXCEPTION_HPP
