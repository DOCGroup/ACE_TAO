// $Id$

#include "orbsvcs/Log/LogMgr_i.h"
#include "orbsvcs/Log/Hash_LogRecordStore.h"
#include "orbsvcs/Log/Hash_Iterator_i.h"
#include "orbsvcs/Log/Log_Constraint_Interpreter.h"
#include "orbsvcs/Log/Log_Constraint_Visitors.h"
#include "orbsvcs/Time_Utilities.h"
#include "tao/Utils/PolicyList_Destroyer.h"
#include "tao/AnyTypeCode/Any_Unknown_IDL_Type.h"
#include "tao/ORB_Core.h"
#include "tao/debug.h"
#include "ace/OS_NS_sys_time.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Hash_LogRecordStore::TAO_Hash_LogRecordStore (
  TAO_LogMgr_i* logmgr_i,
  DsLogAdmin::LogId logid,
  DsLogAdmin::LogFullActionType log_full_action,
  CORBA::ULongLong max_size,
  const DsLogAdmin::CapacityAlarmThresholdList* thresholds)
  : logmgr_i_ (logmgr_i),
    maxid_ (0),
    max_size_ (max_size),
    id_ (logid),
    current_size_ (0),
    num_records_ (0),
    gauge_ (0),
    max_rec_list_len_ (LOG_DEFAULT_MAX_REC_LIST_LEN),
    admin_state_ (DsLogAdmin::unlocked),
    forward_state_ (DsLogAdmin::on),
    log_full_action_ (log_full_action),
    max_record_life_ (0),
    reactor_ (logmgr_i_->orb()->orb_core ()->reactor ())
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

  this->log_qos_.length(1);
  this->log_qos_[0] = DsLogAdmin::QoSNone;

  PortableServer::POA_ptr log_poa =
    logmgr_i_->log_poa();

  // Create POA for iterators
  TAO::Utils::PolicyList_Destroyer policies (2);
  policies.length (2);

  policies[0] =
    log_poa->create_lifespan_policy (PortableServer::TRANSIENT);
  policies[1] =
    log_poa->create_id_assignment_policy (PortableServer::SYSTEM_ID);

  char buf[32];
  ACE_OS::snprintf (buf, sizeof (buf), "Log%d", (int) id_);

  PortableServer::POAManager_var poa_manager =
    log_poa->the_POAManager ();

  this->iterator_poa_ =
    log_poa->create_POA(buf, poa_manager.in(), policies);
}

TAO_Hash_LogRecordStore::~TAO_Hash_LogRecordStore (void)
{
  this->iterator_poa_->destroy (1, 0);
}

int
TAO_Hash_LogRecordStore::open (void)
{
  return rec_map_.open ();
}

int
TAO_Hash_LogRecordStore::close (void)
{
  // Close the hash
  return rec_map_.close ();
}

CORBA::ULongLong
TAO_Hash_LogRecordStore::get_current_size (void)
{
  return this->current_size_;
}

CORBA::ULongLong
TAO_Hash_LogRecordStore::get_n_records (void)
{
  return this->num_records_;
}

CORBA::ULongLong
TAO_Hash_LogRecordStore::get_gauge (void)
{
  return this->gauge_;
}

void
TAO_Hash_LogRecordStore::reset_gauge (void)
{
  this->gauge_ = 0;
}

int
TAO_Hash_LogRecordStore::log (const DsLogAdmin::LogRecord &const_rec)
{
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
  rec.time = ORBSVCS_Time::to_Absolute_TimeT (ACE_OS::gettimeofday ());

  // First, bind the id to the LogRecord in the hash_map
  if (this->rec_map_.bind (rec.id, rec) != 0)
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
  this->gauge_ += record_size;

  return 0;
}

int
TAO_Hash_LogRecordStore::retrieve_i (DsLogAdmin::RecordId id,
                                     DsLogAdmin::LogRecord &rec)
{
  int retval = rec_map_.find (id, rec);
  return retval;
}

int
TAO_Hash_LogRecordStore::update_i (DsLogAdmin::LogRecord &rec)
{
  DsLogAdmin::LogRecord oldrec;

  if (rec_map_.unbind (rec.id, oldrec) != 0)
    {
      return -1;
    }

  --this->num_records_;
  this->current_size_ -= log_record_size(oldrec);

  if (rec_map_.bind (rec.id, rec) != 0)
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
  if (rec_map_.unbind (id, rec) != 0)
    {
      return -1;
    }

  --this->num_records_;
  this->current_size_ -= log_record_size(rec);

  return 0;
}

