// $Id$

#include "MessengerC.h"
#include "tao/TimeBaseC.h"
#include "tao/Messaging/Messaging.h"
#include <iostream>
int ACE_TMAIN(int argc, ACE_TCHAR * argv[])
{
  try {

    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

    CORBA::Object_var obj = orb->string_to_object("file://MessengerServer.ior");
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

    // Set the RelativeRoundtripTimeout value
    long msecs = 0;
    if (argc > 1) {
      msecs = ACE_OS::atoi(argv[1]);
    } else {
      std::cout << "Enter relative roundtrip timeout in milliseconds -->";
      std::cin >> msecs;
    }
    // TimeT is in 10ths of a microsecond (10000 == 1.0e-3 * 1.0e7).
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
