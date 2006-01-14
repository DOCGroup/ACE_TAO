#include "orbsvcs/Log/Log_i.h"
#include "orbsvcs/Log/LogMgr_i.h"
#include "orbsvcs/Time_Utilities.h"

#include "tao/debug.h"
#include "tao/ORB_Core.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_sys_time.h"

ACE_RCSID (Log,
           Log_i,
           "$Id$")


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Log Compaction Interval
const ACE_Time_Value
TAO_Log_i::log_compaction_interval_ = ACE_Time_Value(60);

// Log Flush Interval
const ACE_Time_Value
TAO_Log_i::log_flush_interval_ = ACE_Time_Value(5 * 60);


TAO_Log_i::TAO_Log_i (CORBA::ORB_ptr orb,
		      TAO_LogMgr_i &logmgr_i,
                      DsLogAdmin::LogMgr_ptr factory,
                      DsLogAdmin::LogId logid,
                      TAO_LogNotification *log_notifier)
  : factory_ (DsLogAdmin::LogMgr::_duplicate (factory)),
    logid_ (logid),
    op_state_ (DsLogAdmin::disabled),
    reactor_ (orb->orb_core()->reactor()),
    log_compaction_handler_(reactor_, this, log_compaction_interval_),
    log_flush_handler_(reactor_, this, log_flush_interval_)
{
  ACE_DECLARE_NEW_CORBA_ENV;

  recordstore_ = logmgr_i.get_log_record_store (logid
						ACE_ENV_ARG_PARAMETER);

  // TODO: get log parameters from (persistent?) store.
  avail_status_.off_duty = 0;
  avail_status_.log_full = 0;
  this->notifier_ = log_notifier;
  this->current_threshold_ = 0;
  this->thresholds_.length(1);
  this->thresholds_[0] = 100;
  this->qostype_ = DsLogAdmin::QoSNone;
}

void
TAO_Log_i::init (ACE_ENV_SINGLE_ARG_DECL)
{
  if (recordstore_->open () ==-1)
    ACE_THROW (CORBA::UNKNOWN ());

  if (this->recordstore_->get_max_record_life (ACE_ENV_SINGLE_ARG_PARAMETER) != 0)
    {
      this->log_compaction_handler_.schedule ();
    }

  if (this->qostype_ == DsLogAdmin::QoSFlush)
    {
      this->log_flush_handler_.schedule ();
    }

  this->op_state_ = DsLogAdmin::enabled;
}

TAO_Log_i::~TAO_Log_i (void)
{
  recordstore_->close ();
}

DsLogAdmin::LogMgr_ptr
TAO_Log_i::my_factory (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return DsLogAdmin::LogMgr::_duplicate (this->factory_.in ());
}

DsLogAdmin::LogId
TAO_Log_i::id (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return logid_;
}

