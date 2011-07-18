// $Id$

#include "tao/Messaging/AMH_Response_Handler.h"
#include "tao/TAO_Server_Request.h"
#include "tao/Transport.h"
#include "tao/CDR.h"
#include "tao/ORB_Core.h"
#include "tao/ORB.h"
#include "tao/GIOP_Message_Base.h"
#include "tao/Pluggable_Messaging_Utils.h"
#include "tao/GIOP_Utils.h"
#include "tao/debug.h"
#include "tao/Buffer_Allocator_T.h"
#include "tao/SystemException.h"

#include "ace/Copy_Disabled.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_AMH_Response_Handler::TAO_AMH_Response_Handler ()
  : reply_status_ (GIOP::NO_EXCEPTION)
  , mesg_base_ (0)
  , request_id_ (0)
  , transport_ (0)
  , orb_core_ (0)
  , argument_flag_ (1)
  , rh_reply_status_ (TAO_RS_UNINITIALIZED)
  , allocator_ (0)
{
}

TAO_AMH_Response_Handler::~TAO_AMH_Response_Handler (void)
{
  this->transport_->remove_reference ();

  // Since we are destroying the object we put a huge lock around the
  // whole destruction process (just paranoid).
  {
    ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);

    if (this->response_expected_ == 0) //oneway ?
      {
        return;
      }

    // If the ResponseHandler is being destroyed before a reply has
    // been sent to the client, we send a system exception
    // CORBA::NO_RESPONSE, with minor code to indicate the problem.
    if (this->rh_reply_status_ == TAO_RS_SENT)
      {
        return;
      }
  }

  // If sending the exception to the client fails, then we just give
  // up, release the transport and return.
  try
    {
      CORBA::NO_RESPONSE ex (CORBA::SystemException::_tao_minor_code
                             (TAO_AMH_REPLY_LOCATION_CODE,
                              EFAULT),
                             CORBA::COMPLETED_NO);
      this->_tao_rh_send_exception (ex);
    }
  catch (...)
    {
    }
}

void
TAO_AMH_Response_Handler::init(TAO_ServerRequest &server_request,
                               TAO_AMH_BUFFER_ALLOCATOR* allocator)
{
  mesg_base_ = server_request.mesg_base_;
  request_id_ = server_request.request_id_;
  response_expected_  = server_request.response_expected_;
  transport_ = server_request.transport ();
  orb_core_ = server_request.orb_core ();
  allocator_ = allocator;

  TAO_GIOP_Message_Version v;
  server_request.outgoing()->get_version(v);
  this->_tao_out.set_version(v.major, v.minor);
  this->transport_->assign_translators (0, &this->_tao_out);
  this->transport_->add_reference ();
}

void
TAO_AMH_Response_Handler::_tao_rh_init_reply (void)
{
  {
    ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);
    if (this->rh_reply_status_ != TAO_RS_UNINITIALIZED)
      {
        // Looks like someone is trying to call an AMH method
        // more than once
        //
        // We assume that the servant has already processed the
        // request and is now trying to send back the reply.  Hence we
        // say that the operation has completed but let the server
        // anyway that it is not doing something right.
        throw ::CORBA::BAD_INV_ORDER
                          (CORBA::SystemException::_tao_minor_code
                                  (TAO_AMH_REPLY_LOCATION_CODE,
                                   EEXIST),
                           CORBA::COMPLETED_YES);
      }
  }

  // Construct our reply generator.
  TAO_Pluggable_Reply_Params_Base reply_params;
  reply_params.request_id_ = this->request_id_;
  reply_params.service_context_notowned (&(this->reply_service_context_.service_info ()));
  reply_params.argument_flag_ = this->argument_flag_;
  reply_params.reply_status (this->reply_status_);

  {
    ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);

    this->mesg_base_->generate_reply_header (this->_tao_out, reply_params);

    // We are done initialising the reply
    this->rh_reply_status_ = TAO_RS_INITIALIZED;
  }

}

void
TAO_AMH_Response_Handler::_tao_rh_send_reply (void)
{
  {
    ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);

    // If the reply has not been initialised, raise an exception to the
    // server-app saying it is not doing something right.
    if (this->rh_reply_status_ != TAO_RS_INITIALIZED)
      {
        throw ::CORBA::BAD_INV_ORDER (
                          CORBA::SystemException::_tao_minor_code (
                                                  TAO_AMH_REPLY_LOCATION_CODE,
                                                  ENOTSUP),
                          CORBA::COMPLETED_YES);
      }
    this->rh_reply_status_ = TAO_RS_SENDING;
  }

  // Send the message.
  int result = this->transport_->send_message (this->_tao_out,
                                               0,
                                               TAO_REPLY);

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
    this->rh_reply_status_ = TAO_RS_SENT;
  }
}

