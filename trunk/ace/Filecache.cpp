// $Id$

#define ACE_BUILD_DLL

#include "ace/Filecache.h"

static const int R_MASK = S_IRUSR|S_IRGRP|S_IROTH;
static const int W_MASK = S_IRUSR|S_IRGRP|S_IROTH|S_IWUSR|S_IWGRP|S_IWOTH;

#if defined (ACE_WIN32)
static const int READ_FLAGS = (FILE_FLAG_SEQUENTIAL_SCAN |
                               FILE_FLAG_OVERLAPPED |
                               O_RDONLY);
static const int RCOPY_FLAGS = (FILE_FLAG_SEQUENTIAL_SCAN |
                                O_RDONLY);
static const int WRITE_FLAGS = (FILE_FLAG_SEQUENTIAL_SCAN |
                                FILE_FLAG_OVERLAPPED |
                                O_RDWR | O_CREAT | O_TRUNC);
static const int WCOPY_FLAGS = (FILE_FLAG_SEQUENTIAL_SCAN |
                                O_RDWR | O_CREAT | O_TRUNC);
#else
static const int READ_FLAGS = O_RDONLY;
static const int RCOPY_FLAGS = O_RDONLY;
static const int WRITE_FLAGS = O_RDWR | O_CREAT | O_TRUNC;
static const int WCOPY_FLAGS = O_RDWR | O_CREAT | O_TRUNC;
#endif /* ACE_WIN32 */

// static data members
ACE_Filecache *ACE_Filecache::cvf_ = 0;
ACE_SYNCH_RW_MUTEX ACE_Filecache::lock_;

// This is how you make data opaque in C++.  I'd like to do this with
// ACE_Filecache_Object too, but Doug would pro'ly kill me.
class ACE_Filecache_Singleton
{
public:
  ACE_Filecache_Singleton (void)
  { 
    this->singleton_ = ACE_Filecache::instance ();
  }

  ~ACE_Filecache_Singleton (void)
  { 
    delete this->singleton_; 
  }

private:
  ACE_Filecache * singleton_;
};

// This singleton is necessary for now in order to delete the
// temporary files that are created for the cache.  This won't be
// needed in the future when we remove reliance on copying files.
static ACE_Filecache_Singleton cvf_singleton;

class ACE_Filecache_Object
  // = TITLE
  //     Abstraction over a real file.  This is what the Virtual
  //     Filesystem contains.  This class is not intended for general
  //     consumption.  Please consult a physician before attempting to
  //     use this class.
{
public:
  ACE_Filecache_Object (const char *filename);
  // Creates a file for reading.

  ACE_Filecache_Object (const char *filename,
	     int size);
  // Creates a file for writing.

  ~ACE_Filecache_Object (void);
  // Only if reference count is zero should this be called.

  int acquire (void);
  // Increment the reference_count_.

  int release (void);
  // Decrement the reference_count_.

  // = action_ accessors
  int action (void) const;
  int action (int action_value);

  // = error_ accessors
  int error (void) const;
  int error (int error_value,
	     const char *s = "ACE_Filecache_Object");

  const char *filename (void) const;
  // filename_ accessor

  ACE_HANDLE handle (void) const;
  // handle_ accessor.

  void *address (void) const;
  // Base memory address for memory mapped file.

  size_t size (void) const;
  // size_ accessor.

  int update (void) const;
  // True if file on disk is newer than cached file.

protected:
  ACE_Filecache_Object (void);
  // Prevent from being called.

  void init (void);
  // Common initialization code,

private:
  int error_i (int error_value,
	       const char *s = "ACE_Filecache_Object");
  // Internal error logging method, no locking.

public:

  enum 
  { 
    READING = 1,
    WRITING = 2
  };

  enum 
  {
    SUCCESS = 0,
    ACCESS_FAILED,
    OPEN_FAILED,
    COPY_FAILED,
    STAT_FAILED,
    MEMMAP_FAILED,
    WRITE_FAILED 
  };

private:
  char *tempname_;
  char filename_[MAXPATHLEN + 1];
  // The temporary file name and the real file name.  The real file is
  // copied into the temporary file for safety reasons.

  ACE_Mem_Map mmap_;
  ACE_HANDLE handle_;
  // mmap_ holds the memory mapped version of the temporary file.
  // handle_ is the descriptor to the temporary file.

  struct stat stat_;
  size_t size_;
  // Used to compare against the real file to test if an update is needed.

  int action_;
  int error_;
  // Status indicators.

  int reference_count_;
  ACE_SYNCH_RW_MUTEX lock_;
  // reference_count_ counts how many users of the file there are.
  // lock_ provides a synchronization mechanism for critical sections
  // of code.
};

