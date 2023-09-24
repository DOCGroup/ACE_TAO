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
#include <vector>
#include <iostream>
#include <sstream>

#define ACE_TEST_EXCEPTION_RETURN(expression, message)   \
do                                                       \
{                                                        \
  if (expression)                                        \
  {                                                      \
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT (message)), 1);\
  }                                                      \
}                                                        \
while (0)

#define ACE_ASSERT_RETURN(expression, message)            \
do                                                        \
{                                                         \
  if (!(expression))                                      \
  {                                                       \
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT (message)), 1); \
  }                                                       \
}                                                         \
while (0)

#define DELTA(level, initial_n_chunks, min_initial_n_chunks) \
  (initial_n_chunks >> level) > min_initial_n_chunks ? (initial_n_chunks >> level) : min_initial_n_chunks

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
run_cascaded_multi_size_based_allocator_basic_test ()
{
  ACE_DEBUG ((LM_INFO, "%C begin to run ...\n", __func__));

  const size_t initial_n_chunks = 11;
  const size_t min_initial_n_chunks = 2;
  const size_t chunk_size = sizeof (void*) + 5;
  const size_t nbytes = chunk_size;

  std::vector<void*> ptrs;
  void *ptr;
  size_t pool_sum, pool_depth;

  const char initial_value = '\0';

  ACE_Cascaded_Multi_Size_Based_Allocator<ACE_SYNCH_MUTEX> alloc (initial_n_chunks, chunk_size, min_initial_n_chunks);
  pool_sum = alloc.pool_sum ();
  ACE_ASSERT_RETURN (pool_sum == initial_n_chunks, "  initial pool sum must be initial_n_chunks\n");

  ACE_DEBUG ((LM_INFO, "%C will test unsupported API ...\n", __func__));
  ptr = alloc.calloc (1, sizeof (void*), initial_value);
  ACE_ASSERT_RETURN (ptr == nullptr,
                             "  pool must return nullptr for calloc(size_t n_elem, size_t elem_size, char initial_value) call\n");

  pool_depth = alloc.pool_depth ();
  ACE_ASSERT_RETURN (pool_depth == initial_n_chunks,
                             "  initial pool depth must keep unchanged for call of unsupported API\n");

  ptr = alloc.malloc (nbytes);
  pool_depth = alloc.pool_depth ();
  ACE_ASSERT_RETURN (pool_depth == (initial_n_chunks - 1),
                     "  initial pool depth must decrease by one\n");
  alloc.free (ptr);
  pool_depth = alloc.pool_depth ();
  ACE_ASSERT_RETURN (pool_depth == initial_n_chunks,
                     "  initial pool depth must restore to initial_n_chunks after free\n");

  ACE_DEBUG ((LM_INFO, "%C will test first level cascaded allocator ...\n", __func__));

  for (size_t i = 0; i < (2 * initial_n_chunks); ++i)
  {
    ptr = alloc.malloc (nbytes);
    ACE_ASSERT_RETURN (ptr != nullptr,
                       "  pool must return valid ptr, cascaded pool must support to alloc more times firstly\n");
    ptrs.push_back (ptr);
  }

  pool_sum = alloc.pool_sum ();
  ACE_ASSERT_RETURN (pool_sum == (3 * initial_n_chunks),
                     "  cascaded pool only has two levels, so the pool sum must be 3*initial_n_chunks after alloced 2 * initial_n_chunks times\n");

  pool_depth = alloc.pool_depth ();
  ACE_ASSERT_RETURN (pool_depth == (initial_n_chunks),
                     "  cascaded pool only has two levels, so the pool depth must be initial_n_chunks after alloced 2 * initial_n_chunks times\n");

  for (size_t i = 0; i < ptrs.size(); ++i)
  {
    alloc.free (ptrs[i]);
  }

  pool_sum = alloc.pool_sum ();
  ACE_ASSERT_RETURN (pool_sum == (3 * initial_n_chunks),
                     "  first size-based cascaded allocator only has two levels, so the pool sum must be 3*initial_n_chunks after all freed\n");

  pool_depth = alloc.pool_depth ();
  ACE_ASSERT_RETURN (pool_depth == (3 * initial_n_chunks),
                     "  first size-based cascaded allocator only has two levels, so the pool depth must be initial_n_chunks after all freed\n");

  return 0;
}

