#include "LogMgr_i.h"

ACE_RCSID (Log,
           LogMgr_i,
           "$Id$")

TAO_LogMgr_i::TAO_LogMgr_i (void)
{
  // No-Op.
}

TAO_LogMgr_i::~TAO_LogMgr_i ()
{
  // No-Op.
}

DsLogAdmin::LogList*
TAO_LogMgr_i::list_logs (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  DsLogAdmin::LogList* list;

  // Figure out the length of the list.
  CORBA::ULong len = hash_map_.current_size ();

  // Allocate the list of <len> length.
  ACE_NEW_THROW_EX (list,
                    DsLogAdmin::LogList (len),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  list->length (len);

  // Create an iterator
  HASHMAP::ITERATOR iter (hash_map_);

  // Iterate over and populate the list.
  HASHMAP::ENTRY *hash_entry;

  for (CORBA::ULong i = 0; i < len; i++)
    {
      iter.next (hash_entry);
      iter.advance ();
      (*list)[i] =
        DsLogAdmin::Log::_duplicate (hash_entry->int_id_.in ());
    }

  return list;
}

DsLogAdmin::Log_ptr
TAO_LogMgr_i::find_log (DsLogAdmin::LogId id
                        ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  DsLogAdmin::Log_var v_return;

  if (hash_map_.find (id,
                      v_return) == -1)
    {
      return DsLogAdmin::Log::_nil ();
    }
  else
    {
      // Note: We have a _var in the hash table. when the hash table goes
      // out of scope its contained _var members will release the object
      // references that they hold.
      // Now, <find> fills in <v_return> and when <v_return> goes out
      // of scope, it too will release the object ref.we don't want this.
      // we want the ref. count to remain what it was.
      // So we increment the ref. count here.
      DsLogAdmin::Log::_duplicate (v_return.in ());
      return DsLogAdmin::Log::_duplicate (v_return.in ());
      // This duplicate is to obey rules of returning obj. refs.
      // don't get confused!
    }
}

DsLogAdmin::LogIdList*
TAO_LogMgr_i::list_logs_by_id (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  DsLogAdmin::LogIdList* ret_val;
  ACE_NEW_THROW_EX (ret_val,
    DsLogAdmin::LogIdList (this->logid_list_),
                     CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  return ret_val;
}

int
TAO_LogMgr_i::remove (DsLogAdmin::LogId id)
{
  return this->hash_map_.unbind (id);
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Hash_Map_Entry<DsLogAdmin::LogId,DsLogAdmin::Log_var>;
template class ACE_Hash_Map_Manager<DsLogAdmin::LogId,DsLogAdmin::Log_var,TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Manager_Ex<DsLogAdmin::LogId, DsLogAdmin::Log_var, ACE_Hash<DsLogAdmin::LogId>, ACE_Equal_To<DsLogAdmin::LogId>, TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Iterator<DsLogAdmin::LogId,DsLogAdmin::Log_var,TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Iterator_Ex<DsLogAdmin::LogId, DsLogAdmin::Log_var, ACE_Hash<DsLogAdmin::LogId>, ACE_Equal_To<DsLogAdmin::LogId>, TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Iterator_Base_Ex<DsLogAdmin::LogId, DsLogAdmin::Log_var, ACE_Hash<DsLogAdmin::LogId>, ACE_Equal_To<DsLogAdmin::LogId>, TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Reverse_Iterator<DsLogAdmin::LogId,DsLogAdmin::Log_var,TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<DsLogAdmin::LogId, DsLogAdmin::Log_var, ACE_Hash<DsLogAdmin::LogId>, ACE_Equal_To<DsLogAdmin::LogId>, TAO_SYNCH_MUTEX>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Hash_Map_Entry<DsLogAdmin::LogId,DsLogAdmin::Log_var>
#pragma instantiate ACE_Hash_Map_Manager<DsLogAdmin::LogId,DsLogAdmin::Log_var,TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Manager_Ex<DsLogAdmin::LogId, DsLogAdmin::Log_var, ACE_Hash<DsLogAdmin::LogId>, ACE_Equal_To<DsLogAdmin::LogId>, TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator<DsLogAdmin::LogId,DsLogAdmin::Log_var,TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<DsLogAdmin::LogId, DsLogAdmin::Log_var, ACE_Hash<DsLogAdmin::LogId>, ACE_Equal_To<DsLogAdmin::LogId>, TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<DsLogAdmin::LogId, DsLogAdmin::Log_var, ACE_Hash<DsLogAdmin::LogId>, ACE_Equal_To<DsLogAdmin::LogId>, TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<DsLogAdmin::LogId,DsLogAdmin::Log_var,TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<DsLogAdmin::LogId, DsLogAdmin::Log_var, ACE_Hash<DsLogAdmin::LogId>, ACE_Equal_To<DsLogAdmin::LogId>, TAO_SYNCH_MUTEX>

#endif /* ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA */
