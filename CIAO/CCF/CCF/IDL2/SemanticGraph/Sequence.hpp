// file      : CCF/IDL2/SemanticGraph/Sequence.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_SEMANTIC_GRAPH_SEQUENCE_HPP
#define CCF_IDL2_SEMANTIC_GRAPH_SEQUENCE_HPP

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
      //
      class Sequence : public virtual Specialization
      {
      public:
        Type&
        type () const
        {
          return
            dynamic_cast<ArgumentsWithType&> (**arguments_begin ()).type ();
        }

        virtual bool
        complete () const
        {
          return type ().complete ();
        }

        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        Sequence () // For virtual inheritance only.
        {
          type_info (static_type_info ());
        }

        virtual
        ~Sequence () = 0;
      };


      //
      //
      //
      class UnboundedSequence : public virtual Sequence
      {
      public:
        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        UnboundedSequence (Path const& path, unsigned long line)
            : Node (path, line)
        {
          type_info (static_type_info ());
        }
      };


      //
      //
      //
      class BoundedSequence : public virtual Sequence
      {
      public:
        IntExpression&
        bound () const
        {
          ArgumentsIterator i (arguments_begin ());

          ++i; // Bound is always second to the type.

          return
            dynamic_cast<IntExpression&> (
              dynamic_cast<ArgumentsWithValue&> (**i).value ());
        }

        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        BoundedSequence (Path const& path, unsigned long line)
            : Node (path, line)
        {
          type_info (static_type_info ());
        }
      };
    }
  }
}

#endif  // CCF_IDL2_SEMANTIC_GRAPH_SEQUENCE_HPP
