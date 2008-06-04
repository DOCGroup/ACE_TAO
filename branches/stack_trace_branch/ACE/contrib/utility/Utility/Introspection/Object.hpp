// file      : Utility/Introspection/Object.hpp
// author    : Boris Kolpackov <boris@kolpackov.net>
// copyright : Copyright (c) 2002-2003 Boris Kolpackov
// license   : http://kolpackov.net/license.html

#ifndef UTILITY_INTROSPECTION_OBJECT_HPP
#define UTILITY_INTROSPECTION_OBJECT_HPP

#include "Utility/Introspection/TypeInfo.hpp"

namespace Utility
{
  namespace Introspection
  {
    class Object
    {
    public:
      static TypeInfo const&
      static_type_info () throw ();

      virtual TypeInfo const&
      type_info () const throw ();

    protected:
      virtual
      ~Object ();

      Object () throw ();
      Object (Object const&) throw ();

      Object&
      operator= (Object const&) throw ();

    protected:
      virtual void
      type_info (TypeInfo const& tid) throw ();

    private:
      TypeInfo const* type_info_;
    };
  }
}

#include "Utility/Introspection/Object.ipp"

#endif  // UTILITY_INTROSPECTION_OBJECT_HPP
//$Id$
