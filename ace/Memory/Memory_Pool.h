/* -*- C++ -*- */
//=============================================================================
/**
 *  @file     Memory_Pool.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt <schmidt@cs.wustl.edu> and Prashant Jain <pjain@cs.wustl.edu>
 */
//=============================================================================

#ifndef ACE_MEMORY_POOL_H
#define ACE_MEMORY_POOL_H
#include "ace/pre.h"

#include "ace/ACE.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Event_Handler/Event_Handler.h"
#include "ace/IPC/Signal.h"
#include "ace/Memory/Mem_Map.h"
#if !defined (ACE_WIN32)
#include "ace/IPC/SV_Semaphore_Complex.h"
#endif /* !ACE_WIN32 */

#include "ace/Utils/Unbounded_Set.h"

#if !defined (ACE_LACKS_SBRK)
/**
 * @class ACE_Sbrk_Memory_Pool_Options
 *
 * @brief Helper class for Sbrk Memory Pool constructor options.
 *
 * This should be a nested class, but that breaks too many
 * compilers.
 */
class ACE_Export ACE_Sbrk_Memory_Pool_Options
{
};

/**
 * @class ACE_Sbrk_Memory_Pool
 *
 * @brief Make a memory pool that is based on <sbrk(2)>.
 */
class ACE_Export ACE_Sbrk_Memory_Pool
{
public:
  typedef ACE_Sbrk_Memory_Pool_Options OPTIONS;

  /// Initialize the pool.
  ACE_Sbrk_Memory_Pool (const ACE_TCHAR *backing_store_name = 0,
                        const OPTIONS *options = 0);

  virtual ~ACE_Sbrk_Memory_Pool (void);

  // = Implementor operations.
  /// Ask system for initial chunk of local memory.
  virtual void *init_acquire (size_t nbytes,
                              size_t &rounded_bytes,
                              int &first_time);

  /// Acquire at least NBYTES from the memory pool.  ROUNDED_BYTES is
  /// the actual number of bytes allocated.
  virtual void *acquire (size_t nbytes,
                         size_t &rounded_bytes);

  /// Instruct the memory pool to release all of its resources.
  virtual int release (void);

  /**
   * Sync <len> bytes of the memory region to the backing store
   * starting at <this->base_addr_>.  If <len> == -1 then sync the
   * whole region.
   */
  virtual int sync (ssize_t len = -1, int flags = MS_SYNC);

  /// Sync <len> bytes of the memory region to the backing store
  /// starting at <addr_>.
  virtual int sync (void *addr, size_t len, int flags = MS_SYNC);

  /**
   * Change the protection of the pages of the mapped region to <prot>
   * starting at <this->base_addr_> up to <len> bytes.  If <len> == -1
   * then change protection of all pages in the mapped region.
   */
  virtual int protect (ssize_t len = -1, int prot = PROT_RDWR);

  /// Change the protection of the pages of the mapped region to <prot>
  /// starting at <addr> up to <len> bytes.
  virtual int protect (void *addr, size_t len, int prot = PROT_RDWR);

  /// Dump the state of an object.
  virtual void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

protected:
  /// Implement the algorithm for rounding up the request to an
  /// appropriate chunksize.
  virtual size_t round_up (size_t nbytes);
};
#endif /* !ACE_LACKS_SBRK */

#if !defined (ACE_LACKS_SYSV_SHMEM)

/**
 * @class ACE_Shared_Memory_Pool_Options
 *
 * @brief Helper class for Shared Memory Pool constructor options.
 *
 * This should be a nested class, but that breaks too many
 * compilers.
 */
class ACE_Export ACE_Shared_Memory_Pool_Options
{
public:
  // = Initialization method.
  ACE_Shared_Memory_Pool_Options (const char *base_addr = ACE_DEFAULT_BASE_ADDR,
                                  size_t max_segments = ACE_DEFAULT_MAX_SEGMENTS,
                                  size_t file_perms = ACE_DEFAULT_FILE_PERMS,
                                  off_t minimum_bytes = 0,
                                  size_t segment_size = ACE_DEFAULT_SEGMENT_SIZE);

