// $Id$

ACE_INLINE
Simple_Server_i::Simple_Server_i (CORBA::ORB_ptr orb)
  :  orb_ (CORBA::ORB::_duplicate (orb)),
     mutex_ (),
     validated_ (Simple_Server_i::NOT_VALIDATED)
{
}
