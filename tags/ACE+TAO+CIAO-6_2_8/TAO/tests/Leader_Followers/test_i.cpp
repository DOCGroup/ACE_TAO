// $Id$

#include "test_i.h"
#include "ace/OS_NS_unistd.h"

test_i::test_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
test_i::shutdown (void)
{
  this->orb_->shutdown (0);
}

CORBA::ULong
test_i::method (CORBA::ULong work)
{
  ACE_DEBUG ((LM_DEBUG,
              "Server: Working for %d milli seconds @ %T\n",
              work));

  // Time required to process this request. <work> is time units in
  // milli seconds.
  ACE_Time_Value work_time (0,
                            work * 1000);

  ACE_OS::sleep (work_time);

  return work;
}
