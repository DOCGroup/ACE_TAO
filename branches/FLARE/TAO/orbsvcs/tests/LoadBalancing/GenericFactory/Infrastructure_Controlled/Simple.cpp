#include "Simple.h"


ACE_RCSID (Infrastructure_Controlled,
           Simple,
           "$Id$")


Simple::Simple (void)
{
}

char *
Simple::get_string (void)
{
  return CORBA::string_dup ("TAO Load Balancing Works!");
}
