/* -*- C++ -*- */
// $Id$

ACE_INLINE
CIAO::NodeApplicationManager_Impl::
NodeApplicationManager_Impl (CORBA::ORB_ptr o,
			     PortableServer::POA_ptr p)
  : orb_ (CORBA::ORB::_duplicate (o)),
    poa_ (PortableServer::POA::_duplicate (p)),
    callback_poa_ (PortableServer::POA::_nil ()),
    spawn_delay_ (5)
{
}

Deployment::NodeApplicationManager_ptr
CIAO::NodeApplicationManager_Impl::
get_nadeapp_manager (void)
{
  return this->objref_;
}
