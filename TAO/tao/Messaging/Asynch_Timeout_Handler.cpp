// -*- C++ -*-
// $Id$

#include "tao/Messaging/Asynch_Timeout_Handler.h"

#include "tao/Messaging/Asynch_Reply_Dispatcher.h"
#include "tao/Transport_Mux_Strategy.h"
#include "ace/Reactor.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Asynch_Timeout_Handler::TAO_Asynch_Timeout_Handler (
  ACE_Reactor *reactor)
  : tms_ (0),
    request_id_ (0),
    reactor_ (reactor)
{
  // Enable reference counting on the event handler.
  this->reference_counting_policy ().value (
    ACE_Event_Handler::Reference_Counting_Policy::ENABLED);
}

TAO_Asynch_Timeout_Handler::~TAO_Asynch_Timeout_Handler ()
{
}


long
TAO_Asynch_Timeout_Handler::schedule_timer (TAO_Transport_Mux_Strategy *tms,
                                            CORBA::ULong request_id,
                                            const ACE_Time_Value &max_wait_time)
{
  // Remember them for later.
  this->tms_ = tms;
  this->request_id_ = request_id;

  return this->reactor_->schedule_timer (this,          // handler
                                         0,             // arg
                                         max_wait_time);
}

int
TAO_Asynch_Timeout_Handler::handle_timeout (const ACE_Time_Value &,
                                            const void *)
{
  // Check if there was a reply dispatcher registered in the tms, if not
  // the reply already got dispatched by another thread
  if (this->tms_->reply_timed_out (this->request_id_) == 0)
    {
      if (TAO_debug_level >= 4)
        {
          TAOLIB_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO_Messaging (%P|%t) - Asynch_Timeout_Handler")
                      ACE_TEXT ("::handle_timeout, request %d timed out\n"),
                                this->request_id_));
        }
    }
  else
    {
      if (TAO_debug_level >= 1)
        {
          TAOLIB_ERROR ((LM_ERROR,
                      ACE_TEXT ("TAO_Messaging (%P|%t) - Asynch_Timeout_Handler")
                      ACE_TEXT ("::handle_timeout, unable to dispatch timed out request %d\n"),
                                this->request_id_));
        }
    }

  // reset any possible timeout errno
  errno = 0;

  // we are unregistered anyway
  return 0;
}

void
TAO_Asynch_Timeout_Handler::cancel ()
{
  // The tms_ is only set if we got scheduled.
  if (this->tms_)
    {
      this->reactor_->cancel_timer (this);
    }
}

TAO_END_VERSIONED_NAMESPACE_DECL
