// $Id$

#define ACE_BUILD_DLL

#include "ace/Filecache.h"
#include "ace/Object_Manager.h"

static const int R_MASK = S_IRUSR|S_IRGRP|S_IROTH;
static const int W_MASK = S_IRUSR|S_IRGRP|S_IROTH|S_IWUSR|S_IWGRP|S_IWOTH;

#if defined (ACE_WIN32)
// See if you can get rid of some of these.
static const int READ_FLAGS = (FILE_FLAG_SEQUENTIAL_SCAN |
                               FILE_FLAG_OVERLAPPED |
                               O_RDONLY);
// static const int RCOPY_FLAGS = (FILE_FLAG_SEQUENTIAL_SCAN |
//                                 O_RDONLY);
static const int WRITE_FLAGS = (FILE_FLAG_SEQUENTIAL_SCAN |
                                FILE_FLAG_OVERLAPPED |
                                O_RDWR | O_CREAT | O_TRUNC);
// static const int WCOPY_FLAGS = (FILE_FLAG_SEQUENTIAL_SCAN |
//                                 O_RDWR | O_CREAT | O_TRUNC);
#else
static const int READ_FLAGS = O_RDONLY;
// static const int RCOPY_FLAGS = O_RDONLY;
static const int WRITE_FLAGS = O_RDWR | O_CREAT | O_TRUNC;
// static const int WCOPY_FLAGS = O_RDWR | O_CREAT | O_TRUNC;
#endif /* ACE_WIN32 */

// static data members
ACE_Filecache *ACE_Filecache::cvf_ = 0;

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

ACE_Filecache_Handle::ACE_Filecache_Handle (const char *filename)
{
  this->init ();

  // Fetch the file from the Virtual_Filesystem let the
  // Virtual_Filesystem do the work of cache coherency.

  // Filecache will also do the acquire, since it holds the lock at
  // that time.
  this->file_ = ACE_Filecache::instance ()->fetch (filename);
}

ACE_Filecache_Handle::ACE_Filecache_Handle (const char *filename,
                                            int size)
{
  this->init ();
  // Since this is being opened for a write, simply create a new
  // ACE_Filecache_Object now, and let the destructor add it into CVF
  // later

  // Filecache will also do the acquire, since it holds the lock at
  // that time.
  this->file_ = ACE_Filecache::instance ()->create (filename, size);
}

