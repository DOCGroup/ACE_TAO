#include "ORB_Table.h"
#include "ORB_Core.h"
#include "TAO_Singleton.h"

#if !defined (__ACE_INLINE__)
# include "ORB_Table.inl"
#endif /* ! __ACE_INLINE__ */


ACE_RCSID (tao,
           ORB_Table,
           "$Id$")


// ****************************************************************

TAO_ORB_Table::TAO_ORB_Table (void)
  : table_ (TAO_DEFAULT_ORB_TABLE_SIZE),
    first_orb_ (0)
{
}

TAO_ORB_Table::~TAO_ORB_Table (void)
{
  for (Iterator i = this->begin ();
       i != this->end ();
       ++i)
    {
      // Deallocate the ORBid.
      CORBA::string_free (ACE_const_cast (char *, (*i).ext_id_));

      // Destroy the ORB_Core
      (void) (*i).int_id_->_decr_refcnt ();
    }

  this->table_.close ();
}

TAO_ORB_Table::Iterator
TAO_ORB_Table::begin (void)
{
  return this->table_.begin ();
}

TAO_ORB_Table::Iterator
TAO_ORB_Table::end (void)
{
  return this->table_.end ();
}

int
TAO_ORB_Table::bind (const char *orb_id,
                     TAO_ORB_Core *orb_core)
{
  // Make sure that the supplied ORB core pointer is valid,
  // i.e. non-zero.
  if (orb_id == 0 || orb_core == 0)
    {
      errno = EINVAL;
      return -1;
    };

  CORBA::String_var id = CORBA::string_dup (orb_id);

  int result = this->table_.bind (id.in (), orb_core);
  if (result == 0)
    {
      // Make sure the ORB table owns the ORB Core by increasing the
      // reference count on it.
      (void) orb_core->_incr_refcnt ();

      // Only set the "first_orb_" member if the given ORB Core was
      // successfully added to the ORB table.
      if (this->first_orb_ == 0)
        this->first_orb_ = orb_core;

      (void) id._retn ();  // ORB Table now owns the id.
    }

  return result;
}

TAO_ORB_Core *
TAO_ORB_Table::find (const char *orb_id)
{
  TAO_ORB_Core *found = 0;

  this->table_.find (orb_id, found);

  return found;
}

int
TAO_ORB_Table::unbind (const char *orb_id)
{
  Table::ENTRY *entry = 0;

  int result = this->table_.find (orb_id, entry);

  if (result == 0)
    {
      // Deallocate the external ID and obtain the ORB core pointer
      // before unbinding the entry since the entry is deallocated
      // during the call to unbind().
      CORBA::string_free (ACE_const_cast (char *, entry->ext_id_));
      TAO_ORB_Core *orb_core = entry->int_id_;

      result = this->table_.unbind (entry);

      if (result != 0)
        return result;

      if (orb_core == this->first_orb_)
        {
          Iterator begin = this->begin ();
          Iterator end = this->end ();
          if (begin != end)
            this->first_orb_ = (*begin).int_id_;
          else
            this->first_orb_ = 0;
        }

      orb_core->_decr_refcnt ();
    }

  return result;
}

TAO_ORB_Table *
TAO_ORB_Table::instance (void)
{
  return TAO_Singleton<TAO_ORB_Table, TAO_SYNCH_MUTEX>::instance ();
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class TAO_Singleton<TAO_ORB_Table,TAO_SYNCH_MUTEX>;

template class ACE_Hash_Map_Entry<const char *, TAO_ORB_Core *>;
template class ACE_Hash_Map_Manager_Ex<const char *, TAO_ORB_Core *, ACE_Hash<const char *>, ACE_Equal_To<const char *>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<const char *, TAO_ORB_Core *, ACE_Hash<const char *>, ACE_Equal_To<const char *>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<const char *, TAO_ORB_Core *, ACE_Hash<const char *>, ACE_Equal_To<const char *>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<const char *, TAO_ORB_Core *, ACE_Hash<const char *>, ACE_Equal_To<const char *>, ACE_Null_Mutex>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate TAO_Singleton<TAO_ORB_Table,TAO_SYNCH_MUTEX>

#pragma instantiate ACE_Hash_Map_Entry<const char *, TAO_ORB_Core *>
#pragma instantiate ACE_Hash_Map_Manager_Ex<const char *, TAO_ORB_Core *, ACE_Hash<const char *>, ACE_Equal_To<const char *>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<const char *, TAO_ORB_Core *, ACE_Hash<const char *>, ACE_Equal_To<const char *>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<const char *, TAO_ORB_Core *, ACE_Hash<const char *>, ACE_Equal_To<const char *>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<const char *, TAO_ORB_Core *, ACE_Hash<const char *>, ACE_Equal_To<const char *>, ACE_Null_Mutex>

#elif defined (__GNUC__) && (defined (_AIX) || defined (__hpux) || defined (VXWORKS))

template TAO_Singleton<TAO_ORB_Table,TAO_SYNCH_MUTEX> * TAO_Singleton<TAO_ORB_Table,TAO_SYNCH_MUTEX>::singleton_;

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
