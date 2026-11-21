#include "Hello.h"

Hello::Hello (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
Hello::shutdown ()
{
  this->orb_->shutdown (false);
}

void
Hello::ping ()
{
}

void
Hello::throw_location_forward ()
{
}
