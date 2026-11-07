#include "../Scheduler.h"
#include "tao/RTScheduling/RTScheduler_Manager.h"
#include "testC.h"
#include "ace/Get_Opt.h"
#include "ace/SString.h"

ACE_TString ior = ACE_TEXT("file://test.ior");

int
parse_args (int argc,
            ACE_TCHAR* argv [])
{
  // Parse command line arguments
  ACE_Get_Opt opts (argc, argv, ACE_TEXT("f:"));

  int c;
  while ((c = opts ()) != -1)
    {
      switch (c)
        {
        case 'f':
          ior = ACE_TEXT("file://");
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
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  CORBA::ORB_var orb;
  test_var server;

  try
    {
      orb = CORBA::ORB_init (argc, argv);
      TAO_Scheduler scheduler (orb.in ());

      try
        {
          if (parse_args (argc, argv) == -1)
            return (-1);

          CORBA::Object_var manager_obj = orb->resolve_initial_references ("RTSchedulerManager");

          TAO_RTScheduler_Manager_var manager = TAO_RTScheduler_Manager::_narrow (manager_obj.in ());

          manager->rtscheduler (&scheduler);


          CORBA::Object_var object =
            orb->string_to_object (ior.c_str ());

          server = test::_narrow (object.in ());

          if (CORBA::is_nil (server.in ()))
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                "ERROR: Object reference <%s> is nil\n",
                                ior.c_str ()),
                                1);
            }

          CORBA::Object_var const current_obj =
            orb->resolve_initial_references ("RTScheduler_Current");

          RTScheduling::Current_var current = RTScheduling::Current::_narrow (current_obj.in ());

          char const * const name = nullptr;
          CORBA::Policy_ptr const sched_param = nullptr;
          CORBA::Policy_ptr const implicit_sched_param = nullptr;

          current->begin_scheduling_segment (name,
                                            sched_param,
                                            implicit_sched_param);

          ACE_DEBUG ((LM_DEBUG,
                      "Making a one-way request\n"));
          server->one_way (ACE_TEXT_ALWAYS_CHAR(ior.c_str ()));

          ACE_DEBUG ((LM_DEBUG,
                      "Making a two-way request\n"));
          server->two_way (ACE_TEXT_ALWAYS_CHAR(ior.c_str ()));

          current->end_scheduling_segment (name);
        }
      catch (CORBA::THREAD_CANCELLED const &)
        {
          ACE_DEBUG ((LM_DEBUG,
            "Distributable Thread Cancelled - Expected Exception\n"));
          server->shutdown ();
        }
      catch (CORBA::Exception const & ex)
        {
          ex._tao_print_exception ("Caught exception:");

          return 0;
        }

      orb->destroy ();
    }
  catch (CORBA::Exception const & ex)
    {
      ex._tao_print_exception ("Caught unexpected exception:");

      return 1;
    }

  return 0;
}