void
ACE_Filecache_Handle::init (void)
{
  this->file_ = 0;
  this->handle_ = ACE_INVALID_HANDLE;
}

ACE_Filecache_Handle::ACE_Filecache_Handle (void)
{
  this->init ();
}

ACE_Filecache_Handle::ACE_Filecache_Handle (const char * filename)
{
  this->init ();

  // fetch the file from the Virtual_Filesystem
  // let the Virtual_Filesystem do the work of cache coherency

  this->file_ = ACE_Filecache::instance ()->fetch (filename);

  this->file_->acquire ();
}

ACE_Filecache_Handle::ACE_Filecache_Handle (const char * filename, int size)
{
  this->init ();
  // since this is being opened for a write,
  // simply create a new ACE_Filecache_Object now,
  // and let the destructor add it into CVF later
  this->file_ = new ACE_Filecache_Object (filename, size);
  this->file_->acquire ();
}

ACE_Filecache_Handle::~ACE_Filecache_Handle (void)
{
  if (this->handle_ != ACE_INVALID_HANDLE)
    // this was dup ()'d
    ACE_OS::close (this->handle_);

  if (this->file_ != 0)
    switch (this->file_->action ())
      {
      case ACE_Filecache_Object::WRITING:
	this->file_->release ();
	// assert (this->file_->reference_count () == 0);
	// put it into the CVF
	ACE_Filecache::instance ()->replace (this->file_);
	break;

      default:
	// last one using a stale file is resposible for deleting it
	if (this->file_->release () == 0)
	  delete this->file_;
	break;
      }
}

void *
ACE_Filecache_Handle::address (void) const
{
  return this->file_ == 0 ? 0 : this->file_->address ();
}

ACE_HANDLE
ACE_Filecache_Handle::handle (void) const
{
  if (this->handle_ == ACE_INVALID_HANDLE && this->file_ != 0)
    {
      ACE_Filecache_Handle *mutable_this = 
	(ACE_Filecache_Handle *) this;
      mutable_this->handle_ = ACE_OS::dup (this->file_->handle ());
    }
  return this->handle_;
}

int
ACE_Filecache_Handle::error (void) const
{
  if (this->file_ == 0) 
    return -1;
  else
    return this->file_->error ();
}

size_t
ACE_Filecache_Handle::size (void) const
{
  if (this->file_ == 0)
    return (size_t) -1;
  else
    return this->file_->size ();
}


ACE_Filecache *
ACE_Filecache::instance (void)
{
  // Double check locking pattern.
  if (ACE_Filecache::cvf_ == 0)
    {
      ACE_Guard<ACE_SYNCH_RW_MUTEX> m (ACE_Filecache::lock_);

      if (ACE_Filecache::cvf_ == 0)
	ACE_Filecache::cvf_ = new ACE_Filecache;
    }

  return ACE_Filecache::cvf_;
}

ACE_Filecache::ACE_Filecache (void)
{
  this->size_ = DEFAULT_VIRTUAL_FILESYSTEM_TABLE_SIZE;

  for (int i = 0; i < this->size_; i++)
    this->table_[i] = 0;
}

