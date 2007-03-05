// file      : CCF/IDL2/SemanticGraph/TypeId.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_SEMANTIC_GRAPH_TYPE_ID_HPP
#define CCF_IDL2_SEMANTIC_GRAPH_TYPE_ID_HPP

#include "CCF/IDL2/SemanticGraph/Elements.hpp"
#include "CCF/IDL2/SemanticGraph/Literals.hpp"


namespace CCF
{
  namespace IDL2
  {
    namespace SemanticGraph
    {
      //
      //
      //
      class TypeId : public virtual Nameable
      {
      public:
        //@@ declaration is a bad name.
        //
        ScopedName
        declaration () const
        {
          return name_;
        }

        StringLiteral
        id () const
        {
          return id_;
        }

        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        TypeId (Path const& path,
                unsigned long line,
                ScopedName const& name,
                StringLiteral const& id)
            : Node (path, line),
              name_ (name),
              id_ (id)
        {
          type_info (static_type_info ());
        }

      private:
        ScopedName name_;
        StringLiteral id_;
      };


      //
      //
      //
      class TypePrefix : public virtual Nameable
      {
      public:
        ScopedName
        declaration () const
        {
          return name_;
        }

        StringLiteral
        prefix () const
        {
          return prefix_;
        }

        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        TypePrefix (Path const& path,
                    unsigned long line,
                    ScopedName const& name,
                    StringLiteral const& prefix)
            : Node (path, line),
              name_ (name),
              prefix_ (prefix)
        {
          type_info (static_type_info ());
        }

      private:
        ScopedName name_;
        StringLiteral prefix_;
      };
    }
  }
}

#endif  // CCF_IDL2_SEMANTIC_GRAPH_TYPE_ID_HPP
