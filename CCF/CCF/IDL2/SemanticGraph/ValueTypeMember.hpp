// file      : CCF/IDL2/SemanticGraph/ValueTypeMember.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_SEMANTIC_GRAPH_VALUE_TYPE_MEMBER_HPP
#define CCF_IDL2_SEMANTIC_GRAPH_VALUE_TYPE_MEMBER_HPP

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
      class ValueTypeMember : public virtual Nameable,
                              public virtual Instance
      {
      public:
        struct Access
        {
          enum Value
          {
            public_,
            private_
          };
        };

        virtual Access::Value
        access () const = 0;

      public:
        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        ValueTypeMember () // For virtual inheritance only.
        {
          type_info (static_type_info ());
        }

        virtual
        ~ValueTypeMember () = 0;
      };


      //
      //
      //
      class ValueTypePrivateMember : public virtual ValueTypeMember
      {
      public:
        virtual Access::Value
        access () const
        {
          return Access::private_;
        }

      public:
        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        ValueTypePrivateMember (Path const& path, unsigned long line)
            : Node (path, line)
        {
          type_info (static_type_info ());
        }
      };


      //
      //
      //
      class ValueTypePublicMember : public virtual ValueTypeMember
      {
      public:
        virtual Access::Value
        access () const
        {
          return Access::public_;
        }

      public:
        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        ValueTypePublicMember (Path const& path, unsigned long line)
            : Node (path, line)
        {
          type_info (static_type_info ());
        }
      };
    }
  }
}

#endif  // CCF_IDL2_SEMANTIC_GRAPH_VALUE_TYPE_MEMBER_HPP
