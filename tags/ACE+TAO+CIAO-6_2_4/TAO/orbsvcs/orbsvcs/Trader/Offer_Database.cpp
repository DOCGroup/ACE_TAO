// $Id$

#ifndef TAO_OFFER_DATABASE_CPP
#define TAO_OFFER_DATABASE_CPP

#include "orbsvcs/Trader/Offer_Database.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template <class LOCK_TYPE>
TAO_Offer_Database<LOCK_TYPE>::TAO_Offer_Database (void)
{
}

template <class LOCK_TYPE>
TAO_Offer_Database<LOCK_TYPE>::~TAO_Offer_Database (void)
{
  ACE_WRITE_GUARD (LOCK_TYPE, ace_mon, this->db_lock_);

  for (typename Offer_Database::iterator type_iter (this->offer_db_);
       ! type_iter.done ();
       type_iter++)
    {
      Offer_Map_Entry* offer_map_entry = (*type_iter).int_id_;

      {
        // Artificial scope, so the guard will release the lock before
        // we delete the lock along with the offer_map_entry.
        ACE_WRITE_GUARD (LOCK_TYPE, ace_mon, offer_map_entry->lock_);

        for (TAO_Offer_Map::iterator offer_iter (*offer_map_entry->offer_map_);
             ! offer_iter.done ();
             offer_iter++)
          {
            // Delete all the offers in the offer map.
            CosTrading::Offer* offer = (*offer_iter).int_id_;
            delete offer;
          }

        delete offer_map_entry->offer_map_;
      }

      delete offer_map_entry;
    }
}

template <class LOCK_TYPE> CosTrading::OfferId
TAO_Offer_Database<LOCK_TYPE>::
insert_offer (const char* type, CosTrading::Offer* offer)
{
  CosTrading::OfferId return_value = 0;
  typename Offer_Database::ENTRY* database_entry = 0;
  CORBA::String_var service_type (type);

  ACE_READ_GUARD_RETURN (LOCK_TYPE, ace_mon, this->db_lock_, 0);

  if (this->offer_db_.find (service_type, database_entry) == -1)
    {
      // If there's no entry for the service type, create one.

      Offer_Map_Entry* new_offer_map_entry = 0;
      ACE_NEW_RETURN (new_offer_map_entry, Offer_Map_Entry, 0);
      ACE_NEW_RETURN (new_offer_map_entry->offer_map_, TAO_Offer_Map, 0);
      new_offer_map_entry->counter_ = 1;

      if (this->db_lock_.release () == -1)
        return 0;
      else
        {
          // Add the new entry; upgrade lock.
          ACE_WRITE_GUARD_RETURN (LOCK_TYPE, ace_mon, this->db_lock_, 0);
          this->offer_db_.bind (service_type,
                                new_offer_map_entry,
                                database_entry);
        }

      if (this->db_lock_.acquire_read () == -1)
        return 0;
    }

  Offer_Map_Entry* offer_map_entry =
    database_entry->int_id_;
  ACE_WRITE_GUARD_RETURN (LOCK_TYPE, ace_mon2, offer_map_entry->lock_, 0);

  // Add the offer to the service offer table for this service type.
  offer_map_entry->offer_map_->bind (offer_map_entry->counter_,
                                     offer);
  return_value = this->generate_offer_id (type,
                                          offer_map_entry->counter_);
  offer_map_entry->counter_++;

  return return_value;
}

template <class LOCK_TYPE> int
TAO_Offer_Database<LOCK_TYPE>::
remove_offer (const char* type, CORBA::ULong id)
{
  ACE_READ_GUARD_RETURN (LOCK_TYPE, ace_mon, this->db_lock_, -1);

  int return_value = -1;
  typename Offer_Database::ENTRY* db_entry = 0;
  CORBA::String_var service_type (type);

  if (this->offer_db_.find (service_type, db_entry) == 0)
    {
      CosTrading::Offer* offer = 0;
      Offer_Map_Entry* offer_map_entry = db_entry->int_id_;

      if (offer_map_entry->lock_.acquire_write () == -1)
        return -1;

      return_value = offer_map_entry->offer_map_->unbind (id, offer);
      delete offer;

      // If the service type has no more offers, free the map, lest
      // the memory forever persist.
      if (offer_map_entry->offer_map_->current_size () == 0)
        {
          if (this->db_lock_.release () == -1)
            return -1;
          else
            {
              // Promote the database lock. Will be released by
              // guard.
              if (this->db_lock_.acquire_write () == -1)
                return -1;

              // Unbind the service type from the database.
              this->offer_db_.unbind (service_type);

              // Now that the type has been removed, we can release
              // its lock.
              if (offer_map_entry->lock_.release () == -1)
                return -1;

              // Delete the database resources for this type.
              delete offer_map_entry->offer_map_;
              delete offer_map_entry;
            }
        }
      else if (offer_map_entry->lock_.release () == -1)
        return -1;
    }

  return return_value;
}

