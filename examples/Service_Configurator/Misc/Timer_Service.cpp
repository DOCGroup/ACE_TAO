// $Id$

#include "Timer_Service.h"

int
Timer_Service::init (int argc, char *argv[])
{
  ACE_DEBUG ((LM_DEBUG, 
	      "in Timer_Service::init, argv[0] = %s, argc == %d\n", 
	      argv[0], argc));

  for (int i = 0; i < argc; i++)
    ACE_DEBUG ((LM_DEBUG, "argv[%d] = %s\n", i, argv[i]));

  int interval = Timer_Service::TIMEOUT;

  if (argc > 1)
    {
      // If the second argument exists use this as the interval for
      // the periodic timer.  Otherwise, go off every TIMEOUT seconds.

      interval = ACE_OS::atoi (argv[1]);

      if (interval == 0)
	interval = Timer_Service::TIMEOUT;
    }

  if (argc > 2)
    {
      // If the third argument exists use it to control the maximum
      // number of timeouts.
      this->max_timeouts_ = ACE_OS::atoi (argv[2]);

      if (this->max_timeouts_ == 0)
	this->max_timeouts_ = Timer_Service::MAX_TIMEOUTS;
    }

  this->cur_timeouts_ = 0;

  // If the fourth argument exists take this as an indication to
  // enable tracing.
  if (argc > 3)
    ACE_Trace::start_tracing ();
  else
    ACE_Trace::stop_tracing ();

  // Register the timer to go off in 1 second, and then to go off
  // every <interval> seconds.
  if (ACE_Service_Config::reactor ()->schedule_timer 
      (this, 0, ACE_Time_Value (1), ACE_Time_Value (interval)) == -1)
    return -1;
  else
    return 0;
}

int
Timer_Service::handle_timeout (const ACE_Time_Value &tv,
			       const void *)
{
  ACE_DEBUG ((LM_DEBUG, 
	      "in Timer_Service::handle_timeout sec = %d, usec = %d"
	      " cur_timeouts = %d, max_timeouts = %d\n",
	      tv.sec (), tv.usec (),
	      this->cur_timeouts_, this->max_timeouts_));

  this->cur_timeouts_++;

  if (this->cur_timeouts_ == this->max_timeouts_)
    // Shutdown the test.
    return -1;
  else
    return 0;
}

int
Timer_Service::handle_close (ACE_HANDLE,
			     ACE_Reactor_Mask)
{
  ACE_DEBUG ((LM_DEBUG, "closing down the timer test\n"));

  ACE_Service_Config::end_reactor_event_loop ();
  return 0;
}

// Define the factory function.
ACE_SVC_FACTORY_DEFINE (Timer_Service)

// Define the object that describes the service.
ACE_STATIC_SVC_DEFINE (Timer_Service,
		       "Timer_Service", ACE_SVC_OBJ_T, &ACE_SVC_NAME (Timer_Service),
		       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ, 0)

