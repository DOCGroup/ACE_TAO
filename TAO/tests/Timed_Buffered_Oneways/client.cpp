// $Id$

// ================================================================
//
// = FILENAME
//     client.cpp
//
// = DESCRIPTION
//     This is a client that uses oneways with buffering constraints
//     and roundtrip timeout constraints.
//
// = AUTHOR
//     Irfan Pyarali
//
// ================================================================

#include "testC.h"

#include "tao/Messaging.h"
#include "tao/TAOC.h"
#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"

ACE_RCSID(Timed_Buffered_Oneways, client, "$Id$")

// Eager buffering option.
static int eager_buffering = 0;

// Name of file contains ior.
static const char *IOR = "file://ior";

// Default iterations.
static u_long iterations = 20;

// Default number of bytes to send as data.
static CORBA::ULong data_bytes = 100000;

// Default roundtrip timeout (in milli seconds).
static long timeout = 500;

// Default amount of work.
static CORBA::ULong work = 3000;

// Time interval between invocation (in milli seconds).
static u_long interval = 500;

// Flag indicates whether to shutdown remote server or not upon client
// shutdown.
static int shutdown_server = 0;

static int
parse_args (int argc, char **argv)
{
  ACE_Get_Opt get_opts (argc, argv, "ek:i:d:t:w:z:x");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'e':
        eager_buffering = 1;
        break;

      case 'k':
        IOR = get_opts.opt_arg ();
        break;

      case 'i':
        iterations = ::atoi (get_opts.opt_arg ());
        break;

      case 'd':
        data_bytes = ::atoi (get_opts.opt_arg ());
        break;

      case 't':
        timeout = ::atoi (get_opts.opt_arg ());
        break;

      case 'w':
        work = ::atoi (get_opts.opt_arg ());
        break;

      case 'z':
        interval = ::atoi (get_opts.opt_arg ());
        break;

      case 'x':
        shutdown_server = 1;
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-e eager buffering [default is delayed] "
                           "-k IOR "
                           "-i iterations "
                           "-d data bytes "
                           "-t roundtrip timeout "
                           "-w remote work "
                           "-z interval between calls "
                           "-x shutdown server "
                           "\n",
                           argv [0]),
                          -1);
      }

  if (IOR == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Please specify the IOR for the servant\n"), -1);

  // Indicates successful parsing of command line.
  return 0;
}

void
setup_timeouts (CORBA::ORB_ptr orb
                TAO_ENV_ARG_DECL)
{
  // Escape value.
  if (timeout == -1)
    return;

  // Obtain PolicyCurrent.
  CORBA::Object_var object = orb->resolve_initial_references ("PolicyCurrent"
                                                              TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Narrow down to correct type.
  CORBA::PolicyCurrent_var policy_current =
    CORBA::PolicyCurrent::_narrow (object.in ()
                                   TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  TimeBase::TimeT rt_timeout = 10000 * timeout;

  CORBA::Any rt_timeout_any;
  rt_timeout_any <<= rt_timeout;

  CORBA::PolicyList rt_timeout_policy_list (1);
  rt_timeout_policy_list.length (1);

  rt_timeout_policy_list[0] =
    orb->create_policy (Messaging::RELATIVE_RT_TIMEOUT_POLICY_TYPE,
                        rt_timeout_any
                        TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  policy_current->set_policy_overrides (rt_timeout_policy_list,
                                        CORBA::ADD_OVERRIDE
                                        TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  rt_timeout_policy_list[0]->destroy (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

void
setup_buffering_constraints (CORBA::ORB_ptr orb
                             TAO_ENV_ARG_DECL)
{
  // Obtain PolicyCurrent.
  CORBA::Object_var object = orb->resolve_initial_references ("PolicyCurrent"
                                                              TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Narrow down to correct type.
  CORBA::PolicyCurrent_var policy_current =
    CORBA::PolicyCurrent::_narrow (object.in ()
                                   TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Setup the sync scope policy, i.e., the ORB will buffer oneways.
  Messaging::SyncScope sync =
    eager_buffering ? Messaging::SYNC_EAGER_BUFFERING : Messaging::SYNC_DELAYED_BUFFERING;

  // Setup the sync scope any.
  CORBA::Any sync_any;
  sync_any <<= sync;

  // Setup the sync scope policy list.
  CORBA::PolicyList sync_policy_list (1);
  sync_policy_list.length (1);

  // Setup the sync scope policy.
  sync_policy_list[0] =
    orb->create_policy (Messaging::SYNC_SCOPE_POLICY_TYPE,
                        sync_any
                        TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Setup the sync scope.
  policy_current->set_policy_overrides (sync_policy_list,
                                        CORBA::ADD_OVERRIDE
                                        TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // We are now done with this policy.
  sync_policy_list[0]->destroy (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // Flush buffers.
  TAO::BufferingConstraint buffering_constraint;
  buffering_constraint.mode = TAO::BUFFER_FLUSH;
  buffering_constraint.message_count = 0;
  buffering_constraint.message_bytes = 0;
  buffering_constraint.timeout = 0;

  // Setup the buffering constraint any.
  CORBA::Any buffering_constraint_any;
  buffering_constraint_any <<= buffering_constraint;

  // Setup the buffering constraint policy list.
  CORBA::PolicyList buffering_constraint_policy_list (1);
  buffering_constraint_policy_list.length (1);

  // Setup the buffering constraint policy.
  buffering_constraint_policy_list[0] =
    orb->create_policy (TAO::BUFFERING_CONSTRAINT_POLICY_TYPE,
                        buffering_constraint_any
                        TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Setup the constraints.
  policy_current->set_policy_overrides (buffering_constraint_policy_list,
                                        CORBA::ADD_OVERRIDE
                                        TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // We are done with the policy.
  buffering_constraint_policy_list[0]->destroy (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

int
main (int argc, char **argv)
{
  TAO_ENV_DECLARE_NEW_ENV;

  ACE_TRY
    {
      // Initialize the ORB.
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc,
                         argv,
                         0
                         TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Initialize options based on command-line arguments.
      int parse_args_result = parse_args (argc, argv);
      if (parse_args_result != 0)
        return parse_args_result;

      // Get an object reference from the argument string.
      CORBA::Object_var object =
        orb->string_to_object (IOR
                               TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Try to narrow the object reference to a <test> reference.
      test_var test_object = test::_narrow (object.in ()
                                            TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Setup buffering.
      setup_buffering_constraints (orb.in ()
                                   TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Setup timeout.
      setup_timeouts (orb.in ()
                      TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      test::data the_data (data_bytes);
      the_data.length (data_bytes);

      for (CORBA::ULong i = 1; i <= iterations; ++i)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "client: Iteration %d @ %T\n",
                      i));

          // Invoke the oneway method.
          test_object->method (i,
                               the_data,
                               work
                               TAO_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          // Interval between successive calls.
          ACE_Time_Value sleep_interval (0,
                                         interval * 1000);

          ACE_OS::sleep (sleep_interval);
        }

      // Shutdown server.
      if (shutdown_server)
        {
          test_object->shutdown (TAO_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      // Destroy the ORB.  On some platforms, e.g., Win32, the socket
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
                           "Exception caught:");
      return -1;
    }
  ACE_ENDTRY;

  ACE_CHECK_RETURN (-1);

  return 0;
}
