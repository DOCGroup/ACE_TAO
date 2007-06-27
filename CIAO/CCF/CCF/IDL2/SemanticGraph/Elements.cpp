// file      : CCF/IDL2/SemanticGraph/Elements.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL2/SemanticGraph/Elements.hpp"

#include <ostream>

// This file has no dependency on ACE, thus we turn 
// all ACE_OS checks off
// FUZZ: disable check_for_lack_ACE_OS

namespace CCF
{
  namespace IDL2
  {
    namespace SemanticGraph
    {
      using Introspection::TypeInfo;
      using Introspection::Access;

      // Edge
      //
      //
      Edge::
      ~Edge ()
      {
      }

      Edge::
      Edge ()
      {
        type_info (static_type_info ());
      }

      namespace
      {
        TypeInfo
        edge_init_ ()
        {
          TypeInfo ti (typeid (Edge));
          // I don't really need this information
          // ti.add_base (Access::PUBLIC, true, Object::static_type_info ());
          return ti;
        }

        TypeInfo edge_ (edge_init_ ());
      }

      TypeInfo const& Edge::
      static_type_info () { return edge_; }


      // Node
      //
      //
      Node::
      ~Node ()
      {
      }

      Node::
      Node (Path const& file, unsigned long line)
          : file_ (file), line_ (line)
      {
        type_info (static_type_info ());
      }

      Node::
      Node ()
      {
        abort (); // This ctor should never be called.
      }

      namespace
      {
        TypeInfo
        node_init_ ()
        {
          TypeInfo ti (typeid (Node));
          // I don't really need this information
          // ti.add_base (Access::PUBLIC, true, Object::static_type_info ());
          return ti;
        }

        TypeInfo node_ (node_init_ ());
      }

      TypeInfo const& Node::
      static_type_info () { return node_; }


      // Names
      //
      //
      namespace
      {
        TypeInfo
        names_init_ ()
        {
          TypeInfo ti (typeid (Names));
          ti.add_base (Access::PUBLIC, true, Edge::static_type_info ());
          return ti;
        }

        TypeInfo names_ (names_init_ ());
      }

      TypeInfo const& Names::
      static_type_info () { return names_; }


      // Defines
      //
      //
      namespace
      {
        TypeInfo
        defines_init_ ()
        {
          TypeInfo ti (typeid (Defines));
          ti.add_base (
            Access::PUBLIC, true, Names::static_type_info ());
          return ti;
        }

        TypeInfo defines_ (defines_init_ ());
      }

      TypeInfo const& Defines::
      static_type_info () { return defines_; }


      // Mentions
      //
      //
      namespace
      {
        TypeInfo
        mentions_init_ ()
        {
          TypeInfo ti (typeid (Mentions));
          ti.add_base (Access::PUBLIC, true, Names::static_type_info ());
          return ti;
        }

        TypeInfo mentions_ (mentions_init_ ());
      }

      TypeInfo const& Mentions::
      static_type_info () { return mentions_; }


      // Aliases
      //
      //
      namespace
      {
        TypeInfo
        aliases_init_ ()
        {
          TypeInfo ti (typeid (Aliases));
          ti.add_base (
            Access::PUBLIC, true, Names::static_type_info ());
          return ti;
        }

        TypeInfo aliases_ (aliases_init_ ());
      }

      TypeInfo const& Aliases::
      static_type_info () { return aliases_; }


      // Nameable
      //
      //

      Nameable::
      ~Nameable ()
      {
      }

      SimpleName Nameable::
      name () const
      {
        // The first entry should either be Mentions or Defines
        // and it is the 'primary' name of the nameable entity.
        //
        return named_[0]->name ();
      }

      ScopedName Nameable::
      scoped_name () const
      {
        // In general, scope that named us is named, in turn,
        // by somebody else.
        //
        return ScopedName (named_[0]->scope ().scoped_name (), name ());
      }

      namespace
      {
        TypeInfo
        nameable_init_ ()
        {
          TypeInfo ti (typeid (Nameable));
          ti.add_base (Access::PUBLIC, true, Node::static_type_info ());
          return ti;
        }

        TypeInfo nameable_ (nameable_init_ ());
      }

      TypeInfo const& Nameable::
      static_type_info () { return nameable_; }


      // Extends
      //
      //
      namespace
      {
        TypeInfo
        extends_init_ ()
        {
          TypeInfo ti (typeid (Extends));
          ti.add_base (Access::PUBLIC, true, Edge::static_type_info ());
          return ti;
        }

