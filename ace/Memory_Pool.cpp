// $Id$

// Memory_Pool.cpp
#include "ace/Memory_Pool.h"
#include "ace/Log_Msg.h"

#if !defined (__ACE_INLINE__)
#include "ace/Memory_Pool.i"
#endif /* __ACE_INLINE__ */

#include "ace/Auto_Ptr.h"

#if (ACE_HAS_POSITION_INDEPENDENT_POINTERS == 1)
#include "ace/Based_Pointer_T.h"
#include "ace/Based_Pointer_Repository.h"
#endif /* ACE_HAS_POSITION_INDEPENDENT_POINTERS == 1  */

ACE_RCSID(ace, Memory_Pool, "Memory_Pool.cpp,v 4.79 2001/09/02 22:33:16 schmidt Exp")

ACE_ALLOC_HOOK_DEFINE(ACE_Local_Memory_Pool)

void
ACE_Local_Memory_Pool::dump (void) const
{
#if defined (ACE_HAS_DUMP)
  ACE_TRACE ("ACE_Local_Memory_Pool::dump");
#endif /* ACE_HAS_DUMP */
}

ACE_Local_Memory_Pool::ACE_Local_Memory_Pool (const ACE_TCHAR *,
                                              const OPTIONS *)
{
  ACE_TRACE ("ACE_Local_Memory_Pool::ACE_Local_Memory_Pool");
}

void *
ACE_Local_Memory_Pool::acquire (size_t nbytes,
                                size_t &rounded_bytes)
{
  ACE_TRACE ("ACE_Local_Memory_Pool::acquire");
  rounded_bytes = this->round_up (nbytes);

  char *temp = 0;
  ACE_NEW_RETURN (temp,
                  char[rounded_bytes],
                  0);

  ACE_Auto_Basic_Array_Ptr<char> cp (temp);

  if (this->allocated_chunks_.insert (cp.get ()) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_LIB_TEXT ("(%P|%t) insertion into set failed\n")),
                      0);

  return cp.release ();
}

int
ACE_Local_Memory_Pool::release (int)
{
  ACE_TRACE ("ACE_Local_Memory_Pool::release");

  // Zap the memory we allocated.
  for (ACE_Unbounded_Set<char *>::iterator i = this->allocated_chunks_.begin ();
       i != this->allocated_chunks_.end ();
       ++i)
    delete [] *i;
  this->allocated_chunks_.reset ();
  return 0;
}

#if defined (ACE_WIN32)
int
ACE_Local_Memory_Pool::seh_selector (void *)
{
  return 0;
  // Continue propagate the structural exception up.
}
#endif /* ACE_WIN32 */

int
ACE_Local_Memory_Pool::remap (void *)
{
  return 0;
  // Not much can be done.
}

ACE_ALLOC_HOOK_DEFINE(ACE_MMAP_Memory_Pool)

void
ACE_MMAP_Memory_Pool::dump (void) const
{
#if defined (ACE_HAS_DUMP)
  ACE_TRACE ("ACE_MMAP_Memory_Pool::dump");
#endif /* ACE_HAS_DUMP */
}

int
ACE_MMAP_Memory_Pool::release (int destroy)
{
  ACE_TRACE ("ACE_MMAP_Memory_Pool::release");

#if (ACE_HAS_POSITION_INDEPENDENT_POINTERS == 1)
  ACE_BASED_POINTER_REPOSITORY::instance ()->unbind (this->mmap_.addr ());
#endif /* ACE_HAS_POSITION_INDEPENDENT_POINTERS == 1 */

  if (destroy)
  this->mmap_.remove ();
  else
    this->mmap_.close ();
  return 0;
}

int
ACE_MMAP_Memory_Pool::sync (ssize_t len, int flags)
{
  ACE_TRACE ("ACE_MMAP_Memory_Pool::sync");

  if (len < 0)
    len = ACE_OS::lseek (this->mmap_.handle (), 0, SEEK_END);

  return this->mmap_.sync (len, flags);
}

// Sync <len> bytes of the memory region to the backing store starting
// at <addr_>.

int
ACE_MMAP_Memory_Pool::sync (void *addr, size_t len, int flags)
{
  ACE_TRACE ("ACE_MMAP_Memory_Pool::sync");
  return ACE_OS::msync (addr, len, flags);
}

// Change the protection of the pages of the mapped region to <prot>
// starting at <this->base_addr_> up to <len> bytes.  If <len> == -1
// then change protection of all pages in the mapped region.

int
ACE_MMAP_Memory_Pool::protect (ssize_t len, int prot)
{
  ACE_TRACE ("ACE_MMAP_Memory_Pool::protect");

  if (len < 0)
    len = ACE_OS::lseek (this->mmap_.handle (), 0, SEEK_END);

  return this->mmap_.protect (len, prot);
}

// Change the protection of the pages of the mapped region to <prot>
// starting at <addr> up to <len> bytes.

int
ACE_MMAP_Memory_Pool::protect (void *addr, size_t len, int prot)
{
  ACE_TRACE ("ACE_MMAP_Memory_Pool::protect");
  return ACE_OS::mprotect (addr, len, prot);
}

