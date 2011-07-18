// $Id$

#include "Test_impl.h"

ACE_RCSID(Bug_3444_Regression, Test_impl, "$Id$")

Server_impl::Server_impl (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void Server_impl::shutdown ()
{
  this->orb_->shutdown (0);
}
