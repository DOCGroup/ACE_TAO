// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Cached_Allocator_Test.cpp
//
// = DESCRIPTION
//    Simple test of ACE_Dynamic_Cached_Allocator and ACE_Cached_Allocator.
//
// = AUTHOR
//    Jaroslaw Nozderko <jareknz@polbox.com>
//
// ============================================================================

#include "test_config.h"
#include "ace/Malloc_T.h"
#include "ace/High_Res_Timer.h"

ACE_RCSID(tests, Cached_Allocator_Test, "$Id$")

typedef ACE_Dynamic_Cached_Allocator<ACE_SYNCH_NULL_MUTEX> DYNAMIC_ALLOCATOR;

static int
speed_test (ACE_UINT32 loops)
{
#ifndef ACE_LACKS_FLOATING_POINT
  double tt    = 0.0,
    ut    = 0.0,
    utus  = 0.0,
    speed = 0.0;
#endif /* ACE_LACKS_FLOATING_POINT */

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
#ifdef ACE_LACKS_FLOATING_POINT
  ACE_DEBUG ((LM_INFO, ACE_TEXT (" (%t) Total time : %d s, %d us\n"),
              tc.sec (), tc.usec ()));
#elif !defined ACE_LACKS_FLOATING_POINT
  tt    = tc.sec () + tc.usec ()*1.0e-6;
  ut    = tt/loops;
  utus  = ut*1.0e6;
  speed = loops/tt;

  ACE_DEBUG ((LM_INFO, ACE_TEXT (" (%t) Total time : %.6g [s]\n"), tt));
  ACE_DEBUG ((LM_INFO, ACE_TEXT (" (%t) Unit time  : %.6g [us]\n"), utus));
  ACE_DEBUG ((LM_INFO, ACE_TEXT (" (%t) Speed      : %.6g [1/s]\n"), speed));

#endif /* !defined ACE_LACKS_FLOATING_POINT */

  return 0;
}

typedef char MEMBLOCK[8];
typedef ACE_Cached_Allocator<MEMBLOCK, ACE_SYNCH_NULL_MUTEX> STATIC_ALLOCATOR;

static int
stdspeed_test (ACE_UINT32 loops)
{

#ifndef ACE_LACKS_FLOATING_POINT
  double tt    = 0.0,
    ut    = 0.0,
    utus  = 0.0,
    speed = 0.0;
#endif /* ACE_LACKS_FLOATING_POINT */

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
#ifdef ACE_LACKS_FLOATING_POINT
  ACE_DEBUG ((LM_INFO, ACE_TEXT (" (%t) Total time : %d s, %d us\n"),
              tc.sec (), tc.usec ()));
#elif !defined ACE_LACKS_FLOATING_POINT
  tt    = tc.sec () + tc.usec ()*1.0e-6;
  ut    = tt/loops;
  utus  = ut*1.0e6;
  speed = loops/tt;

  ACE_DEBUG ((LM_INFO, ACE_TEXT (" (%t) Total time : %.6g [s]\n"), tt));
  ACE_DEBUG ((LM_INFO, ACE_TEXT (" (%t) Unit time  : %.6g [us]\n"), utus));
  ACE_DEBUG ((LM_INFO, ACE_TEXT (" (%t) Speed      : %.6g [1/s]\n"), speed));

#endif /* !defined ACE_LACKS_FLOATING_POINT */

  return 0;
}

