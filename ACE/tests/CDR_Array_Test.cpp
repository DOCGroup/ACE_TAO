
//=============================================================================
/**
 *  @file    CDR_Array_Test.cpp
 *
 *  $Id$
 *
 *  Checks ACE_OutputCDR::write_XX_array.
 *  Checks ACE_InputCDR::read_XX_array.
 *  Checks operator<< and operator>> for CDR Streams in
 *  each of the basic CDR types.
 *  Gives a measure of the speed of the ACE CDR streams wrt those
 *  operations.
 *
 *
 *  @author Cristian Ferretti <cristian_ferretti@yahoo.com>
 */
//=============================================================================


// For measuring time, choose your method:
// Define:
//
// * USE_GETRUSAGE for using ACE_OS::getrusage.
//     Ticks only when process is running.
//
// * USE_CLOCK for using clock(2).
//     You're on your own, no ACE_OS:: support.
//     Ticks only when process is running.
//
// * None of the above for using ACE_High_Res_Timer.
//     Ticks independent of running state of process.

// #define USE_CLOCK
// #define USE_GETRUSAGE

#if defined(USE_CLOCK)
#include <time.h>
#endif

#include "test_config.h"
#include "ace/OS_Memory.h"
#include "ace/OS_NS_stdlib.h"
#include "ace/Get_Opt.h"
#include "ace/CDR_Stream.h"
#include "ace/High_Res_Timer.h"
#include "ace/ACE.h"

#if defined(USE_GETRUSAGE) && !defined(ACE_HAS_GETRUSAGE)
#error "Can't define USE_GETRUSAGE on this platform."
#endif



// Default number of elements for check buffer, for each tested CDR type.
// Be aware that time will be affected by the buffer fitting/not fitting
// in the cache (ie, if default_total*sizeof(T) bytes fit in the cache).
// Also, you want that your time measuring method has a resolution
// compatible with this buffer size, if not you will end up measuring 0.
// You can change this value with -t option.
static const int default_total = 32*1024;

// Repeat this many times for each tested CDR type.
// We then take the average time that took for each type and report that.
// You can change this value with -n option.
static const int default_niter = 10;

//
// A simple cronometer in seconds, that encapsulates our time
// measuring method.
//
class Crono {
public:
  Crono() {}
  ~Crono() {}
  void start ()
    {
#if defined(USE_CLOCK)
      start_ = clock ();
#elif defined(USE_GETRUSAGE)
      ACE_OS::getrusage (RUSAGE_SELF, &start_);
#else
      timer.start ();
#endif
    }
  void stop ()
    {
#if defined(USE_CLOCK)
      end_ = clock ();
#elif defined(USE_GETRUSAGE)
      ACE_OS::getrusage (RUSAGE_SELF, &end_);
#else
      timer.stop ();
#endif
    }
  double read_seconds ()
    {
#if defined(USE_CLOCK)
      return (end_ - start_) / (double) CLOCKS_PER_SEC;
#elif defined(USE_GETRUSAGE)
      timeval diff;
      diff.tv_sec = end_.ru_utime.tv_sec - start_.ru_utime.tv_sec;
      diff.tv_usec = end_.ru_utime.tv_usec - start_.ru_utime.tv_usec;
      while (diff.tv_usec < 0)
        {
          --diff.tv_sec;
          diff.tv_usec += ACE_ONE_SECOND_IN_USECS;
        }

      return diff.tv_sec + diff.tv_usec / double(ACE_ONE_SECOND_IN_USECS);
#else
      ACE_Time_Value tv;
      timer.elapsed_time(tv);
      return tv.usec () / 1000000.0;
#endif
    }
private:
#if defined(USE_CLOCK)
  clock_t start_;
  clock_t end_;
#elif defined(USE_GETRUSAGE)
  ACE_Rusage start_;
  ACE_Rusage end_;
#else
  ACE_High_Res_Timer timer;
#endif
};

//
// Our test, performed in the constructor.
// T is one of the CDR types.
// H is a helper class (described later).
//
// All this stuff is in a class and not in template functions
// to avoid having to deal with potential template function
// instantiations problems.
//
template<class T, class H> class CDR_Test
{
public:
  CDR_Test (int total, int niter, int use_array);
  static void do_test (int total, int niter, int use_array,
                       char* srcbuf, char* dstbuf,
                       int src_offset = 0, int dst_offset = 0);
  ~CDR_Test ();

  static void ttoh (const T& t, char* s);
  static T checkval(int i);

private:
  CDR_Test (const CDR_Test<T, H>&);
  CDR_Test<T, H>& operator= (const CDR_Test<T, H>&);
};

