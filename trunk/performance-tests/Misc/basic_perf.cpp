// $Id$

// ============================================================================
//
// = LIBRARY
//    performance-tests/Misc
// 
// = FILENAME
//    basic_perf.cpp
//
// = DESCRIPTION
//    Times various simple operations.
//
//    With Sun C++, use -O2: make CFLAGS="-mt -O2" BIN=perf
//
// = AUTHOR
//    David Levine
// 
// ============================================================================

#include "basic_func.h"
#include "ace/High_Res_Timer.h"
#include "ace/Get_Opt.h"
#include "ace/OS.h"

static const char usage [] = "[-? |\n"
                             "            [-i <iterations> [1000000]]";
static u_int iterations = 1000000;

inline
void
inline_func ()
{
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// function per_iteration
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Given an elapsed time in nanoseconds, returns the time per iteration
// in microseconds.
static
inline
float
per_iteration (const ACE_hrtime_t elapsed /* nanoseconds */)
{
  float ms_per_iteration = elapsed / 1000.0 / iterations;

  // Don't print out "-0.000" or "-0.001" . . .
  return -0.001 < ms_per_iteration  &&  ms_per_iteration < 0
           ?  0.0
           :  ms_per_iteration;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// function get_options
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static
unsigned int
get_options (int argc, char *argv [])
{
  if (argv == 0)
    {
      // VxWorks doesn't necessarily have a non-zero argv.
      return 0;
    }

  ACE_Get_Opt get_opt (argc, argv, "i:?");
  int opt;
  int temp;

  while ((opt = get_opt ()) != EOF)
  {
    switch (opt) {
    case 'i':
      if ((temp = ACE_OS::atoi (get_opt.optarg)) > 0)
        {
          iterations = (u_int) temp;
        }
      else
        {
          ACE_DEBUG ((LM_ERROR, "%s: number of iterations must be > 0\n",
                      argv [0]));
          return 1;
        }
      break;
    case '?':
      ACE_DEBUG ((LM_INFO, "usage: %s %s\n", argv [0], usage));
      ACE_OS::exit (0);
      break;
    default:
      ACE_DEBUG ((LM_ERROR, "%s: unknown arg, %c\n", argv [0], (char) opt));
      ACE_DEBUG ((LM_ERROR, "usage: %s %s\n", argv [0], usage));
      return 1;
    }
  }

  switch (argc - get_opt.optind) {
  case 0:
    // OK
    break;
  default:
    ACE_DEBUG ((LM_ERROR, "%s: too many arguments\n", argv [0]));
    ACE_DEBUG ((LM_ERROR, "usage: %s %s\n", argv [0], usage));
    return 1;
  }

  return 0;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// function main
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
int
main (int argc, char *argv[])
{
  if (get_options (argc, argv)) ACE_OS::exit (-1);

  struct utsname name;
  if (ACE_OS::uname (&name) != -1)
    {
      ACE_DEBUG ((LM_INFO, "%s (%s), %s %s at %T\n",
                  name.nodename, name.machine, name.sysname, name.release));
    }
  ACE_DEBUG ((LM_INFO, "%u iterations\n", iterations));


  ACE_High_Res_Timer timer;
  ACE_hrtime_t iteration_time,
               inline_call_time,
               noninline_call_time,
               inline_member_time,
               noninline_member_time,
               inline_member_v_time,
               noninline_member_v_time,
               virtual_member_v_time;
  u_int i;

  ACE_hrtime_t start, stop;

  timer.reset ();
  timer.start ();
  start = ACE_OS::gethrtime ();
  for (i = 0; i < iterations; ++i)
    {
    }
  stop = ACE_OS::gethrtime ();
  timer.stop ();
  timer.elapsed_time (iteration_time);
  ACE_DEBUG ((LM_INFO, "An empty iteration costs %8.3f microseconds.\n\n",
              per_iteration (iteration_time)));

  timer.reset ();
  timer.start ();
  for (i = 0; i < iterations; ++i)
    {
      inline_func ();
    }
  timer.stop ();
  timer.elapsed_time (inline_call_time);

  timer.reset ();
  timer.start ();
  for (i = 0; i < iterations; ++i)
    {
      func ();
    }
  timer.stop ();
  timer.elapsed_time (noninline_call_time);

  Foo foo;
  timer.reset ();
  timer.start ();
  for (i = 0; i < iterations; ++i)
    {
      foo.inline_func ();
    }
  timer.stop ();
  timer.elapsed_time (inline_member_time);

  timer.reset ();
  timer.start ();
  for (i = 0; i < iterations; ++i)
    {
      foo.func ();
    }
  timer.stop ();
  timer.elapsed_time (noninline_member_time);

  Foo_v foo_v;
  timer.reset ();
  timer.start ();
  for (i = 0; i < iterations; ++i)
    {
      foo_v.inline_func ();
    }
  timer.stop ();
  timer.elapsed_time (inline_member_v_time);

  timer.reset ();
  timer.start ();
  for (i = 0; i < iterations; ++i)
    {
      foo_v.func ();
    }
  timer.stop ();
  timer.elapsed_time (noninline_member_v_time);

  Foo_v &fv = foo_v;
  timer.reset ();
  timer.start ();
  for (i = 0; i < iterations; ++i)
    {
      fv.v_func ();
    }
  timer.stop ();
  timer.elapsed_time (virtual_member_v_time);

  ACE_DEBUG ((LM_INFO, "operation                              "
                  "time, microseconds\n"));
  ACE_DEBUG ((LM_INFO, "=========                              "
                  "=================="));

  ACE_DEBUG ((LM_INFO, "\nglobal function calls:\n"));
  ACE_DEBUG ((LM_INFO, "  inline function call                      %8.3f\n",
                  per_iteration (inline_call_time - iteration_time)));
  ACE_DEBUG ((LM_INFO, "  non-inline function call                  %8.3f\n",
                  per_iteration (noninline_call_time - iteration_time)));

  ACE_DEBUG ((LM_INFO, "\nmember function calls:\n"));
  ACE_DEBUG ((LM_INFO, "  inline member function call               %8.3f\n",
                  per_iteration (inline_member_time - iteration_time)));
  ACE_DEBUG ((LM_INFO, "  non-inline member function call           %8.3f\n",
                  per_iteration (noninline_member_time - iteration_time)));

  ACE_DEBUG ((LM_INFO, "\nmember function calls, class has a virtual "
                       "function:\n"));
  ACE_DEBUG ((LM_INFO, "  inline member function with/virtual call  %8.3f\n",
                  per_iteration (inline_member_v_time - iteration_time)));
  ACE_DEBUG ((LM_INFO, "  noninline member function w/virtual call  %8.3f\n",
                  per_iteration (noninline_member_v_time - iteration_time)));

  ACE_DEBUG ((LM_INFO, "\nvirtual member function calls:\n"));
  ACE_DEBUG ((LM_INFO, "  virtual member function call              %8.3f\n",
                  per_iteration (virtual_member_v_time - iteration_time)));

  return 0;
}


// EOF
