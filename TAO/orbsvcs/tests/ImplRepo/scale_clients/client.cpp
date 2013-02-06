// $Id$

#include "TestC.h"

#include "tao/TimeBaseC.h"
#include "tao/Messaging/Messaging.h"

#include "ace/Get_Opt.h"
#include "ace/OS_NS_unistd.h"

#include <iostream>

int request_delay_secs = 0;
// 0 Indicates don't apply RT timeout policy
long rt_timeout_msecs = 0;
int max_tries = 1;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("d:r:m:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':
        request_delay_secs = ACE_OS::atoi(get_opts.opt_arg ());
        break;

      case 'r':
        rt_timeout_msecs = ACE_OS::atoi(get_opts.opt_arg ());
        break;

      case 'm':
        max_tries = ACE_OS::atoi(get_opts.opt_arg ());
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-d <request delay in seconds> "
                           "-r <rount trip timeout in milliseconds> "
                           "-m <max tries if RT timeout failures> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try {
    // Initialize orb
    CORBA::ORB_var orb = CORBA::ORB_init( argc, argv );

    if (parse_args (argc, argv) != 0)
      return 1;

    CORBA::Object_var obj;

    ///// Specify the relative round trip policy /////

    if (rt_timeout_msecs > 0)
      {

    // Timeout specified in hundreds of nanoseconds which is
    // 10000 milliseconds.
    TimeBase::TimeT relative_rt_timeout = rt_timeout_msecs * 10000;

    CORBA::Any relative_rt_timeout_as_any;
    relative_rt_timeout_as_any <<= relative_rt_timeout;
    CORBA::PolicyList policy_list(1);
    policy_list.length(1);
    policy_list[0] =
      orb->create_policy(Messaging::RELATIVE_RT_TIMEOUT_POLICY_TYPE,
      relative_rt_timeout_as_any);

    // Apply the policy at the ORB level.
    obj = orb->resolve_initial_references("ORBPolicyManager");
    CORBA::PolicyManager_var policy_manager =
      CORBA::PolicyManager::_narrow(obj.in());
    policy_manager->set_policy_overrides (policy_list, CORBA::ADD_OVERRIDE);

    // Destroy the Policy objects.
    for (CORBA::ULong i = 0; i < policy_list.length(); ++i) {
      policy_list[i]->destroy ();
    }
    policy_list.length(0);

      }

    ///// Get object reference /////

    obj = orb->resolve_initial_references("Test");
    ACE_ASSERT (!CORBA::is_nil(obj.in()));
    Test_var test = Test::_narrow( obj.in() );
    ACE_ASSERT (!CORBA::is_nil(test.in()));

    if (max_tries > 1)
      {
        ACE_DEBUG ((LM_DEBUG,
                    "Maximum number of tries = %d\n",
                    max_tries));
      }

    CORBA::Short n;
    for (int i = 0; i < max_tries; ++i)
      {
        try
          {
            n = test->get_num_requests (request_delay_secs);
          }
        catch (const CORBA::TIMEOUT &ex)
          {
            ex._tao_print_exception ("timeout exception:");
            if (i == max_tries - 1)
              throw;
          }
      }

    return 0;

  }
  catch(const CORBA::Exception& ex) {
    ex._tao_print_exception ("client:");
  }

  return -1;
}
