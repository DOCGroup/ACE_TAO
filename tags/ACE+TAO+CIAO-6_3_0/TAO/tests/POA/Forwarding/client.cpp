
//=============================================================================
/**
 *  @file     client.cpp
 *
 *  $Id$
 *
 *
 *   This is a simple test client implementation.  Also looks out
 *   for forwarding exceptions
 *
 *
 *  @author  Irfan Pyarali
 */
//=============================================================================


#include "ace/streams.h"
#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"
#include "testC.h"

static ACE_TCHAR *IOR = 0;
static int servers = 2;
static int iterations = 3;

static int
parse_args (int argc, ACE_TCHAR **argv)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("s:i:k:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        IOR = get_opts.opt_arg ();
        break;
      case 's':
        servers = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 'i':
        iterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           "-k IOR\n"
                           "-i iterations\n"
                           "\n",
                           argv [0]),
                          -1);
      }

  if (IOR == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Please specify the IOR for the servant\n"),
                      -1);

  // Indicates successful parsing of command line.
  return 0;
}

void
do_calls (test_ptr test)
{
  for (int j = 1; j <= servers; j++)
    {
      for (int i = 1; i <= iterations; i++)
        {
          // Invoke the doit() method of the test reference.
          CORBA::Long result = test->doit ();

          // Print the result of doit () method of the test reference.
          ACE_DEBUG ((LM_DEBUG,
                      "doit() returned %d\n",
                      result));
        }

      // Don't forward the last server
      if (j != servers)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Asking server to forward next time\n"));
          test->forward ();
        }
    }
}


int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{

  try
    {
      // Initialize the ORB
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      // Initialize options based on command-line arguments.
      int parse_args_result =
        parse_args (argc, argv);
      if (parse_args_result != 0)
        return parse_args_result;

      // Get an object reference from the argument string.
      CORBA::Object_var object =
        orb->string_to_object (IOR);

      // Try to narrow the object reference to a test reference.
      test_var test =
        test::_narrow (object.in ());

      do_calls (test.in ());

      test->shutdown ();

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught in client");
      return -1;
    }

  return 0;
}
