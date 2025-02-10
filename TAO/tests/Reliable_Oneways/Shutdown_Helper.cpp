#include "Shutdown_Helper.h"

Shutdown_Helper::Shutdown_Helper (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
Shutdown_Helper::shutdown ()
{
  this->orb_->shutdown (false);
}
