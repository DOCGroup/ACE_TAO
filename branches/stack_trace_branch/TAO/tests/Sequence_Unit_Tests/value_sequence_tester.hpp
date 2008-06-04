#ifndef guard_value_sequence_tester_hpp
#define guard_value_sequence_tester_hpp
/**
 * @file
 *
 * @brief Helper class to implement tests for *_value_sequence
 *
 * $Id$
 *
 * @author Carlos O'Ryan
 */
#include "tao/Basic_Types.h"

#include <boost/test/unit_test.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template<class tested_sequence,
    class tested_allocation_traits>
struct value_sequence_tester
{
  typedef typename tested_sequence::value_type value_type;
  typedef typename tested_sequence::const_value_type const_value_type;

  void test_default_constructor()
  {
    expected_calls a(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    {
      tested_sequence x;

      BOOST_CHECK_EQUAL(
          CORBA::ULong(tested_allocation_traits::default_maximum()),
          x.maximum());
      BOOST_CHECK_EQUAL(CORBA::ULong(0), x.length());
      BOOST_CHECK_EQUAL(bounded_, x.release());
    }
    BOOST_CHECK_MESSAGE(a.expect(0), a);
    BOOST_CHECK_MESSAGE(f.expect(bounded_ ? 1 : 0), f);
  }


  void test_copy_constructor_from_default()
  {
    expected_calls a(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    {
      tested_sequence x;
      BOOST_CHECK_MESSAGE(a.expect(0), a);
      BOOST_CHECK_EQUAL(
          CORBA::ULong(tested_allocation_traits::default_maximum()),
          x.maximum());
      BOOST_CHECK_EQUAL(CORBA::ULong(0), x.length());
      BOOST_CHECK_EQUAL(bounded_, x.release());

      tested_sequence y(x);
      BOOST_CHECK_MESSAGE(a.expect(bounded_ ? 1 : 0), a);
      BOOST_CHECK_EQUAL(x.maximum(), y.maximum());
      BOOST_CHECK_EQUAL(x.length(), y.length());
      BOOST_CHECK_EQUAL(x.release(), y.release());
    }
    BOOST_CHECK_MESSAGE(f.expect(bounded_ ? 2 : 0), f);
  }

  void test_index_accessor()
  {
    tested_sequence x;
    x.length(8);

    tested_sequence const & y = x;
    const_value_type & z = y[4];
    BOOST_CHECK_EQUAL(z, y[4]);
  }

  void test_index_modifier()
  {
    tested_sequence x;
    x.length(8);

    tested_sequence const & y = x;
    const_value_type & z = y[4];
    x[4] = 4;
    BOOST_CHECK_EQUAL(4, x[4]);
    BOOST_CHECK_EQUAL(4, y[4]);
    BOOST_CHECK_EQUAL(4, z);
  }

  void test_index_checking()
  {
    tested_sequence x;
    x.length(8);

    tested_sequence const & y = x;
    int z = 0;

    BOOST_CHECK_THROW(z = y[32], std::range_error);
    BOOST_CHECK_THROW(x[32] = z, std::range_error);
  }

  void test_copy_constructor_values()
  {
    tested_sequence a;
    a.length(16);
    for(CORBA::ULong i = 0; i != 16; ++i) a[i] = i*i;

    tested_sequence b(a);
    BOOST_CHECK_EQUAL(a.length(), b.length());
    for(CORBA::ULong i = 0; i != a.length(); ++i)
    {
      BOOST_CHECK_MESSAGE(a[i] == b[i],
          "Mismatched elements at index " << i);
    }
  }

  void test_assignment_from_default()
  {
    expected_calls a(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    {
      tested_sequence x;
      BOOST_CHECK_MESSAGE(a.expect(0), a);
      BOOST_CHECK_EQUAL(
          CORBA::ULong(tested_allocation_traits::default_maximum()),
          x.maximum());
      BOOST_CHECK_EQUAL(CORBA::ULong(0), x.length());
      BOOST_CHECK_EQUAL(bounded_, x.release());

      tested_sequence y;
      BOOST_CHECK_MESSAGE(a.expect(0), a);

      y = x;
      BOOST_CHECK_MESSAGE(a.expect(bounded_ ? 1 : 0), a);
      BOOST_CHECK_MESSAGE(f.expect(bounded_ ? 1 : 0), f);
      BOOST_CHECK_EQUAL(x.maximum(), y.maximum());
      BOOST_CHECK_EQUAL(x.length(), y.length());
      BOOST_CHECK_EQUAL(x.release(), y.release());
    }
    BOOST_CHECK_MESSAGE(f.expect(bounded_ ? 2 : 0), f);
  }

  void test_assignment_values()
  {
    tested_sequence a;
    a.length(16);
    for(CORBA::ULong i = 0; i != 16; ++i) a[i] = i*i;

    tested_sequence b;
    b = a;
    BOOST_CHECK_EQUAL(a.maximum(), b.maximum());
    BOOST_CHECK_EQUAL(a.length(),  b.length());
    BOOST_CHECK_EQUAL(a.release(), b.release());
    for(CORBA::ULong i = 0; i != a.length(); ++i)
    {
      BOOST_CHECK_MESSAGE(a[i] == b[i],
          "Mismatched elements at index " << i);
    }
  }

  void test_exception_in_copy_constructor()
  {
    expected_calls f(tested_allocation_traits::freebuf_calls);
    {
      tested_sequence x; x.length(8);
      f.reset();

      expected_calls a(tested_allocation_traits::allocbuf_calls);
      tested_allocation_traits::allocbuf_calls.failure_countdown(1);
      BOOST_CHECK_THROW(tested_sequence y(x), testing_exception);
      BOOST_CHECK_MESSAGE(a.expect(1), a);
    }
    BOOST_CHECK_MESSAGE(f.expect(1), f);
  }

  void test_exception_in_assignment()
  {
    expected_calls f(tested_allocation_traits::freebuf_calls);
    {
      tested_sequence x; x.length(2);

      tested_sequence y; y.length(3);

      expected_calls a(tested_allocation_traits::allocbuf_calls);
      f.reset();
      tested_allocation_traits::allocbuf_calls.failure_countdown(1);
      BOOST_CHECK_THROW(y = x, testing_exception);

      BOOST_CHECK_MESSAGE(a.expect(1), a);
      BOOST_CHECK_MESSAGE(f.expect(0), f);

      BOOST_CHECK_EQUAL(CORBA::ULong(3), y.length());
    }
    BOOST_CHECK_MESSAGE(f.expect(2), f);
  }

  void test_get_buffer_const()
  {
    tested_sequence a; a.length(4);
    tested_sequence const & b = a;

    value_type const * buffer = b.get_buffer();
    a[0] = 1; a[1] = 4; a[2] = 9; a[3] = 16;

    BOOST_CHECK_EQUAL(1,  buffer[0]);
    BOOST_CHECK_EQUAL(4,  buffer[1]);
    BOOST_CHECK_EQUAL(9,  buffer[2]);
    BOOST_CHECK_EQUAL(16, buffer[3]);
  }

  void add_all(boost::unit_test_framework::test_suite * ts)
  {
    boost::shared_ptr<value_sequence_tester> shared_this(self_);

    ts->add(BOOST_CLASS_TEST_CASE(
                &value_sequence_tester::test_default_constructor,
                shared_this));

    ts->add(BOOST_CLASS_TEST_CASE(
                &value_sequence_tester::test_copy_constructor_from_default,
                shared_this));
    ts->add(BOOST_CLASS_TEST_CASE(
                &value_sequence_tester::test_index_accessor,
                shared_this));
    ts->add(BOOST_CLASS_TEST_CASE(
                &value_sequence_tester::test_index_modifier,
                shared_this));
    ts->add(BOOST_CLASS_TEST_CASE(
                &value_sequence_tester::test_index_checking,
                shared_this));
    ts->add(BOOST_CLASS_TEST_CASE(
                &value_sequence_tester::test_copy_constructor_values,
                shared_this));
    ts->add(BOOST_CLASS_TEST_CASE(
                &value_sequence_tester::test_assignment_from_default,
                shared_this));
    ts->add(BOOST_CLASS_TEST_CASE(
                &value_sequence_tester::test_assignment_values,
                shared_this));

    ts->add(BOOST_CLASS_TEST_CASE(
                &value_sequence_tester::test_exception_in_copy_constructor,
                shared_this));
    ts->add(BOOST_CLASS_TEST_CASE(
                &value_sequence_tester::test_exception_in_assignment,
                shared_this));

    ts->add(BOOST_CLASS_TEST_CASE(
                &value_sequence_tester::test_get_buffer_const,
                shared_this));
  }

  static boost::shared_ptr<value_sequence_tester> allocate(bool bounded = false)
  {
    boost::shared_ptr<value_sequence_tester> ptr(
        new value_sequence_tester(bounded));
    ptr->self_ = ptr;

    return ptr;
  }

private:
  value_sequence_tester(bool bounded)
    : bounded_ (bounded)
  {}

  bool bounded_;
  boost::weak_ptr<value_sequence_tester> self_;
};

TAO_END_VERSIONED_NAMESPACE_DECL
#endif // guard_value_sequence_tester_hpp