void
TAO_Hash_LogRecordStore::remove_i (LOG_RECORD_STORE_ITER iter)
{
  size_t size = log_record_size(iter->item ());

  rec_map_.unbind(&*iter);

  --this->num_records_;
  this->current_size_ -= size;
}

int
TAO_Hash_LogRecordStore::purge_old_records (void)
{
  CORBA::ULongLong num_records_to_purge = this->num_records_ * 5U / 100U;

  if (num_records_to_purge < 1)
    num_records_to_purge = 1;

  CORBA::ULong count = 0; // count of matches found.

  if (num_records_to_purge > 0 )
    {
      // Create iterators
      LOG_RECORD_STORE_ITER iter (rec_map_.begin ());
      LOG_RECORD_STORE_ITER iter_end (rec_map_.end ());

      for (CORBA::ULongLong i = 0;
           iter != iter_end && i < num_records_to_purge;
           ++i)
        {
          this->remove_i (iter++);
          ++count;
        }
    }

  return count;
}

void
TAO_Hash_LogRecordStore::set_record_attribute (DsLogAdmin::RecordId id,
                const DsLogAdmin::NVList
                &attr_list)
{
  // TODO: validate attributes here.

  DsLogAdmin::LogRecord rec;
  if (this->retrieve_i (id, rec) == -1)
    {
      throw DsLogAdmin::InvalidRecordId ();
    }

  rec.attr_list = attr_list;

  if (this->update_i (rec) == -1)
    {
      throw CORBA::PERSIST_STORE ();
    }
}

CORBA::ULong
TAO_Hash_LogRecordStore::set_records_attribute (
  const char *grammar,
  const char *constraint,
  const DsLogAdmin::NVList
  &attr_list)
{
  this->check_grammar (grammar);

  // TODO: validate attributes here.

  // Use an Interpreter to build an expression tree.
  TAO_Log_Constraint_Interpreter interpreter (constraint);

  // Create iterators
  LOG_RECORD_STORE_ITER iter (rec_map_.begin ());
  LOG_RECORD_STORE_ITER iter_end (rec_map_.end ());

  CORBA::ULong count = 0; // count of matches found.

  for ( ; iter != iter_end; ++iter)
    {
      // Use an evaluator.
      TAO_Log_Constraint_Visitor evaluator (iter->item ());

      // Does it match the constraint?
      if (interpreter.evaluate (evaluator) == 1)
        {
          set_record_attribute (iter->item ().id, attr_list);
          ++count;
        }
    }

  return count;
}

DsLogAdmin::NVList*
TAO_Hash_LogRecordStore::get_record_attribute (DsLogAdmin::RecordId id)
{
  DsLogAdmin::LogRecord rec;

  int retval = this->retrieve_i (id, rec);

  if (retval == -1)
    {
      throw DsLogAdmin::InvalidRecordId ();
    }

  DsLogAdmin::NVList* nvlist = 0;
  ACE_NEW_THROW_EX (nvlist,
                    DsLogAdmin::NVList (rec.attr_list),
                    CORBA::NO_MEMORY ());

  return nvlist;
}


int
TAO_Hash_LogRecordStore::flush (void)
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
                                  CORBA::ULong how_many)
{
  // Use an Interpreter to build an expression tree.
  TAO_Log_Constraint_Interpreter interpreter (constraint);

  // Sequentially iterate over all the records and pick the ones that
  // meet the constraints.

  // Allocate the list of <how_many> length.
  DsLogAdmin::RecordList* rec_list;
  ACE_NEW_THROW_EX (rec_list,
                    DsLogAdmin::RecordList (how_many),
                    CORBA::NO_MEMORY ());
  rec_list->length(how_many);

  // Create iterators
  LOG_RECORD_STORE_ITER iter (rec_map_.begin ());
  LOG_RECORD_STORE_ITER iter_end (rec_map_.end ());

  CORBA::ULong count = 0;       // count of matches found.

  for ( ; ((iter != iter_end) && (count < how_many)); ++iter)
    {
      // Use an evaluator.
      TAO_Log_Constraint_Visitor evaluator (iter->item ());

      // Does it match the constraint?
      if (interpreter.evaluate (evaluator) == 1)
        {
          if (TAO_debug_level > 0)
            {
#if defined (ACE_LACKS_LONGLONG_T)
              ACE_DEBUG ((LM_DEBUG,"Matched constraint! d = %d, Time = %d\n",
                ACE_U64_TO_U32 (iter->item ().id),
                ACE_U64_TO_U32 (iter->item ().time)));
#else
              ACE_DEBUG ((LM_DEBUG,"Matched constraint! d = %Q, Time = %Q\n",
                iter->item ().id,
                iter->item ().time));
#endif
            }

        (*rec_list)[count] = iter->item ();
        // copy the log record.
        ++count;
      }
    }

  rec_list->length (count);

  if (iter != iter_end)         // There are more records to process.
    {
      // Create an iterator to pass out.
      TAO_Hash_Iterator_i *iter_query = 0;
      ACE_NEW_THROW_EX (iter_query,
                        TAO_Hash_Iterator_i (this->iterator_poa_.in (),
                                             this->reactor_,
                                             this,
                                             iter,
                                             iter_end,
                                             count,
                                             constraint,
                                             this->max_rec_list_len_),
                        CORBA::NO_MEMORY ());

      // Transfer ownership to the POA.
      PortableServer::ServantBase_var safe_iter_query = iter_query;

      // Activate it.
      PortableServer::ObjectId_var oid =
        this->iterator_poa_->activate_object (iter_query);
      CORBA::Object_var obj =
        this->iterator_poa_->id_to_reference (oid.in ());

      // Narrow it
      iter_out = DsLogAdmin::Iterator::_narrow (obj.in ());
    }

  return rec_list;
}

