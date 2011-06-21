// $Id$

#include "Simple.h"

Simple::Simple (void)
{
}

char *
Simple::get_string (void)
{
  return CORBA::string_dup ("TAO Load Balancing Works!");
}
