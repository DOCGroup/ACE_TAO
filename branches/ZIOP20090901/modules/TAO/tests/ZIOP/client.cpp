// $Id$

#include "TestC.h"
#include "ace/Get_Opt.h"
#include "tao/ZIOP/ZIOP.h"
#include "ace/OS.h"
#include "tao/Compression/zlib/ZlibCompressor_Factory.h"

ACE_RCSID(Hello, client, "$Id$")

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

      Compression::CompressorIdLevelList compressor_id_list(2);
      compressor_id_list.length(2);
      compressor_id_list[0].compressor_id = Compression::COMPRESSORID_ZLIB;
      compressor_id_list[0].compression_level = 5;
      compressor_id_list[1].compressor_id = Compression::COMPRESSORID_BZIP2;
      compressor_id_list[1].compression_level = 5;
      
      //Setting policy whether compression is used.
      CORBA::Boolean compression_enabling = true;
      CORBA::Any compression_enabling_any;
      compression_enabling_any <<= CORBA::Any::from_boolean(compression_enabling);

      //Setting policy for minimum amount of bytes that needs to be 
      //compressed. If a message is smaller than this, it doesn't get
      //compressed
      CORBA::ULong compression_low_value = 100;
      CORBA::Any low_value_any;
      low_value_any <<= compression_low_value;

      CORBA::Any min_compression_ratio_any;
      CORBA::Long min_compression_ratio = 75;
      min_compression_ratio_any <<= min_compression_ratio;
      
      CORBA::Any compressor_id_any;
      compressor_id_any <<= compressor_id_list;

      CORBA::PolicyList policies(4);
      policies.length(4);

      policies[0] = orb->create_policy (ZIOP::COMPRESSION_ENABLING_POLICY_ID, compression_enabling_any);
      policies[1] = orb->create_policy (ZIOP::COMPRESSOR_ID_LEVEL_LIST_POLICY_ID, compressor_id_any);
      policies[2] = orb->create_policy (ZIOP::COMPRESSION_LOW_VALUE_POLICY_ID, low_value_any);
      policies[3] = orb->create_policy (ZIOP::COMPRESSION_MIN_RATIO_POLICY_ID, min_compression_ratio_any);

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
      //shouldn't compress since data.length < low_value policy
      ACE_DEBUG((LM_DEBUG, 
                  ACE_TEXT("Start get_string; large compression ratio")));
      //ACE_OS::sleep(1);
      CORBA::String_var the_string = hello->get_string ("This is a test string"
                                                        "This is a test string"
                                                        "This is a test string"
                                                        "This is a test string"
                                                        "This is a test string"
                                                        "This is a test string"
                                                        "This is a test string"
                                                        "This is a test string"
                                                        "This is a test string");
      
      CORBA::LongLong bytes_send_2 = ::TAO::Transport::Stats().bytes_sent ();
      /*
      ACE_DEBUG ((LM_DEBUG,
        ACE_TEXT("(%P|%t) - Initial bytes send %d; Bytes send after invocation: %d; Difference : %d\n"), 
                  bytes_send_1, bytes_send_2, bytes_send_2 - bytes_send_1));
      */

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) - string returned <%C>\n",
                  the_string.in ()));

      ACE_DEBUG((LM_DEBUG, 
                  ACE_TEXT("Start get_string; small compression ratio")));
      //ACE_OS::sleep(1);

      //shouldn't compress since compress_ratio < min_ratio
      the_string = hello->get_string ("!@#$#%^#@&^%*$@#GFGSd"
                                      "fgdbdfgwe%^@#$#$%EQRT"
                                      "sfdgdafs56#$@@#$&((%$"
                                      "#4&%3#4%^21@!sdfSADHv"
                                      "dsaAhn~1`2#$#sAFDGHdf");

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) - string returned <%C>\n",
                  the_string.in ()));

      ACE_DEBUG((LM_DEBUG, 
                  ACE_TEXT("Start get_big_reply; large compression ratio")));
      //ACE_OS::sleep(1);

      //Prepare to send a large number of bytes. Should be compressed
      Test::Octet_Seq_var dummy = hello->get_big_reply ();
      if (dummy.ptr ()->length () > 0)
        {
          ACE_DEBUG ((LM_DEBUG, 
                      ACE_TEXT("Client side BLOB received\n")));
        }
      else
        {
          ACE_DEBUG ((LM_ERROR, 
                      ACE_TEXT("Error recieving BLOB on Client\n")));
        }

      //now send a large blob from here....
      ACE_DEBUG((LM_DEBUG, 
                  ACE_TEXT("Start big_request; large compression ratio")));
      //ACE_OS::sleep(1);
      int length = 2000;
      Test::Octet_Seq send_msg(length);
      send_msg.length (length);

      hello->big_request(send_msg);

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

