// $Id$

#include "MessengerC.h"
#include "tao/TimeBaseC.h"
#include "tao/Messaging/Messaging.h"
#include <iostream>
#include "ace/Get_Opt.h"

const ACE_TCHAR *ior = ACE_TEXT ("file://MessengerServer.ior");
long msecs = 0; // the RelativeRoundtripTimeout value

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:t:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;
      case 't':
        msecs = ACE_OS::atoi(get_opts.opt_arg ());
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "-t <msec> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int ACE_TMAIN(int argc, ACE_TCHAR * argv[])
{
  try {

    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

    if (parse_args (argc, argv) != 0)
      return 1;

    CORBA::Object_var obj = orb->string_to_object(ior);
    if (CORBA::is_nil(obj.in())) {
      std::cerr << "Nil Messenger reference" << std::endl;
      return 1;
    }
    Messenger_var messenger = Messenger::_narrow(obj.in());
    ACE_ASSERT(! CORBA::is_nil(messenger.in()));

    obj = orb->resolve_initial_references("ORBPolicyManager");
    CORBA::PolicyManager_var policy_manager = CORBA::PolicyManager::_narrow(obj.in());
    ACE_ASSERT(! CORBA::is_nil(policy_manager.in()));
    obj = orb->resolve_initial_references("PolicyCurrent");
    CORBA::PolicyCurrent_var policy_current = CORBA::PolicyCurrent::_narrow (obj.in());
    ACE_ASSERT(! CORBA::is_nil(policy_current.in()));

    // TimeT is in 10ths of a microsecond (10000 == 1.0e-3 * 1.0e7).
    if (msecs <= 0) {
      std::cout << "Enter relative roundtrip timeout in milliseconds -->";
      std::cin >> msecs;
    }
    TimeBase::TimeT relative_rt_timeout = msecs * 10000;

    CORBA::Any relative_rt_timeout_as_any;
    relative_rt_timeout_as_any <<= relative_rt_timeout;
    CORBA::PolicyList policy_list(1);
    policy_list.length(1);
    policy_list[0] =
      orb->create_policy(Messaging::RELATIVE_RT_TIMEOUT_POLICY_TYPE,
      relative_rt_timeout_as_any);

    // We demonstrate setting the policy at each of three levels. In practice,
    // you would only need to set the policy for one of these.
    // Set timeouts for the whole orb.
    policy_manager->set_policy_overrides(policy_list, CORBA::ADD_OVERRIDE);
    // Set timeouts for the current thread.
    policy_current->set_policy_overrides(policy_list, CORBA::ADD_OVERRIDE);
    // Set timeouts for the messenger object.
    obj = messenger->_set_policy_overrides(policy_list, CORBA::SET_OVERRIDE);
    messenger = Messenger::_narrow(obj.in());
    ACE_ASSERT(! CORBA::is_nil(messenger.in()));

    policy_list[0]->destroy();
    policy_list[0] = CORBA::Policy::_nil();

    CORBA::String_var message = CORBA::string_dup("Hello!");

    try {
      messenger->send_message ("TAO User", "TAO Test", message.inout());
      std::cerr << "ERROR: Message did not timeout." << std::endl;
    } catch(const CORBA::TIMEOUT&) {
      std::cout << "Message Timed out as expected." << std::endl;
    }
  }
  catch(const CORBA::Exception& ex) {
    std::cerr << "Caught a CORBA exception: " << ex << std::endl;
    return 1;
  }

  return 0;
}
