// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/OctetSeq
//
// = FILENAME
//    octetseq.cpp
//
// = DESCRIPTION
//   TAO optimizes octet sequences, this test verifies that the
//   optimizations do not break any code and that they effectively
//   improve performance.
//
// = AUTHORS
//    Carlos O'Ryan
//
// ============================================================================

#include "ace/Get_Opt.h"
#include "ace/High_Res_Timer.h"

#include "tao/corba.h"
#include "tao/debug.h"
#include "testC.h"

ACE_RCSID(OctetSeq, OctetSeq, "$Id$")

static int
test_write_octet (TAO_OutputCDR &cdr,
                  char* buf, size_t bufsize,
                  CORBA::Environment& env)
{
  Test::OctetSeq os (bufsize, bufsize, (CORBA::Octet*)buf);

  cdr.encode (Test::_tc_OctetSeq, &os, 0, env);
  if (env.exception () != 0)
    return -1;
  return 0;
}

static int
test_read_octet (TAO_InputCDR &cdr,
                 char* /* buf */, 
                 size_t bufsize,
                 CORBA::Environment& env)
{
  Test::OctetSeq os;

  cdr.decode (Test::_tc_OctetSeq, &os, 0, env);
  if (env.exception () != 0 ||
      os.length () != bufsize
      /* || ACE_OS::memcmp( buf, os.get_buffer (), bufsize) != 0 */)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "OCTET: problem demarshalling or unmatched buffer\n"));
      return -1;
    }
  return 0;
}

static int
test_write_char (TAO_OutputCDR &cdr,
                 char* buf, 
                 size_t bufsize,
                 CORBA::Environment& env)
{
  Test::CharSeq cs (bufsize, bufsize, buf);

  cdr.encode (Test::_tc_CharSeq, &cs, 0, env);
  if (env.exception () != 0)
    return -1;
  return 0;
}

static int
test_read_char (TAO_InputCDR &cdr,
                char* /* buf */, 
                size_t bufsize,
                CORBA::Environment& env)
{
  Test::CharSeq cs;

  cdr.decode (Test::_tc_CharSeq, &cs, 0, env);
  if (env.exception () != 0 ||
      cs.length () != bufsize
      /* || ACE_OS::memcmp( buf, cs.get_buffer (), bufsize) != 0 */)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "CHAR: problem demarshalling or unmatched buffer\n"));
      return -1;
    }
  return 0;
}

typedef
int (*Writer)(TAO_OutputCDR& cdr,
              char* buf, size_t bufsize,
              CORBA::Environment& env);
typedef
int (*Reader)(TAO_InputCDR& cdr,
              char* buf, size_t bufsize,
              CORBA::Environment& env);

int
run (char* buf, size_t bufsize,
     size_t n, size_t lo, size_t s,
     const char* name,
     Writer writer, Reader reader,
     CORBA::Environment& env)
{
  for (size_t x = lo; x <= bufsize; x += s)
    {
      ACE_High_Res_Timer writing;
      ACE_High_Res_Timer reading;

      for (size_t i = 0; i < n; ++i)
        {
          writing.start_incr ();
          TAO_OutputCDR output;

          if (writer (output, buf, x, env) != 0)
            return -1;
          writing.stop_incr ();

          reading.start_incr ();
          TAO_InputCDR input (output);
          if (reader (input, buf, x, env) != 0)
            return -1;
          reading.stop_incr ();
        }
      double m = n;

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
      ACE_DEBUG ((LM_DEBUG,
                  "%s: %d %.3f %.3f\n",
                  name, x, write_average, read_average));
    }
  return 0;
}

int
main (int argc, char *argv[])
{
  TAO_TRY
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                            argv,
                                            0,
                                            TAO_TRY_ENV);
      TAO_CHECK_ENV;

      int n = 64;
      int lo = 64;
      int hi = 128000;
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
              lo = ACE_OS::atoi (get_opt.optarg);
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
                          "Writes and then reads octet sequences to a CDR stream "
                          "starting from <low> up to <high> incrementing "
                          "by <step>, at each step run <n> iterations to "
                          "average."
                          "\n",
                          argv[0]));
              return -1;
            }
        }

      ACE_DEBUG ((LM_DEBUG, "Running:\n"
                  "  low: %d\n"
                  "  hi : %d\n"
                  "  s  : %d\n"
                  "  n  : %d\n",
                  lo, hi, s, n));

      // Create a "big" buffer and fill it up.
      char* buf = new char[hi];
      CORBA::Long l = 0xdeadbeef;
      for (int i = 0; i < hi / (int) sizeof (l); ++i)
        {
          ACE_OS::memcpy (buf + sizeof (l) * i, &l, sizeof (l));
        }

      if (run (buf, hi,
               n, lo, s,
               "OCTET", test_write_octet, test_read_octet,
               TAO_TRY_ENV) != 0)
        return 1;
      TAO_CHECK_ENV;

      if (run (buf, hi,
               n, lo, s,
               "CHAR", test_write_char, test_read_char,
               TAO_TRY_ENV) != 0)
        return 1;
      TAO_CHECK_ENV;
      delete[] buf;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("OctetSeq");
    }
  TAO_ENDTRY;

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class TAO_Unbounded_Sequence<CORBA::Char>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate TAO_Unbounded_Sequence<CORBA::Char>
#endif  /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
