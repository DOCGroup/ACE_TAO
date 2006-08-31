// $Id$

#include "BE_File_Generator.h"

#if !defined (__CUTS_INLINE__)
#include "BE_File_Generator.inl"
#endif

#include <cstdlib>
#include <algorithm>

//
// CUTS_BE_File_Generator
//
CUTS_BE_File_Generator::CUTS_BE_File_Generator (void)
{

}

//
// ~CUTS_BE_File_Generator
//
CUTS_BE_File_Generator::~CUTS_BE_File_Generator (void)
{

}

//
// to_lower
//
CUTS_INLINE
void CUTS_BE_File_Generator::to_lower (std::string & str)
{
  std::transform (str.begin (), str.end (), str.begin (), tolower);
}

//
// to_upper
//
CUTS_INLINE
void CUTS_BE_File_Generator::to_upper (std::string & str)
{
  std::transform (str.begin (), str.end (), str.begin (), toupper);
}
