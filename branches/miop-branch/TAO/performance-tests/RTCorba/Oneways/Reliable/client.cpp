// $Id$

#include "testC.h"

#include "tao/Messaging.h"
#include "tao/TAOC.h"
#include "ace/Get_Opt.h"
#include "ace/Sched_Params.h"
#include "ace/Stats.h"
#include "ace/High_Res_Timer.h"

ACE_RCSID (Reliable,
           client,
           "$Id$")


#define USING_TIMERS
//#define USING_QUANTIFY

#if defined (USING_QUANTIFY)

#if defined (ACE_WIN32)

#include "pure.h"

#else /* !ACE_WIN32 */

#include "quantify.h"

inline int QuantifyClearData ()
{
  return quantify_clear_data ();
}

inline int QuantifyStartRecordingData ()
{
  return quantify_start_recording_data ();
}

inline int QuantifyStopRecordingData ()
{
  return quantify_stop_recording_data ();
}

#endif /* ACE_WIN32 */

#endif /* USING_QUANTIFY */

// Default IOR.
static const char *ior = "file://test.ior";

// Levels at which syncscope policy can be set.
enum LEVEL {ORB_LEVEL, THREAD_LEVEL, OBJECT_LEVEL};

// Default is OBJECT level.
static LEVEL level = OBJECT_LEVEL;

// Default iterations.
static CORBA::ULong iterations = 100;

// Default amount of work.
static CORBA::ULong work = 0;

// Benchmark payload based operations?
static int payload_test = 0;

// Default payload size.
static CORBA::ULong payload_size = 0;

// Default number of invocations to buffer before flushing.
static CORBA::ULong buffering_queue_size = iterations / 2;

// Benchmark the twoway operation?
static int test_twoway = 0;

// Shut down server after test?
static int shutdown_server = 0;

// Default SyncScope value.
static Messaging::SyncScope sync_scope = Messaging::SYNC_WITH_TRANSPORT;

// Global scale factor.
static ACE_UINT32 gsf = 0;

static void
print_params (void)
{
  if (test_twoway)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "\nTesting twoway requests\n"));
    }
  else
    {
      const char *one_way_style = 0;
      if (sync_scope == Messaging::SYNC_NONE)
        one_way_style = "SYNC_NONE";
      else if (sync_scope == Messaging::SYNC_WITH_TRANSPORT)
        one_way_style = "SYNC_WITH_TRANSPORT";
      else if (sync_scope == Messaging::SYNC_WITH_SERVER)
        one_way_style = "SYNC_WITH_SERVER";
      else if (sync_scope == Messaging::SYNC_WITH_TARGET)
        one_way_style = "SYNC_WITH_TARGET";

      const char *payload_style = 0;
      if (payload_test)
        payload_style = "Payload based";
      else
        payload_style = "Work based";

      ACE_DEBUG ((LM_DEBUG,
                  "\nTesting oneway requests: %s : %s\n",
                  one_way_style,
                  payload_style));

      if (sync_scope == Messaging::SYNC_NONE)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Request queue limited to %d messages\n",
                      buffering_queue_size));
        }

      if (payload_test)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Payload size %d bytes\n",
                      payload_size));
        }
    }

  ACE_DEBUG ((LM_DEBUG,
              "%d invocations\n",
              iterations));
}

