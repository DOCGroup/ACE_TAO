/**
 * @file
 *
 * @brief Unit test for unbounded string sequences.
 *
 * $Id$
 *
 * @author Carlos O'Ryan
 */
#include "testing_string_traits.hpp"
#include "string_traits.hpp"
#include "testing_allocation_traits.hpp"
#include "testing_range_checking.hpp"

#include "unbounded_string_sequence.hpp"

#include "ace/OS_NS_string.h"

#include <boost/test/unit_test.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include <sstream>

using namespace boost::unit_test_framework;
using namespace TAO;

struct Tester
{
  typedef char char_type;
  typedef char * value_type;
  typedef char const * const_value_type;

  typedef TAO::details::string_traits<char,true> tested_element_traits;
  typedef TAO::details::unbounded_allocation_traits<char*,true> tested_allocation_traits;
  typedef TAO::details::range_checking<char*,true> range;

  typedef TAO::unbounded_string_sequence tested_sequence;

  /**
   * @brief Make sure the default constructor works as expected.
   */
  void test_default_constructor()
  {
    expected_calls a(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    {
      tested_sequence x;

      BOOST_CHECK_EQUAL(CORBA::ULong(0), x.maximum());
      BOOST_CHECK_EQUAL(CORBA::ULong(0), x.length());
      BOOST_CHECK_EQUAL(true, x.release());
    }
    BOOST_CHECK_MESSAGE(a.expect(0), a);
    BOOST_CHECK_MESSAGE(f.expect(1), f);
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

  void test_copy_constructor_from_default()
  {
    expected_calls a(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    expected_calls i(tested_element_traits::default_initializer_calls);
    expected_calls d(tested_element_traits::duplicate_calls);
    {
      tested_sequence x;
      BOOST_CHECK_MESSAGE(a.expect(0), a);
      BOOST_CHECK_MESSAGE(i.expect(0), i);

      BOOST_CHECK_EQUAL(CORBA::ULong(0), x.maximum());
      BOOST_CHECK_EQUAL(CORBA::ULong(0), x.length());
      BOOST_CHECK_EQUAL(true, x.release());

      tested_sequence y(x);
      BOOST_CHECK_MESSAGE(a.expect(1), a);
      BOOST_CHECK_MESSAGE(i.expect(0), i);

      BOOST_CHECK_EQUAL(CORBA::ULong(0), y.maximum());
      BOOST_CHECK_EQUAL(CORBA::ULong(0), y.length());
      BOOST_CHECK_EQUAL(true, y.release());
    }
    BOOST_CHECK_MESSAGE(f.expect(2), f);
  }

  void test_copy_constructor_from_ulong()
  {
    expected_calls a(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    expected_calls i(tested_element_traits::default_initializer_calls);
    expected_calls d(tested_element_traits::duplicate_calls);
    {
      tested_sequence x(16);
      BOOST_CHECK_MESSAGE(a.expect(1), a);

      x.length(8);

      BOOST_CHECK_EQUAL(CORBA::ULong(16), x.maximum());
      BOOST_CHECK_EQUAL(CORBA::ULong(8), x.length());
      BOOST_CHECK_EQUAL(true, x.release());

      BOOST_CHECK_MESSAGE(i.expect(8), i);

      tested_sequence y(x);
      BOOST_CHECK_MESSAGE(a.expect(1), a);
      BOOST_CHECK_EQUAL(CORBA::ULong(16), y.maximum());
      BOOST_CHECK_EQUAL(CORBA::ULong(8), y.length());
      BOOST_CHECK_EQUAL(true, y.release());

      BOOST_CHECK_MESSAGE(d.expect(8), d);
    }
    BOOST_CHECK_MESSAGE(f.expect(2), f);
    BOOST_CHECK_MESSAGE(i.expect(0), i);
  }

  void test_set_length_less_than_maximum()
  {
    expected_calls a(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    expected_calls i(tested_element_traits::default_initializer_calls);
    {
      tested_sequence x(16);

      x.length(8);
      BOOST_CHECK_EQUAL(CORBA::ULong(16), x.maximum());
      BOOST_CHECK_EQUAL(CORBA::ULong(8), x.length());
      BOOST_CHECK_EQUAL(true, x.release());

      BOOST_CHECK_MESSAGE(i.expect(8), i);
    }
    BOOST_CHECK_MESSAGE(a.expect(1), a);
    BOOST_CHECK_MESSAGE(f.expect(1), f);
  }

  void test_set_length_more_than_maximum()
  {
    expected_calls a(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    expected_calls i(tested_element_traits::default_initializer_calls);
    {
      tested_sequence x(16);
      BOOST_CHECK_MESSAGE(a.expect(1), a);

      x.length(32);
      BOOST_CHECK_MESSAGE(a.expect(1), a);
      BOOST_CHECK_MESSAGE(f.expect(1), f);

      BOOST_CHECK_MESSAGE(i.expect(32), i);

      BOOST_CHECK_EQUAL(CORBA::ULong(32), x.maximum());
      BOOST_CHECK_EQUAL(CORBA::ULong(32), x.length());
      BOOST_CHECK_EQUAL(true, x.release());
    }
    BOOST_CHECK_MESSAGE(f.expect(1), f);
  }

  void test_index_accessor()
  {
    tested_sequence x(16);
    x.length(8);

    tested_sequence const & y = x;
    char const * t = y[4];
    BOOST_CHECK_MESSAGE(std::strcmp(t, "") == 0,
        "Unexpected string value " << t);
  }

  void test_index_modifier()
  {
    tested_sequence x(16);
    x.length(8);

    tested_sequence const & y = x;

    char const * text = "text";
    expected_calls d(tested_element_traits::duplicate_calls);
    x[4] = text;
    BOOST_CHECK_MESSAGE(d.expect(1), d);

    char const * t = y[4];

    BOOST_CHECK_MESSAGE(std::strcmp(text, x[4]) == 0,
        "Mismatched values expected=" << text
        << ", got=" << x[4]);
    BOOST_CHECK_MESSAGE(std::strcmp(text, y[4]) == 0,
        "Mismatched values expected=" << text
        << ", got=" << y[4]);
    BOOST_CHECK(text != t);
  }

  void test_index_checking()
  {
    tested_sequence x;
    x.length(8);

    tested_sequence const & y = x;
    char const * lhs = 0;
    char const * rhs = 0;
    BOOST_CHECK_THROW(lhs = y[32], std::range_error);
    BOOST_CHECK_THROW(x[32] = rhs, std::range_error);
  }

  void test_copy_constructor_values()
  {
    tested_sequence a;
    a.length(16);
    for(CORBA::ULong i = 0; i != 16; ++i)
    {
      std::ostringstream os; os << (i*i);
      a[i] = os.str().c_str();
    }

    expected_calls d(tested_element_traits::duplicate_calls);
    expected_calls r(tested_element_traits::release_calls);
    {
      tested_sequence b(a);
      BOOST_CHECK_MESSAGE(d.expect(16), d);

      BOOST_CHECK_EQUAL(a.length(), b.length());
      for(CORBA::ULong i = 0; i != a.length(); ++i)
      {
        BOOST_CHECK_MESSAGE(ACE_OS::strcmp(a[i], b[i]) == 0,
            "Mismatched elements at index=" << i
            << ", a=" << a[i]
            << ", b=" << b[i]);
      }
    }
    BOOST_CHECK_MESSAGE(r.expect(16), r);
  }

  void test_freebuf_releases_elements()
  {
    value_type * buffer = tested_sequence::allocbuf(32);
    for(int i = 0; i != 32; ++i)
    {
      buffer[i] = tested_element_traits::duplicate("Foo bar baz");
    }

    expected_calls r(tested_element_traits::release_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);

    tested_sequence::freebuf(buffer);

    BOOST_CHECK_MESSAGE(f.expect(0), f);
    BOOST_CHECK_MESSAGE(r.expect(32), r);

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
                &Tester::test_copy_constructor_from_default,
                shared_this));
    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_copy_constructor_from_ulong,
                shared_this));
    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_set_length_less_than_maximum,
                shared_this));
    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_set_length_more_than_maximum,
                shared_this));
    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_index_accessor,
                shared_this));
    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_index_checking,
                shared_this));
    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_copy_constructor_values,
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

