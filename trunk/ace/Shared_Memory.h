/* -*- C++ -*- */
// $Id$


// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    Shared_Memory.h
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (ACE_SHARED_MEMORY_H)
#define ACE_SHARED_MEMORY_H

#include "ace/ACE.h"

class ACE_Export ACE_Shared_Memory
  // = TITLE
  //     This base class adapts both System V shared memory and "BSD"
  //     mmap to a common API.
  //
  // = DESCRIPTION
  //     This is a very simple-minded wrapper, i.e., it really is only
  //     useful for allocating large contiguous chunks of shared
  //     memory.  For a much more sophisticated version, please check
  //     out the <ACE_Malloc> class.
{
public:
  // = Note that all methods are pure virtual.
  virtual int close (void) = 0;
  virtual int remove (void) = 0;
  virtual void *malloc (size_t = 0) = 0;
  virtual int free (void *p) = 0;
  virtual int get_segment_size (void) const = 0;
  virtual ACE_HANDLE get_id (void) const = 0;
};
#endif /* ACE_SHARED_MEMORY_H */
