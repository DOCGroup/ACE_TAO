
// $Id$

// Memory_Pool.cpp
#define ACE_BUILD_DLL
#include "ace/Log_Msg.h"
#include "ace/Memory_Pool.h"

#if !defined (__ACE_INLINE__)
#include "ace/Memory_Pool.i"
#endif /* __ACE_INLINE__ */

ACE_ALLOC_HOOK_DEFINE(ACE_Local_Memory_Pool)

void
ACE_Local_Memory_Pool::dump (void) const
{
  ACE_TRACE ("ACE_Local_Memory_Pool::dump");
}

ACE_Local_Memory_Pool::ACE_Local_Memory_Pool (const char *,
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
  // ACE_DEBUG ((LM_DEBUG, "(%P|%t) acquiring more chunks, nbytes = %d, rounded_bytes = %d\n", nbytes, rounded_bytes));

  void *cp = (void *) new char[rounded_bytes];

  if (cp == 0)
    ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) %u\n", cp), 0);
  else
    // ACE_DEBUG ((LM_DEBUG, "(%P|%t) acquired more chunks, nbytes = %d, rounded_bytes = %d, new break = %d\n", nbytes, rounded_bytes, cp));
  return cp;
}

ACE_ALLOC_HOOK_DEFINE(ACE_MMAP_Memory_Pool)

void
ACE_MMAP_Memory_Pool::dump (void) const
{
  ACE_TRACE ("ACE_MMAP_Memory_Pool::dump");
}

int
ACE_MMAP_Memory_Pool::release (void)
{
  ACE_TRACE ("ACE_MMAP_Memory_Pool::release");
  this->mmap_.remove ();
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

ACE_MMAP_Memory_Pool::ACE_MMAP_Memory_Pool (const char *pool_name,
					    const OPTIONS *options)
  : base_addr_ (0),
    flags_ (MAP_SHARED),
    write_each_page_ (0)
{
  ACE_TRACE ("ACE_MMAP_Memory_Pool::ACE_MMAP_Memory_Pool");

  // Only change the defaults if <options> != 0.
  if (options)
    {
      if (options->use_fixed_addr_)
	{
	  this->base_addr_ = (void *) options->base_addr_;
	  ACE_SET_BITS (flags_, MAP_FIXED);
	}
      this->write_each_page_ = options->write_each_page_;
    }

  if (pool_name == 0)
    // Only create a new unique filename for the backing store file
    // if the user didn't supply one...
    pool_name = ACE_DEFAULT_BACKING_STORE; // from "ace/OS.h"

  ACE_OS::strncpy (this->backing_store_, pool_name, 
		   sizeof this->backing_store_);

  if (this->signal_handler_.register_handler (SIGSEGV, this) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", this->backing_store_));
}

// Compute the new file_offset of the backing store and commit the
// memory.
int
ACE_MMAP_Memory_Pool::commit_backing_store (size_t rounded_bytes,
					    off_t &file_offset)
{
  ACE_TRACE ("ACE_MMAP_Memory_Pool::commit_backing_store");

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
      file_offset = ACE_OS::lseek (this->mmap_.handle () , seek_len - 1, SEEK_END);

      if (file_offset == -1 || ACE_OS::write (this->mmap_.handle (), "", 1) == -1) 
	ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) %p\n", this->backing_store_), -1);
    }

  // Increment by one to put us at the beginning of the next chunk...
  file_offset++;
  return 0;
}

// Memory map the file up to <file_offset> bytes.

