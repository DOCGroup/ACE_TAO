#include "orbsvcs/Log/Hash_LogRecordStore.h"
#include "orbsvcs/Log/Hash_Iterator_i.h"
#include "orbsvcs/Log/Log_Constraint_Interpreter.h"
#include "orbsvcs/Log/Log_Constraint_Visitors.h"
#include "orbsvcs/Time_Utilities.h"
#include "tao/AnyTypeCode/Any_Unknown_IDL_Type.h"
#include "tao/ORB_Core.h"
#include "tao/debug.h"
#include "ace/OS_NS_sys_time.h"

ACE_RCSID (Log,
           Hash_LogRecordStore,
           "$Id$")

TAO_Hash_LogRecordStore::TAO_Hash_LogRecordStore (CORBA::ORB_ptr orb,
                                                  DsLogAdmin::LogId logid,
                                                  DsLogAdmin::LogFullActionType log_full_action,
                                                  CORBA::ULongLong max_size,
                                                  const DsLogAdmin::CapacityAlarmThresholdList* thresholds
                                                  )
  : maxid_ (0),
    max_size_ (max_size),
    id_ (logid),
    current_size_ (0),
    num_records_ (0),
    max_rec_list_len_ (LOG_DEFAULT_MAX_REC_LIST_LEN),
    admin_state_ (DsLogAdmin::unlocked),
    forward_state_ (DsLogAdmin::on),
    log_full_action_ (log_full_action),
    max_record_life_ (0),
    reactor_ (orb->orb_core ()->reactor ())
{
  interval_.start = 0;
  interval_.stop = 0;

  if (thresholds)
    {
      this->thresholds_ = *thresholds;
    }
  else
    {
      this->thresholds_.length(1);
      this->thresholds_[0] = 100;
    }
}

TAO_Hash_LogRecordStore::~TAO_Hash_LogRecordStore (void)
{
  // No-Op.
}

int
TAO_Hash_LogRecordStore::open (void)
{
  return rec_hash_.open ();
}

int
TAO_Hash_LogRecordStore::close (void)
{
  // Close the hash
  return rec_hash_.close ();
}

CORBA::ULongLong
TAO_Hash_LogRecordStore::get_current_size (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_READ_GUARD_THROW_EX (ACE_SYNCH_RW_MUTEX,
                           guard,
                           lock_,
                           CORBA::INTERNAL ());
  ACE_CHECK_RETURN (0);

  return this->current_size_;
}

CORBA::ULongLong
TAO_Hash_LogRecordStore::get_n_records (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_READ_GUARD_THROW_EX (ACE_SYNCH_RW_MUTEX,
                           guard,
                           lock_,
                           CORBA::INTERNAL ());
  ACE_CHECK_RETURN (0);

  return this->num_records_;
}

int
TAO_Hash_LogRecordStore::log (const DsLogAdmin::LogRecord &const_rec
			      ACE_ENV_ARG_DECL)
{
  ACE_WRITE_GUARD_THROW_EX (ACE_SYNCH_RW_MUTEX,
			    guard,
			    lock_,
			    CORBA::INTERNAL ());
  ACE_CHECK_RETURN (-1);

  // Get log record size...
  size_t record_size = log_record_size (const_rec);

  // Check if we are allowed to write...
  if (max_size_ !=0 && ((current_size_ + record_size) >= max_size_))
    return 1; // return code for log rec. full

  // Copy record...
  DsLogAdmin::LogRecord rec = const_rec;

  // Initialize a couple of fields first...
  // ACE emulation of U Long Long (for platforms that don't have one)
  // does not define postfix operators
  rec.id = ++maxid_;
  // TODO: Reuse ids by keeping a list.

  ORBSVCS_Time::Time_Value_to_TimeT(rec.time,ACE_OS::gettimeofday());

  // First, bind the id to the LogRecord in the hash_map
  if (this->rec_hash_.bind (rec.id, rec) != 0)
    {
#if defined (ACE_LACKS_LONGLONG_T)
           ACE_ERROR_RETURN ((LM_ERROR,
                         "LogRecordStore (%P|%t):Failed to bind %d in the hash map\n",
                         ACE_U64_TO_U32(rec.id)),
                             -1);
#else
           ACE_ERROR_RETURN ((LM_ERROR,
                         "LogRecordStore (%P|%t):Failed to bind %Q in the hash map\n",
                         rec.id),
                       -1);
#endif
    }

  // Increment the number of records in the log
  ++this->num_records_;
  this->current_size_ += record_size;

  return 0;
}

