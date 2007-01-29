// $Id$


#include "tao/Messaging/Asynch_Timeout_Handler.h"

#include "tao/Messaging/Asynch_Reply_Dispatcher.h"
#include "tao/Transport_Mux_Strategy.h"
#include "ace/Reactor.h"

ACE_RCSID (Messaging,
           Asynch_Timeout_Handler,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Asynch_Timeout_Handler::TAO_Asynch_Timeout_Handler (
  TAO_Asynch_Reply_Dispatcher_Base *rd,
  ACE_Reactor *reactor)
  : rd_ (rd),
    tms_ (0),
    request_id_ (0),
    reactor_ (reactor)
{
  // Enable reference counting on the event handler.
  this->reference_counting_policy ().value (
    ACE_Event_Handler::Reference_Counting_Policy::ENABLED);

  // We own a reference
  (void) this->rd_->incr_refcount ();
}

TAO_Asynch_Timeout_Handler::~TAO_Asynch_Timeout_Handler ()
{
  // Forget rd's reference
  (void) this->rd_->decr_refcount ();
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
  this->tms_->unbind_dispatcher (request_id_);

  this->rd_->reply_timed_out ();

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
