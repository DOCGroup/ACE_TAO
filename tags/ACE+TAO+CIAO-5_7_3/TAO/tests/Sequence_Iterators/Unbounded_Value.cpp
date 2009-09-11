/**
 * @file Unbounded_Value.cpp
 *
 * @brief test for STL iterator behaviour of CORBA bounded string sequence
 *
 * $Id$
 *
 * @author Friedhelm Wolf (fwolf@dre.vanderbilt.edu)
 */

#include "tao/Unbounded_Value_Sequence_T.h"
#include "ace/Log_Msg.h"

#include <iostream>
#include <iterator>
#include <sstream>

#if defined TAO_HAS_SEQUENCE_ITERATORS && TAO_HAS_SEQUENCE_ITERATORS == 1

typedef TAO::unbounded_value_sequence<int> v_sequence;

#define FAIL_RETURN_IF(CONDITION) \
          if (CONDITION) \
      { \
              ACE_DEBUG ((LM_ERROR, ACE_TEXT ("\tFailed at %N:%l\n"))); \
        return 1; \
      }

template <typename ITERATOR_T>
int test_sequence ()
{
  v_sequence a;

  // test equality operator
  FAIL_RETURN_IF (!(a.begin () == a.begin ()));

  // test non-equality operator
  FAIL_RETURN_IF (a.end () != a.end ());

  // test for correct behaviour for empty sequence
  FAIL_RETURN_IF (a.begin() != a.end ());

  // setup of an example sequence
  a.length (4);

  int elem0 = 0;
  int elem1 = 1;
  int elem2 = 2;
  int elem3 = 3;

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
  // JWH2 - I don't think this test makes sense. I believe the compiler
  // will always return a const value since the dereference is on
  // the right hand side of the assignment (i.e., r value).
  //int value0 = *a_it;
  //FAIL_RETURN_IF (value0 != elem0);

  // test const dereferencing
  int value1 = *a_it;
  FAIL_RETURN_IF (value1 != elem0);

  // test increment operation
  a_it++;
  FAIL_RETURN_IF (a_it == a.begin());
  FAIL_RETURN_IF (*a_it != elem1);

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
  FAIL_RETURN_IF (*a_it != elem3);

  // test pre-decrement operator
  a_it = a.end ();
  --a_it;
  FAIL_RETURN_IF (a_it == a.end ());
  FAIL_RETURN_IF ((a.end () - a_it) != 1);
  FAIL_RETURN_IF (*a_it != elem3);

  // test -= operator
  a_it -= 3;
  FAIL_RETURN_IF ((a_it1 - a_it) != 2);

  // test - operator
  a_it2 = a_it1 - 2;
  FAIL_RETURN_IF ((a_it1 - a_it2) != 2);

  // test operator[] read
  a_it = a.begin ();
  FAIL_RETURN_IF (a_it[0] != a[0]);
  a_it += 2;
  FAIL_RETURN_IF (a_it[0] != a[2]);

  // test operator[] write
  // NOTE: This now changes the sequence a.
  // NOTE: This does not work for const_iterators
  // a_it[0] = elem0;
  // FAIL_RETURN_IF (a[2] != elem0);

  // reset content of sequence a
  //a[2] = elem2;

  // test for loop behaviour
  v_sequence b = a;
  ITERATOR_T b_it = b.begin ();

  for (a_it = a.begin ();
       a_it != a.end ();
       a_it++, b_it++)
    {
      FAIL_RETURN_IF (*a_it != *b_it);
    }

  v_sequence test;
  test.length (4);

  std::copy (a.begin (),
             a.end (),
             test.begin ());

  FAIL_RETURN_IF (test.length () != a.length ());

  ITERATOR_T copytest_iter = test.begin ();
  for (ITERATOR_T copya_iter = a.begin ();
       copya_iter != a.end ();
       ++copya_iter, ++copytest_iter)
    {
      FAIL_RETURN_IF (*copya_iter != *copytest_iter);
    }

  /// Testing - using ostream_iterator

  std::ostringstream ostream;
  std::copy (a.begin (),
             a.end (),
             // JWH2 - I changed value_type to const_value_type. Is that
             // the correct approach?
             std::ostream_iterator<v_sequence::const_value_type> (ostream,
                                                                  "\n"));

  FAIL_RETURN_IF (
    ostream.str ().compare ("0\n1\n2\n3\n") != 0);

  return 0;
}

