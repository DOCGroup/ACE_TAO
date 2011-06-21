// $Id$

#include "orbsvcs/Log/LogNotification.h"

#include "orbsvcs/Time_Utilities.h"
#include "tao/debug.h"
#include "ace/OS_NS_sys_time.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_LogNotification::TAO_LogNotification (void)
{
}

TAO_LogNotification::~TAO_LogNotification (void)
{
}

void
TAO_LogNotification::object_creation (DsLogAdmin::LogId id)
{

  CORBA::Any any;
  DsLogNotification::ObjectCreation event;

  // The log id.
  event.id = id;

  // Time object created.
  event.time = ORBSVCS_Time::to_Absolute_TimeT (ACE_OS::gettimeofday ());

  any <<= event;

  this->send_notification (any);
}

void
TAO_LogNotification::object_deletion (DsLogAdmin::LogId id)
{
  CORBA::Any any;
  DsLogNotification::ObjectDeletion event;

  // The log id.
  event.id = id;

  // Time object deleted.
  event.time = ORBSVCS_Time::to_Absolute_TimeT (ACE_OS::gettimeofday ());

  any <<= event;

  this->send_notification (any);
}

void
TAO_LogNotification::processing_error_alarm (CORBA::ULong error_num,
                                             const char* error_string)
{
  CORBA::Any any;
  DsLogNotification::ProcessingErrorAlarm event;

  event.error_num = error_num;
  event.error_string = CORBA::string_dup (error_string);

  any <<= event;

  this->send_notification (any);
}

void
TAO_LogNotification::attribute_value_change (DsLogAdmin::Log_ptr log,
                                             DsLogAdmin::LogId id,
                                             DsLogNotification::AttributeType type,
                                             CORBA::Any oldValue,
                                             CORBA::Any newValue)
{
  CORBA::Any any;
  DsLogNotification::AttributeValueChange event;

  event.logref = DsLogAdmin::Log::_duplicate (log);
  event.id = id;
  event.time = ORBSVCS_Time::to_Absolute_TimeT (ACE_OS::gettimeofday ());

  // The attribute type e.g. logFullAction, maxLogSize etc.
  event.type = type;
  event.old_value = oldValue;
  event.new_value = newValue;

  any <<= event;

  this->send_notification (any);
}

void
TAO_LogNotification::capacity_alarm_threshold_value_change (
    DsLogAdmin::Log_ptr log,
    DsLogAdmin::LogId id,
    const DsLogAdmin::CapacityAlarmThresholdList& oldValue,
    const DsLogAdmin::CapacityAlarmThresholdList& newValue)
{
  CORBA::Any oldV, newV;
  oldV <<= oldValue;
  newV <<= newValue;

  this->attribute_value_change (log,
                                id,
                                DsLogNotification::capacityAlarmThreshold,
                                oldV,
                                newV);
}

void
TAO_LogNotification::log_full_action_value_change (DsLogAdmin::Log_ptr log,
                                                   DsLogAdmin::LogId id,
                                                   CORBA::ULong oldValue,
                                                   CORBA::ULong newValue)
{
  CORBA::Any oldV, newV;
  oldV <<= oldValue;
  newV <<= newValue;

  this->attribute_value_change (log,
                                id,
                                DsLogNotification::logFullAction,
                                oldV,
                                newV);
}

void
TAO_LogNotification::max_log_size_value_change (DsLogAdmin::Log_ptr log,
                                                DsLogAdmin::LogId id,
                                                CORBA::ULongLong oldValue,
                                                CORBA::ULongLong newValue)
{
  CORBA::Any oldV, newV;
  oldV <<= oldValue;
  newV <<= newValue;

  this->attribute_value_change (log,
                                id,
                                DsLogNotification::maxLogSize,
                                oldV,
                                newV);
}

void
TAO_LogNotification::start_time_value_change (DsLogAdmin::Log_ptr log,
                                              DsLogAdmin::LogId id,
                                              DsLogAdmin::TimeT oldValue,
                                              DsLogAdmin::TimeT newValue)
{
  CORBA::Any oldV, newV;
  oldV <<= oldValue;
  newV <<= newValue;

  this->attribute_value_change (log,
                                id,
                                DsLogNotification::startTime,
                                oldV,
                                newV);
}

