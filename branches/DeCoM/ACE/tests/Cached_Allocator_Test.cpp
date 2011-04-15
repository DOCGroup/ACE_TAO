
//=============================================================================
/**
 *  @file    Cached_Allocator_Test.cpp
 *
 *  $Id$
 *
 *  Simple test of ACE_Dynamic_Cached_Allocator and ACE_Cached_Allocator.
 *
 *
 *  @author Jaroslaw Nozderko <jareknz@polbox.com>
 */
//=============================================================================


#include "test_config.h"
#include "ace/OS_NS_string.h"
#include "ace/Malloc_T.h"
#include "ace/High_Res_Timer.h"



#include "ace/Synch_Traits.h"
#include "ace/Null_Mutex.h"

typedef ACE_Dynamic_Cached_Allocator<ACE_SYNCH_NULL_MUTEX> DYNAMIC_ALLOCATOR;

static int
speed_test (ACE_UINT32 loops)
{
  double tt    = 0.0,
    ut    = 0.0,
    utus  = 0.0,
    speed = 0.0;

  ACE_Time_Value tc;
  void *ptr = 0;
  ACE_UINT32 i = loops;
  size_t n_chunks = 10;
  size_t chunk_size = 8;

  ACE_DEBUG ((LM_INFO,
              ACE_TEXT (" (%t) ACE_Dynamic_Cached_Allocator ")
              ACE_TEXT ("speed test...\n")));

  DYNAMIC_ALLOCATOR allocator (n_chunks, chunk_size);

  ACE_High_Res_Timer timer;
  timer.reset ();

  timer.start ();

  while (i--)
    {
      ptr = allocator.malloc (chunk_size);
      allocator.free (ptr);
    }

  timer.stop ();

  timer.elapsed_time (tc);

  ACE_DEBUG ((LM_INFO, ACE_TEXT (" (%t) Iterations : %d\n"), loops));
  tt    = tc.sec () + tc.usec ()*1.0e-6;
  ut    = tt/loops;
  utus  = ut*1.0e6;
  speed = loops/tt;

  ACE_DEBUG ((LM_INFO, ACE_TEXT (" (%t) Total time : %.6g [s]\n"), tt));
  ACE_DEBUG ((LM_INFO, ACE_TEXT (" (%t) Unit time  : %.6g [us]\n"), utus));
  ACE_DEBUG ((LM_INFO, ACE_TEXT (" (%t) Speed      : %.6g [1/s]\n"), speed));

  return 0;
}

typedef char MEMBLOCK[8];
typedef ACE_Cached_Allocator<MEMBLOCK, ACE_SYNCH_NULL_MUTEX> STATIC_ALLOCATOR;

static int
stdspeed_test (ACE_UINT32 loops)
{

  double tt    = 0.0,
    ut    = 0.0,
    utus  = 0.0,
    speed = 0.0;

  ACE_Time_Value tc;
  void *ptr = 0;
  ACE_UINT32 i = loops;
  size_t n_chunks   = 10,
    chunk_size = 8;

  ACE_DEBUG ((LM_INFO, ACE_TEXT (" (%t) ACE_Cached_Allocator ")
                       ACE_TEXT ("speed test...\n")));

  STATIC_ALLOCATOR allocator (n_chunks);

  ACE_High_Res_Timer timer;
  timer.reset ();

  timer.start ();
  while (i--)
    {
      ptr = allocator.malloc (chunk_size);
      allocator.free (ptr);
    }
  timer.stop ();

  timer.elapsed_time (tc);

  ACE_DEBUG ((LM_INFO, ACE_TEXT (" (%t) Iterations : %d\n"), loops));
  tt    = tc.sec () + tc.usec ()*1.0e-6;
  ut    = tt/loops;
  utus  = ut*1.0e6;
  speed = loops/tt;

  ACE_DEBUG ((LM_INFO, ACE_TEXT (" (%t) Total time : %.6g [s]\n"), tt));
  ACE_DEBUG ((LM_INFO, ACE_TEXT (" (%t) Unit time  : %.6g [us]\n"), utus));
  ACE_DEBUG ((LM_INFO, ACE_TEXT (" (%t) Speed      : %.6g [1/s]\n"), speed));

  return 0;
}

