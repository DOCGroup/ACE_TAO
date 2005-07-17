#include "orbsvcs/Log/Hash_LogStore.h"
#include "orbsvcs/Log/Hash_LogRecordStore.h"
#include "orbsvcs/Log/LogMgr_i.h"

ACE_RCSID (Log,
           Hash_LogStore,
           "$Id$")

TAO_Hash_LogStore::TAO_Hash_LogStore(CORBA::ORB_ptr orb, TAO_LogMgr_i* mgr)
  : next_id_ (0),
    orb_ (CORBA::ORB::_duplicate (orb)),
    mgr_ (mgr)
{
}


TAO_Hash_LogStore::~TAO_Hash_LogStore()
{
}


DsLogAdmin::LogList*
TAO_Hash_LogStore::list_logs (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  ACE_READ_GUARD_THROW_EX (ACE_RW_Thread_Mutex,
                           guard,
                           lock_,
                           CORBA::INTERNAL ());
  ACE_CHECK_RETURN (0);

  DsLogAdmin::LogList* list;

  // Figure out the length of the list.
  CORBA::ULong len = static_cast<CORBA::ULong> (hash_map_.current_size ());

  // Allocate the list of <len> length.
  ACE_NEW_THROW_EX (list,
                    DsLogAdmin::LogList (len),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  list->length (len);

  // Create an iterator
  HASHMAP::ITERATOR iter (hash_map_);

  // Iterate over and populate the list.
  HASHMAP::ENTRY *hash_entry = 0;

  for (CORBA::ULong i = 0; i < len; i++)
    {
      iter.next (hash_entry);
      iter.advance ();
      (*list)[i] = mgr_->create_log_reference (static_cast<DsLogAdmin::LogId> (hash_entry->ext_id_)
                                               ACE_ENV_ARG_PARAMETER);
    }

  return list;
}

DsLogAdmin::LogIdList*
TAO_Hash_LogStore::list_logs_by_id (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  ACE_READ_GUARD_THROW_EX (ACE_RW_Thread_Mutex,
                           guard,
                           lock_,
                           CORBA::INTERNAL ());
  ACE_CHECK_RETURN (0);

  DsLogAdmin::LogIdList* list;

  // Figure out the length of the list.
  CORBA::ULong len = static_cast<CORBA::ULong> (hash_map_.current_size ());

  // Allocate the list of <len> length.
  ACE_NEW_THROW_EX (list,
                    DsLogAdmin::LogIdList (len),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

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
TAO_Hash_LogStore::find_log (DsLogAdmin::LogId id
                        ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  ACE_READ_GUARD_THROW_EX (ACE_RW_Thread_Mutex,
                           guard,
                           lock_,
                           CORBA::INTERNAL ());
  ACE_CHECK (DsLogAdmin::Log::_nil ());

  if (hash_map_.find (id) != 0)
    {
      return DsLogAdmin::Log::_nil ();
    }
  else
    {
      return mgr_->create_log_reference (id ACE_ENV_ARG_PARAMETER);
    }
}


bool
TAO_Hash_LogStore::exists (DsLogAdmin::LogId id)
{
  ACE_READ_GUARD_THROW_EX (ACE_RW_Thread_Mutex,
                           guard,
                           lock_,
                           CORBA::INTERNAL ());
  ACE_CHECK_RETURN (false);

  return (this->hash_map_.find (id) == 0);
}


int
TAO_Hash_LogStore::remove (DsLogAdmin::LogId id)
{
  ACE_WRITE_GUARD_THROW_EX (ACE_RW_Thread_Mutex,
                            guard,
                            lock_,
                            CORBA::INTERNAL ());
  ACE_CHECK_RETURN (-1);

  return this->hash_map_.unbind (id);
}


void
TAO_Hash_LogStore::create(DsLogAdmin::LogFullActionType full_action,
                                      CORBA::ULongLong max_size,
                                      const DsLogAdmin::CapacityAlarmThresholdList* thresholds,
                                      DsLogAdmin::LogId_out id_out
                                      ACE_ENV_ARG_DECL)
{
  ACE_WRITE_GUARD_THROW_EX (ACE_RW_Thread_Mutex,
                            guard,
                            lock_,
                            CORBA::INTERNAL ());
  ACE_CHECK;

  DsLogAdmin::LogId id;

  while (this->hash_map_.find((id = this->next_id_++)) == 0)
    ;
  id_out = id;

  TAO_Hash_LogRecordStore* impl;
  ACE_NEW_THROW_EX (impl,
                    TAO_Hash_LogRecordStore (this->orb_,
                                             id,
                                             full_action,
                                             max_size,
                                             thresholds
                                             ),
                    CORBA::NO_MEMORY ());
  ACE_CHECK;

  auto_ptr<TAO_Hash_LogRecordStore> recordstore (impl);

  if (this->hash_map_.bind (id, recordstore.get ()) != 0)
    {
      ACE_THROW (CORBA::INTERNAL ());
    }

  recordstore.release ();
}


void
TAO_Hash_LogStore::create_with_id (DsLogAdmin::LogId id,
                                               DsLogAdmin::LogFullActionType full_action,
                                               CORBA::ULongLong max_size,
                                               const DsLogAdmin::CapacityAlarmThresholdList* thresholds
                                               ACE_ENV_ARG_DECL)
{
  ACE_WRITE_GUARD_THROW_EX (ACE_RW_Thread_Mutex,
                            guard,
                            lock_,
                            CORBA::INTERNAL ());
  ACE_CHECK;

  if (this->hash_map_.find (id) == 0)
    {
      ACE_THROW (DsLogAdmin::LogIdAlreadyExists ());
    }

  TAO_Hash_LogRecordStore* impl;
  ACE_NEW_THROW_EX (impl,
                    TAO_Hash_LogRecordStore (this->orb_,
                                             id,
                                             full_action,
                                             max_size,
                                             thresholds
                                             ),
                    CORBA::NO_MEMORY ());
  ACE_CHECK;

  auto_ptr<TAO_Hash_LogRecordStore> recordstore (impl);

  if (this->hash_map_.bind (id, recordstore.get ()) != 0)
    {
      ACE_THROW (CORBA::INTERNAL ());
    }

  recordstore.release ();
}


TAO_LogRecordStore*
TAO_Hash_LogStore::get_log_record_store (DsLogAdmin::LogId id
                                         ACE_ENV_ARG_DECL)
{
  TAO_Hash_LogRecordStore* recordstore;

  if (hash_map_.find (id, recordstore) != 0)
    {
      return 0;
    }

  return recordstore;
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Hash_Map_Entry<DsLogAdmin::LogId,DsLogAdmin::Log_var>;
template class ACE_Hash_Map_Manager<DsLogAdmin::LogId,DsLogAdmin::Log_var,ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<DsLogAdmin::LogId, DsLogAdmin::Log_var, ACE_Hash<DsLogAdmin::LogId>, ACE_Equal_To<DsLogAdmin::LogId>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator<DsLogAdmin::LogId,DsLogAdmin::Log_var,ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<DsLogAdmin::LogId, DsLogAdmin::Log_var, ACE_Hash<DsLogAdmin::LogId>, ACE_Equal_To<DsLogAdmin::LogId>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<DsLogAdmin::LogId, DsLogAdmin::Log_var, ACE_Hash<DsLogAdmin::LogId>, ACE_Equal_To<DsLogAdmin::LogId>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator<DsLogAdmin::LogId,DsLogAdmin::Log_var,ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<DsLogAdmin::LogId, DsLogAdmin::Log_var, ACE_Hash<DsLogAdmin::LogId>, ACE_Equal_To<DsLogAdmin::LogId>, ACE_Null_Mutex>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Hash_Map_Entry<DsLogAdmin::LogId,DsLogAdmin::Log_var>
#pragma instantiate ACE_Hash_Map_Manager<DsLogAdmin::LogId,DsLogAdmin::Log_var,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager_Ex<DsLogAdmin::LogId, DsLogAdmin::Log_var, ACE_Hash<DsLogAdmin::LogId>, ACE_Equal_To<DsLogAdmin::LogId>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator<DsLogAdmin::LogId,DsLogAdmin::Log_var,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<DsLogAdmin::LogId, DsLogAdmin::Log_var, ACE_Hash<DsLogAdmin::LogId>, ACE_Equal_To<DsLogAdmin::LogId>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<DsLogAdmin::LogId, DsLogAdmin::Log_var, ACE_Hash<DsLogAdmin::LogId>, ACE_Equal_To<DsLogAdmin::LogId>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<DsLogAdmin::LogId,DsLogAdmin::Log_var,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<DsLogAdmin::LogId, DsLogAdmin::Log_var, ACE_Hash<DsLogAdmin::LogId>, ACE_Equal_To<DsLogAdmin::LogId>, ACE_Null_Mutex>

#endif /* ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA */
