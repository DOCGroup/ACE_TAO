/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Select_Reactor_Base.h
//
// = AUTHOR
//    Doug Schmidt
//
// ============================================================================

#ifndef ACE_SELECT_REACTOR_BASE_H
#define ACE_SELECT_REACTOR_BASE_H

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

// Add useful typedefs to simplify the following code.
typedef void (ACE_Handle_Set::*ACE_FDS_PTMF) (ACE_HANDLE);
typedef int (ACE_Event_Handler::*ACE_EH_PTMF) (ACE_HANDLE);

// Forward declaration.
class ACE_Select_Reactor_Impl;

class ACE_Export ACE_Select_Reactor_Handle_Set
{
  // = TITLE
  //      Track handles we are interested for various events.
public:
  ACE_Handle_Set rd_mask_;
  // Read events (e.g., input pending, accept pending).

  ACE_Handle_Set wr_mask_;
  // Write events (e.g., flow control abated, non-blocking connection
  // complete).

  ACE_Handle_Set ex_mask_;
  // Exception events (e.g., SIG_URG).
};

class ACE_Export ACE_Event_Tuple
{
  // = TITLE
  //     An ACE_Event_Handler and its associated ACE_HANDLE.
  //
  // = DESCRIPTION
  //     One ACE_Event_Handler is registered for one or more
  //     ACE_HANDLE, in some points this information must be stored
  //     explicitly. This structure provides a lightweight mechanism
  //     to do so.
public:
  ACE_Event_Tuple (void);
  ACE_Event_Tuple (ACE_Event_Handler* eh, ACE_HANDLE h);
  ~ACE_Event_Tuple (void);

  int operator== (const ACE_Event_Tuple &rhs) const;
  // Equality operator.

  int operator!= (const ACE_Event_Tuple &rhs) const;
  // Inequality operator.

  ACE_HANDLE handle_;
  ACE_Event_Handler* event_handler_;
};

class ACE_Export ACE_Select_Reactor_Notify : public ACE_Reactor_Notify
{
  // = TITLE
  //     Unblock the <ACE_Select_Reactor> from its event loop.
  //
  // = DESCRIPTION
  //     This implementation is necessary for cases where the
  //     <ACE_Select_Reactor> is run in a multi-threaded program.  In
  //     this case, we need to be able to unblock select() or poll()
  //     when updates occur other than in the main
  //     <ACE_Select_Reactor> thread.  To do this, we signal an
  //     auto-reset event the <ACE_Select_Reactor> is listening on.
  //     If an <ACE_Event_Handler> and <ACE_Select_Reactor_Mask> is
  //     passed to <notify>, the appropriate <handle_*> method is
  //     dispatched in the context of the <ACE_Select_Reactor> thread.
public:
  ACE_Select_Reactor_Notify (void);
  ~ACE_Select_Reactor_Notify (void);
  // Default dtor.

  // = Initialization and termination methods.
  virtual int open (ACE_Reactor_Impl *,
                    ACE_Timer_Queue * = 0,
                    int disable_notify_pipe = 0);
  // Initialize.

  virtual int close (void);
  // Destroy.

  virtual ssize_t notify (ACE_Event_Handler * = 0,
                          ACE_Reactor_Mask = ACE_Event_Handler::EXCEPT_MASK,
                          ACE_Time_Value * = 0);
  // Called by a thread when it wants to unblock the
  // <ACE_Select_Reactor>.  This wakeups the <ACE_Select_Reactor> if
  // currently blocked in select()/poll().  Pass over both the
  // <Event_Handler> *and* the <mask> to allow the caller to dictate
  // which <Event_Handler> method the <ACE_Select_Reactor> will
  // invoke.  The <ACE_Time_Value> indicates how long to blocking
  // trying to notify the <ACE_Select_Reactor>.  If <timeout> == 0,
  // the caller will block until action is possible, else will wait
  // until the relative time specified in *<timeout> elapses).

  virtual int dispatch_notifications (int &number_of_active_handles,
                                      const ACE_Handle_Set &rd_mask);
  // Handles pending threads (if any) that are waiting to unblock the
  // <ACE_Select_Reactor>.

  virtual int handle_input (ACE_HANDLE handle);
  // Called back by the <ACE_Select_Reactor> when a thread wants to
  // unblock us.

  virtual void max_notify_iterations (int);
  // Set the maximum number of times that the
  // <ACE_Select_Reactor_Notify::handle_input> method will iterate and
  // dispatch the <ACE_Event_Handlers> that are passed in via the
  // notify pipe before breaking out of its <recv> loop.  By default,
  // this is set to -1, which means "iterate until the pipe is empty."
  // Setting this to a value like "1 or 2" will increase "fairness"
  // (and thus prevent starvation) at the expense of slightly higher
  // dispatching overhead.

