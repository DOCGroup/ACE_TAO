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

#ifndef ACE_SELECT_REACTOR_BASE_H
#define ACE_SELECT_REACTOR_BASE_H
#include "ace/pre.h"

#include "ace/Signal.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Timer_Queue.h"
#include "ace/Event_Handler.h"
#include "ace/Handle_Set.h"
#include "ace/Token.h"
#include "ace/Pipe.h"
#include "ace/Reactor_Impl.h"

#if defined (ACE_HAS_REACTOR_NOTIFICATION_QUEUE)
#include "ace/Unbounded_Queue.h"
#endif /* ACE_HAS_REACTOR_NOTIFICATION_QUEUE */

// Add useful typedefs to simplify the following code.
typedef void (ACE_Handle_Set::*ACE_FDS_PTMF) (ACE_HANDLE);
typedef int (ACE_Event_Handler::*ACE_EH_PTMF) (ACE_HANDLE);

// Forward declaration.
class ACE_Select_Reactor_Impl;

/**
 * @class ACE_Select_Reactor_Handle_Set
 *
 * @brief Track handles we are interested for various events.
 */
class ACE_Export ACE_Select_Reactor_Handle_Set
{
public:
  /// Read events (e.g., input pending, accept pending).
  ACE_Handle_Set rd_mask_;

  /// Write events (e.g., flow control abated, non-blocking connection
  /// complete).
  ACE_Handle_Set wr_mask_;

  /// Exception events (e.g., SIG_URG).
  ACE_Handle_Set ex_mask_;
};

/**
 * @class ACE_Event_Tuple
 *
 * @brief An ACE_Event_Handler and its associated ACE_HANDLE.
 *
 * One <ACE_Event_Handler> is registered for one or more
 * <ACE_HANDLE>.  At various points, this information must be
 * stored explicitly.  This class provides a lightweight
 * mechanism to do so.
 */
class ACE_Export ACE_Event_Tuple
{
public:
  /// Default constructor.
  ACE_Event_Tuple (void);

  /// Constructor.
  ACE_Event_Tuple (ACE_Event_Handler *eh,
                   ACE_HANDLE h);

  /// Destructor.
  ~ACE_Event_Tuple (void);

  /// Equality operator.
  int operator== (const ACE_Event_Tuple &rhs) const;

  /// Inequality operator.
  int operator!= (const ACE_Event_Tuple &rhs) const;

  /// Handle.
  ACE_HANDLE handle_;

