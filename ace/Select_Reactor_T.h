/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Select_Reactor_T.h
//
// = DESCRIPTION
//    Implementations of Select_Reactor and the Token used by Select_Reactor. 
//
// = AUTHOR
//    Doug Schmidt
//
// ============================================================================

#if !defined (ACE_SELECT_REACTOR_T_H)
#define ACE_SELECT_REACTOR_T_H

#include "ace/Select_Reactor_Base.h"

// Forward declaration.
//template class ACE_Select_Reactor_T<cla

template <class ACE_SELECT_REACTOR_MUTEX>
class ACE_Select_Reactor_Token_T : public ACE_SELECT_REACTOR_MUTEX
{
  // = TITLE
  //     Used as a synchronization mechanism to coordinate concurrent
  //     access to a Select_Reactor object.
  //
  // = DESCRIPTION
  //     This class is used to make the <ACE_Select_Reactor>
  //     thread-safe. By default, the thread that runs the
  //     <handle_events> loop holds the token, even when it is blocked
  //     in the <select> call.  Whenever another thread wants to
  //     access the <ACE_Reactor> via its <register_handler>,
  //     <remove_handler>, etc. methods) it must ask the token owner
  //     for temporary release of the token.  To accomplish this, the
  //     owner of a token must define a <sleep_hook> through which it
  //     can be notified to temporarily release the token if the
  //     current situation permits this.
  //
  //     The owner of the token is responsible for deciding which
  //     request for the token can be granted.  By using the
  //     <ACE_Token::renew> API, the thread that releases the token
  //     temporarily can specify to get the token back right after the
  //     other thread has completed using the token.  Thus, there is a
  //     dedicated thread that owns the token ``by default.''  This
  //     thread grants other threads access to the token by ensuring
  //     that whenever somebody else has finished using the token the
  //     ``default owner'' first holds the token again, i.e., the
  //     owner has the chance to schedule other threads.
  //
  //     The thread that most likely needs the token most of the time
  //     is the thread running the dispatch loop.  Typically the token
  //     gets released prior to entering the <select> call and gets
  //     ``re-acquired'' as soon as the <select> call returns, which
  //     results probably in many calls to <release>/<acquire> that
  //     are not really needed since no other thread would need the
  //     token in the meantime.  That's why the dispatcher thread is
  //     chosen to be the owner of the token.
  //
  //     In case the token would have been released while in <select>
  //     there would be a good chance that the <fd_set> could have
  //     been modified while the <select> returns from blocking and
  //     trying to re-acquire the lock.  Through the token mechanism
  //     it is ensured that while another thread is holding the token,
  //     the dispatcher thread is blocked in the <renew> call and not
  //     in <select>.  Thus, it is not critical to change the
  //     <fd_set>.  The implementation of the <sleep_hook> mechanism
  //     provided by the <ACE_Select_Reactor_Token> enables the
  //     default owner to be the thread that executes the dispatch
  //     loop.
public:
  ACE_Select_Reactor_Token_T (ACE_Select_Reactor_Impl &r);
  ACE_Select_Reactor_Token_T (void);
  virtual ~ACE_Select_Reactor_Token_T (void);

  virtual void sleep_hook (void);
  // Called just before the ACE_Event_Handler goes to sleep.

  ACE_Select_Reactor_Impl &select_reactor (void);
  void select_reactor (ACE_Select_Reactor_Impl &);
  // Set/Get methods

  virtual void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  ACE_Select_Reactor_Impl *select_reactor_;
};

template <class ACE_SELECT_REACTOR_TOKEN>
class ACE_Select_Reactor_T : public ACE_Select_Reactor_Impl
{
  // = TITLE
  //     An object oriented event demultiplexor and event handler
  //     dispatcher.
  //
  // = DESCRIPTION
  //     The ACE_Select_Reactor is an object-oriented event
  //     demultiplexor and event handler dispatcher.  The sources of
  //     events that the ACE_Select_Reactor waits for and dispatches
  //     includes I/O events, signals, and timer events.  All public
  //     methods acquire the main <Select_Reactor> token lock and call
  //     down to private or protected methods, which assume that the
  //     lock is held and so therefore don't (re)acquire the lock.
public:
  // = Initialization and termination methods.

