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

  ACE_Get_Opt get_opt (argc, argv, "n:l:h:s:");
  int opt;

  while ((opt = get_opt ()) != EOF)
    {
      switch (opt)
        {
	case 'n':
	  n = ACE_OS::atoi (get_opt.optarg);
	  break;
	case 'l':
	  low = ACE_OS::atoi (get_opt.optarg);
	  break;
	case 'h':
	  hi = ACE_OS::atoi (get_opt.optarg);
	  break;
	case 's':
	  s = ACE_OS::atoi (get_opt.optarg);
	  break;
        case '?':
        default:
          ACE_DEBUG ((LM_DEBUG,
                      "Usage: %s "
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

      for (int i = 0; i < n; ++i)
	{
	  writing.start ();
	  TAO_OutputCDR output;

	  if (test_write (output, x) != 0)
	    {
	      return 1;
	    }
	  writing.stop();

	  reading.start ();
	  TAO_InputCDR input (output);
	  if (test_read (input, x) != 0)
	    {
	      return 1;
	    }
	  reading.stop ();
	}
      double m = n * x;
      ACE_hrtime_t wusecs;
      writing.elapsed_microseconds (wusecs);
      ACE_hrtime_t rusecs;
      reading.elapsed_microseconds (rusecs);

      double write_average = wusecs / m;
      double read_average = rusecs / m;
      ACE_OS::printf ("AVE: %d %f %f\n",
		      x, write_average, read_average);
    }
  return 0;
}
