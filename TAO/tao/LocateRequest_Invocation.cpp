#include "LocateRequest_Invocation.h"
#include "Profile_Transport_Resolver.h"
#include "operation_details.h"
#include "Stub.h"
#include "Bind_Dispatcher_Guard.h"
#include "Transport_Mux_Strategy.h"
#include "Pluggable_Messaging.h"
#include "Transport.h"
#include "Synch_Reply_Dispatcher.h"
#include "GIOP_Utils.h"
#include "Profile.h"

ACE_RCSID (tao,
           LocateRequest_Invocation,
           "$Id$")

namespace TAO
{
  LocateRequest_Invocation::LocateRequest_Invocation (
      CORBA::Object_ptr otarget,
      Profile_Transport_Resolver &resolver,
      TAO_Operation_Details &detail
    )
    : Synch_Twoway_Invocation (otarget,
                               resolver,
                               detail)
  {
  }

  Invocation_Status
  LocateRequest_Invocation::invoke (ACE_Time_Value *max_wait_time
                                    ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::Exception))
  {
    ACE_Countdown_Time countdown (max_wait_time);

    TAO_Synch_Reply_Dispatcher rd (this->resolver_.stub ()->orb_core (),
                                   this->details_.reply_service_info ());

    // Register a reply dispatcher for this invocation. Use the
    // preallocated reply dispatcher.
    TAO_Bind_Dispatcher_Guard dispatch_guard (this->details_.request_id (),
                                              &rd,
                                              this->resolver_.transport ()->tms ());

    if (dispatch_guard.status () != 0)
      {
        // @@ What is the right way to handle this error? Do we need
        // to call the interceptors in this case?
        this->resolver_.transport ()->close_connection ();

        ACE_THROW_RETURN (CORBA::INTERNAL (TAO_DEFAULT_MINOR_CODE,
                                           CORBA::COMPLETED_NO),
                          TAO_INVOKE_FAILURE);
      }

    TAO_Target_Specification tspec;
    this->init_target_spec (tspec ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (TAO_INVOKE_FAILURE);

    TAO_Transport *transport =
      this->resolver_.transport ();

    TAO_OutputCDR &cdr =
      transport->messaging_object ()->out_stream ();

    int retval =
      transport->generate_locate_request (tspec,
                                          this->details_,
                                          cdr);
    ACE_CHECK_RETURN (TAO_INVOKE_FAILURE);

    if (retval == -1)
      return TAO_INVOKE_FAILURE;

    countdown.update ();

    Invocation_Status s =
      this->send_message (cdr,
                          TAO_Transport::TAO_TWOWAY_REQUEST,
                          max_wait_time
                          ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (TAO_INVOKE_FAILURE);

    if (s != TAO_INVOKE_SUCCESS)
      return s;

    countdown.update ();

    // For some strategies one may want to release the transport
    // back to  cache. If the idling is successfull let the
    // resolver about that.
    if (this->resolver_.transport ()->idle_after_send ())
      this->resolver_.transport_released ();

    s =
      this->wait_for_reply (max_wait_time,
                            rd,
                            dispatch_guard
                            ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (TAO_INVOKE_FAILURE);

    s = this->check_reply (rd
                           ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (TAO_INVOKE_FAILURE);

    // For some strategies one may want to release the transport
    // back to  cache after receiving the reply. If the idling is
    // successfull let the resolver about that.
    if (this->resolver_.transport ()->idle_after_reply ())
      this->resolver_.transport_released ();

    return s;
  }

  Invocation_Status
  LocateRequest_Invocation::check_reply (TAO_Synch_Reply_Dispatcher &rd
                                         ACE_ENV_ARG_DECL)
  {
    TAO_InputCDR &cdr =
      rd.reply_cdr ();

    // Set the translators
    this->resolver_.transport ()->assign_translators (&cdr, 0);

    switch (rd.reply_status ())
      {
      case TAO_GIOP_OBJECT_HERE:
        break;
      case TAO_GIOP_UNKNOWN_OBJECT:
        ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (TAO_DEFAULT_MINOR_CODE,
                                                   CORBA::COMPLETED_YES),
                          TAO_INVOKE_FAILURE);
      case TAO_GIOP_OBJECT_FORWARD:
      case TAO_GIOP_OBJECT_FORWARD_PERM:
        return this->location_forward (cdr
                                       ACE_ENV_ARG_PARAMETER);

      case TAO_GIOP_LOC_SYSTEM_EXCEPTION:
        {
          // Pull the exception from the stream.
          CORBA::String_var buf;

          if ((cdr >> buf.inout ()) == 0)
            {
              // Could not demarshal the exception id, raise a local
              // CORBA::MARSHAL exception.
              ACE_THROW_RETURN (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE,
                                                CORBA::COMPLETED_MAYBE),
                                TAO_INVOKE_SYSTEM_EXCEPTION);
            }

          // This kind of exception shouldn't happen with locate requests,
          // but if it does, we turn it into a CORBA::UNKNOWN exception.
          ACE_THROW_RETURN (CORBA::UNKNOWN (TAO_DEFAULT_MINOR_CODE,
                                            CORBA::COMPLETED_YES),
                            TAO_INVOKE_SYSTEM_EXCEPTION);
        }
      case TAO_GIOP_LOC_NEEDS_ADDRESSING_MODE:
        {
          // We have received an exception with a request to change the
          // addressing mode. First let us read the mode that the
          // server/agent asks for.
          CORBA::Short addr_mode = 0;

          if (cdr.read_short (addr_mode) == 0)
            {
              // Could not demarshal the addressing disposition, raise a local
              // CORBA::MARSHAL exception.
              ACE_THROW_RETURN (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE,
                                                CORBA::COMPLETED_MAYBE),
                                TAO_INVOKE_SUCCESS);
            }

          // Now set this addressing mode in the profile, so that
          // the next invocation need not go through this.
          this->resolver_.profile ()->addressing_mode (addr_mode
                                                       ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (TAO_INVOKE_SUCCESS);

          // Restart the invocation.
          return TAO_INVOKE_RESTART;
        }
      }

    return TAO_INVOKE_SUCCESS;
  }
}