int
TAO_Hash_LogRecordStore::retrieve (DsLogAdmin::RecordId id,
				   DsLogAdmin::LogRecord &rec
				   ACE_ENV_ARG_DECL)
{
  ACE_READ_GUARD_THROW_EX (ACE_SYNCH_RW_MUTEX,
			   guard,
			   lock_,
			   CORBA::INTERNAL ());
  ACE_CHECK_RETURN (-1);

  int retval = rec_hash_.find (id, rec);
  return retval;
}

int
TAO_Hash_LogRecordStore::update (DsLogAdmin::LogRecord &rec
				 ACE_ENV_ARG_DECL)
{
  ACE_WRITE_GUARD_THROW_EX (ACE_SYNCH_RW_MUTEX,
			    guard,
			    lock_,
			    CORBA::INTERNAL ());
  ACE_CHECK_RETURN (-1);

  DsLogAdmin::LogRecord oldrec;

  if (rec_hash_.unbind (rec.id, oldrec) != 0)
    {
      return -1;
    }

  --this->num_records_;
  this->current_size_ -= log_record_size(oldrec);

  if (rec_hash_.bind (rec.id, rec) != 0)
    {
      return -1;
    }

  ++this->num_records_;
  this->current_size_ += log_record_size(rec);

  return 0;
}

int
TAO_Hash_LogRecordStore::remove_i (DsLogAdmin::RecordId id)
{
  DsLogAdmin::LogRecord rec;
  if (rec_hash_.unbind (id, rec) != 0)
    {
      return -1;
    }

  --this->num_records_;
  this->current_size_ -= log_record_size(rec);
  // TODO: return ids to a reuse list.

  return 0;
}

int
TAO_Hash_LogRecordStore::remove (DsLogAdmin::RecordId id
				 ACE_ENV_ARG_DECL)
{
  ACE_WRITE_GUARD_THROW_EX (ACE_SYNCH_RW_MUTEX,
			    guard,
			    lock_,
			    CORBA::INTERNAL ());
  ACE_CHECK_RETURN (-1);

  return remove_i (id);
}


int
TAO_Hash_LogRecordStore::purge_old_records (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_WRITE_GUARD_THROW_EX (ACE_SYNCH_RW_MUTEX,
			    guard,
			    lock_,
			    CORBA::INTERNAL ());
  ACE_CHECK_RETURN (-1);

  CORBA::ULongLong num_records_to_purge = this->num_records_ * 5U / 100U;

  if (num_records_to_purge < 1)
    num_records_to_purge = 1;

  CORBA::ULong count = 0; // count of matches found.

  if (num_records_to_purge > 0 )
    {
      LOG_RECORD_STORE_ITER iter (rec_hash_);
      LOG_RECORD_HASH_MAP_ENTRY *hash_entry = 0;

      for (CORBA::ULongLong i = 0; i < num_records_to_purge; ++i)
        {
          if (iter.next (hash_entry) == -1 || iter.advance () == -1)
            break;

          if (this->remove_i (hash_entry->int_id_.id) == 0)
            count++;
        }
    }

  return count;
}

int
TAO_Hash_LogRecordStore::flush (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  return 0;
}

size_t
TAO_Hash_LogRecordStore::log_record_size (const DsLogAdmin::LogRecord &rec)
{
  size_t mb_size = 0;
  TAO::Any_Impl *impl = rec.info.impl ();

  if (impl->encoded ())
    {
      TAO::Unknown_IDL_Type *unk =
        dynamic_cast<TAO::Unknown_IDL_Type *> (impl);

      mb_size = unk->_tao_get_cdr ().start ()->length ();
    }
  else
    {
      // If the Any is not encoded, it just has a stored value
      // instead of a CDR stream, not sure what info would be
      // useful here.
    }

  return sizeof (rec) + mb_size;
}

