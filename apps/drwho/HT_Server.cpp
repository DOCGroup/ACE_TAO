// $Id$

#include "HT_Server.h"

// Insert a KEY_NAME into the hash table, if it doesn't already exist
// there.  What gets returned is a pointer to the node inserted.  Note
// that we do our own memory allocation here...

Protocol_Record *
HT_Server::insert (const char *key_name, int max_len)
{
  Protocol_Record **frpp = 0;

  // This is tricky...

  for (frpp = &this->hash_table[ACE::hash_pjw (key_name)];
       *frpp != 0 && strncmp ((*frpp)->get_login (), key_name, max_len) != 0;
       frpp = &(*frpp)->next_)
    continue;

  if (*frpp == 0)
    {
      // Remember, the server must be very careful about stuff it
      // receives from the rwho manager, since it may not be
      // NUL-terminated.  That's why we use ACE::strnnew ()...

      ACE_NEW_RETURN (*frpp,
                      Protocol_Record (ACE::strnnew (key_name,
                                                     max_len),
                                       *frpp),
                      0);
      this->count_++;
    }

  return *frpp;
}
