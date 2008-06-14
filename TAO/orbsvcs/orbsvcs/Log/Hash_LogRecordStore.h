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
#include "ace/RB_Tree.h"
#include "ace/Synch_Traits.h"
#include "ace/Null_Mutex.h"
#include "ace/RW_Thread_Mutex.h"
#include "ace/Reactor.h"
#include "tao/PortableServer/PortableServer.h"
#include "orbsvcs/Log/log_serv_export.h"

#define LOG_DEFAULT_MAX_REC_LIST_LEN 100

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_LogMgr_i;

/**
 * @class TAO_Hash_LogRecordStore
 *
 * @brief A concrete container class for storing DsLogAdmin::LogRecords
 *
 * This implementation uses an ACE_RB_Tree to store LogRecords.
 * Permits fast searches by id and iteration, while allowing for
 * ring-buffer like wrapping of entries.
 *
 * @note The name of this class is somewhat misleading, as it no
 * longer uses a ACE_Hash_Map.
 *
 * @note LogRecords should be indexed by both id and timestamp.
 * However, timestamps are not unique keys and there are no ACE
 * multimap containers.
 *
 * @todo If ACE_RB_Tree supported an insertion "hint" like std::map,
 * log insertion would be O(1) instead of O(lgN).  This could be an
 * issue for large log channels.
 */
