#include "Hello.h"

Hello::Hello (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

char *
Hello::get_string (char const *a, char const *b, char const *c)
{
  ACE_DEBUG ((LM_DEBUG, "Received: <%C>, <%C>, <%C>\n", a, b, c));
  return CORBA::string_dup ("Hello there!");
}

void
Hello::shutdown ()
{
  this->orb_->shutdown (false);
}
