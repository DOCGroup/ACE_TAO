// $Id$

#include "Test_impl.h"
#include "ace/OS_NS_stdio.h"
#include "ace/Get_Opt.h"

const ACE_TCHAR *ior_output_file = ACE_TEXT ("server.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-o <iorfile>"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
        CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

        if (parse_args (argc, argv) != 0)
          return 1;

        CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA");
        PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());
        PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();
        poa_manager->activate ();

        Server_impl* server_obj = new Server_impl(orb.in());
        PortableServer::ServantBase_var owner (server_obj);
        server_var server = server_obj->_this();
        CORBA::String_var ior = orb->object_to_string (server.in ());

        FILE *output_file = ACE_OS::fopen (ACE_TEXT_ALWAYS_CHAR(ior_output_file), "w");
        ACE_OS::fprintf (output_file, "%s", ior.in ());
        ACE_OS::fclose (output_file);

        orb->run ();
        orb->destroy ();
    }
  catch(...)
    {
        return 1;
    }

    return 0;
}
