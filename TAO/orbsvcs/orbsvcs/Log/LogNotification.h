/* -*- C++ -*- */
// $Id$

// ============================================================================
// = LIBRARY
//   orbsvcs
//
// = FILENAME
//   LogNotification_i.h
//
// = DESCRIPTION
//   Implementation of the DsLogAdmin::LogNotification interface.
//
// = AUTHOR
//   D A Hanvey <d.hanvey@qub.ac.uk>
//
// ============================================================================

#ifndef TLS_LOG_NOTIFICATION_H
#define TLS_LOG_NOTIFICATION_H
#include "ace/pre.h"

#include "orbsvcs/DsLogAdminS.h"
#include "orbsvcs/DsLogAdminC.h"

//#include "orbsvcs/DsEventLogAdminS.h"
//#include "orbsvcs/DsEventLogAdminC.h"
#include "orbsvcs/DsLogNotificationS.h"
#include "orbsvcs/DsLogNotificationC.h"

#include "log_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


class TAO_Log_Export LogNotification 
{
  // = TITLE
  //   Used to forward log generated events to a logging server
  //
  // = DESCRIPTION
  //   This implementation attempts to conform to the telecom
  //   logging specification.

public:
  // = Initialization and Termination
  LogNotification (void);
  // Constructor.

  virtual ~LogNotification (void);
  // Destructor.

  void object_creation (DsLogAdmin::Log_ptr log, DsLogAdmin::LogId id
                        ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Event generated when a new log is created.

  void object_deletion (DsLogAdmin::LogId id ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Event generated when a new log is deleted.

  void processing_error_alarm (CORBA::ULong error_num, const char* error_string
                               ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Event generated.

  void attribute_value_change (DsLogAdmin::Log_ptr log,
                        DsLogAdmin::LogId id,
                        DsLogNotification::AttributeType type,
                        CORBA::Any oldValue,
                        CORBA::Any newValue ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // AttributeValueChange event generation method.

  void capacity_alarm_threshold_value_change (DsLogAdmin::Log_ptr log,
                        DsLogAdmin::LogId id,
                        const DsLogAdmin::CapacityAlarmThresholdList& oldValue,
                        const DsLogAdmin::CapacityAlarmThresholdList& newValue
                        ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // AttributeValueChange event generation method.

  void log_full_action_value_change (DsLogAdmin::Log_ptr log,
                              DsLogAdmin::LogId id,
                              CORBA::ULong oldValue,
                              CORBA::ULong newValue
                              ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // AttributeValueChange event generation method.

  void max_log_size_value_change (DsLogAdmin::Log_ptr log,
                              DsLogAdmin::LogId id,
                              CORBA::ULongLong oldValue,
                              CORBA::ULongLong newValue
                              ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // AttributeValueChange event generation method.

  void start_time_value_change (DsLogAdmin::Log_ptr log,
                              DsLogAdmin::LogId id,
                              DsLogAdmin::TimeT oldValue,
                              DsLogAdmin::TimeT newValue
                              ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // AttributeValueChange event generation method.

  void stop_time_value_change (DsLogAdmin::Log_ptr log,
                              DsLogAdmin::LogId id,
                              DsLogAdmin::TimeT oldValue,
                              DsLogAdmin::TimeT newValue
                              ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // AttributeValueChange event generation method.    

  void week_mask_value_change (DsLogAdmin::Log_ptr log,
                              DsLogAdmin::LogId id,
                              const DsLogAdmin::WeekMask& oldValue,
                              const DsLogAdmin::WeekMask& newValue
                              ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // AttributeValueChange event generation method.

  void max_record_life_value_change (DsLogAdmin::Log_ptr log,
                              DsLogAdmin::LogId id,
                              CORBA::ULong oldValue,
                              CORBA::ULong newValue
                              ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // AttributeValueChange event generation method.

  void quality_of_service_value_change (DsLogAdmin::Log_ptr log,
                              DsLogAdmin::LogId id,
                              const DsLogAdmin::QoSList& oldValue,
                              const DsLogAdmin::QoSList& newValue
                              ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // AttributeValueChange event generation method.

  void state_change (DsLogAdmin::Log_ptr log,
                    DsLogAdmin::LogId id,
                    DsLogNotification::StateType type,
                    CORBA::Any newValue
                    ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // StateChange event generation method.

  void administrative_state_change (DsLogAdmin::Log_ptr log,
                    DsLogAdmin::LogId id,
                    DsLogAdmin::AdministrativeState newValue
                    ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // StateChange event generation method.

  void operational_state_change (DsLogAdmin::Log_ptr log,
                    DsLogAdmin::LogId id,
                    DsLogAdmin::OperationalState newValue
                    ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // StateChange event generation method.

  void forwarding_state_change (DsLogAdmin::Log_ptr log,
                    DsLogAdmin::LogId id,
                    DsLogAdmin::ForwardingState newValue
                    ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // StateChange event generation method.

  void threshold_alarm (DsLogAdmin::Log_ptr log,
                    DsLogAdmin::LogId id,
                    DsLogAdmin::Threshold crossedValue,
                    DsLogAdmin::Threshold observedValue,
                    DsLogNotification::PerceivedSeverityType severity
                    ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // StateChange event generation method.

  // = Helper methods

  virtual void send_notification (const CORBA::Any& any ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException)){ACE_UNUSED_ARG (any);};
  // Method overriden by by event-aware logging services to cause
  // a notification event to be sent.
};
#include "ace/post.h"
#endif /* TLS_LOG_NOTIFICATION_H */
