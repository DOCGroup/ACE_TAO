// file      : CCF/IDL2/SemanticGraph/Elements.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL2/SemanticGraph/Elements.hpp"

//@@ move this stuff into namespace
using Introspection::TypeInfo;
using Introspection::Access;

//@@ tmp
#include <iostream>
using std::cerr;
using std::endl;

namespace CCF
{
  namespace IDL2
  {
    namespace SemanticGraph
    {
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
      Node ()
      {
        type_info (static_type_info ());
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


      // Scope
      //
      //

      Nameables Scope::
      lookup (Name const& name)
      {
        Nameables r;
        lookup (name, r);
        return r;
      }

      void Scope::
      lookup (Name const& name, Nameables& result)
      {
        SimpleName first (*name.begin ());

        NamesIteratorPair pair (find (first));

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


      // TypeTemplateSpecialization
      //
      //
      namespace
      {
        TypeInfo
        type_template_specialization_init_ ()
        {
          TypeInfo ti (typeid (TypeTemplateSpecialization));
          ti.add_base (Access::PUBLIC, true, Type::static_type_info ());
          return ti;
        }

        TypeInfo type_template_specialization_ (
          type_template_specialization_init_ ());
      }

      TypeInfo const& TypeTemplateSpecialization::
      static_type_info () { return type_template_specialization_; }


      // Specialized
      //
      //
      namespace
      {
        TypeInfo
        specialized_init_ ()
        {
          TypeInfo ti (typeid (Specialized));
          ti.add_base (Access::PUBLIC, true, Edge::static_type_info ());
          return ti;
        }

        TypeInfo specialized_ (specialized_init_ ());
      }

      TypeInfo const& Specialized::
      static_type_info () { return specialized_; }


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
    }
  }
}
