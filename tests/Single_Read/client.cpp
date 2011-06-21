// $Id$

#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"
#include "testC.h"

// Name of file contains ior.
static const ACE_TCHAR *IOR = ACE_TEXT ("file://ior");

// Default iterations.
static u_long iterations = 20;

// Default number of bytes to send as data.
static CORBA::ULong data_bytes = 1000;

// Flag indicates whether to shutdown remote server or not upon client
// shutdown.
static int shutdown_server = 0;

static int
parse_args (int argc, ACE_TCHAR **argv)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:i:d:x"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        IOR = get_opts.opt_arg ();
        break;
      case 'i':
        iterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 'd':
        data_bytes = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 'x':
        shutdown_server = 1;
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k IOR "
                           "-i iterations "
                           "-d data bytes "
                           "-x shutdown server "
                           "\n",
                           argv [0]),
                          -1);
      }

  if (IOR == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Please specify the IOR for the servant\n"), -1);

  if (data_bytes < 10)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Please specify a value of more than 10 bytes\n"),
                      -1);

  // Indicates successful parsing of command line.
  return 0;
}


int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{

  try
    {
      // Initialize the ORB.
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      // Initialize options based on command-line arguments.
      int parse_args_result = parse_args (argc, argv);
      if (parse_args_result != 0)
        return parse_args_result;

      // Get an object reference from the argument string.
      CORBA::Object_var object =
        orb->string_to_object (IOR);

      // Try to narrow the object reference to a <test> reference.
      test_var test_object = test::_narrow (object.in ());

      test::data the_data0 (data_bytes);
      the_data0.length (data_bytes);

      data_bytes *= 10;
      test::data the_data1 (data_bytes);
      the_data1.length (data_bytes);

      data_bytes /= 100;
      test::data the_data2 (data_bytes);
      the_data2.length (data_bytes);

      for (CORBA::ULong i = 1; i <= iterations; ++i)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "client: Iteration %d @ %T\n",
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
      if (shutdown_server)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "(%P|%t) Sending a shutdown call..\n"));
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
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return -1;
    }


  return 0;
}
