#include "Simple.h"

char *
Simple::get_string ()
{
  return CORBA::string_dup ("TAO Load Balancing Works!");
}
