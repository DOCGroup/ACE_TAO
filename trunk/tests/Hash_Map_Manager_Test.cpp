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
#include "ace/SString.h"
#include "ace/Synch.h"
#include "test_config.h"

#if defined (ACE_HAS_TEMPLATE_SPECIALIZATION)

#define HASH_STRING_ENTRY ACE_Hash_Map_Entry<char *, char *>
#define HASH_STRING_MAP ACE_Hash_Map_Manager<char *, char *, ACE_Null_Mutex>

#define MAP_STRING char *
#define ENTRY entry

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

#else

// Do this if we don't have template specialization

#include "Hash_Map_Manager_Test.h"	// Dumb_String is in here

#define HASH_STRING_ENTRY ACE_Hash_Map_Entry<Dumb_String, Dumb_String>
#define HASH_STRING_MAP \
        ACE_Hash_Map_Manager<Dumb_String, Dumb_String, ACE_Null_Mutex>

#define MAP_STRING Dumb_String
#define ENTRY ((char *)entry)

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

    MAP_STRING entry;

    if (hash.find ("hello", entry) == 0)
      ACE_DEBUG ((LM_DEBUG, "`%s' found `%s'\n", "hello", ENTRY));
    if (hash.find ("goodbye", entry) == 0)
      ACE_DEBUG ((LM_DEBUG, "`%s' found `%s'\n", "goodbye", ENTRY));
    if (hash.find ("funny", entry) == 0)
      ACE_DEBUG ((LM_DEBUG, "`%s' found `%s'\n", "funny", ENTRY));

    hash.unbind ("goodbye", entry);

    if (hash.find ("hello", entry) == 0)
      ACE_DEBUG ((LM_DEBUG, "`%s' found `%s'\n", "hello", ENTRY));
    if (hash.find ("goodbye", entry) == 0)
      ACE_DEBUG ((LM_DEBUG, "OOPS!  `%s' found `%s'\n", "goodbye", ENTRY));
    if (hash.find ("funny", entry) == 0)
      ACE_DEBUG ((LM_DEBUG, "`%s' found `%s'\n", "funny", ENTRY));
  }

  ACE_END_TEST;

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Hash_Map_Entry<MAP_STRING, MAP_STRING>;
template class ACE_Hash_Map_Manager<MAP_STRING, MAP_STRING, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator<MAP_STRING, MAP_STRING, ACE_Null_Mutex>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Hash_Map_Entry<MAP_STRING, MAP_STRING>
#pragma instantiate ACE_Hash_Map_Manager<MAP_STRING, MAP_STRING, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator<MAP_STRING, MAP_STRING, ACE_Null_Mutex>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

