/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Event_Handler.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt <schmidt@cs.wustl.edu>
 */
//=============================================================================

#ifndef ACE_EVENT_HANDLER_H
#define ACE_EVENT_HANDLER_H
#include "ace/pre.h"

#include "ace/ACE.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// Forward declaration.
class ACE_Message_Block;
class ACE_Reactor;
class ACE_Thread_Manager;
class ACE_Process;

typedef u_long ACE_Reactor_Mask;

/**
 * @class ACE_Event_Handler
 *
 * @brief Provides an abstract interface for handling various types of
 * I/O, timer, and signal events.
 *
 * Subclasses read/write input/output on an I/O descriptor,
 * handle an exception raised on an I/O descriptor, handle a
 * timer's expiration, or handle a signal.
 */
class ACE_Export ACE_Event_Handler
{
public:
  enum
  {
    LO_PRIORITY = 0,
    HI_PRIORITY = 10,
    NULL_MASK = 0,
#if defined (ACE_USE_POLL)
    READ_MASK = POLLIN,
    WRITE_MASK = POLLOUT,
    EXCEPT_MASK = POLLPRI,
#else /* USE SELECT */
    READ_MASK = (1 << 0),
    WRITE_MASK = (1 << 1),
    EXCEPT_MASK = (1 << 2),
#endif /* ACE_USE_POLL */
    ACCEPT_MASK = (1 << 3),
    CONNECT_MASK = (1 << 4),
    TIMER_MASK = (1 << 5),
    QOS_MASK = (1 << 6),
    GROUP_QOS_MASK = (1 << 7),
    SIGNAL_MASK = (1 << 8),
    ALL_EVENTS_MASK = READ_MASK |
                      WRITE_MASK |
                      EXCEPT_MASK |
                      ACCEPT_MASK |
                      CONNECT_MASK |
                      TIMER_MASK |
                      QOS_MASK |
                      GROUP_QOS_MASK |
                      SIGNAL_MASK,
    RWE_MASK = READ_MASK |
               WRITE_MASK |
               EXCEPT_MASK,
    DONT_CALL = (1 << 9)
  };

  /// Destructor is virtual to enable proper cleanup.
  virtual ~ACE_Event_Handler (void);

  /// Get the I/O handle.
  /// Set the I/O handle.
  virtual ACE_HANDLE get_handle (void) const;
  virtual void set_handle (ACE_HANDLE);

  // = Get/set priority

  // Priorities run from MIN_PRIORITY (which is the "lowest priority")
  // to MAX_PRIORITY (which is the "highest priority").
  /// Get the priority of the Event_Handler.
  /// Set the priority of the Event_Handler.
  virtual int priority (void) const;
  virtual void priority (int priority);

  /// Called when input events occur (e.g., connection or data).
  virtual int handle_input (ACE_HANDLE fd = ACE_INVALID_HANDLE);

  /// Called when output events are possible (e.g., when flow control
  /// abates or non-blocking connection completes).
  virtual int handle_output (ACE_HANDLE fd = ACE_INVALID_HANDLE);

  /// Called when an exceptional events occur (e.g., SIGURG).
  virtual int handle_exception (ACE_HANDLE fd = ACE_INVALID_HANDLE);

  /**
   * Called when timer expires.  <current_time> represents the current
   * time that the <Event_Handler> was selected for timeout
   * dispatching and <act> is the asynchronous completion token that
   * was passed in when <schedule_timer> was invoked.
   */
  virtual int handle_timeout (const ACE_Time_Value &current_time,
                              const void *act = 0);

  /// Called when a process exits.
  virtual int handle_exit (ACE_Process *);

  /// Called when a <handle_*()> method returns -1 or when the
  /// <remove_handler> method is called on an <ACE_Reactor>.  The
  /// <close_mask> indicates which event has triggered the
  /// <handle_close> method callback on a particular <handle>.
  virtual int handle_close (ACE_HANDLE handle,
                            ACE_Reactor_Mask close_mask);

  /// Called when object is signaled by OS (either via UNIX signals or
  /// when a Win32 object becomes signaled).
  virtual int handle_signal (int signum, siginfo_t * = 0, ucontext_t * = 0);

  /// Called to figure out whether the handler needs to resumed by the
  /// reactor or the application can take care of it. The default
  /// value of 0 would be returned which would allow the reactor to
  /// take care of resumption of the handler. The application can
  /// return a value more than zero and decide to resume the handler
  /// themseleves.
  virtual int resume_handler (void);

  virtual int handle_qos (ACE_HANDLE = ACE_INVALID_HANDLE);
  virtual int handle_group_qos (ACE_HANDLE = ACE_INVALID_HANDLE);

  // = Accessors to set/get the various event demultiplexors.
  virtual void reactor (ACE_Reactor *reactor);
  virtual ACE_Reactor *reactor (void) const;

#if !defined (ACE_HAS_WINCE)
  /**
   * Used to read from non-socket ACE_HANDLEs in our own thread to
   * work around Win32 limitations that don't allow us to <select> on
   * non-sockets (such as ACE_STDIN).  This is commonly used in
   * situations where the Reactor is used to demultiplex read events
   * on ACE_STDIN on UNIX.  Note that <event_handler> must be a
   * subclass of <ACE_Event_Handler>.  If the <get_handle> method of
   * this event handler returns <ACE_INVALID_HANDLE> we default to
   * reading from ACE_STDIN.
   */
  static void *read_adapter (void *event_handler);

  /**
   * Abstracts away from the differences between Win32 and ACE with
   * respect to reading from ACE_STDIN, which is non-<select>'able on
   * Win32.
   */
  static int register_stdin_handler (ACE_Event_Handler *eh,
                                     ACE_Reactor *reactor,
                                     ACE_Thread_Manager *thr_mgr,
                                     int flags = THR_DETACHED);

  /// Performs the inverse of the <register_stdin_handler> method.
  static int remove_stdin_handler (ACE_Reactor *reactor,
                                   ACE_Thread_Manager *thr_mgr);
#endif /* ACE_HAS_WINCE */

protected:
  /// Force ACE_Event_Handler to be an abstract base class.
  ACE_Event_Handler (ACE_Reactor * = 0,
                     int priority = ACE_Event_Handler::LO_PRIORITY);

private:

  /// Priority of this Event_Handler.
  int priority_;

  // = Pointers to the various event demultiplexors.
  ACE_Reactor *reactor_;
};

/**
 * @class ACE_Notification_Buffer
 *
 * @brief Simple wrapper for passing <ACE_Event_Handler *>s and
 * <ACE_Reactor_Mask>s between threads.
 */
class ACE_Export ACE_Notification_Buffer
{
public:
  ACE_Notification_Buffer (void);

  ACE_Notification_Buffer (ACE_Event_Handler *eh,
                           ACE_Reactor_Mask mask);

  /// Default dtor.
  ~ACE_Notification_Buffer (void);

  /// Pointer to the Event_Handler that will be dispatched
  /// by the main event loop.
  ACE_Event_Handler *eh_;

  /// Mask that indicates which method to call.
  ACE_Reactor_Mask mask_;
};

#if defined (__ACE_INLINE__)
#include "ace/Demux/Event_Handler.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ACE_EVENT_HANDLER_H */
