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

static const size_t STRING_TABLE_ENTRIES = 3 * 2;
static const size_t MAX_HASH = 6;

typedef ACE_Hash_Map_Entry<const ACE_TCHAR *,
                           const ACE_TCHAR *> HASH_STRING_ENTRY;

// @@ The following requires too much internal implementation
// information about the <ACE_Hash_Map_Manager>.  We need to figure
// out how to simplify this.
static const size_t STRING_TABLE_SIZE =
  sizeof (HASH_STRING_ENTRY) * (STRING_TABLE_ENTRIES + MAX_HASH);

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Hash_Map_Entry<const ACE_TCHAR *, const ACE_TCHAR *>;
template class ACE_Hash_Map_Manager_Ex<const ACE_TCHAR *, const ACE_TCHAR *, ACE_Hash<const ACE_TCHAR *>, ACE_Equal_To<const ACE_TCHAR *>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<const ACE_TCHAR *, const ACE_TCHAR *, ACE_Hash<const ACE_TCHAR *>, ACE_Equal_To<const ACE_TCHAR *>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Const_Iterator_Base_Ex<const ACE_TCHAR *, const ACE_TCHAR *, ACE_Hash<const ACE_TCHAR *>, ACE_Equal_To<const ACE_TCHAR *>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<const ACE_TCHAR *, const ACE_TCHAR *, ACE_Hash<const ACE_TCHAR *>, ACE_Equal_To<const ACE_TCHAR *>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Const_Iterator_Ex<const ACE_TCHAR *, const ACE_TCHAR *, ACE_Hash<const ACE_TCHAR *>, ACE_Equal_To<const ACE_TCHAR *>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<const ACE_TCHAR *, const ACE_TCHAR *, ACE_Hash<const ACE_TCHAR *>, ACE_Equal_To<const ACE_TCHAR *>, ACE_Null_Mutex>;
template class ACE_Static_Allocator<STRING_TABLE_SIZE>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Hash_Map_Entry<const ACE_TCHAR *, const ACE_TCHAR *>
#pragma instantiate ACE_Hash_Map_Manager_Ex<const ACE_TCHAR *, const ACE_TCHAR *, ACE_Hash<const ACE_TCHAR *>, ACE_Equal_To<const ACE_TCHAR *>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<const ACE_TCHAR *, const ACE_TCHAR *, ACE_Hash<const ACE_TCHAR *>, ACE_Equal_To<const ACE_TCHAR *>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Const_Iterator_Base_Ex<const ACE_TCHAR *, const ACE_TCHAR *, ACE_Hash<const ACE_TCHAR *>, ACE_Equal_To<const ACE_TCHAR *>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<const ACE_TCHAR *, const ACE_TCHAR *, ACE_Hash<const ACE_TCHAR *>, ACE_Equal_To<const ACE_TCHAR *>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Const_Iterator_Ex<const ACE_TCHAR *, const ACE_TCHAR *, ACE_Hash<const ACE_TCHAR *>, ACE_Equal_To<const ACE_TCHAR *>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<const ACE_TCHAR *, const ACE_TCHAR *, ACE_Hash<const ACE_TCHAR *>, ACE_Equal_To<const ACE_TCHAR *>, ACE_Null_Mutex>
#pragma instantiate ACE_Static_Allocator<STRING_TABLE_SIZE>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

static ACE_Static_Allocator<STRING_TABLE_SIZE> ace_test_allocator;

typedef ACE_Hash_Map_Manager_Ex<const ACE_TCHAR *,
                                const ACE_TCHAR *,
                                ACE_Hash<const ACE_TCHAR *>,
                                ACE_Equal_To<const ACE_TCHAR *>,
                                ACE_Null_Mutex> HASH_STRING_MAP;

typedef ACE_Hash_Map_Iterator_Ex<const ACE_TCHAR *,
                                 const ACE_TCHAR *,
                                 ACE_Hash<const ACE_TCHAR *>,
                                 ACE_Equal_To<const ACE_TCHAR *>,
                                 ACE_Null_Mutex> HASH_STRING_ITER;