  /// Base address of the memory-mapped backing store.
  const char *base_addr_;

  /// Number of shared memory segments to allocate.
  size_t max_segments_;

  /// What the minimum bytes of the initial segment should be.
  off_t minimum_bytes_;

  /// File permissions to use when creating/opening a segment.
  size_t file_perms_;

  /// Shared memory segment size.
  size_t segment_size_;
};

/**
 * @class ACE_Shared_Memory_Pool
 *
 * @brief Make a memory pool that is based on System V shared memory
 * (shmget(2) etc.).  This implementation allows memory to be
 * shared between processes.  If your platform doesn't support
 * System V shared memory (e.g., Win32 and many RTOS platforms
 * do not) then you should use ACE_MMAP_Memory_Pool instead of this
 * class.  In fact, you should probably use ACE_MMAP_Memory_Pool on
 * platforms that *do* support System V shared memory since it
 * provides more powerful features, such as persistent backing store
 * and greatly scalability.
 */
class ACE_Export ACE_Shared_Memory_Pool : public ACE_Event_Handler
{
public:
  typedef ACE_Shared_Memory_Pool_Options OPTIONS;

  /// Initialize the pool.
  ACE_Shared_Memory_Pool (const ACE_TCHAR *backing_store_name = 0,
                          const OPTIONS *options = 0);

  virtual ~ACE_Shared_Memory_Pool (void);

  /// Ask system for initial chunk of local memory.
  virtual void *init_acquire (size_t nbytes,
                              size_t &rounded_bytes,
                              int &first_time);

  /**
   * Acquire at least NBYTES from the memory pool.  ROUNDED_BYTES is
   * the actual number of bytes allocated.  Also acquires an internal
   * semaphore that ensures proper serialization of Memory_Pool
   * initialization across processes.
   */
  virtual void *acquire (size_t nbytes,
                         size_t &rounded_bytes);

  /// Instruct the memory pool to release all of its resources.
  virtual int release (void);

  /// Sync the memory region to the backing store starting at
  /// <this->base_addr_>.
  virtual int sync (ssize_t len = -1, int flags = MS_SYNC);

  /// Sync the memory region to the backing store starting at <addr_>.
  virtual int sync (void *addr, size_t len, int flags = MS_SYNC);

  /**
   * Change the protection of the pages of the mapped region to <prot>
   * starting at <this->base_addr_> up to <len> bytes.  If <len> == -1
   * then change protection of all pages in the mapped region.
   */
  virtual int protect (ssize_t len = -1, int prot = PROT_RDWR);

  /// Change the protection of the pages of the mapped region to <prot>
  /// starting at <addr> up to <len> bytes.
  virtual int protect (void *addr, size_t len, int prot = PROT_RDWR);

  /// Dump the state of an object.
  virtual void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

protected:
  /// Implement the algorithm for rounding up the request to an
  /// appropriate chunksize.
  virtual size_t round_up (size_t nbytes);

  /**
   * Commits a new shared memory segment if necessary after an
   * <acquire> or a signal.  <offset> is set to the new offset into
   * the backing store.
   */
  virtual int commit_backing_store_name (size_t rounded_bytes,
                                    off_t &offset);

  // = Keeps track of all the segments being used.
  struct SHM_TABLE
  {
    key_t key_;
    // Shared memory segment key.

    int shmid_;
    // Shared memory segment internal id.

    int used_;
    // Is the segment currently used.;
  };

  /**
   * Base address of the shared memory segment.  If this has the value
   * of 0 then the OS is free to select any address, otherwise this
   * value is what the OS must try to use to map the shared memory
   * segment.
   */
  void *base_addr_;

  /// File permissions to use when creating/opening a segment.
  size_t file_perms_;

