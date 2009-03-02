// $Id$

#include "TestC.h"
#include "ace/Get_Opt.h"
#include "tao/ZIOP/ZIOP.h"
#include "ace/OS.h"
#include "tao/Compression/zlib/ZlibCompressor_Factory.h"
#include "tao/ORB_Constants.h"
#include "tao/TransportCurrent/TCC.h"

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

void log_statistics (ACE_TCHAR* test,
                     ::TAO::CounterT initial_bytes_send,
                     ::TAO::CounterT initial_bytes_recv,
                     ::TAO::CounterT bytes_send_after_test,
                     ::TAO::CounterT bytes_recv_after_test)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("%s statistics:")
              ACE_TEXT (" initial bytes sent:%d")
              ACE_TEXT (" initial bytes recv:%d")
              ACE_TEXT (" bytes sent after test:%d")
              ACE_TEXT (" bytes recv after test:%d\n"),
              test,
              initial_bytes_send,
              initial_bytes_recv,
              bytes_send_after_test,
              bytes_recv_after_test));
}

void start_low_value_test(Test::Hello_ptr hello, ::TAO::Transport::Current_ptr)
{
#if defined (TAO_HAS_TRANSPORT_CURRENT) && TAO_HAS_TRANSPORT_CURRENT == 1
//  ::TAO::CounterT initial_bytes_sent = tc->bytes_sent ();
//  ::TAO::CounterT initial_bytes_recv = tc->bytes_received ();
#endif

  ACE_DEBUG((LM_DEBUG,
            ACE_TEXT("Start get_string; large compression ratio\n")));

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

#if defined (TAO_HAS_TRANSPORT_CURRENT) && TAO_HAS_TRANSPORT_CURRENT ==1
//  log_statistics ("low_value_test", initial_bytes_sent, initial_bytes_recv,
//        tc->bytes_sent (), tc->bytes_received ());
#endif
}

void start_min_ratio_test (Test::Hello_ptr hello, ::TAO::Transport::Current_ptr)
{
#if defined (TAO_HAS_TRANSPORT_CURRENT) && TAO_HAS_TRANSPORT_CURRENT ==1
//  ::TAO::CounterT initial_bytes_sent = tc->bytes_sent ();
//  ::TAO::CounterT initial_bytes_recv = tc->bytes_received ();
#endif

  ACE_DEBUG((LM_DEBUG,
              ACE_TEXT("Start get_string; small compression ratio\n")));

  //shouldn't compress since compress_ratio < min_ratio
  CORBA::String_var the_string = hello->get_string ("!@#$#%^#@&^%*$@#GFGSd"
                                                    "fgdbdfgwe%^@#$#$%EQRT"
                                                    "sfdgdafs56#$@@#$&((%$"
                                                    "#4&%3#4%^21@!sdfSADHv"
                                                    "dsaAhn~1`2#$#sAFDGHdf");

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) - string returned <%C>\n",
              the_string.in ()));

#if defined (TAO_HAS_TRANSPORT_CURRENT) && TAO_HAS_TRANSPORT_CURRENT ==1
//  log_statistics ("min_ratio_test", initial_bytes_sent, initial_bytes_recv,
//        tc->bytes_sent (), tc->bytes_received ());
#endif
}

void start_big_reply_test (Test::Hello_ptr hello, ::TAO::Transport::Current_ptr)
{
#if defined (TAO_HAS_TRANSPORT_CURRENT) && TAO_HAS_TRANSPORT_CURRENT ==1
//  ::TAO::CounterT initial_bytes_sent = tc->bytes_sent ();
//  ::TAO::CounterT initial_bytes_recv = tc->bytes_received ();
#endif

  ACE_DEBUG((LM_DEBUG,
              ACE_TEXT("Start get_big_reply; large compression ratio\n")));

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

#if defined (TAO_HAS_TRANSPORT_CURRENT) && TAO_HAS_TRANSPORT_CURRENT ==1
//  log_statistics ("big_reply_test",initial_bytes_sent, initial_bytes_recv,
//        tc->bytes_sent (), tc->bytes_received ());
#endif
}

