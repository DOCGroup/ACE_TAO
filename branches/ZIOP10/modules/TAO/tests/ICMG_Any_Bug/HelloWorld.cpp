//
// $Id$
//

#include "HelloWorld.h"


ACE_RCSID (ICMG_Any_Bug, HelloWorld, "$Id$")


HelloWorld::HelloWorld (void)
{
}

char *
HelloWorld::get_string (void)
{
  return CORBA::string_dup ("TAO Any Implementation Works!");
}
