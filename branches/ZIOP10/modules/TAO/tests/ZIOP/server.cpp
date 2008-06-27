// $Id$

#include "Hello.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "tao/ZIOP/ZIOP.h"
#include "tao/Compression/zlib/ZlibCompressor_Factory.h"

ACE_RCSID (Hello,
           server,
           "$Id$")

const char *ior_output_file = "test.ior";
Compression::CompressionLevel level = 9;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "o:l:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;
      case 'l':
        level = ACE_OS::atoi (get_opts.opt_arg ());
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
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var compression_manager =
        orb->resolve_initial_references("CompressionManager");

      Compression::CompressionManager_var manager =
        Compression::CompressionManager::_narrow (compression_manager.in ());

      if (CORBA::is_nil(manager.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil compression manager\n"),
                          1);

      Compression::CompressorFactory_ptr compressor_factory;

      ACE_NEW_RETURN (compressor_factory, TAO::Zlib_CompressorFactory (), 1);

      Compression::CompressorFactory_var compr_fact = compressor_factory;
      manager->register_factory(compr_fact.in ());

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

CORBA::Boolean compression_enabling = true;
Compression::CompressorIdLevelList compressor_id_list(2);
compressor_id_list.length(2);
Compression::CompressorIdLevel levelid;
levelid.compressor_id = Compression::COMPRESSORID_ZLIB;
levelid.compression_level = 9;
compressor_id_list[0] = levelid;
levelid.compressor_id = Compression::COMPRESSORID_BZIP2;
compressor_id_list[1] = levelid;
//CORBA::ULong compression_low_value = 16384;
//CORBA::ULong min_compression_ratio = 40;
CORBA::Any compression_enabling_any;//, compressor_id_any, low_value_any;
//CORBA::Any min_compression_ratio_any;
compression_enabling_any <<= CORBA::Any::from_boolean(compression_enabling);
CORBA::Any compressor_id_any;
compressor_id_any <<= compressor_id_list;
//low_value_any <<= compression_low_value;
//min_compression_ratio_any <<= min_compression_ratio;
PortableServer::POA_var my_compress_poa = NULL;
CORBA::PolicyList policies(2);
policies.length(2);
try {
policies[0] = orb->create_policy(ZIOP::COMPRESSION_ENABLING_POLICY_ID, compression_enabling_any);
policies[1] = orb->create_policy(ZIOP::COMPRESSION_ID_LIST_POLICY_ID,compressor_id_any);
//policies[2] = orb->create_policy(ZIOP::COMPRESSION_LOW_VALUE_POLICY_ID,compressor_id_any);
//policies[3] = orb->create_policy(ZIOP::MIN_COMPRESSION_RATIO_POLICY_ID,min_compression_ratio);
my_compress_poa = root_poa->create_POA("My_Compress_Poa", NULL, policies);
} catch(const CORBA::PolicyError&) {
policies.length(0);
my_compress_poa = root_poa->create_POA("My_Compress_Poa", NULL, policies);
}
      PortableServer::POAManager_var poa_manager = my_compress_poa->the_POAManager ();

      Hello *hello_impl = 0;
      ACE_NEW_RETURN (hello_impl,
                      Hello (orb.in ()),
                      1);
      PortableServer::ServantBase_var owner_transfer(hello_impl);

      PortableServer::ObjectId_var id =
        my_compress_poa->activate_object (hello_impl);

      CORBA::Object_var object = my_compress_poa->id_to_reference (id.in ());

      Test::Hello_var hello = Test::Hello::_narrow (object.in ());

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

      poa_manager->activate ();

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