ACE_Filecache::~ACE_Filecache (void)
{
  for (int i = 0; i < this->size_; i++)
    if (this->table_[i] != 0)
      delete this->table_[i];
}

ACE_Filecache_Object *
ACE_Filecache::find (const char * filename)
{
  int i;
  ACE_Filecache_Object *handle = 0;

  {
    ACE_Read_Guard<ACE_SYNCH_RW_MUTEX> m (ACE_Filecache::lock_);

    i = this->fetch_i (filename);

    if (i != -1 && ! this->table_[i]->update ())
      handle = this->table_[i];
  }

  // Considerably slower on misses, but should be faster on hits.
  // This is actually the double check locking pattern.

  if (handle == 0)
    {
      ACE_Write_Guard<ACE_SYNCH_RW_MUTEX> m (ACE_Filecache::lock_);

      i = this->fetch_i (filename);

      if (i == -1)
        {
          ACE_DEBUG ((LM_DEBUG, "   (%t) CVF: missed %s\n", filename));
        }
      else if (this->table_[i]->update ())
        {
          ACE_DEBUG ((LM_DEBUG, "   (%t) CVF: updating %s\n", filename)); 
          this->remove_i (i);
          handle = this->table_[i] = new ACE_Filecache_Object (filename);
          handle->acquire ();
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG, "   (%t) CVF: found %s\n", filename)); 
          handle = this->table_[i];
        }
    }

  return handle;
}

ACE_Filecache_Object *
ACE_Filecache::fetch (const char * filename)
{
  int i;
  ACE_Filecache_Object *handle = 0;

  {
    ACE_Read_Guard<ACE_SYNCH_RW_MUTEX> m (ACE_Filecache::lock_);

    i = this->fetch_i (filename);

    if (i != -1 && ! this->table_[i]->update ())
      handle = this->table_[i];
  }

  // Considerably slower on misses, but should be faster on hits.
  // This is actually the double check locking pattern.

  // Nearly equivalent to this->find (), except a miss causes the CVF
  // to insert the file into the cache.

  if (handle == 0)
    {
      ACE_Write_Guard<ACE_SYNCH_RW_MUTEX> m (ACE_Filecache::lock_);

      i = this->fetch_i (filename);

      if (i == -1)
        {
          ACE_DEBUG ((LM_DEBUG, "   (%t) CVF: missed %s\n", filename));
          handle = this->insert_i (new ACE_Filecache_Object (filename));
        }
      else if (this->table_[i]->update ())
        {
          ACE_DEBUG ((LM_DEBUG, "   (%t) CVF: updating %s\n", filename)); 
          this->remove_i (i);
          handle = this->table_[i] = new ACE_Filecache_Object (filename);
          handle->acquire ();
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG, "   (%t) CVF: found %s\n", filename)); 
          handle = this->table_[i];
        }
    }

  return handle;
}

int
ACE_Filecache::fetch_i (const char * filename)
{
  for (int i = 0;
       i < DEFAULT_VIRTUAL_FILESYSTEM_TABLE_SIZE; 
       i++)
    {
      if (this->table_[i] == 0) 
	continue;

      if (ACE_OS::strcmp (filename, this->table_[i]->filename ()) == 0)
	return i;
    }

  return -1;
}

ACE_Filecache_Object *
ACE_Filecache::remove (const char * filename)
{
  ACE_Write_Guard<ACE_SYNCH_RW_MUTEX> m (ACE_Filecache::lock_);

  int i = this->fetch_i (filename);

  return i == -1 ? 0 : this->remove_i (i);
}

ACE_Filecache_Object *
ACE_Filecache::remove_i (int index)
{
  ACE_Filecache_Object * handle = 0;

  if (index != -1)
    {
      handle = this->table_[index];
      this->table_[index] = 0;

      if (handle->release () == 0)
        {
          delete handle;
          handle = 0;
        }
    }

  return handle;
}