  /// Number of shared memory segments in the <SHM_TABLE> table.
  size_t max_segments_;

  /// What the minimim bytes of the initial segment should be.
  off_t minimum_bytes_;

  /// Shared memory segment size.
  size_t segment_size_;

  /// Base shared memory key for the segment.
  key_t base_shm_key_;

  /// find the segment that contains the searchPtr
  virtual int find_seg (const void *const searchPtr,
                        off_t &offset,
                        size_t &counter);

  /// Determine how much memory is currently in use.
  virtual int in_use (off_t &offset,
                      size_t &counter);

  /// Handles SIGSEGV.
  ACE_Sig_Handler signal_handler_;

  /// Handle SIGSEGV and SIGBUS signals to remap shared memory
  /// properly.
  virtual int handle_signal (int signum, siginfo_t *, ucontext_t *);
};
#endif /* !ACE_LACKS_SYSV_SHMEM */

/**
 * @class ACE_Local_Memory_Pool_Options
 *
 * @brief Helper class for Local Memory Pool constructor options.
 *
 * This should be a nested class, but that breaks too many
 * compilers.
 */
class ACE_Export ACE_Local_Memory_Pool_Options
{
};

/**
 * @class ACE_Local_Memory_Pool
 *
 * @brief Make a memory pool that is based on C++ new/delete.  This is
 * useful for integrating existing components that use new/delete
 * into the ACE Malloc scheme...
 */
class ACE_Export ACE_Local_Memory_Pool
{
public:
  typedef ACE_Local_Memory_Pool_Options OPTIONS;

  /// Initialize the pool.
  ACE_Local_Memory_Pool (const ACE_TCHAR *backing_store_name = 0,
                         const OPTIONS *options = 0);

  virtual ~ACE_Local_Memory_Pool (void);

  /// Ask system for initial chunk of local memory.
  virtual void *init_acquire (size_t nbytes,
                              size_t &rounded_bytes,
                              int &first_time);

  /// Acquire at least NBYTES from the memory pool.  ROUNDED_BYTES is
  /// the actual number of bytes allocated.
  virtual void *acquire (size_t nbytes,
                         size_t &rounded_bytes);

  /// Instruct the memory pool to release all of its resources.
  virtual int release (void);

  /**
   * Sync <len> bytes of the memory region to the backing store
   * starting at <this->base_addr_>.  If <len> == -1 then sync the
   * whole region.
   */
  virtual int sync (ssize_t len = -1, int flags = MS_SYNC);

  /// Sync <len> bytes of the memory region to the backing store
  /// starting at <addr_>.
  virtual int sync (void *addr, size_t len, int flags = MS_SYNC);

  /**
   * Change the protection of the pages of the mapped region to <prot>
   * starting at <this->base_addr_> up to <len> bytes.  If <len> == -1
   * then change protection of all pages in the mapped region.
   */
  virtual int protect (ssize_t len = -1, int prot = PROT_RDWR);

  /// Change the protection of the pages of the mapped region to <prot>
  /// starting at <addr> up to <len> bytes.
  virtual int protect (void *addr, size_t len, int prot = PROT_RDWR);

#if defined (ACE_WIN32)
  /**
   * Win32 Structural exception selector.  The return value decides
   * how to handle memory pool related structural exceptions.  Returns
   * 1, 0, or , -1.
   */
  virtual int seh_selector (void *);
#endif /* ACE_WIN32 */

  /**
   * Try to extend the virtual address space so that <addr> is now
   * covered by the address mapping.  Always returns 0 since we can't
   * remap a local memory pool.
   */
  virtual int remap (void *addr);

  /// Dump the state of an object.
  virtual void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

protected:
  /// List of memory that we have allocated.
  ACE_Unbounded_Set<char *> allocated_chunks_;

  virtual size_t round_up (size_t nbytes);

  // Implement the algorithm for rounding up the request to an
  // appropriate chunksize.
};

