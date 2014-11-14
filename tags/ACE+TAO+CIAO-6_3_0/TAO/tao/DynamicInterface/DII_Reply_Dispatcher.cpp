// $Id$

#include "tao/DynamicInterface/DII_Reply_Dispatcher.h"
#include "tao/DynamicInterface/Request.h"
#include "tao/debug.h"
#include "tao/ORB_Core.h"
#include "tao/Pluggable_Messaging_Utils.h"
#include "tao/SystemException.h"

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
    TAO_Pluggable_Reply_Params &params)
{
  if (params.input_cdr_ == 0)
    return -1;

  this->reply_status_ = params.reply_status ();
  this->locate_reply_status_ = params.locate_reply_status ();

  // Transfer the <params.input_cdr_>'s content to this->reply_cdr_
  ACE_Data_Block *db = this->reply_cdr_.clone_from (*params.input_cdr_);

  if (db == 0)
    {
      if (TAO_debug_level > 2)
        {
          TAOLIB_ERROR ((
            LM_ERROR,
            "TAO (%P|%t) - DII_Deferred_Reply_Dispatcher::dispatch_reply "
            "clone_from failed\n"));
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
      TAOLIB_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P | %t):TAO_Asynch_Reply_Dispatcher::dispatch_reply:\n")));
    }

  try
    {
      // Call the Request back and send the reply data.
      this->req_->handle_response (this->reply_cdr_, this->reply_status_);
    }
  catch (const ::CORBA::Exception& ex)
    {
      if (TAO_debug_level >= 4)
        {
          ex._tao_print_exception ("Exception during reply handler");
        }
    }

  // This was dynamically allocated. Now the job is done.
  this->intrusive_remove_ref (this);

  return 1;
}

void
TAO_DII_Deferred_Reply_Dispatcher::connection_closed (void)
{

  try
    {
      // Generate a fake exception....
      CORBA::COMM_FAILURE comm_failure (0, CORBA::COMPLETED_MAYBE);

      TAO_OutputCDR out_cdr;

      comm_failure._tao_encode (out_cdr);

      // Turn into an output CDR
      TAO_InputCDR cdr (out_cdr);

      this->req_->handle_response (cdr, GIOP::SYSTEM_EXCEPTION);
    }
  catch (const ::CORBA::Exception& ex)
    {
      if (TAO_debug_level >= 4)
        {
          ex._tao_print_exception (
            "DII_Deferred_Reply_Dispacher::connection_closed");
        }
    }

  this->intrusive_remove_ref (this);
}

TAO_DII_Asynch_Reply_Dispatcher::TAO_DII_Asynch_Reply_Dispatcher (
    const Messaging::ReplyHandler_ptr callback,
    TAO_ORB_Core *orb_core)
  : TAO_Asynch_Reply_Dispatcher_Base (orb_core),
    db_ (sizeof buf_,
         ACE_Message_Block::MB_DATA,
         this->buf_,
         orb_core->input_cdr_buffer_allocator (),
         orb_core->locking_strategy (),
         ACE_Message_Block::DONT_DELETE,
         orb_core->input_cdr_dblock_allocator ()),
    reply_cdr_ (&db_,
                ACE_Message_Block::DONT_DELETE,
                TAO_ENCAP_BYTE_ORDER,
                TAO_DEF_GIOP_MAJOR,
                TAO_DEF_GIOP_MINOR,
                orb_core),
    callback_ (callback)
{
}

TAO_DII_Asynch_Reply_Dispatcher::~TAO_DII_Asynch_Reply_Dispatcher (void)
{
  // this was handed to us by the caller.
  CORBA::release(callback_);
}

int
TAO_DII_Asynch_Reply_Dispatcher::dispatch_reply (
    TAO_Pluggable_Reply_Params &params)
{
  this->reply_status_ = params.reply_status ();
  this->locate_reply_status_ = params.locate_reply_status ();

  // Transfer the <params.input_cdr_>'s content to this->reply_cdr_
  ACE_Data_Block *db =
    this->reply_cdr_.clone_from (*params.input_cdr_);

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
      TAOLIB_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P | %t):")
                  ACE_TEXT ("TAO_DII_Asynch_Reply_Dispatcher::dispatch_reply: status = %d\n"),
                  this->reply_status_));
    }

  try
    {
      // Call the handler with the reply data.
      CORBA::Request::_tao_reply_stub (this->reply_cdr_,
                                       this->callback_,
                                       this->reply_status_);
    }
  catch (const CORBA::Exception& ex)
    {
      if (TAO_debug_level >= 4)
        {
          ex._tao_print_exception ("Exception during reply handler");
        }
    }
  // This was dynamically allocated. Now the job is done.
  this->intrusive_remove_ref (this);

  return 1;
}

void
TAO_DII_Asynch_Reply_Dispatcher::connection_closed (void)
{
  try
    {
      // Generate a fake exception....
      CORBA::COMM_FAILURE comm_failure (0,
                                        CORBA::COMPLETED_MAYBE);

      TAO_OutputCDR out_cdr;

      comm_failure._tao_encode (out_cdr);

      // Turn into an output CDR
      TAO_InputCDR cdr (out_cdr);
      CORBA::Request::_tao_reply_stub (
          this->reply_cdr_,
          this->callback_,
          GIOP::SYSTEM_EXCEPTION);
    }
  catch (const CORBA::Exception& ex)
    {
      if (TAO_debug_level >= 4)
        {
          ex._tao_print_exception (
            "DII_Asynch_Reply_Dispacher::connection_closed");
        }
    }

  this->intrusive_remove_ref (this);
}
TAO_END_VERSIONED_NAMESPACE_DECL
