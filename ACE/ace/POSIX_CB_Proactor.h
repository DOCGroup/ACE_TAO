// -*- C++ -*-

//=============================================================================
/**
 *  @file    POSIX_CB_Proactor.h
 *
 *  @author Alexander Libman <alibman@ihug.com.au>
 */
//=============================================================================

#ifndef ACE_POSIX_CB_PROACTOR_H
#define ACE_POSIX_CB_PROACTOR_H

#include /**/ "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (ACE_HAS_AIO_CALLS)

#include "ace/Condition_Thread_Mutex.h"
#include "ace/Synch_Traits.h"
#include "ace/Thread_Semaphore.h"
#include "ace/Thread_Mutex.h"
#include "ace/Null_Semaphore.h"

#include "ace/POSIX_Proactor.h"


ACE_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class ACE_POSIX_CB_Proactor
 *
 * @brief Implementation of callback-based Proactor.
 */
class ACE_Export ACE_POSIX_CB_Proactor : public ACE_POSIX_AIOCB_Proactor
{
  /// Tracks pending callback notifications during shutdown.
  class Notification_State
  {
  public:
    /// Construct the state object using the callback startup semaphore.
    explicit Notification_State (ACE_SYNCH_SEMAPHORE &sema);

    /// Record a newly submitted callback that must finish before close().
    void add_pending ();

    /// Mark one pending callback as completed.
    void complete_one ();

    /// Remove one pending callback that was abandoned before completion.
    void abandon_pending ();

    /// Return the number of callbacks still expected to complete.
    size_t pending ();

    /// Wait until the pending callback count reaches zero.
    int wait_for_pending_zero (const ACE_Time_Value *abstime);

    /// Hold an internal reference while a callback may still access the state.
    void add_ref ();

    /// Release a callback lifetime reference.
    void remove_ref ();

    /// Detach the startup semaphore once callback submission is stable.
    void detach ();

  private:
    /// Finalize one pending callback update and optionally wake the waiter.
    void finish_pending_i (bool signal_waiter);

    ACE_Thread_Mutex mutex_;
    ACE_Condition_Thread_Mutex zero_pending_;
    ACE_SYNCH_SEMAPHORE *sema_;
    size_t pending_callbacks_;
    size_t ref_count_;
  };

public:
  virtual Proactor_Type get_impl_type (void);

  /// Destructor.
  virtual ~ACE_POSIX_CB_Proactor (void);

  /// Constructor defines max number asynchronous operations that can
  /// be started at the same time.
  ACE_POSIX_CB_Proactor (size_t max_aio_operations = ACE_AIO_DEFAULT_SIZE);

  /// Close down the Proactor.
  virtual int close (void);

  /// Static completion trampoline used by the POSIX sigevent callback API.
  static void aio_completion_func (sigval cb_data);

protected:

  /**
   * Dispatch a single set of events.  If @a wait_time elapses before
   * any events occur, return 0.  Return 1 on success i.e., when a
   * completion is dispatched, non-zero (-1) on errors and errno is
   * set accordingly.
   */
  virtual int handle_events (ACE_Time_Value &wait_time);

  /**
   * Block indefinitely until at least one event is dispatched.
   * Dispatch a single set of events.  If @a wait_time elapses before
   * any events occur, return 0.  Return 1 on success i.e., when a
   * completion is dispatched, non-zero (-1) on errors and errno is
   * set accordingly.
   */
  virtual int handle_events (void);

  /// Find free slot to store result and aiocb pointer
  virtual ssize_t allocate_aio_slot (ACE_POSIX_Asynch_Result *result);

  /// Initiate an aio operation and track callback lifetime.
  virtual int start_aio (ACE_POSIX_Asynch_Result *result,
                         ACE_POSIX_Proactor::Opcode op);

  /// Notify queue of "post_completed" ACE_POSIX_Asynch_Results
  /// called from post_completion method
  virtual int notify_completion (int sig_num);

  /// Post a result without instantiating the AIOCB notify pipe.
  virtual int post_completion (ACE_POSIX_Asynch_Result *result);

  /// Drop the outstanding callback count for abandoned operations.
  virtual void abandon_pending_aio (void);

  /**
   * Dispatch a single set of events.  If @a milli_seconds elapses
   * before any events occur, return 0. Return 1 if a completion is
   * dispatched. Return -1 on errors.
   */
  int handle_events_i (u_long milli_seconds);

  /// Semaphore variable to notify
  /// used to wait the first AIO start
  ACE_SYNCH_SEMAPHORE sema_;

  /// Shared callback lifetime state for in-flight completions.
  Notification_State *notification_state_;
};

ACE_END_VERSIONED_NAMESPACE_DECL

#endif /* ACE_HAS_AIO_CALLS */
#endif /* ACE_POSIX_CB_PROACTOR_H*/
