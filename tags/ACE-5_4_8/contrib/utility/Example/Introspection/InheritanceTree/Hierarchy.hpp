// file      : Example/Introspection/InheritanceTree/Hierarchy.hpp
// author    : Boris Kolpackov <boris@kolpackov.net>
// copyright : Copyright (c) 2002-2003 Boris Kolpackov
// license   : http://kolpackov.net/license.html

#ifndef HIERARCHY_HPP
#define HIERARCHY_HPP

#include "Utility/Introspection/Introspection.hpp"

namespace Introspection = Utility::Introspection;

using Introspection::TypeInfo;
using Introspection::TypeId;

struct A : virtual Introspection::Object
{
  A ()
  {
    type_info (static_type_info ());
  }

  static TypeInfo const&
  static_type_info ();
};

struct B : virtual A
{
  B ()
  {
    type_info (static_type_info ());
  }

  static TypeInfo const&
  static_type_info ();
};

struct C : virtual A
{
  C ()
  {
    type_info (static_type_info ());
  }

  static TypeInfo const&
  static_type_info ();
};

struct D : virtual B, C
{
  D ()
  {
    type_info (static_type_info ());
  }

  static TypeInfo const&
  static_type_info ();
};

#endif  // HIERARCHY_HPP
//$Id$