int
ACE_MMAP_Memory_Pool::map_file (off_t file_offset)
{
  ACE_TRACE ("ACE_MMAP_Memory_Pool::map_file");

  // Unmap the existing mapping.
  this->mmap_.unmap ();

  // Remap the file.
  if (this->mmap_.map (file_offset, PROT_RDWR,
		       this->flags_, this->base_addr_, 0) == -1
      || this->base_addr_ != 0 && this->mmap_.addr () != this->base_addr_)
    ACE_ERROR_RETURN ((LM_ERROR, 
		       "(%P|%t) base_addr = %u, addr = %u, file_offset = %u, %p\n", 
		      this->mmap_.addr (), this->base_addr_, 
		       file_offset, this->backing_store_), -1);

  return 0;
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

  off_t file_offset;

  if (this->commit_backing_store (rounded_bytes, file_offset) == -1)
    return 0;

  if (this->map_file (file_offset) == -1)
    return 0;

  // ACE_DEBUG ((LM_DEBUG, "(%P|%t) acquired more chunks, nbytes = %d,
  // rounded_bytes = %d, file_offset = %d\n", nbytes, rounded_bytes,
  // file_offset));

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

  if (this->mmap_.open (this->backing_store_, 
			O_RDWR | O_CREAT | O_TRUNC | O_EXCL, 
			ACE_DEFAULT_PERMS) != -1)
    {
      // First time in, so need to acquire memory. 
      first_time = 1;
      return this->acquire (nbytes, rounded_bytes);
    }
  else if (errno == EEXIST)
    {
      errno = 0;
      // Reopen file *without* using O_EXCL...
      if (this->mmap_.map (this->backing_store_,
			   -1,
			   O_RDWR,
			   ACE_DEFAULT_PERMS,
			   PROT_RDWR,
			   this->flags_,
			   this->base_addr_) == -1)
	ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "open"), 0);

      return this->mmap_.addr ();
    }
  else
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "open"), 0);
}

int 
ACE_MMAP_Memory_Pool::remap (void *addr)
{
  ACE_TRACE ("ACE_MMAP_Memory_Pool::remap");
  ACE_DEBUG ((LM_DEBUG, "Remapping with fault address at: %X\n", addr));
  off_t current_file_offset = ACE_OS::filesize (this->mmap_.handle ()); 
  // ACE_OS::lseek (this->mmap_.handle (), 0, SEEK_END);

  if (addr != 0
      && !(addr < (void *) ((char *) this->mmap_.addr () + current_file_offset)
	   && addr >= this->mmap_.addr ()))
    return -1;
  
  // Extend the mapping to cover the size of the backing store. 
  return this->map_file (current_file_offset);
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
  ACE_TRACE ("ACE_MMAP_Memory_Pool::handle_signal");

  if (signum != SIGSEGV)
    ACE_ERROR_RETURN ((LM_ERROR, 
		       "(%P|%t) ignoring signal %S\n", 
		       signum), -1);
  else
    ; // ACE_DEBUG ((LM_DEBUG, "(%P|%t) received %S\n", signum));

  // ACE_DEBUG ((LM_DEBUG, "(%P|%t) new mapping address = %u\n", (char *) this->base_addr_ + current_file_offset));

#if defined (ACE_HAS_SIGINFO_T) && !defined (ACE_LACKS_SI_ADDR)
  // Make sure that the pointer causing the problem is within the
  // range of the backing store.

  if (siginfo != 0)
    {
      // ACE_DEBUG ((LM_DEBUG, "(%P|%t) si_signo = %d, si_code = %d, addr = %u\n", siginfo->si_signo, siginfo->si_code, siginfo->si_addr));
      if (this->remap ((void *) siginfo->si_addr) == -1)
	ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) address %u out of range\n", 
			   siginfo->si_addr), -1);
      return 0;
    }
#endif /* ACE_HAS_SIGINFO_T && !defined ACE_LACKS_SI_ADDR */
  // This is total desperation since we don't know what the faulting
  // address is in this case!  
  this->remap (0);
  return 0;
}

ACE_ALLOC_HOOK_DEFINE(ACE_Lite_MMAP_Memory_Pool)

ACE_Lite_MMAP_Memory_Pool::ACE_Lite_MMAP_Memory_Pool (const char *pool_name,
						      const OPTIONS *options)
  : ACE_MMAP_Memory_Pool (pool_name, options)
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
  // ACE_DEBUG ((LM_DEBUG, "(%P|%t) acquiring more chunks, nbytes = %d, rounded_bytes = %d\n", nbytes, rounded_bytes));
  void *cp = ACE_OS::sbrk (rounded_bytes);

  if (cp == MAP_FAILED)
    ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) cp = %u\n", cp), 0);
  else
    // ACE_DEBUG ((LM_DEBUG, "(%P|%t) acquired more chunks, nbytes = %d, rounded_bytes = %d, new break = %u\n", nbytes, rounded_bytes, cp));
  return cp;
}

void
ACE_Sbrk_Memory_Pool::dump (void) const
{
  ACE_TRACE ("ACE_Sbrk_Memory_Pool::dump");
}

