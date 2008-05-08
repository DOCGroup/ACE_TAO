/**
 * @file Unbounded_String.cpp
 *
 * @brief test for STL iterator behaviour of CORBA unbounded string sequence
 *
 * $Id$
 *
 * @author Friedhelm Wolf (fwolf@dre.vanderbilt.edu)
 */

#include "tao/StringSeqC.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_string.h"

#include <iostream>
#include <iterator>
#include <sstream>

#define FAIL_RETURN_IF(CONDITION) \
          if (CONDITION) \
	    { \
              ACE_DEBUG ((LM_ERROR, ACE_TEXT ("\tFailed at %N:%l\n"))); \
	      return 1; \
	    }

//-----------------------------------------------------------------------------

template <typename ITERATOR_T>
int test_sequence ()
{
  ::CORBA::StringSeq a;

  // test equality operator
  FAIL_RETURN_IF (!(a.begin () == a.begin ()));

  // test non-equality operator
  FAIL_RETURN_IF (a.end () != a.end ());

  // test for correct behaviour for empty sequence

  FAIL_RETURN_IF (a.begin() != a.end ());
  
  // setup of an example sequence

  const char * elem0_cstr = "elem0";
  const char * elem1_cstr = "elem1";
  const char * elem2_cstr = "elem2";
  const char * elem3_cstr = "elem3";

  a.length (4);
  // Create a case to test for memory leaks
  // in the sequence.
  a[0] = CORBA::string_dup (elem0_cstr);
  a[0] = CORBA::string_dup (elem0_cstr);
  a[1] = CORBA::string_dup (elem1_cstr);
  a[2] = CORBA::string_dup (elem2_cstr);
  a[3] = CORBA::string_dup (elem3_cstr);

  // test iterator copy constructor
  ITERATOR_T a_it (a.begin ());
  FAIL_RETURN_IF (a_it != a.begin ());

  // test assignment operator
  a_it = a.begin ();

  // Create a case to test for memory leaks
  // in the iterator.
  *a_it = CORBA::string_dup (elem0_cstr);
  FAIL_RETURN_IF (a_it != a.begin ());

  // test non const dereferencing
  // I'm not sure this test makes sense since what's returned from
  // dereferencing the iterator is an r value.
  const char* value_seq = a[0];
  FAIL_RETURN_IF (ACE_OS::strcmp (value_seq, elem0_cstr) != 0);
  const char* value0 = *a_it;
  FAIL_RETURN_IF (ACE_OS::strcmp (value0, elem0_cstr) != 0);

  // test const dereferencing
  const char* const value1 = *a_it;
  FAIL_RETURN_IF (ACE_OS::strcmp (value1, elem0_cstr) != 0);

  // test increment operation
  a_it++;
  FAIL_RETURN_IF (a_it == a.begin());
  FAIL_RETURN_IF (ACE_OS::strcmp (*a_it, elem1_cstr) != 0);

  // test < operator
  FAIL_RETURN_IF (!(a.begin () < a_it));
  FAIL_RETURN_IF (a_it < a.begin ());

  // test difference type
  int a_diff = a_it - a.begin ();
  FAIL_RETURN_IF (a_diff != 1);

  // test copy constructor 
  ITERATOR_T a_it1 (a_it);
  FAIL_RETURN_IF (a_it1 != a_it);

  // test preincrement operator
  ++a_it1;
  FAIL_RETURN_IF ((a_it1 - a_it) != 1);

  // test = and += operator
  ITERATOR_T a_it2 = a_it += 3;
  FAIL_RETURN_IF (a_it2 != a_it);
  FAIL_RETURN_IF ((a_it - a_it1) != 2);

  // test + operator
  a_it2 = a_it1 + 3;
  FAIL_RETURN_IF ((a_it2 - a_it1) != 3);

  // test post-decrement operation
  a_it = a.end ();
  a_it--;
  FAIL_RETURN_IF (a_it == a.end ());
  FAIL_RETURN_IF ((a.end () - a_it) != 1);
  FAIL_RETURN_IF (ACE_OS::strcmp (*a_it, elem3_cstr) != 0);

  // test pre-decrement operator
  a_it = a.end ();
  --a_it;
  FAIL_RETURN_IF (a_it == a.end ());
  FAIL_RETURN_IF ((a.end () - a_it) != 1);
  FAIL_RETURN_IF (ACE_OS::strcmp (*a_it, elem3_cstr) != 0);

  // test -= operator
  a_it -= 3;
  FAIL_RETURN_IF ((a_it1 - a_it) != 2);

  // test - operator
  a_it2 = a_it1 - 2;
  FAIL_RETURN_IF ((a_it1 - a_it2) != 2);

  // test operator[] read
  a_it = a.begin ();
  FAIL_RETURN_IF (ACE_OS::strcmp (a_it[0],a[0]) != 0);
  a_it += 2;
  FAIL_RETURN_IF (ACE_OS::strcmp (a_it[0],a[2]) != 0);

  // test operator[] write
  // NOTE: This assignment actually modifies the third element
  // in the sequence since the iterator was previously positioned
  // at the third element. I believe these are the proper semantics
  // for using the bracket operator with an iterator. The number
  // in the brackets is additive - not absolute.
  a_it[0] = CORBA::string_dup (elem2_cstr);
  FAIL_RETURN_IF (ACE_OS::strcmp (a[2], elem2_cstr) != 0);

  // reset content of sequence a
  a[1] = CORBA::string_dup (elem1_cstr);

  // test for loop behaviour
  ::CORBA::StringSeq b = a;
  ITERATOR_T b_it = b.begin ();

  for (a_it = a.begin ();
       a_it != a.end ();
       a_it++, b_it++)
    {
      FAIL_RETURN_IF (ACE_OS::strcmp (*a_it, *b_it) != 0);
    }

  ::CORBA::StringSeq test;
  test.length (a.length ());

  std::copy (a.begin (),
             a.end (),
             test.begin ());

  FAIL_RETURN_IF (test.length () != a.length ());
  
  ITERATOR_T copytest_iter = test.begin ();
  for (ITERATOR_T copya_iter = a.begin ();
       copya_iter != a.end ();
       ++copya_iter, ++copytest_iter)
    {
      FAIL_RETURN_IF (ACE_OS::strcmp (*copya_iter, *copytest_iter) != 0);
    }

  /// Testing - using ostream_iterator
  std::ostringstream ostream;
  std::copy (a.begin (),
             a.end (),
             std::ostream_iterator<CORBA::StringSeq::const_value_type> (ostream,
                                                                        "\n"));

  // The third sequence element has been modified. Either we need to restore
  // the original sequence values or modify the test here. I modified the
  // test since it's easier.
  FAIL_RETURN_IF (
    ostream.str ().compare ("elem0\nelem1\nelem2\nelem3\n") != 0);

  return 0;
}

