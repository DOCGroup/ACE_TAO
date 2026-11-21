
//============================================================================
/**
 *  @file    Allocator_Cascaded_Test.cpp
 *
 *  This program tests the basic APIs supported in <ACE_Cascaded_Dynamic_Cached_Allocator> and demonstrates how to use it.
 *
 *  @author Smith Achang <changyunlei@126.com>
 */
//============================================================================


#include "ace/Malloc_T.h"
#include "test_config.h"
#include <vector>

#define ACE_TEST_EXCEPTION_RETURN(expression, message)   \
do                                                       \
{                                                        \
  if (expression)                                        \
  {                                                      \
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT_CHAR_TO_TCHAR (message)), 1);\
  }                                                      \
}                                                        \
while (0)

static int
run_cascaded_allocator_test ()
{
  ACE_DEBUG ((LM_INFO, "%C begin to run ...\n", __func__));

  size_t const initial_n_chunks  = 1;
  size_t const chunk_size        = sizeof(void*) + 7;

  void *ptr, *ptr1, *ptr2;
  size_t nbytes = chunk_size;
  size_t pool_sum, old_pool_sum, pool_depth, old_pool_depth;
  char initial_value = '\0';

  ACE_Cascaded_Dynamic_Cached_Allocator<ACE_SYNCH_MUTEX> alloc (initial_n_chunks, chunk_size);
  pool_sum = alloc.pool_sum ();
  ACE_TEST_EXCEPTION_RETURN (pool_sum != initial_n_chunks, "  initial pool sum must be initial_n_chunks\n");

  ACE_DEBUG ((LM_INFO, "%C will test unsupported API ...\n", __func__));
  ptr = alloc.calloc (1, chunk_size, initial_value);
  ACE_TEST_EXCEPTION_RETURN (ptr != nullptr,
                             "  pool must return nullptr for calloc(size_t n_elem, size_t elem_size, char initial_value) call\n");
  ACE_TEST_EXCEPTION_RETURN(alloc.pool_depth() != initial_n_chunks,
                            "  initial pool depth must keep unchanged for call of unsupported API\n");

  ACE_DEBUG ((LM_INFO, "%C will test supported calloc API ...\n", __func__));
  ptr = alloc.calloc (nbytes + 1, initial_value);
  ACE_TEST_EXCEPTION_RETURN (ptr != nullptr,
                             "  pool must return nullptr for calloc call with bigger nbytes parameter\n");
  ACE_TEST_EXCEPTION_RETURN (alloc.pool_depth () != initial_n_chunks,
                             "  initial pool depth must keep unchanged for call of unsupported API\n");

  ptr = alloc.calloc (nbytes);
  ACE_TEST_EXCEPTION_RETURN (ptr == nullptr, "  pool must return valid ptr for calloc call with normal nbytes\n");
  ACE_TEST_EXCEPTION_RETURN (*static_cast<ACE_UINT64*> (ptr) != 0, "  calloc call will clear the memory to zero\n");
  alloc.free (ptr);

  ACE_DEBUG ((LM_INFO, "%C will test supported malloc API ...\n", __func__));
  ptr = alloc.malloc (nbytes + 1);
  ACE_TEST_EXCEPTION_RETURN (ptr != nullptr,
                             "  pool must return nullptr for malloc call with bigger nbytes parameter\n");

  ptr = alloc.malloc(nbytes);
  ACE_TEST_EXCEPTION_RETURN (ptr == nullptr,
                             "  pool must return valid ptr for malloc call with normal nbytes\n");
  ACE_TEST_EXCEPTION_RETURN (alloc.pool_depth () != 0, "  initial pool depth must be zero\n");
  alloc.free(ptr);
  ACE_TEST_EXCEPTION_RETURN (alloc.pool_depth () != initial_n_chunks,
                             "  initial pool depth must be initial_n_chunks after free\n");

  ACE_DEBUG ((LM_INFO, "%C will test cascaded allocator ...\n", __func__));
  ptr  = alloc.malloc (nbytes);
  ACE_TEST_EXCEPTION_RETURN (ptr == nullptr,
                             "  pool must return valid ptr, cascaded pool must support to alloc more times firstly\n");

  ptr1 = alloc.malloc (nbytes);
  ACE_TEST_EXCEPTION_RETURN (ptr1 == nullptr,
                             "  pool must return valid ptr, cascaded pool must support to alloc more times secondly\n");
  pool_depth = alloc.pool_depth ();
  ACE_TEST_EXCEPTION_RETURN (pool_depth != 1,
                             "  cascaded pool depth must support to alloc twice\n");

  old_pool_depth = alloc.pool_depth();
  ptr2 = alloc.malloc (nbytes);
  ACE_TEST_EXCEPTION_RETURN (ptr2 == nullptr,
                             "  pool must return valid ptr, cascaded pool must support to alloc more times thirdly\n");
  pool_depth = alloc.pool_depth();
  ACE_TEST_EXCEPTION_RETURN (pool_depth != ((2*initial_n_chunks) - old_pool_depth - 1),
                             "  cascaded pool depth must support to alloc three times\n");

  old_pool_sum = pool_sum;
  pool_sum     = alloc.pool_sum ();
  ACE_TEST_EXCEPTION_RETURN (pool_sum < old_pool_sum,
                             "  cascaded pool sum must be bigger than that of initial pool\n");
  ACE_TEST_EXCEPTION_RETURN (pool_sum != (initial_n_chunks + (2 * initial_n_chunks)),
                             "  cascaded pool sum must be as expected, pool has been enlarged\n");

  alloc.free (ptr);
  alloc.free (ptr1);
  alloc.free (ptr2);

  ACE_TEST_EXCEPTION_RETURN (alloc.pool_depth () != (initial_n_chunks + (2 * initial_n_chunks)),
                             "  cascaded pool depth must be three after having freed all malloc ptrs\n");

  ACE_DEBUG ((LM_INFO, "%C will test cascaded allocator deeply ...\n", __func__));
  old_pool_sum = alloc.pool_sum ();
  size_t const totalAllocSum = 8 * 1024;
  char cmpvalues[chunk_size];
  char initial_cmp_value = initial_value;
  std::vector<void*> ptrs;
  for (long i = 0; i < totalAllocSum; ++i, ++initial_cmp_value)
  {
    ACE_OS::memset (cmpvalues, initial_cmp_value, chunk_size);
    ptr = alloc.calloc (nbytes, initial_cmp_value);
    ACE_TEST_EXCEPTION_RETURN (ptr == nullptr,
                               "  pool must return valid ptr for deeply calloc api test with normal nbytes\n");
    ACE_TEST_EXCEPTION_RETURN (ACE_OS::memcmp (ptr, cmpvalues, chunk_size) != 0,
                               "  calloc call must set the memory content as expected\n");
    ptrs.push_back (ptr);
  }

  pool_sum = alloc.pool_sum();
  ACE_TEST_EXCEPTION_RETURN (pool_sum <= old_pool_sum,
                             "  pool sum must greater than old pool sum after alloc many chunks for deeply test\n");

  for (size_t i = 0; i < ptrs.size (); ++i)
  {
    alloc.free (ptrs[i]);
  }

  pool_depth = alloc.pool_depth();
  ACE_TEST_EXCEPTION_RETURN (pool_depth != pool_sum,
                             "  pool depth must equal to pool sum after all chunks has been freed for deeply test\n");

  for (long i = 0; i < totalAllocSum; ++i, ++initial_cmp_value)
  {
    ACE_OS::memset (cmpvalues, initial_cmp_value, chunk_size);
    ptr = alloc.calloc (nbytes, initial_cmp_value);
    ACE_TEST_EXCEPTION_RETURN (ptr == nullptr,
                               "  pool must return valid ptr for deeply calloc api test2 with normal nbytes\n");
    ACE_TEST_EXCEPTION_RETURN (ACE_OS::memcmp (ptr, cmpvalues, chunk_size) != 0,
                               "  deeply calloc api test2 must set the memory content as expected\n");
    alloc.free (ptr);
  }

  return 0;
}

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Allocator_Cascaded_Test"));

  int retval = 0;

  // Run the tests for each type of ordering.
  retval = run_cascaded_allocator_test ();

  ACE_END_TEST;

  return retval;
}
