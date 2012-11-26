// -*- C++ -*-
// $Id$

#include "tao/TAO_Server_Request.h"
#include "tao/ORB_Core.h"
#include "tao/Timeprobe.h"
#include "tao/debug.h"
#include "tao/GIOP_Message_Base.h"
#include "tao/GIOP_Utils.h"
#include "tao/Stub.h"
#include "tao/operation_details.h"
#include "tao/Transport.h"
#include "tao/CDR.h"
#include "tao/SystemException.h"

#if TAO_HAS_INTERCEPTORS == 1
#include "tao/PortableInterceptorC.h"
#include "tao/ServerRequestInterceptor_Adapter.h"
#endif

#if !defined (__ACE_INLINE__)
# include "tao/TAO_Server_Request.inl"
#endif /* ! __ACE_INLINE__ */

#if defined (ACE_ENABLE_TIMEPROBES)

static const char * TAO_Server_Request_Timeprobe_Description[] =
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

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_ServerRequest::TAO_ServerRequest (TAO_GIOP_Message_Base *mesg_base,
                                      TAO_InputCDR &input,
                                      TAO_OutputCDR &output,
                                      TAO_Transport *transport,
                                      TAO_ORB_Core *orb_core)
  : mesg_base_ (mesg_base),
    operation_ (0),
    operation_len_ (0),
    release_operation_ (false),
    is_forwarded_ (false),
    incoming_ (&input),
    outgoing_ (&output),
    response_expected_ (false),
    deferred_reply_ (false),
    sync_with_server_ (false),
    is_dsi_ (false),
    // @@ We shouldn't be using GIOP specific types here. Need to be revisited.
    reply_status_ (GIOP::NO_EXCEPTION),
    orb_core_ (orb_core),
    request_id_ (0),
    profile_ (orb_core),
    requesting_principal_ (0),
    dsi_nvlist_align_ (0),
    operation_details_ (0),
    argument_flag_ (true)
#if TAO_HAS_INTERCEPTORS == 1
    , interceptor_count_ (0)
    , rs_pi_current_ (0)
    , caught_exception_ (0)
    , pi_reply_status_ (-1)
#endif  /* TAO_HAS_INTERCEPTORS == 1 */
    , transport_(transport) //already duplicated in TAO_Transport::process_parsed_messages ()
{
  ACE_FUNCTION_TIMEPROBE (TAO_SERVER_REQUEST_START);
  // No-op.
}

// This constructor is used, by the locate request code.
TAO_ServerRequest::TAO_ServerRequest (TAO_GIOP_Message_Base *mesg_base,
                                      CORBA::ULong request_id,
                                      CORBA::Boolean response_expected,
                                      CORBA::Boolean deferred_reply,
                                      TAO::ObjectKey &object_key,
                                      const char *operation,
                                      TAO_OutputCDR &output,
                                      TAO_Transport *transport,
                                      TAO_ORB_Core *orb_core,
                                      int &parse_error)
  : mesg_base_ (mesg_base),
    operation_ (CORBA::string_dup (operation)),
    operation_len_ (operation == 0 ? 0 : ACE_OS::strlen (operation)),
    release_operation_ (true),
    is_forwarded_ (false),
    incoming_ (0),
    outgoing_ (&output),
    response_expected_ (response_expected),
    deferred_reply_ (deferred_reply),
    sync_with_server_ (false),
    is_dsi_ (false),
    reply_status_ (GIOP::NO_EXCEPTION),
    orb_core_ (orb_core),
    request_id_ (request_id),
    profile_ (orb_core),
    requesting_principal_ (0),
    dsi_nvlist_align_ (0),
    operation_details_ (0),
    argument_flag_ (true)
#if TAO_HAS_INTERCEPTORS == 1
  , interceptor_count_ (0)
  , rs_pi_current_ (0)
  , caught_exception_ (0)
  , pi_reply_status_ (-1)
#endif  /* TAO_HAS_INTERCEPTORS == 1 */
  , transport_(transport) //already duplicated in TAO_Transport::process_parsed_messages ()
{
  this->profile_.object_key (object_key);
  parse_error = 0;
}

