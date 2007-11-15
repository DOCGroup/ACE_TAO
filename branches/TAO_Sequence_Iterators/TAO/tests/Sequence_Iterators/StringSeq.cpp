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

#include <iostream>
#include <iterator>
#include <sstream>

#define FAIL_RETURN_IF(CONDITION) \
          if (CONDITION) \
	    { \
              ACE_DEBUG ((LM_ERROR, ACE_TEXT ("\tFailed at %N:%l\n"))); \
	      return 1; \
	    }

int main(int,char*[])
{
  // Questions:
  // Should assignment of iterators be allowed? g++ seems to allow this.
  //  I'm not sure if it makes sense.
  // What should happen to iterators when the containers are modified?

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

  ::CORBA::StringSeq::value_type elem0 = CORBA::string_dup (elem0_cstr);
  ::CORBA::StringSeq::value_type elem1 = CORBA::string_dup (elem1_cstr);
  ::CORBA::StringSeq::value_type elem2 = CORBA::string_dup (elem2_cstr);
  ::CORBA::StringSeq::value_type elem3 = CORBA::string_dup (elem3_cstr);

  a.length (4);
  a[0] = elem0;
  a[1] = elem1;
  a[2] = elem2;
  a[3] = elem3;

  // test iterator copy constructor
  ::CORBA::StringSeq::iterator a_it (a.begin ());
  FAIL_RETURN_IF (a_it != a.begin ());

  // test assignment operator
  a_it = a.begin ();
  FAIL_RETURN_IF (a_it != a.begin ());

  // test non const dereferencing
  char* value0 = *a_it;
  FAIL_RETURN_IF (ACE_OS::strcmp (value0, elem0) != 0);

  // test const dereferencing
  const char* const value1 = *a_it;
  FAIL_RETURN_IF (ACE_OS::strcmp (value1, elem0) != 0);

  // test increment operation
  a_it++;
  FAIL_RETURN_IF (a_it == a.begin());
  FAIL_RETURN_IF (ACE_OS::strcmp (*a_it, elem1) != 0);

  // test < operator
  FAIL_RETURN_IF (!(a.begin () < a_it));
  FAIL_RETURN_IF (a_it < a.begin ());

  // test difference type
  ::CORBA::StringSeq::iterator::difference_type a_diff =
      a_it - a.begin ();
  FAIL_RETURN_IF (a_diff != 1);

  // test copy constructor 
  ::CORBA::StringSeq::iterator a_it1 (a_it);
  FAIL_RETURN_IF (a_it1 != a_it);

  // test preincrement operator
  ++a_it1;
  FAIL_RETURN_IF ((a_it1 - a_it) != 1);

  // test = and += operator
  ::CORBA::StringSeq::iterator a_it2 = a_it += 3;
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
  FAIL_RETURN_IF (ACE_OS::strcmp (*a_it, elem3) != 0);

  // test pre-decrement operator
  a_it1 = a_it;
  FAIL_RETURN_IF ((a_it - --a_it1) != 1);

  // test -= operator
  a_it -= 3;
  FAIL_RETURN_IF ((a_it1 - a_it) != 2);

  // test - operator
  a_it2 = a_it1 - 2;
  FAIL_RETURN_IF ((a_it1 - a_it2) != 2);

  // test operator[] read
  a_it = a.begin ();
  FAIL_RETURN_IF (ACE_OS::strcmp (a_it[0],a[0]) != 0);
  FAIL_RETURN_IF (ACE_OS::strcmp (a_it[1],a[1]) != 0);

  // test operator[] write
  // NOTE: This now changes the sequence a.
  a_it[2] = elem0;
  FAIL_RETURN_IF (ACE_OS::strcmp (a[2],elem0_cstr) != 0);

  // test for loop behaviour
  ::CORBA::StringSeq b = a;
  ::CORBA::StringSeq::iterator b_it = b.begin ();

  for (::CORBA::StringSeq::iterator a_it4 = a.begin ();
       a_it4 != a.end ();
       a_it4++, b_it++)
    {
      FAIL_RETURN_IF (ACE_OS::strcmp (*a_it4, *b_it) != 0);
    }

  ::CORBA::StringSeq test;
  test.length (a.length ());

  // Memory is leaked here from
  // TAO::details::string_traits_base<char>::default_initializer()

  std::copy (a.begin (),
             a.end (),
             test.begin ());

  FAIL_RETURN_IF (test.length () != a.length ());

  ::CORBA::StringSeq::iterator copytest_iter = test.begin ();
  for (::CORBA::StringSeq::iterator copya_iter = a.begin ();
       copya_iter != a.end ();
       ++copya_iter, ++copytest_iter)
    {
      FAIL_RETURN_IF (ACE_OS::strcmp (*copya_iter, *copytest_iter) != 0);
    }

  /// Testing - using ostream_iterator

  std::ostringstream ostream;
  std::copy (a.begin (),
             a.end (),
             std::ostream_iterator<CORBA::StringSeq::value_type> (ostream,
								  "\n"));

  FAIL_RETURN_IF (
     ostream.str ().compare ("elem0\nelem1\nelem0\nelem3\n") != 0);

  return 0;
}
