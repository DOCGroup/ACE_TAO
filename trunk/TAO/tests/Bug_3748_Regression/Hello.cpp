//
// $Id$
//
#include "Hello.h"

ACE_RCSID(Hello, Hello, "$Id$")

Hello::Hello (CORBA::ORB_ptr orb, CORBA::ORB_ptr shutdown_orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
  , shutdown_orb_ (CORBA::ORB::_duplicate (shutdown_orb))
{
}

char *
Hello::get_string (void)
{
  return CORBA::string_dup ("Hello there!");
}

void
Hello::shutdown (void)
{
  this->orb_->shutdown (0);
  this->shutdown_orb_->shutdown (0);
}
