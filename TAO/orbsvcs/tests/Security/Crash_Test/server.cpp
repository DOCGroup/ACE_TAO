#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_unistd.h"
#include "ace/SString.h"
#include "test_i.h"
#include <memory>
#include <string>

struct Server_Options
{
  ACE_TString ior_output_file {};
};

int
parse_args (int argc, ACE_TCHAR *argv[], Server_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("o:"));
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
                           ACE_TEXT ("SERVER (%P): usage:  %s -o <iorfile>\n"),
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
      std::string env ("SSL_CERT_FILE=cacert.pem");
      ACE_OS::putenv (env.c_str ());

      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);
      Server_Options options;

      CORBA::Object_var poa_object = orb->resolve_initial_references ("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("SERVER (%P): Unable to initialize the POA.\n")),
                          1);

      PortableServer::POA_var root_poa = PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var poa_manager = root_poa->the_POAManager ();

      if (parse_args (argc, argv, options) != 0)
        return 1;

      Simple_Server_i server_impl (orb.in ());

      CORBA::PolicyList policies (2);
      policies.length (2);

      policies[0] =
        root_poa->create_id_assignment_policy (PortableServer::USER_ID);

      policies[1] =
        root_poa->create_lifespan_policy (PortableServer::PERSISTENT);

      PortableServer::POA_var persistent_poa =
        root_poa->create_POA ("PersistentPOA",
                              poa_manager.in (),
                              policies);

      PortableServer::ObjectId_var oid =
        PortableServer::string_to_ObjectId ("object1");

      persistent_poa->activate_object_with_id (oid.in (),
                                               &server_impl);

      CORBA::Object_var server =
        persistent_poa->create_reference_with_id (oid.in (),
                                                  "IDL:Simple_Server:1.0");

      CORBA::String_var ior =
        orb->object_to_string (server.in ());

      // If the ior_output_file exists, output the ior to it
      if (!options.ior_output_file.empty ())
        {
          std::unique_ptr<FILE, int (*) (FILE *)> output_file (
            ACE_OS::fopen (options.ior_output_file.c_str (), ACE_TEXT ("w")), ACE_OS::fclose);
          if (!output_file)
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("SERVER (%P): Cannot open output file "
                                         "for writing IOR: %s"),
                               options.ior_output_file.c_str ()),
                              1);
          ACE_OS::fprintf (output_file.get (), "%s", ior.in ());

          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("SERVER (%P): Activated as file://%s\n"),
                      options.ior_output_file.c_str ()));
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("SERVER (%P): Activated as <%s>\n"),
                      ior.in ()));
        }

      poa_manager->activate ();

      orb->run ();

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("SERVER (%P): Event loop finished.\n")));

      persistent_poa->destroy (true, true);

      root_poa->destroy (true, true);

      ACE_OS::sleep (1);

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("SERVER (%P): Caught exception:");
      return 1;
    }

  return 0;
}
