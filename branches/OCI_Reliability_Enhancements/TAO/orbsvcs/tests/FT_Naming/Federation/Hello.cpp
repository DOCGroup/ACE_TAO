//
// $Id$
//
#include "Hello.h"



Hello::Hello (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

char *
Hello::get_string (void)
{
  return CORBA::string_dup ( ACE_TEXT ("Hello there!"));
}

void
Hello::shutdown (void)
{
  this->orb_->shutdown (0);
}
