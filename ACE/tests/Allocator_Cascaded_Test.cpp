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

#include "test_config.h"
#include "ace/Malloc_T.h"

#define ACE_TEST_EXCEPTION_RETURN(expression, message)        \
do                                                            \
{                                                             \
    if (expression)                                           \
    {                                                         \
        ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT (message)), 1); \
    }                                                         \
}                                                             \
while (0)

static int
run_free_lock_cascaded_allocator_test ()
{
   ACE_DEBUG ((LM_INFO, "%C begin to run ...\n", __func__));

   void *ptr, *ptr1, *ptr2;
   size_t initial_n_chunks  = 1;
   size_t chunk_size        = sizeof(void*);

   ACE_Cascaded_Dynamic_Cached_Allocator<ACE_Null_Mutex> alloc (initial_n_chunks, sizeof (void*));

   ptr = alloc.calloc (1, sizeof(void*));

   ACE_TEST_EXCEPTION_RETURN (ptr != nullptr, "  pool must return nullptr for ccalloc(size_t n_elem, size_t elem_size, char initial_value) call\n");

   ACE_TEST_EXCEPTION_RETURN(alloc.pool_depth() != initial_n_chunks, "  initial pool depth must be one\n");

   size_t nbytes = chunk_size;

   ptr = alloc.malloc(nbytes);
   ACE_TEST_EXCEPTION_RETURN (alloc.pool_depth () != 0, "  initial pool depth must be zero\n");
   alloc.free(ptr);
   ACE_TEST_EXCEPTION_RETURN (alloc.pool_depth () != initial_n_chunks, "  initial pool depth must be one after free\n");

   ACE_DEBUG ((LM_INFO, "%C will test cascaded allocator ...\n", __func__));
   ptr = alloc.malloc (nbytes);
   ptr1 = alloc.malloc (nbytes);
   ACE_TEST_EXCEPTION_RETURN (alloc.pool_depth () != 1, "  cascaded pool depth must can support to alloc twice\n");


   ptr2 = alloc.calloc (nbytes);
   ACE_TEST_EXCEPTION_RETURN (*static_cast<ACE_UINT64*>(ptr2) != 0, "  calloc call will clear the memory to zero\n");

   alloc.free (ptr);
   alloc.free (ptr1);
   alloc.free (ptr2);
   ACE_TEST_EXCEPTION_RETURN (alloc.pool_depth () != (initial_n_chunks + 2 * initial_n_chunks),
                              "  cascaded pool depth must be three after freed all malloc ptrs\n");


   return 0;
}

int
run_main (int, ACE_TCHAR *argv[])
{
  ACE_UNUSED_ARG (argv);

  ACE_START_TEST (ACE_TEXT ("ACE_Cascaded_Dynamic_Cached_Allocator_Test"));

  int retval = 0;


  // Run the tests for each type of ordering.
  retval = run_free_lock_cascaded_allocator_test ();


  ACE_END_TEST;


  return retval;
}
