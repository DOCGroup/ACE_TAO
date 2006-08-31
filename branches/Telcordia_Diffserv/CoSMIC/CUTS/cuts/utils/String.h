// -*- C++ -*-

//=============================================================================
/**
 * @file    String.h
 *
 * $Id$
 *
 * [insert desciption here]
 *
 * @author James H. Hill <hillj@isis.vanderbilt.edu>
 */
//=============================================================================

#ifndef _CUTS_STRING_H_
#define _CUTS_STRING_H_

#include "Utils_Export.h"
#include <string>

//=============================================================================
/**
 * @class String
 */
//=============================================================================

class CUTS_UTILS_Export String
{
public:
  /**
   * Normalize a string. This will method will remove all invalid
   * characters from a string by converting them to an underscore.
   */
  static std::string normalize (std::string str, char replace);

  static std::string uppercase (std::string str);
};

#endif  // !defined _CUTS_STRING_H_
