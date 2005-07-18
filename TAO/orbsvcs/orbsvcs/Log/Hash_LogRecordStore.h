// -*- C++ -*-

//=============================================================================
/**
 *  @file   Hash_LogRecordStore.h
 *
 *  $Id$
 *
 *  @author Matthew Braun <mjb2@cs.wustl.edu>
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 *  @author David A. Hanvey <d.hanvey@qub.ac.uk>
 */
//=============================================================================

#ifndef TAO_HASH_LOG_RECORD_STORE_H
#define TAO_HASH_LOG_RECORD_STORE_H

#include /**/ "ace/pre.h"
#include /**/ "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Log/LogRecordStore.h"
#include "ace/Containers.h"
#include "ace/Hash_Map_Manager.h"
#include "ace/Null_Mutex.h"
#include "ace/RW_Thread_Mutex.h"
#include "ace/Reactor.h"
#include "log_serv_export.h"

#define LOG_DEFAULT_MAX_REC_LIST_LEN 100

/**
 * @class TAO_Hash_LogRecordStore
 *
 * @brief A concrete container class for storing DsLogAdmin::LogRecords
 *
 * This implementation uses an ACE_RB_Tree and and ACE_Hash_Map
 * internally to store the pointers. Permits fast searches by id
 * and iteration, while allowing for ring-buffer like wrapping of
 * entries. Other features include searching by time ranges.
  * @@ pradeep: The ACE_RB_Tree will come later.
 */
