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

ACE_RCSID(Bug_1270_Regression, Server_Timer, "$Id$")

Server_Timer::Server_Timer(Test::Echo_ptr echo,
                           ACE_Reactor * reactor)
  : ACE_Event_Handler (reactor)
  , echo_(Test::Echo::_duplicate(echo))
  , refcnt_ (1)
{
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
  refcnt_++;

  Test::Payload pload (1024);
  pload.length (1024);

  ACE_OS::memset (pload.get_buffer(), pload.length(), 0);

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      Test::Echo_var echo =
        Test::Echo::_duplicate (this->echo_.in ());

      if(CORBA::is_nil (echo.in ()))
        return 0;

      echo->echo_payload (pload
                          ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      this->echo_ = Test::Echo::_nil ();

      if (this->reactor ()->cancel_timer (this) != 0)
        refcnt_--;
    }
  ACE_ENDTRY;

  refcnt_--;
  if(refcnt_ == 0)
    return -1;

  return 0;
}

int
Server_Timer::handle_close (ACE_HANDLE, ACE_Reactor_Mask)
{
  delete this;
  return 0;
}
