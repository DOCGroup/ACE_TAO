// file      : CCF/IDL2/SemanticGraph/Array.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_SEMANTIC_GRAPH_ARRAY_HPP
#define CCF_IDL2_SEMANTIC_GRAPH_ARRAY_HPP

#include "CCF/IDL2/SemanticGraph/Elements.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace SemanticGraph
    {
      //
      //
      //
      class Array : public virtual Specialization
      {
      public:
        Type&
        type () const
        {
          return
            dynamic_cast<ArgumentsWithType&> (**arguments_begin ()).type ();
        }

        // You will have to iterate through Arguments edges starting
        // from begin + 1 to get to the list of bounds. I suggest that
        // you use traversal instead.
        //

        virtual bool
        complete () const
        {
          // Is this the right semantic for arrays?
          //
          return type ().complete ();
        }

        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        Array (Path const& path, unsigned long line)
            : Node (path, line)
        {
          type_info (static_type_info ());
        }
      };
    }
  }
}

#endif  // CCF_IDL2_SEMANTIC_GRAPH_ARRAY_HPP
