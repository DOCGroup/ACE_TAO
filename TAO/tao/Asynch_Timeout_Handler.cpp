// $Id$


#include "tao/Asynch_Timeout_Handler.h"

#include "ace/Reactor.h"
#include "tao/Asynch_Reply_Dispatcher.h"
#include "tao/Transport_Mux_Strategy.h"


ACE_RCSID(tao, Asynch_Timeout_Handler, "$Id$")


#if (TAO_HAS_AMI_CALLBACK == 1) || (TAO_HAS_AMI_POLLER == 1)


TAO_Asynch_Timeout_Handler::TAO_Asynch_Timeout_Handler (
  TAO_Asynch_Reply_Dispatcher_Base *rd,
  TAO_Transport_Mux_Strategy *tms,
  CORBA::ULong request_id)
  : rd_ (rd),
    tms_ (tms),
    request_id_ (request_id)
{

}

TAO_Asynch_Timeout_Handler::~TAO_Asynch_Timeout_Handler ()
{

}

int 
TAO_Asynch_Timeout_Handler::handle_timeout (const ACE_Time_Value &,
                                            const void *)
{
  tms_->unbind_dispatcher (request_id_);

  rd_->reply_timed_out ();

  errno = 0;

  // let the reactor unregister us.
  return -1;
}

int
TAO_Asynch_Timeout_Handler::handle_close (ACE_HANDLE,
                                          ACE_Reactor_Mask)
{
  // commit suicide, we are a per invocation object.
  delete this;

  // @@ Michael: We should avoid dynamic creation of the timeout
  //             handler, so that it is statically allocated 
  //             in the asynch reply dispatcher. Thanks to Carlos
  //             for this hint.

  return 0;
}



void
TAO_Asynch_Timeout_Handler::cancel ()
{
  this->reactor ()->cancel_timer (this);

  // The reply handler, which invokes this operation is going
  // to delete this object.
}

#endif /* (TAO_HAS_AMI_CALLBACK == 1) || (TAO_HAS_AMI_POLLER == 1) == 0 */
