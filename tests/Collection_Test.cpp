// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Collection_Test.cpp
//
// = DESCRIPTION
//     This is a simple test of the ACE collection classes and its
//     iterators.
//
// = AUTHOR
//    Irfan Pyarali
//
// ============================================================================

#include "test_config.h"

ACE_RCSID(tests, Collection_Test, "$Id$")

#if defined(__BORLANDC__) && __BORLANDC__ >= 0x0530
USELIB("..\ace\aced.lib");
//---------------------------------------------------------------------------
#endif /* defined(__BORLANDC__) && __BORLANDC__ >= 0x0530 */

#include "ace/Containers.h"

typedef int DATA;

typedef ACE_Unbounded_Set<DATA> UNBOUNDED_SET;
typedef ACE_Unbounded_Set_Iterator<DATA> UNBOUNDED_SET_ITERATOR;
typedef ACE_Array<DATA> ARRAY;
typedef ACE_Array_Iterator<DATA> ARRAY_ITERATOR;

int main (int, char *[])
{
  ACE_START_TEST (ASYS_TEXT ("Collection_Test"));

  {
    UNBOUNDED_SET unbounded_set;

    unbounded_set.insert (1);
    unbounded_set.insert (2);

    {
      for (UNBOUNDED_SET::iterator iterator = unbounded_set.begin ();
           iterator != unbounded_set.end ();
           ++iterator)
        {
          ACE_DEBUG ((LM_DEBUG, "%d\n",
                      (*iterator)));
        }
    }

    {
      UNBOUNDED_SET_ITERATOR iterator (unbounded_set);
      while (!iterator.done ())
        {
          DATA *data = 0;
          iterator.next (data);
          ACE_DEBUG ((LM_DEBUG, "%d\n",
                      (*data)));
          iterator.advance ();
        }
    }
  }

  {
    ARRAY array1;
    array1.size (2);
    array1[0] = 4;
    array1[1] = 4;

    ARRAY array2 (2, 4);

    ARRAY array3 (array2);

    ARRAY array4;
    array4 = array2;

    ACE_ASSERT (array1 == array2);
    ACE_ASSERT (array1 == array3);
    ACE_ASSERT (array1 == array4);

    {
      for (size_t i = 0;
           i != array1.size ();
           ++i)
        {
          ACE_DEBUG ((LM_DEBUG, "%d\n",
                      array1[i]));
          ACE_ASSERT (array1[i] == 4);
        }
    }

    {
      ARRAY_ITERATOR iterator (array1);
      while (!iterator.done ())
        {
          DATA *data = 0;
          iterator.next (data);
          ACE_DEBUG ((LM_DEBUG, "%d\n",
                      (*data)));
          ACE_ASSERT (*data == 4);
          iterator.advance ();
        }
    }
  }

  ACE_END_TEST;

  return 0;
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class UNBOUNDED_SET;
template class UNBOUNDED_SET_ITERATOR;
template class ACE_Node<DATA>;
template class ARRAY;
template class ARRAY_ITERATOR;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate UNBOUNDED_SET
#pragma instantiate UNBOUNDED_SET_ITERATOR
#pragma instantiate ACE_Node<DATA>
#pragma instantiate ARRAY
#pragma instantiate ARRAY_ITERATOR
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