DsLogAdmin::RecordList*
TAO_Hash_LogRecordStore::query (const char *grammar,
                                const char *constraint,
                                DsLogAdmin::Iterator_out iter_out)
{
  this->check_grammar (grammar);

  return this->query_i (constraint,
                        iter_out,
                        this->max_rec_list_len_);
}

DsLogAdmin::RecordList*
TAO_Hash_LogRecordStore::retrieve (DsLogAdmin::TimeT from_time,
                                   CORBA::Long how_many,
                                   DsLogAdmin::Iterator_out iter_out)
{
  // Decide between forward vs backward retrieval.
  char constraint[32];
  ACE_TCHAR uint64_formating[32];

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
                        how_many);
}

CORBA::ULong
TAO_Hash_LogRecordStore::match (const char* grammar,
                                const char *constraint)
{
  this->check_grammar (grammar);

  // Use an Interpreter to build an expression tree.
  TAO_Log_Constraint_Interpreter interpreter (constraint);

  // Create iterators
  LOG_RECORD_STORE_ITER iter (rec_map_.begin ());
  LOG_RECORD_STORE_ITER iter_end (rec_map_.end ());

  CORBA::ULong count = 0; // count of matches found.

  for ( ; iter != iter_end; ++iter)
    {
      // Use an evaluator.
      TAO_Log_Constraint_Visitor evaluator (iter->item ());

      // Does it match the constraint?
      if (interpreter.evaluate (evaluator) == 1)
        {
          ++count;
        }
    }

  return count;
}

CORBA::ULong
TAO_Hash_LogRecordStore::delete_records (const char *grammar,
                                         const char *constraint)
{
  this->check_grammar (grammar);

  // Use an Interpreter to build an expression tree.
  TAO_Log_Constraint_Interpreter interpreter (constraint);

  // Create iterators
  LOG_RECORD_STORE_ITER iter (rec_map_.begin ());
  LOG_RECORD_STORE_ITER iter_end (rec_map_.end ());

  CORBA::ULong count = 0; // count of matches found.

  while (iter != iter_end)
    {
      // Use an evaluator.
      TAO_Log_Constraint_Visitor evaluator (iter->item ());

      // Does it match the constraint?
      if (interpreter.evaluate (evaluator) == 1)
        {
          this->remove_i (iter++);
          ++count;
        }
      else
        {
          ++iter;
        }
    }

  return count;
}

CORBA::ULong
TAO_Hash_LogRecordStore::delete_records_by_id (const DsLogAdmin::RecordIdList &ids)
{
  CORBA::ULong count (0);

  for (CORBA::ULong i = 0; i < ids.length (); i++)
    {
      if (this->remove_i (ids [i]) == 0)
        {
          ++count;
        }
    }

  return count;
}

CORBA::ULong
TAO_Hash_LogRecordStore::remove_old_records (void)
{
  if (this->max_record_life_ == 0) {
    return 0;
  }

  TimeBase::TimeT purge_time (ORBSVCS_Time::to_Absolute_TimeT ((ACE_OS::gettimeofday () - ACE_Time_Value(this->max_record_life_))));

  // As there is no separate timestamp index, we must iterate through
  // the entire map.  We can't be tempted to think timestamps will be
  // monotonically increasing with record id.

  // Create iterators
  LOG_RECORD_STORE_ITER iter (rec_map_.begin ());
  LOG_RECORD_STORE_ITER iter_end (rec_map_.end ());

  CORBA::ULong count = 0; // count of matches found.

  while (iter != iter_end)
    {
      // Does it match the constraint?
      if (iter->item().time < purge_time)
        {
          this->remove_i (iter++);
          ++count;
        }
      else
        {
          ++iter;
        }
    }

  return count;
}

