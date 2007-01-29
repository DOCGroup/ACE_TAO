// -*- C++ -*-

//=============================================================================
/**
 *  @file   Log_i.h
 *
 *  $Id$
 *
 *  Implementation of the DsLogAdmin::Log interface.
 *
 *  @author Matthew Braun <mjb2@cs.wustl.edu>
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 *  @author David A. Hanvey <d.hanvey@qub.ac.uk>
 */
//=============================================================================


#ifndef TAO_TLS_LOG_I_H
#define TAO_TLS_LOG_I_H

#include /**/ "ace/pre.h"

#include "orbsvcs/Log/LogRecordStore.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Log/Log_Compaction_Handler.h"
#include "orbsvcs/Log/Log_Flush_Handler.h"
#include "orbsvcs/Log/LogNotification.h"
#include "orbsvcs/Log/LogRecordStore.h"
#include "orbsvcs/DsLogAdminS.h"
#include "ace/Reactor.h"
#include "orbsvcs/Log/log_serv_export.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_LogMgr_i;

/**
 * @class TAO_Log_i
 *
 * @brief Used to log events to a logging server
 *
 * This implementation attempts to conform to the telecom
 * logging specification.
 */
class TAO_Log_Serv_Export TAO_Log_i : public virtual POA_DsLogAdmin::Log
{

public:

  /// Constructor.
  TAO_Log_i (CORBA::ORB_ptr orb,
             TAO_LogMgr_i& logmgr_i,
             DsLogAdmin::LogMgr_ptr factory,
             DsLogAdmin::LogId id,
             TAO_LogNotification *log_notifier);

  /// Destructor.
  ~TAO_Log_i ();

  /// Initlialize the Log.
  void init (void);