ACE_MMAP_Memory_Pool::ACE_MMAP_Memory_Pool (const ACE_TCHAR *backing_store_name,
                                            const OPTIONS *options)
  : base_addr_ (0),
    use_fixed_addr_(0),
    flags_ (MAP_SHARED),
    write_each_page_ (0),
    minimum_bytes_ (0),
    sa_ (0),
    file_mode_ (ACE_DEFAULT_FILE_PERMS)
{
  ACE_TRACE ("ACE_MMAP_Memory_Pool::ACE_MMAP_Memory_Pool");

#if (defined (ACE_HAS_SIGINFO_T) && !defined (ACE_LACKS_SI_ADDR)) || defined (ACE_WIN32)
      // For plaforms that give the faulting address.
      guess_on_fault_ = 0;
#else
      // For plaforms that do NOT give the faulting address, let the
      // options decide whether to guess or not.
      if (options)
        guess_on_fault_ = options->guess_on_fault_;
      else
        // If no options are specified, default to true.
        guess_on_fault_ = 1;
#endif /* (defined (ACE_HAS_SIGINFO_T) && !defined (ACE_LACKS_SI_ADDR)) || defined (ACE_WIN32) */

  // Only change the defaults if <options> != 0.
  if (options)
    {
      if (options->flags_ != 0)
        this->flags_ = options->flags_;
      use_fixed_addr_ = options->use_fixed_addr_;

      if (use_fixed_addr_ == ACE_MMAP_Memory_Pool_Options::ALWAYS_FIXED)
        {
          this->base_addr_ = ACE_const_cast (void *, options->base_addr_);
          ACE_SET_BITS (flags_, MAP_FIXED);
        }
      this->write_each_page_ = options->write_each_page_;
      this->minimum_bytes_ = options->minimum_bytes_;
      if (options->sa_ != 0)
        this->sa_ = options->sa_;
      this->file_mode_ = options->file_mode_;
    }

  if (backing_store_name == 0)
    {
      // Only create a new unique filename for the backing store file
      // if the user didn't supply one...
#if defined (ACE_DEFAULT_BACKING_STORE)
      // Create a temporary file.
      ACE_OS::strcpy (this->backing_store_name_,
                      ACE_DEFAULT_BACKING_STORE);
#else /* ACE_DEFAULT_BACKING_STORE */
      if (ACE_Lib_Find::get_temp_dir (this->backing_store_name_,
                                      MAXPATHLEN - 17) == -1)
        // -17 for ace-malloc-XXXXXX
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_LIB_TEXT ("Temporary path too long, ")
                      ACE_LIB_TEXT ("defaulting to current directory\n")));
          this->backing_store_name_[0] = 0;
        }

      // Add the filename to the end
      ACE_OS::strcat (this->backing_store_name_,
                      ACE_LIB_TEXT ("ace-malloc-XXXXXX"));

#endif /* ACE_DEFAULT_BACKING_STORE */
    }
  else
    ACE_OS::strsncpy (this->backing_store_name_,
                      backing_store_name,
                      (sizeof this->backing_store_name_ / sizeof (ACE_TCHAR)));

#if !defined (ACE_WIN32) && !defined (CHORUS)
  if (this->signal_handler_.register_handler (SIGSEGV, this) == -1)
    ACE_ERROR ((LM_ERROR,
                "%p\n", this->backing_store_name_));
#endif /* ACE_WIN32 */
}

// Compute the new map_size of the backing store and commit the
// memory.
int
ACE_MMAP_Memory_Pool::commit_backing_store_name (size_t rounded_bytes,
                                                 off_t &map_size)
{
  ACE_TRACE ("ACE_MMAP_Memory_Pool::commit_backing_store_name");

#if defined (CHORUS)
  map_size = rounded_bytes;
#else
  size_t seek_len;

  if (this->write_each_page_)
    // Write to the end of every block to ensure that we have enough
    // space in the backing store.
    seek_len = this->round_up (1); // round_up(1) is one page.
  else
    // We're willing to risk it all in the name of efficiency...
    seek_len = rounded_bytes;

  // The following loop will execute multiple times (if
  // this->write_each_page == 1) or just once (if
  // this->write_each_page == 0).

  for (size_t cur_block = 0;
       cur_block < rounded_bytes;
       cur_block += seek_len)
    {
      map_size = ACE_OS::lseek (this->mmap_.handle (),
                                ACE_static_cast (off_t, seek_len - 1),
                                SEEK_END);

      if (map_size == -1
          || ACE_OS::write (this->mmap_.handle (),
                            "",
                            1) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_LIB_TEXT ("(%P|%t) %p\n"),
                           this->backing_store_name_),
                          -1);
    }

  // Increment by one to put us at the beginning of the next chunk...
  map_size++;
#endif /* CHORUS */
  return 0;
}

// Memory map the file up to <map_size> bytes.

int
ACE_MMAP_Memory_Pool::map_file (off_t map_size)
{
  ACE_TRACE ("ACE_MMAP_Memory_Pool::map_file");

  // Unmap the existing mapping.
  this->mmap_.unmap ();

#if (ACE_HAS_POSITION_INDEPENDENT_POINTERS == 1)
  if(use_fixed_addr_ == ACE_MMAP_Memory_Pool_Options::NEVER_FIXED)
    this->base_addr_ = 0;
#endif /* ACE_HAS_POSITION_INDEPENDENT_POINTERS == 1 */

  // Remap the file.
  if (this->mmap_.map (map_size,
                       PROT_RDWR,
                       this->flags_,
                       this->base_addr_,
                       0,
                       this->sa_) == -1
      || this->base_addr_ != 0
#ifdef ACE_HAS_WINCE
      && this->mmap_.addr () == 0)  // WinCE does not allow users to specify alloc addr.
#else
      && this->mmap_.addr () != this->base_addr_)
#endif  // ACE_HAS_WINCE
    {
#if 0
      ACE_ERROR ((LM_ERROR,
                  ACE_LIB_TEXT ("(%P|%t) addr = %u, base_addr = %u, map_size = %u, %p\n"),
                  this->mmap_.addr (),
                  this->base_addr_,
                  map_size,
                  this->backing_store_name_));
#endif /* 0 */
      return -1;
    }
  else
    {
#if (ACE_HAS_POSITION_INDEPENDENT_POINTERS == 1)
      this->base_addr_ = this->mmap_.addr ();
      ACE_BASED_POINTER_REPOSITORY::instance ()->bind (this->base_addr_,
                                                       map_size);
#endif /* ACE_HAS_POSITION_INDEPENDENT_POINTERS == 1 */
      return 0;
    }
}

// Ask operating system for more shared memory, increasing the mapping
// accordingly.  Note that this routine assumes that the appropriate
// locks are held when it is called.

