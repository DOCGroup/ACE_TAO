// $Id$


#include "tao/Asynch_Reply_Dispatcher.h"

ACE_RCSID(tao, Asynch_Reply_Dispatcher, "$Id$")

#include "tao/GIOP_Message_State.h"
#include "tao/ORB_Core.h"
#include "tao/Leader_Follower.h"
#include "tao/debug.h"

#if !defined (__ACE_INLINE__)
#include "tao/Asynch_Reply_Dispatcher.i"
#endif /* __ACE_INLINE__ */

// Constructor.
TAO_Asynch_Reply_Dispatcher_Base::TAO_Asynch_Reply_Dispatcher_Base (void)
  : message_state_ (0),
    transport_ (0)
{
}

// Destructor.
TAO_Asynch_Reply_Dispatcher_Base::~TAO_Asynch_Reply_Dispatcher_Base (void)
{
  if (this->transport_ != 0)
    {
      this->transport_->idle_after_reply ();
    }
}

// Must override pure virtual method in TAO_Reply_Dispatcher.
int
TAO_Asynch_Reply_Dispatcher_Base::dispatch_reply (
    CORBA::ULong /* reply_status */,
    const TAO_GIOP_Version & /* version */,
    IOP::ServiceContextList & /* reply_ctx */,
    TAO_GIOP_Message_State * /* message_state */
  )
{
  return 0;
}

TAO_GIOP_Message_State *
TAO_Asynch_Reply_Dispatcher_Base::message_state (void)
{
  return this->message_state_;
}

void
TAO_Asynch_Reply_Dispatcher_Base::dispatcher_bound (TAO_Transport *)
{
}

void
TAO_Asynch_Reply_Dispatcher_Base::connection_closed (void)
{
}

// ************************************************************************

#if (TAO_HAS_AMI_CALLBACK == 1) || (TAO_HAS_AMI_POLLER == 1)

// Constructor.
TAO_Asynch_Reply_Dispatcher::TAO_Asynch_Reply_Dispatcher (
    const TAO_Reply_Handler_Skeleton &reply_handler_skel,
    Messaging::ReplyHandler_ptr reply_handler
  )
  : reply_handler_skel_ (reply_handler_skel),
    reply_handler_ (Messaging::ReplyHandler::_duplicate (reply_handler))
{
}

// Destructor.
TAO_Asynch_Reply_Dispatcher::~TAO_Asynch_Reply_Dispatcher (void)
{
}

// Dispatch the reply.
int
TAO_Asynch_Reply_Dispatcher::dispatch_reply (
    CORBA::ULong reply_status,
    const TAO_GIOP_Version & /* version */,
    IOP::ServiceContextList &reply_ctx,
    TAO_GIOP_Message_State *message_state
  )
{
  this->reply_status_ = reply_status;
  this->message_state_ = message_state;

  // Steal the buffer, that way we don't do any unnecesary copies of
  // this data.
  CORBA::ULong max = reply_ctx.maximum ();
  CORBA::ULong len = reply_ctx.length ();
  IOP::ServiceContext *context_list = reply_ctx.get_buffer (1);
  this->reply_service_info_.replace (max, len, context_list, 1);


  if (TAO_debug_level >= 4)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P | %t):TAO_Asynch_Reply_Dispatcher::")
                  ACE_TEXT ("dispatch_reply:\n")));
    }

  CORBA::ULong reply_error = TAO_AMI_REPLY_NOT_OK;
  switch (reply_status)
    {
    case TAO_PLUGGABLE_MESSAGE_NO_EXCEPTION:
      reply_error = TAO_AMI_REPLY_OK;
      break;
    case TAO_PLUGGABLE_MESSAGE_USER_EXCEPTION:
      reply_error = TAO_AMI_REPLY_USER_EXCEPTION;
      break;
    case TAO_PLUGGABLE_MESSAGE_SYSTEM_EXCEPTION:
      reply_error = TAO_AMI_REPLY_SYSTEM_EXCEPTION;
      break;
    default:
    case TAO_PLUGGABLE_MESSAGE_LOCATION_FORWARD:
      // @@ Michael: Not even the spec mentions this case.
      //             We have to think about this case.
      // Handle the forwarding and return so the stub restarts the
      // request!
      reply_error = TAO_AMI_REPLY_NOT_OK;
      break;
    }

  ACE_TRY_NEW_ENV
    {
      // Call the Reply Handler's skeleton.
      reply_handler_skel_ (this->message_state_->cdr,
                           this->reply_handler_.in (),
                           reply_error,
                           ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      if (TAO_debug_level >= 4)
        ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                             "Exception during reply handler");
    }
  ACE_ENDTRY;

  // This was dynamically allocated. Now the job is done. Commit
  // suicide here.
  delete this;

  return 1;
}

void
TAO_Asynch_Reply_Dispatcher::connection_closed (void)
{
  ACE_TRY_NEW_ENV
    {
      // Generate a fake exception....
      CORBA::COMM_FAILURE comm_failure (0, CORBA::COMPLETED_MAYBE);

      TAO_OutputCDR out_cdr;

      comm_failure._tao_encode (out_cdr, ACE_TRY_ENV);

      ACE_TRY_CHECK;

      // Turn into an output CDR
      TAO_InputCDR cdr (out_cdr);

      this->reply_handler_skel_ (cdr,
                                 this->reply_handler_.in (),
                                 TAO_AMI_REPLY_SYSTEM_EXCEPTION,
                                 ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      if (TAO_debug_level >= 4)
        {
          ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                               "Asynch_Reply_Dispacher::connection_closed");
        }

    }
  ACE_ENDTRY;
}

#endif /* TAO_HAS_AMI_CALLBACK == 1 || TAO_HAS_AMI_POLLER == 1 */
