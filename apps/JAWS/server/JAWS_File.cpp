// $Id$

// ============================================================================
//
// = LIBRARY
//    apps
// 
// = FILENAME
//    JAWS_File.cpp
//
// = AUTHOR
//    James Hu
// 
// ============================================================================

#include "JAWS_File.h"

static const int R_MASK = S_IRUSR|S_IRGRP|S_IROTH;
static const int W_MASK = S_IRUSR|S_IRGRP|S_IROTH|S_IWUSR|S_IWGRP|S_IWOTH;

#if defined (ACE_WIN32)
static const int READ_FLAGS = (FILE_FLAG_SEQUENTIAL_SCAN |
                               FILE_FLAG_OVERLAPPED |
                               O_RDONLY);
static const int WRITE_FLAGS = (FILE_FLAG_SEQUENTIAL_SCAN |
                                FILE_FLAG_OVERLAPPED |
                                O_RDWR |
                                O_CREAT |
                                O_TRUNC);
#else
static const int READ_FLAGS = O_RDONLY;
static const int WRITE_FLAGS = O_RDWR | O_CREAT | O_TRUNC;
#endif /* ACE_WIN32 */

// static data members
JAWS_Virtual_Filesystem * JAWS_Virtual_Filesystem::cvf_ = 0;
ACE_SYNCH_MUTEX JAWS_Virtual_Filesystem::lock_;

// this is how you make data opaque in C++
// I'd like to do this with JAWS_File too, but Doug would pro'ly kill me
class JAWS_Virtual_Filesystem_Singleton
{
public:
  JAWS_Virtual_Filesystem_Singleton (void)
  { this->singleton_ = JAWS_Virtual_Filesystem::instance (); }

  ~JAWS_Virtual_Filesystem_Singleton (void)
  { delete this->singleton_; }

private:
  JAWS_Virtual_Filesystem * singleton_;
};

static JAWS_Virtual_Filesystem_Singleton cvf_singleton;

void
JAWS_File_Handle::init (void)
{
  this->file_ = 0;
  this->handle_ = ACE_INVALID_HANDLE;
}

JAWS_File_Handle::JAWS_File_Handle (void)
{
  this->init ();
}

JAWS_File_Handle::JAWS_File_Handle (const char * filename)
{
  this->init ();
  // fetch the file from the Virtual_Filesystem
  // let it do the work of cache coherency
  this->file_ = JAWS_Virtual_Filesystem::instance ()->fetch (filename);
  this->file_->acquire ();
}

JAWS_File_Handle::JAWS_File_Handle (const char * filename, int size)
{
  this->init ();
  // since this is being opened for a write,
  // simply create a new JAWS_File now,
  // and let the destructor add it into CVF later
  this->file_ = new JAWS_File (filename, size);
  this->file_->acquire ();
}

JAWS_File_Handle::~JAWS_File_Handle (void)
{
  if (this->handle_ != ACE_INVALID_HANDLE)
    {
      // this was dup ()'d
      ACE_OS::close (this->handle_);
    }

  if (this->file_ != 0)
    {
      switch (this->file_->action ())
        {
        case JAWS_File::WRITING:
          this->file_->release ();
          // assert (this->file_->reference_count () == 0);
          // put it into the CVF
          JAWS_Virtual_Filesystem::instance ()->replace (this->file_);
          break;

        case JAWS_File::WAITING:
          // last one using a stale file is resposible for deleting it
          if (this->file_->release () == 0)
            delete this->file_;
          break;

        default:
          this->file_->release ();
        }
    }
}

void *
JAWS_File_Handle::address (void) const
{
  return ((this->file_ == 0) ? 0 : this->file_->address ());
}

ACE_HANDLE
JAWS_File_Handle::handle (void) const
{
  if (this->handle_ == ACE_INVALID_HANDLE && this->file_ != 0)
    {
      JAWS_File_Handle *mutable_this = (JAWS_File_Handle *)this;
      mutable_this->handle_ = ACE_OS::dup (this->file_->handle ());
    }
  return this->handle_;
}

