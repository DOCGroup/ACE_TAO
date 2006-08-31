// $Id$

#include "DB_Parameter.h"

#if !defined (__CUTS_INLINE__)
#include "DB_Parameter.inl"
#endif

//
// CUTS_DB_Parameter
//
CUTS_DB_Parameter::CUTS_DB_Parameter (void)
: index_ (-1),
  type_ (PT_UNKNOWN),
  null_ (0)
{

}

//
// CUTS_DB_Parameter
//
CUTS_DB_Parameter::~CUTS_DB_Parameter (void)
{

}
