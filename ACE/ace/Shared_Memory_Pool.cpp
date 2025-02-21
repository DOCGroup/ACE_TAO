#include "ace/Shared_Memory_Pool.h"
#include "ace/OS_NS_sys_shm.h"
#include "ace/Log_Category.h"

#if !defined (ACE_LACKS_SYSV_SHMEM)

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_ALLOC_HOOK_DEFINE(ACE_Shared_Memory_Pool)

ACE_Shared_Memory_Pool_Options::ACE_Shared_Memory_Pool_Options (
  const char *base_addr,
  size_t max_segments,
  size_t file_perms,
  ACE_OFF_T minimum_bytes,
  size_t segment_size)
  : base_addr_ (base_addr),
    max_segments_ (max_segments),
    minimum_bytes_ (minimum_bytes),
    file_perms_ (file_perms),
    segment_size_ (segment_size)
{
  ACE_TRACE ("ACE_Shared_Memory_Pool_Options::ACE_Shared_Memory_Pool_Options");
}

void
ACE_Shared_Memory_Pool::dump () const
{
#if defined (ACE_HAS_DUMP)
  ACE_TRACE ("ACE_Shared_Memory_Pool::dump");
#endif /* ACE_HAS_DUMP */
}

int
ACE_Shared_Memory_Pool::in_use (ACE_OFF_T &offset,
                                size_t &counter)
{
#ifndef ACE_HAS_SYSV_IPC
  ACE_UNUSED_ARG (offset);
  ACE_UNUSED_ARG (counter);
  ACE_NOTSUP_RETURN (-1);
#else
  offset = 0;
  SHM_TABLE *st = reinterpret_cast<SHM_TABLE *> (this->shm_addr_table_[0]);
  shmid_ds buf;

  for (counter = 0; counter < this->max_segments_ && st[counter].used_ == 1; counter++)
    {
      if (ACE_OS::shmctl (st[counter].shmid_, IPC_STAT, &buf) == -1)
        ACELIB_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("(%P|%t) ACE_Shared_Memory_Pool::in_use, %p\n"),
                           ACE_TEXT ("shmctl")),
                          -1);
      offset += buf.shm_segsz;
      // ACELIB_DEBUG ((LM_DEBUG,  ACE_TEXT ("(%P|%t) ACE_Shared_Memory_Pool::in_use, segment size = %d, offset = %d\n"), buf.shm_segsz, offset));
    }
  return 0;
#endif
}

int
ACE_Shared_Memory_Pool::find_seg (const void* const searchPtr,
                                  ACE_OFF_T &offset,
                                  size_t &counter)
{
#ifndef ACE_HAS_SYSV_IPC
  ACE_UNUSED_ARG (searchPtr);
  ACE_UNUSED_ARG (offset);
  ACE_UNUSED_ARG (counter);
  ACE_NOTSUP_RETURN (-1);
#else
  offset = 0;
  SHM_TABLE *st = reinterpret_cast<SHM_TABLE *> (this->shm_addr_table_[0]);
  shmid_ds buf;

  for (counter = 0; counter < this->max_segments_ && st[counter].used_ == 1; counter++)
    {
      if (ACE_OS::shmctl (st[counter].shmid_, IPC_STAT, &buf) == -1)
        ACELIB_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("(%P|%t) ACE_Shared_Memory_Pool::find_seg, %p\n"),
                           ACE_TEXT ("shmctl")),
                          -1);
      offset += buf.shm_segsz;

      // If segment 'counter' starts at a location greater than the
      // place we are searching for. We then decrement the offset to
      // the start of counter-1. (flabar@vais.net)
      if (((ptrdiff_t) offset + (ptrdiff_t) (this->shm_addr_table_[0])) > (ptrdiff_t) searchPtr)
        {
          --counter;
          offset -= buf.shm_segsz;
          return 0;
        }
      // ACELIB_DEBUG ((LM_DEBUG,  ACE_TEXT ("(%P|%t) ACE_Shared_Memory_Pool::find_seg, segment size = %d, offset = %d\n"), buf.shm_segsz, offset));
    }

  return 0;
