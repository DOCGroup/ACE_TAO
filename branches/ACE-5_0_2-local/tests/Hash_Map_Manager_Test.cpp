// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Hash_Map_Manager_Test.cpp
//
// = DESCRIPTION
//      This test illustrates the use of <ACE_Hash_Map_Manager> to
//      maintain a hash table using strings.  In addition, it also
//      illustrates how the <ACE_Static_Allocator> works in
//      conjunction with the <ACE_Hash_Map_Manager>.
//
// = AUTHOR
//    James Hu <jxh@cs.wustl.edu> and
//    Douglas C. Schmidt <schmidt@cs.wustl.edu>
//
// ============================================================================

#include "test_config.h"
#include "ace/Hash_Map_Manager.h"
#include "ace/Malloc_T.h"
#include "ace/Synch.h"

ACE_RCSID(tests, Hash_Map_Manager_Test, "$Id$")

#if defined(__BORLANDC__) && __BORLANDC__ >= 0x0530
USELIB("..\ace\aced.lib");
//---------------------------------------------------------------------------
#endif /* defined(__BORLANDC__) && __BORLANDC__ >= 0x0530 */

typedef ACE_Hash_Map_Entry<LPCTSTR,
                           LPCTSTR> HASH_STRING_ENTRY;

typedef ACE_Hash_Map_Manager_Ex<LPCTSTR,
                                LPCTSTR,
                                ACE_Hash<LPCTSTR>,
                                ACE_Equal_To<LPCTSTR>,
                                ACE_Null_Mutex> HASH_STRING_MAP;

typedef ACE_Hash_Map_Iterator_Ex<LPCTSTR,
                                 LPCTSTR,
                                 ACE_Hash<LPCTSTR>,
                                 ACE_Equal_To<LPCTSTR>,
                                 ACE_Null_Mutex> HASH_STRING_ITER;

typedef ACE_Hash_Map_Reverse_Iterator_Ex<LPCTSTR,
                                         LPCTSTR,
                                         ACE_Hash<LPCTSTR>,
                                         ACE_Equal_To<LPCTSTR>,
                                         ACE_Null_Mutex> HASH_STRING_REVERSE_ITER;

struct String_Table
{
  LPCTSTR key_;
  LPCTSTR value_;
};

static String_Table string_table[] =
{
  {
    ACE_TEXT ("hello"),
    ACE_TEXT ("guten Tag")
  },
  {
    ACE_TEXT ("goodbye"),
    ACE_TEXT ("auf wiedersehen")
  },
  {
    ACE_TEXT ("funny"),
    ACE_TEXT ("lustig")
  },
  {
    0,
    0
  }
};

static const size_t STRING_TABLE_SIZE = 3;
static const size_t MAX_HASH = 6;

// @@ The following requires too much internal implementation
// information about the <ACE_Hash_Map_Manager>.  We need to figure
// out how to simplify this.
static const size_t String_Table_size =
  sizeof (HASH_STRING_ENTRY) * (STRING_TABLE_SIZE + MAX_HASH);
static ACE_Static_Allocator<String_Table_size> alloc;

