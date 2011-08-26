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
#include "tao/SystemException.h"

#include "ace/OS_NS_string.h"

#include "test_macros.h"

using namespace TAO_VERSIONED_NAMESPACE_NAME::TAO::details;



template<typename charT>
struct helper
{
};

template<>
struct helper <char>
{
  static char const *
  empty ()
  {
    return "";
  }
  static char const *
  sample0 ()
  {
    return "Hello";
  }
  static char const *
  sample1 ()
  {
    return "World";
  }
  static char *
  dup_sample0 ()
  {
    return string_traits <char, true>::duplicate (sample0 ());
  }
  static char *
  dup_sample1 ()
  {
    return string_traits <char, true>::duplicate (sample1 ());
  }
  static bool
  equal (char const *lhs, char const *rhs)
  {
    return ACE_OS::strcmp (lhs, rhs) == 0;
  }
};

#if defined(ACE_HAS_WCHAR)
template<>
struct helper <CORBA::WChar>
{
  static CORBA::WChar const *
  empty ()
  {
    return L"";
  }
  static CORBA::WChar const *
  sample0 ()
  {
    return L"Hello";
  }
  static CORBA::WChar const *
  sample1 ()
  {
    return L"World";
  }
  static CORBA::WChar *
  dup_sample0 ()
  {
    return string_traits <CORBA::WChar, true>::duplicate (sample0 ());
  }
  static CORBA::WChar *
  dup_sample1 ()
  {
    return string_traits <CORBA::WChar, true>::duplicate (sample1 ());
  }
  static bool
  equal (CORBA::WChar const *lhs, CORBA::WChar const *rhs)
  {
    return ACE_OS::strcmp (lhs, rhs) == 0;
  }
};
#endif

template<class charT>
struct Tester
{
  typedef string_traits <charT, true> tested_element_traits;
  typedef string_sequence_element <tested_element_traits> tested_element;
  typedef charT *string_type;
  typedef charT const *const_string_type;
  typedef typename tested_element_traits::string_var string_var;
  typedef typename tested_element_traits::string_mgr string_mgr;

  int
  test_assignment_from_const_string ()
  {
    expected_calls d (tested_element_traits::duplicate_calls);
    expected_calls r (tested_element_traits::release_calls);

    {
      string_type xe = helper <charT>::dup_sample0 ();
      const_string_type y = helper <charT>::sample1 ();
      d.reset ();
      r.reset ();

      tested_element x(xe, true);
      FAIL_RETURN_IF_NOT(d.expect(0), d);
      FAIL_RETURN_IF_NOT(r.expect(0), r);

      x = y;

      FAIL_RETURN_IF_NOT(d.expect(0), d);
      FAIL_RETURN_IF_NOT(r.expect(1), r);

      FAIL_RETURN_IF_NOT(
    helper<charT>::equal(helper<charT>::sample1(), xe),
    "Mismatch after assignment from const. expected="
    << helper<charT>::sample0()
    << ", got=" << x);
      tested_element_traits::release (xe);
      FAIL_RETURN_IF_NOT(r.expect(1), r);
    }
    FAIL_RETURN_IF_NOT(d.expect(0), d);
    FAIL_RETURN_IF_NOT(r.expect(0), r);
    return 0;
  }

  int
  test_assignment_from_element ()
  {
    expected_calls d (tested_element_traits::duplicate_calls);
    expected_calls r (tested_element_traits::release_calls);
    {
      string_type xe = helper <charT>::dup_sample0 ();
      tested_element x(xe, true);

      string_type ye = helper <charT>::dup_sample1 ();
      tested_element y(ye, true);

      d.reset ();
      r.reset ();

      x = y;

      FAIL_RETURN_IF_NOT(d.expect(0), d);
      FAIL_RETURN_IF_NOT(r.expect(1), r);

      FAIL_RETURN_IF_NOT(
    helper<charT>::equal(helper<charT>::sample1(), xe),
    "Mismatch after assignment from element. expected="
    << helper<charT>::sample1()
    << ", got=" << xe);

      tested_element_traits::release (xe);
      tested_element_traits::release (ye);
      FAIL_RETURN_IF_NOT(r.expect(2), r);
    }
    FAIL_RETURN_IF_NOT(d.expect(0), d);
    FAIL_RETURN_IF_NOT(r.expect(0), r);
    return 0;
  }

