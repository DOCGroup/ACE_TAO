#include "test_i.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_sys_time.h"
#include "ace/Time_Value.h"

test_i::test_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
test_i::method (CORBA::ULong request_number, CORBA::Long start_time,
                test::data const &, CORBA::ULong work)
{
  ACE_Time_Value start (0);
  start.msec (static_cast<long> (start_time));
  ACE_DEBUG ((LM_DEBUG,
              "server:\t%d took\t%Qms\n",
              request_number,
              static_cast<ACE_UINT64> ((ACE_OS::gettimeofday () - start).msec ())));

  // Time required to process this request. <work> is time units in
  // milli seconds.
  ACE_Time_Value const work_time (0, work * 1000);

  ACE_OS::sleep (work_time);
}

void
test_i::flush ()
{
  ACE_DEBUG ((LM_DEBUG, "server: got flush request\n"));
}

void
test_i::shutdown (CORBA::Long start_time)
{
  ACE_Time_Value start (0);
  start.msec (static_cast<long> (start_time));
  ACE_DEBUG ((LM_DEBUG, "server: Shutting down... (%Qms)\n",
              static_cast<ACE_UINT64> ((ACE_OS::gettimeofday () - start).msec ())));
  this->orb_->shutdown (false);
}
