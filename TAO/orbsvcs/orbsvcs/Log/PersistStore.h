/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//   orbsvcs
//
// = FILENAME
//   PersistStore.h
//
// = DESCRIPTION
//    This flat file demonstrates basic persistence.
//    
//
// = AUTHORS
//   Rob Ruff <rruff@scires.com>
//   D A Hanvey <d.hanvey@qub.ac.uk>
//
// ============================================================================

#ifndef PERSIST_STORE_H
#define PERSIST_STORE_H

#include "orbsvcs/DsLogAdminS.h"
#include "orbsvcs/DsLogAdminC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#define PERSIST_LOG_DEFAULT_MAX_REC_LIST_LEN 250

struct PersistentData
{
  DsLogAdmin::RecordId id;
  DsLogAdmin::TimeT time;
  CORBA::TCKind kind;
  int byte_order;
  size_t mb_size;
};

class TAO_Log_Export PersistStore
{
  // = TITLE
  //     A class for storing DsLogAdmin::LogRecord *'s
  //
  // = Description
  //     This implementation is to serve for illustration purposes.

 public:
  // = Initialization and termination methods

  PersistStore (CORBA::ULongLong max_size = 0,
                CORBA::ULong max_rec_list_len
                = PERSIST_LOG_DEFAULT_MAX_REC_LIST_LEN);
  // Constructor

  ~PersistStore (void);
  // Destructor

  int open (const char * file_name);
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

  unsigned short get_percentage_full (void);
  // Return the percentage of the max space that is currently being used,
  // If size is infinite, return 0.

 protected:

  DsLogAdmin::RecordId maxid_;
  // Assigned to a new RecordId and then incremented

  CORBA::ULongLong max_size_;
  // The maximum size of the log.

  CORBA::ULongLong current_size_;
  // The current size (in bytes) of the log

  CORBA::ULongLong num_records_;
  // The current number of records in the log

  CORBA::ULong max_rec_list_len_;
  // The max size of the record list returned in a query.

  ACE_HANDLE write_persistent_file_;
  ACE_HANDLE read_persistent_file_;
  // The file descriptor for the persistent log storage file.

  ACE_Mutex write_lock_;
  // Lock the log operation 
};

#endif /*PERSIST_STORE_H*/
