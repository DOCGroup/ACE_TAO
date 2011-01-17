//
// $Id$
//
#include "Hello.h"



Hello::Hello (CORBA::ORB_ptr orb, Test::Hello_ptr, CORBA::ULong)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
Hello::shutdown (void)
{
  this->orb_->shutdown (0);
}

void
Hello::ping (void)
{
  return;
}

void
Hello::throw_location_forward (void)
{
  return;
}
