/* -*- C++ -*- */
// $Id$

ACE_INLINE
CIAO::ServerActivator_Impl::ServerActivator_Impl (CORBA::ORB_ptr o,
                                                  PortableServer::POA_ptr p)
  : orb_ (CORBA::ORB::_duplicate (o)),
    poa_ (PortableServer::POA::_duplicate (p)),
    spawn_delay_ (5)
{
}

ACE_INLINE ::Components::Deployment::ServerActivator_ptr
CIAO::ServerActivator_Impl::_ciao_get_objref ()
{
  return this->objref_.in ();
}

ACE_INLINE const char *
CIAO::ServerActivator_Impl::_ciao_get_ior ()
{
  return this->ior_.in ();
}
