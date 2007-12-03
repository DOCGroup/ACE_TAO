/**
 * @file
 *
 * @brief Unit test for the testing_allocation_traits.
 *
 * $Id$
 *
 * @author Carlos O'Ryan
 */
#include "testing_allocation_traits.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#define CHECK_NO_THROW(statement) \
try { statement; } catch(...) { \
  BOOST_ERROR("unexpected exception raised"); }

using namespace TAO_VERSIONED_NAMESPACE_NAME::TAO::details;

using namespace boost::unit_test_framework;

CORBA::ULong const MAXIMUM = 32;

template<class value_type>
struct Tester
{
  typedef unbounded_value_allocation_traits<value_type,true> unbounded;
  typedef bounded_value_allocation_traits<value_type,MAXIMUM,true> bounded;

  template<class aspect>
  void test_allocbuf()
  {
    expected_calls c(aspect::allocbuf_calls);

    aspect::allocbuf_calls.failure_countdown(2);
    value_type * s = 0;
    CHECK_NO_THROW(s = aspect::allocbuf(4));
    aspect::freebuf(s);
    BOOST_CHECK_THROW(s = aspect::allocbuf(4), testing_exception);
    CHECK_NO_THROW(s = aspect::allocbuf(4));
    aspect::freebuf(s);

    BOOST_CHECK_MESSAGE(c.expect(3), c);
  }

  template<class aspect>
  void test_freebuf()
  {
    expected_calls c(aspect::freebuf_calls);

    aspect::freebuf_calls.failure_countdown(2);
    value_type * s = aspect::allocbuf(4);
    CHECK_NO_THROW(aspect::freebuf(s));
    s = aspect::allocbuf(4);
    BOOST_CHECK_THROW(aspect::freebuf(s), testing_exception);
    aspect::freebuf(s);
    s = aspect::allocbuf(4);
    CHECK_NO_THROW(aspect::freebuf(s));

    BOOST_CHECK_MESSAGE(c.expect(4), c);
  }

  void test_default_buffer_allocation_value()
  {
    expected_calls u(unbounded::default_buffer_allocation_calls);
    expected_calls b(bounded::default_buffer_allocation_calls);

    value_type * s = unbounded::default_buffer_allocation();
    BOOST_CHECK_MESSAGE(u.expect(1), u);
    BOOST_CHECK_MESSAGE(b.expect(0), b);
    BOOST_CHECK_EQUAL(static_cast<value_type*>(0), s);
    bounded::freebuf(s);

    s = bounded::default_buffer_allocation();
    BOOST_CHECK_MESSAGE(u.expect(0), u);
    BOOST_CHECK_MESSAGE(b.expect(1), b);
    BOOST_CHECK(static_cast<value_type*>(0) != s);
    bounded::freebuf(s);
  }

  template<class aspect>
  void test_default_buffer_allocation()
  {
    expected_calls c(aspect::default_buffer_allocation_calls);

    aspect::default_buffer_allocation_calls.failure_countdown(2);
    value_type * s = 0;
    CHECK_NO_THROW(
        s = aspect::default_buffer_allocation());
    aspect::freebuf(s);
    BOOST_CHECK_THROW(
        s = aspect::default_buffer_allocation(), testing_exception);
    CHECK_NO_THROW(
        s = aspect::default_buffer_allocation());
    aspect::freebuf(s);

    BOOST_CHECK_MESSAGE(c.expect(3), c);
  }

  void test_default_buffer_allocation_unbounded()
  {
    test_default_buffer_allocation<unbounded>();
  }

  void test_default_buffer_allocation_bounded()
  {
    test_default_buffer_allocation<bounded>();
  }

  void test_allocbuf_unbounded()
  {
    test_allocbuf<unbounded>();
  }

  void test_allocbuf_bounded()
  {
    test_allocbuf<bounded>();
  }

  void test_freebuf_unbounded()
  {
    test_freebuf<unbounded>();
  }

  void test_freebuf_bounded()
  {
    test_freebuf<bounded>();
  }

  void add_all(test_suite * ts)
  {
    boost::shared_ptr<Tester> shared_this(self_);

    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_default_buffer_allocation_value,
                shared_this));
    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_default_buffer_allocation_unbounded,
                shared_this));
    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_default_buffer_allocation_bounded,
                shared_this));
    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_allocbuf_unbounded,
                shared_this));
    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_allocbuf_bounded,
                shared_this));
    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_freebuf_unbounded,
                shared_this));
    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_freebuf_bounded,
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

struct Foo { int y; };

ACE_Proper_Export_Flag test_suite *
init_unit_test_suite(int, char*[])
{
  test_suite * ts =
      BOOST_TEST_SUITE("testing allocation traits unit test");

  {
    boost::shared_ptr<Tester<int> > tester(
        Tester<int>::allocate());
    tester->add_all(ts);
  }

  {
    boost::shared_ptr<Tester<Foo> > tester(
        Tester<Foo>::allocate());
    tester->add_all(ts);
  }

  {
    boost::shared_ptr<Tester<char*> > tester(
        Tester<char*>::allocate());
    tester->add_all(ts);
  }

  return ts;
}

