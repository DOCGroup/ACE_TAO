#include "Timer_Service.h"
// $Id$

int
Timer_Service::init (int argc, char *argv[])
{
  int interval = argc > 1 ? ACE_OS::atoi (argv[1]) : 4;

  if (argc > 2)
    ACE_Trace::start_tracing ();
  else
    ACE_Trace::stop_tracing ();

  ACE_DEBUG ((LM_DEBUG, 
	      "in Timer_Service::init, argv[0] = %s, argc == %d\n", 
	      argv[0], argc));

  ACE_Service_Config::reactor ()->schedule_timer 
    (this, 0, ACE_Time_Value (1), ACE_Time_Value (interval));
  return 0;
}

int
Timer_Service::handle_timeout (const ACE_Time_Value &tv,
			       const void *)
{
  ACE_DEBUG ((LM_DEBUG, 
	      "in Timer_Service::handle_timeout sec = %d, usec = %d\n",
	      tv.sec (), tv.usec ()));
  return 0;
}

// Define the factory function.
ACE_SVC_FACTORY_DEFINE (Timer_Service)

// Define the object that describes the service.
ACE_STATIC_SVC_DEFINE (Timer_Service,
		       "Timer_Service", ACE_SVC_OBJ_T, &ACE_SVC_NAME (Timer_Service),
		       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ, 0)