  ACE_Select_Reactor_T (ACE_Sig_Handler * = 0,
                        ACE_Timer_Queue * = 0,
                        int disable_notify_pipe = 0,
                        ACE_Reactor_Notify *notify = 0);
  // Initialize <ACE_Select_Reactor> with the default size.

  ACE_Select_Reactor_T (size_t size,
                        int restart = 0,
                        ACE_Sig_Handler * = 0,
                        ACE_Timer_Queue * = 0,
                        int disable_notify_pipe = 0,
                        ACE_Reactor_Notify *notify = 0);
  // Initialize <ACE_Select_Reactor> with size <size>.

  virtual int open (size_t size = DEFAULT_SIZE,
                    int restart = 0,
                    ACE_Sig_Handler * = 0,
                    ACE_Timer_Queue * = 0,
                    int disable_notify_pipe = 0,
                    ACE_Reactor_Notify * = 0);
  // Initialize <ACE_Select_Reactor> with size <size>.

  virtual int close (void);
  // Close down the select_reactor and release all of its resources.

  virtual ~ACE_Select_Reactor_T (void);
  // Close down the select_reactor and release all of its resources.

  // = Event loop drivers.

  virtual int handle_events (ACE_Time_Value *max_wait_time = 0);

  // = Register and remove <ACE_Event_Handler>s.
  virtual int register_handler (ACE_Event_Handler *eh,
                                ACE_Reactor_Mask mask);
  // Register a <eh> with a particular <mask>.  Note that the
  // <Select_Reactor> will call eh->get_handle() to extract the
  // underlying I/O handle.

  virtual int register_handler (ACE_HANDLE handle,
                                ACE_Event_Handler *eh,
                                ACE_Reactor_Mask mask);
  // Register a <eh> with a particular <mask>.  Note that since the
  // <handle> is given the Select_Reactor will *not* call
  // eh->get_handle() to extract the underlying I/O handle.

  virtual int register_handler (const ACE_Handle_Set &handles,
                                ACE_Event_Handler *eh,
                                ACE_Reactor_Mask mask);
  // Register <eh> with all the <handles> in the <Handle_Set>.

  virtual int remove_handler (ACE_Event_Handler *eh,
                              ACE_Reactor_Mask mask);
  // Removes the <mask> binding of <eh> from the Select_Reactor.  If
  // there are no more bindings for this <eh> then it is removed from
  // the Select_Reactor.  Note that the Select_Reactor will call
  // eh->get_handle() to extract the underlying I/O handle.

  virtual int remove_handler (ACE_HANDLE handle,
                              ACE_Reactor_Mask);
  // Removes the <mask> bind of <Event_Handler> whose handle is
  // <handle> from the Select_Reactor.  If there are no more bindings
  // for this <eh> then it is removed from the Select_Reactor.

  virtual int remove_handler (const ACE_Handle_Set &handle_set,
                              ACE_Reactor_Mask);
  // Removes all the <mask> bindings for handles in the <handle_set>
  // bind of <Event_Handler>.  If there are no more bindings for any
  // of these handlers then they are removed from the Select_Reactor.

  virtual int suspend_handler (ACE_HANDLE handle);
  // Temporarily suspend the <Event_Handler> associated with <handle>.

  virtual int suspend_handler (const ACE_Handle_Set &handles);
  // Suspend all <handles> in handle set temporarily.

  virtual int suspend_handlers (void);
  // Suspend all the <Event_Handlers> in the Select_Reactor.

  virtual int resume_handler (ACE_HANDLE handle);
  // Resume a temporarily suspended <Event_Handler> associated with
  // <handle>.

