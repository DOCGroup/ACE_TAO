/* -*- C++ -*- */
// $Id$


// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//     ACE_Memory_Pool.h
//
// = AUTHOR
//    Doug Schmidt and Prashant Jain
// 
// ============================================================================

#if !defined (ACE_MEMORY_POOL_H)
#define ACE_MEMORY_POOL_H

#include "ace/ACE.h"
#include "ace/Event_Handler.h"
#include "ace/Signal.h"
#include "ace/Mem_Map.h"
#if !defined (ACE_WIN32)
#include "ace/SV_Semaphore_Complex.h"
#endif /* !ACE_WIN32 */

#if !defined (ACE_LACKS_SBRK)
class ACE_Export ACE_Sbrk_Memory_Pool_Options
  // = TITLE
  //     Helper class for constructor options.
  //
  // = DESCRIPTION
  //     This should be a nested class, but that breaks too many 
  //     compilers.
{};

class ACE_Export ACE_Sbrk_Memory_Pool
  // = TITLE
  //     Make a memory pool that is based on <sbrk(2)>.
{
public:
  typedef ACE_Sbrk_Memory_Pool_Options OPTIONS;

  ACE_Sbrk_Memory_Pool (const char *pool_name = 0, 
			const OPTIONS *options = 0);
  // Initialize the pool.

  // = Implementor operations.
  virtual void *init_acquire (size_t nbytes, 
			      size_t &rounded_bytes, 
			      int &first_time);
  // Ask system for initial chunk of local memory. 

  virtual void *acquire (size_t nbytes, 
			 size_t &rounded_bytes);
  // Acquire at least NBYTES from the memory pool.  ROUNDED_BYTES is
  // the actual number of bytes allocated.

  virtual int release (void);
  // Instruct the memory pool to release all of its resources.

  virtual int sync (ssize_t len = -1, int flags = MS_SYNC);
  // Sync <len> bytes of the memory region to the backing store
  // starting at <this->base_addr_>.  If <len> == -1 then sync the
  // whole region.

  virtual int sync (void *addr, size_t len, int flags = MS_SYNC);
  // Sync <len> bytes of the memory region to the backing store
  // starting at <addr_>.

  virtual int protect (ssize_t len = -1, int prot = PROT_RDWR);
  // Change the protection of the pages of the mapped region to <prot>
  // starting at <this->base_addr_> up to <len> bytes.  If <len> == -1
  // then change protection of all pages in the mapped region.

  virtual int protect (void *addr, size_t len, int prot = PROT_RDWR);
  // Change the protection of the pages of the mapped region to <prot>
  // starting at <addr> up to <len> bytes.

  virtual void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  virtual size_t round_up (size_t nbytes);
  // Implement the algorithm for rounding up the request to an
  // appropriate chunksize.
};
#endif /* !ACE_LACKS_SBRK */

#if !defined (ACE_LACKS_SYSV_SHMEM)

class ACE_Export ACE_Shared_Memory_Pool_Options
  // = TITLE
  //     Helper class for constructor options.
  //
  // = DESCRIPTION
  //     This should be a nested class, but that breaks too many 
  //     compilers.
{};

