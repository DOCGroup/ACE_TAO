// $Id$

#include "test_i.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_sys_time.h"
#include "ace/Time_Value.h"

test_i::test_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
test_i::method (CORBA::ULong request_number,
                CORBA::Long start_time,
                const test::data &,
                CORBA::ULong work)
{
  ACE_Time_Value start (0);
  // HPUX seems to require this cast
  start.msec (static_cast<long> (start_time));
  ACE_DEBUG ((LM_DEBUG,
              "server:\t%d took\t%dms\n",
              request_number,
              (ACE_OS::gettimeofday () - start).msec ()));

  // Time required to process this request. <work> is time units in
  // milli seconds.
  ACE_Time_Value work_time (0, work * 1000);

  ACE_OS::sleep (work_time);
}

void
test_i::flush (void)
{
  ACE_DEBUG ((LM_DEBUG, "server: got flush request\n"));
}

void
test_i::shutdown (CORBA::Long start_time)
{
  ACE_Time_Value start (0);
  start.msec (static_cast<long> (start_time)); // HPUX seems to require this cast
  ACE_DEBUG ((LM_DEBUG, "server: Shutting down... (%dms)\n",
              (ACE_OS::gettimeofday() - start).msec ()));
  this->orb_->shutdown (0);
}