ACE_Sbrk_Memory_Pool::ACE_Sbrk_Memory_Pool (const char *,
					    const OPTIONS *)
{
  ACE_TRACE ("ACE_Sbrk_Memory_Pool::ACE_Sbrk_Memory_Pool");
}
#endif /* !ACE_LACKS_SBRK */

#if !defined (ACE_LACKS_SYSV_SHMEM)
ACE_ALLOC_HOOK_DEFINE(ACE_Shared_Memory_Pool)

void
ACE_Shared_Memory_Pool::dump (void) const
{
  ACE_TRACE ("ACE_Shared_Memory_Pool::dump");
}

int 
ACE_Shared_Memory_Pool::in_use (off_t &offset, 
				int &counter)
{
  offset = 0;
  SHM_TABLE *st = (SHM_TABLE *) ACE_DEFAULT_BASE_ADDR;
  shmid_ds buf;

  for (counter = 0; 
       counter < ACE_DEFAULT_MAX_SEGMENTS
	 && st[counter].used == 1;
       counter++)
    {
      if (ACE_OS::shmctl (st[counter].shmid, IPC_STAT, &buf) == -1)
	ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) %p\n", "shmctl"), -1);
      offset += buf.shm_segsz;
      // ACE_DEBUG ((LM_DEBUG, "(%P|%t) segment size = %d, offset = %d\n", buf.shm_segsz, offset));
    }

  return 0;
}

int
ACE_Shared_Memory_Pool::commit_backing_store (size_t rounded_bytes,
					      off_t &offset)
{
  ACE_TRACE ("ACE_Shared_Memory_Pool::update");
  int counter;
  SHM_TABLE *st = (SHM_TABLE *) ACE_DEFAULT_BASE_ADDR;

  if (this->in_use (offset, counter) == -1)
    return -1;

  if (counter == ACE_DEFAULT_MAX_SEGMENTS)
    ACE_ERROR_RETURN ((LM_ERROR, 
		      "exceeded max number of segments = %d, base = %u, offset = %u\n",
		       counter, ACE_DEFAULT_BASE_ADDR, offset), -1);
  else
    {
      int shmid = ACE_OS::shmget (st[counter].key,
			    rounded_bytes,
			    ACE_DEFAULT_PERMS | IPC_CREAT | IPC_EXCL);
      if (shmid == -1)
	ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) %p\n", "shmget"), 0);

      st[counter].shmid = shmid;
      st[counter].used = 1;

      void *address = (void *) (ACE_DEFAULT_BASE_ADDR + offset);
      void *shmem = ACE_OS::shmat (st[counter].shmid, (char *) address, 0);

      if (shmem != address)
	ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) %p, shmem = %u, address = %u\n", 
			   "shmat", shmem, address), 0);
    }

  return 0;
}

// Handle SIGSEGV and SIGBUS signals to remap shared memory properly.

int
ACE_Shared_Memory_Pool::handle_signal (int , siginfo_t *siginfo, ucontext_t *)
{
  ACE_TRACE ("ACE_Shared_Memory_Pool::handle_signal");
  // ACE_DEBUG ((LM_DEBUG, "signal %S occurred\n", signum)); 
  off_t offset;

#if defined (ACE_HAS_SIGINFO_T) && !defined (ACE_LACKS_SI_ADDR)
  // Make sure that the pointer causing the problem is within the
  // range of the backing store.

  if (siginfo != 0)
    {
      // ACE_DEBUG ((LM_DEBUG, "(%P|%t) si_signo = %d, si_code = %d, addr = %u\n", siginfo->si_signo, siginfo->si_code, siginfo->si_addr));
      int counter;
      if (this->in_use (offset, counter) == -1)
	ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n", "in_use"));
      else if (!(siginfo->si_code == SEGV_MAPERR
	   && siginfo->si_addr < (((char *) ACE_DEFAULT_BASE_ADDR) + offset)
	   && siginfo->si_addr >= ((char *) ACE_DEFAULT_BASE_ADDR)))
	ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) address %u out of range\n", 
			   siginfo->si_addr), -1);
    }
#endif /* ACE_HAS_SIGINFO_T && !defined (ACE_LACKS_SI_ADDR) */
  this->commit_backing_store (this->round_up (ACE_DEFAULT_SEGMENT_SIZE), 
			      offset);
  return 0;
}

