
#include "HelloWorld.h"

HelloWorld::HelloWorld ()
{
}

char *
HelloWorld::get_string ()
{
  return CORBA::string_dup ("TAO Any Implementation Works!");
}
