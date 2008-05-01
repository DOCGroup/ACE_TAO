/**
 * @file Unbounded_Objectref.cpp
 *
 * @brief test for STL iterator behaviour of CORBA unbounded object reference 
 *        sequence
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
  /*
   * JWH2 - I don't think _duplicate is needed. Memory leaks show
   * up when it's used.
  a[0] = mock_reference::_duplicate (elem0);
  a[1] = mock_reference::_duplicate (elem1);
  a[2] = mock_reference::_duplicate (elem2);
  a[3] = mock_reference::_duplicate (elem3);
  */
  a[0] = elem0;
  a[1] = elem1;
  a[2] = elem2;
  a[3] = elem3;

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
  /// JWH2 - I don't think the ostream test makes sense for object references.
  /*
  std::ostringstream ostream;
  std::copy (a.begin (),
             a.end (),
             std::ostream_iterator<tested_sequence::value_type> (ostream,
                                                                  "\n"));

  FAIL_RETURN_IF (
    ostream.str ().compare ("elem0\nelem1\nelem2\nelem3\n") != 0);
  */

  return 0;
}

//-----------------------------------------------------------------------------

template <typename ITERATOR_T>
int test_const_sequence ()
{
  // setup of an example sequence
  tested_sequence setup;
  setup.length (4);

  mock_reference* elem0 = mock_reference::allocate (0);
  mock_reference* elem1 = mock_reference::allocate (1);
  mock_reference* elem2 = mock_reference::allocate (2);
  mock_reference* elem3 = mock_reference::allocate (3);

  // setup of an example sequence
  /*
   * JWH2 - I don't think _duplicate is needed. Memory leaks show
   * up when it's used.
  setup[0] = mock_reference::_duplicate (elem0);
  setup[1] = mock_reference::_duplicate (elem1);
  setup[2] = mock_reference::_duplicate (elem2);
  setup[3] = mock_reference::_duplicate (elem3);
  */
  setup[0] = elem0;
  setup[1] = elem1;
  setup[2] = elem2;
  setup[3] = elem3;

  // JWH2 - I think the double delete problem is here. Shouldn't the
  // underlying assignment operator make sure memory is allocated as
  // needed? It seems this isn't happening at some level since valgrind
  // is complaining that the sequence dtor is being called twice.
  const tested_sequence a = setup;

  // test equality operator
  FAIL_RETURN_IF (!(a.begin () == a.begin ()));

  // test non-equality operator
  FAIL_RETURN_IF (a.end () != a.end ());

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
  FAIL_RETURN_IF ((a_it[0])->id () != a[0]->id ());
  a_it += 2;
  FAIL_RETURN_IF ((a_it[0])->id () != a[2]->id ());

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
  test.length (4);

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
  /// JWH2 - I don't think the ostream test makes sense for object references.
  /*
  std::ostringstream ostream;
  std::copy (a.begin (),
             a.end (),
             std::ostream_iterator<tested_sequence::value_type> (ostream,
							    "\n"));

  FAIL_RETURN_IF (
    ostream.str ().compare ("elem0\nelem1\nelem2\nelem3\n") != 0);
  */

  return 0;
}

//-----------------------------------------------------------------------------

