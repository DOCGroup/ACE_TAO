// $Id$

ACE_INLINE
Test_i::Test_i (CORBA::ORB_ptr orb,
                int workload)
  :  orb_ (CORBA::ORB::_duplicate (orb)),
     workload_ (workload)
{
}
