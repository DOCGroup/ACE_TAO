/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Shared_Memory_SV.h
//
// = AUTHOR
//    Doug Schmidt
//
// ============================================================================

#ifndef ACE_SHARED_MALLOC_SV_H
#define ACE_SHARED_MALLOC_SV_H

#include "ace/Shared_Memory.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/SV_Shared_Memory.h"

class ACE_Export ACE_Shared_Memory_SV : public ACE_Shared_Memory
{
  // = TITLE
  //     Shared memory wrapper based on System V shared memory.
public:
  enum
    {
      ACE_CREATE = IPC_CREAT,
      ACE_OPEN = 0
    };

  // = Initialization and termination methods.
  ACE_Shared_Memory_SV (void);
  ACE_Shared_Memory_SV (key_t id,
                        int length,
                        int create = ACE_Shared_Memory_SV::ACE_OPEN,
                        int perms = ACE_DEFAULT_FILE_PERMS,
                        void *addr = 0,
                        int flags = 0);

  int open (key_t id,
            int length,
            int create = ACE_Shared_Memory_SV::ACE_OPEN,
            int perms = ACE_DEFAULT_FILE_PERMS,
            void *addr = 0,
            int flags = 0);

  virtual int close (void);
  // Close down the shared memory segment.

  virtual int remove (void);
  // Remove the underlying shared memory segment.

  // = Allocation and deallocation methods.
  virtual void *malloc (size_t = 0);
  // Create a new chuck of memory containing <size> bytes.

  virtual int free (void *p);
  // Free a chuck of memory allocated by <ACE_Shared_Memory_SV::malloc>.

  virtual int get_segment_size (void) const;
  // Return the size of the shared memory segment.

  virtual ACE_HANDLE get_id (void) const;
  // Return the ID of the shared memory segment (i.e., a System V
  // shared memory internal id).

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
   ACE_SV_Shared_Memory shared_memory_;
   // This version is implemented with System V shared memory
   // segments.
};

#if defined (__ACE_INLINE__)
#include "ace/Shared_Memory_SV.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_SHARED_MALLOC_SV_H */
