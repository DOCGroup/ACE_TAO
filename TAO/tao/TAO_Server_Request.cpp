// $Id$


// Implementation of the Dynamic Server Skeleton Interface  (for GIOP)

#include "TAO_Server_Request.h"
#include "CDR.h"
#include "Environment.h"
#include "Principal.h"
#include "ORB_Core.h"
#include "ORB.h"
#include "Timeprobe.h"
#include "debug.h"
#include "Pluggable_Messaging_Utils.h"
#include "Pluggable_Messaging.h"

// @@ Should not be included. But, for the timebeing.
#include "GIOP_Utils.h"

#include "Transport.h"

#if !defined (__ACE_INLINE__)
# include "TAO_Server_Request.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID (tao,
           TAO_Server_Request,
           "$Id$")

#if defined (ACE_ENABLE_TIMEPROBES)

static const char *TAO_Server_Request_Timeprobe_Description[] =
  {
    "TAO_ServerRequest::TAO_ServerRequest - start",
    "TAO_ServerRequest::TAO_ServerRequest - end",
  };

enum
  {
    // Timeprobe description table start key.
    TAO_SERVER_REQUEST_START = 400,
    TAO_SERVER_REQUEST_END
  };

// Setup Timeprobes
ACE_TIMEPROBE_EVENT_DESCRIPTIONS (TAO_Server_Request_Timeprobe_Description,
                                  TAO_SERVER_REQUEST_START);

#endif /* ACE_ENABLE_TIMEPROBES */

TAO_ServerRequest::TAO_ServerRequest (TAO_Pluggable_Messaging *mesg_base,
                                      TAO_InputCDR &input,
                                      TAO_OutputCDR &output,
                                      TAO_Transport *transport,
                                      TAO_ORB_Core *orb_core)
  : mesg_base_ (mesg_base),
    operation_ (),
    incoming_ (&input),
    outgoing_ (&output),
    transport_(TAO_Transport::_duplicate (transport)),
    response_expected_ (0),
    deferred_reply_ (0),
    sync_with_server_ (0),
    // @@ We shouldn't be using GIOP specific types here. Need to be revisited.
    exception_type_ (TAO_GIOP_NO_EXCEPTION),
    orb_core_ (orb_core),
    request_id_ (0),
    profile_ (orb_core),
    requesting_principal_ (0),
    is_dsi_ (0),
    dsi_nvlist_align_ (0),
    argument_flag_ (1)
#if TAO_HAS_INTERCEPTORS == 1
    , interceptor_count_ (0)
    , rs_pi_current_ ()
#endif  /* TAO_HAS_INTERCEPTORS == 1 */
{
  ACE_FUNCTION_TIMEPROBE (TAO_SERVER_REQUEST_START);
  // No-op.
}

// This constructor is used, by the locate request code.
TAO_ServerRequest::TAO_ServerRequest (TAO_Pluggable_Messaging *mesg_base,
                                      CORBA::ULong request_id,
                                      CORBA::Boolean response_expected,
                                      CORBA::Boolean deferred_reply,
                                      TAO_ObjectKey &object_key,
                                      const char *operation,
                                      TAO_OutputCDR &output,
                                      TAO_Transport *transport,
                                      TAO_ORB_Core *orb_core,
                                      int &parse_error)
  : mesg_base_ (mesg_base),
    operation_ (operation),
    incoming_ (0),
    outgoing_ (&output),
    transport_ (TAO_Transport::_duplicate (transport)),
    response_expected_ (response_expected),
    deferred_reply_ (deferred_reply),
    sync_with_server_ (0),
    exception_type_ (TAO_GIOP_NO_EXCEPTION),
    orb_core_ (orb_core),
    request_id_ (request_id),
    profile_ (orb_core),
    requesting_principal_ (0),
    is_dsi_ (0),
    dsi_nvlist_align_ (0),
    argument_flag_ (1)
#if TAO_HAS_INTERCEPTORS == 1
    , interceptor_count_ (0)
    , rs_pi_current_ ()
#endif  /* TAO_HAS_INTERCEPTORS == 1 */
{
  profile_.object_key (object_key);
  parse_error = 0;
}

TAO_ServerRequest::~TAO_ServerRequest (void)
{
  TAO_Transport::release (this->transport_);
}

void 
TAO_ServerRequest::instantiate_AMH_Handler (TAO_AMH_Response_Handler* response_handler)  
{
  response_handler->mesg_base (mesg_base_);
  response_handler->request_id (request_id ());
  response_handler->response_expected (response_expected_);
  response_handler->transport (TAO_Transport::_duplicate (transport()));
  //response_handler->reply_service_info (reply_service_info ());
}


CORBA::ORB_ptr
TAO_ServerRequest::orb (void)
{
  return this->orb_core_->orb ();
}

