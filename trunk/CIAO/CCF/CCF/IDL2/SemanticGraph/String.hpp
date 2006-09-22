// file      : CCF/IDL2/SemanticGraph/String.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_SEMANTIC_GRAPH_STRING_HPP
#define CCF_IDL2_SEMANTIC_GRAPH_STRING_HPP

#include "CCF/IDL2/SemanticGraph/Elements.hpp"
#include "CCF/IDL2/SemanticGraph/IntExpression.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace SemanticGraph
    {
      //
      //
      class BoundedString : public virtual Specialization
      {
      public:
        IntExpression&
        bound () const
        {
          return
            dynamic_cast<IntExpression&> (
              dynamic_cast<ArgumentsWithValue&> (
                **arguments_begin ()).value ());
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

        BoundedString (Path const& path, unsigned long line)
            : Node (path, line)
        {
          type_info (static_type_info ());
        }
      };


      //
      //
      class BoundedWideString : public virtual Specialization
      {
      public:
        IntExpression&
        bound () const
        {
          return
            dynamic_cast<IntExpression&> (
              dynamic_cast<ArgumentsWithValue&> (
                **arguments_begin ()).value ());
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

        BoundedWideString (Path const& path, unsigned long line)
            : Node (path, line)
        {
          type_info (static_type_info ());
        }
      };
    }
  }
}

#endif  // CCF_IDL2_SEMANTIC_GRAPH_STRING_HPP
