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
#include <boost/enable_shared_from_this.hpp>

using namespace TAO::details;

using namespace boost::unit_test_framework;

CORBA::ULong const MAXIMUM = 32;

template<class value_type>
struct Tester : public boost::enable_shared_from_this<Tester<value_type> >
{
  typedef unbounded_allocation_traits<value_type,true> unbounded;
  typedef bounded_allocation_traits<value_type,MAXIMUM,true> bounded;

  void add_all(test_suite * ts)
  {
    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_default_buffer_allocation_value,
                shared_from_this()));
    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_default_buffer_allocation<unbounded>,
                shared_from_this()));
    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_default_buffer_allocation<bounded>,
                shared_from_this()));
    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_allocbuf<unbounded>,
                shared_from_this()));
    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_allocbuf<bounded>,
                shared_from_this()));
    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_freebuf<unbounded>,
                shared_from_this()));
    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_freebuf<bounded>,
                shared_from_this()));
  }

  template<class aspect>
  void test_allocbuf()
  {
    expected_calls c(aspect::allocbuf_calls);

    aspect::allocbuf_calls.failure_countdown(2);
    value_type * s;
    BOOST_CHECK_NO_THROW(s = aspect::allocbuf(4));
    aspect::freebuf(s);
    BOOST_CHECK_THROW(s = aspect::allocbuf(4), testing_exception);
    BOOST_CHECK_NO_THROW(s = aspect::allocbuf(4));
    aspect::freebuf(s);

    BOOST_CHECK_MESSAGE(c.expect(3), c);
  }

  template<class aspect>
  void test_freebuf()
  {
    expected_calls c(aspect::freebuf_calls);

    aspect::freebuf_calls.failure_countdown(2);
    value_type * s = aspect::allocbuf(4);
    BOOST_CHECK_NO_THROW(aspect::freebuf(s));
    s = aspect::allocbuf(4);
    BOOST_CHECK_THROW(aspect::freebuf(s), testing_exception);
    aspect::freebuf(s);
    s = aspect::allocbuf(4);
    BOOST_CHECK_NO_THROW(aspect::freebuf(s));

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
    value_type * s;
    BOOST_CHECK_NO_THROW(
        s = aspect::default_buffer_allocation());
    aspect::freebuf(s);
    BOOST_CHECK_THROW(
        s = aspect::default_buffer_allocation(), testing_exception);
    BOOST_CHECK_NO_THROW(
        s = aspect::default_buffer_allocation());
    aspect::freebuf(s);

    BOOST_CHECK_MESSAGE(c.expect(3), c);
  }

};

struct Foo { int y; };

test_suite *
init_unit_test_suite(int, char*[])
{
  std::auto_ptr<test_suite> ts(
      BOOST_TEST_SUITE("testing allocation traits unit test"));

  {
    boost::shared_ptr<Tester<int> > tester(new Tester<int>);
    tester->add_all(ts.get());
  }

  {
    boost::shared_ptr<Tester<Foo> > tester(new Tester<Foo>);
    tester->add_all(ts.get());
  }

  {
    boost::shared_ptr<Tester<char*> > tester(new Tester<char*>);
    tester->add_all(ts.get());
  }

  return ts.release();
}

#if 0
// This is just to convince MPC that I do not need a main() to have a
// program.
int main() {}
#endif
