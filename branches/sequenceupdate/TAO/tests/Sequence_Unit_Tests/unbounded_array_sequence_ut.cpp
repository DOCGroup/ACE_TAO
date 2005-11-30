/**
 * @file
 *
 * @brief Unit test for unbounded sequences of arrays.
 *
 * $Id$
 *
 * @author Carlos O'Ryan
 */

#include "mock_array.hpp"
#include "testing_allocation_traits.hpp"
#include "testing_range_checking.hpp"

#include "tao/Unbounded_Array_Sequence_T.h"

#include <boost/test/unit_test.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

using namespace boost::unit_test_framework;
using namespace TAO;

struct Tester
{
  typedef unbounded_array_sequence<my_array, my_array_slice, my_array_tag> tested_sequence;
  typedef tested_sequence::value_type value_type;
  typedef tested_sequence::const_value_type const_value_type;

  typedef tested_sequence::element_traits tested_element_traits;
  typedef tested_sequence::allocation_traits tested_allocation_traits;
  typedef TAO::details::range_checking<value_type,true> range;

  void test_default_constructor()
  {
    {
      tested_sequence x;

      BOOST_CHECK_EQUAL(CORBA::ULong(0), x.maximum());
      BOOST_CHECK_EQUAL(CORBA::ULong(0), x.length());
      BOOST_CHECK_EQUAL(true, x.release());
    }
  }

  void add_all(test_suite * ts)
  {
    boost::shared_ptr<Tester> shared_this(self_);
    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_default_constructor,
                shared_this));
  }
  static boost::shared_ptr<Tester> allocate()
  {
    boost::shared_ptr<Tester> ptr(new Tester);
    ptr->self_ = ptr;

    return ptr;
  }

private:
  Tester() {}

  boost::weak_ptr<Tester> self_;
};

test_suite *
init_unit_test_suite(int, char*[])
{
  test_suite * ts =
      BOOST_TEST_SUITE("unbounded array sequence unit test");

  boost::shared_ptr<Tester> tester(Tester::allocate());
  tester->add_all(ts);

  return ts;
}

