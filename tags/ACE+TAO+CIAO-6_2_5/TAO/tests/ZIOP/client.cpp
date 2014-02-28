// $Id$

#include "TestC.h"
#include "ace/Get_Opt.h"
#include "tao/ZIOP/ZIOP.h"
#include "tao/Compression/zlib/ZlibCompressor_Factory.h"
#include "tao/Compression/bzip2/Bzip2Compressor_Factory.h"
#include "TestCompressor/TestCompressor_Factory.h"

#include "common.h"
static const ACE_TCHAR *ior = ACE_TEXT("file://") DEFAULT_IOR_FILENAME;
static ::Compression::CompressionManager_var compression_manager = 0;

int start_tests (Test::Hello_ptr hello, CORBA::ORB_ptr orb);

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:t:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;
      case 't':
        test = ACE_OS::atoi (get_opts.opt_arg ());
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
register_factories (CORBA::ORB_ptr orb)
{
  CORBA::Object_var compression_manager_obj =
    orb->resolve_initial_references("CompressionManager");

  compression_manager = ::Compression::CompressionManager::_narrow (
            compression_manager_obj.in ());

  if (CORBA::is_nil(compression_manager.in ()))
    ACE_ERROR_RETURN ((LM_ERROR,
                       " (%P|%t) Panic: nil compression manager\n"),
                      1);
  //register Zlib compressor
  ::Compression::CompressorFactory_ptr compressor_factory;
  ACE_NEW_RETURN (compressor_factory, TAO::Zlib_CompressorFactory (), 1);
  ::Compression::CompressorFactory_var compr_fact = compressor_factory;
  compression_manager->register_factory(compr_fact.in ());

  // register bzip2 compressor
  ACE_NEW_RETURN (compressor_factory, TAO::Bzip2_CompressorFactory (), 1);
  compr_fact = compressor_factory;
  compression_manager->register_factory(compr_fact.in ());

  // register test compressor
  ACE_NEW_RETURN (compressor_factory, TAO::Test_CompressorFactory (), 1);
  compr_fact = compressor_factory;
  compression_manager->register_factory(compr_fact.in ());
  return 0;
}