  /// <ACE_Event_Handler> associated with the <ACE_HANDLE>.
  ACE_Event_Handler *event_handler_;
};

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
  /// Default dtor.
  ACE_Select_Reactor_Notify (void);
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
  virtual ssize_t notify (ACE_Event_Handler * = 0,
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

/**
 * @class ACE_Select_Reactor_Handler_Repository
 *
 * @brief Used to map <ACE_HANDLE>s onto the appropriate
 * <ACE_Event_Handler> *.
 *
 * This class is necessary to shield differences between UNIX
 * and Win32.  In UNIX, <ACE_HANDLE> is an int, whereas in Win32
 * it's a void *.  This class hides all these details from the
 * bulk of the <ACE_Select_Reactor> code.  All of these methods
 * are called with the main <Select_Reactor> token lock held.
 */
class ACE_Export ACE_Select_Reactor_Handler_Repository
{
public:
  friend class ACE_Select_Reactor_Handler_Repository_Iterator;

  // = Initialization and termination methods.
  /// Default "do-nothing" constructor.
  ACE_Select_Reactor_Handler_Repository (ACE_Select_Reactor_Impl &);

  /// dtor.
  ~ACE_Select_Reactor_Handler_Repository (void);

  /// Initialize a repository of the appropriate <size>.
  /**
   * On Unix platforms, the size parameter should be as large as the
   * maximum number of file descriptors allowed for a given process.
   * This is necessary since a file descriptor is used to directly
   * index the array of event handlers maintained by the Reactor's
   * handler repository.  Direct indexing is used for efficiency
   * reasons.
   */
  int open (size_t size);

  /// Close down the repository.
  int close (void);

  // = Search structure operations.

  /**
   * Return the <ACE_Event_Handler *> associated with <ACE_HANDLE>.
   * If <index_p> is non-0, then return the index location of the
   * <handle>, if found.
   */
  ACE_Event_Handler *find (ACE_HANDLE handle, size_t *index_p = 0);

  /// Bind the <ACE_Event_Handler *> to the <ACE_HANDLE> with the
  /// appropriate <ACE_Reactor_Mask> settings.
  int bind (ACE_HANDLE,
            ACE_Event_Handler *,
            ACE_Reactor_Mask);

  /// Remove the binding of <ACE_HANDLE> in accordance with the <mask>.
  int unbind (ACE_HANDLE,
              ACE_Reactor_Mask mask);

  /// Remove all the <ACE_HANDLE, ACE_Event_Handler> tuples.
  int unbind_all (void);

  // = Sanity checking.

  // Check the <handle> to make sure it's a valid ACE_HANDLE that
  // within the range of legal handles (i.e., >= 0 && < max_size_).
  int invalid_handle (ACE_HANDLE handle);

  // Check the <handle> to make sure it's a valid ACE_HANDLE that
  // within the range of currently registered handles (i.e., >= 0 && <
  // max_handlep1_).
  int handle_in_range (ACE_HANDLE handle);

  // = Accessors.
  /// Returns the current table size.
  size_t size (void) const;

  /// Maximum ACE_HANDLE value, plus 1.
  size_t max_handlep1 (void);

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

private:
  /// Reference to our <Select_Reactor>.
  ACE_Select_Reactor_Impl &select_reactor_;

  /// Maximum number of handles.
  ssize_t max_size_;

  /// The highest currently active handle, plus 1 (ranges between 0 and
  /// <max_size_>.
  int max_handlep1_;

#if defined (ACE_WIN32)
  // = The mapping from <HANDLES> to <Event_Handlers>.

  /**
   * The NT version implements this via a dynamically allocated
   * array of <ACE_Event_Tuple *>.  Since NT implements ACE_HANDLE
   * as a void * we can't directly index into this array.  Therefore,
   * we just do a linear search (for now).  Next, we'll modify
   * things to use hashing or something faster...
   */
  ACE_Event_Tuple *event_handlers_;
#else
  /**
   * The UNIX version implements this via a dynamically allocated
   * array of <ACE_Event_Handler *> that is indexed directly using
   * the ACE_HANDLE value.
   */
  ACE_Event_Handler **event_handlers_;
#endif /* ACE_WIN32 */
};

/**
 * @class ACE_Select_Reactor_Handler_Repository_Iterator
 *
 * @brief Iterate through the <ACE_Select_Reactor_Handler_Repository>.
 */
class ACE_Export ACE_Select_Reactor_Handler_Repository_Iterator
{
public:
  // = Initialization method.
  ACE_Select_Reactor_Handler_Repository_Iterator (const ACE_Select_Reactor_Handler_Repository *s);

  /// dtor.
  ~ACE_Select_Reactor_Handler_Repository_Iterator (void);

  // = Iteration methods.

  /// Pass back the <next_item> that hasn't been seen in the Set.
  /// Returns 0 when all items have been seen, else 1.
  int next (ACE_Event_Handler *&next_item);

  /// Returns 1 when all items have been seen, else 0.
  int done (void) const;

  /// Move forward by one element in the set.  Returns 0 when all the
  /// items in the set have been seen, else 1.
  int advance (void);

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

private:
  /// Reference to the Handler_Repository we are iterating over.
  const ACE_Select_Reactor_Handler_Repository *rep_;

  /// Pointer to the current iteration level.
  ssize_t current_;
};

/**
 * @class ACE_Select_Reactor_Impl
 *
 * @brief This class simply defines how Select_Reactor's basic interface
 * functions should look like and provides a common base class for
 * <Select_Reactor> using various locking mechanism.
 */
class ACE_Export ACE_Select_Reactor_Impl : public ACE_Reactor_Impl
{
public:
  enum
  {
    /// Default size of the Select_Reactor's handle table.
    DEFAULT_SIZE = ACE_DEFAULT_SELECT_REACTOR_SIZE
  };

  /// Constructor.
  ACE_Select_Reactor_Impl (void);

  friend class ACE_Select_Reactor_Notify;
  friend class ACE_Select_Reactor_Handler_Repository;

  /**
   * Purge any notifications pending in this reactor for the specified
   * <ACE_Event_Handler> object. Returns the number of notifications
   * purged. Returns -1 on error.
   */
  virtual int purge_pending_notifications (ACE_Event_Handler * = 0,
                                           ACE_Reactor_Mask    = ACE_Event_Handler::ALL_EVENTS_MASK);

  /// Does the reactor allow the application to resume the handle on
  /// its own ie. can it pass on the control of handle resumption to
  /// the application.  The select reactor has no handlers that can be
  /// resumed by the  application. So return 0;
  virtual int resumable_handler (void);

protected:
  /// Allow manipulation of the <wait_set_> mask and <ready_set_> mask.
  virtual int bit_ops (ACE_HANDLE handle,
                       ACE_Reactor_Mask mask,
                       ACE_Select_Reactor_Handle_Set &wait_Set,
                       int ops);

  /// Enqueue ourselves into the list of waiting threads at the
  /// appropriate point specified by <requeue_position_>.
  virtual void renew (void) = 0;

  /// Table that maps <ACE_HANDLEs> to <ACE_Event_Handler *>'s.
  ACE_Select_Reactor_Handler_Repository handler_rep_;

  /// Tracks handles that are waited for by <select>.
  ACE_Select_Reactor_Handle_Set wait_set_;

  /// Tracks handles that are currently suspended.
  ACE_Select_Reactor_Handle_Set suspend_set_;

  /// Track HANDLES we are interested in for various events that must
  /// be dispatched *without* going through <select>.
  ACE_Select_Reactor_Handle_Set ready_set_;

  /// Defined as a pointer to allow overriding by derived classes...
  ACE_Timer_Queue *timer_queue_;

  /// Keeps track of whether we should delete the timer queue (if we
  /// didn't create it, then we don't delete it).
  int delete_timer_queue_;

  /// Handle signals without requiring global/static variables.
  ACE_Sig_Handler *signal_handler_;

  /// Keeps track of whether we should delete the signal handler (if we
  /// didn't create it, then we don't delete it).
  int delete_signal_handler_;

  /// Callback object that unblocks the <ACE_Select_Reactor> if it's
  /// sleeping.
  ACE_Reactor_Notify *notify_handler_;

  /// Keeps track of whether we need to delete the notify handler (if
  /// we didn't create it, then we don't delete it).
  int delete_notify_handler_;

  /// Restart the <handle_events> event-loop method automatically when
  /// <select> is interrupted via <EINTR>.
  int restart_;

  /**
   * Position that the main ACE_Select_Reactor thread is requeued in
   * the list of waiters during a <notify> callback.  If this value ==
   * -1 we are requeued at the end of the list.  Else if it's 0 then
   * we are requeued at the front of the list.  Else if it's > 1 then
   * that indicates the number of waiters to skip over.
   */
  int requeue_position_;

  /// True if we've been initialized yet...
  int initialized_;

  /// The original thread that created this Select_Reactor.
  ACE_thread_t owner_;

  /**
   * True if state has changed during dispatching of
   * <ACE_Event_Handlers>, else false.  This is used to determine
   * whether we need to make another trip through the
   * <Select_Reactor>'s <wait_for_multiple_events> loop.
   */
  int state_changed_;

  /// Controls/access whether the notify handler should renew the
  /// Select_Reactor's token or not.
  int supress_notify_renew (void);
  void supress_notify_renew (int sr);

private:
  /// Determine whether we should renew Select_Reactor's token after handling
  /// the notification message.
  int supress_renew_;

  /// Deny access since member-wise won't work...
  ACE_Select_Reactor_Impl (const ACE_Select_Reactor_Impl &);
  ACE_Select_Reactor_Impl &operator = (const ACE_Select_Reactor_Impl &);
};

#if defined (__ACE_INLINE__)
#include "ace/Select_Reactor_Base.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ACE_SELECT_REACTOR_BASE_H */
