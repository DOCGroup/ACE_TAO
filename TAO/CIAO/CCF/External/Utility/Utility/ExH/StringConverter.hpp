// file      : Utility/ExH/StringConverter.hpp
// author    : Boris Kolpackov <boris@kolpackov.net>
// copyright : Copyright (c) 2002-2003 Boris Kolpackov
// license   : http://kolpackov.net/license.html

#ifndef UTILITY_EX_H_STRING_CONVERTER_HPP
#define UTILITY_EX_H_STRING_CONVERTER_HPP

#include <string>

#include "Utility/ExH/Converter.hpp"

namespace Utility
{
  namespace ExH
  {
    template <>
    char const*
    converter (std::string const& t) throw ();
  }
}

#include "Utility/ExH/StringConverter.ipp"

#endif  // UTILITY_EX_H_STRING_CONVERTER_HPP