class ACE_Export ACE_Shared_Memory_Pool : public ACE_Event_Handler
  // = TITLE
  //     Make a memory pool that is based on System V shared memory
  //     (shmget(2) etc.).  This implementation allows memory to be
  //     shared between processes.
{
public:
  typedef ACE_Shared_Memory_Pool_Options OPTIONS;

  ACE_Shared_Memory_Pool (const char *pool_name = ACE_ITOA (ACE_DEFAULT_SHM_KEY),
			  const OPTIONS *options = 0);
  // Initialize the pool.

  virtual void *init_acquire (size_t nbytes, 
			      size_t &rounded_bytes, 
			      int &first_time);
  // Ask system for initial chunk of local memory.

  virtual void *acquire (size_t nbytes, 
			 size_t &rounded_bytes);
  // Acquire at least NBYTES from the memory pool.  ROUNDED_BYTES is
  // the actual number of bytes allocated.  Also acquires an internal
  // semaphore that ensures proper serialization of Memory_Pool
  // initialization across processes.

  virtual int release (void);
  // Instruct the memory pool to release all of its resources.

  virtual int sync (ssize_t len = -1, int flags = MS_SYNC);
  // Sync the memory region to the backing store starting at
  // <this->base_addr_>.

  virtual int sync (void *addr, size_t len, int flags = MS_SYNC);
  // Sync the memory region to the backing store starting at <addr_>.

  virtual int protect (ssize_t len = -1, int prot = PROT_RDWR);
  // Change the protection of the pages of the mapped region to <prot>
  // starting at <this->base_addr_> up to <len> bytes.  If <len> == -1
  // then change protection of all pages in the mapped region.

  virtual int protect (void *addr, size_t len, int prot = PROT_RDWR);
  // Change the protection of the pages of the mapped region to <prot>
  // starting at <addr> up to <len> bytes.

  virtual void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  virtual size_t round_up (size_t nbytes);
  // Implement the algorithm for rounding up the request to an
  // appropriate chunksize.

  virtual int commit_backing_store (size_t rounded_bytes, 
				    off_t &offset);
  // Commits a new shared memory segment if necessary after an
  // acquire() or a signal.  <offset> is set to the new offset into
  // the backing store.

  struct SHM_TABLE 
  {
    key_t key;
    int shmid;
    int used;
  };

  virtual int in_use (off_t &offset, int &counter);
  // Determine how much memory is currently in use.

  ACE_Sig_Handler signal_handler_;
  // Handles SIGSEGV.

  virtual int handle_signal (int signum, siginfo_t *, ucontext_t *);
  // Handle SIGSEGV and SIGBUS signals to remap shared memory
  // properly.

  ACE_SV_Semaphore_Complex init_finished_;
  // Used to serialize initialization of the Memory_Pool and Malloc.
};
#endif /* !ACE_LACKS_SYSV_SHMEM */

class ACE_Export ACE_Local_Memory_Pool_Options
  // = TITLE
  //     Helper class for constructor options.
  //
  // = DESCRIPTION
  //     This should be a nested class, but that breaks too many 
  //     compilers.
{};

class ACE_Export ACE_Local_Memory_Pool
  // = TITLE
  //   Make a memory pool that is based on C++ new/delete.  This is
  //   useful for integrating existing components that use new/delete
  //   into the ACE Malloc scheme...
{
public:
  typedef ACE_Local_Memory_Pool_Options OPTIONS;

  ACE_Local_Memory_Pool (const char *pool_name = 0,
			 const OPTIONS *options = 0);
  // Initialize the pool.

  virtual void *init_acquire (size_t nbytes, 
			      size_t &rounded_bytes, 
			      int &first_time);
  // Ask system for initial chunk of local memory. 

  virtual void *acquire (size_t nbytes, 
			 size_t &rounded_bytes);
  // Acquire at least NBYTES from the memory pool.  ROUNDED_BYTES is
  // the actual number of bytes allocated.

  virtual int release (void);
  // Instruct the memory pool to release all of its resources. 

  virtual int sync (ssize_t len = -1, int flags = MS_SYNC);
  // Sync <len> bytes of the memory region to the backing store
  // starting at <this->base_addr_>.  If <len> == -1 then sync the
  // whole region.

  virtual int sync (void *addr, size_t len, int flags = MS_SYNC);
  // Sync <len> bytes of the memory region to the backing store
  // starting at <addr_>.

  virtual int protect (ssize_t len = -1, int prot = PROT_RDWR);
  // Change the protection of the pages of the mapped region to <prot>
  // starting at <this->base_addr_> up to <len> bytes.  If <len> == -1
  // then change protection of all pages in the mapped region.

  virtual int protect (void *addr, size_t len, int prot = PROT_RDWR);
  // Change the protection of the pages of the mapped region to <prot>
  // starting at <addr> up to <len> bytes.

  virtual void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  virtual size_t round_up (size_t nbytes);

  // Implement the algorithm for rounding up the request to an
  // appropriate chunksize.
};

class ACE_Export ACE_MMAP_Memory_Pool_Options
  // = TITLE
  //     Helper class for constructor options.
  //
  // = DESCRIPTION
  //     This should be a nested class, but that breaks too many 
  //     compilers.
{
public:
  ACE_MMAP_Memory_Pool_Options (const char *base_addr = ACE_DEFAULT_BASE_ADDR,
				int use_fixed_addr = 1,
				int write_each_page = 1)
    : base_addr_ (base_addr),
      use_fixed_addr_ (use_fixed_addr),
      write_each_page_ (write_each_page) {}

  const char *base_addr_;
  int use_fixed_addr_;
  int write_each_page_;
};