int
JAWS_File_Handle::error (void) const
{
  return ((this->file_ == 0) ? -1 : this->file_->error ());
}

size_t
JAWS_File_Handle::size (void) const
{
  return ((this->file_ == 0) ? -1 : this->file_->size ());
}


JAWS_Virtual_Filesystem *
JAWS_Virtual_Filesystem::instance (void)
{
  // Double check locking pattern
  if (JAWS_Virtual_Filesystem::cvf_ == 0)
    {
      ACE_Guard<ACE_SYNCH_MUTEX> m (JAWS_Virtual_Filesystem::lock_);
      if (JAWS_Virtual_Filesystem::cvf_ == 0)
        {
          JAWS_Virtual_Filesystem::cvf_ = new JAWS_Virtual_Filesystem;
        }
    }

  return JAWS_Virtual_Filesystem::cvf_;
}

JAWS_Virtual_Filesystem::JAWS_Virtual_Filesystem (void)
{
  this->size_ = DEFAULT_VIRTUAL_FILESYSTEM_TABLE_SIZE;
  for (int i = 0; i < this->size_; i++)
    this->table_[i] = 0;
}

JAWS_Virtual_Filesystem::~JAWS_Virtual_Filesystem (void)
{
  for (int i = 0; i < this->size_; i++)
    {
      if (this->table_[i] != 0) delete this->table_[i];
    }
}

JAWS_File *
JAWS_Virtual_Filesystem::fetch (const char * filename)
{
  ACE_Guard<ACE_SYNCH_MUTEX> m (JAWS_Virtual_Filesystem::lock_);

  int i = this->fetch_i (filename);
  JAWS_File * jf;
  if (i == -1)
    {
      jf = this->insert_i (new JAWS_File (filename));
    }
  else if (this->table_[i]->update ())
    {
      this->remove_i (i);
      jf = this->table_[i] = new JAWS_File (filename);
    }
  else jf = this->table_[i];

  return jf;
}

int
JAWS_Virtual_Filesystem::fetch_i (const char * filename)
{
  int i;
  int notfound = 1;
  for (i = 0; i < DEFAULT_VIRTUAL_FILESYSTEM_TABLE_SIZE; i++)
    {
      if (this->table_[i] == 0) continue;
      notfound = ACE_OS::strcmp (filename, this->table_[i]->filename ());
      if (!notfound) break;
    }
  return (notfound ? -1 : i);
}

JAWS_File *
JAWS_Virtual_Filesystem::remove (const char * filename)
{
  ACE_Guard<ACE_SYNCH_MUTEX> m (JAWS_Virtual_Filesystem::lock_);

  int i = this->fetch_i (filename);
  return ((i == -1) ? 0 : this->remove_i (i));
}

JAWS_File *
JAWS_Virtual_Filesystem::remove_i (int index)
{
  JAWS_File * jf = 0;
  if (index != -1)
    {
      jf = this->table_[index];
      this->table_[index] = 0;
      if (jf->action () == JAWS_File::IDLE)
        {
          delete jf;
          jf = 0;
        }
      else
        jf->action (JAWS_File::WAITING);
    }
  return jf;
}

JAWS_File *
JAWS_Virtual_Filesystem::insert (JAWS_File * new_file)
{
  // Assume the filename associated with this file is unique.
  ACE_Guard<ACE_SYNCH_MUTEX> m (JAWS_Virtual_Filesystem::lock_);

  return this->insert_i (new_file);
}

JAWS_File *
JAWS_Virtual_Filesystem::insert_i (JAWS_File * new_file)
{
  int i;
  for (i = 0; i < DEFAULT_VIRTUAL_FILESYSTEM_TABLE_SIZE; i++)
    {
      if (this->table_[i] != 0) continue;
      this->table_[i] = new_file;
      break;
    }
  return new_file;
}