  int
  test_self_assignment ()
  {
    expected_calls d (tested_element_traits::duplicate_calls);
    expected_calls r (tested_element_traits::release_calls);
    {
      string_type xe = helper <charT>::dup_sample0 ();

      tested_element x(xe, true);

      d.reset ();
      r.reset ();

      x = x;

      FAIL_RETURN_IF_NOT(d.expect(0), d);
      FAIL_RETURN_IF_NOT(r.expect(1), r);

      FAIL_RETURN_IF_NOT(
    helper<charT>::equal(helper<charT>::sample0(), xe),
    "Mismatch after self assignment. expected="
    << helper<charT>::sample0()
    << ", got=" << xe);

      tested_element_traits::release (xe);
      FAIL_RETURN_IF_NOT(r.expect(1), r);
    }
    FAIL_RETURN_IF_NOT(d.expect(0), d);
    FAIL_RETURN_IF_NOT(r.expect(0), r);
    return 0;
  }

  int
  test_assignment_from_non_const_string ()
  {
    expected_calls d (tested_element_traits::duplicate_calls);
    expected_calls r (tested_element_traits::release_calls);

    {
      string_type xe = 0;
      tested_element x(xe, true);

      string_type y =
        tested_element_traits::duplicate (helper <charT>::sample0 ());
      FAIL_RETURN_IF_NOT(d.expect(1), d);
      FAIL_RETURN_IF_NOT(r.expect(0), r);

      x = y;

      FAIL_RETURN_IF_NOT(d.expect(0), d);
      FAIL_RETURN_IF_NOT(r.expect(1), r);

      FAIL_RETURN_IF_NOT(
    helper<charT>::equal(helper<charT>::sample0(), xe),
    "Mismatch after assignment from non-const. expected="
    << helper<charT>::sample0()
    << ", got=" << x);
      tested_element_traits::release (xe);
      FAIL_RETURN_IF_NOT(r.expect(1), r);
    }
    FAIL_RETURN_IF_NOT(d.expect(0), d);
    FAIL_RETURN_IF_NOT(r.expect(0), r);
    return 0;
  }

  int
  test_copy_constructor ()
  {
    expected_calls d (tested_element_traits::duplicate_calls);
    expected_calls r (tested_element_traits::release_calls);

    {
      string_type xe =
        tested_element_traits::duplicate (helper <charT>::sample0 ());
      tested_element x(xe, true);

      d.reset ();
      r.reset ();

      tested_element y (x);

      FAIL_RETURN_IF_NOT(d.expect(0), d);
      FAIL_RETURN_IF_NOT(r.expect(0), r);

      FAIL_RETURN_IF_NOT(
    helper<charT>::equal(helper<charT>::sample0(), y),
    "Mismatch after copy constructor. expected="
    << helper<charT>::sample0()
    << ", got=" << y);

      tested_element_traits::release (xe);
      FAIL_RETURN_IF_NOT(r.expect(1), r);
    }
    FAIL_RETURN_IF_NOT(d.expect(0), d);
    FAIL_RETURN_IF_NOT(r.expect(0), r);
    return 0;
  }