//-----------------------------------------------------------------------------

template <typename ITERATOR_T>
int test_const_sequence ()
{
  // setup of an example sequence
  v_sequence setup;
  setup.length (4);

  int elem0 = 0;
  int elem1 = 1;
  int elem2 = 2;
  int elem3 = 3;

  setup[0] = elem0;
  setup[1] = elem1;
  setup[2] = elem2;
  setup[3] = elem3;

  const v_sequence a = setup;

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
  // JWH2 - I don't think this test makes sense. I believe the compiler
  // will always return a const value since the dereference is on
  // the right hand side of the assignment (i.e., r value).
  //char* value0 = *a_it;
  //FAIL_RETURN_IF (value0 != elem0);

  // test const dereferencing
  int value1 = *a_it;
  FAIL_RETURN_IF (value1 != elem0);

  // test increment operation
  a_it++;
  FAIL_RETURN_IF (a_it == a.begin());
  FAIL_RETURN_IF (*a_it != elem1);

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
  FAIL_RETURN_IF (*a_it != elem3);

  // test pre-decrement operator
  a_it = a.end ();
  --a_it;
  FAIL_RETURN_IF (a_it == a.end ());
  FAIL_RETURN_IF ((a.end () - a_it) != 1);
  FAIL_RETURN_IF (*a_it != elem3);

  // test -= operator
  a_it -= 3;
  FAIL_RETURN_IF ((a_it1 - a_it) != 2);

  // test - operator
  a_it2 = a_it1 - 2;
  FAIL_RETURN_IF ((a_it1 - a_it2) != 2);

  // test operator[] read
  a_it = a.begin ();
  FAIL_RETURN_IF (a_it[0] != a[0]);
  a_it += 2;
  FAIL_RETURN_IF (a_it[0] != a[2]);

  // test operator[] write
  // NOTE: This now changes the sequence a.
  // NOTE: This does not work for const_iterators
  // a_it[0] = elem0;
  // FAIL_RETURN_IF (a[2] != elem0);

  // reset content of sequence a
  //a[2] = elem2;

  // test for loop behaviour
  v_sequence b = a;
  ITERATOR_T b_it = b.begin ();

  for (a_it = a.begin ();
       a_it != a.end ();
       a_it++, b_it++)
    {
      FAIL_RETURN_IF (*a_it != *b_it);
    }

  v_sequence test;
  test.length (4);

  std::copy (a.begin (),
             a.end (),
             test.begin ());

  FAIL_RETURN_IF (test.length () != a.length ());

  ITERATOR_T copytest_iter = test.begin ();
  for (ITERATOR_T copya_iter = a.begin ();
       copya_iter != a.end ();
       ++copya_iter, ++copytest_iter)
    {
      FAIL_RETURN_IF (*copya_iter != *copytest_iter);
    }

  /// Testing - using ostream_iterator

  std::ostringstream ostream;
  std::copy (a.begin (),
             a.end (),
             // JWH2 - I changed value_type to const_value_type. Is that
             // the correct approach?
             std::ostream_iterator<v_sequence::const_value_type> (ostream,
                                                                  "\n"));

  FAIL_RETURN_IF (
    ostream.str ().compare ("0\n1\n2\n3\n") != 0);

  return 0;
}

//-----------------------------------------------------------------------------

