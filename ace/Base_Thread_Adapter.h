//=============================================================================
/**
 *  @file    Base_Thread_Adapter.h
 *
 *  $Id$
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 */
//=============================================================================

#ifndef ACE_BASE_THREAD_ADAPTER_H
#define ACE_BASE_THREAD_ADAPTER_H
#include "ace/pre.h"

#include "ace/OS_Log_Msg_Attributes.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/OS_Export.h"

// Run the thread entry point for the <ACE_Thread_Adapter>.  This must
// be an extern "C" to make certain compilers happy...
#if defined (ACE_PSOS)
extern "C" void ace_thread_adapter (unsigned long args);
#else /* ! defined (ACE_PSOS) */
extern "C" ACE_OS_Export void *ace_thread_adapter (void *args);
#endif /* ACE_PSOS */

/**
 * @class ACE_OS_Thread_Descriptor
 *
 * @brief Parent class of all ACE_Thread_Descriptor classes.
 * =
 * Container for ACE_Thread_Descriptor members that are
 * used in ACE_OS.
 */
class ACE_OS_Export ACE_OS_Thread_Descriptor
{
public:
  /// Get the thread creation flags.
  long flags (void) const;

protected:
  /// For use by ACE_Thread_Descriptor.
  ACE_OS_Thread_Descriptor (long flags = 0);

  /**
   * Keeps track of whether this thread was created "detached" or not.
   * If a thread is *not* created detached then if someone calls
   * <ACE_Thread_Manager::wait>, we need to join with that thread (and
   * close down the handle).
   */
  long flags_;
};

/**
 * @class ACE_Base_Thread_Adapter
 *
 * @brief Base class for all the Thread_Adapters.
 *
 * Converts a C++ function into a function <ace_thread_adapter>
 * function that can be called from a thread creation routine
 * (e.g., <pthread_create> or <_beginthreadex>) that expects an
 * extern "C" entry point.  This class also makes it possible to
 * transparently provide hooks to register a thread with an
 * <ACE_Thread_Manager>.
 * This class is used in <ACE_OS::thr_create>.  In general, the
 * thread that creates an object of this class is different from
 * the thread that calls <invoke> on this object.  Therefore,
 * the <invoke> method is responsible for deleting itself.
 */
class ACE_OS_Export ACE_Base_Thread_Adapter
{
public:
  ACE_Base_Thread_Adapter (ACE_THR_FUNC user_func,
                           void *arg,
                           ACE_THR_C_FUNC entry_point = (ACE_THR_C_FUNC) ace_thread_adapter,
                           ACE_OS_Thread_Descriptor *td = 0
# if defined (ACE_HAS_WIN32_STRUCTURAL_EXCEPTIONS)
                           , ACE_SEH_EXCEPT_HANDLER selector = 0
                           , ACE_SEH_EXCEPT_HANDLER handler = 0
# endif /* ACE_HAS_WIN32_STRUCTURAL_EXCEPTIONS */
  /// Constructor.
                      );

  /// Virtual method invoked by the thread entry point.
  virtual void *invoke (void) = 0;

  /// Accessor for the C entry point function to the OS thread creation
  /// routine.
  ACE_THR_C_FUNC entry_point (void);

  /// Invoke the close_log_msg_hook, if it is present
  static void close_log_msg (void);

  /// Invoke the sync_log_msg_hook, if it is present
  static void sync_log_msg (const ACE_TCHAR *prog_name);

  /// Invoke the thr_desc_log_msg_hook, if it is present
  static ACE_OS_Thread_Descriptor *thr_desc_log_msg (void);

protected:
  /// Destructor, making it private ensures that objects of this class
  /// are allocated on the heap.
  virtual ~ACE_Base_Thread_Adapter (void);

  /// Inherit the logging features if the parent thread has an
  /// <ACE_Log_Msg>.
  void inherit_log_msg (void);

private:
  /// The hooks to inherit and cleanup the Log_Msg attributes
  static ACE_INIT_LOG_MSG_HOOK init_log_msg_hook_;
  static ACE_INHERIT_LOG_MSG_HOOK inherit_log_msg_hook_;
  static ACE_CLOSE_LOG_MSG_HOOK close_log_msg_hook_;
  static ACE_SYNC_LOG_MSG_HOOK sync_log_msg_hook_;
  static ACE_THR_DESC_LOG_MSG_HOOK thr_desc_log_msg_hook_;

  /// Set the Log_Msg hooks
  static void set_log_msg_hooks (ACE_INIT_LOG_MSG_HOOK init_hook,
                                 ACE_INHERIT_LOG_MSG_HOOK inherit_hook,
                                 ACE_CLOSE_LOG_MSG_HOOK close_hook,
                                 ACE_SYNC_LOG_MSG_HOOK sync_hook,
                                 ACE_THR_DESC_LOG_MSG_HOOK thr_desc);

  /// Allow the ACE_Log_Msg class to set its hooks.
  friend class ACE_Log_Msg;

protected:
  /// Thread startup function passed in by the user (C++ linkage).
  ACE_THR_FUNC user_func_;

  /// Argument to thread startup function.
  void *arg_;

  /// Entry point to the underlying OS thread creation call (C
  /// linkage).
  ACE_THR_C_FUNC entry_point_;

  /**
   * Optional thread descriptor.  Passing this pointer in will force
   * the spawned thread to cache this location in <Log_Msg> and wait
   * until <Thread_Manager> fills in all information in thread
   * descriptor.
   */
  ACE_OS_Thread_Descriptor *thr_desc_;

  /// The ACE_Log_Msg attributes.
  ACE_OS_Log_Msg_Attributes log_msg_attributes_;

  /// Friend declaration to avoid compiler warning:  only defines a private
  /// destructor and has no friends.
  friend class ACE_Thread_Adapter_Has_Private_Destructor;
};

# if defined (ACE_HAS_INLINED_OSCALLS)
#   if defined (ACE_INLINE)
#     undef ACE_INLINE
#   endif /* ACE_INLINE */
#   define ACE_INLINE inline
#   include "ace/Base_Thread_Adapter.inl"
# endif /* ACE_HAS_INLINED_OSCALLS */

#include "ace/post.h"
#endif /* ACE_BASE_THREAD_ADAPTER_H */
