// $Id$

#include "MessengerC.h"
#include "MessengerHandler.h"

#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_sys_time.h"
#include <iostream>

//-----------------------------------------------------------------------------

int
ACE_TMAIN (int argc, ACE_TCHAR *argv [])
{
  try {

    // assume any command line parameter means we want an automated test.
    bool automated = argc > 1;

    // Initialize orb
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

    CORBA::Object_var obj = orb->string_to_object("file://MessengerServer.ior");
    if (CORBA::is_nil(obj.in())) {
      std::cerr << "Nil Messenger reference" << std::endl;
      return 1;
    }

    // Narrow
    Messenger_var messenger = Messenger::_narrow(obj.in());
    if (CORBA::is_nil(messenger.in())) {
      std::cerr << "Argument is not a Messenger reference" << std::endl;
      return 1;
    }

    // Get reference to Root POA.
    obj = orb->resolve_initial_references("RootPOA");
    PortableServer::POA_var poa = PortableServer::POA::_narrow(obj.in());

    // Activate POA manager
    PortableServer::POAManager_var mgr = poa->the_POAManager();
    mgr->activate();

    // Register an AMI handler for the Messenger interface
    MessengerHandler servant(orb.in());
    PortableServer::ObjectId_var oid = poa->activate_object(&servant);
    obj = poa->id_to_reference(oid.in());
    AMI_MessengerHandler_var handler = AMI_MessengerHandler::_narrow(obj.in());

    CORBA::String_var user = CORBA::string_alloc(81);
    CORBA::String_var subject = CORBA::string_alloc(81);
    CORBA::String_var message = CORBA::string_alloc(81);

    if (! automated) {
      std::cout << "Enter user name -->";
      std::cin.getline(user, 81);

      std::cout << "Enter subject -->";
      std::cin.getline(subject, 81);

      std::cout << "Enter message -->";
      std::cin.getline(message, 81);
    } else {
      user = CORBA::string_dup("TestUser");
      subject = CORBA::string_dup("TestSubject");
      message = CORBA::string_dup("Have a nice day.");
    }

    // Record the time the request was made.
    ACE_Time_Value time_sent = ACE_OS::gettimeofday();

    messenger->sendc_send_message(handler.in(), user.in(), subject.in(), message.in());

    // Do some work to prove that we can send the message asynchronously, do some work,
    // then come back later and retrieve the results.
    for (int i = 0; i < 10; ++i) {
      ACE_OS::printf(".");
      ACE_OS::sleep(ACE_Time_Value(0, 10 * 1000));
    }

    // Our simple servant will exit as soon as it receives the results.
    orb->run();

    if (servant.message_was_sent())
    {
      // Note : We can't use the
      ACE_Time_Value delay = ACE_OS::gettimeofday() - time_sent;
      std::cout << std::endl << "Reply Delay = " << delay.msec() << "ms" << std::endl;
    }

    orb->destroy();
  }
  catch(const CORBA::Exception& ex) {
    std::cerr << "Caught a CORBA::Exception: " << ex << std::endl;
    return 1;
  }

  return 0;
}
