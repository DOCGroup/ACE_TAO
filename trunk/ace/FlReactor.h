/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//   ace
//
// = FILENAME
//   FlReactor.h
//
// = AUTHOR
//   Carlos O'Ryan <coryan@cs.wustl.edu>
//
//   Based in part in the ACE_XtReactor implementation by:
//
//    Eric C. Newton's <ecn@clark.net>,
//    Kirill Rybaltchenko <Kirill.Rybaltchenko@cern.ch>, and
//    Douglas C. Schmidt <schmidt@cs.wustl.edu>
//
// ============================================================================

#ifndef ACE_FLREACTOR_H
#define ACE_FLREACTOR_H

#include "ace/Select_Reactor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (ACE_HAS_FL)

class ACE_Export ACE_FlReactor : public ACE_Select_Reactor
{
  // = TITLE
  //   A Reactor implementation that uses the Fast-Light (FL) toolkit
  //   for event demultiplexing.  This will let us integrate the FL
  //   toolkit with ACE and/or TAO.
  //
  // = DESCRIPTION
  //   As many other GUI toolkits FL supports a minimal set of
  //   callbacks to handle event demultiplexing, namely simple methods 
  //   to add file descriptors to the event demuxing set or timeout
  //   events.
  //   This class adapts this simple mechanisms so they are compatible 
  //   with ACE's Reactor.
  //   

public:
  // = Initialization and termination methods.
  ACE_FlReactor (size_t size = DEFAULT_SIZE,
                 int restart = 0,
                 ACE_Sig_Handler * = 0);
  virtual ~ACE_FlReactor (void);

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
  // = Register timers/handles with Fl.
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

  virtual int wait_for_multiple_events (ACE_Select_Reactor_Handle_Set &,
                                        ACE_Time_Value *);
  // Wait for events to occur.

private:
  void reset_timeout (void);

  // = Integrate with the FL callback function mechanism.
  static void fl_io_proc (int fd, void*);
  static void fl_timeout_proc (void*);

  ACE_FlReactor (const ACE_FlReactor &);
  ACE_FlReactor &operator = (const ACE_FlReactor &);
  // Deny access since member-wise won't work...
};

#if defined(__ACE_INLINE__)
#include "ace/FlReactor.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_HAS_FL */

#endif /* ACE_FLREACTOR_H */