void
TAO_AMH_Response_Handler::_tao_rh_send_exception (const CORBA::Exception &ex)
{
  {
    ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);
    if (this->rh_reply_status_ != TAO_RS_UNINITIALIZED)
      {
        throw ::CORBA::BAD_INV_ORDER (
          CORBA::SystemException::_tao_minor_code (
            TAO_AMH_REPLY_LOCATION_CODE,
            ENOTSUP),
          CORBA::COMPLETED_YES);
      }
    this->rh_reply_status_ = TAO_RS_SENDING;
  }

  TAO_Pluggable_Reply_Params_Base reply_params;
  reply_params.request_id_ = this->request_id_;
  reply_params.svc_ctx_.length (0);
  reply_params.service_context_notowned (&this->reply_service_context_.service_info ());
  reply_params.argument_flag_ = true;
  // @@ It appears as if there should be a more efficient way to do
  //    this: the caller already knows this because it is part of the
  //    ExceptionHolder information.
  if (CORBA::SystemException::_downcast (&ex))
    {
      reply_params.reply_status (GIOP::SYSTEM_EXCEPTION);
    }
  else
    {
      reply_params.reply_status (GIOP::USER_EXCEPTION);
    }

  if (this->mesg_base_->generate_exception_reply (this->_tao_out,
                                                  reply_params,
                                                  ex) == -1)
    {
      throw ::CORBA::INTERNAL ();
    }

  // Send the Exception
  if (this->transport_->send_message (this->_tao_out,
                                      0,
                                      TAO_REPLY) == -1)
    {
      if (TAO_debug_level > 0)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("TAO: (%P|%t|%N|%l):  ")
                    ACE_TEXT ("TAO_AMH_Response_Handler:")
                    ACE_TEXT (" could not send exception reply\n")));
    }

  {
    ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);
    this->rh_reply_status_ = TAO_RS_SENT;
  }
}

void
TAO_AMH_Response_Handler::_tao_rh_send_location_forward (CORBA::Object_ptr fwd,
                                                         CORBA::Boolean is_perm)
{
  {
    ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);
    if (this->rh_reply_status_ != TAO_RS_UNINITIALIZED)
      {
        throw ::CORBA::BAD_INV_ORDER (
          CORBA::SystemException::_tao_minor_code (
            TAO_AMH_REPLY_LOCATION_CODE,
            ENOTSUP),
          CORBA::COMPLETED_YES);
      }
    this->rh_reply_status_ = TAO_RS_SENDING;
  }


  TAO_Pluggable_Reply_Params_Base reply_params;
  reply_params.request_id_ = this->request_id_;
  reply_params.svc_ctx_.length (0);
  reply_params.service_context_notowned
    (&this->reply_service_context_.service_info ());
  reply_params.argument_flag_ = true;
  if (is_perm)
    {
      reply_params.reply_status (GIOP::LOCATION_FORWARD_PERM);
    }
  else
    {
      reply_params.reply_status (GIOP::LOCATION_FORWARD);
    }

  if (this->mesg_base_->generate_reply_header (this->_tao_out,
                                               reply_params) == -1)
    {
      throw ::CORBA::INTERNAL ();
    }

  if (!(this->_tao_out << fwd))
    {
      if (TAO_debug_level > 0)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("TAO (%P|%t) ERROR: Unable to marshal ")
                    ACE_TEXT ("forward reference.\n")));
      return;
    }

  // Send the Exception
  if (this->transport_->send_message (this->_tao_out,
                                      0,
                                      TAO_REPLY) == -1)
    {
      if (TAO_debug_level > 0)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("TAO: (%P|%t|%N|%l):  ")
                    ACE_TEXT ("TAO_AMH_Response_Handler: could not send ")
                    ACE_TEXT ("location forward reply\n")));
    }

  {
    ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);
    this->rh_reply_status_ = TAO_RS_SENT;
  }
}

void
TAO_AMH_Response_Handler::_remove_ref (void)
{
  if (this->refcount_.decrement () > 0)
    return;

  if (this->allocator_)
    {
      TAO::TAO_Buffer_Allocator<TAO_AMH_Response_Handler, TAO_AMH_BUFFER_ALLOCATOR> allocator (this->allocator_);

      allocator.release (this);
    }
  else
    {
      delete this;
    }
}

namespace TAO
{
  void
  ARH_Refcount_Functor::operator () (TAO_AMH_Response_Handler *arh) throw ()
  {
    (void) arh->_remove_ref ();
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
