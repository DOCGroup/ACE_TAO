// $Id$
#include "new.h"
#include "HT_Client.h"

/* Insert a KEY_NAME into the hash table, if it doesn't already exist there. 
   What gets returned is a pointer to the node inserted.  Note that we do our
   own memory allocation here...  */

Protocol_Record *
HT_Client::insert (char *key_name, int max_len)
{
  /* This is tricky... */

  for (Protocol_Record **frpp = &this->hash_table[this->hash (key_name)];
       *frpp != 0 && strncmp ((*frpp)->get_login (), key_name, max_len) != 0;
       frpp = &(*frpp)->next)
    ;

  if (*frpp == 0)
    {
      *frpp = new (PRIVATE_POOL) Protocol_Record (ACE::strnew (key_name), *frpp);
      this->count++;
    }
  return *frpp;
}


