// $Id$

#include "NamingTask.h"
#include "orbsvcs/Naming/Naming_Server.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Argv_Type_Converter.h"

NamingTask::NamingTask (const char* orbname, int argc, char** argv, unsigned short port)
 : initialized_(false)
{
  char** my_argv = new char*[argc + 2];
  char buf[128];
  int my_argc = 0;
  for(; my_argc<argc ;++my_argc)
    my_argv[my_argc] = argv[my_argc];
  ACE_OS::sprintf (buf, "iiop://localhost:%d", port);
  const char* epopt = "-ORBEndpoint";
  my_argv[my_argc++] = const_cast<char*> (epopt);
  my_argv[my_argc++] = buf;

  ACE_Argv_Type_Converter satc (my_argc, my_argv);
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
  this->wait();
}

const char* NamingTask::ior()
{
  return ior_.in ();
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
      ior_ = CORBA::string_dup (naming.naming_service_ior ());
      initialized_ = true;
      // Accept requests
      orb_->run();
      return 0;
    }
    else
    {
      ACE_ERROR ((LM_ERROR, "Unable to initialize the Naming Service.\n"));
    }
  }
  catch (const CORBA::Exception& ex)
  {
      ex._tao_print_exception ("NamingTask::svc() CORBA::Exception: ");
  }

  return -1;
}