static int
run_cascaded_multi_size_based_allocator_hierarchy_test ()
{
  ACE_DEBUG ((LM_INFO, "%C begin to run ...\n", __func__));

  const size_t initial_n_chunks = 11;
  const size_t min_initial_n_chunks = 2;
  const size_t chunk_size = sizeof (void*) + 5;

  void *ptr;
  size_t pool_sum, old_pool_sum, pool_depth, old_pool_depth;
  size_t level  = 0, delta;
  size_t nbytes = chunk_size;


  ACE_Cascaded_Multi_Size_Based_Allocator<ACE_SYNCH_MUTEX> alloc (initial_n_chunks, chunk_size, min_initial_n_chunks);
  ACE_DEBUG ((LM_INFO, "%C Only test the basic malloc API  ...\n", __func__));
  ptr = alloc.malloc (nbytes);
  ACE_ASSERT_RETURN (ptr != nullptr,
                     "  pool must return valid ptr when requesting initial chunk_size\n");
  alloc.free (ptr);

  ACE_DEBUG ((LM_INFO, "%C Will trigger the creation of nested allocator on next level  ...\n", __func__));
  level = 1;
  old_pool_sum   = alloc.pool_sum ();
  old_pool_depth = alloc.pool_depth ();
  nbytes         = chunk_size << level;
  ptr            = alloc.malloc (nbytes );
  ACE_ASSERT_RETURN (ptr != nullptr,
                     "  pool must return valid ptr when requesting 2 * chunk_size\n");

  pool_sum   = alloc.pool_sum ();
  delta      = DELTA (level, initial_n_chunks, min_initial_n_chunks);
  ACE_ASSERT_RETURN (pool_sum == (old_pool_sum + delta),
                     "  pool sum must increase as delta\n");

  alloc.free (ptr);
  pool_depth = alloc.pool_depth ();
  ACE_ASSERT_RETURN (pool_depth == (old_pool_depth + delta),
                     "  pool depth must increase as delta\n");

  ACE_DEBUG ((LM_INFO, "%C Will trigger the creation of allocator on more lowwer level  ...\n", __func__));
  level = 11;
  old_pool_sum   = alloc.pool_sum ();
  old_pool_depth = alloc.pool_depth ();
  nbytes         = chunk_size << level;
  ptr            = alloc.malloc (nbytes);
  ACE_ASSERT_RETURN (ptr != nullptr,
                     "  pool must return valid ptr when requesting chunk_size << 11\n");

  pool_sum = alloc.pool_sum ();
  delta    = DELTA (level, initial_n_chunks, min_initial_n_chunks);
  ACE_ASSERT_RETURN (pool_sum == (old_pool_sum + delta),
                    "  pool sum must increase as delta only created request level\n");

  alloc.free (ptr);
  pool_depth = alloc.pool_depth ();
  ACE_ASSERT_RETURN (pool_depth == (old_pool_depth + delta),
                     "  pool depth must increase as delta only created request level\n");

  for (size_t i = 2; i < level; ++i)
  {
    std::stringstream ss;
    old_pool_sum    = alloc.pool_sum ();
    old_pool_depth  = alloc.pool_depth ();
    nbytes          = chunk_size << i;
    ptr             = alloc.malloc (nbytes);
    ss << "  pool must return valid ptr when requesting chunk_size: << " << nbytes << std::endl;
    ACE_ASSERT_RETURN (ptr != nullptr, ACE_TEXT_CHAR_TO_TCHAR(ss.str().c_str()));

    ss.str ("");
    pool_sum = alloc.pool_sum ();
    delta = DELTA (i, initial_n_chunks, min_initial_n_chunks);
    ss << "  pool sum must increase as delta: " << delta << " because only created request level: " << i  << std::endl;
    ACE_ASSERT_RETURN (pool_sum == (old_pool_sum + delta), ACE_TEXT_CHAR_TO_TCHAR(ss.str().c_str()));

    ss.str ("");
    alloc.free (ptr);
    pool_depth = alloc.pool_depth ();
    ss << "  pool depth must increase as delta: " << delta << " because only created request level: " << i << std::endl;
    ACE_ASSERT_RETURN (pool_depth == (old_pool_depth + delta), ACE_TEXT_CHAR_TO_TCHAR(ss.str().c_str()));
  }

  return 0;
}