#endif
}

int
ACE_Shared_Memory_Pool::commit_backing_store_name (size_t rounded_bytes,
                                                   ACE_OFF_T &offset)
{
  ACE_TRACE ("ACE_Shared_Memory_Pool::commit_backing_store_name");

  if (this->shm_addr_table_[0] == nullptr)
    {
      ACELIB_ERROR_RETURN ((LM_ERROR,
                        "ACE_Shared_Memory_Pool::commit_backing_store_name, base address is zero\n"),
                        -1);
    }

  size_t counter;
  SHM_TABLE *st = reinterpret_cast<SHM_TABLE *> (this->shm_addr_table_[0]);

  if (this->in_use (offset, counter) == -1)
    return -1;

  if (counter == this->max_segments_)
    {
      ACELIB_ERROR_RETURN ((LM_ERROR,
                        "ACE_Shared_Memory_Pool::commit_backing_store_name, exceeded max number of segments = %d, base = %u, offset = %u\n",
                        counter,
                        this->shm_addr_table_[0],
                        static_cast<unsigned int>(offset)),
                        -1);
    }
  else
    {
      int const shmid = ACE_OS::shmget (st[counter].key_,
                                        rounded_bytes,
                                        this->file_perms_ | IPC_CREAT | IPC_EXCL);
      if (shmid == -1)
        ACELIB_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("(%P|%t) ACE_Shared_Memory_Pool::commit_backing_store_name, %p\n"),
                           ACE_TEXT ("shmget")),
                          -1);
      st[counter].shmid_ = shmid;
      st[counter].used_ = 1;

      void *address = (void *) (((char *) this->shm_addr_table_[0]) + offset);
      void *shmem = ACE_OS::shmat (st[counter].shmid_, (char *) address, 0);

      if (shmem != address)
        ACELIB_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT("(%P|%t) ACE_Shared_Memory_Pool::commit_backing_store_name, %p, shmem = %u, address = %u\n"),
                           ACE_TEXT("shmat"),
                           shmem,
                           address),
                          -1);

      shm_addr_table_[counter] = shmem;
    }
  return 0;
}

/// Handle SIGSEGV and SIGBUS signals to remap shared memory properly.
int
ACE_Shared_Memory_Pool::handle_signal (int, siginfo_t *siginfo, ucontext_t *)
{
  ACE_TRACE ("ACE_Shared_Memory_Pool::handle_signal");

  // While FreeBSD 5.X has a siginfo_t struct with a si_addr field,
  // it does not define SEGV_MAPERR.
#if defined (ACE_HAS_SIGINFO_T) && !defined (ACE_LACKS_SI_ADDR) && \
        (defined (SEGV_MAPERR) || defined (SEGV_MEMERR))
  if (siginfo == 0)
    return -1;

  // Make sure that the pointer causing the problem is within the
  // range of the backing store.
  // ACELIB_DEBUG ((LM_DEBUG,  ACE_TEXT ("(%P|%t) ACE_Shared_Memory_Pool::handle_signal, si_signo = %d, si_code = %d, addr = %u\n"), siginfo->si_signo, siginfo->si_code, siginfo->si_addr));
  ACE_OFF_T offset;
  size_t counter = 0;
  if (this->in_use (offset, counter) == -1)
    {
      ACELIB_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) ACE_Shared_Memory_Pool::handle_signal, %p\n"),
                  ACE_TEXT ("in_use")));
    }
  else if (!(siginfo->si_code == SEGV_MAPERR
        && siginfo->si_addr < (((char *) this->shm_addr_table_[0]) + offset)
        && siginfo->si_addr >= ((char *) this->shm_addr_table_[0])))
    {
      ACELIB_ERROR_RETURN ((LM_ERROR,
                          "(%P|%t) ACE_Shared_Memory_Pool::handle_signal, address %u out of range, base = %u, offset = %u\n",
                          siginfo->si_addr,
                          this->shm_addr_table_[0],
                          static_cast<unsigned int>(offset)),
                        -1);
    }

  // The above if case will check to see that the address is in the
  // proper range.  Therefore there is a segment out there that the
  // pointer wants to point into.  Find the segment that someone else
  // has used and attach to it (flabar@vais.net)
  counter = 0; // ret value to get shmid from the st table.
  if (this->find_seg (siginfo->si_addr, offset, counter) == -1)
      ACELIB_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%P|%t) ACE_Shared_Memory_Pool::handle_signal, %p\n"),
                         ACE_TEXT ("in_use")),
                        -1);

  void *address = (void *) (((char *) this->shm_addr_table_[0]) + offset);
  SHM_TABLE *st = reinterpret_cast<SHM_TABLE *> (this->shm_addr_table_[0]);

  void *shmem = ACE_OS::shmat (st[counter].shmid_, (char *) address, 0);

  if (shmem != address)
      ACELIB_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT("(%P|%t) ACE_Shared_Memory_Pool::handle_signal, %p, shmem = %u, address = %u\n"),
                         ACE_TEXT("shmat"),
                         shmem,
                         address),
                        -1);

  // NOTE: this won't work if we dont have SIGINFO_T or SI_ADDR
