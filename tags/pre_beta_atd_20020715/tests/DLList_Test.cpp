// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    DLList_Test.cpp
//
// = DESCRIPTION
//      This test illustrates the use of <ACE_DLList>.
//
// = AUTHOR
//    James Hu <jxh@cs.wustl.edu> and Douglas C. Schmidt <schmidt@cs.wustl.edu>
//
// ============================================================================

#include "test_config.h"
#include "ace/Containers.h"
#include "ace/SString.h"
#include "ace/Malloc.h"

typedef ACE_TCHAR *ACE_STRING;
typedef ACE_DLList<ACE_STRING> STRLIST;
typedef ACE_DLList_Iterator<ACE_STRING> STRLIST_ITERATOR;

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_DLList<ACE_STRING>;
template class ACE_DLList_Iterator<ACE_STRING>;
template class ACE_DLList_Reverse_Iterator<ACE_STRING>;
template class ACE_Static_Allocator<8192>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_DLList<ACE_STRING>
#pragma instantiate ACE_DLList_Iterator<ACE_STRING>
#pragma instantiate ACE_DLList_Reverse_Iterator<ACE_STRING>
#pragma instantiate ACE_Static_Allocator<8192>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

static ACE_STRING string_table[] =
{
  // Note: all these casts are to appease SC 5.0 which is not pleased
  // with using string literals (i.e. const char *'s) as char
  // *'s. It's ugly, but necessary.
  ACE_const_cast (ACE_TCHAR *, ACE_TEXT ("hello")),
  ACE_const_cast (ACE_TCHAR *, ACE_TEXT ("guten Tag")),
  ACE_const_cast (ACE_TCHAR *, ACE_TEXT ("goodbye")),
  ACE_const_cast (ACE_TCHAR *, ACE_TEXT ("auf wiedersehen")),
  ACE_const_cast (ACE_TCHAR *, ACE_TEXT ("funny")),
  ACE_const_cast (ACE_TCHAR *, ACE_TEXT ("lustig")),
  0
};

static void
run_iterate (STRLIST &list)
{
  ACE_STRING *entry;
  size_t i = 0;

  for (STRLIST_ITERATOR iter (list);
       (entry = iter.next ()) != 0;
       iter.advance (), i++)
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("iterating (%d): [%s]\n"),
                  i,
                  (ACE_TCHAR *) *entry));
    }
}

static int
run_test (void)
{
  STRLIST list;

  size_t i;

  for (i = 0; string_table[i] != 0; i++)
    {
      if (ACE_EVEN (i)
          && list.insert_tail ((ACE_STRING *) &string_table[i]) == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%p failed for %s \n"),
                           ACE_TEXT ("insert"),
                           string_table[i]),
                          -1);
      else if (list.insert_head ((ACE_STRING *) &string_table[i]) == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%p failed for %s \n"),
                           ACE_TEXT ("insert"),
                           string_table[i]),
                          -1);
      run_iterate (list);
    }

  run_iterate (list);

  list.delete_tail ();
  list.delete_tail ();

  run_iterate (list);

  list.delete_head ();
  list.delete_head ();

  run_iterate (list);

  return 0;
}

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("DLList_Test"));

  run_test ();

  ACE_END_TEST;

  return 0;
}