//-----------------------------------------------------------------------------

template <typename ITERATOR_T>
int test_sequence_const_iterator ()
{
  ::CORBA::StringSeq a;

  // test equality operator
  FAIL_RETURN_IF (!(a.begin () == a.begin ()));

  // test non-equality operator
  FAIL_RETURN_IF (a.end () != a.end ());

  // test for correct behaviour for empty sequence

  FAIL_RETURN_IF (a.begin() != a.end ());
  
  // setup of an example sequence

  const char * elem0_cstr = "elem0";
  const char * elem1_cstr = "elem1";
  const char * elem2_cstr = "elem2";
  const char * elem3_cstr = "elem3";

  a.length (4);
  a[0] = CORBA::string_dup (elem0_cstr);
  a[0] = CORBA::string_dup (elem0_cstr);
  a[1] = CORBA::string_dup (elem1_cstr);
  a[2] = CORBA::string_dup (elem2_cstr);
  a[3] = CORBA::string_dup (elem3_cstr);

  // test iterator copy constructor
  ITERATOR_T a_it (a.begin ());
  FAIL_RETURN_IF (a_it != a.begin ());

  // test assignment operator
  a_it = a.begin ();
  FAIL_RETURN_IF (a_it != a.begin ());

  // test non const dereferencing
  // I'm not sure non-const deferencing makes sense since
  // we are getting an r value.
  const char * value0 = *a_it;
  FAIL_RETURN_IF (ACE_OS::strcmp (value0, elem0_cstr) != 0);

  // test const dereferencing
  const char* const value1 = *a_it;
  FAIL_RETURN_IF (ACE_OS::strcmp (value1, elem0_cstr) != 0);

  // test increment operation
  a_it++;
  FAIL_RETURN_IF (a_it == a.begin());
  FAIL_RETURN_IF (ACE_OS::strcmp (*a_it, elem1_cstr) != 0);

  // test < operator
  FAIL_RETURN_IF (!(a.begin () < a_it));
  FAIL_RETURN_IF (a_it < a.begin ());

  // test difference type
  int a_diff = a_it - a.begin ();
  FAIL_RETURN_IF (a_diff != 1);

  // test copy constructor 
  ITERATOR_T a_it1 (a_it);
  FAIL_RETURN_IF (a_it1 != a_it);

  // test preincrement operator
  ++a_it1;
  FAIL_RETURN_IF ((a_it1 - a_it) != 1);

  // test = and += operator
  ITERATOR_T a_it2 = a_it += 3;
  FAIL_RETURN_IF (a_it2 != a_it);
  FAIL_RETURN_IF ((a_it - a_it1) != 2);

  // test + operator
  a_it2 = a_it1 + 3;
  FAIL_RETURN_IF ((a_it2 - a_it1) != 3);

  // test post-decrement operation
  a_it = a.end ();
  a_it--;
  FAIL_RETURN_IF (a_it == a.end ());
  FAIL_RETURN_IF ((a.end () - a_it) != 1);
  FAIL_RETURN_IF (ACE_OS::strcmp (*a_it, elem3_cstr) != 0);

  // test pre-decrement operator
  a_it = a.end ();
  --a_it;
  FAIL_RETURN_IF (a_it == a.end ());
  FAIL_RETURN_IF ((a.end () - a_it) != 1);
  FAIL_RETURN_IF (ACE_OS::strcmp (*a_it, elem3_cstr) != 0);

  // test -= operator
  a_it -= 3;
  FAIL_RETURN_IF ((a_it1 - a_it) != 2);

  // test - operator
  a_it2 = a_it1 - 2;
  FAIL_RETURN_IF ((a_it1 - a_it2) != 2);

  // test operator[] read
  a_it = a.begin ();
  FAIL_RETURN_IF (ACE_OS::strcmp (a_it[0],a[0]) != 0);
  a_it += 2;
  FAIL_RETURN_IF (ACE_OS::strcmp (a_it[0],a[2]) != 0);

  // test for loop behaviour
  ::CORBA::StringSeq b = a;
  ITERATOR_T b_it = b.begin ();

  for (a_it = a.begin ();
       a_it != a.end ();
       a_it++, b_it++)
    {
      FAIL_RETURN_IF (ACE_OS::strcmp (*a_it, *b_it) != 0);
    }

  ::CORBA::StringSeq test;
  test.length (a.length ());

  // Memory is leaked here from
  // TAO::details::string_traits_base<char>::default_initializer()

  std::copy (a.begin (),
             a.end (),
             test.begin ());

  FAIL_RETURN_IF (test.length () != a.length ());
  
  ITERATOR_T copytest_iter = test.begin ();
  for (ITERATOR_T copya_iter = a.begin ();
       copya_iter != a.end ();
       ++copya_iter, ++copytest_iter)
    {
      FAIL_RETURN_IF (ACE_OS::strcmp (*copya_iter, *copytest_iter) != 0);
    }

  /// Testing - using ostream_iterator

  std::ostringstream ostream;
  std::copy (a.begin (),
             a.end (),
             std::ostream_iterator<CORBA::StringSeq::const_value_type>(ostream,
								       "\n"));

  FAIL_RETURN_IF (
    ostream.str ().compare ("elem0\nelem1\nelem2\nelem3\n") != 0);

  return 0;
}

