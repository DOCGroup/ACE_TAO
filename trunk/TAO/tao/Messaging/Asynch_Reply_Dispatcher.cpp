// $Id$

#include "tao/Messaging/Asynch_Reply_Dispatcher.h"

#include "tao/Pluggable_Messaging_Utils.h"
#include "tao/ORB_Core.h"
#include "tao/debug.h"
#include "tao/ORB_Core.h"
#include "tao/Transport.h"

#include "ace/CORBA_macros.h"

ACE_RCSID(Messaging, Asynch_Reply_Dispatcher, "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Constructor.
TAO_Asynch_Reply_Dispatcher::TAO_Asynch_Reply_Dispatcher (
    const TAO_Reply_Handler_Skeleton &reply_handler_skel,
    Messaging::ReplyHandler_ptr reply_handler,
    TAO_ORB_Core *orb_core,
    ACE_Allocator *allocator
  )
  :TAO_Asynch_Reply_Dispatcher_Base (orb_core, allocator)
  , reply_handler_skel_ (reply_handler_skel)
  , reply_handler_ (Messaging::ReplyHandler::_duplicate (reply_handler))
  , timeout_handler_ (0)
{
}

// Destructor.
TAO_Asynch_Reply_Dispatcher::~TAO_Asynch_Reply_Dispatcher (void)
{
}

// Dispatch the reply.
int
TAO_Asynch_Reply_Dispatcher::dispatch_reply (
    TAO_Pluggable_Reply_Params &params
  )
{
  if (params.input_cdr_ == 0)
    return -1;

  if (!this->try_dispatch_reply ())
    return 0;

  if (this->timeout_handler_)
    {
      // If we had registered timeout handlers just cancel them and
      // loose ownership of the handlers
      this->timeout_handler_->cancel ();
      this->timeout_handler_->remove_reference ();
      this->timeout_handler_ = 0;
      // AMI Timeout Handling End
    }

  this->reply_status_ = params.reply_status_;

  // Transfer the <params.input_cdr_>'s content to this->reply_cdr_
  ACE_Data_Block *db =
    this->reply_cdr_.clone_from (*params.input_cdr_);

  if (db == 0)
    {
      if (TAO_debug_level > 2)
        {
          ACE_ERROR ((
            LM_ERROR,
            "TAO_Messaging (%P|%t) - Asynch_Reply_Dispatcher::dispatch_reply "
            "clone_from failed \n"));
        }
      return -1;
    }

  // See whether we need to delete the data block by checking the
  // flags. We cannot be happy that we initally allocated the
  // datablocks of the stack. If this method is called twice, as is in
  // some cases where the same invocation object is used to make two
  // invocations like forwarding, the release becomes essential.
  if (ACE_BIT_DISABLED (db->flags (),
                        ACE_Message_Block::DONT_DELETE))
    db->release ();

  // Steal the buffer, that way we don't do any unnecesary copies of
  // this data.
  CORBA::ULong const max = params.svc_ctx_.maximum ();
  CORBA::ULong const len = params.svc_ctx_.length ();
  IOP::ServiceContext *context_list = params.svc_ctx_.get_buffer (1);
  this->reply_service_info_.replace (max, len, context_list, 1);

  if (TAO_debug_level >= 4)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO_Messaging (%P|%t) - Asynch_Reply_Dispatcher::")
                  ACE_TEXT ("dispatch_reply\n")));
    }

  CORBA::ULong reply_error = TAO_AMI_REPLY_NOT_OK;
  switch (this->reply_status_)
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
    case TAO_PLUGGABLE_MESSAGE_LOCATION_FORWARD_PERM:
      // @@ Michael: Not even the spec mentions this case.
      //             We have to think about this case.
      // Handle the forwarding and return so the stub restarts the
      // request!
      reply_error = TAO_AMI_REPLY_NOT_OK;
      break;
    }

  if (!CORBA::is_nil (this->reply_handler_.in ()))
    {
      ACE_TRY_NEW_ENV
        {
          // Call the Reply Handler's skeleton.
          reply_handler_skel_ (this->reply_cdr_,
                               this->reply_handler_.in (),
                               reply_error
                                ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      ACE_CATCHANY
        {
          if (TAO_debug_level >= 4)
            ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                                 "Exception during reply handler");
        }
      ACE_ENDTRY;
    }

  this->decr_refcount ();

  return 1;
}

