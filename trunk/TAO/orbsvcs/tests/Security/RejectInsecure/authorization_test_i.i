// $Id$

ACE_INLINE
Authorization_Server_i::Authorization_Server_i (CORBA::ORB_ptr orb)
  :  orb_ (CORBA::ORB::_duplicate (orb))
{
}

