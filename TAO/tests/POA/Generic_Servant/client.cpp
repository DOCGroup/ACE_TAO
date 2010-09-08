// $Id$

#include "ace/Get_Opt.h"
#include "ace/Argv_Type_Converter.h"
#include "ace/Profile_Timer.h"
#include "ace/Read_Buffer.h"
#include "testC.h"

static ACE_TCHAR *IOR = 0;
static int iterations = 1;
static int oneway = 0;
static int shutdown_server = 0;
static CORBA::ULong timeout = 5;
static int timed_method = 0;

static int
parse_args (int argc, ACE_TCHAR **argv)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("f:k:i:T:otx"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        IOR = get_opts.opt_arg ();
        break;

      case 'o':
        oneway = 1;
        break;

      case 't':
        timed_method = 1;
        break;

      case 'i':
        iterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'T':
        timeout = static_cast<CORBA::ULong> (ACE_OS::atoi (get_opts.opt_arg ()));
        break;

      case 'x':
        shutdown_server = 1;
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k IOR "
                           "-o oneway "
                           "-t timed operations "
                           "-T timeout for timed operations "
                           "-i iterations "
                           "-x shutdown server "
                           "\n",
                           argv [0]),
                          -1);
      }

  if (IOR == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Please specify the IOR for the servant\n"), -1);

  // Indicates successful parsing of command line.
  return 0;
}

void
print_stats (ACE_Profile_Timer::ACE_Elapsed_Time &elapsed_time,
             int iterations)
{
  if (iterations > 0)
    {
      elapsed_time.real_time *= ACE_ONE_SECOND_IN_MSECS;
      elapsed_time.user_time *= ACE_ONE_SECOND_IN_MSECS;
      elapsed_time.system_time *= ACE_ONE_SECOND_IN_MSECS;

      elapsed_time.real_time /= iterations;
      elapsed_time.user_time /= iterations;
      elapsed_time.system_time /= iterations;

      double tmp = 1000 / elapsed_time.real_time;

      ACE_DEBUG ((LM_DEBUG,
                  "\titerations\t = %d,\n"
                  "\treal_time\t = %0.06f ms,\n"
                  "\tuser_time\t = %0.06f ms,\n"
                  "\tsystem_time\t = %0.06f ms,\n"
                  "\t%0.00f calls/second\n",
                  iterations,
                  elapsed_time.real_time   < 0.0 ? 0.0 : elapsed_time.real_time,
                  elapsed_time.user_time   < 0.0 ? 0.0 : elapsed_time.user_time,
                  elapsed_time.system_time < 0.0 ? 0.0 : elapsed_time.system_time,
                  tmp < 0.0 ? 0.0 : tmp));
    }
  else
    ACE_ERROR ((LM_ERROR,
                "\tNo time stats printed.  Zero iterations or error ocurred.\n"));
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{

  try
    {
      // Initialize the ORB
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      // Initialize options based on command-line arguments.
      int parse_args_result =
        parse_args (argc, argv);
      if (parse_args_result != 0)
        return parse_args_result;

      // Get an object reference from the argument string.
      CORBA::Object_var object =
        orb->string_to_object (IOR);

      // Try to narrow the object reference to a test reference.
      test_var test = test::_narrow (object.in ());

      CORBA::String_var ior =
        orb->object_to_string (test.in ());

      ACE_DEBUG ((LM_DEBUG,
                  "\nConnecting to: %C\n\n",
                  ior.in ()));

      ACE_Profile_Timer timer;
      ACE_Profile_Timer::ACE_Elapsed_Time elapsed_time;

      // We start an ACE_Profile_Timer here...
      timer.start ();

      int i = 0;

      for (i = 0; i < iterations; i++)
        {
          if (oneway && timed_method)
            {
              test->timed_oneway_method (timeout);
            }
          else if (oneway)
            {
              test->oneway_method ();
            }
          else if (!oneway && timed_method)
            {
              test->timed_method (timeout);
            }
          else
            {
              test->method ();
            }
        }

      // stop the timer.
      timer.stop ();
      timer.elapsed_time (elapsed_time);

      // compute average time.
      print_stats (elapsed_time, i);

      if (shutdown_server)
        {
          test->shutdown ();
        }

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Error!");
      return -1;
    }

  return 0;
}