template <class LOCK_TYPE> int
TAO_Offer_Database<LOCK_TYPE>::
remove_offer (const CosTrading::OfferId offer_id)
{
  char* stype = 0;
  CORBA::ULong index;

  this->parse_offer_id (offer_id,
                        stype,
                        index);

  if (this->remove_offer (stype,
                          index) == -1)
    throw CosTrading::UnknownOfferId (offer_id);

  return 0;
}

template <class LOCK_TYPE> CosTrading::Offer*
TAO_Offer_Database<LOCK_TYPE>::
lookup_offer (const CosTrading::OfferId offer_id,
              char*& type_name)
{
  CORBA::ULong index;
  CosTrading::Offer* offer = 0;
  this->parse_offer_id (offer_id, type_name, index);

  if ((offer = this->lookup_offer (type_name, index)) == 0)
    throw CosTrading::UnknownOfferId (offer_id);

  return offer;
}


template <class LOCK_TYPE> CosTrading::Offer*
TAO_Offer_Database<LOCK_TYPE>::
lookup_offer (const CosTrading::OfferId offer_id)
{
  char* type_name;
  CORBA::ULong index;
  CosTrading::Offer* offer = 0;

  this->parse_offer_id (offer_id, type_name, index);

  if ((offer = this->lookup_offer (type_name, index)) == 0)
    throw CosTrading::UnknownOfferId (offer_id);

  return offer;
}

template <class LOCK_TYPE> CosTrading::Offer*
TAO_Offer_Database<LOCK_TYPE>::
lookup_offer (const char* type, CORBA::ULong id)
{
  ACE_READ_GUARD_RETURN (LOCK_TYPE, ace_mon, this->db_lock_, 0);

  CosTrading::Offer* return_value = 0;
  typename Offer_Database::ENTRY* db_entry = 0;
  CORBA::String_var service_type (type);

  if (this->offer_db_.find (service_type, db_entry) == 0)
    {
      Offer_Map_Entry* offer_map_entry = db_entry->int_id_;
      ACE_READ_GUARD_RETURN (LOCK_TYPE, ace_mon, offer_map_entry->lock_, 0);

      TAO_Offer_Map::ENTRY* offer_entry_ptr = 0;
      if (offer_map_entry->offer_map_->find (id, offer_entry_ptr) == 0)
        return_value = offer_entry_ptr->int_id_;
    }

  return return_value;
}

template <class LOCK_TYPE> TAO_Offer_Id_Iterator*
TAO_Offer_Database<LOCK_TYPE>::retrieve_all_offer_ids (void)
{
  ACE_READ_GUARD_RETURN (LOCK_TYPE, ace_mon, this->db_lock_, 0);

  // Fill an TAO_Offer_Id_Iterator with the offer_ids of all offers
  // exported to the TAO_Offer_Database. Iterates through the entire
  // map, cramming offer_id strings into a newly constructed
  // TAO_Offer_Id_Iterator.
  TAO_Offer_Id_Iterator* id_iterator;
  ACE_NEW_RETURN (id_iterator,
                  TAO_Offer_Id_Iterator (),
                  0);

  for (typename Offer_Database::iterator type_iter (this->offer_db_);
       ! type_iter.done ();
       type_iter++)
    {
      const char* type_name = (*type_iter).ext_id_.in ();
      Offer_Map_Entry* offer_map_entry = (*type_iter).int_id_;

      ACE_READ_GUARD_RETURN (LOCK_TYPE, ace_mon, offer_map_entry->lock_, 0);

      for (TAO_Offer_Map::iterator offer_iter (*offer_map_entry->offer_map_);
           ! offer_iter.done ();
           offer_iter++)
        {
          CORBA::ULong offer_index = (*offer_iter).ext_id_;
          CosTrading::OfferId offer_id =
            this->generate_offer_id (type_name, offer_index);

          id_iterator->insert_id (offer_id);
        }
    }

  return id_iterator;
}

