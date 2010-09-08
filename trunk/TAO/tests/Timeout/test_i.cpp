// $Id$

#include "test_i.h"
#include "ace/OS_NS_unistd.h"
#include "ace/High_Res_Timer.h"

ACE_High_Res_Timer sleep_duration;


Simple_Server_i::Simple_Server_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
  sleep_duration.calibrate();
}

CORBA::Long
Simple_Server_i::echo (CORBA::Long x,
                       CORBA::Long msecs)
{
  ACE_Time_Value tv (msecs / 1000, (msecs % 1000) * 1000);

  ACE_DEBUG ((LM_DEBUG, "server (%P) will request sleep for %d sec and %d msecs.\n",
              tv.sec(), tv.msec ()));

  sleep_duration.reset();
  sleep_duration.start();
  ACE_OS::sleep (tv);
  sleep_duration.stop();
  ACE_hrtime_t usec;
  sleep_duration.elapsed_microseconds(usec);
  ACE_DEBUG ((LM_DEBUG, "server (%P) actually slept for %d sec and %d msecs.\n",
              usec/1000000, (usec % 1000000)/1000));

  return x;
}

void
Simple_Server_i::shutdown (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "server (%P) Received shutdown request from client\n"));
  this->orb_->shutdown (0);
}
