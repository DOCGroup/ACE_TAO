/**
 * @file
 *
 * @brief Unit test for string_sequence_element, this is the type
 *        returned by operator[] from a string sequence.
 *
 * $Id$
 *
 * @author Carlos O'Ryan
 */
#include "testing_string_traits.hpp"
#include "tao/String_Sequence_Element_T.h"
#include "tao/String_Manager_T.h"
#include "tao/CORBA_String.h"

#include "ace/OS_NS_string.h"

#include <sstream>
#include <stdexcept>
#include <iostream>

#include <boost/test/unit_test.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

using namespace TAO_VERSIONED_NAMESPACE_NAME::TAO::details;

using namespace boost::unit_test_framework;

template<typename charT>
struct helper {};

template<>
struct helper<char>
{
  static char const * empty() {
    return "";
  }
  static char const * sample0() {
    return "Hello";
  }
  static char const * sample1() {
    return "World";
  }
  static char * dup_sample0() {
    return string_traits<char,true>::duplicate(sample0());
  }
  static char * dup_sample1() {
    return string_traits<char,true>::duplicate(sample1());
  }
  static bool equal(char const * lhs, char const * rhs) {
    return ACE_OS::strcmp(lhs, rhs) == 0;
  }
};

template<>
struct helper<CORBA::WChar>
{
  static CORBA::WChar const * empty() {
    return L"";
  }
  static CORBA::WChar const * sample0() {
    return L"Hello";
  }
  static CORBA::WChar const * sample1() {
    return L"World";
  }
  static CORBA::WChar * dup_sample0() {
    return string_traits<CORBA::WChar,true>::duplicate(sample0());
  }
  static CORBA::WChar * dup_sample1() {
    return string_traits<CORBA::WChar,true>::duplicate(sample1());
  }
  static bool equal(CORBA::WChar const * lhs, CORBA::WChar const * rhs) {
    return ACE_OS::strcmp(lhs, rhs) == 0;
  }
};

template<class charT>
struct Tester
{
  typedef string_traits<charT,true> tested_element_traits;
  typedef string_sequence_element<tested_element_traits> tested_element;
  typedef charT * string_type;
  typedef charT const * const_string_type;
  typedef typename tested_element_traits::string_var string_var;
  typedef typename tested_element_traits::string_mgr string_mgr;

  void test_assignment_from_const_string()
  {
    expected_calls d(tested_element_traits::duplicate_calls);
    expected_calls r(tested_element_traits::release_calls);

    {
      string_type xe = helper<charT>::dup_sample0();
      const_string_type y = helper<charT>::sample1();
      d.reset(); r.reset();

      tested_element x(xe, true);
      BOOST_CHECK_MESSAGE(d.expect(0), d);
      BOOST_CHECK_MESSAGE(r.expect(0), r);

      x = y;

      BOOST_CHECK_MESSAGE(d.expect(0), d);
      BOOST_CHECK_MESSAGE(r.expect(1), r);

      BOOST_CHECK_MESSAGE(
          helper<charT>::equal(helper<charT>::sample1(), xe),
          "Mismatch after assignment from const. expected="
          << helper<charT>::sample0()
          << ", got=" << x);
      tested_element_traits::release(xe);
      BOOST_CHECK_MESSAGE(r.expect(1), r);
    }
    BOOST_CHECK_MESSAGE(d.expect(0), d);
    BOOST_CHECK_MESSAGE(r.expect(0), r);
  }