/**
 * @class ACE_MMAP_Memory_Pool_Options
 *
 * @brief Helper class for MMAP Memory Pool constructor options.
 *
 * This should be a nested class, but that breaks too many
 * compilers.
 */
class ACE_Export ACE_MMAP_Memory_Pool_Options
{
public:
  // = Initialization method.
  ACE_MMAP_Memory_Pool_Options (const void *base_addr = ACE_DEFAULT_BASE_ADDR,
                                int use_fixed_addr = 1,
                                int write_each_page = 1,
                                off_t minimum_bytes = 0,
                                u_int flags = 0,
                                int guess_on_fault = 1,
                                LPSECURITY_ATTRIBUTES sa = 0);

  /// Base address of the memory-mapped backing store.
  const void *base_addr_;

  /// Must we use the <base_addr_> or can we let mmap(2) select it?
  int use_fixed_addr_;

  /// Should each page be written eagerly to avoid surprises later
  /// on?
  int write_each_page_;

  /// What the minimim bytes of the initial segment should be.
  off_t minimum_bytes_;

  /// Any special flags that need to be used for <mmap>.
  u_int flags_;

  /**
   * Try to remap without knowing the faulting address.  This
   * parameter is ignored on platforms that know the faulting address
   * (UNIX with SI_ADDR and Win32).
   */
  int guess_on_fault_;

  /// Pointer to a security attributes object.  Only used on NT.
  LPSECURITY_ATTRIBUTES sa_;

};

/**
 * @class ACE_MMAP_Memory_Pool
 *
 * @brief Make a memory pool that is based on <mmap(2)>.  This
 * implementation allows memory to be shared between processes.
 */
class ACE_Export ACE_MMAP_Memory_Pool : public ACE_Event_Handler
{
public:
  typedef ACE_MMAP_Memory_Pool_Options OPTIONS;

  // = Initialization and termination methods.

  /// Initialize the pool.
  ACE_MMAP_Memory_Pool (const ACE_TCHAR *backing_store_name = 0,
                        const OPTIONS *options = 0);

  virtual ~ACE_MMAP_Memory_Pool (void);

  /// Ask system for initial chunk of shared memory.
  virtual void *init_acquire (size_t nbytes,
                              size_t &rounded_bytes,
                              int &first_time);

  /**
   * Acquire at least <nbytes> from the memory pool.  <rounded_bytes>
   * is the actual number of bytes allocated.  Also acquires an
   * internal semaphore that ensures proper serialization of
   * <ACE_MMAP_Memory_Pool> initialization across processes.
   */
  virtual void *acquire (size_t nbytes,
                         size_t &rounded_bytes);

  /// Instruct the memory pool to release all of its resources.
  virtual int release (void);

  /// Sync the memory region to the backing store starting at
  /// <this->base_addr_>.
  virtual int sync (ssize_t len = -1, int flags = MS_SYNC);

  /// Sync the memory region to the backing store starting at <addr_>.
  virtual int sync (void *addr, size_t len, int flags = MS_SYNC);

  /**
   * Change the protection of the pages of the mapped region to <prot>
   * starting at <this->base_addr_> up to <len> bytes.  If <len> == -1
   * then change protection of all pages in the mapped region.
   */
  virtual int protect (ssize_t len = -1, int prot = PROT_RDWR);

  /// Change the protection of the pages of the mapped region to <prot>
  /// starting at <addr> up to <len> bytes.
  virtual int protect (void *addr, size_t len, int prot = PROT_RDWR);

#if defined (ACE_WIN32)
  /**
   * Win32 Structural exception selector.  The return value decides
   * how to handle memory pool related structural exceptions.  Returns
   * 1, 0, or , -1.
   */
  virtual int seh_selector (void *);
#endif /* ACE_WIN32 */

