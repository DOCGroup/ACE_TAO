/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Select_Reactor_Base.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt <schmidt@cs.wustl.edu>
 */
//=============================================================================

#ifndef ACE_SELECT_REACTOR_NOTIFY_H
#define ACE_SELECT_REACTOR_NOTIFY_H
#include "ace/pre.h"
#include "Reactor_Impl.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */
#include "ace/Pipe.h"

#if defined (ACE_HAS_REACTOR_NOTIFICATION_QUEUE)
#include "Unbounded_Queue.h"
#endif /* ACE_HAS_REACTOR_NOTIFICATION_QUEUE */

class ACE_Select_Reactor_Impl;


/**
 * @class ACE_Select_Reactor_Notify
 *
 * @brief Unblock the <ACE_Select_Reactor> from its event loop.
 *
 * This implementation is necessary for cases where the
 * <ACE_Select_Reactor> is run in a multi-threaded program.  In
 * this case, we need to be able to unblock <select> or <poll>
 * when updates occur other than in the main
 * <ACE_Select_Reactor> thread.  To do this, we signal an
 * auto-reset event the <ACE_Select_Reactor> is listening on.
 * If an <ACE_Event_Handler> and <ACE_Select_Reactor_Mask> is
 * passed to <notify>, the appropriate <handle_*> method is
 * dispatched in the context of the <ACE_Select_Reactor> thread.
 */
class ACE_Export ACE_Select_Reactor_Notify : public ACE_Reactor_Notify
{
public:
  /// Constructor.
  ACE_Select_Reactor_Notify (void);

  /// Destructor.
  ~ACE_Select_Reactor_Notify (void);

  // = Initialization and termination methods.
  /// Initialize.
  virtual int open (ACE_Reactor_Impl *,
                    ACE_Timer_Queue * = 0,
                    int disable_notify_pipe = 0);

  /// Destroy.
  virtual int close (void);

  /**
   * Called by a thread when it wants to unblock the
   * <ACE_Select_Reactor>.  This wakeups the <ACE_Select_Reactor> if
   * currently blocked in <select>/<poll>.  Pass over both the
   * <Event_Handler> *and* the <mask> to allow the caller to dictate
   * which <Event_Handler> method the <ACE_Select_Reactor> will
   * invoke.  The <ACE_Time_Value> indicates how long to blocking
   * trying to notify the <ACE_Select_Reactor>.  If <timeout> == 0,
   * the caller will block until action is possible, else will wait
   * until the relative time specified in *<timeout> elapses).
   */
  virtual int notify (ACE_Event_Handler * = 0,
                      ACE_Reactor_Mask = ACE_Event_Handler::EXCEPT_MASK,
                      ACE_Time_Value * = 0);

  /// Handles pending threads (if any) that are waiting to unblock the
  /// <ACE_Select_Reactor>.
  virtual int dispatch_notifications (int &number_of_active_handles,
                                      ACE_Handle_Set &rd_mask);

  /// Returns the ACE_HANDLE of the notify pipe on which the reactor
  /// is listening for notifications so that other threads can unblock
  /// the Select_Reactor
  virtual ACE_HANDLE notify_handle (void);

  /// Handle one of the notify call on the <handle>. This could be
  /// because of a thread trying to unblock the <Reactor_Impl>
  virtual int dispatch_notify (ACE_Notification_Buffer &buffer);

  /// Read one of the notify call on the <handle> into the
  /// <buffer>. This could be because of a thread trying to unblock
  /// the <Reactor_Impl>
  virtual int read_notify_pipe (ACE_HANDLE handle,
                                ACE_Notification_Buffer &buffer);

  /// Verify whether the buffer has dispatchable info  or not.
  virtual int is_dispatchable (ACE_Notification_Buffer &buffer);

  /// Called back by the <ACE_Select_Reactor> when a thread wants to
  /// unblock us.
  virtual int handle_input (ACE_HANDLE handle);