// Constructor used in Thru-POA collocation code.
TAO_ServerRequest::TAO_ServerRequest (TAO_ORB_Core * orb_core,
                                      TAO_Operation_Details const & details,
                                      CORBA::Object_ptr target)
  : mesg_base_ (0),
    operation_ (details.opname ()),
    operation_len_ (details.opname_len ()),
    release_operation_ (false),
    is_forwarded_ (false),
    incoming_ (0),
    outgoing_ (0),
    response_expected_ (details.response_flags () == TAO_TWOWAY_RESPONSE_FLAG
                        || details.response_flags () == static_cast<CORBA::Octet> (Messaging::SYNC_WITH_SERVER)
                        || details.response_flags () == static_cast<CORBA::Octet> (Messaging::SYNC_WITH_TARGET)),
    deferred_reply_ (false),
    sync_with_server_ (details.response_flags () == static_cast<CORBA::Octet> (Messaging::SYNC_WITH_SERVER)),
    is_dsi_ (false),
    reply_status_ (GIOP::NO_EXCEPTION),
    orb_core_ (orb_core),
    request_id_ (0),
    profile_ (orb_core),
    requesting_principal_ (0),
    dsi_nvlist_align_ (0),
    operation_details_ (&details),
    argument_flag_ (false)
#if TAO_HAS_INTERCEPTORS == 1
  , interceptor_count_ (0)
  , rs_pi_current_ (0)
  , caught_exception_ (0)
  , pi_reply_status_ (-1)
#endif  /* TAO_HAS_INTERCEPTORS == 1 */
  , transport_ (0)
{
  // Have to use a const_cast<>.  *sigh*
  this->profile_.object_key (
    const_cast<TAO::ObjectKey &> (target->_stubobj ()->object_key ()));

  // Shallow copy the request service context list. This way the operation
  // details and server request share the request context.
  IOP::ServiceContextList & dest_request_contexts =
    this->request_service_context_.service_info ();

  IOP::ServiceContextList & src_request_contexts =
    (const_cast <TAO_Operation_Details&> (details)).request_service_info ();

  dest_request_contexts.replace (src_request_contexts.maximum (),
                                 src_request_contexts.length (),
                                 src_request_contexts.get_buffer (),
                                 false /* Do not release. */);

  // Don't shallow copy the reply service context. It is probably empty,
  // when then during the request it is used, the buffer gets allocated and
  // then the operation details don't get the reply service context
}

TAO_ServerRequest::~TAO_ServerRequest (void)
{
#if TAO_HAS_INTERCEPTORS == 1
  if (this->rs_pi_current_)
    {
      TAO::ServerRequestInterceptor_Adapter *interceptor_adapter =
        this->orb_core_->serverrequestinterceptor_adapter ();

      if (interceptor_adapter)
        {
          interceptor_adapter->deallocate_pi_current (
            this->rs_pi_current_);
        }
    }
#endif  /* TAO_HAS_INTERCEPTORS == 1 */
  if (this->release_operation_)
    CORBA::string_free (const_cast<char*> (this->operation_));
}

CORBA::ORB_ptr
TAO_ServerRequest::orb (void)
{
  return this->orb_core_->orb ();
}

TAO_Service_Context &
TAO_ServerRequest::reply_service_context (void)
{
  if (!operation_details_)
    {
      return this->reply_service_context_;
    }
  else
    {
      return const_cast <TAO_Operation_Details*> (
        this->operation_details_)->reply_service_context ();
    }
}


