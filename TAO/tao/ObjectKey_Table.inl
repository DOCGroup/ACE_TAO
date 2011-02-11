// $Id$

#include "tao/Refcounted_ObjectKey.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
int
TAO::ObjectKey_Table::bind (const TAO::ObjectKey &key,
                            TAO::Refcounted_ObjectKey *&key_new)

{
  key_new = 0;

  int retval = 0;

  {
    ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                      ace_mon,
                      this->lock_,
                      0);

    // This is a tradeoff.. We could avoid this two stage process of
    // using a find () and then a bind () , which would make things
    // efficient. BUT we may have to do allocation upfront and delete if
    // bind () returns with an entry. We take one of the routes that
    // avoids allocation.
    retval = this->table_.find (key, key_new);

    if (retval == -1)
      {
        return this->bind_i (key, key_new);
      }

    (void) key_new->incr_refcount ();
  }

  return retval;
}

ACE_INLINE
int
TAO::ObjectKey_Table::unbind (TAO::Refcounted_ObjectKey *&key_new)

{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                    ace_mon,
                    this->lock_,
                    0);

  // If the refcount has dropped to 1, just go ahead and unbind it
  // from the table.
  if (key_new && key_new->decr_refcount () == 1)
    {
      return this->unbind_i (key_new);
    }

  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
