#include "Manager_i.h"

#include <ace/Get_Opt.h>
#include <iostream>
#include <fstream>

struct Manager_Options
{
  long retry_count {};
  ACE_TCHAR const *ior_arg {ACE_TEXT ("manager.ior")};
};

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      Manager_Options options;
      CORBA::ORB_var orb (CORBA::ORB_init (argc, argv));

      ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("r:o:"));
      int c {};
      while ((c = get_opts ()) != -1)
        {
          switch (c)
            {
            case 'r':
              options.retry_count = ACE_OS::atoi (get_opts.opt_arg ());
              break;
            case 'o':
              options.ior_arg = get_opts.opt_arg ();
              break;
            case '?':
            default:
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("Usage:  %s [-r retryCount] [-o iorfile]\n"),
                          argv[0]));
              break;
            }
        }

      CORBA::Object_var obj = orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var poa = PortableServer::POA::_narrow (obj.in ());

      PortableServer::POAManager_var mgr = poa->the_POAManager ();

      Manager_i servant (options.retry_count);
      PortableServer::ObjectId_var oid = poa->activate_object (&servant);
      obj = poa->id_to_reference (oid.in ());
      {
        CORBA::String_var ior = orb->object_to_string (obj.in ());
        std::ofstream ior_file (ACE_TEXT_ALWAYS_CHAR (options.ior_arg));
        ior_file << ior.in () << std::endl;
      }

      std::cout << "IMR Test Manager activated (servers retry = " << options.retry_count
                << ")." << std::endl;

      mgr->activate ();

      orb->run ();

      poa->destroy (1, 1);
      orb->destroy ();
      return 0;
    }
  catch (CORBA::Exception const &ex)
    {
      std::cerr << "Manager: " << ex << std::endl;
    }
  return 1;
}