void *
ACE_MMAP_Memory_Pool::acquire (size_t nbytes,
                               size_t &rounded_bytes)
{
  ACE_TRACE ("ACE_MMAP_Memory_Pool::acquire");
  rounded_bytes = this->round_up (nbytes);

  // ACE_DEBUG ((LM_DEBUG, "(%P|%t) acquiring more chunks, nbytes =
  // %d, rounded_bytes = %d\n", nbytes, rounded_bytes));

  off_t map_size;

  if (this->commit_backing_store_name (rounded_bytes,
                                       map_size) == -1)
    return 0;
  else if (this->map_file (map_size) == -1)
    return 0;

  // ACE_DEBUG ((LM_DEBUG, "(%P|%t) acquired more chunks, nbytes = %d,
  // rounded_bytes = %d, map_size = %d\n", nbytes, rounded_bytes,
  // map_size));

  return (void *) ((char *) this->mmap_.addr () + (this->mmap_.size () - rounded_bytes));
}

// Ask system for initial chunk of shared memory.

void *
ACE_MMAP_Memory_Pool::init_acquire (size_t nbytes,
                                    size_t &rounded_bytes,
                                    int &first_time)
{
  ACE_TRACE ("ACE_MMAP_Memory_Pool::init_acquire");

  first_time = 0;

  if (nbytes < (size_t) this->minimum_bytes_)
    nbytes = this->minimum_bytes_;

  if (this->mmap_.open (this->backing_store_name_,
                        O_RDWR | O_CREAT | O_TRUNC | O_EXCL,
                        this->file_mode_, this->sa_) != -1)
    {
      // First time in, so need to acquire memory.
      first_time = 1;
      return this->acquire (nbytes, rounded_bytes);
    }
  else if (errno == EEXIST)
    {
      errno = 0;
      // Reopen file *without* using O_EXCL...
      if (this->mmap_.map (this->backing_store_name_,
#if defined (CHORUS)
                           nbytes,
#else
                           -1,
#endif /* CHORUS */
                           O_RDWR,
                           this->file_mode_,
                           PROT_RDWR,
                           this->flags_,
                           this->base_addr_,
                           0,
                           this->sa_) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_LIB_TEXT ("%p\n"),
                           ACE_LIB_TEXT ("open")),
                          0);

      return this->mmap_.addr ();
    }
  else
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_LIB_TEXT ("%p\n"),
                       ACE_LIB_TEXT ("open")),
                      0);
}

#if defined (ACE_WIN32)
int
ACE_MMAP_Memory_Pool::seh_selector (void *ep)
{
  DWORD ecode = ((EXCEPTION_POINTERS *) ep)->ExceptionRecord->ExceptionCode;

  if (ecode == EXCEPTION_ACCESS_VIOLATION)
    {
      void * fault_addr = (void *)
        ((EXCEPTION_POINTERS *) ep)->ExceptionRecord->ExceptionInformation[1];

      if (this->remap (fault_addr) == 0)
        return 1;
    }

  return 0;
}
#endif /* ACE_WIN32 */

int
ACE_MMAP_Memory_Pool::remap (void *addr)
{
  ACE_TRACE ("ACE_MMAP_Memory_Pool::remap");
  //  ACE_DEBUG ((LM_DEBUG,  ACE_LIB_TEXT ("Remapping with fault address at: %X\n"), addr));
  off_t current_map_size = ACE_OS::filesize (this->mmap_.handle ());
  // ACE_OS::lseek (this->mmap_.handle (), 0, SEEK_END);

  if (!(addr < (void *) ((char *) this->mmap_.addr () + current_map_size)
        && addr >= this->mmap_.addr ()))
    return -1;

  // Extend the mapping to cover the size of the backing store.
  return this->map_file (current_map_size);
}

ACE_MMAP_Memory_Pool_Options::ACE_MMAP_Memory_Pool_Options (const void *base_addr,
                                                            int use_fixed_addr,
                                                            int write_each_page,
                                                            off_t minimum_bytes,
                                                            u_int flags,
                                                            int guess_on_fault,
                                                            LPSECURITY_ATTRIBUTES sa,
                                                            mode_t file_mode)
  : base_addr_ (base_addr),
    use_fixed_addr_ (use_fixed_addr),
    write_each_page_ (write_each_page),
    minimum_bytes_ (minimum_bytes),
    flags_ (flags),
    guess_on_fault_ (guess_on_fault),
    sa_ (sa),
    file_mode_ (file_mode)
{
  ACE_TRACE ("ACE_MMAP_Memory_Pool_Options::ACE_MMAP_Memory_Pool_Options");
  // for backwards compatability
  if (base_addr_ == 0 && use_fixed_addr_ == ALWAYS_FIXED)
    use_fixed_addr_ = FIRSTCALL_FIXED;

  // HP-UX 11, 64-bit bug workaround.
#if defined (__hpux) && defined (__LP64__)
  long temp = ACE_DEFAULT_BASE_ADDRL;
  base_addr_ = (void *) temp;
#endif /* defined (__hpux) && defined (__LP64__) */
}

// Handle SIGSEGV and SIGBUS signals to remap memory properly.  When a
// process reads or writes to non-mapped memory a signal (SIGBUS or
// SIGSEGV) will be triggered.  At that point, the ACE_Sig_Handler
// (which is part of the ACE_Reactor) will catch the signal and
// dispatch the handle_signal() method defined here.  If the SIGSEGV
// signal occurred due to the fact that the mapping wasn't uptodate
// with respect to the backing store, the handler method below will
// update the mapping accordingly.  When the signal handler returns,
// the instruction should be restarted and the operation should work.