ACE_Shared_Memory_Pool::ACE_Shared_Memory_Pool (const char *,
						const OPTIONS *)
{
  ACE_TRACE ("ACE_Shared_Memory_Pool::ACE_Shared_Memory_Pool");

  if (this->signal_handler_.register_handler (SIGSEGV, this) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "ACE_Sig_Handler::register_handler"));
}

// Ask system for more shared memory.

void *
ACE_Shared_Memory_Pool::acquire (size_t nbytes, 
				 size_t &rounded_bytes)
{
  ACE_TRACE ("ACE_Shared_Memory_Pool::acquire");

  rounded_bytes = this->round_up (nbytes);

  // ACE_DEBUG ((LM_DEBUG, "(%P|%t) acquiring more chunks, nbytes = %d, rounded_bytes = %d\n", nbytes, rounded_bytes));

  off_t offset;

  if (this->commit_backing_store (rounded_bytes, offset) == -1)
    return 0;

  // ACE_DEBUG ((LM_DEBUG, "(%P|%t) acquired more chunks, nbytes = %d, rounded_bytes = %d\n", nbytes, rounded_bytes));
  return ((char *) ACE_DEFAULT_BASE_ADDR) + offset;
}
 
// Ask system for initial chunk of shared memory.

void *
ACE_Shared_Memory_Pool::init_acquire (size_t nbytes, 
				      size_t &rounded_bytes, 
				      int &first_time)
{  
  ACE_TRACE ("ACE_Shared_Memory_Pool::init_acquire");

  int counter;
  void *base_addr = (void *) ACE_DEFAULT_BASE_ADDR;
  off_t shm_table_offset = ACE::round_to_pagesize (sizeof (SHM_TABLE));
  rounded_bytes = this->round_up (nbytes);

  // Acquire the semaphore to serialize initialization and prevent
  // race conditions.

  int shmid = ACE_OS::shmget (ACE_DEFAULT_SHM_KEY,
			      rounded_bytes + shm_table_offset,
			      ACE_DEFAULT_PERMS | IPC_CREAT | IPC_EXCL);
  if (shmid == -1)
    {
      if (errno != EEXIST)
	ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) %p\n", "shmget"), 0);

      first_time = 0;

      shmid = ACE_OS::shmget (ACE_DEFAULT_SHM_KEY, 0, 0);

      if (shmid == -1)
	ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) %p\n", "shmget"), 0);

      void *shmem = ACE_OS::shmat (shmid, (char *) base_addr, 0);

      if (shmem != base_addr)
	ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) %p, shmem = %u, base_addr = %u\n", 
			   "shmat", shmem, base_addr), 0);
    }
  else 
    {
      first_time = 1;

      void *shmem = ACE_OS::shmat (shmid, (char *) base_addr, 0);

      if (shmem != base_addr)
	ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) %p, shmem = %u, base_addr = %u\n", 
			   "shmat", shmem, base_addr), 0);

      SHM_TABLE *st = (SHM_TABLE *) base_addr;

      st[0].key = ACE_DEFAULT_SHM_KEY;
      st[0].shmid = shmid;
      st[0].used = 1;

      for (counter = 1; // Skip over the first entry...
	   counter < ACE_DEFAULT_MAX_SEGMENTS;
	   counter++)
	{
	  st[counter].key = ACE_DEFAULT_SHM_KEY + counter;
	  st[counter].shmid = 0;
	  st[counter].used = 0;
	}
    }

  return (void *) (((char *) base_addr) + shm_table_offset);
}

// Instruct the memory pool to release all of its resources.

int
ACE_Shared_Memory_Pool::release (void)
{
  ACE_TRACE ("ACE_Shared_Memory_Pool::release");

  int result = 0;
  SHM_TABLE *st = (SHM_TABLE *) ACE_DEFAULT_BASE_ADDR;

  for (int counter = 0; 
       counter < ACE_DEFAULT_MAX_SEGMENTS && st[counter].used == 1; 
       counter++)
    if (ACE_OS::shmctl (st[counter].shmid, IPC_RMID, NULL) == -1)
      result = -1;

  return result;
}
#endif /* !ACE_LACKS_SYSV_SHMEM */
