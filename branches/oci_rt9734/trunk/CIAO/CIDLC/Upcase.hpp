// file      : CIDLC/Upcase.hpp
// author    : Jeff Parsons <j.parsons@vanderbilt.edu>
// cvs-id    : $Id$

#ifndef UPCASE_HPP
#define UPCASE_HPP

#include <cctype>

namespace
{
  // On some platforms toupper can be something other than a
  // function.
  int
  upcase (int c)
  {
    return std::toupper (c);
  }
}

#endif  // UPCASE_HPP
