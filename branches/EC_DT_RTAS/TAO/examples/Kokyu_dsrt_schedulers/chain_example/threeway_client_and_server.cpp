//$Id$ 

#include "test_i.h"
#include "workC.h"
#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include "ace/High_Res_Timer.h"
#include "tao/RTScheduling/RTScheduler_Manager.h"
#include "EDF_Scheduler.h"
#include "Task_Stats.h"
#include "cpuload.h"

#include "chain_config.h"
#include "chain_dsui_families.h"
#include <dsui.h>
#include <cstdlib>


ACE_RCSID(MT_Server, server, "server.cpp,v 1.3 2003/10/14 05:57:01 jwillemsen Exp")

const char *ior_input_file = "file://test2.ior";
const char *ior_output_file = "test1.ior";

int nthreads = 1;
int enable_dynamic_scheduling = 1;
const CORBA::Short max_importance = 100;
int enable_yield = 1;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "o:n:s");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;

      case 'n':
        nthreads = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 's':
        enable_yield = 0;
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-o <iorfile>"
			   "-n (thread num)"
                           "-s (disable yield)"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

class Worker : public ACE_Task_Base
{
  // = TITLE
  //   Run a server thread
  //
  // = DESCRIPTION
  //   Use the ACE_Task_Base class to run server threads
  //
public:
  Worker (CORBA::ORB_ptr orb);
  // ctor

  virtual int svc (void);
  // The thread entry point.

private:
  CORBA::ORB_var orb_;
  // The orb
};

Task_Stats task_stats;

int
main (int argc, char *argv[])
{
  ds_control ds_cntrl ("DT_Chain_Middle", "chain_enable.dsui");

  EDF_Scheduler* scheduler = 0;
  RTScheduling::Current_var current;
  long flags;
  int sched_policy = ACE_SCHED_RR;
  int sched_scope = ACE_SCOPE_THREAD;

  if (sched_policy == ACE_SCHED_RR)
    flags = THR_NEW_LWP | THR_BOUND | THR_JOINABLE | THR_SCHED_RR;
  else
    flags = THR_NEW_LWP | THR_BOUND | THR_JOINABLE | THR_SCHED_FIFO;

  task_stats.init (100000);

//  TAO_debug_level =1;

  Object_ID oid = ACE_OBJECT_COUNTER->increment();

  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return 1;

      ACE_DEBUG((LM_DEBUG,"Before complex server\n"));
      CORBA::Object_var object =
        orb->string_to_object (ior_input_file ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Complex_Server_var server2 =
        Complex_Server::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG((LM_DEBUG,"After complex server\n"));
      if (CORBA::is_nil (server2.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference <%s> is nil\n",
                             ior_input_file),
                            1);
        }

      if (enable_dynamic_scheduling)
        {
          CORBA::Object_ptr manager_obj =
            orb->resolve_initial_references ("RTSchedulerManager"
                                             ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          TAO_RTScheduler_Manager_var manager =
            TAO_RTScheduler_Manager::_narrow (manager_obj
                                              ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          Kokyu::DSRT_Dispatcher_Impl_t disp_impl_type;
          if (enable_yield)
            {
              disp_impl_type = Kokyu::DSRT_CV_BASED;
            }
          else
            {
              disp_impl_type = Kokyu::DSRT_OS_BASED;
            }

          ACE_NEW_RETURN (scheduler,
                          EDF_Scheduler (orb.in (),
                                         disp_impl_type,
                                         sched_policy,
                                         sched_scope), -1);

          manager->rtscheduler (scheduler);

          CORBA::Object_var object =
            orb->resolve_initial_references ("RTScheduler_Current"
                                              ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          current  =
            RTScheduling::Current::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      ACE_DEBUG((LM_DEBUG,"before activate server\n"));
      Simple_Server_i server_impl (orb.in (),
                                   current.in (),
				   server2.in (),
                                   task_stats,
                                   enable_yield);

      Simple_Server_var server =
        server_impl._this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::String_var ior =
        orb->object_to_string (server.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "Activated as <%s>\n", ior.in ()));

      // If the ior_output_file exists, output the ior to it
      if (ior_output_file != 0)
        {
          FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
          if (output_file == 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Cannot open output file for writing IOR: %s",
                               ior_output_file),
                              1);
          ACE_OS::fprintf (output_file, "%s", ior.in ());
          ACE_OS::fclose (output_file);
        }

  ACE_OS::printf ( ACE_TEXT ("The Client_and_Server has generated %s\n"),ior_output_file);
 
  CPULoad::calibrate(5);

      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

  ACE_Time_Value start_time=ACE_OS::gettimeofday();
  ACE_OS::printf ( ACE_TEXT ("The Start time: %u (sec), %u (usec)\n"), start_time.sec(), start_time.usec());
  DSTRM_EVENT(MAIN_GROUP_FAM, START,0,sizeof(Object_ID), (char*)&oid);

      Worker worker (orb.in ());
      if (worker.activate (flags,
                           nthreads,
                           0,
                           ACE_Sched_Params::priority_max(sched_policy,
                                                          sched_scope)) != 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "Cannot activate threads in RT class.",
                      "Trying to activate in non-RT class\n"));

          flags = THR_NEW_LWP | THR_JOINABLE | THR_BOUND;
          if (worker.activate (flags, nthreads) != 0)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "Cannot activate server threads\n"),
                                1);
            }
        }

      worker.wait ();

      ACE_DEBUG ((LM_DEBUG, "event loop finished\n"));

      ACE_DEBUG ((LM_DEBUG, "shutting down scheduler\n"));
      scheduler->shutdown ();
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception caught:");
      return 1;
    }
  ACE_ENDTRY;

  DSTRM_EVENT(MAIN_GROUP_FAM, STOP, 0, sizeof(Object_ID), (char*)&oid);

  ACE_DEBUG ((LM_DEBUG, "Exiting main...\n"));
  return 0;
}

// ****************************************************************

Worker::Worker (CORBA::ORB_ptr orb)
  :  orb_ (CORBA::ORB::_duplicate (orb))
{
}

int
Worker::svc (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_Time_Value tv(600);

  ACE_TRY
    {
      this->orb_->run (tv ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
    }
  ACE_ENDTRY;
  ACE_DEBUG ((LM_DEBUG, "(%t|%T): Worker thread exiting...\n"));
  return 0;
}
