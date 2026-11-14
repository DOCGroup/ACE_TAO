#include "NamingTask.h"
#include "orbsvcs/Naming/Naming_Server.h"
#include "ace/OS_NS_unistd.h"

#include <iostream>

NamingTask::NamingTask (int argc, ACE_TCHAR *argv[])
  : orb_ (CORBA::ORB_init (argc, argv, "NamingORB"))
{
}

void NamingTask::waitInit ()
{
  // Wait for Naming Service initialized.
  while (!this->initialized_.load (std::memory_order_acquire))
    ACE_OS::sleep (ACE_Time_Value (0, 100 * 1000));
}

void NamingTask::end ()
{
  orb_->shutdown (false);
  this->wait ();
}

int NamingTask::svc ()
{
  try
    {
      // Get reference to Root POA
      CORBA::Object_var const obj = orb_->resolve_initial_references ("RootPOA");
      PortableServer::POA_var const poa = PortableServer::POA::_narrow (obj.in ());

      // Activate POA Manager
      PortableServer::POAManager_var const poa_manager = poa->the_POAManager ();
      poa_manager->activate ();

      // Initialize the naming service without discovering other naming servers.
      TAO_Naming_Server naming;
      if (naming.init (orb_.in (),
                       poa.in (),
                       ACE_DEFAULT_MAP_SIZE,
                       nullptr,
                       false) == 0)
        {
          std::cout << "The Naming Service Task is ready." << std::endl;
          this->initialized_.store (true, std::memory_order_release);
          orb_->run ();
          orb_->destroy ();
          return 0;
        }

      std::cerr << "Unable to initialize the Naming Service." << std::endl;
    }
  catch (CORBA::Exception const &ex)
    {
      std::cerr << "NamingTask::svc() CORBA::Exception: " << ex << std::endl;
    }

  return -1;
}

