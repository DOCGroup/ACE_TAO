// $Id$

#include "orbsvcs/Log/LogRecordStore.h"
#include "orbsvcs/Log/Log_Constraint_Interpreter.h"
#include "orbsvcs/Log/Log_Constraint_Visitors.h"

LogRecordStore::LogRecordStore (CORBA::ULongLong max_size,
                                CORBA::ULong max_rec_list_len)
  : maxid_ (0),
    max_size_ (max_size),
    current_size_ (0),
    num_records_ (0),
    max_rec_list_len_ (max_rec_list_len)
{
  //No-Op.
}

LogRecordStore::~LogRecordStore (void)
{
  // No-Op.
}

int
LogRecordStore::open (void)
{
  return rec_hash_.open ();
}

int
LogRecordStore::close (void)
{
  // Close the hash
  return rec_hash_.close ();
}

// TODO: make these inline ..
CORBA::ULongLong
LogRecordStore::get_max_size (void)
{
  return max_size_;
}

void
LogRecordStore::set_max_size (CORBA::ULongLong size)
{
  this->max_size_ = size;
}

CORBA::ULongLong
LogRecordStore::get_current_size (void)
{
  return this->current_size_;
}

CORBA::ULongLong
LogRecordStore::get_n_records (void)
{
  return this->num_records_;
}

// inline ...

int
LogRecordStore::log (DsLogAdmin::LogRecord &rec)
{
  // Check if we are allowed to write...
  if (max_size_ !=0 && current_size_ >= max_size_)
    return 1; // return code for log rec. full

  // Initialize a couple of fields first...
  rec.id = maxid_++;
  // TODO: Reuse ids by keeping a list.

  ACE_Time_Value tv (ACE_OS::gettimeofday ());
  rec.time =
    ACE_static_cast (DsLogAdmin::TimeT, tv.usec ());

  // First, bind the id to the LogRecord in the hash_map
  if (this->rec_hash_.bind (rec.id, rec) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "LogRecordStore (%P|%t):Failed to bind %d in the hash map\n",
                         rec.id),
                        -1);
    }

  // Increment the number of records in the log
  this->num_records_++;
  this->current_size_ =
    this->current_size_ + sizeof (rec);

  return 0;
}

int
LogRecordStore::retrieve (DsLogAdmin::RecordId id, DsLogAdmin::LogRecord &rec)
{
  int retval = rec_hash_.find (id, rec);
  return retval;
}

int
LogRecordStore::update (DsLogAdmin::LogRecord &rec)
{
  if (rec_hash_.unbind (rec.id, rec) != 0)
    {
      return -1;
    }

  return rec_hash_.bind (rec.id, rec);
}

int
LogRecordStore::remove (DsLogAdmin::RecordId id)
{
  DsLogAdmin::LogRecord rec;
  if (rec_hash_.unbind (id, rec) != 0)
    {
      return -1;
    }

  this->num_records_--;
  this->current_size_ =
    this->current_size_ - sizeof (rec);
  // TODO: return ids to a reuse list.
  return 0;
}

int
LogRecordStore::purge_old_records (void)
{
  // TBD:
  // Delete 5% of the old records.
  return -1;
}

DsLogAdmin::RecordList_ptr
LogRecordStore::query (const char *constraint,
                       DsLogAdmin::Iterator_out& iter_out,
                       CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   DsLogAdmin::InvalidConstraint))
{
  // Use an Interpreter to build an expression tree.
  TAO_Log_Constraint_Interpreter interpreter (constraint,
                                              ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  // Sequentially iterate over all the records and pick the ones that
  // meet the constraints.

  // Create an iterator
  LOG_RECORD_HASH_MAP_ITER iter (this->rec_hash_);

  CORBA::ULong len = rec_hash_.current_size ();
  // How many entries?

  // Iterate over and populate the list.
  LOG_RECORD_HASH_MAP_ENTRY *hash_entry;

  DsLogAdmin::RecordList_ptr rec_list;
  // Figure out the length of the list.

  // Allocate the list of <max_rec_list_len_> length.
  ACE_NEW_THROW_EX (rec_list,
                    DsLogAdmin::RecordList (this->max_rec_list_len_),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  CORBA::ULong count = 0;

  for (CORBA::ULong i = 0;
       i < len && count < this->max_rec_list_len_;
       ++i)
    {
      iter.next (hash_entry);
      iter.advance ();

      ACE_DEBUG ((LM_DEBUG,"Query::id = %ul, Time = %ul\n",
                  hash_entry->int_id_.id,
                  hash_entry->int_id_.time));

      // Use an evaluator.
      TAO_Log_Constraint_Evaluator evaluator (hash_entry->int_id_);

      // Does it match the constraint?
      if (interpreter.evaluate (evaluator) == 1)
      {
        ACE_DEBUG ((LM_DEBUG,"Matched constraint! d = %ul, Time = %ul\n",
                    hash_entry->int_id_.id,
                    hash_entry->int_id_.time));

        (*rec_list)[count] = hash_entry->int_id_;
        // copy the log record.
        count++;
      }
    }

  rec_list->length (count);

  if (i < len) // There are more records to process.
    {
      // Create an iterator to pass out.
    }

  return rec_list;
}

LOG_RECORD_STORE&
LogRecordStore::get_storage (void)
{
  return rec_hash_;
}

/*
const LOG_RECORD_STORE_ITER&
LogRecordStore::get_record_iter_start (void)
{
  return rec_hash_.start ();
}
*/

/*
const LOG_RECORD_STORE_ITER&
LogRecordStore::get_record_iter_end (void)
{
  return rec_hash_.end ();
}
*/
