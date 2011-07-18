//
// $Id$
//
#include "Hello.h"

ACE_RCSID(Hello, Hello, "$Id$")

Hello::Hello (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

char *
Hello::get_string (void)
{
  return CORBA::string_dup ("Hello there!");
}

