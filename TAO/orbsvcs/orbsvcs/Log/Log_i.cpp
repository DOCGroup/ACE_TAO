#include "orbsvcs/Log/Log_i.h"

#include "orbsvcs/Log/Iterator_i.h"
#include "orbsvcs/Log/Log_Constraint_Interpreter.h"
#include "orbsvcs/Log/Log_Constraint_Visitors.h"
#include "orbsvcs/Time_Utilities.h"

#include "tao/debug.h"


ACE_RCSID (Log,
           Log_i,
           "$Id$")


#define QUERY_LANG_SUPPORTED_BY_LOG "TCL"

TAO_Log_i::TAO_Log_i (DsLogAdmin::LogMgr_ptr factory,
                      DsLogAdmin::LogId id,
                      TAO_LogNotification *log_notifier,
                      DsLogAdmin::LogFullActionType log_full_action,
                      CORBA::ULongLong max_size,
                      ACE_Reactor *reactor)
  : factory_ (DsLogAdmin::LogMgr::_duplicate (factory)),
    log_full_action_ (log_full_action),
    logid_ (id),
    admin_state_ (DsLogAdmin::locked),
    forward_state_ (DsLogAdmin::off),
    op_state_ (DsLogAdmin::disabled),
    reactor_ (reactor),
    recordstore_ (max_size, id),
    max_rec_list_len_ (LOG_DEFAULT_MAX_REC_LIST_LEN)
{
  avail_status_.off_duty = 0;
  avail_status_.log_full = 0;
  interval_.start = 0;
  interval_.stop = 0;
  this->notifier_ = log_notifier;
  this->max_record_life_ = 0;
  this->current_threshold_ = 0;
  this->thresholds_.length(1);
  this->thresholds_[0] = 100;

}

void
TAO_Log_i::init (ACE_ENV_SINGLE_ARG_DECL)
{
  if (recordstore_.open () ==-1)
    ACE_THROW (CORBA::UNKNOWN ());

  // enable the log now.
  this->admin_state_ = DsLogAdmin::unlocked;
  this->forward_state_ = DsLogAdmin::on;
  this->op_state_ = DsLogAdmin::enabled;
}