  virtual int resume_handler (const ACE_Handle_Set &handles);
  // Resume all <handles> in handle set.

  virtual int resume_handlers (void);
  // Resume all the <Event_Handlers> in the Select_Reactor.

  // = Timer management.
  virtual long schedule_timer (ACE_Event_Handler *,
                               const void *arg,
                               const ACE_Time_Value &delta_time,
                               const ACE_Time_Value &interval = ACE_Time_Value::zero);
  // Schedule an <event_handler> that will expire after <delta_time>
  // amount of time.  If it expires then <arg> is passed in as the
  // value to the <event_handler>'s <handle_timeout> callback method.
  // If <interval> is != to <ACE_Time_Value::zero> then it is used to
  // reschedule the <event_handler> automatically.  This method
  // returns a <timer_id> that uniquely identifies the <event_handler>
  // in an internal list.  This <timer_id> can be used to cancel an
  // <event_handler> before it expires.  The cancellation ensures that
  // <timer_ids> are unique up to values of greater than 2 billion
  // timers.  As long as timers don't stay around longer than this
  // there should be no problems with accidentally deleting the wrong
  // timer.  Returns -1 on failure (which is guaranteed never to be a
  // valid <timer_id>.

  // = Notification methods.

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

  virtual void requeue_position (int);
  // Set position that the main ACE_Select_Reactor thread is requeued in the
  // list of waiters during a notify() callback.

  virtual int requeue_position (void);
  // Get position that the main ACE_Select_Reactor thread is requeued in the
  // list of waiters during a notify() callback.

  virtual int mask_ops (ACE_HANDLE handle,
                        ACE_Reactor_Mask mask,
                        int ops);
  // GET/SET/ADD/CLR the dispatch MASK "bit" bound with the <handle>
  // and <mask>.

  virtual int ready_ops (ACE_HANDLE handle,
                         ACE_Reactor_Mask,
                         int ops);
  // GET/SET/ADD/CLR the ready "bit" bound with the <handle> and <mask>.

  // = Only the owner thread that can perform a <handle_events>.

  virtual int owner (ACE_thread_t n_id, ACE_thread_t *o_id = 0);
  // Set the new owner of the thread and return the old owner.

  virtual int owner (ACE_thread_t *);
  // Return the current owner of the thread.

  // = Miscellaneous Handler operations.
  virtual int handler (ACE_HANDLE handle,
                       ACE_Reactor_Mask mask,
                       ACE_Event_Handler **eh = 0);
  // Check to see if <handle> is associated with a valid Event_Handler
  // bound to <mask>.  Return the <eh> associated with this <handler>
  // if <eh> != 0.

  virtual int initialized (void);
  // Returns true if we've been successfully initialized, else false.

  virtual ACE_Lock &lock (void);
  // Returns a reference to the <ACE_Select_Reactor_Token> that is
  // used to serialize the internal Select_Reactor's processing logic.
  // This can be useful for situations where you need to avoid
  // deadlock efficiently when <ACE_Event_Handlers> are used in
  // multiple threads.

  virtual void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  virtual void renew (void);
  // Enqueue ourselves into the list of waiting threads at the
  // appropriate point specified by <requeue_position_>.

  virtual int release_token (void);
  // Release the token lock when a Win32 structured exception occurs.

  ACE_SELECT_REACTOR_TOKEN token_;
  // Synchronization token for the MT_SAFE ACE_Select_Reactor.

  ACE_Lock_Adapter<ACE_SELECT_REACTOR_TOKEN> lock_adapter_;
  // Adapter used to return internal lock to outside world.

private:
  ACE_Select_Reactor_T (const ACE_Select_Reactor_T &);
  ACE_Select_Reactor_T &operator = (const ACE_Select_Reactor_T &);
  // Deny access since member-wise won't work...
};

#if defined (__ACE_INLINE__)
#include "ace/Select_Reactor_T.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Select_Reactor_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Select_Reactor_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* ACE_SELECT_REACTOR_T_H */
