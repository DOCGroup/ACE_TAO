// $Id$

#include "TreeBaseC.h"
#include "TreeControllerC.h"
#include "TreeNodeC.h"
#include "Test_impl.h"

#include "ace/Get_Opt.h"

ACE_RCSID(Forward, server, "$Id$")

const char *ior_output_file = "test.ior";

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "o:");
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
                           "usage:  %s "
                           "-o <iorfile>"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "", ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // All factories are kindly provided by
      // compiler so we just to put everything in a right order.

      // Create and register factory for BaseNode.
      BaseNode_init *bn_factory = 0; 
      ACE_NEW_RETURN (bn_factory, 
                      BaseNode_init,
                      1);

      orb->register_value_factory (bn_factory->tao_repository_id (),
                                   bn_factory, 
                                   ACE_TRY_ENV);
      ACE_TRY_CHECK;
      bn_factory->_remove_ref (); // release ownership

      // Create and register factory for TreeController.
      TreeController_init *tc_factory = 0; 
      ACE_NEW_RETURN (tc_factory, 
                      TreeController_init,
                      1);

      orb->register_value_factory (tc_factory->tao_repository_id (),
                                   tc_factory, 
                                   ACE_TRY_ENV);
      ACE_TRY_CHECK;
      tc_factory->_remove_ref (); // release ownership

      // Create and register factory for StringNode.
      StringNode_init *sn_factory = 0; 
      ACE_NEW_RETURN (sn_factory, 
                      StringNode_init,
                      1);

      orb->register_value_factory (sn_factory->tao_repository_id (),
                                   sn_factory, 
                                   ACE_TRY_ENV);
      ACE_TRY_CHECK;
      sn_factory->_remove_ref (); // release ownership

      //Well, done with factories.

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA", ACE_TRY_ENV);
      ACE_TRY_CHECK;

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil RootPOA\n"),
                          1);

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return 1;

      Test_impl *test_impl;
      ACE_NEW_RETURN (test_impl,
                      Test_impl (orb.in ()),
                      1);

      PortableServer::ServantBase_var owner_transfer(test_impl);

      Test_var test = test_impl->_this (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CORBA::String_var ior =
        orb->object_to_string (test.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // If the ior_output_file exists, output the ior to it
      FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file for writing IOR: %s",
                           ior_output_file),
                              1);
      ACE_OS::fprintf (output_file, "%s", ior.in ());
      ACE_OS::fclose (output_file);

      poa_manager->activate (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      orb->run (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - event loop finished\n"));

      root_poa->destroy (1, 1, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      orb->destroy (ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception caught:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
