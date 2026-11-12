// -*- C++ -*-

//=============================================================================
/**
 *  @file    OctetSeq.cpp
 *
 * TAO optimizes octet sequences, this test verifies that the
 * optimizations do not break any code and that they effectively
 * improve performance.
 *
 *  @author Carlos O'Ryan
 */
//=============================================================================

#include "ace/Get_Opt.h"
#include "ace/High_Res_Timer.h"

#include "tao/ORB.h"
#include "tao/CDR.h"
#include "testC.h"

#include <cstddef>
#include <cstring>
#include <memory>

struct Options
{
  int iterations {64};
  int low {128};
  int high {65536};
  int step {32};
  bool quiet {};
};

static int
test_write_octet (TAO_OutputCDR &cdr,
                  char * /* buf */,
                  size_t bufsize)
{
#if (TAO_NO_COPY_OCTET_SEQUENCES == 1)
  ACE_Message_Block mb (bufsize);
  mb.wr_ptr (bufsize);
  Test::OctetSeq os (bufsize, &mb);
#else
  Test::OctetSeq os (bufsize);
  os.length (bufsize);
#endif /* TAO_NO_COPY_OCTET_SEQUENCES == 1 */

  if (!(cdr << os))
    return -1;
  return 0;
}

static int
test_read_octet (TAO_InputCDR &cdr,
                 char * /* buf */,
                 size_t /* bufsize */)
{
  Test::OctetSeq os;

  if (!(cdr >> os))
    return -1;
  return 0;
}

static int
test_write_char (TAO_OutputCDR &cdr,
                 char *buf,
                 size_t bufsize)
{
  Test::CharSeq cs (bufsize, bufsize, buf);

  if (!(cdr << cs))
    return -1;
  return 0;
}

static int
test_read_char (TAO_InputCDR &cdr,
                char * /* buf */,
                size_t /* bufsize */)
{
  Test::CharSeq cs;

  if (!(cdr >> cs))
    return -1;
  return 0;
}

using Writer = int (*) (TAO_OutputCDR &cdr, char *buf, size_t bufsize);
using Reader = int (*) (TAO_InputCDR &cdr, char *buf, size_t bufsize);

int
run (char *buf, size_t bufsize,
     size_t n, size_t lo, size_t s,
     bool quiet,
     ACE_TCHAR const *name,
     Writer writer, Reader reader)
{
  size_t count {};
  double sum_read {};
  double sum_write {};

  for (size_t x = lo; x <= bufsize; x += s)
    {
      ACE_High_Res_Timer writing;
      ACE_High_Res_Timer reading;

      for (size_t i = 0; i < n; ++i)
        {
          writing.start_incr ();
          TAO_OutputCDR output;

          if (writer (output, buf, x) != 0)
            return -1;
          writing.stop_incr ();

          TAO_InputCDR input (output);
          reading.start_incr ();
          if (reader (input, buf, x) != 0)
            return -1;
          reading.stop_incr ();
        }

      double const m = n;

      ACE_Time_Value wtv;
      writing.elapsed_time_incr (wtv);
      ACE_hrtime_t wusecs = wtv.sec ();
      wusecs *= static_cast<ACE_UINT32> (ACE_ONE_SECOND_IN_USECS);
      wusecs += wtv.usec ();

      ACE_Time_Value rtv;
      reading.elapsed_time_incr (rtv);
      ACE_hrtime_t rusecs = rtv.sec ();
      rusecs *= static_cast<ACE_UINT32> (ACE_ONE_SECOND_IN_USECS);
      rusecs += rtv.usec ();

      double const write_average = ACE_HRTIME_CONVERSION(wusecs) / m;
      double const read_average = ACE_HRTIME_CONVERSION(rusecs) / m;

      ++count;
      sum_read += read_average;
      sum_write += write_average;
      if (!quiet)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "%s: %B %.3f %.3f\n",
                      name, x, write_average, read_average));
        }
    }

  if (count != 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%s total: %.3f %.3f\n",
                  name, sum_write / count, sum_read / count));
    }
  return 0;
}

static int
parse_args (int argc, ACE_TCHAR *argv[], Options &options)
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT("qn:l:h:s:"));
  int opt {};

  while ((opt = get_opt ()) != EOF)
    {
      switch (opt)
        {
        case 'n':
          options.iterations = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        case 'l':
          options.low = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        case 'h':
          options.high = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        case 's':
          options.step = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        case 'q':
          options.quiet = true;
          break;

        case '?':
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Usage: %s "
                             "-l low "
                             "-h high "
                             "-s step "
                             "-n n "
                             "-q\n"
                             "Writes and then reads octet sequences to a CDR stream "
                             "starting from <low> up to <high> incrementing "
                             "by <step>, at each step run <n> iterations to "
                             "average.\n",
                             argv[0]),
                            -1);
        }
    }

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv);

      Options options {};
      if (parse_args (argc, argv, options) != 0)
        return 1;

      ACE_DEBUG ((LM_DEBUG, "Running:\n"
                  "  low: %d\n"
                  "  hi : %d\n"
                  "  s  : %d\n"
                  "  n  : %d\n",
                  options.low, options.high, options.step, options.iterations));

      // Create a "big" buffer and fill it up.
      std::unique_ptr<char[]> const buf =
        std::make_unique<char[]> (options.high);
      CORBA::Long const l = 0xdeadbeef;
      for (int i = 0; i < options.high / static_cast<int> (sizeof (l)); ++i)
        {
          std::memcpy (buf.get () + sizeof (l) * i, &l, sizeof (l));
        }

      if (run (buf.get (), options.high,
               options.iterations, options.low, options.step, options.quiet,
               ACE_TEXT ("OCTET"), test_write_octet, test_read_octet) != 0)
        return 1;

      if (run (buf.get (), options.high,
               options.iterations, options.low, options.step, options.quiet,
               ACE_TEXT ("CHAR"), test_write_char, test_read_char) != 0)
        return 1;

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("OctetSeq");
      return 1;
    }

  return 0;
}