  virtual int max_notify_iterations (void);
  // Get the maximum number of times that the
  // <ACE_Select_Reactor_Notify::handle_input> method will iterate and
  // dispatch the <ACE_Event_Handlers> that are passed in via the
  // notify pipe before breaking out of its <recv> loop.

  virtual void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  ACE_Select_Reactor_Impl *select_reactor_;
  // Keep a back pointer to the <ACE_Select_Reactor>.  If this value
  // if NULL then the <ACE_Select_Reactor> has been initialized with
  // <disable_notify_pipe>.

  ACE_Pipe notification_pipe_;
  // Contains the <ACE_HANDLE> the <ACE_Select_Reactor> is listening
  // on, as well as the <ACE_HANDLE> that threads wanting the
  // attention of the <ACE_Select_Reactor> will write to.

  int max_notify_iterations_;
  // Keeps track of the maximum number of times that the
  // <ACE_Select_Reactor_Notify::handle_input> method will iterate and
  // dispatch the <ACE_Event_Handlers> that are passed in via the
  // notify pipe before breaking out of its <recv> loop.  By default,
  // this is set to -1, which means "iterate until the pipe is empty."
};

class ACE_Export ACE_Select_Reactor_Handler_Repository
{
  // = TITLE
  //     Used to map <ACE_HANDLE>s onto the appropriate
  //     <ACE_Event_Handler> *.
  //
  // = DESCRIPTION
  //     This class is necessary to shield differences between UNIX
  //     and Win32.  In UNIX, <ACE_HANDLE> is an int, whereas in Win32
  //     it's a void *.  This class hides all these details from the
  //     bulk of the <ACE_Select_Reactor> code.  All of these methods
  //     are called with the main <Select_Reactor> token lock held.
public:
  friend class ACE_Select_Reactor_Handler_Repository_Iterator;

  // = Initialization and termination methods.
  ACE_Select_Reactor_Handler_Repository (ACE_Select_Reactor_Impl &);
  // Default "do-nothing" constructor.

  ~ACE_Select_Reactor_Handler_Repository (void);
  // dtor.

  int open (size_t size);
  // Initialize a repository of the appropriate <size>.

  int close (void);
  // Close down the repository.

  // = Search structure operations.

  ACE_Event_Handler *find (ACE_HANDLE handle, size_t *index_p = 0);
  // Return the <ACE_Event_Handler *> associated with <ACE_HANDLE>.
  // If <index_p> is non-0, then return the index location of the
  // <handle>, if found.

  int bind (ACE_HANDLE,
            ACE_Event_Handler *,
            ACE_Reactor_Mask);
  // Bind the <ACE_Event_Handler *> to the <ACE_HANDLE> with the
  // appropriate <ACE_Reactor_Mask> settings.

  int unbind (ACE_HANDLE,
              ACE_Reactor_Mask mask);
  // Remove the binding of <ACE_HANDLE> in accordance with the <mask>.

  int unbind_all (void);
  // Remove all the <ACE_HANDLE, ACE_Event_Handler> tuples.

  // = Sanity checking.

  // Check the <handle> to make sure it's a valid ACE_HANDLE that
  // within the range of legal handles (i.e., >= 0 && < max_size_).
  int invalid_handle (ACE_HANDLE handle);

  // Check the <handle> to make sure it's a valid ACE_HANDLE that
  // within the range of currently registered handles (i.e., >= 0 && <
  // max_handlep1_).
  int handle_in_range (ACE_HANDLE handle);

  // = Accessors.
  size_t size (void);
  // Returns the current table size.

  size_t max_handlep1 (void);
  // Maximum ACE_HANDLE value, plus 1.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  ACE_Select_Reactor_Impl &select_reactor_;
  // Reference to our <Select_Reactor>.

  ssize_t max_size_;
  // Maximum number of handles.

  int max_handlep1_;
  // The highest currently active handle, plus 1 (ranges between 0 and
  // <max_size_>.

#if defined (ACE_WIN32)
  // = The mapping from <HANDLES> to <Event_Handlers>.

  ACE_Event_Tuple *event_handlers_;
  // The NT version implements this via a dynamically allocated
  // array of <ACE_Event_Tuple *>.  Since NT implements ACE_HANDLE
  // as a void * we can't directly index into this array.  Therefore,
  // we just do a linear search (for now).  Next, we'll modify
  // things to use hashing or something faster...
#else
  ACE_Event_Handler **event_handlers_;
  // The UNIX version implements this via a dynamically allocated
  // array of <ACE_Event_Handler *> that is indexed directly using
  // the ACE_HANDLE value.
#endif /* ACE_WIN32 */
};