  /**
   * Try to extend the virtual address space so that <addr> is now
   * covered by the address mapping.  The method succeeds and returns
   * 0 if the backing store has adequate memory to cover this address.
   * Otherwise, it returns -1.  This method is typically called by a
   * UNIX signal handler for SIGSEGV or a Win32 structured exception
   * when another process has grown the backing store (and its
   * mapping) and our process now incurs a fault because our mapping
   * isn't in range (yet).
   */
  virtual int remap (void *addr);


  /// Return the base address of this memory pool.
  virtual void *base_addr (void) const;

  /// Dump the state of an object.
  virtual void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

protected:
  // = Implement the algorithm for rounding up the request to an
  // appropriate chunksize.

  virtual size_t round_up (size_t nbytes);

  /// Compute the new <map_size> of the backing store and commit the
  /// memory.
  virtual int commit_backing_store_name (size_t rounded_bytes,
                                         off_t &map_size);

  /// Memory map the file up to <map_size> bytes.
  virtual int map_file (off_t map_size);

  /// Handle SIGSEGV and SIGBUS signals to remap shared memory
  /// properly.
  virtual int handle_signal (int signum, siginfo_t *, ucontext_t *);

  /// Handles SIGSEGV.
  ACE_Sig_Handler signal_handler_;

  /// Memory-mapping object.
  ACE_Mem_Map mmap_;

  /**
   * Base of mapped region.  If this has the value of 0 then the OS is
   * free to select any address to map the file, otherwise this value
   * is what the OS must try to use to mmap the file.
   */
  void *base_addr_;

  /// Flags passed into <ACE_OS::mmap>.
  int flags_;

  /// Should we write a byte to each page to forceably allocate memory
  /// for this backing store?
  int write_each_page_;

  /// What the minimum bytes of the initial segment should be.
  off_t minimum_bytes_;

  /// Name of the backing store where the shared memory pool is kept.
  ACE_TCHAR backing_store_name_[MAXPATHLEN + 1];

  /**
   * Try to remap without knowing the faulting address.  This
   * parameter is ignored on platforms that know the faulting address
   * (UNIX with SI_ADDR and Win32).
   */
  int guess_on_fault_;

  /// Security attributes object, only used on NT.
  LPSECURITY_ATTRIBUTES sa_;

};

/**
 * @class ACE_Lite_MMAP_Memory_Pool
 *
 * @brief Make a ``lighter-weight'' memory pool based <ACE_Mem_Map>.
 *
 * This implementation allows memory to be shared between
 * processes.  However, unlike the <ACE_MMAP_Memory_Pool>
 * the <sync> methods are no-ops, which means that we don't pay
 * for the price of flushing the memory to the backing store on
 * every update.  Naturally, this trades off increased
 * performance for less reliability if the machine crashes.
 */
class ACE_Export ACE_Lite_MMAP_Memory_Pool : public ACE_MMAP_Memory_Pool
{
public:
  // = Initialization and termination methods.

  /// Initialize the pool.
  ACE_Lite_MMAP_Memory_Pool (const ACE_TCHAR *backing_store_name = 0,
                             const OPTIONS *options = 0);

  virtual ~ACE_Lite_MMAP_Memory_Pool (void);

  /// Overwrite the default sync behavior with no-op
  virtual int sync (ssize_t len = -1, int flags = MS_SYNC);

  /// Overwrite the default sync behavior with no-op
  virtual int sync (void *addr, size_t len, int flags = MS_SYNC);
};

#if defined (ACE_WIN32)

/**
 * @class ACE_Pagefile_Memory_Pool_Options
 *
 * @brief Helper class for Pagefile Memory Pool constructor options.
 *
 * This should be a nested class, but that breaks too many
 * compilers.
 */
class ACE_Export ACE_Pagefile_Memory_Pool_Options
{
public:
  // Initialization method.
  ACE_Pagefile_Memory_Pool_Options (void *base_addr = ACE_DEFAULT_PAGEFILE_POOL_BASE,
                                    size_t max_size = ACE_DEFAULT_PAGEFILE_POOL_SIZE);