  /**
   * @name DsLogAdmin::Log Methods
   *
   * DsLogAdmin::Log methods.
   */
  //@{
  /// Return the factory of the log.
  DsLogAdmin::LogMgr_ptr my_factory (void)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Return the id of the log.
  DsLogAdmin::LogId id (void)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Get the list of the QoS properties supported by the log.
  DsLogAdmin::QoSList*
  get_log_qos (void)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Set the list of the QoS properties supported by the log.
  void set_log_qos (const DsLogAdmin::QoSList & qos)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     DsLogAdmin::UnsupportedQoS));

  /// Get the record life in seconds (0 infinite).
  CORBA::ULong
  get_max_record_life (void)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Set the record life in seconds (0 infinite).
  void
  set_max_record_life (CORBA::ULong life)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Get the maximum size in octets.
  CORBA::ULongLong
  get_max_size (void)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Set the maximum size in octets.
  void
  set_max_size (CORBA::ULongLong size)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     DsLogAdmin::InvalidParam));

  /// Get the current size of the log in octets.
  CORBA::ULongLong
  get_current_size (void)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Return the number of records in the log.
  CORBA::ULongLong
  get_n_records (void)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Get the action to be taken when the log reaches its maximum size.
  DsLogAdmin::LogFullActionType
  get_log_full_action (void)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Set the action to be taken when the log reaches its maximum size.
  void
  set_log_full_action (DsLogAdmin::LogFullActionType action)
    ACE_THROW_SPEC ((CORBA::SystemException,
		     DsLogAdmin::InvalidLogFullAction));

  /// Get the administrative state of the log.
  DsLogAdmin::AdministrativeState
  get_administrative_state (void)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Set the administrative state of the log.
  void
  set_administrative_state (DsLogAdmin::AdministrativeState state)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Get the forwarding state of the log.
  /// @@ Note: is it ok to make this method virtual?
  /// @@@ Of course!  get_forwarding_state() is virtual in the base
  ///     class dictated by the IDL!  -Ossama
  virtual DsLogAdmin::ForwardingState
  get_forwarding_state (void)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Set the forwarding state of the log.
  /// @@ Note: is it ok to make this method virtual?
  void
    set_forwarding_state (DsLogAdmin::ForwardingState state)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Get the operational state of the log.
  DsLogAdmin::OperationalState
  get_operational_state (void)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Get the log duration
  DsLogAdmin::TimeInterval
  get_interval (void)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Set the log duration.
  void
  set_interval (const DsLogAdmin::TimeInterval & interval)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     DsLogAdmin::InvalidTime,
                     DsLogAdmin::InvalidTimeInterval));

  /// Get the availability status
  DsLogAdmin::AvailabilityStatus
  get_availability_status (void)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Get the capacity alarm threshold
  DsLogAdmin::CapacityAlarmThresholdList*
    get_capacity_alarm_thresholds (void)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /**
   * Set the capacity alarm threshold. Threshold values represent
   * percentage levels at which the log should emit events to signal
   * that a log full or log wrap condition is approaching. If the log
   * is set to wrap, the thresholds are relative to the log size
   * between wraps (after a wrap, the percentage full resets to
   * 0).
   */
  void
  set_capacity_alarm_thresholds
  (const DsLogAdmin::CapacityAlarmThresholdList & threshs)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     DsLogAdmin::InvalidThreshold));

  /// Get the weekly scheduling parameters
  DsLogAdmin::WeekMask*
  get_week_mask (void)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Set the weekly scheduling parameters.
  void
  set_week_mask (const DsLogAdmin::WeekMask & masks)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     DsLogAdmin::InvalidTime,
                     DsLogAdmin::InvalidTimeInterval,
                     DsLogAdmin::InvalidMask));

  /// Returns all records in the log that match the given constraint
  /// <c>.
  DsLogAdmin::RecordList*
  query (const char * grammar,
         const char * c,
         DsLogAdmin::Iterator_out i)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     DsLogAdmin::InvalidGrammar,
                     DsLogAdmin::InvalidConstraint));

  /// Retrieve <how_many> records from time <from_time> using iterator
  /// <i>.
  DsLogAdmin::RecordList*
  retrieve (DsLogAdmin::TimeT from_time,
            CORBA::Long how_many,
            DsLogAdmin::Iterator_out i)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Returns the number of records matching constraint <c>.
  CORBA::ULong
  match (const char * grammar,
         const char * c)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     DsLogAdmin::InvalidGrammar,
                     DsLogAdmin::InvalidConstraint));

  /// Delete records matching constraint <c>.
  CORBA::ULong
  delete_records (const char * grammar,
                  const char * c)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     DsLogAdmin::InvalidGrammar,
                     DsLogAdmin::InvalidConstraint));

  /// Delete records matching ids in <ids>
  CORBA::ULong
  delete_records_by_id (const DsLogAdmin::RecordIdList & ids)
    ACE_THROW_SPEC ((CORBA::SystemException));


  /// Write records to the log storage.
  void
  write_records (const DsLogAdmin::Anys & records)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     DsLogAdmin::LogFull,
                     DsLogAdmin::LogOffDuty,
                     DsLogAdmin::LogLocked,
                     DsLogAdmin::LogDisabled));

  /// Write a list of record ids to storage. Raises DsLogAdmin::LogFull
  /// and DsLogAdmin::LogLocked
  void
  write_recordlist (const DsLogAdmin::RecordList & list)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     DsLogAdmin::LogFull,
                     DsLogAdmin::LogOffDuty,
                     DsLogAdmin::LogLocked,
                     DsLogAdmin::LogDisabled));

  /// Set single record attributes.
  void
  set_record_attribute (DsLogAdmin::RecordId id,
                        const DsLogAdmin::NVList & attr_list)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     DsLogAdmin::InvalidRecordId,
                     DsLogAdmin::InvalidAttribute));

  /// Set the attributes of all records that matches the
  /// constraints with same attr_list.
  CORBA::ULong
  set_records_attribute (const char * grammar,
                         const char * c,
                         const DsLogAdmin::NVList & attr_list)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     DsLogAdmin::InvalidGrammar,
                     DsLogAdmin::InvalidConstraint,
                     DsLogAdmin::InvalidAttribute));

  /// Get the attributes of the record with id <id>. Raises
  /// DsLogAdmin::InvalidRecordId
  DsLogAdmin::NVList*
  get_record_attribute (DsLogAdmin::RecordId id)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     DsLogAdmin::InvalidRecordId));

  /// Causes all pending events to be written to storage. Raises
  /// DsLogAdmin::UnsupportedQos
  void
  flush (void)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     DsLogAdmin::UnsupportedQoS));
  //@}

  /// Remove records that have exceeded max_record_life_.
  void remove_old_records (void)
    ACE_THROW_SPEC ((CORBA::SystemException));

protected:
  /// Get the availability status
  /// @note must be called with locks held
  DsLogAdmin::AvailabilityStatus
  get_availability_status_i (void)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Tells if the Log is scheduled to run now.
  /// @note must be called with locks held
  CORBA::Boolean scheduled (void)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Copy the attributes of the log being passed.
  void copy_attributes (DsLogAdmin::Log_ptr log)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Check if threshold reached.
  void check_capacity_alarm_threshold (void)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Reset capacity alarm threshold.
  void reset_capacity_alarm_threshold (void)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Check that valid threshold values have been given.
  static CORBA::Boolean validate_capacity_alarm_thresholds (
    const DsLogAdmin::CapacityAlarmThresholdList & threshs)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Reset Log QoS
  void reset_log_qos (const DsLogAdmin::QoSList& qos);

  /// Validate log QoS
  void validate_log_qos (const DsLogAdmin::QoSList& qos)
    ACE_THROW_SPEC ((DsLogAdmin::UnsupportedQoS));

  /// Reset Week Mask
  ///
  /// Used to initialize internal data structures that represent the
  /// week mask list when the log service starts, and to reinitialize
  /// them when they are changed.
  ///
  void reset_week_mask (const DsLogAdmin::WeekMask& masks);

  /// Validate Week Mask
  ///
  /// Used to check whether week mask is valid.  If not, throws an
  /// InvalidTime, InvalidTimeInterval, or InvalidMask exception.
  ///
  void validate_week_mask (const DsLogAdmin::WeekMask& masks)
    ACE_THROW_SPEC ((DsLogAdmin::InvalidTime,
                     DsLogAdmin::InvalidTimeInterval,
                     DsLogAdmin::InvalidMask));