class ACE_Export ACE_MMAP_Memory_Pool : public ACE_Event_Handler
  // = TITLE
  //     Make a memory pool that is based on <mmap(2)>.  This
  //     implementation allows memory to be shared between processes.
{
public:
  typedef ACE_MMAP_Memory_Pool_Options OPTIONS;

  // = Initialization and termination methods.

  ACE_MMAP_Memory_Pool (const char *pool_name = 0,
			const OPTIONS *options = 0);
  // Initialize the pool.

  virtual void *init_acquire (size_t nbytes, 
			      size_t &rounded_bytes, 
			      int &first_time);
  // Ask system for initial chunk of shared memory. 

  virtual void *acquire (size_t nbytes, 
			 size_t &rounded_bytes);
  // Acquire at least <nbytes> from the memory pool.  <rounded_bytes>
  // is the actual number of bytes allocated.  Also acquires an
  // internal semaphore that ensures proper serialization of
  // <ACE_MMAP_Memory_Pool> initialization across processes.

  virtual int release (void);
  // Instruct the memory pool to release all of its resources. 

  virtual int sync (ssize_t len = -1, int flags = MS_SYNC);
  // Sync the memory region to the backing store starting at
  // <this->base_addr_>.

  virtual int sync (void *addr, size_t len, int flags = MS_SYNC);
  // Sync the memory region to the backing store starting at <addr_>.

  virtual int protect (ssize_t len = -1, int prot = PROT_RDWR);
  // Change the protection of the pages of the mapped region to <prot>
  // starting at <this->base_addr_> up to <len> bytes.  If <len> == -1
  // then change protection of all pages in the mapped region.

  virtual int protect (void *addr, size_t len, int prot = PROT_RDWR);
  // Change the protection of the pages of the mapped region to <prot>
  // starting at <addr> up to <len> bytes.

  virtual int remap (void *addr);
  // Try to extend the virtual address space so that <addr> is now
  // covered by the address mapping.  The method succeeds and returns
  // 0 if the backing store has adequate memory to cover this address.
  // Otherwise, it returns -1.  This method is typically called by a
  // UNIX signal handler for SIGSEGV or a Win32 structured exception
  // when another process has grown the backing store (and its
  // mapping) and our process now incurs a fault because our mapping
  // isn't in range (yet).

  virtual void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  // = Implement the algorithm for rounding up the request to an
  // appropriate chunksize.

  virtual size_t round_up (size_t nbytes);

  virtual int commit_backing_store (size_t rounded_bytes, off_t &file_offset);
  // Compute the new file_offset of the backing store and commit the
  // memory.

  virtual int map_file (off_t file_offset);
  // Memory map the file up to <file_offset> bytes.

  virtual int handle_signal (int signum, siginfo_t *, ucontext_t *);
  // Handle SIGSEGV and SIGBUS signals to remap shared memory
  // properly.

  ACE_Sig_Handler signal_handler_;
  // Handles SIGSEGV.

  ACE_Mem_Map mmap_;
  // Memory-mapping object.

  void *base_addr_;
  // Base of mapped region.  If this has the value of 0 then the OS is
  // free to select any address to map the file, otherwise this value
  // is what the OS must try to use to mmap the file.

  int flags_;
  // Flags passed into <ACE_OS::mmap>.
  
  int write_each_page_;
  // Should we write a byte to each page to forceably allocate memory
  // for this backing store?

  char backing_store_[MAXPATHLEN];
  // Name of the backing store where the shared memory is kept.
};

class ACE_Export ACE_Lite_MMAP_Memory_Pool : public ACE_MMAP_Memory_Pool
  // = TITLE
  //     Make a ``lighter-weight'' memory pool based <ACE_Mem_Map>.  
  // 
  // = DESCRIPTION
  //     This implementation allows memory to be shared between
  //     processes.  However, unlike the <ACE_MMAP_Memory_Pool> 
  //     the sync() methods are no-ops, which means that we don't pay
  //     for the price of flushing the memory to the backing store on
  //     every update.  Naturally, this trades off increased
  //     performance for less reliability if the machine crashes.
{
public:
  // = Initialization and termination methods.

  ACE_Lite_MMAP_Memory_Pool (const char *pool_name = 0,
			     const OPTIONS *options = 0);
  // Initialize the pool.

  int sync (ssize_t len = -1, int flags = MS_SYNC);
  // Overwrite the default sync behavior with no-op

  int sync (void *addr, size_t len, int flags = MS_SYNC);
  // Overwrite the default sync behavior with no-op
};

#if defined (__ACE_INLINE__)
#include "ace/Memory_Pool.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_MEMORY_POOL_H */
