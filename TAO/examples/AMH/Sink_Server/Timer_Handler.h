// -*- C++ -*-

// =========================================================================
/**
 *  @file  Timer_Handler.h
 *
 *  @desc  Fires servant upcall after a specified period of delay
 *
 *  @author Mayur Deshpande <mayur@ics.uci.edu>
 */
// =========================================================================

#ifndef TIMER_HANDLER_H
#define TIMER_HANDLER_H

#include "ace/Timer_Queue.h"
#include "TestS.h"

class Timer_Handler : public ACE_Event_Handler
{
public:
  Timer_Handler (Timer_Handler const&) = delete;
  Timer_Handler (Timer_Handler&&) = delete;
  Timer_Handler& operator= (Timer_Handler const&) = delete;
  Timer_Handler& operator= (Timer_Handler&&) = delete;

  Timer_Handler (Test::AMH_RoundtripResponseHandler_ptr rh,
                 Test::Timestamp send_time);

  int handle_timeout (ACE_Time_Value const&,
                      void const*) override;

  int handle_close (ACE_HANDLE handle,
                    ACE_Reactor_Mask mask) override;

private:
  // Store the RH and the parameter that we send back to the client.
  // We create one Timer_Handler instance for each RH.
  Test::AMH_RoundtripResponseHandler_var rh_;
  Test::Timestamp send_time_;
};

#endif /* TIMER_HANDLER_H */