  /**
   * Set the maximum number of times that the
   * <ACE_Select_Reactor_Notify::handle_input> method will iterate and
   * dispatch the <ACE_Event_Handlers> that are passed in via the
   * notify pipe before breaking out of its <recv> loop.  By default,
   * this is set to -1, which means "iterate until the pipe is empty."
   * Setting this to a value like "1 or 2" will increase "fairness"
   * (and thus prevent starvation) at the expense of slightly higher
   * dispatching overhead.
   */
  virtual void max_notify_iterations (int);

  /**
   * Get the maximum number of times that the
   * <ACE_Select_Reactor_Notify::handle_input> method will iterate and
   * dispatch the <ACE_Event_Handlers> that are passed in via the
   * notify pipe before breaking out of its <recv> loop.
   */
  virtual int max_notify_iterations (void);

  /**
   * Purge any notifications pending in this reactor for the specified
   * <ACE_Event_Handler> object. If <eh> == 0, all notifications for all
   * handlers are removed (but not any notifications posted just to wake up
   * the reactor itself). Returns the number of notifications purged.
   * Returns -1 on error.
   */
  virtual int purge_pending_notifications (ACE_Event_Handler *,
                                           ACE_Reactor_Mask = ACE_Event_Handler::ALL_EVENTS_MASK);

  /// Dump the state of an object.
  virtual void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

protected:

#if defined (ACE_HAS_REACTOR_NOTIFICATION_QUEUE)

  /// Add the <buffer> to the user level notification queue. The
  /// <notification_required> returns whether the queue needs a
  /// notification to be sent in the pipe or not.
  int notify_message_to_queue (ACE_Notification_Buffer &buffer,
                               int &notification_required);
#endif/*ACE_HAS_REACTOR_NOTIFICATION_QUEUE*/

protected:
  /**
   * Keep a back pointer to the <ACE_Select_Reactor>.  If this value
   * if NULL then the <ACE_Select_Reactor> has been initialized with
   * <disable_notify_pipe>.
   */
  ACE_Select_Reactor_Impl *select_reactor_;

  /**
   * Contains the <ACE_HANDLE> the <ACE_Select_Reactor> is listening
   * on, as well as the <ACE_HANDLE> that threads wanting the
   * attention of the <ACE_Select_Reactor> will write to.
   */
  ACE_Pipe notification_pipe_;

  /**
   * Keeps track of the maximum number of times that the
   * <ACE_Select_Reactor_Notify::handle_input> method will iterate and
   * dispatch the <ACE_Event_Handlers> that are passed in via the
   * notify pipe before breaking out of its <recv> loop.  By default,
   * this is set to -1, which means "iterate until the pipe is empty."
   */
  int max_notify_iterations_;

#if defined (ACE_HAS_REACTOR_NOTIFICATION_QUEUE)
  // = This configuration queues up notifications in separate buffers that
  //   are in user-space, rather than stored in a pipe in the OS
  //   kernel.  The kernel-level notifications are used only to trigger
  //   the Reactor to check its notification queue.  This enables many
  //   more notifications to be stored than would otherwise be the case.

  /// Keeps track of allocated arrays of type
  /// <ACE_Notification_Buffer>.
  ACE_Unbounded_Queue <ACE_Notification_Buffer *> alloc_queue_;

  /// Keeps track of all pending notifications.
  ACE_Unbounded_Queue <ACE_Notification_Buffer *> notify_queue_;

  /// Keeps track of all free buffers.
  ACE_Unbounded_Queue <ACE_Notification_Buffer *> free_queue_;

  /// Synchronization for handling of queues.
  ACE_SYNCH_MUTEX notify_queue_lock_;
#endif /* ACE_HAS_REACTOR_NOTIFICATION_QUEUE */
};


#include "ace/post.h"
#endif /*ACE_SELECT_REACTOR_NOTIFY_H*/