void
TAO_ServerRequest::init_reply (void)
{
  if (!this->outgoing_)
    return;  // Collocated

  // Construct our reply generator.
  TAO_Pluggable_Reply_Params_Base reply_params;

  // We put all the info that we have in to this <reply_params> and
  // call the <write_reply_header> in the
  // pluggable_messaging_interface. One point to be noted however is
  // that, it was the pluggable_messaging classes who created us and
  // delegated us to do work on its behalf. But we would be calling
  // back. As we don't have a LOCK or any such things we can call
  // pluggable_messaging guys again. We would be on the same thread of
  // invocation. So *theoretically* there should not be a problem.
  reply_params.request_id_ = this->request_id_;
  reply_params.is_dsi_ = this->is_dsi_;
  reply_params.dsi_nvlist_align_ = this->dsi_nvlist_align_;

  // Send back the reply service context.
  reply_params.service_context_notowned (&this->reply_service_info ());

  // Are we going to marshall any data with the reply?
  reply_params.argument_flag_ = this->argument_flag_;

  // Forward exception only.
  if (this->is_forwarded_)
    {
      CORBA::Boolean const permanent_forward_condition =
        this->orb_core_->is_permanent_forward_condition (this->forward_location_.in (),
                                                         this->request_service_context ());

      reply_params.reply_status (
        permanent_forward_condition
        ? GIOP::LOCATION_FORWARD_PERM
        : GIOP::LOCATION_FORWARD);
    }
  // Any exception at all.
  else
    {
      reply_params.reply_status (this->reply_status_);
    }

  this->outgoing_->message_attributes (this->request_id_,
                                       0,
                                       TAO_Message_Semantics (TAO_Message_Semantics::TAO_REPLY),
                                       0);

  // Construct a REPLY header.
  this->mesg_base_->generate_reply_header (*this->outgoing_, reply_params);

  // Finish the GIOP Reply header, then marshal the exception.
  if (reply_params.reply_status () == GIOP::LOCATION_FORWARD ||
      reply_params.reply_status () == GIOP::LOCATION_FORWARD_PERM)
    {
      // Marshal the forward location pointer.
      CORBA::Object_ptr object_ptr = this->forward_location_.in ();

      if ((*this->outgoing_ << object_ptr) == 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) - ServerRequest::init_reply, ")
                      ACE_TEXT ("TAO_GIOP_ServerRequest::marshal - ")
                      ACE_TEXT ("marshal encoding forwarded objref failed\n")));
        }
    }
  this->transport_->assign_translators (0, this->outgoing_);
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

  reply_params.reply_status (GIOP::NO_EXCEPTION);

  // No data anyway.
  reply_params.argument_flag_ = false;

  this->outgoing_->message_attributes (this->request_id_,
                                       0,
                                       TAO_Message_Semantics (TAO_Message_Semantics::TAO_REPLY),
                                       0);

  // Construct a REPLY header.
  this->mesg_base_->generate_reply_header (*this->outgoing_, reply_params);

  this->outgoing_->more_fragments (false);

  // Send the message.
  int const result = this->transport_->send_message (*this->outgoing_,
                                                     0,
                                                     this,
                                                     TAO_Message_Semantics (TAO_Message_Semantics::TAO_REPLY));

  if (result == -1)
    {
      if (TAO_debug_level > 0)
        {
          // No exception but some kind of error, yet a response
          // is required.
          ACE_ERROR ((
                      LM_ERROR,
                      ACE_TEXT ("TAO (%P|%t) - ServerRequest::send_no_exception_reply, ")
                      ACE_TEXT ("cannot send NO_EXCEPTION reply\n")
                      ));
        }
    }
}

void
TAO_ServerRequest::tao_send_reply (void)
{
  if (this->collocated ())
    return;  // No transport in the collocated case.

  this->outgoing_->more_fragments (false);

  int const result = this->transport_->send_message (*this->outgoing_,
                                                     0,
                                                     this,
                                                     TAO_Message_Semantics (TAO_Message_Semantics::TAO_REPLY));
  if (result == -1)
    {
      if (TAO_debug_level > 0)
        {
          // No exception but some kind of error, yet a response
          // is required.
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("TAO (%P|%t) - ServerRequest::tao_send_reply, ")
                      ACE_TEXT ("cannot send reply\n")));
        }
    }
}

