// $Id$

#include "tao/Asynch_Reply_Dispatcher.h"

#include "tao/Pluggable_Messaging_Utils.h"
#include "tao/ORB_Core.h"
#include "tao/debug.h"
#include "tao/ORB_Core.h"
#include "tao/Transport.h"
#include "tao/Asynch_Timeout_Handler.h"

#if !defined (__ACE_INLINE__)
#include "tao/Asynch_Reply_Dispatcher.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(tao, Asynch_Reply_Dispatcher, "$Id$")

// Constructor.
TAO_Asynch_Reply_Dispatcher_Base::TAO_Asynch_Reply_Dispatcher_Base (TAO_ORB_Core *orb_core)
  : db_ (sizeof buf_,
         ACE_Message_Block::MB_DATA,
         this->buf_,
         orb_core->message_block_buffer_allocator (),
         orb_core->locking_strategy (),
         ACE_Message_Block::DONT_DELETE,
         orb_core->message_block_dblock_allocator ()),
    reply_cdr_ (&db_,
                ACE_Message_Block::MB_DATA,
                TAO_ENCAP_BYTE_ORDER,
                TAO_DEF_GIOP_MAJOR,
                TAO_DEF_GIOP_MINOR,
                orb_core),
    transport_ (0)
{
}

// Destructor.
TAO_Asynch_Reply_Dispatcher_Base::~TAO_Asynch_Reply_Dispatcher_Base (void)
{
  if (this->transport_ != 0)
    {
      this->transport_->idle_after_reply ();
      TAO_Transport::release (this->transport_);
    }
}

// Must override pure virtual method in TAO_Reply_Dispatcher.
int
TAO_Asynch_Reply_Dispatcher_Base::dispatch_reply (
    TAO_Pluggable_Reply_Params & /*params*/
  )
{
  return 0;
}


void
TAO_Asynch_Reply_Dispatcher_Base::connection_closed (void)
{
}

void
TAO_Asynch_Reply_Dispatcher_Base::reply_timed_out (void)
{
}

long
TAO_Asynch_Reply_Dispatcher_Base::schedule_timer (CORBA::ULong /*request_id */,
                                                  const ACE_Time_Value & /*max_wait_time*/)
{
  return 0;
}


// ************************************************************************

#if (TAO_HAS_AMI_CALLBACK == 1) || (TAO_HAS_AMI_POLLER == 1)

// Constructor.
TAO_Asynch_Reply_Dispatcher::TAO_Asynch_Reply_Dispatcher (
    const TAO_Reply_Handler_Skeleton &reply_handler_skel,
    Messaging::ReplyHandler_ptr reply_handler,
    TAO_ORB_Core *orb_core
  )
  :TAO_Asynch_Reply_Dispatcher_Base (orb_core),
   reply_handler_skel_ (reply_handler_skel),
   reply_handler_ (Messaging::ReplyHandler::_duplicate (reply_handler)),
   timeout_handler_ (this, orb_core->reactor ())
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
  // AMI Timeout Handling Begin

  timeout_handler_.cancel ();

  // AMI Timeout Handling End

  this->reply_status_ = params.reply_status_;

  // Transfer the <params.input_cdr_>'s content to this->reply_cdr_
  ACE_Data_Block *db =
    this->reply_cdr_.clone_from (params.input_cdr_);

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
  CORBA::ULong max = params.svc_ctx_.maximum ();
  CORBA::ULong len = params.svc_ctx_.length ();
  IOP::ServiceContext *context_list = params.svc_ctx_.get_buffer (1);
  this->reply_service_info_.replace (max, len, context_list, 1);


  if (TAO_debug_level >= 4)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P | %t):TAO_Asynch_Reply_Dispatcher::")
                  ACE_TEXT ("dispatch_reply:\n")));
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
      // AMI Timeout Handling Begin

      timeout_handler_.cancel ();

      // AMI Timeout Handling End

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

  // Commit suicide.
  delete this;
}

// AMI Timeout Handling Begin

void
TAO_Asynch_Reply_Dispatcher::reply_timed_out (void)
{
  ACE_TRY_NEW_ENV
    {
      // Generate a fake exception....
      CORBA::TIMEOUT timeout_failure (
        CORBA_SystemException::_tao_minor_code (TAO_TIMEOUT_SEND_MINOR_CODE,
                                                errno),
         CORBA::COMPLETED_NO);

      TAO_OutputCDR out_cdr;

      timeout_failure._tao_encode (out_cdr ACE_ENV_ARG_PARAMETER);
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
                               "Asynch_Reply_Dispacher::reply_timed_out");
        }

    }
  ACE_ENDTRY;

  // This was dynamically allocated. Now the job is done. Commit
  // suicide here.
  delete this;
}

long
TAO_Asynch_Reply_Dispatcher::schedule_timer (CORBA::ULong request_id,
                                             const ACE_Time_Value &max_wait_time)
{
  return this->timeout_handler_.schedule_timer (this->transport_->tms (),
                                                request_id,
                                                max_wait_time);
}

// AMI Timeout Handling End

#endif /* TAO_HAS_AMI_CALLBACK == 1 || TAO_HAS_AMI_POLLER == 1 */
