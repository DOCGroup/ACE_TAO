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
        "testing_range_checking.hpp"

#include "testing_string_traits.hpp"
#include "tao/String_Traits_T.h"
#include "testing_allocation_traits.hpp"

#include "tao/Bounded_Basic_String_Sequence_T.h"
#include "tao/CORBA_String.h"

#include "string_sequence_tester.hpp"

#include "ace/OS_NS_string.h"

#include "test_macros.h"


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

  int test_set_length_less_than_maximum()
  {
    expected_calls a(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    expected_calls i(tested_element_traits::default_initializer_calls);
    {
      tested_sequence x;

      x.length(8);
      // length() after default constructed sequence leads to
      // buffer allocation.
      FAIL_RETURN_IF_NOT(a.expect(1), a);
      CHECK_EQUAL(CORBA::ULong(MAXIMUM), x.maximum());
      CHECK_EQUAL(CORBA::ULong(8), x.length());
      CHECK_EQUAL(true, x.release());

      // 32 is here because allocbuf for bounded sequences
      // initializes all elements.
      FAIL_RETURN_IF_NOT(i.expect(32), i);
    }
    FAIL_RETURN_IF_NOT(f.expect(1), f);
    return 0;
  }

  int test_set_length_more_than_maximum()
  {
    tested_sequence x;

    CHECK_THROW(x.length(64), std::runtime_error);
    return 0;
  }

  value_type * alloc_and_init_buffer()
  {
    value_type * buf = tested_sequence::allocbuf();
    delete[] buf[0];
    buf[0] = helper::to_string(1);
    delete[] buf[1];
    buf[1] = helper::to_string(4);
    delete[] buf[2];
    buf[2] = helper::to_string(9);
    delete[] buf[3];
    buf[3] = helper::to_string(16);

    return buf;
  }

  int check_values(tested_sequence const & a)
  {
    CHECK(helper::compare(1,  a[0]));
    CHECK(helper::compare(4,  a[1]));
    CHECK(helper::compare(9,  a[2]));
    CHECK(helper::compare(16, a[3]));
    return 0;
  }

  int test_regression_2201()
  {
    value_type * buffer = alloc_and_init_buffer();
    expected_calls a(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    expected_calls r(tested_element_traits::release_calls);
    {
      tested_sequence x(4, buffer, true);
      CHECK_EQUAL(CORBA::ULong(MAXIMUM), x.maximum());
      CHECK_EQUAL(CORBA::ULong(4), x.length());
      CHECK_EQUAL(buffer, x.get_buffer());
      CHECK_EQUAL(true, x.release());
      x.length (3);
      CHECK_EQUAL(CORBA::ULong(3), x.length());
      x.length (4);
      CHECK_EQUAL(CORBA::ULong(4), x.length());
      CHECK(helper::compare_empty(x[3]));
    }
    FAIL_RETURN_IF_NOT(a.expect(0), a);
    FAIL_RETURN_IF_NOT(f.expect(1), f);
    // 1 additional release call happens when we shrink
    // the sequence to length 3.
    FAIL_RETURN_IF_NOT(r.expect(MAXIMUM + 1), r);
    return 0;
  }

  int test_buffer_constructor_default()
  {
    value_type * buffer = alloc_and_init_buffer();
    expected_calls a(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    expected_calls r(tested_element_traits::release_calls);
    {
      tested_sequence a(4, buffer, false);
      CHECK_EQUAL(CORBA::ULong(MAXIMUM), a.maximum());
      CHECK_EQUAL(CORBA::ULong(4), a.length());
      CHECK_EQUAL(buffer, a.get_buffer());
      CHECK_EQUAL(false, a.release());
      check_values(a);
    }
    FAIL_RETURN_IF_NOT(a.expect(0), a);
    FAIL_RETURN_IF_NOT(f.expect(0), f);
    tested_sequence::freebuf(buffer);
    FAIL_RETURN_IF_NOT(r.expect(MAXIMUM), r);
    return 0;
  }

  int test_buffer_constructor_false()
  {
    value_type * buffer = alloc_and_init_buffer();
    expected_calls a(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    expected_calls r(tested_element_traits::release_calls);
    {
      tested_sequence a(4, buffer, false);
      CHECK_EQUAL(CORBA::ULong(MAXIMUM), a.maximum());
      CHECK_EQUAL(CORBA::ULong(4), a.length());
      CHECK_EQUAL(buffer, a.get_buffer());
      CHECK_EQUAL(false, a.release());
      check_values(a);
    }
    FAIL_RETURN_IF_NOT(a.expect(0), a);
    FAIL_RETURN_IF_NOT(f.expect(0), f);
    tested_sequence::freebuf(buffer);
    FAIL_RETURN_IF_NOT(r.expect(MAXIMUM), r);
    return 0;
  }

  int test_buffer_constructor_true()
  {
    value_type * buffer = alloc_and_init_buffer();
    expected_calls a(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    expected_calls r(tested_element_traits::release_calls);
    {
      tested_sequence a(4, buffer, true);
      CHECK_EQUAL(CORBA::ULong(MAXIMUM), a.maximum());
      CHECK_EQUAL(CORBA::ULong(4), a.length());
      CHECK_EQUAL(buffer, a.get_buffer());
      CHECK_EQUAL(true, a.release());
      check_values(a);
    }
    FAIL_RETURN_IF_NOT(a.expect(0), a);
    FAIL_RETURN_IF_NOT(f.expect(1), f);
    FAIL_RETURN_IF_NOT(r.expect(MAXIMUM), r);
    return 0;
  }

  int test_replace_default()
  {
    value_type * buffer = alloc_and_init_buffer();

    expected_calls c(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    expected_calls r(tested_element_traits::release_calls);
    {
      tested_sequence a;
      a.replace(4, buffer);
      FAIL_RETURN_IF_NOT(c.expect(0), c);
      // Default constructed sequence doesn't allocate a buffer
      // thus nothing to release and free.
      FAIL_RETURN_IF_NOT(f.expect(0), f);
      FAIL_RETURN_IF_NOT(r.expect(0), r);

      CHECK_EQUAL(CORBA::ULong(MAXIMUM), a.maximum());
      CHECK_EQUAL(CORBA::ULong(4), a.length());
      CHECK_EQUAL(buffer, a.get_buffer());
      CHECK_EQUAL(false, a.release());
      check_values(a);
    }
    FAIL_RETURN_IF_NOT(c.expect(0), c);
    FAIL_RETURN_IF_NOT(f.expect(0), f);
    tested_sequence::freebuf(buffer);
    FAIL_RETURN_IF_NOT(r.expect(MAXIMUM), r);
    return 0;
  }

  int test_replace_false()
  {
    value_type * buffer = alloc_and_init_buffer();

    expected_calls c(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    expected_calls r(tested_element_traits::release_calls);
    {
      tested_sequence a;
      a.replace(4, buffer, false);
      FAIL_RETURN_IF_NOT(c.expect(0), c);
      // Default constructed sequence doesn't allocate a buffer.
      FAIL_RETURN_IF_NOT(f.expect(0), f);
      FAIL_RETURN_IF_NOT(r.expect(0), r);

      CHECK_EQUAL(CORBA::ULong(MAXIMUM), a.maximum());
      CHECK_EQUAL(CORBA::ULong(4), a.length());
      CHECK_EQUAL(buffer, a.get_buffer());
      CHECK_EQUAL(false, a.release());
      check_values(a);
    }
    FAIL_RETURN_IF_NOT(c.expect(0), c);
    FAIL_RETURN_IF_NOT(f.expect(0), f);
    tested_sequence::freebuf(buffer);
    FAIL_RETURN_IF_NOT(r.expect(MAXIMUM), r);
    return 0;
  }

  int test_replace_true()
  {
    value_type * buffer = alloc_and_init_buffer();

    expected_calls c(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    expected_calls r(tested_element_traits::release_calls);
    {
      tested_sequence a;
      a.replace(4, buffer, true);
      FAIL_RETURN_IF_NOT(c.expect(0), c);
      // Default constructed sequence doesn't allocate a buffer.
      FAIL_RETURN_IF_NOT(f.expect(0), f);
      FAIL_RETURN_IF_NOT(r.expect(0), r);

      CHECK_EQUAL(CORBA::ULong(MAXIMUM), a.maximum());
      CHECK_EQUAL(CORBA::ULong(4), a.length());
      CHECK_EQUAL(buffer, a.get_buffer());
      CHECK_EQUAL(true, a.release());
      check_values(a);
    }
    FAIL_RETURN_IF_NOT(c.expect(0), c);
    // Since we've given away the ownership the buffer is deallocated by
    // the sequence.
    FAIL_RETURN_IF_NOT(f.expect(1), f);
    FAIL_RETURN_IF_NOT(r.expect(MAXIMUM), r);
    return 0;
  }

  int test_get_buffer_default()
  {
    value_type * buffer = alloc_and_init_buffer();
    tested_sequence a(4, buffer, true);
    CHECK_EQUAL(a.get_buffer(), buffer);
    return 0;
  }

  int test_get_buffer_false()
  {
    value_type * buffer = alloc_and_init_buffer();
    tested_sequence a(4, buffer, true);
    CHECK_EQUAL(a.get_buffer(), buffer);
    return 0;
  }

  int test_get_buffer_true_with_release_false()
  {
    value_type * buffer = alloc_and_init_buffer();
    tested_sequence a(4, buffer, false);
    CHECK(0 == a.get_buffer(true));
    tested_sequence::freebuf(buffer);
    return 0;
  }

  int test_get_buffer_true_with_release_true()
  {
    value_type * buffer = alloc_and_init_buffer();
    expected_calls c(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    expected_calls r(tested_element_traits::release_calls);
    {
      tested_sequence a(4, buffer, true);
      CHECK_EQUAL(buffer, a.get_buffer(true));

      tested_sequence const & b = a;
      CHECK_EQUAL(CORBA::ULong(MAXIMUM), b.maximum());
      CHECK_EQUAL(CORBA::ULong(0), b.length());
      CHECK(0 != b.get_buffer());
      CHECK_EQUAL(true, b.release());

      // Ownership was taken by get_buffer(true) and later get_buffer call
      // allocated a new buffer.
      FAIL_RETURN_IF_NOT(c.expect(1), c);

      CHECK(buffer != b.get_buffer());
    }
    FAIL_RETURN_IF_NOT(f.expect(1), f);
    FAIL_RETURN_IF_NOT(r.expect(MAXIMUM), r);
    FAIL_RETURN_IF_NOT(c.expect(0), c);

    tested_sequence::freebuf(buffer);
    FAIL_RETURN_IF_NOT(f.expect(1), f);
    FAIL_RETURN_IF_NOT(r.expect(MAXIMUM), r);
    return 0;
  }

  int test_all ()
  {
    int status = 0;

    typedef string_sequence_tester<tested_sequence> common;
    common X;
    status += X.test_all ();

    status += this->test_set_length_less_than_maximum();
    status += this->test_set_length_more_than_maximum();
    status += this->test_regression_2201();
    status += this->test_buffer_constructor_default();
    status += this->test_buffer_constructor_false();
    status += this->test_buffer_constructor_true();
    status += this->test_replace_default();
    status += this->test_replace_false();
    status += this->test_replace_true();
    status += this->test_get_buffer_default();
    status += this->test_get_buffer_false();
    status += this->test_get_buffer_true_with_release_false();
    status += this->test_get_buffer_true_with_release_true();
    return status;
  }
};

int ACE_TMAIN(int,ACE_TCHAR*[])
{
  int status = 0;

  try
    {
      typedef TAO::bounded_basic_string_sequence<char, MAXIMUM> s_sequence;
      typedef Tester<s_sequence> nTester;
      nTester myntester;

      status += myntester.test_all();

#if defined(ACE_HAS_WCHAR) && !defined(TAO_LACKS_WCHAR_CXX_STDLIB)
      typedef TAO::bounded_basic_string_sequence<CORBA::WChar, MAXIMUM> w_sequence;
      typedef Tester<w_sequence> wTester;
      wTester mywtester;
      status += mywtester.test_all();
#endif
    }
  catch (const ::CORBA::Exception &ex)
    {
      ex._tao_print_exception("ERROR : unexpected CORBA exception caugth :");
      ++status;
    }

  return status;
}
