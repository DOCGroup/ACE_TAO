/* -*- C++ -*- */
// $Id$

/*
// ============================================================================
//
// = LIBRARY
//     ACE
//
// = FILENAME
//     CLASSIX_Reactor.h
//
// = AUTHOR(S)
//     Nokia Telecommunications
//
// ============================================================================
*/

#ifndef ACE_CLASSIX_REACTOR_H
#include "ace/pre.h"
#define ACE_CLASSIX_REACTOR_H

/* ------------------------------------------------------------------------- */
#include "ace/Timer_Queue.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Select_Reactor.h"
#include "ace/Message_Block.h"
#include "ace/CLASSIX/CLASSIX_SAP.h"

class ACE_CLASSIX_Select_Reactor;

class ACE_Export ACE_CLASSIX_Select_Reactor_Notify :
    public ACE_Reactor_Notify
{
  // = TITLE
  //     Unblock the <ACE_CLASSIX_Select_Reactor> from its event loop.
  //
  // = DESCRIPTION
  //   This replaces <ACE_Select_Reactor_Notify> class.
  //   Instead of using pipe, this class uses an Chorus IPC port to
  //   trigger the unblock event.
  //
public:
    // = Initialization and termination methods.
    ACE_CLASSIX_Select_Reactor_Notify(void);
    ~ACE_CLASSIX_Select_Reactor_Notify(void);

    virtual int open (ACE_Reactor_Impl *,
                      ACE_Timer_Queue * = 0,
                      int  = 0 /* dusable_notify_pip */);
    // Enables the notification port and register it with the reactor
    virtual int close (void);
    // Disables the notification port

    int dispatch_notifications (int &number_of_active_handles,
                                const ACE_Handle_Set &rd_mask);
    // Handles pending threads (if any) that are waiting to unblock the
    // Select_Reactor.

    ssize_t notify (ACE_Event_Handler * = 0,
                    ACE_Reactor_Mask = ACE_Event_Handler::EXCEPT_MASK,
                    ACE_Time_Value * = 0);
    // Called by a thread when it wants to unblock the Select_Reactor.
    // This wakeups the <ACE_Select_Reactor> if currently blocked in
    // select()/poll().  Pass over both the <Event_Handler> *and* the
    // <mask> to allow the caller to dictate which <Event_Handler>
    // method the <Select_Reactor> will invoke.  The <ACE_Time_Value>
    // indicates how long to blocking trying to notify the
    // <Select_Reactor>.  If <timeout> == 0, the caller will block until
    // action is possible, else will wait until the relative time
    // specified in *<timeout> elapses).

    virtual int handle_input (ACE_HANDLE handle);
    // Called back by the Select_Reactor when a thread wants to unblock us.

    virtual void max_notify_iterations (int);
    // Set the maximum number of times that the
    // <ACE_CLASSIX_Reactor_Notify::handle_input> method will iterate
    // Always 1.

    virtual int max_notify_iterations (void);
    // Get the maximum number of times that the
    // <ACE_Select_Reactor_Notify::handle_input> method will iterate.
    // Always 1

    void dump (void) const;
    // Dump the state of an object.

    ACE_ALLOC_HOOK_DECLARE;
    // Declare the dynamic allocation hooks.

private:
    ACE_CLASSIX_Select_Reactor       *select_reactor_;
    // Keep a back pointer to the <ACE_Select_Reactor>.  If this value
    // if NULL then the <ACE_Select_Reactor> has been initialized with
    // <disable_notify_pipe>.
    ACE_CLASSIX_Port_Core    notification_port_; // CLASSIX' IPC port
    ACE_CLASSIX_SAP          notification_sap_;
    // The HANDLE that the Select_Reactor is listening on and
    // the HANDLE that threads wanting the attention of the
    // Select_Reactor will write t are the same.
    // It can be seen that the notification port is implicitly connected to the
    // port from where the notification is written.  In many cases, it will be
    // the actor's default port.
};

class ACE_CLASSIX_Select_Reactor : public ACE_Select_Reactor
    // = TITLE
    //   Implementing a Reactor for monitoring incoming data
    //
    // = DESCRIPTION
    //     This class redefines the wait_for_events() method of
    //     <ACE_Select_Reactor> class.
    //
    //     The wait_for_events() method uses ipcReceive() to monitor
    //     all ENABLED ports for incoming messages.  The ipcReceive() is
    //     set up such that it will find out the size of the message but will
    //     not read the message.  The Chorus OS provides an ipcGetData()
    //     to read the message that has just been detected on the same
    //     thread.
    //     Once a message is detected on a particular
    //     port, the reactor  will invoke the port handler's
    //     handle_input() method on the same thread.  It is important that
    //     handle_input() shall immediately retrieve(use ipcGetData())
    //     the message on the same
    //     thread and it shall not hold up this thread in the handle_input()
    //     routine for too long. This will allow the Reactor to respond to
    //     events more quickly.
    //
    //  = NOTE
    //   Since chorus does not allow ipcReceive to monitor a subset of
    //   ENABLED ports.  One cannot have two ACE_CLASSIX_Select_Reactors
    //   in one actor.
    //
{
  public:


    // = CURRENT MESSAGE
    // The following message should be called in the handler's
    // handle_input() routine.
    // The handle_input() routine should not hold the reactor for too long.
    virtual int current_info(ACE_HANDLE /* port */,
                             size_t& /* msg size */);
    // Retruns the current messageage to the caleer if the supplied
    // handle id is the same as the <current_handle_>
    // Returns 0, if <msg size> constains the size of the message
    // that the handler shall read.
    // Returns -1, if the caller is not the expected handler.

 protected:
  // = INITIALIZATION
  ACE_CLASSIX_Select_Reactor (ACE_Timer_Queue * = 0);
  // Initializes <ACE_CLASSIX_Select__Reactor> with the handler
  // repository of the default size.

  ACE_CLASSIX_Select_Reactor (size_t size,
                              int restart = 0,
                              ACE_Timer_Queue * = 0);
  // Initializes <ACE_CLASSIX_Select_Reactor> with the handler repository of
  // size <size>.

  // multiplexer
  virtual int wait_for_multiple_events (ACE_Select_Reactor_Handle_Set &,
                                        ACE_Time_Value *);
  // Wait for events to occur.

  virtual int set_current_info_(ACE_HANDLE /* handle */,
                                size_t /* size of data */);
  // record the handler and the size of the message that the handler shall
  // read next.


  // ACE_Recursive_Thread_Mutex  lock_current_;
  // lock for data relating to the current message.
  // use token lock

  size_t                      current_msg_size_;
  // size of the message that has just been detected.

  ACE_HANDLE                  current_handle_;
  // handler id of the current address.

  friend class ACE_CLASSIX_Select_Reactor_Notify;
  friend class ACE_CLASSIX_OS;
 // for constructors

 private:
  int handle_error_(int /* port handle */);
  // Takes corrective action when errors occur.
  int check_handles_(int /* port handle */);
  // Makes sure the handler is valid.

  ACE_CLASSIX_Select_Reactor(ACE_CLASSIX_Select_Reactor const&);
  ACE_CLASSIX_Select_Reactor const& operator=(ACE_CLASSIX_Select_Reactor const&);
};

/* ------------------------------------------------------------------------- */

#if defined (__ACE_INLINE__)
#include "ace/CLASSIX/CLASSIX_Select_Reactor.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ACE_CLASSIX_REACTOR_H */
