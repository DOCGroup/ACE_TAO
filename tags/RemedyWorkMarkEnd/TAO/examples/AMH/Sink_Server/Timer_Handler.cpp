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

#include "Timer_Handler.h"

Timer_Handler::Timer_Handler (Test::AMH_RoundtripResponseHandler_ptr rh,
                              Test::Timestamp send_time)
  : rh_ (Test::AMH_RoundtripResponseHandler::_duplicate (rh))
  , send_time_ (send_time)
{
}

int
Timer_Handler::handle_timeout (const ACE_Time_Value &,
                               const void *)
{
  try
    {
      this->rh_->test_method (this->send_time_);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "Exception in Timer_Handler::handle_timeout\n");
      return -1;
    }

  // Returning -1 calls handle_close which will clean up and
  // deregister this handle from the reactor. DO not return anything
  // else.  I changes the return value inadvertently and it caused a
  // memory leak.  should have RTMFed this before.  Took me a ****
  // long time to figure out this memory leak, though in retrospect I
  // should looked here first.
  return -1;
}


int
Timer_Handler::handle_close (ACE_HANDLE,
                             ACE_Reactor_Mask)
{
  // deregister from the reactor and destruct

  delete this;
  return 0;
}
