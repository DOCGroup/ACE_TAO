// $Id$

#include "..\Supports_Test_impl.h"
#include "tao/Strategies/advanced_resource.h"

const char * ior_output_file = "test.ior";

int
main (int argc, char * argv[])
{
	ACE_TRY_NEW_ENV
	{

		CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);
		ACE_TRY_CHECK;

		CORBA::Object_var poa_object = orb->resolve_initial_references ("RootPOA" ACE_ENV_ARG_PARAMETER);
		ACE_TRY_CHECK;

		PortableServer::POA_var root_poa = PortableServer::POA::_narrow (poa_object.in () ACE_ENV_ARG_PARAMETER);
		ACE_TRY_CHECK;

		if (CORBA::is_nil (root_poa.in ())) ACE_ERROR_RETURN ((LM_ERROR," (%P|%t) Nil RootPOA\n"), 1);

		PortableServer::POAManager_var poa_manager = root_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
		ACE_TRY_CHECK;

 
		/* Create, register factories */

		Supports_Test::Node_init * node_factory = 0;
  
		ACE_NEW_RETURN (node_factory, node_init_impl, 1);

		CORBA::ValueFactory returned_factory = orb->register_value_factory (node_factory->tao_repository_id (), node_factory ACE_ENV_ARG_PARAMETER);
		ACE_TRY_CHECK;
	
		ACE_ASSERT (returned_factory == 0);

	  node_factory->_remove_ref ();


		Supports_Test::vt_graph_init * vt_graph_factory = 0;

		ACE_NEW_RETURN (vt_graph_factory, vt_graph_init_impl, 1);

		returned_factory = orb->register_value_factory (vt_graph_factory->tao_repository_id (), vt_graph_factory ACE_ENV_ARG_PARAMETER);
		ACE_TRY_CHECK;
		
		ACE_ASSERT (returned_factory == 0);

	  vt_graph_factory->_remove_ref ();


	  test_impl * a_test_impl;
  
		ACE_NEW_RETURN (a_test_impl, test_impl (orb.in ()), 1);

	  PortableServer::ServantBase_var owner_transfer(a_test_impl);

		Supports_Test::test_var a_test = a_test_impl->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
		ACE_TRY_CHECK;

	  CORBA::String_var ior = orb->object_to_string (a_test.in () ACE_ENV_ARG_PARAMETER);
		ACE_TRY_CHECK;

	  FILE * output_file = ACE_OS::fopen (ior_output_file, "w");

	  if (output_file == 0) ACE_ERROR_RETURN ((LM_ERROR, "Cannot open output file for writing IOR: %s", ior_output_file), 1);

	  ACE_OS::fprintf (output_file, "%s", ior.in ());

	  ACE_OS::fclose (output_file);

	  poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
		ACE_TRY_CHECK;

	  orb->run (ACE_ENV_SINGLE_ARG_PARAMETER);
		ACE_TRY_CHECK;

	  root_poa->destroy (1, 1 ACE_ENV_ARG_PARAMETER);
		ACE_TRY_CHECK;
  
	  orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
		ACE_TRY_CHECK;

    ACE_DEBUG ((LM_DEBUG, "(%P|%t) Server test finished\n"));

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