int
ACE_MMAP_Memory_Pool::handle_signal (int signum, siginfo_t *siginfo, ucontext_t *)
{
  if (signum != SIGSEGV)
    return -1;
  else
    ; // ACE_DEBUG ((LM_DEBUG,  ACE_LIB_TEXT ("(%P|%t) received %S\n"), signum));

  // ACE_DEBUG ((LM_DEBUG,  ACE_LIB_TEXT ("(%P|%t) new mapping address = %u\n"), (char *) this->base_addr_ + current_map_size));

#if defined (ACE_HAS_SIGINFO_T) && !defined (ACE_LACKS_SI_ADDR)
  // Make sure that the pointer causing the problem is within the
  // range of the backing store.

  if (siginfo != 0)
    {
      // ACE_DEBUG ((LM_DEBUG,  ACE_LIB_TEXT ("(%P|%t) si_signo = %d, si_code = %d, addr = %u\n"), siginfo->si_signo, siginfo->si_code, siginfo->si_addr));
      if (this->remap ((void *) siginfo->si_addr) == -1)
        return -1;
      // ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) address %u out of range\n",
      // siginfo->si_addr), -1);
      return 0;
    }
#else
  ACE_UNUSED_ARG(siginfo);
#endif /* ACE_HAS_SIGINFO_T && !defined ACE_LACKS_SI_ADDR */
  // If guess_on_fault_ is true, then we want to try to remap without
  // knowing the faulting address.  guess_on_fault_ can only be true
  // on platforms that do not provide the faulting address through
  // signals or exceptions.  We check to see if the mapping is up to
  // date. If it is, then this fault isn't due to this mapping and we
  // pass it on.
  if (guess_on_fault_)
    {
      // Check if the current mapping is up to date.
      off_t current_map_size = ACE_OS::filesize (this->mmap_.handle ());

      if (ACE_static_cast (size_t, current_map_size) == this->mmap_.size ())
        {
          // The mapping is up to date so this really is a bad
          // address.  Thus, remove current signal handler so process
          // will fail with default action and core file will be
          // written.
          this->signal_handler_.remove_handler (SIGSEGV);
          return 0;
        }

      // Extend the mapping to cover the size of the backing store.
      return this->map_file (current_map_size);
    }
  else
    return -1;
}

ACE_ALLOC_HOOK_DEFINE(ACE_Lite_MMAP_Memory_Pool)

ACE_Lite_MMAP_Memory_Pool::ACE_Lite_MMAP_Memory_Pool (const ACE_TCHAR *backing_store_name,
                                                      const OPTIONS *options)
  : ACE_MMAP_Memory_Pool (backing_store_name, options)
{
  ACE_TRACE ("ACE_Lite_MMAP_Memory_Pool::ACE_Lite_MMAP_Memory_Pool");
}

int
ACE_Lite_MMAP_Memory_Pool::sync (ssize_t, int)
{
  ACE_TRACE ("ACE_Lite_MMAP_Memory_Pool::sync");
  return 0;
}

int
ACE_Lite_MMAP_Memory_Pool::sync (void *, size_t, int)
{
  ACE_TRACE ("ACE_Lite_MMAP_Memory_Pool::sync");
  return 0;
}

#if !defined (ACE_LACKS_SBRK)
ACE_ALLOC_HOOK_DEFINE(ACE_Sbrk_Memory_Pool)

// Ask system for more local memory via sbrk(2).

void *
ACE_Sbrk_Memory_Pool::acquire (size_t nbytes,
                               size_t &rounded_bytes)
{
  ACE_TRACE ("ACE_Sbrk_Memory_Pool::acquire");
  rounded_bytes = this->round_up (nbytes);
  // ACE_DEBUG ((LM_DEBUG,  ACE_LIB_TEXT ("(%P|%t) acquiring more chunks, nbytes = %d, rounded_bytes = %d\n"), nbytes, rounded_bytes));
  void *cp = ACE_OS::sbrk (rounded_bytes);

  if (cp == MAP_FAILED)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) cp = %u\n",
                       cp),
                      0);
  else
    // ACE_DEBUG ((LM_DEBUG,  ACE_LIB_TEXT ("(%P|%t) acquired more chunks, nbytes = %d, rounded_bytes = %d, new break = %u\n"), nbytes, rounded_bytes, cp));
  return cp;
}

void
ACE_Sbrk_Memory_Pool::dump (void) const
{
#if defined (ACE_HAS_DUMP)
  ACE_TRACE ("ACE_Sbrk_Memory_Pool::dump");
#endif /* ACE_HAS_DUMP */
}

ACE_Sbrk_Memory_Pool::ACE_Sbrk_Memory_Pool (const ACE_TCHAR *,
                                            const OPTIONS *)
{
  ACE_TRACE ("ACE_Sbrk_Memory_Pool::ACE_Sbrk_Memory_Pool");
}
#endif /* !ACE_LACKS_SBRK */

#if !defined (ACE_LACKS_SYSV_SHMEM)
ACE_ALLOC_HOOK_DEFINE(ACE_Shared_Memory_Pool)

ACE_Shared_Memory_Pool_Options::ACE_Shared_Memory_Pool_Options (const char *base_addr,
                                                                size_t max_segments,
                                                                size_t file_perms,
                                                                off_t minimum_bytes,
                                                                size_t segment_size)
  : base_addr_ (base_addr),
    max_segments_ (max_segments),
    minimum_bytes_ (minimum_bytes),
    file_perms_ (file_perms),
    segment_size_ (segment_size)
{
  ACE_TRACE ("ACE_Shared_Memory_Pool_Options::ACE_Shared_Memory_Pool_Options");
  // HP-UX 11, 64-bit bug workaround
#if defined (__hpux) && defined (__LP64__)
  long temp = ACE_DEFAULT_BASE_ADDRL;
  base_addr_ = (char *) temp;
#endif /* defined (__hpux) && defined (__LP64__) */
}

void
ACE_Shared_Memory_Pool::dump (void) const
{
#if defined (ACE_HAS_DUMP)
  ACE_TRACE ("ACE_Shared_Memory_Pool::dump");
#endif /* ACE_HAS_DUMP */
}

int
ACE_Shared_Memory_Pool::in_use (off_t &offset,
                                size_t &counter)
{
  offset = 0;
  SHM_TABLE *st = ACE_reinterpret_cast (SHM_TABLE *,
                                        this->base_addr_);
  shmid_ds buf;

  for (counter = 0;
       counter < this->max_segments_ && st[counter].used_ == 1;
       counter++)
    {
      if (ACE_OS::shmctl (st[counter].shmid_, IPC_STAT, &buf) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_LIB_TEXT ("(%P|%t) %p\n"),
                           ACE_LIB_TEXT ("shmctl")),
                          -1);
      offset += buf.shm_segsz;
      // ACE_DEBUG ((LM_DEBUG,  ACE_LIB_TEXT ("(%P|%t) segment size = %d, offset = %d\n"), buf.shm_segsz, offset));
    }

  return 0;
}

