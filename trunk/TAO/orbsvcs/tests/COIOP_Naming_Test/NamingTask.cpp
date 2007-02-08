
#include "NamingTask.h"
#include <tao/Version.h>
#include <orbsvcs/Naming/Naming_Server.h>
#include <ace/OS_NS_unistd.h>
#include "ace/Argv_Type_Converter.h"
#include <iostream>

NamingTask::NamingTask (const char* orbname, int argc, char** argv)
 : initialized_(false)
{
  ACE_Argv_Type_Converter satc (argc, argv);
  orb_ = CORBA::ORB_init(satc.get_argc (), satc.get_TCHAR_argv (), orbname);
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
  wait();
}

CosNaming::NamingContext_ptr
NamingTask::root (void) const
{
  return CosNaming::NamingContext::_duplicate (root_);
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
                    0,
                    0,
                    TAO_NAMING_BASE_ADDR,
                    0) == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "The Naming Service Task is ready.\n"));
      root_ = CosNaming::NamingContext::_duplicate (naming.operator->());
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
  catch (CORBA::Exception& ex) {
    std::cerr << "NamingTask::svc() CORBA::Exception: " << ex << std::endl;
  }

  return -1;
}

