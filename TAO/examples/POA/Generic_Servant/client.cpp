// $Id$

// ================================================================
//
//
// = FILENAME
//     client.cpp
//
// = DESCRIPTION
//     This is a simple foo client implementation.
//
// = AUTHOR
//     Irfan Pyarali
//
// ================================================================

#include "ace/streams.h"
#include "ace/Get_Opt.h"
#include "ace/Profile_Timer.h"
#include "tao/Timeprobe.h"
#include "ace/Read_Buffer.h"
#include "FooC.h"

ACE_RCSID(Generic_Servant, client, "$Id$")

static char *IOR = 0;
static char *IOR_file = 0;
static int iterations = 1;
static int oneway = 0;
static int shutdown_server = 0;
static CORBA::ULong timeout = 5;
static int timed_operations = 0;

static int
parse_args (int argc, char **argv)
{
  ACE_Get_Opt get_opts (argc, argv, "f:k:i:T:otx");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        IOR = ACE_OS::strdup (get_opts.optarg);
        break;

      case 'f':
        IOR_file = get_opts.optarg;
        break;

      case 'o':
        oneway = 1;
        break;

      case 't':
        timed_operations = 1;
        break;

      case 'i':
        iterations = ::atoi (get_opts.optarg);
        break;

      case 'T':
        timeout = ACE_static_cast (CORBA::ULong, ::atoi (get_opts.optarg));
        break;

      case 'x':
	shutdown_server = 1;
	break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k IOR "
                           "-f IOR file "
                           "-o oneway "
                           "-t timed operations "
                           "-T timeout for timed operations "
                           "\n",
                           argv [0]),
                          -1);
      }

  if (IOR == 0 && IOR_file == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Please specify the IOR or IOR_file for the servant\n"), -1);

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
		  "\titerations\t = %d, \n"
		  "\treal_time\t = %0.06f ms, \n"
		  "\tuser_time\t = %0.06f ms, \n"
		  "\tsystem_time\t = %0.06f ms, \n"
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
read_IOR_from_file (void)
{
  // Open the file for reading.
  ACE_HANDLE f_handle = ACE_OS::open (IOR_file, 0);

  if (f_handle == ACE_INVALID_HANDLE)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Unable to open %s for reading\n",
                       IOR_file),
                      -1);

  ACE_Read_Buffer ior_buffer (f_handle);
  char *data = ior_buffer.read ();

  if (data == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Unable to read ior\n"),
                      -1);

  IOR = ACE_OS::strdup (data);
  ior_buffer.alloc ()->free (data);

  ACE_OS::close (f_handle);

  return 0;
}

int
main (int argc, char **argv)
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      // Initialize the ORB
      CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                            argv,
                                            0,
                                            ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Initialize options based on command-line arguments.
      int parse_args_result = parse_args (argc, argv);
      if (parse_args_result != 0)
        return parse_args_result;

      if (IOR == 0)
        {
          int result = read_IOR_from_file ();
          if (result != 0)
            ACE_ERROR_RETURN ((LM_ERROR, "Cannot read IOR from %s\n", IOR_file), -1);
        }

      // Get an object reference from the argument string.
      CORBA::Object_var object = orb->string_to_object (IOR,
                                                        ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Try to narrow the object reference to a Foo reference.
      Foo_var foo = Foo::_narrow (object.in (),
                                  ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CORBA::String_var ior =
        orb->object_to_string (foo.in (),
                               ACE_TRY_ENV);
      ACE_TRY_CHECK;


      ACE_DEBUG ((LM_DEBUG,
                  "\nConnecting to: %s\n\n",
                  ior.in ()));

      ACE_Profile_Timer timer;
      ACE_Profile_Timer::ACE_Elapsed_Time elapsed_time;

      // We start an ACE_Profile_Timer here...
      timer.start ();

      CORBA::Long result = 0;
      int i = 0;

      for (i = 0; i < iterations; i++)
        {
          if (oneway)
            {
              // Invoke the simply_doit() method of the foo reference.
              foo->simply_doit (ACE_TRY_ENV);
              ACE_TRY_CHECK;
            }
          else if (timed_operations)
            {
              // Invoke the timed_operation() method of the foo reference.
              foo->timed_operation (timeout,
                                    ACE_TRY_ENV);
              ACE_TRY_CHECK;
            }
          else
            {
              // Invoke the doit() method of the foo reference.
              result = foo->doit (ACE_TRY_ENV);
              ACE_TRY_CHECK;
            }
        }

      // stop the timer.
      timer.stop ();
      timer.elapsed_time (elapsed_time);

      // compute average time.
      print_stats (elapsed_time, i);

      if (shutdown_server)
        {
          foo->shutdown (ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }

      // Print the result of doit () method of the foo reference.
      ACE_DEBUG ((LM_DEBUG, "The result of doit is %d\n", result));

      ACE_TIMEPROBE_PRINT;

      ACE_OS::free (IOR);
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"Error!");
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  return 0;
}
