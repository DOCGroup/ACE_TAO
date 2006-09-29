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
#if defined (__BORLANDC__) && (__BORLANDC__ <= 0x570)
// Borland C++ Builder 6 and earlier don't handle the second template
// argument correctly. We have to pass it explicitly
typedef ACE_Vector<DATA, ACE_VECTOR_DEFAULT_SIZE> VECTOR;
typedef ACE_Vector<DATA, ACE_VECTOR_DEFAULT_SIZE>::Iterator ITERATOR;
#else
typedef ACE_Vector<DATA> VECTOR;
typedef ACE_Vector<DATA>::Iterator ITERATOR;
#endif

const size_t TOP = 100;
const size_t LEFT = 10;
const size_t RESIZE = 20;

int run_main (int, ACE_TCHAR *[])
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

  // Test to be sure the iterator gets the correct count and entries.
  ITERATOR iter (vector);
  DATA *p_item = 0 ;
  size_t iter_count = 0;
  while (!iter.done ())
    {
      if (iter.next (p_item) == 0)
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("Fail to get value on iter pass %d\n"),
                    iter_count));
      if (*p_item != iter_count)
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("Iter pass %d got %d\n"),
                    iter_count, *p_item));
      iter_count++;
      iter.advance();
    }
  if (iter_count != TOP)
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Iterated %d elements; expected %d\n"),
                iter_count, TOP));

  for (i = 0; i < (TOP - LEFT); ++i)
    vector.pop_back ();

  ACE_ASSERT (vector.size () == LEFT);
  ACE_DEBUG ((LM_DEBUG,
	      ACE_TEXT ("Size: %d\n"),
	      vector.size ()));

  for (i = 0; i < LEFT; ++i)
    {
      ACE_ASSERT (vector[i] == i);
      ACE_DEBUG ((LM_DEBUG,
            ACE_TEXT ("vector[%d]:%d\n"),
            i, vector[i]));
    }

  vector.resize(RESIZE, 0);
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("After resize\n")));

  for (i = 0; i < RESIZE ; ++i)
    {
      // The original vector of size LEFT must have the same original contents
      // the new elements should have the value 0 (this value is passed as
      // second argument of the resize() call.
      if (i < LEFT)
        {
          ACE_ASSERT (vector[i] == i);
        }
      else
        {
          ACE_ASSERT (vector[i] == 0);
        }
      ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("vector[%d]:%d\n"),
              i, vector[i]));
    }

  vector.clear ();
  ACE_ASSERT (vector.size () == 0);
  ACE_DEBUG ((LM_DEBUG,
	      ACE_TEXT ("Size: %d\n"),
	      vector.size ()));

  VECTOR v1;
  VECTOR v2;
  v1.push_back (1);
  v2.push_back (1);
  v1.push_back (2);
  v2.push_back (2);
  if (v1 != v2)
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Inequality test failed!\n")));
  if (!(v1 == v2))
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Equality test failed!\n")));

  ACE_END_TEST;

  return 0;
}

