#include "Ping_Service.h"

Ping_Service::Ping_Service (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
Ping_Service::ping ()
{
}

void
Ping_Service::shutdown ()
{
  this->orb_->shutdown (false);
}
