// -*- C++ -*-

//=============================================================================
/**
 *  @file   LogRecordStore.h
 *
 *  $Id$
 *
 *  This file declares the tools used to provide the store of
 *  DsLogAdmin::LogRecords for the Telecom Logging_Service
 *
 *  @author Matthew Braun <mjb2@cs.wustl.edu>
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 *  @author David A. Hanvey <d.hanvey@qub.ac.uk>
 */
//=============================================================================

#ifndef TAO_LOG_RECORD_STORE_H
#define TAO_LOG_RECORD_STORE_H
#include /**/ "ace/pre.h"

#include "orbsvcs/DsLogAdminS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/DsLogAdminC.h"
#include "ace/Containers.h"
#include "ace/Hash_Map_Manager.h"
#include "log_export.h"

#define LOG_DEFAULT_MAX_REC_LIST_LEN 30

/**
 * @class TAO_LogRecordStore
 *
 * @brief A container class for storing DsLogAdmin::LogRecord *'s
 *
 * This implementation uses an ACE_RB_Tree and and ACE_Hash_Map
 * internally to store the pointers. Permits fast searches by id
 * and iteration, while allowing for ring-buffer like wrapping of
 * entries. Other features include searching by time ranges.
 * @@ pradeep: The ACE_RB_Tree will come later.
 */
class TAO_Log_Export TAO_LogRecordStore
{
 public:

  // = Initialization and termination methods

  /// Constructor.
  TAO_LogRecordStore (CORBA::ULongLong max_size = 0,
                      DsLogAdmin::LogId logid = 0,
                      CORBA::ULong max_rec_list_len
                      = LOG_DEFAULT_MAX_REC_LIST_LEN);

  /// Destructor.
  ~TAO_LogRecordStore (void);

  /// Initialization.
  int open (void);

  /// Close the record store.
  int close (void);

  // = LogRecordStore status methods

  /// Get the current set value of the max size of the log data.
  CORBA::ULongLong get_max_size (void);

  /// Set the max size of log data. size == 0, => infinite.
  void set_max_size (CORBA::ULongLong size);

  /// Gets the current size of the log data.
  CORBA::ULongLong get_current_size (void);

  /// Get the number of records in the log right now.
  CORBA::ULongLong get_n_records (void);

  // = Record logging, retrieval, update and removal methods.

  /// Insert rec into storage. Returns 0 on success -1 on failure and 1
  /// if the log is full.
  int log (DsLogAdmin::LogRecord &rec);

  /// Set rec to the pointer to the LogRecord with the given
  /// id. Returns 0 on success, -1 on failure.
  int retrieve (DsLogAdmin::RecordId id, DsLogAdmin::LogRecord &rec);

  /// Update into storage. Returns 0 on success -1 on failure.
  int update (DsLogAdmin::LogRecord &rec);

  /// Remove the record with id <id> from the LogRecordStore. Returns 0 on
  /// success, -1 on failure.
  int remove (DsLogAdmin::RecordId id);

  /// Deletes "old" records from the store.
  int purge_old_records (void);

  /// Defines macros to represent the hash that maps ids to
  /// DsLogAdmin::LogRecords.
  typedef ACE_Hash_Map_Manager <DsLogAdmin::RecordId,
    DsLogAdmin::LogRecord, ACE_Null_Mutex> LOG_RECORD_HASH_MAP;
  typedef ACE_Hash_Map_Iterator <DsLogAdmin::RecordId,
    DsLogAdmin::LogRecord, ACE_Null_Mutex> LOG_RECORD_HASH_MAP_ITER;
  typedef ACE_Hash_Map_Entry <DsLogAdmin::RecordId,
    DsLogAdmin::LogRecord> LOG_RECORD_HASH_MAP_ENTRY;

  /// Don't want to be tied to hash maps!.
  typedef LOG_RECORD_HASH_MAP_ITER LOG_RECORD_STORE_ITER;
  typedef LOG_RECORD_HASH_MAP LOG_RECORD_STORE;

  /// Get the underlying storage.
  /// @@ return a const ref? we don't want anyone to modify the storage.
  TAO_LogRecordStore::LOG_RECORD_STORE& get_storage (void);

 protected:

  /// Assigned to a new RecordId and then incremented
  /// @@ Should I have a list of reclaimed id's for when records are
  /// deleted?
  DsLogAdmin::RecordId maxid_;

  /// The maximum size of the log.
  CORBA::ULongLong max_size_;

  /// The log id to which this LogRecordStore relates.
  DsLogAdmin::LogId logid_;

  /// The current size (in bytes) of the log.
  CORBA::ULongLong current_size_;

  /// The current number of records in the log.
  CORBA::ULongLong num_records_;

  /// The max size of the record list returned in a query.
  CORBA::ULong max_rec_list_len_;

  /// The hash of LogRecord ids to LogRecord 's
  TAO_LogRecordStore::LOG_RECORD_HASH_MAP rec_hash_;
};

#include /**/ "ace/post.h"
#endif /*TAO_LOG_RECORD_STORE_H*/