typedef ACE_Hash_Map_Const_Iterator_Ex<const ACE_TCHAR *,
                                       const ACE_TCHAR *,
                                       ACE_Hash<const ACE_TCHAR *>,
                                       ACE_Equal_To<const ACE_TCHAR *>,
                                       ACE_Null_Mutex> HASH_STRING_CONST_ITER;

typedef ACE_Hash_Map_Reverse_Iterator_Ex<const ACE_TCHAR *,
                                         const ACE_TCHAR *,
                                         ACE_Hash<const ACE_TCHAR *>,
                                         ACE_Equal_To<const ACE_TCHAR *>,
                                         ACE_Null_Mutex> HASH_STRING_REVERSE_ITER;

struct String_Table
{
  const ACE_TCHAR *key_;
  const ACE_TCHAR *value_;
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

static int
run_test (void)
{
  ace_test_allocator.dump ();

  HASH_STRING_MAP hash (MAX_HASH, &ace_test_allocator);

  size_t i;

  // Check the <bind> operation.
  for (i = 0; string_table[i].key_ != 0; i++)
    if (hash.bind (string_table[i].key_,
                   string_table[i].value_) == -1)
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("%p failed for %s \n"),
                         ACE_TEXT ("bind"),
                         string_table[i].key_), -1);

  const ACE_TCHAR *entry;

  // Check the <find> operation.
  for (i = 0; string_table[i].key_ != 0; i++)
    if (hash.find (string_table[i].key_,
                   entry) == 0)
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("`%s' found `%s'\n"),
                  string_table[i].key_,
                  entry));
    else
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("`%s' not found\n"),
                         string_table[i].key_),
                        -1);

  // Check the <trybind> operation.
  {
    const ACE_TCHAR *pc = string_table[1].value_;
    if (hash.trybind (string_table[0].key_,
                      pc) != 1)
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("::trybind missed existing entry.")),
                        -1);
    else if (pc != string_table[0].value_)
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("::trybind doesn't return existing value.")),
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
                    ACE_TEXT ("iterating (%d): [%s, %s]\n"),
                    i,
                    entry->ext_id_,
                    entry->int_id_));
        i++;
      }
  }

  // And now test the const iterator
  {
    HASH_STRING_ENTRY *entry;
    size_t i = 0;

    for (HASH_STRING_CONST_ITER hash_iter (hash);
         hash_iter.next (entry) != 0;
         hash_iter.advance ())
      {
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("const iterating (%d): [%s, %s]\n"),
                    i,
                    entry->ext_id_,
                    entry->int_id_));
        i++;
      }
  }

  hash.unbind (string_table[2].key_, entry);

  // Check the <find> operation again.
  for (i = 0; string_table[i].key_ != 0; i++)
    if (hash.find (string_table[i].key_,
                   entry) == 0)
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("`%s' found `%s'\n"),
                  string_table[i].key_,
                  entry));
    else if (i != 2)
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("`%s' not found\n"),
                         string_table[i].key_),
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
                    ACE_TEXT ("iterating (%d): [%s, %s]\n"),
                    i,
                    entry->ext_id_,
                    entry->int_id_));
        i++;
      }
  }

  // Remove all the entries.
  if (hash.unbind_all () != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("unbind_all failed\n")),
                       -1);

  // Redo the <bind> operations.
  for (i = 0; string_table[i].key_ != 0; i++)
    if (hash.bind (string_table[i].key_,
                   string_table[i].value_) != 0)
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("%p failed for %s \n"),
                         ACE_TEXT ("bind"),
                         string_table[i].key_), -1);

  ace_test_allocator.dump ();
  return 0;
}

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Hash_Map_Manager_Test"));

  run_test ();

  ACE_END_TEST;

  return 0;
}
