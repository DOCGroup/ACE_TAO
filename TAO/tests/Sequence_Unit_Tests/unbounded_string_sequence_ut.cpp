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

#include <boost/test/unit_test.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

using namespace boost::unit_test_framework;
using namespace TAO;

struct Tester
  : public boost::enable_shared_from_this<Tester>
{
  typedef TAO::details::string_traits<char,true> tested_element_traits;
  typedef TAO::details::unbounded_allocation_traits<char*,true> tested_allocation_traits;
  typedef TAO::details::range_checking<char*,true> range;

  typedef TAO::unbounded_string_sequence tested_sequence;

  void add_all(test_suite * ts)
  {
    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_default_constructor, shared_from_this()));
    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_ulong_constructor, shared_from_this()));
    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_copy_constructor, shared_from_this()));
    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_set_length_less_than_maximum,
                shared_from_this()));
    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_set_length_more_than_maximum,
                shared_from_this()));
    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_index_accessor,
                shared_from_this()));
  }

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

  void test_copy_constructor()
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
};

test_suite *
init_unit_test_suite(int, char*[])
{
  std::auto_ptr<test_suite> ts(
      BOOST_TEST_SUITE("unbounded string sequence unit test"));

  boost::shared_ptr<Tester> tester(new Tester);
  tester->add_all(ts.get());

  return ts.release();
}

#if 0
// This is just to convince MPC that I do not need a main() to have a
// program.
int main() {}
#endif
