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
  //       Derived classes read/write input/output on an I/O
  //       descriptor, handle an exception raised on an I/O
  //       descriptor, handle a timer's expiration, or handle a
  //       signal.
{
public:
  enum 
  {
    LO_PRIORITY = 0,
    HI_PRIORITY = 10,
    NULL_MASK   = 0,
#if defined (ACE_USE_POLL)
    READ_MASK   = POLLIN,
    WRITE_MASK  = POLLOUT,
    EXCEPT_MASK = POLLPRI,
#else /* USE SELECT */
    READ_MASK   = 0x1,
    WRITE_MASK  = 0x4,
    EXCEPT_MASK = 0x2,
#endif /* ACE_USE_POLL */
    RWE_MASK    = READ_MASK | WRITE_MASK | EXCEPT_MASK,
    DONT_CALL   = 0x100
  };

  virtual ~ACE_Event_Handler (void);
  // Destructor is virtual to enable proper cleanup.

  // = The following methods must be supplied by subclasses in order
  // to specialize the behavior of an Event_Handler.
  virtual ACE_HANDLE  get_handle (void) const;
  // Get the I/O handle.
  virtual void set_handle (ACE_HANDLE);
  // Set the I/O handle.

  // = Priority runs from MIN_PRIORITY (which is the "lowest priority") to MAX_PRIORITY (which is the "highest priority").
  virtual int get_priority (void) const;
  // Get the priority of the Event_Handler.
  virtual void set_priority (int priority);
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

  virtual int handle_close (ACE_HANDLE fd, 
			    ACE_Reactor_Mask close_mask);
  // Called when object is removed from the ACE_Reactor

  virtual int handle_signal (int signum, siginfo_t * = 0, ucontext_t * = 0);
  // Called when object is signaled by OS (either via UNIX signals or
  // when a Win32 object becomes signaled).

  // = <ACE_Proactor> callbacks.  These are Win32 specific.  An Event_Handler can be given to a Proactor with a {RECV,SEND}_MASK.  The Proactor calls back <get_message> and <get_handle> to perform the correct operations (send/recv).  When the send/recv is complete, handle_{input,output} is called.  Thus, Event_Handlers are used for "proactive I/O" where they are told WHEN THE OPERATION IS COMPLETE.  Alternatively, the _Reactor_ tells Event_Handlers WHEN THE OPERATION CAN BE PERFORMED.

  virtual int handle_input_complete (ACE_Message_Block *message,
				     long bytes_transferred);
  // Called back by the <ACE_Proactor> when an asynchronous input
  // operation is complete.  If this method returns > 0 the Proactor
  // will initiate another asynchronous receive; if a 0 is returned
  // the Proactor will not reinitiate a receive; and if a -1 is
  // returned, then no receive is reinitiated and <handle_close> is
  // called.  If <bytes_transferred> >= 0, then the I/O operation
  // completed successfully.  If <bytes_transferred> == -1, then the
  // I/O operation failed.  Check <message> for total bytes received
  // and <errno> for reason.

  virtual int handle_output_complete (ACE_Message_Block *message, 
				      long bytes_transferred);
  // Called back by the <ACE_Proactor> when an asynchronous output
  // operation is complete.  If this method returns > 0 the Proactor
  // will initiate another asynchronous send; if a 0 is returned the
  // Proactor will not reinitiate a send; and if a -1 is returned,
  // then no send is reinitiated and <handle_close> is called.  If
  // <bytes_transferred> >= 0, then the I/O operation completed
  // successfully.  If <bytes_transferred> == -1, then the I/O
  // operation failed.  Check <message> for total bytes received and
  // <errno> for reason.

  virtual ACE_Message_Block *get_message (void);
  // Factory that creates an <ACE_Message_Block> that is used by the
  // Proactor to send or receive a message asynchronously.  When the
  // asynchronous call completes, this message is returned to the
  // <Event_Handler> with its contents filled in.  By default,
  // get_message dynamically creates a new ACE_Message_Block.

  // = Accessors to set/get the various event demultiplexors.
  virtual void reactor (ACE_Reactor *reactor);
  virtual ACE_Reactor *reactor (void) const;

  virtual void reactorex (ACE_ReactorEx *reactorex);
  virtual ACE_ReactorEx *reactorex (void) const;

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

#if defined (__ACE_INLINE__)
#include "ace/Event_Handler.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_EVENT_HANDLER_H */
