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
  , response_expected_ (server_request.response_expected_)
  , transport_ (server_request.transport ())
  , orb_core_ (server_request.orb_core ())
  , argument_flag_ (1)
  , exception_type_ (TAO_GIOP_NO_EXCEPTION)
  , reply_status_ (TAO_RS_UNINITIALIZED)
{
  this->transport_->add_reference ();
}

TAO_AMH_Response_Handler::~TAO_AMH_Response_Handler (void)
{
  // Since we are destroying the object we put a huge lock around the
  // whole destruction process (just paranoid).
  {
    ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);

    if (this->response_expected_ == 0) //oneway ?
      {
        // if client is not expecting anything, don't send anything
        this->transport_->remove_reference ();
        return;
      }

    // If the ResponseHandler is being destroyed before a reply has
    // been sent to the client, we send a system exception
    // CORBA::NO_RESPONSE, with minor code to indicate the problem.
    if (this->reply_status_ != TAO_RS_SENT)
      {
        // Is sending the exception to the client fails, then we just
        // give up, release the transport and return.
        ACE_DECLARE_NEW_CORBA_ENV;
        ACE_TRY
          {
            CORBA::NO_RESPONSE ex (CORBA::SystemException::_tao_minor_code
                                                 (TAO_AMH_REPLY_LOCATION_CODE,
                                                  EFAULT),
                                            CORBA::COMPLETED_NO);
            this->_tao_rh_send_exception (ex ACE_ENV_ARG_PARAMETER);
            ACE_TRY_CHECK;

            this->transport_->remove_reference ();
          }
        ACE_CATCHALL
          {
            this->transport_->remove_reference ();
          }
        ACE_ENDTRY;
      }
    else
      {
        this->transport_->remove_reference ();
      }
  }
}

void
TAO_AMH_Response_Handler::_tao_rh_init_reply (ACE_ENV_SINGLE_ARG_DECL)
{
  {
    ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);
    if (this->reply_status_ != TAO_RS_UNINITIALIZED)
      {
        // Looks like someone is trying to call an AMH method
        // more than once
        //
        // We assume that the servant has already processed the
        // request and is now trying to send back the reply.  Hence we
        // say that the operation has completed but let the server
        // anyway that it is not doing something right.
        ACE_THROW (CORBA::BAD_INV_ORDER
                          (CORBA::SystemException::_tao_minor_code
                                  (TAO_AMH_REPLY_LOCATION_CODE,
                                   EEXIST),
                           CORBA::COMPLETED_YES));
      }
  }

  // Construct our reply generator.
  TAO_Pluggable_Reply_Params_Base reply_params;
  reply_params.request_id_ = this->request_id_;
  reply_params.service_context_notowned (&(this->reply_service_context_.service_info ()));
  reply_params.argument_flag_ = this->argument_flag_;

  if (this->exception_type_ == TAO_GIOP_NO_EXCEPTION)
    {
      reply_params.reply_status_ = TAO_PLUGGABLE_MESSAGE_NO_EXCEPTION;
    }
  else
    {
      reply_params.reply_status_ = this->exception_type_;
    }

  {
    ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);

    this->mesg_base_->generate_reply_header (this->_tao_out,
                                             reply_params);

    // We are done initialising the reply
    this->reply_status_ = TAO_RS_INITIALIZED;
  }

}

void
TAO_AMH_Response_Handler::_tao_rh_send_reply (ACE_ENV_SINGLE_ARG_DECL)
{

  {
    ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);

    // If the reply has not been initialised, raise an exception to the
    // server-app saying it is not doing something right.
    if (this->reply_status_ != TAO_RS_INITIALIZED)
      {
        ACE_THROW (CORBA::BAD_INV_ORDER (
                          CORBA::SystemException::_tao_minor_code (
                                                  TAO_AMH_REPLY_LOCATION_CODE,
                                                  ENOTSUP),
                          CORBA::COMPLETED_YES));
      }
    this->reply_status_ = TAO_RS_SENDING;
  }

  // Send the message.
  int result = this->transport_->send_message (this->_tao_out,
                                               0,
                                               TAO_Transport::TAO_REPLY);

  if (result == -1)
    {
      if (TAO_debug_level > 0)
        {
          // No exception but some kind of error, yet a response
          // is required.
          ACE_ERROR ((
                      LM_ERROR,
                      ACE_TEXT ("TAO: (%P|%t) %p: cannot send NO_EXCEPTION reply\n"),
                      ACE_TEXT ("TAO_AMH_Response_Handler::_tao_rh_send_reply")
                      ));
        }
    }

  {
    ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);
    this->reply_status_ = TAO_RS_SENT;
  }
}

void
TAO_AMH_Response_Handler::_tao_rh_send_exception (CORBA::Exception &ex
                                                  ACE_ENV_ARG_DECL)
{
  {
    ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);
    if (this->reply_status_ != TAO_RS_UNINITIALIZED)
      {
        ACE_THROW (CORBA::BAD_INV_ORDER (
                                         CORBA::SystemException::_tao_minor_code (
                                                                                  TAO_AMH_REPLY_LOCATION_CODE,
                                                                                  ENOTSUP),
                                         CORBA::COMPLETED_YES));
      }
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

  if (this->mesg_base_->generate_exception_reply (this->_tao_out,
                                                  reply_params,
                                                  ex) == -1)
    {
      ACE_THROW (CORBA::INTERNAL ());
    }

  // Send the Exception
  if (this->transport_->send_message (this->_tao_out,
                                      0,
                                      TAO_Transport::TAO_REPLY) == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("TAO: (%P|%t|%N|%l):  ")
                  ACE_TEXT ("TAO_AMH_Response_Handler: could not send exception reply\n")));
    }

  {
    ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);
    this->reply_status_ = TAO_RS_SENT;
  }
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
