// $Id$

#include "..\Supports_Test_impl.h"

const char * ior = "file://test.ior";

int
main (int argc, char *argv[])
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

    poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
		ACE_TRY_CHECK;

		/* Create, register factories */

		Supports_Test::Node_init * node_factory = 0;
  
		ACE_NEW_RETURN (node_factory, node_init_impl, 1);

		CORBA::ValueFactory returned_factory =
      orb->register_value_factory (node_factory->tao_repository_id (),
                                   node_factory
                                   ACE_ENV_ARG_PARAMETER);
		ACE_TRY_CHECK;
	
		ACE_ASSERT (returned_factory == 0);

		node_factory->_remove_ref ();


		Supports_Test::vt_graph_init * vt_graph_factory = 0;

		ACE_NEW_RETURN (vt_graph_factory, vt_graph_init_impl, 1);

		returned_factory = orb->register_value_factory (vt_graph_factory->tao_repository_id (), vt_graph_factory ACE_ENV_ARG_PARAMETER);
		ACE_TRY_CHECK;
	
		ACE_ASSERT (returned_factory == 0);

	  vt_graph_factory->_remove_ref ();


		/* Check return values for register_value_factory */

		Supports_Test::Node_init * node_factory2 = 0;
  
		ACE_NEW_RETURN (node_factory2, node_init_impl, 1);

		returned_factory = orb->register_value_factory (node_factory2->tao_repository_id (), node_factory2 ACE_ENV_ARG_PARAMETER);
		ACE_TRY_CHECK;
	
		ACE_ASSERT (returned_factory == node_factory);

	  node_factory2->_remove_ref ();

	
		/* Get test object reference */
	
		CORBA::Object_var tmp = orb->string_to_object (ior ACE_ENV_ARG_PARAMETER);
		ACE_TRY_CHECK;

	  Supports_Test::test_var my_test = Supports_Test::test::_narrow (tmp.in () ACE_ENV_ARG_PARAMETER);
		ACE_TRY_CHECK;

	  if (CORBA::is_nil (my_test.in ())) ACE_ERROR_RETURN ((LM_DEBUG, "Nil Supports_Test::test obj ref <%s>\n", ior), 1);

	
		/* Perform test */

		vt_graph_impl * the_vt_graph = 0;
		ACE_NEW_RETURN (the_vt_graph, vt_graph_impl (3), 1);
		Supports_Test::vt_graph_var test_vt_graph = the_vt_graph;

    vt_graph_impl * the_vt_graph2 = 0;
		ACE_NEW_RETURN (the_vt_graph2, vt_graph_impl (4), 1);
    Supports_Test::graph_var test_graph = the_vt_graph2->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
		ACE_TRY_CHECK;
		if (CORBA::is_nil (test_graph.in ())) ACE_ERROR_RETURN ((LM_DEBUG, "Nil Supports_Test::graph obj ref\n"), 1);

		ACE_ASSERT (test_vt_graph->size () == 3);
		my_test->pass_vt_graph_in (test_vt_graph.in () ACE_ENV_ARG_PARAMETER);
		ACE_TRY_CHECK;
		ACE_ASSERT (test_vt_graph->size () == 3);

		ACE_ASSERT (test_graph->size () == 4);
		my_test->pass_obj_graph_in (test_graph.in () ACE_ENV_ARG_PARAMETER);
		ACE_TRY_CHECK;
		ACE_ASSERT (test_graph->size () == 5);

    test_vt_graph->add_node ("NEW1");

    test_graph->add_node ("NEW2");

    ACE_ASSERT (test_vt_graph->size () == 4);
    my_test->pass_vt_graph_out (test_vt_graph.out () ACE_ENV_ARG_PARAMETER);
		ACE_TRY_CHECK;
		ACE_ASSERT (test_vt_graph->size () == 4);

		ACE_ASSERT (test_graph->size () == 6);
		my_test->pass_obj_graph_out (test_graph.out () ACE_ENV_ARG_PARAMETER);
		ACE_TRY_CHECK;
		ACE_ASSERT (test_graph->size () == 5);

    test_vt_graph->add_node ("NEW2");

    test_graph->add_node ("NEW2");

    ACE_ASSERT (test_vt_graph->size () == 5);
    my_test->pass_vt_graph_inout (test_vt_graph.inout () ACE_ENV_ARG_PARAMETER);
		ACE_TRY_CHECK;
		ACE_ASSERT (test_vt_graph->size () == 6);

		ACE_ASSERT (test_graph->size () == 6);
		my_test->pass_obj_graph_inout (test_graph.inout () ACE_ENV_ARG_PARAMETER);
		ACE_TRY_CHECK;
		ACE_ASSERT (test_graph->size () == 7);

		/* Shut down */

		my_test->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
		ACE_TRY_CHECK;

	  orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
		ACE_TRY_CHECK;

    ACE_DEBUG ((LM_DEBUG, "(%P|%t) Client test finished\n"));

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
