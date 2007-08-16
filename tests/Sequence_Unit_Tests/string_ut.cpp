/**
 * @file
 *
 * @brief Unit test for CORBA string
 *
 * $Id$
 *
 * @author Johnny Willemsen
 */
#include "testing_string_traits.hpp"
#include "tao/CORBA_String.h"

#include "ace/OS_NS_string.h"

#include <sstream>
#include <stdexcept>
#include <iostream>

#include <boost/test/unit_test.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

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
  typedef string_traits<charT,true> tested_string_traits;
  typedef charT * string_type;
  typedef charT const * const_string_type;
  typedef typename tested_string_traits::string_var string_var;
  typedef typename tested_string_traits::string_mgr string_mgr;
  typedef typename tested_string_traits::string_out string_out;

  void test_copy_constructor()
  {
    expected_calls d(tested_string_traits::duplicate_calls);
    expected_calls r(tested_string_traits::release_calls);

    {
      string_var xe = helper<charT>::dup_sample0();
      string_var xb (xe);
    }

    BOOST_CHECK_MESSAGE(d.expect(1), d);
    BOOST_CHECK_MESSAGE(r.expect(1), r);
  }

  void add_all(test_suite * ts)
  {
    boost::shared_ptr<Tester> shared_this(self_);

    ts->add(BOOST_CLASS_TEST_CASE(
                &Tester::test_copy_constructor,
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
  test_suite * ts =
      BOOST_TEST_SUITE("string unit test");

  boost::shared_ptr<Tester<char> > char_tester(
      Tester<char>::allocate());
  char_tester->add_all(ts);

  boost::shared_ptr<Tester<CORBA::WChar> > wchar_tester(
      Tester<CORBA::WChar>::allocate());
  wchar_tester->add_all(ts);

  return ts;
}

