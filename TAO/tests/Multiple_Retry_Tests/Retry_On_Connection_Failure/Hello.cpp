#include "Hello.h"
#include "ace/Log_Msg.h"

Hello::Hello (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

char *
Hello::get_string ()
{
  return CORBA::string_dup ("Hello there!");
}

void
Hello::set_string (char const *s)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Call to Hello::set_string (%C)\n"),
              s));
}

void
Hello::shutdown ()
{
  this->orb_->shutdown (false);
}
