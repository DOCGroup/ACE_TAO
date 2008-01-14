// $Id$

#ifndef ACE_TESTS_STL_ALGORITHM_TEST_T_CPP
#define ACE_TESTS_STL_ALGORITHM_TEST_T_CPP

#include "test_config.h"
#include <algorithm>
#include <typeinfo>

template <typename ITEM>
static void
for_each_callback (ITEM & item)
{
  // do nothing; just testing compilation and execution
  ACE_DEBUG ((LM_DEBUG,
              "algorithm test: for_each_callback\n"));

  ACE_UNUSED_ARG (item);
}

template <typename T>
int test_STL_algorithm (T & container)
{
  // We are only validating that the container's iterators compile with
  // the <algorithm> header.
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("running STL algorithm test for `%s'\n"),
              typeid (T).name ()));

  // Test the (reverse) iterator using std::for_each.
  std::for_each (container.begin (),
                 container.end (),
                 &for_each_callback <typename T::value_type>);

  std::for_each (container.rbegin (),
                 container.rend (),
                 &for_each_callback <typename T::value_type>);

  return 0;
}

#endif /* ACE_TESTS_STL_ALGORITHM_TEST_T_CPP */