int
run_main (int argc, ACE_TCHAR *argv[])
{
  ACE_START_TEST (ACE_TEXT ("Cached_Allocator_Test"));

  size_t chunk_size = 0;
  size_t n_chunks   = 0;
  size_t requested_size = 0;
  size_t depth = 0;
  char *ptr1 = 0;
  char *ptr2 = 0;
  char *ptr3 = 0;
  char *ptr4 = 0;
  ACE_UINT32 loops = 0;

  const char *str1 = "12345678";
  const char *str3 = "ABCDEFGH";

  if (argc < 2)
    loops = 10000000;
  else
    loops = ACE_OS::atoi (argv[1]);

  chunk_size = 8;
  n_chunks = 2;

  ACE_DEBUG ((LM_INFO,
              ACE_TEXT (" (%t) Creating allocator: ")
              ACE_TEXT ("%B chunks, %B bytes each\n"),
              n_chunks,
              chunk_size));

  DYNAMIC_ALLOCATOR allocator (n_chunks, chunk_size);

  if ((depth = allocator.pool_depth ()) != n_chunks)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("Expected pool depth %B but reported %B\n"),
                n_chunks, depth));
  requested_size = chunk_size;
  ACE_DEBUG ((LM_INFO,
              ACE_TEXT (" (%t) Allocating chunk 1: %B bytes, should succeed...\n"),
              requested_size));

  ptr1 = (char *) allocator.malloc (requested_size);
  if (!ptr1)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT (" (%t) Failed, exiting.\n")), -1);

  ACE_DEBUG ((LM_INFO, ACE_TEXT (" (%t) OK, succeeded.\n")));
  if ((depth = allocator.pool_depth ()) != (n_chunks - 1))
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("Expected pool depth %B but reported %B\n"),
                n_chunks - 1, depth));

  requested_size = chunk_size + 1;
  ACE_DEBUG ((LM_INFO,
              ACE_TEXT (" (%t) Allocating chunk 2: %B bytes, too big, should fail...\n"),
              requested_size));

  ptr2 = (char *) allocator.malloc (requested_size);
  if (!ptr2)
    ACE_DEBUG ((LM_INFO, ACE_TEXT (" (%t) OK, failed.\n")));
  else
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT (" (%t) Something is wrong...\n")), -1);

  requested_size = chunk_size - 1;
  ACE_DEBUG ((LM_INFO,
              ACE_TEXT (" (%t) Allocating chunk 3: %B bytes, ")
              ACE_TEXT ("should succeed...\n"),
              requested_size));
  ptr3 = (char *) allocator.malloc (requested_size);
  if (!ptr3)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT (" (%t) Failed, exiting.\n")), -1);

  ACE_DEBUG ((LM_INFO, ACE_TEXT (" (%t) OK, succeeded.\n")));

  // One chunk too far...
  if ((depth = allocator.pool_depth ()) != 0)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("Expected pool depth 0 but reported %B\n"),
                depth));
  requested_size = chunk_size;
  ACE_DEBUG ((LM_INFO,
              ACE_TEXT (" (%t) Allocating chunk 4: %B bytes, no free chunks,")
              ACE_TEXT (" should fail...\n"),
              requested_size));

  ptr4 = (char *) allocator.malloc (requested_size);
  if (!ptr4)
    ACE_DEBUG ((LM_INFO, ACE_TEXT (" (%t) OK, failed.\n")));
  else
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT (" (%t) Something is wrong\n")), -1);

  ACE_DEBUG ((LM_INFO, ACE_TEXT (" (%t) Writing to chunk 1:   %C\n"), str1));
  ACE_OS::memcpy (ptr1, str1, chunk_size);
  ptr1[chunk_size - 1] = '\0';
  ACE_DEBUG ((LM_INFO, ACE_TEXT (" (%t) Reading from chunk 1: %C\n"), ptr1));

  ACE_DEBUG ((LM_INFO, ACE_TEXT (" (%t) Writing to chunk 3:   %C\n"), str3));
  ACE_OS::memcpy (ptr3, str3, chunk_size);
  ptr3[chunk_size - 1] = '\0';
  ACE_DEBUG ((LM_INFO, ACE_TEXT (" (%t) Reading from chunk 3: %C\n"), ptr3));

  ACE_DEBUG ((LM_INFO, ACE_TEXT (" (%t) Deallocating chunk 1\n")));
  allocator.free (ptr1);

  requested_size = chunk_size;
  ACE_DEBUG ((LM_INFO, ACE_TEXT (" (%t) Allocating chunk: %B bytes, ")
                       ACE_TEXT ("should succeed...\n"),
              requested_size));
  ptr1 = (char *) allocator.malloc (requested_size);
  if (!ptr1)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT (" (%t) Failed, exiting.\n")), -1);

  ACE_DEBUG ((LM_INFO, ACE_TEXT (" (%t) OK, succeeded.\n")));

  ACE_DEBUG ((LM_INFO, ACE_TEXT (" (%t) Deallocating chunk 1\n")));
  allocator.free (ptr1);
  ACE_DEBUG ((LM_INFO, ACE_TEXT (" (%t) Deallocating chunk 3\n")));
  allocator.free (ptr3);

  speed_test (loops);
  stdspeed_test (loops);

  ACE_END_TEST;
  return 0;
}

