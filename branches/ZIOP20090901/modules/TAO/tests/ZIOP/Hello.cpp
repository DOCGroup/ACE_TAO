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
Hello::get_string (const char * mystring)
{
  ACE_DEBUG ((LM_DEBUG, "Received <%s>\n", mystring));
  return CORBA::string_dup ("Hello there!Hello there!Hello there!Hello there!Hello there!Hello there!Hello there!Hello there!Hello there!\n");
}

void
Hello::shutdown (void)
{
  this->orb_->shutdown (0);
}
