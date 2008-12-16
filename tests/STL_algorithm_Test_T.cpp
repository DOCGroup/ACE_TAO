// $Id$

#ifndef ACE_TESTS_STL_ALGORITHM_TEST_T_CPP
#define ACE_TESTS_STL_ALGORITHM_TEST_T_CPP

#include "test_config.h"
#include <algorithm>
#include <typeinfo>

template <typename T>
class Element_Counter
{
public:
  Element_Counter (void)
    : count_ (0)
  {

  }

  void operator () (typename T::value_type & item)
  {
    ++ this->count_;
    ACE_UNUSED_ARG (item);
  }

  const Element_Counter & operator = (const Element_Counter & ec)
  {
    this->count_ = ec.count_;
    return *this;
  }

  typename T::difference_type get_count (void) const
  {
    return this->count_;
  }

private:
  // Number of elements iterated over.
  typename T::difference_type count_;
};

template <typename T>
int test_STL_algorithm (T & container)
{
  // We are only validating that the container's iterators
  // compile with the <algorithm> header.
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("running STL algorithm test for `%s'\n"),
              typeid (T).name ()));

  // Test the forward iterator using std::for_each.
  ACE_DEBUG ((LM_DEBUG,
              "testing forward iterator\n"));

  typename T::difference_type count =
    std::for_each (container.begin (),
                   container.end (),
                   Element_Counter <T> ()).get_count ();

  ACE_DEBUG ((LM_DEBUG,
              "std::for_each handled %d elements\n",
              count));

  // Test the reverse iterator using std::for_each.
  ACE_DEBUG ((LM_DEBUG,
              "testing reverse iterator\n"));

  count =
    std::for_each (container.rbegin (),
                   container.rend (),
                   Element_Counter <T> ()).get_count ();

  ACE_DEBUG ((LM_DEBUG,
              "std::for_each handled %d elements\n",
              count));

  return 0;
}

#endif /* ACE_TESTS_STL_ALGORITHM_TEST_T_CPP */