ACE_SYNCH_RW_MUTEX&
TAO_Hash_LogRecordStore::lock()
{
  return lock_;
}


void
TAO_Hash_LogRecordStore::check_grammar (const char* grammar)
{
  // Verify grammar
  if (ACE_OS::strcmp (grammar, "TCL") != 0 &&
      ACE_OS::strcmp (grammar, "ETCL") != 0 &&
      ACE_OS::strcmp (grammar, "EXTENDED_TCL") != 0)
    throw DsLogAdmin::InvalidGrammar ();
}


DsLogAdmin::AdministrativeState
TAO_Hash_LogRecordStore::get_administrative_state (void) const
{
  return this->admin_state_;
}

void
TAO_Hash_LogRecordStore::set_administrative_state (DsLogAdmin::AdministrativeState state)
{
  this->admin_state_ = state;
}


DsLogAdmin::CapacityAlarmThresholdList*
TAO_Hash_LogRecordStore::get_capacity_alarm_thresholds (void) const
{
  DsLogAdmin::CapacityAlarmThresholdList* ret_val;
  ACE_NEW_THROW_EX (ret_val,
                    DsLogAdmin::CapacityAlarmThresholdList (this->thresholds_),
                    CORBA::NO_MEMORY ());

  return ret_val;
}

void
TAO_Hash_LogRecordStore::set_capacity_alarm_thresholds (const DsLogAdmin::CapacityAlarmThresholdList& thresholds)
{
  this->thresholds_ = thresholds;
}



DsLogAdmin::ForwardingState
TAO_Hash_LogRecordStore::get_forwarding_state (void) const
{
  return this->forward_state_;
}

void
TAO_Hash_LogRecordStore::set_forwarding_state (DsLogAdmin::ForwardingState state)
{
  this->forward_state_ = state;
}

DsLogAdmin::TimeInterval
TAO_Hash_LogRecordStore::get_interval (void) const
{
  return this->interval_;
}

void
TAO_Hash_LogRecordStore::set_interval (const DsLogAdmin::TimeInterval &interval)
{
  this->interval_ = interval;
}


DsLogAdmin::LogFullActionType
TAO_Hash_LogRecordStore::get_log_full_action (void) const
{
  return this->log_full_action_;
}

void
TAO_Hash_LogRecordStore::set_log_full_action (DsLogAdmin::LogFullActionType action)
{
  this->log_full_action_ = action;
}

DsLogAdmin::QoSList *
TAO_Hash_LogRecordStore::get_log_qos (void) const
{
  DsLogAdmin::QoSList* ret_val = 0;
  ACE_NEW_THROW_EX (ret_val,
                    DsLogAdmin::QoSList (this->log_qos_),
                    CORBA::NO_MEMORY ());

  return ret_val;
}

void
TAO_Hash_LogRecordStore::set_log_qos (const DsLogAdmin::QoSList& qos)
{
  this->log_qos_ = qos;
}

CORBA::ULong
TAO_Hash_LogRecordStore::get_max_record_life (void) const
{
  return this->max_record_life_;
}

void
TAO_Hash_LogRecordStore::set_max_record_life (CORBA::ULong max_record_life)
{
  this->max_record_life_ = max_record_life;
}

CORBA::ULongLong
TAO_Hash_LogRecordStore::get_max_size (void) const
{
  return this->max_size_;
}

void
TAO_Hash_LogRecordStore::set_max_size (CORBA::ULongLong size)
{
  this->max_size_ = size;
}

DsLogAdmin::WeekMask*
TAO_Hash_LogRecordStore::get_week_mask (void)
{
  DsLogAdmin::WeekMask* ret_val = 0;
  ACE_NEW_THROW_EX (ret_val,
                    DsLogAdmin::WeekMask (this->weekmask_),
                    CORBA::NO_MEMORY ());

  return ret_val;
}

void
TAO_Hash_LogRecordStore::set_week_mask (const DsLogAdmin::WeekMask &masks)
{
  this->weekmask_ = masks;
}

TAO_END_VERSIONED_NAMESPACE_DECL
