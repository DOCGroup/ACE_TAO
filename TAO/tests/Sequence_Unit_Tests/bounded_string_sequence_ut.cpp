/**
 * @file
 *
 * @brief Unit test for bounded string sequences.
 *
 * $Id$
 *
 * @author Carlos O'Ryan
 */
#define TAO_USER_DEFINED_SEQUENCE_RANGE_CHECKING_INCLUDE \
        "tests/Sequence_Unit_Tests/testing_range_checking.hpp"

#include "testing_string_traits.hpp"
#include "tao/String_Traits_T.h"
#include "testing_allocation_traits.hpp"

#include "tao/Bounded_Basic_String_Sequence_T.h"
#include "tao/CORBA_String.h"

#include "string_sequence_tester.hpp"

#include "ace/OS_NS_string.h"

#include <boost/test/unit_test.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include <sstream>

using namespace boost::unit_test_framework;
using namespace TAO_VERSIONED_NAMESPACE_NAME::TAO;

CORBA::ULong const MAXIMUM = 32;

template<class tested_sequence>
struct Tester
{
  typedef typename tested_sequence::character_type char_type;
  typedef string_sequence_test_helpers<char_type> helper;
  typedef typename tested_sequence::value_type value_type;
  typedef typename tested_sequence::const_value_type const_value_type;

  typedef typename tested_sequence::element_traits tested_element_traits;
  typedef typename tested_sequence::allocation_traits tested_allocation_traits;
  typedef TAO::details::range_checking<value_type,true> range;