class ACE_Export ACE_Select_Reactor_Handler_Repository_Iterator
{
  // = TITLE
  //     Iterate through the <ACE_Select_Reactor_Handler_Repository>.
public:
  // = Initialization method.
  ACE_Select_Reactor_Handler_Repository_Iterator (const ACE_Select_Reactor_Handler_Repository *s);

  ~ACE_Select_Reactor_Handler_Repository_Iterator (void);
  // dtor.

  // = Iteration methods.

  int next (ACE_Event_Handler *&next_item);
  // Pass back the <next_item> that hasn't been seen in the Set.
  // Returns 0 when all items have been seen, else 1.

  int done (void) const;
  // Returns 1 when all items have been seen, else 0.

  int advance (void);
  // Move forward by one element in the set.  Returns 0 when all the
  // items in the set have been seen, else 1.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  const ACE_Select_Reactor_Handler_Repository *rep_;
  // Reference to the Handler_Repository we are iterating over.

  ssize_t current_;
  // Pointer to the current iteration level.
};

class ACE_Export ACE_Select_Reactor_Impl : public ACE_Reactor_Impl
{
  // = TITLE
  //     This class simply defines how Select_Reactor's basic interface
  //     functions should look like and provides a common base class for
  //     Select_Reactor using various locking mechanism.
public:
  enum
  {
    DEFAULT_SIZE = ACE_DEFAULT_SELECT_REACTOR_SIZE
    // Default size of the Select_Reactor's handle table.
  };

  ACE_Select_Reactor_Impl ();

  friend class ACE_Select_Reactor_Notify;
  friend class ACE_Select_Reactor_Handler_Repository;

protected:
  virtual int bit_ops (ACE_HANDLE handle,
                       ACE_Reactor_Mask mask,
                       ACE_Select_Reactor_Handle_Set &wait_Set,
                       int ops);
  // Allow manipulation of the <wait_set_> mask and <ready_set_> mask.

  virtual void renew (void) = 0;
  // Enqueue ourselves into the list of waiting threads at the
  // appropriate point specified by <requeue_position_>.

  ACE_Select_Reactor_Handler_Repository handler_rep_;
  // Table that maps <ACE_HANDLEs> to <ACE_Event_Handler *>'s.

  ACE_Select_Reactor_Handle_Set wait_set_;
  // Tracks handles that are waited for by select().

  ACE_Select_Reactor_Handle_Set suspend_set_;
  // Tracks handles that are currently suspended.

  ACE_Select_Reactor_Handle_Set ready_set_;
  // Track HANDLES we are interested in for various events that must
  // be dispatched *without* going through select().

  ACE_Timer_Queue *timer_queue_;
  // Defined as a pointer to allow overriding by derived classes...

  int delete_timer_queue_;
  // Keeps track of whether we should delete the timer queue (if we
  // didn't create it, then we don't delete it).

  ACE_Sig_Handler *signal_handler_;
  // Handle signals without requiring global/static variables.

  int delete_signal_handler_;
  // Keeps track of whether we should delete the signal handler (if we
  // didn't create it, then we don't delete it).

  ACE_Reactor_Notify *notify_handler_;
  // Callback object that unblocks the <ACE_Select_Reactor> if it's
  // sleeping.

  int delete_notify_handler_;
  // Keeps track of whether we need to delete the notify handler (if
  // we didn't create it, then we don't delete it).

  int restart_;
  // Restart automatically when interrupted

  int requeue_position_;
  // Position that the main ACE_Select_Reactor thread is requeued in
  // the list of waiters during a notify() callback.  If this value ==
  // -1 we are requeued at the end of the list.  Else if it's 0 then
  // we are requeued at the front of the list.  Else if it's > 1 then
  // that indicates the number of waiters to skip over.

  int initialized_;
  // True if we've been initialized yet...

  ACE_thread_t owner_;
  // The original thread that created this Select_Reactor.

  int state_changed_;
  // True if state has changed during dispatching of
  // <ACE_Event_Handlers>, else false.  This is used to determine
  // whether we need to make another trip through the
  // <Select_Reactor>'s <wait_for_multiple_events> loop.

  int supress_notify_renew (void);
  void supress_notify_renew (int sr);
  // Controls/access whether the notify handler should renew the
  // Select_Reactor's token or not.

private:
  int supress_renew_;
  // Determine whether we should renew Select_Reactor's token after handling
  // the notification message.

  ACE_Select_Reactor_Impl (const ACE_Select_Reactor_Impl &);
  ACE_Select_Reactor_Impl &operator = (const ACE_Select_Reactor_Impl &);
  // Deny access since member-wise won't work...
};

#if defined (__ACE_INLINE__)
#include "ace/Select_Reactor_Base.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_SELECT_REACTOR_BASE_H */
