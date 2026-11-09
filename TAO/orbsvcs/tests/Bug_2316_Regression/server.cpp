#include "Test_impl.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include <cstdio>
#include <memory>

struct Server_Options
{
  ACE_TString ior_output_file { ACE_TEXT ("server.ior") };
};

using File_Ptr = std::unique_ptr<FILE, int (*) (FILE *)>;

int
parse_args (int argc, ACE_TCHAR *argv[], Server_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("o:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        options.ior_output_file = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage: %s -o <iorfile>\n"),
                           argv [0]),
                          -1);
      }
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

      Server_Options options;
      if (parse_args (argc, argv, options) != 0)
        return 1;

      CORBA::Object_var const poa_object =
        orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (poa_object.in ());
      PortableServer::POAManager_var const poa_manager =
        root_poa->the_POAManager ();
      poa_manager->activate ();

      Server_impl *server_obj {};
      ACE_NEW_RETURN (server_obj, Server_impl (orb.in ()), 1);
      // The ServantBase_var adopts the servant's initial reference.
      PortableServer::ServantBase_var owner (server_obj);
      server_var const server = server_obj->_this ();
      CORBA::String_var const ior = orb->object_to_string (server.in ());

      {
        File_Ptr output_file (ACE_OS::fopen (options.ior_output_file.c_str (), ACE_TEXT ("w")),
                              ACE_OS::fclose);
        if (!output_file)
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Cannot open output file for writing IOR: %s"),
                             options.ior_output_file.c_str ()),
                            1);
        if (std::fprintf (output_file.get (), "%s", ior.in ()) < 0)
          ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Cannot write IOR to output file\n")), 1);
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
