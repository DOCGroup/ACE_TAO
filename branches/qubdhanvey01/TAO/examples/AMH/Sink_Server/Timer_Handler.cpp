// -*- C++ -*-

// =========================================================================
/**
 *  @file  Timer_Handler.cpp
 *
 *  $Id$
 *
 *  @desc  Fires servant upcall after a specified period of delay
 *
 *  @author Mayur Deshpande <mayur@ics.uci.edu>
 *
 */
// =========================================================================

#include "ace/Timer_Queue.h"
#include "TestS.h"

class Timer_Handler : public ACE_Event_Handler
{
public:

  Timer_Handler (Test::Timestamp send_time)
    : send_time_ (send_time)
  {
  }

  virtual int handle_timeout (const ACE_Time_Value &time,
                              const void *arg)
  {
    Test::AMH_RoundtripResponseHandler* rh =
      (Test::AMH_RoundtripResponseHandler*) arg;

    rh->test_method (send_time_);

    ACE_UNUSED_ARG (time);
    return -1;

  }


  virtual int handle_close (ACE_HANDLE handle,
                            ACE_Reactor_Mask mask)
  {
    // deregister from reactor and destruct
    ACE_UNUSED_ARG (handle);
    ACE_UNUSED_ARG (mask);
    delete this;
    return 0;
  }

private:
  Test::Timestamp send_time_;
};
