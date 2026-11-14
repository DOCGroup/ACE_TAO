#include "NamingTask.h"
#include "tao/Version.h"
#include "orbsvcs/Naming/Naming_Server.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Argv_Type_Converter.h"

NamingTask::NamingTask (char const *orbname, int argc, ACE_TCHAR **argv)
{
  ACE_Argv_Type_Converter satc (argc, argv);
  orb_ = CORBA::ORB_init (satc.get_argc (), satc.get_TCHAR_argv (), orbname);
}

void
NamingTask::waitInit ()
{
  // Wait for Naming Service initialized.
  while (!this->initialized_.load (std::memory_order_acquire))
    ACE_OS::sleep (ACE_Time_Value (0, 100 * 1000));
}

void
NamingTask::end ()
{
  this->orb_->shutdown (false);
  this->wait ();
}

CosNaming::NamingContext_ptr
NamingTask::root () const
{
  return CosNaming::NamingContext::_duplicate (root_);
}

int
NamingTask::svc ()
{
  try
    {
      CORBA::Object_var const obj = this->orb_->resolve_initial_references ("RootPOA");
      PortableServer::POA_var const poa = PortableServer::POA::_narrow (obj.in ());

      PortableServer::POAManager_var const poa_manager = poa->the_POAManager ();
      poa_manager->activate ();

      // Initialize the naming service without searching for other naming servers.
      TAO_Naming_Server naming;
      if (naming.init (this->orb_.in (),
                       poa.in (),
                       ACE_DEFAULT_MAP_SIZE,
                       0,
                       0,
                       0,
                       TAO_NAMING_BASE_ADDR,
                       0) == 0)
        {
          ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("The Naming Service Task is ready.\n")));
          this->root_ = CosNaming::NamingContext::_duplicate (naming.operator-> ());
          this->initialized_.store (true, std::memory_order_release);
          this->orb_->run ();
          this->orb_->destroy ();
          return 0;
        }
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Error, unable to initialize the Naming Service.\n")));
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("CORBA exception: NamingTask::svc()");
    }

  return -1;
}

