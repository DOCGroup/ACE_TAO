/* -*- C++ -*- */
// $Id$


// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    SV_Shared_Memory.h
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (ACE_SV_SHARED_MEMORY_H)
#define ACE_SV_SHARED_MEMORY_H

#include "ace/ACE.h"

class ACE_Export ACE_SV_Shared_Memory
  // = TITLE
  //     This is a wrapper for System V shared memory.
{
public:
  enum 
  {
    ACE_CREATE = IPC_CREAT, 
    ACE_OPEN   = 0
  };

  // = Initialization and termination methods.
  ACE_SV_Shared_Memory (void);
  ACE_SV_Shared_Memory (key_t external_id, 
			size_t size, 
			int create,
			int perms = ACE_DEFAULT_FILE_PERMS, 
			void *virtual_addr = 0, 
			int flags = 0);

  ACE_SV_Shared_Memory (ACE_HANDLE internal_id,
			int flags = 0);

  int  open (key_t external_id, 
	     size_t size,
	     int create = ACE_SV_Shared_Memory::ACE_OPEN,
	     int perms = ACE_DEFAULT_FILE_PERMS);

  int  open_and_attach (key_t external_id, 
			size_t size, 
			int create = ACE_SV_Shared_Memory::ACE_OPEN,
			int perms = ACE_DEFAULT_FILE_PERMS, 
			void *virtual_addr = 0, 
			int flags = 0);

  int  attach (void *virtual_addr = 0, 
	       int flags =0);
  // Attach this shared memory segment.

  int  detach (void);
  // Detach this shared memory segment.

  int  remove (void);
  // Remove this shared memory segment.

  int  control (int cmd, void *buf);
  // Forward to underlying System V <shmctl>.

  // = Segment-related info.
  void *get_segment_ptr (void) const;
  int  get_segment_size (void) const;

  ACE_HANDLE get_id (void) const;
  // Return the ID of the shared memory segment (i.e., an ACE_HANDLE).

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  enum
  {
    ALIGN_WORDB = 8   // Most restrictive alignment. 
  };

  ACE_HANDLE internal_id_;
  // Internal identifier.

  int size_;
  // Size of the mapped segment.
  
  void *segment_ptr_;
  // Pointer to the beginning of the segment.

  int round_up (size_t len);
  // Round up to an appropriate page size.
};

#if defined (__ACE_INLINE__)
#include "ace/SV_Shared_Memory.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_SV_SHARED_MEMORY_H */
