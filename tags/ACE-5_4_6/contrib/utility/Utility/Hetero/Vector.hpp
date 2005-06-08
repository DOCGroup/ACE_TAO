// file      : Utility/Hetero/Vector.hpp
// author    : Boris Kolpackov <boris@kolpackov.net>
// copyright : Copyright (c) 2002-2003 Boris Kolpackov
// license   : http://kolpackov.net/license.html

#ifndef UTILITY_HETERO_VECTOR_HPP
#define UTILITY_HETERO_VECTOR_HPP

#include <vector>

#include "Utility/Hetero/TypeList.hpp"
#include "Utility/Hetero/TypedContainer.hpp"

namespace Utility
{
  namespace Hetero
  {
    template <typename T0 = NullType,
              typename T1 = NullType,
              typename T2 = NullType>
    class Vector;

    template <>
    class Vector<NullType, NullType, NullType>;

    template <typename T0>
    class Vector<T0, NullType, NullType> :
      public std::vector<TypedContainer<TypeList<T0> > >
    {
    };

    template <typename T0, typename T1>
    class Vector<T0, T1, NullType> :
      public std::vector<TypedContainer<TypeList<T0, T1> > >
    {
    };

    template <typename T0, typename T1, typename T2>
    class Vector :
      public std::vector<TypedContainer<TypeList<T0, T1, T2> > >
    {
    };
  }
}

#endif  // UTILITY_HETERO_VECTOR_HPP
//$Id$
