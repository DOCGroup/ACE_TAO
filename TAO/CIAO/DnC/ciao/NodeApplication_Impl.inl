/* -*- C++ -*- */
// $Id$

ACE_INLINE
CIAO::NodeApplication_Impl::
NodeApplication_Impl (CORBA::ORB_ptr o,
		      PortableServer::POA_ptr p)

  : orb_ (CORBA::ORB::_duplicate (o)),
    poa_ (PortableServer::POA::_duplicate (p))
{
}
