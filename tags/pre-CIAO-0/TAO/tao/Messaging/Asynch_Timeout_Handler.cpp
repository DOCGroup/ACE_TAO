// $Id$


#include "Asynch_Timeout_Handler.h"

#include "Asynch_Reply_Dispatcher.h"
#include "tao/Transport_Mux_Strategy.h"
#include "ace/Reactor.h"

ACE_RCSID(tao, Asynch_Timeout_Handler, "$Id$")

#if (TAO_HAS_AMI_CALLBACK == 1) || (TAO_HAS_AMI_POLLER == 1)

TAO_Asynch_Timeout_Handler::TAO_Asynch_Timeout_Handler (
  TAO_Asynch_Reply_Dispatcher_Base *rd,
  ACE_Reactor *reactor)
  : rd_ (rd),
    tms_ (0),
    request_id_ (0),
    reactor_ (reactor)
{

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

#endif /* (TAO_HAS_AMI_CALLBACK == 1) || (TAO_HAS_AMI_POLLER == 1) == 0 */
