//
// $Id$
//

#include "HelloWorld.h"

HelloWorld::HelloWorld (void)
{
}

char *
HelloWorld::get_string (void)
{
  return CORBA::string_dup ("TAO Any Implementation Works!");
}
