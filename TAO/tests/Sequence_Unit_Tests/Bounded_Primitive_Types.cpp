/**
 * @file
 *
 * @brief Unit tests for bounded sequences, primitive-type case.
 *
 * Verify that bounded sequences of primitive types, self-managed
 * types work correctly.  Such types include all the integer types,
 * the floating point types but do not include strings as those are
 * not self-managed.  The fundamental difference with other
 * self-managed types, such as structures, is that the constructors
 * and assignment operators of primitive types cannot throw.
 *
 * $Id$
 *
 * @author Carlos O'Ryan
 */
#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>

#include "sequence_traits.hpp"
#include "generic_sequence.hpp"

#include <sstream>
#include <stdexcept>
#include <iostream>

struct local_exception {};

CORBA::ULong const MAXIMUM = 32;

namespace TAO
{
namespace details
{

template<>
struct bounded_sequence_traits<int,MAXIMUM>
{
  static long allocbuf_failure_counter;
  static long allocbuf_calls;
  static long freebuf_calls;

  typedef int value_type;
  inline static void check_index(
      CORBA::ULong index,
      CORBA::ULong length,
      CORBA::ULong maximum,
      char const * function_name)
  {
    if (index < length) {
      return;
    }
    std::ostringstream error;
    error << "Out of range access in " << function_name
          << ", index=" << index
          << ", length=" << length
          << ", maximum=" << maximum
          << ", class=bounded_sequence<int>";
    throw std::range_error(error.str());
  }

  inline static CORBA::ULong default_maximum()
  {
    return MAXIMUM;
  }

  inline static int * default_buffer_allocation()
  {
    return allocbuf(MAXIMUM);
  }

  inline static value_type * allocbuf(CORBA::ULong maximum)
  {
    ++allocbuf_calls;
    if (--allocbuf_failure_counter == 0)
    {
      throw local_exception();
    }
    if (maximum != MAXIMUM)
    {
      throw local_exception();
    }
    return new value_type[MAXIMUM];
  }

  inline static void freebuf(value_type * buffer)
  {
    ++freebuf_calls;
    delete[] buffer;
  }
};

} // namespace details
} // namespace TAO

#include "sequence.hpp"

using namespace TAO;
using namespace TAO::details;

typedef bounded_sequence<int,MAXIMUM> tested_seq;
typedef bounded_sequence_traits<int,MAXIMUM> test_traits;

long test_traits::allocbuf_failure_counter = 0;
long test_traits::allocbuf_calls = 0;
long test_traits::freebuf_calls = 0;


BOOST_AUTO_UNIT_TEST(test_allocations_in_constructors)
{
  long a = test_traits::allocbuf_calls;
  long f = test_traits::freebuf_calls;

  {
    tested_seq default_constructed;
  }
  BOOST_CHECK_EQUAL(++a, test_traits::allocbuf_calls);
  BOOST_CHECK_EQUAL(++f, test_traits::freebuf_calls);
}

void init(CORBA::ULong count, tested_seq & a)
{
  a.length(count);
  for(CORBA::ULong i = 0; i != count; ++i) {
    a[i] = int(i);
  }
}

void check_init(CORBA::ULong count, tested_seq const & a)
{
  BOOST_CHECK_EQUAL(CORBA::ULong(count), a.length());
  BOOST_CHECK_EQUAL(MAXIMUM, a.maximum());
  for(CORBA::ULong i = 0; i != count; ++i) {
    BOOST_CHECK_EQUAL(int(i), a[i]);
  }
}

void check_equal(
    tested_seq const & a,
    tested_seq const & b)
{
  BOOST_CHECK_EQUAL(a.length(), b.length());
  BOOST_CHECK_EQUAL(a.maximum(), b.maximum());
  for(CORBA::ULong i = 0; i != a.length(); ++i) {
    BOOST_CHECK_EQUAL(a[i], b[i]);
  }
}

BOOST_AUTO_UNIT_TEST(test_default_constructor)
{
  tested_seq default_constructed;

  BOOST_CHECK_EQUAL(CORBA::ULong(0), default_constructed.length());
  BOOST_CHECK_EQUAL(MAXIMUM, default_constructed.maximum());
}

BOOST_AUTO_UNIT_TEST(test_copy_constructor_from_default)
{
  tested_seq default_constructed;
  tested_seq copy_constructed(default_constructed);
  BOOST_CHECK_EQUAL(CORBA::ULong(0), copy_constructed.length());
  BOOST_CHECK_EQUAL(MAXIMUM, copy_constructed.maximum());
}

BOOST_AUTO_UNIT_TEST(test_set_length)
{
  tested_seq x;

  x.length(16);
  BOOST_CHECK_EQUAL(CORBA::ULong(16), x.length());
  BOOST_CHECK(CORBA::ULong(16) <= x.maximum());

  x.length(8);
  BOOST_CHECK_EQUAL(CORBA::ULong(8), x.length());
  BOOST_CHECK(CORBA::ULong(8) <= x.maximum());
}

