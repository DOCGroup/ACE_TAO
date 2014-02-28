// $Id$

#include "tao/ObjectKey_Table.h"
#include "tao/ORB_Core.h"
#include "tao/Refcounted_ObjectKey.h"

#if !defined (__ACE_INLINE__)
# include "tao/ObjectKey_Table.inl"
#endif /* ! __ACE_INLINE__ */

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
  : table_ ()
{

}

TAO::ObjectKey_Table::~ObjectKey_Table (void)
{
  this->table_.close ();
}

int
TAO::ObjectKey_Table::destroy (void)
{
  if (this->table_.current_size ())
    {
      ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                        ace_mon,
                        this->lock_,
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
