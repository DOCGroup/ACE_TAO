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
//    James Hu and Douglas C. Schmidt
//
// ============================================================================

#include "test_config.h"
#include "ace/Hash_Map_Manager.h"
#include "ace/Malloc_T.h"
#include "ace/SString.h"
#include "ace/Synch.h"

#if defined (ACE_HAS_TEMPLATE_SPECIALIZATION)

#define HASH_STRING_ENTRY ACE_Hash_Map_Entry<char *, char *>
#define HASH_STRING_MAP ACE_Hash_Map_Manager<char *, char *, ACE_Null_Mutex>
#define HASH_STRING_ITER ACE_Hash_Map_Iterator<char *, char *, ACE_Null_Mutex>
#define HASH_STRING_REVERSE_ITER ACE_Hash_Map_Reverse_Iterator<char *, char *, ACE_Null_Mutex>

#define MAP_STRING char *
#define ENTRY entry

HASH_STRING_ENTRY::ACE_Hash_Map_Entry (char *const &ext_id,
                                       char *const &int_id,
                                       HASH_STRING_ENTRY *next,
                                       HASH_STRING_ENTRY *prev)
  : ext_id_ (ACE_OS::strdup (ext_id)),
    int_id_ (ACE_OS::strdup (int_id)),
    next_ (next),
    prev_ (prev)
{
  ACE_DEBUG ((LM_DEBUG, "Creating `%s' and `%s'\n", ext_id_, int_id_));
}

HASH_STRING_ENTRY::ACE_Hash_Map_Entry (HASH_STRING_ENTRY *next,
                                       HASH_STRING_ENTRY *prev)
  : ext_id_ (0),
    int_id_ (0),
    next_ (next),
    prev_ (prev)
{
}

HASH_STRING_ENTRY::~ACE_Hash_Map_Entry (void)
{
  char *key = ext_id_;
  char *value = int_id_;

  if (key != 0 && value != 0)
    ACE_DEBUG ((LM_DEBUG, "Freeing `%s' and `%s'\n", key, value));
  ACE_OS::free (key);
  ACE_OS::free (value);
}

// We need this template specialization since KEY is defined as a
// char*, which doesn't have a hash() method defined on it.

long unsigned int
HASH_STRING_MAP::hash (char *const &ext_id)
{
  return ACE::hash_pjw (ext_id);
}

int
HASH_STRING_MAP::equal (char *const &id1, char *const &id2)
{
  return ACE_OS::strcmp (id1, id2) == 0;
}

#else

// Do this if we don't have template specialization

#include "Hash_Map_Manager_Test.h"      // Dumb_String is in here

#define HASH_STRING_ENTRY ACE_Hash_Map_Entry<Dumb_String, Dumb_String>
#define HASH_STRING_MAP \
        ACE_Hash_Map_Manager<Dumb_String, Dumb_String, ACE_Null_Mutex>
#define HASH_STRING_ITER \
        ACE_Hash_Map_Iterator<Dumb_String, Dumb_String, ACE_Null_Mutex>
#define HASH_STRING_REVERSE_ITER \
        ACE_Hash_Map_Reverse_Iterator<Dumb_String, Dumb_String, ACE_Null_Mutex>

#define MAP_STRING Dumb_String
#define ENTRY ((char *) entry)

Dumb_String::Dumb_String (char *s)
  : s_ (s ? ACE_OS::strdup (s) : s),
    copy_ (s ? *(new int (1)) : junk_),
    junk_ (1)
{
}

Dumb_String::Dumb_String (const Dumb_String &ds)
  : s_ (ds.s_),
    copy_ (ds.copy_),
    junk_ (1)
{
  copy_++;
}

Dumb_String::~Dumb_String (void)
{
  if (--copy_ == 0)
    {
      ACE_OS::free (s_);
      if (&copy_ != &junk_)
        delete &copy_;
    }
}

u_long
Dumb_String::hash (void) const
{
  return ACE::hash_pjw (s_);
}

int
Dumb_String::operator== (char const * s) const
{
  return ACE_OS::strcmp (s_, s) == 0;
}

int
Dumb_String::operator== (const Dumb_String &ds) const
{
  return ACE_OS::strcmp (s_, ds.s_) == 0;
}

char *
Dumb_String::operator= (const Dumb_String &ds)
{
  this->Dumb_String::~Dumb_String ();
  new (this) Dumb_String (ds);
  return s_;
}

