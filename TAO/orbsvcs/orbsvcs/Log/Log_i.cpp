// $Id$

#include "orbsvcs/Log/Log_i.h"
#include "orbsvcs/Log/Iterator_i.h"
#include "orbsvcs/Log/Log_Constraint_Interpreter.h"
#include "orbsvcs/Log/Log_Constraint_Visitors.h"

#include "tao/debug.h"

#define QUERY_LANG_SUPPORTED_BY_LOG "TCL"

Log_i::Log_i (DsLogAdmin::LogMgr_ptr factory,
              DsLogAdmin::LogId id,
              DsLogAdmin::LogFullAction log_full_action,
              CORBA::ULongLong max_size,
              ACE_Reactor *reactor)
  : factory_ (DsLogAdmin::LogMgr::_duplicate (factory)),
    log_full_action_ (log_full_action),
    logid_ (id),
    admin_state_ (DsLogAdmin::locked),
    forward_state_ (DsLogAdmin::off),
    op_state_ (DsLogAdmin::disabled),
    reactor_ (reactor),
    recordstore_ (max_size),
    max_rec_list_len_ (LOG_DEFAULT_MAX_REC_LIST_LEN)
{
  avail_status_.off_duty = 0;
  avail_status_.log_full = 0;
}

void
Log_i::init (ACE_ENV_SINGLE_ARG_DECL)
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
Log_i::my_factory (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return DsLogAdmin::LogMgr::_duplicate (factory_.in ());
}

DsLogAdmin::LogId
Log_i::id (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return logid_;
}

DsLogAdmin::QoSList*
Log_i::get_qos (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  DsLogAdmin::QoSList* ret_val;
  ACE_NEW_THROW_EX (ret_val,
                    DsLogAdmin::QoSList (this->qoslist_),
                    CORBA::NO_MEMORY ());

  return ret_val;
}

void
Log_i::set_qos (const DsLogAdmin::QoSList &qos
                ACE_ENV_ARG_DECL)
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
Log_i::get_max_record_life (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return max_record_life_;
}

void
Log_i::set_max_record_life (CORBA::ULong life
                            ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // life is in seconds.
  // @@ TODO: Need to iterate over all records in storage to timeout all
  // those outside the lifetime....
  max_record_life_ = life;
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

CORBA::ULongLong
Log_i::get_max_size (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return recordstore_.get_max_size ();
}

void
Log_i::set_max_size (CORBA::ULongLong size
                     ACE_ENV_ARG_DECL)
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
Log_i::get_current_size (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->recordstore_.get_current_size ();
}

CORBA::ULongLong
Log_i::get_n_records (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->recordstore_.get_n_records ();
}

DsLogAdmin::LogFullAction
Log_i::get_log_full_action (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return log_full_action_;
}

void
Log_i::set_log_full_action (DsLogAdmin::LogFullAction action
                            ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  log_full_action_ = action;
}

DsLogAdmin::AdministrativeState
Log_i::get_administrative_state (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return admin_state_;
}

void
Log_i::set_administrative_state (DsLogAdmin::AdministrativeState state
                                 ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->admin_state_ = state;
}

DsLogAdmin::ForwardingState
Log_i::get_forwarding_state (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->forward_state_;
}

void
Log_i::set_forwarding_state (DsLogAdmin::ForwardingState state
                             ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->forward_state_ = state;
}

DsLogAdmin::OperationalState
Log_i::get_operational_state (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->op_state_;
}

DsLogAdmin::TimeInterval
Log_i::get_interval (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->interval_;
}

void
Log_i::set_interval (const DsLogAdmin::TimeInterval &interval
                     ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   DsLogAdmin::InvalidTime,
                   DsLogAdmin::InvalidTimeInterval))
{
  //@@ TODO: validate the time.
  //         modify the timer settings to the new values.
  this->interval_ = interval;
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

DsLogAdmin::AvailabilityStatus
Log_i::get_availability_status (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
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

DsLogAdmin::CapacityAlarmThresholdList*
Log_i::get_capacity_alarm_thresholds (ACE_ENV_SINGLE_ARG_DECL)
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
Log_i::set_capacity_alarm_thresholds (const
                                      DsLogAdmin::CapacityAlarmThresholdList
                                      &threshs
                                      ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   DsLogAdmin::InvalidThreshold))
{
  // @@ TODO: validate thresholds
  //          How do we implement thresholds in DsLogAdmin !?
  //          According to the spec, the log must "notify" when threshold
  //          conditions are reached.we need to be an Event Channel to do that.  //          now what!?
  this->thresholds_ = threshs;
}

DsLogAdmin::WeekMask*
Log_i::get_week_mask (ACE_ENV_SINGLE_ARG_DECL)
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
Log_i::set_week_mask (const DsLogAdmin::WeekMask &masks
                      ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   DsLogAdmin::InvalidTime,
                   DsLogAdmin::InvalidTimeInterval,
                   DsLogAdmin::InvalidMask))
{
  // @@ TODO: validate masks.
  //           Activate various timers to trigger appropriate acition.
  this->weekmask_ = masks;
}

