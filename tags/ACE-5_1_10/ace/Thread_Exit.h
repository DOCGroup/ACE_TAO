// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Thread_Exit.h
//
// = AUTHOR
//    Carlos O'Ryan <coryan@uci.edu>
//
// ============================================================================

#ifndef ACE_THREAD_EXIT_H
#define ACE_THREAD_EXIT_H
#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/OS.h"
#include "ace/Thread_Control.h"

class ACE_Export ACE_Thread_Exit
{
  // = TITLE
  //    Keep exit information for a Thread in thread specific storage.
  //    so that the thread-specific exit hooks will get called no
  //    matter how the thread exits (e.g., via <ACE_Thread::exit>, C++
  //    or Win32 exception, "falling off the end" of the thread entry
  //    point function, etc.).
  //
  // = DESCRIPTION
  //    This clever little helper class is stored in thread-specific
  //    storage using the <ACE_TSS> wrapper.  When a thread exits the
  //    <ACE_TSS::cleanup> function deletes this object, thereby
  //    closing it down gracefully.
public:
  ACE_Thread_Exit (void);
  // Capture the Thread that will be cleaned up automatically.

  void thr_mgr (ACE_Thread_Manager *tm);
  // Set the <ACE_Thread_Manager>.

  ~ACE_Thread_Exit (void);
  // Destructor calls the thread-specific exit hooks when a thread
  // exits.

  static ACE_Thread_Exit *instance (void);
  // Singleton access point.

  static void cleanup (void *instance, void *);
  // Cleanup method, used by the <ACE_Object_Manager> to destroy the
  // singleton.

private:
  friend class ACE_OS_Object_Manager;
  // Allow OS_Object_Manager to reset the status of <is_constructed_>.

  ACE_Thread_Control thread_control_;
  // Automatically add/remove the thread from the
  // <ACE_Thread_Manager>.

  static u_int is_constructed_;
  // Used to detect whether we should create a new instance (or not)
  // within the instance method -- we don't trust the instance_ ptr
  // because the destructor may have run (if ACE::fini() was called).
  // See bug #526.
  // We don't follow the singleton pattern due to dependency issues.
};

class ACE_Export ACE_Thread_Exit_Maybe
  // = TITLE
  //     A version of ACE_Thread_Exit that is created dynamically
  //     under the hood if the flag is set to TRUE.
  // = DESCRIPTION
  //     Allows the appearance of a "smart pointer", but is not
  //     always created.
{
public:
  ACE_Thread_Exit_Maybe (int flag = 0);
  // Don't create an ACE_Thread_Exit instance by default.

  ~ACE_Thread_Exit_Maybe (void);
  // Destroys the underlying ACE_Thread_Exit instance if it exists.

  ACE_Thread_Exit * operator -> (void) const;
  // Delegates to underlying instance.

  ACE_Thread_Exit * instance (void) const;
  // Returns the underlying instance.

private:

  ACE_Thread_Exit *instance_;
  // Holds the underlying instance.

};

#include "ace/post.h"
#endif /* ACE_THREAD_EXIT_H */
