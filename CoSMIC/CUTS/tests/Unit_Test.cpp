// $Id$

#include "Unit_Test.h"

//
// CUTS_Unit_Test
//
CUTS_Unit_Test::CUTS_Unit_Test (const char * name)
: name_ (name)
{

}

//
// ~CUTS_Unit_Test
//
CUTS_Unit_Test::~CUTS_Unit_Test (void)
{

}

//
// message
//
const char * CUTS_Unit_Test::message (void) const
{
  return this->message_.c_str ();
}

//
// name
//
const char * CUTS_Unit_Test::name (void) const
{
  return this->name_.c_str ();
}
