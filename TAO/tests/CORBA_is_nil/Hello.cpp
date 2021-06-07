#include "Hello.h"

Hello::Hello (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

char *
Hello::get_string ()
{
  return CORBA::string_dup ("Hello there!");
}

Test::Hello_ptr
Hello::get_Hello ()
{
   return Test::Hello::_nil();
}

void
Hello::shutdown ()
{
  this->orb_->shutdown (false);
}
