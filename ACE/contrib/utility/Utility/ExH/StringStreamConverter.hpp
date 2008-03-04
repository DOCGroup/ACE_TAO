// file      : Utility/ExH/StringStreamConverter.hpp
// author    : Boris Kolpackov <boris@kolpackov.net>
// copyright : Copyright (c) 2002-2003 Boris Kolpackov
// license   : http://kolpackov.net/license.html

#ifndef UTILITY_EX_H_STRING_STREAM_CONVERTER_HPP
#define UTILITY_EX_H_STRING_STREAM_CONVERTER_HPP

#include <sstream>

#include "Utility/ExH/Converter.hpp"

namespace Utility
{
  namespace ExH
  {
    template <>
    std::string
    converter (std::ostringstream const& t);
  }
}

#include "Utility/ExH/StringStreamConverter.ipp"

#endif  // UTILITY_EX_H_STRING_STREAM_CONVERTER_HPP
//$Id$
