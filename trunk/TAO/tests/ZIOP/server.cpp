// $Id$

#include "Hello.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "tao/ZIOP/ZIOP.h"
#include "tao/Compression/zlib/ZlibCompressor_Factory.h"
#include "tao/Compression/bzip2/Bzip2Compressor_Factory.h"
#include "TestCompressor/TestCompressor_Factory.h"
#include "tao/Policy_ManagerC.h"
#include "tao/Policy_CurrentC.h"
#include "tao/Transport.h"

#include "common.h"

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:t:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;
      case 't':
        test = ACE_OS::atoi (get_opts.opt_arg ());
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
register_factories (CORBA::ORB_ptr orb)
{
  CORBA::Object_var compression_manager =
    orb->resolve_initial_references("CompressionManager");

  ::Compression::CompressionManager_var manager =
    ::Compression::CompressionManager::_narrow (compression_manager.in ());

  if (CORBA::is_nil(manager.in ()))
    ACE_ERROR_RETURN ((LM_ERROR,
                       " (%P|%t) Panic: nil compression manager\n"),
                      1);
  //register Zlib compressor
  ::Compression::CompressorFactory_ptr compressor_factory;
  ACE_NEW_RETURN (compressor_factory, TAO::Zlib_CompressorFactory (), 1);
  ::Compression::CompressorFactory_var compr_fact = compressor_factory;
  manager->register_factory(compr_fact.in ());

  // register bzip2 compressor
  ACE_NEW_RETURN (compressor_factory, TAO::Bzip2_CompressorFactory (), 1);
  compr_fact = compressor_factory;
  manager->register_factory(compr_fact.in ());

  // register test compressor
  ACE_NEW_RETURN (compressor_factory, TAO::Test_CompressorFactory (), 1);
  compr_fact = compressor_factory;
  manager->register_factory(compr_fact.in ());

  return 0;
}

CORBA::Policy_ptr
create_compressor_id_level_list_policy (CORBA::ORB_ptr orb)
{
  ::Compression::CompressorIdLevelList compressor_id_list;

  if (test == 2)
    {
      ACE_DEBUG((LM_DEBUG,
                 ACE_TEXT("*** NOTE TestCompressor is EXPECTED to throw IDL:omg.org/Compression/CompressionException ***\n")));
      compressor_id_list.length(1);
      compressor_id_list[0].compressor_id = COMPRESSORID_FOR_TESTING;
      compressor_id_list[0].compression_level = SERVER_COMPRESSION_LEVEL;
    }
  else
    {
      compressor_id_list.length(2);
      compressor_id_list[0].compressor_id = ::Compression::COMPRESSORID_BZIP2;
      compressor_id_list[0].compression_level = SERVER_COMPRESSION_LEVEL;
      compressor_id_list[1].compressor_id = ::Compression::COMPRESSORID_ZLIB;
      compressor_id_list[1].compression_level = SERVER_COMPRESSION_LEVEL;
      //compressor_id_list[2].compressor_id = COMPRESSORID_FOR_TESTING;
      //compressor_id_list[2].compression_level = SERVER_COMPRESSION_LEVEL;
    }

  CORBA::Any compressor_id_any;
  compressor_id_any <<= compressor_id_list;

  return orb->create_policy (ZIOP::COMPRESSOR_ID_LEVEL_LIST_POLICY_ID, compressor_id_any);
}

CORBA::Policy_ptr
create_low_value_policy (CORBA::ORB_ptr orb)
{
  // Setting policy for minimum amount of bytes that needs to be
  // compressed. If a message is smaller than this, it doesn't get
  // compressed
  CORBA::ULong compression_low_value = 100;
  CORBA::Any low_value_any;
  low_value_any <<= compression_low_value;

  return orb->create_policy (ZIOP::COMPRESSION_LOW_VALUE_POLICY_ID, low_value_any);
}

CORBA::Policy_ptr
create_compression_enabled_policy (CORBA::ORB_ptr orb)
{
  // Setting policy whether compression is used.
  CORBA::Boolean compression_enabling = true;
  CORBA::Any compression_enabling_any;
  compression_enabling_any <<= CORBA::Any::from_boolean(compression_enabling);

  return orb->create_policy (ZIOP::COMPRESSION_ENABLING_POLICY_ID, compression_enabling_any);
}

CORBA::Policy_ptr
create_min_ratio_policy (CORBA::ORB_ptr orb)
{
  CORBA::Any min_compression_ratio_any;
  Compression::CompressionRatio min_compression_ratio = 0.75;
  min_compression_ratio_any <<= min_compression_ratio;

  return orb->create_policy (ZIOP::COMPRESSION_MIN_RATIO_POLICY_ID, min_compression_ratio_any);
}

Test::Hello_var
prepare_tests (CORBA::ORB_ptr orb, PortableServer::POA_ptr root_poa)
{
  register_factories(orb);

  CORBA::Object_var objectman =
    orb->resolve_initial_references ("ORBPolicyManager");

  CORBA::PolicyManager_var policy_manager =
    CORBA::PolicyManager::_narrow (objectman.in ());

  PortableServer::POA_var my_compress_poa = 0;
  CORBA::PolicyList policies(4);
  policies.length(4);

  try
    {
      policies[0] = create_compressor_id_level_list_policy (orb);
      policies[1] = create_low_value_policy (orb);
      policies[2] = create_compression_enabled_policy (orb);
      policies[3] = create_min_ratio_policy (orb);

      my_compress_poa = root_poa->create_POA("My_Compress_Poa", 0, policies);
    }
  catch(const CORBA::PolicyError&)
    {
      policies.length(0);
      my_compress_poa = root_poa->create_POA("My_Compress_Poa", 0, policies);
    }

  policy_manager->set_policy_overrides (policies, CORBA::ADD_OVERRIDE);

  CORBA::Object_var pcobject =
    orb->resolve_initial_references ("PolicyCurrent");

  CORBA::PolicyCurrent_var policy_current =
    CORBA::PolicyCurrent::_narrow (pcobject.in ());

  policy_current->set_policy_overrides (policies, CORBA::ADD_OVERRIDE);

  PortableServer::POAManager_var poa_manager = my_compress_poa->the_POAManager ();

  Hello *hello_impl = 0;
  ACE_NEW_RETURN (hello_impl,
                  Hello (orb),
                  0);
  PortableServer::ServantBase_var owner_transfer(hello_impl);

  PortableServer::ObjectId_var id =
    my_compress_poa->activate_object (hello_impl);

  CORBA::Object_var object = my_compress_poa->id_to_reference (id.in ());

  Test::Hello_var hello = Test::Hello::_narrow (object.in ());

  poa_manager->activate ();

  return hello._retn ();
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

      if (parse_args (argc, argv) != 0)
        return 1;

      Test::Hello_var hello = prepare_tests (orb.in (), root_poa.in ());

      CORBA::String_var ior = orb->object_to_string (hello.in ());

      // Output the IOR to the <ior_output_file>
      FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file for writing IOR: %s\n",
                           ior_output_file),
                           1);
      ACE_OS::fprintf (output_file, "%s", ior.in ());
      ACE_OS::fclose (output_file);

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
