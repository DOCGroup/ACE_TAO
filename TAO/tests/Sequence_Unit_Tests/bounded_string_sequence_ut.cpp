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
      FAIL_RETURN_IF_NOT(a.expect(0), a);
      CHECK_EQUAL(CORBA::ULong(MAXIMUM), x.maximum());
      CHECK_EQUAL(CORBA::ULong(8), x.length());
      CHECK_EQUAL(true, x.release());

      FAIL_RETURN_IF_NOT(i.expect(8), i);
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
    buf[0] = helper::to_string(1);
    buf[1] = helper::to_string(4);
    buf[2] = helper::to_string(9);
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
      tested_sequence a(4, buffer, false);
      CHECK_EQUAL(CORBA::ULong(MAXIMUM), a.maximum());
      CHECK_EQUAL(CORBA::ULong(4), a.length());
      CHECK_EQUAL(buffer, a.get_buffer());
      CHECK_EQUAL(false, a.release());
      a.length (3);
      CHECK_EQUAL(CORBA::ULong(3), a.length());
      a.length (4);
      CHECK_EQUAL(CORBA::ULong(4), a.length());
      CHECK(helper::compare_empty(a[3]));
    }
    FAIL_RETURN_IF_NOT(a.expect(0), a);
    FAIL_RETURN_IF_NOT(f.expect(0), f);
    tested_sequence::freebuf(buffer);
    FAIL_RETURN_IF_NOT(r.expect(MAXIMUM), r);
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
      FAIL_RETURN_IF_NOT(f.expect(1), f);
      FAIL_RETURN_IF_NOT(r.expect(MAXIMUM), r);

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
      FAIL_RETURN_IF_NOT(f.expect(1), f);
      FAIL_RETURN_IF_NOT(r.expect(MAXIMUM), r);

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
      a.replace(4, buffer, false);
      FAIL_RETURN_IF_NOT(c.expect(0), c);
      FAIL_RETURN_IF_NOT(f.expect(1), f);
      FAIL_RETURN_IF_NOT(r.expect(MAXIMUM), r);

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

      FAIL_RETURN_IF_NOT(c.expect(0), c);

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
    common X (true);
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

  typedef TAO::bounded_basic_string_sequence<char, MAXIMUM> s_sequence;
  typedef Tester<s_sequence> nTester;
  nTester myntester;
  
  status += myntester.test_all();

  typedef TAO::bounded_basic_string_sequence<CORBA::WChar, MAXIMUM> w_sequence;
  typedef Tester<w_sequence> wTester;
  wTester mywtester;
  
  status += mywtester.test_all();

  return status;
}

