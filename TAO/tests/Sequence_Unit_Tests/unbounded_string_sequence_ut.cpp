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

    BOOST_CHECK_MESSAGE(f.expect(1), f);
    BOOST_CHECK_MESSAGE(r.expect(32), r);
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
      BOOST_CHECK_EQUAL(true, x.release());

      tested_sequence y;
      BOOST_CHECK_MESSAGE(a.expect(0), a);

      y = x;
      BOOST_CHECK_MESSAGE(a.expect(1), a);
      BOOST_CHECK_MESSAGE(f.expect(1), f);
      BOOST_CHECK_EQUAL(x.maximum(), y.maximum());
      BOOST_CHECK_EQUAL(x.length(), y.length());
      BOOST_CHECK_EQUAL(x.release(), y.release());
    }
    BOOST_CHECK_MESSAGE(f.expect(2), f);
  }

  void test_assignment_values()
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
      tested_sequence b;
      b = a;
      BOOST_CHECK_MESSAGE(d.expect(16), d);

      BOOST_CHECK_EQUAL(a.maximum(), b.maximum());
      BOOST_CHECK_EQUAL(a.length(),  b.length());
      BOOST_CHECK_EQUAL(a.release(), b.release());
      for(CORBA::ULong i = 0; i != a.length(); ++i)
      {
        BOOST_CHECK_MESSAGE(ACE_OS::strcmp(a[i], b[i]) == 0,
            "Mismatched elements at index " << i);
      }
    }
    BOOST_CHECK_MESSAGE(r.expect(16), r);
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

  void test_exception_in_ulong_constructor()
  {
    expected_calls a(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    {
      tested_allocation_traits::allocbuf_calls.failure_countdown(1);
      BOOST_CHECK_THROW(tested_sequence x(16), testing_exception);
      BOOST_CHECK_MESSAGE(a.expect(1), a);
    }
    BOOST_CHECK_MESSAGE(f.expect(0), f);
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

  void test_duplicate_exception_in_copy_constructor()
  {
    expected_calls f(tested_allocation_traits::freebuf_calls);
    {
      tested_sequence x; x.length(8);
      f.reset();

      for(CORBA::ULong i = 0; i != 8; ++i)
      {
        x[i] = tested_element_traits::duplicate("foo bar baz");
      }

      expected_calls a(tested_allocation_traits::allocbuf_calls);
      expected_calls d(tested_element_traits::duplicate_calls);
      expected_calls r(tested_element_traits::release_calls);

      tested_element_traits::duplicate_calls.failure_countdown(4);

      BOOST_CHECK_THROW(tested_sequence y(x), testing_exception);
      BOOST_CHECK_MESSAGE(a.expect(1), a);
      BOOST_CHECK_MESSAGE(f.expect(1), f);
      BOOST_CHECK_MESSAGE(d.expect(4), d);
      BOOST_CHECK_MESSAGE(r.expect(8), r);
    }
    BOOST_CHECK_MESSAGE(f.expect(1), f);
  }

  void test_exception_in_length()
  {
    expected_calls f(tested_allocation_traits::freebuf_calls);
    expected_calls a(tested_allocation_traits::allocbuf_calls);
    expected_calls d(tested_element_traits::duplicate_calls);
    expected_calls r(tested_element_traits::release_calls);
    {
      tested_sequence y; y.length(4);
      for(CORBA::ULong i = 0; i != 4; ++i)
      {
        y[i] = tested_element_traits::duplicate("check");
      }

      a.reset();
      d.reset();
      r.reset();
      f.reset();
      tested_allocation_traits::allocbuf_calls.failure_countdown(1);
      BOOST_CHECK_THROW(y.length(8), testing_exception);
      BOOST_CHECK_MESSAGE(a.expect(1), a);
      BOOST_CHECK_MESSAGE(f.expect(0), f);
      BOOST_CHECK_MESSAGE(d.expect(0), d);
      BOOST_CHECK_MESSAGE(r.expect(0), r);

      BOOST_CHECK_EQUAL(CORBA::ULong(4), y.length());
      for(CORBA::ULong i = 0; i != 4; ++i)
      {
        BOOST_CHECK_MESSAGE(
            ACE_OS::strcmp(
                const_cast<const_value_type>("check"),
                y[i]) == 0,
            "Mismatch in element " << i
            << ", expected=" << const_cast<const_value_type>("check")
            << ", got=" << y[i]);
      }
    }
    BOOST_CHECK_MESSAGE(f.expect(1), f);
  }

  void test_duplicate_exception_in_assignment()
  {
    expected_calls f(tested_allocation_traits::freebuf_calls);
    {
      tested_sequence x; x.length(8);
      f.reset();

      for(CORBA::ULong i = 0; i != 8; ++i)
      {
        x[i] = tested_element_traits::duplicate("foo bar baz");
      }

      expected_calls a(tested_allocation_traits::allocbuf_calls);
      expected_calls d(tested_element_traits::duplicate_calls);
      expected_calls r(tested_element_traits::release_calls);


      {
        tested_sequence y; y.length(4);
        for(CORBA::ULong i = 0; i != 4; ++i)
        {
          y[i] = tested_element_traits::duplicate("check");
        }

        a.reset();
        d.reset();
        r.reset();
        f.reset();
        tested_element_traits::duplicate_calls.failure_countdown(4);
        BOOST_CHECK_THROW(y = x, testing_exception);
        BOOST_CHECK_MESSAGE(a.expect(1), a);
        BOOST_CHECK_MESSAGE(f.expect(1), f);
        BOOST_CHECK_MESSAGE(d.expect(4), d);
        BOOST_CHECK_MESSAGE(r.expect(8), r);

        BOOST_CHECK_EQUAL(CORBA::ULong(4), y.length());
        for(CORBA::ULong i = 0; i != 4; ++i)
        {
          BOOST_CHECK_MESSAGE(
              ACE_OS::strcmp(
                  const_cast<const_value_type>("check"),
                  y[i]) == 0,
              "Mismatch in element " << i
              << ", expected=" << const_cast<const_value_type>("check")
              << ", got=" << y[i]);
        }
      }
      BOOST_CHECK_MESSAGE(f.expect(1), f);
    }
    BOOST_CHECK_MESSAGE(f.expect(1), f);
  }

  void test_duplicate_exception_in_length()
  {
    expected_calls f(tested_allocation_traits::freebuf_calls);
    expected_calls a(tested_allocation_traits::allocbuf_calls);
    expected_calls d(tested_element_traits::duplicate_calls);
    expected_calls r(tested_element_traits::release_calls);
    {
      tested_sequence y; y.length(4);
      for(CORBA::ULong i = 0; i != 4; ++i)
      {
        y[i] = tested_element_traits::duplicate("check");
      }

      a.reset();
      d.reset();
      r.reset();
      f.reset();
      tested_element_traits::duplicate_calls.failure_countdown(3);
      BOOST_CHECK_THROW(y.length(8), testing_exception);
      BOOST_CHECK_MESSAGE(a.expect(1), a);
      BOOST_CHECK_MESSAGE(f.expect(1), f);
      BOOST_CHECK_MESSAGE(d.expect(3), d);
      BOOST_CHECK_MESSAGE(r.expect(8), r);

      BOOST_CHECK_EQUAL(CORBA::ULong(4), y.length());
      for(CORBA::ULong i = 0; i != 4; ++i)
      {
        BOOST_CHECK_MESSAGE(
            ACE_OS::strcmp(
                const_cast<const_value_type>("check"),
                y[i]) == 0,
            "Mismatch in element " << i
            << ", expected=" << const_cast<const_value_type>("check")
            << ", got=" << y[i]);
      }
    }
    BOOST_CHECK_MESSAGE(f.expect(1), f);
  }

  void test_get_buffer_const()
  {
    tested_sequence a(8); a.length(8);
    tested_sequence const & b = a;

    const_value_type const * buffer = b.get_buffer();
    a[0] = const_value_type("abcd");

    BOOST_CHECK_EQUAL(buffer, b.get_buffer());
    BOOST_CHECK_MESSAGE(ACE_OS::strcmp(a[0], buffer[0]) == 0,
        "Mismatched elements a[0]=" << a[0]
        << ", buffer[0]=" << buffer[0]);
  }

  value_type * alloc_and_init_buffer()
  {
    value_type * buf = tested_sequence::allocbuf(8);
    buf[0] = tested_element_traits::duplicate("1");
    buf[1] = tested_element_traits::duplicate("4");
    buf[2] = tested_element_traits::duplicate("9");
    buf[3] = tested_element_traits::duplicate("16");

    return buf;
  }

  void check_values(tested_sequence const & a)
  {
    BOOST_CHECK_EQUAL(const_cast<char const*>("1"),  a[0]);
    BOOST_CHECK_EQUAL(const_cast<char const*>("4"),  a[1]);
    BOOST_CHECK_EQUAL(const_cast<char const*>("9"),  a[2]);
    BOOST_CHECK_EQUAL(const_cast<char const*>("16"), a[3]);
  }

  void test_buffer_constructor_default()
  {
    value_type * buffer = alloc_and_init_buffer();
    expected_calls a(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    expected_calls r(tested_element_traits::release_calls);
    {
      tested_sequence a(8, 4, buffer);
      BOOST_CHECK_EQUAL(CORBA::ULong(8), a.maximum());
      BOOST_CHECK_EQUAL(CORBA::ULong(4), a.length());
      BOOST_CHECK_EQUAL(buffer, a.get_buffer());
      BOOST_CHECK_EQUAL(false, a.release());
      check_values(a);
    }
    BOOST_CHECK_MESSAGE(a.expect(0), a);
    BOOST_CHECK_MESSAGE(f.expect(0), f);
    tested_sequence::freebuf(buffer);
    BOOST_CHECK_MESSAGE(r.expect(8), r);
  }

  void test_buffer_constructor_false()
  {
    value_type * buffer = alloc_and_init_buffer();
    expected_calls a(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    expected_calls r(tested_element_traits::release_calls);
    {
      tested_sequence a(8, 4, buffer, false);
      BOOST_CHECK_EQUAL(CORBA::ULong(8), a.maximum());
      BOOST_CHECK_EQUAL(CORBA::ULong(4), a.length());
      BOOST_CHECK_EQUAL(buffer, a.get_buffer());
      BOOST_CHECK_EQUAL(false, a.release());
      check_values(a);
    }
    BOOST_CHECK_MESSAGE(a.expect(0), a);
    BOOST_CHECK_MESSAGE(f.expect(0), f);
    tested_sequence::freebuf(buffer);
    BOOST_CHECK_MESSAGE(r.expect(8), r);
  }

  void test_buffer_constructor_true()
  {
    value_type * buffer = alloc_and_init_buffer();
    expected_calls a(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    expected_calls r(tested_element_traits::release_calls);
    {
      tested_sequence a(8, 4, buffer, true);
      BOOST_CHECK_EQUAL(CORBA::ULong(8), a.maximum());
      BOOST_CHECK_EQUAL(CORBA::ULong(4), a.length());
      BOOST_CHECK_EQUAL(buffer, a.get_buffer());
      BOOST_CHECK_EQUAL(true, a.release());
      check_values(a);
    }
    BOOST_CHECK_MESSAGE(a.expect(0), a);
    BOOST_CHECK_MESSAGE(f.expect(1), f);
    BOOST_CHECK_MESSAGE(r.expect(8), r);
  }

  void test_replace_default()
  {
    value_type * buffer = alloc_and_init_buffer();

    expected_calls c(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    expected_calls r(tested_element_traits::release_calls);
    {
      tested_sequence a;
      a.replace(8, 4, buffer);
      BOOST_CHECK_MESSAGE(c.expect(0), c);
      BOOST_CHECK_MESSAGE(f.expect(1), f);
      BOOST_CHECK_MESSAGE(r.expect(0), 0);

      BOOST_CHECK_EQUAL(CORBA::ULong(8), a.maximum());
      BOOST_CHECK_EQUAL(CORBA::ULong(4), a.length());
      BOOST_CHECK_EQUAL(buffer, a.get_buffer());
      BOOST_CHECK_EQUAL(false, a.release());
      check_values(a);
    }
    BOOST_CHECK_MESSAGE(c.expect(0), c);
    BOOST_CHECK_MESSAGE(f.expect(0), f);
    tested_sequence::freebuf(buffer);
    BOOST_CHECK_MESSAGE(r.expect(8), r);
  }

  void test_replace_false()
  {
    value_type * buffer = alloc_and_init_buffer();

    expected_calls c(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    expected_calls r(tested_element_traits::release_calls);
    {
      tested_sequence a;
      a.replace(8, 4, buffer, false);
      BOOST_CHECK_MESSAGE(c.expect(0), c);
      BOOST_CHECK_MESSAGE(f.expect(1), f);
      BOOST_CHECK_MESSAGE(r.expect(0), 0);

      BOOST_CHECK_EQUAL(CORBA::ULong(8), a.maximum());
      BOOST_CHECK_EQUAL(CORBA::ULong(4), a.length());
      BOOST_CHECK_EQUAL(buffer, a.get_buffer());
      BOOST_CHECK_EQUAL(false, a.release());
      check_values(a);
    }
    BOOST_CHECK_MESSAGE(c.expect(0), c);
    BOOST_CHECK_MESSAGE(f.expect(0), f);
    tested_sequence::freebuf(buffer);
    BOOST_CHECK_MESSAGE(r.expect(8), r);
  }

  void test_replace_true()
  {
    value_type * buffer = alloc_and_init_buffer();

    expected_calls c(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    expected_calls r(tested_element_traits::release_calls);
    {
      tested_sequence a;
      a.replace(8, 4, buffer, false);
      BOOST_CHECK_MESSAGE(c.expect(0), c);
      BOOST_CHECK_MESSAGE(f.expect(1), f);
      BOOST_CHECK_MESSAGE(r.expect(0), r);

      BOOST_CHECK_EQUAL(CORBA::ULong(8), a.maximum());
      BOOST_CHECK_EQUAL(CORBA::ULong(4), a.length());
      BOOST_CHECK_EQUAL(buffer, a.get_buffer());
      BOOST_CHECK_EQUAL(false, a.release());
      check_values(a);
    }
    BOOST_CHECK_MESSAGE(c.expect(0), c);
    BOOST_CHECK_MESSAGE(f.expect(0), f);
    tested_sequence::freebuf(buffer);
    BOOST_CHECK_MESSAGE(r.expect(8), r);
  }

  void test_get_buffer_default()
  {
    value_type * buffer = alloc_and_init_buffer();
    tested_sequence a(8, 4, buffer, true);
    BOOST_CHECK_EQUAL(a.get_buffer(), buffer);
  }

  void test_get_buffer_false()
  {
    value_type * buffer = alloc_and_init_buffer();
    tested_sequence a(8, 4, buffer, true);
    BOOST_CHECK_EQUAL(a.get_buffer(), buffer);
  }

  void test_get_buffer_true_with_release_false()
  {
    value_type * buffer = alloc_and_init_buffer();
    tested_sequence a(8, 4, buffer, false);
    BOOST_CHECK_EQUAL(static_cast<value_type*>(0), a.get_buffer(true));
    tested_sequence::freebuf(buffer);
  }

  void test_get_buffer_true_with_release_true()
  {
    value_type * buffer = alloc_and_init_buffer();
    expected_calls c(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    expected_calls r(tested_element_traits::release_calls);
    {
      tested_sequence a(8, 4, buffer, true);
      BOOST_CHECK_EQUAL(buffer, a.get_buffer(true));

      tested_sequence const & b = a;
      BOOST_CHECK_EQUAL(0UL, b.maximum());
      BOOST_CHECK_EQUAL(0UL, b.length());
      BOOST_CHECK(0 != b.get_buffer());

      BOOST_CHECK_MESSAGE(c.expect(1), c);

      BOOST_CHECK(buffer != b.get_buffer());
    }
    BOOST_CHECK_MESSAGE(c.expect(0), c);
    BOOST_CHECK_MESSAGE(f.expect(1), f);
    tested_sequence::freebuf(buffer);
    BOOST_CHECK_MESSAGE(r.expect(8), r);
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
    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_freebuf_releases_elements,
                shared_this));

    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_assignment_from_default,
                shared_this));
    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_assignment_values,
                shared_this));

    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_exception_in_copy_constructor,
                shared_this));
    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_exception_in_ulong_constructor,
                shared_this));
    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_exception_in_assignment,
                shared_this));

    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_duplicate_exception_in_copy_constructor,
                shared_this));
    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_duplicate_exception_in_assignment,
                shared_this));
    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_exception_in_length,
                shared_this));
    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_duplicate_exception_in_length,
                shared_this));


    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_get_buffer_const,
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

test_suite *
init_unit_test_suite(int, char*[])
{
  std::auto_ptr<test_suite> ts(
      BOOST_TEST_SUITE("unbounded string sequence unit test"));

  boost::shared_ptr<Tester> tester(Tester::allocate());
  tester->add_all(ts.get());

  return ts.release();
}

