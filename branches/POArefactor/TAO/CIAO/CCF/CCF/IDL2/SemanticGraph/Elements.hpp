// file      : CCF/IDL2/SemanticGraph/Elements.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_SEMANTIC_GRAPH_ELEMENTS_HPP
#define CCF_IDL2_SEMANTIC_GRAPH_ELEMENTS_HPP

#include <set>
#include <vector>

#include "Graph.hpp"

#include "CCF/CompilerElements/Introspection.hpp"
#include "CCF/CompilerElements/Context.hpp"

#include "CCF/IDL2/SemanticGraph/Name.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace SemanticGraph
    {
      //
      //
      //

      class Node;
      class Edge;


      // Node & Edge
      //
      //
      class Edge : public virtual Introspection::Object
      {
      public:
        CompilerElements::Context const&
        context () const
        {
          return context_;
        }

        CompilerElements::Context&
        context ()
        {
          return context_;
        }

      public:
        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        virtual
        ~Edge ();

        Edge ();

      private:
        CompilerElements::Context context_;
      };

      inline bool
      operator== (Edge const& x, Edge const& y)
      {
        return &x == &y;
      }

      class Node : public virtual Introspection::Object
      {
      public:
        CompilerElements::Context const&
        context () const
        {
          return context_;
        }

        CompilerElements::Context&
        context ()
        {
          return context_;
        }

      public:
        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        virtual
        ~Node ();

        Node ();

        // This is a bunch of experimantal sink functions that allow
        // extensions in the form of one-way edges (see Executor stuff
        // in CIDL for example).
        //
        void
        add_edge_left (Edge&)
        {
        }

        void
        add_edge_right (Edge&)
        {
        }

      private:
        CompilerElements::Context context_;
      };

      inline bool
      operator== (Node const& x, Node const& y)
      {
        return &x == &y;
      }


      //
      //
      //
      class Scope;
      class Nameable;

      class Names : public virtual Edge
      {
      public:
        SimpleName
        name () const
        {
          return name_;
        }

        Scope&
        scope () const
        {
          return *scope_;
        }

        Nameable&
        named () const
        {
          return *named_;
        }

      public:
        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        Names (SimpleName const& name)
            : name_ (name)
        {
          type_info (static_type_info ());
        }

        void
        set_left_node (Scope& n)
        {
          scope_ = &n;
        }

        void
        set_right_node (Nameable& n)
        {
          named_ = &n;
        }

      private:
        Scope* scope_;
        Nameable* named_;
        SimpleName name_;
      };

      class Defines : public virtual Names
      {
      public:
        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        Defines (SimpleName const& name)
            : Names (name)
        {
          type_info (static_type_info ());
        }
      };


      class Mentions : public virtual Names
      {
      public:
        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        Mentions (SimpleName const& name)
            : Names (name)
        {
          type_info (static_type_info ());
        }
      };


      class Aliases : public virtual Names
      {
      public:
        SimpleName
        synonym () const
        {
          return name ();
        }

        /*

        Let me know if you need this function.

        ScopedName
        eponym () const
        {
        }
        */

      public:
        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        Aliases (SimpleName const& synonym)
            : Names (synonym)

        {
          type_info (static_type_info ());
        }
      };


      class Nameable : public virtual Node
      {
        typedef
        std::vector<Names*>
        Named_;

      public:
        virtual SimpleName
        name () const;

        virtual ScopedName
        scoped_name () const;

        bool
        defined () const
        {
          return defined_;
        }

        typedef
        Named_::const_iterator
        NamedIterator;

        NamedIterator
        named_begin () const
        {
          return named_.begin ();
        }

        NamedIterator
        named_end () const
        {
          return named_.end ();
        }

      public:
        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        Nameable ()
            : defined_ (false)
        {
          type_info (static_type_info ());
        }

        void
        add_edge_right (Defines& e)
        {
          defined_ = true;
          named_.push_back (&e);
        }

        void
        add_edge_right (Names& e)
        {
          named_.push_back (&e);
        }

      private:
        Named_ named_;
        bool defined_;
      };

      typedef
      std::set<Nameable*>
      Nameables;

      class Scope;

      class Extends : public virtual Edge
      {
      public:
        Scope& extender () const
        {
          return *extender_;
        }

        Scope& extendee () const
        {
          return *extendee_;
        }

        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        Extends ()
        {
          type_info (static_type_info ());
        }

        void
        set_left_node (Scope& s)
        {
          extender_ = &s;
        }

        void
        set_right_node (Scope& s)
        {
          extendee_ = &s;
        }

      private:
        Scope* extender_;
        Scope* extendee_;
      };

      class Scope : public virtual Nameable
      {
      private:
        typedef
        std::vector<Extends*>
        Extends_;

        typedef
        std::vector<Names*>
        Names_;

        typedef
        std::map<SimpleName, Names_>
        NamesMap_;

      public:
        //
        //
        typedef
        Extends_::const_iterator
        ExtendsIterator;

        ExtendsIterator
        extends_begin () const
        {
          return extends_.begin ();
        }

        ExtendsIterator
        extends_end () const
        {
          return extends_.end ();
        }


        //
        //
        typedef
        Names_::const_iterator
        NamesIterator;

        typedef
        std::pair <NamesIterator, NamesIterator>
        NamesIteratorPair;

        NamesIterator
        names_begin () const
        {
          return names_.begin ();
        }

        NamesIterator
        names_end () const
        {
          return names_.end ();
        }

        NamesIteratorPair
        find (SimpleName const& name) const
        {
          NamesMap_::const_iterator i (names_map_.find (name));
          if (i == names_map_.end ())
          {
            return NamesIteratorPair (names_.end (), names_.end ());
          }
          else
          {
            return NamesIteratorPair (i->second.begin (), i->second.end ());
          }
        }

        Nameables
        lookup (Name const& name) const;

        void
        lookup (Name const& name, Nameables& result) const;

      public:
        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        Scope ()
        {
          type_info (static_type_info ());
        }

        void
        add_edge_left (Names& e);

        void
        add_edge_left (Extends& e)
        {
          extends_.push_back (&e);
        }

        void
        add_edge_right (Extends&)
        {
        }

        using Nameable::add_edge_right;

      private:
        Extends_ extends_;

        Names_ names_;
        NamesMap_ names_map_;
      };



      //
      //
      //
      class Belongs;
      class Specialized;

      //@@ I can make an alias Classifies for Belongs?
      //

      class Type : public virtual Nameable
      {
      public:
        virtual bool
        complete () const = 0;

        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        Type ()
        {
          type_info (static_type_info ());
        }

        using Nameable::add_edge_right;

        void
        add_edge_right (Belongs& e)
        {
          slassifies_.push_back (&e);
        }

        void
        add_edge_right (Specialized&)
        {
        }

        using Node::add_edge_right;
        using Node::add_edge_left;

      private:
        typedef
        std::vector<Belongs*>
        Classifies_;

        Classifies_ slassifies_;
      };


      class Instance : public virtual Node
      {
      public:
        Belongs&
        belongs () const
        {
          return *belongs_;
        }

        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        Instance ()
        {
          type_info (static_type_info ());
        }

        void
        add_edge_left (Belongs& e)
        {
          belongs_ = &e;
        }

      private:
        Belongs* belongs_;
      };


      class Belongs : public virtual Edge
      {
      public:
        Instance&
        instance () const
        {
          return *instance_;
        }

        Type&
        type () const
        {
          return *type_;
        }


        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        Belongs ()
        {
          type_info (static_type_info ());
        }

        void
        set_left_node (Instance& n)
        {
          instance_ = &n;
        }

        void
        set_right_node (Type& n)
        {
          type_ = &n;
        }

      private:
        Instance* instance_;
        Type* type_;
      };


      //@@ Maybe rename it to just Specialization
      //
      class TypeTemplateSpecialization : public virtual Type
      {
      public:
        Specialized&
        specialized () const
        {
          return *specialized_;
        }

        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        TypeTemplateSpecialization ()
        {
          type_info (static_type_info ());
        }

        void
        add_edge_left (Specialized& e)
        {
          specialized_ = &e;
        }

      private:
        Specialized* specialized_;
      };



      class Specialized : public virtual Edge
      {
      public:
        TypeTemplateSpecialization&
        specialization () const
        {
          return *specialization_;
        }

        Type&
        type () const
        {
          return *type_;
        }

        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        Specialized ()
        {
          type_info (static_type_info ());
        }

        void
        set_left_node (TypeTemplateSpecialization& n)
        {
          specialization_ = &n;
        }

        void
        set_right_node (Type& n)
        {
          type_ = &n;
        }

      private:
        TypeTemplateSpecialization* specialization_;
        Type* type_;
      };


      //
      //
      //
      class IsA : public virtual Edge
      {
      public:
        virtual Type&
        subject () const
        {
          return *subject_;
        }

        virtual Type&
        object () const
        {
          return *object_;
        }

        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        IsA ()
        {
          type_info (static_type_info ());
        }

        void
        set_left_node (Type& n)
        {
          subject_ = &n;
        }

        void
        set_right_node (Type& n)
        {
          object_ = &n;
        }

      private:
        Type* subject_;
        Type* object_;
      };


      //
      //
      //
      class Inherits : public virtual IsA
      {
      public:
        virtual Type&
        inheritor () const
        {
          return subject ();
        }

        virtual Type&
        inheritee () const
        {
          return object ();
        }

        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        Inherits ()
        {
          type_info (static_type_info ());
        }
      };


      //
      //
      //

      class Container;

      class Contains : public virtual Edge
      {
      public:
        Node&
        element () const
        {
          return *element_;
        }

        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        Contains ()
        {
          type_info (static_type_info ());
        }

        void
        set_left_node (Container& n)
        {
          container_ = &n;
        }

        void
        set_right_node (Node& n)
        {
          element_ = &n;
        }

      private:
        Container* container_;
        Node* element_;
      };


      class Container : public virtual Node
      {
        typedef
        std::vector<Contains*>
        Contains_;

      public:
        typedef
        Contains_::const_iterator
        ContainsIterator;

        ContainsIterator
        contains_begin () const
        {
          return contains_.begin ();
        }

        ContainsIterator
        contains_end () const
        {
          return contains_.end ();
        }

        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        Container ()
        {
          type_info (static_type_info ());
        }

        void
        add_edge_left (Contains& e)
        {
          contains_.push_back (&e);
        }

      private:
        Contains_ contains_;
      };
    }
  }
}

#include "CCF/IDL2/SemanticGraph/Elements.tpp"

#endif  // CCF_IDL2_SEMANTIC_GRAPH_ELEMENTS_HPP
