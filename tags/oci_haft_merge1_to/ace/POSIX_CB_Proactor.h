/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    POSIX_CB_Proactor.h
 *
 *  $Id$
 *
 *  @author Alexander Libman <alibman@ihug.com.au>
 */
//=============================================================================

#ifndef ACE_POSIX_CB_PROACTOR_H
#define ACE_POSIX_CB_PROACTOR_H

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (ACE_HAS_AIO_CALLS) && !defined(__sun) && !defined(__Lynx__)

#include "ace/POSIX_Proactor.h"

/**
 * @class ACE_POSIX_CB_Proactor
 *
 * @brief Implementation of Callback-based Proactor
 * };
 */
class ACE_Export ACE_POSIX_CB_Proactor : public ACE_POSIX_AIOCB_Proactor
{

public:
  virtual Proactor_Type get_impl_type (void);

  /// Destructor.
  virtual ~ACE_POSIX_CB_Proactor (void);

  /// Constructor defines max number asynchronous operations that can
  /// be started at the same time.
  ACE_POSIX_CB_Proactor (size_t max_aio_operations = ACE_AIO_DEFAULT_SIZE);

protected:

  static void aio_completion_func (sigval_t cb_data);

  /**
   * Dispatch a single set of events.  If <wait_time> elapses before
   * any events occur, return 0.  Return 1 on success i.e., when a
   * completion is dispatched, non-zero (-1) on errors and errno is
   * set accordingly.
   */
  virtual int handle_events (ACE_Time_Value &wait_time);

  /**
   * Block indefinitely until at least one event is dispatched.
   * Dispatch a single set of events.  If <wait_time> elapses before
   * any events occur, return 0.  Return 1 on success i.e., when a
   * completion is dispatched, non-zero (-1) on errors and errno is
   * set accordingly.
   */
  virtual int handle_events (void);

  /// Find free slot to store result and aiocb pointer
  virtual ssize_t allocate_aio_slot (ACE_POSIX_Asynch_Result *result);

  /// Notify queue of "post_completed" ACE_POSIX_Asynch_Results
  /// called from post_completion method
  virtual int notify_completion (int sig_num);

  /**
   * Dispatch a single set of events.  If <milli_seconds> elapses
   * before any events occur, return 0. Return 1 if a completion is
   * dispatched. Return -1 on errors.
   */
  int handle_events_i (u_long milli_seconds);
 
  /// semaphore variable to notify
  /// used to wait the first AIO start
  ACE_SYNCH_SEMAPHORE sema_;
};

#if defined (__ACE_INLINE__)
#include "ace/POSIX_CB_Proactor.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_HAS_AIO_CALLS && !__sun && !__Lynx__ */
#endif /* ACE_POSIX_CB_PROACTOR_H*/
