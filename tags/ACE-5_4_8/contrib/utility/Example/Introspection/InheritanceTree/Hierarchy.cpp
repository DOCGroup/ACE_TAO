// file      : Hierarchy.cpp
// author    : Boris Kolpackov <boris@kolpackov.net>
// copyright : Copyright (c) 2002-2003 Boris Kolpackov
// license   : http://kolpackov.net/license.html

#include "Hierarchy.hpp"

// A
//
//

using Introspection::Object;
using Introspection::Access;

namespace
{
  TypeInfo
  a_init_ ()
  {
    TypeInfo ti (typeid (A));
    ti.add_base (Access::PUBLIC, true, Object::static_type_info ());
    return ti;
  }

  TypeInfo a_ (a_init_ ());
}

TypeInfo const& A::
static_type_info ()
{
  return a_;
}

// B
//
//

namespace
{
  TypeInfo
  b_init_ ()
  {
    TypeInfo ti (typeid (B));
    ti.add_base (Access::PUBLIC, false, A::static_type_info ());
    return ti;
  }

  TypeInfo b_ (b_init_ ());
}

TypeInfo const& B::
static_type_info ()
{
  return b_;
}

// C
//
//

namespace
{
  TypeInfo
  c_init_ ()
  {
    TypeInfo ti (typeid (C));
    ti.add_base (Access::PUBLIC, true, A::static_type_info ());
    return ti;
  }

  TypeInfo c_ (c_init_ ());
}

TypeInfo const& C::
static_type_info ()
{
  return c_;
}


// D
//
//

namespace
{
  TypeInfo
  d_init_ ()
  {
    TypeInfo ti (typeid (D));
    ti.add_base (Access::PUBLIC, true, B::static_type_info ());
    ti.add_base (Access::PUBLIC, false, C::static_type_info ());
    return ti;
  }

  TypeInfo d_ (d_init_ ());
}

TypeInfo const& D::
static_type_info ()
{
  return d_;
}
//$Id$
