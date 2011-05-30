#ifndef guard_string_sequence_tester_hpp
#define guard_string_sequence_tester_hpp
/**
 * @file
 *
 * @brief Helper class to implement tests for *_string_sequence
 *
 * $Id$
 *
 * @author Carlos O'Ryan
 */
#include "tao/String_Traits_T.h"

#include "ace/OS_NS_string.h"

#include "test_macros.h"

#include <sstream>

template<typename charT>
struct string_sequence_test_helpers
{
};

template<>
struct string_sequence_test_helpers<char>
{
  inline static char const * test_string()
  {
    return "In a hole in the ground there lived a Hobbit";
  }

  inline static char * allocate_test_string()
  {
    return TAO::details::string_traits<char,true>::duplicate(
        test_string());
  }

  static bool compare_test_string(char const * value)
  {
    return ACE_OS::strcmp(test_string(), value) == 0;
  }

  inline static char * to_string(CORBA::ULong i)
  {
    std::ostringstream os;
    os << i;
    return TAO::details::string_traits<char,true>::duplicate(
        os.str().c_str());
  }

  inline static bool compare(int i, char const * value)
  {
    std::ostringstream os;
    os << i;
    return ACE_OS::strcmp(os.str().c_str(), value) == 0;
  }

  inline static bool compare_empty(char const * value)
  {
    return ACE_OS::strcmp(value, "") == 0;
  }
};

#if defined(_GLIBCPP_VERSION) && !defined(_GLIBCPP_USE_WCHAR_T)
# define TAO_LACKS_WCHAR_CXX_STDLIB
#endif

#if defined(ACE_HAS_WCHAR) && !defined(TAO_LACKS_WCHAR_CXX_STDLIB)
template<>
struct string_sequence_test_helpers<CORBA::WChar>
{
  inline static CORBA::WChar const * test_string()
  {
    return L"In a hole in the ground there lived a Hobbit";
  }

  inline static CORBA::WChar * allocate_test_string()
  {
    return TAO::details::string_traits<CORBA::WChar,true>::duplicate(
        test_string());
  }

  static bool compare_test_string(CORBA::WChar const * value)
  {
    return ACE_OS::strcmp(test_string(), value) == 0;
  }

  inline static CORBA::WChar * to_string(CORBA::ULong i)
  {
    std::wostringstream os;
    os << i;
    return TAO::details::string_traits<CORBA::WChar,true>::duplicate(
        os.str().c_str());
  }

  inline static bool compare(int i, CORBA::WChar const * value)
  {
    std::wostringstream os;
    os << i;
    return ACE_OS::strcmp(os.str().c_str(), value) == 0;
  }

  inline static bool compare_empty(CORBA::WChar const * value)
  {
    return ACE_OS::strcmp(value, L"") == 0;
  }
};
#endif /* ACE_HAS_WCHAR */

template<class tested_sequence>
struct string_sequence_tester
{
  typedef typename tested_sequence::character_type character_type;
  typedef string_sequence_test_helpers<character_type> helper;
  typedef typename tested_sequence::value_type value_type;
  typedef typename tested_sequence::const_value_type const_value_type;
  typedef typename tested_sequence::element_traits tested_element_traits;
  typedef typename tested_sequence::allocation_traits tested_allocation_traits;

