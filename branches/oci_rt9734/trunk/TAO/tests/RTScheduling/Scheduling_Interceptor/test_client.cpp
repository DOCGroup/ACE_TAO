//$Id$
#include "../Scheduler.h"
#include "tao/RTScheduling/RTScheduler_Manager.h"
#include "testC.h"
#include "ace/Get_Opt.h"
#include "ace/SString.h"

ACE_CString ior = "file://test.ior";

int
parse_args (int argc,
            ACE_TCHAR* argv [])
{
  // Parse command line arguments
  ACE_Get_Opt opts (argc, argv, "f:");

  int c;
  while ((c= opts ()) != -1)
    {
      switch (c)
        {
        case 'f':
          ior = "file://";
          ior += opts.opt_arg ();
          break;
        default:
          ACE_DEBUG ((LM_DEBUG, "Unknown Option\n"));
          return -1;
        }
    }
  return 0;
}

#if defined (THREAD_CANCELLED)
#undef THREAD_CANCELLED
#endif /* THREAD_CANCELLED */

int
main (int argc, char* argv [])
{
  CORBA::ORB_var orb;
  test_var server;
  RTScheduling::Scheduler_var safe_scheduler;

  ACE_TRY_NEW_ENV
    {

      orb = CORBA::ORB_init (argc,
                             argv,
                             ""
                             ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) == -1)
        return (-1);

      CORBA::Object_var manager_obj = orb->resolve_initial_references ("RTSchedulerManager"
                                                                       ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      TAO_RTScheduler_Manager_var manager = TAO_RTScheduler_Manager::_narrow (manager_obj.in ()
                                                                              ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      TAO_Scheduler* scheduler;
      ACE_NEW_RETURN (scheduler,
		      TAO_Scheduler (orb.in ()),
		      -1);
      safe_scheduler = scheduler;

      manager->rtscheduler (scheduler);


      CORBA::Object_var object =
        orb->string_to_object (ior.c_str ()
                               ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      server = test::_narrow (object.in ()
                              ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "ERROR: Object reference <%s> is nil\n",
                             ior.c_str ()),
                            1);
        }

      CORBA::Object_var current_obj = orb->resolve_initial_references ("RTScheduler_Current"
								       ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      RTScheduling::Current_var current = RTScheduling::Current::_narrow (current_obj.in ()
                                                                          ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      const char * name = 0;
      CORBA::Policy_ptr sched_param = 0;
      CORBA::Policy_ptr implicit_sched_param = 0;

      current->begin_scheduling_segment (name,
                                         sched_param,
                                         implicit_sched_param
                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG,
                  "Making a one-way request\n"));
      server->one_way (ior.c_str ());

      ACE_DEBUG ((LM_DEBUG,
                  "Making a two-way request\n"));
      server->two_way (ior.c_str ()
                       ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      current->end_scheduling_segment (name
                                       ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

    }
  ACE_CATCH (CORBA::THREAD_CANCELLED, thr_ex)
    {
      ACE_DEBUG ((LM_DEBUG,
		  "Distributable Thread Cancelled - Expected Exception\n"));
      server->shutdown ();
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Caught exception:");

      return 0;
    }
  ACE_ENDTRY;

  orb->destroy ();

  return 0;
}
