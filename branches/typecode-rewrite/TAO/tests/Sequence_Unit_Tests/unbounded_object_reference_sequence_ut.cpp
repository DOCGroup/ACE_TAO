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
#include "object_reference_traits.hpp"
#include "testing_allocation_traits.hpp"
#include "testing_range_checking.hpp"

#include "mock_reference.hpp"
#include "unbounded_object_reference_sequence.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

using namespace boost::unit_test_framework;
using namespace TAO;

struct Tester
{
  typedef unbounded_object_reference_sequence<mock_reference> tested_sequence;
  typedef tested_sequence::value_type value_type;
  typedef tested_sequence::const_value_type const_value_type;

  typedef tested_sequence::element_traits tested_element_traits;
  typedef tested_sequence::allocation_traits tested_allocation_traits;
  typedef TAO::details::range_checking<value_type,true> range;

  void test_default_constructor()
  {
    expected_calls a(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    expected_calls i(tested_element_traits::default_initializer_calls);
    {
      tested_sequence x;

      BOOST_CHECK_EQUAL(CORBA::ULong(0), x.maximum());
      BOOST_CHECK_EQUAL(CORBA::ULong(0), x.length());
      BOOST_CHECK_EQUAL(true, x.release());
    }
    BOOST_CHECK_MESSAGE(a.expect(0), a);
    BOOST_CHECK_MESSAGE(f.expect(1), f);
    BOOST_CHECK_MESSAGE(i.expect(0), i);
  }

  void test_ulong_constructor()
  {
    expected_calls a(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    expected_calls i(tested_element_traits::default_initializer_calls);
    {
      tested_sequence x(16);

      BOOST_CHECK_EQUAL(CORBA::ULong(16), x.maximum());
      BOOST_CHECK_EQUAL(CORBA::ULong(0), x.length());
      BOOST_CHECK_EQUAL(true, x.release());
    }
    BOOST_CHECK_MESSAGE(a.expect(1), a);
    BOOST_CHECK_MESSAGE(f.expect(1), f);
    BOOST_CHECK_MESSAGE(i.expect(0), i);
  }

  void test_ulong_constructor_throw()
  {
    expected_calls a(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    expected_calls i(tested_element_traits::default_initializer_calls);
    {
      tested_allocation_traits::allocbuf_calls.failure_countdown(1);
      BOOST_CHECK_THROW(tested_sequence x(16), testing_exception);
    }
    BOOST_CHECK_MESSAGE(a.expect(1), a);
    BOOST_CHECK_MESSAGE(f.expect(0), f);
    BOOST_CHECK_MESSAGE(i.expect(0), i);
  }

  void test_buffer_constructor_release_true()
  {
    expected_calls a(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    expected_calls i(tested_element_traits::default_initializer_calls);
    {
      CORBA::ULong maximum = 32;
      tested_sequence::value_type * data = tested_sequence::allocbuf(maximum);
      a.reset();
      tested_sequence x(maximum, maximum / 2, data, true);

      BOOST_CHECK_EQUAL(x.maximum(), maximum);
      BOOST_CHECK_EQUAL(x.length(), maximum / 2);
      BOOST_CHECK_EQUAL(x.get_buffer(), data);
      BOOST_CHECK_EQUAL(x.release(), true);
    }
    BOOST_CHECK_MESSAGE(a.expect(0), a);
    BOOST_CHECK_MESSAGE(f.expect(1), f);
    BOOST_CHECK_MESSAGE(i.expect(0), i);
  }

  void test_buffer_constructor_release_false()
  {
    expected_calls a(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    expected_calls i(tested_element_traits::default_initializer_calls);
    {
      CORBA::ULong maximum = 64;
      tested_sequence::value_type * data = tested_sequence::allocbuf(maximum);
      a.reset();
      {
        tested_sequence x(maximum, maximum / 2, data, false);

        BOOST_CHECK_EQUAL(x.maximum(), maximum);
        BOOST_CHECK_EQUAL(x.length(), maximum / 2);
        BOOST_CHECK_EQUAL(x.get_buffer(), data);
        BOOST_CHECK_EQUAL(x.release(), false);
      }
      BOOST_CHECK_MESSAGE(a.expect(0), a);
      BOOST_CHECK_MESSAGE(f.expect(0), f);
      BOOST_CHECK_MESSAGE(i.expect(0), i);

      tested_sequence::freebuf(data);
    }
    BOOST_CHECK_MESSAGE(a.expect(0), a);
    BOOST_CHECK_MESSAGE(f.expect(1), f);
    BOOST_CHECK_MESSAGE(i.expect(0), i);
  }

  void test_copy_constructor_from_default()
  {
    expected_calls a(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    expected_calls i(tested_element_traits::default_initializer_calls);
    {
      tested_sequence x;

      tested_sequence y(x);
    }
    BOOST_CHECK_MESSAGE(a.expect(1), a);
    BOOST_CHECK_MESSAGE(f.expect(2), f);
    BOOST_CHECK_MESSAGE(i.expect(0), i);
  }

  void test_set_length()
  {
    expected_calls a(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    expected_calls i(tested_element_traits::default_initializer_calls);
    {
      tested_sequence x(16);
      BOOST_CHECK_MESSAGE(a.expect(1), a);

      x.length(16);
      BOOST_CHECK_EQUAL(CORBA::ULong(16), x.length());
      BOOST_CHECK_MESSAGE(a.expect(0), a);
      BOOST_CHECK_MESSAGE(f.expect(0), a);

      for(int i = 0; i != 16; ++i)
      {
        BOOST_CHECK_EQUAL(mock_reference::_nil(), x[i]);
        x[i] = mock_reference::allocate(i);
      }

      x.length(32);
      BOOST_CHECK_EQUAL(CORBA::ULong(32), x.length());
      BOOST_CHECK_MESSAGE(a.expect(1), a);
      BOOST_CHECK_MESSAGE(f.expect(1), f);

      tested_sequence const & y = x;

      for(CORBA::ULong i = 0; i != 16UL; ++i)
      {
        BOOST_REQUIRE(mock_reference::_nil() != y[i]);
        BOOST_CHECK_EQUAL(int(i), y[i]->id());
      }
      for(CORBA::ULong i = 16; i != 32UL; ++i)
      {
        BOOST_CHECK_EQUAL(mock_reference::_nil(), y[i]);
      }
    }
    BOOST_CHECK_MESSAGE(f.expect(1), f);
    BOOST_CHECK_MESSAGE(i.expect(0), i);
  }

  void test_set_length_throw()
  {
    expected_calls a(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    expected_calls i(tested_element_traits::default_initializer_calls);
    {
      tested_sequence x(16);
      BOOST_CHECK_MESSAGE(a.expect(1), a);
      x.length(16);
      for(int i = 0; i != 16; ++i)
      {
        BOOST_CHECK_EQUAL(mock_reference::_nil(), x[i]);
        x[i] = mock_reference::allocate(i);
      }

      tested_allocation_traits::allocbuf_calls.failure_countdown(1);
      BOOST_CHECK_THROW(x.length(32), testing_exception);
      BOOST_CHECK_MESSAGE(a.expect(1), a);
      BOOST_CHECK_MESSAGE(f.expect(0), f);
      BOOST_CHECK_EQUAL(CORBA::ULong(16), x.length());

      tested_sequence const & y = x;

      for(int i = 0; i != 16; ++i)
      {
        BOOST_REQUIRE(mock_reference::_nil() != y[i]);
        BOOST_CHECK_EQUAL(i, y[i]->id());
      }
    }
    BOOST_CHECK_MESSAGE(a.expect(0), a);
    BOOST_CHECK_MESSAGE(f.expect(1), f);
    BOOST_CHECK_MESSAGE(i.expect(0), i);
  }

  void add_all(test_suite * ts)
  {
    boost::shared_ptr<Tester> shared_this(self_);
    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_default_constructor,
                shared_this));
    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_ulong_constructor,
                shared_this));
    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_ulong_constructor_throw,
                shared_this));
    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_buffer_constructor_release_true,
                shared_this));
    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_buffer_constructor_release_false,
                shared_this));
    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_copy_constructor_from_default,
                shared_this));
    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_set_length,
                shared_this));
    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_set_length_throw,
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
  std::auto_ptr<test_suite> ts(
      BOOST_TEST_SUITE("unbounded string sequence unit test"));

  boost::shared_ptr<Tester> tester(Tester::allocate());
  tester->add_all(ts.get());

  return ts.release();
}