//-----------------------------------------------------------------------------

template <typename ITERATOR_T>
int test_const_sequence ()
{
  // setup of an example sequence
  const char * elem0_cstr = "elem0";
  const char * elem1_cstr = "elem1";
  const char * elem2_cstr = "elem2";
  const char * elem3_cstr = "elem3";

  ::CORBA::StringSeq setup;
  setup.length (4);
  setup[0] = CORBA::string_dup (elem0_cstr);
  setup[1] = CORBA::string_dup (elem1_cstr);
  setup[2] = CORBA::string_dup (elem2_cstr);
  setup[3] = CORBA::string_dup (elem3_cstr);

  const ::CORBA::StringSeq a = setup;

  // test equality operator
  // JWH2 - Below are (failed) attempts to make the compiler happy.
  // The compiler wants to construct a UBS_Sequence_Iterator and
  // complains about the constructor being passed a const sequence
  // pointer when it expects a non-const pointer. Why is the compiler
  // trying to construct a UBS_Sequence_Iterator rather than a
  // Const_UBS_Sequence_Iterator??
  //
  FAIL_RETURN_IF (!(a.begin () == a.begin ()));
  //ITERATOR_T c_iter1 (a.begin ());
  //::CORBA::StringSeq::const_iterator c_iter1 (a.begin ());
  //::TAO::Const_UBS_Sequence_Iterator<TAO::unbounded_basic_string_sequence<char> > c_iter1 = a.begin ();
  //ITERATOR_T c_iter2 = a.begin ();
  //FAIL_RETURN_IF (!(c_iter1 == c_iter2));

  // test non-equality operator
  FAIL_RETURN_IF (a.end () != a.end ());

  // test iterator copy constructor
  ITERATOR_T a_it (a.begin ());
  FAIL_RETURN_IF (a_it != a.begin ());

  // test non const dereferencing
  // JWH2 - I guess this test shouldn't be done since we are dealing with
  // a const iterator. Is that right?
  //typename ITERATOR_T::element_type value0 = *a_it;
  //FAIL_RETURN_IF (ACE_OS::strcmp (value0, elem0_cstr) != 0);

  // test const dereferencing
  typename ITERATOR_T::const_element_type value1 = *a_it;
  FAIL_RETURN_IF (ACE_OS::strcmp (value1, elem0_cstr) != 0);

  // test increment operation
  a_it++;
  FAIL_RETURN_IF (a_it == a.begin());
  FAIL_RETURN_IF (ACE_OS::strcmp (*a_it, elem1_cstr) != 0);

  // test < operator
  FAIL_RETURN_IF (!(a.begin () < a_it));
  FAIL_RETURN_IF (a_it < a.begin ());

  // test difference type
  int a_diff = a_it - a.begin ();
  FAIL_RETURN_IF (a_diff != 1);

  // test copy constructor 
  ITERATOR_T a_it1 (a_it);
  FAIL_RETURN_IF (a_it1 != a_it);

  // test preincrement operator
  ++a_it1;
  FAIL_RETURN_IF ((a_it1 - a_it) != 1);

  // test = and += operator
  ITERATOR_T a_it2 = a_it += 3;
  FAIL_RETURN_IF (a_it2 != a_it);
  FAIL_RETURN_IF ((a_it - a_it1) != 2);

  // test + operator
  a_it2 = a_it1 + 3;
  FAIL_RETURN_IF ((a_it2 - a_it1) != 3);

  // test post-decrement operation
  a_it = a.end ();
  a_it--;
  FAIL_RETURN_IF (a_it == a.end ());
  FAIL_RETURN_IF ((a.end () - a_it) != 1);
  FAIL_RETURN_IF (ACE_OS::strcmp (*a_it, elem3_cstr) != 0);

  // test pre-decrement operator
  a_it = a.end ();
  --a_it;
  FAIL_RETURN_IF (a_it == a.end ());
  FAIL_RETURN_IF ((a.end () - a_it) != 1);
  FAIL_RETURN_IF (ACE_OS::strcmp (*a_it, elem3_cstr) != 0);

  // test -= operator
  a_it -= 3;
  FAIL_RETURN_IF ((a_it1 - a_it) != 2);

  // test - operator
  a_it2 = a_it1 - 2;
  FAIL_RETURN_IF ((a_it1 - a_it2) != 2);

  // test operator[] read
  a_it = a.begin ();
  FAIL_RETURN_IF (ACE_OS::strcmp (a_it[0],a[0]) != 0);
  a_it += 2;
  FAIL_RETURN_IF (ACE_OS::strcmp (a_it[0],a[2]) != 0);

  // test for loop behaviour
  const ::CORBA::StringSeq b = a;
  ITERATOR_T b_it = b.begin ();

  for (a_it = a.begin ();
       a_it != a.end ();
       a_it++, b_it++)
    {
      FAIL_RETURN_IF (ACE_OS::strcmp (*a_it, *b_it) != 0);
    }

  ::CORBA::StringSeq test;
  test.length (a.length ());

  // Memory is leaked here from
  // TAO::details::string_traits_base<char>::default_initializer()

  std::copy (a.begin (),
             a.end (),
             test.begin ());

  FAIL_RETURN_IF (test.length () != a.length ());
  
  ITERATOR_T copytest_iter = test.begin ();
  for (ITERATOR_T copya_iter = a.begin ();
       copya_iter != a.end ();
       ++copya_iter, ++copytest_iter)
    {
      FAIL_RETURN_IF (ACE_OS::strcmp (*copya_iter, *copytest_iter) != 0);
    }

  /// Testing - using ostream_iterator

  std::ostringstream ostream;
  std::copy (a.begin (),
             a.end (),
             std::ostream_iterator<CORBA::StringSeq::const_value_type> (ostream,
                                                                        "\n"));

  FAIL_RETURN_IF (
    ostream.str ().compare ("elem0\nelem1\nelem2\nelem3\n") != 0);

  return 0;
}

