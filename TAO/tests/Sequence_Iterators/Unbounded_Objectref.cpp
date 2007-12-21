/**
 * @file Unbounded_Objectref.cpp
 *
 * @brief test for STL iterator behaviour of CORBA unbounded value sequence
 *
 * $Id$
 *
 * @author Friedhelm Wolf (fwolf@dre.vanderbilt.edu)
 */

#include "ace/Log_Msg.h"

#include <iostream>
#include <iterator>
#include <sstream>

#include "tao/Object_Reference_Traits_T.h"
#include "mock_reference.hpp"
#include "tao/Unbounded_Object_Reference_Sequence_T.h"

#define FAIL_RETURN_IF(CONDITION) \
          ACE_DEBUG ((LM_INFO, ACE_TEXT ("in %N:%l\n"))); \
          if (CONDITION) \
	    { \
              ACE_DEBUG ((LM_ERROR, ACE_TEXT ("\tFailed at %N:%l\n"))); \
	      return 1; \
	    }

typedef TAO::unbounded_object_reference_sequence<mock_reference, mock_reference_var> tested_sequence;

template <typename ITERATOR_T>
int test_sequence ()
{
  tested_sequence a;

  // test equality operator
  FAIL_RETURN_IF (!(a.begin () == a.begin ()));

  // test non-equality operator
  FAIL_RETURN_IF (a.end () != a.end ());

  // test for correct behaviour for empty sequence

  FAIL_RETURN_IF (a.begin() != a.end ());
  
  mock_reference* elem0 = mock_reference::allocate (0);
  mock_reference* elem1 = mock_reference::allocate (1);
  mock_reference* elem2 = mock_reference::allocate (2);
  mock_reference* elem3 = mock_reference::allocate (3);

  // setup of an example sequence
  a.length (4);
  a[0] = mock_reference::_duplicate (elem0);
  a[1] = mock_reference::_duplicate (elem1);
  a[2] = mock_reference::_duplicate (elem2);
  a[3] = mock_reference::_duplicate (elem3);

  // test iterator copy constructor
  ITERATOR_T a_it (a.begin ());
  FAIL_RETURN_IF (a_it != a.begin ());

  // test assignment operator
  a_it = a.begin ();
  FAIL_RETURN_IF (a_it != a.begin ());

  // test non const dereferencing
  mock_reference* value0 = *a_it;
  FAIL_RETURN_IF (value0->id () != elem0->id ());

  // test const dereferencing
  const mock_reference* const value1 = *a_it;
  FAIL_RETURN_IF (value1->id () != elem0->id ());

  // test increment operation
  a_it++;
  FAIL_RETURN_IF (a_it == a.begin());
  FAIL_RETURN_IF ((*a_it)->id () != elem1->id ());

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
  FAIL_RETURN_IF ((*a_it)->id () != elem3->id ());

  // test pre-decrement operator
  a_it = a.end ();
  --a_it;
  FAIL_RETURN_IF (a_it == a.end ());
  FAIL_RETURN_IF ((a.end () - a_it) != 1);
  FAIL_RETURN_IF ((*a_it)->id () != elem3->id ());

  // test -= operator
  a_it -= 3;
  FAIL_RETURN_IF ((a_it1 - a_it) != 2);

  // test - operator
  a_it2 = a_it1 - 2;
  FAIL_RETURN_IF ((a_it1 - a_it2) != 2);

  // test operator[] read
  a_it = a.begin ();
  FAIL_RETURN_IF (a_it[0]->id () != a[0]->id ());
  a_it += 2;
  FAIL_RETURN_IF (a_it[0]->id () != a[2]->id ());

  // test for loop behaviour
  tested_sequence b = a;
  ITERATOR_T b_it = b.begin ();

  for (a_it = a.begin ();
       a_it != a.end ();
       a_it++, b_it++)
    {
      FAIL_RETURN_IF ((*a_it)->id () != (*b_it)->id ());
    }

  tested_sequence test;
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
      FAIL_RETURN_IF ((*copya_iter)->id () != (*copytest_iter)->id ());
    }

  /// Testing - using ostream_iterator

  std::ostringstream ostream;
  std::copy (a.begin (),
             a.end (),
             std::ostream_iterator<tested_sequence::value_type> (ostream,
                                                                  "\n"));

  FAIL_RETURN_IF (
    ostream.str ().compare ("elem0\nelem1\nelem2\nelem3\n") != 0);

  return 0;
}

int main(int,char*[])
{
  int status = 0;

  // Test Generic_Sequence_Iterator.
  status += test_sequence< tested_sequence::iterator> ();

  return status;
}
