// $Id$

#include "Latency.h"
#include "orbsvcs/Event_Service_Constants.h"

#include "tao/Messaging.h"
#include "tao/Strategies/advanced_resource.h"
#include "tao/PortableServer/PortableServer.h"
#include "ace/High_Res_Timer.h"
#include "ace/Get_Opt.h"
#include "ace/Sample_History.h"
#include "ace/Basic_Stats.h"
#include "ace/Sched_Params.h"

ACE_RCSID(EC_Tests_Performance, Latency, "$Id$")

int iterations = 1000;
int do_dump_history = 0;
const char *ec_ior = "file://ec.ior";

/// Parse the arguments.
static int parse_args (int argc, char *argv[]);

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
        CORBA::ORB_init (argc, argv, "" TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

#if (TAO_HAS_CORBA_MESSAGING == 1)
      CORBA::Object_var manager_object =
        orb->resolve_initial_references ("ORBPolicyManager"
                                         TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::PolicyManager_var policy_manager =
        CORBA::PolicyManager::_narrow (manager_object.in ()
                                       TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Any sync_scope;
      sync_scope <<= Messaging::SYNC_WITH_TARGET;

      CORBA::PolicyList policy_list (1);
      policy_list.length (1);
      policy_list[0] =
        orb->create_policy (Messaging::SYNC_SCOPE_POLICY_TYPE,
                            sync_scope
                            TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      policy_manager->set_policy_overrides (policy_list,
                                            CORBA::SET_OVERRIDE
                                            TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
#else
      ACE_DEBUG ((LM_DEBUG,
                  "CORBA Messaging disabled in this configuration,"
                  " test may not be optimally configured\n"));
#endif /* TAO_HAS_MESSAGING */

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA" TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in () TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      poa_manager->activate (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return 1;

      // Get the event channel object reference
      CORBA::Object_var object =
        orb->string_to_object (ec_ior TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      RtecEventChannelAdmin::EventChannel_var ec =
        RtecEventChannelAdmin::EventChannel::_narrow (object.in ()
                                                      TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      if (CORBA::is_nil (ec.in ()))
        {
          ACE_ERROR ((LM_ERROR,
                      "(%P|%t) Invalid or nil event channel\n"));
          return 1;
        }

      ACE_DEBUG ((LM_DEBUG, "Resolved event service\n"));

      // Now create the history
      ACE_Sample_History history (iterations);
      TAO_SYNCH_MUTEX history_mutex;

      // The consumer
      EC_Latency_Consumer consumer (&history,
                                    &history_mutex,
                                    iterations);
      // Connect the consumer

      RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin =
        ec->for_consumers (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      RtecEventChannelAdmin::ProxyPushSupplier_var proxy_supplier =
        consumer_admin->obtain_push_supplier (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      RtecEventComm::PushConsumer_var consumer_reference =
        consumer._this (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Simple subscription, but usually the helper classes in
      // $TAO_ROOT/orbsvcs/Event_Utils.h are a better way to do this.
      RtecEventChannelAdmin::ConsumerQOS consumer_qos;
      consumer_qos.dependencies.length (2);
      RtecEventComm::EventHeader& h0 =
        consumer_qos.dependencies[0].event.header;
      h0.type   = ACE_ES_DISJUNCTION_DESIGNATOR;
      h0.source = 1;

      RtecEventComm::EventHeader& h1 =
        consumer_qos.dependencies[1].event.header;
      h1.type   = ACE_ES_EVENT_UNDEFINED; // first free event type
      h1.source = ACE_ES_EVENT_SOURCE_ANY;

      proxy_supplier->connect_push_consumer (consumer_reference.in (),
                                             consumer_qos
                                             TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "Connected consumer\n"));

      // The supplier
      EC_Latency_Supplier supplier;

      // The canonical protocol to connect to the EC
      RtecEventChannelAdmin::SupplierAdmin_var supplier_admin =
        ec->for_suppliers (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      RtecEventChannelAdmin::ProxyPushConsumer_var proxy_consumer =
        supplier_admin->obtain_push_consumer (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      RtecEventComm::PushSupplier_var supplier_reference =
        supplier._this (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Simple publication, but usually the helper classes in
      // $TAO_ROOT/orbsvcs/Event_Utils.h are a better way to do this.
      RtecEventChannelAdmin::SupplierQOS supplier_qos;
      supplier_qos.publications.length (1);
      RtecEventComm::EventHeader& sh0 =
        supplier_qos.publications[0].event.header;
      sh0.type   = ACE_ES_EVENT_UNDEFINED; // first free event type
      sh0.source = 1;                      // first free event source

      proxy_consumer->connect_push_supplier (supplier_reference.in (),
                                             supplier_qos
                                             TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "Connected supplier\n"));

      Task task (proxy_consumer.in (), iterations);

      task.activate ();

      ACE_hrtime_t start = ACE_OS::gethrtime ();
      while (!task.done () || !consumer.done ())
        {
          ACE_Time_Value tv (1, 0);
          orb->run (tv TAO_ENV_ARG_PARAMETER);
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
      if (do_dump_history)
        {
          history.dump_samples ("HISTORY", gsf);
        }

      ACE_Basic_Stats stats;
      history.collect_basic_stats (stats);
      stats.dump_results ("Latency", gsf);

      ACE_hrtime_t elapsed_microseconds = (end - start) / gsf;
      double elapsed_seconds =
        ACE_CU64_TO_CU32(elapsed_microseconds) / 1000000.0;
      double throughput =
        double(iterations) / elapsed_seconds;

      ACE_DEBUG ((LM_DEBUG, "Throughtput: %f\n", throughput));

      proxy_supplier->disconnect_push_supplier (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      proxy_consumer->disconnect_push_consumer (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::ObjectId_var id;

      id = root_poa->servant_to_id (&consumer TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      root_poa->deactivate_object (id.in () TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      id = root_poa->servant_to_id (&supplier TAO_ENV_ARG_PARAMETER);
      root_poa->deactivate_object (id.in () TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      orb->destroy (TAO_ENV_SINGLE_ARG_PARAMETER);
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
        ec_ior = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-i <iterations>"
                           "-k <IOR>"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

// ****************************************************************

EC_Latency_Consumer::EC_Latency_Consumer (ACE_Sample_History *history,
                                          TAO_SYNCH_MUTEX *mutex,
                                          int message_count)
  : history_ (history)
  , mutex_ (mutex)
  , remaining_messages_ (message_count)
{
}

int
EC_Latency_Consumer::done (void)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, *this->mutex_, -1);
  return this->remaining_messages_ <= 0;
}

void
EC_Latency_Consumer::push (const RtecEventComm::EventSet& events
                           TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_hrtime_t creation;
  ORBSVCS_Time::TimeT_to_hrtime (creation,
                                 events[0].header.creation_time);
  ACE_hrtime_t now = ACE_OS::gethrtime ();

  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, *this->mutex_);
  this->history_->sample (now - creation);
  if (this->remaining_messages_ % 1000 == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "Only %d messages to go\n",
                  this->remaining_messages_));
    }

  this->remaining_messages_--;
}

void
EC_Latency_Consumer::disconnect_push_consumer (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

// ****************************************************************

EC_Latency_Supplier::EC_Latency_Supplier (void)
{
}

void
EC_Latency_Supplier::disconnect_push_supplier (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

// ****************************************************************

Task::Task (RtecEventChannelAdmin::ProxyPushConsumer_ptr consumer,
            int iterations)
  : consumer_ (RtecEventChannelAdmin::ProxyPushConsumer::_duplicate (consumer))
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
  ACE_TRY_NEW_ENV
    {
      RtecEventComm::EventSet event (1);
      event.length (1);
      event[0].header.type   = ACE_ES_EVENT_UNDEFINED;
      event[0].header.source = 1;
      event[0].header.ttl    = 1;
      event[0].data.payload.length(1024);

      for (;;)
        {
          ACE_hrtime_t creation = ACE_OS::gethrtime ();
          ORBSVCS_Time::hrtime_to_TimeT (event[0].header.creation_time,
                                         creation);
          this->consumer_->push (event TAO_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          // ACE_Time_Value tv (0, 5000);
          // ACE_OS::sleep (tv);

          ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->mutex_, -1);
          if (this->remaining_messages_ % 1000 == 0)
            {
              ACE_DEBUG ((LM_DEBUG, "Only %d messages to go\n",
                          this->remaining_messages_));
            }

          this->remaining_messages_--;
          if (this->remaining_messages_ == 0)
            return 0;
        }
    }
  ACE_CATCH (CORBA::SystemException, ex)
    {
      ACE_PRINT_EXCEPTION (ex, "Task::svc");
    }
  ACE_ENDTRY;
  return 0;
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
