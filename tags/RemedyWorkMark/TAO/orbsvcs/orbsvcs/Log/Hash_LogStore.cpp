// $Id$

#include "orbsvcs/Log/Hash_LogStore.h"
#include "orbsvcs/Log/Hash_LogRecordStore.h"
#include "orbsvcs/Log/LogMgr_i.h"
#include "ace/Auto_Ptr.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Hash_LogStore::TAO_Hash_LogStore(TAO_LogMgr_i* logmgr_i)
  : next_id_ (0),
    logmgr_i_ (logmgr_i)
{
}


TAO_Hash_LogStore::~TAO_Hash_LogStore()
{
}


DsLogAdmin::LogList*
TAO_Hash_LogStore::list_logs (void)
{
  ACE_READ_GUARD_THROW_EX (ACE_SYNCH_RW_MUTEX,
                           guard,
                           lock_,
                           CORBA::INTERNAL ());

  DsLogAdmin::LogList* list;

  // Figure out the length of the list.
  CORBA::ULong len = static_cast<CORBA::ULong> (hash_map_.current_size ());

  // Allocate the list of <len> length.
  ACE_NEW_THROW_EX (list,
                    DsLogAdmin::LogList (len),
                    CORBA::NO_MEMORY ());

  list->length (len);

  // Create an iterator
  HASHMAP::ITERATOR iter (hash_map_);

  // Iterate over and populate the list.
  HASHMAP::ENTRY *hash_entry = 0;

  for (CORBA::ULong i = 0; i < len; i++)
    {
      iter.next (hash_entry);
      iter.advance ();
      (*list)[i] = logmgr_i_->create_log_reference (static_cast<DsLogAdmin::LogId> (hash_entry->ext_id_));
    }

  return list;
}

DsLogAdmin::LogIdList*
TAO_Hash_LogStore::list_logs_by_id (void)
{
  ACE_READ_GUARD_THROW_EX (ACE_SYNCH_RW_MUTEX,
                           guard,
                           lock_,
                           CORBA::INTERNAL ());

  DsLogAdmin::LogIdList* list;

  // Figure out the length of the list.
  CORBA::ULong len = static_cast<CORBA::ULong> (hash_map_.current_size ());

  // Allocate the list of <len> length.
  ACE_NEW_THROW_EX (list,
                    DsLogAdmin::LogIdList (len),
                    CORBA::NO_MEMORY ());

  list->length (len);

  // Create an iterator
  HASHMAP::ITERATOR iter (hash_map_);

  // Iterate over and populate the list.
  HASHMAP::ENTRY *hash_entry = 0;

  for (CORBA::ULong i = 0; i < len; i++)
    {
      iter.next (hash_entry);
      iter.advance ();
      (*list)[i] = hash_entry->ext_id_;
    }

  return list;
}


DsLogAdmin::Log_ptr
TAO_Hash_LogStore::find_log (DsLogAdmin::LogId id)
{
  ACE_READ_GUARD_THROW_EX (ACE_SYNCH_RW_MUTEX,
                           guard,
                           lock_,
                           CORBA::INTERNAL ());

  if (hash_map_.find (id) != 0)
    {
      return DsLogAdmin::Log::_nil ();
    }
  else
    {
      return logmgr_i_->create_log_reference (id);
    }
}


bool
TAO_Hash_LogStore::exists (DsLogAdmin::LogId id)
{
  ACE_READ_GUARD_THROW_EX (ACE_SYNCH_RW_MUTEX,
                           guard,
                           lock_,
                           CORBA::INTERNAL ());

  return (this->hash_map_.find (id) == 0);
}


int
TAO_Hash_LogStore::remove (DsLogAdmin::LogId id)
{
  ACE_WRITE_GUARD_THROW_EX (ACE_SYNCH_RW_MUTEX,
                            guard,
                            lock_,
                            CORBA::INTERNAL ());

  TAO_Hash_LogRecordStore* recordstore;

  int retval = this->hash_map_.unbind (id, recordstore);
  if (retval == 0)
    {
      delete recordstore;
    }

  return retval;
}


void
TAO_Hash_LogStore::create(DsLogAdmin::LogFullActionType full_action,
                                      CORBA::ULongLong max_size,
                                      const DsLogAdmin::CapacityAlarmThresholdList* thresholds,
                                      DsLogAdmin::LogId_out id_out)
{
  ACE_WRITE_GUARD_THROW_EX (ACE_SYNCH_RW_MUTEX,
                            guard,
                            lock_,
                            CORBA::INTERNAL ());

  DsLogAdmin::LogId id;

  while (this->hash_map_.find((id = this->next_id_++)) == 0)
    ;
  id_out = id;

  TAO_Hash_LogRecordStore* impl = 0;
  ACE_NEW_THROW_EX (impl,
                    TAO_Hash_LogRecordStore (this->logmgr_i_,
                                             id,
                                             full_action,
                                             max_size,
                                             thresholds
                                             ),
                    CORBA::NO_MEMORY ());

  auto_ptr<TAO_Hash_LogRecordStore> recordstore (impl);

  if (this->hash_map_.bind (id, recordstore.get ()) != 0)
    {
      throw CORBA::INTERNAL ();
    }

  recordstore.release ();
}


void
TAO_Hash_LogStore::create_with_id (DsLogAdmin::LogId id,
                                               DsLogAdmin::LogFullActionType full_action,
                                               CORBA::ULongLong max_size,
                                               const DsLogAdmin::CapacityAlarmThresholdList* thresholds)
{
  ACE_WRITE_GUARD_THROW_EX (ACE_SYNCH_RW_MUTEX,
                            guard,
                            lock_,
                            CORBA::INTERNAL ());

  if (this->hash_map_.find (id) == 0)
    {
      throw DsLogAdmin::LogIdAlreadyExists ();
    }

  TAO_Hash_LogRecordStore* impl = 0;
  ACE_NEW_THROW_EX (impl,
                    TAO_Hash_LogRecordStore (this->logmgr_i_,
                                             id,
                                             full_action,
                                             max_size,
                                             thresholds
                                             ),
                    CORBA::NO_MEMORY ());

  auto_ptr<TAO_Hash_LogRecordStore> recordstore (impl);

  if (this->hash_map_.bind (id, recordstore.get ()) != 0)
    {
      throw CORBA::INTERNAL ();
    }

  recordstore.release ();
}


TAO_LogRecordStore*
TAO_Hash_LogStore::get_log_record_store (DsLogAdmin::LogId id)
{
  ACE_READ_GUARD_THROW_EX (ACE_SYNCH_RW_MUTEX,
                           guard,
                           lock_,
                           CORBA::INTERNAL ());

  TAO_Hash_LogRecordStore* recordstore = 0;

  if (hash_map_.find (id, recordstore) != 0)
    {
      return 0;
    }

  return recordstore;
}

TAO_END_VERSIONED_NAMESPACE_DECL
