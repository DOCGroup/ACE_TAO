// $Id$

#ifndef TAO_AMH_RESPONSE_HANDLER_CPP
#define TAO_AMH_RESPONSE_HANDLER_CPP

#include "TAO_AMH_Response_Handler.h"
#include "tao/Transport.h"
#include "tao/CDR.h"
#include "tao/ORB_Core.h"
#include "tao/ORB.h"
#include "tao/Pluggable_Messaging.h"
#include "tao/Pluggable_Messaging_Utils.h"
#include "tao/GIOP_Utils.h"



TAO_AMH_Response_Handler::TAO_AMH_Response_Handler ()
:    argument_flag_(1),
exception_type_ (TAO_GIOP_NO_EXCEPTION),
once_only_(0)
{
tao_out_ = new TAO_OutputCDR();
}

TAO_AMH_Response_Handler::~TAO_AMH_Response_Handler (void)
{
// release the transport
TAO_Transport::release (transport_);
delete tao_out_;
}


void
TAO_AMH_Response_Handler::mesg_base (TAO_Pluggable_Messaging *mesg_base)
{
this->mesg_base_ = mesg_base;
}

void
TAO_AMH_Response_Handler::request_id (CORBA::ULong request_id)
{
this->request_id_ = request_id;
}

void
TAO_AMH_Response_Handler::response_expected (CORBA::Boolean response_expected)
{
this->response_expected_ = response_expected;
}

void
TAO_AMH_Response_Handler::transport (TAO_Transport *transport)
{
this->transport_ = transport;
}

/*void
TAO_AMH_Response_Handler::reply_service_info (IOP::ServiceContext &service_context)
{
this->reply_service_context_.set_context (service_context);
}*/



void
TAO_AMH_Response_Handler::init_reply (void)
{
once_only_ ++; // equals 1 if called for first time
if (once_only_ == 1)
{
once_only_++;

ACE_DEBUG ((LM_DEBUG, ACE_TEXT("Initialising RH ... \n")));
// Construct our reply generator.
TAO_Pluggable_Reply_Params_Base reply_params;

ACE_DEBUG ((LM_DEBUG, ACE_TEXT("\tSetting request_id ... \n")));
reply_params.request_id_ = this->request_id_;

ACE_DEBUG ((LM_DEBUG, ACE_TEXT("\tInitialising reply svc cntxt ... \n")));
reply_params.service_context_notowned (&(this->reply_service_context_.service_info ()));

ACE_DEBUG ((LM_DEBUG, ACE_TEXT("\tSetting argument flag ... \n")));
reply_params.argument_flag_ = this->argument_flag_;

ACE_DEBUG ((LM_DEBUG, ACE_TEXT("\tSetting reply_status ... \n")));
if (this->exception_type_ == TAO_GIOP_NO_EXCEPTION)
{
reply_params.reply_status_ = TAO_PLUGGABLE_MESSAGE_NO_EXCEPTION;
}
else
{
reply_params.reply_status_ = this->exception_type_;
}

ACE_DEBUG ((LM_DEBUG, ACE_TEXT("\tgenerating reply header ... \n")));
this->mesg_base_->generate_reply_header (*(this->tao_out_),
reply_params);
ACE_DEBUG ((LM_DEBUG, ACE_TEXT("Done Initialising RH. \n")));
}
else
{
// we should raise an exception here
ACE_DEBUG ((LM_DEBUG, ACE_TEXT("ERROR. Tried calling method twice  \n")));
}
}

void TAO_AMH_Response_Handler::send_reply (void)
{

if (once_only_ == 2) // should not be necessary, but just precautionary
{
ACE_DEBUG ((LM_DEBUG, ACE_TEXT("TAO_RH sending message ... \n")));

// Send the message.
int result = this->transport_->send_message (*this->tao_out_);

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
ACE_DEBUG ((LM_DEBUG, ACE_TEXT("TAO_RH: message sent. \n")));
}
}

#endif /* TAO_AMH_RESPONSE_HANDLER_CPP */
