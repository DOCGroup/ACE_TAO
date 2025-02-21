#include "Simple.h"

Simple::Simple ()
{
}

char *
Simple::get_string ()
{
  return CORBA::string_dup ("TAO Load Balancing Works!");
}
