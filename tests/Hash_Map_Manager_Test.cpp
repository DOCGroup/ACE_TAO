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
#include "STL_algorithm_Test_T.h"
#include "ace/Hash_Map_Manager.h"
#include "ace/Malloc_T.h"
#include "ace/Null_Mutex.h"



static const size_t STRING_TABLE_ENTRIES = 3 * 2;
static const size_t MAX_HASH = 6;

typedef ACE_Hash_Map_Entry<const ACE_TCHAR *,
                           const ACE_TCHAR *> HASH_STRING_ENTRY;

// @@ The following requires too much internal implementation
// information about the <ACE_Hash_Map_Manager>.  We need to figure
// out how to simplify this.
static const size_t STRING_TABLE_SIZE =
  sizeof (HASH_STRING_ENTRY) * (STRING_TABLE_ENTRIES + MAX_HASH);

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

static
int test_two_allocators ()
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing both allocators\n")));
  int status = 0;

  // Number of entries in string_table above:
  size_t chunks     = 3;
  size_t chunk_size = sizeof (HASH_STRING_MAP::ENTRY);

  // Allocators:
  ACE_Dynamic_Cached_Allocator<ACE_Null_Mutex> table_alloc (1 , chunk_size * chunks);
  ACE_Dynamic_Cached_Allocator<ACE_Null_Mutex> table_alloc_small (1, chunk_size * chunks - 1);
  ACE_Cached_Allocator<HASH_STRING_MAP::ENTRY, ACE_Null_Mutex> entry_alloc (chunks);

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Testing hash map manager with %d elements...\n"), chunks));

  HASH_STRING_MAP hash;

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Opening hash map manager with ")
              ACE_TEXT ("insufficient table allocator, should fail...\n")));
  ACE_OS::last_error (0);
  status = hash.open (chunks, &table_alloc_small, &entry_alloc);
  if (status < 0)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("OK, failed: %d (%s)\n"),
                status, ACE_OS::strerror (ACE_OS::last_error ())));
  else
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("*** Something is wrong...\n")), -1);

  status = hash.close ();

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Table allocator depth: %d.\n"),
              table_alloc.pool_depth ()));
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Entry allocator depth: %d.\n"),
              entry_alloc.pool_depth ()));

  ACE_DEBUG
    ((LM_DEBUG,
      ACE_TEXT ("Opening hash map manager again, should succeed...\n")));
  ACE_OS::last_error (0);
  status = hash.open (chunks, &table_alloc, &entry_alloc);
  if (status < 0)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("*** Something is wrong: %d (%s)\n"),
                       status, ACE_OS::strerror (ACE_OS::last_error ())), -1);
  else
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("OK.\n")));

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Table allocator depth: %d\n"),
              table_alloc.pool_depth ()));
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Entry allocator depth: %d.\n"),
              entry_alloc.pool_depth ()));

  for (size_t i = 0; i < chunks; i++)
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("- Binding ('%s', '%s'), should succeed...\n"),
                  string_table[i].key_ , string_table[i].value_));
      ACE_OS::last_error (0);
      status = hash.bind (string_table[i].key_, string_table[i].value_);
      if (status < 0)
        ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("*** Something is wrong: %d (%s)\n"),
                           status, ACE_OS::strerror (ACE_OS::last_error ())), -1);
      else
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("OK, entry allocator depth: %d\n"),
                    entry_alloc.pool_depth ()));
    }

  const ACE_TCHAR *key = ACE_TEXT ("key");
  const ACE_TCHAR *val = ACE_TEXT ("value");

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("- Binding ('%s', '%s'), should fail...\n"),
              key, val));
  ACE_OS::last_error (0);
  status = hash.bind (key, val);
  if (status < 0)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("OK, failed (%s).\n"),
                ACE_OS::strerror (ACE_OS::last_error ())));
  else
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("*** Something is wrong: %d (%s)\n"),
                       status, ACE_OS::strerror (ACE_OS::last_error ())), -1);

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Closing hash map.\n")));
  status = hash.close ();

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Table allocator depth: %d.\n"),
              table_alloc.pool_depth ()));
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Entry allocator depth: %d.\n"),
              entry_alloc.pool_depth ()));

  return 0;
}

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
                         ACE_TEXT ("%p failed for %s\n"),
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
    HASH_STRING_ENTRY *entry = 0;
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
    HASH_STRING_ENTRY *entry = 0;
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
    HASH_STRING_ENTRY *entry = 0;
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
                         ACE_TEXT ("%p failed for %s\n"),
                         ACE_TEXT ("bind"),
                         string_table[i].key_), -1);

  ace_test_allocator.dump ();

  test_two_allocators();

  // Run the STL algorithm test on the hash map.
  const HASH_STRING_MAP & chash = hash;

  test_STL_algorithm (hash);
  test_STL_algorithm (chash);

  return 0;
}

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Hash_Map_Manager_Test"));

  run_test ();

  ACE_END_TEST;

  return 0;
}