static void
twoway_work_test (Test_ptr server
                  TAO_ENV_ARG_DECL)
{
#if defined (USING_TIMERS)
  ACE_Throughput_Stats latency;
  ACE_hrtime_t base = ACE_OS::gethrtime ();
#endif /* USING_TIMERS */

#if defined (USING_QUANTIFY)
  // Reset Quantify data recording; whatever happened in the past is
  // not relevant to this test.
  QuantifyClearData ();
  QuantifyStartRecordingData ();
#endif /* USING_QUANTIFY */

  for (CORBA::ULong i = 0; i != iterations; ++i)
    {
#if defined (USING_TIMERS)
      ACE_hrtime_t latency_base = ACE_OS::gethrtime ();
#endif /* USING_TIMERS */

      server->twoway_work_test (work
                                TAO_ENV_ARG_PARAMETER);
      ACE_CHECK;

#if defined (USING_TIMERS)
      ACE_hrtime_t now = ACE_OS::gethrtime ();

      latency.sample (now - base,
                      now - latency_base);
#endif /* USING_TIMERS */
    }

#if defined (USING_QUANTIFY)
  // Stop recording data here; whatever happens after this in the test
  // is not relevant to this test.
  QuantifyStopRecordingData ();
#endif /* USING_QUANTIFY */

#if defined (USING_TIMERS)
  latency.dump_results ("Twoway", gsf);
#endif /* USING_TIMERS */
}

static void
oneway_work_test (Test_ptr server
                  TAO_ENV_ARG_DECL)
{
#if defined (USING_TIMERS)
  ACE_Throughput_Stats latency;
  ACE_hrtime_t base = ACE_OS::gethrtime ();
#endif /* USING_TIMERS */

#if defined (USING_QUANTIFY)
  // Reset Quantify data recording; whatever happened in the past is
  // not relevant to this test.
  QuantifyClearData ();
  QuantifyStartRecordingData ();
#endif /* USING_QUANTIFY */

  for (CORBA::ULong i = 0; i != iterations; ++i)
    {
#if defined (USING_TIMERS)
      ACE_hrtime_t latency_base = ACE_OS::gethrtime ();
#endif /* USING_TIMERS */

      server->oneway_work_test (work
                                TAO_ENV_ARG_PARAMETER);
      ACE_CHECK;

#if defined (USING_TIMERS)
      ACE_hrtime_t now = ACE_OS::gethrtime ();

      latency.sample (now - base,
                      now - latency_base);
#endif /* USING_TIMERS */
    }

#if defined (USING_QUANTIFY)
  // Stop recording data here; whatever happens after this in the test
  // is not relevant to this test.
  QuantifyStopRecordingData ();
#endif /* USING_QUANTIFY */

#if defined (USING_TIMERS)
  latency.dump_results ("Oneway (work based)", gsf);
#endif /* USING_TIMERS */
}

static void
oneway_payload_test (Test_ptr server
                     TAO_ENV_ARG_DECL)
{
#if defined (USING_TIMERS)
  ACE_Throughput_Stats latency;
  ACE_hrtime_t base = ACE_OS::gethrtime ();
#endif /* USING_TIMERS */

#if defined (USING_QUANTIFY)
  // Reset Quantify data recording; whatever happened in the past is
  // not relevant to this test.
  QuantifyClearData ();
  QuantifyStartRecordingData ();
#endif /* USING_QUANTIFY */

  Test::data the_data (payload_size);
  the_data.length (payload_size);

  for (CORBA::ULong i = 0; i != iterations; ++i)
    {
#if defined (USING_TIMERS)
      ACE_hrtime_t latency_base = ACE_OS::gethrtime ();
#endif /* USING_TIMERS */

      server->oneway_payload_test (the_data
                                   TAO_ENV_ARG_PARAMETER);
      ACE_CHECK;

#if defined (USING_TIMERS)
      ACE_hrtime_t now = ACE_OS::gethrtime ();

      latency.sample (now - base,
                      now - latency_base);
#endif /* USING_TIMERS */
    }

#if defined (USING_QUANTIFY)
  // Stop recording data here; whatever happens after this in the test
  // is not relevant to this test.
  QuantifyStopRecordingData ();
#endif /* USING_QUANTIFY */

#if defined (USING_TIMERS)
  latency.dump_results ("Oneway (payload based)", gsf);
#endif /* USING_TIMERS */
}

