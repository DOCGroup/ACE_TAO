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

        Attribute () // For virtual inheritance only.
        {
          type_info (static_type_info ());
        }

        virtual
        ~Attribute () = 0;
      };


      //
      //
      //
      class ReadAttribute;

      //
      //
      //
      class GetRaises : public virtual Edge
      {
      public:
        ReadAttribute&
        attribute () const
        {
          return *attribute_;
        }

        Exception&
        exception () const
        {
          return *exception_;
        }

        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        GetRaises ()
        {
          type_info (static_type_info ());
        }

        void
        set_left_node (ReadAttribute& a)
        {
          attribute_ = &a;
        }

        void
        set_right_node (Exception& n)
        {
          exception_ = &n;
        }

      private:
        ReadAttribute* attribute_;
        Exception* exception_;
      };


      //
      //
      //
      class ReadAttribute : public virtual Attribute
      {
        typedef
        std::vector<GetRaises*>
        GetRaises_;

      public:
        typedef
        GetRaises_::const_iterator
        GetRaisesIterator;

        GetRaisesIterator
        get_raises_begin () const
        {
          return get_raises_.begin ();
        }

        GetRaisesIterator
        get_raises_end () const
        {
          return get_raises_.end ();
        }

      public:
        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        ReadAttribute (Path const& path, unsigned long line)
            : Node (path, line)
        {
          type_info (static_type_info ());
        }

        void
        add_edge_left (GetRaises& e)
        {
          get_raises_.push_back (&e);
        }

      private:
        GetRaises_ get_raises_;
      };


      //
      //
      //
      class WriteAttribute;

      //
      //
      //
      class SetRaises : public virtual Edge
      {
      public:
        WriteAttribute&
        attribute () const
        {
          return *attribute_;
        }

        Exception&
        exception () const
        {
          return *exception_;
        }

        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        SetRaises ()
        {
          type_info (static_type_info ());
        }

        void
        set_left_node (WriteAttribute& a)
        {
          attribute_ = &a;
        }

        void
        set_right_node (Exception& n)
        {
          exception_ = &n;
        }

      private:
        WriteAttribute* attribute_;
        Exception* exception_;
      };


      class WriteAttribute : public virtual Attribute
      {
        typedef
        std::vector<SetRaises*>
        SetRaises_;

      public:
        typedef
        SetRaises_::const_iterator
        SetRaisesIterator;

        SetRaisesIterator
        set_raises_begin () const
        {
          return set_raises_.begin ();
        }

        SetRaisesIterator
        set_raises_end () const
        {
          return set_raises_.end ();
        }
      public:
        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        WriteAttribute (Path const& path, unsigned long line)
            : Node (path, line)
        {
          type_info (static_type_info ());
        }

        void
        add_edge_left (SetRaises& e)
        {
          set_raises_.push_back (&e);
        }

      private:
        SetRaises_ set_raises_;
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

        ReadWriteAttribute (Path const& path, unsigned long line)
            : Node (path, line),
              ReadAttribute (path, line),
              WriteAttribute (path, line)
        {
          type_info (static_type_info ());
        }
      };
    }
  }
}

#endif  // CCF_IDL2_SEMANTIC_GRAPH_ATTRIBUTE_HPP
