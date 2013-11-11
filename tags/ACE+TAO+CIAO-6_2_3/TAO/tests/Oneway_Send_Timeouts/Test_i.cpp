// $Id$

#include "Test_i.h"

#include "ace/High_Res_Timer.h"
#include "ace/OS_NS_unistd.h"

Test_i::Test_i (CORBA::ORB_ptr orb)
  : sleep_ (false), unsleep_ (true), shutdown_ (false)
  , orb_ (CORBA::ORB::_duplicate (orb))
{ }

Test_i::~Test_i ()
{
  if (!shutdown_) {
    this->shutdown ();
  }
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) ~Test_i>\n"));
}

void
Test_i::sleep (CORBA::Long sec, CORBA::Long msec)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Test_i::sleep> enter\n"));
  sleep_ = true;
  unsleep_ = false;

  ACE_Time_Value duration (sec);
  duration.msec (static_cast<long> (msec)); // HPUX seems to require this cast

  if (sec == 0 && msec == 0) {
    // 1000 seconds is infinity
    duration.sec (1000);
  }

  ACE_Time_Value current = ACE_High_Res_Timer::gettimeofday_hr ();
  ACE_Time_Value timeout = current + duration;

  ACE_Time_Value sleep_time;
  sleep_time.msec (10);
  while (current < timeout) {
    if (!sleep_) {
      break;
    }
    ACE_OS::sleep (sleep_time);
    current += sleep_time;
  }
  sleep_ = false;
  unsleep_ = true;
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Test_i::sleep> exit\n"));
}

void
Test_i::unsleep ()
{
  sleep_ = false;

  ACE_Time_Value sleep_time;
  sleep_time.msec (10);
  while (unsleep_ == false) {
    ACE_OS::sleep (sleep_time);
  }
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Test_i::unsleep>\n"));
}

void
Test_i::dummy_one_way (const char* msg)
{
  if (msg != 0) {
    ACE_DEBUG ((LM_DEBUG, "(%P|%t) Test_i::dummy_one_way> %.25C\n", msg));
  }
  // do nothing
}

void
Test_i::dummy_two_way ()
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Test_i::dummy_two_way>\n"));
}

void
Test_i::shutdown ()
{
  orb_->shutdown (0);
  shutdown_ = true;
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Test_i::shutdown>\n"));
}
