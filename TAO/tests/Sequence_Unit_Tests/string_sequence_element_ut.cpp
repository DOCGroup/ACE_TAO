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
#include "string_sequence_element.hpp"

#include "ace/OS_NS_string.h"

#include <sstream>
#include <stdexcept>
#include <iostream>

#include <boost/test/unit_test.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

using namespace TAO::details;

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
  static bool equal(CORBA::WChar const * lhs, CORBA::WChar const * rhs) {
    return ACE_OS::strcmp(lhs, rhs) == 0;
  }
};

template<class charT>
struct Tester : public boost::enable_shared_from_this<Tester<charT> >
{
  typedef string_traits<charT,true> tested_element_traits;
  typedef string_sequence_element<charT> tested_element;
  typedef charT * string_type;
  typedef typename tested_element_traits::string_var string_var;
  typedef typename tested_element_traits::string_mgr string_mgr;

  void test_assignment_from_const_string()
  {
    expected_calls d(tested_element_traits::duplicate_calls);
    expected_calls r(tested_element_traits::release_calls);

    {
      string_type xe = 0;
      tested_element x(xe, true);
      BOOST_CHECK_MESSAGE(d.expect(0), d);
      BOOST_CHECK_MESSAGE(r.expect(0), r);
      x = helper<charT>::sample0();
      
      BOOST_CHECK_MESSAGE(d.expect(1), d);
      BOOST_CHECK_MESSAGE(r.expect(1), r);

      BOOST_CHECK_MESSAGE(
          helper<charT>::equal(helper<charT>::sample0(), xe),
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
      string_type xe = 0;

      tested_element x(xe, true);
      x = helper<charT>::sample0();

      string_type ye = 0;

      tested_element y(xe, true);
      y = helper<charT>::sample1();

      d.reset(); r.reset();

      x = y;

      BOOST_CHECK_MESSAGE(d.expect(1), d);
      BOOST_CHECK_MESSAGE(r.expect(1), r);

      BOOST_CHECK_MESSAGE(
          helper<charT>::equal(helper<charT>::sample1(), x),
          "Mismatch after assignment from element. expected="
          << helper<charT>::sample1()
          << ", got=" << x);

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
      string_type xe = 0;

      tested_element x(xe, true);
      x = helper<charT>::sample0();

      d.reset(); r.reset();

      x = x;

      BOOST_CHECK_MESSAGE(d.expect(1), d);
      BOOST_CHECK_MESSAGE(r.expect(1), r);

      BOOST_CHECK_MESSAGE(
          helper<charT>::equal(helper<charT>::sample0(), x),
          "Mismatch after self assignment. expected="
          << helper<charT>::sample1()
          << ", got=" << x);

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
      string_type xe =
        tested_element_traits::duplicate(helper<charT>::sample0());
      tested_element x(xe, true);

      d.reset(); r.reset();

      tested_element y(x);

      BOOST_CHECK_MESSAGE(d.expect(0), d);
      BOOST_CHECK_MESSAGE(r.expect(0), r);

      x = y;

      BOOST_CHECK_MESSAGE(d.expect(1), d);
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
      string_type xe = 0;
      tested_element x(xe, true);

      string_var y(helper<charT>::sample0());

      BOOST_CHECK_MESSAGE(d.expect(0), d);
      BOOST_CHECK_MESSAGE(r.expect(0), r);

      x = y;
      
      BOOST_CHECK_MESSAGE(d.expect(1), d);
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
      string_type xe = 0;
      tested_element x(xe, true);

      string_mgr y;
      y = helper<charT>::sample0();

      BOOST_CHECK_MESSAGE(d.expect(0), d);
      BOOST_CHECK_MESSAGE(r.expect(0), r);

      x = y;
      
      BOOST_CHECK_MESSAGE(d.expect(1), d);
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
    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_assignment_from_const_string,
                shared_from_this()));
    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_assignment_from_element,
                shared_from_this()));
    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_self_assignment,
                shared_from_this()));
    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_assignment_from_non_const_string,
                shared_from_this()));
    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_copy_constructor,
                shared_from_this()));
    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_assignment_from_copy,
                shared_from_this()));
    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_assignment_from_var,
                shared_from_this()));
    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_assignment_from_mgr,
                shared_from_this()));
  }
}; 

test_suite *
init_unit_test_suite(int, char*[])
{
  std::auto_ptr<test_suite> ts(
      BOOST_TEST_SUITE("string sequence element unit test"));

  boost::shared_ptr<Tester<char> > char_tester(
      new Tester<char>);
  char_tester->add_all(ts.get());

  boost::shared_ptr<Tester<CORBA::WChar> > wchar_tester(
      new Tester<CORBA::WChar>);
  wchar_tester->add_all(ts.get());

  return ts.release();
}

#if 0
// This is just to convince MPC that I do not need a main() to have a
// program.
int main() {}
#endif
