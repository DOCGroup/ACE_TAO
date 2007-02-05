// file      : CCF/IDL2/SemanticGraph/Elements.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_SEMANTIC_GRAPH_ELEMENTS_HPP
#define CCF_IDL2_SEMANTIC_GRAPH_ELEMENTS_HPP

#include <set>
#include <vector>
#include <iosfwd>

#include "Graph.hpp"

#include "CCF/CompilerElements/FileSystem.hpp"
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
      typedef fs::path Path;

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
        Path const&
        file () const
        {
          return file_;
        }

        unsigned long
        line () const
        {
          return line_;
        }

      public:
        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        virtual
        ~Node () = 0;

        Node (Path const& path, unsigned long line);

        Node (); // For virtual inheritance only.

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
        Path file_;
        unsigned long line_;
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

        Nameable () // For virtual inheritance only.
        {
          type_info (static_type_info ());
        }

        virtual
        ~Nameable () = 0;

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

        Scope () // For virtual inheritance only.
        {
          type_info (static_type_info ());
        }

        virtual
        ~Scope () = 0;

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
      class ArgumentsWithType;

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

        Type () // For virtual inheritance only.
        {
          type_info (static_type_info ());
        }

        virtual
        ~Type () = 0;

        using Nameable::add_edge_right;

        void
        add_edge_right (Belongs& e)
        {
          slassifies_.push_back (&e);
        }

        void
        add_edge_left (ArgumentsWithType&)
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

        Instance () // For virtual inheritance only.
        {
          type_info (static_type_info ());
        }

        virtual
        ~Instance () = 0;

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


      //
      //
      class Arguments;
      class ArgumentsWithType;
      class ArgumentsWithValue;

      class Specialization: public virtual Type
      {
        typedef
        std::vector<Arguments*>
        Arguments_;

      public:
        typedef
        Arguments_::const_iterator
        ArgumentsIterator;

        ArgumentsIterator
        arguments_begin () const
        {
          return arguments_.begin ();
        }

        ArgumentsIterator
        arguments_end () const
        {
          return arguments_.end ();
        }

        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        Specialization () // For virtual inheritance only.
        {
          type_info (static_type_info ());
        }

        virtual
        ~Specialization () = 0;

        void
        add_edge_right (ArgumentsWithType& e);

        void
        add_edge_right (ArgumentsWithValue& e);

      private:
        Arguments_ arguments_;
      };


      class Arguments: public virtual Edge
      {
      public:
        Specialization&
        specialization () const
        {
          return *specialization_;
        }

        Node&
        argument () const
        {
          return *argument_;
        }

        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        Arguments ()
        {
          type_info (static_type_info ());
        }

        void
        set_left_node (Node& n)
        {
          argument_ = &n;
        }

        void
        set_right_node (Specialization& n)
        {
          specialization_ = &n;
        }

      private:
        Specialization* specialization_;
        Node* argument_;
      };

      class ArgumentsWithType: public virtual Arguments
      {
      public:
        Type&
        type () const
        {
          return dynamic_cast<Type&> (argument ());
        }

        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        ArgumentsWithType ()
        {
          type_info (static_type_info ());
        }

        void
        set_left_node (Type& n)
        {
          Arguments::set_left_node (n);
        }
      };

      class Expression;

      class ArgumentsWithValue: public virtual Arguments
      {
      public:
        Expression&
        value () const;

        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        ArgumentsWithValue ()
        {
          type_info (static_type_info ());
        }

        void
        set_left_node (Expression& n);
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

      class Expression;
      class Const;

      class Initializes: public virtual Edge
      {
      public:
        Expression&
        expression () const
        {
          return *expression_;
        }

        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        Initializes ()
        {
          type_info (static_type_info ());
        }

        void
        set_left_node (Expression& e)
        {
          expression_ = &e;
        }

        void
        set_right_node (Const&)
        {
        }

      private:
        Expression* expression_;
      };


      class Expression: public virtual Node
      {
      public:
        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        Expression () // For virtual inheritance only.
        {
          type_info (static_type_info ());
        }

        virtual
        ~Expression () = 0;

        void
        add_edge_left (Initializes&)
        {
        }

        void
        add_edge_left (ArgumentsWithValue&)
        {
        }
      };


      class Const: public virtual Instance,
                   public virtual Nameable,
                   public virtual Expression
      {
      public:
        Initializes&
        initialized () const
        {
          return *initialized_;
        }

        static Introspection::TypeInfo const&
        static_type_info ();

      protected:
        friend class Graph<Node, Edge>;

        Const () // For virtual inheritance only.
        {
          type_info (static_type_info ());
        }

        virtual
        ~Const () = 0;

        using Instance::add_edge_left;
        using Expression::add_edge_left;

        void
        add_edge_right (Initializes& i)
        {
          initialized_ = &i;
        }

        using Nameable::add_edge_right;

      private:
        Initializes* initialized_;
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

        Container () // For virtual inheritance only.
        {
          type_info (static_type_info ());
        }

        virtual
        ~Container () = 0;

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

// ADL won't find it because Path is a typedef. Note that this
// function prints in native format.
//
std::ostream&
operator<< (std::ostream& os, CCF::IDL2::SemanticGraph::Path const& path);

#include "CCF/IDL2/SemanticGraph/Elements.tpp"

#endif  // CCF_IDL2_SEMANTIC_GRAPH_ELEMENTS_HPP