BOOST_AUTO_UNIT_TEST(test_setting_and_copy)
{
  tested_seq a;
  init(16, a);
  check_init(16, a);

  tested_seq b(a);
  check_equal(a, b);
}

BOOST_AUTO_UNIT_TEST(test_setting_and_assign)
{
  tested_seq a;
  init(16, a);
  check_init(16, a);

  tested_seq c;
  c = a;
  check_equal(a, c);
}

BOOST_AUTO_UNIT_TEST(test_exceptions_in_default_constructor)
{
  long freebuf_calls = test_traits::freebuf_calls;
  long allocbuf_calls = test_traits::allocbuf_calls;

  test_traits::allocbuf_failure_counter = 1;
  BOOST_CHECK_THROW(tested_seq dst, local_exception);

  BOOST_CHECK_EQUAL(
      freebuf_calls,
      test_traits::freebuf_calls);
  BOOST_CHECK_EQUAL(
      allocbuf_calls + 1,
      test_traits::allocbuf_calls);
}

BOOST_AUTO_UNIT_TEST(test_exceptions_in_copy_constructor)
{
  tested_seq src; src.length(16);
  for(CORBA::ULong i = 0; i != 16UL; ++i)
    src[i] = i;

  long freebuf_calls = test_traits::freebuf_calls;
  long allocbuf_calls = test_traits::allocbuf_calls;

  test_traits::allocbuf_failure_counter = 1;
  BOOST_CHECK_THROW(tested_seq dst(src), local_exception);

  BOOST_CHECK_EQUAL(
      freebuf_calls,
      test_traits::freebuf_calls);
  BOOST_CHECK_EQUAL(
      allocbuf_calls + 1,
      test_traits::allocbuf_calls);
}

BOOST_AUTO_UNIT_TEST(test_exceptions_in_assignment)
{
  tested_seq src; src.length(16);
  for(CORBA::ULong i = 0; i != 16UL; ++i)
    src[i] = i;

  tested_seq dst; dst.length(8);
  dst[0] = 0; dst[6] = 5;

  long freebuf_calls = test_traits::freebuf_calls;
  long allocbuf_calls = test_traits::allocbuf_calls;

  test_traits::allocbuf_failure_counter = 1;
  BOOST_CHECK_THROW(dst = src, local_exception);

  BOOST_CHECK_EQUAL(
      freebuf_calls,
      test_traits::freebuf_calls);
  BOOST_CHECK_EQUAL(
      allocbuf_calls + 1,
      test_traits::allocbuf_calls);

  BOOST_CHECK_EQUAL(8UL, dst.length());
  BOOST_CHECK_EQUAL(0, dst[0]);
  BOOST_CHECK_EQUAL(5, dst[6]);
}

BOOST_AUTO_UNIT_TEST(test_exceptions_in_set_length)
{
  tested_seq src; src.length(16);
  for(CORBA::ULong i = 0; i != 16UL; ++i)
    src[i] = i;

  long freebuf_calls = test_traits::freebuf_calls;
  long allocbuf_calls = test_traits::allocbuf_calls;

  test_traits::allocbuf_failure_counter = 1;
  BOOST_CHECK_THROW(src.length(64), local_exception);

  BOOST_CHECK_EQUAL(
      freebuf_calls,
      test_traits::freebuf_calls);
  BOOST_CHECK_EQUAL(
      allocbuf_calls + 1,
      test_traits::allocbuf_calls);

  BOOST_CHECK_EQUAL(16UL, src.length());
  for(CORBA::ULong i = 0; i != 16UL; ++i)
    BOOST_CHECK_EQUAL(int(i), src[i]);
}

BOOST_AUTO_UNIT_TEST(test_check_index_is_called)
{
  tested_seq a;
  init(16, a);

  try
  {
    a[16] = 16;
    BOOST_ERROR("Missing range error exception");
  }
  catch(std::range_error const &) {}
  catch(...) {
    BOOST_ERROR("Unexpected exception raised");
  }
  check_init(16, a);
}

int * init_data()
{
  int * data = tested_seq::allocbuf(MAXIMUM);
  data[0] = 1; data[1] = 4; data[2] = 9;

  return data;
}

void check_data(tested_seq const & a)
{
  BOOST_CHECK_EQUAL(MAXIMUM, a.maximum());
  BOOST_CHECK_EQUAL(8UL,  a.length());
  BOOST_CHECK_EQUAL(1, a[0]);
  BOOST_CHECK_EQUAL(4, a[1]);
  BOOST_CHECK_EQUAL(9, a[2]);
}


BOOST_AUTO_UNIT_TEST(test_buffer_constructor_default)
{
  int * data = init_data();

  long a = test_traits::allocbuf_calls;
  long f = test_traits::freebuf_calls;
  {
    tested_seq a(8, data);
    check_data(a);
    BOOST_CHECK_EQUAL(false, a.release());
  }
  BOOST_CHECK_EQUAL(a, test_traits::allocbuf_calls);
  BOOST_CHECK_EQUAL(f, test_traits::freebuf_calls);
  tested_seq::freebuf(data);
}

