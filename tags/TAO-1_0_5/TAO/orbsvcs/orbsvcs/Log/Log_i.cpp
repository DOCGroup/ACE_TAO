// $Id$

#include "orbsvcs/Log/Log_i.h"
#include "Iterator_i.h"

Log_i::Log_i (DsLogAdmin::LogMgr_ptr factory,
              DsLogAdmin::LogId id,
              DsLogAdmin::LogFullAction log_full_action,
              CORBA::ULongLong max_size,
              ACE_Reactor *reactor)
  : factory_ (factory),
    log_full_action_ (log_full_action),
    logid_ (id),
    admin_state_ (DsLogAdmin::locked),
    forward_state_ (DsLogAdmin::off),
    op_state_ (DsLogAdmin::disabled),
    reactor_ (reactor),
    recordstore_ (max_size)
{
  avail_status_.off_duty = 0;
  avail_status_.log_full = 0;
}

void
Log_i::init (CORBA::Environment &ACE_TRY_ENV)
{
  if (recordstore_.open () ==-1)
    ACE_THROW (CORBA::UNKNOWN ());

  // enable the log now.
  this->admin_state_ = DsLogAdmin::unlocked;
  this->forward_state_ = DsLogAdmin::on;
  this->op_state_ = DsLogAdmin::enabled;
}

Log_i::~Log_i (void)
{
  recordstore_.close ();
}

DsLogAdmin::LogMgr_ptr
Log_i::my_factory (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return DsLogAdmin::LogMgr::_duplicate (factory_.in ());
}

DsLogAdmin::LogId
Log_i::id (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return logid_;
}

DsLogAdmin::QoSList_ptr
Log_i::get_qos (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  DsLogAdmin::QoSList_ptr ret_val;
  ACE_NEW_THROW_EX (ret_val,
                    DsLogAdmin::QoSList (this->qoslist_),
                    CORBA::NO_MEMORY ());

  return ret_val;
}

void
Log_i::set_qos (const DsLogAdmin::QoSList &qos,
                CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   DsLogAdmin::UnsupportedQoS))
{
  // validate supported properties..
  for (CORBA::ULong i = 0; i < qos.length (); i++)
    {
      DsLogAdmin::QoSType qostype = qos[i];
      if (qostype == DsLogAdmin::QoSFlush ||
          qostype == DsLogAdmin::QoSReliability)
        {
          DsLogAdmin::QoSList denied;
          denied._allocate_buffer (2);
          denied.length (0);

          denied[0] = DsLogAdmin::QoSFlush;
          denied[1] = DsLogAdmin::QoSReliability;

          ACE_THROW (DsLogAdmin::UnsupportedQoS (denied));
        }
    }

  // store this list.
  this->qoslist_ = qos;
}

CORBA::ULong
Log_i::get_max_record_life (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return max_record_life_;
}

void
Log_i::set_max_record_life (CORBA::ULong life,
                            CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // life is in seconds.
  // @@ TODO: Need to iterate over all records in storage to timeout all
  // those outside the lifetime....
  max_record_life_ = life;
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

CORBA::ULongLong
Log_i::get_max_size (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return recordstore_.get_max_size ();
}

void
Log_i::set_max_size (CORBA::ULongLong size,
                     CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   DsLogAdmin::InvalidParam))
{
  // size == 0 => infinite size.
  if (size == 0)
    {
      this->recordstore_.set_max_size (size);
    }
  else
    if (size < this->recordstore_.get_current_size ())
      {
        ACE_THROW (DsLogAdmin::InvalidParam ());
      }
    else
      this->recordstore_.set_max_size (size);
}

CORBA::ULongLong
Log_i::get_current_size (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->recordstore_.get_current_size ();
}

CORBA::ULongLong
Log_i::get_n_records (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->recordstore_.get_n_records ();
}

DsLogAdmin::LogFullAction
Log_i::get_log_full_action (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return log_full_action_;
}

void
Log_i::set_log_full_action (DsLogAdmin::LogFullAction action,
                            CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  log_full_action_ = action;
}

DsLogAdmin::AdministrativeState
Log_i::get_administrative_state (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return admin_state_;
}

void
Log_i::set_administrative_state (DsLogAdmin::AdministrativeState state,
                                 CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->admin_state_ = state;
}

DsLogAdmin::ForwardingState
Log_i::get_forwarding_state (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->forward_state_;
}

void
Log_i::set_forwarding_state (DsLogAdmin::ForwardingState state,
                             CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->forward_state_ = state;
}