  void test_assignment_from_element()
  {
    expected_calls d(tested_element_traits::duplicate_calls);
    expected_calls r(tested_element_traits::release_calls);
    {
      string_type xe = helper<charT>::dup_sample0();
      tested_element x(xe, true);

      string_type ye = helper<charT>::dup_sample1();
      tested_element y(ye, true);

      d.reset(); r.reset();

      x = y;

      BOOST_CHECK_MESSAGE(d.expect(0), d);
      BOOST_CHECK_MESSAGE(r.expect(1), r);

      BOOST_CHECK_MESSAGE(
          helper<charT>::equal(helper<charT>::sample1(), xe),
          "Mismatch after assignment from element. expected="
          << helper<charT>::sample1()
          << ", got=" << xe);

      tested_element_traits::release(xe);
      tested_element_traits::release(ye);
      BOOST_CHECK_MESSAGE(r.expect(2), r);
    }
    BOOST_CHECK_MESSAGE(d.expect(0), d);
    BOOST_CHECK_MESSAGE(r.expect(0), r);
  }

  void test_self_assignment()
  {
    expected_calls d(tested_element_traits::duplicate_calls);
    expected_calls r(tested_element_traits::release_calls);
    {
      string_type xe = helper<charT>::dup_sample0();

      tested_element x(xe, true);

      d.reset(); r.reset();

      x = x;

      BOOST_CHECK_MESSAGE(d.expect(0), d);
      BOOST_CHECK_MESSAGE(r.expect(1), r);

      BOOST_CHECK_MESSAGE(
          helper<charT>::equal(helper<charT>::sample0(), xe),
          "Mismatch after self assignment. expected="
          << helper<charT>::sample0()
          << ", got=" << xe);

      tested_element_traits::release(xe);
      BOOST_CHECK_MESSAGE(r.expect(1), r);
    }
    BOOST_CHECK_MESSAGE(d.expect(0), d);
    BOOST_CHECK_MESSAGE(r.expect(0), r);
  }

  void test_assignment_from_non_const_string()
  {
    expected_calls d(tested_element_traits::duplicate_calls);
    expected_calls r(tested_element_traits::release_calls);

    {
      string_type xe = 0;
      tested_element x(xe, true);

      string_type y =
        tested_element_traits::duplicate(helper<charT>::sample0());
      BOOST_CHECK_MESSAGE(d.expect(1), d);
      BOOST_CHECK_MESSAGE(r.expect(0), r);

      x = y;

      BOOST_CHECK_MESSAGE(d.expect(0), d);
      BOOST_CHECK_MESSAGE(r.expect(1), r);

      BOOST_CHECK_MESSAGE(
          helper<charT>::equal(helper<charT>::sample0(), xe),
          "Mismatch after assignment from non-const. expected="
          << helper<charT>::sample0()
          << ", got=" << x);
      tested_element_traits::release(xe);
      BOOST_CHECK_MESSAGE(r.expect(1), r);
    }
    BOOST_CHECK_MESSAGE(d.expect(0), d);
    BOOST_CHECK_MESSAGE(r.expect(0), r);
  }

  void test_copy_constructor()
  {
    expected_calls d(tested_element_traits::duplicate_calls);
    expected_calls r(tested_element_traits::release_calls);

    {
      string_type xe =
        tested_element_traits::duplicate(helper<charT>::sample0());
      tested_element x(xe, true);

      d.reset(); r.reset();

      tested_element y(x);

      BOOST_CHECK_MESSAGE(d.expect(0), d);
      BOOST_CHECK_MESSAGE(r.expect(0), r);

      BOOST_CHECK_MESSAGE(
          helper<charT>::equal(helper<charT>::sample0(), y),
          "Mismatch after copy constructor. expected="
          << helper<charT>::sample0()
          << ", got=" << y);

      tested_element_traits::release(xe);
      BOOST_CHECK_MESSAGE(r.expect(1), r);
    }
    BOOST_CHECK_MESSAGE(d.expect(0), d);
    BOOST_CHECK_MESSAGE(r.expect(0), r);
  }

