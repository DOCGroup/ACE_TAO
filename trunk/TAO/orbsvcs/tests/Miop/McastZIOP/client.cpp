// $Id$

#include "TestC.h"
#include "ace/Get_Opt.h"
#include "tao/Object_T.h"
#include "orbsvcs/PortableGroup/MIOP.h"
#include "tao/ZIOP/ZIOP.h"
#include "tao/Compression/zlib/ZlibCompressor_Factory.h"

#define MAX_MIOP_OCTET_SEQUENCE  (ACE_MAX_DGRAM_SIZE - 272 /* MIOP_MAX_HEADER_SIZE */)

const ACE_TCHAR *ior = ACE_TEXT("file://test.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:"));
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
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  int exit_status = 0;
  try
    {
      CORBA::ORB_var orb (CORBA::ORB_init (argc, argv));

      CORBA::Object_var compression_manager_obj (
        orb->resolve_initial_references("CompressionManager"));

      ::Compression::CompressionManager_var compression_manager (
        ::Compression::CompressionManager::_narrow (
          compression_manager_obj.in ()));

      if (CORBA::is_nil(compression_manager.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil compression manager\n"),
                          1);

      //register Zlib compressor
      ::Compression::CompressorFactory_ptr compressor_factory;
      ACE_NEW_RETURN (compressor_factory, TAO::Zlib_CompressorFactory (), 1);
      ::Compression::CompressorFactory_var compr_fact (compressor_factory);
      compression_manager->register_factory (compr_fact.in ());

      //Register Client ZIOP policies
      CORBA::PolicyList policies(4);
      policies.length(4);

      ::Compression::CompressorIdLevelList compressor_id_list;
      compressor_id_list.length (1);
      compressor_id_list[0].compressor_id     = ::Compression::COMPRESSORID_ZLIB;
      compressor_id_list[0].compression_level = 9;
      CORBA::Any any;
      any <<= compressor_id_list;

      policies[0] = orb->create_policy (ZIOP::COMPRESSOR_ID_LEVEL_LIST_POLICY_ID, any);
      any <<= static_cast <CORBA::ULong> (190u); // shutdown is 180, send_forty_two is 192
      policies[1] = orb->create_policy (ZIOP::COMPRESSION_LOW_VALUE_POLICY_ID, any);
      any <<= CORBA::Any::from_boolean (true);
      policies[2] = orb->create_policy (ZIOP::COMPRESSION_ENABLING_POLICY_ID, any);
      any <<= static_cast <Compression::CompressionRatio> (0.50); // send_forty_two is 0.66, send_large_octet_array is 0.06
      policies[3] = orb->create_policy (ZIOP::COMPRESSION_MIN_RATIO_POLICY_ID, any);

      // Parse our own client arguments
      if (parse_args (argc, argv) != 0)
        return 1;

      // Obtain the servant reference with our active ZIOP policies.
      CORBA::Object_var tmp (orb->string_to_object(ior));
      tmp = tmp->_set_policy_overrides (policies, CORBA::ADD_OVERRIDE);

      /* Do an unchecked narrow since there's no way to do an is_a on
       * a multicast reference (yet...).
       */
      Test::McastHello_var hello =
        TAO::Narrow_Utils<Test::McastHello>::unchecked_narrow (
            tmp.in ());

      if (CORBA::is_nil (hello.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Test::Hello reference <%s>\n",
                             ior),
                            1);
        }

      // To enable us to check that we compress the correct messages.
      ::Compression::Compressor_var compressor (
        compression_manager->get_compressor (
          compressor_id_list[0].compressor_id,
          compressor_id_list[0].compression_level));
      if (CORBA::is_nil (compressor))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "ERROR : compressor not found!\n"),
                            1);
        }

      ACE_DEBUG ((LM_DEBUG,
                  "Client sending send_forty_two() message to server\n"));
      hello->send_forty_two (42);
      // Note we can't actually check that we did NOT compress the message,
      // as the compressor is used to TRIAL the compression before it is
      // rejected based upon the min ratio. The compressor thus records
      // this trail compression data length.  (ZIOP is almost completely
      // transparrent in operation to the client and server when operating).
      CORBA::ULong total_compressed_so_far= compressor->compressed_bytes ();

      Test::Octets payload (MAX_MIOP_OCTET_SEQUENCE);
      payload.length (MAX_MIOP_OCTET_SEQUENCE);

      for (CORBA::ULong j = 0; j != MAX_MIOP_OCTET_SEQUENCE; ++j)
        {
          payload[j] = j % 256;
        }

      ACE_DEBUG ((LM_DEBUG,
                  "Client sending send_large_octet_array() message to server\n"));
      hello->send_large_octet_array (payload);
      // Check we did compress the message
      if (compressor->compressed_bytes ()-total_compressed_so_far)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Correct. Client did use compression!\n"));
        }
      else
        {
          ACE_DEBUG ((LM_ERROR,
                      "ERROR : check_results, no compression used!\n"));
          exit_status = 1;
        }
      total_compressed_so_far= compressor->compressed_bytes ();

      // Now shutdown the server
      ACE_DEBUG ((LM_DEBUG,
                  "Client sending shutdown() message to server\n"));
      hello->shutdown ();
      // Check we did NOT compress the message
      if (compressor->compressed_bytes ()-total_compressed_so_far)
        {
          ACE_DEBUG ((LM_ERROR,
                      "ERROR : check_results, Client did use compression!\n"));
          exit_status = 1;
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Correct. Client did NOT use compression!\n"));
        }
      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      exit_status = 1;
    }

  return exit_status;
}
