// $Id$

#include "test_i.h"
#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include "tao/RTScheduling/RTScheduler_Manager.h"
#include "MIF_Scheduler.h"

ACE_RCSID(MT_Server, server, "$Id$")

const char *ior_output_file = 0;

int nthreads = 2;
int enable_dynamic_scheduling = 0;
const CORBA::Short max_importance = 100;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "o:n:d");
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

      case 'd':
        enable_dynamic_scheduling = 1;
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-o <iorfile>"
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

int
main (int argc, char *argv[])
{
  MIF_Scheduler* scheduler;
  RTScheduling::Current_var current;

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

      if (enable_dynamic_scheduling)
        {
          CORBA::Object_ptr manager_obj =
            orb->resolve_initial_references ("RTSchedulerManager"
                                             ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (-1);

          TAO_RTScheduler_Manager_var manager =
            TAO_RTScheduler_Manager::_narrow (manager_obj
                                              ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;

          ACE_NEW_RETURN (scheduler,
                          MIF_Scheduler (orb.in ()), -1);

          manager->rtscheduler (scheduler);

          CORBA::Object_var object =
            orb->resolve_initial_references ("RTScheduler_Current"
                                              ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;

          current  =
            RTScheduling::Current::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;
        }

      Simple_Server_i server_impl (orb.in ());

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

      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      long flags = THR_NEW_LWP | THR_JOINABLE | THR_BOUND | THR_SCHED_RR;
      Worker worker (orb.in ());
      if (worker.activate (flags,
                           nthreads) != 0)
        {
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
  ACE_Time_Value tv(120);

  ACE_TRY
    {
      this->orb_->run (tv ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
    }
  ACE_ENDTRY;
  return 0;
}