#else
  ACE_UNUSED_ARG (siginfo);
#endif /* ACE_HAS_SIGINFO_T && !defined (ACE_LACKS_SI_ADDR) */

  return 0;
}

ACE_Shared_Memory_Pool::ACE_Shared_Memory_Pool (
  const ACE_TCHAR *backing_store_name,
  const OPTIONS *options)
  : file_perms_ (options ? options->file_perms_ : ACE_DEFAULT_FILE_PERMS),
    max_segments_ (options ? options->max_segments_ : ACE_DEFAULT_MAX_SEGMENTS),
    minimum_bytes_ (options ? options->minimum_bytes_ : 0),
    segment_size_ (options ? options->segment_size_ : ACE_DEFAULT_SEGMENT_SIZE),
    shm_addr_table_ (std::make_unique<void *[]> (this->max_segments_))
{
  ACE_TRACE ("ACE_Shared_Memory_Pool::ACE_Shared_Memory_Pool");

  // Only change the defaults if options != nullptr.
  if (options)
    {
      this->shm_addr_table_[0] = reinterpret_cast<void *> (const_cast<char *> (options->base_addr_));
    }

#ifndef ACE_HAS_SYSV_IPC
  ACE_UNUSED_ARG (backing_store_name);
#else
  if (backing_store_name)
    {
      // Convert the string into a number that is used as the segment
      // key.
      int segment_key = 0;
#if !defined (ACE_LACKS_SSCANF)
      int const result = ::sscanf (ACE_TEXT_ALWAYS_CHAR (backing_store_name),
                                   "%d",
                                   &segment_key);
#else
      int const result = 0;
#endif /* ACE_LACKS_SSCANF */
      if (result == 0 || result == EOF)
        {
          // The conversion to a number failed so hash with crc32
          // ACE::crc32 is also used in <SV_Semaphore_Simple>.
          this->base_shm_key_ = (key_t) ACE::crc32 (ACE_TEXT_ALWAYS_CHAR (backing_store_name));
        }
      else
        {
          this->base_shm_key_ = segment_key;
        }

      if (this->base_shm_key_ == IPC_PRIVATE)
        {
          // Make sure that the segment can be shared between unrelated
          // processes.
          this->base_shm_key_ = ACE_DEFAULT_SHM_KEY;
        }
    }
  else
    this->base_shm_key_ = ACE_DEFAULT_SHM_KEY;
#endif // ACE_HAS_SYSV_IPC

  if (this->signal_handler_.register_handler (SIGSEGV, this) == -1)
    {
      ACELIB_ERROR ((LM_ERROR,
                  ACE_TEXT ("ACE_Shared_Memory_Pool::ACE_Shared_Memory_Pool, %p\n"),
                  ACE_TEXT ("ACE_Sig_Handler::register_handler")));
    }
}

