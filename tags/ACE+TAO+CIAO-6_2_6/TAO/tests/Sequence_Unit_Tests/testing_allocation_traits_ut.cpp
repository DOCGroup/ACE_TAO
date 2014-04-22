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
#include "test_macros.h"

#include "tao/SystemException.h"

#define CHECK_NO_THROW(statement) \
try { statement; } catch(...) { \
  return 1; }

using namespace TAO_VERSIONED_NAMESPACE_NAME::TAO::details;



CORBA::ULong const MAXIMUM = 32;

template<class value_type>
struct Tester
{
  typedef unbounded_value_allocation_traits<value_type,true> unbounded;
  typedef bounded_value_allocation_traits<value_type,MAXIMUM,true> bounded;

  template<class aspect>
  int test_allocbuf()
  {
    expected_calls c(aspect::allocbuf_calls);

    aspect::allocbuf_calls.failure_countdown(2);
    value_type * s = 0;
    CHECK_NO_THROW(s = aspect::allocbuf(4));
    aspect::freebuf(s);
    CHECK_THROW(s = aspect::allocbuf(4), testing_exception);
    CHECK_NO_THROW(s = aspect::allocbuf(4));
    aspect::freebuf(s);

    FAIL_RETURN_IF_NOT(c.expect(3), c);
    return 0;
  }

  template<class aspect>
  int test_freebuf()
  {
    expected_calls c(aspect::freebuf_calls);

    aspect::freebuf_calls.failure_countdown(2);
    value_type * s = aspect::allocbuf(4);
    CHECK_NO_THROW(aspect::freebuf(s));
    s = aspect::allocbuf(4);
    CHECK_THROW(aspect::freebuf(s), testing_exception);
    aspect::freebuf(s);
    s = aspect::allocbuf(4);
    CHECK_NO_THROW(aspect::freebuf(s));

    FAIL_RETURN_IF_NOT(c.expect(4), c);
    return 0;
  }

  int test_default_buffer_allocation_value()
  {
    expected_calls u(unbounded::default_buffer_allocation_calls);
    expected_calls b(bounded::default_buffer_allocation_calls);

    value_type * s = unbounded::default_buffer_allocation();
    FAIL_RETURN_IF_NOT(u.expect(1), u);
    FAIL_RETURN_IF_NOT(b.expect(0), b);
    CHECK_EQUAL(static_cast<value_type*>(0), s);
    bounded::freebuf(s);

    s = bounded::default_buffer_allocation();
    FAIL_RETURN_IF_NOT(u.expect(0), u);
    FAIL_RETURN_IF_NOT(b.expect(1), b);
    // default_buffer_allocation doesn't allocate a buffer for
    // bounded sequences (see bug 3042).
    CHECK_EQUAL(static_cast<value_type*>(0), s);
    bounded::freebuf(s);
    return 0;
  }

  template<class aspect>
  int test_default_buffer_allocation()
  {
    expected_calls c(aspect::default_buffer_allocation_calls);

    aspect::default_buffer_allocation_calls.failure_countdown(2);
    value_type * s = 0;
    CHECK_NO_THROW(
        s = aspect::default_buffer_allocation());
    aspect::freebuf(s);
    CHECK_THROW(
        s = aspect::default_buffer_allocation(), testing_exception);
    CHECK_NO_THROW(
        s = aspect::default_buffer_allocation());
    aspect::freebuf(s);

    FAIL_RETURN_IF_NOT(c.expect(3), c);
    return 0;
  }

  int test_default_buffer_allocation_unbounded()
  {
    return test_default_buffer_allocation<unbounded>();
  }

  int test_default_buffer_allocation_bounded()
  {
    return test_default_buffer_allocation<bounded>();
  }

  int test_allocbuf_unbounded()
  {
    return test_allocbuf<unbounded>();
  }

  int test_allocbuf_bounded()
  {
    return test_allocbuf<bounded>();
  }

  int test_freebuf_unbounded()
  {
    return test_freebuf<unbounded>();
  }

  int test_freebuf_bounded()
  {
    return test_freebuf<bounded>();
  }

  int test_all()
  {
    int status = 0;
    status += this->test_default_buffer_allocation_value();
    status += this->test_default_buffer_allocation_unbounded();
    status += this->test_default_buffer_allocation_bounded();
    status += this->test_allocbuf_unbounded();
    status += this->test_allocbuf_bounded();
    status += this->test_freebuf_unbounded();
    status += this->test_freebuf_bounded();
     return status;
  }
  Tester() {}
};

struct Foo { int y; };

int ACE_TMAIN(int,ACE_TCHAR*[])
{
  int status = 0;

  try
    {
      {
        Tester<int>  tester;
        status += tester.test_all ();
      }

      {
        Tester<Foo> tester;
        status += tester.test_all ();
      }

      {
        Tester<char*>  tester;
        status += tester.test_all ();
      }
    }
  catch (const ::CORBA::Exception &ex)
    {
      ex._tao_print_exception("ERROR : unexpected CORBA exception caugth :");
      ++status;
    }

  return status;
}
