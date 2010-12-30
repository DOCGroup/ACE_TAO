// file      : Example/Introspection/Traversal/SyntaxTree.hpp
// author    : Boris Kolpackov <boris@kolpackov.net>
// copyright : Copyright (c) 2002-2003 Boris Kolpackov
// license   : http://kolpackov.net/license.html

#ifndef SYNTAX_TREE_HPP
#define SYNTAX_TREE_HPP

#include <vector>

#include "Utility/Introspection/Introspection.hpp"

namespace SyntaxTree
{
  //
  //
  //

  struct Node : virtual Utility::Introspection::Object
  {
    Node ()
    {
      type_info (static_type_info ());
    }

    static Utility::Introspection::TypeInfo const&
    static_type_info ();
  };

  //
  //
  //

  struct Declaration : virtual Node
  {
    Declaration ()
    {
      type_info (static_type_info ());
    }

    static Utility::Introspection::TypeInfo const&
    static_type_info ();
  };

  typedef
  std::vector<Declaration*>
  DeclarationList;

  //
  //
  //
  struct Scope : virtual Declaration
  {
    Scope ()
    {
      type_info (static_type_info ());
    }

    static Utility::Introspection::TypeInfo const&
    static_type_info ();

    DeclarationList content_;
  };

  //
  //
  //
  struct InterfaceDecl : virtual Declaration
  {
    InterfaceDecl ()
    {
      type_info (static_type_info ());
    }

    static Utility::Introspection::TypeInfo const&
    static_type_info ();
  };

  //
  //
  //
  struct InterfaceDef : virtual InterfaceDecl, virtual Scope
  {
    InterfaceDef ()
    {
      type_info (static_type_info ());
    }

    static Utility::Introspection::TypeInfo const&
    static_type_info ();
  };
}

#endif  // SYNTAX_TREE_HPP
//$Id$
