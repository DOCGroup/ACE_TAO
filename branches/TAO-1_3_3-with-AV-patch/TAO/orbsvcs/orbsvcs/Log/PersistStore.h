// -*- C++ -*-

//=============================================================================
/**
 *  @file   PersistStore.h
 *
 *  $Id$
 *
 *  This flat file demonstrates basic persistence.
 *
 *  @author Rob Ruff <rruff@scires.com>
 *  @author David A. Hanvey <d.hanvey@qub.ac.uk>
 */
//=============================================================================

#ifndef TAO_PERSIST_STORE_H
#define TAO_PERSIST_STORE_H
#include "ace/pre.h"

#include "orbsvcs/DsLogAdminS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/DsLogAdminC.h"

#define PERSIST_LOG_DEFAULT_MAX_REC_LIST_LEN 250

struct PersistentData
{
  DsLogAdmin::RecordId id;
  DsLogAdmin::TimeT time;
  CORBA::TCKind kind;
  int byte_order;
  size_t mb_size;
};

/**
 * @class TAO_PersistStore
 *
 * @brief A class for storing DsLogAdmin::LogRecord *'s
 *
 * This implementation is to serve for illustration purposes.
 */

class TAO_Log_Export TAO_PersistStore
{
 public:

  // = Initialization and termination methods

  /// Constructor.
  TAO_PersistStore (CORBA::ULongLong max_size = 0,
                    CORBA::ULong max_rec_list_len
                    = PERSIST_LOG_DEFAULT_MAX_REC_LIST_LEN);

  /// Destructor.
  ~TAO_PersistStore (void);

  /// Initialization.
  int open (const char * file_name);

  /// Close the record store.
  int close (void);

  // = LogRecordStore status methods.

  /// Get the current set value of the max size of the log data.
  CORBA::ULongLong get_max_size (void);

  /// Set the max size of log data. size == 0, => infinite.
  void set_max_size (CORBA::ULongLong size);

  /// Gets the current size of the log data.
  CORBA::ULongLong get_current_size (void);

  /// Get the number of records in the log right now.
  CORBA::ULongLong get_n_records (void);

  // = Record logging, retrieval, update and removal methods

  /// Insert rec into storage. Returns 0 on success -1 on failure and 1
  /// if the log is full.
  int log (DsLogAdmin::LogRecord &rec);

  /// Set rec to the pointer to the LogRecord with the given
  /// id. Returns 0 on success, -1 on failure.
  int retrieve (DsLogAdmin::RecordId id, DsLogAdmin::LogRecord &rec);

  /// Return the percentage of the max space that is currently being used,
  /// If size is infinite, return 0.
  unsigned short get_percentage_full (void);

 protected:

  /// Assigned to a new RecordId and then incremented.
  DsLogAdmin::RecordId maxid_;

  /// The maximum size of the log.
  CORBA::ULongLong max_size_;

  /// The current size (in bytes) of the log.
  CORBA::ULongLong current_size_;

  /// The current number of records in the log.
  CORBA::ULongLong num_records_;

  /// The max size of the record list returned in a query.
  CORBA::ULong max_rec_list_len_;

  /// The file descriptor for the persistent log storage file.
  ACE_HANDLE write_persistent_file_;
  ACE_HANDLE read_persistent_file_;

  /// Lock the log operation.
  ACE_Mutex write_lock_;
};

#include "ace/post.h"
#endif /*TAO_PERSIST_STORE_H*/
