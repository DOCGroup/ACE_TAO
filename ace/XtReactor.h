/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    XtReactor.h
//
// = AUTHOR
//    Eric C. Newton's <ecn@clark.net> and Douglas C. Schmidt <schmidt@cs.wustl.edu>
// 
// ============================================================================

#if !defined (ACE_XTREACTOR_H)
#define ACE_XTREACTOR_H

#include "ace/Reactor.h"

#if defined (ACE_HAS_XT)

#define String XtString
#include /**/ <X11/Intrinsic.h>
#undef String

// Forward decl.
struct ACE_XtReactorID;

class ACE_XtReactor : public ACE_Reactor
  // = TITLE
  //     An object oriented event demultiplexor and event handler
  //     dispatcher that uses the X Toolkit functions.
  //     
  // = DESCRIPTION
  //     The ACE_Reactor is an object-oriented event demultiplexor
  //     and event handler dispatcher.  The sources of events that the
  //     ACE_Reactor waits for and dispatches includes I/O events,
  //     signals, and timer events.
{
public:
  // = Initialization and termination methods.
  ACE_XtReactor (XtAppContext context,
		 size_t size = DEFAULT_SIZE, 
		 int restart = 0, 
		 ACE_Sig_Handler * = 0);
  virtual ~ACE_XtReactor (void);

  XtAppContext context (void);

  // Register timers/handles with Xt.
  virtual int attach (ACE_HANDLE handle, 
	              ACE_Event_Handler *handler,
	              ACE_Reactor_Mask mask);

  virtual int detach (ACE_HANDLE handle, ACE_Reactor_Mask mask);

  virtual int schedule_timer (ACE_Event_Handler *handler, 
		              const void *arg,
		              const ACE_Time_Value &delta_time, 
		              const ACE_Time_Value &interval);

  virtual int cancel_timer (ACE_Event_Handler *handler);
  virtual int cancel_timer (int timer_id, const void **arg);

protected:

  virtual int wait_for_multiple_events (ACE_Reactor_Handle_Set &,
					ACE_Time_Value *); 

  virtual int XtWaitForMultipleEvents (int, 
				       ACE_Reactor_Handle_Set &,
				       ACE_Time_Value *); 

  ACE_XtReactor (const ACE_Reactor &);
  ACE_XtReactor &operator = (const ACE_Reactor &);

  XtAppContext context_;
  struct ACE_XtReactorID *ids_;
  int id_len_;
  XtIntervalId timeout_;

private:
  void reset_timeout (void);
  static void TimerCallbackProc (XtPointer closure, XtIntervalId *id);
  static void InputCallbackProc (XtPointer closure, int* source, XtInputId *id);
};
#endif /* ACE_HAS_XT */

#endif /* ACE_XTREACTOR_H */
