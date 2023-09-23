// ============================================================================
// ============================================================================
//
// = LIBRARY
//    tests
//
// = DESCRIPTION
//    This program tests the basic APIs supported in
//    <ACE_Cascaded_Dynamic_Cached_Allocator>, and demonstrates how to use it.
//
// = AUTHOR
//    Smith Achang <changyunlei@126.com>
//
// ============================================================================

#include "ace/Malloc_T.h"
#include "test_config.h"

#define ACE_TEST_EXCEPTION_RETURN(expression, message)   \
do                                                       \
{                                                        \
  if (expression)                                        \
  {                                                      \
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT (message)), 1);\
  }                                                      \
}                                                        \
while (0)

static int
run_free_lock_cascaded_allocator_test ()
{
  ACE_DEBUG ((LM_INFO, "%C begin to run ...\n", __func__));

  const size_t initial_n_chunks  = 1;
  const size_t chunk_size        = sizeof(void*);

  void *ptr, *ptr1, *ptr2;
  size_t nbytes = chunk_size;
  size_t chunk_sum, old_chunk_sum;
  char initial_value = '\0';

  ACE_Cascaded_Dynamic_Cached_Allocator<ACE_SYNCH_MUTEX> alloc (initial_n_chunks, sizeof (void*));
  chunk_sum = alloc.pool_sum ();
  ACE_TEST_EXCEPTION_RETURN (chunk_sum != initial_n_chunks, "  initial pool sum must be initial_n_chunks\n");

  ACE_DEBUG ((LM_INFO, "%C will test unsupported API ...\n", __func__));
  ptr = alloc.calloc (1, sizeof (void*), initial_value);
  ACE_TEST_EXCEPTION_RETURN (ptr != nullptr, "  pool must return nullptr for calloc(size_t n_elem, size_t elem_size, char initial_value) call\n");
  ACE_TEST_EXCEPTION_RETURN(alloc.pool_depth() != initial_n_chunks, "  initial pool depth must keep unchanged for call of unsupported API\n");

  ptr = alloc.malloc(nbytes);
  ACE_TEST_EXCEPTION_RETURN (alloc.pool_depth () != 0, "  initial pool depth must be zero\n");
  alloc.free(ptr);
  ACE_TEST_EXCEPTION_RETURN (alloc.pool_depth () != initial_n_chunks, "  initial pool depth must be initial_n_chunks after free\n");

  ACE_DEBUG ((LM_INFO, "%C will test cascaded allocator ...\n", __func__));
  ptr  = alloc.malloc (nbytes);
  ACE_TEST_EXCEPTION_RETURN (ptr == nullptr, "  pool must return valid ptr, cascaded pool must support to alloc more times firstly\n");

  ptr1 = alloc.malloc (nbytes);
  ACE_TEST_EXCEPTION_RETURN (ptr1 == nullptr, "  pool must return valid ptr, cascaded pool must support to alloc more times secondly\n");
  ACE_TEST_EXCEPTION_RETURN (alloc.pool_depth () != 1, "  cascaded pool depth must support to alloc twice\n");

  old_chunk_sum = chunk_sum;
  chunk_sum     = alloc.pool_sum ();
  ACE_TEST_EXCEPTION_RETURN (chunk_sum < old_chunk_sum, "  cascaded pool sum must be bigger than that of initial pool\n");
  ACE_TEST_EXCEPTION_RETURN (chunk_sum != (3*initial_n_chunks), "  cascaded pool sum must be as expected, pool has been enlarged\n");

  ptr2 = alloc.calloc (nbytes);
  ACE_TEST_EXCEPTION_RETURN (*static_cast<ACE_UINT64*>(ptr2) != 0, "  calloc call will clear the memory to zero\n");

  alloc.free (ptr);
  alloc.free (ptr1);
  alloc.free (ptr2);
  ACE_TEST_EXCEPTION_RETURN (alloc.pool_depth () != (initial_n_chunks + (2 * initial_n_chunks)),
                             "  cascaded pool depth must be three after having freed all malloc ptrs\n");

  return 0;
}

static int
run_free_lock_cascaded_multi_size_based_allocator_test ()
{
  ACE_DEBUG ((LM_INFO, "%C begin to run ...\n", __func__));

  const size_t initial_n_chunks = 2;
  const size_t chunk_size = sizeof (void*);

  void *ptr, *ptr1, *ptr2;
  size_t nbytes = chunk_size;
  size_t chunk_sum, old_chunk_sum;
  char initial_value = '\0';

  ACE_Cascaded_Multi_Size_Based_Allocator<ACE_SYNCH_MUTEX> alloc (initial_n_chunks, sizeof (void*));
  chunk_sum = alloc.pool_sum ();
  ACE_TEST_EXCEPTION_RETURN (chunk_sum != initial_n_chunks, "  initial pool sum must be initial_n_chunks\n");

  ACE_DEBUG ((LM_INFO, "%C will test unsupported API ...\n", __func__));
  ptr = alloc.calloc (1, sizeof (void*), initial_value);
  ACE_TEST_EXCEPTION_RETURN (
    ptr != nullptr, "  pool must return nullptr for calloc(size_t n_elem, size_t elem_size, char initial_value) call\n");
  ACE_TEST_EXCEPTION_RETURN (alloc.pool_depth () != initial_n_chunks,
                             "  initial pool depth must keep unchanged for call of unsupported API\n");

  ptr = alloc.malloc (nbytes);
  ACE_TEST_EXCEPTION_RETURN (alloc.pool_depth () != 0, "  initial pool depth must be zero\n");
  alloc.free (ptr);
  ACE_TEST_EXCEPTION_RETURN (alloc.pool_depth () != initial_n_chunks,
                             "  initial pool depth must be initial_n_chunks after free\n");

  ACE_DEBUG ((LM_INFO, "%C will test cascaded allocator ...\n", __func__));
  ptr = alloc.malloc (nbytes);
  ACE_TEST_EXCEPTION_RETURN (ptr == nullptr,
                             "  pool must return valid ptr, cascaded pool must support to alloc more times firstly\n");

  ptr1 = alloc.malloc (nbytes);
  ACE_TEST_EXCEPTION_RETURN (ptr1 == nullptr,
                             "  pool must return valid ptr, cascaded pool must support to alloc more times secondly\n");
  ACE_TEST_EXCEPTION_RETURN (alloc.pool_depth () != 1, "  cascaded pool depth must support to alloc twice\n");

  old_chunk_sum = chunk_sum;
  chunk_sum = alloc.pool_sum ();
  ACE_TEST_EXCEPTION_RETURN (chunk_sum < old_chunk_sum, "  cascaded pool sum must be bigger than that of initial pool\n");
  ACE_TEST_EXCEPTION_RETURN (chunk_sum != (3 * initial_n_chunks),
                             "  cascaded pool sum must be as expected, pool has been enlarged\n");

  ptr2 = alloc.calloc (nbytes);
  ACE_TEST_EXCEPTION_RETURN (*static_cast<ACE_UINT64*> (ptr2) != 0, "  calloc call will clear the memory to zero\n");

  alloc.free (ptr);
  alloc.free (ptr1);
  alloc.free (ptr2);
  ACE_TEST_EXCEPTION_RETURN (alloc.pool_depth () != (initial_n_chunks + (2 * initial_n_chunks)),
                             "  cascaded pool depth must be three after having freed all malloc ptrs\n");

  return 0;
}

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Allocator_Cascaded_Test"));

  int retval = 0;

  // Run the tests for each type of ordering.
  retval = run_free_lock_cascaded_allocator_test ();

  ACE_END_TEST;

  return retval;
}