  /// Base address of the memory-mapped backing store.
  void *base_addr_;

  /// Maximum size the pool may grow.
  size_t max_size_;
};

/**
 * @class ACE_Pagefile_Memory_Pool
 *
 * @brief Make a memory pool that is based on "anonymous" memory
 * regions allocated from the Win32 page file.
 */
class ACE_Export ACE_Pagefile_Memory_Pool
{
public:
  typedef ACE_Pagefile_Memory_Pool_Options OPTIONS;

  /// Initialize the pool.
  ACE_Pagefile_Memory_Pool (const ACE_TCHAR *backing_store_name = 0,
                            const OPTIONS *options = 0);

  /// Ask system for initial chunk of shared memory.
  void *init_acquire (size_t nbytes,
                      size_t &rounded_bytes,
                      int &first_time);

  /// Acquire at least <nbytes> from the memory pool.  <rounded_bytes>
  /// is the actual number of bytes allocated.
  void *acquire (size_t nbytes,
                 size_t &rounded_bytes);

  /// Instruct the memory pool to release all of its resources.
  int release (void);

  /**
   * Win32 Structural exception selector.  The return value decides
   * how to handle memory pool related structural exceptions.  Returns
   * 1, 0, or , -1.
   */
  virtual int seh_selector (void *);

  /**
   * Try to extend the virtual address space so that <addr> is now
   * covered by the address mapping.  The method succeeds and returns
   * 0 if the backing store has adequate memory to cover this address.
   * Otherwise, it returns -1.  This method is typically called by an
   * exception handler for a Win32 structured exception when another
   * process has grown the backing store (and its mapping) and our
   * process now incurs a fault because our mapping isn't in range
   * (yet).
   */
  int remap (void *addr);

  /// Round up to system page size.
  size_t round_to_page_size (size_t nbytes);

  /// Round up to the chunk size required by the operation system
  size_t round_to_chunk_size (size_t nbytes);

  // = Don't need this methods here ...
  int sync (ssize_t = -1, int = MS_SYNC);
  int sync (void *, size_t, int = MS_SYNC);
  int protect (ssize_t = -1, int = PROT_RDWR);
  int protect (void *, size_t, int = PROT_RDWR);
  void dump (void) const {}

protected:

  /**
   * Map portions or the entire pool into the local virtual address
   * space.  To do this, we compute the new <file_offset> of the
   * backing store and commit the memory.
   */
  int map (int &firstTime, int appendBytes = 0);

  /// Release the mapping.
  int unmap (void);

private:

  /**
   * @class Control_Block
   *
   * @brief Attributes that are meaningful in local storage only.
   */
  class Control_Block
  {
  public:
    /// required base address
    void *req_base_;

    /// Base address returned from system call
    void *mapped_base_;

    /**
     * @class Shared_Control_Block
     *
     * @brief Pool statistics
     */
    class Shared_Control_Block
    {
    public:
      /// Maximum size the pool may grow
      size_t max_size_;

      /// Size of mapped shared memory segment
      int mapped_size_;

      /// Offset to mapped but not yet acquired address space
      int free_offset_;

      /// Size of mapped but not yet acquired address space
      int free_size_;
    };

    Shared_Control_Block sh_;
  };

  // Base of mapped region.  If this has the value of 0 then the OS is
  // free to select any address to map the file, otherwise this value
  // is what the OS must try to use to mmap the file.

  /// Description of what our process mapped.
  Control_Block local_cb_;

  /// Shared memory pool statistics.
  Control_Block *shared_cb_;

  /// File mapping handle.
  ACE_HANDLE object_handle_;

  /// System page size.
  size_t page_size_;

  /// Name of the backing store where the shared memory pool is kept.
  ACE_TCHAR backing_store_name_[MAXPATHLEN];
};

#endif /* ACE_WIN32 */

#if defined (__ACE_INLINE__)
#include "ace/Memory/Memory_Pool.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ACE_MEMORY_POOL_H */
