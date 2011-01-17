// $Id$

#include "tao/ObjectKey_Table.h"
#include "tao/ORB_Core.h"
#include "tao/Refcounted_ObjectKey.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

bool
TAO::Less_Than_ObjectKey::operator () (const TAO::ObjectKey &lhs,
                                       const TAO::ObjectKey &rhs) const
{
  const CORBA::ULong rlen = rhs.length ();
  const CORBA::ULong llen = lhs.length ();
  if (llen < rlen)
    {
      return 1;
    }
  else if (llen > rlen)
    {
      return 0;
    }

  const CORBA::Octet * rhs_buff = rhs.get_buffer ();
  const CORBA::Octet * lhs_buff = lhs.get_buffer ();
  const bool result = (ACE_OS::memcmp (lhs_buff, rhs_buff, rlen) < 0);

  return result;
}

/********************************************************/
TAO::ObjectKey_Table::ObjectKey_Table (void)
  : lock_ (0)
  , table_ ()
{

}

TAO::ObjectKey_Table::~ObjectKey_Table (void)
{
  this->table_.close ();
  delete this->lock_;
}

int
TAO::ObjectKey_Table::init (TAO_ORB_Core *oc)
{
  /// Create the lock that is needed for internal usage.
  this->lock_ =
    oc->resource_factory ()->create_object_key_table_lock ();

  return 0;
}

int
TAO::ObjectKey_Table::bind (const TAO::ObjectKey &key,
                            TAO::Refcounted_ObjectKey *&key_new)

{
  key_new = 0;

  int retval = 0;

  {
    ACE_GUARD_RETURN (ACE_Lock,
                      ace_mon,
                      *this->lock_,
                      0);

    // This is a tradeoff.. We could avoid this two stage process of
    // using a find () and then a bind () , which would make things
    // efficient. BUT we may have to do allocation upfront and delete if
    // bind () returns with an entry. We take one of the routes that
    // avoids allocation.
    retval = this->table_.find (key,
                                key_new);

    if (retval == -1)
      {
        return this->bind_i (key,
                             key_new);
      }

    (void) key_new->incr_refcount ();
  }

  return retval;
}

int
TAO::ObjectKey_Table::unbind (TAO::Refcounted_ObjectKey *&key_new)

{
  ACE_GUARD_RETURN (ACE_Lock,
                    ace_mon,
                    *this->lock_,
                    0);

  // If the refcount has dropped to 1, just go ahead and unbind it
  // from the table.
  if (key_new && key_new->decr_refcount () == 1)
    {
      return this->unbind_i (key_new);
    }

  return 0;
}

int
TAO::ObjectKey_Table::destroy (void)
{
  if (this->table_.current_size ())
    {
      ACE_GUARD_RETURN (ACE_Lock,
                        ace_mon,
                        *this->lock_,
                        0);

      TABLE::ITERATOR end_iter = this->table_.end ();
      TABLE::ITERATOR start;

      while ((start = this->table_.begin ()) != end_iter)
        {
          TABLE::ENTRY &ent = (*start);

          ent.item ()->decr_refcount ();
          this->table_.unbind (&ent);
        }
    }

  return 0;
}

int
TAO::ObjectKey_Table::bind_i (const TAO::ObjectKey &key,
                              TAO::Refcounted_ObjectKey *&key_new)
{
  ACE_NEW_RETURN (key_new,
                  TAO::Refcounted_ObjectKey (key),
                  -1);

  int const retval =  this->table_.bind (key, key_new);

  if (retval != -1)
    {
      key_new->incr_refcount ();
    }
  else
    {
      key_new->decr_refcount ();
    }

  return retval;
}

int
TAO::ObjectKey_Table::unbind_i (TAO::Refcounted_ObjectKey *&key_new)
{
  TAO::Refcounted_ObjectKey *tmp = 0;

  if (this->table_.unbind (key_new->object_key (), tmp) != -1)
    {
      // @@ Cant do much if the unbind fails.
      // Remove our refcount on the ObjectKey
      (void) tmp->decr_refcount ();
    }

  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
