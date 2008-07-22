// $Id$

#include "TestC.h"
#include "ace/Get_Opt.h"
#include "tao/ZIOP/ZIOP.h"
#include "tao/Compression/zlib/ZlibCompressor_Factory.h"

ACE_RCSID(Hello, client, "$Id$")

const char *ior = "file://test.ior";

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "k:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
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
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

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

      CORBA::Boolean comp_enables = true;
      CORBA::ULong min_value = 0;
      CORBA::Any compression_enabled;
      CORBA::Any compmin;
      Compression::CompressorIdLevelList compressor_id_list(2);
      compressor_id_list.length(2);
      Compression::CompressorIdLevel levelid;
      levelid.compressor_id = Compression::COMPRESSORID_ZLIB;
      levelid.compression_level = 9;
      compressor_id_list[0] = levelid;
      levelid.compressor_id = Compression::COMPRESSORID_BZIP2;
      compressor_id_list[1] = levelid;
      CORBA::Any compressor_id_any;
      compressor_id_any <<= compressor_id_list;
      compression_enabled <<= CORBA::Any::from_boolean (comp_enables);
      compmin <<= min_value;
      CORBA::PolicyList policies (3);
      policies.length (3);
      policies[0] = orb->create_policy (ZIOP::COMPRESSION_ENABLING_POLICY_ID, compression_enabled);
      policies[1] = orb->create_policy (ZIOP::COMPRESSION_LOW_VALUE_POLICY_ID, compmin);
      policies[2] = orb->create_policy(ZIOP::COMPRESSION_ID_LEVEL_LIST_POLICY_ID,compressor_id_any);
      CORBA::Object_var tmp = orb->string_to_object(ior);
      CORBA::Object_var tmp2 = tmp->_set_policy_overrides (policies, CORBA::ADD_OVERRIDE);

      Test::Hello_var hello = Test::Hello::_narrow(tmp2.in ());

      if (CORBA::is_nil (hello.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Test::Hello reference <%s>\n",
                             ior),
                            1);
        }

      CORBA::String_var the_string = hello->get_string ("This is a test stringThis is a test stringThis is a test stringThis is a test stringThis is a test stringThis is a test stringThis is a test stringThis is a test stringThis is a test string\n");

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) - string returned <%s>\n",
                  the_string.in ()));

      hello->shutdown ();

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}