template <typename REVERSE_ITERATOR_T>
int test_sequence_reverse ()
{
  v_sequence a;

  // test equality operator
  FAIL_RETURN_IF (!(a.begin () == a.begin ()));

  // test non-equality operator
  FAIL_RETURN_IF (a.end () != a.end ());

  // test for correct behaviour for empty sequence

  FAIL_RETURN_IF (a.begin() != a.end ());

  // setup of an example sequence
  a.length (4);

  int elem0 = 0;
  int elem1 = 1;
  int elem2 = 2;
  int elem3 = 3;

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
  // JWH2 - I don't think this test makes sense. I believe the compiler
  // will always return a const value since the dereference is on
  // the right hand side of the assignment (i.e., r value).
  //int value0 = *a_it;
  //FAIL_RETURN_IF (value0 != elem3);

  // test const dereferencing
  int value1 = *a_it;
  FAIL_RETURN_IF (value1 != elem3);

  // test increment operation
  a_it++;
  FAIL_RETURN_IF (a_it == a.rbegin());
  FAIL_RETURN_IF (*a_it != elem2);

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
  FAIL_RETURN_IF (*a_it != elem0);

  // test pre-decrement operator
  a_it = a.rend ();
  --a_it;
  FAIL_RETURN_IF (a_it == a.rend ());
  FAIL_RETURN_IF ((a.rend () - a_it) != 1);
  FAIL_RETURN_IF (*a_it != elem0);

  // test -= operator
  a_it -= 3;
  FAIL_RETURN_IF ((a_it1 - a_it) != 2);

  // test - operator
  a_it2 = a_it1 - 2;
  FAIL_RETURN_IF ((a_it1 - a_it2) != 2);

  // test operator[] read
  a_it = a.rbegin ();
  FAIL_RETURN_IF (a_it[0] != a[3]);
  a_it += 2;
  FAIL_RETURN_IF (a_it[0] != a[1]);

  // test operator[] write
  // NOTE: This now changes the sequence a.
  // this is not possible for const iterators
  // a_it[0] = elem0;
  // FAIL_RETURN_IF (a[1] != elem0);

  // reset content of sequence a
  //a[1] = elem1;

  // test for loop behaviour
  v_sequence b = a;
  REVERSE_ITERATOR_T b_it = b.rbegin ();

  for (a_it = a.rbegin ();
       a_it != a.rend ();
       a_it++, b_it++)
    {
      FAIL_RETURN_IF (*a_it != *b_it);
    }

  v_sequence test;
  test.length (a.length ());

  std::copy (a.begin (),
             a.end (),
             test.begin ());

  FAIL_RETURN_IF (test.length () != a.length ());

  REVERSE_ITERATOR_T copytest_iter = test.rbegin ();
  for (REVERSE_ITERATOR_T copya_iter = a.rbegin ();
       copya_iter != a.rend ();
       ++copya_iter, ++copytest_iter)
    {
      FAIL_RETURN_IF (*copya_iter != *copytest_iter);
    }

  /// Testing - using ostream_iterator

  std::ostringstream ostream;
  std::copy (a.rbegin (),
             a.rend (),
             // JWH2 - I changed value_type to const_value_type. Is that
             // the correct approach?
             std::ostream_iterator<v_sequence::const_value_type> (ostream,
                  "\n"));

  FAIL_RETURN_IF (
    ostream.str ().compare ("3\n2\n1\n0\n") != 0);

  return 0;
}

//-----------------------------------------------------------------------------

