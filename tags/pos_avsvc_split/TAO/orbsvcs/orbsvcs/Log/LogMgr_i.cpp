// $Id$

#include "LogMgr_i.h"

LogMgr_i::LogMgr_i (void)
{
  // No-Op.
}

LogMgr_i::~LogMgr_i ()
{
  // No-Op.
}

DsLogAdmin::LogList_ptr
LogMgr_i::list_logs (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  DsLogAdmin::LogList_ptr list;

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
        DsLogAdmin::BasicLog::_duplicate (hash_entry->int_id_.in ());
    }

  return list;
}

DsLogAdmin::Log_ptr
LogMgr_i::find_log (DsLogAdmin::LogId id,
                    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  DsLogAdmin::BasicLog_var v_return;

  if (hash_map_.find (id,
                      v_return) == -1)
    {
      return DsLogAdmin::BasicLog::_nil ();
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

      DsLogAdmin::BasicLog::_duplicate (v_return.in ());

      return DsLogAdmin::BasicLog::_duplicate (v_return.in ());
      // This duplicate is to obey rules of returning obj. refs.
      // don't get confused!
    }
}

DsLogAdmin::LogIdList_ptr
LogMgr_i::list_logs_by_id (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  // Later: We need to maintain a sorted list of logids to do this efficiently.
  return 0;
}

int
LogMgr_i::remove (DsLogAdmin::LogId id)
{
  return this->hash_map_.unbind (id);
}