static ACE_UINT32 seal = 0xdeadbeef;

void
zero (char* p, size_t k)
{
  char* end = p + k;
  while (p < end)
    {
      *p++ = '\0';
    }
}

inline int
mymax (int a, int b)
{
  return (a >= b) ? a : b;
}

void
memabort ()
{
  ACE_ERROR((LM_ERROR,
             ACE_TEXT ("new failed, aborting\n")));
  ACE_OS::exit (1);
}

template<class T, class H>
CDR_Test<T, H>::CDR_Test (int total, int niter, int use_array)
{
  if (total <= 0)
    {
      return ;
    }

  char* srcbuf;
  char* dstbuf;
  {
    const size_t stotal =
      (total + 10) * H::size () + sizeof(ACE_UINT32) + ACE_CDR::MAX_ALIGNMENT;

    ACE_NEW(srcbuf, char[stotal]);
    if (srcbuf == 0)
      {
        memabort ();
      }
    zero(srcbuf, stotal);

    ACE_NEW(dstbuf, char[stotal]);
    if (dstbuf == 0)
      {
        memabort ();
      }
    zero(dstbuf, stotal);
  }

  if (use_array)
    {
      // We want to test all the possible loop unrolling deltas.
      int t;
      for (t = total - 3; t <= total; t++)
        {
          int delta;
          if (sizeof(long) <= H::size ())
            {
              delta = 1;
            }
          else
            {
              delta = (int) (sizeof(long) / H::size ());
            }

          // We want to test all the posible source/destination buffer
          // alignment combinations.
          int sk;
          for (sk = 0; sk < delta; sk++)
            {
              int dk;
              for (dk = 0; dk < delta; dk++)
                {
                  int tdelta = t - mymax(sk, dk);

                  CDR_Test<T, H>::do_test(tdelta, niter, 1,
                                          srcbuf, dstbuf,
                                          sk, dk);

                }
            }
        }
    }
  else
    {
      do_test(total, niter, use_array, srcbuf, dstbuf);
    }

  delete[] srcbuf;
  delete[] dstbuf;
}

// Generate an ``interesting'' value for testing at pos >i<.
template<class T, class H> T
CDR_Test<T, H>::checkval (int i)
{
  if (!H::integral ())
    {
      // If T is not an integral type, we don't want to risk
      // getting an invalid bit pattern for a T value.
      return T(i);
    }
  else
    {
      T v;
      unsigned char* s = reinterpret_cast<unsigned char*> ((&v));
      unsigned int j;
      for (j = 0; j < H::size (); j++)
        {
          s[j] = (unsigned char) ((j + i * H::size ()) % 256);
        }

      return v;
    }
}

//
// Returns in s an hex representation of T's memory.
// (differences in byte order will be noticed in s).
//
// If T = int,
//       t = 0xaabbccdd,
//    => s = "aabbccdd" for big endian machines,
//       s = "ddccbbaa" for little endian machines.
//
template<class T, class H> void
CDR_Test<T, H>::ttoh (const T& t, char* s)
{
  const unsigned char *const p =
    reinterpret_cast<const unsigned char*> (&t);

  static char digits[16] = {
    '0', '1', '2', '3',
    '4', '5', '6', '7',
    '8', '9', 'a', 'b',
    'c', 'd', 'e', 'f'
  };

  const unsigned char* q;
  for (q = p; q < p + H::size (); ++q)
    {
      int k = *q;
      *s++ = digits[ k >> 4 ];
      *s++ = digits[ k & 15 ];
    }

  *s = 0;
}

void
do_seal (char* pos)
{
  char* ps = reinterpret_cast<char*> (&seal);
  pos[0] = ps[0];
  pos[1] = ps[1];
  pos[2] = ps[2];
  pos[3] = ps[3];
}

int
check_seal (char* pos)
{
  char* ps = reinterpret_cast<char*> (&seal);
  return (pos[0] == ps[0]
          && pos[1] == ps[1]
          && pos[2] == ps[2]
          && pos[3] == ps[3]);
}

