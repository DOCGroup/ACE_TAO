// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/CDR
//
// = FILENAME
//    growth.cpp
//
// = DESCRIPTION
//   Helps in measuring how the growth strategy affects the
//   performance of CDR streams.
//
// = AUTHORS
//    Carlos O'Ryan
//
// ============================================================================

#include "ace/Get_Opt.h"
#include "ace/High_Res_Timer.h"

#include "tao/corba.h"
#include "tao/debug.h"

ACE_RCSID(CDR, growth, "$Id$")

static int
test_write (TAO_OutputCDR &cdr, int n)
{
  CORBA::Long l = 0xdeadbeef;

  for (int i = 0; i < n; ++i)
    {
      if (cdr.write_long (l) == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "write_long[%d] failed\n",
                           i),
                          1);
    }

  return 0;
}

static int
test_read (TAO_InputCDR &cdr, int n)
{
  CORBA::Long xl;

  for (int i = 0; i < n; ++i)
    {
      if (cdr.read_long (xl) == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "read_long[%d] failed\n",
                           i),
                          1);
    }

  return 0;
}

int
main (int argc, char *argv[])
{
  int n = 100;
  int low = 64;
  int hi = 4096;
  int s = 4;
  int quiet = 0;

  ACE_Get_Opt get_opt (argc, argv, "dn:l:h:s:q");
  int opt;

  while ((opt = get_opt ()) != EOF)
    {
      switch (opt)
        {
	case 'd':
	  TAO_debug_level++;
	  break;
        case 'n':
          n = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        case 'l':
          low = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        case 'h':
          hi = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        case 's':
          s = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        case 'q':
          quiet = 1;
          break;
        case '?':
        default:
          ACE_DEBUG ((LM_DEBUG,
                      "Usage: %s "
		      "-d debug"
                      "-l low "
                      "-h high "
                      "-s step "
                      "-n n "
                      "\n"
                      "Writes and then reads longs to a CDR stream "
                      "starting from <low> up to <high> incrementing "
                      "by <step>, at each step run <n> iterations to "
                      "average."
                      "\n",
                      argv[0]));
          return -1;
        }
    }

  for (int x = low; x <= hi; x += s)
    {
      ACE_High_Res_Timer writing;
      ACE_High_Res_Timer reading;
      if (TAO_debug_level > 0)
	ACE_DEBUG ((LM_DEBUG, "\nx= %d\n", x));

      for (int i = 0; i < n; ++i)
        {
          writing.start_incr ();
          TAO_OutputCDR output;

          if (test_write (output, x) != 0)
            {
              return 1;
            }
          writing.stop_incr ();

          reading.start_incr ();
          TAO_InputCDR input (output);
          if (test_read (input, x) != 0)
            {
              return 1;
            }
          reading.stop_incr ();
        }
      double m = n * x;

      ACE_Time_Value wtv;
      writing.elapsed_time_incr (wtv);
      ACE_hrtime_t wusecs = wtv.sec ();
      wusecs *= ACE_static_cast (ACE_UINT32, ACE_ONE_SECOND_IN_USECS);
      wusecs += wtv.usec ();

      ACE_Time_Value rtv;
      reading.elapsed_time_incr (rtv);
      ACE_hrtime_t rusecs = rtv.sec ();
      rusecs *= ACE_static_cast (ACE_UINT32, ACE_ONE_SECOND_IN_USECS);
      rusecs += rtv.usec ();

      double write_average = ACE_U64_TO_U32(wusecs) / m;
      double read_average = ACE_U64_TO_U32(rusecs) / m;
      if (!quiet)
        ACE_OS::printf ("AVE: %d %f %f\n",
                        x, write_average, read_average);
    }
  return 0;
}
