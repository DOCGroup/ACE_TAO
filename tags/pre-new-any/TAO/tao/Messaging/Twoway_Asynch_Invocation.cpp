// $Id$

#include "Twoway_Asynch_Invocation.h"

#include "Asynch_Timeout_Handler.h"
#include "tao/Stub.h"
#include "tao/Object_KeyC.h"
#include "tao/Transport_Mux_Strategy.h"
#include "tao/Transport.h"

#if !defined (__ACE_INLINE__)
# include "Twoway_Asynch_Invocation.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(Messaging, Twoway_Asynch_Invocation, "$Id$")

#if (TAO_HAS_AMI_CALLBACK == 1) || (TAO_HAS_AMI_POLLER == 1)

int
TAO_GIOP_Twoway_Asynch_Invocation::invoke_i (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  int retval = 0;

  // Only do the following steps if we have a valid reply dispatcher installed.
  // In cases where no reply handler got installed, no reply dispatcher will
  // have been set up. The ORB will drop replies to which it cannot associate
  // a reply dispatcher.

  if (this->rd_)
    {

      // Register a reply dispatcher for this Asynch_Invocation. Use the
      // heap allocated reply dispatcher.

      retval =
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

      // Lets remember the transport for later, so that we can idle the transport
      // when the reply dispatcher goes away.
      this->rd_->transport (this->transport_);


      // AMI Timeout Handling Begin

      if (this->max_wait_time_ != 0)
        {
          this->rd_->schedule_timer (this->op_details_.request_id (),
                                     *this->max_wait_time_);
        }

      // AMI Timeout Handling End

    } // if (this->rd_)

  // Just send the request, without trying to wait for the reply.
  retval = TAO_GIOP_Invocation::invoke (0
                                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (retval);

  if (retval != TAO_INVOKE_OK)
    {
      return retval;
    }


  // We do not wait for the reply. Let us return.
  return TAO_INVOKE_OK;
}

#endif /* (TAO_HAS_AMI_CALLBACK == 1) || (TAO_HAS_AMI_POLLER == 1) == 0 */
