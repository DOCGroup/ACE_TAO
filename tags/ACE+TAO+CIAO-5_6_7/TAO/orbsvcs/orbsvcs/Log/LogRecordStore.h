// -*- C++ -*-

//=============================================================================
/**
 *  @file   LogRecordStore.h
 *
 *  $Id$
 *
 *  @author Matthew Braun <mjb2@cs.wustl.edu>
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 *  @author David A. Hanvey <d.hanvey@qub.ac.uk>
 */
//=============================================================================

#ifndef TAO_LOG_RECORD_STORE_H
#define TAO_LOG_RECORD_STORE_H

#include /**/ "ace/pre.h"
#include /**/ "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/DsLogAdminC.h"
#include "orbsvcs/Log/log_serv_export.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_LogRecordStore
 *
 * @brief Abstract base class for storing DsLogAdmin::LogRecord's
 *
 * Implements a Table Data Gateway(144) for log records, and a Row
 * Data Gateway(152) for log channel parameters.
 */
class TAO_Log_Serv_Export TAO_LogRecordStore
{
 public:

  // = Initialization and termination methods

  /// Destructor.
  virtual ~TAO_LogRecordStore (void);

  /// Initialization.
  virtual int open (void) = 0;

  /// Close the record store.
  virtual int close (void) = 0;


  // = Log Parameters

  /// Gets the administrative state of the log
  virtual DsLogAdmin::AdministrativeState
    get_administrative_state (void) const = 0;

  /// Sets the administrative state of the log
  virtual void
    set_administrative_state (DsLogAdmin::AdministrativeState state) = 0;

  /// Get the capacity alarm thresholds
  virtual DsLogAdmin::CapacityAlarmThresholdList*
    get_capacity_alarm_thresholds (void) const = 0;

  /// Set the capacity alarm thresholds
  virtual void
    set_capacity_alarm_thresholds (const DsLogAdmin::CapacityAlarmThresholdList& thresholds) = 0;

  /// Gets the forwarding state
  virtual DsLogAdmin::ForwardingState
    get_forwarding_state (void) const = 0;

  /// Sets the forwarding state
  virtual void
    set_forwarding_state (DsLogAdmin::ForwardingState state) = 0;

  /// Get the log duration
  virtual DsLogAdmin::TimeInterval get_interval (void) const = 0;

  /// Set the log duration.
  virtual void
    set_interval (const DsLogAdmin::TimeInterval& interval) = 0;

  /// Get the log full action
  virtual DsLogAdmin::LogFullActionType
    get_log_full_action (void) const = 0;

  /// Set the log full action
  virtual void
    set_log_full_action(DsLogAdmin::LogFullActionType action) = 0;

  /// Get the list of the QoS properties supported by the log.
  virtual DsLogAdmin::QoSList*
    get_log_qos (void) const = 0;

  /// Set the list of the QoS properties supported by the log.
  virtual void
    set_log_qos (const DsLogAdmin::QoSList& qos) = 0;

  /// Gets the max record life
  virtual CORBA::ULong
    get_max_record_life (void) const = 0;

  /// Sets the max record life
  virtual void
    set_max_record_life (CORBA::ULong life) = 0;

  /// Get the current set value of the max size of the log data.
  virtual CORBA::ULongLong
    get_max_size (void) const = 0;

  /// Set the max size of log data. size == 0, => infinite.
  virtual void
    set_max_size (CORBA::ULongLong size) = 0;

  /// Get the weekly scheduling parameters
  virtual DsLogAdmin::WeekMask*
    get_week_mask (void) = 0;

  /// Set the weekly scheduling parameters.
  virtual void
    set_week_mask (const DsLogAdmin::WeekMask& masks) = 0;


  // = LogRecordStore status methods

  /// Gets the current size of the log data.
  virtual CORBA::ULongLong
    get_current_size (void) = 0;

  /// Get the number of records in the log right now.
  virtual CORBA::ULongLong
    get_n_records (void) = 0;


  // = LogRecordStore gauge

  /// Gets the current value of the "gauge" that measures the total
  /// size of the records written to the log.
  virtual CORBA::ULongLong
    get_gauge(void) = 0;

  /// Resets the "gauge" to 0
  virtual void
    reset_gauge(void) = 0;


  // = Record logging, retrieval, update and removal methods.

  /// Insert rec into storage.
  /// Returns 0 on success -1 on failure and 1 if the log is full.
  virtual int
    log (const DsLogAdmin::LogRecord &rec) = 0;

  /// Deletes "old" records from the store.
  virtual int
    purge_old_records (void) = 0;

  /// Set single record attributes.
  virtual void
    set_record_attribute (DsLogAdmin::RecordId id,
                          const DsLogAdmin::NVList & attr_list) = 0;

  /// Set the attributes of all records that matches the
  /// constraints with same attr_list.
  virtual CORBA::ULong
    set_records_attribute (const char * grammar,
                           const char * c,
                           const DsLogAdmin::NVList & attr_list) = 0;

  /// Get the attributes of the record with id <id>. Raises
  /// DsLogAdmin::InvalidRecordId
  virtual DsLogAdmin::NVList*
    get_record_attribute (DsLogAdmin::RecordId id) = 0;

  /// Ensure changes have been flushed to persistent media
  /// Returns 0 on success, -1 on failure
  virtual int
    flush (void) = 0;


  /// Returns all records in the log that match the given constraint
  /// <c>.
  virtual DsLogAdmin::RecordList*
    query (const char * grammar,
           const char * c,
           DsLogAdmin::Iterator_out i) = 0;

  /// Retrieve <how_many> records from time <from_time> using iterator
  /// <i>.
  virtual DsLogAdmin::RecordList*
    retrieve (DsLogAdmin::TimeT from_time,
              CORBA::Long how_many,
              DsLogAdmin::Iterator_out i) = 0;

  /// Returns the number of records matching constraint <c>.
  virtual CORBA::ULong
    match (const char * grammar,
           const char * c) = 0;

  /// Delete records matching constraint <c>.
  virtual CORBA::ULong
    delete_records (const char * grammar,
                    const char * c) = 0;

  /// Delete records matching ids in <ids>
  virtual CORBA::ULong
    delete_records_by_id (const DsLogAdmin::RecordIdList & ids) = 0;


  virtual CORBA::ULong
    remove_old_records (void) = 0;

  /// Read-Write Lock
  virtual ACE_SYNCH_RW_MUTEX& lock() = 0;

protected:
  /// Constructor.
  TAO_LogRecordStore (void);


private:
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /*TAO_LOG_RECORD_STORE_H*/
