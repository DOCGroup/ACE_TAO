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
  , once_only_ (TAO_UNINITIALISED_REPLY)
  , mutex_ ()
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
// TAO_ENV_SINGLE_ARG_DECL... and then change the generated code in
// the IDL compiler too!
void
TAO_AMH_Response_Handler::_tao_rh_init_reply (void)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);

  if (this->once_only_ != TAO_UNINITIALISED_REPLY)
  {
    // Raise exception ... when exceptions have been implemented, send the 
    // appropriate exception to the client
    ACE_ERROR ((LM_ERROR, ACE_TEXT("ERROR. Tried calling method twice  \n")));
  }

  // We are now ready to initialise the reply
  this->once_only_ = TAO_INITIALISING_REPLY;
  
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
  this->once_only_ = TAO_INITIALISED_REPLY; 
}

void TAO_AMH_Response_Handler::_tao_rh_send_reply (void)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);
  
  // If the reply has not been initialised, raise an exception
  if (this->once_only_ != TAO_INITIALISED_REPLY)
    {
      // Raise exception ... when exceptions have been implemented, send the 
      // appropriate exception to the client
      ACE_ERROR ((LM_ERROR, ACE_TEXT("ERROR. Tried calling method twice  \n")));
    }
  
  this->once_only_ = TAO_SENDING_REPLY;
  
  //ACE_DEBUG ((LM_DEBUG, ACE_TEXT("TAO_RH sending message ... \n")));
  
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

  this->once_only_ = TAO_SENT_REPLY;
}



#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Condition <ACE_SYNCH_MUTEX>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
