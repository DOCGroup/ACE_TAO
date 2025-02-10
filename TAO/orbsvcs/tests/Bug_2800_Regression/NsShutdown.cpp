#include "NsShutdown.h"

NsShutdown::NsShutdown (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
NsShutdown::shutdown ()
{
  this->orb_->shutdown (false);
}
