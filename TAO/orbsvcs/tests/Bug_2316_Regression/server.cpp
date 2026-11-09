#include "Test_impl.h"
#include "ace/OS_NS_stdio.h"
#include "ace/Get_Opt.h"
#include <memory>

struct Server_Options
{
  ACE_TString ior_output_file { ACE_TEXT ("server.ior") };
};

using File_Ptr = std::unique_ptr<FILE, int (*) (FILE *)>;

int
parse_args (int argc, ACE_TCHAR *argv[], Server_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        options.ior_output_file = get_opts.opt_arg ();
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
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      Server_Options options;
      if (parse_args (argc, argv, options) != 0)
        return 1;

      CORBA::Object_var poa_object =
        orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());
      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();
      poa_manager->activate ();

      Server_impl *server_obj {};
      ACE_NEW_RETURN (server_obj, Server_impl (orb.in ()), 1);
      PortableServer::ServantBase_var owner (server_obj);
      server_var server = server_obj->_this ();
      CORBA::String_var ior = orb->object_to_string (server.in ());

      {
        File_Ptr output_file (ACE_OS::fopen (options.ior_output_file.c_str (), "w"),
                              ACE_OS::fclose);
        if (!output_file)
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Cannot open output file for writing IOR: %s"),
                             options.ior_output_file.c_str ()),
                            1);
        ACE_OS::fprintf (output_file.get (), "%s", ior.in ());
      }

      orb->run ();
      orb->destroy ();
    }
  catch (...)
    {
      return 1;
    }

  return 0;
}