DsLogAdmin::OperationalState
Log_i::get_operational_state (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->op_state_;
}

DsLogAdmin::TimeInterval
Log_i::get_interval (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->interval_;
}

void
Log_i::set_interval (const DsLogAdmin::TimeInterval &interval,
                     CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   DsLogAdmin::InvalidTime,
                   DsLogAdmin::InvalidTimeInterval))
{
  //@@ TODO: validate the time.
  //         modify the timer settings to the new values.
  this->interval_ = interval;
  ACE_THROW (CORBA::NO_IMPLEMENT);
}

DsLogAdmin::AvailabilityStatus
Log_i::get_availability_status (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO:
  // "on duty" => "enabled" AND "unlocked" AND current time within log
  // duration time AND current time within log scheduling times.
  if (this->op_state_ == DsLogAdmin::enabled
      && this->admin_state_ == DsLogAdmin::unlocked
      && this->scheduled () == 1)
    {
      this->avail_status_.off_duty = 0; // "on duty"
    }
  // The log_full flag is set by the write operations.
  return this->avail_status_;
}

DsLogAdmin::CapacityAlarmThresholdList_ptr
Log_i::get_capacity_alarm_thresholds (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  DsLogAdmin::CapacityAlarmThresholdList_ptr ret_val;
  ACE_NEW_THROW_EX (ret_val,
                     DsLogAdmin::CapacityAlarmThresholdList (this->thresholds_),
                     CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  return ret_val;
}

void
Log_i::set_capacity_alarm_thresholds (const
                                      DsLogAdmin::CapacityAlarmThresholdList
                                      &threshs,
                                      CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   DsLogAdmin::InvalidThreshold))
{
  // @@ TODO: validate thresholds
  //          How do we implement thresholds in DsLogAdmin !?
  //          According to the spec, the log must "notify" when threshold
  //          conditions are reached.we need to be an Event Channel to do that.  //          now what!?
  this->thresholds_ = threshs;
}

