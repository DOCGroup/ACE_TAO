#include "Hello.h"
#include "ace/OS_NS_unistd.h"

Hello::Hello (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

char *
Hello::get_string ()
{
  ACE_DEBUG((LM_DEBUG, ACE_TEXT("%D get_string: sleep 5 seconds before returning\n")));
  ACE_OS::sleep (5);
  return CORBA::string_dup ("Hello there!");
}

void
Hello::shutdown ()
{
  this->orb_->shutdown (false);
}
