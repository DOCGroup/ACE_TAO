//
// $Id$
//
#include "Hello.h"

ACE_RCSID(Hello, Hello, "$Id$")

Hello_i::Hello_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

char *
Hello_i::get_string (void)
{
  ACE_DEBUG ((LM_DEBUG, "returning string\n"));
  return CORBA::string_dup ("Hello there!");
}

void
Hello_i::shutdown (void)
{
  this->orb_->shutdown (0);
}