DsLogAdmin::QoSList*
TAO_Log_i::get_log_qos (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // @@ The current revision of the specification (formal/03-07-01)
  // states that get_log_qos() returns a list of the QoS properties
  // supported by the log, not the current value.  However, because
  // that is inconsistent with both the Log Service's other get
  // methods and the Notification Service's QoS get_qos methods, I
  // have submitted a defect report to the OMG for clarification.
  //    --jtc

  DsLogAdmin::QoSList* ret_ptr;
  ACE_NEW_THROW_EX (ret_ptr,
                    DsLogAdmin::QoSList (1),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  DsLogAdmin::QoSList_var ret_val = ret_ptr;
  ret_val->length(1);
  ret_val[0] = qostype_;

  return ret_val._retn ();
}

void
TAO_Log_i::set_log_qos (const DsLogAdmin::QoSList &qos
                        ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   DsLogAdmin::UnsupportedQoS))
{
  // @@ The current revision of the specification (formal/03-07-01)
  // does not clearly define the semantics to follow when the QoSList
  // contains mutually exclusive, unsupported, or unknown properties.
  // I have submitted a defect report to the OMG for clarification.
  //
  // In the mean time, the last known/supported property found in the
  // QoSList takes presidence.  If any unknown/unsupported properties
  // were found, an UnsupportedQoS exception is thrown.
  //    --jtc

  DsLogAdmin::QoSType old_qos = this->qostype_;
  DsLogAdmin::QoSType qostype = old_qos;
  DsLogAdmin::QoSList denied;

  // validate properties..
  for (CORBA::ULong i = 0; i < qos.length (); ++i)
    {
      qostype = qos[i];
      if (qostype != DsLogAdmin::QoSNone &&
	  qostype != DsLogAdmin::QoSFlush &&
          qostype != DsLogAdmin::QoSReliability)
        {
	  CORBA::ULong len = denied.length();
	  denied.length(len + 1);
	  denied[len] = qostype;
        }
    }

  // if there were any unknown/unsupported properties, thrown an
  // exception.
  if (denied.length() != 0)
    {
      ACE_THROW (DsLogAdmin::UnsupportedQoS (denied));
    }

  if (qostype == old_qos)
    return;

  this->qostype_ = qostype;

  if (this->qostype_ == DsLogAdmin::QoSFlush)
    this->log_flush_handler_.schedule ();
  else
    this->log_flush_handler_.cancel ();

  if (notifier_)
    {
      DsLogAdmin::Log_var log =
        this->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      DsLogAdmin::QoSList* old_qoslist_ptr;
      ACE_NEW_THROW_EX (old_qoslist_ptr,
			DsLogAdmin::QoSList (1),
			CORBA::NO_MEMORY ());
      ACE_CHECK;

      DsLogAdmin::QoSList_var old_qoslist = old_qoslist_ptr;
      old_qoslist->length(1);
      old_qoslist[0] = old_qos;

      DsLogAdmin::QoSList* new_qoslist_ptr;
      ACE_NEW_THROW_EX (new_qoslist_ptr,
			DsLogAdmin::QoSList (1),
			CORBA::NO_MEMORY ());
      ACE_CHECK;

      DsLogAdmin::QoSList_var new_qoslist = new_qoslist_ptr;
      new_qoslist->length(1);
      new_qoslist[0] = qostype;

      notifier_->quality_of_service_value_change (log.in (),
                                                  logid_,
                                                  old_qoslist.in (),
                                                  new_qoslist.in ()
                                                  ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
}

CORBA::ULong
TAO_Log_i::get_max_record_life (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return
    this->recordstore_->get_max_record_life(ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_Log_i::set_max_record_life (CORBA::ULong life
                                ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::ULong old_life;
  old_life = this->get_max_record_life (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (life == old_life)
    return;

  this->recordstore_->set_max_record_life(life ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (life != 0)
    this->log_compaction_handler_.schedule();
  else
    this->log_compaction_handler_.cancel();

  if (notifier_)
    {
      DsLogAdmin::Log_var log =
        this->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      notifier_->max_record_life_value_change (log.in (),
                                               logid_,
                                               old_life,
                                               life
                                               ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
  }
}

CORBA::ULongLong
TAO_Log_i::get_max_size (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return recordstore_->get_max_size (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_Log_i::set_max_size (CORBA::ULongLong size
                         ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   DsLogAdmin::InvalidParam))
{
  // size == 0 => infinite size.
  CORBA::ULongLong old_size;
  old_size = this->get_max_size (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if ((size != 0) && (size <
      this->recordstore_->get_current_size (ACE_ENV_SINGLE_ARG_PARAMETER)))
    {
      ACE_THROW (DsLogAdmin::InvalidParam ());
    }
  else
    {
      this->recordstore_->set_max_size (size ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      if (notifier_ && old_size != size)
        {
          DsLogAdmin::Log_var log =
            this->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK;

          notifier_->max_log_size_value_change (log.in (),
                                                logid_,
                                                old_size,
                                                size
                                                ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;
        }
    }
}

CORBA::ULongLong
TAO_Log_i::get_current_size (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->recordstore_->get_current_size (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::ULongLong
TAO_Log_i::get_n_records (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->recordstore_->get_n_records (ACE_ENV_SINGLE_ARG_PARAMETER);
}

DsLogAdmin::LogFullActionType
TAO_Log_i::get_log_full_action (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return
    this->recordstore_->get_log_full_action(ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_Log_i::set_log_full_action (DsLogAdmin::LogFullActionType action
                                ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
		   DsLogAdmin::InvalidLogFullAction))
{
  if (action != DsLogAdmin::wrap && action != DsLogAdmin::halt)
    ACE_THROW (DsLogAdmin::InvalidLogFullAction ());

  DsLogAdmin::LogFullActionType old_action =
    this->get_log_full_action (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (action == old_action)
    return;

  this->recordstore_->set_log_full_action (action ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (notifier_)
    {
      DsLogAdmin::Log_var log =
        this->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      notifier_->log_full_action_value_change (log.in (),
                                               logid_,
                                               old_action,
                                               action
                                               ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
   }
}

DsLogAdmin::AdministrativeState
TAO_Log_i::get_administrative_state (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->recordstore_->get_administrative_state (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_Log_i::set_administrative_state (DsLogAdmin::AdministrativeState state
                                     ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  DsLogAdmin::AdministrativeState old_state =
    this->get_administrative_state (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (state == old_state)
    return;

  this->recordstore_->set_administrative_state (state ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (notifier_)
    {
      DsLogAdmin::Log_var log =
        this->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      notifier_->administrative_state_change (log.in (),
                                              logid_,
                                              state
                                              ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
  }
}

DsLogAdmin::ForwardingState
TAO_Log_i::get_forwarding_state (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return
    this->recordstore_->get_forwarding_state (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_Log_i::set_forwarding_state (DsLogAdmin::ForwardingState state
                                 ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  DsLogAdmin::ForwardingState old_state =
    this->get_forwarding_state (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (state == old_state)
    return;

  this->recordstore_->set_forwarding_state (state ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (notifier_)
    {
      DsLogAdmin::Log_var log =
        this->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      notifier_->forwarding_state_change (log.in (),
                                          logid_,
                                          state
                                          ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
  }
}

DsLogAdmin::OperationalState
TAO_Log_i::get_operational_state (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->op_state_;
}

DsLogAdmin::TimeInterval
TAO_Log_i::get_interval (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->recordstore_->get_interval (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_Log_i::set_interval (const DsLogAdmin::TimeInterval &interval
                         ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   DsLogAdmin::InvalidTime,
                   DsLogAdmin::InvalidTimeInterval))
{

  if (interval.start != 0)
    {
      if (interval.start >= interval.stop)
        ACE_THROW (DsLogAdmin::InvalidTimeInterval ());
    }

  DsLogAdmin::TimeInterval old_interval =
    this->get_interval (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (interval.start == old_interval.start &&
      interval.stop  == old_interval.stop)
    return;

  this->recordstore_->set_interval (interval ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (notifier_)
    {
      DsLogAdmin::Log_var log =
        this->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      if (interval.start != old_interval.start)
        {
          notifier_->start_time_value_change (log.in (),
                                              logid_,
                                              old_interval.start,
                                              interval.start
                                              ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;
        }

      if (interval.stop != old_interval.stop)
        {
          notifier_->stop_time_value_change (log.in (),
                                             logid_,
                                             old_interval.stop,
                                             interval.stop
                                             ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;
        }
    }
}

DsLogAdmin::AvailabilityStatus
TAO_Log_i::get_availability_status (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // The log is considered "on duty" if all the following are true:
  //   * operational state is enabled
  //   * adminstrative state is unlocked
  //   * current time falls within the log duration time.
  //   * current time falls within one (or more) of the log
  //     scheduling times.

  const CORBA::Boolean s = this->scheduled (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (this->avail_status_);

  DsLogAdmin::AdministrativeState admin_state =
    this->get_administrative_state (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (this->avail_status_);

  if (this->op_state_ == DsLogAdmin::enabled
      && admin_state == DsLogAdmin::unlocked
      && s == 1)
    {
      this->avail_status_.off_duty = 0; // "on duty"

    }
  else
    this->avail_status_.off_duty = 1;
  // The log_full flag is set by the write operations.
  return this->avail_status_;
}

DsLogAdmin::CapacityAlarmThresholdList*
TAO_Log_i::get_capacity_alarm_thresholds (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  DsLogAdmin::CapacityAlarmThresholdList* ret_val;
  ACE_NEW_THROW_EX (ret_val,
                    DsLogAdmin::CapacityAlarmThresholdList (this->thresholds_),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  return ret_val;
}

void
TAO_Log_i::set_capacity_alarm_thresholds (const
                                          DsLogAdmin::CapacityAlarmThresholdList
                                          &threshs
                                          ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   DsLogAdmin::InvalidThreshold))
{
  const CORBA::Boolean validated =
    TAO_Log_i::validate_capacity_alarm_thresholds (threshs
                                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (!validated)
    ACE_THROW (DsLogAdmin::InvalidThreshold ());

  DsLogAdmin::CapacityAlarmThresholdList old_threshs;
  old_threshs = thresholds_;

  if (threshs == old_threshs)
      return;

  this->thresholds_ = threshs;

  if (notifier_)
    {
      DsLogAdmin::Log_var log =
        this->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      notifier_->capacity_alarm_threshold_value_change (log.in (),
                                                        logid_,
                                                        old_threshs,
                                                        threshs
                                                        ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
}

DsLogAdmin::WeekMask*
TAO_Log_i::get_week_mask (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  DsLogAdmin::WeekMask* ret_val;
  ACE_NEW_THROW_EX (ret_val,
                    DsLogAdmin::WeekMask (this->weekmask_),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  return ret_val;
}

void
TAO_Log_i::set_week_mask (const DsLogAdmin::WeekMask &masks
                          ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   DsLogAdmin::InvalidTime,
                   DsLogAdmin::InvalidTimeInterval,
                   DsLogAdmin::InvalidMask))
{
  for (CORBA::ULong i = 0; i < masks.length (); ++i)
    {
      if (masks[i].days > (DsLogAdmin::Sunday +
                           DsLogAdmin::Monday +
                           DsLogAdmin::Tuesday +
                           DsLogAdmin::Wednesday +
                           DsLogAdmin::Thursday +
                           DsLogAdmin::Friday +
                           DsLogAdmin::Saturday)
          )
        ACE_THROW (DsLogAdmin::InvalidMask ());

      for (CORBA::ULong j = 0; j < masks[i].intervals.length (); ++j)
        {
          if (masks[i].intervals[j].start.hour > 23 ||
              masks[i].intervals[j].start.minute > 59 ||
              masks[i].intervals[j].stop.hour > 23 ||
              masks[i].intervals[j].stop.minute > 59)
            ACE_THROW (DsLogAdmin::InvalidTime ());

          if (masks[i].intervals[j].stop.hour <
              masks[i].intervals[j].start.hour)
            ACE_THROW (DsLogAdmin::InvalidTimeInterval ());

          if (masks[i].intervals[j].stop.hour ==
              masks[i].intervals[j].start.hour &&
              masks[i].intervals[j].stop.minute <=
              masks[i].intervals[j].start.minute)
            ACE_THROW (DsLogAdmin::InvalidTimeInterval ());

        }
    }

  DsLogAdmin::WeekMask old_masks;
  old_masks = weekmask_;

  DsLogAdmin::TimeInterval temp_interval;
  CORBA::ULong count = 0;
  weekly_intervals_.length (100);

  // convert the weekmask into a sequence of time intervals.
  for (CORBA::ULong k = 0; k < masks.length (); ++k)
    {
      for (CORBA::ULong j = 0; j < masks[k].intervals.length (); ++j)
        {
          for (int d = 0; d < 7; ++d)
            {
              if ( (1 << d) & masks[k].days)
                {
                  temp_interval.start =
                    CORBA::ULongLong (
                      (d * 3600* 24) +
                      (masks[k].intervals[j].start.hour * 3600) +
                      (masks[k].intervals[j].start.minute * 60)) * 10000000;

                  temp_interval.stop =
                    CORBA::ULongLong (
                      (d * 3600* 24) +
                      (masks[k].intervals[j].stop.hour * 3600) +
                      (masks[k].intervals[j].stop.minute * 60)) * 10000000;

                  weekly_intervals_[count] = temp_interval;
                  ++count;
                }
            }
        }
    }
  weekly_intervals_.length (count);

  //TODO: SORT AND CLEAN

  if (notifier_)
    {
      DsLogAdmin::Log_var log =
        this->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      notifier_->week_mask_value_change (log.in (),
                                         logid_,
                                         old_masks,
                                         masks
                                         ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
}


DsLogAdmin::RecordList*
TAO_Log_i::query (const char *grammar,
                  const char *constraint,
                  DsLogAdmin::Iterator_out iter_out
                  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   DsLogAdmin::InvalidGrammar,
                   DsLogAdmin::InvalidConstraint))
{
  return this->recordstore_->query (grammar,
				    constraint,
				    iter_out
				    ACE_ENV_ARG_PARAMETER);
}


DsLogAdmin::RecordList*
TAO_Log_i::retrieve (DsLogAdmin::TimeT from_time,
                     CORBA::Long how_many,
                     DsLogAdmin::Iterator_out iter_out
                     ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->recordstore_->retrieve (from_time,
				       how_many,
				       iter_out
				       ACE_ENV_ARG_PARAMETER);
}


CORBA::ULong
TAO_Log_i::match (const char* grammar,
                  const char *constraint
                  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   DsLogAdmin::InvalidGrammar,
                   DsLogAdmin::InvalidConstraint))
{
  CORBA::ULong count =
    this->recordstore_->match (grammar,
			       constraint
			       ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return count;
}


CORBA::ULong
TAO_Log_i::delete_records (const char *grammar,
                           const char *constraint
                           ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     DsLogAdmin::InvalidGrammar,
                     DsLogAdmin::InvalidConstraint))
{
  CORBA::ULong count =
    this->recordstore_->delete_records (grammar,
					constraint
					ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  if (avail_status_.log_full && count > 0)
    {
      const CORBA::ULongLong current_size =
        this->get_current_size (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      const CORBA::ULongLong max_size =
        this->get_max_size (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      if (current_size < max_size)
        {
          avail_status_.log_full = 0;
        }
    }

  this->reset_capacity_alarm_threshold (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return count;
}


CORBA::ULong
TAO_Log_i::delete_records_by_id (const DsLogAdmin::RecordIdList &ids
                                 ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::ULong count =
    this->recordstore_->delete_records_by_id (ids ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  if (avail_status_.log_full && count > 0)
    {
      const CORBA::ULongLong current_size =
        this->get_current_size (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      const CORBA::ULongLong max_size =
        this->get_max_size (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      if (current_size < max_size)
        {
          avail_status_.log_full = 0;
        }
    }

  this->reset_capacity_alarm_threshold (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return count;
}


void
TAO_Log_i::write_records (const DsLogAdmin::Anys &records
                          ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   DsLogAdmin::LogFull,
                   DsLogAdmin::LogOffDuty,
                   DsLogAdmin::LogLocked,
                   DsLogAdmin::LogDisabled))
{
  // create a record list..
  DsLogAdmin::RecordList reclist (records.length ());
  reclist.length (records.length ());

  for (CORBA::ULong i = 0; i < records.length (); ++i)
    {
      reclist [i].info = records[i];
    }

  this->write_recordlist (reclist ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_Log_i::write_recordlist (const DsLogAdmin::RecordList &reclist
                             ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   DsLogAdmin::LogFull,
                   DsLogAdmin::LogOffDuty,
                   DsLogAdmin::LogLocked,
                   DsLogAdmin::LogDisabled))
{
  DsLogAdmin::LogFullActionType log_full_action =
    this->get_log_full_action (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  DsLogAdmin::AdministrativeState admin_state =
    this->get_administrative_state (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // @@ The current revision of the specification (formal/03-07-01)
  // does not explicitly specify the preference of exceptions to be
  // thrown when multiple error conditions are present.
  //
  // However, the because log is considered off duty if the log's
  // operational state is disabled or its administrative state is
  // locked, we handle the LogOffDuty exception last so the more
  // specific LogLocked and LogDisabled exceptions will be thrown.

  DsLogAdmin::AvailabilityStatus avail_stat =
    this->get_availability_status (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (avail_stat.log_full == 1
      && log_full_action == DsLogAdmin::halt)
    {
      ACE_THROW (DsLogAdmin::LogFull (0));
    }
  else if (admin_state == DsLogAdmin::locked)
    {
      ACE_THROW (DsLogAdmin::LogLocked ());
    }
  else if (this->op_state_ == DsLogAdmin::disabled)
    {
      ACE_THROW (DsLogAdmin::LogDisabled ());
    }
  else if (avail_stat.off_duty == 1)
    {
      ACE_THROW (DsLogAdmin::LogOffDuty ());
    }

  CORBA::Short num_written (0);
  DsLogAdmin::LogRecord record;

  for (CORBA::ULong i = 0; i < reclist.length (); i++)
    {
      // Check if the log is full.
      if (avail_status_.log_full == 1
          && log_full_action == DsLogAdmin::halt)
        {
          ACE_THROW (DsLogAdmin::LogFull (num_written));
        }
      else
        {
          // retval == 1 => log store reached max size.

          int retval = this->recordstore_->log (reclist[i] ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;

          if (retval == 1)
            {
              // The Log is full . check what the policy is
              // and take appropriate action.
              if (log_full_action == DsLogAdmin::halt)
                avail_status_.log_full = 1;
              else // the policy is to wrap. for this we need to delete
              {  // a few records. let the record store decide how many.

                if (this->recordstore_->purge_old_records (ACE_ENV_SINGLE_ARG_PARAMETER) == -1)
                  ACE_THROW (CORBA::PERSIST_STORE ());
              }
              // Now, we want to attempt to write the same record again
              // so decrement the index to balance the inc. in the for loop.
              --i;
            }
          else
            if (retval == 0)
              num_written++;
            else
              ACE_THROW (CORBA::PERSIST_STORE ());
        } // else

      this->check_capacity_alarm_threshold (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

    } // for

}

void
TAO_Log_i::set_record_attribute (DsLogAdmin::RecordId id,
                                 const DsLogAdmin::NVList &attr_list
                                 ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   DsLogAdmin::InvalidRecordId,
                   DsLogAdmin::InvalidAttribute))
{
  // TODO: validate attributes here.

  DsLogAdmin::LogRecord rec;
  if (this->recordstore_->retrieve (id, rec ACE_ENV_ARG_PARAMETER) == -1)
    {
      ACE_THROW (DsLogAdmin::InvalidRecordId ());
    }

  rec.attr_list = attr_list;

  if (this->recordstore_->update (rec ACE_ENV_ARG_PARAMETER) == -1)
    {
      ACE_THROW (CORBA::PERSIST_STORE ());
    }
}

CORBA::ULong
TAO_Log_i::set_records_attribute (const char *grammar,
                                  const char *constraint,
                                  const DsLogAdmin::NVList
                                  &attr_list ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     DsLogAdmin::InvalidGrammar,
                     DsLogAdmin::InvalidConstraint,
                     DsLogAdmin::InvalidAttribute))
{
  DsLogAdmin::Iterator_var iter_out;

  DsLogAdmin::RecordList_var rec_list =
    this->query (grammar,
		 constraint,
		 iter_out
		 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  CORBA::ULong count = rec_list->length ();

  for (CORBA::ULong i = 0; i < count; ++i)
    {
      this->set_record_attribute (rec_list[i].id,
				  attr_list
				  ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);
    }

  // if iterator is not nil, process remainder of sequence
  if (!CORBA::is_nil (iter_out.in ()))
    {
      CORBA::ULong len;
      do
        {
	  rec_list = iter_out->get (count, 0 ACE_ENV_ARG_PARAMETER);
	  ACE_CHECK_RETURN (0);

	  len = rec_list->length ();
	  for (CORBA::ULong i = 0; i < len; ++i)
	    {
	      this->set_record_attribute (rec_list[i].id,
					  attr_list
					  ACE_ENV_ARG_PARAMETER);
	      ACE_CHECK_RETURN (0);
	    }

	  count += len;
	}
      while (len != 0);
    }

  return count;
}

DsLogAdmin::NVList*
TAO_Log_i::get_record_attribute (DsLogAdmin::RecordId id
                             ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   DsLogAdmin::InvalidRecordId))
{
  DsLogAdmin::LogRecord rec;

  int retval = this->recordstore_->retrieve (id, rec ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  if (retval == -1)
    {
      ACE_THROW_RETURN (DsLogAdmin::InvalidRecordId (),
                        0);
    }

  DsLogAdmin::NVList* nvlist = 0;
  ACE_NEW_THROW_EX (nvlist,
                    DsLogAdmin::NVList (rec.attr_list),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  return nvlist;
}

void
TAO_Log_i::flush (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                     DsLogAdmin::UnsupportedQoS))
{
  this->recordstore_->flush (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::Boolean
TAO_Log_i::scheduled (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  DsLogAdmin::TimeInterval interval =
    this->get_interval (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  TimeBase::TimeT current_time;
  ACE_Time_Value now = ACE_OS::gettimeofday ();
  ORBSVCS_Time::Time_Value_to_TimeT (current_time, now);

  if ((current_time >= interval.start) &&
          ((current_time <= interval.stop) || (interval.stop == 0)) )
  {
    if (weekly_intervals_.length () > 0)
    {
      // work out when sunday is in nanoseconds.
      timeval t;
      t = (timeval) now;
      struct tm *sunday;

      time_t clock = (time_t) t.tv_sec;
      sunday = ACE_OS::localtime (&clock);

      sunday->tm_sec = 0;
      sunday->tm_min = 0;
      sunday->tm_hour = 0;
      sunday->tm_mday -= sunday->tm_wday;

      t.tv_sec = ACE_OS::mktime (sunday) ;
      t.tv_usec = 0;

      TimeBase::TimeT nano_sunday =
        (CORBA::ULongLong) t.tv_sec * 10000000;

      for (CORBA::ULong i = 0; i < weekly_intervals_.length (); ++i)
        {
          if (current_time >= (weekly_intervals_[i].start + nano_sunday) &&
              current_time <= (weekly_intervals_[i].stop + nano_sunday))
            {
              return true;
            }
        }
      return false;
    }
    else
      return true;
  }
  else
    return false;
}

void
TAO_Log_i::copy_attributes (DsLogAdmin::Log_ptr log
                            ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  const DsLogAdmin::LogFullActionType log_full_action =
    this->get_log_full_action (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  log->set_log_full_action (log_full_action
                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  const CORBA::ULongLong max_size =
    this->get_max_size (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  log->set_max_size (max_size
                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  DsLogAdmin::QoSList_var log_qos =
    this->get_log_qos (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  log->set_log_qos (log_qos.in ()
                    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  const CORBA::ULong max_record_life =
    this->get_max_record_life (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  log->set_max_record_life (max_record_life
                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  const DsLogAdmin::AdministrativeState adminstrative_state =
    this->get_administrative_state (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  log->set_administrative_state (adminstrative_state
                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  const DsLogAdmin::ForwardingState forwarding_state =
    this->get_forwarding_state (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  log->set_forwarding_state (forwarding_state
                             ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  const DsLogAdmin::TimeInterval interval =
    this->get_interval (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  log->set_interval (interval
                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  DsLogAdmin::CapacityAlarmThresholdList_var capacity_list =
    this->get_capacity_alarm_thresholds (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  log->set_capacity_alarm_thresholds (capacity_list.in ()
                                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  DsLogAdmin::WeekMask_var week_mask =
    this->get_week_mask (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  log->set_week_mask (week_mask.in ()
                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_Log_i::remove_old_records (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::ULong count =
    this->recordstore_->remove_old_records (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (avail_status_.log_full && count > 0)
    {
      const CORBA::ULongLong current_size =
        this->get_current_size (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      const CORBA::ULongLong max_size =
        this->get_max_size (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      if (current_size < max_size)
        {
          avail_status_.log_full = 0;
        }
    }

  this->reset_capacity_alarm_threshold (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_Log_i::check_capacity_alarm_threshold (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::ULongLong max_size =
    this->get_max_size (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (max_size != 0 && this->thresholds_.length () > 0)
    {
      CORBA::ULongLong current_size =
        this->recordstore_->get_current_size (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      const CORBA::UShort percent =
        static_cast<CORBA::UShort> (((double) ACE_UINT64_DBLCAST_ADAPTER (current_size * 100U) /
           (double) ACE_UINT64_DBLCAST_ADAPTER (max_size)));

      while (current_threshold_ < this->thresholds_.length ()
             && this->thresholds_[this->current_threshold_] <= percent)
        {
          DsLogNotification::PerceivedSeverityType severity =
            percent == 100 ? DsLogNotification::critical :
            DsLogNotification::minor;

          if (notifier_)
            {
              DsLogAdmin::Log_var log =
                this->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
              ACE_CHECK;

              notifier_->threshold_alarm (
                log.in (),
                logid_,
                this->thresholds_[this->current_threshold_],
                percent,
                severity
                ACE_ENV_ARG_PARAMETER);
              ACE_CHECK;
            }
          else
            {
              if (TAO_debug_level > 0)
                ACE_DEBUG ((LM_DEBUG,
                            "threshold of %d breached\n",
                            this->thresholds_[this->current_threshold_]));
            }

          ++this->current_threshold_;
        }

      if (this->current_threshold_ == this->thresholds_.length ()
          && this->recordstore_->get_log_full_action (ACE_ENV_SINGLE_ARG_PARAMETER) != DsLogAdmin::halt)
        {
          this->current_threshold_ = 0;
        }
    }
}

void
TAO_Log_i::reset_capacity_alarm_threshold (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::ULongLong max_size =
    this->get_max_size (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (max_size != 0 && this->thresholds_.length() > 0)
    {
      CORBA::ULongLong current_size =
        this->recordstore_->get_current_size (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      const CORBA::UShort percent =
        static_cast<CORBA::UShort> ((((double) ACE_UINT64_DBLCAST_ADAPTER (current_size * 100U)) /
            (double) ACE_UINT64_DBLCAST_ADAPTER (max_size)));

      this->current_threshold_ = 0;

      while (this->current_threshold_ < this->thresholds_.length ()
             && this->thresholds_[this->current_threshold_] <= percent)
        ++this->current_threshold_;
    }
}

CORBA::Boolean
TAO_Log_i::validate_capacity_alarm_thresholds (
    const DsLogAdmin::CapacityAlarmThresholdList & threshs
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  for (CORBA::ULong i = 0; i < threshs.length (); i++)
    if (threshs[i] > 100)
      return false;

  // @@ The current revision of the specification (formal/03-07-01)
  // does not require theshold values be in order or unique.  This
  // appears to be an TAO TLS implementation artifact for efficent
  // threshold checks.  We could eliminate this restriction by
  // normalizing the sequence.

  if (threshs.length () > 1)
    for (CORBA::ULong i = 0; i < threshs.length () - 1; i++)
      if (threshs[i] >= threshs[i + 1])
        return false;

  return true;
}

bool
operator==(const DsLogAdmin::CapacityAlarmThresholdList& rhs,
           const DsLogAdmin::CapacityAlarmThresholdList& lhs)
{
  const CORBA::ULong length = rhs.length ();

  if (length != lhs.length())
    {
      return false;
    }

  for (CORBA::ULong i = 0; i < length; ++i) 
    {
      if (rhs[i] != lhs[i]) 
	{
	  return false;
	}
    }

  return true;
}

bool
operator!=(const DsLogAdmin::CapacityAlarmThresholdList& rhs,
           const DsLogAdmin::CapacityAlarmThresholdList& lhs)
{
  return !(lhs == rhs);
}

TAO_END_VERSIONED_NAMESPACE_DECL
