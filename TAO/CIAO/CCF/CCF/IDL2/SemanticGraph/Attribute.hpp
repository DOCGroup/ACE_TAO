// file      : CCF/IDL2/SemanticGraph/Attribute.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_SEMANTIC_GRAPH_ATTRIBUTE_HPP
#define CCF_IDL2_SEMANTIC_GRAPH_ATTRIBUTE_HPP

#include "CCF/IDL2/SemanticGraph/Elements.hpp"
#include "CCF/IDL2/SemanticGraph/Exception.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace SemanticGraph
    {
      //
      //
      //
      class Attribute : public virtual Nameable, public virtual Instance
      {
      public:
        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        Attribute ()
        {
          type_info (static_type_info ());
        }
      };


      class ReadAttribute;

      class GetRaises : public virtual Edge
      {
      public:
        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        GetRaises ()
        {
          type_info (static_type_info ());
        }
      };


      class ReadAttribute : public virtual Attribute
      {
      public:
        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        ReadAttribute ()
        {
          type_info (static_type_info ());
        }
      };


      //
      //
      //
      class SetRaises : public virtual Edge
      {
      public:
        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        SetRaises ()
        {
          type_info (static_type_info ());
        }
      };


      class WriteAttribute : public virtual Attribute
      {
      public:
        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        WriteAttribute ()
        {
          type_info (static_type_info ());
        }
      };


      //
      //
      //
      class ReadWriteAttribute : public virtual ReadAttribute,
                                 public virtual WriteAttribute
      {
      public:
        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        ReadWriteAttribute ()
        {
          type_info (static_type_info ());
        }
      };
    }
  }
}

#endif  // CCF_IDL2_SEMANTIC_GRAPH_ATTRIBUTE_HPP
