// -*- C++ -*-
// $Id$

#include "Backend_Impl.h"

#include "tao/Strategies/advanced_resource.h"

#include "tao/Utils/Servant_Var.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"

const ACE_TCHAR *ior_output_file = ACE_TEXT ("backend.ior");
bool verbose = false;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("vo:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;

      case 'v':
        verbose = true;
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT("usage:  %s ")
                           ACE_TEXT("-o <iorfile>")
                           ACE_TEXT("\n"),
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
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      if (CORBA::is_nil (root_poa.in ()))
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                "backend_server(%P|%t) - panic: nil RootPOA\n"),
            1);
      }

      PortableServer::POAManager_var poa_manager = root_poa->the_POAManager ();

      if (parse_args (argc, argv) != 0)
        return 1;

      using namespace Bug_3647_Regression;
      TAO::Utils::Servant_Var<Backend_Impl> impl(
          new Backend_Impl(orb.in(), verbose));

      PortableServer::ObjectId_var id =
          root_poa->activate_object (impl.in());

      CORBA::Object_var object = root_poa->id_to_reference (id.in ());

      Bug_3647_Regression::Backend_var backend =
          Bug_3647_Regression::Backend::_narrow (object.in ());

      CORBA::String_var ior = orb->object_to_string (backend.in ());

      // Output the IOR to the <ior_output_file>
      FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
      if (output_file == 0)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                "backend_server(%P|%) - cannot open output file "
                "for writing IOR: %s\n",
                ior_output_file),
            1);
      }
      ACE_OS::fprintf (output_file, "%s", ior.in ());
      ACE_OS::fclose (output_file);

      poa_manager->activate ();

      orb->run ();

      ACE_DEBUG ((LM_DEBUG, "backend_server(%P|%t) - event loop finished\n"));

      root_poa->destroy (1, 1);

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ACE_DEBUG ((LM_DEBUG,
              "backend_server"));
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
