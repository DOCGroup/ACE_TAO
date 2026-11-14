#include "HelloWorld.h"

char *
HelloWorld::get_string ()
{
  return CORBA::string_dup ("TAO Any Implementation Works!");
}
