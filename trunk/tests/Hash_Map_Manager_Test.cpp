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
//      This test illustrates the use of ACE_Hash_Map_Manager to
//      maintain a hash table using strings. No command line arguments
//      are needed to run this program.
//
// = AUTHOR
//    James Hu
// 
// ============================================================================

#include <iostream.h>

#include "ace/Hash_Map_Manager.h"
#include "ace/Synch.h"
#include "test_config.h"

#define HASH_STRING_ENTRY ACE_Hash_Map_Entry<char *, char *>
#define HASH_STRING_MAP ACE_Hash_Map_Manager<char *, char *, ACE_Null_Mutex>

#if defined (ACE_HAS_TEMPLATE_SPECIALIZATION)

HASH_STRING_ENTRY::ACE_Hash_Map_Entry (char *const &ext_id,
                                       char *const &int_id,
                                       HASH_STRING_ENTRY *ptr)
  : ext_id_ (ACE_OS::strdup (ext_id)),
    int_id_ (ACE_OS::strdup (int_id)),
    next_ (ptr)
{
  ACE_DEBUG ((LM_DEBUG, "Creating `%s' and `%s'\n", ext_id_, int_id_));
}

HASH_STRING_ENTRY::~ACE_Hash_Map_Entry (void)
{
  ACE_DEBUG ((LM_DEBUG, "Freeing `%s' and `%s'\n", ext_id_, int_id_));
  ACE_OS::free (ext_id_);
  ACE_OS::free (int_id_);
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

#endif /* ACE_HAS_TEMPLATE_SPECIALIZATION */

// If code below breaks, it's probably because template specialization
// is not supported.

static const int MAX_HASH = 256;

int
main(void)
{
  ACE_START_TEST ("Hash_Map_Manager_Test");

  // Scoping below so that result of destruction can be seen in the log.
  {
    HASH_STRING_MAP hash (MAX_HASH);

    hash.bind ("hello", "guten Tag");
    hash.bind ("goodbye", "auf wiedersehen");
    hash.bind ("funny", "lustig");

    char * entry;

    if (hash.find ("hello", entry) == 0)
      ACE_DEBUG ((LM_DEBUG, "`%s' found `%s'\n", "hello", entry));
    if (hash.find ("goodbye", entry) == 0)
      ACE_DEBUG ((LM_DEBUG, "`%s' found `%s'\n", "goodbye", entry));
    if (hash.find ("funny", entry) == 0)
      ACE_DEBUG ((LM_DEBUG, "`%s' found `%s'\n", "funny", entry));

    hash.unbind ("goodbye", entry);

    if (hash.find ("hello", entry) == 0)
      ACE_DEBUG ((LM_DEBUG, "`%s' found `%s'\n", "hello", entry));
    if (hash.find ("goodbye", entry) == 0)
      ACE_DEBUG ((LM_DEBUG, "OOPS!  `%s' found `%s'\n", "goodbye", entry));
    if (hash.find ("funny", entry) == 0)
      ACE_DEBUG ((LM_DEBUG, "`%s' found `%s'\n", "funny", entry));
  }

  ACE_END_TEST;

  return 0;
}

#if defined (ACE_TEMPLATES_REQUIRE_SPECIALIZATION)
template class ACE_Hash_Map_Entry<char *, char *>;
template class ACE_Hash_Map_Manager<char *, char *, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator<char *, char *, ACE_Null_Mutex>;
#endif /* ACE_TEMPLATES_REQUIRE_SPECIALIZATION */