//
// returns the alignment of ptr, wrt ACE_CDR::MAX_ALIGNMENT.
//
int
tellalign (const char* const ptr)
{
  int align = ACE_CDR::MAX_ALIGNMENT;
  while (ptr != ACE_ptr_align_binary(ptr, align))
    {
      align = align >> 1;
    }

  return align;
}

template<class T, class H> void
CDR_Test<T, H>::do_test (int total, int niter, int use_array,
                         char* srcbuf, char* dstbuf,
                         int src_offset, int dst_offset)
{
  if (!use_array)
    {
      dst_offset = src_offset = 0;
    }

  ACE_DEBUG((LM_DEBUG,
             ACE_TEXT( "Starting Test for %s: %d elements " )
             ACE_TEXT( "%susing arrays.\n" ),
             H::name (),
             total,
             ((use_array) ? ACE_TEXT( "" ) : ACE_TEXT( "not " ))));


  if (!use_array && (total % 4) != 0)
    {
      int lasttotal = total;
      total -= (total % 4);
      ACE_DEBUG((LM_DEBUG,
                 ACE_TEXT( "Rounding from %d to %d elements.\n" ),
                 lasttotal,
                 total));
    }

  char* src = ACE_ptr_align_binary(srcbuf, H::size ());
  T* idata = reinterpret_cast<T*> (src);
  idata += src_offset;
  src = reinterpret_cast<char*> (idata);

  {
    int i;
    for (i = 0; i < total; i++)
      {
        idata[i] = CDR_Test<T, H>::checkval (i);
      }
  }

  ACE_DEBUG((LM_DEBUG,
             ACE_TEXT( "Writing data...\n" )));

  char* toread = 0;
  {
    ACE_TEST_ASSERT(use_array || total % 4 == 0);

    double totalsecs = 0.0;
    int n;
    for (n = 0; n < niter; n++)
      {
        size_t size = H::size () * (dst_offset + total) +
                      ACE_CDR::MAX_ALIGNMENT;
        ACE_OutputCDR os (dstbuf, size);

        // This is intrusive...
        char* const end = os.begin ()->wr_ptr() + size;

        do_seal (end);

        double secs = 0.0;
        if (use_array)
          {
            {
              int i;
              for (i = 0; i < dst_offset; i++)
                {
                  os << T(0);
                }
            }

            if (n == 0)
              {
                ACE_DEBUG((LM_DEBUG,
                           ACE_TEXT ("* src align = %d, dst align = %d\n"),
                           tellalign (src),
                           tellalign (os.begin ()->wr_ptr ())));
              }

            Crono crono;
            crono.start ();
            H::write_array (os, idata, total);
            crono.stop ();
            secs = crono.read_seconds ();
          }
        else
          {
            int i = 0;
            for (; i < dst_offset; i++)
              {
                os << T(0);
              }
            i = 0;

            Crono crono;
            crono.start();
            while (i < total)
              {
                os << idata[i++];
                os << idata[i++];
                os << idata[i++];
                os << idata[i++];
                // static char rs[32 + 1];
                // CDR_Test<T,H>::ttoh (idata[i], rs);
                // ACE_DEBUG ((LM_DEBUG, "Write idata[%d] = %s\n", i, rs));
                // os << idata[i];
                // i++;
              }
            crono.stop ();
            secs = crono.read_seconds ();
          }

        if (!check_seal(end))
          {
            ACE_ERROR((LM_ERROR,
                       ACE_TEXT( "Broken seal, aborting.\n" )));
            ACE_OS::exit(1);
          }

        totalsecs += secs;

        if (n == niter - 1)
          {
            toread = os.begin ()->rd_ptr ();
          }
      }

    totalsecs = totalsecs / niter;

    ACE_DEBUG((LM_DEBUG,
               ACE_TEXT ("Writing to stream %d %s values: %f seconds.\n"),
               total,
               H::name (),
               totalsecs));
  }

  {
    int i;
    for (i = 0; i < total; i++)
      {
        idata[i] = 0;
      }
  }

  ACE_DEBUG((LM_DEBUG,
             ACE_TEXT( "Reading them back in opposing byte order...\n" )));

  const int opposite_byte_order = 1 - ACE_CDR_BYTE_ORDER;

  {
    double totalsecs = 0.0;
    int n;
    for (n = 0; n < niter; n++)
      {
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("====== Read iteration %d\n"), n));

        size_t size = (total + dst_offset) * H::size ();
        ACE_InputCDR is (toread, size, opposite_byte_order);

        // This is intrusive...
        char* const end = is.rd_ptr () + size;

        do_seal (end);

        double secs = 0.0;
        if (use_array)
          {
            {
              int i;
              for (i = 0; i < dst_offset; i++)
                {
                  T v;
                  is >> v;
                }
            }

            if (n == 0)
              {
                ACE_DEBUG((LM_DEBUG,
                           ACE_TEXT ("* src align = %d, dst align = %d\n"),
                           tellalign (is.rd_ptr ()),
                           tellalign (src)));
              }

            Crono crono;
            crono.start ();
            H::read_array (is, idata, total);
            crono.stop ();
            secs = crono.read_seconds ();

            // Testing for good bit value. Try reading atleast 10
            // times the size of total. It should fail with good bit
            // set to 0.
            H::read_array (is, idata, 10 * total);

            if (is.good_bit () != 0)
              {
                ACE_ERROR ((LM_ERROR,
                            ACE_TEXT ("Test for good bit failed in %s Array_test\n"),
                            H::name ()));
              }
          }
        else
          {
            int i = 0;
            Crono crono;
            crono.start ();
            while (i < total)
              {
#if 0
                T v;
                is >> v;
                static char rs[32 + 1];
                CDR_Test<T,H>::ttoh (v, rs);
                ACE_DEBUG ((LM_DEBUG, "Read idata[%d] = %s\n", i, rs));
                idata[i] = v;
                i++;
#else
                is >> idata[i++];
                is >> idata[i++];
                is >> idata[i++];
                is >> idata[i++];
#endif /* 0 */
              }
            crono.stop ();
            secs = crono.read_seconds ();
          }
        totalsecs += secs;

        if (!check_seal (end))
          {
            ACE_ERROR((LM_ERROR,
                       ACE_TEXT( "Broken seal, aborting.\n" )));
            ACE_OS::exit(1);
          }
      }

    totalsecs = totalsecs / niter;

    ACE_DEBUG((LM_DEBUG,
               ACE_TEXT ("Reading from stream %d %s values")
               ACE_TEXT (" (byte swapping): %f seconds.\n"),
               total,
               H::name (),
               totalsecs));
  }

  ACE_DEBUG((LM_DEBUG,
             ACE_TEXT ("Now checking data...\n") ));

  int errors = 0;
  const int maxerrors = 6;

  {
    int i;
    for (i = 0; i < total; i++)
      {
        T rv;

        const char* src = reinterpret_cast<const char*> ((idata + i));
        char* dst = reinterpret_cast<char*> ((&rv));

        H::swap(src, dst);

        // Due to a "feature" of the gcc 4.1.1 optimizer, we need to do
        // something with the src pointer so that it doesn't optimize it
        // away.  Calling tellalign() is benign, but the optimizer
        // doesn't know/care. -- Chad Elliott 1/10/2007
        tellalign (src);

        T cv = CDR_Test<T, H>::checkval (i);
        if (!ACE::is_equal (rv, cv))
          {
            static char rs[32 + 1];
            static char cs[32 + 1];
            CDR_Test<T, H>::ttoh (rv, rs);
            CDR_Test<T, H>::ttoh (cv, cs);
            ACE_ERROR((LM_ERROR,
                       ACE_TEXT ( "Wrong value at pos %d:" )
                       ACE_TEXT ( " '%s' should be '%s'.\n" ),
                       i, rs, cs));
            errors++;
            if (errors == maxerrors)
              {
                ACE_ERROR((LM_ERROR,
                           ACE_TEXT ( "%d errors found, ")
                           ACE_TEXT ( "interrupting check.\n" ),
                           errors));
                break;
              }
          }
      }
  }

  if (errors != 0)
    {
      ACE_ERROR((LM_ERROR,
                 ACE_TEXT (" assertion failed: Inconsistencies found (%d), ")
                 ACE_TEXT ("aborting.\n"), errors));
      ACE_OS::exit(1);
    }

  ACE_DEBUG((LM_DEBUG,
             ACE_TEXT ("Data OK, test %s completed.\n"),
             H::name ()));
}

