// file      : Utility/Introspection/TypeId.hpp
// author    : Boris Kolpackov <boris@kolpackov.net>
// copyright : Copyright (c) 2002-2003 Boris Kolpackov
// license   : http://kolpackov.net/license.html

#ifndef UTILITY_INTROSPECTION_TYPE_ID_HPP
#define UTILITY_INTROSPECTION_TYPE_ID_HPP

#include <typeinfo>
#include <iosfwd>

namespace Utility
{
  namespace Introspection
  {
    class TypeId
    {
    public:
      template<typename T>
      TypeId (T const& t);

      TypeId (std::type_info const& tid);

    public:
      bool
      operator == (TypeId const& other) const;

      bool
      operator != (TypeId const& other) const;

      bool
      operator < (TypeId const& other) const;

      friend std::ostream&
      operator << (std::ostream& os, TypeId const& t);

    private:
      std::type_info const* tid_;
    };
  }
}

#include "Utility/Introspection/TypeId.tpp"
#include "Utility/Introspection/TypeId.ipp"

#endif  // UTILITY_INTROSPECTION_TYPE_ID_HPP
//$Id$
