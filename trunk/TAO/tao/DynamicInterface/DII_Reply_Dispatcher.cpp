// $Id$

#include "tao/DynamicInterface/DII_Reply_Dispatcher.h"
#include "tao/DynamicInterface/Request.h"
#include "tao/Environment.h"
#include "tao/debug.h"
#include "tao/ORB_Core.h"
#include "tao/Pluggable_Messaging_Utils.h"


ACE_RCSID(DynamicInterface,
          DII_Reply_Dispatcher,
          "$Id$")


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Constructor.
TAO_DII_Deferred_Reply_Dispatcher::TAO_DII_Deferred_Reply_Dispatcher (
    const CORBA::Request_ptr req,
    TAO_ORB_Core *orb_core)
  : TAO_Asynch_Reply_Dispatcher_Base (orb_core)
  , req_ (req)
{
}

// Destructor.
TAO_DII_Deferred_Reply_Dispatcher::~TAO_DII_Deferred_Reply_Dispatcher (void)
{
}

// Dispatch the reply.
int
TAO_DII_Deferred_Reply_Dispatcher::dispatch_reply (
    TAO_Pluggable_Reply_Params &params
  )
{
  if (params.input_cdr_ == 0)
    return -1;

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
            "TAO (%P|%t) - DII_Deferred_Reply_Dispatcher::dispatch_reply "
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
  CORBA::ULong max = params.svc_ctx_.maximum ();
  CORBA::ULong len = params.svc_ctx_.length ();
  IOP::ServiceContext* context_list = params.svc_ctx_.get_buffer (1);
  this->reply_service_info_.replace (max, len, context_list, 1);

  if (TAO_debug_level >= 4)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P | %t):TAO_Asynch_Reply_Dispatcher::dispatch_reply:\n")));
    }

  ACE_TRY_NEW_ENV
    {
      // Call the Request back and send the reply data.
      this->req_->handle_response (this->reply_cdr_,
                                   this->reply_status_
                                   ACE_ENV_ARG_PARAMETER);
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

  // This was dynamically allocated. Now the job is done.
  (void) this->decr_refcount ();

  return 1;
}

void
TAO_DII_Deferred_Reply_Dispatcher::connection_closed (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      // Generate a fake exception....
      CORBA::COMM_FAILURE comm_failure (0,
                                        CORBA::COMPLETED_MAYBE);

      TAO_OutputCDR out_cdr;

      comm_failure._tao_encode (out_cdr
                                ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Turn into an output CDR
      TAO_InputCDR cdr (out_cdr);

      this->req_->handle_response (cdr,
                                   TAO_PLUGGABLE_MESSAGE_SYSTEM_EXCEPTION
                                   ACE_ENV_ARG_PARAMETER);
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
  ACE_CHECK;

  (void) this->decr_refcount ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
