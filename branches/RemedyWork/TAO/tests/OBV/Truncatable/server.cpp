// $Id$

#include "TruncatableS_impl.h"
#include "TruncatableC.h"
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
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil RootPOA\n"),
                          1);

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      if (parse_args (argc, argv) != 0)
        return 1;

      Test_impl *test_impl;
      ACE_NEW_RETURN (test_impl,
                      Test_impl (orb.in ()),
                      1);

      PortableServer::ServantBase_var owner_transfer(test_impl);

      PortableServer::ObjectId_var id =
        root_poa->activate_object (test_impl);

      CORBA::Object_var object = root_poa->id_to_reference (id.in ());

      OBV_TruncatableTest::Test_var test =
        OBV_TruncatableTest::Test::_narrow (object.in ());

      CORBA::String_var ior =
        orb->object_to_string (test.in ());

      // If the ior_output_file exists, output the ior to it
      FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file for writing IOR: %s",
                           ior_output_file),
                              1);
      ACE_OS::fprintf (output_file, "%s", ior.in ());
      ACE_OS::fclose (output_file);

      poa_manager->activate ();

      OBV_TruncatableTest::BaseValue_init *base_factory = 0;
      ACE_NEW_RETURN (base_factory,
                      OBV_TruncatableTest::BaseValue_init,
                      1);

      orb->register_value_factory (base_factory->tao_repository_id (),
                                   base_factory);
      base_factory->_remove_ref (); // release ownership

      OBV_TruncatableTest::TValue1_init *value1_factory = 0;
      ACE_NEW_RETURN (value1_factory,
                      OBV_TruncatableTest::TValue1_init,
                      1);

      orb->register_value_factory (value1_factory->tao_repository_id (),
                                   value1_factory);
      value1_factory->_remove_ref ();

      OBV_TruncatableTest::TValue4_init *value4_factory = 0;
      ACE_NEW_RETURN (value4_factory,
                      OBV_TruncatableTest::TValue4_init,
                      1);

      orb->register_value_factory (value4_factory->tao_repository_id (),
                                   value4_factory);
      value4_factory->_remove_ref ();

      OBV_TruncatableTest::NestedValue_init *nested_value_factory = 0;
      ACE_NEW_RETURN (nested_value_factory,
                      OBV_TruncatableTest::NestedValue_init,
                      1);

      orb->register_value_factory (nested_value_factory->tao_repository_id (),
                                   nested_value_factory);
      nested_value_factory->_remove_ref ();

      orb->run ();

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - event loop finished\n"));

      root_poa->destroy (1, 1);

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