template <class T, class N>
CDR_Test<T, N>::~CDR_Test ()
{
}

//
// Helper Clases for the second template parameter of CDR_Test.
// One for each tested CDR type.
//

struct DoubleHelper
{
  static const ACE_TCHAR* name ()
    {
      return ACE_TEXT ("CDR::Double");
    }
  static int integral ()
    {
      return 0;
    }
  static void read_array (ACE_InputCDR& is,
                          ACE_CDR::Double* x,
                          ACE_UINT32 n)
    {
      is.read_double_array (x, n);
    }
  static void write_array (ACE_OutputCDR& os,
                           ACE_CDR::Double* x,
                           ACE_UINT32 n)
    {
      os.write_double_array (x, n);
    }
  static void swap (const char *src, char *dst)
    {
      ACE_CDR::swap_8 (src, dst);
    }
  static size_t size ()
    {
      return sizeof(ACE_CDR::Double);
    }
};

struct FloatHelper
{
  static const ACE_TCHAR* name ()
    {
      return ACE_TEXT ("CDR::Float");
    }
  static int integral ()
    {
      return 0;
    }
  static void read_array (ACE_InputCDR& is,
                          ACE_CDR::Float* x,
                          ACE_UINT32 n)
    {
      is.read_float_array (x, n);
    }
  static void write_array (ACE_OutputCDR& os,
                           ACE_CDR::Float* x,
                           ACE_UINT32 n)
    {
      os.write_float_array (x, n);
    }
  static void swap (const char *src, char *dst)
    {
      ACE_CDR::swap_4 (src, dst);
    }
  static size_t size ()
    {
      return sizeof(ACE_CDR::Float);
    }
};