void
TAO_LogNotification::stop_time_value_change (DsLogAdmin::Log_ptr log,
                                             DsLogAdmin::LogId id,
                                             DsLogAdmin::TimeT oldValue,
                                             DsLogAdmin::TimeT newValue)
{
  CORBA::Any oldV, newV;
  oldV <<= oldValue;
  newV <<= newValue;

  this->attribute_value_change (log,
                                id,
                                DsLogNotification::stopTime,
                                oldV,
                                newV);
}

void
TAO_LogNotification::week_mask_value_change (DsLogAdmin::Log_ptr log,
                                             DsLogAdmin::LogId id,
                                             const DsLogAdmin::WeekMask& oldValue,
                                             const DsLogAdmin::WeekMask& newValue)
{
  CORBA::Any oldV, newV;
  oldV <<= oldValue;
  newV <<= newValue;

  this->attribute_value_change (log,
                                id,
                                DsLogNotification::weekMask,
                                oldV,
                                newV);
}

void
TAO_LogNotification::max_record_life_value_change (DsLogAdmin::Log_ptr log,
                                                   DsLogAdmin::LogId id,
                                                   CORBA::ULong oldValue,
                                                   CORBA::ULong newValue)
{
  CORBA::Any oldV, newV;
  oldV <<= oldValue;
  newV <<= newValue;

  this->attribute_value_change (log,
                                id,
                                DsLogNotification::maxRecordLife,
                                oldV,
                                newV);
}

void
TAO_LogNotification::quality_of_service_value_change (
    DsLogAdmin::Log_ptr log,
    DsLogAdmin::LogId id,
    const DsLogAdmin::QoSList& oldValue,
    const DsLogAdmin::QoSList& newValue)
{
  CORBA::Any oldV, newV;
  oldV <<= oldValue;
  newV <<= newValue;

  this->attribute_value_change (log,
                                id,
                                DsLogNotification::qualityOfService,
                                oldV,
                                newV);
}

void
TAO_LogNotification::state_change (DsLogAdmin::Log_ptr log,
                                   DsLogAdmin::LogId id,
                                   DsLogNotification::StateType type,
                                   CORBA::Any newValue)
{
  CORBA::Any any;
  DsLogNotification::StateChange event;

  event.logref = DsLogAdmin::Log::_duplicate (log);
  event.id = id;
  event.time = ORBSVCS_Time::to_Absolute_TimeT (ACE_OS::gettimeofday ());

  // Administrative, Operational or Forwarding state.
  event.type = type;

  event.new_value = newValue;

  any <<= event;

  this->send_notification (any);
}

void
TAO_LogNotification::administrative_state_change (
    DsLogAdmin::Log_ptr log,
    DsLogAdmin::LogId id,
    DsLogAdmin::AdministrativeState newValue)
{
  CORBA::Any newV;
  newV <<= newValue;

  this->state_change (log,
                      id,
                      DsLogNotification::administrativeState,
                      newV);
}

void
TAO_LogNotification::operational_state_change (
    DsLogAdmin::Log_ptr log,
    DsLogAdmin::LogId id,
    DsLogAdmin::OperationalState newValue)
{
  CORBA::Any newV;
  newV <<= newValue;

  this->state_change (log,
                      id,
                      DsLogNotification::operationalState,
                      newV);
}

void
TAO_LogNotification::forwarding_state_change (DsLogAdmin::Log_ptr log,
                                              DsLogAdmin::LogId id,
                                              DsLogAdmin::ForwardingState newValue)
{
  CORBA::Any newV;
  newV <<= newValue;

  this->state_change (log,
                      id,
                      DsLogNotification::forwardingState,
                      newV);
}

void
TAO_LogNotification::threshold_alarm (
    DsLogAdmin::Log_ptr log,
    DsLogAdmin::LogId id,
    DsLogAdmin::Threshold crossedValue,
    DsLogAdmin::Threshold observedValue,
    DsLogNotification::PerceivedSeverityType severity)
{
  CORBA::Any any;
  DsLogNotification::ThresholdAlarm event;

  event.logref = DsLogAdmin::Log::_duplicate (log);
  event.id = id;
  event.time = ORBSVCS_Time::to_Absolute_TimeT (ACE_OS::gettimeofday ());

  event.crossed_value = crossedValue;
  event.observed_value = observedValue;
  event.perceived_severity = severity;

  any <<= event;

  this->send_notification (any);
}

void
TAO_LogNotification::send_notification (const CORBA::Any & /* any */)
{
}

TAO_END_VERSIONED_NAMESPACE_DECL
