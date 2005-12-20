// file      : Example/Introspection/Traversal/SyntaxTree.cpp
// author    : Boris Kolpackov <boris@kolpackov.net>
// copyright : Copyright (c) 2002-2003 Boris Kolpackov
// license   : http://kolpackov.net/license.html

#include "SyntaxTree.hpp"

using namespace Utility::Introspection;

namespace SyntaxTree
{

  // Node
  //
  //

  namespace
  {
    TypeInfo
    node_init_ ()
    {
      TypeInfo ti (typeid (Node));
      ti.add_base (Access::PUBLIC, true, Object::static_type_info ());
      return ti;
    }

    TypeInfo node_ (node_init_ ());
  }

  TypeInfo const& Node::
  static_type_info () { return node_; }


  // Declaration
  //
  //

  namespace
  {
    TypeInfo
    declaration_init_ ()
    {
      TypeInfo ti (typeid (Declaration));
      ti.add_base (Access::PUBLIC, true, Node::static_type_info ());
      return ti;
    }

    TypeInfo declaration_ (declaration_init_ ());
  }

  TypeInfo const& Declaration::
  static_type_info () { return declaration_; }


  // Scope
  //
  //

  namespace
  {
    TypeInfo
    scope_init_ ()
    {
      TypeInfo ti (typeid (Scope));
      ti.add_base (Access::PUBLIC, true, Declaration::static_type_info ());
      return ti;
    }

    TypeInfo scope_ (scope_init_ ());
  }

  TypeInfo const& Scope::
  static_type_info () { return scope_; }


  // InterfaceDecl
  //
  //

  namespace
  {
    TypeInfo
    interface_decl_init_ ()
    {
      TypeInfo ti (typeid (InterfaceDecl));
      ti.add_base (Access::PUBLIC, true, Declaration::static_type_info ());
      return ti;
    }

    TypeInfo interface_decl_ (interface_decl_init_ ());
  }

  TypeInfo const& InterfaceDecl::
  static_type_info () { return interface_decl_; }


  // InterfaceDef
  //
  //

  namespace
  {
    TypeInfo
    interface_def_init_ ()
    {
      TypeInfo ti (typeid (InterfaceDef));
      ti.add_base (Access::PUBLIC, true, InterfaceDecl::static_type_info ());
      ti.add_base (Access::PUBLIC, true, Scope::static_type_info ());
      return ti;
    }

    TypeInfo interface_def_ (interface_def_init_ ());
  }

  TypeInfo const& InterfaceDef::
  static_type_info () { return interface_def_; }

}
//$Id$
