// $Id$

#include "AMH_Response_Handler.h"
#include "tao/TAO_Server_Request.h"
#include "tao/Transport.h"
#include "tao/CDR.h"
#include "tao/ORB_Core.h"
#include "tao/ORB.h"
#include "tao/Pluggable_Messaging.h"
#include "tao/Pluggable_Messaging_Utils.h"
#include "tao/GIOP_Utils.h"
#include "tao/debug.h"

TAO_AMH_Response_Handler::
TAO_AMH_Response_Handler (TAO_ServerRequest &server_request)
  : mesg_base_ (server_request.mesg_base_)
  , request_id_ (server_request.request_id_)
  //  , response_expected_ (server_request.response_expected_)
  , transport_ (TAO_Transport::_duplicate (server_request.transport ()))
  , orb_core_ (server_request.orb_core ())
  , argument_flag_ (1)
  , exception_type_ (TAO_GIOP_NO_EXCEPTION)
  , reply_status_ (TAO_RS_UNINITIALIZED)
{
  // @@ Mayur: I think only *servers* can use the AMH_Response_Handler
  // class, right?  If that is the case we need to come up with an
  // scheme to move the AMH_Response_Handler class to the PortableServer
  // library. Growing the footprint of a pure-client to support AMH
  // makes no sense.
}

TAO_AMH_Response_Handler::~TAO_AMH_Response_Handler (void)
{
  // @@ Mayur: what if the class is destroyed before a reply is sent
  // to the client?  IMHO, we should send some sort of system
  // exception (maybe CORBA::NO_RESPONSE or CORBA::INV_ORDER), with a
  // clear minor code to indicate the problem.

  // release the transport
  TAO_Transport::release (transport_);
}

// @@ Mayur: change the signature of this function to use
// ACE_ENV_SINGLE_ARG_DECL... and then change the generated code in
// the IDL compiler too!
void
TAO_AMH_Response_Handler::_tao_rh_init_reply (void)
{
  // @@ Mayur: you had ACE_SYNCH_MUTEX in the header file, but
  //    TAO_SYNCH_MUTEX on the .cpp file, careful with that stuff!
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);

  if (this->reply_status_ != TAO_RS_UNINITIALIZED)
  {
    // @@ Mayur: the comment is incorrect, you want to raise an
    //    exception to the *caller*, that's where the bug happened.
    // Raise exception ... when exceptions have been implemented, send the
    // appropriate exception to the client
    ACE_ERROR ((LM_ERROR, ACE_TEXT("ERROR. Tried calling method twice  \n")));
  }

  // We are now ready to initialise the reply
  // @@ Mayur, that was a perfectly useless state, nobody can ever see
  //    it because the change from UNINITIALIZED -> INITIALIZING ->
  //    INITIALIZED is atomic...
  //
  // this->reply_status_ = TAO_RS_INITIALIZING;

  //ACE_DEBUG ((LM_DEBUG, ACE_TEXT("Initialising RH ... \n")));
  // Construct our reply generator.
  TAO_Pluggable_Reply_Params_Base reply_params;

  //ACE_DEBUG ((LM_DEBUG, ACE_TEXT("\tSetting request_id ... \n")));
  reply_params.request_id_ = this->request_id_;

  //ACE_DEBUG ((LM_DEBUG, ACE_TEXT("\tInitialising reply svc cntxt ... \n")));
  reply_params.service_context_notowned (&(this->reply_service_context_.service_info ()));

  //ACE_DEBUG ((LM_DEBUG, ACE_TEXT("\tSetting argument flag ... \n")));
  reply_params.argument_flag_ = this->argument_flag_;

  //ACE_DEBUG ((LM_DEBUG, ACE_TEXT("\tSetting reply_status ... \n")));
  if (this->exception_type_ == TAO_GIOP_NO_EXCEPTION)
    {
      reply_params.reply_status_ = TAO_PLUGGABLE_MESSAGE_NO_EXCEPTION;
    }
  else
    {
      reply_params.reply_status_ = this->exception_type_;
    }

  //ACE_DEBUG ((LM_DEBUG, ACE_TEXT("\tgenerating reply header ... \n")));
  this->mesg_base_->generate_reply_header (this->_tao_out,
                                           reply_params);
  //ACE_DEBUG ((LM_DEBUG, ACE_TEXT("Done Initialising RH. \n")));

  // We are done initialising the reply
  this->reply_status_ = TAO_RS_INITIALIZED;
}

void
TAO_AMH_Response_Handler::_tao_rh_send_reply (void)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);

  // If the reply has not been initialised, raise an exception
  if (this->reply_status_ != TAO_RS_INITIALIZED)
    {
      // Raise exception ... when exceptions have been implemented, send the
      // appropriate exception to the client
      ACE_ERROR ((LM_ERROR, ACE_TEXT("ERROR. Tried calling method twice  \n")));
    }

  this->reply_status_ = TAO_RS_SENDING;

  //ACE_DEBUG ((LM_DEBUG, ACE_TEXT("TAO_RH sending message ... \n")));

  // @@ Mayur: do you realize that the following call can take a *LONG*
  //    time, worse, you are implicitly calling the ORB event loop (to
  //    do async I/O for the outgoing path), that can result in calls
  //    up to the application, and there is no telling what the user
  //    will do there, for example, call this method again and
  //    deadlock the system.
  //    As a rule: always release your mutexes before calling out to
  //    functions that you do not control tightly.  Check what I did
  //    for the exceptions.
  //

  // Send the message.
  int result = this->transport_->send_message (this->_tao_out);

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

  this->reply_status_ = TAO_RS_SENT;
}

void
TAO_AMH_Response_Handler::_tao_rh_send_exception (CORBA::Exception &ex
                                                  ACE_ENV_ARG_DECL)
{
  // @@ Mayur: we need to allocate minor codes for the system
  //    exceptions raised here...
  {
    ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);
    if (this->reply_status_ != TAO_RS_UNINITIALIZED)
      ACE_THROW (CORBA::BAD_INV_ORDER ());
    this->reply_status_ = TAO_RS_SENDING;
  }

  TAO_Pluggable_Reply_Params_Base reply_params;
  reply_params.request_id_ = this->request_id_;
  reply_params.svc_ctx_.length (0);
  reply_params.service_context_notowned (&this->reply_service_context_.service_info ());
  reply_params.argument_flag_ = 1;
  reply_params.reply_status_ = TAO_GIOP_USER_EXCEPTION;
  // @@ It appears as if there should be a more efficient way to do
  //    this: the caller already knows this because it is part of the
  //    ExceptionHolder information.
  if (CORBA::SystemException::_downcast (&ex))
    reply_params.reply_status_ = TAO_GIOP_SYSTEM_EXCEPTION;

  if (this->mesg_base_->generate_exception_reply (_tao_out,
                                                  reply_params,
                                                  ex) == -1)
    {
      ACE_THROW (CORBA::INTERNAL ());
    }
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
