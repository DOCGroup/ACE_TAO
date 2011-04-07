// -*- C++ -*-

//=============================================================================
/**
 *  @file   LogNotification.h
 *
 *  $Id$
 *
 *  Implementation of the DsLogAdmin::LogNotification interface.
 *
 *  @author D A Hanvey <d.hanvey@qub.ac.uk>
 */
//=============================================================================


#ifndef TAO_TLS_LOG_NOTIFICATION_H
#define TAO_TLS_LOG_NOTIFICATION_H

#include /**/ "ace/pre.h"

#include "orbsvcs/DsLogAdminS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/DsLogNotificationS.h"
#include "orbsvcs/Log/log_serv_export.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_LogNotification
 *
 * @brief Used to forward log generated events to a logging server
 *
 * This implementation attempts to conform to the telecom
 * logging specification.
 */
class TAO_Log_Serv_Export TAO_LogNotification
{

public:

  /// Constructor.
  TAO_LogNotification (void);

  /// Destructor.
  virtual ~TAO_LogNotification (void);

  /// Event generated when a new log is created.
  void object_creation (DsLogAdmin::LogId id);

  /// Event generated when a new log is deleted.
  void object_deletion (DsLogAdmin::LogId id);

  /// Event generated.
  void processing_error_alarm (CORBA::ULong error_num, const char* error_string);

  /// AttributeValueChange event generation method.
  void attribute_value_change (DsLogAdmin::Log_ptr log,
                        DsLogAdmin::LogId id,
                        DsLogNotification::AttributeType type,
                        CORBA::Any oldValue,
                        CORBA::Any newValue);

  /// AttributeValueChange event generation method.
  void capacity_alarm_threshold_value_change (DsLogAdmin::Log_ptr log,
                        DsLogAdmin::LogId id,
                        const DsLogAdmin::CapacityAlarmThresholdList& oldValue,
                        const DsLogAdmin::CapacityAlarmThresholdList& newValue);

  /// AttributeValueChange event generation method.
  void log_full_action_value_change (DsLogAdmin::Log_ptr log,
                              DsLogAdmin::LogId id,
                              CORBA::ULong oldValue,
                              CORBA::ULong newValue);

  /// AttributeValueChange event generation method.
  void max_log_size_value_change (DsLogAdmin::Log_ptr log,
                              DsLogAdmin::LogId id,
                              CORBA::ULongLong oldValue,
                              CORBA::ULongLong newValue);

  /// AttributeValueChange event generation method.
  void start_time_value_change (DsLogAdmin::Log_ptr log,
                              DsLogAdmin::LogId id,
                              DsLogAdmin::TimeT oldValue,
                              DsLogAdmin::TimeT newValue);

  /// AttributeValueChange event generation method.
  void stop_time_value_change (DsLogAdmin::Log_ptr log,
                              DsLogAdmin::LogId id,
                              DsLogAdmin::TimeT oldValue,
                              DsLogAdmin::TimeT newValue);

  /// AttributeValueChange event generation method.
  void week_mask_value_change (DsLogAdmin::Log_ptr log,
                              DsLogAdmin::LogId id,
                              const DsLogAdmin::WeekMask& oldValue,
                              const DsLogAdmin::WeekMask& newValue);

  /// AttributeValueChange event generation method.
  void max_record_life_value_change (DsLogAdmin::Log_ptr log,
                              DsLogAdmin::LogId id,
                              CORBA::ULong oldValue,
                              CORBA::ULong newValue);

  /// AttributeValueChange event generation method.
  void quality_of_service_value_change (DsLogAdmin::Log_ptr log,
                              DsLogAdmin::LogId id,
                              const DsLogAdmin::QoSList& oldValue,
                              const DsLogAdmin::QoSList& newValue);

  /// StateChange event generation method.
  void state_change (DsLogAdmin::Log_ptr log,
                    DsLogAdmin::LogId id,
                    DsLogNotification::StateType type,
                    CORBA::Any newValue);

  /// StateChange event generation method.
  void administrative_state_change (DsLogAdmin::Log_ptr log,
                    DsLogAdmin::LogId id,
                    DsLogAdmin::AdministrativeState newValue);

  /// StateChange event generation method.
  void operational_state_change (DsLogAdmin::Log_ptr log,
                    DsLogAdmin::LogId id,
                    DsLogAdmin::OperationalState newValue);

  /// StateChange event generation method.
  void forwarding_state_change (DsLogAdmin::Log_ptr log,
                    DsLogAdmin::LogId id,
                    DsLogAdmin::ForwardingState newValue);

  /// StateChange event generation method.
  void threshold_alarm (DsLogAdmin::Log_ptr log,
                    DsLogAdmin::LogId id,
                    DsLogAdmin::Threshold crossedValue,
                    DsLogAdmin::Threshold observedValue,
                    DsLogNotification::PerceivedSeverityType severity);

  /// Method overriden by by event-aware logging services to cause
  /// a notification event to be sent.
  virtual void send_notification (const CORBA::Any &);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif  /* TAO_TLS_LOG_NOTIFICATION_H */