template <typename REVERSE_ITERATOR_T>
int test_const_sequence_reverse ()
{
  // setup of an example sequence
  v_sequence setup;
  setup.length (4);

  int elem0 = 0;
  int elem1 = 1;
  int elem2 = 2;
  int elem3 = 3;

  setup[0] = elem0;
  setup[1] = elem1;
  setup[2] = elem2;
  setup[3] = elem3;

  const v_sequence a = setup;

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
  // JWH2 - I don't think this test makes sense. I believe the compiler
  // will always return a const value since the dereference is on
  // the right hand side of the assignment (i.e., r value).
  //int value0 = *a_it;
  //FAIL_RETURN_IF (value0 != elem3);

  // test const dereferencing
  int value1 = *a_it;
  FAIL_RETURN_IF (value1 != elem3);

  // test increment operation
  a_it++;
  FAIL_RETURN_IF (a_it == a.rbegin());
  FAIL_RETURN_IF (*a_it != elem2);

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
  FAIL_RETURN_IF (*a_it != elem0);

  // test pre-decrement operator
  a_it = a.rend ();
  --a_it;
  FAIL_RETURN_IF (a_it == a.rend ());
  FAIL_RETURN_IF ((a.rend () - a_it) != 1);
  FAIL_RETURN_IF (*a_it != elem0);

  // test -= operator
  a_it -= 3;
  FAIL_RETURN_IF ((a_it1 - a_it) != 2);

  // test - operator
  a_it2 = a_it1 - 2;
  FAIL_RETURN_IF ((a_it1 - a_it2) != 2);

  // test operator[] read
  a_it = a.rbegin ();
  FAIL_RETURN_IF (a_it[0] != a[3]);
  a_it += 2;
  FAIL_RETURN_IF (a_it[0] != a[1]);

  // test operator[] write
  // NOTE: This now changes the sequence a.
  // this is not possible for const iterators
  // a_it[0] = elem0;
  // FAIL_RETURN_IF (a[1] != elem0);

  // reset content of sequence a
  //a[1] = elem1;

  // test for loop behaviour
  v_sequence b = a;
  REVERSE_ITERATOR_T b_it = b.rbegin ();

  for (a_it = a.rbegin ();
       a_it != a.rend ();
       a_it++, b_it++)
    {
      FAIL_RETURN_IF (*a_it != *b_it);
    }

  v_sequence test;
  test.length (a.length ());

  std::copy (a.begin (),
             a.end (),
             test.begin ());

  FAIL_RETURN_IF (test.length () != a.length ());

  REVERSE_ITERATOR_T copytest_iter = test.rbegin ();
  for (REVERSE_ITERATOR_T copya_iter = a.rbegin ();
       copya_iter != a.rend ();
       ++copya_iter, ++copytest_iter)
    {
      FAIL_RETURN_IF (*copya_iter != *copytest_iter);
    }

  /// Testing - using ostream_iterator

  std::ostringstream ostream;
  std::copy (a.rbegin (),
             a.rend (),
             // JWH2 - I changed value_type to const_value_type. Is that
             // the correct approach?
             std::ostream_iterator<v_sequence::const_value_type> (ostream,
                  "\n"));

  FAIL_RETURN_IF (
    ostream.str ().compare ("3\n2\n1\n0\n") != 0);

  return 0;
}

#endif

//-----------------------------------------------------------------------------

int ACE_TMAIN(int,ACE_TCHAR*[])
{
  int status = 0;

#if defined TAO_HAS_SEQUENCE_ITERATORS && TAO_HAS_SEQUENCE_ITERATORS == 1

  // Test Generic_Sequence_Iterator.
  status += test_sequence<v_sequence::iterator> ();

  // g++ seems to make the conversion from iterator to const_iterator
  // and Windows doesn't. Not sure why.
  // Test Const_Generic_Sequence_Iterator with non-const sequence.
  status += test_sequence<v_sequence::const_iterator> ();

  // Test Const_Generic_Sequence_Iterator with const sequence.
  status += test_const_sequence<v_sequence::const_iterator> ();

  // Test Generic_Sequence_Reverse_Iterator.
  status += test_sequence_reverse<v_sequence::reverse_iterator> ();

  // Test Const_Generic_Sequence_Reverse_Iterator with non-const sequence.
  status += test_sequence_reverse<v_sequence::const_reverse_iterator> ();

  // Test Const_Generic_Sequence_Reverse_Iterator with const sequence.
  status += test_const_sequence_reverse<v_sequence::const_reverse_iterator> ();
#endif

  return status;
}