ACE_Filecache_Object *
ACE_Filecache::insert (ACE_Filecache_Object * new_file)
{
  // Assume the filename associated with this file is unique.
  ACE_Write_Guard<ACE_SYNCH_RW_MUTEX> m (ACE_Filecache::lock_);

  return this->insert_i (new_file);
}

ACE_Filecache_Object *
ACE_Filecache::insert_i (ACE_Filecache_Object * new_file)
{
  int i;
  int max = 0;
  size_t maxsize = 0;

  for (i = 0; i < DEFAULT_VIRTUAL_FILESYSTEM_TABLE_SIZE; i++)
    {
      if (this->table_[i] != 0)
        {
          if (this->table_[i]->size () > maxsize)
            maxsize = this->table_[max = i]->size ();
          continue;
        }
      this->table_[i] = new_file;
      break;
    }

  if (i == DEFAULT_VIRTUAL_FILESYSTEM_TABLE_SIZE)
    {
      // Forced to exercise a replacement policy here.  Let's play
      // nice, and remove the largest object from the cache.
      this->remove_i (max);
      this->table_[max] = new_file;
    }

  new_file->acquire ();
  return new_file;
}

ACE_Filecache_Object *
ACE_Filecache::replace (ACE_Filecache_Object *new_file)
{
  ACE_Write_Guard<ACE_SYNCH_RW_MUTEX> m (ACE_Filecache::lock_);

  int i = this->fetch_i (new_file->filename ());

  if (i == -1)
    this->insert_i (new_file);
  else
    {
      this->remove_i (i);
      this->table_[i] = new_file;
      new_file->acquire ();
    }

  return new_file;
}

void
ACE_Filecache_Object::init (void)
{
  this->filename_[0] = '\0';
  this->handle_ = ACE_INVALID_HANDLE;
  this->error_ = SUCCESS;
  this->tempname_ = 0;
  this->size_ = 0;
  this->reference_count_ = 0 ;

  ACE_OS::memset (&(this->stat_), 0, sizeof (this->stat_));
}

ACE_Filecache_Object::ACE_Filecache_Object (void)
{
  this->init ();
}

ACE_Filecache_Object::ACE_Filecache_Object (const char *filename)
{
  this->init ();

  // ASSERT strlen(filename) < sizeof (this->filename_)
  ACE_OS::strcpy (this->filename_, filename);
  this->action (ACE_Filecache_Object::READING);
  // place ourselves into the READING state

  // Can we access the file?
  if (ACE_OS::access (this->filename_, R_OK) == -1)
    {
      this->error (ACE_Filecache_Object::ACCESS_FAILED);
      return;
    }

  // Can we stat the file?
  if (ACE_OS::stat (this->filename_, &this->stat_) == -1)
    {
      this->error (ACE_Filecache_Object::STAT_FAILED);
      return;
    }

  this->size_ = this->stat_.st_size;

  // Create the temporary file, which will become our copy.
  this->tempname_ = ACE_OS::tempnam (".", "zJAWS");

  // Can we open the original file?
  ACE_HANDLE original = ACE_OS::open (this->filename_, RCOPY_FLAGS, R_MASK);

  if (original == ACE_INVALID_HANDLE)
    {
      this->error (ACE_Filecache_Object::OPEN_FAILED);
      return;
    }

  // Can we open the copy?
  ACE_HANDLE copy = ACE_OS::open (this->tempname_, WCOPY_FLAGS, W_MASK);
  if (copy == ACE_INVALID_HANDLE)
    {
      this->error (ACE_Filecache_Object::COPY_FAILED);
      ACE_OS::close (original);
      return;
    }

  // Copy the original into the copy.
  ACE_Mem_Map original_map (original);
  ACE_Mem_Map copy_map (copy, this->size_, PROT_WRITE, MAP_SHARED);

  void *src = original_map.addr ();
  void *dst = copy_map.addr ();
  
  if (src == MAP_FAILED || dst == MAP_FAILED)
    {
      this->error (ACE_Filecache_Object::MEMMAP_FAILED);
      copy_map.remove ();
    }
  else
    {
      ACE_OS::memcpy (dst, src, this->size_);

      if (original_map.unmap () == -1 
	  || copy_map.unmap () == -1)
	this->error (ACE_Filecache_Object::MEMMAP_FAILED);
    }

  ACE_OS::close (original);
  ACE_OS::close (copy);
}

