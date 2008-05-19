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

#include <boost/test/unit_test.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

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



template<class tested_sequence>
struct string_sequence_tester
{
  typedef typename tested_sequence::character_type character_type;
  typedef string_sequence_test_helpers<character_type> helper;
  typedef typename tested_sequence::value_type value_type;
  typedef typename tested_sequence::const_value_type const_value_type;
  typedef typename tested_sequence::element_traits tested_element_traits;
  typedef typename tested_sequence::allocation_traits tested_allocation_traits;

  void test_default_constructor()
  {
    expected_calls a(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    {
      tested_sequence x;

      BOOST_CHECK_EQUAL(
          CORBA::ULong(tested_allocation_traits::default_maximum()),
          x.maximum());
      BOOST_CHECK_EQUAL(CORBA::ULong(0), x.length());
      BOOST_CHECK_EQUAL(bounded_, x.release());
    }
    BOOST_CHECK_MESSAGE(a.expect(0), a);
    BOOST_CHECK_MESSAGE(f.expect(bounded_ ? 1 : 0), f);
  }

  void test_copy_constructor_from_default()
  {
    expected_calls a(tested_allocation_traits::allocbuf_calls);
    expected_calls f(tested_allocation_traits::freebuf_calls);
    expected_calls i(tested_element_traits::default_initializer_calls);
    expected_calls d(tested_element_traits::duplicate_calls);
    {
      tested_sequence x;

      a.reset(); f.reset(); i.reset(); d.reset();

      tested_sequence y(x);
      BOOST_CHECK_MESSAGE(a.expect(bounded_ ? 1 : 0), a);
      BOOST_CHECK_MESSAGE(f.expect(0), f);
      BOOST_CHECK_MESSAGE(i.expect(0), i);
      BOOST_CHECK_MESSAGE(d.expect(0), d);

      BOOST_CHECK_EQUAL(x.maximum(), y.maximum());
      BOOST_CHECK_EQUAL(x.length(), y.length());
      BOOST_CHECK_EQUAL(x.release(), y.release());
    }
    BOOST_CHECK_MESSAGE(f.expect(bounded_ ? 2 : 0), f);
  }

  void test_index_accessor()
  {
    tested_sequence x;
    x.length(8);

    tested_sequence const & y = x;
    character_type const * t = y[4];
    BOOST_CHECK_MESSAGE(helper::compare_empty(t),
        "Unexpected string value " << t);
  }

  void test_index_modifier()
  {
    tested_sequence x;
    x.length(8);

    tested_sequence const & y = x;

    character_type const * text = helper::test_string();
    x[4] = text;

    character_type const * t = y[4];

    BOOST_CHECK_MESSAGE(ACE_OS::strcmp(text, x[4]) == 0,
        "Mismatched values expected=" << text
        << ", got=" << x[4]);
    BOOST_CHECK_MESSAGE(ACE_OS::strcmp(text, y[4]) == 0,
        "Mismatched values expected=" << text
        << ", got=" << y[4]);
    BOOST_CHECK(text != t);
  }

  void test_index_checking()
  {
    tested_sequence x;
    x.length(8);

    tested_sequence const & y = x;
    character_type const * lhs = 0;
    character_type const * rhs = 0;
    BOOST_CHECK_THROW(lhs = y[32], std::range_error);
    BOOST_CHECK_THROW(x[32] = rhs, std::range_error);
  }

  void test_copy_constructor_values()
  {
    tested_sequence a;
    a.length(16);
    for(CORBA::ULong i = 0; i != 16; ++i)
    {
      a[i] = helper::to_string(i);
    }

    expected_calls d(tested_element_traits::duplicate_calls);
    expected_calls r(tested_element_traits::release_calls);

    CORBA::ULong max;
    {
      tested_sequence b(a);
      BOOST_CHECK_MESSAGE(d.expect(16), d);
      max = b.maximum();

      BOOST_CHECK_EQUAL(a.length(), b.length());
      for(CORBA::ULong i = 0; i != a.length(); ++i)
      {
        BOOST_CHECK_MESSAGE(ACE_OS::strcmp(a[i], b[i]) == 0,
            "Mismatched elements at index=" << i
            << ", a=" << a[i]
            << ", b=" << b[i]);
      }
    }
    BOOST_CHECK_MESSAGE(r.expect(max), r);
  }

  void test_freebuf_releases_elements()
  {
    value_type * buffer = tested_sequence::allocbuf(32);
    for(int i = 0; i != 32; ++i)
    {
      buffer[i] = helper::allocate_test_string();
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
      BOOST_CHECK_EQUAL(bounded_ , x.release());

      tested_sequence y;
      BOOST_CHECK_MESSAGE(a.expect(0), a);

      y = x;
      BOOST_CHECK_MESSAGE(a.expect(bounded_ ? 1 : 0), a);
      BOOST_CHECK_MESSAGE(f.expect(bounded_ ? 1 : 0), f);
      BOOST_CHECK_EQUAL(x.maximum(), y.maximum());
      BOOST_CHECK_EQUAL(x.length(), y.length());
      BOOST_CHECK_EQUAL(x.release(), y.release());
    }
    BOOST_CHECK_MESSAGE(f.expect(bounded_ ? 2 : 0), f);
  }

  void test_assignment_values()
  {
    tested_sequence a;
    a.length(16);
    for(CORBA::ULong i = 0; i != 16; ++i)
    {
      a[i] = helper::to_string(i);
    }

    expected_calls d(tested_element_traits::duplicate_calls);
    expected_calls r(tested_element_traits::release_calls);
    CORBA::ULong max;
    {
      tested_sequence b;
      b = a;
      BOOST_CHECK_MESSAGE(d.expect(16), d);

      max = b.maximum();

      BOOST_CHECK_EQUAL(a.maximum(), b.maximum());
      BOOST_CHECK_EQUAL(a.length(),  b.length());
      BOOST_CHECK_EQUAL(a.release(), b.release());
      for(CORBA::ULong i = 0; i != a.length(); ++i)
      {
        BOOST_CHECK_MESSAGE(ACE_OS::strcmp(a[i], b[i]) == 0,
            "Mismatched elements at index " << i);
      }
      r.reset();
    }
    BOOST_CHECK_MESSAGE(r.expect(max), r);
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
        x[i] = helper::allocate_test_string();
      }

      expected_calls a(tested_allocation_traits::allocbuf_calls);
      expected_calls d(tested_element_traits::duplicate_calls);
      expected_calls r(tested_element_traits::release_calls);

      tested_element_traits::duplicate_calls.failure_countdown(4);

      BOOST_CHECK_THROW(tested_sequence y(x), testing_exception);
      BOOST_CHECK_MESSAGE(a.expect(1), a);
      BOOST_CHECK_MESSAGE(f.expect(1), f);
      BOOST_CHECK_MESSAGE(d.expect(4), d);
      BOOST_CHECK_MESSAGE(r.expect(x.maximum()), r);
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
        x[i] = helper::allocate_test_string();
      }

