// $Id$

#include "TAO_AMH_Response_Handler.h"
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
  , response_expected_ (server_request.response_expected_)
  , transport_ (TAO_Transport::_duplicate (server_request.transport ()))
  , orb_core_ (server_request.orb_core ())
  , argument_flag_ (1)
  , exception_type_ (TAO_GIOP_NO_EXCEPTION)
  , once_only_ (0)
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
TAO_AMH_Response_Handler::init_reply (void)
{
  // @@ Mayur: as you pointed out this stuff is not thread safe, also,
  // I would use the "state pattern"..
  // {
  // ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);
  // if (once_only_ != NO_REQUEST_SENT)
  //   // Raise exception
  // once_only_ = SENDING_REQUEST;
  // }
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
      this->mesg_base_->generate_reply_header (this->_tao_out,
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
  // @@ Mayur: as you pointed out this stuff is not thread safe, also,
  // I would use the "state pattern"..
  // {
  // ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);
  // if (once_only_ != SENDING_REQUEST)
  //   // Raise exception
  // once_only_ = REQUEST_SENT;
  // }

  if (once_only_ == 2) // should not be necessary, but just precautionary
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT("TAO_RH sending message ... \n")));

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
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT("TAO_RH: message sent. \n")));
    }
}
