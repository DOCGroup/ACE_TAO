// $Id$

#include "Test_impl.h"

void Server_impl::shutdown ()
{
  this->orb_->shutdown (0);
}

Server_impl::Server_impl (CORBA::ORB_ptr orb)
    : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void Server_impl::method (CORBA::Object_ptr)
{
}
