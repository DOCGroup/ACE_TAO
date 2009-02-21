/**
 * @file
 *
 * @brief Unit test for unbounded sequences of octet types
 *
 * $Id$
 *
 * @author Johnny Willemsen
 */

#include <boost/test/unit_test.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include "testing_allocation_traits.hpp"
#include "testing_range_checking.hpp"

#include "tao/Unbounded_Value_Sequence_T.h"

#include "value_sequence_tester.hpp"

#include "tao/Basic_Types.h"
#include "tao/CDR.h"

using namespace TAO_VERSIONED_NAMESPACE_NAME::TAO;
using namespace boost::unit_test_framework;

typedef unbounded_value_sequence<CORBA::Octet> tested_sequence;
typedef tested_sequence::element_traits tested_element_traits;
typedef tested_sequence::allocation_traits tested_allocation_traits;
typedef details::range_checking<CORBA::Octet,true> range;

struct Tester
{
  typedef tested_sequence::value_type value_type;

  void test_copy_constructor_from_ulong()
  {
    expected_calls a(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    {
      tested_sequence x(16);
      BOOST_CHECK_MESSAGE(a.expect(1), a);
      x.length(8);

      BOOST_CHECK_EQUAL(CORBA::ULong(16), x.maximum());
      BOOST_CHECK_EQUAL(CORBA::ULong(8), x.length());
      BOOST_CHECK_EQUAL(true, x.release());

      tested_sequence y(x);
      BOOST_CHECK_MESSAGE(a.expect(1), a);
      BOOST_CHECK_EQUAL(CORBA::ULong(16), y.maximum());
      BOOST_CHECK_EQUAL(CORBA::ULong(8), y.length());
      BOOST_CHECK_EQUAL(true, y.release());
    }
    BOOST_CHECK_MESSAGE(f.expect(2), f);
  }

  void test_assignment_from_ulong()
  {
    expected_calls a(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    {
      tested_sequence x(16);
      x.length(8);
      BOOST_CHECK_MESSAGE(a.expect(1), a);
      BOOST_CHECK_EQUAL(CORBA::ULong(16), x.maximum());
      BOOST_CHECK_EQUAL(CORBA::ULong(8), x.length());
      BOOST_CHECK_EQUAL(true, x.release());

      tested_sequence y;
      BOOST_CHECK_MESSAGE(a.expect(0),  a);

      y = x;
      BOOST_CHECK_MESSAGE(a.expect(1), a);
      BOOST_CHECK_MESSAGE(f.expect(0), f);
      BOOST_CHECK_EQUAL(CORBA::ULong(16), y.maximum());
      BOOST_CHECK_EQUAL(CORBA::ULong(8), y.length());
      BOOST_CHECK_EQUAL(true, y.release());
    }
    BOOST_CHECK_MESSAGE(f.expect(2), f);
  }

  void test_ulong_constructor()
  {
    expected_calls a(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    {
      tested_sequence x(16);

      BOOST_CHECK_EQUAL(CORBA::ULong(16), x.maximum());
      BOOST_CHECK_EQUAL(CORBA::ULong(0), x.length());
      BOOST_CHECK_EQUAL(true, x.release());
    }
    BOOST_CHECK_MESSAGE(a.expect(1), a);
    BOOST_CHECK_MESSAGE(f.expect(1), f);
  }

  void test_exception_in_ulong_constructor()
  {
    expected_calls a(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    {
      tested_allocation_traits::allocbuf_calls.failure_countdown(1);
      BOOST_CHECK_THROW(tested_sequence x(16), testing_exception);
      BOOST_CHECK_MESSAGE(a.expect(1), a);
    }
    BOOST_CHECK_MESSAGE(f.expect(0), f);
  }

  void test_set_length_less_than_maximum()
  {
    expected_calls a(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    {
      tested_sequence x(16);

      x.length(8);
      BOOST_CHECK_EQUAL(CORBA::ULong(16), x.maximum());
      BOOST_CHECK_EQUAL(CORBA::ULong(8), x.length());
      BOOST_CHECK_EQUAL(true, x.release());
    }
    BOOST_CHECK_MESSAGE(a.expect(1), a);
    BOOST_CHECK_MESSAGE(f.expect(1), f);
  }

  void test_set_length_more_than_maximum()
  {
    expected_calls a(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    {
      tested_sequence x(16);
      BOOST_CHECK_MESSAGE(a.expect(1), a);

      x.length(32);
      BOOST_CHECK_MESSAGE(a.expect(1), a);
      BOOST_CHECK_MESSAGE(f.expect(1), f);

      BOOST_CHECK_EQUAL(CORBA::ULong(32), x.maximum());
      BOOST_CHECK_EQUAL(CORBA::ULong(32), x.length());
      BOOST_CHECK_EQUAL(true, x.release());
    }
    BOOST_CHECK_MESSAGE(f.expect(1), f);
  }

  void test_exception_in_set_length()
  {
    expected_calls f(tested_allocation_traits::freebuf_calls);
    {
      tested_sequence x;

      expected_calls a(tested_allocation_traits::allocbuf_calls);
      tested_allocation_traits::allocbuf_calls.failure_countdown(1);
      BOOST_CHECK_THROW(x.length(8), testing_exception);
      BOOST_CHECK_MESSAGE(a.expect(1), a);
    }
    BOOST_CHECK_MESSAGE(f.expect(0), f);
  }

  value_type * alloc_and_init_buffer()
  {
    value_type * buf = tested_sequence::allocbuf(8);
    buf[0] = 1; buf[1] = 4; buf[2] = 9; buf[3] = 16;

    return buf;
  }

  void test_buffer_constructor_default()
  {
    value_type * buffer = alloc_and_init_buffer();

    expected_calls a(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    {
      tested_sequence a(8, 4, buffer);
      BOOST_CHECK_EQUAL(CORBA::ULong(8), a.maximum());
      BOOST_CHECK_EQUAL(CORBA::ULong(4), a.length());
      BOOST_CHECK_EQUAL(buffer, a.get_buffer());
      BOOST_CHECK_EQUAL(CORBA::Octet( 1), a[0]);
      BOOST_CHECK_EQUAL(CORBA::Octet( 4), a[1]);
      BOOST_CHECK_EQUAL(CORBA::Octet( 9), a[2]);
      BOOST_CHECK_EQUAL(CORBA::Octet(16), a[3]);
      BOOST_CHECK_EQUAL(false, a.release());
    }
    BOOST_CHECK_MESSAGE(a.expect(0), a);
    BOOST_CHECK_MESSAGE(f.expect(0), f);
    tested_sequence::freebuf(buffer);
  }

  void test_buffer_constructor_false()
  {
    value_type * buffer = alloc_and_init_buffer();
    expected_calls a(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    {
      tested_sequence a(8, 4, buffer, false);
      BOOST_CHECK_EQUAL(CORBA::ULong(8), a.maximum());
      BOOST_CHECK_EQUAL(CORBA::ULong(4), a.length());
      BOOST_CHECK_EQUAL(buffer, a.get_buffer());
      BOOST_CHECK_EQUAL(CORBA::Octet( 1), a[0]);
      BOOST_CHECK_EQUAL(CORBA::Octet( 4), a[1]);
      BOOST_CHECK_EQUAL(CORBA::Octet( 9), a[2]);
      BOOST_CHECK_EQUAL(CORBA::Octet(16), a[3]);
      BOOST_CHECK_EQUAL(false, a.release());
    }
    BOOST_CHECK_MESSAGE(a.expect(0), a);
    BOOST_CHECK_MESSAGE(f.expect(0), f);
    tested_sequence::freebuf(buffer);
  }

  void test_buffer_constructor_true()
  {
    value_type * buffer = alloc_and_init_buffer();
    expected_calls a(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    {
      tested_sequence a(8, 4, buffer, true);
      BOOST_CHECK_EQUAL(CORBA::ULong(8), a.maximum());
      BOOST_CHECK_EQUAL(CORBA::ULong(4), a.length());
      BOOST_CHECK_EQUAL(buffer, a.get_buffer());
      BOOST_CHECK_EQUAL(CORBA::Octet( 1), a[0]);
      BOOST_CHECK_EQUAL(CORBA::Octet( 4), a[1]);
      BOOST_CHECK_EQUAL(CORBA::Octet( 9), a[2]);
      BOOST_CHECK_EQUAL(CORBA::Octet(16), a[3]);
      BOOST_CHECK_EQUAL(true, a.release());
    }
    BOOST_CHECK_MESSAGE(a.expect(0), a);
    BOOST_CHECK_MESSAGE(f.expect(1), f);
  }

  void test_replace_default()
  {
    value_type * buffer = alloc_and_init_buffer();

    expected_calls c(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    {
      tested_sequence a;
      a.replace(8, 4, buffer);
      BOOST_CHECK_MESSAGE(c.expect(0), c);
      BOOST_CHECK_MESSAGE(f.expect(0), f);

      BOOST_CHECK_EQUAL(CORBA::ULong(8), a.maximum());
      BOOST_CHECK_EQUAL(CORBA::ULong(4), a.length());
      BOOST_CHECK_EQUAL(buffer, a.get_buffer());
      BOOST_CHECK_EQUAL(CORBA::Octet( 1), a[0]);
      BOOST_CHECK_EQUAL(CORBA::Octet( 4), a[1]);
      BOOST_CHECK_EQUAL(CORBA::Octet( 9), a[2]);
      BOOST_CHECK_EQUAL(CORBA::Octet(16), a[3]);
      BOOST_CHECK_EQUAL(false, a.release());
    }
    BOOST_CHECK_MESSAGE(c.expect(0), c);
    BOOST_CHECK_MESSAGE(f.expect(0), f);
    tested_sequence::freebuf(buffer);
  }

  void test_replace_false()
  {
    value_type * buffer = alloc_and_init_buffer();
    expected_calls c(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);

    {
      tested_sequence a;
      a.replace(8, 4, buffer, false);
      BOOST_CHECK_MESSAGE(c.expect(0), c);
      BOOST_CHECK_MESSAGE(f.expect(0), f);

      BOOST_CHECK_EQUAL(CORBA::ULong(8), a.maximum());
      BOOST_CHECK_EQUAL(CORBA::ULong(4), a.length());
      BOOST_CHECK_EQUAL(buffer, a.get_buffer());
      BOOST_CHECK_EQUAL(CORBA::Octet( 1), a[0]);
      BOOST_CHECK_EQUAL(CORBA::Octet( 4), a[1]);
      BOOST_CHECK_EQUAL(CORBA::Octet( 9), a[2]);
      BOOST_CHECK_EQUAL(CORBA::Octet(16), a[3]);
      BOOST_CHECK_EQUAL(false, a.release());
    }
    BOOST_CHECK_MESSAGE(c.expect(0), c);
    BOOST_CHECK_MESSAGE(f.expect(0), f);
    tested_sequence::freebuf(buffer);
  }

  void test_replace_true()
  {
    value_type * buffer = alloc_and_init_buffer();
    expected_calls c(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);

    {
      tested_sequence a;
      a.replace(8, 4, buffer, true);
      BOOST_CHECK_MESSAGE(c.expect(0), c);
      BOOST_CHECK_MESSAGE(f.expect(0), f);

      BOOST_CHECK_EQUAL(CORBA::ULong(8), a.maximum());
      BOOST_CHECK_EQUAL(CORBA::ULong(4), a.length());
      BOOST_CHECK_EQUAL(buffer, a.get_buffer());
      BOOST_CHECK_EQUAL(CORBA::Octet( 1), a[0]);
      BOOST_CHECK_EQUAL(CORBA::Octet( 4), a[1]);
      BOOST_CHECK_EQUAL(CORBA::Octet( 9), a[2]);
      BOOST_CHECK_EQUAL(CORBA::Octet(16), a[3]);
      BOOST_CHECK_EQUAL(true, a.release());
    }
    BOOST_CHECK_MESSAGE(c.expect(0), c);
    BOOST_CHECK_MESSAGE(f.expect(1), f);
  }

  void test_get_buffer_default()
  {
    value_type * buffer = alloc_and_init_buffer();
    tested_sequence a(8, 4, buffer, true);
    BOOST_CHECK_EQUAL(a.get_buffer(), buffer);
  }

  void test_get_buffer_false()
  {
    value_type * buffer = alloc_and_init_buffer();
    tested_sequence a(8, 4, buffer, true);
    BOOST_CHECK_EQUAL(a.get_buffer(), buffer);
  }

  void test_get_buffer_true_with_release_false()
  {
    value_type * buffer = alloc_and_init_buffer();
    tested_sequence a(8, 4, buffer, false);
    BOOST_CHECK(0 == a.get_buffer(true));
    tested_sequence::freebuf(buffer);
  }

  void test_get_buffer_true_with_release_true()
  {
    value_type * buffer = alloc_and_init_buffer();
    expected_calls c(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    {
      tested_sequence a(8, 4, buffer, true);
      BOOST_CHECK_EQUAL(buffer, a.get_buffer(true));

      tested_sequence const & b = a;
      BOOST_CHECK_EQUAL(0UL, b.maximum());
      BOOST_CHECK_EQUAL(0UL, b.length());
      BOOST_CHECK(0 != b.get_buffer());
      BOOST_CHECK_EQUAL(true, b.release());

      BOOST_CHECK_MESSAGE(c.expect(1), c);

      BOOST_CHECK(buffer != b.get_buffer());
    }
    BOOST_CHECK_MESSAGE(c.expect(0), c);
    BOOST_CHECK_MESSAGE(f.expect(1), f);
    tested_sequence::freebuf(buffer);
  }

  void add_all(test_suite * ts)
  {
    boost::shared_ptr<Tester> shared_this(self_);

    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_ulong_constructor,
                shared_this));

    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_copy_constructor_from_ulong,
                shared_this));

    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_assignment_from_ulong,
                shared_this));

    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_exception_in_ulong_constructor,
                shared_this));

    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_set_length_less_than_maximum,
                shared_this));

    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_set_length_more_than_maximum,
                shared_this));

    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_exception_in_set_length,
                shared_this));

    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_buffer_constructor_default,
                shared_this));
    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_buffer_constructor_false,
                shared_this));
    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_buffer_constructor_true,
                shared_this));
    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_replace_default,
                shared_this));
    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_replace_false,
                shared_this));
    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_replace_true,
                shared_this));
    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_get_buffer_false,
                shared_this));
    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_get_buffer_true_with_release_false,
                shared_this));
    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_get_buffer_true_with_release_true,
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

using namespace boost::unit_test_framework;

ACE_Proper_Export_Flag test_suite *
init_unit_test_suite(int, char*[])
{
  test_suite * ts =
      BOOST_TEST_SUITE("unbounded octet sequence unit test");

  {
    boost::shared_ptr<Tester> tester(Tester::allocate());
    tester->add_all(ts);
  }

  {
    typedef value_sequence_tester<tested_sequence,tested_allocation_traits> common;
    boost::shared_ptr<common> tester(common::allocate());
    tester->add_all(ts);
  }

  return ts;
}