static int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "ps:k:i:t:l:m:w:x");
  int error = 0;
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 's':
        payload_size = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'p':
        payload_test = 1;
        break;

      case 'i':
        iterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case 't':
        {
          char *tmp = get_opts.opt_arg ();

          if (!ACE_OS::strcmp (tmp, "none"))
            sync_scope = Messaging::SYNC_NONE;
          else if (!ACE_OS::strcmp (tmp, "transport"))
            sync_scope = Messaging::SYNC_WITH_TRANSPORT;
          else if (!ACE_OS::strcmp (tmp, "server"))
            sync_scope = Messaging::SYNC_WITH_SERVER;
          else if (!ACE_OS::strcmp (tmp, "target"))
            sync_scope = Messaging::SYNC_WITH_TARGET;
          else if (!ACE_OS::strcmp (tmp, "twoway"))
            test_twoway = 1;
          else
            error = 1;
          break;
        }

      case 'l':
        {
          char *tmp = get_opts.opt_arg ();

          if (!ACE_OS::strcmp (tmp, "orb"))
            level = ORB_LEVEL;
          else if (!ACE_OS::strcmp (tmp, "thread"))
            level = THREAD_LEVEL;
          else if (!ACE_OS::strcmp (tmp, "object"))
            level = OBJECT_LEVEL;
          else
            error = 1;
          break;
        }

      case 'm':
        buffering_queue_size = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'w':
        work = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'x':
        shutdown_server = 1;
        break;

      case '?':
      default:
        error = 1;
        break;
      }

  if (error)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "usage:  %s "
                       "-k <ior> "
                       "-s <payload size> "
                       "-p <payload based test> "
                       "-i <# of iterations> "
                       "-t <none|transport|server|target|twoway> "
                       "-l <orb|thread|object> "
                       "-m <message count> "
                       "-w <# of server loops> "
                       "-x shutdown server "
                       "\n",
                       argv [0]),
                      -1);

  // Indicates sucessful parsing of the command line
  return 0;
}

int
set_rt_mode (void)
{
  int policy = ACE_SCHED_FIFO;
  int priority =
    (ACE_Sched_Params::priority_min (policy)
     + ACE_Sched_Params::priority_max (policy)) / 2;

  // Enable FIFO scheduling, e.g., RT scheduling class on Solaris.
  int result =
    ACE_OS::sched_params (ACE_Sched_Params (policy,
                                            priority,
                                            ACE_SCOPE_PROCESS));
  if (result != 0)
    {
      if (ACE_OS::last_error () == EPERM)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "client (%P|%t): user is not superuser, "
                      "test runs in time-shared class\n"));
        }
      else
        ACE_ERROR ((LM_ERROR,
                    "client (%P|%t): sched_params failed %p\n",
                    "set_rt_mode"));
    }

  // Get our thread handle.
  ACE_hthread_t self;
  ACE_OS::thr_self (self);

  // Set our thread priority.
  if (ACE_OS::thr_setprio (self, priority) != 0)
    ACE_ERROR ((LM_ERROR,
                "server (%P|%t):thr_setprio failed %p\n",
                "set_rt_mode"));

  // Do a sanity check.
  if (ACE_OS::thr_getprio (self, priority) == 0)
    ACE_DEBUG ((LM_DEBUG,
                "client (%P|%t): thread priority = %d.\n",
                priority));

  return 0;
}

