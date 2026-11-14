#include "NamingTask.h"
#include "orbsvcs/Naming/Naming_Server.h"
#include "ace/OS_NS_unistd.h"

#include <array>
#include <vector>

NamingTask::NamingTask (char const *orbname, int argc, ACE_TCHAR **argv, unsigned short port)
{
  std::vector<ACE_TCHAR *> my_argv (argc + 2);
  std::array<ACE_TCHAR, 128> buf {};
  int my_argc {};
  for (; my_argc < argc; ++my_argc)
    my_argv[my_argc] = argv[my_argc];
  ACE_OS::snprintf (buf.data (),
                    buf.size (),
                    ACE_TEXT ("iiop://localhost:%hu"),
                    port);
  ACE_TCHAR endpoint_option[] = ACE_TEXT ("-ORBEndpoint");
  my_argv[my_argc++] = endpoint_option;
  my_argv[my_argc++] = buf.data ();

  this->orb_ = CORBA::ORB_init (my_argc, my_argv.data (), orbname);
}

void
NamingTask::waitInit ()
{
  // Wait for Naming Service initialized.
  while (!this->initialized_.load ())
    ACE_OS::sleep (ACE_Time_Value (0, 100 * 1000));
}

void
NamingTask::end ()
{
  this->orb_->shutdown (false);
  this->wait ();
}

char const *
NamingTask::ior () const
{
  return this->ior_.in ();
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

      // We are not going to look for other naming servers.
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
          ACE_DEBUG ((LM_DEBUG, "The Naming Service Task is ready.\n"));
          this->ior_ = naming.naming_service_ior ();
          this->initialized_.store (true);
          this->orb_->run ();
          return 0;
        }

      ACE_ERROR ((LM_ERROR, "Unable to initialize the Naming Service.\n"));
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("NamingTask::svc() CORBA::Exception: ");
    }

  return -1;
}

