// $Id$


#include "DII_Invocation.h"

ACE_RCSID(DynamicInterface, DII_Invocation, "$Id$")

#if (TAO_HAS_MINIMUM_CORBA == 0)

#include "tao/Stub.h"
#include "tao/Principal.h"
#include "tao/Object_KeyC.h"
#include "tao/Transport_Mux_Strategy.h"
#include "tao/debug.h"

#if !defined (__ACE_INLINE__)
# include "DII_Invocation.inl"
#endif /* ! __ACE_INLINE__ */

void
TAO_GIOP_DII_Deferred_Invocation::start (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->TAO_GIOP_Invocation::start (ACE_TRY_ENV);
  ACE_CHECK;

  this->target_spec_.target_specifier (this->profile_->object_key ());
  this->transport_->start_request (this->orb_core_,
                                   this->target_spec_,
                                   this->out_stream_,
                                   ACE_TRY_ENV);
  ACE_CHECK;
}

int
TAO_GIOP_DII_Deferred_Invocation::invoke (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->invoke_i (ACE_TRY_ENV);
}


int
TAO_GIOP_DII_Deferred_Invocation::invoke_i (CORBA::Environment &ACE_TRY_ENV)
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

#endif /* TAO_HAS_MINIMUM_CORBA */
