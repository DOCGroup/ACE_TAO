#include "ace/Get_Opt.h"
#include "ace/SString.h"
#include "testC.h"

struct Client_Options
{
  ACE_TString ior {ACE_TEXT ("file://ior")};
  CORBA::ULong iterations {20};
  CORBA::ULong data_bytes {1000};
  bool shutdown_server {};
};

static int
parse_args (int argc, ACE_TCHAR *argv[], Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:i:d:x"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        options.ior = get_opts.opt_arg ();
        break;
      case 'i':
        options.iterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 'd':
        options.data_bytes = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 'x':
        options.shutdown_server = true;
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage: %s -k <ior> -i <iterations> -d <data bytes> -x\n"),
                           argv[0]),
                          -1);
      }

  if (options.ior.empty ())
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Please specify the IOR for the servant\n")), -1);

  if (options.data_bytes < 10)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Please specify a value of at least 10 bytes\n")),
                      -1);

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      Client_Options options;
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv, options) != 0)
        return 1;

      CORBA::Object_var const object = orb->string_to_object (options.ior.c_str ());

      test_var const test_object = test::_narrow (object.in ());

      CORBA::ULong const data_bytes0 = options.data_bytes;
      CORBA::ULong const data_bytes1 = options.data_bytes * 10;
      CORBA::ULong const data_bytes2 = data_bytes1 / 100;

      test::data the_data0 (data_bytes0);
      the_data0.length (data_bytes0);

      test::data the_data1 (data_bytes1);
      the_data1.length (data_bytes1);

      test::data the_data2 (data_bytes2);
      the_data2.length (data_bytes2);

      for (CORBA::ULong i {1}; i <= options.iterations; ++i)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("client: Iteration %u @ %T\n"),
                      i));

          // Invoke the oneway method.
          test_object->method (i,
                               the_data0);

          // Invoke the oneway method.
          test_object->method (i,
                               the_data1);

          // Invoke the oneway method.
          test_object->method (i,
                               the_data2);
        }

      // Shutdown server.
      if (options.shutdown_server)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) Sending a shutdown call..\n")));
          test_object->shutdown ();
        }

      // Destroy the ORB.  On some platforms, e.g., Win32, the socket
      // library is closed at the end of main().  This means that any
      // socket calls made after main() fail. Hence if we wait for
      // static destructors to flush the queues, it will be too late.
      // Therefore, we use explicit destruction here and flush the
      // queues before main() ends.
      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return -1;
    }
  return 0;
}
