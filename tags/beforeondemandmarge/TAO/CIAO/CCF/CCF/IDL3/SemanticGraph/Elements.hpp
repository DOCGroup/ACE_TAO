// file      : CCF/IDL3/SemanticGraph/Elements.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL3_SEMANTIC_GRAPH_ELEMENTS_HPP
#define CCF_IDL3_SEMANTIC_GRAPH_ELEMENTS_HPP

#include "CCF/IDL2/SemanticGraph/Elements.hpp"

namespace CCF
{
  namespace IDL3
  {
    namespace SemanticGraph
    {
      using namespace CCF::IDL2::SemanticGraph;

      //
      //
      //
      class Manages : public virtual Edge
      {
      public:
        Node&
        manager () const
        {
          return *manager_;
        }

        Node&
        managee () const
        {
          return *managee_;
        }

        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        Manages ()
        {
          type_info (static_type_info ());
        }

        void
        set_left_node (Node& n)
        {
          manager_ = &n;
        }

        void
        set_right_node (Node& n)
        {
          managee_ = &n;
        }

      private:
        Node* manager_;
        Node* managee_;
      };
    }
  }
}

#endif  // CCF_IDL3_SEMANTIC_GRAPH_ELEMENTS_HPP
