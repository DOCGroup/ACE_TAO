// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Thread_Control.h
//
// = AUTHOR
//    Carlos O'Ryan <coryan@uci.edu>
//
// ============================================================================

#ifndef ACE_THREAD_CONTROL_H
#define ACE_THREAD_CONTROL_H
#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if !defined (ACE_LEGACY_MODE)
#  include "ace/OS.h"
#endif  /* ACE_LEGACY_MODE */

class ACE_Thread_Manager;

class ACE_Export ACE_Thread_Control
{
  // = TITLE
  //     Used to keep track of a thread's activities within its entry
  //     point function.
  //
  // = DESCRIPTION
  //     A <ACE_Thread_Manager> uses this class to ensure that threads
  //     it spawns automatically register and unregister themselves
  //     with it.
  //
  //     This class can be stored in thread-specific storage using the
  //     <ACE_TSS> wrapper.  When a thread exits the
  //     <ACE_TSS::cleanup> function deletes this object, thereby
  //     ensuring that it gets removed from its associated
  //     <ACE_Thread_Manager>.
public:
  ACE_Thread_Control (ACE_Thread_Manager *tm = 0,
                      int insert = 0);
  // Initialize the thread control object.  If <insert> != 0, then
  // register the thread with the Thread_Manager.

  ~ACE_Thread_Control (void);
  // Remove the thread from its associated <Thread_Manager> and exit
  // the thread if <do_thr_exit> is enabled.

  void *exit (void *status,
              int do_thr_exit);
  // Remove this thread from its associated <Thread_Manager> and exit
  // the thread if <do_thr_exit> is enabled.

  int insert (ACE_Thread_Manager *tm, int insert = 0);
  // Store the <Thread_Manager> and use it to register ourselves for
  // correct shutdown.

  ACE_Thread_Manager *thr_mgr (void);
  // Returns the current <Thread_Manager>.

  ACE_Thread_Manager *thr_mgr (ACE_Thread_Manager *);
  // Atomically set a new <Thread_Manager> and return the old
  // <Thread_Manager>.

  void *status (void *status);
  // Set the exit status (and return existing status).

  void *status (void);
  // Get the current exit status.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  ACE_Thread_Manager *tm_;
  // Pointer to the thread manager for this block of code.

  void *status_;
  // Keeps track of the exit status for the thread.
};

# if defined (ACE_HAS_INLINED_OSCALLS)
#   if defined (ACE_INLINE)
#     undef ACE_INLINE
#   endif /* ACE_INLINE */
#   define ACE_INLINE inline
#   include "ace/Thread_Control.inl"
# endif /* ACE_HAS_INLINED_OSCALLS */

#include "ace/post.h"
#endif /* ACE_THREAD_CONTROL_H */