//-----------------------------------------------------------------------------

template <typename REVERSE_ITERATOR_T>
int test_sequence_reverse ()
{
 ::CORBA::StringSeq a;

  // test equality operator
  FAIL_RETURN_IF (!(a.begin () == a.begin ()));

  // test non-equality operator
  FAIL_RETURN_IF (a.end () != a.end ());

  // test for correct behaviour for empty sequence

  FAIL_RETURN_IF (a.begin() != a.end ());
  
  // setup of an example sequence

  const char * elem0_cstr = "elem0";
  const char * elem1_cstr = "elem1";
  const char * elem2_cstr = "elem2";
  const char * elem3_cstr = "elem3";

  a.length (4);
  a[0] = CORBA::string_dup (elem0_cstr);
  a[1] = CORBA::string_dup (elem1_cstr);
  a[2] = CORBA::string_dup (elem2_cstr);
  a[3] = CORBA::string_dup (elem3_cstr);

  // test iterator copy constructor
  REVERSE_ITERATOR_T a_it (a.rbegin ());
  FAIL_RETURN_IF (a_it != a.rbegin ());

  // test assignment operator
  a_it = a.rbegin ();

  // Create a case to test for memory leaks
  // in the iterator.
  *a_it = CORBA::string_dup (elem3_cstr);
  FAIL_RETURN_IF (a_it != a.rbegin ());

  // test non const dereferencing
  typename REVERSE_ITERATOR_T::element_type value0 = *a_it;
  FAIL_RETURN_IF (ACE_OS::strcmp (value0, elem3_cstr) != 0);

  // test const dereferencing
  const char* const value1 = *a_it;
  FAIL_RETURN_IF (ACE_OS::strcmp (value1, elem3_cstr) != 0);

  // test increment operation
  a_it++;
  FAIL_RETURN_IF (a_it == a.rbegin());
  FAIL_RETURN_IF (ACE_OS::strcmp (*a_it, elem2_cstr) != 0);

  // test < operator
  FAIL_RETURN_IF (!(a.rbegin () < a_it));
  FAIL_RETURN_IF (a_it < a.rbegin ());

  // test difference type
  int a_diff = a_it - a.rbegin ();
  FAIL_RETURN_IF (a_diff != 1);

  // test copy constructor 
  REVERSE_ITERATOR_T a_it1 (a_it);
  FAIL_RETURN_IF (a_it1 != a_it);

  // test preincrement operator
  ++a_it1;
  FAIL_RETURN_IF ((a_it1 - a_it) != 1);

  // test = and += operator
  REVERSE_ITERATOR_T a_it2 = a_it += 3;
  FAIL_RETURN_IF (a_it2 != a_it);
  FAIL_RETURN_IF ((a_it - a_it1) != 2);

  // test + operator
  a_it2 = a_it1 + 3;
  FAIL_RETURN_IF ((a_it2 - a_it1) != 3);

  // test post-decrement operation
  a_it = a.rend ();
  a_it--;
  FAIL_RETURN_IF (a_it == a.rend ());
  FAIL_RETURN_IF ((a.rend () - a_it) != 1);
  FAIL_RETURN_IF (ACE_OS::strcmp (*a_it, elem0_cstr) != 0);

  // test pre-decrement operator
  a_it = a.rend ();
  --a_it;
  FAIL_RETURN_IF (a_it == a.rend ());
  FAIL_RETURN_IF ((a.rend () - a_it) != 1);
  FAIL_RETURN_IF (ACE_OS::strcmp (*a_it, elem0_cstr) != 0);

  // test -= operator
  a_it -= 3;
  FAIL_RETURN_IF ((a_it1 - a_it) != 2);

  // test - operator
  a_it2 = a_it1 - 2;
  FAIL_RETURN_IF ((a_it1 - a_it2) != 2);

  // test operator[] read
  a_it = a.rbegin ();
  FAIL_RETURN_IF (ACE_OS::strcmp (a_it[0],a[3]) != 0);
  a_it += 2;
  FAIL_RETURN_IF (ACE_OS::strcmp (a_it[0],a[1]) != 0);

  // test operator[] write
  a_it[0] = CORBA::string_dup (elem0_cstr);
  FAIL_RETURN_IF (ACE_OS::strcmp (a[1],elem0_cstr) != 0);

  // reset content of sequence a
  a[1] = CORBA::string_dup (elem1_cstr);

  // test for loop behaviour
  ::CORBA::StringSeq b = a;
  REVERSE_ITERATOR_T b_it = b.rbegin ();

  for (a_it = a.rbegin ();
       a_it != a.rend ();
       a_it++, b_it++)
    {
      FAIL_RETURN_IF (ACE_OS::strcmp (*a_it, *b_it) != 0);
    }

  ::CORBA::StringSeq test;
  test.length (a.length ());

  // Memory is leaked here from
  // TAO::details::string_traits_base<char>::default_initializer()

  std::copy (a.begin (),
             a.end (),
             test.begin ());

  FAIL_RETURN_IF (test.length () != a.length ());

  REVERSE_ITERATOR_T copytest_iter = test.rbegin ();
  for (REVERSE_ITERATOR_T copya_iter = a.rbegin ();
       copya_iter != a.rend ();
       ++copya_iter, ++copytest_iter)
    {
      FAIL_RETURN_IF (ACE_OS::strcmp (*copya_iter, *copytest_iter) != 0);
    }

  /// Testing - using ostream_iterator

  std::ostringstream ostream;
  std::copy (a.rbegin (),
             a.rend (),
             std::ostream_iterator<CORBA::StringSeq::element_type> (ostream,
                                                                    "\n"));

  FAIL_RETURN_IF (
    ostream.str ().compare ("elem3\nelem2\nelem1\nelem0\n") != 0);

  return 0;
}