template <class LOCK_TYPE> void
TAO_Offer_Database<LOCK_TYPE>::
parse_offer_id (const CosTrading::OfferId offer_id,
                char*&service_type,
                CORBA::ULong& id)
{
  // Get service type: it is everything from 17th character to the end.
  service_type = (char *) offer_id + 16;

  // Get id: temporarily put the end of string character where the service
  // type starts, convert to number, replace the character back.
  char* start_of_type =  (char *) (offer_id + 16);
  char temp = *start_of_type;
  *start_of_type = '\0';
  id = ACE_OS::atoi (offer_id);

  *start_of_type = temp;

  if (! TAO_Trader_Base::is_valid_identifier_name (service_type))
    throw CosTrading::IllegalOfferId (offer_id);
}

template <class LOCK_TYPE> CosTrading::OfferId
TAO_Offer_Database<LOCK_TYPE>::
generate_offer_id (const char *service_type_name, CORBA::ULong id)
{
  // holds final id string.
  size_t total_size = 16 + ACE_OS::strlen (service_type_name);

  // hold portions of id string.
  CosTrading::OfferId offer_id =
     CORBA::string_alloc (static_cast<CORBA::ULong> (total_size));
  char* return_value = 0;
  ACE_OS::sprintf (offer_id, "%016u%s", id, service_type_name);

  return_value = CORBA::string_dup (offer_id);
  delete [] offer_id;

  return return_value;
}


template <class LOCK_TYPE>
TAO_Service_Offer_Iterator<LOCK_TYPE>::
TAO_Service_Offer_Iterator (const char* type,
                            TAO_Offer_Database<LOCK_TYPE>& offer_database)
  : stm_ (offer_database),
    lock_ (0),
    offer_iter_ (0),
    type_ (type)
{
  CORBA::String_var service_type (type);

  if (this->stm_.db_lock_.acquire_read () == -1)
    return;

  typename TAO_Offer_Database<LOCK_TYPE>::Offer_Map_Entry* entry = 0;
  if (this->stm_.offer_db_.find (service_type, entry) == -1)
    return;
  else
    {
      this->lock_ = &entry->lock_;
      if (this->lock_->acquire_read () == -1)
        return;

      ACE_NEW (offer_iter_,
               TAO_Offer_Map::iterator (*entry->offer_map_));
    }
}

template <class LOCK_TYPE>
TAO_Service_Offer_Iterator<LOCK_TYPE>::~TAO_Service_Offer_Iterator (void)
{
  this->stm_.db_lock_.release ();

  if (this->lock_ != 0)
    {
      this->lock_->release ();
      delete this->offer_iter_;
    }
}

template <class LOCK_TYPE> CosTrading::OfferId
TAO_Service_Offer_Iterator<LOCK_TYPE>::get_id (void)
{
  return (this->offer_iter_ != 0)
    ? TAO_Offer_Database<LOCK_TYPE>::generate_offer_id (this->type_, (**this->offer_iter_).ext_id_)
    : 0;
}

template <class LOCK_TYPE> int
TAO_Service_Offer_Iterator<LOCK_TYPE>::has_more_offers (void)
{
  return (this->offer_iter_ != 0) ? ! this->offer_iter_->done () : 0;
}


template <class LOCK_TYPE> CosTrading::Offer*
TAO_Service_Offer_Iterator<LOCK_TYPE>::get_offer (void)
{
  return (this->offer_iter_ != 0) ? (**this->offer_iter_).int_id_ : 0;
}

template <class LOCK_TYPE> void
TAO_Service_Offer_Iterator<LOCK_TYPE>::next_offer (void)
{
  if (this->offer_iter_ != 0)
    this->offer_iter_->advance ();
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_OFFER_DATABASE_CPP */
