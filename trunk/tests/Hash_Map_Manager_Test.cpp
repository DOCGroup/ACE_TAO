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

#if defined (ACE_HAS_TEMPLATE_SPECIALIZATION)

ACE_Hash_Map_Entry<char * , char *>::ACE_Hash_Map_Entry (char * const &ext_id,
							 char * const &int_id,
							 ACE_Hash_Map_Entry<char *, char *> *ptr)
  : ext_id_ (ext_id),
    int_id_ (ACE_OS::strdup (int_id)),
    next_ (ptr)
{
}

ACE_Hash_Map_Entry<char *, char *>::~ACE_Hash_Map_Entry (void)
{
  cerr << "Freeing " << int_id_ << endl;
  ACE_OS::free (int_id_);
}

// We need this template specialization since KEY is defined as a
// char*, which doesn't have a hash() method defined on it.

long unsigned int
ACE_Hash_Map_Manager<char *, char *, ACE_Null_Mutex>::hash (char * const & ext_id)
{
  return ACE::hash_pjw (ext_id);
}

int
ACE_Hash_Map_Manager<char *, char *, ACE_Null_Mutex>::equal (char * const & id1,
							     char * const & id2)
{
  return (ACE_OS::strcmp (id1, id2) == 0);
}

#endif /* ACE_HAS_TEMPLATE_SPECIALIZATION */

// If code below breaks, it's probably because template specialization
// is not supported.

static const int MAX_HASH = 256;

int main(void)
{
  ACE_Hash_Map_Manager<char *, char *, ACE_Null_Mutex> hash;
  hash.open (MAX_HASH);

  hash.bind ("hello", "guten Tag");
  hash.bind ("goodbye", "auf widersehen");
  hash.bind ("funny", "spiele");

  char * entry;

  if (hash.find ("hello", entry) == 0)
    cout << "Found " << entry << endl;
  if (hash.find ("goodbye", entry) == 0)
    cout << "Found " << entry << endl;
  if (hash.find ("funny", entry) == 0)
    cout << "Found " << entry << endl;

  hash.unbind ("goodbye", entry);

  if (hash.find ("hello", entry) == 0)
    cout << "Found " << entry << endl;
  if (hash.find ("goodbye", entry) == 0)
    cout << "OOPS! Found " << entry << endl;
  if (hash.find ("funny", entry) == 0)
    cout << "Found " << entry << endl;

  hash.close ();
  return 0;
}

#if defined (ACE_TEMPLATES_REQUIRE_SPECIALIZATION)
template class ACE_Hash_Map_Entry<char *, char *>;
template class ACE_Hash_Map_Manager<char *, char *, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator<char *, char *, ACE_Null_Mutex>;
#endif /* ACE_TEMPLATES_REQUIRE_SPECIALIZATION */
