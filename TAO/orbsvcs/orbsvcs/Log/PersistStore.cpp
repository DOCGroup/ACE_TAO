#include "PersistStore.h"

#include "orbsvcs/Time_Utilities.h"

ACE_RCSID (Log,
           PersistStore,
           "$Id$")


TAO_PersistStore::TAO_PersistStore (CORBA::ULongLong max_size,
                                    CORBA::ULong max_rec_list_len)
  : maxid_ (0),
    max_size_ (max_size),
    current_size_ (0),
    num_records_ (0),
    max_rec_list_len_ (max_rec_list_len)
{
  // No-Op.
}

TAO_PersistStore::~TAO_PersistStore (void)
{
  // No-Op.
}

int
TAO_PersistStore::open (const char * file_name)
{
  // Open the persistent log file for writing.
  this->write_persistent_file_ = ACE_OS::open (file_name,
                                O_RDWR | O_CREAT | O_TRUNC,
                                ACE_DEFAULT_FILE_PERMS);

  if (this->write_persistent_file_ == ACE_INVALID_HANDLE) {
    perror ("open:write");
    return -1;
  }

  // Open the persistnet log file for reading.
  this->read_persistent_file_ = ACE_OS::open (file_name,
                                          O_RDONLY);

  if (this->write_persistent_file_ == ACE_INVALID_HANDLE) {
    perror ("open:read");
    return -1;
  }

  return 0;
}

int
TAO_PersistStore::close (void)
{
  ACE_OS::close (this->write_persistent_file_);

  return ACE_OS::close (this->read_persistent_file_);
}

// TODO: make these inline ..
CORBA::ULongLong
TAO_PersistStore::get_max_size (void)
{
  return max_size_;
}

void
TAO_PersistStore::set_max_size (CORBA::ULongLong size)
{
  this->max_size_ = size;
}

CORBA::ULongLong
TAO_PersistStore::get_current_size (void)
{
  return this->current_size_;
}

CORBA::ULongLong
TAO_PersistStore::get_n_records (void)
{
  return this->num_records_;
}

// inline ...

int
TAO_PersistStore::log (DsLogAdmin::LogRecord &rec)
{
  CORBA::TypeCode_ptr tc;
  ACE_Message_Block *mb;
  struct PersistentData data;
  char *rd_ptr;

  // Check if we are allowed to write...
  if (max_size_ !=0 && current_size_ >= max_size_)
    return 1; // return code for log rec. full

  // Acquire the mutex so that no other client can write to the log
  write_lock_.acquire();

  // Initialize a couple of fields first...
  // ACE emulation of U Long Long (for platforms that don't have one)
  // does not define postfix operators
  rec.id = ++maxid_;

  ORBSVCS_Time::Time_Value_to_TimeT(rec.time,ACE_OS::gettimeofday());

  // Extract the typecode and message block from the record.
  tc = rec.info.type ();
  mb = rec.info._tao_get_cdr ();  // TAO extension
  data.byte_order = rec.info._tao_byte_order ();

  // Get a pointer to the actual data in the ACE_Message_Block.
  rd_ptr = mb->rd_ptr ();

  // Get the size of the actual data in the ACE_Message_Block.
  data.mb_size = mb->length ();

  // Store the first chunk of persistent data.
  // Get the kind of the CORBA::Any stored in the record.
  data.id = rec.id;
  data.time = rec.time;
  data.kind = tc->kind ();

  // Write the first block of information to the file.
  ACE_OS::write (this->write_persistent_file_, (void*)&data, sizeof (PersistentData));
  this->current_size_ += sizeof (PersistentData);

  // Write the typecode to the file.
  ACE_OS::write (this->write_persistent_file_, (void*) tc, sizeof (CORBA::TypeCode));
  this->current_size_ += sizeof (CORBA::TypeCode);

  // Write the actual message block data to the file.
  ACE_OS::write (this->write_persistent_file_, (void*) rd_ptr, data.mb_size);
  this->current_size_ += data.mb_size;

  // Unlock the mutex
  write_lock_.release();

  // Increment the number of records in the log
  ++(this->num_records_);

  // Debug print: # of records and current size in bytes
//  cout << "Log now has " << this->num_records_ ;
//  cout << " records using " << this->current_size_ << " bytes" << endl;

  return 0;
}

int
TAO_PersistStore::retrieve (DsLogAdmin::RecordId id, DsLogAdmin::LogRecord &rec)
{
  CORBA::TypeCode_var tc;
  char *mb_data = NULL;
  int retval = -1;
  struct PersistentData data;

  // Move to the beginning of the file.
  ACE_OS::lseek (this->read_persistent_file_, 0, SEEK_SET);

  while (ACE_OS::read (this->write_persistent_file_,
                       (void*) &data,
                       sizeof (PersistentData)) > 0)
  {
    tc = new CORBA::TypeCode (data.kind);

    ACE_OS::read (this->write_persistent_file_,
                  (void*) tc.in (),
                  sizeof (CORBA::TypeCode));

    mb_data = new char[data.mb_size];
    ACE_OS::read (this->write_persistent_file_,
                  (void*) mb_data,
                  data.mb_size);

    // Check to see if this id matches.
    if (id == data.id)
    {
      // Create the message block.
      ACE_Message_Block mb2 (mb_data, data.mb_size);

      // Set the write pointer
      mb2.wr_ptr (data.mb_size);

      rec.id = id;
      rec.time = data.time;

      TAO::Unknown_IDL_Type *unk = 0;
      ACE_NEW_RETURN (unk,
                      TAO::Unknown_IDL_Type (tc.in (),
                                             &mb2,
                                             data.byte_order),
                      -1);
      rec.info.replace (unk);

      retval = 1;
      break;
    }
  }

  return retval;
}

unsigned short
TAO_PersistStore::get_percentage_full (void)
{
  // If the log size is unlimited, return 0.
  if (this->max_size_ == 0)
    return 0;

  return (100U * this->current_size_ / this->max_size_);
}
