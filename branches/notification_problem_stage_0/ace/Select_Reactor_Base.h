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
#include "ace/Reactor_Impl.h"
#include "Select_Reactor_Handler_Repository.h"

// Add useful typedefs to simplify the following code.
typedef void (ACE_Handle_Set::*ACE_FDS_PTMF) (ACE_HANDLE);
typedef int (ACE_Event_Handler::*ACE_EH_PTMF) (ACE_HANDLE);

#if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0)
typedef ACE_Token ACE_SELECT_TOKEN;
#else
typedef ACE_Noop_Token ACE_SELECT_TOKEN;
#endif /* ACE_MT_SAFE && ACE_MT_SAFE != 0 */

// Forward declaration.
class ACE_Select_Reactor_Notify;

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


//=====================================================================

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
