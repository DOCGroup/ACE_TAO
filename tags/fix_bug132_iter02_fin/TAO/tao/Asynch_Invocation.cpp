// $Id$


#include "tao/Asynch_Invocation.h"

ACE_RCSID(tao, Asynch_Invocation, "$Id$")

#include "tao/Timeprobe.h"
#include "tao/Stub.h"
#include "tao/Object_KeyC.h"
#include "tao/Transport_Mux_Strategy.h"
#include "tao/Transport.h"
#include "tao/Asynch_Timeout_Handler.h"
#include "tao/try_macros.h"

#if !defined (__ACE_INLINE__)
# include "tao/Asynch_Invocation.i"
#endif /* ! __ACE_INLINE__ */

#if defined (ACE_ENABLE_TIMEPROBES)

static const char *TAO_Asynch_Invocation_Timeprobe_Description[] =
  {
    "GIOP_Asynch_Invocation::invoke - start",
    "GIOP_Asynch_Invocation::invoke - end",

    "GIOP_Asynch_Invocation::start - enter",
    "GIOP_Asynch_Invocation::start - leave",
    "GIOP_Asynch_Invocation::start - connect",
    "GIOP_Asynch_Invocation::start - start_msg",
    "GIOP_Asynch_Invocation::start - request_hdr"
  };

enum
  {
    TAO_GIOP_ASYNCH_INVOCATION_INVOKE_START = 1100,
    TAO_GIOP_ASYNCH_INVOCATION_INVOKE_END,

    TAO_GIOP_ASYNCH_INVOCATION_START_ENTER,
    TAO_GIOP_ASYNCH_INVOCATION_START_LEAVE,
    TAO_GIOP_ASYNCH_INVOCATION_START_CONNECT,
    TAO_GIOP_ASYNCH_INVOCATION_START_START_MSG,
    TAO_GIOP_ASYNCH_INVOCATION_START_REQUEST_HDR
  };


// Setup Timeprobes
ACE_TIMEPROBE_EVENT_DESCRIPTIONS (TAO_Asynch_Invocation_Timeprobe_Description,
                                  TAO_GIOP_ASYNCH_INVOCATION_INVOKE_START);

#endif /* ACE_ENABLE_TIMEPROBES */

TAO_GIOP_Asynch_Invocation::TAO_GIOP_Asynch_Invocation (void)
  : rd_ (0)
{
}

int
TAO_GIOP_Asynch_Invocation::invoke (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_FUNCTION_PP_TIMEPROBE (TAO_GIOP_ASYNCH_INVOCATION_INVOKE_START);

  return this->invoke_i (ACE_TRY_ENV);
}

void
TAO_GIOP_Asynch_Invocation::start (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->TAO_GIOP_Invocation::start (ACE_TRY_ENV);
  ACE_CHECK;

  this->target_spec_.target_specifier (this->profile_->object_key ());
  this->transport_->start_request (this->orb_core_,
                                   this->target_spec_,
                                   this->out_stream_,
                                   ACE_TRY_ENV);
}

// **************************************************************************

#if (TAO_HAS_AMI_CALLBACK == 1) || (TAO_HAS_AMI_POLLER == 1)

int
TAO_GIOP_Twoway_Asynch_Invocation::invoke_i (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Register a reply dispatcher for this Asynch_Invocation. Use the
  // heap allocated reply dispatcher.

  int retval =
    this->transport_->tms ()->bind_dispatcher (this->op_details_.request_id (),
                                               this->rd_);
  if (retval == -1)
    {
      // @@ What is the right way to handle this error?
      this->close_connection ();

      ACE_THROW_RETURN (CORBA::INTERNAL (TAO_DEFAULT_MINOR_CODE,
                                         CORBA::COMPLETED_NO),
                        TAO_INVOKE_EXCEPTION);
    }

  // AMI Timeout Handling Begin

  if (this->max_wait_time_ != 0)
    {
      ACE_Reactor *r = this->orb_core_->reactor ();

      // @@ Michael: Optimization: The timeout handler should actually 
      //             be allocated on the stack in the reply handler!
      TAO_Asynch_Timeout_Handler *timeout_handler;
      ACE_NEW_THROW_EX (timeout_handler,
                        TAO_Asynch_Timeout_Handler (this->rd_,
                                                    this->transport_->tms (),
                                                    this->op_details_.request_id ()),
                        CORBA::NO_MEMORY (
                          CORBA::SystemException::_tao_minor_code (TAO_DEFAULT_MINOR_CODE,
                                                                   ENOMEM),
                          CORBA::COMPLETED_NO));
      ACE_CHECK_RETURN (TAO_INVOKE_EXCEPTION);

      // Let the reply dispatcher remember the timeout_handler
      this->rd_->timeout_handler (timeout_handler);

      r->schedule_timer (timeout_handler,  // handler
                         0,                // arg
                         *this->max_wait_time_);
    }

  // AMI Timeout Handling End



  // Just send the request, without trying to wait for the reply.
  retval = TAO_GIOP_Invocation::invoke (0,
                                        ACE_TRY_ENV);
  ACE_CHECK_RETURN (retval);

  if (retval != TAO_INVOKE_OK)
    {
      return retval;
    }

  // Everything executed ok; lets remember the transport for later.
  this->rd_->transport (this->transport_);

  // We do not wait for the reply. Let us return.
  return TAO_INVOKE_OK;
}

#endif /* (TAO_HAS_AMI_CALLBACK == 1) || (TAO_HAS_AMI_POLLER == 1) == 0 */
