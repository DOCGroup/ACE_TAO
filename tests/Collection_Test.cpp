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
//    Irfan Pyarali <irfan@cs.wustl.edu>
//
// ============================================================================

#include "test_config.h"

ACE_RCSID(tests, Collection_Test, "$Id$")

#include "ace/Containers.h"

typedef int DATA;
typedef ACE_Unbounded_Set<DATA> UNBOUNDED_SET;
typedef ACE_Unbounded_Set_Iterator<DATA> UNBOUNDED_SET_ITERATOR;
typedef ACE_Unbounded_Set_Const_Iterator<DATA> UNBOUNDED_SET_CONST_ITERATOR;
typedef ACE_Array<DATA> ARRAY;
typedef ACE_Array_Iterator<DATA> ARRAY_ITERATOR;

void iterate_const(const UNBOUNDED_SET& set)
{
  {
    UNBOUNDED_SET_CONST_ITERATOR iterator (set);
    while (!iterator.done ())
    {
      DATA *data = 0;
      iterator.next (data);
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%d\n"),
                  (*data)));
      DATA *data_second = *iterator;
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%d\n"),
                  (*data_second)));
      iterator.advance ();
    }
  }
}

int main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Collection_Test"));

  {
    UNBOUNDED_SET unbounded_set;

    unbounded_set.insert (1);
    unbounded_set.insert (2);

    {
      for (UNBOUNDED_SET::iterator iterator = unbounded_set.begin ();
           iterator != unbounded_set.end ();
           ++iterator)
        {
          ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%d\n"),
                      (*iterator)));
        }
    }

    {
      UNBOUNDED_SET_ITERATOR iterator (unbounded_set);
      while (!iterator.done ())
        {
          DATA *data = 0;
          iterator.next (data);
          ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%d\n"),
                      (*data)));
          iterator.advance ();
        }
    }
    iterate_const (unbounded_set);
  }

  {
    ARRAY array;
  }

  {
    ARRAY array (0);
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
          ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%d\n"),
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
          ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%d\n"),
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
template class ACE_Unbounded_Set<DATA>;
template class ACE_Unbounded_Set_Iterator<DATA>;
template class ACE_Unbounded_Set_Const_Iterator<DATA>;

#if (ACE_SIZEOF_INT != 4)
// These might be already instantiated in ace/stats.cpp
// (if ACE_INT32 == int)
template class ACE_Node<DATA>;
#endif /* ACE_SIZEOF_INT != 4 */

template class ACE_Array<DATA>;
template class ACE_Array_Base<DATA>;
template class ACE_Array_Iterator<DATA>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Unbounded_Set<DATA>
#pragma instantiate ACE_Unbounded_Set_Iterator<DATA>
#pragma instantiate ACE_Unbounded_Set_Const_Iterator<DATA>
#pragma instantiate ACE_Node<DATA>
#pragma instantiate ACE_Array<DATA>
#pragma instantiate ACE_Array_Base<DATA>
#pragma instantiate ACE_Array_Iterator<DATA>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
