// $Id$

#include "HT_Client.h"

// Insert a KEY_NAME into the hash table, if it doesn't already exist
// there.  What gets returned is a pointer to the node inserted.  Note
// that we do our own memory allocation here...

Protocol_Record *
HT_Client::insert (const char *key_name, int max_len)
{
  Protocol_Record **frpp = 0;

  // This is tricky... 

  for (frpp = &this->hash_table[ACE::hash_pjw (key_name)];
       *frpp != 0
         && ACE_OS::strncmp ((*frpp)->get_login (),
                             key_name, max_len) != 0;
       frpp = &(*frpp)->next_)
    continue;

  if (*frpp == 0)
    {
      ACE_NEW_RETURN (*frpp,
                      Protocol_Record (ACE::strnew (key_name),
                                       *frpp),
                      0);
      this->count_++;
    }

  return *frpp;
}