        TypeInfo extends_ (extends_init_ ());
      }

      TypeInfo const& Extends::
      static_type_info () { return extends_; }


      // Scope
      //
      //
      Scope::
      ~Scope ()
      {
      }

      Nameables Scope::
      lookup (Name const& name) const
      {
        Nameables r;
        lookup (name, r);
        return r;
      }

      void Scope::
      lookup (Name const& name, Nameables& result) const
      {
        SimpleName first (*name.begin ());

        NamesIteratorPair pair (find (first));

        if (pair.first != pair.second) // Found something.
        {
          if (name.simple ()) // last name
          {
            for (NamesIterator n (pair.first); n != pair.second; ++n)
            {
              result.insert (&((**n).named ()));
            }
          }
          else
          {
            Name rest (name.begin () + 1, name.end ());

            for (NamesIterator n (pair.first); n != pair.second; ++n)
            {
              Nameable& node ((**n).named ());
              if (Scope* s = dynamic_cast<Scope*> (&node))
              {
                s->lookup (rest, result);
              }
            }
          }
        }
        else // Try scopes that we are an extension of.
        {
          for (ExtendsIterator i (extends_begin ()), e (extends_end ());
               i != e; ++i)
          {
            (**i).extendee ().lookup (name, result);
          }
        }
      }

      void Scope::
      add_edge_left (Names& e)
      {
        names_.push_back (&e);
        names_map_[e.name ()].push_back (&e);
      }

      namespace
      {
        TypeInfo
        scope_init_ ()
        {
          TypeInfo ti (typeid (Scope));
          ti.add_base (Access::PUBLIC, true, Nameable::static_type_info ());
          return ti;
        }

        TypeInfo scope_ (scope_init_ ());
      }

      TypeInfo const& Scope::
      static_type_info () { return scope_; }


      // Type
      //
      //
      namespace
      {
        TypeInfo
        type_init_ ()
        {
          TypeInfo ti (typeid (Type));
          ti.add_base (Access::PUBLIC, true, Nameable::static_type_info ());
          return ti;
        }

        TypeInfo type_ (type_init_ ());
      }

      TypeInfo const& Type::
      static_type_info () { return type_; }

      Type::
      ~Type ()
      {
      }

      // Instance
      //
      //
      namespace
      {
        TypeInfo
        instance_init_ ()
        {
          TypeInfo ti (typeid (Instance));
          ti.add_base (Access::PUBLIC, true, Node::static_type_info ());
          return ti;
        }

        TypeInfo instance_ (instance_init_ ());
      }

      TypeInfo const& Instance::
      static_type_info () { return instance_; }

      Instance::
      ~Instance ()
      {
      }

      // Belongs
      //
      //
      namespace
      {
        TypeInfo
        belongs_init_ ()
        {
          TypeInfo ti (typeid (Belongs));
          ti.add_base (Access::PUBLIC, true, Edge::static_type_info ());
          return ti;
        }

        TypeInfo belongs_ (belongs_init_ ());
      }

      TypeInfo const& Belongs::
      static_type_info () { return belongs_; }


      // Specialization
      //
      //
      namespace
      {
        TypeInfo
        specialization_init_ ()
        {
          TypeInfo ti (typeid (Specialization));
          ti.add_base (Access::PUBLIC, true, Type::static_type_info ());
          return ti;
        }

        TypeInfo specialization_ (
          specialization_init_ ());
      }

      TypeInfo const& Specialization::
      static_type_info () { return specialization_; }

      void Specialization::
      add_edge_right (ArgumentsWithType& e)
      {
        arguments_.push_back (&e);
      }

      void Specialization::
      add_edge_right (ArgumentsWithValue& e)
      {
        arguments_.push_back (&e);
      }

      Specialization::
      ~Specialization ()
      {
      }


      // Arguments
      //
      //
      namespace
      {
        TypeInfo
        arguments_init_ ()
        {
          TypeInfo ti (typeid (Arguments));
          ti.add_base (Access::PUBLIC, true, Edge::static_type_info ());
          return ti;
        }

        TypeInfo arguments_ (arguments_init_ ());
      }

      TypeInfo const& Arguments::
      static_type_info () { return arguments_; }


