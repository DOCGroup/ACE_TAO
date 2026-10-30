#include "tao/ObjectKey_Table.h"
#include "tao/ORB_Core.h"
#include "tao/Refcounted_ObjectKey.h"

#if !defined (__ACE_INLINE__)
# include "tao/ObjectKey_Table.inl"
#endif /* ! __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

bool
TAO::Less_Than_ObjectKey::operator () (TAO::ObjectKey const &lhs,
                                       TAO::ObjectKey const &rhs) const
{
  CORBA::ULong const rlen = rhs.length ();
  CORBA::ULong const llen = lhs.length ();
  if (llen < rlen)
    {
      return true;
    }
  else if (llen > rlen)
    {
      return false;
    }

  CORBA::Octet const * const rhs_buff = rhs.get_buffer ();
  CORBA::Octet const * const lhs_buff = lhs.get_buffer ();
  bool const result = (ACE_OS::memcmp (lhs_buff, rhs_buff, rlen) < 0);

  return result;
}

/********************************************************/
TAO::ObjectKey_Table::~ObjectKey_Table ()
{
  this->table_.close ();
}

int
TAO::ObjectKey_Table::destroy ()
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
TAO::ObjectKey_Table::bind_i (TAO::ObjectKey const &key,
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
  TAO::Refcounted_ObjectKey *tmp = nullptr;

  if (this->table_.unbind (key_new->object_key (), tmp) != -1)
    {
      // @@ Cant do much if the unbind fails.
      // Remove our refcount on the ObjectKey
      (void) tmp->decr_refcount ();
    }

  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
