/* -*- C++ -*- */
// $Id$

ACE_INLINE
CIAO::AssemblyFactory_Impl::AssemblyFactory_Impl (CORBA::ORB_ptr o)
  : orb_ (o)
{
}

ACE_INLINE
CIAO::Assembly_Impl::Assembly_Impl (CORBA::ORB_ptr o)
  : orb_ (o)
{
}
