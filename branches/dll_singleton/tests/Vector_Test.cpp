// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Vector_Test.cpp
//
// = DESCRIPTION
//     This is a simple test of the ACE_Vector class and its iterators.
//
// = AUTHOR
//    Gonzalo A. Diethelm <gonzalo.diethelm@aditiva.com>
//
// ============================================================================

#include "test_config.h"

ACE_RCSID(tests, Vector_Test, "$Id$")

#include "ace/Vector_T.h"

typedef size_t DATA;
typedef ACE_Vector<DATA> VECTOR;
typedef ACE_Vector<DATA>::Iterator ITERATOR;

const size_t TOP = 100;
const size_t LEFT = 10;

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Vector_Test"));

  VECTOR vector;
  size_t i;

  for (i = 0; i < TOP; ++i)
    vector.push_back (i);

  ACE_ASSERT (vector.size () == TOP);
  ACE_DEBUG ((LM_DEBUG,
	      ACE_TEXT ("Size: %d\n"),
	      vector.size ()));

  for (i = 0; i < TOP; ++i)
    ACE_ASSERT (vector[i] == i);

  for (i = 0; i < (TOP - LEFT); ++i)
    vector.pop_back ();

  ACE_ASSERT (vector.size () == LEFT);
  ACE_DEBUG ((LM_DEBUG,
	      ACE_TEXT ("Size: %d\n"),
	      vector.size ()));

  for (i = 0; i < LEFT; ++i)
    ACE_ASSERT (vector[i] == i);

  vector.clear ();
  ACE_ASSERT (vector.size () == 0);
  ACE_DEBUG ((LM_DEBUG,
	      ACE_TEXT ("Size: %d\n"),
	      vector.size ()));

  ACE_END_TEST;

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Vector<DATA>;
template class ACE_Array<DATA>;
template class ACE_Array_Base<DATA>;
template class ACE_Array_Iterator<DATA>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Vector<DATA>
#pragma instantiate ACE_Array<DATA>
#pragma instantiate ACE_Array_Base<DATA>
#pragma instantiate ACE_Array_Iterator<DATA>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
