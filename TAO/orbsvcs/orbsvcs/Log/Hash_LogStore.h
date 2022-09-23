// -*- C++ -*-

//=============================================================================
/**
 *  @file   Hash_LogStore.h
 *
 *  @author Matthew Braun <mjb2@cs.wustl.edu>
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 *  @author David A. Hanvey <d.hanvey@qub.ac.uk>
 */
//=============================================================================

#ifndef TAO_TLS_HASH_LOGSTORE_H
#define TAO_TLS_HASH_LOGSTORE_H

#include /**/ "ace/pre.h"
#include /**/ "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Log/LogStore.h"
#include "ace/Hash_Map_Manager.h"
#include "ace/Synch_Traits.h"
#include "ace/Null_Mutex.h"
#include "ace/RW_Thread_Mutex.h"
#include "orbsvcs/Log/log_serv_export.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_Reactor;
ACE_END_VERSIONED_NAMESPACE_DECL

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_LogMgr_i;
class TAO_Hash_LogRecordStore;

class TAO_Log_Serv_Export TAO_Hash_LogStore
  : public TAO_LogStore
{
public:
  /// Constructor.
  TAO_Hash_LogStore (TAO_LogMgr_i* mgr);

  /// Destructor.
  ~TAO_Hash_LogStore () override;


  /// Lists all logs created by the log factory.
  DsLogAdmin::LogList *
    list_logs () override;

  /// Lists all log ids.
  DsLogAdmin::LogIdList *
    list_logs_by_id () override;

  /// Returns a reference to the log with the supplied id.
  DsLogAdmin::Log_ptr
    find_log (DsLogAdmin::LogId id) override;

  /// Returns true if log exists, otherwise false
  bool exists (DsLogAdmin::LogId id) override;

  /// Remove the given entry from the hash table.
  int remove (DsLogAdmin::LogId id) override;

  /// @brief Create log
  void
    create (DsLogAdmin::LogFullActionType full_action,
            CORBA::ULongLong max_size,
            const DsLogAdmin::CapacityAlarmThresholdList* thresholds,
            DsLogAdmin::LogId_out id_out) override;

  /// @brief Create log
  void
    create_with_id (DsLogAdmin::LogId id,
                    DsLogAdmin::LogFullActionType full_action,
                    CORBA::ULongLong max_size,
                    const DsLogAdmin::CapacityAlarmThresholdList* thresholds) override;

  /// @brief Get log record store
  ///
  /// Get/Create a log record store for log channel @a id.
  ///
  /// @param id log id
  ///
  TAO_LogRecordStore*
    get_log_record_store (DsLogAdmin::LogId id) override;

private:
  ACE_SYNCH_RW_MUTEX  lock_;

  /// Define the HASHMAP.
  typedef ACE_Hash_Map_Manager <DsLogAdmin::LogId,
                                TAO_Hash_LogRecordStore*,
                                ACE_Null_Mutex> HASHMAP;

  /// The map of Logs created.
  HASHMAP hash_map_;

  /// The next log id to be assigned (if it hasn't already been
  /// taken by create_with_id().
  DsLogAdmin::LogId     next_id_;

  TAO_LogMgr_i*         logmgr_i_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_TLS_HASH_LOGSTORE_H */