int
ACE_Shared_Memory_Pool::find_seg (const void* const searchPtr,
                                  off_t &offset,
                                  size_t &counter)
{
  offset = 0;
  SHM_TABLE *st = ACE_reinterpret_cast (SHM_TABLE *,
                                        this->base_addr_);
  shmid_ds buf;

  for (counter = 0;
       counter < this->max_segments_
         && st[counter].used_ == 1;
       counter++)
    {
      if (ACE_OS::shmctl (st[counter].shmid_, IPC_STAT, &buf) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_LIB_TEXT ("(%P|%t) %p\n"),
                           ACE_LIB_TEXT ("shmctl")),
                          -1);
      offset += buf.shm_segsz;

      // If segment 'counter' starts at a location greater than the
      // place we are searching for. We then decrement the offset to
      // the start of counter-1. (flabar@vais.net)
      if (((ptrdiff_t) offset + (ptrdiff_t) (this->base_addr_)) > (ptrdiff_t) searchPtr)
        {
          --counter;
          offset -= buf.shm_segsz;
          return 0;
        }
      // ACE_DEBUG ((LM_DEBUG,  ACE_LIB_TEXT ("(%P|%t) segment size = %d, offset = %d\n"), buf.shm_segsz, offset));
    }

  return 0;
}

int
ACE_Shared_Memory_Pool::commit_backing_store_name (size_t rounded_bytes,
                                                   off_t &offset)
{
  ACE_TRACE ("ACE_Shared_Memory_Pool::commit_backing_store_name");

  size_t counter;
  SHM_TABLE *st = ACE_reinterpret_cast (SHM_TABLE *,
                                        this->base_addr_);

  if (this->in_use (offset, counter) == -1)
    return -1;

  if (counter == this->max_segments_)
    ACE_ERROR_RETURN ((LM_ERROR,
                      "exceeded max number of segments = %d, base = %u, offset = %u\n",
                       counter,
                       this->base_addr_,
                       offset),
                      -1);
  else
    {
      int shmid = ACE_OS::shmget (st[counter].key_,
                                  rounded_bytes,
                                  this->file_perms_ | IPC_CREAT | IPC_EXCL);
      if (shmid == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_LIB_TEXT ("(%P|%t) %p\n"),
                           ACE_LIB_TEXT ("shmget")),
                          0);
      st[counter].shmid_ = shmid;
      st[counter].used_ = 1;

      void *address = (void *) (((char *) this->base_addr_) + offset);
      void *shmem = ACE_OS::shmat (st[counter].shmid_,
                                   (char *) address,
                                   0);

      if (shmem != address)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) %p, shmem = %u, address = %u\n",
                           "shmat",
                           shmem,
                           address),
                          0);
    }
  return 0;
}

// Handle SIGSEGV and SIGBUS signals to remap shared memory properly.

int
ACE_Shared_Memory_Pool::handle_signal (int , siginfo_t *siginfo, ucontext_t *)
{
  ACE_TRACE ("ACE_Shared_Memory_Pool::handle_signal");
  // ACE_DEBUG ((LM_DEBUG,  ACE_LIB_TEXT ("signal %S occurred\n"), signum));

#if defined (ACE_HAS_SIGINFO_T) && !defined (ACE_LACKS_SI_ADDR)
  off_t offset;
  // Make sure that the pointer causing the problem is within the
  // range of the backing store.

  if (siginfo != 0)
    {
      // ACE_DEBUG ((LM_DEBUG,  ACE_LIB_TEXT ("(%P|%t) si_signo = %d, si_code = %d, addr = %u\n"), siginfo->si_signo, siginfo->si_code, siginfo->si_addr));
      size_t counter;
      if (this->in_use (offset, counter) == -1)
        ACE_ERROR ((LM_ERROR,
                    ACE_LIB_TEXT ("(%P|%t) %p\n"),
                    ACE_LIB_TEXT ("in_use")));
#if !defined(_UNICOS)
      else if (!(siginfo->si_code == SEGV_MAPERR
           && siginfo->si_addr < (((char *) this->base_addr_) + offset)
           && siginfo->si_addr >= ((char *) this->base_addr_)))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) address %u out of range\n",
                           siginfo->si_addr),
                          -1);
#else /* ! _UNICOS */
      else if (!(siginfo->si_code == SEGV_MEMERR
           && siginfo->si_addr < (((unsigned long) this->base_addr_) + offset)
           && siginfo->si_addr >= ((unsigned long) this->base_addr_)))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) address %u out of range\n",
                           siginfo->si_addr),
                          -1);
#endif /* ! _UNICOS */
    }

  // The above if case will check to see that the address is in the
  // proper range.  Therefore there is a segment out there that the
  // pointer wants to point into.  Find the segment that someone else
  // has used and attach to it (flabar@vais.net)

  size_t counter; // ret value to get shmid from the st table.

#if !defined(_UNICOS)
  if (this->find_seg (siginfo->si_addr, offset, counter) == -1)
#else /* ! _UNICOS */
  if (this->find_seg ((const void *)siginfo->si_addr, offset, counter) == -1)
#endif /* ! _UNICOS */
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_LIB_TEXT ("(%P|%t) %p\n"),
                         ACE_LIB_TEXT ("in_use")),
                        -1);

  void *address = (void *) (((char *) this->base_addr_) + offset);
  SHM_TABLE *st = ACE_reinterpret_cast (SHM_TABLE *,
                                        this->base_addr_);

  void *shmem = ACE_OS::shmat (st[counter].shmid_, (char *) address, 0);

  if (shmem != address)
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%P|%t) %p, shmem = %u, address = %u\n",
                         "shmat",
                         shmem,
                         address),
                        0);

  // NOTE: this won't work if we dont have SIGINFO_T or SI_ADDR
#else
  ACE_UNUSED_ARG (siginfo);
#endif /* ACE_HAS_SIGINFO_T && !defined (ACE_LACKS_SI_ADDR) */

  return 0;
}