TAO_Log_i::~TAO_Log_i (void)
{
  recordstore_.close ();
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
  DsLogAdmin::QoSList* ret_val;
  ACE_NEW_THROW_EX (ret_val,
                    DsLogAdmin::QoSList (this->qoslist_),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  return ret_val;
}

void
TAO_Log_i::set_log_qos (const DsLogAdmin::QoSList &qos
                        ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   DsLogAdmin::UnsupportedQoS))
{
  // validate supported properties..

  // implement after persistence is added.
  DsLogAdmin::QoSList_var old_qos;
  old_qos = get_log_qos (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::Boolean change = false;


  for (CORBA::ULong i = 0; i < qos.length (); ++i)
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

  if (notifier_ && change)
    {
      DsLogAdmin::Log_var log =
        this->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      notifier_->quality_of_service_value_change (log.in (),
                                                  logid_,
                                                  old_qos.in (),
                                                  qos
                                                  ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
}

CORBA::ULong
TAO_Log_i::get_max_record_life (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return max_record_life_;
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

  this->max_record_life_ = life;

  this->remove_old_records (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

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
TAO_Log_i::get_max_size (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return recordstore_.get_max_size ();
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

  if ((size != 0) && (size < this->recordstore_.get_current_size ()))
    {
      ACE_THROW (DsLogAdmin::InvalidParam ());
    }
  else
    {
      this->recordstore_.set_max_size (size);
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
  this->remove_old_records (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->recordstore_.get_current_size ();
}

CORBA::ULongLong
TAO_Log_i::get_n_records (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->remove_old_records (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->recordstore_.get_n_records ();
}

DsLogAdmin::LogFullActionType
TAO_Log_i::get_log_full_action (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return log_full_action_;
}

void
TAO_Log_i::set_log_full_action (DsLogAdmin::LogFullActionType action
                                ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  DsLogAdmin::LogFullActionType old_action =
    this->get_log_full_action (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  log_full_action_ = action;

  if (notifier_ && action != old_action)
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
TAO_Log_i::get_administrative_state (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return admin_state_;
}

void
TAO_Log_i::set_administrative_state (DsLogAdmin::AdministrativeState state
                                     ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (this->admin_state_ == state)
    return;

  this->admin_state_ = state;
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
TAO_Log_i::get_forwarding_state (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->forward_state_;
}

void
TAO_Log_i::set_forwarding_state (DsLogAdmin::ForwardingState state
                                 ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (this->forward_state_ == state)
    return;
  this->forward_state_ = state;

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
TAO_Log_i::get_interval (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->interval_;
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

  DsLogAdmin::TimeT old_start_time;
  DsLogAdmin::TimeT old_stop_time;
  old_start_time = interval_.start;
  old_stop_time = interval_.stop;

  if (interval.start == old_start_time && interval.stop == old_stop_time)
    return;

  this->interval_ = interval;

  if (notifier_)
    {
      DsLogAdmin::Log_var log =
        this->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      if (interval.start != old_start_time)
        {
          notifier_->start_time_value_change (log.in (),
                                              logid_,
                                              old_start_time,
                                              interval.start
                                              ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;
        }

      if (interval.stop != old_stop_time)
        {
          notifier_->stop_time_value_change (log.in (),
                                             logid_,
                                             old_stop_time,
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
  const CORBA::Boolean s = this->scheduled (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (this->avail_status_);

  if (this->op_state_ == DsLogAdmin::enabled
      && this->admin_state_ == DsLogAdmin::unlocked
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
TAO_Log_i::query_i (const char *constraint,
                    DsLogAdmin::Iterator_out &iter_out,
                    CORBA::ULong how_many
                    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   DsLogAdmin::InvalidConstraint))
{
  this->remove_old_records (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  // Use an Interpreter to build an expression tree.
  TAO_Log_Constraint_Interpreter interpreter (constraint
                                              ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  // Sequentially iterate over all the records and pick the ones that
  // meet the constraints.


  // get the underlying storage.
  TAO_LogRecordStore::LOG_RECORD_STORE &store =
    this->recordstore_.get_storage ();

  // Create an iterator
  TAO_LogRecordStore::LOG_RECORD_HASH_MAP_ITER iter (store);

  CORBA::ULong len = store.current_size ();

  // How many entries?

  // Iterate over and populate the list.
  TAO_LogRecordStore::LOG_RECORD_HASH_MAP_ENTRY *hash_entry;

  DsLogAdmin::RecordList* rec_list;
  // Figure out the length of the list.

  // Allocate the list of <how_many> length.
  ACE_NEW_THROW_EX (rec_list,
                    DsLogAdmin::RecordList (how_many),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  CORBA::ULong count = 0; // count of matches found.
  CORBA::Boolean done = 0; // flag to end "for" operation.
  CORBA::ULong i = 0;
  for (;
       i < len && count < how_many;
       ++i)
    {
      if (iter.next (hash_entry) == -1 || iter.advance () == -1)
        {
          done = 1;
          break;
        }

      // Use an evaluator.
      TAO_Log_Constraint_Visitor evaluator (hash_entry->int_id_);

      // Does it match the constraint?
      if (interpreter.evaluate (evaluator) == 1)
      {
        if (TAO_debug_level > 0)
#if defined (ACE_LACKS_LONGLONG_T)
               ACE_DEBUG ((LM_DEBUG,"Matched constraint! d = %Q, Time = %Q\n",
                      ACE_U64_TO_U32 (hash_entry->int_id_.id),
                      ACE_U64_TO_U32 (hash_entry->int_id_.time)));

#else
               ACE_DEBUG ((LM_DEBUG,"Matched constraint! d = %Q, Time = %Q\n",
                      hash_entry->int_id_.id,
                      hash_entry->int_id_.time));
#endif

        (*rec_list)[count] = hash_entry->int_id_;
        // copy the log record.
        count++;
      }
    }

  rec_list->length (count);

  if (i < len && done == 0) // There are more records to process.
    {
      // Create an iterator to pass out.
      TAO_Iterator_i *iter_query = 0;
      ACE_NEW_THROW_EX (iter_query,
                        TAO_Iterator_i (store,
                                        i,
                                        constraint,
                                        len,
                                        how_many),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (rec_list);

      // Transfer ownership to the POA.
      PortableServer::ServantBase_var safe_iter_query = iter_query;

      // Activate it.
      iter_out = iter_query->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (rec_list);
    }


  return rec_list;
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
  this->check_grammar (grammar ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->query_i (constraint,
                        iter_out,
                        this->max_rec_list_len_
                        ACE_ENV_ARG_PARAMETER);
}

DsLogAdmin::RecordList*
TAO_Log_i::retrieve (DsLogAdmin::TimeT from_time,
                     CORBA::Long how_many,
                     DsLogAdmin::Iterator_out iter_out
                     ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Decide between forward vs backward retrieval.
  char constraint[32];
  char uint64_formating[32];

#if defined (ACE_LACKS_LONGLONG_T)
         ACE_OS::sprintf (uint64_formating,
                          "%u",
                          ACE_U64_TO_U32 (from_time));
#else
         ACE_OS::sprintf (uint64_formating,
                          ACE_UINT64_FORMAT_SPECIFIER,
                          from_time);
#endif

  if (how_many >= 0)
    ACE_OS::sprintf (constraint, "time >= %s", uint64_formating);
  else
    {
      ACE_OS::sprintf (constraint, "time < %s", uint64_formating);
      how_many = -(how_many);
    }

  return this->query_i (constraint,
                        iter_out,
                        how_many
                        ACE_ENV_ARG_PARAMETER);
}

CORBA::ULong
TAO_Log_i::match_i (const char *constraint,
                    CORBA::Boolean delete_rec
                    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   DsLogAdmin::InvalidConstraint))
{
  this->remove_old_records (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  // Use an Interpreter to build an expression tree.
  TAO_Log_Constraint_Interpreter interpreter (constraint
                                              ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  // Get the underlying storage.
  TAO_LogRecordStore::LOG_RECORD_STORE &store =
    this->recordstore_.get_storage ();

  // Create an iterator
  TAO_LogRecordStore::LOG_RECORD_STORE_ITER iter (store);

  CORBA::ULong len = store.current_size ();
  // How many entries?

  // Iterate over and populate the list.
  TAO_LogRecordStore::LOG_RECORD_HASH_MAP_ENTRY *hash_entry;

  CORBA::ULong count = 0; // count of matches found.

  for (CORBA::ULong i = 0; i < len; ++i)
    {
      if (iter.next (hash_entry) == -1 || iter.advance () == -1)
        {
          break;
        }
      // Use an evaluator.
      TAO_Log_Constraint_Visitor evaluator (hash_entry->int_id_);

      // Does it match the constraint?
      if (interpreter.evaluate (evaluator) == 1)
        {
          if (delete_rec == 1)
            {
              if (this->recordstore_.remove (hash_entry->int_id_.id) == 0)
                count++;
            }
          else
            count++;
        }
    }

  return count;
}

CORBA::ULong
TAO_Log_i::match (const char* grammar,
                  const char *constraint
                  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   DsLogAdmin::InvalidGrammar,
                   DsLogAdmin::InvalidConstraint))
{
  this->check_grammar (grammar ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  CORBA::ULong count =
    this->match_i (constraint, 0 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (count);

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
  this->check_grammar (grammar ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  CORBA::ULong count =
    this->match_i (constraint, 1 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (count);

  this->reset_capacity_alarm_threshold (ACE_ENV_SINGLE_ARG_PARAMETER);

  return count;
}

CORBA::ULong
TAO_Log_i::delete_records_by_id (const DsLogAdmin::RecordIdList &ids
                                 ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->remove_old_records (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  CORBA::ULong numdone (0);

  for (CORBA::ULong i = 0; i < ids.length (); i++)
    {
      if (this->recordstore_.remove (ids [i]) == 0)
        {
          numdone++;
        }
    }

  if (numdone > 0)
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

  return numdone;
}

void
TAO_Log_i::write_records (const DsLogAdmin::Anys &records
                          ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   DsLogAdmin::LogFull,
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
                     DsLogAdmin::LogLocked,
                     DsLogAdmin::LogDisabled))
{
  this->remove_old_records (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // Check if the log is on duty
  // @@ Wait for Comittee ruling on the proper behavior
  DsLogAdmin::AvailabilityStatus avail_stat =
    this->get_availability_status (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // Log is not available for writing. Throw specific reason why.
  if (avail_stat.off_duty == 1)
    {

      // why are we off duty? investigate ...
      // Check if the log is full.
      if (avail_stat.log_full == 1
          && this->log_full_action_ == DsLogAdmin::halt)
        {
          ACE_THROW (DsLogAdmin::LogFull (0));
        }
      else   // Check the administrative state.
        {
          if (this->admin_state_ == DsLogAdmin::locked)
            ACE_THROW (DsLogAdmin::LogLocked ());
          else
            if (this->op_state_ == DsLogAdmin::disabled)
              ACE_THROW (DsLogAdmin::LogDisabled ());
        }
      return; // we are not scheduled at this time.
    }

  CORBA::Short num_written (0);
  DsLogAdmin::LogRecord record;

  for (CORBA::ULong i = 0; i < reclist.length (); i++)
    {
      // Check if the log is full.
      if (avail_status_.log_full == 1
          && this->log_full_action_ == DsLogAdmin::halt )
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
              {  // a few records. let the record store decide how many.

                if (this->recordstore_.purge_old_records () == -1)
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
  this->remove_old_records (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

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
TAO_Log_i::set_records_attribute (const char *grammar,
                                  const char *constraint,
                                  const DsLogAdmin::NVList
                                  &attr_list ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     DsLogAdmin::InvalidGrammar,
                     DsLogAdmin::InvalidConstraint,
                     DsLogAdmin::InvalidAttribute))
{
  this->remove_old_records (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  DsLogAdmin::Iterator_var iter_out;

  DsLogAdmin::RecordList_var rec_list =
    this->query (grammar,
                  constraint,
                   iter_out
                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  for (CORBA::ULong i = 0; i < rec_list->length (); ++i)
  {
    this->set_record_attribute (rec_list[i].id,
                                attr_list
                                ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (0);
  }
  return rec_list->length ();
}

DsLogAdmin::NVList*
TAO_Log_i::get_record_attribute (DsLogAdmin::RecordId id
                             ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   DsLogAdmin::InvalidRecordId))
{
  this->remove_old_records (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  DsLogAdmin::LogRecord rec;
  if (this->recordstore_.retrieve (id, rec) == -1)
    {
      ACE_THROW_RETURN (DsLogAdmin::InvalidRecordId (),
                        0);
    }

  DsLogAdmin::NVList* nvlist;
  ACE_NEW_THROW_EX (nvlist,
                    DsLogAdmin::NVList (rec.attr_list),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  return nvlist;
}

// @@ Should I just raise the exception?
void
TAO_Log_i::flush (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                     DsLogAdmin::UnsupportedQoS))
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
  // @@ Perhaps later use a MMAP_Memory_Pool as the backing store, and
  // just have this map to its sync method
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

  if (threshs.length () != 0)
    for (CORBA::ULong i = 0; i < threshs.length () - 1; i++)
      if (threshs[i] >= threshs[i +1])
        return false;

  return true;
}

CORBA::Boolean
TAO_Log_i::scheduled (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TimeBase::TimeT current_time;
  ACE_Time_Value now = ACE_OS::gettimeofday ();
  ORBSVCS_Time::Time_Value_to_TimeT (current_time, now);

  if ((current_time >= interval_.start) &&
          ((current_time <= interval_.stop) || (interval_.stop == 0)) )
  {
    if (weekly_intervals_.length () > 0)
    {
      TimeBase::TimeT current_time;
      ACE_Time_Value now = ACE_OS::gettimeofday ();
      ORBSVCS_Time::Time_Value_to_TimeT (current_time, now);

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
TAO_Log_i::check_grammar (const char* grammar
                          ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   DsLogAdmin::InvalidGrammar))
{
  // Verify that the grammar is "TCL".
  // The spec. asks for "extended TCL"
  if (ACE_OS::strcmp (grammar, QUERY_LANG_SUPPORTED_BY_LOG) != 0)
    ACE_THROW (DsLogAdmin::InvalidGrammar ());
}

void
TAO_Log_i::copy_attributes (DsLogAdmin::Log_ptr log
                            ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->remove_old_records (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

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

  TimeBase::TimeT time;
  ORBSVCS_Time::Time_Value_to_TimeT (time, ACE_OS::gettimeofday ());

  TimeBase::TimeT purge_time = time - this->max_record_life_;

  CORBA::ULongLong p_time = (CORBA::ULongLong) purge_time;

  static char out[256] = "";

  double temp1 =
    ACE_UINT64_DBLCAST_ADAPTER (p_time);

  ACE_OS::sprintf (out, "time > %.0f", temp1);

  // Use an Interpreter to build an expression tree.
  TAO_Log_Constraint_Interpreter interpreter (out
                                              ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Get the underlying storage.
  TAO_LogRecordStore::LOG_RECORD_STORE &store =
    this->recordstore_.get_storage ();

  // Create an iterator
  TAO_LogRecordStore::LOG_RECORD_STORE_ITER iter (store);

  CORBA::ULong len = store.current_size ();
  // How many entries?

  // Iterate over and populate the list.
  TAO_LogRecordStore::LOG_RECORD_HASH_MAP_ENTRY *hash_entry;

  CORBA::ULong count = 0; // count of matches found.

  for (CORBA::ULong i = 0; i < len; ++i)
    {
      if (iter.next (hash_entry) == -1 || iter.advance () == -1)
        {
          break;
        }
      // Use an evaluator.
      TAO_Log_Constraint_Visitor evaluator (hash_entry->int_id_);

      // Does it match the constraint?
      if (interpreter.evaluate (evaluator) == 1)
      {
            if (this->recordstore_.remove (hash_entry->int_id_.id) == 0)
              count++;
      }
    }
  this->reset_capacity_alarm_threshold (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_Log_i::check_capacity_alarm_threshold (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->remove_old_records (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::ULongLong max_size = this->recordstore_.get_max_size ();
  if (max_size != 0 && this->thresholds_.length () > 0)
    {
      CORBA::ULongLong current_size = this->recordstore_.get_current_size ();
      const CORBA::UShort percent =
        ACE_static_cast (
          CORBA::UShort,
          ((double) ACE_UINT64_DBLCAST_ADAPTER (current_size * 100U) /
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
          && this->log_full_action_ != DsLogAdmin::halt)
        {
          this->current_threshold_ = 0;
        }
    }
}

void
TAO_Log_i::reset_capacity_alarm_threshold (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::ULongLong max_size = this->recordstore_.get_max_size ();
  if (max_size != 0 && this->thresholds_.length() > 0)
    {
      CORBA::ULongLong current_size = this->recordstore_.get_current_size ();
      const CORBA::UShort percent =
        ACE_static_cast (
          CORBA::UShort,
          (((double) ACE_UINT64_DBLCAST_ADAPTER (current_size * 100U)) /
            (double) ACE_UINT64_DBLCAST_ADAPTER (max_size)));

      this->current_threshold_ = 0;

      while (this->current_threshold_ < this->thresholds_.length ()
             && this->thresholds_[this->current_threshold_] <= percent)
        ++this->current_threshold_;
    }
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class TAO_Unbounded_Sequence<DsLogAdmin::TimeInterval>;
#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate TAO_Unbounded_Sequence<DsLogAdmin::TimeInterval>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
