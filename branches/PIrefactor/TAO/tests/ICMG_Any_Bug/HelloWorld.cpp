//
// $Id$
//

#include "HelloWorld.h"


ACE_RCSID (ICMG_Any_Bug, HelloWorld, "$Id$")


HelloWorld::HelloWorld (void)
{
}

char *
HelloWorld::get_string (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup ("TAO Any Implementation Works!");
}
