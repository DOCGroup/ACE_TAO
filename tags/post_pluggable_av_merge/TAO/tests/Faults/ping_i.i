// $Id$

ACE_INLINE
Ping_i::Ping_i (CORBA::ORB_ptr orb,
                PortableServer::POA_ptr poa)
  :  orb_ (CORBA::ORB::_duplicate (orb)),
     poa_ (PortableServer::POA::_duplicate (poa))
{
}
