// -*- C++ -*-

// =========================================================================
/**
 *  @file  Timer_Handler.h
 *
 *  $Id$
 *
 *  @desc  Fires servant upcall after a specified period of delay
 *
 *  @author Mayur Deshpande <mayur@ics.uci.edu>
 *
 */
// =========================================================================

#ifndef TIMER_HANDLER_H
#define TIMER_HANDLER_H

#include "ace/Timer_Queue.h"
#include "TestS.h"

// @@ Mayur, please do not program like a Java programmer.  Please
//    move the class declaration to a separate header file, and keep
//    the implementation code in this file.
//
// Mayur: Okie.  I thought it would be easier if everything was
// present in a single file because this class is small.  Of course,
// if this class grows big (which shouldn't happen), that might be a
// problem.

class Timer_Handler : public ACE_Event_Handler
{
public:

  Timer_Handler (Test::AMH_RoundtripResponseHandler_ptr rh, 
                 Test::Timestamp send_time);

  virtual int handle_timeout (const ACE_Time_Value &,
                              const void *);

  virtual int handle_close (ACE_HANDLE handle,
                            ACE_Reactor_Mask mask);

private:
  // Store the RH and the parameter that we send back to the client.
  // We create one Timer_Handler instance for each RH.
  Test::AMH_RoundtripResponseHandler_var rh_;
  Test::Timestamp send_time_;
};

#endif /* TIMER_HANDLER_H */
