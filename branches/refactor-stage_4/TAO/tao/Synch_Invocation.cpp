//$Id$
#include "Synch_Invocation.h"
#include "Profile_Transport_Resolver.h"
#include "Synch_Reply_Dispatcher.h"
#include "Transport.h"
#include "Stub.h"
#include "Bind_Dispatcher_Guard.h"
#include "operation_details.h"
#include "Pluggable_Messaging.h"
#include "Wait_Strategy.h"
#include "Messaging_SyncScopeC.h"
#include "TAOC.h"

ACE_RCSID (tao,
           Synch_Invocation,
           "$Id$")

namespace TAO
{
  Synch_Twoway_Invocation::Synch_Twoway_Invocation (Profile_Transport_Resolver &resolver,
                                                    TAO_Operation_Details &detail)
    : Remote_Invocation (resolver, detail)
  {

  }

  Invocation_Status
  Synch_Twoway_Invocation::communicate (Argument **args,
                                        int args_number
                                        ACE_ENV_ARG_DECL)
  {
    TAO_Synch_Reply_Dispatcher rd (this->resolver_.stub ()->orb_core (),
                                   this->detail_.reply_service_info ());

    // Register a reply dispatcher for this invocation. Use the
    // preallocated reply dispatcher.
    TAO_Bind_Dispatcher_Guard dispatch_guard (this->detail_.request_id (),
                                              &rd,
                                              this->resolver_.transport ()->tms ());

    if (dispatch_guard.status () != 0)
      {
        // @@ What is the right way to handle this error?
        // this->close_connection ();

        ACE_THROW_RETURN (CORBA::INTERNAL (TAO_DEFAULT_MINOR_CODE,
                                           CORBA::COMPLETED_NO),
                          TAO_INVOKE_FAILURE);
      }

    TAO_Target_Specification tspec;
    this->init_target_spec (tspec ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (TAO_INVOKE_FAILURE);


    TAO_OutputCDR &cdr =
      this->resolver_.transport ()->messaging_object ()->out_stream ();

    this->write_header (tspec,
                        cdr
                        ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (TAO_INVOKE_FAILURE);

    this->marshal_data (args,
                        args_number,
                        cdr
                        ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (TAO_INVOKE_FAILURE);


    this->send_message (TAO_Transport::TAO_TWOWAY_REQUEST,
                        cdr
                        ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (TAO_INVOKE_FAILURE);

    // @@ In all MT environments, there's a cancellation point lurking
    // here; need to investigate.  Client threads would frequently be
    // canceled sometime during recv_request ... the correct action to
    // take on being canceled is to issue a CancelRequest message to the
    // server and then imediately let other client-side cancellation
    // handlers do their jobs.
    //
    // In C++, that basically means to unwind the stack using almost
    // normal procedures: all destructors should fire, and some "catch"
    // blocks should probably be able to handle things like releasing
    // pointers. (Without unwinding the C++ stack, resources that must
    // be freed by thread cancellation won't be freed, and the process
    // won't continue to function correctly.)  The tricky part is that
    // according to POSIX, all C stack frames must also have their
    // (explicitly coded) handlers called.  We assume a POSIX.1c/C/C++
    // environment.

    this->resolver_.transport ()->wait_strategy ()->wait (0,
                                                          rd);

    return this->check_reply_status (rd,
                                     args,
                                     args_number
                                     ACE_ENV_ARG_PARAMETER);
  }


  Invocation_Status
  Synch_Twoway_Invocation::check_reply_status (TAO_Synch_Reply_Dispatcher &rd,
                                               Argument **args,
                                               int args_number
                                               ACE_ENV_ARG_DECL)
  {
    // Grab the reply CDR
    TAO_InputCDR &cdr =
      rd.reply_cdr ();

    // Set the translators
    this->resolver_.transport ()->assign_translators (&cdr, 0);

    int i = 0;

    switch (rd.reply_status ())
      {
      case TAO_PLUGGABLE_MESSAGE_NO_EXCEPTION:
        for (i = 0; i != args_number; ++i)
          {
            if (!((*args[i]).demarshal (rd.reply_cdr ())))
              ACE_THROW_RETURN (CORBA::MARSHAL (),
                                TAO_INVOKE_FAILURE);
          }
        break;
      case TAO_PLUGGABLE_MESSAGE_LOCATION_FORWARD:
        // Handle the forwarding and return so the stub restarts the
        // request!
        return this->location_forward (cdr
                                       ACE_ENV_ARG_PARAMETER);
      case TAO_PLUGGABLE_MESSAGE_USER_EXCEPTION:
        // return this->handle_user_exception (ACE_ENV_SINGLE_ARG_PARAMETER);
        break;
      case TAO_PLUGGABLE_MESSAGE_SYSTEM_EXCEPTION:
        // return this->handle_system_exception (ACE_ENV_SINGLE_ARG_PARAMETER)
        break;

#if 0
        {
        // @@ Add the location macros for this exceptions...

        CORBA::String_var type_id;

        if ((this->inp_stream () >> type_id.inout ()) == 0)
          {
            // Could not demarshal the exception id, raise an local
            // CORBA::MARSHAL
            ACE_THROW_RETURN (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE,
                                              CORBA::COMPLETED_MAYBE),
                              TAO_INVOKE_OK);
          }

        CORBA::ULong minor = 0;
        CORBA::ULong completion = 0;

        if ((this->inp_stream () >> minor) == 0
            || (this->inp_stream () >> completion) == 0)
          {
            ACE_THROW_RETURN (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE,
                                              CORBA::COMPLETED_MAYBE),
                              TAO_INVOKE_OK);
          }

        CORBA::SystemException *ex =
          TAO_Exceptions::create_system_exception (type_id.in ()
                                                   ACE_ENV_ARG_PARAMETER);
        ACE_CHECK_RETURN (TAO_INVOKE_OK);

        if (ex == 0)
          {
            // @@ We should raise a CORBA::NO_MEMORY, but we ran out
            //    of memory already. We need a pre-allocated, TSS,
            //    CORBA::NO_MEMORY instance
            ACE_NEW_RETURN (ex,
                            CORBA::UNKNOWN,
                            TAO_INVOKE_EXCEPTION);
          }

        ex->minor (minor);
        ex->completed (CORBA::CompletionStatus (completion));

#if defined (TAO_HAS_EXCEPTIONS)
        // Without this, the call to create_system_exception() above
        // causes a memory leak. On platforms without native exceptions,
        // the CORBA::Environment class manages the memory.
        auto_ptr<CORBA::SystemException> safety (ex);
#endif

        // Raise the exception.
        ACE_ENV_RAISE (ex);

        return TAO_INVOKE_OK;
      }
      // NOTREACHED.


    case TAO_PLUGGABLE_MESSAGE_NEEDS_ADDRESSING_MODE:
      {
        // We have received an exception with a request to change the
        // addressing mode. First let us read the mode that the
        // server/agent asks for.
        CORBA::Short addr_mode = 0;
        if (this->inp_stream ().read_short (addr_mode) == 0)
          {
            // Could not demarshal the addressing disposition, raise an local
            // CORBA::MARSHAL
            ACE_THROW_RETURN (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE,
                                              CORBA::COMPLETED_MAYBE),
                              TAO_INVOKE_OK);
          }

        // Now set this addressing mode in the profile, so that
        // the next invocation need not go through this.
        this->profile_->addressing_mode (addr_mode
                                         ACE_ENV_ARG_PARAMETER);

        // Now restart the invocation
        return TAO_INVOKE_RESTART;
      }
#endif

    }
    return TAO_INVOKE_SUCCESS;
  }

  Invocation_Status
  Synch_Twoway_Invocation::location_forward (TAO_InputCDR &inp_stream
                                             ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    CORBA::Object_var forward_reference;

    // It can be assumed that the GIOP header and the reply header
    // are already handled.  Further it can be assumed that the
    // reply body contains an object reference to the new object.
    // This object pointer will be now extracted.

    if ((inp_stream >> forward_reference.out ()) == 0)
      {
        ACE_THROW_RETURN (CORBA::MARSHAL (),
                          TAO_INVOKE_FAILURE);
      }

    // The object pointer has to be changed to a TAO_Stub pointer
    // in order to obtain the profiles.
    TAO_Stub *stubobj = forward_reference->_stubobj ();

    if (stubobj == 0)
      ACE_THROW_RETURN (CORBA::INTERNAL (),
                        TAO_INVOKE_FAILURE);

    // Reset the profile in the stubs
    this->resolver_.stub ()->add_forward_profiles (stubobj->base_profiles ());

    // Is this check needed?
    /*
    if (this->resolver_.stub ()->next_profile () == 0)
      ACE_THROW (CORBA::TRANSIENT (
        CORBA::SystemException::_tao_minor_code (
          TAO_INVOCATION_LOCATION_FORWARD_MINOR_CODE,
          errno),
        CORBA::COMPLETED_NO));
    */
    return TAO_INVOKE_RESTART;
  }


  /*************************************************************************/

  Synch_Oneway_Invocation::Synch_Oneway_Invocation (Profile_Transport_Resolver &r,
                                                    TAO_Operation_Details &d)
    : Synch_Twoway_Invocation (r, d)
  {
  }

  Invocation_Status
  Synch_Oneway_Invocation::communicate (Argument **args,
                                        int args_number
                                        ACE_ENV_ARG_DECL)
  {
    const CORBA::Octet response_flags =
      this->detail_.response_flags ();

    if (response_flags == CORBA::Octet (Messaging::SYNC_NONE)
        || response_flags == CORBA::Octet (TAO::SYNC_EAGER_BUFFERING)
        || response_flags == CORBA::Octet (TAO::SYNC_DELAYED_BUFFERING))
      {
        TAO_Target_Specification tspec;
        this->init_target_spec (tspec ACE_ENV_ARG_PARAMETER);
        ACE_CHECK_RETURN (TAO_INVOKE_FAILURE);

        TAO_OutputCDR &cdr =
          this->resolver_.transport ()->messaging_object ()->out_stream ();

        this->write_header (tspec,
                            cdr
                            ACE_ENV_ARG_PARAMETER);
        ACE_CHECK_RETURN (TAO_INVOKE_FAILURE);

        this->marshal_data (args,
                            args_number,
                            cdr
                            ACE_ENV_ARG_PARAMETER);
        ACE_CHECK_RETURN (TAO_INVOKE_FAILURE);


        this->send_message (TAO_Transport::TAO_ONEWAY_REQUEST,
                            cdr
                            ACE_ENV_ARG_PARAMETER);
        ACE_CHECK_RETURN (TAO_INVOKE_FAILURE);
      }
    else
      {
        return Synch_Twoway_Invocation::communicate (args,
                                                     args_number
                                                     ACE_ENV_ARG_PARAMETER);
      }

    return TAO_INVOKE_SUCCESS;
  }


}
