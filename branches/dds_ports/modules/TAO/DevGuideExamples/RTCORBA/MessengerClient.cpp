// $Id$

#include "MessengerC.h"
#include "common.h"
#include <iostream>
#include "tao/RTCORBA/RTCORBA.h"

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try {
    // Initialize orb
    CORBA::ORB_var orb = CORBA::ORB_init( argc, argv );

    // Get the RTORB
    CORBA::Object_var obj = orb->resolve_initial_references("RTORB");
    RTCORBA::RTORB_var rt_orb = RTCORBA::RTORB::_narrow (obj.in());

    // PolicyCurrent.
    obj = orb->resolve_initial_references("PolicyCurrent");
    CORBA::PolicyCurrent_var policy_current =
              CORBA::PolicyCurrent::_narrow(obj.in());
    if (CORBA::is_nil(policy_current.in())) {
      std::cerr << "Unable to narrow the PolicyCurrent" << std::endl;
      return 1;
    }

    // Destringify ior
    obj = orb->string_to_object( "file://Messenger.ior" );
    if( CORBA::is_nil( obj.in() ) ) {
      std::cerr << "Nil Messenger reference" << std::endl;
      return 1;
    }

    // Narrow
    Messenger_var messenger = Messenger::_narrow( obj.in() );
    if( CORBA::is_nil( messenger.in() ) ) {
      std::cerr << "Argument is not a Messenger reference" << std::endl;
      return 1;
    }

    // Set the Private Connection Policy
    CORBA::PolicyList policy_list(1);
    policy_list.length (1);
    policy_list[0] = rt_orb->create_private_connection_policy();
    policy_current->set_policy_overrides (policy_list,
                                          CORBA::SET_OVERRIDE);

    // Get the RTCurrent.
    obj = orb->resolve_initial_references ("RTCurrent");
    RTCORBA::Current_var current =
        RTCORBA::Current::_narrow(obj.in ());

    // Change to a priority that matches the server
    current->the_priority(0);

    // Explicitly bind a connection to the server
    CORBA::PolicyList_var inconsistent_policies;
    CORBA::Boolean status =
         messenger->_validate_connection(inconsistent_policies.out());
    if (!status) {
      std::cerr << "Unable to explicitly bind to the server" << std::endl;
      return 1;
    }

    static const CORBA::Short increment = get_increment();
    for(CORBA::ULong i = 0; i < get_total_lanes(); i++) {

      // Set the priority to one that matches one of the lanes
      CORBA::Short priority = i * increment;
      current->the_priority(priority);

      // Send the message
      CORBA::String_var message = CORBA::string_dup( "Hello!" );
      messenger->send_message( "TAO User", "TAO Test", message.inout() );
    }
  }
  catch(const CORBA::Exception& ex) {
    std::cerr << "MessengerClient caught CORBA exception: " << ex << std::endl;
    return 1;
  }
  catch(...) {
    std::cerr << "MessengerClient exception" << std::endl;
    return 1;
  }

  std::cout << "messages were sent" << std::endl;
  return 0;
}