ACE_Filecache_Handle::~ACE_Filecache_Handle (void)
{
  if (this->handle_ != ACE_INVALID_HANDLE)
    // this was dup ()'d
    ACE_OS::close (this->handle_);

  ACE_Filecache::instance ()->finish (this->file_);
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


// ------------------
// ACE_Filecache_Hash
// ------------------

#if defined (ACE_HAS_TEMPLATE_SPECIALIZATION)

#define ACE_Filecache_Hash \
        ACE_Hash_Map_Manager<const char *, ACE_Filecache_Object *, ACE_Null_Mutex>
#define ACE_Filecache_Hash_Entry \
        ACE_Hash_Map_Entry<const char *, ACE_Filecache_Object *>


ACE_Filecache_Hash_Entry::ACE_Hash_Map_Entry (const char *const &ext_id,
                                              ACE_Filecache_Object *const &int_id,
                                              ACE_Filecache_Hash_Entry *ptr)
  : ext_id_ (ext_id ? ACE_OS::strdup (ext_id) : ACE_OS::strdup ("")),
    int_id_ (int_id),
    next_ (ptr)
{
}

ACE_Filecache_Hash_Entry::~ACE_Hash_Map_Entry (void)
{
  ACE_OS::free ((void *) ext_id_);
}

// We need these template specializations since KEY is defined as a
// char*, which doesn't have a hash() or equal() method defined on it.

long unsigned int
ACE_Filecache_Hash::hash (const char *const &ext_id)
{
  return ACE::hash_pjw (ext_id);
}

int
ACE_Filecache_Hash::equal (const char *const &id1, const char *const &id2)
{
  return ACE_OS::strcmp (id1, id2) == 0;
}

#undef ACE_Filecache_Hash
#undef ACE_Filecache_Hash_Entry

#endif /* ACE_HAS_TEMPLATE_SPECIALIZATION */


// -------------
// ACE_Filecache
// -------------

ACE_Filecache *
ACE_Filecache::instance (void)
{
  // Double check locking pattern.
  if (ACE_Filecache::cvf_ == 0)
    {
      ACE_SYNCH_RW_MUTEX &lock =
        *ACE_Managed_Object<ACE_SYNCH_RW_MUTEX>::get_preallocated_object
          (ACE_Object_Manager::ACE_FILECACHE_LOCK);
      ACE_Guard<ACE_SYNCH_RW_MUTEX> m (lock);

      // @@ James, please check each of the ACE_NEW_RETURN calls to
      // make sure that it is safe to return if allocation fails.
      if (ACE_Filecache::cvf_ == 0)
        ACE_NEW_RETURN (ACE_Filecache::cvf_, ACE_Filecache, 0);
    }

  return ACE_Filecache::cvf_;
}

ACE_Filecache::ACE_Filecache (void)
  : size_ (DEFAULT_VIRTUAL_FILESYSTEM_TABLE_SIZE),
    hash_ (this->size_)
{
}

ACE_Filecache::~ACE_Filecache (void)
{
}

ACE_Filecache_Object *
ACE_Filecache::insert_i (const char *filename, ACE_SYNCH_RW_MUTEX &filelock)
{
  ACE_Filecache_Object *handle = 0;

  if (this->hash_.find (filename, handle) == -1)
    {
      ACE_NEW_RETURN (handle,
                      ACE_Filecache_Object (filename, filelock),
                      0);

      ACE_DEBUG ((LM_DEBUG, "   (%t) CVF: creating %s\n", filename));

      if (this->hash_.bind (filename, handle) == -1)
        {
          delete handle;
          handle = 0;
        }
    }
  else
    handle = 0;

  return handle;
}

ACE_Filecache_Object *
ACE_Filecache::remove_i (const char *filename)
{
  ACE_Filecache_Object *handle = 0;

  // Disassociate file from the cache.
  if (this->hash_.unbind (filename, handle) == 0)
    {
      handle->stale_ = 1;

      // Try a lock.  If it succeds, we can delete it now.
      // Otherwise, it will clean itself up later.
      if (handle->lock_.tryacquire_write () == 0)
        {
          delete handle;
          handle = 0;
        }
    }
  else
    handle = 0;

  return handle;
}

ACE_Filecache_Object *
ACE_Filecache::update_i (const char *filename, ACE_SYNCH_RW_MUTEX &filelock)
{
  ACE_Filecache_Object *handle = 0;

  // Just in case someone removed it
  if (this->hash_.find (filename, handle) == -1)
    {
      ACE_NEW_RETURN (handle,
                      ACE_Filecache_Object (filename, filelock),
                      0);

      ACE_DEBUG ((LM_DEBUG, "   (%t) CVF: creating %s\n", filename));

      if (this->hash_.bind (filename, handle) == -1)
        {
          delete handle;
          handle = 0;
        }
    }
  else
    {
      if (handle->update ())
        {
          handle = this->remove_i (filename);

          ACE_NEW_RETURN (handle,
                          ACE_Filecache_Object (filename, filelock),
                          0);

          ACE_DEBUG ((LM_DEBUG, "   (%t) CVF: updating %s\n", filename));

          if (this->hash_.bind (filename, handle) == -1)
            {
              delete handle;
              handle = 0;
            }
        }
    }

  return handle;
}

int
ACE_Filecache::find (const char *filename)
{
  return this->hash_.find (filename);
}


ACE_Filecache_Object *
ACE_Filecache::fetch (const char *filename)
{
  ACE_Filecache_Object *handle = 0;

  u_long loc = ACE::hash_pjw (filename) % this->size_;
  ACE_SYNCH_RW_MUTEX &hashlock = this->hash_lock_[loc];
  ACE_SYNCH_RW_MUTEX &filelock = this->file_lock_[loc];

  filelock.acquire_read ();

  if (this->hash_.find (filename, handle) == -1)
    {
      ACE_Write_Guard<ACE_SYNCH_RW_MUTEX> m (hashlock);

      // Second check in the method call
      handle = this->insert_i (filename, filelock);

      if (handle == 0)
        filelock.release ();
    }
  else
    {
      if (handle->update ())
        {
          filelock.release ();
          {
            // Double check locking pattern
            ACE_Write_Guard<ACE_SYNCH_RW_MUTEX> m (hashlock);

            // Second check in the method call
            handle = this->update_i (filename, filelock);
          }

          if (handle)
            filelock.acquire_read ();
        }
      ACE_DEBUG ((LM_DEBUG, "   (%t) CVF: found %s\n", filename));
    }

  return handle;
}

ACE_Filecache_Object *
ACE_Filecache::create (const char *filename, int size)
{
  ACE_Filecache_Object *handle = 0;

  u_long loc = ACE::hash_pjw (filename) % this->size_;
  ACE_SYNCH_RW_MUTEX &filelock = this->file_lock_[loc];

  ACE_NEW_RETURN (handle,
                  ACE_Filecache_Object (filename, size, filelock),
                  0);
  handle->acquire ();

  return handle;
}

ACE_Filecache_Object *
ACE_Filecache::finish (ACE_Filecache_Object *&file)
{
  if (file == 0)
    return file;

  int result;

  u_long loc = ACE::hash_pjw (file->filename_) % this->size_;
  ACE_SYNCH_RW_MUTEX &hashlock = this->hash_lock_[loc];

  if (file != 0)
    switch (file->action_)
      {
      case ACE_Filecache_Object::WRITING:
        {
          ACE_Write_Guard<ACE_SYNCH_RW_MUTEX> m (hashlock);

          file->release ();

          this->remove_i ((char *) file->filename_);
          result = this->hash_.bind (file->filename_, file);

          if (result == 0)
            file->acquire ();
        }

        break;
      default:
        file->release ();

        // Last one using a stale file is resposible for deleting it.
        if (file->stale_)
          {
            // Try a lock.  If it succeds, we can delete it now.
            // Otherwise, it will clean itself up later.
            if (file->lock_.tryacquire_write () == 0)
              {
                delete file;
                file = 0;
              }
          }

        break;
      }

  return file;
}

void
ACE_Filecache_Object::init (void)
{
  this->filename_[0] = '\0';
  this->handle_ = ACE_INVALID_HANDLE;
  this->error_ = SUCCESS;
  this->tempname_ = 0;
  this->size_ = 0;

  ACE_OS::memset (&(this->stat_), 0, sizeof (this->stat_));
}

ACE_Filecache_Object::ACE_Filecache_Object (void)
  : stale_ (0),
    lock_ (junklock_)
{
  this->init ();
}

ACE_Filecache_Object::ACE_Filecache_Object (const char *filename,
                                            ACE_SYNCH_RW_MUTEX &lock,
                                            LPSECURITY_ATTRIBUTES sa)
  : stale_ (0),
    sa_ (sa),
    lock_ (lock)
{
  this->init ();

  // ASSERT strlen(filename) < sizeof (this->filename_)
  ACE_OS::strcpy (this->filename_, filename);
  this->action_ = ACE_Filecache_Object::READING;
  // place ourselves into the READING state

  // Can we access the file?
  if (ACE_OS::access (this->filename_, R_OK) == -1)
    {
      this->error_i (ACE_Filecache_Object::ACCESS_FAILED);
      return;
    }

  // Can we stat the file?
  if (ACE_OS::stat (this->filename_, &this->stat_) == -1)
    {
      this->error_i (ACE_Filecache_Object::STAT_FAILED);
      return;
    }

  this->size_ = this->stat_.st_size;
  this->tempname_ = (char *) this->filename_;

  // Can we open the file?
  this->handle_ = ACE_OS::open (this->tempname_, READ_FLAGS, R_MASK, this->sa_);
  if (this->handle_ == ACE_INVALID_HANDLE)
    {
      this->error_i (ACE_Filecache_Object::OPEN_FAILED,
                     "ACE_Filecache_Object::acquire: open");
      return;
    }

  // Can we map the file?
   if (this->mmap_.map (this->handle_, -1,
                            PROT_READ, MAP_PRIVATE, 0, 0, this->sa_) != 0)
    {
      this->error_i (ACE_Filecache_Object::MEMMAP_FAILED,
                     "ACE_Filecache_Object::acquire: map");
      ACE_OS::close (this->handle_);
      this->handle_ = ACE_INVALID_HANDLE;
      return;
    }

   // Ok, finished!
   this->action_ = ACE_Filecache_Object::READING;
}

ACE_Filecache_Object::ACE_Filecache_Object (const char *filename,
                                            int size,
                                            ACE_SYNCH_RW_MUTEX &lock,
                                            LPSECURITY_ATTRIBUTES sa)
  : stale_ (0),
    sa_ (sa),
    lock_ (lock)
{
  this->init ();

  this->size_ = size;
  ACE_OS::strcpy (this->filename_, filename);
  this->action_ = ACE_Filecache_Object::WRITING;

  // Can we access the file?
  if (ACE_OS::access (this->filename_, R_OK|W_OK) == -1
      // Does it exist?
      && ACE_OS::access (this->filename_, F_OK) != -1)
    {
      // File exists, but we cannot access it.
      this->error_i (ACE_Filecache_Object::ACCESS_FAILED);
      return;
    }

  this->tempname_ = this->filename_;

  // Can we open the file?
  this->handle_ = ACE_OS::open (this->tempname_, WRITE_FLAGS, W_MASK, this->sa_);
  if (this->handle_ == ACE_INVALID_HANDLE)
    {
      this->error_i (ACE_Filecache_Object::OPEN_FAILED,
                     "ACE_Filecache_Object::acquire: open");
      return;
    }

  // Can we seek?
  if (ACE_OS::lseek (this->handle_, this->size_ - 1, SEEK_SET) == -1)
    {
      this->error_i (ACE_Filecache_Object::OPEN_FAILED,
                     "ACE_Filecache_Object::acquire: lseek");
      ACE_OS::close (this->handle_);
      return;
    }

  // Can we write?
  if (ACE_OS::write (this->handle_, "", 1) != 1)
    {
      this->error_i (ACE_Filecache_Object::WRITE_FAILED,
                     "ACE_Filecache_Object::acquire: write");
      ACE_OS::close (this->handle_);
      return;
    }

  // Can we map?
  if (this->mmap_.map (this->handle_, this->size_, PROT_RDWR, MAP_SHARED,
                       0, 0, this->sa_) != 0)
    {
      this->error_i (ACE_Filecache_Object::MEMMAP_FAILED,
                     "ACE_Filecache_Object::acquire: map");
      ACE_OS::close (this->handle_);
    }

  // Ok, done!
}

ACE_Filecache_Object::~ACE_Filecache_Object (void)
{
  if (this->error_ == SUCCESS)
    {
      this->mmap_.unmap ();
      ACE_OS::close (this->handle_);
      this->handle_ = ACE_INVALID_HANDLE;
    }
}

int
ACE_Filecache_Object::acquire (void)
{
  return this->lock_.tryacquire_read ();
}

int
ACE_Filecache_Object::release (void)
{
  if (this->action_ == WRITING)
    {
      // We are safe since only one thread has a writable Filecache_Object

      ACE_HANDLE original = ACE_OS::open (this->filename_, WRITE_FLAGS, W_MASK,
                                          this->sa_);
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

      // Leave the file in an acquirable state.
      this->handle_ = ACE_OS::open (this->tempname_, READ_FLAGS, R_MASK);
      if (this->handle_ == ACE_INVALID_HANDLE)
        {
          this->error_i (ACE_Filecache_Object::OPEN_FAILED,
                         "ACE_Filecache_Object::acquire: open");
        }
      else if (this->mmap_.map (this->handle_, -1,
                                PROT_READ,
                                MAP_PRIVATE,
                                0,
                                0,
                                this->sa_) != 0)
        {
          this->error_i (ACE_Filecache_Object::MEMMAP_FAILED,
                         "ACE_Filecache_Object::acquire: map");
          ACE_OS::close (this->handle_);
          this->handle_ = ACE_INVALID_HANDLE;
        }

      this->action_ = ACE_Filecache_Object::READING;
    }

  return this->lock_.release ();
}

int
ACE_Filecache_Object::error (void) const
{
  // The existence of the object means a read lock is being held.
  return this->error_;
}

int
ACE_Filecache_Object::error_i (int error_value, const char *s)
{
  s = s;
  ACE_ERROR ((LM_ERROR, "%p.\n", s));
  this->error_ = error_value;
  return error_value;
}

const char *
ACE_Filecache_Object::filename (void) const
{
  // The existence of the object means a read lock is being held.
  return this->filename_;
}

size_t
ACE_Filecache_Object::size (void) const
{
  // The existence of the object means a read lock is being held.
  return this->size_;
}

ACE_HANDLE
ACE_Filecache_Object::handle (void) const
{
  // The existence of the object means a read lock is being held.
  return this->handle_;
}

void *
ACE_Filecache_Object::address (void) const
{
  // The existence of the object means a read lock is being held.
  return this->mmap_.addr ();
}

int
ACE_Filecache_Object::update (void) const
{
  // The existence of the object means a read lock is being held.
  int result;
  struct stat statbuf;

  if (ACE_OS::stat (this->filename_, &statbuf) == -1)
    result = 1;
  else
    // non-portable code may follow
    result = ACE_OS::difftime (this->stat_.st_mtime, statbuf.st_mtime) < 0;

  return result;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
#if defined (ACE_HAS_TEMPLATE_SPECIALIZATION)
template class ACE_Hash_Map_Entry<const char *, ACE_Filecache_Object *>;
template class ACE_Hash_Map_Manager<const char *, ACE_Filecache_Object *, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator<const char *, ACE_Filecache_Object *, ACE_Null_Mutex>;
#else
template class ACE_Hash_Map_Entry<ACE_CString, ACE_Filecache_Object *>;
template class ACE_Hash_Map_Manager<ACE_CString, ACE_Filecache_Object *, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator<ACE_CString, ACE_Filecache_Object *, ACE_Null_Mutex>;
#endif /* ACE_HAS_TEMPLATE_SPECIALIZATION */
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#if defined (ACE_HAS_TEMPLATE_SPECIALIZATION)
#pragma instantiate ACE_Hash_Map_Entry<const char *, ACE_Filecache_Object *>
#pragma instantiate ACE_Hash_Map_Manager<const char *, ACE_Filecache_Object *, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator<const char *, ACE_Filecache_Object *, ACE_Null_Mutex>
#else
#pragma instantiate ACE_Hash_Map_Entry<ACE_CString, ACE_Filecache_Object *>
#pragma instantiate ACE_Hash_Map_Manager<ACE_CString, ACE_Filecache_Object *, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator<ACE_CString, ACE_Filecache_Object *, ACE_Null_Mutex>
#endif /* ACE_HAS_TEMPLATE_SPECIALIZATION */
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