ACE_Filecache_Object::ACE_Filecache_Object (const char * filename, int size)
{
  this->init ();

  this->size_ = size;
  ACE_OS::strcpy (this->filename_, filename);
  this->action (ACE_Filecache_Object::WRITING);
  
  // Can we access the file?
  if (ACE_OS::access (this->filename_, R_OK|W_OK) == -1
      // Does it exist?
      && ACE_OS::access (this->filename_, F_OK) != -1)
    {
      // File exists, but we cannot access it.
      this->error (ACE_Filecache_Object::ACCESS_FAILED);
      return;
    }

  this->tempname_ = ACE_OS::tempnam (".", "zJAWS");
}

ACE_Filecache_Object::~ACE_Filecache_Object (void)
{
  if (this->tempname_)
    {
      if (this->error_ == SUCCESS)
        ACE_OS::unlink (this->tempname_);
      ACE_OS::free (this->tempname_);
    }
}

int
ACE_Filecache_Object::acquire (void)
{
  ACE_Write_Guard<ACE_SYNCH_RW_MUTEX> m (this->lock_);

  if (this->reference_count_++ == 0)
    {
      if (this->error_ == SUCCESS)
        // load file into memory
        switch (this->action_)
          {
          case ACE_Filecache_Object::READING:
            this->handle_ = ACE_OS::open (this->tempname_, READ_FLAGS, R_MASK);

            if (this->handle_ == ACE_INVALID_HANDLE)
              this->error_i (ACE_Filecache_Object::OPEN_FAILED,
                             "ACE_Filecache_Object::acquire: open");
            else if (this->mmap_.map (this->handle_, -1,
                                      PROT_READ, MAP_PRIVATE) != 0)
              {
                this->error_i (ACE_Filecache_Object::MEMMAP_FAILED,
                               "ACE_Filecache_Object::acquire: map");
                ACE_OS::close (this->handle_);
                this->handle_ = ACE_INVALID_HANDLE;
              }
            else
              this->action_ = ACE_Filecache_Object::READING;
            break;

          case ACE_Filecache_Object::WRITING:
            this->handle_ = ACE_OS::open (this->tempname_,
					  WRITE_FLAGS,
					  W_MASK);

            if (this->handle_ == ACE_INVALID_HANDLE)
              this->error_i (ACE_Filecache_Object::OPEN_FAILED,
                             "ACE_Filecache_Object::acquire: open");
            else if (ACE_OS::lseek (this->handle_,
                                    this->size_ - 1,
                                    SEEK_SET) == -1)
              {
                this->error_i (ACE_Filecache_Object::OPEN_FAILED,
                               "ACE_Filecache_Object::acquire: lseek");
                ACE_OS::close (this->handle_);
              }
            else if (ACE_OS::write (this->handle_, "", 1) != 1)
              {
                this->error_i (ACE_Filecache_Object::WRITE_FAILED,
                               "ACE_Filecache_Object::acquire: write");
                ACE_OS::close (this->handle_);
              }
            else if (this->mmap_.map (this->handle_, this->size_,
                                      PROT_RDWR, MAP_SHARED) != 0)
              {
                this->error_i (ACE_Filecache_Object::MEMMAP_FAILED,
                               "ACE_Filecache_Object::acquire: map");
                ACE_OS::close (this->handle_);
              }
            break;

          default:
            break;
          }
    }

  return this->reference_count_;
}