DsLogAdmin::RecordList*
Log_i::query_i (const char *constraint,
                DsLogAdmin::Iterator_out &iter_out,
                CORBA::ULong how_many
                ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   DsLogAdmin::InvalidConstraint))
{
  // Use an Interpreter to build an expression tree.
  TAO_Log_Constraint_Interpreter interpreter (constraint
                                              ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  // Sequentially iterate over all the records and pick the ones that
  // meet the constraints.

  // get the underlying storage.
  LogRecordStore::LOG_RECORD_STORE &store =
    this->recordstore_.get_storage ();

  // Create an iterator
  LogRecordStore::LOG_RECORD_HASH_MAP_ITER iter (store);

  CORBA::ULong len = store.current_size ();
  // How many entries?

  // Iterate over and populate the list.
  LogRecordStore::LOG_RECORD_HASH_MAP_ENTRY *hash_entry;

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
      TAO_Log_Constraint_Evaluator evaluator (hash_entry->int_id_);

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
      Iterator_i *iter_query = 0;
      ACE_NEW_THROW_EX (iter_query,
                        Iterator_i (store,
                                    i,
                                    constraint,
                                    len,
                                    how_many),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (rec_list);

      // Activate it.
      iter_out = iter_query->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (rec_list);

      // Give ownership to the POA.
      this->_remove_ref (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (rec_list);
    }

  return rec_list;
}

DsLogAdmin::RecordList*
Log_i::query (const char *grammar,
              const char *constraint,
              DsLogAdmin::Iterator_out iter_out
              ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   DsLogAdmin::InvalidGrammar,
                   DsLogAdmin::InvalidConstraint))
{
  this->check_grammar (grammar ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  DsLogAdmin::RecordList* rec_list =
    this->query_i (constraint,
                   iter_out,
                   this->max_rec_list_len_
                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (rec_list);

  return rec_list;
}

DsLogAdmin::RecordList*
Log_i::retrieve (DsLogAdmin::TimeT from_time,
                 CORBA::Long how_many,
                 DsLogAdmin::Iterator_out iter_out
                 ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Decide between forward vs backward retrieval.
  ACE_TCHAR constraint[32];
  ACE_TCHAR uint64_formating[32];

  #if defined (ACE_LACKS_LONGLONG_T)
         ACE_OS::sprintf (uint64_formating,
                   ACE_UINT64_FORMAT_SPECIFIER, ACE_U64_TO_U32 (from_time));
  #else
         ACE_OS::sprintf (uint64_formating,
                   ACE_UINT64_FORMAT_SPECIFIER, from_time);
  #endif

  if (how_many >= 0)
    ACE_OS::sprintf (constraint, ACE_LIB_TEXT("time >= %s"), uint64_formating);
  else
    ACE_OS::sprintf (constraint, ACE_LIB_TEXT("time < %s"), uint64_formating);

  DsLogAdmin::RecordList* rec_list =
    this->query_i (ACE_TEXT_ALWAYS_CHAR(constraint),
                   iter_out,
                   how_many
                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (rec_list);

  return rec_list;
}

CORBA::ULong
Log_i::match_i (const char *constraint,
                CORBA::Boolean delete_rec
                ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   DsLogAdmin::InvalidConstraint))
{
  // Use an Interpreter to build an expression tree.
  TAO_Log_Constraint_Interpreter interpreter (constraint
                                              ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  // Get the underlying storage.
  LogRecordStore::LOG_RECORD_STORE &store =
    this->recordstore_.get_storage ();

  // Create an iterator
  LogRecordStore::LOG_RECORD_STORE_ITER iter (store);

  CORBA::ULong len = store.current_size ();
  // How many entries?

  // Iterate over and populate the list.
  LogRecordStore::LOG_RECORD_HASH_MAP_ENTRY *hash_entry;

  CORBA::ULong count = 0; // count of matches found.

  for (CORBA::ULong i = 0; i < len; ++i)
    {
      if (iter.next (hash_entry) == -1 || iter.advance () == -1)
        {
          break;
        }
      // Use an evaluator.
      TAO_Log_Constraint_Evaluator evaluator (hash_entry->int_id_);

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
Log_i::match (const char* grammar,
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
Log_i::delete_records (const char *grammar,
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

  return count;
}

CORBA::ULong
Log_i::delete_records_by_id (const
                             DsLogAdmin::RecordIdList &ids
                             ACE_ENV_ARG_DECL_NOT_USED)
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
Log_i::write_records (const DsLogAdmin::Anys &records
                      ACE_ENV_ARG_DECL)
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

  this->write_recordlist (reclist ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
Log_i::write_recordlist (const DsLogAdmin::RecordList &reclist
                         ACE_ENV_ARG_DECL)
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
    this->get_availability_status (ACE_ENV_SINGLE_ARG_PARAMETER);
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
                             const DsLogAdmin::NVList &attr_list
                             ACE_ENV_ARG_DECL)
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
                              &attr_list ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     DsLogAdmin::InvalidGrammar,
                     DsLogAdmin::InvalidConstraint,
                     DsLogAdmin::InvalidAttribute))
{
  ACE_UNUSED_ARG (grammar);
  ACE_UNUSED_ARG (constraint);
  ACE_UNUSED_ARG (attr_list);

  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

DsLogAdmin::NVList*
Log_i::get_record_attribute (DsLogAdmin::RecordId id
                             ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   DsLogAdmin::InvalidRecordId))
{
  DsLogAdmin::LogRecord rec;
  if (this->recordstore_.retrieve (id, rec) == -1)
    {
      ACE_THROW_RETURN(DsLogAdmin::InvalidRecordId (),
                       0);
    }

  DsLogAdmin::NVList* nvlist;
  ACE_NEW_THROW_EX (nvlist,
                    DsLogAdmin::NVList (rec.attr_list),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  return nvlist;
}

// @@ These two methods are non-trivial. Let's hold off on them for a while
DsLogAdmin::Log_ptr
Log_i::copy (DsLogAdmin::LogId_out id
             ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_UNUSED_ARG (id);
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

DsLogAdmin::Log_ptr
Log_i::copy_with_id (DsLogAdmin::LogId id
                     ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   DsLogAdmin::LogIdAlreadyExists))
{
  ACE_UNUSED_ARG (id);
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

// @@ Should I just raise the exception?
void
Log_i::flush (ACE_ENV_SINGLE_ARG_DECL)
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

void
Log_i::check_grammar (const char* grammar
                      ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   DsLogAdmin::InvalidGrammar))
{
  // Verify that the grammar is "TCL".
  // The spec. asks for "extended TCL"
  if (ACE_OS::strcmp (grammar, QUERY_LANG_SUPPORTED_BY_LOG) != 0)
    ACE_THROW (DsLogAdmin::InvalidGrammar ());
}
