#include "tao/LocateRequest_Invocation.h"
#include "tao/Profile_Transport_Resolver.h"
#include "tao/operation_details.h"
#include "tao/Stub.h"
#include "tao/Bind_Dispatcher_Guard.h"
#include "tao/Transport.h"
#include "tao/Synch_Reply_Dispatcher.h"
#include "tao/GIOP_Utils.h"
#include "tao/Profile.h"
#include "tao/ORB_Constants.h"
#include "tao/SystemException.h"

#include "ace/Countdown_Time.h"

ACE_RCSID (tao,
           LocateRequest_Invocation,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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
  LocateRequest_Invocation::invoke (ACE_Time_Value *max_wait_time)
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

        throw ::CORBA::INTERNAL (TAO::VMCID, CORBA::COMPLETED_NO);
      }

    TAO_Target_Specification tspec;
    this->init_target_spec (tspec);

    TAO_Transport *transport = this->resolver_.transport ();

    Invocation_Status s = TAO_INVOKE_FAILURE;
    {
      ACE_GUARD_RETURN (ACE_Lock, ace_mon, *transport->output_cdr_lock ()
                        , TAO_INVOKE_FAILURE);
      TAO_OutputCDR &cdr = transport->out_stream ();

      if (transport->generate_locate_request (tspec, this->details_, cdr) == -1)
        return TAO_INVOKE_FAILURE;

      countdown.update ();

      s = this->send_message (cdr, TAO_Transport::TAO_TWOWAY_REQUEST
                              , max_wait_time);
    }

    if (s != TAO_INVOKE_SUCCESS)
      return s;

    countdown.update ();

    // For some strategies one may want to release the transport
    // back to  cache. If the idling is successfull let the
    // resolver about that.
    if (this->resolver_.transport ()->idle_after_send ())
      this->resolver_.transport_released ();

    s = this->wait_for_reply (max_wait_time, rd, dispatch_guard);

    s = this->check_reply (rd);

    // For some strategies one may want to release the transport
    // back to  cache after receiving the reply. If the idling is
    // successfull let the resolver about that.
    if (this->resolver_.transport ()->idle_after_reply ())
      this->resolver_.transport_released ();

    return s;
  }

  Invocation_Status
  LocateRequest_Invocation::check_reply (TAO_Synch_Reply_Dispatcher &rd)
  {
    TAO_InputCDR &cdr = rd.reply_cdr ();

    // Set the translators
    this->resolver_.transport ()->assign_translators (&cdr, 0);

    switch (rd.locate_reply_status ())
      {
      case GIOP::OBJECT_HERE:
        break;
      case GIOP::UNKNOWN_OBJECT:
        throw ::CORBA::OBJECT_NOT_EXIST (TAO::VMCID, CORBA::COMPLETED_YES);
      case GIOP::OBJECT_FORWARD:
      case GIOP::OBJECT_FORWARD_PERM:
        return this->location_forward (cdr);
      case GIOP::LOC_SYSTEM_EXCEPTION:
        {
          // Pull the exception from the stream.
          CORBA::String_var buf;

          if ((cdr >> buf.inout ()) == 0)
            {
              // Could not demarshal the exception id, raise a local
              // CORBA::MARSHAL exception.
              throw ::CORBA::MARSHAL (TAO::VMCID, CORBA::COMPLETED_MAYBE);
            }

          // This kind of exception shouldn't happen with locate requests,
          // but if it does, we turn it into a CORBA::UNKNOWN exception.
          throw ::CORBA::UNKNOWN (TAO::VMCID, CORBA::COMPLETED_YES);
        }
      case GIOP::LOC_NEEDS_ADDRESSING_MODE:
        {
          // We have received an exception with a request to change the
          // addressing mode. First let us read the mode that the
          // server/agent asks for.
          CORBA::Short addr_mode = 0;

          if (cdr.read_short (addr_mode) == 0)
            {
              // Could not demarshal the addressing disposition, raise a local
              // CORBA::MARSHAL exception.
              throw ::CORBA::MARSHAL (TAO::VMCID, CORBA::COMPLETED_MAYBE);
            }

          // Now set this addressing mode in the profile, so that
          // the next invocation need not go through this.
          this->resolver_.profile ()->addressing_mode (addr_mode);

          // Restart the invocation.
          return TAO_INVOKE_RESTART;
        }
      }

    return TAO_INVOKE_SUCCESS;
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