  int test_default_constructor()
  {
    expected_calls a(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    {
      tested_sequence x;

      CHECK_EQUAL(
          CORBA::ULong(tested_allocation_traits::default_maximum()),
          x.maximum());
      CHECK_EQUAL(CORBA::ULong(0), x.length());
    }
    FAIL_RETURN_IF_NOT(a.expect(0), a);
    // Nothing was allocated then there is nothing to free.
    FAIL_RETURN_IF_NOT(f.expect(0), f);
    return 0;
  }

  int test_copy_constructor_from_default()
  {
    expected_calls a(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    expected_calls i(tested_element_traits::default_initializer_calls);
    expected_calls d(tested_element_traits::duplicate_calls);
    {
      tested_sequence x;

      a.reset(); f.reset(); i.reset(); d.reset();

      tested_sequence y(x);
      // Default constructed sequence doesn't have elements,
      // thus there is nothing to allocate/copy in copy constructor.
      FAIL_RETURN_IF_NOT(a.expect(0), a);
      FAIL_RETURN_IF_NOT(f.expect(0), f);
      FAIL_RETURN_IF_NOT(i.expect(0), i);
      FAIL_RETURN_IF_NOT(d.expect(0), d);

      CHECK_EQUAL(x.maximum(), y.maximum());
      CHECK_EQUAL(x.length(), y.length());
      CHECK_EQUAL(x.release(), y.release());
    }
    // Nothing was allocated then there is nothing to free.
    FAIL_RETURN_IF_NOT(f.expect(0), f);
    return 0;
  }

  int test_index_accessor()
  {
    tested_sequence x;
    x.length(8);

    tested_sequence const & y = x;
    character_type const * t = y[4];
    FAIL_RETURN_IF_NOT(helper::compare_empty(t),
        "Unexpected string value " << t);
    return 0;
  }

  int test_index_modifier()
  {
    tested_sequence x;
    x.length(8);

    tested_sequence const & y = x;

    character_type const * text = helper::test_string();
    x[4] = text;

    character_type const * t = y[4];

    FAIL_RETURN_IF_NOT(ACE_OS::strcmp(text, x[4]) == 0,
        "Mismatched values expected=" << text
        << ", got=" << x[4]);
    FAIL_RETURN_IF_NOT(ACE_OS::strcmp(text, y[4]) == 0,
        "Mismatched values expected=" << text
        << ", got=" << y[4]);
    CHECK(text != t);
    return 0;
  }

  int test_index_checking()
  {
    tested_sequence x;
    x.length(8);

    tested_sequence const & y = x;
    character_type const * lhs;
    character_type const * rhs = 0;
    CHECK_THROW(lhs = y[32], std::range_error);
    CHECK_THROW(x[32] = rhs, std::range_error);
    return 0;
  }

  int test_copy_constructor_values()
  {
    tested_sequence a;
    a.length(16);
    for(CORBA::ULong i = 0; i != 16; ++i)
    {
      a[i] = helper::to_string(i);
    }

    expected_calls d(tested_element_traits::duplicate_calls);
    expected_calls r(tested_element_traits::release_calls);

    CORBA::ULong max = 0;
    {
      tested_sequence b(a);
      FAIL_RETURN_IF_NOT(d.expect(16), d);
      max = b.maximum();

      CHECK_EQUAL(a.length(), b.length());
      for(CORBA::ULong i = 0; i != a.length(); ++i)
      {
        FAIL_RETURN_IF_NOT(ACE_OS::strcmp(a[i], b[i]) == 0,
            "Mismatched elements at index=" << i
            << ", a=" << a[i]
            << ", b=" << b[i]);
      }
    }
    FAIL_RETURN_IF_NOT(r.expect(max), r);
    return 0;
  }

  int test_freebuf_releases_elements()
  {
    value_type * buffer = tested_sequence::allocbuf(32);

    expected_calls r(tested_element_traits::release_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);

    tested_sequence::freebuf(buffer);

    FAIL_RETURN_IF_NOT(f.expect(1), f);
    FAIL_RETURN_IF_NOT(r.expect(32), r);
    return 0;
  }

  int test_assignment_from_default()
  {
    expected_calls a(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);

    {
      tested_sequence x;
      FAIL_RETURN_IF_NOT(a.expect(0), a);
      CHECK_EQUAL(
          CORBA::ULong(tested_allocation_traits::default_maximum()),
          x.maximum());
      CHECK_EQUAL(CORBA::ULong(0), x.length());

      tested_sequence y;
      FAIL_RETURN_IF_NOT(a.expect(0), a);

      y = x;
      // Default constructed sequence doesn't have elements,
      // thus there is nothing to allocate/copy in operator=.
      FAIL_RETURN_IF_NOT(a.expect(0), a);
      FAIL_RETURN_IF_NOT(f.expect(0), f);
      CHECK_EQUAL(x.maximum(), y.maximum());
      CHECK_EQUAL(x.length(), y.length());
      CHECK_EQUAL(x.release(), y.release());
    }
    // Nothing was allocated then there is nothing to free.
    FAIL_RETURN_IF_NOT(f.expect(0), f);
    return 0;
  }

  int test_assignment_values()
  {
    tested_sequence a;
    a.length(16);
    for(CORBA::ULong i = 0; i != 16; ++i)
    {
      a[i] = helper::to_string(i);
    }

    expected_calls d(tested_element_traits::duplicate_calls);
    expected_calls r(tested_element_traits::release_calls);
    CORBA::ULong max = 0;
    {
      tested_sequence b;
      b = a;
      FAIL_RETURN_IF_NOT(d.expect(16), d);

      max = b.maximum();

      CHECK_EQUAL(a.maximum(), b.maximum());
      CHECK_EQUAL(a.length(),  b.length());
      CHECK_EQUAL(a.release(), b.release());
      for(CORBA::ULong i = 0; i != a.length(); ++i)
      {
        FAIL_RETURN_IF_NOT(ACE_OS::strcmp(a[i], b[i]) == 0,
            "Mismatched elements at index " << i);
      }
      r.reset();
    }
    FAIL_RETURN_IF_NOT(r.expect(max), r);
    return 0;
  }

  int test_exception_in_copy_constructor()
  {
    expected_calls f(tested_allocation_traits::freebuf_calls);
    {
      tested_sequence x; x.length(8);
      f.reset();

      expected_calls a(tested_allocation_traits::allocbuf_calls);
      tested_allocation_traits::allocbuf_calls.failure_countdown(1);
      CHECK_THROW(tested_sequence y(x), testing_exception);
      FAIL_RETURN_IF_NOT(a.expect(1), a);
    }
    FAIL_RETURN_IF_NOT(f.expect(1), f);
    return 0;
  }

  int test_exception_in_assignment()
  {
    expected_calls f(tested_allocation_traits::freebuf_calls);
    {
      tested_sequence x; x.length(2);

      tested_sequence y; y.length(3);

      expected_calls a(tested_allocation_traits::allocbuf_calls);
      f.reset();
      tested_allocation_traits::allocbuf_calls.failure_countdown(1);
      CHECK_THROW(y = x, testing_exception);

      FAIL_RETURN_IF_NOT(a.expect(1), a);
      FAIL_RETURN_IF_NOT(f.expect(0), f);

      CHECK_EQUAL(CORBA::ULong(3), y.length());
    }
    FAIL_RETURN_IF_NOT(f.expect(2), f);
    return 0;
  }

  int test_duplicate_exception_in_copy_constructor()
  {
    expected_calls f(tested_allocation_traits::freebuf_calls);
    {
      tested_sequence x; x.length(8);
      f.reset();

      for(CORBA::ULong i = 0; i != 8; ++i)
      {
        x[i] = helper::allocate_test_string();
      }

      expected_calls a(tested_allocation_traits::allocbuf_calls);
      expected_calls d(tested_element_traits::duplicate_calls);
      expected_calls r(tested_element_traits::release_calls);

      tested_element_traits::duplicate_calls.failure_countdown(4);

      CHECK_THROW(tested_sequence y(x), testing_exception);
      FAIL_RETURN_IF_NOT(a.expect(1), a);
      FAIL_RETURN_IF_NOT(f.expect(1), f);
      FAIL_RETURN_IF_NOT(d.expect(4), d);
      FAIL_RETURN_IF_NOT(r.expect(x.maximum()), r);
    }
    FAIL_RETURN_IF_NOT(f.expect(1), f);
    return 0;
  }

  int test_duplicate_exception_in_assignment()
  {
    expected_calls f(tested_allocation_traits::freebuf_calls);
    {
      tested_sequence x; x.length(4);
      f.reset();

      for(CORBA::ULong i = 0; i != 4; ++i)
      {
        x[i] = helper::allocate_test_string();
      }

      expected_calls a(tested_allocation_traits::allocbuf_calls);
      expected_calls d(tested_element_traits::duplicate_calls);
      expected_calls r(tested_element_traits::release_calls);


      {
        tested_sequence y; y.length(8);
        for(CORBA::ULong i = 0; i != 8; ++i)
        {
          y[i] = helper::allocate_test_string();
        }

        a.reset();
        d.reset();
        r.reset();
        f.reset();
        tested_element_traits::duplicate_calls.failure_countdown(4);
        CHECK_THROW(x = y, testing_exception);
        FAIL_RETURN_IF_NOT(a.expect(1), a);
        FAIL_RETURN_IF_NOT(f.expect(1), f);
        FAIL_RETURN_IF_NOT(d.expect(4), d);
        FAIL_RETURN_IF_NOT(r.expect(y.maximum()), r);

        CHECK_EQUAL(CORBA::ULong(8), y.length());
        for(CORBA::ULong i = 0; i != 8; ++i)
        {
          FAIL_RETURN_IF_NOT(
              helper::compare_test_string(y[i]),
              "Mismatch in element " << i
              << ", got=" << y[i]);
        }
      }
      FAIL_RETURN_IF_NOT(f.expect(1), f);
    }
    FAIL_RETURN_IF_NOT(f.expect(1), f);
    return 0;
  }

  int test_get_buffer_const()
  {
    tested_sequence a; a.length(8);
    tested_sequence const & b = a;

    const_value_type const * buffer = b.get_buffer();
    a[0] = helper::test_string();

    CHECK_EQUAL(buffer, b.get_buffer());
    FAIL_RETURN_IF_NOT(ACE_OS::strcmp(a[0], buffer[0]) == 0,
        "Mismatched elements a[0]=" << a[0]
        << ", buffer[0]=" << buffer[0]);
    return 0;
  }

  int test_all ()
  {
  int status=0;
  status += this->test_default_constructor();
  status += this->test_copy_constructor_from_default();
  status += this->test_index_accessor();
  status += this->test_index_modifier();
  status += this->test_index_checking();
  status += this->test_copy_constructor_values();
  status += this->test_freebuf_releases_elements();
  status += this->test_assignment_from_default();
  status += this->test_assignment_values();
  status += this->test_exception_in_copy_constructor();
  status += this->test_exception_in_assignment();
  status += this->test_duplicate_exception_in_copy_constructor();
  status += this->test_duplicate_exception_in_assignment();
  status += this->test_get_buffer_const();

  return status;
  }
};

#endif // guard_string_sequence_tester_hpp
