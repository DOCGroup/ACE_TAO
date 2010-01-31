// $Id$

namespace CIAO
{
  ACE_INLINE PortableServer::POA_ptr
  Container_i::the_POA (void)
  {
    return this->component_poa_.in ();
  }

  ACE_INLINE PortableServer::POA_ptr
  Container_i::the_port_POA (void)
  {
    return this->facet_cons_poa_.in ();
  }

  ACE_INLINE CORBA::ORB_ptr
  Container_i::the_ORB (void) const
  {
    return this->orb_.in ();
  }
}