void start_big_request_test (Test::Hello_ptr hello, ::TAO::Transport::Current_ptr)
{
#if defined (TAO_HAS_TRANSPORT_CURRENT) && TAO_HAS_TRANSPORT_CURRENT ==1
//  ::TAO::CounterT initial_bytes_sent = tc->bytes_sent ();
//  ::TAO::CounterT initial_bytes_recv = tc->bytes_received ();
#endif

  ACE_DEBUG((LM_DEBUG,
              ACE_TEXT("Start big_request; large compression ratio\n")));
  //ACE_OS::sleep(1);
  int length = 40000;
  Test::Octet_Seq send_msg(length);
  send_msg.length (length);

  hello->big_request(send_msg);

#if defined (TAO_HAS_TRANSPORT_CURRENT) && TAO_HAS_TRANSPORT_CURRENT ==1
//  log_statistics ("big_request_test", initial_bytes_sent, initial_bytes_recv,
//        tc->bytes_sent (), tc->bytes_received ());
#endif
}

void start_tests (Test::Hello_ptr hello, ::TAO::Transport::Current_ptr tc)
{
  //::CORBA::String_var rhost (tc->remote_host ());
  //::CORBA::String_var lhost (tc->local_host ());
  //::CORBA::Long id = tc->id ();
  //::TAO::CounterT bs = tc->bytes_sent ();
  //::TAO::CounterT br = tc->bytes_received ();
  //::TAO::CounterT rs = tc->messages_sent ();
  //::TAO::CounterT rr = tc->messages_received ();


#if !defined (TAO_HAS_TRANSPORT_CURRENT) || TAO_HAS_TRANSPORT_CURRENT == 0
  ACE_DEBUG((LM_DEBUG,
              ACE_TEXT ("No statistical information available since TAO_HAS_TRANSPORT_CURRENT is not set")));
#endif

//  start_low_value_test (hello, tc);

//  start_min_ratio_test (hello, tc);

  start_big_reply_test (hello, tc);

//  start_big_request_test (hello, tc);
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  int extra_argc = 2;

  try
    {
      ACE_TCHAR **extra = 0;
      ACE_NEW_RETURN (extra, ACE_TCHAR *[extra_argc], -1);
      extra[0] = ACE::strnew (ACE_TEXT ("-ORBSvcConfDirective"));
      extra[1] = ACE::strnew (ACE_TEXT ("dynamic TAO_Transport_Current_Loader Service_Object")
                              ACE_TEXT (" * TAO_TC:_make_TAO_Transport_Current_Loader() ''"));

      ACE_TCHAR **largv = new ACE_TCHAR *[argc+extra_argc];
      for (int i = 0; i < argc; i++)
        largv[i] = argv[i];

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("client adding args: ")));
      for (int i = 0; i < extra_argc; i++)
        {
          ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%s "), extra[i]));
          largv[argc+i] = extra[i];
        }
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\n")));

      argc += extra_argc;

      CORBA::ORB_var orb = CORBA::ORB_init (argc, largv);

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

#if defined TAO_HAS_ZIOP && TAO_HAS_ZIOP == 1
      CORBA::PolicyList policies(4);
      policies.length(4);

      policies[0] = orb->create_policy (ZIOP::COMPRESSION_ENABLING_POLICY_ID, compression_enabling_any);
      policies[1] = orb->create_policy (ZIOP::COMPRESSOR_ID_LEVEL_LIST_POLICY_ID, compressor_id_any);
      policies[2] = orb->create_policy (ZIOP::COMPRESSION_LOW_VALUE_POLICY_ID, low_value_any);
      policies[3] = orb->create_policy (ZIOP::COMPRESSION_MIN_RATIO_POLICY_ID, min_compression_ratio_any);

      CORBA::Object_var tmp = orb->string_to_object(ior);
      CORBA::Object_var tmp2 = tmp->_set_policy_overrides (policies, CORBA::ADD_OVERRIDE);
      Test::Hello_var hello = Test::Hello::_narrow(tmp2.in ());
#else
      CORBA::Object_var tmp = orb->string_to_object(ior);
      Test::Hello_var hello = Test::Hello::_narrow(tmp.in ());
#endif


      if (CORBA::is_nil (hello.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Test::Hello reference <%s>\n",
                             ior),
                            1);
        }

      CORBA::Object_var tcobject =
                orb->resolve_initial_references ("TAO::Transport::Current");

      ::TAO::Transport::Current_var tc =
        ::TAO::Transport::Current::_narrow (tcobject.in ());
      if (CORBA::is_nil (tc.in ()))
        throw ::CORBA::INTERNAL ();

      //for (int i = 0; i < 1000; ++i)
        start_tests(hello.in (), tc.in ());

      hello->shutdown ();

      orb->destroy ();

      for (int i = 0; i < extra_argc; i++)
        ACE::strdelete (extra[i]);
      delete [] extra;
      delete [] largv;
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}