static int
run_cascaded_multi_size_based_allocator_hierarchy_free_test ()
{
  ACE_DEBUG ((LM_INFO, "%C begin to run ...\n", __func__));

  const size_t initial_n_chunks = 11;
  const size_t min_initial_n_chunks = 2;
  const size_t chunk_size = sizeof (void*) + 5;

  void* ptr;
  size_t level  = 3;
  size_t nbytes = chunk_size << level;
  std::stringstream ss;

  ACE_Cascaded_Multi_Size_Based_Allocator<ACE_SYNCH_MUTEX> alloc (initial_n_chunks, chunk_size, min_initial_n_chunks);
  ACE_DEBUG ((LM_INFO, "%C Only test the basic malloc API  ...\n", __func__));
  ptr = alloc.malloc (nbytes);
  ss << "  level: " << level << " size-based cascaded allocator must return valid ptr when requesting normal chunk_size: " << nbytes << std::endl;
  ACE_ASSERT_RETURN (ptr != nullptr, ACE_TEXT_CHAR_TO_TCHAR(ss.str().c_str()));
  alloc.free (ptr);

  for (size_t i = 3; i < 6; ++i)
  {
    size_t pool_sum, old_pool_sum, pool_depth, old_pool_depth;
    level = i;
    ACE_DEBUG ((LM_INFO, "%C test level: %u size-based cascaded allocator ...\n", __func__, level));
    nbytes = chunk_size << level;
    ptr    = alloc.malloc (nbytes);
    ss.str ("");
    ss << "  level: " << level
       << " size-based cascaded allocator must return valid ptr when requesting normal chunk_size: " << nbytes
       << std::endl;
    ACE_ASSERT_RETURN (ptr != nullptr, ACE_TEXT_CHAR_TO_TCHAR(ss.str().c_str()));
    alloc.free (ptr);

    ACE_DEBUG ((LM_INFO, "%C test free pos API for level: %u ...\n", __func__, level));
    old_pool_depth = alloc.pool_depth ();
    old_pool_sum   = alloc.pool_sum ();
    std::vector<void*> ptrs;
    size_t delta = DELTA (level, initial_n_chunks, min_initial_n_chunks);
    for (size_t j = 0; j < delta; ++j)
    {
      ptr = alloc.malloc (nbytes);
      ss.str ("");
      ss << "  level: " << level
         << " size-based cascaded allocator must return valid ptr when requesting normal chunk_size: " << nbytes
         << " at loop: " << j << std::endl;
      ACE_ASSERT_RETURN (ptr != nullptr, ACE_TEXT_CHAR_TO_TCHAR(ss.str().c_str()));
      ptrs.push_back (ptr);
    }

    for (size_t k = 0; k < ptrs.size (); ++k)
      alloc.free (ptrs[k]);

    ptrs.clear ();

    ACE_DEBUG ((LM_INFO, "%C test level: %u size-based cascaded allocator has freed all initial chunks, when alloc again, the pool sum must not changed ...\n", __func__, level));
    ptr = alloc.malloc (nbytes);
    pool_depth = alloc.pool_depth ();
    pool_sum   = alloc.pool_sum ();
    ss.str ("");
    ss << "  level: " << level << " pool depth: " << old_pool_depth  <<" must keep unchanged" << std::endl;
    ACE_ASSERT_RETURN (old_pool_depth == (pool_depth + 1), ACE_TEXT_CHAR_TO_TCHAR(ss.str().c_str()));

    ss.str ("");
    ss << "  level: " << level << " pool sum: " << old_pool_sum << " must keep unchanged" << std::endl;
    ACE_ASSERT_RETURN (old_pool_sum == pool_sum, ACE_TEXT_CHAR_TO_TCHAR(ss.str().c_str()));
    alloc.free (ptr);
  }

  return 0;
}

