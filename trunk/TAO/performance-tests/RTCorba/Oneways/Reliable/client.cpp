// $Id$

#include "testC.h"
#include "ace/Get_Opt.h"
#include "ace/Sched_Params.h"
#include "ace/Stats.h"
#include "ace/High_Res_Timer.h"

ACE_RCSID(Reliable, client, "$Id$")

// 'File format' IOR
static const char *ior = "file://test.ior";

// Default iterations.
static CORBA::ULong iterations = 100;

// Default argument value.
static CORBA::ULong work = 10;

// Default number of invocations to buffer before flushing.
static CORBA::Long message_limit = -1;

// Benchmark the twoway operation?
static int test_twoway = 0;

// Shut down server after test?
static int do_shutdown = 0;

// Default SyncScope value.
Messaging::SyncScope sync_scope = Messaging::SYNC_WITH_TRANSPORT;

// Create a sync scope policy list.
CORBA::PolicyList sync_scope_policy_list (1);

//ACE_UINT64 *hist_array;

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
      ACE_DEBUG ((LM_DEBUG,
                  "\nTesting oneway requests\n"));

      switch (sync_scope)
        {
          case 0:
            ACE_DEBUG ((LM_DEBUG,
                        "SyncScope = SYNC_NONE\n"));
            break;
          case 1:
            ACE_DEBUG ((LM_DEBUG,
                        "SyncScope = SYNC_WITH_TRANSPORT\n"));
            break;
          case 2:
            ACE_DEBUG ((LM_DEBUG,
                        "SyncScope = SYNC_WITH_SERVER\n"));
            break;
          case 3:
            ACE_DEBUG ((LM_DEBUG,
                        "SyncScope = SYNC_WITH_TARGET\n"));
            break;
          default:
            break;
        }
    }

  ACE_DEBUG ((LM_DEBUG,
              "%d invocations\n",
              iterations));

  if (!test_twoway && sync_scope == Messaging::SYNC_NONE)
    {
      if (message_limit != -1)
        ACE_DEBUG ((LM_DEBUG,
                    "Request queue limited to %d messages\n",
                    message_limit));
    }
}

static void 
twoway_test (Test_ptr servant,
             ACE_UINT32 gsf,
             CORBA::Environment &ACE_TRY_ENV)
{
  ACE_Throughput_Stats latency;

  ACE_hrtime_t base = ACE_OS::gethrtime ();

  for (CORBA::ULong i = 0; i != iterations; ++i)
    {
      ACE_hrtime_t latency_base = ACE_OS::gethrtime ();
      
      servant->twoway_op (work,
                          ACE_TRY_ENV);

      ACE_hrtime_t now = ACE_OS::gethrtime ();

      ACE_CHECK;

      latency.sample (now - base,
                      now - latency_base);
    }

  print_params ();

  latency.dump_results ("Twoway", gsf);
}

static void 
reliable_test (Test_ptr servant,
               ACE_UINT32 gsf,
               CORBA::Environment &ACE_TRY_ENV)
{
  ACE_Throughput_Stats latency;

  ACE_hrtime_t base = ACE_OS::gethrtime ();

  for (CORBA::ULong i = 0; i != iterations; ++i)
    {
      ACE_hrtime_t latency_base = ACE_OS::gethrtime ();
      
      servant->oneway_op (work,
                          ACE_TRY_ENV);

      ACE_hrtime_t now = ACE_OS::gethrtime ();

      ACE_CHECK;

//      hist_array[i] = now - latency_base;

      latency.sample (now - base,
                      now - latency_base);
    }

  print_params ();

  latency.dump_results ("Reliable oneway", gsf);
}

static void 
buffered_test (Test_ptr servant,
               ACE_UINT32 gsf,
               CORBA::Environment &ACE_TRY_ENV)
{
  ACE_Throughput_Stats latency;

  ACE_hrtime_t base = ACE_OS::gethrtime ();

  for (CORBA::ULong i = 0; i != iterations; ++i)
    {
      ACE_hrtime_t latency_base = ACE_OS::gethrtime ();
      
      servant->oneway_op (0,
                          ACE_TRY_ENV);

      ACE_hrtime_t now = ACE_OS::gethrtime ();

      ACE_CHECK;

//      hist_array[i] = now - latency_base;

      latency.sample (now - base,
                      now - latency_base);
    }

  print_params ();

  latency.dump_results ("Buffered oneway", gsf);
}