Dumb_String::operator char * (void) const
{
  return s_;
}

// Note that in this version, you will not get the Creating and Freeing
// diagnostic messages.

#endif /* ACE_HAS_TEMPLATE_SPECIALIZATION */

struct String_Table
{
  char *key_;
  char *value_;
};

static String_Table string_table[] =
{
  { "hello",
    "guten Tag"
  },
  { "goodbye",
    "auf wiedersehen"
  },
  { "funny",
    "lustig"
  },
  { 0,
    0
  }
};

static const size_t STRING_TABLE_SIZE = 3;
static const size_t MAX_HASH = 6;

// @@ The following requires too much internal implementation
// information about the <ACE_Hash_Map_Manager>.  We need to figure
// out how to simplify this.
static const size_t String_Table_size = sizeof (HASH_STRING_ENTRY) * (STRING_TABLE_SIZE + MAX_HASH);
static ACE_Static_Allocator<String_Table_size> allocator;

static int
run_test (void)
{
  allocator.dump ();

  HASH_STRING_MAP hash (MAX_HASH, &allocator);

  size_t i;

  for (i = 0; string_table[i].key_ != 0; i++)
    if (hash.bind (string_table[i].key_,
                   string_table[i].value_) == -1)
      ACE_ERROR_RETURN ((LM_ERROR,
                         "%p failed for %s \n",
                         "bind",
                         string_table[i].key_), -1);

  MAP_STRING entry;

  for (i = 0; string_table[i].key_ != 0; i++)
    if (hash.find (string_table[i].key_,
                   entry) == 0)
      ACE_DEBUG ((LM_DEBUG,
                  "`%s' found `%s'\n",
                  string_table[i].key_,
                  ENTRY));
    else
      ACE_ERROR_RETURN ((LM_ERROR,
                         "`%s' not found\n",
                         string_table[i].key_),
                        -1);

  // Let's test the iterator while we are at it.
  {
    HASH_STRING_ENTRY *entry;
    size_t i = 0;

    for (HASH_STRING_ITER hash_iter (hash);
         hash_iter.next (entry) != 0;
         hash_iter.advance (), i++)
      ACE_DEBUG ((LM_DEBUG, "iterating (%d): [%s, %s]\n",
                  i,
                  (char *) entry->ext_id_,
                  (char *) entry->int_id_));
  }

  hash.unbind (string_table[2].key_, entry);

  for (i = 0; string_table[i].key_ != 0; i++)
    if (hash.find (string_table[i].key_,
                   entry) == 0)
      ACE_DEBUG ((LM_DEBUG,
                  "`%s' found `%s'\n",
                  string_table[i].key_,
                  ENTRY));
    else if (i != 2)
      ACE_ERROR_RETURN ((LM_ERROR,
                         "`%s' not found\n",
                         string_table[i].key_),
                        -1);

  // Let's test the iterator backwards.
  {
    HASH_STRING_ENTRY *entry;
    size_t i = 0;

    for (HASH_STRING_REVERSE_ITER hash_iter (hash);
         hash_iter.next (entry) != 0;
         hash_iter.advance (), i++)
      ACE_DEBUG ((LM_DEBUG, "iterating (%d): [%s, %s]\n",
                  i,
                  (char *) entry->ext_id_,
                  (char *) entry->int_id_));
  }

  allocator.dump ();
  return 0;
}

int
main (int, char *[])
{
  ACE_START_TEST ("Hash_Map_Manager_Test");

  run_test ();

  ACE_END_TEST;

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Hash_Map_Entry<MAP_STRING, MAP_STRING>;
template class ACE_Hash_Map_Manager<MAP_STRING, MAP_STRING, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator<MAP_STRING, MAP_STRING, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base<MAP_STRING, MAP_STRING, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator<MAP_STRING, MAP_STRING, ACE_Null_Mutex>;
template class ACE_Static_Allocator<String_Table_size>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Hash_Map_Entry<MAP_STRING, MAP_STRING>
#pragma instantiate ACE_Hash_Map_Manager<MAP_STRING, MAP_STRING, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator<MAP_STRING, MAP_STRING, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base<MAP_STRING, MAP_STRING, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<MAP_STRING, MAP_STRING, ACE_Null_Mutex>
#pragma instantiate ACE_Static_Allocator<String_Table_size>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
