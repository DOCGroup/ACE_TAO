//
// $Id$
//
#include "NsShutdown.h"

ACE_RCSID(Hello,
          NsShutdown,
          "$Id$")

NsShutdown::NsShutdown (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
NsShutdown::shutdown (void)
{
  this->orb_->shutdown (0);
}