  int
  test_assignment_from_copy ()
  {
    expected_calls d (tested_element_traits::duplicate_calls);
    expected_calls r (tested_element_traits::release_calls);
    {
      string_type xe = helper <charT>::dup_sample0 ();
      tested_element x(xe, true);

      d.reset ();
      r.reset ();

      tested_element y (x);

      x = y;

      FAIL_RETURN_IF_NOT(r.expect(1), r);

      FAIL_RETURN_IF_NOT(
    helper<charT>::equal(helper<charT>::sample0(), xe),
    "Mismatch after assignment. expected="
    << helper<charT>::sample0()
    << ", got=" << xe);

      FAIL_RETURN_IF_NOT(
    helper<charT>::equal(helper<charT>::sample0(), y),
    "Mismatch after assignment. expected="
    << helper<charT>::sample0()
    << ", got=" << y);

      tested_element_traits::release (xe);
      FAIL_RETURN_IF_NOT(r.expect(1), r);
    }
    FAIL_RETURN_IF_NOT(d.expect(0), d);
    FAIL_RETURN_IF_NOT(r.expect(0), r);
    return 0;
  }

  int
  test_assignment_from_var ()
  {
    expected_calls d (tested_element_traits::duplicate_calls);
    expected_calls r (tested_element_traits::release_calls);

    {
      string_type xe = helper <charT>::dup_sample1 ();
      tested_element x(xe, true);
      FAIL_RETURN_IF_NOT(d.expect(1), d);

      string_var y (helper <charT>::sample0());

      FAIL_RETURN_IF_NOT(d.expect(0), d);
      FAIL_RETURN_IF_NOT(r.expect(0), r);

      x = y;

      FAIL_RETURN_IF_NOT(r.expect(1), r);

      FAIL_RETURN_IF_NOT(
    helper<charT>::equal(helper<charT>::sample0(), xe),
    "Mismatch after assignment from var. expected="
    << helper<charT>::sample0()
    << ", got=" << x);

      tested_element_traits::release (xe);
      FAIL_RETURN_IF_NOT(r.expect(1), r);
    }
    FAIL_RETURN_IF_NOT(d.expect(0), d);
    FAIL_RETURN_IF_NOT(r.expect(0), r);
    return 0;
  }

  int
  test_assignment_from_mgr ()
  {
    expected_calls d (tested_element_traits::duplicate_calls);
    expected_calls r (tested_element_traits::release_calls);

    {
      string_type xe = helper <charT>::dup_sample1 ();
      tested_element x(xe, true);
      FAIL_RETURN_IF_NOT(d.expect(1), d);

      string_mgr y;
      y = helper <charT>::sample0 ();

      FAIL_RETURN_IF_NOT(d.expect(0), d);
      FAIL_RETURN_IF_NOT(r.expect(0), r);

      x = y;

      FAIL_RETURN_IF_NOT(r.expect(1), r);

      FAIL_RETURN_IF_NOT(
    helper<charT>::equal(helper<charT>::sample0(), xe),
    "Mismatch after assignment from mgr. expected="
    << helper<charT>::sample0()
    << ", got=" << x);

      tested_element_traits::release (xe);
      FAIL_RETURN_IF_NOT(r.expect(1), r);
    }
    FAIL_RETURN_IF_NOT(d.expect(0), d);
    FAIL_RETURN_IF_NOT(r.expect(0), r);
    return 0;
  }
  int
  test_all ()
  {
    int status = 0;
    status += this->test_assignment_from_const_string ();
    status += this->test_assignment_from_element ();
    status += this->test_self_assignment ();
    status += this->test_assignment_from_non_const_string ();
    status += this->test_copy_constructor ();
    status += this->test_assignment_from_copy ();
    status += this->test_assignment_from_var ();
    status += this->test_assignment_from_mgr ();
    return status;
  }
};

int ACE_TMAIN (int, ACE_TCHAR*[])
{
  int status = 0;

  try
    {
      Tester <char> char_tester;
      status += char_tester.test_all ();
      Tester <char> wchar_tester;
      status += wchar_tester.test_all ();
    }
  catch (const ::CORBA::Exception& ex)
    {
      ex._tao_print_exception("ERROR : unexpected CORBA exception caugth :");
      ++status;
    }

  return status;
}

