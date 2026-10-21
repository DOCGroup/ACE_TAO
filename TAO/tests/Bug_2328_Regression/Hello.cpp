#include "Hello.h"

Hello::Hello (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

char *
Hello::get_string (char const *A, char const *B, char const *C)
{
  ACE_DEBUG ((LM_DEBUG, "Received: <%C>, <%C>, <%C>\n", A, B, C));
  return CORBA::string_dup ("Hello there!");
}

void
Hello::shutdown ()
{
  this->orb_->shutdown (false);
}