ACE_Shared_Memory_Pool::ACE_Shared_Memory_Pool (const ACE_TCHAR *backing_store_name,
                                                const OPTIONS *options)
  : base_addr_ (0),
    file_perms_ (ACE_DEFAULT_FILE_PERMS),
    max_segments_ (ACE_DEFAULT_MAX_SEGMENTS),
    minimum_bytes_ (0),
    segment_size_ (ACE_DEFAULT_SEGMENT_SIZE)
{
  ACE_TRACE ("ACE_Shared_Memory_Pool::ACE_Shared_Memory_Pool");

  // Only change the defaults if <options> != 0.
  if (options)
    {
      this->base_addr_ =
        ACE_reinterpret_cast (void *,
                              ACE_const_cast (char *,
                                              options->base_addr_));
      this->max_segments_ = options->max_segments_;
      this->file_perms_ = options->file_perms_;
      this->minimum_bytes_ = options->minimum_bytes_;
      this->segment_size_ = options->segment_size_;
    }

  if (backing_store_name)
    {
      // Convert the string into a number that is used as the segment
      // key.

      int segment_key;
      int result = ::sscanf (backing_store_name,
                             "%d",
                             &segment_key);

      if (result == 0 || result == EOF)
        // The conversion to a number failed so hash with crc32
        // ACE::crc32 is also used in <SV_Semaphore_Simple>.
        this->base_shm_key_ = (key_t) ACE::crc32 (backing_store_name);
      else
        this->base_shm_key_ = segment_key;

      if (this->base_shm_key_ == IPC_PRIVATE)
        // Make sure that the segment can be shared between unrelated
        // processes.
        this->base_shm_key_ = ACE_DEFAULT_SHM_KEY;
    }
  else
    this->base_shm_key_ = ACE_DEFAULT_SHM_KEY;

  if (this->signal_handler_.register_handler (SIGSEGV, this) == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_LIB_TEXT ("%p\n"),
                ACE_LIB_TEXT ("ACE_Sig_Handler::register_handler")));
}

// Ask system for more shared memory.

void *
ACE_Shared_Memory_Pool::acquire (size_t nbytes,
                                 size_t &rounded_bytes)
{
  ACE_TRACE ("ACE_Shared_Memory_Pool::acquire");

  rounded_bytes = this->round_up (nbytes);

  // ACE_DEBUG ((LM_DEBUG,  ACE_LIB_TEXT ("(%P|%t) acquiring more chunks, nbytes = %d, rounded_bytes = %d\n"), nbytes, rounded_bytes));

  off_t offset;

  if (this->commit_backing_store_name (rounded_bytes, offset) == -1)
    return 0;

  // ACE_DEBUG ((LM_DEBUG,  ACE_LIB_TEXT ("(%P|%t) acquired more chunks, nbytes = %d, rounded_bytes = %d\n"), nbytes, rounded_bytes));
  return ((char *) this->base_addr_) + offset;
}

// Ask system for initial chunk of shared memory.

void *
ACE_Shared_Memory_Pool::init_acquire (size_t nbytes,
                                      size_t &rounded_bytes,
                                      int &first_time)
{
  ACE_TRACE ("ACE_Shared_Memory_Pool::init_acquire");

  off_t shm_table_offset = ACE::round_to_pagesize (sizeof (SHM_TABLE));
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
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_LIB_TEXT ("(%P|%t) %p\n"),
                           ACE_LIB_TEXT ("shmget")),
                          0);
      first_time = 0;

      shmid = ACE_OS::shmget (this->base_shm_key_, 0, 0);

      if (shmid == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_LIB_TEXT ("(%P|%t) %p\n"),
                           ACE_LIB_TEXT ("shmget")),
                          0);

      // This implementation doesn't care if we don't get the key we
      // want...
      this->base_addr_ =
        ACE_OS::shmat (shmid,
                       ACE_reinterpret_cast (char *,
                                             this->base_addr_),
                       0);
      if (this->base_addr_ == ACE_reinterpret_cast (void *, -1))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) %p, base_addr = %u\n",
                           "shmat",
                           this->base_addr_),
                          0);
    }
  else
    {
      first_time = 1;

      // This implementation doesn't care if we don't get the key we
      // want...
      this->base_addr_ =
        ACE_OS::shmat (shmid,
                       ACE_reinterpret_cast (char *,
                                             this->base_addr_),
                       0);
      if (this->base_addr_ == ACE_reinterpret_cast (char *, -1))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) %p, base_addr = %u\n",
                           "shmat",
                           this->base_addr_), 0);

      SHM_TABLE *st = ACE_reinterpret_cast (SHM_TABLE *,
                                            this->base_addr_);
      st[0].key_ = this->base_shm_key_;
      st[0].shmid_ = shmid;

      st[0].used_ = 1;

      for (size_t counter = 1; // Skip over the first entry...
           counter < this->max_segments_;
           counter++)
        {
          st[counter].key_ = this->base_shm_key_ + counter;
          st[counter].shmid_ = 0;
          st[counter].used_ = 0;
        }
    }

  return (void *) (((char *) this->base_addr_) + shm_table_offset);
}

// Instruct the memory pool to release all of its resources.

int
ACE_Shared_Memory_Pool::release (int)
{
  ACE_TRACE ("ACE_Shared_Memory_Pool::release");

  int result = 0;
  SHM_TABLE *st = ACE_reinterpret_cast (SHM_TABLE *,
                                        this->base_addr_);

  for (size_t counter = 0;
       counter < this->max_segments_ && st[counter].used_ == 1;
       counter++)
    if (ACE_OS::shmctl (st[counter].shmid_, IPC_RMID, 0) == -1)
      result = -1;

  return result;
}
#endif /* !ACE_LACKS_SYSV_SHMEM */

#if defined (ACE_WIN32)
#if !defined (ACE_HAS_WINCE)
#define ACE_MAP_FILE(_hnd, _access, _offHigh, _offLow, _nBytes, _baseAdd)\
  MapViewOfFileEx (_hnd, _access, _offHigh, _offLow, _nBytes, _baseAdd)
#else //if !defined (ACE_HAS_WINCE)
#define ACE_MAP_FILE(_hnd, _access, _offHigh, _offLow, _nBytes, _baseAdd)\
  MapViewOfFile (_hnd, _access, _offHigh, _offLow, _nBytes)
#endif /* !defined (ACE_HAS_WINCE) */

