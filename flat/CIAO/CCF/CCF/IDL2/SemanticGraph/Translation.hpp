// file      : CCF/IDL2/SemanticGraph/Translation.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_SEMANTIC_GRAPH_TRANSLATION_HPP
#define CCF_IDL2_SEMANTIC_GRAPH_TRANSLATION_HPP

#include <vector>
#include <string>

#include "CCF/IDL2/SemanticGraph/Elements.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace SemanticGraph
    {
      class Root;
      class TranslationRegion;
      class TranslationUnit;


      //
      //
      //
      class ContainsPrincipal : public virtual Contains
      {
      public:
        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        ContainsPrincipal ()
        {
          type_info (static_type_info ());
        }

        void
        set_left_node (TranslationUnit& n);

        void
        set_right_node (TranslationRegion& n);
      };


      //
      //
      //
      class ContainsImplied : public virtual Contains
      {
      public:
        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        ContainsImplied (std::string const& name = "")
            : name_ (name)
        {
          type_info (static_type_info ());
        }

        void
        set_left_node (TranslationUnit& n);

        void
        set_right_node (TranslationRegion& n);

      private:
        std::string name_;
      };


      //
      //
      //
      class ContainsRoot : public virtual Contains
      {
      public:
        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        ContainsRoot ()
        {
          type_info (static_type_info ());
        }

        void
        set_left_node (TranslationRegion& n);

        void
        set_right_node (Root& n);
      };

      //
      //
      //
      class Includes : public virtual Contains
      {
      public:
        Path
        file () const
        {
          return file_;
        }

        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        Includes (Path const& file)
            : file_ (file)
        {
          type_info (static_type_info ());
        }

        void
        set_left_node (TranslationRegion& n);

        void
        set_right_node (TranslationRegion& n);

      private:
        Path file_;
      };


      //
      //
      //
      class QuoteIncludes : public virtual Includes
      {
      public:
        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        QuoteIncludes (Path const& file)
            : Includes (file)
        {
          type_info (static_type_info ());
        }
      };


      //
      //
      //
      class BracketIncludes : public virtual Includes
      {
      public:
        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        BracketIncludes (Path const& file)
            : Includes (file)
        {
          type_info (static_type_info ());
        }
      };


      //
      //
      //
      class Root : public virtual Scope
      {
      public:
        virtual SimpleName
        name () const
        {
          return SimpleName ("");
        }

        virtual ScopedName
        scoped_name () const
        {
          return ScopedName ("");
        }

        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        Root (Path const& path, unsigned long line)
            : Node (path, line)
        {
          type_info (static_type_info ());
        }

        void
        add_edge_right (Contains&)
        {
          // TranslationRegion contains Root
        }
      };


      //
      //
      //
      class TranslationRegion : public virtual Container
      {
      public:
        Nameables
        lookup (ScopedName const& name) const;

        void
        lookup (ScopedName const& name, Nameables& result) const;

        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        TranslationRegion (Path const& path, unsigned long line)
            : Node (path, line)
        {
          type_info (static_type_info ());
        }

        void
        add_edge_right (Contains&)
        {
        }
      };


      class TranslationUnit : public virtual Container,
                              public Graph<Node, Edge>
      {
      public:
        TranslationUnit (Path const& path, unsigned long line)
            : Node (path, line)
        {
          type_info (static_type_info ());
        }

        Nameables
        lookup (ScopedName const& name) const;

        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        void
        add_edge_left (ContainsPrincipal& e)
        {
          Container::add_edge_left (e);
        }

        void
        add_edge_left (ContainsImplied& e)
        {
          Container::add_edge_left (e);
        }

      private:
        TranslationUnit (TranslationUnit const&);
        void operator= (TranslationUnit const&);
      };
    }
  }
}

#include "CCF/IDL2/SemanticGraph/Translation.ipp"

#endif  // CCF_IDL2_SEMANTIC_GRAPH_TRANSLATION_HPP