class TAO_Log_Serv_Export TAO_Hash_LogRecordStore
  : public TAO_LogRecordStore
{
 public:

  // = Initialization and termination methods

  /// Constructor.
  TAO_Hash_LogRecordStore (CORBA::ORB_ptr orb,
                           DsLogAdmin::LogId id,
                           DsLogAdmin::LogFullActionType log_full_action,
                           CORBA::ULongLong max_size,
                           const DsLogAdmin::CapacityAlarmThresholdList* thresholds
                           );

  /// Destructor.
  virtual ~TAO_Hash_LogRecordStore (void);

  /// Initialization.
  virtual int open (void);

  /// Close the record store.
  virtual int close (void);


  // = Log Parameters

  /// Gets the administrative state of the log
  virtual DsLogAdmin::AdministrativeState
    get_administrative_state (ACE_ENV_SINGLE_ARG_DECL) const;

  /// Sets the administrative state of the log
  virtual void
    set_administrative_state (DsLogAdmin::AdministrativeState
                              ACE_ENV_ARG_DECL);

  /// Get the capacity alarm thresholds
  virtual DsLogAdmin::CapacityAlarmThresholdList*
    get_capacity_alarm_thresholds (ACE_ENV_SINGLE_ARG_DECL) const;

  /// Set the capacity alarm thresholds
  virtual void
    set_capacity_alarm_thresholds (const DsLogAdmin::CapacityAlarmThresholdList& thresholds
                                   ACE_ENV_ARG_DECL);

  /// Gets the forwarding state
  virtual DsLogAdmin::ForwardingState
    get_forwarding_state (ACE_ENV_SINGLE_ARG_DECL) const;

  /// Sets the forwarding state
  virtual void
    set_forwarding_state (DsLogAdmin::ForwardingState state
                          ACE_ENV_ARG_DECL);

  /// Get the log duration
  virtual DsLogAdmin::TimeInterval
    get_interval (ACE_ENV_SINGLE_ARG_DECL) const;

  /// Set the log duration.
  virtual void
    set_interval (const DsLogAdmin::TimeInterval & interval
                  ACE_ENV_ARG_DECL);

  /// Gets the log full action
  virtual DsLogAdmin::LogFullActionType
    get_log_full_action (ACE_ENV_SINGLE_ARG_DECL) const;

  /// Sets the log full action
  virtual void
    set_log_full_action(DsLogAdmin::LogFullActionType action
                        ACE_ENV_ARG_DECL);

  /// Gets the max record life
  virtual CORBA::ULong
    get_max_record_life (ACE_ENV_SINGLE_ARG_DECL) const;

  /// Sets the max record life
  virtual void
    set_max_record_life (CORBA::ULong life
                         ACE_ENV_ARG_DECL);

  /// Get the current set value of the max size of the log data.
  virtual CORBA::ULongLong
    get_max_size (ACE_ENV_SINGLE_ARG_DECL) const;

  /// Set the max size of log data. size == 0, => infinite.
  virtual void
    set_max_size (CORBA::ULongLong size
                  ACE_ENV_ARG_DECL);


  // = LogRecordStore status methods

  /// Gets the current size of the log data.
  virtual CORBA::ULongLong
    get_current_size (ACE_ENV_SINGLE_ARG_DECL);

  /// Get the number of records in the log right now.
  virtual CORBA::ULongLong
    get_n_records (ACE_ENV_SINGLE_ARG_DECL);

  // = Record logging, retrieval, update and removal methods.

  /// Insert rec into storage. Returns 0 on success -1 on failure and 1
  /// if the log is full.
  virtual int
    log (DsLogAdmin::LogRecord &rec ACE_ENV_ARG_DECL);

  /// Set rec to the pointer to the LogRecord with the given
  /// id. Returns 0 on success, -1 on failure.
  virtual int
    retrieve (DsLogAdmin::RecordId id,
	      DsLogAdmin::LogRecord &rec
	      ACE_ENV_ARG_DECL);

  /// Update into storage. Returns 0 on success -1 on failure.
  virtual int
    update (DsLogAdmin::LogRecord &rec
	    ACE_ENV_ARG_DECL);

  /// Remove the record with id <id> from the LogRecordStore.
  /// Returns 0 on success, -1 on failure.
  virtual int
    remove (DsLogAdmin::RecordId id
	    ACE_ENV_ARG_DECL);

  /// Deletes "old" records from the store.
  virtual int
    purge_old_records (ACE_ENV_SINGLE_ARG_DECL);

  /// Ensure changes have been flushed to persistent media
  /// Returns 0 on success, -1 on failure.
  virtual int
    flush (ACE_ENV_SINGLE_ARG_DECL);

  /// Returns all records in the log that match the given constraint
  /// <c>.
  virtual DsLogAdmin::RecordList*
    query (const char * grammar,
           const char * c,
           DsLogAdmin::Iterator_out i
           ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     DsLogAdmin::InvalidGrammar,
                     DsLogAdmin::InvalidConstraint));

  /// Retrieve <how_many> records from time <from_time> using iterator
  /// <i>.
  virtual DsLogAdmin::RecordList*
    retrieve (DsLogAdmin::TimeT from_time,
              CORBA::Long how_many,
              DsLogAdmin::Iterator_out i
              ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Returns the number of records matching constraint <c>.
  virtual CORBA::ULong
    match (const char * grammar,
           const char * c
           ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     DsLogAdmin::InvalidGrammar,
                     DsLogAdmin::InvalidConstraint));

  /// Delete records matching constraint <c>.
  virtual CORBA::ULong
    delete_records (const char * grammar,
                    const char * c
                    ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     DsLogAdmin::InvalidGrammar,
                     DsLogAdmin::InvalidConstraint));

  /// Delete records matching ids in <ids>
  virtual CORBA::ULong
    delete_records_by_id (const DsLogAdmin::RecordIdList & ids
                          ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::ULong
    remove_old_records (ACE_ENV_SINGLE_ARG_DECL);

/* protected: */
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
  typedef LOG_RECORD_HASH_MAP_ENTRY LOG_RECORD_STORE_ENTRY;

protected:
  DsLogAdmin::RecordList* query_i (const char *constraint,
                                   DsLogAdmin::Iterator_out &iter_out,
                                   CORBA::ULong how_many
                                   ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     DsLogAdmin::InvalidConstraint));

  CORBA::ULong match_i (const char *constraint,
                        CORBA::Boolean delete_rec
                        ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     DsLogAdmin::InvalidConstraint));

  int remove_i (DsLogAdmin::RecordId id
	        ACE_ENV_ARG_DECL);

  /// Throws DsLogAdmin::InvalidGrammar if we don't support this grammar.
  void check_grammar (const char* grammar ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     DsLogAdmin::InvalidGrammar));


  /// The size of a LogRecord
  size_t log_record_size(const DsLogAdmin::LogRecord &rec);

  /// Assigned to a new RecordId and then incremented
  /// @@ Should I have a list of reclaimed id's for when records are
  /// deleted?
  DsLogAdmin::RecordId maxid_;

  /// The maximum size of the log.
  CORBA::ULongLong max_size_;

  /// The log id to which this LogRecordStore relates.
  DsLogAdmin::LogId id_;

  /// The current size (in bytes) of the log.
  CORBA::ULongLong current_size_;

  /// The current number of records in the log.
  CORBA::ULongLong num_records_;

  /// The max size of the record list returned in a query.
  CORBA::ULong max_rec_list_len_;

  /// The hash of LogRecord ids to LogRecord 's
  TAO_Hash_LogRecordStore::LOG_RECORD_HASH_MAP rec_hash_;


  /// The administrative state of the log
  DsLogAdmin::AdministrativeState       admin_state_;

  DsLogAdmin::CapacityAlarmThresholdList
                                        thresholds_;

  /// The forwarding state of the log
  DsLogAdmin::ForwardingState           forward_state_;

  /// The interval during which the log should be in operation
  DsLogAdmin::TimeInterval              interval_;

  /// The action to take if the log reaches max capacity
  DsLogAdmin::LogFullActionType         log_full_action_;

  /// The maximum record lifetime
  CORBA::ULong                          max_record_life_;

  ACE_Reactor*                          reactor_;

  mutable ACE_RW_Thread_Mutex           lock_;
};

#include /**/ "ace/post.h"
#endif /*TAO_HASH_LOG_RECORD_STORE_H*/