void flush_queue (Test_var &server,
                  CORBA::ORB_var &orb,
                  CORBA::Object_var &obj,
                  CORBA::Environment &ACE_TRY_ENV)
{  
  // Set up the flush sync scope Any.
  CORBA::Any sync_flush_any;
  sync_flush_any <<= Messaging::SYNC_FLUSH;

  // Set up the flush sync scope policy list.
  CORBA::PolicyList sync_flush_policy_list (1);
  sync_flush_policy_list.length (1);

  // Set up the flush sync scope policy.
  sync_flush_policy_list[0] =
  orb->create_policy (Messaging::SYNC_SCOPE_POLICY_TYPE,
                      sync_flush_any,
                      ACE_TRY_ENV);
  ACE_CHECK;

  // Set up explicit flushing.
  obj = server->_set_policy_overrides (sync_flush_policy_list,
                                       CORBA::ADD_OVERRIDE,
                                       ACE_TRY_ENV);
  ACE_CHECK;

  // Get the new object reference with the update policy.
  server = Test::_narrow (obj.in (),
                          ACE_TRY_ENV);
  ACE_CHECK;

  // Flush the queue and/or establish the connection.
  server->oneway_op (0,
                     ACE_TRY_ENV);
  ACE_CHECK;

  // Done with this policy.
  sync_flush_policy_list[0]->destroy (ACE_TRY_ENV);
  ACE_CHECK;

  // Now set the policy back to SYNC_NONE.
  obj = server->_set_policy_overrides (sync_scope_policy_list,
                                       CORBA::ADD_OVERRIDE,
                                       ACE_TRY_ENV);
  ACE_CHECK;

  server = Test::_narrow (obj.in (),
                          ACE_TRY_ENV);
  ACE_CHECK;              
}

static int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "i:t:m:w:x");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
        case 'i':
          iterations = ACE_OS::atoi (get_opts.optarg);
          break;
        case 't':
          {
            char *tmp = get_opts.optarg;

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
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "usage:  %s "
                                 "-i <# of iterations> "
                                 "-t <none|transport|server|target|twoway> "
                                 "-m <message count> "
                                 "-w <# of servant loops of 1000> "
                                 "-x shutdown server "
                                 "\n",
                                 argv [0]),
                                -1);
            
            break;
          }
        case 'm':
          message_limit = ACE_OS::atoi (get_opts.optarg);
          break;
        case 'w':
          work = ACE_OS::atoi (get_opts.optarg);
          break;
        case 'x':
          do_shutdown = 1;
          break;
        case '?':
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             "usage:  %s "
                             "-i <# of iterations> "
                             "-t <none|transport|server|target> "
                             "-m <message count> "
                             "-w <# of servant loops of 1000> "
                             "-x shutdown server "
                             "\n",
                             argv [0]),
                            -1);
      }

  // Indicates sucessful parsing of the command line
  return 0;
}

int
main (int argc, char *argv[])
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
        ACE_ERROR_RETURN ((LM_ERROR,
                           "client (%P|%t): sched_params failed\n"),
                          1);
    }

  // Get our thread handle.
  ACE_hthread_t self;
  ACE_OS::thr_self (self);

  // Set our thread priority.
  if (ACE_OS::thr_setprio (self, priority) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "server (%P|%t):thr_setprio failed\n"),
                      1);

  // Do a sanity check.
  if (ACE_OS::thr_getprio (self, priority) == 0)
    ACE_DEBUG ((LM_DEBUG, 
                "server (%P|%t): thread priority = %d.\n", 
                priority));

  ACE_TRY_NEW_ENV
    {
      // Calibrate the timer.
      ACE_DEBUG ((LM_DEBUG, 
                  "High res. timer calibration...."));
      ACE_UINT32 gsf = ACE_High_Res_Timer::global_scale_factor ();
      ACE_DEBUG ((LM_DEBUG, 
                  "done\n"));

      // Initialize the ORB, the POA, and get the server reference.

      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, 
                         argv, 
                         "", 
                         ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CORBA::Object_var obj =
        orb->resolve_initial_references ("RootPOA",
                                         ACE_TRY_ENV);
      ACE_TRY_CHECK;

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (obj.in (),
                                      ACE_TRY_ENV);
      ACE_TRY_CHECK;

      obj = orb->string_to_object (ior, 
                                   ACE_TRY_ENV);
      ACE_TRY_CHECK;

      Test_var server = Test::_narrow (obj.in (), 
                                       ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (server.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object reference <%s> is nil\n",
                             ior),
                            1);
        }

      // Get the command line options.
      if (parse_args (argc, argv) != 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "parse_args failed\n"),
                            1);
        }

