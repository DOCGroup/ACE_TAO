// file      : CCF/IDL2/SemanticGraph/Sequence.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_SEMANTIC_GRAPH_SEQUENCE_HPP
#define CCF_IDL2_SEMANTIC_GRAPH_SEQUENCE_HPP


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
      class Sequence : public virtual TypeTemplateSpecialization
      {
      public:
        virtual bool
        complete () const
        {
          return specialized ().type ().complete ();
        }

        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        Sequence ()
        {
          type_info (static_type_info ());
        }

        using TypeTemplateSpecialization::add_edge_right;
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

        UnboundedSequence ()
        {
          type_info (static_type_info ());
        }
      };
    }
  }
}

#endif  // CCF_IDL2_SEMANTIC_GRAPH_SEQUENCE_HPP