template <typename REVERSE_ITERATOR_T>
int test_sequence_reverse ()
{
  tested_sequence a;

  // test equality operator
  FAIL_RETURN_IF (!(a.begin () == a.begin ()));

  // test non-equality operator
  FAIL_RETURN_IF (a.end () != a.end ());

  // test for correct behaviour for empty sequence

  FAIL_RETURN_IF (a.begin() != a.end ());
  
  // setup of an example sequence
  a.length (4);

  mock_reference* elem0 = mock_reference::allocate (0);
  mock_reference* elem1 = mock_reference::allocate (1);
  mock_reference* elem2 = mock_reference::allocate (2);
  mock_reference* elem3 = mock_reference::allocate (3);

  // setup of an example sequence
  a.length (4);
  /*
   * JWH2 - I don't think _duplicate is needed. Memory leaks show
   * up when it's used.
  a[0] = mock_reference::_duplicate (elem0);
  a[1] = mock_reference::_duplicate (elem1);
  a[2] = mock_reference::_duplicate (elem2);
  a[3] = mock_reference::_duplicate (elem3);
  */
  a[0] = elem0;
  a[1] = elem1;
  a[2] = elem2;
  a[3] = elem3;

  // test iterator copy constructor
  REVERSE_ITERATOR_T a_it (a.rbegin ());
  FAIL_RETURN_IF (a_it != a.rbegin ());

  // test assignment operator
  a_it = a.rbegin ();
  FAIL_RETURN_IF (a_it != a.rbegin ());

  // test non const dereferencing
  mock_reference* value0 = *a_it;
  FAIL_RETURN_IF (value0->id () != elem3->id ());

  // test const dereferencing
  const mock_reference* const value1 = *a_it;
  FAIL_RETURN_IF (value1->id () != elem3->id ());

  // test increment operation
  a_it++;
  FAIL_RETURN_IF (a_it == a.rbegin());
  FAIL_RETURN_IF ((*a_it)->id () != elem2->id ());

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
  FAIL_RETURN_IF ((*a_it)->id () != elem0->id ());

  // test pre-decrement operator
  a_it = a.rend ();
  --a_it;
  FAIL_RETURN_IF (a_it == a.rend ());
  FAIL_RETURN_IF ((a.rend () - a_it) != 1);
  FAIL_RETURN_IF ((*a_it)->id () != elem0->id ());

  // test -= operator
  a_it -= 3;
  FAIL_RETURN_IF ((a_it1 - a_it) != 2);

  // test - operator
  a_it2 = a_it1 - 2;
  FAIL_RETURN_IF ((a_it1 - a_it2) != 2);

  // test operator[] read
  a_it = a.rbegin ();
  FAIL_RETURN_IF ((a_it[0])->id () != a[3]->id ());
  a_it += 2;
  FAIL_RETURN_IF ((a_it[0])->id () != a[1]->id ());

  // test for loop behaviour
  tested_sequence b = a;
  REVERSE_ITERATOR_T b_it = b.rbegin ();

  for (a_it = a.rbegin ();
       a_it != a.rend ();
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

  REVERSE_ITERATOR_T copytest_iter = test.rbegin ();
  for (REVERSE_ITERATOR_T copya_iter = a.rbegin ();
       copya_iter != a.rend ();
       ++copya_iter, ++copytest_iter)
    {
      FAIL_RETURN_IF ((*copya_iter)->id () != (*copytest_iter)->id ());
    }

  /// Testing - using ostream_iterator
  /// JWH2 - I don't think the ostream test makes sense for object references.
  /*
  std::ostringstream ostream;
  std::copy (a.rbegin (),
             a.rend (),
             std::ostream_iterator<tested_sequence::value_type> (ostream,
								  "\n"));

  FAIL_RETURN_IF (
    ostream.str ().compare ("elem3\nelem2\nelem1\nelem0\n") != 0);
  */

  return 0;
}

//-----------------------------------------------------------------------------

template <typename REVERSE_ITERATOR_T>
int test_const_sequence_reverse ()
{
  // setup of an example sequence
  tested_sequence setup;
  setup.length (4);

  mock_reference* elem0 = mock_reference::allocate (0);
  mock_reference* elem1 = mock_reference::allocate (1);
  mock_reference* elem2 = mock_reference::allocate (2);
  mock_reference* elem3 = mock_reference::allocate (3);

  // setup of an example sequence
  /*
   * JWH2 - I don't think _duplicate is needed. Memory leaks show
   * up when it's used.
  setup[0] = mock_reference::_duplicate (elem0);
  setup[1] = mock_reference::_duplicate (elem1);
  setup[2] = mock_reference::_duplicate (elem2);
  setup[3] = mock_reference::_duplicate (elem3);
  */
  setup[0] = elem0;
  setup[1] = elem1;
  setup[2] = elem2;
  setup[3] = elem3;

  // JWH2 - I think the double delete problem is here. Shouldn't the
  // underlying assignment operator make sure memory is allocated as
  // needed? It seems this isn't happening at some level since valgrind
  // is complaining that the sequence dtor is being called twice.
  const tested_sequence a = setup;

  // test equality operator
  FAIL_RETURN_IF (!(a.begin () == a.begin ()));

  // test non-equality operator
  FAIL_RETURN_IF (a.end () != a.end ());

  // test iterator copy constructor
  REVERSE_ITERATOR_T a_it (a.rbegin ());
  FAIL_RETURN_IF (a_it != a.rbegin ());

  // test assignment operator
  a_it = a.rbegin ();
  FAIL_RETURN_IF (a_it != a.rbegin ());

  // test non const dereferencing
  mock_reference* value0 = *a_it;
  FAIL_RETURN_IF (value0->id () != elem3->id ());

  // test const dereferencing
  const mock_reference* const value1 = *a_it;
  FAIL_RETURN_IF (value1->id () != elem3->id ());

  // test increment operation
  a_it++;
  FAIL_RETURN_IF (a_it == a.rbegin());
  FAIL_RETURN_IF ((*a_it)->id () != elem2->id ());

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
  FAIL_RETURN_IF ((*a_it)->id () != elem0->id ());

  // test pre-decrement operator
  a_it = a.rend ();
  --a_it;
  FAIL_RETURN_IF (a_it == a.rend ());
  FAIL_RETURN_IF ((a.rend () - a_it) != 1);
  FAIL_RETURN_IF ((*a_it)->id () != elem0->id ());

  // test -= operator
  a_it -= 3;
  FAIL_RETURN_IF ((a_it1 - a_it) != 2);

  // test - operator
  a_it2 = a_it1 - 2;
  FAIL_RETURN_IF ((a_it1 - a_it2) != 2);

  // test operator[] read
  a_it = a.rbegin ();
  FAIL_RETURN_IF ((a_it[0])->id () != a[3]->id ());
  a_it += 2;
  FAIL_RETURN_IF ((a_it[0])->id () != a[1]->id ());

  // test operator[] write
  // NOTE: This now changes the sequence a.
  // this is not possible for const iterators
  // a_it[0] = CORBA::string_dup (elem0_cstr);
  // FAIL_RETURN_IF (ACE_OS::strcmp (a[1],elem0_cstr) != 0);

  // reset content of sequence a
  //a[1] = CORBA::string_dup (elem1_cstr);

  // test for loop behaviour
  tested_sequence b = a;
  REVERSE_ITERATOR_T b_it = b.rbegin ();

  for (a_it = a.rbegin ();
       a_it != a.rend ();
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

  REVERSE_ITERATOR_T copytest_iter = test.rbegin ();
  for (REVERSE_ITERATOR_T copya_iter = a.rbegin ();
       copya_iter != a.rend ();
       ++copya_iter, ++copytest_iter)
    {
      FAIL_RETURN_IF ((*copya_iter)->id () != (*copytest_iter)->id ());
    }

  /// Testing - using ostream_iterator
  /// JWH2 - I don't think the ostream test makes sense for object references.
  /*
  std::ostringstream ostream;
  std::copy (a.rbegin (),
             a.rend (),
             std::ostream_iterator<tested_sequence::value_type> (ostream,
								  "\n"));

  FAIL_RETURN_IF (
    ostream.str ().compare ("elem3\nelem2\nelem1\nelem0\n") != 0);
  */

  return 0;
}

//-----------------------------------------------------------------------------

int main(int,char*[])
{
  int status = 0;

  // Test Generic_Sequence_Iterator.
  status += test_sequence< tested_sequence::iterator> ();

    // Test Const_Generic_Sequence_Iterator with non-const sequence.
  status += test_sequence< tested_sequence::const_iterator> ();

  // Test Const_Generic_Sequence_Iterator with const sequence.
  // JWH2 - This test is causing a segmentation fault - a double delete of the
  // sequence (and the elements in it). I'm not sure why.
  status += test_const_sequence< tested_sequence::const_iterator> ();

  // Test Generic_Sequence_Reverse_Iterator.
  status += test_sequence_reverse< tested_sequence::reverse_iterator> ();

  // Test Const_Generic_Sequence_Reverse_Iterator with non-const sequence.
  status += test_sequence_reverse< tested_sequence::const_reverse_iterator> ();

  // Test Const_Generic_Sequence_Reverse_Iterator with const sequence.
  // JWH2 - This test is causing a segmentation fault - a double delete of the
  // sequence (and the elements in it). I'm not sure why.
  status += test_const_sequence_reverse< tested_sequence::const_reverse_iterator> ();

  return status;
}