/// Ask system for more shared memory.
void *
ACE_Shared_Memory_Pool::acquire (size_t nbytes, size_t &rounded_bytes)
{
  ACE_TRACE ("ACE_Shared_Memory_Pool::acquire");

  rounded_bytes = this->round_up (nbytes);

  // ACELIB_DEBUG ((LM_DEBUG,  ACE_TEXT ("(%P|%t) ACE_Shared_Memory_Pool::acquire, acquiring more chunks, nbytes = %d, rounded_bytes = %d\n"), nbytes, rounded_bytes));

  ACE_OFF_T offset;

  if (this->commit_backing_store_name (rounded_bytes, offset) == -1)
    return nullptr;

  // ACELIB_DEBUG ((LM_DEBUG,  ACE_TEXT ("(%P|%t) ACE_Shared_Memory_Pool::acquire, acquired more chunks, nbytes = %d, rounded_bytes = %d\n"), nbytes, rounded_bytes));
  return ((char *) this->shm_addr_table_[0]) + offset;
}

/// Ask system for initial chunk of shared memory.
void *
ACE_Shared_Memory_Pool::init_acquire (size_t nbytes,
                                      size_t &rounded_bytes,
                                      int &first_time)
{
  ACE_TRACE ("ACE_Shared_Memory_Pool::init_acquire");

  ACE_OFF_T const shm_table_offset = ACE::round_to_pagesize (sizeof (SHM_TABLE));
  rounded_bytes = this->round_up (nbytes > (size_t) this->minimum_bytes_
                                  ? nbytes
                                  : (size_t) this->minimum_bytes_);

  // Acquire the semaphore to serialize initialization and prevent
  // race conditions.
  int shmid = ACE_OS::shmget (this->base_shm_key_,
                              rounded_bytes + shm_table_offset,
                              this->file_perms_ | IPC_CREAT | IPC_EXCL);
  if (shmid == -1)
    {
      if (errno != EEXIST)
        ACELIB_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("(%P|%t) ACE_Shared_Memory_Pool::init_acquire, %p\n"),
                           ACE_TEXT ("shmget")),
                          0);
      first_time = 0;

      shmid = ACE_OS::shmget (this->base_shm_key_, 0, 0);

      if (shmid == -1)
        ACELIB_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("(%P|%t) ACE_Shared_Memory_Pool::init_acquire, %p\n"),
                           ACE_TEXT ("shmget")),
                          0);

      // This implementation doesn't care if we don't get the key we want...
      this->shm_addr_table_[0] = ACE_OS::shmat (shmid, reinterpret_cast<char *> (this->shm_addr_table_[0]), 0);

      if (this->shm_addr_table_[0] == reinterpret_cast<void *> (-1))
        {
          ACELIB_ERROR_RETURN ((LM_ERROR,
                            ACE_TEXT("(%P|%t) ACE_Shared_Memory_Pool::init_acquire, %p, base_addr = %u\n"),
                            ACE_TEXT("shmat"),
                            this->shm_addr_table_[0]),
                            0);
        }
    }
  else
    {
      first_time = 1;

      // This implementation doesn't care if we don't get the key we want...
      this->shm_addr_table_[0] = ACE_OS::shmat (shmid, reinterpret_cast<char *> (this->shm_addr_table_[0]), 0);

      if (this->shm_addr_table_[0] == reinterpret_cast<char *> (-1))
        {
          ACELIB_ERROR_RETURN ((LM_ERROR,
                            ACE_TEXT("(%P|%t) ACE_Shared_Memory_Pool::init_acquire, %p, base_addr = %u\n"),
                            ACE_TEXT("shmat"),
                            this->shm_addr_table_[0]), 0);
        }

      SHM_TABLE *st = reinterpret_cast<SHM_TABLE *> (this->shm_addr_table_[0]);
      st[0].key_ = this->base_shm_key_;
      st[0].shmid_ = shmid;
      st[0].used_ = 1;

      for (size_t counter = 1; // Skip over the first entry...
           counter < this->max_segments_;
           counter++)
        {
#ifdef ACE_HAS_SYSV_IPC
          st[counter].key_ = this->base_shm_key_ + counter;
#endif
          st[counter].shmid_ = 0;
          st[counter].used_ = 0;
          shm_addr_table_[counter] = nullptr;
        }
    }

  return (void *) (((char *) this->shm_addr_table_[0]) + shm_table_offset);
}