protected:
  /// Reference to the LogMgr servant
  TAO_LogMgr_i& logmgr_i_;

  /// The log's object reference
  DsLogAdmin::Log_var log_;

  /// The log's factory's object reference
  DsLogAdmin::LogMgr_var factory_;

  /// The log's id
  DsLogAdmin::LogId logid_;

  /// The log's operational state
  DsLogAdmin::OperationalState op_state_;

  /// The log's availability status
  DsLogAdmin::AvailabilityStatus avail_status_;

  /// The list of points at which the log should generate events
  /// indicating approaching full capacity
  DsLogAdmin::CapacityAlarmThresholdList thresholds_;

  /// The next capacity alarm threshold.
  CORBA::ULong current_threshold_;

  /// A Sequence of the weekly intervals when the log is in operation.
  DsLogAdmin::TimeIntervalSeq weekly_intervals_;

  /// The QoS type of the log
  DsLogAdmin::QoSType qostype_;

  /// The reactor used for timing (startup and shutdown), etc
  ACE_Reactor *reactor_;

  /// The storage for all the records
  TAO_LogRecordStore*		recordstore_;

  /// For sending Log Generated events.
  TAO_LogNotification* 		notifier_;

  /// Log Compaction Handler
  TAO_Log_Compaction_Handler	log_compaction_handler_;

  /// Log Compaction Interval
  static const ACE_Time_Value	log_compaction_interval_;

  /// Log Flush Handler
  TAO_Log_Flush_Handler		log_flush_handler_;

  /// Log Flush Interval
  static const ACE_Time_Value	log_flush_interval_;
};

bool TAO_Log_Serv_Export
operator==(const DsLogAdmin::CapacityAlarmThresholdList& rhs,
           const DsLogAdmin::CapacityAlarmThresholdList& lhs);

bool TAO_Log_Serv_Export
operator!=(const DsLogAdmin::CapacityAlarmThresholdList& rhs,
           const DsLogAdmin::CapacityAlarmThresholdList& lhs);

bool TAO_Log_Serv_Export
operator==(const DsLogAdmin::IntervalsOfDay& rhs,
           const DsLogAdmin::IntervalsOfDay& lhs);

bool TAO_Log_Serv_Export
operator!=(const DsLogAdmin::IntervalsOfDay& rhs,
           const DsLogAdmin::IntervalsOfDay& lhs);

bool TAO_Log_Serv_Export
operator==(const DsLogAdmin::QoSList& rhs,
           const DsLogAdmin::QoSList& lhs);

bool TAO_Log_Serv_Export
operator!=(const DsLogAdmin::QoSList& rhs,
           const DsLogAdmin::QoSList& lhs);

bool TAO_Log_Serv_Export
operator==(const DsLogAdmin::Time24& rhs,
           const DsLogAdmin::Time24& lhs);

bool TAO_Log_Serv_Export
operator!=(const DsLogAdmin::Time24& rhs,
           const DsLogAdmin::Time24& lhs);

bool TAO_Log_Serv_Export
operator==(const DsLogAdmin::Time24Interval& rhs,
           const DsLogAdmin::Time24Interval& lhs);

bool TAO_Log_Serv_Export
operator!=(const DsLogAdmin::Time24Interval& rhs,
           const DsLogAdmin::Time24Interval& lhs);

bool TAO_Log_Serv_Export
operator==(const DsLogAdmin::TimeInterval& rhs,
           const DsLogAdmin::TimeInterval& lhs);

bool TAO_Log_Serv_Export
operator!=(const DsLogAdmin::TimeInterval& rhs,
           const DsLogAdmin::TimeInterval& lhs);

bool TAO_Log_Serv_Export
operator==(const DsLogAdmin::WeekMaskItem& rhs,
           const DsLogAdmin::WeekMaskItem& lhs);

bool TAO_Log_Serv_Export
operator!=(const DsLogAdmin::WeekMaskItem& rhs,
           const DsLogAdmin::WeekMaskItem& lhs);

bool TAO_Log_Serv_Export
operator==(const DsLogAdmin::WeekMask& rhs,
           const DsLogAdmin::WeekMask& lhs);

bool TAO_Log_Serv_Export
operator!=(const DsLogAdmin::WeekMask& rhs,
           const DsLogAdmin::WeekMask& lhs);

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif  /* TAO_TLS_LOG_I_H */