  void test_set_length_less_than_maximum()
  {
    expected_calls a(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    expected_calls i(tested_element_traits::default_initializer_calls);
    {
      tested_sequence x;

      x.length(8);
      BOOST_CHECK_MESSAGE(a.expect(0), a);
      BOOST_CHECK_EQUAL(CORBA::ULong(MAXIMUM), x.maximum());
      BOOST_CHECK_EQUAL(CORBA::ULong(8), x.length());
      BOOST_CHECK_EQUAL(true, x.release());

      BOOST_CHECK_MESSAGE(i.expect(8), i);
    }
    BOOST_CHECK_MESSAGE(f.expect(1), f);
  }

  void test_set_length_more_than_maximum()
  {
    tested_sequence x;

    BOOST_CHECK_THROW(x.length(64), std::runtime_error);
  }

  value_type * alloc_and_init_buffer()
  {
    value_type * buf = tested_sequence::allocbuf();
    buf[0] = helper::to_string(1);
    buf[1] = helper::to_string(4);
    buf[2] = helper::to_string(9);
    buf[3] = helper::to_string(16);

    return buf;
  }

  void check_values(tested_sequence const & a)
  {
    BOOST_CHECK(helper::compare(1,  a[0]));
    BOOST_CHECK(helper::compare(4,  a[1]));
    BOOST_CHECK(helper::compare(9,  a[2]));
    BOOST_CHECK(helper::compare(16, a[3]));
  }

  void test_regression_2201()
  {
    value_type * buffer = alloc_and_init_buffer();
    expected_calls a(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    expected_calls r(tested_element_traits::release_calls);
    {
      tested_sequence a(4, buffer, false);
      BOOST_CHECK_EQUAL(CORBA::ULong(MAXIMUM), a.maximum());
      BOOST_CHECK_EQUAL(CORBA::ULong(4), a.length());
      BOOST_CHECK_EQUAL(buffer, a.get_buffer());
      BOOST_CHECK_EQUAL(false, a.release());
      a.length (3);
      BOOST_CHECK_EQUAL(CORBA::ULong(3), a.length());
      a.length (4);
      BOOST_CHECK_EQUAL(CORBA::ULong(4), a.length());
      BOOST_CHECK(helper::compare_empty(a[3]));
    }
    BOOST_CHECK_MESSAGE(a.expect(0), a);
    BOOST_CHECK_MESSAGE(f.expect(0), f);
    tested_sequence::freebuf(buffer);
    BOOST_CHECK_MESSAGE(r.expect(MAXIMUM), r);
  }

  void test_buffer_constructor_default()
  {
    value_type * buffer = alloc_and_init_buffer();
    expected_calls a(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    expected_calls r(tested_element_traits::release_calls);
    {
      tested_sequence a(4, buffer, false);
      BOOST_CHECK_EQUAL(CORBA::ULong(MAXIMUM), a.maximum());
      BOOST_CHECK_EQUAL(CORBA::ULong(4), a.length());
      BOOST_CHECK_EQUAL(buffer, a.get_buffer());
      BOOST_CHECK_EQUAL(false, a.release());
      check_values(a);
    }
    BOOST_CHECK_MESSAGE(a.expect(0), a);
    BOOST_CHECK_MESSAGE(f.expect(0), f);
    tested_sequence::freebuf(buffer);
    BOOST_CHECK_MESSAGE(r.expect(MAXIMUM), r);
  }

  void test_buffer_constructor_false()
  {
    value_type * buffer = alloc_and_init_buffer();
    expected_calls a(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    expected_calls r(tested_element_traits::release_calls);
    {
      tested_sequence a(4, buffer, false);
      BOOST_CHECK_EQUAL(CORBA::ULong(MAXIMUM), a.maximum());
      BOOST_CHECK_EQUAL(CORBA::ULong(4), a.length());
      BOOST_CHECK_EQUAL(buffer, a.get_buffer());
      BOOST_CHECK_EQUAL(false, a.release());
      check_values(a);
    }
    BOOST_CHECK_MESSAGE(a.expect(0), a);
    BOOST_CHECK_MESSAGE(f.expect(0), f);
    tested_sequence::freebuf(buffer);
    BOOST_CHECK_MESSAGE(r.expect(MAXIMUM), r);
  }

  void test_buffer_constructor_true()
  {
    value_type * buffer = alloc_and_init_buffer();
    expected_calls a(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    expected_calls r(tested_element_traits::release_calls);
    {
      tested_sequence a(4, buffer, true);
      BOOST_CHECK_EQUAL(CORBA::ULong(MAXIMUM), a.maximum());
      BOOST_CHECK_EQUAL(CORBA::ULong(4), a.length());
      BOOST_CHECK_EQUAL(buffer, a.get_buffer());
      BOOST_CHECK_EQUAL(true, a.release());
      check_values(a);
    }
    BOOST_CHECK_MESSAGE(a.expect(0), a);
    BOOST_CHECK_MESSAGE(f.expect(1), f);
    BOOST_CHECK_MESSAGE(r.expect(MAXIMUM), r);
  }

  void test_replace_default()
  {
    value_type * buffer = alloc_and_init_buffer();

    expected_calls c(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    expected_calls r(tested_element_traits::release_calls);
    {
      tested_sequence a;
      a.replace(4, buffer);
      BOOST_CHECK_MESSAGE(c.expect(0), c);
      BOOST_CHECK_MESSAGE(f.expect(1), f);
      BOOST_CHECK_MESSAGE(r.expect(MAXIMUM), r);

      BOOST_CHECK_EQUAL(CORBA::ULong(MAXIMUM), a.maximum());
      BOOST_CHECK_EQUAL(CORBA::ULong(4), a.length());
      BOOST_CHECK_EQUAL(buffer, a.get_buffer());
      BOOST_CHECK_EQUAL(false, a.release());
      check_values(a);
    }
    BOOST_CHECK_MESSAGE(c.expect(0), c);
    BOOST_CHECK_MESSAGE(f.expect(0), f);
    tested_sequence::freebuf(buffer);
    BOOST_CHECK_MESSAGE(r.expect(MAXIMUM), r);
  }

  void test_replace_false()
  {
    value_type * buffer = alloc_and_init_buffer();

    expected_calls c(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    expected_calls r(tested_element_traits::release_calls);
    {
      tested_sequence a;
      a.replace(4, buffer, false);
      BOOST_CHECK_MESSAGE(c.expect(0), c);
      BOOST_CHECK_MESSAGE(f.expect(1), f);
      BOOST_CHECK_MESSAGE(r.expect(MAXIMUM), r);

      BOOST_CHECK_EQUAL(CORBA::ULong(MAXIMUM), a.maximum());
      BOOST_CHECK_EQUAL(CORBA::ULong(4), a.length());
      BOOST_CHECK_EQUAL(buffer, a.get_buffer());
      BOOST_CHECK_EQUAL(false, a.release());
      check_values(a);
    }
    BOOST_CHECK_MESSAGE(c.expect(0), c);
    BOOST_CHECK_MESSAGE(f.expect(0), f);
    tested_sequence::freebuf(buffer);
    BOOST_CHECK_MESSAGE(r.expect(MAXIMUM), r);
  }

  void test_replace_true()
  {
    value_type * buffer = alloc_and_init_buffer();

    expected_calls c(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    expected_calls r(tested_element_traits::release_calls);
    {
      tested_sequence a;
      a.replace(4, buffer, false);
      BOOST_CHECK_MESSAGE(c.expect(0), c);
      BOOST_CHECK_MESSAGE(f.expect(1), f);
      BOOST_CHECK_MESSAGE(r.expect(MAXIMUM), r);

      BOOST_CHECK_EQUAL(CORBA::ULong(MAXIMUM), a.maximum());
      BOOST_CHECK_EQUAL(CORBA::ULong(4), a.length());
      BOOST_CHECK_EQUAL(buffer, a.get_buffer());
      BOOST_CHECK_EQUAL(false, a.release());
      check_values(a);
    }
    BOOST_CHECK_MESSAGE(c.expect(0), c);
    BOOST_CHECK_MESSAGE(f.expect(0), f);
    tested_sequence::freebuf(buffer);
    BOOST_CHECK_MESSAGE(r.expect(MAXIMUM), r);
  }

  void test_get_buffer_default()
  {
    value_type * buffer = alloc_and_init_buffer();
    tested_sequence a(4, buffer, true);
    BOOST_CHECK_EQUAL(a.get_buffer(), buffer);
  }

  void test_get_buffer_false()
  {
    value_type * buffer = alloc_and_init_buffer();
    tested_sequence a(4, buffer, true);
    BOOST_CHECK_EQUAL(a.get_buffer(), buffer);
  }

  void test_get_buffer_true_with_release_false()
  {
    value_type * buffer = alloc_and_init_buffer();
    tested_sequence a(4, buffer, false);
    BOOST_CHECK(0 == a.get_buffer(true));
    tested_sequence::freebuf(buffer);
  }

  void test_get_buffer_true_with_release_true()
  {
    value_type * buffer = alloc_and_init_buffer();
    expected_calls c(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    expected_calls r(tested_element_traits::release_calls);
    {
      tested_sequence a(4, buffer, true);
      BOOST_CHECK_EQUAL(buffer, a.get_buffer(true));

      tested_sequence const & b = a;
      BOOST_CHECK_EQUAL(CORBA::ULong(MAXIMUM), b.maximum());
      BOOST_CHECK_EQUAL(CORBA::ULong(0), b.length());
      BOOST_CHECK(0 != b.get_buffer());
      BOOST_CHECK_EQUAL(true, b.release());

      BOOST_CHECK_MESSAGE(c.expect(0), c);

      BOOST_CHECK(buffer != b.get_buffer());
    }
    BOOST_CHECK_MESSAGE(f.expect(1), f);
    BOOST_CHECK_MESSAGE(r.expect(MAXIMUM), r);
    BOOST_CHECK_MESSAGE(c.expect(0), c);

    tested_sequence::freebuf(buffer);
    BOOST_CHECK_MESSAGE(f.expect(1), f);
    BOOST_CHECK_MESSAGE(r.expect(MAXIMUM), r);
  }

  void add_all(test_suite * ts)
  {
    typedef string_sequence_tester<tested_sequence> common;
    boost::shared_ptr<common> tester(common::allocate(true));
    tester->add_all(ts);

    boost::shared_ptr<Tester> shared_this(self_);

    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_set_length_less_than_maximum,
                shared_this));
    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_set_length_more_than_maximum,
                shared_this));

    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_regression_2201,
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
                &Tester::test_get_buffer_default,
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

ACE_Proper_Export_Flag test_suite *
init_unit_test_suite(int, char*[])
{
  test_suite * ts =
      BOOST_TEST_SUITE("bounded string sequence unit test");

  {
    typedef TAO::bounded_basic_string_sequence<char, MAXIMUM> s_sequence;
    typedef Tester<s_sequence> nTester;
    boost::shared_ptr<nTester> tester(nTester::allocate());
    tester->add_all(ts);
  }

  {
    typedef TAO::bounded_basic_string_sequence<CORBA::WChar, MAXIMUM> w_sequence;
    typedef Tester<w_sequence> wTester;
    boost::shared_ptr<wTester> tester(wTester::allocate());
    tester->add_all(ts);
  }

  return ts;
}
