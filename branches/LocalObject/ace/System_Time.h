/* -*- C++ -*- */
// $Id$


// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    System_Time.h
//
// = AUTHOR
//    Prashant Jain, Tim H. Harrison and Douglas C. Schmidt
//
// ============================================================================

#ifndef ACE_SYSTEM_TIME_H
#define ACE_SYSTEM_TIME_H
#include "ace/pre.h"

#include "ace/OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Memory_Pool.h"
#include "ace/Malloc_T.h"

class ACE_Export ACE_System_Time
{
  // = TITLE
  //     Defines the timer services of the OS interface to access the
  //     system time either on the local host or on the central time
  //     server in the network.
public:
  enum Sync_Mode { Jump, Adjust };
  // enumeration types to specify mode of synchronization with master
  // clock.  Jump will set local system time directly (thus possibly
  // producing time gaps or ambiguous local system times.  Adjust will
  // smoothly slow down or speed up the local system clock to reach
  // the system time of the master clock.

  ACE_System_Time (LPCTSTR poolname = 0);
  // Default constructor.

  ~ACE_System_Time (void);
  // Default destructor.

  static int get_local_system_time (ACE_UINT32 &time_out);
  // Get the local system time, i.e., the value returned by
  // <ACE_OS::time>.

  static int get_local_system_time (ACE_Time_Value &time_out);
  // Get the local system time, i.e., the value returned by
  // <ACE_OS::time>.

  int get_master_system_time (ACE_UINT32 &time_out);
  // Get the system time of the central time server.

  int get_master_system_time (ACE_Time_Value &time_out);
  // Get the system time of the central time server.

  int sync_local_system_time (ACE_System_Time::Sync_Mode mode);
  // synchronize local system time with the central time server using
  // specified mode.

private:
  typedef ACE_Malloc <ACE_MMAP_MEMORY_POOL, ACE_Null_Mutex> MALLOC;
  typedef ACE_Allocator_Adapter<MALLOC> ALLOCATOR;

  ALLOCATOR *shmem_;
  // Our allocator (used for obtaining system time from shared memory).

  TCHAR poolname_[MAXPATHLEN + 1];
  // The name of the pool used by the allocator.

  long *delta_time_;
  // Pointer to delta time kept in shared memory.
};

#include "ace/post.h"
#endif /* ACE_SYSTEM_TIME_H */
