// $Id$

#include "orbsvcs/Log/LogRecordStore.h"

LogRecordStore::LogRecordStore (CORBA::ULongLong max_size)
  : maxid_ (0),
    max_size_ (max_size),
    current_size_ (0),
    num_records_ (0)
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
  // Check we are allowed to write...
  if (max_size_ !=0 && current_size_ >= max_size_)
    return 1; // return code for log rec. full

  // Initialize a couple of fields first...
  rec.id = maxid_++;
  // TODO: Reuse ids by keeping a list.

  ACE_Time_Value tv (ACE_OS::gettimeofday ());
  rec.time =
    ACE_static_cast (DsLogAdmin::TimeT, tv.sec ());

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
