// file      : CCF/IDL2/SemanticGraph/Enum.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_SEMANTIC_GRAPH_ENUM_HPP
#define CCF_IDL2_SEMANTIC_GRAPH_ENUM_HPP

#include "CCF/IDL2/SemanticGraph/IntExpression.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace SemanticGraph
    {
      class Enum : public virtual Type
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

        Enum (Path const& path, unsigned long line)
            : Node (path, line)
        {
          type_info (static_type_info ());
        }
      };


      class Enumerator : public virtual Nameable, public virtual IntConst
      {
      public:
        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        Enumerator (Path const& path, unsigned long line)
            : Node (path, line), IntConst (path, line)
        {
          type_info (static_type_info ());
        }
      };
    }
  }
}

#endif  // CCF_IDL2_SEMANTIC_GRAPH_ENUM_HPP
