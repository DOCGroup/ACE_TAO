/* -*- C++ -*- */
// $Id$
// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    TkReactor.h
//
// = AUTHOR
//    Nagarajan Surendran <naga@cs.wustl.edu>
//
// ============================================================================

#ifndef ACE_TKREACTOR_H
#define ACE_TKREACTOR_H

#include "ace/Select_Reactor.h"
#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (ACE_HAS_TK)
#include <tk.h>

class ACE_Export ACE_TkReactorID
{
  // = TITLE
  //     This little class is necessary due to the way that Microsoft
  //     implements sockets to be pointers rather than indices.
public:
  ACE_HANDLE handle_;
  // Underlying handle.

  ACE_TkReactorID *next_;
  // Pointer to next node in the linked list.
};

class ACE_TkReactor;

class ACE_Export ACE_TkReactor_Input_Callback
{
public:
  ACE_TkReactor *reactor_;
  ACE_HANDLE handle_;
};

class ACE_Export ACE_TkReactor : public ACE_Select_Reactor
{
  // = TITLE
  //     An object-oriented event demultiplexor and event handler
  //     dispatcher that uses the Tk functions.
public:
  // = Initialization and termination methods.
  ACE_TkReactor (size_t size = DEFAULT_SIZE,
                 int restart = 0,
                 ACE_Sig_Handler * = 0);

  virtual ~ACE_TkReactor (void);

  // = Timer operations.
  virtual long schedule_timer (ACE_Event_Handler *handler,
                               const void *arg,
                               const ACE_Time_Value &delta_time,
                               const ACE_Time_Value &interval);

  virtual int cancel_timer (ACE_Event_Handler *handler,
                            int dont_call_handle_close = 1);

  virtual int cancel_timer (long timer_id,
                            const void **arg = 0,
                            int dont_call_handle_close = 1);

protected:
  // = Register timers/handles with Tk.
  virtual int register_handler_i (ACE_HANDLE handle,
                                  ACE_Event_Handler *handler,
                                  ACE_Reactor_Mask mask);
  // Register a single <handler>.

  virtual int register_handler_i (const ACE_Handle_Set &handles,
                                  ACE_Event_Handler *handler,
                                  ACE_Reactor_Mask mask);
  // Register a set of <handlers>.

  virtual int remove_handler_i (ACE_HANDLE handle,
                                ACE_Reactor_Mask mask);
  // Remove the <handler> associated with this <handle>.

  virtual int remove_handler_i (const ACE_Handle_Set &handles,
                                ACE_Reactor_Mask);
  // Remove a set of <handles>.

  virtual void remove_TkFileHandler (ACE_HANDLE handle);
  // Removes an Tk FileHandler.

  virtual int wait_for_multiple_events (ACE_Select_Reactor_Handle_Set &,
                                        ACE_Time_Value *);
  // Wait for events to occur.

  virtual int TkWaitForMultipleEvents (int,
                                       ACE_Select_Reactor_Handle_Set &,
                                       ACE_Time_Value *);
  //Wait for Tk events to occur.

  ACE_TkReactorID *ids_;
  Tk_TimerToken timeout_;

private:
  void reset_timeout (void);

  // = Integrate with the X callback function mechanism.
  static void TimerCallbackProc (ClientData cd);
  static void InputCallbackProc (ClientData cd,int mask);

  ACE_TkReactor (const ACE_TkReactor &);
  ACE_TkReactor &operator = (const ACE_TkReactor &);
  // Deny access since member-wise won't work...
};

#endif /* ACE_HAS_TK */
#endif /* ACE_TK_REACTOR_H */