CORBA::Policy_ptr
create_compressor_id_level_list_policy (CORBA::ORB_ptr orb, bool add_zlib_for_test_1)
{
  ::Compression::CompressorIdLevelList compressor_id_list;

  switch (test)
    {
    case 1:
      if (add_zlib_for_test_1)
        compressor_id_list.length(2);
      else
        compressor_id_list.length(1);
      compressor_id_list[0].compressor_id = ::Compression::COMPRESSORID_LZO;
      compressor_id_list[0].compression_level = CLIENT_COMPRESSION_LEVEL;
      if (add_zlib_for_test_1)
        {
          compressor_id_list[1].compressor_id = ::Compression::COMPRESSORID_ZLIB;
          compressor_id_list[1].compression_level = CLIENT_COMPRESSION_LEVEL;
        }
      break;
    case 2:
      compressor_id_list.length(1);
      compressor_id_list[0].compressor_id = COMPRESSORID_FOR_TESTING;
      compressor_id_list[0].compression_level = CLIENT_COMPRESSION_LEVEL;
      break;
    case 3:
    case 4:
    default:
      compressor_id_list.length(2);
      compressor_id_list[0].compressor_id = ::Compression::COMPRESSORID_ZLIB;
      compressor_id_list[0].compression_level = CLIENT_COMPRESSION_LEVEL;
      compressor_id_list[1].compressor_id = ::Compression::COMPRESSORID_BZIP2;
      compressor_id_list[1].compression_level = CLIENT_COMPRESSION_LEVEL;
      break;
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
  // compressed.
  // make sure everything gets compressed.
  CORBA::ULong compression_low_value = 10;
  if (test == 3)
    compression_low_value = 5000000;

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
  Compression::CompressionRatio min_compression_ratio = 0.50;
  min_compression_ratio_any <<= min_compression_ratio;

  return orb->create_policy (ZIOP::COMPRESSION_MIN_RATIO_POLICY_ID, min_compression_ratio_any);
}

Test::Hello_var
create_policies (CORBA::ORB_ptr orb, bool add_zlib_compressor)
{
  CORBA::PolicyList policies(4);
  policies.length(4);

  policies[0] = create_compressor_id_level_list_policy (orb, add_zlib_compressor);
  policies[1] = create_low_value_policy (orb);
  policies[2] = create_compression_enabled_policy (orb);
  policies[3] = create_min_ratio_policy (orb);

  CORBA::Object_var tmp = orb->string_to_object(ior);
  CORBA::Object_var tmp2 = tmp->_set_policy_overrides (policies, CORBA::ADD_OVERRIDE);
  Test::Hello_var hello = Test::Hello::_narrow(tmp2.in ());
  return hello._retn ();
}

Test::Hello_var
prepare_tests (CORBA::ORB_ptr orb, bool create_factories=true)
{
#if defined TAO_HAS_ZIOP && TAO_HAS_ZIOP == 1
  if (create_factories)
    {
      register_factories(orb);
    }

  return create_policies (orb, !create_factories);
#else
  ACE_UNUSED_ARG (create_factories);
  CORBA::Object_var tmp = orb->string_to_object(ior);
  Test::Hello_var hello = Test::Hello::_narrow(tmp.in ());
  return hello._retn ();
#endif
}

int
check_results (CORBA::ORB_ptr orb)
{
#if defined TAO_HAS_ZIOP && TAO_HAS_ZIOP == 1
  switch (test)
    {
    case 1:
      try
        {
          // should throw an exception
          ::Compression::Compressor_var compressor (
            compression_manager->get_compressor (
              ::Compression::COMPRESSORID_LZO,
              LEAST_COMPRESSION_LEVEL ));
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("ERROR : check_results, ")
                             ACE_TEXT ("no exception thrown when applying for ")
                             ACE_TEXT ("LZO Compressor\n")),
                            1);
        }
      catch (::Compression::UnknownCompressorId)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("check_results, expected exception caught, ")
                      ACE_TEXT ("(unknown factory)\n")));
          Test::Hello_var hello = prepare_tests (orb, false);
          test = -1;
          return start_tests (hello.in (), orb);
        }
      break;
    case 4:
    case -1:
      {
        ::Compression::Compressor_var compressor (
          compression_manager->get_compressor (
            ::Compression::COMPRESSORID_ZLIB,
            LEAST_COMPRESSION_LEVEL ));
        if (!CORBA::is_nil (compressor))
          {
            if (compressor->compressed_bytes () == 0)
              ACE_ERROR_RETURN ((LM_ERROR,
                                 ACE_TEXT ("ERROR : check_results, no compression used ")
                                 ACE_TEXT ("during test 1a\n")),
                                1);
            else
               return 0;
          }
        else
          {
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("ERROR : check_results, zlib compressor not found ")
                               ACE_TEXT ("during test 1a\n")),
                              1);
          }
      }
      break;
    case 2:
      return 0;
      break;
    case 3:
      {
        // low value policy test. No compression should be used.
        ::Compression::Compressor_var compressor (
        compression_manager->get_compressor (
          ::Compression::COMPRESSORID_ZLIB,
          LEAST_COMPRESSION_LEVEL ));
        if (!CORBA::is_nil (compressor))
          {
            if (compressor->compressed_bytes () != 0)
              ACE_ERROR_RETURN ((LM_ERROR,
                                 ACE_TEXT ("ERROR : check_results, compression used ")
                                 ACE_TEXT ("during test %d\n"), test),
                                1);
            else
              return 0;
          }
        else
          {
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("ERROR : check_results, zlib compressor not found ")
                               ACE_TEXT ("during test %d\n"), test),
                              1);
          }
      }
      break;
    default:
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("ERROR : check_results, unknown test ID\n")),
                        1);
    }
  ACE_ERROR_RETURN ((LM_ERROR,
                     ACE_TEXT ("ERROR : check_results, unexpected\n")),
                    1);
