
//=============================================================================
/**
 *  @file     client.cpp
 *
 *   This is a simple client implementation.
 *
 *  @author  Irfan Pyarali
 */
//=============================================================================
#include "ace/streams.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdlib.h"
#include "ace/Profile_Timer.h"
#include "testC.h"
#include <array>

struct Options
{
  std::array<ACE_TCHAR const *, 6> iors {};
  int iterations {1};
};

static int
parse_args (int argc, ACE_TCHAR **argv, Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("a:b:c:d:e:f:i:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'a':
        options.iors[0] = get_opts.opt_arg ();
        break;
      case 'b':
        options.iors[1] = get_opts.opt_arg ();
        break;
      case 'c':
        options.iors[2] = get_opts.opt_arg ();
        break;
      case 'd':
        options.iors[3] = get_opts.opt_arg ();
        break;
      case 'e':
        options.iors[4] = get_opts.opt_arg ();
        break;
      case 'f':
        options.iors[5] = get_opts.opt_arg ();
        break;
      case 'i':
        options.iterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-a IOR 1 "
                           "-b IOR 2 "
                           "-c IOR 3 "
                           "-d IOR 4 "
                           "-e IOR 5 "
                           "-f IOR 6 "
                           "\n",
                           argv [0]),
                          -1);
      }

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

      double const tmp = 1000 / elapsed_time.real_time;

      ACE_DEBUG ((LM_DEBUG,
                  "\treal_time\t = %0.06f ms,\n"
                  "\tuser_time\t = %0.06f ms,\n"
                  "\tsystem_time\t = %0.06f ms,\n"
                  "\t%0.00f calls/second\n",
                  elapsed_time.real_time   < 0.0 ? 0.0 : elapsed_time.real_time,
                  elapsed_time.user_time   < 0.0 ? 0.0 : elapsed_time.user_time,
                  elapsed_time.system_time < 0.0 ? 0.0 : elapsed_time.system_time,
                  tmp < 0.0 ? 0.0 : tmp));
    }
  else
    ACE_ERROR ((LM_ERROR,
                "\tNo time stats printed.  Zero iterations or error occurred.\n"));
}

template <class T, class T_var>
class Test
{
public:
  static void run (CORBA::ORB_ptr orb,
                   ACE_TCHAR const *ior,
                   int iterations)
  {
    if (ior != nullptr)
      {
        // Get an object reference from the argument string.
        CORBA::Object_var const object = orb->string_to_object (ior);

        // Try to narrow the object reference to a reference.
        T_var const test = T::_narrow (object.in ());

        ACE_Profile_Timer timer;
        ACE_Profile_Timer::ACE_Elapsed_Time elapsed_time {};

        // We start an ACE_Profile_Timer here...
        timer.start ();

        CORBA::Long result {};
        int i {};
        for (; i < iterations; ++i)
          {
            // Invoke the doit() method on the reference.
            result = test->doit ();
          }

        // stop the timer.
        timer.stop ();
        timer.elapsed_time (elapsed_time);

        // compute average time.
        print_stats (elapsed_time, i);

        // Print the result of doit () method on the reference.
        ACE_DEBUG ((LM_DEBUG,
                    "%d\n",
                    result));
      }
  }
};

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      // Initialize the ORB.
      Options options;
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

      // Initialize options based on command-line arguments.
      int const parse_args_result = parse_args (argc, argv, options);
      if (parse_args_result != 0)
        return parse_args_result;

      Test<A, A_var>::run (orb.in (), options.iors[0], options.iterations);
      Test<Outer::B, Outer::B_var>::run (orb.in (), options.iors[1], options.iterations);
      Test<Outer::Inner::C, Outer::Inner::C_var>::run (orb.in (), options.iors[2], options.iterations);
      Test<A, A_var>::run (orb.in (), options.iors[3], options.iterations);
      Test<Outer::B, Outer::B_var>::run (orb.in (), options.iors[4], options.iterations);
      Test<Outer::Inner::C, Outer::Inner::C_var>::run (orb.in (), options.iors[5], options.iterations);

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception Caught in main");
      return -1;
    }
  return 0;
}

