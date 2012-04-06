//
// $Id$
//
#include "Hello.h"



Hello::Hello (CORBA::ORB_ptr orb, Test::Hello_ptr, CORBA::ULong)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
Hello::shutdown ()
{
  this->orb_->shutdown (0 );
}

void
Hello::ping ()
{
  return;
}

void
Hello::throw_location_forward ()
{
  return;
}
