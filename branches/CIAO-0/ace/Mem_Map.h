/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Mem_Map.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt <schmidt@cs.wustl.edu>
 */
//=============================================================================

#ifndef ACE_MEM_MAP_H
#define ACE_MEM_MAP_H
#include "ace/pre.h"

#include "ace/ACE.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class ACE_Mem_Map
 *
 * @brief C++ interface OS memory mapping system call.
 *
 * This class works with both the mmap(2) UNIX system and the
 * Win32 family of memory mapping system calls.
 */
class ACE_Export ACE_Mem_Map
{
public:
  // = Initialization and termination methods.

  /// Default constructor.
  ACE_Mem_Map (void);

  /// Map a file from an open file descriptor <handle>.  This function
  /// will lookup the length of the file if it is not given.
  ACE_Mem_Map (ACE_HANDLE handle,
               int length = -1,
               int prot = PROT_RDWR,
               int share = ACE_MAP_PRIVATE,
               void *addr = 0,
               off_t offset = 0,
               LPSECURITY_ATTRIBUTES sa = 0);

  /// Map a file specified by <file_name>.
  ACE_Mem_Map (const ACE_TCHAR *filename,
               int len = -1,
               int flags = O_RDWR | O_CREAT,
               int mode = ACE_DEFAULT_FILE_PERMS,
               int prot = PROT_RDWR,
               int share = ACE_MAP_PRIVATE,
               void *addr = 0,
               off_t offset = 0,
               LPSECURITY_ATTRIBUTES sa = 0);

  /// Map a file from an open file descriptor <handle>.  This function
  /// will lookup the length of the file if it is not given.
  int map (ACE_HANDLE handle,
           int length = -1,
           int prot = PROT_RDWR,
           int share = ACE_MAP_PRIVATE,
           void *addr = 0,
           off_t offset = 0,
           LPSECURITY_ATTRIBUTES sa = 0);

  /// Remap the file associated with <handle_>.
  int map (int length = -1,
           int prot = PROT_RDWR,
           int share = ACE_MAP_PRIVATE,
           void *addr = 0,
           off_t offset = 0,
           LPSECURITY_ATTRIBUTES sa = 0);

  /// Map a file specified by <filename>.
  int map (const ACE_TCHAR *filename,
           int len = -1,
           int flags = O_RDWR | O_CREAT,
           int mode = ACE_DEFAULT_FILE_PERMS,
           int prot = PROT_RDWR,
           int share = ACE_MAP_PRIVATE,
           void *addr = 0,
           off_t offset = 0,
           LPSECURITY_ATTRIBUTES sa = 0);

  /// Destructor.
  ~ACE_Mem_Map (void);

  /// Open the file without mapping it.
  int open (const ACE_TCHAR *filename,
            int flags = O_RDWR | O_CREAT,
            int mode = ACE_DEFAULT_FILE_PERMS,
            LPSECURITY_ATTRIBUTES sa = 0);

  /// Close down the <handle_> if necessary and unmap the mapping.
  int close (void);

  /// Close down the <handle_> if necessary.
  int close_handle (void);

  /**
   * Close down the internal <file_mapping_> if necessary.  This is
   * mostly necessary on Win32, which has a different handle for
   * file-mapping kernel object.
   */
  int close_filemapping_handle (void);

  /// This operator passes back the starting address of the mapped
  /// file.
  int operator () (void *&addr);

  /// Return the base address.
  void *addr (void) const;

  /// This function returns the number of bytes currently mapped in the
  /// file.
  size_t size (void) const;

  /// Unmap the region starting at <base_addr_>.
  int unmap (int len = -1);

  /// Unmap the region starting at <addr_>.
  int unmap (void *addr, int len);

  /**
   * Sync <len> bytes of the memory region to the backing store
   * starting at <base_addr_>.  If <len> == -1 then sync the whole
   * region.
   */
  int sync (ssize_t len = -1, int flags = MS_SYNC);

  /// Sync <len> bytes of the memory region to the backing store
  /// starting at <addr_>.
  int sync (void *addr, size_t len, int flags = MS_SYNC);

  /**
   * Change the protection of the pages of the mapped region to <prot>
   * starting at <base_addr_> up to <len> bytes.  If <len> == -1 then
   * change protection of all pages in the mapped region.
   */
  int protect (ssize_t len = -1, int prot = PROT_RDWR);

  /// Change the protection of the pages of the mapped region to <prot>
  /// starting at <addr> up to <len> bytes.
  int protect (void *addr, size_t len, int prot = PROT_RDWR);

  /// Close and remove the file from the file system.
  int remove (void);

  /// Hook into the underlying VM system.
  int advise (int behavior, int len = -1);

  /// Return the underlying <handle_>.
  ACE_HANDLE handle (void) const;

  /// Return the name of file that is mapped (if any).
  const ACE_TCHAR *filename (void) const;

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

private:
  /// Base address of the memory-mapped file.
  void *base_addr_;

  /// Name of the file that is mapped.
  ACE_TCHAR filename_[MAXPATHLEN + 1];

  /// Length of the mapping.
  size_t length_;

  /// HANDLE for the open file.
  ACE_HANDLE handle_;

  /// HANDLE for the open mapping.
  ACE_HANDLE file_mapping_;

#if defined (__Lynx__)
  /// Flag to indicate that PROT_WRITE has been enabled.
  int write_enabled_;
#endif /* __Lynx__ */

  /// Keeps track of whether we need to close the handle.  This is set
  /// if we opened the file.
  int close_handle_;

  /// This method does the dirty work of actually calling ::mmap to map
  /// the file into memory.
  int map_it (ACE_HANDLE handle,
              int len = -1,
              int prot = PROT_RDWR,
              int share = MAP_SHARED,
              void *addr = 0,
              off_t offset = 0,
              LPSECURITY_ATTRIBUTES sa = 0);

  // = Disallow copying and assignment.
  ACE_UNIMPLEMENTED_FUNC (ACE_Mem_Map (const ACE_Mem_Map &))
  ACE_UNIMPLEMENTED_FUNC (void operator = (const ACE_Mem_Map &))
};

#if defined (__ACE_INLINE__)
#include "ace/Mem_Map.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ACE_MEM_MAP_H */
