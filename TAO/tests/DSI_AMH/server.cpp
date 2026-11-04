#include "Roundtrip.h"
#include "ace/Get_Opt.h"
#include "ace/SString.h"
#include <memory>

struct Server_Options
{
  ACE_TString ior_output_file {ACE_TEXT ("test.ior")};
};

int
parse_args (int argc, ACE_TCHAR *argv[], Server_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:"));
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
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  Server_Options options;
  try
    {
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var const poa_object =
        orb->resolve_initial_references ("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var const poa_manager =
        root_poa->the_POAManager ();

      if (parse_args (argc, argv, options) != 0)
        return 1;

      // The POA activation API uses this borrowed pointer; owner_transfer owns the servant.
      Roundtrip *roundtrip_impl {};
      ACE_NEW_RETURN (roundtrip_impl,
                      Roundtrip (orb.in ()),
                      1);
      PortableServer::ServantBase_var const owner_transfer (roundtrip_impl);

      PortableServer::ObjectId_var const oid =
        root_poa->activate_object (roundtrip_impl);

      CORBA::Object_var const roundtrip =
        root_poa->id_to_reference (oid.in ());

      CORBA::String_var const ior =
        orb->object_to_string (roundtrip.in ());

      // If the ior_output_file exists, output the ior to it
      {
        std::unique_ptr<FILE, int (*) (FILE *)> output_file (
          ACE_OS::fopen (options.ior_output_file.c_str (), ACE_TEXT ("w")), ACE_OS::fclose);
        if (!output_file)
          ACE_ERROR_RETURN ((
              LM_ERROR,
              ACE_TEXT ("Cannot open output file for writing IOR: %s\n"),
              options.ior_output_file.c_str ()),
              1);
        ACE_OS::fprintf (output_file.get (), "%s", ior.in ());
      }

      poa_manager->activate ();

      orb->run ();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - event loop finished\n"));

      root_poa->destroy (true, true);

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
