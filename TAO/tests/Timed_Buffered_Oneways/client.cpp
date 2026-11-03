
//=============================================================================
/**
 *  @file     client.cpp
 *
 *   This is a client that uses oneways with buffering constraints
 *   and roundtrip timeout constraints.
 *
 *  @author  Irfan Pyarali
 */
//=============================================================================

#include "testC.h"

#include "tao/Messaging/Messaging.h"
#include "tao/AnyTypeCode/TAOA.h"
#include "tao/AnyTypeCode/Any.h"
#include "tao/TAOC.h"
#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_sys_time.h"
#include <string>

// Eager buffering option.
static bool eager_buffering {};

// Name of file contains ior.
static std::basic_string<ACE_TCHAR> IOR = ACE_TEXT ("file://ior");

// Default iterations.
static u_long iterations {20};

// Default number of bytes to send as data.
static CORBA::ULong data_bytes {100000};

// Default roundtrip timeout (in milli seconds).
static long timeout {500};

// Default amount of work.
static CORBA::ULong work {3000};

// Time interval between invocation (in milli seconds).
static u_long interval {500};

// Flag indicates whether to shutdown remote server or not upon client
// shutdown.
static bool shutdown_server {};

static int
parse_args (int argc, ACE_TCHAR **argv)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("ek:i:d:t:w:z:x"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'e':
        eager_buffering = true;
        break;

      case 'k':
        IOR = get_opts.opt_arg ();
        break;

      case 'i':
        iterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'd':
        data_bytes = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 't':
        timeout = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'w':
        work = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'z':
        interval = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'x':
        shutdown_server = true;
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

  if (IOR.empty ())
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Please specify the IOR for the servant\n"), -1);

  // Indicates successful parsing of command line.
  return 0;
}

test_ptr
setup_policies (CORBA::ORB_ptr orb, test_ptr object)
{
  test_var object_with_policy;
  CORBA::PolicyList policy_list (1);
  if (timeout == -1)
    {
      object_with_policy = test::_duplicate (object);
    }
  else
    {
      policy_list.length (1);
      TimeBase::TimeT const rt_timeout = 10 * 1000 * timeout;

      CORBA::Any rt_timeout_any;
      rt_timeout_any <<= rt_timeout;
      policy_list[0] =
        orb->create_policy (Messaging::RELATIVE_RT_TIMEOUT_POLICY_TYPE,
                            rt_timeout_any);

      CORBA::Object_var object_temp =
        object->_set_policy_overrides (policy_list,
                                       CORBA::ADD_OVERRIDE);

      object_with_policy = test::_narrow (object_temp.in ());

      policy_list[0]->destroy ();
    }

  Messaging::SyncScope const sync =
    eager_buffering ? Messaging::SYNC_NONE : TAO::SYNC_DELAYED_BUFFERING;

  CORBA::Any sync_any;
  sync_any <<= sync;

  policy_list.length (1);
  policy_list[0] =
    orb->create_policy (Messaging::SYNC_SCOPE_POLICY_TYPE,
                        sync_any);

  CORBA::Object_var object_temp =
    object_with_policy->_set_policy_overrides (policy_list,
                                               CORBA::ADD_OVERRIDE);

  test_var object_with_two_policies = test::_narrow (object_temp.in ());

  policy_list[0]->destroy ();

  return object_with_two_policies._retn ();
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      // Initialize the ORB.
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv);

      // Initialize options based on command-line arguments.
      int const parse_args_result = parse_args (argc, argv);
      if (parse_args_result != 0)
        return parse_args_result;

      // Get an object reference from the argument string.
      CORBA::Object_var const object =
        orb->string_to_object (IOR.c_str ());

      // Try to narrow the object reference to a <test> reference.
      test_var const test_object_no_policy = test::_narrow (object.in ());

      // Setup buffering and timeout
      test_var const test_object = setup_policies (orb.in (),
                                                   test_object_no_policy.in ());

      test::data the_data (data_bytes);
      the_data.length (data_bytes);

      for (CORBA::ULong i = 1; i <= iterations; ++i)
        {
          ACE_Time_Value const start = ACE_OS::gettimeofday ();
          ACE_Time_Value end;
          try
            {
              // Invoke the oneway method.
              test_object->method (i,
                                   start.msec (),
                                   the_data,
                                   work);
              end = ACE_OS::gettimeofday ();

              ACE_DEBUG ((LM_DEBUG,
                          "client:\t%d took\t%dms\n",
                          i, (end - start).msec ()));

              // Interval between successive calls.
              ACE_Time_Value sleep_interval (0, interval * 1000);

              // If we don't run the orb, then no data will be sent, and no
              // connection will be made initially.
              orb->run (sleep_interval);
            }
          catch (CORBA::TIMEOUT const &)
            {
              // The timeout could be from a previous loop.
              // A simplistic analysis could incorrectly conclude
              // that as an unexpected timeout exception.
              // We need to maintain a base start time thats updated
              // in each iteration.

              ACE_DEBUG ((LM_DEBUG, "client: caught expected timeout\n"));
            }
        }

      ACE_DEBUG ((LM_DEBUG, "client: flushing\n"));
      test_object_no_policy->flush ();

      ACE_DEBUG ((LM_DEBUG, "client: Shutting down...\n"));
      if (shutdown_server)
        {
          ACE_DEBUG ((LM_DEBUG,"client killing server\n"));
          long const now = ACE_OS::gettimeofday ().msec ();
          test_object_no_policy->shutdown (now);
        }

      orb->shutdown (true);

      // Destroy the ORB.  On some platforms, e.g., Win32, the socket
      // library is closed at the end of main().  This means that any
      // socket calls made after main() fail. Hence if we wait for
      // static destructors to flush the queues, it will be too late.
      // Therefore, we use explicit destruction here and flush the
      // queues before main() ends.
      orb->destroy ();
    }
  catch (CORBA::Exception const & ex)
    {
      ex._tao_print_exception ("Client side exception caught:");
      return -1;
    }

  return 0;
}
