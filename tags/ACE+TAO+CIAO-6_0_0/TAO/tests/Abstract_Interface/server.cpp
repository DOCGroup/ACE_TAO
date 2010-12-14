// -*- C++ -*-
// $Id$

#include "test_i.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"

const ACE_TCHAR *ior_output_file = ACE_TEXT("test.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
              ior_output_file = get_opts.optarg;
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
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      // Create and register factory for BaseNode.
      BaseNode_init *bn_factory = 0;
      ACE_NEW_RETURN (bn_factory,
                      BaseNode_init,
                      1);

      orb->register_value_factory (bn_factory->tao_repository_id (),
                                   bn_factory);
      bn_factory->_remove_ref (); // release ownership

      // Create and register factory for TreeController.
      TreeController_init *tc_factory = 0;
      ACE_NEW_RETURN (tc_factory,
                      TreeController_init,
                      1);

      orb->register_value_factory (tc_factory->tao_repository_id (),
                                   tc_factory);
      tc_factory->_remove_ref (); // release ownership

      // Create and register factory for StringNode.
      StringNode_init *sn_factory = 0;
      ACE_NEW_RETURN (sn_factory,
                      StringNode_init,
                      1);

      orb->register_value_factory (sn_factory->tao_repository_id (),
                                   sn_factory);
      sn_factory->_remove_ref (); // release ownership

      CORBA::Object_var poa_object =
        orb->resolve_initial_references ("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             " (%P|%t) Unable to initialize the POA.\n"),
                            1);
        }

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      if (parse_args (argc, argv) != 0)
        {
          return 1;
        }

      passer_i servant (orb.in (), root_poa.in ());
      PortableServer::ObjectId_var id =
        root_poa->activate_object (&servant);

      CORBA::Object_var object = root_poa->id_to_reference (id.in ());

      passer_var server =
        passer::_narrow (object.in ());

      CORBA::String_var ior =
              orb->object_to_string (server.in ());

      ACE_DEBUG ((LM_DEBUG,
                  "Activated as <%C>\n",
                  ior.in ()));

      // If the ior_output_file exists, output the ior to it
      if (ior_output_file != 0)
        {
          FILE *output_file = ACE_OS::fopen (ior_output_file, "w");

          if (output_file == 0)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "Can't open output file for writing IOR: %s",
                                 ior_output_file),
                                1);
            }

          ACE_OS::fprintf (output_file,
                           "%s",
                           ior.in ());
          ACE_OS::fclose (output_file);
        }

      poa_manager->activate ();

      orb->run ();

      // Destroy the POA, waiting until the destruction terminates
      root_poa->destroy (1, 1);

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Server: exception caught - ");
      return 1;
    }

  return 0;
}
