// -*- C++ -*-
//
// $Id$


ACE_INLINE CORBA::ORB_ptr
TAO_DLL_ORB::orb (void)
{
  return CORBA::ORB::_duplicate (this->orb_.in());
}
