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
//    James Hu and Douglas C. Schmidt
//
// ============================================================================

#include "test_config.h"
#include "ace/Containers.h"
#include "ace/SString.h"
#include "ace/Malloc.h"

#if defined(__BORLANDC__) && __BORLANDC__ >= 0x0530
USELIB("..\ace\aced.lib");
//---------------------------------------------------------------------------
#endif /* defined(__BORLANDC__) && __BORLANDC__ >= 0x0530 */

static ASYS_TCHAR * string_table[] =
{
  ASYS_TEXT ("hello"),
  ASYS_TEXT ("guten Tag"),
  ASYS_TEXT ("goodbye"),
  ASYS_TEXT ("auf wiedersehen"),
  ASYS_TEXT ("funny"),
  ASYS_TEXT ("lustig"),
  0
};

static ACE_Static_Allocator<8192> alloc;

typedef ASYS_TCHAR * STRING;
typedef ACE_DLList<STRING> STRLIST;
typedef ACE_DLList_Iterator<STRING> STRLIST_ITERATOR;

static void
run_iterate (STRLIST &list)
{
  STRING *entry;
  size_t i = 0;

  for (STRLIST_ITERATOR iter (list);
       (entry = iter.next ()) != 0;
       iter.advance (), i++)
    {
      ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("iterating (%d): [%s]\n"),
                  i,
                  (ASYS_TCHAR *) *entry));
    }
}

static int
run_test (void)
{
  alloc.dump ();

  STRLIST list;

  size_t i;

  for (i = 0; string_table[i] != 0; i++)
    {
      if (i % 2)
        {
          if (list.insert_tail (&string_table[i]) == 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               ASYS_TEXT ("%p failed for %s \n"),
                               ASYS_TEXT ("insert"),
                               string_table[i]), -1);
        }
      else
        {
          if (list.insert_head (&string_table[i]) == 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               ASYS_TEXT ("%p failed for %s \n"),
                               ASYS_TEXT ("insert"),
                               string_table[i]), -1);
        }

      run_iterate (list);
    }

  run_iterate (list);

  list.delete_tail ();
  list.delete_tail ();

  run_iterate (list);

  list.delete_head ();
  list.delete_head ();

  run_iterate (list);

  alloc.dump ();
  return 0;
}

int
main (int, ASYS_TCHAR *[])
{
  ACE_START_TEST (ASYS_TEXT ("DLList_Test"));

  run_test ();

  ACE_END_TEST;

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_DLList<STRING>;
template class ACE_DLList_Iterator<STRING>;
template class ACE_Static_Allocator<8192>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_DLList<STRING>
#pragma instantiate ACE_DLList_Iterator<STRING>
#pragma instantiate ACE_Static_Allocator<8192>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
