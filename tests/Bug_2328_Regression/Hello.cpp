//
// $Id$
//
#include "Hello.h"

Hello::Hello (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

char *
Hello::get_string (const char * A, const char * B, const char * C)
{
  ACE_DEBUG ((LM_DEBUG, "Received: <%C>, <%C>, <%C>\n", A, B, C));
  return CORBA::string_dup ("Hello there!");
}

void
Hello::shutdown (void)
{
  this->orb_->shutdown (0);
}