ACE_Pagefile_Memory_Pool_Options::ACE_Pagefile_Memory_Pool_Options (void *base_addr,
                                                                    size_t max_size)
  : base_addr_ (base_addr),
    max_size_ (max_size)
{
}

int
ACE_Pagefile_Memory_Pool::release (int)
{
  return this->unmap ();
}

ACE_Pagefile_Memory_Pool::ACE_Pagefile_Memory_Pool (const ACE_TCHAR *backing_store_name,
                                                    const OPTIONS *options)
  : shared_cb_ (0),
    object_handle_ (0),
    page_size_ (ACE_Pagefile_Memory_Pool::round_to_page_size (1))
{
  // Initialize local copy of pool statistics.
  if (options != 0)
    {
      this->local_cb_.req_base_ = options->base_addr_;
      this->local_cb_.mapped_base_ = 0;
      this->local_cb_.sh_.max_size_ =
        options->max_size_;
      this->local_cb_.sh_.mapped_size_ = 0;
      this->local_cb_.sh_.free_offset_ =
        this->local_cb_.sh_.mapped_size_;
      this->local_cb_.sh_.free_size_ = 0;
    }

  if (backing_store_name == 0)
    // Only create a new unique filename for the backing store file if
    // the user didn't supply one...
    backing_store_name = ACE_DEFAULT_PAGEFILE_POOL_NAME;

  ACE_OS::strsncpy (this->backing_store_name_,
                    backing_store_name,
                    (sizeof this->backing_store_name_ / sizeof (ACE_TCHAR)));
}

void *
ACE_Pagefile_Memory_Pool::acquire (size_t nbytes,
                                   size_t &rounded_bytes)
{
  rounded_bytes = round_to_page_size (nbytes);
  void *result = 0;
  int first_time = 0;

  // Check local_cb_ for consistency.  Remap, if extra space is too
  // small and/or we didn't map the whole shared memory section
  if (this->shared_cb_->sh_.mapped_size_
      > this->local_cb_.sh_.mapped_size_
      || this->shared_cb_->sh_.free_size_ < rounded_bytes)
    {
      size_t append = 0;
      if (rounded_bytes > this->shared_cb_->sh_.free_size_)
        append = rounded_bytes - this->shared_cb_->sh_.free_size_;

      if (this->map (first_time, append) < 0)
        return result;
    }

  // Get the block from extra space and update shared and local
  // control block
  if (this->shared_cb_->sh_.free_size_ < rounded_bytes)
    return result;

  result = (void *)((char *) this->local_cb_.mapped_base_
                    + this->shared_cb_->sh_.free_offset_);
  this->shared_cb_->sh_.free_offset_ += rounded_bytes;
  this->shared_cb_->sh_.free_size_ -= rounded_bytes;
  this->local_cb_.sh_ = this->shared_cb_->sh_;

  return result;
}

void *
ACE_Pagefile_Memory_Pool::init_acquire (size_t nbytes,
                                        size_t &rounded_bytes,
                                        int &first_time)
{
  // Map the shared memory and get information, if we created the
  // shared memory.
  if (this->map (first_time) < 0)
    return 0;

  if (first_time != 0)
    // We created the shared memory. So we have to allocate the
    // requested memory.
    return this->acquire (nbytes, rounded_bytes);
  else
    // We just mapped the memory and return the base address
    return (void *)((char *) this->local_cb_.mapped_base_
                    + ACE_Pagefile_Memory_Pool::round_to_page_size
                    ((int) sizeof (Control_Block)));
}

int
ACE_Pagefile_Memory_Pool::seh_selector (void *ep)
{
  DWORD ecode = ((EXCEPTION_POINTERS *) ep)->ExceptionRecord->ExceptionCode;

  if (ecode == EXCEPTION_ACCESS_VIOLATION)
    {
      void * fault_addr = (void *)
        ((EXCEPTION_POINTERS *) ep)->ExceptionRecord->ExceptionInformation[1];

      if (this->remap (fault_addr) == 0)
        return 1;
    }

  return 0;
}

int
ACE_Pagefile_Memory_Pool::remap (void *addr)
{
  // If the shared memory is not mapped or the address, that caused
  // the memory fault is outside of the commited range of chunks, we
  // return.
  if (this->shared_cb_ == 0
      || addr < this->local_cb_.mapped_base_
      || addr >= (void *)((char *) this->local_cb_.mapped_base_
                          + this->shared_cb_->sh_.mapped_size_))
    return -1;

  // We can solve the problem by committing additional chunks.
  int first_time = 0;
  return this->map (first_time);
}

int
ACE_Pagefile_Memory_Pool::unmap (void)
{
#if (ACE_HAS_POSITION_INDEPENDENT_POINTERS == 1)
  ACE_BASED_POINTER_REPOSITORY::instance ()->unbind
    (this->local_cb_.mapped_base_);
#endif /* ACE_HAS_POSITION_INDEPENDENT_POINTERS == 1 */

  // Cleanup cached pool pointer.
  this->shared_cb_ = 0;

  if (this->local_cb_.sh_.mapped_size_ > 0)
    ::UnmapViewOfFile (this->local_cb_.mapped_base_);

  // Reset local pool statistics.
  this->local_cb_.req_base_ =
    ACE_DEFAULT_PAGEFILE_POOL_BASE;
  this->local_cb_.mapped_base_ = 0;
  this->local_cb_.sh_.max_size_ =
    ACE_DEFAULT_PAGEFILE_POOL_SIZE;
  this->local_cb_.sh_.mapped_size_ = 0;
  this->local_cb_.sh_.free_offset_ =
    this->local_cb_.sh_.mapped_size_;
  this->local_cb_.sh_.free_size_ = 0;

  // Release the pool
  if (this->object_handle_ != 0)
    {
      ::CloseHandle (this->object_handle_);
      this->object_handle_ = 0;
    }
  return 0;
}

