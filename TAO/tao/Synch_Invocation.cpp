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

  int
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
                          -1);
      }

    TAO_Target_Specification tspec;
    this->init_target_spec (tspec ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (-1);


    TAO_OutputCDR &cdr =
      this->resolver_.transport ()->messaging_object ()->out_stream ();

    this->write_header (tspec,
                        cdr
                        ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (-1);

    this->marshal_data (args,
                        args_number,
                        cdr
                        ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (-1);


    this->send_message (TAO_Transport::TAO_TWOWAY_REQUEST,
                        cdr
                        ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (-1);

    this->resolver_.transport ()->wait_strategy ()->wait (0,
                                                          rd);

    this->check_reply_status (rd,
                              args,
                              args_number
                              ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (-1);

    return 0;
  }


  void
  Synch_Twoway_Invocation::check_reply_status (TAO_Synch_Reply_Dispatcher &rd,
                                               Argument **args,
                                               int args_number
                                               ACE_ENV_ARG_DECL)
  {
    int i = 0;
    switch (rd.reply_status ())
      {
      case TAO_PLUGGABLE_MESSAGE_NO_EXCEPTION:
        for (i = 0; i != args_number; ++i)
          {
            if (!((*args[i]).demarshal (rd.reply_cdr ())))
              ACE_THROW (CORBA::MARSHAL ());
          }
        break;

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

    case TAO_PLUGGABLE_MESSAGE_LOCATION_FORWARD:
      // Handle the forwarding and return so the stub restarts the
      // request!
      return this->location_forward (this->inp_stream ()
                                     ACE_ENV_ARG_PARAMETER);
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
    return;
  }



  /*************************************************************************/

  Synch_Oneway_Invocation::Synch_Oneway_Invocation (Profile_Transport_Resolver &r,
                                                    TAO_Operation_Details &d)
    : Synch_Twoway_Invocation (r, d)
  {
  }

  int
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
        ACE_CHECK_RETURN (-1);

        TAO_OutputCDR &cdr =
          this->resolver_.transport ()->messaging_object ()->out_stream ();

        this->write_header (tspec,
                            cdr
                            ACE_ENV_ARG_PARAMETER);
        ACE_CHECK_RETURN (-1);

        this->marshal_data (args,
                            args_number,
                            cdr
                            ACE_ENV_ARG_PARAMETER);
        ACE_CHECK_RETURN (-1);


        this->send_message (TAO_Transport::TAO_ONEWAY_REQUEST,
                            cdr
                            ACE_ENV_ARG_PARAMETER);
        ACE_CHECK_RETURN (-1);
      }
    else
      {
        return Synch_Twoway_Invocation::communicate (args,
                                                     args_number
                                                     ACE_ENV_ARG_PARAMETER);
      }

    return 0;
  }
}
