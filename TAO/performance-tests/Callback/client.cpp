// $Id$

#include "Callback_i.h"

#include "tao/Messaging/Messaging.h"
#include "tao/PortableServer/PortableServer.h"
#include "tao/Strategies/advanced_resource.h"
#include "tao/AnyTypeCode/Any.h"
#include "ace/High_Res_Timer.h"
#include "ace/Get_Opt.h"
#include "ace/Basic_Stats.h"
#include "ace/Stats.h"
#include "ace/Throughput_Stats.h"
#include "ace/Sched_Params.h"
#include "ace/Task.h"
#include "ace/OS_NS_errno.h"

int iterations = 1000;
int do_dump_history = 0;
const ACE_TCHAR *ior = ACE_TEXT("file://server.ior");

/// Parse the arguments.
static int parse_args (int argc, ACE_TCHAR *argv[]);

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
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
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
                      "client (%P|%t): user is not superuser, "
                      "test runs in time-shared class\n"));
        }
      else
        ACE_ERROR ((LM_ERROR,
                    "client (%P|%t): sched_params failed\n"));
    }

  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var manager_object =
        orb->resolve_initial_references ("ORBPolicyManager");

      CORBA::PolicyManager_var policy_manager =
        CORBA::PolicyManager::_narrow (manager_object.in ());

      CORBA::Any sync_scope;
      sync_scope <<= Messaging::SYNC_WITH_TARGET;

      CORBA::PolicyList policy_list (1);
      policy_list.length (1);
      policy_list[0] =
        orb->create_policy (Messaging::SYNC_SCOPE_POLICY_TYPE,
                            sync_scope);
      policy_manager->set_policy_overrides (policy_list,
                                            CORBA::SET_OVERRIDE);

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      poa_manager->activate ();

      if (parse_args (argc, argv) != 0)
        return 1;

      // Get the event channel object reference
      CORBA::Object_var object =
        orb->string_to_object (ior);

      Test::Server_var server =
        Test::Server::_narrow (object.in ());
      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR ((LM_ERROR,
                      "(%P|%t) Invalid or nil event channel\n"));
          return 1;
        }

      ACE_DEBUG ((LM_DEBUG, "Resolved server\n"));

      Callback_i callback_i (iterations);

      Test::Callback_var callback = callback_i._this ();

      server->set_callback (callback.in ());

      ACE_DEBUG ((LM_DEBUG, "Connected callback\n"));

      Task task (server.in (), iterations);
      task.activate ();

      ACE_hrtime_t start = ACE_OS::gethrtime ();
      while (!callback_i.done () || !task.done ())
        {
          ACE_Time_Value tv (1, 0);
          orb->run (tv);
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
          history.dump_samples (ACE_TEXT("HISTORY"), gsf);
        }

      ACE_Basic_Stats stats;
      history.collect_basic_stats (stats);
      stats.dump_results (ACE_TEXT("Total"), gsf);

      ACE_Throughput_Stats::dump_throughput (ACE_TEXT("Total"), gsf,
                                             end - start,
                                             stats.samples_count ());

      server->shutdown ();

      PortableServer::ObjectId_var id =
        root_poa->servant_to_id (&callback_i);
      root_poa->deactivate_object (id.in ());
      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (argv[0]);
    }
  return 0;
}

// ****************************************************************

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("hi:k:"));
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
  // Indicates successful parsing of the command line
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
  try
    {
      for (;;)
        {
          Test::TimeStamp creation = ACE_OS::gethrtime ();
          this->server_->request (creation, payload);

          // ACE_Time_Value tv (0, 5000);
          // ACE_OS::sleep (tv);

          ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->mutex_, -1);

          this->remaining_messages_--;
          if (this->remaining_messages_ == 0)
            return 0;
        }
    }
  catch (const CORBA::SystemException& ex)
    {
      ex._tao_print_exception ("Task::svc");
      this->remaining_messages_ = 0;
    }
  return 0;
}
