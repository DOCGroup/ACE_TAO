// $Id$

#include "_pch.h"
#include "Message_i.h"
#include <iostream>

const char* server_ior = "file://server.ior";

int ACE_TMAIN (int ac, ACE_TCHAR* av[]) {

  try {

    CORBA::ORB_var orb = CORBA::ORB_init(ac, av);

    // Normally we wouldn't have to register the factory in the client, but
    // in this case the valuetype will be returned as an inout parameter, and
    // we'll need the factory to support this.
    MessageFactory::register_new_factory(* orb.in());

    CORBA::Object_var obj = orb->string_to_object(server_ior);
    Messenger_var tst = Messenger::_narrow(obj.in());
    ACE_ASSERT(! CORBA::is_nil(tst.in()));

    Message_var msg = new MessageImpl("Son", "Mom", "Dinner's Ready.", "Hurry home.");

    if (tst->send_message(msg)) {

      std::cout << "Message sent successfully.\n";
      msg->print();
      std::cout << std::endl;

    } else {
      std::cout << "Message refused." << std::endl;
    }

    while (orb->work_pending()) {
      orb->perform_work();
    }

    orb->destroy();

  } catch(const CORBA::Exception& e) {
    std::cerr << e << std::endl;
    return 1;
  }

  return 0;
}