void
TAO_ServerRequest::tao_send_reply_exception (const CORBA::Exception &ex)
{
  if (this->response_expected_ && !this->collocated ())
    {
      // A copy of the reply parameters
      TAO_Pluggable_Reply_Params_Base reply_params;

      reply_params.request_id_ = this->request_id_;
      reply_params.svc_ctx_.length (0);

      // Send back the reply service context.
      reply_params.service_context_notowned (&this->reply_service_info ());

      // We are going to send some data
      reply_params.argument_flag_ = true;

      // Make a default reply status

      // Check whether we are able to downcast the exception
      if (CORBA::SystemException::_downcast (&ex) != 0)
        {
          reply_params.reply_status (GIOP::SYSTEM_EXCEPTION);
        }
      else
        {
          reply_params.reply_status (GIOP::USER_EXCEPTION);
        }

      // Create a new output CDR stream
#if defined(ACE_INITIALIZE_MEMORY_BEFORE_USE)
      // Only inititialize the buffer if we're compiling with a profiler.
      // Otherwise, there is no real need to do so, especially since
      // we can avoid the initialization overhead at runtime if we
      // are not compiling with memory profiler support.
      char repbuf[ACE_CDR::DEFAULT_BUFSIZE] = { 0 };
#else
      char repbuf[ACE_CDR::DEFAULT_BUFSIZE];
#endif /* ACE_INITIALIZE_MEMORY_BEFORE_USE */
      TAO_GIOP_Message_Version gv;
      if (this->outgoing_)
        this->outgoing_->get_version (gv);

      TAO_OutputCDR output (repbuf,
                            sizeof repbuf,
                            TAO_ENCAP_BYTE_ORDER,
                            this->orb_core_->output_cdr_buffer_allocator (),
                            this->orb_core_->output_cdr_dblock_allocator (),
                            this->orb_core_->output_cdr_msgblock_allocator (),
                            this->orb_core_->orb_params ()->cdr_memcpy_tradeoff (),
                            this->mesg_base_->fragmentation_strategy (),
                            gv.major,
                            gv.minor);

      this->transport_->assign_translators (0, &output);

      try
        {
          // Make the reply message
          if (this->mesg_base_->generate_exception_reply (*this->outgoing_,
                                                          reply_params,
                                                          ex) == -1)
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("TAO (%P|%t) - ServerRequest::")
                          ACE_TEXT ("tao_send_reply_exception, ")
                          ACE_TEXT ("could not make exception reply\n")));
            }

          this->outgoing_->more_fragments (false);

          // Send the message
          if (this->transport_->send_message (*this->outgoing_,
                                              0,
                                              this,
                                              TAO_Message_Semantics (TAO_Message_Semantics::TAO_REPLY)) == -1)
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("TAO (%P|%t) - ServerRequest::")
                          ACE_TEXT ("tao_send_reply_exception, ")
                          ACE_TEXT ("could not send exception reply\n")));
            }
        }
      catch (const ::CORBA::BAD_PARAM &bp_ex)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("TAO (%P|%t) - ServerRequest::")
                      ACE_TEXT ("tao_send_reply_exception, ")
                      ACE_TEXT ("could not marshal exception reply\n")));
          this->tao_send_reply_exception (bp_ex);
        }
    }
  else if (TAO_debug_level > 0)
    {
      // It is unfortunate that an exception (probably a system
      // exception) was thrown by the upcall code (even by the
      // user) when the client was not expecting a response.
      // However, in this case, we cannot close the connection
      // down, since it really isn't the client's fault.

      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("TAO (%P|%t) - ServerRequest::tao_send_reply_exception, ")
                  ACE_TEXT ("exception thrown ")
                  ACE_TEXT ("but client is not waiting a response\n")));
    }
}