class TAO_Log_Serv_Export TAO_Hash_LogRecordStore
  : public TAO_LogRecordStore
{
 public:

  // = Initialization and termination methods

  /// Constructor.
  TAO_Hash_LogRecordStore (TAO_LogMgr_i* logmgr,
                           DsLogAdmin::LogId id,
                           DsLogAdmin::LogFullActionType log_full_action,
                           CORBA::ULongLong max_size,
                           const DsLogAdmin::CapacityAlarmThresholdList* thresholds);

  /// Destructor.
  virtual ~TAO_Hash_LogRecordStore (void);

  /// Initialization.
  virtual int open (void);

  /// Close the record store.
  virtual int close (void);


  // = Log Parameters

  /// Gets the administrative state of the log
  virtual DsLogAdmin::AdministrativeState
    get_administrative_state (void) const;

  /// Sets the administrative state of the log
  virtual void
    set_administrative_state (DsLogAdmin::AdministrativeState);

  /// Get the capacity alarm thresholds
  virtual DsLogAdmin::CapacityAlarmThresholdList*
    get_capacity_alarm_thresholds (void) const;

  /// Set the capacity alarm thresholds
  virtual void
    set_capacity_alarm_thresholds (const DsLogAdmin::CapacityAlarmThresholdList& thresholds);

  /// Gets the forwarding state
  virtual DsLogAdmin::ForwardingState
    get_forwarding_state (void) const;

  /// Sets the forwarding state
  virtual void
    set_forwarding_state (DsLogAdmin::ForwardingState state);

  /// Get the log duration
  virtual DsLogAdmin::TimeInterval get_interval (void) const;

  /// Set the log duration.
  virtual void
    set_interval (const DsLogAdmin::TimeInterval & interval);

  /// Gets the log full action
  virtual DsLogAdmin::LogFullActionType get_log_full_action (void) const;

  /// Sets the log full action
  virtual void set_log_full_action(DsLogAdmin::LogFullActionType action);

  /// Get the list of the QoS properties supported by the log.
  virtual DsLogAdmin::QoSList* get_log_qos (void) const;

  /// Set the list of the QoS properties supported by the log.
  virtual void set_log_qos (const DsLogAdmin::QoSList& qos);

  /// Gets the max record life
  virtual CORBA::ULong get_max_record_life (void) const;

  /// Sets the max record life
  virtual void set_max_record_life (CORBA::ULong life);

  /// Get the current set value of the max size of the log data.
  virtual CORBA::ULongLong get_max_size (void) const;

  /// Set the max size of log data. size == 0, => infinite.
  virtual void set_max_size (CORBA::ULongLong size);

  /// Get the weekly scheduling parameters
  virtual DsLogAdmin::WeekMask* get_week_mask (void);

  /// Set the weekly scheduling parameters.
  virtual void set_week_mask (const DsLogAdmin::WeekMask & masks);

  // = LogRecordStore status methods

  /// Gets the current size of the log data.
  virtual CORBA::ULongLong get_current_size (void);

  /// Get the number of records in the log right now.
  virtual CORBA::ULongLong get_n_records (void);


  // = LogRecordStore gauge

  /// Gets the current value of the "gauge" that measures the total
  /// size of the records written to the log.
  virtual CORBA::ULongLong get_gauge(void);

  /// Resets the "gauge" to 0
  virtual void reset_gauge(void);

  // = Record logging, retrieval, update and removal methods.

  /// Insert rec into storage. Returns 0 on success -1 on failure and 1
  /// if the log is full.
  virtual int log (const DsLogAdmin::LogRecord &rec);

  /// Deletes "old" records from the store.
  virtual int purge_old_records (void);

  /// Set single record attributes.
  virtual void
    set_record_attribute (DsLogAdmin::RecordId id,
                          const DsLogAdmin::NVList & attr_list);

  /// Set the attributes of all records that matches the
  /// constraints with same attr_list.
  virtual CORBA::ULong
    set_records_attribute (const char * grammar,
                           const char * c,
                           const DsLogAdmin::NVList & attr_list);

  /// Get the attributes of the record with id <id>. Raises
  /// DsLogAdmin::InvalidRecordId
  virtual DsLogAdmin::NVList*
    get_record_attribute (DsLogAdmin::RecordId id);

  /// Ensure changes have been flushed to persistent media
  /// Returns 0 on success, -1 on failure.
  virtual int flush (void);

  /// Returns all records in the log that match the given constraint
  /// <c>.
  virtual DsLogAdmin::RecordList*
    query (const char * grammar,
           const char * c,
           DsLogAdmin::Iterator_out i);

  /// Retrieve <how_many> records from time <from_time> using iterator
  /// <i>.
  virtual DsLogAdmin::RecordList*
    retrieve (DsLogAdmin::TimeT from_time,
              CORBA::Long how_many,
              DsLogAdmin::Iterator_out i);

  /// Returns the number of records matching constraint <c>.
  virtual CORBA::ULong match (const char * grammar, const char * c);

  /// Delete records matching constraint <c>.
  virtual CORBA::ULong
    delete_records (const char * grammar,
                    const char * c);

  /// Delete records matching ids in <ids>
  virtual CORBA::ULong
    delete_records_by_id (const DsLogAdmin::RecordIdList & ids);

  virtual CORBA::ULong remove_old_records (void);

  /// Read-Write Lock
  virtual ACE_SYNCH_RW_MUTEX& lock();

/* protected: */
  /// Defines types to represent the container that maps RecordIds to
  /// LogRecords.
  typedef ACE_RB_Tree <DsLogAdmin::RecordId,
                       DsLogAdmin::LogRecord,
                       ACE_Less_Than<DsLogAdmin::RecordId>,
                       ACE_Null_Mutex> LOG_RECORD_STORE;
  typedef LOG_RECORD_STORE::ITERATOR LOG_RECORD_STORE_ITER;
  typedef LOG_RECORD_STORE::ENTRY LOG_RECORD_STORE_ENTRY;

protected:
  /// Set rec to the pointer to the LogRecord with the given
  /// id. Returns 0 on success, -1 on failure.
  int retrieve_i (DsLogAdmin::RecordId id,
                  DsLogAdmin::LogRecord &rec);

  /// Update into storage. Returns 0 on success -1 on failure.
  int update_i (DsLogAdmin::LogRecord &rec);

  /// Remove the record with id <id> from the LogRecordStore.
  /// Returns 0 on success, -1 on failure.
  int remove_i (DsLogAdmin::RecordId id);

  /// Remove the record from the LogRecordStore.
  void remove_i (LOG_RECORD_STORE_ITER iter);

  DsLogAdmin::RecordList* query_i (const char *constraint,
                                   DsLogAdmin::Iterator_out &iter_out,
                                   CORBA::ULong how_many);

  /// Throws DsLogAdmin::InvalidGrammar if we don't support this grammar.
  void check_grammar (const char* grammar);


  /// The size of a LogRecord
  size_t log_record_size(const DsLogAdmin::LogRecord &rec);

  TAO_LogMgr_i*		logmgr_i_;

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

  /// The current value of the "gauge" that measures the total size of
  /// the records written to the log.
  CORBA::ULongLong gauge_;

  /// The max size of the record list returned in a query.
  CORBA::ULong max_rec_list_len_;

  /// The map of RecordId's to LogRecord's
  LOG_RECORD_STORE			rec_map_;


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

  /// The list of the QoS properties supported by the log.
  DsLogAdmin::QoSList			log_qos_;

  /// The maximum record lifetime
  CORBA::ULong                          max_record_life_;

  /// The days of the week that the log should be operational
  DsLogAdmin::WeekMask			weekmask_;


  ACE_Reactor*                          reactor_;

  PortableServer::POA_var		iterator_poa_;

  mutable ACE_SYNCH_RW_MUTEX		lock_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /*TAO_HASH_LOG_RECORD_STORE_H*/
