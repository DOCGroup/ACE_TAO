// $Id$

#include "test_i.h"
#include "ace/OS_NS_unistd.h"
#include "ace/High_Res_Timer.h"

ACE_RCSID(Timeout, test_i, "$Id$")

ACE_High_Res_Timer sleep_duration;


Simple_Server_i::Simple_Server_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
  , echo_count_ (0)
{
  sleep_duration.calibrate();
}

void
Simple_Server_i::echo (CORBA::Long ,
                       CORBA::Long msecs, const char* )
{
  ACE_Time_Value tv;
  tv.msec (msecs);
  echo_count_++;
  ACE_OS::sleep (tv);
}

void
Simple_Server_i::shutdown (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "server (%P) Received shutdown request from client\n"));

  this->orb_->shutdown (0);
}
