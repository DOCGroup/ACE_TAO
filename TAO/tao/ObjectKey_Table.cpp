//$Id$
#include "ObjectKey_Table.h"
#include "ORB_Core.h"
#include "Refcounted_ObjectKey.h"

ACE_RCSID(tao,
          ObjectKey_Table,
          "$Id$")


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

  // NOTE:Place for optimization. This is real expensive..
  CORBA::String_var str;
  TAO::ObjectKey::encode_sequence_to_string (str.inout (),
                                             key);

  // Hash values can be tricky, but even then thy are useful at
  // times.
  u_long hash_val =
    ACE::hash_pjw (str.in ());

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
    retval = this->table_.find (hash_val,
                                key_new);

    if (retval == -1)
      return this->bind_i (hash_val,
                           key,
                           key_new);


    // Do a sanity comparison check and increment the refcount.
    // Place for optimization by removing the comparison.
    if (key_new->object_key () == key)
      {
        (void) key_new->incr_refcount ();
      }
    else
      {
        // Do we bind it again. And if so, how?
        // Need to thinkk....
      }
  }

  return 0;
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
  if (key_new->decr_refcount () == 1)
    return this->unbind_i (key_new);

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

      for (TABLE::ITERATOR start = this->table_.begin ();
           start != end_iter;
           ++start)
        {
          TABLE::ENTRY &ent = (*start);

          (void) ent.item ()->decr_refcount ();

          this->table_.unbind (&ent);
        }
    }

  return 0;
}

int
TAO::ObjectKey_Table::bind_i (u_long hash_val,
                              const TAO::ObjectKey &key,
                              TAO::Refcounted_ObjectKey *&key_new)
{
  ACE_NEW_RETURN (key_new,
                  TAO::Refcounted_ObjectKey (key),
                  -1);

  int retval =  this->table_.bind (hash_val,
                                   key_new);

  if (retval != -1)
    key_new->incr_refcount ();
  else
    key_new->decr_refcount ();

  return retval;
}

int
TAO::ObjectKey_Table::unbind_i (TAO::Refcounted_ObjectKey *&key_new)
{
  // NOTE:Place for optimization. This is real expensive..
  CORBA::String_var str;
  TAO::ObjectKey::encode_sequence_to_string (str.inout (),
                                             key_new->object_key ());

  u_long hash_val =
    ACE::hash_pjw (str.in ());

  (void) this->table_.unbind (hash_val,
                              key_new);

  // Remove our refcount on the ObjectKey
  (void) key_new->decr_refcount ();

  return 0;
}
