// $Id$

#include "Callback_i.h"

#include "tao/Messaging/Messaging.h"
#include "tao/PortableServer/PortableServer.h"
#include "tao/Strategies/advanced_resource.h"
#include "ace/High_Res_Timer.h"
#include "ace/Get_Opt.h"
#include "ace/Basic_Stats.h"
#include "ace/Stats.h"
#include "ace/Sched_Params.h"
#include "ace/Task.h"

ACE_RCSID(Callback, client, "$Id$")

int iterations = 1000;
int do_dump_history = 0;
const char *ior = "file://server.ior";

/// Parse the arguments.
static int parse_args (int argc, char *argv[]);

/// Send events in another thread
class Task : public ACE_Task_Base
{
public:
  /// Constructor
  Task (Test::Server_ptr server, int iterations);

  /// Return 1 when all the messages have been sent
  int done (void);

  /// Run the experiment
  int svc (void);

private:
  /// The consumer
  Test::Server_var server_;

  /// Number of messages that have to be sent
  int remaining_messages_;

  /// Synchronize access to remaining_messages_
  TAO_SYNCH_MUTEX mutex_;
};

int
main (int argc, char *argv [])
{
  int priority =
    (ACE_Sched_Params::priority_min (ACE_SCHED_FIFO)
     + ACE_Sched_Params::priority_max (ACE_SCHED_FIFO)) / 2;
  priority = ACE_Sched_Params::next_priority (ACE_SCHED_FIFO,
                                                  priority);
  // Enable FIFO scheduling, e.g., RT scheduling class on Solaris.

  if (ACE_OS::sched_params (ACE_Sched_Params (ACE_SCHED_FIFO,
                                              priority,
                                              ACE_SCOPE_PROCESS)) != 0)
    {
      if (ACE_OS::last_error () == EPERM)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "server (%P|%t): user is not superuser, "
                      "test runs in time-shared class\n"));
        }
      else
        ACE_ERROR ((LM_ERROR,
                    "server (%P|%t): sched_params failed\n"));
    }

  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var manager_object =
        orb->resolve_initial_references ("ORBPolicyManager"
                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::PolicyManager_var policy_manager =
        CORBA::PolicyManager::_narrow (manager_object.in ()
                                       ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Any sync_scope;
      sync_scope <<= Messaging::SYNC_WITH_TARGET;

      CORBA::PolicyList policy_list (1);
      policy_list.length (1);
      policy_list[0] =
        orb->create_policy (Messaging::SYNC_SCOPE_POLICY_TYPE,
                            sync_scope
                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      policy_manager->set_policy_overrides (policy_list,
                                            CORBA::SET_OVERRIDE
                                            ACE_ENV_ARG_PARAMETER);
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

      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return 1;

      // Get the event channel object reference
      CORBA::Object_var object =
        orb->string_to_object (ior ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Test::Server_var server =
        Test::Server::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR ((LM_ERROR,
                      "(%P|%t) Invalid or nil event channel\n"));
          return 1;
        }

      ACE_DEBUG ((LM_DEBUG, "Resolved server\n"));

      Callback_i callback_i (iterations);

      Test::Callback_var callback = callback_i._this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      server->set_callback (callback.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "Connected callback\n"));

      Task task (server.in (), iterations);
      task.activate ();

      ACE_hrtime_t start = ACE_OS::gethrtime ();
      while (!callback_i.done () || !task.done ())
        {
          ACE_Time_Value tv (1, 0);
          orb->run (tv ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      ACE_hrtime_t end = ACE_OS::gethrtime ();

      ACE_Thread_Manager::instance ()->wait ();

      // Calibrate the high resolution timer *before* starting the
      // test.
      ACE_DEBUG ((LM_DEBUG, "Calibrating high res timer ...."));
      ACE_High_Res_Timer::calibrate ();

      ACE_UINT32 gsf = ACE_High_Res_Timer::global_scale_factor ();
      ACE_DEBUG ((LM_DEBUG, "Done (%d)\n", gsf));

      ACE_Sample_History &history =
        callback_i.sample_history ();
      if (do_dump_history)
        {
          history.dump_samples ("HISTORY", gsf);
        }

      ACE_Basic_Stats stats;
      history.collect_basic_stats (stats);
      stats.dump_results ("Total", gsf);

      ACE_Throughput_Stats::dump_throughput ("Total", gsf,
                                             end - start,
                                             stats.samples_count ());

      server->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::ObjectId_var id =
        root_poa->servant_to_id (&callback_i ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      root_poa->deactivate_object (id.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::Exception, ex)
    {
      ACE_PRINT_EXCEPTION (ex, argv[0]);
    }
  ACE_ENDTRY;
  return 0;
}

// ****************************************************************

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "hi:k:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'h':
        do_dump_history = 1;
        break;

      case 'i':
        iterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-h (dump full sample history) "
                           "-i <iterations> "
                           "-k <IOR> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

// ****************************************************************

Task::Task (Test::Server_ptr server,
            int iterations)
  : server_ (Test::Server::_duplicate (server))
  , remaining_messages_ (iterations)
{
}

int
Task::done (void)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->mutex_, 1);
  return this->remaining_messages_ == 0;
}

int
Task::svc (void)
{
  Test::Payload payload(1024); payload.length(1024);
  ACE_TRY_NEW_ENV
    {
      for (;;)
        {
          Test::TimeStamp creation = ACE_OS::gethrtime ();
          this->server_->request (creation, payload ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          // ACE_Time_Value tv (0, 5000);
          // ACE_OS::sleep (tv);

          ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->mutex_, -1);

          this->remaining_messages_--;
          if (this->remaining_messages_ == 0)
            return 0;
        }
    }
  ACE_CATCH (CORBA::SystemException, ex)
    {
      ACE_PRINT_EXCEPTION (ex, "Task::svc");
      this->remaining_messages_ = 0;
    }
  ACE_ENDTRY;
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
