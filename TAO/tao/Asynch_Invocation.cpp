// $Id$

#include "tao/Asynch_Invocation.h"

#include "tao/Timeprobe.h"
#include "tao/Stub.h"
#include "tao/Principal.h"
#include "tao/Object_KeyC.h"
#include "tao/Transport_Mux_Strategy.h"
#include "tao/debug.h"

#if !defined (__ACE_INLINE__)
# include "tao/Asynch_Invocation.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(tao, Asynch_Invocation, "$Id$")


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

#if (TAO_HAS_CORBA_MESSAGING == 1)
#if (TAO_HAS_AMI_CALLBACK == 1) || (TAO_HAS_AMI_POLLER == 1)

void
TAO_GIOP_Twoway_Asynch_Invocation::start (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->TAO_GIOP_Invocation::start (ACE_TRY_ENV);
  ACE_CHECK;

  TAO_Target_Specification spec;
  this->transport_->start_request (this->orb_core_,
                                   spec,
                                   this->out_stream_,
                                   ACE_TRY_ENV);
}

int
TAO_GIOP_Twoway_Asynch_Invocation::invoke (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_FUNCTION_PP_TIMEPROBE (TAO_GIOP_ASYNCH_INVOCATION_INVOKE_START);

  return this->invoke_i (ACE_TRY_ENV);
}


int
TAO_GIOP_Twoway_Asynch_Invocation::invoke_i (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Register a reply dispatcher for this Asynch_Invocation. Use the
  // heap allocated reply dispatcher.

  int retval =
    this->transport_->tms ()->bind_dispatcher (this->request_id_,
                                               this->rd_);
  if (retval == -1)
    {
      // @@ What is the right way to handle this error?
      this->close_connection ();
      ACE_THROW_RETURN (CORBA::INTERNAL (TAO_DEFAULT_MINOR_CODE,
                                         CORBA::COMPLETED_NO),
                        TAO_INVOKE_EXCEPTION);
    }

  // Just send the request, without trying to wait for the reply.
  retval = TAO_GIOP_Invocation::invoke (0,
                                        ACE_TRY_ENV);
  ACE_CHECK_RETURN (retval);

  if (retval != TAO_INVOKE_OK)
    return retval;

  // We do not wait for the reply. Let us return.

  return TAO_INVOKE_OK;
}

#endif /* TAO_HAS_AMI_CALLBACK == 1 || TAO_HAS_AMI_POLLER == 1 */
#endif /* TAO_HAS_CORBA_MESSAGING == 1 */

//**************************************************************************

#if !defined (TAO_HAS_MINIMUM_CORBA)

void
TAO_GIOP_DII_Deferred_Invocation::start (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->TAO_GIOP_Invocation::start (ACE_TRY_ENV);
  ACE_CHECK;

  TAO_Target_Specification spec;
  spec.target_specifier (this->profile_->object_key ());
  this->transport_->start_request (this->orb_core_,
                                   spec,
                                   this->out_stream_,
                                   ACE_TRY_ENV);
  ACE_CHECK;
}

int
TAO_GIOP_DII_Deferred_Invocation::invoke (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_FUNCTION_PP_TIMEPROBE (TAO_GIOP_ASYNCH_INVOCATION_INVOKE_START);

  return this->invoke_i (ACE_TRY_ENV);
}


int
TAO_GIOP_DII_Deferred_Invocation::invoke_i (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Register a reply dispatcher for this Asynch_Invocation. Use the
  // heap allocated reply dispatcher.

  int retval =
    this->transport_->tms ()->bind_dispatcher (this->request_id_,
                                               this->rd_);
  if (retval == -1)
    {
      // @@ What is the right way to handle this error?
      this->close_connection ();
      ACE_THROW_RETURN (CORBA::INTERNAL (TAO_DEFAULT_MINOR_CODE,
                                         CORBA::COMPLETED_NO),
                        TAO_INVOKE_EXCEPTION);
    }

  // Just send the request, without trying to wait for the reply.
  retval = TAO_GIOP_Invocation::invoke (0,
                                        ACE_TRY_ENV);
  ACE_CHECK_RETURN (retval);

  if (retval != TAO_INVOKE_OK)
    return retval;

  // We do not wait for the reply. Let us return.

  return TAO_INVOKE_OK;
}

#endif /* TAO_HAS_MINIMUM_CORBA */