BOOST_AUTO_UNIT_TEST(test_buffer_constructor_false)
{
  int * data = init_data();
  long a = test_traits::allocbuf_calls;
  long f = test_traits::freebuf_calls;
  {
    tested_seq a(8, data, false);
    check_data(a);
    BOOST_CHECK_EQUAL(false, a.release());
  }
  BOOST_CHECK_EQUAL(a, test_traits::allocbuf_calls);
  BOOST_CHECK_EQUAL(f, test_traits::freebuf_calls);
}

BOOST_AUTO_UNIT_TEST(test_buffer_constructor_true)
{
  int * data = init_data();
  long a = test_traits::allocbuf_calls;
  long f = test_traits::freebuf_calls;
  {
    tested_seq a(8, data, true);
    check_data(a);
    BOOST_CHECK_EQUAL(true, a.release());
  }
  BOOST_CHECK_EQUAL(a, test_traits::allocbuf_calls);
  BOOST_CHECK_EQUAL(++f, test_traits::freebuf_calls);
}

BOOST_AUTO_UNIT_TEST(test_replace_default)
{
  int * data = init_data();

  long a = test_traits::allocbuf_calls;
  long f = test_traits::freebuf_calls;
  {
    tested_seq a;
    a.replace(8, data);
    check_data(a);
    BOOST_CHECK_EQUAL(false, a.release());
  }
  BOOST_CHECK_EQUAL(++a, test_traits::allocbuf_calls);
  BOOST_CHECK_EQUAL(++f, test_traits::freebuf_calls);
  tested_seq::freebuf(data);
}

BOOST_AUTO_UNIT_TEST(test_replace_false)
{
  int * data = init_data();

  long a = test_traits::allocbuf_calls;
  long f = test_traits::freebuf_calls;
  {
    tested_seq a;
    a.replace(8, data, false);
    check_data(a);
    BOOST_CHECK_EQUAL(false, a.release());
  }
  BOOST_CHECK_EQUAL(++a, test_traits::allocbuf_calls);
  BOOST_CHECK_EQUAL(++f, test_traits::freebuf_calls);
  tested_seq::freebuf(data);
}

BOOST_AUTO_UNIT_TEST(test_replace_true)
{
  int * data = init_data();

  long a = test_traits::allocbuf_calls;
  long f = test_traits::freebuf_calls;
  {
    tested_seq a;
    a.replace(8, data, true);
    check_data(a);
    BOOST_CHECK_EQUAL(true, a.release());
  }
  BOOST_CHECK_EQUAL(++a, test_traits::allocbuf_calls);
  ++f; ++f;
  BOOST_CHECK_EQUAL(f, test_traits::freebuf_calls);
}

BOOST_AUTO_UNIT_TEST(test_get_buffer_const)
{
  int * data = init_data();
  tested_seq a(8, data, true);
  tested_seq const & b = a;
  BOOST_CHECK_EQUAL(b.get_buffer(), data);
}

BOOST_AUTO_UNIT_TEST(test_get_buffer_default)
{
  int * data = init_data();
  tested_seq a(8, data, true);
  BOOST_CHECK_EQUAL(a.get_buffer(), data);
}

BOOST_AUTO_UNIT_TEST(test_get_buffer_false)
{
  int * data = init_data();
  tested_seq a(8, data, true);
  BOOST_CHECK_EQUAL(a.get_buffer(), data);
}

BOOST_AUTO_UNIT_TEST(test_get_buffer_true_with_release_false)
{
  int * data = init_data();
  tested_seq a(8, data, false);
  BOOST_CHECK_EQUAL(static_cast<int*>(0), a.get_buffer(true));
  tested_seq::freebuf(data);
}

BOOST_AUTO_UNIT_TEST(test_get_buffer_true_with_release_true)
{
  int * data = init_data();
  tested_seq a(8, data, true);
  BOOST_CHECK_EQUAL(data, a.get_buffer(true));
  tested_seq const & b = a;
  BOOST_CHECK_EQUAL(MAXIMUM, b.maximum());
  BOOST_CHECK_EQUAL(0UL, b.length());
  BOOST_CHECK(0 != b.get_buffer());
  BOOST_CHECK(data != b.get_buffer());
  tested_seq::freebuf(data);
}

BOOST_AUTO_UNIT_TEST(test_allocbuf_default)
{
  int * data = tested_seq::allocbuf();
  tested_seq::freebuf(data);
}

BOOST_AUTO_UNIT_TEST(test_wrong_allocbuf_throws)
{
  BOOST_CHECK_THROW(tested_seq::allocbuf(64), local_exception);
  BOOST_CHECK_THROW(tested_seq::allocbuf(16), local_exception);
}

#if !defined(BOOST_AUTO_TEST_MAIN)
// This is just to convince MPC that I do not need a main() to have a
// program.
int main() {}
#endif
