// $Id$

#include "Supports_Test_impl.h"

const ACE_TCHAR *ior = ACE_TEXT("file://test.ior");
int num_trials = 1;
int id = 0;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("t:i:k:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 't':
        num_trials = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'i':
        id = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-t <trials> "
                           "-k <ior> "
                           "-i <id> "
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

  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var poa_object =
        orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR," (%P|%t) Nil RootPOA\n"), 1);

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      poa_manager->activate ();

      /* Create, register factories */

      Supports_Test::Node_init * node_factory = 0;

      ACE_NEW_RETURN (node_factory, node_init_impl, 1);

      CORBA::ValueFactory returned_factory =
        orb->register_value_factory (node_factory->tao_repository_id (),
                                     node_factory);

      ACE_ASSERT (returned_factory == 0);

      node_factory->_remove_ref ();


      Supports_Test::vt_graph_init * vt_graph_factory = 0;

      ACE_NEW_RETURN (vt_graph_factory, vt_graph_init_impl, 1);

      returned_factory =
        orb->register_value_factory (vt_graph_factory->tao_repository_id (),
                                     vt_graph_factory);

      ACE_ASSERT (returned_factory == 0);

      vt_graph_factory->_remove_ref ();


      /* Check return values for register_value_factory */

      Supports_Test::Node_init * node_factory2 = 0;

      ACE_NEW_RETURN (node_factory2, node_init_impl, 1);

      returned_factory =
        orb->register_value_factory (node_factory2->tao_repository_id (),
                                     node_factory2);

      ACE_ASSERT (returned_factory == node_factory);

      node_factory2->_remove_ref ();


      /* Get test object reference */

      CORBA::Object_var tmp = orb->string_to_object (ior);

      Supports_Test::test_var my_test =
        Supports_Test::test::_narrow (tmp.in ());

      if (CORBA::is_nil (my_test.in ()))
        ACE_ERROR_RETURN ((LM_DEBUG, "Nil Supports_Test::test obj ref <%s>\n", ior), 1);

      my_test->start ();

      /* Perform test */

      for (int i = 0; i < num_trials; i++)
        {

          // Create a vt_graph_impl instance and store the reference as a
          // vt_graph_var. Then register the instance with the POA to obtain an
          // object reference, stored as a graph_var. Increment the reference count
          // of the vt_graph_impl instance.
          vt_graph_impl * the_vt_graph = 0;
          ACE_NEW_RETURN (the_vt_graph, vt_graph_impl (3), 1);
          Supports_Test::vt_graph_var test_vt_graph = the_vt_graph;

          Supports_Test::graph_var test_graph =
            the_vt_graph->_this ();
          if (CORBA::is_nil (test_graph.in ()))
            ACE_ERROR_RETURN ((LM_DEBUG, "Nil Supports_Test::graph obj ref\n"), 1);
          the_vt_graph->DefaultValueRefCountBase::_add_ref ();

          // At this point, test_vt_graph and test_graph refer to the same object.
          ACE_ASSERT (test_vt_graph->size () == 3);
          my_test->pass_vt_graph_in (test_vt_graph.in ());
          ACE_ASSERT (test_vt_graph->size () == 3);

          ACE_ASSERT (test_graph->size () == 3);
          my_test->pass_obj_graph_in (test_graph.in ());
          ACE_ASSERT (test_graph->size () == 4);

          test_vt_graph->add_node ("NEW2");

          test_graph->add_node ("NEW3");

          // After the 'pass_vt_graph_out' call returns, test_vt_graph will refer to
          // a new object.
          ACE_ASSERT (test_vt_graph->size () == 6);
          my_test->pass_vt_graph_out (test_vt_graph.out ());
          ACE_ASSERT (test_vt_graph->size () == 5);

          // 'test_graph' still refers to the original object, but after the
          // 'pass_obj_graph_out' call returns, it will refer to a new object,
          // residing on the server.
          ACE_ASSERT (test_graph->size () == 6);
          my_test->pass_obj_graph_out (test_graph.out ());
          ACE_ASSERT (test_graph->size () == 5);

          // test_vt_graph and test_graph now refer to different objects.
          test_vt_graph->add_node ("NEW2");

          test_graph->add_node ("NEW2");

          ACE_ASSERT (test_vt_graph->size () == 6);
          my_test->pass_vt_graph_inout (test_vt_graph.inout ());
          ACE_ASSERT (test_vt_graph->size () == 7);

          ACE_ASSERT (test_graph->size () == 6);
          my_test->pass_obj_graph_inout (test_graph.inout ());
          ACE_ASSERT (test_graph->size () == 7);

        }

      my_test->finish ();

      /* Shut down */

      orb->destroy ();

      ACE_DEBUG ((LM_DEBUG, "Client (%P.%t) completed test successfully\n", id));

    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
