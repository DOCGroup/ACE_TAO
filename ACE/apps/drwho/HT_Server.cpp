#include "HT_Server.h"
#include "ace/ACE.h"
#include "ace/OS_Memory.h"
#include <cstring>

// Insert a KEY_NAME into the hash table, if it doesn't already exist
// there.  What gets returned is a pointer to the node inserted.  Note
// that we do our own memory allocation here...

Protocol_Record *
HT_Server::insert (const char *key_name, int max_len)
{
  Protocol_Record **prpp = nullptr;

  // This is tricky...

  for (prpp = &this->hash_table[ACE::hash_pjw (key_name) % this->hash_table_size];
       *prpp != nullptr && std::strncmp ((*prpp)->get_login (), key_name, max_len) != 0;
       prpp = &(*prpp)->next_)
    continue;

  if (*prpp == nullptr)
    {
      // Remember, the server must be very careful about stuff it
      // receives from the rwho manager, since it may not be
      // NUL-terminated.  That's why we use ACE::strnnew ()...

      ACE_NEW_RETURN (*prpp,
                      Protocol_Record (ACE::strnnew (key_name,
                                                     max_len),
                                       *prpp),
                      nullptr);
      this->count_++;
    }

  return *prpp;
}
