// $Id$

#include "McastHello.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "orbsvcs/PortableGroup/MIOP.h"
#include "orbsvcs/PortableGroup/GOA.h"
#include "tao/ZIOP/ZIOP.h"
#include "tao/Compression/zlib/ZlibCompressor_Factory.h"

static const ACE_TCHAR *ior_output_file = ACE_TEXT("test.ior");

// Use a multicast address in the administrative "site local" range, 239.255.0.0 to
// 239.255.255.255.  The range 224.255.0.0 to 238.255.255.255 should also be valid
// too.
//static const char *group_ior = "corbaloc:miop:1.0@1.0-TestDomain-1/239.255.0.1:16000";
static const ACE_TCHAR *group_ior = ACE_TEXT("corbaloc:miop:1.0@1.0-TestDomain-1/224.1.239.2:1234");

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

      case 'g':
        group_ior = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-o <iorfile>"
                           "-g <group ior corbaloc>"
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
  int exit_status = 0;
  try
    {
      // Init orb
      CORBA::ORB_var orb (CORBA::ORB_init (argc, argv));

      // Obtain root POA and GOA
      CORBA::Object_var poa_object (
        orb->resolve_initial_references("RootPOA"));

      PortableGroup::GOA_var root_poa (
        PortableGroup::GOA::_narrow (poa_object.in ()));

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil RootPOA\n"),
                          1);

      // Obtain the root POA's manager
      PortableServer::POAManager_var poa_manager (
        root_poa->the_POAManager ());

      // Obtain the orb's compression manager
      CORBA::Object_var compression_manager (
        orb->resolve_initial_references("CompressionManager"));

      ::Compression::CompressionManager_var comp_manager (
        ::Compression::CompressionManager::_narrow (compression_manager.in ()));

      if (CORBA::is_nil(comp_manager.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil compression manager\n"),
                           1);

      //register Zlib compressor
      ::Compression::CompressorFactory_ptr compressor_factory;
      ACE_NEW_RETURN (compressor_factory, TAO::Zlib_CompressorFactory (), 1);
      ::Compression::CompressorFactory_var compr_fact (compressor_factory);
      comp_manager->register_factory (compr_fact.in ());

      // Parse our own specific arguments
      if (parse_args (argc, argv) != 0)
        return 1;

      // Create the servant on the group ior
      CORBA::Object_var group1 =
        orb->string_to_object (group_ior);

      McastHello server_impl (orb.in (), 0);

      PortableServer::ObjectId_var id =
        root_poa->create_id_for_reference (group1.in ());

      root_poa->activate_object_with_id (id.in (),
                                         &server_impl);

      poa_manager->activate ();

      // Output the servant's IOR to the <ior_output_file>
      FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
      if (output_file == 0)
        {
          ACE_DEBUG ((LM_ERROR,
                      "Cannot open output file for writing IOR: %s",
                      ior_output_file));
          exit_status = 1;
        }
      else
        {
          ACE_OS::fprintf (output_file, "%s", ACE_TEXT_ALWAYS_CHAR (group_ior));
          ACE_OS::fclose (output_file);

          // Service the requests until shutdown.
          ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - event loop started\n"));
          try
            {
              orb->run ();
            }
          catch (const CORBA::Exception& ex)
            {
              ex._tao_print_exception ("(%P|%t) server - Orb->run Exception caught:");
              exit_status = 1;
            }
          catch (...)
            {
              ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - unknown exception\n"));
              exit_status = 1;
            }

          ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - event loop finished\n"));
        }

      // Shutdown the orb
      root_poa->destroy (1, 1);
      orb->destroy ();

      // Validate that our servants got the right requests.
      if (server_impl.get_status () == 0)
        {
          ACE_DEBUG ((LM_ERROR,
                      "Multicast Servant did not receive expected requests!\n"));
          exit_status = 1;
        }
      else
        ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - Success!\n"));
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("(%P|%t) server - Exception caught:");
      exit_status = 1;
    }
  catch (...)
    {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - unknown exception\n"));
      exit_status = 1;
    }

  return exit_status;
}
