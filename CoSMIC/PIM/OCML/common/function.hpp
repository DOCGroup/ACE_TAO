// -*- C++ -*-

/**
 * @file XercesHelpers.hpp
 *
 * $Id$
 *
 * @author Emre Turkay <turkaye@dre.vanderbilt.edu>
 */

#ifndef _COMMON_FUNCTION_HPP_
#define _COMMON_FUNCTION_HPP_

#include <string>

namespace common
{
  /// Used to sort strings
  struct ltstr
  {
    /// Compare two const char*
    bool operator() (const char *s1, const char *s2) const;

    /// Compare two std::string
    bool operator() (const std::string & s1, const std::string & s2) const;
  };

} // namespace common

#include "function.ipp"

#endif // _COMMON_FUNCTION_HPP_
