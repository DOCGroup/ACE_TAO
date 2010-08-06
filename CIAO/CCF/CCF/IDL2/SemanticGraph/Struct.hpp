// file      : CCF/IDL2/SemanticGraph/Struct.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_SEMANTIC_GRAPH_STRUCT_HPP
#define CCF_IDL2_SEMANTIC_GRAPH_STRUCT_HPP

#include "CCF/IDL2/SemanticGraph/Elements.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace SemanticGraph
    {
      class Struct : public virtual Type, public virtual Scope
      {
      public:
        virtual bool
        complete () const;

        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        Struct (Path const& path, unsigned long line)
            : Node (path, line)
        {
          type_info (static_type_info ());
        }

        using Type::add_edge_right;
        using Scope::add_edge_left;
      };
    }
  }
}

#endif  // CCF_IDL2_SEMANTIC_GRAPH_STRUCT_HPP
