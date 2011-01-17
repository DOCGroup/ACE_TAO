/**
 * @file Server_Timer.cpp
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@atdesk.com>
 *
 */
#include "Server_Timer.h"
#include "ace/Reactor.h"
#include "ace/OS_NS_string.h"

Server_Timer::Server_Timer(Test::Echo_ptr echo,
                           ACE_Reactor * reactor)
  : ACE_Event_Handler (reactor)
  , echo_(Test::Echo::_duplicate(echo))
{
  this->reference_counting_policy ().value (
    ACE_Event_Handler::Reference_Counting_Policy::ENABLED);
}

void
Server_Timer::activate (void)
{
  ACE_Time_Value tv (0, 20000);
  this->reactor()->schedule_timer (this, 0, tv, tv);
}

int
Server_Timer::handle_timeout (ACE_Time_Value const &, void const *)
{
  Test::Payload pload (1024);
  pload.length (1024);

  ACE_OS::memset (pload.get_buffer(), 0, pload.length());

  try
    {
      if(CORBA::is_nil (this->echo_.in ()))
        return -1;

      Test::Echo_var echo =
        Test::Echo::_duplicate (this->echo_.in ());

      echo->echo_payload (pload);
    }
  catch (const CORBA::Exception&)
    {
      this->echo_ = Test::Echo::_nil ();
      this->reactor ()->cancel_timer (this);
      return -1;
    }

  return 0;
}