DsLogAdmin::RecordList*
TAO_Hash_LogRecordStore::query_i (const char *constraint,
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

  // Allocate the list of <how_many> length.
  DsLogAdmin::RecordList* rec_list;
  ACE_NEW_THROW_EX (rec_list,
                    DsLogAdmin::RecordList (how_many),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  // Create iterators
  LOG_RECORD_STORE_ITER iter (rec_hash_.begin ());
  LOG_RECORD_STORE_ITER iter_end (rec_hash_.end ());

  CORBA::ULong count = 0;       // count of matches found.

  for ( ; ((iter != iter_end) && (count < how_many)); ++iter)
    {
      // Use an evaluator.
      TAO_Log_Constraint_Visitor evaluator ((*iter).int_id_);

      // Does it match the constraint?
      if (interpreter.evaluate (evaluator) == 1)
      {
        if (TAO_debug_level > 0)
#if defined (ACE_LACKS_LONGLONG_T)
               ACE_DEBUG ((LM_DEBUG,"Matched constraint! d = %d, Time = %d\n",
                      ACE_U64_TO_U32 ((*iter).int_id_.id),
                      ACE_U64_TO_U32 ((*iter).int_id_.time)));

#else
               ACE_DEBUG ((LM_DEBUG,"Matched constraint! d = %Q, Time = %Q\n",
                      (*iter).int_id_.id,
                      (*iter).int_id_.time));
#endif

        (*rec_list)[count] = (*iter).int_id_;
        // copy the log record.
        count++;
      }
    }

  rec_list->length (count);

  if (iter != iter_end)         // There are more records to process.
    {
      // Create an iterator to pass out.
      TAO_Hash_Iterator_i *iter_query = 0;
      ACE_NEW_THROW_EX (iter_query,
                        TAO_Hash_Iterator_i (this->reactor_,
                                             iter,
                                             iter_end,
                                             count,
                                             constraint,
                                             this->max_rec_list_len_),
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
TAO_Hash_LogRecordStore::query (const char *grammar,
                                const char *constraint,
                                DsLogAdmin::Iterator_out iter_out
                                ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   DsLogAdmin::InvalidGrammar,
                   DsLogAdmin::InvalidConstraint))
{
  ACE_READ_GUARD_THROW_EX (ACE_SYNCH_RW_MUTEX,
			   guard,
			   lock_,
			   CORBA::INTERNAL ());
  ACE_CHECK_RETURN (0);

  this->check_grammar (grammar ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->query_i (constraint,
                        iter_out,
                        this->max_rec_list_len_
                        ACE_ENV_ARG_PARAMETER);
}

DsLogAdmin::RecordList*
TAO_Hash_LogRecordStore::retrieve (DsLogAdmin::TimeT from_time,
                                   CORBA::Long how_many,
                                   DsLogAdmin::Iterator_out iter_out
                                   ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_READ_GUARD_THROW_EX (ACE_SYNCH_RW_MUTEX,
			   guard,
			   lock_,
			   CORBA::INTERNAL ());
  ACE_CHECK_RETURN (0);

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
TAO_Hash_LogRecordStore::match_i (const char *constraint,
                                  CORBA::Boolean delete_rec
                                  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   DsLogAdmin::InvalidConstraint))
{
  // Use an Interpreter to build an expression tree.
  TAO_Log_Constraint_Interpreter interpreter (constraint
                                              ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  // Create iterators
  LOG_RECORD_STORE_ITER iter (rec_hash_.begin ());
  LOG_RECORD_STORE_ITER iter_end (rec_hash_.end ());

  CORBA::ULong count = 0; // count of matches found.

  for ( ; iter != iter_end; ++iter)
    {
      // Use an evaluator.
      TAO_Log_Constraint_Visitor evaluator ((*iter).int_id_);

      // Does it match the constraint?
      if (interpreter.evaluate (evaluator) == 1)
        {
          if (delete_rec == 1)
            {
              if (this->remove_i ((*iter).int_id_.id) == 0)
                count++;
            }
          else
            count++;
        }
    }

  return count;
}

CORBA::ULong
TAO_Hash_LogRecordStore::match (const char* grammar,
                  const char *constraint
                  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   DsLogAdmin::InvalidGrammar,
                   DsLogAdmin::InvalidConstraint))
{
  ACE_READ_GUARD_THROW_EX (ACE_SYNCH_RW_MUTEX,
			   guard,
			   lock_,
			   CORBA::INTERNAL ());
  ACE_CHECK_RETURN (0);

  this->check_grammar (grammar ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  CORBA::ULong count =
    this->match_i (constraint, 0 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (count);

  return count;
}

CORBA::ULong
TAO_Hash_LogRecordStore::delete_records (const char *grammar,
                           const char *constraint
                                         ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     DsLogAdmin::InvalidGrammar,
                     DsLogAdmin::InvalidConstraint))
{
  ACE_WRITE_GUARD_THROW_EX (ACE_SYNCH_RW_MUTEX,
			    guard,
			    lock_,
			    CORBA::INTERNAL ());
  ACE_CHECK_RETURN (0);

  this->check_grammar (grammar ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  CORBA::ULong count =
    this->match_i (constraint, 1 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return count;
}

CORBA::ULong
TAO_Hash_LogRecordStore::delete_records_by_id (const DsLogAdmin::RecordIdList &ids
                                               ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_WRITE_GUARD_THROW_EX (ACE_SYNCH_RW_MUTEX,
			    guard,
			    lock_,
			    CORBA::INTERNAL ());
  ACE_CHECK_RETURN (0);

  CORBA::ULong count (0);

  for (CORBA::ULong i = 0; i < ids.length (); i++)
    {
      if (this->remove_i (ids [i]) == 0)
        {
          count++;
        }
    }

  return count;
}

CORBA::ULong
TAO_Hash_LogRecordStore::remove_old_records (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_WRITE_GUARD_THROW_EX (ACE_SYNCH_RW_MUTEX,
			    guard,
			    lock_,
			    CORBA::INTERNAL ());
  ACE_CHECK_RETURN (0);

  if (this->max_record_life_ == 0) {
    return 0;
  }

  TimeBase::TimeT purge_time;
  ORBSVCS_Time::Time_Value_to_TimeT (purge_time,
                                     (ACE_OS::gettimeofday() - ACE_Time_Value(this->max_record_life_)));

  CORBA::ULongLong p_time = (CORBA::ULongLong) purge_time;

  static char out[256] = "";

  double temp1 =
    static_cast<double> (ACE_UINT64_DBLCAST_ADAPTER (p_time));

  ACE_OS::sprintf (out, "time < %.0f", temp1);

  // Use an Interpreter to build an expression tree.
  TAO_Log_Constraint_Interpreter interpreter (out
                                              ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  // Create iterators
  LOG_RECORD_STORE_ITER iter (rec_hash_.begin ());
  LOG_RECORD_STORE_ITER iter_end (rec_hash_.end ());

  CORBA::ULong count = 0; // count of matches found.

  for ( ; iter != iter_end; ++iter)
    {
      // Use an evaluator.
      TAO_Log_Constraint_Visitor evaluator ((*iter).int_id_);

      // Does it match the constraint?
      if (interpreter.evaluate (evaluator) == 1)
        {
          if (this->remove_i ((*iter).int_id_.id) == 0)
            count++;
        }
    }

  return count;
}

void
TAO_Hash_LogRecordStore::check_grammar (const char* grammar
                                        ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   DsLogAdmin::InvalidGrammar))
{
  // Verify grammar
  if (ACE_OS::strcmp (grammar, "TCL") != 0 &&
      ACE_OS::strcmp (grammar, "ETCL") != 0 &&
      ACE_OS::strcmp (grammar, "EXTENDED_TCL") != 0)
    ACE_THROW (DsLogAdmin::InvalidGrammar ());
}


DsLogAdmin::AdministrativeState
TAO_Hash_LogRecordStore::get_administrative_state (ACE_ENV_SINGLE_ARG_DECL) const
{
  ACE_READ_GUARD_THROW_EX (ACE_SYNCH_RW_MUTEX,
                           guard,
                           lock_,
                           CORBA::INTERNAL ());
  ACE_CHECK_RETURN (this->admin_state_);

  return this->admin_state_;
}

void
TAO_Hash_LogRecordStore::set_administrative_state (DsLogAdmin::AdministrativeState state
                                                   ACE_ENV_ARG_DECL)
{
  ACE_WRITE_GUARD_THROW_EX (ACE_SYNCH_RW_MUTEX,
                            guard,
                            lock_,
                            CORBA::INTERNAL ());
  ACE_CHECK;

  this->admin_state_ = state;
}


DsLogAdmin::CapacityAlarmThresholdList*
TAO_Hash_LogRecordStore::get_capacity_alarm_thresholds (ACE_ENV_SINGLE_ARG_DECL) const
{
  ACE_READ_GUARD_THROW_EX (ACE_SYNCH_RW_MUTEX,
                           guard,
                           lock_,
                           CORBA::INTERNAL ());
  ACE_CHECK_RETURN (0);

  DsLogAdmin::CapacityAlarmThresholdList* ret_val;
  ACE_NEW_THROW_EX (ret_val,
                    DsLogAdmin::CapacityAlarmThresholdList (this->thresholds_),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  return ret_val;
}

void
TAO_Hash_LogRecordStore::set_capacity_alarm_thresholds (const DsLogAdmin::CapacityAlarmThresholdList& thresholds
                                                        ACE_ENV_ARG_DECL)
{
  ACE_WRITE_GUARD_THROW_EX (ACE_SYNCH_RW_MUTEX,
                            guard,
                            lock_,
                            CORBA::INTERNAL ());
  ACE_CHECK;

  this->thresholds_ = thresholds;
}



DsLogAdmin::ForwardingState
TAO_Hash_LogRecordStore::get_forwarding_state (ACE_ENV_SINGLE_ARG_DECL) const
{
  ACE_READ_GUARD_THROW_EX (ACE_SYNCH_RW_MUTEX,
                           guard,
                           lock_,
                           CORBA::INTERNAL ());
  ACE_CHECK_RETURN (this->forward_state_);

  return this->forward_state_;
}

void
TAO_Hash_LogRecordStore::set_forwarding_state (DsLogAdmin::ForwardingState state
                                               ACE_ENV_ARG_DECL)
{
  ACE_WRITE_GUARD_THROW_EX (ACE_SYNCH_RW_MUTEX,
                            guard,
                            lock_,
                            CORBA::INTERNAL ());
  ACE_CHECK;

  this->forward_state_ = state;
}

DsLogAdmin::TimeInterval
TAO_Hash_LogRecordStore::get_interval (ACE_ENV_SINGLE_ARG_DECL) const
{
  ACE_READ_GUARD_THROW_EX (ACE_SYNCH_RW_MUTEX,
                           guard,
                           lock_,
                           CORBA::INTERNAL ());
  ACE_CHECK_RETURN (this->interval_);

  return this->interval_;
}

void
TAO_Hash_LogRecordStore::set_interval (const DsLogAdmin::TimeInterval &interval
                                       ACE_ENV_ARG_DECL)
{
  ACE_WRITE_GUARD_THROW_EX (ACE_SYNCH_RW_MUTEX,
                            guard,
                            lock_,
                            CORBA::INTERNAL ());
  ACE_CHECK;

  this->interval_ = interval;
}


DsLogAdmin::LogFullActionType
TAO_Hash_LogRecordStore::get_log_full_action (ACE_ENV_SINGLE_ARG_DECL) const
{
  ACE_READ_GUARD_THROW_EX (ACE_SYNCH_RW_MUTEX,
                           guard,
                           lock_,
                           CORBA::INTERNAL ());
  ACE_CHECK_RETURN (this->log_full_action_);

  return this->log_full_action_;
}

void
TAO_Hash_LogRecordStore::set_log_full_action (DsLogAdmin::LogFullActionType action
                                              ACE_ENV_ARG_DECL)
{
  ACE_WRITE_GUARD_THROW_EX (ACE_SYNCH_RW_MUTEX,
                            guard,
                            lock_,
                            CORBA::INTERNAL ());
  ACE_CHECK;

  this->log_full_action_ = action;
}


CORBA::ULong
TAO_Hash_LogRecordStore::get_max_record_life (ACE_ENV_SINGLE_ARG_DECL) const
{
  ACE_READ_GUARD_THROW_EX (ACE_SYNCH_RW_MUTEX,
                           guard,
                           lock_,
                           CORBA::INTERNAL ());
  ACE_CHECK_RETURN (0);

  return this->max_record_life_;
}

void
TAO_Hash_LogRecordStore::set_max_record_life (CORBA::ULong max_record_life
                                              ACE_ENV_ARG_DECL)
{
  ACE_WRITE_GUARD_THROW_EX (ACE_SYNCH_RW_MUTEX,
                            guard,
                            lock_,
                            CORBA::INTERNAL ());
  ACE_CHECK;

  this->max_record_life_ = max_record_life;
}

CORBA::ULongLong
TAO_Hash_LogRecordStore::get_max_size (ACE_ENV_SINGLE_ARG_DECL) const
{
  ACE_READ_GUARD_THROW_EX (ACE_SYNCH_RW_MUTEX,
                           guard,
                           lock_,
                           CORBA::INTERNAL ());
  ACE_CHECK_RETURN (0);

  return this->max_size_;
}

void
TAO_Hash_LogRecordStore::set_max_size (CORBA::ULongLong size
                                       ACE_ENV_ARG_DECL)
{
  ACE_WRITE_GUARD_THROW_EX (ACE_SYNCH_RW_MUTEX,
                            guard,
                            lock_,
                            CORBA::INTERNAL ());
  ACE_CHECK;

  this->max_size_ = size;
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Hash_Map_Entry<DsLogAdmin::RecordId,DsLogAdmin::LogRecord>;
template class ACE_Hash_Map_Manager<DsLogAdmin::RecordId,DsLogAdmin::LogRecord,ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<DsLogAdmin::RecordId, DsLogAdmin::LogRecord,
  ACE_Hash<DsLogAdmin::RecordId>, ACE_Equal_To<DsLogAdmin::RecordId>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator<DsLogAdmin::RecordId,DsLogAdmin::LogRecord,ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<DsLogAdmin::RecordId, DsLogAdmin::LogRecord, ACE_Hash<DsLogAdmin::RecordId>,
  ACE_Equal_To<DsLogAdmin::RecordId>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<DsLogAdmin::RecordId, DsLogAdmin::LogRecord, ACE_Hash<DsLogAdmin::RecordId>,
  ACE_Equal_To<DsLogAdmin::RecordId>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator<DsLogAdmin::RecordId,DsLogAdmin::LogRecord,ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<DsLogAdmin::RecordId, DsLogAdmin::LogRecord, ACE_Hash<DsLogAdmin::RecordId>,
  ACE_Equal_To<DsLogAdmin::RecordId>, ACE_Null_Mutex>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Hash_Map_Entry<DsLogAdmin::RecordId,DsLogAdmin::LogRecord>
#pragma instantiate ACE_Hash_Map_Manager<DsLogAdmin::RecordId,DsLogAdmin::LogRecord,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager_Ex<DsLogAdmin::RecordId, DsLogAdmin::LogRecord, ACE_Hash<DsLogAdmin::RecordId>,
ACE_Equal_To<DsLogAdmin::RecordId>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator<DsLogAdmin::RecordId,DsLogAdmin::LogRecord,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<DsLogAdmin::RecordId, DsLogAdmin::LogRecord, ACE_Hash<DsLogAdmin::RecordId>,
ACE_Equal_To<DsLogAdmin::RecordId>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<DsLogAdmin::RecordId, DsLogAdmin::LogRecord, ACE_Hash<DsLogAdmin::RecordId>,
ACE_Equal_To<DsLogAdmin::RecordId>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<DsLogAdmin::RecordId,DsLogAdmin::LogRecord,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<DsLogAdmin::RecordId, DsLogAdmin::LogRecord, ACE_Hash<DsLogAdmin::RecordId>,
ACE_Equal_To<DsLogAdmin::RecordId>, ACE_Null_Mutex>

#endif /* ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA */
