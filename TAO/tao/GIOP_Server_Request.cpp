// $Id$

// Implementation of the Dynamic Server Skeleton Interface  (for GIOP)

#include "tao/GIOP_Server_Request.h"

#include "tao/GIOP_Message_Acceptors.h"

#include "tao/CDR.h"
#include "tao/Environment.h"
#include "tao/NVList.h"
#include "tao/Principal.h"
#include "tao/ORB_Core.h"
#include "tao/ORB.h"
#include "tao/Timeprobe.h"
#include "tao/Any.h"
#include "tao/Marshal.h"
#include "tao/debug.h"
#include "tao/GIOP_Utils.h"

#if !defined (__ACE_INLINE__)
# include "tao/GIOP_Server_Request.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(tao, GIOP_Server_Request, "$Id$")

#if defined (ACE_ENABLE_TIMEPROBES)

static const char *TAO_Server_Request_Timeprobe_Description[] =
  {
    "GIOP_Server_Request::GIOP_Server_Request - start",
    "GIOP_Server_Request::GIOP_Server_Request - end",
  };

enum
  {
    // Timeprobe description table start key
    TAO_SERVER_REQUEST_START = 400,
    TAO_SERVER_REQUEST_END
  };

// Setup Timeprobes
ACE_TIMEPROBE_EVENT_DESCRIPTIONS (TAO_Server_Request_Timeprobe_Description,
                                  TAO_SERVER_REQUEST_START);

#endif /* ACE_ENABLE_TIMEPROBES */

TAO_GIOP_ServerRequest::
    TAO_GIOP_ServerRequest (TAO_Pluggable_Messaging *mesg_base,
                            TAO_InputCDR &input,
                            TAO_OutputCDR &output,
                            TAO_Transport *transport,
                            TAO_ORB_Core *orb_core)
      :mesg_base_ (mesg_base),
       incoming_ (&input),
       outgoing_ (&output),
       transport_(transport),
       response_expected_ (0),
       deferred_reply_ (0),
       sync_with_server_ (0),
       lazy_evaluation_ (0),

#if (TAO_HAS_MINIMUM_CORBA == 0)

       params_ (0),

#endif /* TAO_HAS_MINIMUM_CORBA */

       retval_ (0),
       exception_ (0),
       exception_type_ (TAO_GIOP_NO_EXCEPTION),
       orb_core_ (orb_core),
       service_info_ (),
       request_id_ (0),
       profile_ (orb_core),
       requesting_principal_ (0)
{
  ACE_FUNCTION_TIMEPROBE (TAO_SERVER_REQUEST_START);
  //no-op
}

// This constructor is used, by the locate request code

TAO_GIOP_ServerRequest::
    TAO_GIOP_ServerRequest (TAO_Pluggable_Messaging *mesg_base,
                            CORBA::ULong request_id,
                            CORBA::Boolean response_expected,
                            CORBA::Boolean deferred_reply,
                            TAO_ObjectKey &object_key,
                            const ACE_CString &operation,
                            TAO_OutputCDR &output,
                            TAO_Transport *transport,
                            TAO_ORB_Core *orb_core,
                            int &parse_error)
      : mesg_base_ (mesg_base),
        operation_ (operation),
        incoming_ (0),
        outgoing_ (&output),
        transport_ (transport),
        response_expected_ (response_expected),
        deferred_reply_ (deferred_reply),
        sync_with_server_ (0),
        lazy_evaluation_ (0),

#if (TAO_HAS_MINIMUM_CORBA == 0)

        params_ (0),

#endif /* TAO_HAS_MINIMUM_CORBA */

        retval_ (0),
        exception_ (0),
        exception_type_ (TAO_GIOP_NO_EXCEPTION),
        orb_core_ (orb_core),
        service_info_ (),
        request_id_ (request_id),
        profile_ (orb_core),
        requesting_principal_ (0)
{
  profile_.object_key (object_key);
  parse_error = 0;
}

TAO_GIOP_ServerRequest::~TAO_GIOP_ServerRequest (void)
{

#if (TAO_HAS_MINIMUM_CORBA == 0)

  if (this->params_)
    CORBA::release (this->params_);

#endif /* TAO_HAS_MINIMUM_CORBA */

  delete this->retval_;
  delete this->exception_;
}

CORBA::ORB_ptr
TAO_GIOP_ServerRequest::orb (void)
{
  return this->orb_core_->orb ();
}

#if (TAO_HAS_MINIMUM_CORBA == 0)

// Unmarshal in/inout params, and set up to marshal the appropriate
// inout/out/return values later on.

void
TAO_GIOP_ServerRequest::arguments (CORBA::NVList_ptr &list,
                                   CORBA::Environment &ACE_TRY_ENV)
{
  // Save params for later use when marshaling the reply.
  this->params_ = list;

  this->params_->_tao_incoming_cdr (*this->incoming_,
                                    CORBA::ARG_IN | CORBA::ARG_INOUT,
                                    this->lazy_evaluation_,
                                    ACE_TRY_ENV);
}

// Store the result value.  There's either an exception, or a result,
// but not both of them.  Results (and exceptions) can be reported
// only after the parameter list has been provided (maybe empty).

void
TAO_GIOP_ServerRequest::set_result (const CORBA::Any &value,
                                    CORBA::Environment &ACE_TRY_ENV)
{
  // setting a result when another result already exists or if an exception
  // exists is an error
  if (this->retval_ || this->exception_)
    ACE_THROW (CORBA::BAD_INV_ORDER ());

  ACE_NEW_THROW_EX (this->retval_,
                    CORBA::Any (value),
                    CORBA::NO_MEMORY ());
  ACE_CHECK;
}

// Store the exception value.

void
TAO_GIOP_ServerRequest::set_exception (const CORBA::Any &value,
                                       CORBA::Environment &ACE_TRY_ENV)
{
  if (this->retval_ || this->exception_)
    ACE_THROW (CORBA::BAD_INV_ORDER ());

  ACE_NEW_THROW_EX (this->exception_,
                    CORBA::Any (value),
                    CORBA::NO_MEMORY ());
  ACE_CHECK;

  this->exception_type_ = TAO_GIOP_USER_EXCEPTION;

  if (value.value ())
    {
      // @@ This cast is not safe, but we haven't implemented the >>=
      // and <<= operators for base exceptions (yet).
      CORBA_Exception* x = (CORBA_Exception*)value.value ();

      if (CORBA_SystemException::_downcast (x) != 0)
        {
          this->exception_type_ = TAO_GIOP_SYSTEM_EXCEPTION;
        }
    }
}

// this method will be utilized by the DSI servant to marshal outgoing
// parameters

void
TAO_GIOP_ServerRequest::dsi_marshal (CORBA::Environment &ACE_TRY_ENV)
{
  // NOTE: if "env" is set, it takes precedence over exceptions
  // reported using the mechanism of the ServerRequest.  Only system
  // exceptions are reported that way ...
  //
  // XXX Exception reporting is ambiguous; it can be cleaner than
  // this.  With both language-mapped and dynamic/explicit reporting
  // mechanisms, one of must be tested "first" ... so an exception
  // reported using the other mechanism could be "lost".  Perhaps only
  // the language mapped one should be used for system exceptions.


  // only if there wasn't any exception, we proceed
  if (this->exception_type_ == TAO_GIOP_NO_EXCEPTION &&
      CORBA::is_nil (this->forward_location_.in ()))
    {
      // ... then send any return value ...
      if (this->retval_)
        {
          CORBA::TypeCode_var tc = this->retval_->type ();
          if (this->retval_->any_owns_data ())
            {
              this->retval_->_tao_encode (*this->outgoing_,
                                          this->orb_core_,
                                          ACE_TRY_ENV);
              ACE_CHECK;
            }
          else
            {
              TAO_InputCDR cdr (this->retval_->_tao_get_cdr (),
                                this->retval_->_tao_byte_order ());
              (void) TAO_Marshal_Object::perform_append (tc.in (),
                                                         &cdr,
                                                         this->outgoing_,
                                                         ACE_TRY_ENV);
              ACE_CHECK;
            }
        }

      // ... Followed by "inout" and "out" parameters, left to right
      if (this->params_)
        {
          this->params_->_tao_encode (*this->outgoing_,
                                      this->orb_core_,
                                      CORBA::ARG_INOUT | CORBA::ARG_OUT,
                                      ACE_TRY_ENV);
          ACE_CHECK;
        }
    }
}

#endif /* TAO_HAS_MINIMUM_CORBA */

