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

#include <boost/test/unit_test.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

template<class tested_sequence,
    class tested_allocation_traits>
struct value_sequence_tester
  : public boost::enable_shared_from_this<
               value_sequence_tester<
                   tested_sequence,tested_allocation_traits> >
{
  typedef typename tested_sequence::value_type value_type;

  void add_all(boost::unit_test_framework::test_suite * ts)
  {
    ts->add(BOOST_CLASS_TEST_CASE(
                &value_sequence_tester::test_default_constructor,
                shared_from_this()));

    ts->add(BOOST_CLASS_TEST_CASE(
                &value_sequence_tester::test_copy_constructor_from_default,
                shared_from_this()));
    ts->add(BOOST_CLASS_TEST_CASE(
                &value_sequence_tester::test_index_accessor,
                shared_from_this()));
    ts->add(BOOST_CLASS_TEST_CASE(
                &value_sequence_tester::test_index_modifier,
                shared_from_this()));
    ts->add(BOOST_CLASS_TEST_CASE(
                &value_sequence_tester::test_index_checking,
                shared_from_this()));
    ts->add(BOOST_CLASS_TEST_CASE(
                &value_sequence_tester::test_copy_constructor_values,
                shared_from_this()));
    ts->add(BOOST_CLASS_TEST_CASE(
                &value_sequence_tester::test_assignment_from_default,
                shared_from_this()));
    ts->add(BOOST_CLASS_TEST_CASE(
                &value_sequence_tester::test_assignment_values,
                shared_from_this()));

    ts->add(BOOST_CLASS_TEST_CASE(
                &value_sequence_tester::test_exception_in_copy_constructor,
                shared_from_this()));
    ts->add(BOOST_CLASS_TEST_CASE(
                &value_sequence_tester::test_exception_in_assignment,
                shared_from_this()));
  }


  void test_default_constructor()
  {
    long a = tested_allocation_traits::allocbuf_calls;
    long f = tested_allocation_traits::freebuf_calls;
    {
      tested_sequence x;

      BOOST_CHECK_EQUAL(
          CORBA::ULong(tested_allocation_traits::default_maximum()),
          x.maximum());
      BOOST_CHECK_EQUAL(CORBA::ULong(0), x.length());
      BOOST_CHECK_EQUAL(true, x.release());
    }
    BOOST_CHECK_EQUAL(  a, tested_allocation_traits::allocbuf_calls);
    BOOST_CHECK_EQUAL(++f, tested_allocation_traits::freebuf_calls);
  }


  void test_copy_constructor_from_default()
  {
    long a = tested_allocation_traits::allocbuf_calls;
    long f = tested_allocation_traits::freebuf_calls;
    {
      tested_sequence x;
      BOOST_CHECK_EQUAL(a, tested_allocation_traits::allocbuf_calls);
      BOOST_CHECK_EQUAL(
          CORBA::ULong(tested_allocation_traits::default_maximum()),
          x.maximum());
      BOOST_CHECK_EQUAL(CORBA::ULong(0), x.length());
      BOOST_CHECK_EQUAL(true, x.release());

      tested_sequence y(x);
      BOOST_CHECK_EQUAL(++a, tested_allocation_traits::allocbuf_calls);
      BOOST_CHECK_EQUAL(x.maximum(), y.maximum());
      BOOST_CHECK_EQUAL(x.length(), y.length());
      BOOST_CHECK_EQUAL(x.release(), y.release());
    }
    f += 2;
    BOOST_CHECK_EQUAL(  f, tested_allocation_traits::freebuf_calls);
  }

  void test_index_accessor()
  {
    tested_sequence x;
    x.length(8);

    tested_sequence const & y = x;
    int const & z = y[4];
    BOOST_CHECK_EQUAL(z, y[4]);
  }

  void test_index_modifier()
  {
    tested_sequence x;
    x.length(8);

    tested_sequence const & y = x;
    int const & z = y[4];
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
    long a = tested_allocation_traits::allocbuf_calls;
    long f = tested_allocation_traits::freebuf_calls;
    {
      tested_sequence x;
      BOOST_CHECK_EQUAL(a, tested_allocation_traits::allocbuf_calls);
      BOOST_CHECK_EQUAL(
          CORBA::ULong(tested_allocation_traits::default_maximum()),
          x.maximum());
      BOOST_CHECK_EQUAL(CORBA::ULong(0), x.length());
      BOOST_CHECK_EQUAL(true, x.release());

      tested_sequence y;
      BOOST_CHECK_EQUAL(a, tested_allocation_traits::allocbuf_calls);

      y = x;
      BOOST_CHECK_EQUAL(++a, tested_allocation_traits::allocbuf_calls);
      BOOST_CHECK_EQUAL(++f, tested_allocation_traits::freebuf_calls);
      BOOST_CHECK_EQUAL(x.maximum(), y.maximum());
      BOOST_CHECK_EQUAL(x.length(), y.length());
      BOOST_CHECK_EQUAL(x.release(), y.release());
    }
    f += 2;
    BOOST_CHECK_EQUAL(  f, tested_allocation_traits::freebuf_calls);
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
    long f;
    {
      tested_sequence x; x.length(8);

      f = tested_allocation_traits::freebuf_calls;

      long a = tested_allocation_traits::allocbuf_calls;
      tested_allocation_traits::calls_until_failure_in_allocbuf = 1;
      BOOST_CHECK_THROW(tested_sequence y(x), testing_exception);
      BOOST_CHECK_EQUAL(++a, tested_allocation_traits::allocbuf_calls);
    }
    BOOST_CHECK_EQUAL(++f, tested_allocation_traits::freebuf_calls);
  }

  void test_exception_in_assignment()
  {
    long f;
    {
      tested_sequence x; x.length(2);

      tested_sequence y; y.length(3);

      long a = tested_allocation_traits::allocbuf_calls;
      f = tested_allocation_traits::freebuf_calls;
      tested_allocation_traits::calls_until_failure_in_allocbuf = 1;
      BOOST_CHECK_THROW(y = x, testing_exception);

      BOOST_CHECK_EQUAL(++a, tested_allocation_traits::allocbuf_calls);
      BOOST_CHECK_EQUAL(  f, tested_allocation_traits::freebuf_calls);

      BOOST_CHECK_EQUAL(CORBA::ULong(3), y.length());
    }
    ++f /* for x */; ++f /* for y */;
    BOOST_CHECK_EQUAL(  f, tested_allocation_traits::freebuf_calls);
  }

};

#endif // guard_value_sequence_tester_hpp