//-----------------------------------------------------------------------------

template <typename REVERSE_ITERATOR_T>
int test_sequence_reverse_const_iterator ()
{
 ::CORBA::StringSeq a;

  // test equality operator
  FAIL_RETURN_IF (!(a.begin () == a.begin ()));

  // test non-equality operator
  FAIL_RETURN_IF (a.end () != a.end ());

  // test for correct behaviour for empty sequence

  FAIL_RETURN_IF (a.begin() != a.end ());
  
  // setup of an example sequence

  const char * elem0_cstr = "elem0";
  const char * elem1_cstr = "elem1";
  const char * elem2_cstr = "elem2";
  const char * elem3_cstr = "elem3";

  a.length (4);
  a[0] = CORBA::string_dup (elem0_cstr);
  a[1] = CORBA::string_dup (elem1_cstr);
  a[2] = CORBA::string_dup (elem2_cstr);
  a[3] = CORBA::string_dup (elem3_cstr);

  // test iterator copy constructor
  REVERSE_ITERATOR_T a_it (a.rbegin ());
  FAIL_RETURN_IF (a_it != a.rbegin ());

  // test assignment operator
  a_it = a.rbegin ();
  FAIL_RETURN_IF (a_it != a.rbegin ());

  // test non const dereferencing
  typename REVERSE_ITERATOR_T::const_element_type value0 = *a_it;
  FAIL_RETURN_IF (ACE_OS::strcmp (value0, elem3_cstr) != 0);

  // test const dereferencing
  const char* const value1 = *a_it;
  FAIL_RETURN_IF (ACE_OS::strcmp (value1, elem3_cstr) != 0);

  // test increment operation
  a_it++;
  FAIL_RETURN_IF (a_it == a.rbegin());
  FAIL_RETURN_IF (ACE_OS::strcmp (*a_it, elem2_cstr) != 0);

  // test < operator
  FAIL_RETURN_IF (!(a.rbegin () < a_it));
  FAIL_RETURN_IF (a_it < a.rbegin ());

  // test difference type
  int a_diff = a_it - a.rbegin ();
  FAIL_RETURN_IF (a_diff != 1);

  // test copy constructor 
  REVERSE_ITERATOR_T a_it1 (a_it);
  FAIL_RETURN_IF (a_it1 != a_it);

  // test preincrement operator
  ++a_it1;
  FAIL_RETURN_IF ((a_it1 - a_it) != 1);

  // test = and += operator
  REVERSE_ITERATOR_T a_it2 = a_it += 3;
  FAIL_RETURN_IF (a_it2 != a_it);
  FAIL_RETURN_IF ((a_it - a_it1) != 2);

  // test + operator
  a_it2 = a_it1 + 3;
  FAIL_RETURN_IF ((a_it2 - a_it1) != 3);

  // test post-decrement operation
  a_it = a.rend ();
  a_it--;
  FAIL_RETURN_IF (a_it == a.rend ());
  FAIL_RETURN_IF ((a.rend () - a_it) != 1);
  FAIL_RETURN_IF (ACE_OS::strcmp (*a_it, elem0_cstr) != 0);

  // test pre-decrement operator
  a_it = a.rend ();
  --a_it;
  FAIL_RETURN_IF (a_it == a.rend ());
  FAIL_RETURN_IF ((a.rend () - a_it) != 1);
  FAIL_RETURN_IF (ACE_OS::strcmp (*a_it, elem0_cstr) != 0);

  // test -= operator
  a_it -= 3;
  FAIL_RETURN_IF ((a_it1 - a_it) != 2);

  // test - operator
  a_it2 = a_it1 - 2;
  FAIL_RETURN_IF ((a_it1 - a_it2) != 2);

  // test operator[] read
  a_it = a.rbegin ();
  FAIL_RETURN_IF (ACE_OS::strcmp (a_it[0],a[3]) != 0);
  a_it += 2;
  FAIL_RETURN_IF (ACE_OS::strcmp (a_it[0],a[1]) != 0);

  // test for loop behaviour
  ::CORBA::StringSeq b = a;
  REVERSE_ITERATOR_T b_it = b.rbegin ();

  for (a_it = a.rbegin ();
       a_it != a.rend ();
       a_it++, b_it++)
    {
      FAIL_RETURN_IF (ACE_OS::strcmp (*a_it, *b_it) != 0);
    }

  ::CORBA::StringSeq test;
  test.length (a.length ());

  // Memory is leaked here from
  // TAO::details::string_traits_base<char>::default_initializer()

  std::copy (a.begin (),
             a.end (),
             test.begin ());

  FAIL_RETURN_IF (test.length () != a.length ());

  REVERSE_ITERATOR_T copytest_iter = test.rbegin ();
  for (REVERSE_ITERATOR_T copya_iter = a.rbegin ();
       copya_iter != a.rend ();
       ++copya_iter, ++copytest_iter)
    {
      FAIL_RETURN_IF (ACE_OS::strcmp (*copya_iter, *copytest_iter) != 0);
    }

  /// Testing - using ostream_iterator

  std::ostringstream ostream;
  std::copy (a.rbegin (),
             a.rend (),
             std::ostream_iterator<CORBA::StringSeq::element_type> (ostream,
                                                                    "\n"));

  FAIL_RETURN_IF (
    ostream.str ().compare ("elem3\nelem2\nelem1\nelem0\n") != 0);

  return 0;
}