int
ACE_Pagefile_Memory_Pool::map (int &first_time,
                               size_t append_bytes)
{
  size_t map_size;
  void *map_addr;

  // Create file mapping, if not yet done
  if (object_handle_ == 0)
    {
#if (defined (ACE_HAS_WINNT4) && (ACE_HAS_WINNT4 != 0))
      // Allow access by all users.
      SECURITY_ATTRIBUTES sa;
      SECURITY_DESCRIPTOR sd;
      ::InitializeSecurityDescriptor (&sd,
                                      SECURITY_DESCRIPTOR_REVISION);
      ::SetSecurityDescriptorDacl (&sd,
                                   TRUE,
                                   0,
                                   FALSE);
      sa.nLength = sizeof (SECURITY_ATTRIBUTES);
      sa.lpSecurityDescriptor = &sd;
      sa.bInheritHandle = FALSE;
#endif /* (defined (ACE_HAS_WINNT4) && (ACE_HAS_WINNT4 != 0)) */

      // Get an object handle to the named reserved memory object.
      DWORD size_high;
      DWORD size_low;
#if defined (ACE_WIN64)
      size_high = ACE_static_cast (DWORD,
                                   this->local_cb_.sh_.max_size_ >> 32);
      size_low  = ACE_static_cast (DWORD,
                                   this->local_cb_.sh_.max_size_ & 0xFFFFFFFF);
#else
      size_high = 0;
      size_low = this->local_cb_.sh_.max_size_;
#endif

      object_handle_ =
        ACE_TEXT_CreateFileMapping (INVALID_HANDLE_VALUE,
#if (defined (ACE_HAS_WINNT4) && (ACE_HAS_WINNT4 != 0))
                                    &sa,
#else
                                    0,
#endif /* (defined (ACE_HAS_WINNT4) && (ACE_HAS_WINNT4 != 0)) */
                                    PAGE_READWRITE | SEC_RESERVE,
                                    size_high,
                                    size_low,
                                    this->backing_store_name_);
      if (object_handle_ == 0)
        return -1;
      first_time =
        ::GetLastError () == ERROR_ALREADY_EXISTS
        ? 0
        : 1;
    }

  // Do the initial mapping.
  if (this->shared_cb_ == 0)
    {
      // Map a view to the shared memory.  Note: <MapViewOfFile[Ex]>
      // does *not* commit the pages!
      this->shared_cb_ = (ACE_Pagefile_Memory_Pool::Control_Block *)
        ACE_MAP_FILE (this->object_handle_,
                      FILE_MAP_WRITE,
                      0,
                      0,
                      this->local_cb_.sh_.max_size_,
                      this->local_cb_.req_base_);
      if (this->shared_cb_ == 0)
        return -1;

      // There was no previous mapping, so we map the first chunk and
      // initialize the shared pool statistics.
      if (first_time)
        {
          // 1st block is used to keep shared memory statistics.
          map_size =
            ACE_Pagefile_Memory_Pool::round_to_chunk_size
            (ACE_Pagefile_Memory_Pool::round_to_page_size
             ((int) sizeof(Control_Block))
             + append_bytes);

          if (::VirtualAlloc ((void *) this->shared_cb_,
                              map_size,
                              MEM_COMMIT,
                              PAGE_READWRITE) == 0)
            return -1;

          this->shared_cb_->req_base_ = 0;
          this->shared_cb_->mapped_base_ = 0;
          this->local_cb_.mapped_base_ = this->shared_cb_;
          this->local_cb_.sh_.mapped_size_ = map_size;
          this->local_cb_.sh_.free_offset_ =
            round_to_page_size ((int) sizeof (Control_Block));
          this->local_cb_.sh_.free_size_ =
            this->local_cb_.sh_.mapped_size_ -
            this->local_cb_.sh_.free_offset_;
          this->shared_cb_->sh_ = this->local_cb_.sh_;
        }

      // The shared memory exists, so we map the first chunk to the
      // base address of the pool to get the shared pool statistics.
      else
        {
          // 1st block is used to keep shared memory statistics.
          map_size =
            ACE_Pagefile_Memory_Pool::round_to_chunk_size
            ((int) sizeof (Control_Block));

          if (::VirtualAlloc ((void *) this->shared_cb_,
                              map_size,
                              MEM_COMMIT,
                              PAGE_READWRITE) == 0)
            return -1;
          this->local_cb_.mapped_base_ = this->shared_cb_;
          this->local_cb_.sh_.mapped_size_ = map_size;
        }
    }

  // If the shared memory is larger than the part we've already
  // committed, we have to remap it.
  if (this->shared_cb_->sh_.mapped_size_ >
      this->local_cb_.sh_.mapped_size_
      || append_bytes > 0)
    {
      map_size =
        (this->shared_cb_->sh_.mapped_size_ -
         this->local_cb_.sh_.mapped_size_)
        + ACE_Pagefile_Memory_Pool::round_to_chunk_size
        (append_bytes);

      map_addr = (void *)((char *) this->shared_cb_ +
                          this->local_cb_.sh_.mapped_size_);

      if (::VirtualAlloc (map_addr,
                          map_size,
                          MEM_COMMIT,
                          PAGE_READWRITE) == 0)
        return -1;
      else if (append_bytes > 0)
        {
          this->shared_cb_->sh_.mapped_size_ +=
            round_to_chunk_size (append_bytes);
          this->shared_cb_->sh_.free_size_ =
            this->shared_cb_->sh_.mapped_size_ -
            this->shared_cb_->sh_.free_offset_;
        }
    }

  // Update local copy of the shared memory statistics.
  this->local_cb_.sh_ =
    this->shared_cb_->sh_;
#if (ACE_HAS_POSITION_INDEPENDENT_POINTERS == 1)
  ACE_BASED_POINTER_REPOSITORY::instance ()->bind
    (this->local_cb_.mapped_base_,
     this->local_cb_.sh_.mapped_size_);
#endif /* ACE_HAS_POSITION_INDEPENDENT_POINTERS == 1 */

  return 0;
}

#endif /* ACE_WIN32 */

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Auto_Basic_Array_Ptr<char>;
template class ACE_Unbounded_Set<char *>;
template class ACE_Unbounded_Set_Iterator<char *>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Auto_Basic_Array_Ptr<char>
#pragma instantiate ACE_Unbounded_Set<char *>
#pragma instantiate ACE_Unbounded_Set_Iterator<char *>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