//      hist_array = new ACE_UINT64[iterations];

      // Run the test.
      if (test_twoway)
        {
          // Establish the connection
          server->twoway_op (0,
                             ACE_TRY_ENV);
          ACE_TRY_CHECK;

          twoway_test (server.in (),
                       gsf,
                       ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
      else
        {
          // Set up the sync scope any.
          CORBA::Any sync_scope_any;
          sync_scope_any <<= sync_scope;

          // Set the length of the policy list.
          sync_scope_policy_list.length (1);

          // Set up the sync scope policy.
          sync_scope_policy_list[0] = 
            orb->create_policy (Messaging::SYNC_SCOPE_POLICY_TYPE,
                                sync_scope_any,
                                ACE_TRY_ENV);
          ACE_TRY_CHECK;

          // Set up the sync scope (at the object level).
          obj = 
            server->_set_policy_overrides (sync_scope_policy_list,
                                           CORBA::ADD_OVERRIDE,
                                           ACE_TRY_ENV);
          ACE_TRY_CHECK;

          // Get the new object reference with the updated policy.
          server = Test::_narrow (obj.in (),
                                  ACE_TRY_ENV);
          ACE_TRY_CHECK;

          // Are we buffering the oneway requests?
          if (sync_scope == Messaging::SYNC_NONE)
            {
              // Start off with no constraints.
              TAO::BufferingConstraint buffering_constraint;
              buffering_constraint.mode = TAO::BUFFER_NONE;

              // If valid <message_limit>, set the implicit flushing to
              // account for queued messages.
              if (message_limit != -1)
                {
                  buffering_constraint.mode |= TAO::BUFFER_MESSAGE_COUNT;
                  buffering_constraint.message_count = message_limit;
                }

              // Set up the buffering constraint any.
              CORBA::Any buffering_constraint_any;
              buffering_constraint_any <<= buffering_constraint;

              // Set up the buffering constraint policy list.
              CORBA::PolicyList buffering_constraint_policy_list (1);
              buffering_constraint_policy_list.length (1);

              // Set up the buffering constraint policy.
              buffering_constraint_policy_list[0] =
                orb->create_policy (TAO::BUFFERING_CONSTRAINT_POLICY_TYPE,
                                    buffering_constraint_any,
                                    ACE_TRY_ENV);
              ACE_TRY_CHECK;

              // Set up the constraints (at the object level).
              obj = server->_set_policy_overrides (
                        buffering_constraint_policy_list,
                        CORBA::ADD_OVERRIDE,
                        ACE_TRY_ENV
                      );
              ACE_TRY_CHECK;

              // We are done with this policy.
              buffering_constraint_policy_list[0]->destroy (ACE_TRY_ENV);
              ACE_TRY_CHECK;

              // Get the new object reference with the updated policy.
              server = Test::_narrow (obj.in (),
                                      ACE_TRY_ENV);
              ACE_TRY_CHECK;

              // Flush the queue and establish the connection.
              flush_queue (server,
                           orb,
                           obj,
                           ACE_TRY_ENV);
              ACE_TRY_CHECK;
            }
          else
            {
              // Establish the connection.
              server->oneway_op (0,
                                 ACE_TRY_ENV);
              ACE_TRY_CHECK;
            }

          // Run the oneway test.
          if (sync_scope == Messaging::SYNC_NONE
              || sync_scope == Messaging::SYNC_WITH_TRANSPORT)
            {
              buffered_test (server.in (),
                             gsf,
                             ACE_TRY_ENV);
              ACE_TRY_CHECK;
            }
          else
            {
              reliable_test (server.in (),
                             gsf,
                             ACE_TRY_ENV);
              ACE_TRY_CHECK;
            }

          // We are done with this policy.
          sync_scope_policy_list[0]->destroy (ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
/*
      for (CORBA::ULong i = 0; i < iterations; i++)
        ACE_DEBUG ((LM_DEBUG,
                    "%Q\n",
                    hist_array[i]));

      delete [] hist_array;
*/
      root_poa->destroy (1,
                         1,
                         ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (do_shutdown)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "\nShutting down server\n"));

          server->shutdown (ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }

      // Destroy the ORB. On some platforms, e.g., Win32, the socket
      // library is closed at the end of main().  This means that any
      // socket calls made after main() fail. Hence if we wait for
      // static destructors to flush the queues, it will be too late.
      // Therefore, we use explicit destruction here and flush the
      // queues before main() ends.
      orb->destroy (ACE_TRY_ENV);
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
