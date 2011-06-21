// -*- C++ -*-
// $Id$

#include "tao/Reactive_Connect_Strategy.h"
#include "tao/Connection_Handler.h"
#include "tao/ORB_Core.h"
#include "tao/debug.h"
#include "tao/Transport.h"
#include "tao/LF_Multi_Event.h"

#include "ace/Synch_Options.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Reactive_Connect_Strategy::TAO_Reactive_Connect_Strategy (
    TAO_ORB_Core *orb_core)
  : TAO_Connect_Strategy (orb_core)
{
}

TAO_Reactive_Connect_Strategy::~TAO_Reactive_Connect_Strategy (void)
{
}

void
TAO_Reactive_Connect_Strategy::synch_options (ACE_Time_Value *timeout,
                                             ACE_Synch_Options &options)
{
  if (timeout != 0)
    {
      options.set (ACE_Synch_Options::USE_REACTOR,
                   *timeout);
    }
  else
    {
      // Making it sure it is blocking.
      options.set (ACE_Synch_Options::USE_REACTOR,
                   ACE_Time_Value::zero);
    }
}

int
TAO_Reactive_Connect_Strategy::wait_i (TAO_LF_Event *ev,
                                       TAO_Transport *,
                                       ACE_Time_Value * max_wait_time)
{
  int result = 0;
  if (ev == 0)
    return -1;

  if (TAO_debug_level > 2)
    {
      //FUZZ: disable check_for_lack_ACE_OS
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) - Waiting in the Reactor for ")
                  ACE_TEXT ("connection completion - wait ()\n")));
      //FUZZ: enable check_for_lack_ACE_OS
    }

  try
    {
      while (ev->keep_waiting ())
        {
          result =
            this->orb_core_->run (max_wait_time, 1);

          // Did we timeout? If so, stop running the loop.
          if (result == 0 &&
              max_wait_time != 0 &&
              *max_wait_time == ACE_Time_Value::zero)
            {
              errno = ETIME;
              result = -1;
              break;
            }

          // Other errors? If so, stop running the loop.
          if (result == -1)
            break;
        }
    }
  catch (const ::CORBA::Exception&)
    {
      result = -1;
    }

  // Set the result.
  if (result != -1 && ev->error_detected ())
    {
      result = -1;
    }

  return result;
}

TAO_END_VERSIONED_NAMESPACE_DECL
