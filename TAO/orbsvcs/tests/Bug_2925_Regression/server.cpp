#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "orbsvcs/PortableGroup/GOA.h"
#include "tao/Policy_Manager.h"
#include "Hello_Impl.h"
#include <memory>

constexpr int HELLO_CALL_NUMBER { 100 };

struct Server_Options
{
  ACE_TCHAR const *uipmc_url {};
  ACE_TCHAR const *ior_output_file {};
};

using File_Ptr = std::unique_ptr<FILE, int (*) (FILE *)>;

int
parse_args (int argc, ACE_TCHAR *argv[], Server_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("u:o:"));
  int c {};
  while ((c = get_opts ()) != -1)
    {
      switch (c)
        {
        case 'u':
          options.uipmc_url = get_opts.opt_arg ();
          break;
        case 'o':
          options.ior_output_file = get_opts.opt_arg ();
          break;
        case '?':
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("usage: %s -u <url> -o <iorfile>\n"),
                             argv [0]),
                            -1);
        }
    }

  if (options.uipmc_url == nullptr || options.ior_output_file == nullptr)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("usage: %s -u <url> -o <iorfile>\n"),
                       argv [0]),
                      -1);

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  MessageLog logger (HELLO_CALL_NUMBER);

  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var poa_object =
        orb->resolve_initial_references ("RootPOA");

      PortableGroup::GOA_var root_poa =
        PortableGroup::GOA::_narrow (poa_object.in ());

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT (" (%P|%t) Panic: nil RootPOA\n")),
                          1);

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      // servant
      Hello_Impl *hello_impl {};
      ACE_NEW_RETURN (hello_impl,
                      Hello_Impl (orb.in (), logger),
                      1);
      PortableServer::ServantBase_var owner_transfer (hello_impl);

      Server_Options options;
      if (parse_args (argc, argv, options) != 0)
        return 2;

      // create UIPMC reference
      CORBA::Object_var miop_ref =
        orb->string_to_object (options.uipmc_url);

      // create id
      PortableServer::ObjectId_var id =
        root_poa->create_id_for_reference (miop_ref.in ());

      // activate Hello Object
      root_poa->activate_object_with_id (id.in (),
                                         hello_impl);

      CORBA::String_var ior =
        orb->object_to_string (miop_ref.in ());

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Activated as <%C>\n"), ior.in ()));

      {
        File_Ptr output_file (
          ACE_OS::fopen (options.ior_output_file, ACE_TEXT ("w")), ACE_OS::fclose);

        if (!output_file)
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Cannot open output file for writing IOR: %s"),
                             options.ior_output_file),
                            1);

        if (ACE_OS::fprintf (output_file.get (), "%s", ior.in ()) < 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Cannot write output file for IOR: %s"),
                             options.ior_output_file),
                            1);
      }

      poa_manager->activate ();

      orb->run ();

      root_poa->destroy (true, true);

      orb->destroy ();

      if (logger.report_statistics () <= 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("\n (%P|%t) ERROR: No single call got through to the server\n")),
                          3);
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught in server main ():");
      return 4;
    }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("\n (%P|%t) server finished successfully..\n")));
  return 0;
}
