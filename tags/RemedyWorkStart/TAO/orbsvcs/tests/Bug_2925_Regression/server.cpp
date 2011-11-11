//
// $Id$
//

#include "ace/Get_Opt.h"
#include "orbsvcs/PortableGroup/GOA.h"
#include "tao/Policy_Manager.h"
#include "Hello_Impl.h"

#define HELLO_CALL_NUMBER       100

const ACE_TCHAR *uipmc_url = 0;
const ACE_TCHAR *ior_output_file = 0;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("u:o:"));
  const unsigned char full_success = 0x03;
  unsigned char success = 0;

  do
    {
      int c = get_opts ();
      if (success == full_success && c == -1)
        break;

      if (c == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " -u <url>"
                           " -o <iorfile>"
                           "\n",
                           argv [0]),
                           -1);

      switch (c)
        {
        case 'u':
          uipmc_url = get_opts.opt_arg ();
          success |= 0x01;
          break;
        case 'o':
          ior_output_file = get_opts.opt_arg ();
          success |= 0x02;
          break;
        }
    }
  while (true);

  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  MessageLog logger(HELLO_CALL_NUMBER);

  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA");

      PortableGroup::GOA_var root_poa =
        PortableGroup::GOA::_narrow (poa_object.in ());

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil RootPOA\n"),
                           1);

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      // servant
      Hello_Impl* hello_impl;
      ACE_NEW_RETURN (hello_impl,
                      Hello_Impl (orb.in (), &logger),
                      1);
      PortableServer::ServantBase_var owner_transfer (hello_impl);

      if (parse_args (argc, argv) != 0)
        return 2;

      // create UIPMC reference
      CORBA::String_var multicast_url =
        CORBA::string_dup (ACE_TEXT_ALWAYS_CHAR(uipmc_url));
      CORBA::Object_var miop_ref =
        orb->string_to_object (multicast_url.in ());

      // create id
      PortableServer::ObjectId_var id =
        root_poa->create_id_for_reference (miop_ref.in ());

      // activate Hello Object
      root_poa->activate_object_with_id (id.in (),
                                         hello_impl);

      CORBA::String_var ior =
        orb->object_to_string (miop_ref.in ());

      ACE_DEBUG ((LM_DEBUG, "Activated as <%C>\n", ior.in ()));

      // If the ior_output_file exists, output the ior to it
      if (ior_output_file != 0)
        {
          FILE *output_file= ACE_OS::fopen (ior_output_file, "w");

          if (output_file == 0)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "Cannot open output file for writing IOR: %s",
                                 ior_output_file),
                                 1);
            }

          ACE_OS::fprintf (output_file, "%s", ior.in ());
          ACE_OS::fclose (output_file);
        }

      poa_manager->activate ();

      orb->run ();

      root_poa->destroy (1, 1);

      orb->destroy ();

      if (logger.report_statistics () == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "\n (%P|%t) ERROR: No single call got through to the server\n"),
                           3);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught in server main ():");
      return 4;
    }

  ACE_DEBUG ((LM_DEBUG,
              "\n (%P|%t) server finished successfully..\n"));
  return 0;
}
