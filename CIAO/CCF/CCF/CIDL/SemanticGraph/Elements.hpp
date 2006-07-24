// file      : CCF/CIDL/SemanticGraph/Elements.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_CIDL_SEMANTIC_GRAPH_ELEMENTS_HPP
#define CCF_CIDL_SEMANTIC_GRAPH_ELEMENTS_HPP

#include "CCF/IDL3/SemanticGraph/Elements.hpp"

namespace CCF
{
  namespace CIDL
  {
    namespace SemanticGraph
    {
      using namespace IDL3::SemanticGraph;


      class Implements : public virtual Edge
      {
      public:
        Type&
        implementer () const
        {
          return *implementer_;
        }

        Type&
        implementee () const
        {
          return *implementee_;
        }

        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        Implements ()
        {
          type_info (static_type_info ());
        }

        void
        set_left_node (Type& n)
        {
          implementer_ = &n;
        }

        void
        set_right_node (Type& n)
        {
          implementee_ = &n;
        }

      private:
        Type* implementer_;
        Type* implementee_;
      };
    }
  }
}

#endif  // CCF_CIDL_SEMANTIC_GRAPH_ELEMENTS_HPP
