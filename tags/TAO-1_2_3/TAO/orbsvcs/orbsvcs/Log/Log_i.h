/* -*- C++ -*- */
// $Id$

// ============================================================================
// = LIBRARY
//   orbsvcs
//
// = FILENAME
//   Log_i.h
//
// = DESCRIPTION
//   Implementation of the DsLogAdmin::Log interface.
//
// = AUTHOR
//    Matthew Braun (mjb2@cs.wustl.edu) and Pradeep Gore <pradeep@cs.wustl.edu>
//
// ============================================================================

#ifndef TLS_LOG_I_H
#define TLS_LOG_I_H
#include "ace/pre.h"

#include "orbsvcs/Log/LogRecordStore.h"
#include "orbsvcs/DsLogAdminS.h"
#include "ace/Reactor.h"
#include "log_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// Bug in MSVC 5, See KB article Q167350
#if defined (_MSC_VER) && (_MSC_VER == 1100)
using DsLogAdmin::wrap;
#endif /* (_MSC_VER) && (_MSC_VER == 1100) */

class TAO_Log_Export Log_i : public virtual POA_DsLogAdmin::Log
{
  // = TITLE
  //   Used to log events to a logging server
  //
  // = DESCRIPTION
  //   This implementation attempts to conform to the telecom
  //   logging specification.

public:
  // = Initialization and Termination
  Log_i (DsLogAdmin::LogMgr_ptr factory,
         DsLogAdmin::LogId id,
         DsLogAdmin::LogFullAction log_full_action = DsLogAdmin::wrap,
         CORBA::ULongLong max_size = 0,
         ACE_Reactor *reactor = ACE_Reactor::instance ());
  // Constructor.

  ~Log_i ();
  // Destructor.

  void init (ACE_ENV_SINGLE_ARG_DECL);
  // Initlialize the Log.

