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
}

void
Server_Timer::activate (void)
{
  ACE_Time_Value tv (0, 20000);
  this->reactor()->schedule_timer (this, 0, tv);
}

int
Server_Timer::handle_timeout (ACE_Time_Value const &, void const *)
{
  Test::Payload pload(1024);
  pload.length(1024);

  ACE_OS::memset(pload.get_buffer(), 0, pload.length());

  try
  {
    Test::Echo_var echo =
      Test::Echo::_duplicate (this->echo_.in());

    if(CORBA::is_nil (echo.in()))
      return 0;

    echo->echo_payload (pload);

    ACE_Time_Value tv (0, 20000);
    this->reactor()->schedule_timer (this, 0, tv);
  }
  catch (const CORBA::Exception&)
  {
    this->echo_ = Test::Echo::_nil ();

    this->reactor()->cancel_timer(this);

    return -1;
  }

  return 0;
}

int
Server_Timer::handle_close (ACE_HANDLE, ACE_Reactor_Mask)
{
  delete this;
  return 0;
}
