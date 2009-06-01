// file      : Utility/Hetero/TypedContainer.hpp
// author    : Boris Kolpackov <boris@kolpackov.net>
// copyright : Copyright (c) 2002-2003 Boris Kolpackov
// license   : http://kolpackov.net/license.html

#ifndef UTILITY_HETERO_TYPED_CONTAINER_HPP
#define UTILITY_HETERO_TYPED_CONTAINER_HPP

#include "Utility/Hetero/Container.hpp"
#include "Utility/Hetero/TypeList.hpp"

namespace Utility
{
  namespace Hetero
  {
    template <typename TL>
    class TypedContainer : public Container
    {
    public:
      typedef TL Types;

    public:
      template <typename T>
      TypedContainer (T const& t)
          : Container (t)
      {
      }

      TypedContainer (TypedContainer const& c)
          : Container (static_cast<Container const&> (c))
      {
      }

    public:
      template <typename T>
      TypedContainer&
      operator= (T const& t)
      {
        Container& self = *this;
        self = t;
        return *this;
      }

      TypedContainer&
      operator= (TypedContainer const& c)
      {
        Container& self = *this;
        Container const& other = c;
        self = other;
        return *this;
      }
    };
  }
}

#endif  // UTILITY_HETERO_TYPED_CONTAINER_HPP
//$Id$
