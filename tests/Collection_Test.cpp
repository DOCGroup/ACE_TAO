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

typedef ACE_Unbounded_Set<DATA> COLLECTION;
typedef ACE_Unbounded_Set_Iterator<DATA> ITERATOR;

int main (int, char *[])
{
  ACE_START_TEST (ASYS_TEXT ("Collection_Test"));

  COLLECTION collection;

  collection.insert (1);
  collection.insert (2);

  for (COLLECTION::iterator iterator1 = collection.begin ();
       iterator1 != collection.end ();
       iterator1++)
    {
      ACE_DEBUG ((LM_DEBUG, "%d\n",
                  (*iterator1)));
    }

  ITERATOR iterator2 (collection);
  while (!iterator2.done ())
    {
      int *data = 0;
      iterator2.next (data);
      ACE_DEBUG ((LM_DEBUG, "%d\n",
                  (*data)));
      iterator2.advance ();
    }

  ACE_END_TEST;

  return 0;
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Unbounded_Set_Iterator<DATA>;
template class ACE_Unbounded_Set<DATA>;
template class ACE_Node<DATA>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Unbounded_Set_Iterator<DATA>
#pragma instantiate ACE_Unbounded_Set<DATA>
#pragma instantiate ACE_Node<DATA>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
