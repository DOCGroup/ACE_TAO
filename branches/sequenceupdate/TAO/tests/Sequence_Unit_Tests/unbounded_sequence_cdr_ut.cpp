/**
 * @file
 *
 * @brief Unit test for unbounded sequences of object references.
 *
 * $Id$
 *
 * @author Carlos O'Ryan
 */
#include "testing_object_reference_traits.hpp"
#include "tao/object_reference_traits.hpp"
#include "testing_allocation_traits.hpp"
#include "testing_range_checking.hpp"

#include "mock_reference.hpp"
#include "mock_stream.hpp"

#include "tao/unbounded_value_sequence.hpp"
#include "tao/unbounded_object_reference_sequence.hpp"
#include "tao/unbounded_sequence_cdr.hpp"
#include "tao/CDR.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

using namespace boost::unit_test_framework;
using namespace TAO;

typedef unbounded_object_reference_sequence<mock_reference, mock_reference_var> tested_sequence;

CORBA::Boolean operator<< (TAO_OutputCDR &strm, const tested_sequence &sequence)
{
  return TAO::insert_sequence(strm, sequence);
}

CORBA::Boolean operator>> (TAO_InputCDR &strm, tested_sequence &sequence)
{
  return TAO::extract_sequence(strm, sequence);
}

struct Tester
{
  typedef tested_sequence::value_type value_type;
  typedef tested_sequence::const_value_type const_value_type;

  typedef tested_sequence::element_traits tested_element_traits;
  typedef tested_sequence::allocation_traits tested_allocation_traits;
  typedef TAO::details::range_checking<value_type,true> range;

  value_type * alloc_and_init_buffer()
  {
    value_type * buf = tested_sequence::allocbuf(8);
    buf[0] = mock_reference::allocate(1);
    buf[1] = mock_reference::allocate(4);
    buf[2] = mock_reference::allocate(9);
    buf[3] = mock_reference::allocate(16);

    return buf;
  }

  void check_values(tested_sequence const & a)
  {
    BOOST_CHECK_EQUAL( 1, a[0]->id());
    BOOST_CHECK_EQUAL( 4, a[1]->id());
    BOOST_CHECK_EQUAL( 9, a[2]->id());
    BOOST_CHECK_EQUAL(16, a[3]->id());
  }

  void test_stream()
  {
    value_type * buffer = alloc_and_init_buffer();

    expected_calls s(mock_reference::serialize_calls);
    {
      tested_sequence a;
      a.replace(8, 4, buffer, false);

      BOOST_CHECK_EQUAL(CORBA::ULong(8), a.maximum());
      BOOST_CHECK_EQUAL(CORBA::ULong(4), a.length());
      BOOST_CHECK_EQUAL(buffer, a.get_buffer());
      BOOST_CHECK_EQUAL(false, a.release());
      check_values(a);

      TAO_OutputCDR stream;
      stream << a;
      BOOST_CHECK_MESSAGE(s.expect(4), s);
    }
    tested_sequence::freebuf(buffer);
  }

  void add_all(test_suite * ts)
  {
    boost::shared_ptr<Tester> shared_this(self_);
    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_stream,
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
      BOOST_TEST_SUITE("unbounded object reference sequence unit test");

  boost::shared_ptr<Tester> tester(Tester::allocate());
  tester->add_all(ts);

  return ts;
}