void
TAO_ServerRequest::init_reply (void)
{
  // Construct our reply generator.
  TAO_Pluggable_Reply_Params_Base reply_params;

  // We put all the info that we have in to this <reply_params> and
  // call the <write_reply_header> in the
  // pluggable_messaging_interface. One point to be noted however is
  // that, it was the pluggable_messaging classes who created us and
  // delegated us to do work on its behalf. But we would be calling
  // back. As we dont have a LOCK or any such things we can call
  // pluggable_messaging guys again. We would be on the same thread of
  // invocation. So *theoratically* there should not be a problem.
  reply_params.request_id_ = this->request_id_;
  reply_params.is_dsi_ = this->is_dsi_;
  reply_params.dsi_nvlist_align_ = this->dsi_nvlist_align_;

  // Send back the reply service context.
  reply_params.service_context_notowned (&this->reply_service_info ());

  // Are we going to marshall any data with the reply?
  reply_params.argument_flag_ = this->argument_flag_;

  // Forward exception only.
  if (!CORBA::is_nil (this->forward_location_.in ()))
    {
      reply_params.reply_status_ = TAO_PLUGGABLE_MESSAGE_LOCATION_FORWARD;
    }
  // Any exception at all.
  else if (this->exception_type_ == TAO_GIOP_NO_EXCEPTION)
    {
      reply_params.reply_status_ = TAO_PLUGGABLE_MESSAGE_NO_EXCEPTION;
    }
  else
    {
      reply_params.reply_status_ = this->exception_type_;
    }

  // Construct a REPLY header.
  this->mesg_base_->generate_reply_header (*this->outgoing_,
                                           reply_params);

  // Finish the GIOP Reply header, then marshal the exception.
  if (reply_params.reply_status_ == TAO_PLUGGABLE_MESSAGE_LOCATION_FORWARD)
    {
      // Marshal the forward location pointer.
      CORBA::Object_ptr object_ptr = this->forward_location_.in ();

      if ((*this->outgoing_ << object_ptr) == 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO_GIOP_ServerRequest::marshal - ")
                      ACE_TEXT ("encoding forwarded objref failed\n")));
        }
    }
}

void
TAO_ServerRequest::send_no_exception_reply (void)
{
  // Construct our reply generator.
  TAO_Pluggable_Reply_Params_Base reply_params;
  reply_params.request_id_ = this->request_id_;
  reply_params.is_dsi_ = this->is_dsi_;
  reply_params.dsi_nvlist_align_ = this->dsi_nvlist_align_;

  // Change this to pass back the same thing we received, as well as
  // leave a comment why this is important!
  reply_params.svc_ctx_.length (0);

  // Send back the reply service context.
  reply_params.service_context_notowned (&this->reply_service_info ());

  reply_params.reply_status_ = TAO_GIOP_NO_EXCEPTION;

  // No data anyway.
  reply_params.argument_flag_ = 0;

  // Construct a REPLY header.
  this->mesg_base_->generate_reply_header (*this->outgoing_,
                                           reply_params);

  // Send the message.
  int result = this->transport_->send_message (*this->outgoing_);

  if (result == -1)
    {
      if (TAO_debug_level > 0)
        {
          // No exception but some kind of error, yet a response
          // is required.
          ACE_ERROR ((
              LM_ERROR,
              ACE_TEXT ("TAO: (%P|%t) %p: cannot send NO_EXCEPTION reply\n"),
              ACE_TEXT ("TAO_GIOP_ServerRequest::send_no_exception_reply")
            ));
        }
    }
}

void
TAO_ServerRequest::tao_send_reply (void)
{
  int result = this->transport_->send_message (*this->outgoing_);
  if (result == -1)
    {
      if (TAO_debug_level > 0)
        {
          // No exception but some kind of error, yet a response
          // is required.
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("TAO: (%P|%t) %p: cannot send reply\n"),
                      ACE_TEXT ("TAO_ServerRequest::tao_send_reply")));

        }
    }
}

void
TAO_ServerRequest::tao_send_reply_exception (CORBA::Exception &ex)
{
  //  int result = 0;
  if (this->response_expected_)
    {
      // A copy of the reply parameters
      TAO_Pluggable_Reply_Params_Base reply_params;

      reply_params.request_id_ = this->request_id_;
      reply_params.svc_ctx_.length (0);

      // Send back the reply service context.
      reply_params.service_context_notowned (&this->reply_service_info ());

      // We are going to send some data
      reply_params.argument_flag_ = 1;

      // Make a default reply status
      reply_params.reply_status_ = TAO_GIOP_USER_EXCEPTION;

      // Check whether we are able to downcast the exception
      if (CORBA::SystemException::_downcast (&ex) != 0)
        {
          reply_params.reply_status_ = TAO_GIOP_SYSTEM_EXCEPTION;
        }

      // Create a new output CDR stream
#if defined(ACE_HAS_PURIFY)
      // Only inititialize the buffer if we're compiling with Purify.
      // Otherwise, there is no real need to do so, especially since
      // we can avoid the initialization overhead at run-time if we
      // are not compiling with Purify support.
      char repbuf[ACE_CDR::DEFAULT_BUFSIZE] = { 0 };
#else
      char repbuf[ACE_CDR::DEFAULT_BUFSIZE];
#endif /* ACE_HAS_PURIFY */

      TAO_OutputCDR output (repbuf,
                            sizeof repbuf,
                            TAO_ENCAP_BYTE_ORDER,
                            this->orb_core_->output_cdr_buffer_allocator (),
                            this->orb_core_->output_cdr_dblock_allocator (),
                            this->orb_core_->output_cdr_msgblock_allocator (),
                            this->orb_core_->orb_params ()->cdr_memcpy_tradeoff (),
                            TAO_DEF_GIOP_MAJOR,
                            TAO_DEF_GIOP_MINOR,
                            this->orb_core_->to_iso8859 (),
                            this->orb_core_->to_unicode ());

      // Make the reply message
      if (this->mesg_base_->generate_exception_reply (output,
                                                      reply_params,
                                                      ex) == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("TAO: (%P|%t|%N|%l): ")
                      ACE_TEXT ("could not make exception reply\n")));

        }

      // Send the message
      if (this->transport_->send_message (output) == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("TAO: (%P|%t|%N|%l):  ")
                      ACE_TEXT ("could not send exception reply\n")));
        }
    }
  else if (TAO_debug_level > 0)
    {
      // It is unfotunate that an exception (probably a system
      // exception) was thrown by the upcall code (even by the
      // user) when the client was not expecting a response.
      // However, in this case, we cannot close the connection
      // down, since it really isn't the client's fault.

      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) exception thrown ")
                  ACE_TEXT ("but client is not waiting a response\n")));
    }
}
