// file      : Utility/Hetero/TypeList.hpp
// author    : Boris Kolpackov <boris@kolpackov.net>
// copyright : Copyright (c) 2002-2003 Boris Kolpackov
// license   : http://kolpackov.net/license.html

#ifndef UTILITY_HETERO_TYPE_LIST_HPP
#define UTILITY_HETERO_TYPE_LIST_HPP

namespace Utility
{
  namespace Hetero
  {
    class NullType {};

    template <typename t0 = NullType,
              typename t1 = NullType,
              typename t2 = NullType>
    struct TypeList
    {
      typedef t0 T0;
      typedef t1 T1;
      typedef t2 T2;
    };

    template <typename t0, typename t1>
    struct TypeList<t0, t1, NullType>
    {
      typedef t0 T0;
      typedef t1 T1;
    };

    template <typename t0>
    struct TypeList<t0, NullType, NullType>
    {
      typedef t0 T0;
    };

    template <>
    struct TypeList<NullType, NullType, NullType>
    {
    };
  }
}

#endif  // UTILITY_HETERO_TYPE_LIST_HPP
//$Id$
