// $Id$


#include "DII_Reply_Dispatcher.h"

ACE_RCSID(DynamicInterface, DII_Reply_Dispatcher, "$Id$")

#include "Request.h"
#include "tao/Pluggable.h"
#include "tao/Environment.h"
#include "tao/GIOP_Message_State.h"
#include "tao/debug.h"

// Constructor.
TAO_DII_Deferred_Reply_Dispatcher::TAO_DII_Deferred_Reply_Dispatcher (const CORBA::Request_ptr req)
  : req_ (req)
{
}

// Destructor.
TAO_DII_Deferred_Reply_Dispatcher::~TAO_DII_Deferred_Reply_Dispatcher (void)
{
}

// Dispatch the reply.
int
TAO_DII_Deferred_Reply_Dispatcher::dispatch_reply (
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
  IOP::ServiceContext* context_list = reply_ctx.get_buffer (1);
  this->reply_service_info_.replace (max, len, context_list, 1);

  if (TAO_debug_level >= 4)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P | %t):TAO_Asynch_Reply_Dispatcher::dispatch_reply:\n")));
    }

  ACE_TRY_NEW_ENV
    {
      // Call the Request back and send the reply data.
      this->req_->handle_response (this->message_state_->cdr,
                                   reply_status,
                                   ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      if (TAO_debug_level >= 4)
        {
          ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                               "Exception during reply handler");
        }
    }
  ACE_ENDTRY;

  // This was dynamically allocated. Now the job is done. Commit
  // suicide here.
  delete this;

  return 1;
}

void
TAO_DII_Deferred_Reply_Dispatcher::connection_closed (void)
{
  ACE_TRY_NEW_ENV
    {
      // Generate a fake exception....
      CORBA::COMM_FAILURE comm_failure (0, 
                                        CORBA::COMPLETED_MAYBE);

      TAO_OutputCDR out_cdr;

      comm_failure._tao_encode (out_cdr, 
                                ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Turn into an output CDR
      TAO_InputCDR cdr (out_cdr);
      
      this->req_->handle_response (cdr,
                                   TAO_PLUGGABLE_MESSAGE_SYSTEM_EXCEPTION,
                                   ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      if (TAO_debug_level >= 4)
        {
          ACE_PRINT_EXCEPTION (
              ACE_ANY_EXCEPTION,
              "DII_Deferred_Reply_Dispacher::connection_closed"
            );
        }
    }
  ACE_ENDTRY;
}