int
ACE_Filecache_Object::release (void)
{
  ACE_Write_Guard<ACE_SYNCH_RW_MUTEX> m (this->lock_);

  if (--this->reference_count_ == 0)
    {
      if (this->error_ == SUCCESS)
        // Free file from memory if reference count is zero.
        switch (this->action_)
          {
          case ACE_Filecache_Object::READING:
            this->mmap_.unmap ();
            ACE_OS::close (this->handle_);
            this->handle_ = ACE_INVALID_HANDLE;
            break;

          case ACE_Filecache_Object::WRITING:
            do
              {
                ACE_HANDLE original = ACE_OS::open (this->filename_,
                                                    WRITE_FLAGS, W_MASK);
                if (original == ACE_INVALID_HANDLE)
		  this->error_ = ACE_Filecache_Object::OPEN_FAILED;
                else if (ACE_OS::write (original, this->mmap_.addr (),
                                        this->size_) == -1)
                  {
                    this->error_ = ACE_Filecache_Object::WRITE_FAILED;
                    ACE_OS::close (original);
                    ACE_OS::unlink (this->filename_);
                  }
                else if (ACE_OS::stat (this->filename_, &this->stat_) == -1)
		  this->error_ = ACE_Filecache_Object::STAT_FAILED;

                this->mmap_.unmap ();
                ACE_OS::close (this->handle_);
                this->handle_ = ACE_INVALID_HANDLE;
                this->action_ = ACE_Filecache_Object::READING;
              }
            while (0);
            break;

          default:
            break;
          }
    }

  return this->reference_count_;
}

int
ACE_Filecache_Object::action (void) const
{
  ACE_Read_Guard<ACE_SYNCH_RW_MUTEX> m (((ACE_Filecache_Object *) this)->lock_);
  return this->action_;
}

int
ACE_Filecache_Object::action (int action_value)
{
  ACE_Write_Guard<ACE_SYNCH_RW_MUTEX> m (this->lock_);
  return (this->action_ = action_value);
}

int
ACE_Filecache_Object::error (void) const
{
  ACE_Read_Guard<ACE_SYNCH_RW_MUTEX> m (((ACE_Filecache_Object *) this)->lock_);
  return this->error_;
}

int
ACE_Filecache_Object::error (int error_value, const char * s)
{
  ACE_Write_Guard<ACE_SYNCH_RW_MUTEX> m (this->lock_);
  return this->error_i (error_value, s);
}

int
ACE_Filecache_Object::error_i (int error_value, const char * s)
{
  s = s;
  ACE_ERROR ((LM_ERROR, "%p.\n", s));
  this->error_ = error_value;
  return error_value;
}

const char *
ACE_Filecache_Object::filename (void) const
{
  ACE_Read_Guard<ACE_SYNCH_RW_MUTEX> m (((ACE_Filecache_Object *) this)->lock_);
  return this->filename_;
}

size_t
ACE_Filecache_Object::size (void) const
{
  ACE_Read_Guard<ACE_SYNCH_RW_MUTEX> m (((ACE_Filecache_Object *) this)->lock_);
  return this->size_;
}

ACE_HANDLE
ACE_Filecache_Object::handle (void) const
{
  ACE_Read_Guard<ACE_SYNCH_RW_MUTEX> m (((ACE_Filecache_Object *) this)->lock_);
  return this->handle_;
}

void *
ACE_Filecache_Object::address (void) const
{
  ACE_Read_Guard<ACE_SYNCH_RW_MUTEX> m (((ACE_Filecache_Object *) this)->lock_);
  return this->mmap_.addr ();
}

int
ACE_Filecache_Object::update (void) const
{
  ACE_Read_Guard<ACE_SYNCH_RW_MUTEX> m (((ACE_Filecache_Object *) this)->lock_);
  int result;
  struct stat statbuf;

  if (ACE_OS::stat (this->filename_, &statbuf) == -1)
    result = 1;
  else
    // non-portable code may follow
      result = (ACE_OS::difftime (this->stat_.st_mtime, statbuf.st_mtime) < 0);

  return result;
}