JAWS_File *
JAWS_Virtual_Filesystem::replace (JAWS_File * new_file)
{
  ACE_Guard<ACE_SYNCH_MUTEX> m (JAWS_Virtual_Filesystem::lock_);

  int i = this->fetch_i (new_file->filename ());
  if (i == -1) this->insert_i (new_file);
  else
    {
      this->remove_i (i);
      this->table_[i] = new_file;
    }

  return new_file;
}



void
JAWS_File::init (void)
{
  this->filename_[0] = '\0';
  this->handle_ = ACE_INVALID_HANDLE;
  this->error_ = OKIE_DOKIE;
  this->tempname_ = 0;
  this->size_ = 0;
  this->reference_count_ = 0 ;

  ACE_OS::memset (&(this->stat_), 0, sizeof (this->stat_));
}

JAWS_File::JAWS_File (void)
{
  this->init ();
}

JAWS_File::JAWS_File (const char * filename)
{
  this->init ();

  ACE_OS::strcpy (this->filename_, filename);
  this->action (JAWS_File::IDLE);

  if (ACE_OS::access (this->filename_, R_OK) == -1)
    {
      this->error (JAWS_File::ACCESS_FAILED);
      return;
    }
  if (ACE_OS::stat (this->filename_, &this->stat_) == -1)
    {
      this->error (JAWS_File::STAT_FAILED);
      return;
    }
  this->size_ = this->stat_.st_size;

  this->tempname_ = ACE_OS::tempnam (".", "zJAWS");

  ACE_HANDLE original = ACE_OS::open (this->filename_, READ_FLAGS, R_MASK);
  if (original == ACE_INVALID_HANDLE)
    {
      this->error (JAWS_File::OPEN_FAILED);
      return;
    }
  ACE_HANDLE copy = ACE_OS::open (this->tempname_, WRITE_FLAGS, W_MASK);
  if (copy == ACE_INVALID_HANDLE)
    {
      this->error (JAWS_File::COPY_FAILED);
      ACE_OS::close (original);
      return;
    }

  char buf[4096];
  int count = ACE_OS::read (original, buf, sizeof (buf));
  while (count > 0)
    {
      ACE_OS::write (copy, buf, count);
      count = ACE_OS::read (original, buf, sizeof (buf));
    }
  ACE_OS::close (original);
  ACE_OS::close (copy);
}

JAWS_File::JAWS_File (const char * filename, int size)
{
  this->init ();

  this->size_ = size;
  ACE_OS::strcpy (this->filename_, filename);
  this->action (JAWS_File::WRITING);
  
  if (ACE_OS::access (this->filename_, R_OK|W_OK) == -1)
    {
      if (ACE_OS::access (this->filename_, F_OK) != -1)
        {
          this->error (JAWS_File::ACCESS_FAILED);
          return;
        }
    }

  this->tempname_ = ACE_OS::tempnam (".", "zJAWS");
}

JAWS_File::~JAWS_File (void)
{
  if (this->tempname_)
    {
      if (this->error_ == OKIE_DOKIE)
        ACE_OS::unlink (this->tempname_);
      ACE_OS::free (this->tempname_);
    }
}

