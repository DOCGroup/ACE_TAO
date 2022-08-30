// -*- C++ -*-

//=============================================================================
/**
 *  @file   Hash_LogRecordStore.h
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
  /// Constructor.
  TAO_Hash_LogRecordStore (TAO_LogMgr_i* logmgr,
                           DsLogAdmin::LogId id,
                           DsLogAdmin::LogFullActionType log_full_action,
                           CORBA::ULongLong max_size,
                           const DsLogAdmin::CapacityAlarmThresholdList* thresholds);

  /// Destructor.
  ~TAO_Hash_LogRecordStore (void) override;

  /// Initialization.
  int open (void) override;

  /// Close the record store.
  int close (void) override;


  // = Log Parameters

  /// Gets the administrative state of the log
  DsLogAdmin::AdministrativeState
    get_administrative_state () const override;

  /// Sets the administrative state of the log
  void
    set_administrative_state (DsLogAdmin::AdministrativeState) override;

  /// Get the capacity alarm thresholds
  DsLogAdmin::CapacityAlarmThresholdList*
    get_capacity_alarm_thresholds () const override;

  /// Set the capacity alarm thresholds
  void
    set_capacity_alarm_thresholds (const DsLogAdmin::CapacityAlarmThresholdList& thresholds) override;

  /// Gets the forwarding state
  DsLogAdmin::ForwardingState
    get_forwarding_state () const override;

  /// Sets the forwarding state
  void
    set_forwarding_state (DsLogAdmin::ForwardingState state) override;

  /// Get the log duration
  DsLogAdmin::TimeInterval get_interval () const override;

  /// Set the log duration.
  void
    set_interval (const DsLogAdmin::TimeInterval & interval) override;

  /// Gets the log full action
  DsLogAdmin::LogFullActionType get_log_full_action () const override;

  /// Sets the log full action
  void set_log_full_action(DsLogAdmin::LogFullActionType action) override;

  /// Get the list of the QoS properties supported by the log.
  DsLogAdmin::QoSList* get_log_qos () const override;

  /// Set the list of the QoS properties supported by the log.
  void set_log_qos (const DsLogAdmin::QoSList& qos) override;

  /// Gets the max record life
  CORBA::ULong get_max_record_life () const override;

  /// Sets the max record life
  void set_max_record_life (CORBA::ULong life) override;

  /// Get the current set value of the max size of the log data.
  CORBA::ULongLong get_max_size () const override;

  /// Set the max size of log data. size == 0, => infinite.
  void set_max_size (CORBA::ULongLong size) override;

  /// Get the weekly scheduling parameters
  DsLogAdmin::WeekMask* get_week_mask (void) override;

  /// Set the weekly scheduling parameters.
  void set_week_mask (const DsLogAdmin::WeekMask & masks) override;

  // = LogRecordStore status methods

  /// Gets the current size of the log data.
  CORBA::ULongLong get_current_size (void) override;

  /// Get the number of records in the log right now.
  CORBA::ULongLong get_n_records (void) override;


  // = LogRecordStore gauge

  /// Gets the current value of the "gauge" that measures the total
  /// size of the records written to the log.
  CORBA::ULongLong get_gauge(void) override;

  /// Resets the "gauge" to 0
  void reset_gauge(void) override;

  // = Record logging, retrieval, update and removal methods.

  /// Insert rec into storage. Returns 0 on success -1 on failure and 1
  /// if the log is full.
  int log (const DsLogAdmin::LogRecord &rec) override;

  /// Deletes "old" records from the store.
  int purge_old_records (void) override;

  /// Set single record attributes.
  void
    set_record_attribute (DsLogAdmin::RecordId id,
                          const DsLogAdmin::NVList & attr_list) override;

  /// Set the attributes of all records that matches the
  /// constraints with same attr_list.
  CORBA::ULong
    set_records_attribute (const char * grammar,
                           const char * c,
                           const DsLogAdmin::NVList & attr_list) override;

  /// Get the attributes of the record with id <id>. Raises
  /// DsLogAdmin::InvalidRecordId
  DsLogAdmin::NVList*
    get_record_attribute (DsLogAdmin::RecordId id) override;

  /// Ensure changes have been flushed to persistent media
  /// Returns 0 on success, -1 on failure.
  int flush (void) override;

  /// Returns all records in the log that match the given constraint
  /// <c>.
  DsLogAdmin::RecordList*
    query (const char * grammar,
           const char * c,
           DsLogAdmin::Iterator_out i) override;

  /// Retrieve <how_many> records from time <from_time> using iterator
  /// <i>.
  DsLogAdmin::RecordList*
    retrieve (DsLogAdmin::TimeT from_time,
              CORBA::Long how_many,
              DsLogAdmin::Iterator_out i) override;

  /// Returns the number of records matching constraint <c>.
  CORBA::ULong match (const char * grammar, const char * c) override;

  /// Delete records matching constraint <c>.
  CORBA::ULong
    delete_records (const char * grammar,
                    const char * c) override;

  /// Delete records matching ids in <ids>
  CORBA::ULong
    delete_records_by_id (const DsLogAdmin::RecordIdList & ids) override;

  CORBA::ULong remove_old_records (void) override;

  /// Read-Write Lock
  ACE_SYNCH_RW_MUTEX& lock() override;

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

  TAO_LogMgr_i*   logmgr_i_;

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
  LOG_RECORD_STORE      rec_map_;


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
  DsLogAdmin::QoSList     log_qos_;

  /// The maximum record lifetime
  CORBA::ULong                          max_record_life_;

  /// The days of the week that the log should be operational
  DsLogAdmin::WeekMask      weekmask_;


  ACE_Reactor*                          reactor_;

  PortableServer::POA_var   iterator_poa_;

  mutable ACE_SYNCH_RW_MUTEX    lock_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /*TAO_HASH_LOG_RECORD_STORE_H*/
