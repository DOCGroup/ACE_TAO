/* -*- C++ -*- */
// $Id$

ACE_INLINE
CIAO::NodeDaemon_Impl::NodeDaemon_Impl (const char *name,
					CORBA::ORB_ptr o,
					PortableServer::POA_ptr p)
  : orb_ (CORBA::ORB::_duplicate (o)),
    poa_ (PortableServer::POA::_duplicate (p)),
    name_ (CORBA::string_dup (name)),
    callback_poa_ (PortableServer::POA::_nil ())
{
}