  // = DsLogAdmin::Log methods
  DsLogAdmin::LogMgr_ptr my_factory (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Return the factory of the log

  DsLogAdmin::LogId id (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Return the id of the log

  DsLogAdmin::QoSList*
  get_qos (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  // Get the list of the QoS properties supported by the log

  void set_qos (const DsLogAdmin::QoSList & qos
                ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     DsLogAdmin::UnsupportedQoS));
  // Set the list of the QoS properties supported by the log.

  CORBA::ULong
  get_max_record_life (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Get the record life in seconds (0 infinite)

  void
  set_max_record_life (CORBA::ULong life
                       ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Set the record life in seconds (0 infinite)

  CORBA::ULongLong
  get_max_size (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Get the maximum size in octets

  void
  set_max_size (CORBA::ULongLong size
                ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     DsLogAdmin::InvalidParam));
  // Set the maximum size in octets.

  CORBA::ULongLong
  get_current_size (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Get the current size of the log in octets.

  CORBA::ULongLong
  get_n_records (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Return the number of records in the log

  DsLogAdmin::LogFullAction
  get_log_full_action (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Get the action to be taken when the log reaches its maximum size

  void
  set_log_full_action (DsLogAdmin::LogFullAction action
                       ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Set the action to be taken when the log reaches its maximum size

  DsLogAdmin::AdministrativeState
  get_administrative_state (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Get the administrative state of the log.

  void
  set_administrative_state (DsLogAdmin::AdministrativeState state
                            ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Set the administrative state of the log.

  virtual DsLogAdmin::ForwardingState
  get_forwarding_state (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Get the forwarding state of the log.
  // @@ Note: is it ok to make this method virtual?

  void
    set_forwarding_state (DsLogAdmin::ForwardingState state
                          ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Set the forwarding state of the log.
  // @@ Note: is it ok to make this method virtual?

  DsLogAdmin::OperationalState
  get_operational_state (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Get the operational state of the log.

  DsLogAdmin::TimeInterval
  get_interval (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Get the log duration

  void
  set_interval (const DsLogAdmin::TimeInterval & interval
                ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     DsLogAdmin::InvalidTime,
                     DsLogAdmin::InvalidTimeInterval));
  // Set the log duration.

  DsLogAdmin::AvailabilityStatus
  get_availability_status (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Get the availability status

  DsLogAdmin::CapacityAlarmThresholdList*
    get_capacity_alarm_thresholds (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Get the capacity alarm threshold

  void
    set_capacity_alarm_thresholds
  (const DsLogAdmin::CapacityAlarmThresholdList & threshs
   ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     DsLogAdmin::InvalidThreshold));
  // Set the capacity alarm threshold. Threshold values represent
  // percentage levels at which the log should emit events to signal
  // that a log full or log wrap condition is approaching. If the log
  // is set to wrap, the thresholds are relative to the log size
  // between wraps (after a wrap, the percentage full resets to
  // 0).

  DsLogAdmin::WeekMask*
  get_week_mask (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Get the weekly scheduling parameters

  void
  set_week_mask (const DsLogAdmin::WeekMask & masks
                 ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     DsLogAdmin::InvalidTime,
                     DsLogAdmin::InvalidTimeInterval,
                     DsLogAdmin::InvalidMask));
  // Set the weekly scheduling parameters.

  DsLogAdmin::RecordList*
  query (const char * grammar,
         const char * c,
         DsLogAdmin::Iterator_out i
         ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     DsLogAdmin::InvalidGrammar,
                     DsLogAdmin::InvalidConstraint));
  // Returns all records in the log that match the given constraint
  // <c>.

  DsLogAdmin::RecordList*
  retrieve (DsLogAdmin::TimeT from_time,
            CORBA::Long how_many,
            DsLogAdmin::Iterator_out i
            ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Retrieve <how_many> records from time <from_time> using iterator
  // <i>.

  CORBA::ULong
  match (const char * grammar,
         const char * c
         ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     DsLogAdmin::InvalidGrammar,
                     DsLogAdmin::InvalidConstraint));
  // Returns the number of records matching constraint <c>.

  CORBA::ULong
  delete_records (const char * grammar,
                  const char * c
                  ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     DsLogAdmin::InvalidGrammar,
                     DsLogAdmin::InvalidConstraint));
  // Delete records matching constraint <c>.

  CORBA::ULong
  delete_records_by_id (const DsLogAdmin::RecordIdList & ids
                        ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Delete records matching ids in <ids>


  void
  write_records (const DsLogAdmin::Anys & records
                 ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     DsLogAdmin::LogFull,
                     DsLogAdmin::LogLocked));
  // Write records to the log storage.

  void
  write_recordlist (const DsLogAdmin::RecordList & list
                    ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     DsLogAdmin::LogFull,
                     DsLogAdmin::LogLocked));
  // Write a list of record ids to storage. Raises DsLogAdmin::LogFull
  // and DsLogAdmin::LogLocked

  void
  set_record_attribute (DsLogAdmin::RecordId id,
                        const DsLogAdmin::NVList & attr_list
                        ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     DsLogAdmin::InvalidRecordId,
                     DsLogAdmin::InvalidAttribute));
  // Set single record attributes.

  CORBA::ULong
  set_records_attribute (const char * grammar,
                         const char * c,
                         const DsLogAdmin::NVList & attr_list
                         ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     DsLogAdmin::InvalidGrammar,
                     DsLogAdmin::InvalidConstraint,
                     DsLogAdmin::InvalidAttribute));
  // Set the attributes of all records that matches the
  // constraints with same attr_list.

  DsLogAdmin::NVList*
  get_record_attribute (DsLogAdmin::RecordId id
                        ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     DsLogAdmin::InvalidRecordId));
  // Get the attributes of the record with id <id>. Raises
  // DsLogAdmin::InvalidRecordId

  DsLogAdmin::Log_ptr
  copy (DsLogAdmin::LogId_out id
        ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  // Copy the log on which the operation was invoked.

  DsLogAdmin::Log_ptr
  copy_with_id (DsLogAdmin::LogId id
                ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     DsLogAdmin::LogIdAlreadyExists));
  // Copy the log on which the operation was invoked and assign it the
  // given id.

  void
  flush (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     DsLogAdmin::UnsupportedQoS));
  // Causes all pending events to be written to storage. Raises
  // DsLogAdmin::UnsupportedQos

protected:
  // = Helper methods

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

  void check_grammar (const char* grammar ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     DsLogAdmin::InvalidGrammar));
  // Throws DsLogAdmin::InvalidGrammar if we don't support this grammar.

  void check_threshold_list (void);
  // This method checks to see if any threshold limit is reached.

  CORBA::Boolean scheduled (void);
  // Tells if the Log is scheduled to run now.

  // = Data Members
  DsLogAdmin::LogMgr_var factory_;
  // The factory of the log

  DsLogAdmin::LogFullAction log_full_action_;
  // The action to take if the log reaches max capacity

  DsLogAdmin::LogId logid_;
  // The id of the log

  CORBA::ULong max_record_life_;
  // The maximum record lifetime

  DsLogAdmin::AdministrativeState admin_state_;
  // The administrative state of the log

  DsLogAdmin::ForwardingState forward_state_;
  // The forwarding state of the log

  DsLogAdmin::OperationalState op_state_;
  // The operational state of the log

  DsLogAdmin::TimeInterval interval_;
  // The interval during which the log should be in operation

  DsLogAdmin::AvailabilityStatus avail_status_;
  // The availability of the log

  DsLogAdmin::CapacityAlarmThresholdList thresholds_;
  // The list of points at which the log should generate events
  // indicating approaching full capacity

  DsLogAdmin::WeekMask weekmask_;
  // The days of the week that the log should be operational

  DsLogAdmin::QoSList qoslist_;
  // The QoS list of the log

  ACE_Reactor *reactor_;
  // The reactor used for timing (startup and shutdown), etc

  LogRecordStore recordstore_;
  // The storage for all the records

  CORBA::ULong max_rec_list_len_;
  // Max count to return in queries.
};
#include "ace/post.h"
#endif /* TLS_LOG_I_H */