      expected_calls a(tested_allocation_traits::allocbuf_calls);
      expected_calls d(tested_element_traits::duplicate_calls);
      expected_calls r(tested_element_traits::release_calls);


      {
        tested_sequence y; y.length(4);
        for(CORBA::ULong i = 0; i != 4; ++i)
        {
          y[i] = helper::allocate_test_string();
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
        BOOST_CHECK_MESSAGE(r.expect(x.maximum()), r);

        BOOST_CHECK_EQUAL(CORBA::ULong(4), y.length());
        for(CORBA::ULong i = 0; i != 4; ++i)
        {
          BOOST_CHECK_MESSAGE(
              helper::compare_test_string(y[i]),
              "Mismatch in element " << i
              << ", got=" << y[i]);
        }
      }
      BOOST_CHECK_MESSAGE(f.expect(1), f);
    }
    BOOST_CHECK_MESSAGE(f.expect(1), f);
  }

  void test_get_buffer_const()
  {
    tested_sequence a; a.length(8);
    tested_sequence const & b = a;

    const_value_type const * buffer = b.get_buffer();
    a[0] = helper::test_string();

    BOOST_CHECK_EQUAL(buffer, b.get_buffer());
    BOOST_CHECK_MESSAGE(ACE_OS::strcmp(a[0], buffer[0]) == 0,
        "Mismatched elements a[0]=" << a[0]
        << ", buffer[0]=" << buffer[0]);
  }

  void add_all(boost::unit_test_framework::test_suite * ts)
  {
    boost::shared_ptr<string_sequence_tester> shared_this(self_);

    ts->add(BOOST_CLASS_TEST_CASE(
                &string_sequence_tester::test_default_constructor,
                shared_this));
    ts->add(BOOST_CLASS_TEST_CASE(
                &string_sequence_tester::test_copy_constructor_from_default,
                shared_this));
    ts->add(BOOST_CLASS_TEST_CASE(
                &string_sequence_tester::test_index_accessor,
                shared_this));
    ts->add(BOOST_CLASS_TEST_CASE(
                &string_sequence_tester::test_index_modifier,
                shared_this));
    ts->add(BOOST_CLASS_TEST_CASE(
                &string_sequence_tester::test_index_checking,
                shared_this));
    ts->add(BOOST_CLASS_TEST_CASE(
                &string_sequence_tester::test_copy_constructor_values,
                shared_this));
    ts->add(BOOST_CLASS_TEST_CASE(
                &string_sequence_tester::test_freebuf_releases_elements,
                shared_this));
    ts->add(BOOST_CLASS_TEST_CASE(
                &string_sequence_tester::test_assignment_from_default,
                shared_this));
    ts->add(BOOST_CLASS_TEST_CASE(
                &string_sequence_tester::test_assignment_values,
                shared_this));
    ts->add(BOOST_CLASS_TEST_CASE(
                &string_sequence_tester::test_exception_in_copy_constructor,
                shared_this));
    ts->add(BOOST_CLASS_TEST_CASE(
                &string_sequence_tester::test_exception_in_assignment,
                shared_this));
    ts->add(BOOST_CLASS_TEST_CASE(
                &string_sequence_tester::test_duplicate_exception_in_copy_constructor,
                shared_this));
    ts->add(BOOST_CLASS_TEST_CASE(
                &string_sequence_tester::test_duplicate_exception_in_assignment,
                shared_this));
    ts->add(BOOST_CLASS_TEST_CASE(
                &string_sequence_tester::test_get_buffer_const,
                shared_this));


  }

  static boost::shared_ptr<string_sequence_tester> allocate(bool bounded = false)
  {
    boost::shared_ptr<string_sequence_tester> ptr(
        new string_sequence_tester(bounded));
    ptr->self_ = ptr;

    return ptr;
  }

private:
  string_sequence_tester(bool bounded)
    : bounded_(bounded)
  {}

  bool bounded_;

  boost::weak_ptr<string_sequence_tester> self_;
};

#endif // guard_string_sequence_tester_hpp