/// Instruct the memory pool to release all of its resources.
int
ACE_Shared_Memory_Pool::release (int destroy)
{
  ACE_TRACE ("ACE_Shared_Memory_Pool::release");

  int result = 0;

  // At the moment we have attached any segments we have to release/destroy these
  if (this->shm_addr_table_[0])
    {
      // The shared memory table is store in segment[0]
      SHM_TABLE *st = reinterpret_cast<SHM_TABLE *> (this->shm_addr_table_[0]);

      // Detach the mapped shared memory segments in reverse order.
      // We store the shared memory table in segment[0], so we have to destroy that
      // as last
      size_t counter = this->max_segments_;
      while (counter > 0)
        {
          --counter;

          // Get the shared memory id and used flag on the stack as we can't read the shared memory
          // anymore after we detached it
          int const shmid = st[counter].shmid_;
          int const used = st[counter].used_;

          // When we have an address attached for this segment we have to detach it
          if (this->shm_addr_table_[counter])
            {
              if (ACE_OS::shmdt (this->shm_addr_table_[counter]) == -1)
                {
                  result = -1;
                }
              this->shm_addr_table_[counter] = nullptr;
            }

          // When the segment is used and we are asked to destroy it we instruct the
          // OS to release it
          if (destroy == 1 && used == 1)
            {
              if (ACE_OS::shmctl (shmid, IPC_RMID, 0) == -1)
                {
                  result = -1;
                }
            }
        }
    }

  return result;
}

int
ACE_Shared_Memory_Pool::sync (ssize_t, int)
{
  ACE_TRACE ("ACE_Shared_Memory_Pool::sync");
  return 0;
}

int
ACE_Shared_Memory_Pool::sync (void *, size_t, int)
{
  ACE_TRACE ("ACE_Shared_Memory_Pool::sync");
  return 0;
}

int
ACE_Shared_Memory_Pool::protect (ssize_t, int)
{
  ACE_TRACE ("ACE_Shared_Memory_Pool::protect");
  return 0;
}

int
ACE_Shared_Memory_Pool::protect (void *, size_t, int)
{
  ACE_TRACE ("ACE_Shared_Memory_Pool::protect");
  return 0;
}

void *
ACE_Shared_Memory_Pool::base_addr () const
{
  ACE_TRACE ("ACE_Shared_Memory_Pool::base_addr");
  return this->shm_addr_table_[0];
}

/// Implement the algorithm for rounding up the request to an
/// appropriate chunksize.
size_t
ACE_Shared_Memory_Pool::round_up (size_t nbytes)
{
  ACE_TRACE ("ACE_Shared_Memory_Pool::round_up");
  if (nbytes < this->segment_size_)
    nbytes = this->segment_size_;

  return ACE::round_to_pagesize (nbytes);
}

ACE_END_VERSIONED_NAMESPACE_DECL

#endif /* !ACE_LACKS_SYSV_SHMEM */
