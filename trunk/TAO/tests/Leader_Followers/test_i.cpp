// $Id$

#include "test_i.h"

ACE_RCSID(Leader_Followers, test_i, "$Id$")

test_i::test_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
test_i::shutdown (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC (())
{
  this->orb_->shutdown (0,
                        ACE_TRY_ENV);
}

CORBA::ULong
test_i::method (CORBA::ULong work,
                CORBA::ULong requesting_thread_id,
                CORBA::Environment &)
  ACE_THROW_SPEC (())
{
  ACE_DEBUG ((LM_DEBUG,
              "Server: Working for %d milli seconds requested by thread %d @ %T\n",
              work,
              requesting_thread_id));

  // Time required to process this request. <work> is time units in
  // milli seconds.
  ACE_Time_Value work_time (0,
                            work * 1000);

  ACE_OS::sleep (work_time);

  return work;
}