static int
run_cascaded_multi_size_based_allocator_hierarchy_differential_test ()
{
  ACE_DEBUG ((LM_INFO, "%C begin to run ...\n", __func__));

  const size_t initial_n_chunks = 11;
  const size_t min_initial_n_chunks = 2;
  const size_t chunk_size = sizeof (void*) + 5;

  ACE_Cascaded_Multi_Size_Based_Allocator<ACE_SYNCH_MUTEX> alloc (initial_n_chunks, chunk_size, min_initial_n_chunks);
  ACE_DEBUG ((LM_INFO, "%C Only test the hierarchy differential ...\n", __func__));

  for (size_t i = 3; i < 6; ++i)
  {
    size_t pool_sum, old_pool_sum, pool_depth, old_pool_depth, delta;
    std::stringstream ss;
    size_t level = i;
    old_pool_depth = alloc.pool_depth ();
    old_pool_sum   = alloc.pool_sum ();

    ACE_DEBUG ((LM_INFO, "%C test level: %u size-based cascaded allocator ...\n", __func__, level));
    size_t nbytes = chunk_size << level;
    void* ptr = alloc.malloc (nbytes);
    ss.str ("");
    ss << "  level: " << level
       << " size-based cascaded allocator must return valid ptr when requesting normal chunk_size: " << nbytes
       << std::endl;
    ACE_ASSERT_RETURN (ptr != nullptr, ACE_TEXT_CHAR_TO_TCHAR(ss.str().c_str()));

    if (i == 3)
    {
      pool_depth = alloc.pool_depth ();
      pool_sum   = alloc.pool_sum ();
      delta      = DELTA (level, initial_n_chunks, min_initial_n_chunks);

      ss.str ("");
      ss << "  level: " << (level) << " must be created, pool depth must increased by " << delta << std::endl;
      ACE_ASSERT_RETURN ((old_pool_depth + delta - 1) == pool_depth, ACE_TEXT_CHAR_TO_TCHAR(ss.str().c_str()));

      ss.str ("");
      ss << "  level: " << (level) << " must be created, pool sum must increased by " << delta << std::endl;
      ACE_ASSERT_RETURN ((old_pool_sum + delta) == pool_sum, ACE_TEXT_CHAR_TO_TCHAR(ss.str().c_str()));
      alloc.free (ptr);
    }

    ACE_DEBUG ((LM_INFO, "%C test alloc bytes greater than level: %u ...\n", __func__, level));
    old_pool_depth = alloc.pool_depth ();
    old_pool_sum   = alloc.pool_sum ();

    const size_t next_level = level + 1;
    delta = DELTA (next_level, initial_n_chunks, min_initial_n_chunks);
    ptr = alloc.malloc (nbytes + 1);
    alloc.free (ptr);
    pool_depth = alloc.pool_depth ();
    pool_sum   = alloc.pool_sum ();

    ss.str ("");
    ss << "  next level: " << next_level << " must be created, pool depth must increased by " << delta << std::endl;
    ACE_ASSERT_RETURN ((old_pool_depth + delta) == pool_depth, ACE_TEXT_CHAR_TO_TCHAR(ss.str().c_str()));

    ss.str ("");
    ss << "  next level: " << next_level << " must be created, pool sum must increased by " << delta << std::endl;
    ACE_ASSERT_RETURN ((old_pool_sum + delta) == pool_sum, ACE_TEXT_CHAR_TO_TCHAR(ss.str().c_str()));

    const size_t next_nbytes = chunk_size << next_level;
    old_pool_depth = alloc.pool_depth ();
    old_pool_sum   = alloc.pool_sum ();
    std::vector<void*> ptrs;
    for (size_t j = nbytes + 1; j < next_nbytes; ++j)
    {
      ptr = alloc.malloc (j);
      ss.str ("");
      ss << "  level: " << next_level
         << " size-based cascaded allocator must return valid ptr when requesting normal chunk_size: " << j
         << std::endl;
      ACE_ASSERT_RETURN (ptr != nullptr, ACE_TEXT_CHAR_TO_TCHAR(ss.str().c_str()));
      ptrs.push_back (ptr);
    }

    for (size_t k = 0; k < ptrs.size(); ++k)
    {
      alloc.free (ptrs[k]);
    }
    ptrs.clear ();

    ss.str ("");
    ss << "  next level: " << next_level << " pool depth must unchanged" << std::endl;
    ACE_ASSERT_RETURN ((old_pool_depth) == pool_depth, ACE_TEXT_CHAR_TO_TCHAR(ss.str().c_str()));

    ss.str ("");
    ss << "  next level: " << next_level << " pool sum must unchanged" << std::endl;
    ACE_ASSERT_RETURN ((old_pool_sum) == pool_sum, ACE_TEXT_CHAR_TO_TCHAR(ss.str().c_str()));
  }

  return 0;
}

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Allocator_Cascaded_Test"));

  int retval = 0;

  ACE_DEBUG ((LM_INFO, "%C Run the tests for Cascaded_Allocator ...\n", __func__));
  retval += run_free_lock_cascaded_allocator_test();

  ACE_DEBUG ((LM_INFO, "%C Run the tests for Cascaded_Multi_Size_Based_Allocator ...\n", __func__));
  retval += run_cascaded_multi_size_based_allocator_basic_test();
  retval += run_cascaded_multi_size_based_allocator_hierarchy_test ();
  retval += run_cascaded_multi_size_based_allocator_hierarchy_free_test ();
  retval += run_cascaded_multi_size_based_allocator_hierarchy_differential_test ();

  ACE_END_TEST;

  return retval;
}