static int
run_test (void)
{
  alloc.dump ();

  HASH_STRING_MAP hash (MAX_HASH, &alloc);

  size_t i;

  // Check the <bind> operation.
  for (i = 0; string_table[i].key_ != 0; i++)
    if (hash.bind (string_table[i].key_,
                   string_table[i].value_) == -1)
      ACE_ERROR_RETURN ((LM_ERROR,
                         ASYS_TEXT ("%p failed for %s \n"),
                         ASYS_TEXT ("bind"),
                         ASYS_MULTIBYTE_STRING (string_table[i].key_)), -1);

  LPCTSTR entry;

  // Check the <find> operation.
  for (i = 0; string_table[i].key_ != 0; i++)
    if (hash.find (string_table[i].key_,
                   entry) == 0)
      ACE_DEBUG ((LM_DEBUG,
                  ASYS_TEXT ("`%s' found `%s'\n"),
                  ASYS_MULTIBYTE_STRING (string_table[i].key_),
                  entry));
    else
      ACE_ERROR_RETURN ((LM_ERROR,
                         ASYS_TEXT ("`%s' not found\n"),
                         ASYS_MULTIBYTE_STRING (string_table[i].key_)),
                        -1);

  // Check the <trybind> operation.
  {
    LPCTSTR pc = string_table[1].value_;
    if (hash.trybind (string_table[0].key_,
                      pc) != 1)
      ACE_ERROR_RETURN ((LM_ERROR,
                         ASYS_TEXT ("::trybind missed existing entry.")),
                        -1);
    else if (pc != string_table[0].value_)
      ACE_ERROR_RETURN ((LM_ERROR,
                         ASYS_TEXT ("::trybind doesn't return existing value.")),
                        -1);
  }

  // Let's test the iterator while we are at it.
  {
    HASH_STRING_ENTRY *entry;
    size_t i = 0;

    for (HASH_STRING_ITER hash_iter (hash);
         hash_iter.next (entry) != 0;
         hash_iter.advance ())
      {
        ACE_DEBUG ((LM_DEBUG,
                    ASYS_TEXT ("iterating (%d): [%s, %s]\n"),
                    i,
                    ASYS_MULTIBYTE_STRING ((LPCTSTR) entry->ext_id_),
                    ASYS_MULTIBYTE_STRING ((LPCTSTR) entry->int_id_)));
        i++;
      }
  }

  hash.unbind (string_table[2].key_, entry);

  // Check the <find> operation again.
  for (i = 0; string_table[i].key_ != 0; i++)
    if (hash.find (string_table[i].key_,
                   entry) == 0)
      ACE_DEBUG ((LM_DEBUG,
                  ASYS_TEXT ("`%s' found `%s'\n"),
                  ASYS_MULTIBYTE_STRING (string_table[i].key_),
                  ASYS_MULTIBYTE_STRING (entry)));
    else if (i != 2)
      ACE_ERROR_RETURN ((LM_ERROR,
                         ASYS_TEXT ("`%s' not found\n"),
                         ASYS_MULTIBYTE_STRING (string_table[i].key_)),
                        -1);

  // Let's test the iterator backwards.
  {
    HASH_STRING_ENTRY *entry;
    size_t i = 0;

    for (HASH_STRING_REVERSE_ITER hash_iter (hash);
         hash_iter.next (entry) != 0;
         hash_iter.advance ())
      {
        ACE_DEBUG ((LM_DEBUG,
                    ASYS_TEXT ("iterating (%d): [%s, %s]\n"),
                    i,
                    ASYS_MULTIBYTE_STRING ((LPCTSTR) entry->ext_id_),
                    ASYS_MULTIBYTE_STRING ((LPCTSTR) entry->int_id_)));
        i++;
      }
  }

  alloc.dump ();
  return 0;
}

int
main (int, ASYS_TCHAR *[])
{
  ACE_START_TEST (ASYS_TEXT ("Hash_Map_Manager_Test"));

  run_test ();

  ACE_END_TEST;

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Hash_Map_Entry<LPCTSTR, LPCTSTR>;
template class ACE_Hash_Map_Manager_Ex<LPCTSTR, LPCTSTR, ACE_Hash<LPCTSTR>, ACE_Equal_To<LPCTSTR>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<LPCTSTR, LPCTSTR, ACE_Hash<LPCTSTR>, ACE_Equal_To<LPCTSTR>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<LPCTSTR, LPCTSTR, ACE_Hash<LPCTSTR>, ACE_Equal_To<LPCTSTR>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<LPCTSTR, LPCTSTR, ACE_Hash<LPCTSTR>, ACE_Equal_To<LPCTSTR>, ACE_Null_Mutex>;
template class ACE_Static_Allocator<String_Table_size>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Hash_Map_Entry<LPCTSTR, LPCTSTR>
#pragma instantiate ACE_Hash_Map_Manager_Ex<LPCTSTR, LPCTSTR, ACE_Hash<LPCTSTR>, ACE_Equal_To<LPCTSTR>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<LPCTSTR, LPCTSTR, ACE_Hash<LPCTSTR>, ACE_Equal_To<LPCTSTR>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<LPCTSTR, LPCTSTR, ACE_Hash<LPCTSTR>, ACE_Equal_To<LPCTSTR>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<LPCTSTR, LPCTSTR, ACE_Hash<LPCTSTR>, ACE_Equal_To<LPCTSTR>, ACE_Null_Mutex>
#pragma instantiate ACE_Static_Allocator<String_Table_size>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