int
JAWS_File::acquire (void)
{
  ACE_Guard<ACE_SYNCH_MUTEX> m (this->lock_);
  if (this->reference_count_++ == 0)
    {
      if (this->error_ == OKIE_DOKIE)
        // load file into memory
        switch (this->action ())
          {
          case JAWS_File::IDLE:
            this->handle_ = ACE_OS::open (this->tempname_, READ_FLAGS, R_MASK);
            if (this->handle_ == ACE_INVALID_HANDLE)
              {
                this->error (JAWS_File::OPEN_FAILED,
                             "JAWS_File::acquire: open");
              }
            else if (this->mmap_.map (this->handle_, -1,
                                      PROT_READ, MAP_PRIVATE) != 0)
              {
                this->error (JAWS_File::MEMMAP_FAILED,
                             "JAWS_File::acquire: map");
                ACE_OS::close (this->handle_);
                this->handle_ = ACE_INVALID_HANDLE;
              }
            else this->action (JAWS_File::READING);
            break;

          case JAWS_File::WRITING:
            this->handle_ = ACE_OS::open (this->tempname_, WRITE_FLAGS, W_MASK);
            if (this->handle_ == ACE_INVALID_HANDLE)
              {
                this->error (JAWS_File::OPEN_FAILED,
                             "JAWS_File::acquire: open");
              }
            else if (ACE_OS::lseek (this->handle_, this->size_ - 1, SEEK_SET)
                     == -1)
              {
                this->error (JAWS_File::OPEN_FAILED,
                             "JAWS_File::acquire: lseek");
                ACE_DEBUG ((LM_DEBUG, "hey--> %d, %u, %d\n", this->handle_, this->size_, SEEK_SET));
                ACE_OS::close (this->handle_);
              }
            else if (ACE_OS::write (this->handle_, "", 1) != 1)
              {
                this->error (JAWS_File::WRITE_FAILED,
                             "JAWS_File::acquire: write");
                ACE_OS::close (this->handle_);
              }
            else if (this->mmap_.map (this->handle_, this->size_,
                                      PROT_RDWR, MAP_SHARED) != 0)
              {
                this->error (JAWS_File::MEMMAP_FAILED,
                             "JAWS_File::acquire: map");
                ACE_OS::close (this->handle_);
              }
            break;

          case JAWS_File::READING:
          default:
            // Nothing to do in these cases.
            break;
          }
    }

  return this->reference_count_;
}

int
JAWS_File::release (void)
{
  ACE_Guard<ACE_SYNCH_MUTEX> m (this->lock_);
  if (--this->reference_count_ == 0)
    {
      if (this->error_ == OKIE_DOKIE)
        // free file from memory if reference count is zero
        switch (this->action ())
          {
          case JAWS_File::READING:
            this->mmap_.unmap ();
            ACE_OS::close (this->handle_);
            this->handle_ = ACE_INVALID_HANDLE;
            this->action (JAWS_File::IDLE);
            break;

          case JAWS_File::WRITING:
            do
              {
                ACE_HANDLE original = ACE_OS::open (this->filename_,
                                                    WRITE_FLAGS, W_MASK);
                if (original == ACE_INVALID_HANDLE)
                  {
                    this->error_ = JAWS_File::OPEN_FAILED;
                  }
                else if (ACE_OS::write (original, this->mmap_.addr (),
                                        this->size_)
                         == -1)
                  {
                    this->error_ = JAWS_File::WRITE_FAILED;
                    ACE_OS::close (original);
                    ACE_OS::unlink (this->filename_);
                  }
                else if (ACE_OS::stat (this->filename_, &this->stat_) == -1)
                  {
                    this->error_ = JAWS_File::STAT_FAILED;
                  }
                this->mmap_.unmap ();
                ACE_OS::close (this->handle_);
                this->handle_ = ACE_INVALID_HANDLE;
                this->action (JAWS_File::IDLE);
              }
            while (0);
            break;

          case JAWS_File::IDLE:
          default:
            break;
          }
    }

  return this->reference_count_;
}

int
JAWS_File::action (void) const
{
  return this->action_;
}

int
JAWS_File::action (int action_value)
{
  return (this->action_ = action_value);
}

int
JAWS_File::error (void) const
{
  return this->error_;
}

int
JAWS_File::error (int error_value, const char * s)
{
  ACE_ERROR ((LM_ERROR, "%p.\n", s));
  return (this->error_ = error_value);
}

const char *
JAWS_File::filename (void) const
{
  return this->filename_;
}

size_t
JAWS_File::size (void) const
{
  return this->size_;
}

ACE_HANDLE
JAWS_File::handle (void) const
{
  return this->handle_;
}

void *
JAWS_File::address (void) const
{
  return this->mmap_.addr ();
}

int
JAWS_File::update (void) const
{
  int result;
  struct stat statbuf;

  if (ACE_OS::stat (this->filename_, &statbuf) == -1)
    {
      result = 1;
    }
  else
    {
      // non-portable code may follow
      result = (ACE_OS::difftime (this->stat_.st_mtime, statbuf.st_mtime) < 0);
    }

  return result;
}