void
TAO_Asynch_Reply_Dispatcher::connection_closed (void)
{
  ACE_TRY_NEW_ENV
    {
      if (!this->try_dispatch_reply ())
        return;

      if (this->timeout_handler_)
        {
          // If we had registered timeout handlers just cancel them and
          // loose ownership of the handlers
          this->timeout_handler_->cancel ();
          this->timeout_handler_->remove_reference ();
          this->timeout_handler_ = 0;
        }

      // Generate a fake exception....
      CORBA::COMM_FAILURE comm_failure (0, CORBA::COMPLETED_MAYBE);

      TAO_OutputCDR out_cdr;

      comm_failure._tao_encode (out_cdr ACE_ENV_ARG_PARAMETER);

      ACE_TRY_CHECK;

      // Turn into an output CDR
      TAO_InputCDR cdr (out_cdr);

      if (!CORBA::is_nil (this->reply_handler_.in ()))
        {
          this->reply_handler_skel_ (cdr,
                                     this->reply_handler_.in (),
                                     TAO_AMI_REPLY_SYSTEM_EXCEPTION
                                      ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
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

  (void) this->decr_refcount ();
}

// AMI Timeout Handling Begin

void
TAO_Asynch_Reply_Dispatcher::reply_timed_out (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      // Generate a fake exception....
      CORBA::TIMEOUT timeout_failure (
        CORBA::SystemException::_tao_minor_code (
            TAO_TIMEOUT_RECV_MINOR_CODE,
            errno),
         CORBA::COMPLETED_MAYBE);

      TAO_OutputCDR out_cdr;

      timeout_failure._tao_encode (out_cdr ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // This is okay here... Everything relies on our refcount being
      // held by the timeout handler, whose refcount in turn is held
      // by the reactor.
      if (!this->try_dispatch_reply ())
        return;

      // @@ This check probably is unnecessary..
      if (this->timeout_handler_)
        {
          // If we had registered timeout handlers just cancel them and
          // loose ownership of the handlers
          this->timeout_handler_->remove_reference ();
          this->timeout_handler_ = 0;
        }

      // Turn into an output CDR
      TAO_InputCDR cdr (out_cdr);

      if (!CORBA::is_nil (this->reply_handler_.in ()))
        {
          this->reply_handler_skel_ (cdr,
                                     this->reply_handler_.in (),
                                     TAO_AMI_REPLY_SYSTEM_EXCEPTION
                                      ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      if (TAO_debug_level >= 4)
        {
          ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                               "Asynch_Reply_Dispacher::reply_timed_out");
        }

    }
  ACE_ENDTRY;
  ACE_CHECK;

  (void) this->decr_refcount ();
}

long
TAO_Asynch_Reply_Dispatcher::schedule_timer (CORBA::ULong request_id,
                                             const ACE_Time_Value &max_wait_time
                                             ACE_ENV_ARG_DECL)
{
  if (this->timeout_handler_ == 0)
    {
      // @@ Need to use the pool for this..
      ACE_NEW_THROW_EX (this->timeout_handler_,
                        TAO_Asynch_Timeout_Handler (
                          this,
                          this->transport_->orb_core ()->reactor ()),
                        CORBA::NO_MEMORY ());
    }

  return this->timeout_handler_->schedule_timer (
      this->transport_->tms (),
      request_id,
      max_wait_time);
}

TAO_END_VERSIONED_NAMESPACE_DECL
