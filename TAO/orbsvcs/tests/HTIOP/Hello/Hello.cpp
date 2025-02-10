#include "Hello.h"


Hello::Hello (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

char *
Hello::get_string ()
{
  ACE_DEBUG ((LM_DEBUG, "in get_string\n"));
  return CORBA::string_dup ("Hello there!");
}

void
Hello::shutdown ()
{
  this->orb_->shutdown (false);
  ACE_DEBUG ((LM_DEBUG,"in shutdown\n"));
}