int
run_main (int argc, ACE_TCHAR *argv[])
{
  ACE_START_TEST (ACE_TEXT ("Cached_Allocator_Test"));

  size_t chunk_size = 0;
  size_t n_chunks   = 0;
  size_t requested_size = 0;
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
              ACE_TEXT ("%d chunks, %d bytes each\n"),
              n_chunks,
              chunk_size));

  DYNAMIC_ALLOCATOR allocator (n_chunks, chunk_size);

  requested_size = chunk_size;
  ACE_DEBUG ((LM_INFO,
              ACE_TEXT (" (%t) Allocating chunk 1: %d bytes, should succeed...\n"),
              requested_size));

  ptr1 = (char *) allocator.malloc (requested_size);
  if (!ptr1)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT (" (%t) Failed, exiting.\n")), -1);

  ACE_DEBUG ((LM_INFO, ACE_TEXT (" (%t) OK, succeeded.\n")));

  requested_size = chunk_size + 1;
  ACE_DEBUG ((LM_INFO,
              ACE_TEXT (" (%t) Allocating chunk 2: %d bytes, too big, should fail...\n"),
              requested_size));

  ptr2 = (char *) allocator.malloc (requested_size);
  if (!ptr2)
    ACE_DEBUG ((LM_INFO, ACE_TEXT (" (%t) OK, failed.\n")));
  else
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT (" (%t) Something is wrong...\n")), -1);

  requested_size = chunk_size - 1;
  ACE_DEBUG ((LM_INFO,
              ACE_TEXT (" (%t) Allocating chunk 3: %d bytes, ")
              ACE_TEXT ("should succeed...\n"),
              requested_size));
  ptr3 = (char *) allocator.malloc (requested_size);
  if (!ptr3)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT (" (%t) Failed, exiting.\n")), -1);

  ACE_DEBUG ((LM_INFO, ACE_TEXT (" (%t) OK, succeeded.\n")));

  // One chunk too far...
  requested_size = chunk_size;
  ACE_DEBUG ((LM_INFO,
              ACE_TEXT (" (%t) Allocating chunk 4: %d bytes, no free chunks,")
              ACE_TEXT (" should fail...\n"),
              requested_size));

  ptr4 = (char *) allocator.malloc (requested_size);
  if (!ptr4)
    ACE_DEBUG ((LM_INFO, ACE_TEXT (" (%t) OK, failed.\n")));
  else
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT (" (%t) Something is wrong\n")), -1);

  ACE_DEBUG ((LM_INFO, ACE_TEXT (" (%t) Writing to chunk 1:   %s\n"), str1));
  ACE_OS::memcpy (ptr1, str1, chunk_size);
  ptr1[chunk_size - 1] = '\0';
  ACE_DEBUG ((LM_INFO, ACE_TEXT (" (%t) Reading from chunk 1: %s\n"), ptr1));

  ACE_DEBUG ((LM_INFO, ACE_TEXT (" (%t) Writing to chunk 3:   %s\n"), str3));
  ACE_OS::memcpy (ptr3, str3, chunk_size);
  ptr3[chunk_size - 1] = '\0';
  ACE_DEBUG ((LM_INFO, ACE_TEXT (" (%t) Reading from chunk 3: %s\n"), ptr3));

  ACE_DEBUG ((LM_INFO, ACE_TEXT (" (%t) Deallocating chunk 1\n")));
  allocator.free (ptr1);

  requested_size = chunk_size;
  ACE_DEBUG ((LM_INFO, ACE_TEXT (" (%t) Allocating chunk: %d bytes, ")
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

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Free_List<ACE_Cached_Mem_Pool_Node<MEMBLOCK> >;
template class ACE_Free_List<ACE_Cached_Mem_Pool_Node<char> >;
template class ACE_Dynamic_Cached_Allocator<ACE_SYNCH_NULL_MUTEX>;
template class ACE_Cached_Allocator<MEMBLOCK, ACE_SYNCH_NULL_MUTEX>;
template class ACE_Locked_Free_List<ACE_Cached_Mem_Pool_Node<char>, ACE_SYNCH_NULL_MUTEX>;
template class ACE_Cached_Mem_Pool_Node<char>;
template class ACE_Locked_Free_List<ACE_Cached_Mem_Pool_Node<MEMBLOCK>, ACE_SYNCH_NULL_MUTEX>;
template class ACE_Cached_Mem_Pool_Node<MEMBLOCK>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Free_List<ACE_Cached_Mem_Pool_Node<MEMBLOCK> >
#pragma instantiate ACE_Free_List<ACE_Cached_Mem_Pool_Node<char> >
#pragma instantiate ACE_Dynamic_Cached_Allocator<ACE_SYNCH_NULL_MUTEX>
#pragma instantiate ACE_Cached_Allocator<MEMBLOCK, ACE_SYNCH_NULL_MUTEX>
#pragma instantiate ACE_Locked_Free_List<ACE_Cached_Mem_Pool_Node<char>, ACE_SYNCH_NULL_MUTEX>
#pragma instantiate ACE_Cached_Mem_Pool_Node<char>
#pragma instantiate ACE_Locked_Free_List<ACE_Cached_Mem_Pool_Node<MEMBLOCK>, ACE_SYNCH_NULL_MUTEX>
#pragma instantiate ACE_Cached_Mem_Pool_Node<MEMBLOCK>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