struct ShortHelper
{
  static const ACE_TCHAR* name ()
    {
      return ACE_TEXT ("CDR::Short");
    }
  static int integral ()
    {
      return 1;
    }
  static void read_array (ACE_InputCDR& is,
                          ACE_CDR::Short* x,
                          ACE_UINT32 n)
    {
      is.read_short_array (x, n);
    }
  static void write_array (ACE_OutputCDR& os,
                           ACE_CDR::Short* x,
                           ACE_UINT32 n)
    {
      os.write_short_array (x, n);
    }
  static void swap (const char *src, char *dst)
    {
      ACE_CDR::swap_2 (src, dst);
    }
  static size_t size ()
    {
      return sizeof(ACE_CDR::Short);
    }
};

struct LongHelper
{
  static const ACE_TCHAR* name ()
    {
      return ACE_TEXT ("CDR::Long");
    }
  static int integral ()
    {
      return 1;
    }
  static void read_array (ACE_InputCDR& is,
                          ACE_CDR::Long* x,
                          ACE_UINT32 n)
    {
      is.read_long_array (x, n);
    }
  static void write_array (ACE_OutputCDR& os,
                           ACE_CDR::Long* x,
                           ACE_UINT32 n)
    {
      os.write_long_array (x, n);
    }
  static void swap (const char *src, char *dst)
    {
      ACE_CDR::swap_4 (src, dst);
    }
  static size_t size ()
    {
      return sizeof(ACE_CDR::Long);
    }
};

struct LongLongHelper
{
  static const ACE_TCHAR* name ()
    {
      return ACE_TEXT ("CDR::LongLong");
    }
  static int integral ()
    {
      return 1;
    }
  static void read_array (ACE_InputCDR& is,
                          ACE_CDR::LongLong* x,
                          ACE_UINT32 n)
    {
      is.read_longlong_array (x, n);
    }
  static void write_array (ACE_OutputCDR& os,
                           ACE_CDR::LongLong* x,
                           ACE_UINT32 n)
    {
      os.write_longlong_array (x, n);
    }

  static void swap (const char *src, char *dst)
    {
      ACE_CDR::swap_8 (src, dst);
    }
  static size_t size ()
    {
      return sizeof(ACE_CDR::LongLong);
    }
};

struct CharHelper
{
  static const ACE_TCHAR* name ()
    {
      return ACE_TEXT ("CDR::Char");
    }
  static int integral ()
    {
      return 1;
    }
  static void read_array (ACE_InputCDR& is,
                          ACE_CDR::Char* x,
                          ACE_UINT32 n)
    {
      is.read_char_array (x, n);
    }
  static void write_array (ACE_OutputCDR& os,
                           ACE_CDR::Char* x,
                           ACE_UINT32 n)
    {
      os.write_char_array (x, n);
    }
  static void swap (const char *src, char *dst)
    {
      *dst = *src;
    }
  static size_t size ()
    {
      return sizeof(ACE_CDR::Char);
    }
};

