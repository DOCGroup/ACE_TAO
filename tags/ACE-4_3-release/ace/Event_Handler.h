/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    Event_Handler.h
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (ACE_EVENT_HANDLER_H)
#define ACE_EVENT_HANDLER_H

#include "ace/ACE.h"

// Forward declaration.
class ACE_Message_Block;
class ACE_Reactor;
class ACE_ReactorEx;
class ACE_Proactor;

typedef u_long ACE_Reactor_Mask;

class ACE_Export ACE_Event_Handler 
  // = TITLE
  //     Provides an abstract interface for handling various types of
  //     I/O, timer, and signal events.
  //
  // = DESCRIPTION
  //       Subclasses read/write input/output on an I/O descriptor,
  //       handle an exception raised on an I/O descriptor, handle a
  //       timer's expiration, or handle a signal.
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
    READ_MASK = 0x1,
    WRITE_MASK = 0x4,
    EXCEPT_MASK = 0x2,
#endif /* ACE_USE_POLL */
    ACCEPT_MASK = 0x8,
    CONNECT_MASK = READ_MASK | WRITE_MASK
#if defined (ACE_WIN32)
    | EXCEPT_MASK
#endif /* ACE_WIN32 */
    ,
    TIMER_MASK = 0x10,
    QOS_MASK = 0x20,
    GROUP_QOS_MASK = 0x40,
    CLOSE_MASK = 0x80,
    ALL_EVENTS_MASK = READ_MASK | WRITE_MASK | EXCEPT_MASK | ACCEPT_MASK | QOS_MASK | GROUP_QOS_MASK | CLOSE_MASK,
    RWE_MASK = READ_MASK | WRITE_MASK | EXCEPT_MASK,
    DONT_CALL = 0x100
  };

  virtual ~ACE_Event_Handler (void);
  // Destructor is virtual to enable proper cleanup.

  virtual ACE_HANDLE  get_handle (void) const;
  // Get the I/O handle.
  virtual void set_handle (ACE_HANDLE);
  // Set the I/O handle.

  // = Get/set priority 

  // Priorities run from MIN_PRIORITY (which is the "lowest priority")
  // to MAX_PRIORITY (which is the "highest priority").
  virtual int priority (void) const;
  // Get the priority of the Event_Handler.
  virtual void priority (int priority);
  // Set the priority of the Event_Handler.

  virtual int handle_input (ACE_HANDLE fd = ACE_INVALID_HANDLE);
  // Called when input events occur (e.g., connection or data).

  virtual int handle_output (ACE_HANDLE fd = ACE_INVALID_HANDLE);
  // Called when output events are possible (e.g., flow control
  // abates).

  virtual int handle_exception (ACE_HANDLE fd = ACE_INVALID_HANDLE);
  // Called when execption events occur (e.g., SIGURG).

  virtual int handle_timeout (const ACE_Time_Value &tv,
			      const void *arg = 0);
  // Called when timer expires.

  virtual int handle_close (ACE_HANDLE handle,
			    ACE_Reactor_Mask close_mask);
  // Called when object is removed from the ACE_Reactor

  virtual int handle_signal (int signum, siginfo_t * = 0, ucontext_t * = 0);
  // Called when object is signaled by OS (either via UNIX signals or
  // when a Win32 object becomes signaled).

  virtual int handle_qos (ACE_HANDLE = ACE_INVALID_HANDLE);
  virtual int handle_group_qos (ACE_HANDLE = ACE_INVALID_HANDLE);

  // = Accessors to set/get the various event demultiplexors.
  virtual void reactor (ACE_Reactor *reactor);
  virtual ACE_Reactor *reactor (void) const;

  virtual void reactorEx (ACE_ReactorEx *reactorex);
  virtual ACE_ReactorEx *reactorEx (void) const;

  virtual void proactor (ACE_Proactor *proactor);
  virtual ACE_Proactor *proactor (void) const;

protected:
  ACE_Event_Handler (void);
  // Force ACE_Event_Handler to be an abstract base class.

  int priority_;
  // Priority of this Event_Handler.

  // = Pointers to the various event demultiplexors.  
  ACE_Reactor *reactor_;
  ACE_ReactorEx *reactorex_;
  ACE_Proactor *proactor_;
};

struct ACE_Export ACE_Notification_Buffer
  // = TITLE
  //     Simple wrapper for passing <ACE_Event_Handler *>s and
  //     <ACE_Reactor_Mask>s between threads.
{
  ACE_Notification_Buffer (void);

  ACE_Notification_Buffer (ACE_Event_Handler *eh,
			   ACE_Reactor_Mask mask);

  ACE_Event_Handler *eh_;
  // Pointer to the Event_Handler that will be dispatched 
  // by the main event loop.
  
  ACE_Reactor_Mask mask_;
  // Mask that indicates which method to call.
};

#if defined (__ACE_INLINE__)
#include "ace/Event_Handler.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_EVENT_HANDLER_H */
