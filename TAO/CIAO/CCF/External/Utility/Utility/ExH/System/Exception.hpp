// file      : Utility/ExH/System/Exception.hpp
// author    : Boris Kolpackov <boris@kolpackov.net>
// copyright : Copyright (c) 2002-2003 Boris Kolpackov
// license   : http://kolpackov.net/license.html

#ifndef UTILITY_EX_H_SYSTEM_EXCEPTION_HPP
#define UTILITY_EX_H_SYSTEM_EXCEPTION_HPP

#include <exception>

namespace Utility
{
  namespace ExH
  {
    namespace System
    {
      // This is the only way to make predefined exceptions like
      // std::bad_alloc, etc to appear in the right place of the hierarchy.
      //

      typedef std::exception Exception;
    }
  }
}

#endif  // UTILITY_EX_H_SYSTEM_EXCEPTION_HPP