  void test_assignment_from_copy()
  {
    expected_calls d(tested_element_traits::duplicate_calls);
    expected_calls r(tested_element_traits::release_calls);
    {
      string_type xe = helper<charT>::dup_sample0();
      tested_element x(xe, true);

      d.reset(); r.reset();

      tested_element y(x);

      x = y;

      BOOST_CHECK_MESSAGE(r.expect(1), r);

      BOOST_CHECK_MESSAGE(
          helper<charT>::equal(helper<charT>::sample0(), xe),
          "Mismatch after assignment. expected="
          << helper<charT>::sample0()
          << ", got=" << xe);

      BOOST_CHECK_MESSAGE(
          helper<charT>::equal(helper<charT>::sample0(), y),
          "Mismatch after assignment. expected="
          << helper<charT>::sample0()
          << ", got=" << y);

      tested_element_traits::release(xe);
      BOOST_CHECK_MESSAGE(r.expect(1), r);
    }
    BOOST_CHECK_MESSAGE(d.expect(0), d);
    BOOST_CHECK_MESSAGE(r.expect(0), r);
  }

  void test_assignment_from_var()
  {
    expected_calls d(tested_element_traits::duplicate_calls);
    expected_calls r(tested_element_traits::release_calls);

    {
      string_type xe = helper<charT>::dup_sample1();
      tested_element x(xe, true);
      BOOST_CHECK_MESSAGE(d.expect(1), d);

      string_var y(helper<charT>::sample0());

      BOOST_CHECK_MESSAGE(d.expect(0), d);
      BOOST_CHECK_MESSAGE(r.expect(0), r);

      x = y;

      BOOST_CHECK_MESSAGE(r.expect(1), r);

      BOOST_CHECK_MESSAGE(
          helper<charT>::equal(helper<charT>::sample0(), xe),
          "Mismatch after assignment from var. expected="
          << helper<charT>::sample0()
          << ", got=" << x);

      tested_element_traits::release(xe);
      BOOST_CHECK_MESSAGE(r.expect(1), r);
    }
    BOOST_CHECK_MESSAGE(d.expect(0), d);
    BOOST_CHECK_MESSAGE(r.expect(0), r);
  }

  void test_assignment_from_mgr()
  {
    expected_calls d(tested_element_traits::duplicate_calls);
    expected_calls r(tested_element_traits::release_calls);

    {
      string_type xe = helper<charT>::dup_sample1();
      tested_element x(xe, true);
      BOOST_CHECK_MESSAGE(d.expect(1), d);

      string_mgr y;
      y = helper<charT>::sample0();

      BOOST_CHECK_MESSAGE(d.expect(0), d);
      BOOST_CHECK_MESSAGE(r.expect(0), r);

      x = y;

      BOOST_CHECK_MESSAGE(r.expect(1), r);

      BOOST_CHECK_MESSAGE(
          helper<charT>::equal(helper<charT>::sample0(), xe),
          "Mismatch after assignment from mgr. expected="
          << helper<charT>::sample0()
          << ", got=" << x);

      tested_element_traits::release(xe);
      BOOST_CHECK_MESSAGE(r.expect(1), r);
    }
    BOOST_CHECK_MESSAGE(d.expect(0), d);
    BOOST_CHECK_MESSAGE(r.expect(0), r);
  }

  void add_all(test_suite * ts)
  {
    boost::shared_ptr<Tester> shared_this(self_);

    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_assignment_from_const_string,
                shared_this));
    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_assignment_from_element,
                shared_this));
    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_self_assignment,
                shared_this));
    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_assignment_from_non_const_string,
                shared_this));
    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_copy_constructor,
                shared_this));
    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_assignment_from_copy,
                shared_this));
    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_assignment_from_var,
                shared_this));
    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_assignment_from_mgr,
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
      BOOST_TEST_SUITE("string sequence element unit test");

  boost::shared_ptr<Tester<char> > char_tester(
      Tester<char>::allocate());
  char_tester->add_all(ts);

  boost::shared_ptr<Tester<CORBA::WChar> > wchar_tester(
      Tester<CORBA::WChar>::allocate());
  wchar_tester->add_all(ts);

  return ts;
}