#if TAO_HAS_INTERCEPTORS == 1
void
TAO_ServerRequest::send_cached_reply (CORBA::OctetSeq &s)
{
#if defined(ACE_INITIALIZE_MEMORY_BEFORE_USE)
  // Only inititialize the buffer if we're compiling with Purify.
  // Otherwise, there is no real need to do so, especially since
  // we can avoid the initialization overhead at runtime if we
  // are not compiling with Purify support.
  char repbuf[ACE_CDR::DEFAULT_BUFSIZE] = { 0 };
#else
  char repbuf[ACE_CDR::DEFAULT_BUFSIZE];
#endif /* ACE_HAS_PURIFY */
  TAO_GIOP_Message_Version gv;
  if (this->outgoing_)
    {
      this->outgoing_->get_version (gv);
    }
  TAO_OutputCDR output (repbuf,
                        sizeof repbuf,
                        TAO_ENCAP_BYTE_ORDER,
                        this->orb_core_->output_cdr_buffer_allocator (),
                        this->orb_core_->output_cdr_dblock_allocator (),
                        this->orb_core_->output_cdr_msgblock_allocator (),
                        this->orb_core_->orb_params ()->cdr_memcpy_tradeoff (),
                        this->mesg_base_->fragmentation_strategy (),
                        gv.major,
                        gv.minor);

  this->transport_->assign_translators (0, &output);

  // A copy of the reply parameters
  TAO_Pluggable_Reply_Params_Base reply_params;

  reply_params.request_id_ = this->request_id_;

  reply_params.svc_ctx_.length (0);

  // Send back the empty reply service context.
  reply_params.service_context_notowned (&this->reply_service_info ());

  // We are going to send some data
  reply_params.argument_flag_ = true;

  // Make a default reply status
  reply_params.reply_status (GIOP::NO_EXCEPTION);

  this->outgoing_->message_attributes (this->request_id_,
                                       0,
                                       TAO_Message_Semantics (TAO_Message_Semantics::TAO_REPLY),
                                       0);

  // Make the reply message
  if (this->mesg_base_->generate_reply_header (*this->outgoing_,
                                               reply_params) == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("TAO (%P|%t) - ServerRequest::send_cached_reply, ")
                  ACE_TEXT ("could not make cached reply\n")));

    }

  /// Append reply here....
  this->outgoing_->write_octet_array (
    s.get_buffer (),
    s.length ());

  if (!this->outgoing_->good_bit ())
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("TAO (%P|%t) - ServerRequest::send_cached_reply, ")
                  ACE_TEXT ("could not marshal reply\n")));
    }

  this->outgoing_->more_fragments (false);

  // Send the message
  if (this->transport_->send_message (*this->outgoing_,
                                      0,
                                      this,
                                      TAO_Message_Semantics (TAO_Message_Semantics::TAO_REPLY)) == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("TAO (%P|%t) - ServerRequest::send_cached_reply, ")
                  ACE_TEXT ("could not send cached reply\n")));
    }
}

void
TAO_ServerRequest::caught_exception (CORBA::Exception *exception)
{
  if (CORBA::SystemException::_downcast (exception) != 0)
    this->pi_reply_status_ = PortableInterceptor::SYSTEM_EXCEPTION;
  else if (CORBA::UserException::_downcast (exception) != 0)
    this->pi_reply_status_ = PortableInterceptor::USER_EXCEPTION;

  this->caught_exception_ = exception;
}

TAO::PICurrent_Impl *
TAO_ServerRequest::rs_pi_current (void)
{
  if (!this->rs_pi_current_)
    {
      TAO::ServerRequestInterceptor_Adapter *interceptor_adapter =
        this->orb_core_->serverrequestinterceptor_adapter ();

      if (interceptor_adapter)
        {
          this->rs_pi_current_ = interceptor_adapter->allocate_pi_current ();
        }
    }

  return this->rs_pi_current_;
}

#endif /* TAO_HAS_INTERCEPTORS */

#if TAO_HAS_ZIOP == 1
CORBA::Policy_ptr
TAO_ServerRequest::clientCompressionEnablingPolicy ()
{
  return this->clientCompressionEnablingPolicy_.in ();
}

void
TAO_ServerRequest::clientCompressionEnablingPolicy (CORBA::Policy_ptr policy)
{
  this->clientCompressionEnablingPolicy_ = policy;
}

CORBA::Policy_ptr
TAO_ServerRequest::clientCompressorIdLevelListPolicy ()
{
  return this->clientCompressorIdLevelListPolicy_.in ();
}

void
TAO_ServerRequest::clientCompressorIdLevelListPolicy (CORBA::Policy_ptr policy)
{
  this->clientCompressorIdLevelListPolicy_ = policy;
}
#endif /* TAO_HAS_ZIOP == 1 */

TAO_END_VERSIONED_NAMESPACE_DECL