void
TAO_GIOP_ServerRequest::init_reply (CORBA::Environment &ACE_TRY_ENV)
{
  // Construct our reply generator
  TAO_Pluggable_Reply_Params reply_params;

  // We put all the info that we have in to this <reply_params> and
  // call the <write_reply_header> in the
  // pluggable_messaging_interface. One point to be noted however is
  // that, it was the pluggable_messaging classes who created us and
  // delegated us to do work on its behalf. But we would be calling
  // back. As we dont have a LOCK or any such things we can call
  // pluggable_messaging guys again. We would be on the same thread of
  // invocation. So *theoratically* there should not be a problem.
  reply_params.request_id_ = this->request_id_;

#if (TAO_HAS_MINIMUM_CORBA == 0)
  if (this->lazy_evaluation_ == 0 || this->params_ == 0)
    reply_params.params_ = 0;
  else
    reply_params.params_ = this->params_;
#endif /*TAO_HAS_MINIMUM_CORBA */

  // Pass in the service context list.  We are sending back what we
  // received in the Request.  (RTCORBA relies on it.  Check before
  // modifying...) marina
  reply_params.service_context_notowned (&this->service_info_);

  // Forward exception only.
  if (!CORBA::is_nil (this->forward_location_.in ()))
    {
      reply_params.reply_status_ = TAO_PLUGGABLE_MESSAGE_LOCATION_FORWARD;
    }

  // Any exception at all.
  else if (this->exception_ == 0)
    {
      reply_params.reply_status_ = TAO_PLUGGABLE_MESSAGE_NO_EXCEPTION;
    }
  else
    {
      reply_params.reply_status_ = this->exception_type_;
    }

  // Construct a REPLY header.
  this->mesg_base_->write_reply_header (*this->outgoing_,
                                        reply_params);

  // Finish the GIOP Reply header, then marshal the exception.
  if (reply_params.reply_status_ ==
      TAO_PLUGGABLE_MESSAGE_LOCATION_FORWARD)
    {
      // Marshall the forward location pointrr
      CORBA::Object_ptr object_ptr = this->forward_location_.in ();
      if ((*this->outgoing_ << object_ptr) == 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO_GIOP_ServerRequest::marshal - ")
                      ACE_TEXT ("encoding forwarded objref failed\n")));
          return;
        }
    }
  else if (reply_params.reply_status_ !=
           TAO_PLUGGABLE_MESSAGE_NO_EXCEPTION)
    {
      // We are checking for NO exception. If there was a NO_EXCEPTION
      // type the IDL compiler would marshall the return values, the
      // out & inout parameters in to the stream <*this->output>

      // In this special case we only marshall the exception type
      CORBA::TypeCode_ptr except_tc = this->exception_->type ();

      // we use the any's ACE_Message_Block
      TAO_InputCDR cdr (this->exception_->_tao_get_cdr (),
                        this->exception_->_tao_byte_order ());
      (void) TAO_Marshal_Object::perform_append (except_tc,
                                                 &cdr,
                                                 this->outgoing_,
                                                 ACE_TRY_ENV);
    }
}

CORBA::Object_ptr
TAO_GIOP_ServerRequest::forward_location (void)
// get the forward_location
{
  return CORBA::Object::_duplicate (this->forward_location_.in ());
}

CORBA::ULong
TAO_GIOP_ServerRequest::exception_type (void)
// get the exception type
{
  return this->exception_type_;
}

void
TAO_GIOP_ServerRequest::send_no_exception_reply (void)
{
  // Construct our reply generator
  TAO_Pluggable_Reply_Params reply_params;
  reply_params.request_id_ = this->request_id_;

#if (TAO_HAS_MINIMUM_CORBA == 0)
  reply_params.params_ = 0;
#endif /*TAO_HAS_MINIMUM_CORBA*/

  // Change this to pass back the same thing we received, as well as
  // leave a comment why this is important!
  reply_params.svc_ctx_.length (0);

  // Pass in the service context list.  We are sending back what we
  // received in the Request.  (RTCORBA relies on it.  Check before
  // modifying...) marina
  reply_params.service_context_notowned (&this->service_info_);

  reply_params.reply_status_ = TAO_GIOP_NO_EXCEPTION;

  // Construct a REPLY header.
  this->mesg_base_->write_reply_header (*this->outgoing_,
                                        reply_params);

  // Send the message
  int result = this->mesg_base_->send_message (transport_,
                                               *this->outgoing_);

  if (result == -1)
    {
      if (TAO_debug_level > 0)
        {
          // No exception but some kind of error, yet a response
          // is required.
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("TAO: (%P|%t) %p: cannot send NO_EXCEPTION reply\n"),
                      ACE_TEXT ("TAO_GIOP_ServerRequest::send_no_exception_reply")));
        }
    }
}


void
TAO_GIOP_ServerRequest::tao_send_reply(void)
{


  int result = mesg_base_->send_message(transport_,
                                        *outgoing_);
  if (result == -1)
    {
      if (TAO_debug_level > 0)
        {
          // No exception but some kind of error, yet a response
          // is required.
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("TAO: (%P|%t) %p: cannot send reply\n"),
                      ACE_TEXT ("TAO_GIOP_ServerRequest::tao_send_reply")));

      }
    }
}

void
TAO_GIOP_ServerRequest::tao_send_reply_exception(CORBA::Exception& ex)
{
  int result = 0;
  if (response_expected_)
  {
    result = ACE_static_cast(TAO_GIOP_Message_Acceptors*,mesg_base_)->send_reply_exception (transport_,
                                               orb_core_,
                                               request_id_,
                                               &service_info(),
                                               &ex);
    if (result == -1)
    {
      if (TAO_debug_level > 0)
         ACE_ERROR ((LM_ERROR,
                     ACE_TEXT ("TAO: (%P|%t|%N|%l) %p: cannot send exception reply\n"),
                     ACE_TEXT ("tao_send_reply_exception()")));
         ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                              "TAO: ");
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
    ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "TAO: ");
   }

}

#if 0 // PPOA
CORBA::Object_ptr
TAO_GIOP_ServerRequest::objref (CORBA_Environment &ACE_TRY_ENV)
{
  TAO_POA_Current_Impl *pci = TAO_TSS_RESOURCES::instance ()->poa_current_impl_;

  return pci->poa ()->id_to_reference (pci->object_id (), ACE_TRY_ENV);
}
#endif /* 0 PPOA */
