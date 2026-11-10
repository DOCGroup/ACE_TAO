#include "NamingTask.h"
#include "orbsvcs/Naming/Naming_Server.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_stdio.h"
#include <cstddef>
#include <vector>

NamingTask::NamingTask (ACE_TCHAR const *orbname, int argc, ACE_TCHAR **argv, unsigned short port)
{
  ACE_TCHAR endpoint[128] {};
  ACE_OS::snprintf (endpoint, sizeof endpoint / sizeof endpoint[0],
                    ACE_TEXT ("iiop://localhost:%u"), static_cast<unsigned int> (port));
  ACE_TCHAR endpoint_option[] = ACE_TEXT ("-ORBEndpoint");
  std::vector<ACE_TCHAR *> my_argv;
  my_argv.reserve (static_cast<std::size_t> (argc) + 2);
  for (int i = 0; i < argc; ++i)
    my_argv.push_back (argv[i]);
  my_argv.push_back (endpoint_option);
  my_argv.push_back (endpoint);

  int my_argc = static_cast<int> (my_argv.size ());
  orb_ = CORBA::ORB_init (my_argc, my_argv.data (), orbname);
}

void
NamingTask::waitInit ()
{
  while (!this->initialized_.load ())
    ACE_OS::sleep (ACE_Time_Value (0, 100 * 1000));
}

void
NamingTask::end ()
{
  orb_->shutdown (false);
  this->wait ();
}

char const *NamingTask::ior () const
{
  return ior_.in ();
}

int
NamingTask::svc ()
{
  try
    {
      CORBA::Object_var obj = orb_->resolve_initial_references ("RootPOA");
      PortableServer::POA_var const poa = PortableServer::POA::_narrow (obj.in ());
      PortableServer::POAManager_var const poa_manager = poa->the_POAManager ();
      poa_manager->activate ();

      TAO_Naming_Server naming;
      if (naming.init (orb_.in (),
                       poa.in (),
                       ACE_DEFAULT_MAP_SIZE,
                       nullptr,
                       false,
                       nullptr,
                       TAO_NAMING_BASE_ADDR,
                       false) == 0)
        {
          ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("The Naming Service Task is ready.\n")));
          ior_ = naming.naming_service_ior ();
          this->initialized_.store (true);
          orb_->run ();
          return 0;
        }

      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Unable to initialize the Naming Service.\n")));
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("NamingTask::svc() CORBA::Exception: ");
    }

  return -1;
}