      // ArgumentsWithType
      //
      //
      namespace
      {
        TypeInfo
        arguments_with_type_init_ ()
        {
          TypeInfo ti (typeid (ArgumentsWithType));
          ti.add_base (Access::PUBLIC, true, Arguments::static_type_info ());
          return ti;
        }

        TypeInfo arguments_with_type_ (arguments_with_type_init_ ());
      }

      TypeInfo const& ArgumentsWithType::
      static_type_info () { return arguments_with_type_; }


      // ArgumentsWithValue
      //
      //
      namespace
      {
        TypeInfo
        arguments_with_value_init_ ()
        {
          TypeInfo ti (typeid (ArgumentsWithValue));
          ti.add_base (Access::PUBLIC, true, Arguments::static_type_info ());
          return ti;
        }

        TypeInfo arguments_with_value_ (arguments_with_value_init_ ());
      }

      TypeInfo const& ArgumentsWithValue::
      static_type_info () { return arguments_with_value_; }

      Expression& ArgumentsWithValue::
      value () const
      {
        return dynamic_cast<Expression&> (argument ());
      }

      void ArgumentsWithValue::
      set_left_node (Expression& n)
      {
        Arguments::set_left_node (n);
      }

      // IsA
      //
      //
      namespace
      {
        TypeInfo
        is_a_init_ ()
        {
          TypeInfo ti (typeid (IsA));
          ti.add_base (Access::PUBLIC, true, Edge::static_type_info ());
          return ti;
        }

        TypeInfo is_a_ (is_a_init_ ());
      }

      TypeInfo const& IsA::
      static_type_info () { return is_a_; }


      // Inherits
      //
      //
      namespace
      {
        TypeInfo
        inherits_init_ ()
        {
          TypeInfo ti (typeid (Inherits));
          ti.add_base (Access::PUBLIC, true, IsA::static_type_info ());
          return ti;
        }

        TypeInfo inherits_ (inherits_init_ ());
      }

      TypeInfo const& Inherits::
      static_type_info () { return inherits_; }


      // Initializes
      //
      //
      namespace
      {
        TypeInfo
        initializes_init_ ()
        {
          TypeInfo ti (typeid (Initializes));
          ti.add_base (Access::PUBLIC, true, Edge::static_type_info ());
          return ti;
        }

        TypeInfo initializes_ (initializes_init_ ());
      }

      TypeInfo const& Initializes::
      static_type_info () { return initializes_; }


      // Expression
      //
      //
      namespace
      {
        TypeInfo
        expression_init_ ()
        {
          TypeInfo ti (typeid (Expression));
          ti.add_base (Access::PUBLIC, true, Node::static_type_info ());
          return ti;
        }

        TypeInfo expression_ (expression_init_ ());
      }

      TypeInfo const& Expression::
      static_type_info () { return expression_; }

      Expression::
      ~Expression ()
      {
      }


      // Const
      //
      //
      namespace
      {
        TypeInfo
        const_init_ ()
        {
          TypeInfo ti (typeid (Const));
          ti.add_base (Access::PUBLIC, true, Instance::static_type_info ());
          ti.add_base (Access::PUBLIC, true, Nameable::static_type_info ());
          ti.add_base (Access::PUBLIC, true, Expression::static_type_info ());
          return ti;
        }

        TypeInfo const_ (const_init_ ());
      }

      TypeInfo const& Const::
      static_type_info () { return const_; }

      Const::
      ~Const ()
      {
      }

      // Contains
      //
      //
      namespace
      {
        TypeInfo
        contains_init_ ()
        {
          TypeInfo ti (typeid (Contains));
          ti.add_base (Access::PUBLIC, true, Edge::static_type_info ());
          return ti;
        }

        TypeInfo contains_ (contains_init_ ());
      }

      TypeInfo const& Contains::
      static_type_info () { return contains_; }


      // Container
      //
      //
      namespace
      {
        TypeInfo
        container_init_ ()
        {
          TypeInfo ti (typeid (Container));
          ti.add_base (Access::PUBLIC, true, Node::static_type_info ());
          return ti;
        }

        TypeInfo container_ (container_init_ ());
      }

      TypeInfo const& Container::
      static_type_info () { return container_; }

      Container::
      ~Container ()
      {
      }
    }
  }
}

//
//
std::ostream&
operator<< (std::ostream& os, CCF::IDL2::SemanticGraph::Path const& path)
{
  return os << path.native_file_string ();
}
