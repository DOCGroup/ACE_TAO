#include "ORB_Table.h"
#include "ORB_Core.h"
#include "TAO_Singleton.h"

#if !defined (__ACE_INLINE__)
# include "ORB_Table.inl"
#endif /* ! __ACE_INLINE__ */

#include "ace/OS_NS_string.h"


ACE_RCSID (tao,
           ORB_Table,
           "$Id$")


// ****************************************************************

TAO_ORB_Table::TAO_ORB_Table (void)
  : first_orb_not_default_ (0),
    table_ (TAO_DEFAULT_ORB_TABLE_SIZE),
    first_orb_ (0),
    orbs_ (0),
    num_orbs_ (0)
{
}

TAO_ORB_Table::~TAO_ORB_Table (void)
{
  const Iterator end = this->end ();
  for (Iterator i = this->begin (); i != end; ++i)
    {
      // Deallocate the ORBid.
      CORBA::string_free (const_cast<char *> ((*i).ext_id_));

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

TAO_ORB_Core* const *
TAO_ORB_Table::get_orbs (size_t& num_orbs)
{
  num_orbs = this->num_orbs_;
  return this->orbs_;
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

      // This is not the first ORB .. but if the current default
      // ORB decided not to be the default and there is more than
      // one orb then set this orb to be the default ORB.
      if ((this->first_orb_ != 0)
          && (this->first_orb_not_default_))
        {
          this->first_orb_ = orb_core;
          this->first_orb_not_default_ = 0;
        }

      // Set the "first_orb_" member for the first given ORB Core
      // that was successfully added to the ORB table.
      if (this->first_orb_ == 0)
        {
          this->first_orb_ = orb_core;
        }

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
      CORBA::string_free (const_cast<char *> (entry->ext_id_));
      TAO_ORB_Core *orb_core = entry->int_id_;

      result = this->table_.unbind (entry);

      if (result != 0)
        {
          return result;
        }

      if (orb_core == this->first_orb_)
        {
          Iterator begin = this->begin ();
          Iterator end = this->end ();

          if (begin != end)
            {
              this->first_orb_ = (*begin).int_id_;
            }
          else
            {
              this->first_orb_ = 0;
            }
        }

      orb_core->_decr_refcnt ();
    }

  return result;
}

void
TAO_ORB_Table::set_default (const char *orb_id)
{
  this->table_.find (orb_id, this->first_orb_);
}

void
TAO_ORB_Table::not_default (const char *orb_id)
{
  // @@  This method now works for restricted cases. Should work on
  // generalizing it. It works if the first ORB that is registered
  // decides to not want be the default ORB. Should generalize it to
  // handle all cases.

  //check if there is a default ORB already and if
  // it is *not* the same as the orb_id thats passed in.. we dont have
  // to do anything.
  if (this->first_orb_ != 0)
    {
      if (ACE_OS::strcmp (this->first_orb_->orbid (), orb_id) != 0)
        {
          // There is another default ORB. No need to change anything
          return;
        }
      else
        {
          // The ORB with orbid 'orb_id' is the default now. We need
          // to change it.
          this->first_orb_not_default_ = 1;
        }
    }
}

/// Accessor to the underlying table_
ACE_Hash_Map_Manager_Ex<const char *,TAO_ORB_Core *,ACE_Hash<const char *>,ACE_Equal_To<const char *>,ACE_Null_Mutex> *
TAO_ORB_Table::table (void)
{
  return &this->table_;
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

#elif defined (ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION)

template TAO_Singleton<TAO_ORB_Table,TAO_SYNCH_MUTEX> * TAO_Singleton<TAO_ORB_Table,TAO_SYNCH_MUTEX>::singleton_;

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