int
main (int argc, char *argv[])
{
  int result = set_rt_mode ();
  if (result != 0)
    return result;

  ACE_TRY_NEW_ENV
    {
      // Calibrate the timer.
      gsf = ACE_High_Res_Timer::global_scale_factor ();

      // Initialize the ORB, the POA, and get the server reference.
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc,
                         argv,
                         ""
                         TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Get the command line options.
      if (parse_args (argc, argv) != 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "parse_args failed\n"),
                            1);
        }

      CORBA::Object_var obj =
        orb->resolve_initial_references ("ORBPolicyManager"
                                         TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::PolicyManager_var policy_manager =
        CORBA::PolicyManager::_narrow (obj.in ()
                                       TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      obj = orb->resolve_initial_references ("PolicyCurrent"
                                             TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::PolicyCurrent_var policy_current =
        CORBA::PolicyCurrent::_narrow (obj.in ()
                                       TAO_ENV_ARG_PARAMETER);

      ACE_TRY_CHECK;

      obj = orb->string_to_object (ior
                                   TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Test_var server = Test::_narrow (obj.in ()
                                       TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Print testing parameters.
      print_params ();

      // Run the test.
      if (test_twoway)
        {
          twoway_work_test (server.in ()
                            TAO_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      else
        {
          // Set up the sync scope any.
          CORBA::Any sync_scope_any;
          sync_scope_any <<= sync_scope;

          // Set the length of the policy list.
          CORBA::PolicyList sync_scope_policy_list (1);
          sync_scope_policy_list.length (1);

          // Set up the sync scope policy.
          sync_scope_policy_list[0] =
            orb->create_policy (Messaging::SYNC_SCOPE_POLICY_TYPE,
                                sync_scope_any
                                TAO_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          switch (level)
          {
            case ORB_LEVEL:
              // Set the sync scope policy at the ORB level.
              policy_manager->set_policy_overrides (sync_scope_policy_list,
                                                    CORBA::ADD_OVERRIDE
                                                    TAO_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;
              break;

            case THREAD_LEVEL:
              // Set the sync scope policy at the thread level.
              policy_current->set_policy_overrides (sync_scope_policy_list,
                                                    CORBA::ADD_OVERRIDE
                                                    TAO_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;
              break;

            case OBJECT_LEVEL:
              // Set the sync scope policy at the object level.
              obj = server->_set_policy_overrides (sync_scope_policy_list,
                                                   CORBA::ADD_OVERRIDE
                                                   TAO_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;

              // Get the new object reference with the updated policy.
              server = Test::_narrow (obj.in ()
                                      TAO_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;
              break;

            default:
              break;
          }

          // We are done with this policy.
          sync_scope_policy_list[0]->destroy (TAO_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          // Are we buffering the oneway requests?
          if (sync_scope == Messaging::SYNC_NONE)
            {
              TAO::BufferingConstraint buffering_constraint;
              buffering_constraint.mode = TAO::BUFFER_MESSAGE_COUNT;
              buffering_constraint.message_count = buffering_queue_size;
              buffering_constraint.message_bytes = 0;
              buffering_constraint.timeout = 0;

              // Set up the buffering constraint any.
              CORBA::Any buffering_constraint_any;
              buffering_constraint_any <<= buffering_constraint;

              // Set up the buffering constraint policy list.
              CORBA::PolicyList buffering_constraint_policy_list (1);
              buffering_constraint_policy_list.length (1);

              // Set up the buffering constraint policy.
              buffering_constraint_policy_list[0] =
                orb->create_policy (TAO::BUFFERING_CONSTRAINT_POLICY_TYPE,
                                    buffering_constraint_any
                                    TAO_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;

              // Set up the constraints (at the object level).
              obj = server->_set_policy_overrides (buffering_constraint_policy_list,
                                                   CORBA::ADD_OVERRIDE
                                                   TAO_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;

              // We are done with this policy.
              buffering_constraint_policy_list[0]->destroy (TAO_ENV_SINGLE_ARG_PARAMETER);
              ACE_TRY_CHECK;

              // Get the new object reference with the updated policy.
              server = Test::_narrow (obj.in ()
                                      TAO_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;
            }

          // Run the oneway test.
          if (payload_test)
            oneway_payload_test (server.in ()
                                 TAO_ENV_ARG_PARAMETER);
          else
            oneway_work_test (server.in ()
                              TAO_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      if (shutdown_server)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "\nShutting down server\n"));

          server->shutdown (TAO_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      // Destroy the ORB. On some platforms, e.g., Win32, the socket
      // library is closed at the end of main().  This means that any
      // socket calls made after main() fail. Hence if we wait for
      // static destructors to flush the queues, it will be too late.
      // Therefore, we use explicit destruction here and flush the
      // queues before main() ends.
      orb->destroy (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "client");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
