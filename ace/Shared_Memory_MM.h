/* -*- C++ -*- */
// $Id$


// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    Shared_Memory_MM.h
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (ACE_SHARED_MALLOC_MM_H)
#define ACE_SHARED_MALLOC_MM_H

#include "ace/Shared_Memory.h"
#include "ace/Mem_Map.h"

class ACE_Export ACE_Shared_Memory_MM : public ACE_Shared_Memory
  // = TITLE
  //     Shared memory wrapper based on MMAP.
{
public:
  // = Initialization and termination methods.
  ACE_Shared_Memory_MM (void);
  ACE_Shared_Memory_MM (ACE_HANDLE handle, 
			int length = -1, 
			int prot = PROT_RDWR, 
			int share = MAP_PRIVATE, 
			char *addr = 0, 
			off_t pos = 0);
  ACE_Shared_Memory_MM (char file_name[], 
			int len = -1, 
			int flags = O_RDWR | O_CREAT,
			int mode = ACE_DEFAULT_FILE_PERMS, 
			int prot = PROT_RDWR, 
			int share = MAP_SHARED, 
			char *addr = 0, off_t pos = 0);

  int open (ACE_HANDLE handle, 
	    int length = -1, 
	    int prot = PROT_RDWR, 
	    int share = MAP_PRIVATE, 
	    char *addr = 0, 
	    off_t pos = 0);

  int open (char file_name[], 
	    int len = -1, 
	    int flags = O_RDWR | O_CREAT, 
	    int mode = ACE_DEFAULT_FILE_PERMS, 
	    int prot = PROT_RDWR, 
	    int share = MAP_SHARED, 
	    char *addr = 0, 
	    off_t pos = 0);

  virtual int close (void);
  // Close down the shared memory segment.

  virtual int remove (void);
  // Remove the shared memory segment and the underlying file.

  // = Allocation and deallocation methods.
  virtual void *malloc (size_t size = 0);
  // Create a new chuck of memory containing <size> bytes.

  virtual int free (void *p);
  // Free a chuck of memory allocated by <ACE_Shared_Memory_MM::malloc>.
  
  virtual int get_segment_size (void) const;
  // Return the size of the shared memory segment.
  
  virtual ACE_HANDLE get_id (void) const;
  // Return the ID of the shared memory segment (i.e., an ACE_HANDLE).

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
   ACE_Mem_Map shared_memory_;
   // This version is implemented with memory-mapped files.
};

#if defined (__ACE_INLINE__)
#include "ace/Shared_Memory_MM.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_SHARED_MALLOC_MM_H */