DsLogAdmin::WeekMask_ptr
Log_i::get_week_mask (CORBA::Environment &ACE_TRY_ENV)
{
  DsLogAdmin::WeekMask_ptr ret_val;
  ACE_NEW_THROW_EX (ret_val,
                    DsLogAdmin::WeekMask (this->weekmask_),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  return ret_val;
}

void
Log_i::set_week_mask (const DsLogAdmin::WeekMask &masks,
                      CORBA::Environment &ACE_TRY_ENV)
{
  // @@ TODO: validate masks.
  //           Activate various timers to trigger appropriate acition.
  this->weekmask_ = masks;
}

DsLogAdmin::RecordList_ptr
Log_i::query (const char *grammar,
              const char *constraint,
              DsLogAdmin::Iterator_out i,
              CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   DsLogAdmin::InvalidGrammar,
                   DsLogAdmin::InvalidConstraint))
{
  // @@ TODO: lots of things TBD here.
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

DsLogAdmin::RecordList_ptr
Log_i::retrieve (DsLogAdmin::TimeT from_time,
                 CORBA::Long how_many,
                 DsLogAdmin::Iterator_out i,
                 CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // @@ NEEDS ITERATOR
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

CORBA::ULong
Log_i::match (const char* grammar,
              const char *constraint,
              CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   DsLogAdmin::InvalidGrammar,
                   DsLogAdmin::InvalidConstraint))
{
  // @@ NEEDS ITERATOR
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

CORBA::ULong
Log_i::delete_records (const char *grammar,
                       const char *constraint,
                       CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     DsLogAdmin::InvalidGrammar,
                     DsLogAdmin::InvalidConstraint))
{
  // @@ NEEDS ITERATOR
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

CORBA::ULong
Log_i::delete_records_by_id (const
                             DsLogAdmin::RecordIdList &ids,
                             CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::ULong numdone (0);

  for (CORBA::ULong i = 0; i < ids.length (); i++)
    {
      if (this->recordstore_.remove (ids [i]) == 0)
        {
          numdone++;
        }
    }

  return numdone;
}

void
Log_i::write_records (const DsLogAdmin::Anys &records,
                      CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   DsLogAdmin::LogFull,
                   DsLogAdmin::LogLocked))
{
  // create a record list..
  DsLogAdmin::RecordList reclist (records.length ());
  reclist.length (records.length ());

  for (CORBA::ULong i = 0; i < records.length (); i++)
    {
      reclist [i].info = records[i];
    }

  this->write_recordlist (reclist, ACE_TRY_ENV);
  ACE_CHECK;
}

void
Log_i::write_recordlist (const DsLogAdmin::RecordList &reclist,
                         CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     DsLogAdmin::LogFull,
                     DsLogAdmin::LogLocked))
{
  // Check the operational status.
  if (this->op_state_ == DsLogAdmin::disabled)
    return; // return without a squeak.

  // Check if the log is on duty
  // @@ Wait for Comittee ruling on the proper behavior
  DsLogAdmin::AvailabilityStatus avail_stat =
    this->get_availability_status (ACE_TRY_ENV);
  ACE_CHECK;

  if (avail_stat.off_duty == 1)
    {
      // why are we off duty? investigate ...
      // Check if the log is full.
      if (avail_stat.log_full == 1)
        ACE_THROW (DsLogAdmin::LogFull (0));
      else   // Check the administrative state.
        if (this->admin_state_ == DsLogAdmin::locked)
          ACE_THROW (DsLogAdmin::LogLocked ());
        else
          return; // we are not scheduled at this time.
    }

  CORBA::Short num_written (0);
  DsLogAdmin::LogRecord record;

  for (CORBA::ULong i = 0; i < reclist.length (); i++)
    {
      // Check if the log is full.
      if (avail_status_.log_full == 1)
        {
          ACE_THROW (DsLogAdmin::LogFull (num_written));
        }
      else
        {
          // retval == 1 => log store reached max size.
          record = reclist[i]; // can't avoid this copy, reclist is const.
          int retval = this->recordstore_.log (record);
          if (retval == 1)
            {
              // The Log is full . check what the policy is
              // and take appropriate action.
              if (this->log_full_action_ == DsLogAdmin::halt)
                avail_status_.log_full = 1;
              else // the policy is to wrap. for this we need to delete
                // a few records. let the record store decide how many.
                if (this->recordstore_.purge_old_records () == -1)
                  ACE_THROW (CORBA::PERSIST_STORE ());

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

      this->check_threshold_list ();
    } // for
}

void
Log_i::set_record_attribute (DsLogAdmin::RecordId id,
                             const DsLogAdmin::NVList &attr_list,
                             CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   DsLogAdmin::InvalidRecordId,
                   DsLogAdmin::InvalidAttribute))
{
  // TODO: validate attributes here.

  DsLogAdmin::LogRecord rec;
  if (this->recordstore_.retrieve (id, rec) == -1)
    {
      ACE_THROW (DsLogAdmin::InvalidRecordId ());
    }

  rec.attr_list = attr_list;

  if (this->recordstore_.update (rec) == -1)
    {
      ACE_THROW (CORBA::PERSIST_STORE ());
    }
}

CORBA::ULong
Log_i::set_records_attribute (const char *grammar,
                              const char *constraint,
                              const DsLogAdmin::NVList
                              &attr_list, CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     DsLogAdmin::InvalidGrammar,
                     DsLogAdmin::InvalidConstraint,
                     DsLogAdmin::InvalidAttribute))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

DsLogAdmin::NVList_ptr
Log_i::get_record_attribute (DsLogAdmin::RecordId id,
                             CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   DsLogAdmin::InvalidRecordId))
{
  DsLogAdmin::LogRecord rec;
  if (this->recordstore_.retrieve (id, rec) == -1)
    {
      ACE_THROW_RETURN(DsLogAdmin::InvalidRecordId,
                       0);
    }

  DsLogAdmin::NVList_ptr nvlist;
  ACE_NEW_THROW_EX (nvlist,
                    DsLogAdmin::NVList (rec.attr_list),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  return nvlist;
}

// @@ These two methods are non-trivial. Let's hold off on them for a while
DsLogAdmin::Log_ptr
Log_i::copy (DsLogAdmin::LogId_out id,
             CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

DsLogAdmin::Log_ptr
Log_i::copy_with_id (DsLogAdmin::LogId id,
                     CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   DsLogAdmin::LogIdAlreadyExists))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

// @@ Should I just raise the exception?
void
Log_i::flush (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                     DsLogAdmin::UnsupportedQoS))
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
  // @@ Perhaps later use a MMAP_Memory_Pool as the backing store, and
  // just have this map to its sync method
}

void
Log_i::check_threshold_list (void)
{
  // TBD: Anyways we can't notify from DsLogAdmin.
  //      Finish this when we use the CosEC.
  return;
}

CORBA::Boolean
Log_i::scheduled (void)
{
  // TBD: lie for now.
  return 1;
}
