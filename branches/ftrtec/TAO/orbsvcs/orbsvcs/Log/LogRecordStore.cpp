#include "orbsvcs/Log/LogRecordStore.h"

#include "orbsvcs/Time_Utilities.h"
#include "orbsvcs/Log/Log_Constraint_Interpreter.h"
#include "orbsvcs/Log/Log_Constraint_Visitors.h"

ACE_RCSID (Log,
           LogRecordStore,
           "$Id$")


TAO_LogRecordStore::TAO_LogRecordStore (CORBA::ULongLong max_size,
                                        DsLogAdmin::LogId logid,
                                        CORBA::ULong max_rec_list_len)
  : maxid_ (0),
    max_size_ (max_size),
    logid_ (logid),
    current_size_ (0),
    num_records_ (0),
    max_rec_list_len_ (max_rec_list_len)
{
  //No-Op.
}

TAO_LogRecordStore::~TAO_LogRecordStore (void)
{
  // No-Op.
}

int
TAO_LogRecordStore::open (void)
{
  return rec_hash_.open ();
}

int
TAO_LogRecordStore::close (void)
{
  // Close the hash
  return rec_hash_.close ();
}

// TODO: make these inline ..
CORBA::ULongLong
TAO_LogRecordStore::get_max_size (void)
{
  return max_size_;
}

void
TAO_LogRecordStore::set_max_size (CORBA::ULongLong size)
{
  this->max_size_ = size;
}

CORBA::ULongLong
TAO_LogRecordStore::get_current_size (void)
{
  return this->current_size_;
}

CORBA::ULongLong
TAO_LogRecordStore::get_n_records (void)
{
  return this->num_records_;
}

// inline ...

int
TAO_LogRecordStore::log (DsLogAdmin::LogRecord &rec)
{
  // Check if we are allowed to write...

  if (max_size_ !=0 && (current_size_ + sizeof (rec)) >= max_size_)
    return 1; // return code for log rec. full

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
                         "LogRecordStore (%P|%t):Failed to bind %Q in the hash map\n",
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
  ++(this->num_records_);
  this->current_size_ =
    this->current_size_ + sizeof (rec);

  return 0;
}

int
TAO_LogRecordStore::retrieve (DsLogAdmin::RecordId id, DsLogAdmin::LogRecord &rec)
{
  int retval = rec_hash_.find (id, rec);
  return retval;
}

int
TAO_LogRecordStore::update (DsLogAdmin::LogRecord &rec)
{
  if (rec_hash_.unbind (rec.id, rec) != 0)
    {
      return -1;
    }

  return rec_hash_.bind (rec.id, rec);
}

int
TAO_LogRecordStore::remove (DsLogAdmin::RecordId id)
{
  DsLogAdmin::LogRecord rec;
  if (rec_hash_.unbind (id, rec) != 0)
    {
      return -1;
    }

  --(this->num_records_);
  this->current_size_ =
    this->current_size_ - sizeof (rec);
  // TODO: return ids to a reuse list.

  return 0;
}

int
TAO_LogRecordStore::purge_old_records (void)
{
  CORBA::ULongLong num_records_to_purge = this->num_records_ * 5U / 100U;

  if (num_records_to_purge < 1)
    num_records_to_purge = 1;

  LOG_RECORD_STORE_ITER iter (rec_hash_);
  LOG_RECORD_HASH_MAP_ENTRY *hash_entry;
  CORBA::ULong count = 0; // count of matches found.

  if (num_records_to_purge > 0 )
    {
      for (CORBA::ULongLong i = 0; i < num_records_to_purge; ++i)
        {
          if (iter.next (hash_entry) == -1 || iter.advance () == -1)
            break;

          if (this->remove (hash_entry->int_id_.id) == 0)
            count++;
        }
    }

  return count;
}



TAO_LogRecordStore::LOG_RECORD_STORE &
TAO_LogRecordStore::get_storage (void)
{
  return rec_hash_;
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
