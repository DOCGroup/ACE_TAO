//=============================================================================
/**
 *  @file    Memcpy_Test.cpp
 *
 *  $Id$
 *
 *  @author Mike Marinez <mmartinez@oci.com>
 *
 * This test compares the performance of ACE_OS::memcpy with
 * that of smemcpy which unrolls the memcpy loop upto size = 16.
 * This test is also run via autoconf and if smemcpy is faster,
 * ACE_HAS_MEMCPY_LOOP_UNROLL flag is set.
 *
 */
//=============================================================================

#include "ace/OS_NS_string.h"
#include "ace/High_Res_Timer.h"

#include "test_config.h"

void *
smemcpy (void *dest, const void *src, const size_t n)
{
  unsigned char *to = static_cast<unsigned char*> (dest);
  const unsigned char *from = static_cast<const unsigned char*> (src);
  // Unroll the loop...
  switch (n) {
    case 16: to[ 15] = from[ 15];
    case 15: to[ 14] = from[ 14];
    case 14: to[ 13] = from[ 13];
    case 13: to[ 12] = from[ 12];
    case 12: to[ 11] = from[ 11];
    case 11: to[ 10] = from[ 10];
    case 10: to[  9] = from[  9];
    case  9: to[  8] = from[  8];
    case  8: to[  7] = from[  7];
    case  7: to[  6] = from[  6];
    case  6: to[  5] = from[  5];
    case  5: to[  4] = from[  4];
    case  4: to[  3] = from[  3];
    case  3: to[  2] = from[  2];
    case  2: to[  1] = from[  1];
    case  1: to[  0] = from[  0];
    case  0: return dest;
    default: return ACE_OS::memcpy (dest, src, n);
  }
}

void
testit (int type)
{
  char buffer[16];
  size_t size = 16;

  switch (type)
    {
    case 0: smemcpy ((void*)buffer, (void*)" THIS IS A TEST", size); break;
    case 1:  ACE_OS::memcpy ((void*)buffer, (void*)" THIS IS A TEST", size); break;
    }

}

namespace { enum { ITERATIONS = 100000000 }; }

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Memcpy_Test"));

  //ACE_Time_Value start, now;
  struct timeval start, now;

  for (int i = ITERATIONS; i > 0; --i)
    testit (0);

  start = ACE_High_Res_Timer::gettimeofday_hr ();
  for (int j = ITERATIONS; j > 0; --j)
    testit (0);

  now = ACE_High_Res_Timer::gettimeofday_hr ();

  double fast = 1000000  *(now.tv_sec - start.tv_sec) +
    now.tv_usec - start.tv_usec;
  ACE_DEBUG ((LM_DEBUG,
               ACE_TEXT ("%f uSec per iteration for fast version.\n"),
               fast / ITERATIONS));

  start = ACE_High_Res_Timer::gettimeofday_hr ();
  for (int k = ITERATIONS; k > 0; --k)
    testit (1);

  now = ACE_High_Res_Timer::gettimeofday_hr ();

  double slow = 1000000 * (now.tv_sec-start.tv_sec) + now.tv_usec - start.tv_usec;
  ACE_DEBUG ((LM_DEBUG,
               ACE_TEXT ("%f uSec per iteration for slow version.\n"),
               slow / ITERATIONS));

  ACE_END_TEST;

  return 0;
}
