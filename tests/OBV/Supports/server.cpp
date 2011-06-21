// $Id$

#include "Supports_Test_impl.h"
#include "tao/Strategies/advanced_resource.h"
#include "ace/Get_Opt.h"

const ACE_TCHAR *ior_output_file = ACE_TEXT ("test.ior");

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

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-o <iorfile>"
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

      CORBA::Object_var poa_object = orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var root_poa = PortableServer::POA::_narrow (poa_object.in ());

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Nil RootPOA\n"),
                          1);

      PortableServer::POAManager_var poa_manager = root_poa->the_POAManager ();

      /* Create, register factories */

      Supports_Test::Node_init * node_factory = 0;

      ACE_NEW_RETURN (node_factory, node_init_impl, 1);

      CORBA::ValueFactory returned_factory = orb->register_value_factory (
        node_factory->tao_repository_id (),
        node_factory);

      ACE_ASSERT (returned_factory == 0);

      node_factory->_remove_ref ();

      Supports_Test::vt_graph_init * vt_graph_factory = 0;

      ACE_NEW_RETURN (vt_graph_factory, vt_graph_init_impl, 1);

      returned_factory = orb->register_value_factory (
        vt_graph_factory->tao_repository_id (),
        vt_graph_factory);

      ACE_ASSERT (returned_factory == 0);

      vt_graph_factory->_remove_ref ();

      /* create and activate test servant */

      test_impl * a_test_impl;

      ACE_NEW_RETURN (a_test_impl, test_impl (orb.in ()), 1);

      PortableServer::ServantBase_var owner_transfer = a_test_impl;

      PortableServer::ObjectId_var id =
        root_poa->activate_object (a_test_impl);

      CORBA::Object_var object = root_poa->id_to_reference (id.in ());

      Supports_Test::test_var a_test = Supports_Test::test::_narrow (object.in ());

      CORBA::String_var ior = orb->object_to_string (a_test.in ());

      FILE * output_file = ACE_OS::fopen (ACE_TEXT_ALWAYS_CHAR(ior_output_file), "w");

      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR, "Cannot open output file for writing IOR: %s", ior_output_file), 1);

      ACE_OS::fprintf (output_file, "%s", ior.in ());

      ACE_OS::fclose (output_file);

      poa_manager->activate ();

      orb->run ();

      root_poa->destroy (0, 0);

      orb->destroy ();

      ACE_DEBUG ((LM_DEBUG, "Server (%P.%t) completed test successfully\n"));
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
