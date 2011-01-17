// $Id$

#include "NamingTask.h"
#include "orbsvcs/Naming/Naming_Server.h"
#include "ace/OS_NS_unistd.h"
#include <iostream>

NamingTask::NamingTask (int argc, ACE_TCHAR** argv)
: argc_ (argc),
  argv_ (argv),
  initialized_(false)
{
  orb_ = CORBA::ORB_init(argc, argv, "NamingORB");
}

void NamingTask::waitInit ()
{
  // Wait for Naming Service initialized.
  while (! initialized_) {
    ACE_OS::sleep(ACE_Time_Value(0, 100 * 1000));
  }
}

void NamingTask::end()
{
  orb_->shutdown(0);
  this->wait();
}

int NamingTask::svc()
{
  try {
    // Get reference to Root POA
    CORBA::Object_var obj = orb_->resolve_initial_references("RootPOA");
    PortableServer::POA_var poa = PortableServer::POA::_narrow(obj.in());

    // Activate POA Manager
    PortableServer::POAManager_var poaManager = poa->the_POAManager();
    poaManager->activate();

    // Initialize the naming service
    // We are not going to look for other naming servers
    TAO_Naming_Server naming;
    if (naming.init(orb_.in(),
                    poa.in(),
                    ACE_DEFAULT_MAP_SIZE,
                    0,
                    0) == 0) {
      std::cout << "The Naming Service Task is ready." << std::endl;
      initialized_ = true;
      // Accept requests
      orb_->run();
      orb_->destroy();
      return 0;
    }
    else {
      std::cerr << "Unable to initialize the Naming Service." << std::endl;
    }
  }
  catch(const CORBA::Exception& ex) {
    std::cerr << "NamingTask::svc() CORBA::Exception: " << ex << std::endl;
  }

  return -1;
}

