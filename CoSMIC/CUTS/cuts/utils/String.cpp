// $Id$

#include "String.h"
#include <algorithm>
#include <ctype.h>

//
// normalize
//
std::string String::normalize (std::string str, char replace)
{
  for ( std::string::iterator iter = str.begin ();
        iter != str.end ();
        iter ++)
  {
    if (!isalnum (*iter))
      *iter = replace;
  }

  return str;
}

//
// uppercase
//
std::string String::uppercase (std::string str)
{
  std::transform (str.begin (), str.end (), str.begin (), toupper);
  return str;
}
