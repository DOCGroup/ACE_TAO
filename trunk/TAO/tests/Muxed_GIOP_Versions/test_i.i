// $Id test_i.i,v 1.1 1999/06/19 22:39:53 coryan Exp$

ACE_INLINE
Simple_Server_i::Simple_Server_i (CORBA::ORB_ptr orb)
  :  orb_ (CORBA::ORB::_duplicate (orb))
{
}
