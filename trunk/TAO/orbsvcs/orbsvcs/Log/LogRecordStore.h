/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    orbsvcs
//
// = FILENAME
//    LogRecordStore.h
//
// = DESCRIPTION
//     This file declares the tools used to provide the store of
//     DsLogAdmin::LogRecords for the Telecom Logging_Service
//
// = AUTHORS
//    Matthew Braun (mjb2@cs.wustl.edu) and Pradeep Gore <pradeep@cs.wustl.edu>
//
// ============================================================================

#ifndef LOG_RECORD_STORE_H
#define LOG_RECORD_STORE_H

#include "orbsvcs/DsLogAdminS.h"
#include "orbsvcs/DsLogAdminC.h"
#include "ace/Containers.h"
#include "ace/Hash_Map_Manager.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#define LOG_DEFAULT_MAX_REC_LIST_LEN 30

class LogRecordStore
{
  // = TITLE
  //     A container class for storing DsLogAdmin::LogRecord *'s
  //
  // = Description
  //     This implementation uses an ACE_RB_Tree and and ACE_Hash_Map
  //     internally to store the pointers. Permits fast searches by id
  //     and iteration, while allowing for ring-buffer like wrapping of
  //     entries. Other features include searching by time ranges.
  //     @@ pradeep: The ACE_RB_Tree will come later.
 public:
  // = Initialization and termination methods

  LogRecordStore (CORBA::ULongLong max_size = 0,
                  CORBA::ULong max_rec_list_len
                  = LOG_DEFAULT_MAX_REC_LIST_LEN);
  // Constructor

  ~LogRecordStore (void);
  // Destructor

  int open (void);
  // Initialization.

  int close (void);
  // Close the record store.

  // = LogRecordStore status methods

  CORBA::ULongLong get_max_size (void);
  // Get the current set value of the max size of the log data.

  void set_max_size (CORBA::ULongLong size);
  // Set the max size of log data. size == 0, => infinite.

  CORBA::ULongLong get_current_size (void);
  // Gets the current size of the log data.

  CORBA::ULongLong get_n_records (void);
  // Get the number of records in the log right now.

  // = Record logging, retrieval, update and removal methods

  int log (DsLogAdmin::LogRecord &rec);
  // Insert rec into storage. Returns 0 on success -1 on failure and 1
  // if the log is full

  int retrieve (DsLogAdmin::RecordId id, DsLogAdmin::LogRecord &rec);
  // Set rec to the pointer to the LogRecord with the given
  // id. Returns 0 on success, -1 on failure

  int update (DsLogAdmin::LogRecord &rec);
  // update into storage. Returns 0 on success -1 on failure

  int remove (DsLogAdmin::RecordId id);
  // Remove the record with id <id> from the LogRecordStore. Returns 0 on
  // success, -1 on failure.

  int purge_old_records (void);
  // Deletes "old" records from the store.

  DsLogAdmin::RecordList_ptr query (const char *constraint,
                                    DsLogAdmin::Iterator_out& i,
                                    CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     DsLogAdmin::InvalidConstraint));
  // Do the query operation.

  typedef ACE_Hash_Map_Manager <DsLogAdmin::LogId,
    DsLogAdmin::LogRecord, ACE_Null_Mutex> LOG_RECORD_HASH_MAP;
  typedef ACE_Hash_Map_Iterator <DsLogAdmin::LogId,
    DsLogAdmin::LogRecord, ACE_Null_Mutex> LOG_RECORD_HASH_MAP_ITER;
  typedef ACE_Hash_Map_Entry <DsLogAdmin::LogId,
    DsLogAdmin::LogRecord> LOG_RECORD_HASH_MAP_ENTRY;
  // Defines macros to represent the hash that maps ids to
  // DsLogAdmin::LogRecords.

  typedef LOG_RECORD_HASH_MAP_ITER LOG_RECORD_STORE_ITER;
  typedef LOG_RECORD_HASH_MAP LOG_RECORD_STORE;
  // Don't want to be tied to hash maps!.

  /*
  const LOG_RECORD_STORE_ITER& get_record_iter_start (void);
  // Returns the record has map start iterator.

  const LOG_RECORD_STORE_ITER& get_record_iter_end (void);
  // Returns the record has map end iterator.
  */

  LOG_RECORD_STORE& get_storage (void);
  // Get the underlying storage.
  // @@ return a const ref? we don't want anyone to modify the storage.
 protected:

  LOG_RECORD_HASH_MAP rec_hash_;
  // The hash of LogRecord ids to LogRecord 's

  DsLogAdmin::RecordId maxid_;
  // Assigned to a new RecordId and then incremented
  // @@ Should I have a list of reclaimed id's for when records are
  // deleted?

  CORBA::ULongLong max_size_;
  // The maximum size of the log.

  CORBA::ULong current_size_;
  // The current size (in bytes) of the log

  CORBA::ULongLong num_records_;
  // The current number of records in the log

  CORBA::ULong max_rec_list_len_;
  // The max size of the record list returned in a query.
};

#endif /*LOG_RECORD_STORE_H*/