//-----------------------------------------------------------------------------

template <typename REVERSE_ITERATOR_T>
int test_const_sequence_reverse ()
{
  // setup of an example sequence
  const char * elem0_cstr = "elem0";
  const char * elem1_cstr = "elem1";
  const char * elem2_cstr = "elem2";
  const char * elem3_cstr = "elem3";

  ::CORBA::StringSeq setup;;
  setup.length (4);
  setup[0] = CORBA::string_dup (elem0_cstr);
  setup[1] = CORBA::string_dup (elem1_cstr);
  setup[2] = CORBA::string_dup (elem2_cstr);
  setup[3] = CORBA::string_dup (elem3_cstr);

  const ::CORBA::StringSeq a = setup;

  // test equality operator
  FAIL_RETURN_IF (!(a.begin () == a.begin ()));

  // test non-equality operator
  FAIL_RETURN_IF (a.end () != a.end ());

  // test iterator copy constructor
  REVERSE_ITERATOR_T a_it (a.rbegin ());
  FAIL_RETURN_IF (a_it != a.rbegin ());

  // test non const dereferencing
  typename REVERSE_ITERATOR_T::const_element_type value0 = *a_it;
  FAIL_RETURN_IF (ACE_OS::strcmp (value0, elem3_cstr) != 0);

  // test const dereferencing
  const char* const value1 = *a_it;
  FAIL_RETURN_IF (ACE_OS::strcmp (value1, elem3_cstr) != 0);

  // test increment operation
  a_it++;
  FAIL_RETURN_IF (a_it == a.rbegin());
  FAIL_RETURN_IF (ACE_OS::strcmp (*a_it, elem2_cstr) != 0);

  // test < operator
  FAIL_RETURN_IF (!(a.rbegin () < a_it));
  FAIL_RETURN_IF (a_it < a.rbegin ());

  // test difference type
  int a_diff = a_it - a.rbegin ();
  FAIL_RETURN_IF (a_diff != 1);

  // test copy constructor 
  REVERSE_ITERATOR_T a_it1 (a_it);
  FAIL_RETURN_IF (a_it1 != a_it);

  // test preincrement operator
  ++a_it1;
  FAIL_RETURN_IF ((a_it1 - a_it) != 1);

  // test = and += operator
  REVERSE_ITERATOR_T a_it2 = a_it += 3;
  FAIL_RETURN_IF (a_it2 != a_it);
  FAIL_RETURN_IF ((a_it - a_it1) != 2);

  // test + operator
  a_it2 = a_it1 + 3;
  FAIL_RETURN_IF ((a_it2 - a_it1) != 3);

  // test post-decrement operation
  a_it = a.rend ();
  a_it--;
  FAIL_RETURN_IF (a_it == a.rend ());
  FAIL_RETURN_IF ((a.rend () - a_it) != 1);
  FAIL_RETURN_IF (ACE_OS::strcmp (*a_it, elem0_cstr) != 0);

  // test pre-decrement operator
  a_it = a.rend ();
  --a_it;
  FAIL_RETURN_IF (a_it == a.rend ());
  FAIL_RETURN_IF ((a.rend () - a_it) != 1);
  FAIL_RETURN_IF (ACE_OS::strcmp (*a_it, elem0_cstr) != 0);

  // test -= operator
  a_it -= 3;
  FAIL_RETURN_IF ((a_it1 - a_it) != 2);

  // test - operator
  a_it2 = a_it1 - 2;
  FAIL_RETURN_IF ((a_it1 - a_it2) != 2);

  // test operator[] read
  a_it = a.rbegin ();
  FAIL_RETURN_IF (ACE_OS::strcmp (a_it[0],a[3]) != 0);
  a_it += 2;
  FAIL_RETURN_IF (ACE_OS::strcmp (a_it[0],a[1]) != 0);

  // test for loop behaviour
  ::CORBA::StringSeq b = a;
  REVERSE_ITERATOR_T b_it = b.rbegin ();

  for (a_it = a.rbegin ();
       a_it != a.rend ();
       a_it++, b_it++)
    {
      FAIL_RETURN_IF (ACE_OS::strcmp (*a_it, *b_it) != 0);
    }

  ::CORBA::StringSeq test;
  test.length (a.length ());

  // Memory is leaked here from
  // TAO::details::string_traits_base<char>::default_initializer()

  std::copy (a.begin (),
             a.end (),
             test.begin ());

  FAIL_RETURN_IF (test.length () != a.length ());

  REVERSE_ITERATOR_T copytest_iter = test.rbegin ();
  for (REVERSE_ITERATOR_T copya_iter = a.rbegin ();
       copya_iter != a.rend ();
       ++copya_iter, ++copytest_iter)
    {
      FAIL_RETURN_IF (ACE_OS::strcmp (*copya_iter, *copytest_iter) != 0);
    }

  /// Testing - using ostream_iterator

  std::ostringstream ostream;
  std::copy (a.rbegin (),
             a.rend (),
             std::ostream_iterator<CORBA::StringSeq::const_element_type> (ostream,
                                                                          "\n"));

  FAIL_RETURN_IF (
    ostream.str ().compare ("elem3\nelem2\nelem1\nelem0\n") != 0);

  return 0;
}

//-----------------------------------------------------------------------------

int main(int,char*[])
{
  int status = 0;

  // Test Generic_Sequence_Iterator.
  status += test_sequence< ::CORBA::StringSeq::iterator> ();

  // Test Const_Generic_Sequence_Iterator with non-const sequence.
  status += test_sequence_const_iterator < 
               ::CORBA::StringSeq::const_iterator> ();

  // Test Const_Generic_Sequence_Iterator with const sequence.
  status += test_const_sequence< ::CORBA::StringSeq::const_iterator> ();

  // Test Generic_Sequence_Reverse_Iterator.
  status += test_sequence_reverse< ::CORBA::StringSeq::reverse_iterator> ();

  // Test Const_Generic_Sequence_Reverse_Iterator with non-const sequence.
  status += test_sequence_reverse_const_iterator< 
                ::CORBA::StringSeq::const_reverse_iterator> ();

  // Test Const_Generic_Sequence_Reverse_Iterator with const sequence.
  status += test_const_sequence_reverse< 
                ::CORBA::StringSeq::const_reverse_iterator> ();

  return status;
}

//-----------------------------------------------------------------------------
