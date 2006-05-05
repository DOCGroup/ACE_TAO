// $Id$

// MMAP_Memory_Pool.cpp
#include "ace/MMAP_Memory_Pool.h"
#include "ace/OS_NS_sys_mman.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_sys_stat.h"
#include "ace/Log_Msg.h"

#if (ACE_HAS_POSITION_INDEPENDENT_POINTERS == 1)
#include "ace/Based_Pointer_T.h"
#include "ace/Based_Pointer_Repository.h"
#endif /* ACE_HAS_POSITION_INDEPENDENT_POINTERS == 1  */

#if !defined (__ACE_INLINE__)
#include "ace/MMAP_Memory_Pool.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ace,
          MMAP_Memory_Pool,
          "$Id$")

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

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
          this->base_addr_ = const_cast<void *> (options->base_addr_);
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
      if (ACE::get_temp_dir (this->backing_store_name_,
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

      // If requested an unique filename, use mktemp to get a random file.
      if (options->unique_)
        ACE_OS::mktemp(this->backing_store_name_);

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

ACE_MMAP_Memory_Pool::~ACE_MMAP_Memory_Pool (void)
{
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
                                static_cast<off_t> (seek_len - 1),
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

#if defined (ACE_OPENVMS)
  ::fsync(this->mmap_.handle());
#endif

  // Increment by one to put us at the beginning of the next chunk...
  ++map_size;
#endif /* CHORUS */
  return 0;
}

// Memory map the file up to <map_size> bytes.

int
ACE_MMAP_Memory_Pool::map_file (off_t map_size)
{
  ACE_TRACE ("ACE_MMAP_Memory_Pool::map_file");
#if (ACE_HAS_POSITION_INDEPENDENT_POINTERS == 1)
    void* obase_addr = this->base_addr_;
#endif /* ACE_HAS_POSITION_INDEPENDENT_POINTERS == 1 */

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
      if(obase_addr && this->base_addr_ != obase_addr)
         ACE_BASED_POINTER_REPOSITORY::instance ()->unbind (obase_addr);
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
                           ACE_LIB_TEXT ("MMAP_Memory_Pool::init_acquire, EEXIST")),
                          0);

#if (ACE_HAS_POSITION_INDEPENDENT_POINTERS == 1)
      // Update the mapped segment information
      ACE_BASED_POINTER_REPOSITORY::instance ()->bind (this->mmap_.addr(),
                                                       this->mmap_.size());
#endif /* ACE_HAS_POSITION_INDEPENDENT_POINTERS == 1 */

      return this->mmap_.addr ();
    }
  else
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_LIB_TEXT ("%p\n"),
                       ACE_LIB_TEXT ("MMAP_Memory_Pool::init_acquire")),
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
                                                            mode_t file_mode,
                                                            bool unique)
  : base_addr_ (base_addr),
    use_fixed_addr_ (use_fixed_addr),
    write_each_page_ (write_each_page),
    minimum_bytes_ (minimum_bytes),
    flags_ (flags),
    guess_on_fault_ (guess_on_fault),
    sa_ (sa),
    file_mode_ (file_mode),
    unique_ (unique)
{
  ACE_TRACE ("ACE_MMAP_Memory_Pool_Options::ACE_MMAP_Memory_Pool_Options");
  // for backwards compatability
  if (base_addr_ == 0 && use_fixed_addr_ == ALWAYS_FIXED)
    use_fixed_addr_ = FIRSTCALL_FIXED;
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
      off_t const current_map_size = ACE_OS::filesize (this->mmap_.handle ());

      if (static_cast<size_t> (current_map_size) == this->mmap_.size ())
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

void *
ACE_MMAP_Memory_Pool::base_addr (void) const
{
  ACE_TRACE ("ACE_MMAP_Memory_Pool::base_addr");
  return this->base_addr_;
}

size_t
ACE_MMAP_Memory_Pool::round_up (size_t nbytes)
{
  ACE_TRACE ("ACE_MMAP_Memory_Pool::round_up");
  return ACE::round_to_pagesize (static_cast<off_t> (nbytes));
}

ACE_ALLOC_HOOK_DEFINE(ACE_Lite_MMAP_Memory_Pool)

ACE_Lite_MMAP_Memory_Pool::ACE_Lite_MMAP_Memory_Pool (const ACE_TCHAR *backing_store_name,
                                                      const OPTIONS *options)
  : ACE_MMAP_Memory_Pool (backing_store_name, options)
{
  ACE_TRACE ("ACE_Lite_MMAP_Memory_Pool::ACE_Lite_MMAP_Memory_Pool");
}

ACE_Lite_MMAP_Memory_Pool::~ACE_Lite_MMAP_Memory_Pool (void)
{
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

ACE_END_VERSIONED_NAMESPACE_DECL