#else
  ACE_UNUSED_ARG (orb);
  return 0;
#endif
}

int
run_string_test (Test::Hello_ptr hello)
{
  ACE_DEBUG((LM_DEBUG,
            ACE_TEXT("run_string_test, start\n")));

  if (test == 2)
    {
      ACE_DEBUG((LM_DEBUG,
                 ACE_TEXT("*** NOTE TestCompressor is EXPECTED to throw IDL:omg.org/Compression/CompressionException ***\n")));
    }

  CORBA::String_var the_string = hello->get_string ("This is a test string"
                                                    "This is a test string"
                                                    "This is a test string"
                                                    "This is a test string"
                                                    "This is a test string"
                                                    "This is a test string"
                                                    "This is a test string"
                                                    "This is a test string"
                                                    "This is a test string");
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) - string returned <%C>\n",
              the_string.in ()));
  if (ACE_OS::strstr (the_string.in (), "Hello there") == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("ERROR : run_string_test, unexpected string received\n")),
                        1);
    }

  return 0;
}

int
run_big_reply_test (Test::Hello_ptr hello)
{
  ACE_DEBUG((LM_DEBUG,
              ACE_TEXT("start get_big_reply\n")));

  if (test == 2)
    {
      ACE_DEBUG((LM_DEBUG,
                 ACE_TEXT("*** NOTE TestCompressor is EXPECTED to throw IDL:omg.org/Compression/CompressionException ***\n")));
    }

  //Prepare to send a large number of bytes. Should be compressed
  Test::Octet_Seq_var dummy = hello->get_big_reply ();
  if (dummy->length () > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT("get_big_reply, received = %d bytes\n"),
                      dummy->length ()));
    }
  else
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("ERROR : get_big_reply, ")
                         ACE_TEXT ("error receiving client side blob\n")),
                        1);
    }
  return 0;
}

int
run_big_request_test (Test::Hello_ptr hello)
{
  const int length = 40000;
  Test::Octet_Seq send_msg(length);
  send_msg.length (length);

  for (int i= 0; i<length; ++i)
    {
      send_msg[i]= static_cast<CORBA::Octet> (i & 0xff);
    }

  ACE_DEBUG((LM_DEBUG,
              ACE_TEXT("run_big_request_test, send = %d bytes\n"), length));

  if (test == 2)
    {
      ACE_DEBUG((LM_DEBUG,
                 ACE_TEXT("*** NOTE TestCompressor is EXPECTED to throw IDL:omg.org/Compression/CompressionException ***\n")));
    }

  hello->big_request(send_msg);
  return 0;
}

int
start_tests (Test::Hello_ptr hello, CORBA::ORB_ptr orb)
{
  int result = 0;
  if (test != 4)
    {
      result += run_string_test (hello);
      result += run_big_request_test (hello);
    }
  result += run_big_reply_test (hello);

  result += check_results (orb);
  return result;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  int result = 0;
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      Test::Hello_var hello = prepare_tests (orb.in ());

      if (CORBA::is_nil (hello.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "ERROR : Nil Test::Hello reference <%C>\n",
                             ior),
                            1);
        }

      try
        {
          result += start_tests(hello.in (), orb.in ());
        }
      catch (const CORBA::Exception& ex)
        {
          ex._tao_print_exception ("Exception caught:");
          ++result;
        }

      if (test == 2)
        {
          ACE_DEBUG((LM_DEBUG,
                     ACE_TEXT("*** NOTE TestCompressor is EXPECTED to throw IDL:omg.org/Compression/CompressionException ***\n")));
        }

      hello->shutdown ();

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      ++result;
    }

  return result;
}
