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

#include "ace/pre.h"

#include "orbsvcs/Log/LogRecordStore.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Log/LogNotification.h"
#include "orbsvcs/DsLogAdminS.h"
#include "ace/Reactor.h"
#include "log_export.h"

/**
 * @class TAO_Log_i
 *
 * @brief Used to log events to a logging server
 *
 * This implementation attempts to conform to the telecom
 * logging specification.
 */
class TAO_Log_Export TAO_Log_i : public virtual POA_DsLogAdmin::Log
{

public:

  /// Constructor.
  TAO_Log_i (DsLogAdmin::LogMgr_ptr factory,
             DsLogAdmin::LogId id,
             TAO_LogNotification *log_notifier,
             DsLogAdmin::LogFullActionType log_full_action = DsLogAdmin::wrap,
             CORBA::ULongLong max_size = 0,
             ACE_Reactor *reactor = ACE_Reactor::instance ());

  /// Destructor.
  ~TAO_Log_i ();

  /// Initlialize the Log.
  void init (ACE_ENV_SINGLE_ARG_DECL);

  /**
   * @name DsLogAdmin::Log Methods
   *
   * DsLogAdmin::Log methods.
   */
  //@{
  /// Return the factory of the log.
  DsLogAdmin::LogMgr_ptr my_factory (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Return the id of the log.
  DsLogAdmin::LogId id (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Get the list of the QoS properties supported by the log.
  DsLogAdmin::QoSList*
  get_log_qos (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Set the list of the QoS properties supported by the log.
  void set_log_qos (const DsLogAdmin::QoSList & qos
                ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     DsLogAdmin::UnsupportedQoS));

  /// Get the record life in seconds (0 infinite).
  CORBA::ULong
  get_max_record_life (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Set the record life in seconds (0 infinite).
  void
  set_max_record_life (CORBA::ULong life
                       ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Get the maximum size in octets.
  CORBA::ULongLong
  get_max_size (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Set the maximum size in octets.
  void
  set_max_size (CORBA::ULongLong size
                ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     DsLogAdmin::InvalidParam));

  /// Get the current size of the log in octets.
  CORBA::ULongLong
  get_current_size (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Return the number of records in the log.
  CORBA::ULongLong
  get_n_records (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Get the action to be taken when the log reaches its maximum size.
  DsLogAdmin::LogFullActionType
  get_log_full_action (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Set the action to be taken when the log reaches its maximum size.
  void
  set_log_full_action (DsLogAdmin::LogFullActionType action
                       ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Get the administrative state of the log.
  DsLogAdmin::AdministrativeState
  get_administrative_state (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Set the administrative state of the log.
  void
  set_administrative_state (DsLogAdmin::AdministrativeState state
                            ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Get the forwarding state of the log.
  /// @@ Note: is it ok to make this method virtual?
  /// @@@ Of course!  get_forwarding_state() is virtual in the base
  ///     class dictated by the IDL!  -Ossama
  virtual DsLogAdmin::ForwardingState
  get_forwarding_state (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Set the forwarding state of the log.
  /// @@ Note: is it ok to make this method virtual?
  void
    set_forwarding_state (DsLogAdmin::ForwardingState state
                          ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Get the operational state of the log.
  DsLogAdmin::OperationalState
  get_operational_state (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Get the log duration
  DsLogAdmin::TimeInterval
  get_interval (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Set the log duration.
  void
  set_interval (const DsLogAdmin::TimeInterval & interval
                ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     DsLogAdmin::InvalidTime,
                     DsLogAdmin::InvalidTimeInterval));

  /// Get the availability status
  DsLogAdmin::AvailabilityStatus
  get_availability_status (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Get the capacity alarm threshold
  DsLogAdmin::CapacityAlarmThresholdList*
    get_capacity_alarm_thresholds (ACE_ENV_SINGLE_ARG_DECL)
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
  (const DsLogAdmin::CapacityAlarmThresholdList & threshs
   ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     DsLogAdmin::InvalidThreshold));

  /// Get the weekly scheduling parameters
  DsLogAdmin::WeekMask*
  get_week_mask (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Set the weekly scheduling parameters.
  void
  set_week_mask (const DsLogAdmin::WeekMask & masks
                 ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     DsLogAdmin::InvalidTime,
                     DsLogAdmin::InvalidTimeInterval,
                     DsLogAdmin::InvalidMask));

  /// Returns all records in the log that match the given constraint
  /// <c>.
  DsLogAdmin::RecordList*
  query (const char * grammar,
         const char * c,
         DsLogAdmin::Iterator_out i
         ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     DsLogAdmin::InvalidGrammar,
                     DsLogAdmin::InvalidConstraint));

  /// Retrieve <how_many> records from time <from_time> using iterator
  /// <i>.
  DsLogAdmin::RecordList*
  retrieve (DsLogAdmin::TimeT from_time,
            CORBA::Long how_many,
            DsLogAdmin::Iterator_out i
            ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Returns the number of records matching constraint <c>.
  CORBA::ULong
  match (const char * grammar,
         const char * c
         ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     DsLogAdmin::InvalidGrammar,
                     DsLogAdmin::InvalidConstraint));

  /// Delete records matching constraint <c>.
  CORBA::ULong
  delete_records (const char * grammar,
                  const char * c
                  ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     DsLogAdmin::InvalidGrammar,
                     DsLogAdmin::InvalidConstraint));

  /// Delete records matching ids in <ids>
  CORBA::ULong
  delete_records_by_id (const DsLogAdmin::RecordIdList & ids
                        ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));


  /// Write records to the log storage.
  void
  write_records (const DsLogAdmin::Anys & records
                 ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     DsLogAdmin::LogFull,
                     DsLogAdmin::LogLocked,
                     DsLogAdmin::LogDisabled));

  /// Write a list of record ids to storage. Raises DsLogAdmin::LogFull
  /// and DsLogAdmin::LogLocked
  void
  write_recordlist (const DsLogAdmin::RecordList & list
                    ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     DsLogAdmin::LogFull,
                     DsLogAdmin::LogLocked,
                     DsLogAdmin::LogDisabled));

  /// Set single record attributes.
  void
  set_record_attribute (DsLogAdmin::RecordId id,
                        const DsLogAdmin::NVList & attr_list
                        ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     DsLogAdmin::InvalidRecordId,
                     DsLogAdmin::InvalidAttribute));

  /// Set the attributes of all records that matches the
  /// constraints with same attr_list.
  CORBA::ULong
  set_records_attribute (const char * grammar,
                         const char * c,
                         const DsLogAdmin::NVList & attr_list
                         ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     DsLogAdmin::InvalidGrammar,
                     DsLogAdmin::InvalidConstraint,
                     DsLogAdmin::InvalidAttribute));

  /// Get the attributes of the record with id <id>. Raises
  /// DsLogAdmin::InvalidRecordId
  DsLogAdmin::NVList*
  get_record_attribute (DsLogAdmin::RecordId id
                        ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     DsLogAdmin::InvalidRecordId));

  /// Causes all pending events to be written to storage. Raises
  /// DsLogAdmin::UnsupportedQos
  void
  flush (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     DsLogAdmin::UnsupportedQoS));
  //@}

  /// Check that valid threshold values have been given.
  static CORBA::Boolean validate_capacity_alarm_thresholds (
    const DsLogAdmin::CapacityAlarmThresholdList & threshs
   ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

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

  /// Throws DsLogAdmin::InvalidGrammar if we don't support this grammar.
  void check_grammar (const char* grammar ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     DsLogAdmin::InvalidGrammar));

  /// Tells if the Log is scheduled to run now.
  CORBA::Boolean scheduled (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Copy the attributes of the log being passed.
  void copy_attributes (DsLogAdmin::Log_ptr log
                        ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Remove records that have exceeded max_record_life_.
  void remove_old_records (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Check if threshold reached.
  void check_capacity_alarm_threshold (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Reset capacity alarm threshold.
  void reset_capacity_alarm_threshold (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

protected:

  /// The factory of the log
  DsLogAdmin::LogMgr_var factory_;

  /// The action to take if the log reaches max capacity
  DsLogAdmin::LogFullActionType log_full_action_;

  /// The id of the log
  DsLogAdmin::LogId logid_;

  /// The maximum record lifetime
  CORBA::ULong max_record_life_;

  /// The administrative state of the log
  DsLogAdmin::AdministrativeState admin_state_;

  /// The forwarding state of the log
  DsLogAdmin::ForwardingState forward_state_;

  /// The operational state of the log
  DsLogAdmin::OperationalState op_state_;

  /// The interval during which the log should be in operation
  DsLogAdmin::TimeInterval interval_;

  /// The availability of the log
  DsLogAdmin::AvailabilityStatus avail_status_;

  /// The list of points at which the log should generate events
  /// indicating approaching full capacity
  DsLogAdmin::CapacityAlarmThresholdList thresholds_;

  /// The next capacity alarm threshold.
  CORBA::ULong current_threshold_;

  /// The days of the week that the log should be operational
  DsLogAdmin::WeekMask weekmask_;

  /// A Sequence of the weekly intervals when the log is in operation.
  TAO_Unbounded_Sequence<DsLogAdmin::TimeInterval> weekly_intervals_;

  /// The QoS list of the log
  DsLogAdmin::QoSList qoslist_;

  /// The reactor used for timing (startup and shutdown), etc
  ACE_Reactor *reactor_;

  /// The storage for all the records
  TAO_LogRecordStore recordstore_;

  /// Max count to return in queries.
  CORBA::ULong max_rec_list_len_;

  /// For sending Log Generated events.
  TAO_LogNotification* notifier_;
};

#include "ace/post.h"

#endif  /* TAO_TLS_LOG_I_H */