void usage (const ACE_TCHAR* cmd)
{
  ACE_ERROR((LM_ERROR,
             ACE_TEXT ("Usage: %s ")
             ACE_TEXT ("[-n n] " )
             ACE_TEXT ("[[-d n|-f n|-q n|-w n|-h n|-c n|-t n] | [-t n]]\n")
             ACE_TEXT ("  -n n: average time for n iterations.\n")
             ACE_TEXT ("  -d n: n double precision floating point\n")
             ACE_TEXT ("  -f n: n single precision floating point\n")
             ACE_TEXT ("  -q n: n quadwords (int64).\n")
             ACE_TEXT ("  -w n: n words (int32).\n")
             ACE_TEXT ("  -h n: n halfwords (int16).\n")
             ACE_TEXT ("  -c n: n chars.\n")
             ACE_TEXT ("  -t n: n iterations for every type.\n")
             ACE_TEXT ("  n must be >= 16 for dfqwhct.\n")
             ACE_TEXT ("  If you provide one of dfqwhc, then only the\n")
             ACE_TEXT ("  test for the corresponding type ")
             ACE_TEXT ("will be performed.\n"),
             cmd));
  ACE_OS::exit(1);
}

int
validtotal (int n)
{
  return n >= 16;
}

int
validiters (int n)
{
  return n > 0;
}

int
run_main (int argc, ACE_TCHAR *argv[])
{
  ACE_START_TEST (ACE_TEXT ("CDR_Array_Test"));

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("This is ACE Version %u.%u.%u\n\n"),
              ACE::major_version (),
              ACE::minor_version(),
              ACE::beta_version()));

  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("d:f:q:w:h:c:t:n:"));
  int dtotal = 0;
  int ftotal = 0;
  int qtotal = 0;
  int wtotal = 0;
  int htotal = 0;
  int ctotal = 0;
  int total = 0;
  int niter = 0;

  struct { int c; int *v; int (*checkf)(int); } opts[] = {
    { 'd', &dtotal, validtotal },
    { 'f', &ftotal, validtotal },
    { 'q', &qtotal, validtotal },
    { 'w', &wtotal, validtotal },
    { 'h', &htotal, validtotal },
    { 'c', &ctotal, validtotal },
    { 't', &total, validtotal },
    { 'n', &niter, validiters },
  };

  int n = sizeof(opts)/sizeof(opts[0]);

  int opt;
  while ((opt = get_opt ()) != EOF)
    {
      int got = 0;
      int i;
      for (i = 0; i < n; i++)
        {
          if (opts[i].c == opt)
            {
              int v = ACE_OS::atoi (get_opt.opt_arg ());
              if (!(opts[i].checkf) (v))
                {
                  usage(ACE_TEXT("CDR_Array_Test"));
                }

              *(opts[i].v) = v;
              got = 1;
              break;
            }
        }

      if (!got)
        {
          usage(ACE_TEXT("CDR_Array_Test"));
        }
    }

  if (total == 0)
    {
      total = default_total;
    }

  if (niter == 0)
    {
      niter = default_niter;
    }

  if (dtotal == 0
      && ftotal == 0
      && qtotal == 0
      && wtotal == 0
      && htotal == 0
      && ctotal == 0)
    {
      dtotal = ftotal = qtotal = wtotal = htotal = ctotal = total;
    }

  int use_array;
  for (use_array = 0; use_array < 2; use_array++)
    {
      {
        CDR_Test<ACE_CDR::Double, DoubleHelper>
          test (dtotal, niter, use_array);
      }
      {
        CDR_Test<ACE_CDR::Float, FloatHelper>
          test (ftotal, niter, use_array);
      }
      {
        CDR_Test<ACE_CDR::LongLong, LongLongHelper>
          test (qtotal, niter, use_array);
      }
      {
        CDR_Test<ACE_CDR::Long, LongHelper>
          test (wtotal, niter, use_array);
      }
      {
        CDR_Test<ACE_CDR::Short, ShortHelper>
          test (htotal, niter, use_array);
      }
      {
        CDR_Test<ACE_CDR::Char, CharHelper>
          test (ctotal, niter, use_array);
      }
    }

  ACE_END_TEST;
  return 0;
}

