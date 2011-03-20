// $Id$

// Implementation of the Dynamic Server Skeleton Interface.

#include "tao/DynamicInterface/Server_Request.h"
#include "tao/DynamicInterface/DII_Arguments.h"
#include "tao/DynamicInterface/Unknown_User_Exception.h"
#include "tao/PortableServer/Collocated_Arguments_Converter.h"
#include "tao/AnyTypeCode/NVList.h"
#include "tao/GIOP_Utils.h"
#include "tao/AnyTypeCode/Marshal.h"
#include "tao/AnyTypeCode/TypeCode.h"
#include "tao/AnyTypeCode/Any_Impl.h"
#include "tao/operation_details.h"
#include "tao/SystemException.h"

#if !defined (__ACE_INLINE__)
# include "tao/DynamicInterface/Server_Request.inl"
#endif /* ! __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Reference counting for DSI ServerRequest object.

CORBA::ULong
CORBA::ServerRequest::_incr_refcount (void)
{
  return ++this->refcount_;
}

CORBA::ULong
CORBA::ServerRequest::_decr_refcount (void)
{
  CORBA::ULong const new_count = --this->refcount_;

  if (new_count == 0)
    delete this;

  return new_count;
}

CORBA::ServerRequest::ServerRequest (TAO_ServerRequest &orb_server_request)
  : lazy_evaluation_ (0),
    ctx_ (CORBA::Context::_nil ()),
    params_ (CORBA::NVList::_nil ()),
    retval_ (0),
    exception_ (0),
    refcount_ (1),
    orb_server_request_ (orb_server_request),
    sent_gateway_exception_ (false)
{
  this->orb_server_request_.is_dsi ();
}

CORBA::ServerRequest::~ServerRequest (void)
{
  if (this->params_ != 0)
    {
      ::CORBA::release (this->params_);
    }

  delete this->retval_;
  delete this->exception_;
}

// Unmarshal in/inout params, and set up to marshal the appropriate
// inout/out/return values later on.
void
CORBA::ServerRequest::arguments (CORBA::NVList_ptr &list)
{
  // arguments() must be called before either of these.
  if (this->params_ != 0 || this->exception_ != 0)
    {
      throw ::CORBA::BAD_INV_ORDER (CORBA::OMGVMCID | 7, CORBA::COMPLETED_NO);
    }

  // In a collocated situation there will not be an incoming CDR stream
  // in which case we can get the arguments from the
  // operation_details using the 'collocated argument converter'.
  if (this->orb_server_request_.collocated ())
  {
    this->params_ = list;

    if (this->orb_server_request_.operation_details ()->cac () != 0)
      {
        TAO_OutputCDR output;
        this->orb_server_request_.operation_details ()->cac (
                )->dsi_convert_request (this->orb_server_request_,
                                        output);

        TAO_InputCDR input(output);
        this->params_->_tao_decode (input, CORBA::ARG_IN | CORBA::ARG_INOUT);
      }
  }
  else
  {
    // Save params for later use when marshaling the reply.
    this->params_ = list;

    // in the case of a GIOP::LocateRequest there is no incoming CDR stream
    // so skip any attempt to decode arguments (now or later)
    if (this->orb_server_request_.incoming () != 0)
    {
      this->params_->_tao_incoming_cdr (*this->orb_server_request_.incoming (),
                                        CORBA::ARG_IN | CORBA::ARG_INOUT,
                                        this->lazy_evaluation_);

      // Pass this alignment back to the TAO_ServerRequest.
      this->orb_server_request_.dsi_nvlist_align (
                                    this->params_->_tao_target_alignment ());
    }
  }
}

// Store the result value.  There's either an exception, or a result,
// but not both of them.  Results can be reported (at most once)
// only after the parameter list has been provided (maybe empty).
void
CORBA::ServerRequest::set_result (const CORBA::Any &value
                                  )
{
  // Setting a result when another result already exists or if an exception
  // exists or before the args have been processeed is an error.
  if (this->retval_ != 0 || this->exception_ != 0 || this->params_ == 0)
    {
      throw ::CORBA::BAD_INV_ORDER (CORBA::OMGVMCID | 8, CORBA::COMPLETED_NO);
    }

  ACE_NEW_THROW_EX (this->retval_,
                    CORBA::Any (value),
                    CORBA::NO_MEMORY ());
}

  // NOTE: if "" is set, there has been a system exception,
  // and it will take precedence over exceptions reported using the
  // set_exception() mechanism of the ServerRequest, which we assume
  // the application writer will use to report only user exceptions.
  // If both types of exception happen on the same invocation, the user
  // exception will be lost.

// Store the exception value.
void
CORBA::ServerRequest::set_exception (const CORBA::Any &value)
{
  CORBA::TypeCode_var tc = value.type ();

  CORBA::TCKind const kind = tc->kind ();

  // set_exception() can be called at any time, but the Any arg MUST
  // contain an exception.
  if (kind != CORBA::tk_except)
    {
      throw ::CORBA::BAD_PARAM (CORBA::OMGVMCID | 21, CORBA::COMPLETED_MAYBE);
    }

  ACE_NEW_THROW_EX (this->exception_,
                    CORBA::Any (value),
                    CORBA::NO_MEMORY ());

  this->orb_server_request_.reply_status (GIOP::USER_EXCEPTION);
}

// This method will be utilized by the DSI servant to marshal outgoing
// parameters.
void
CORBA::ServerRequest::dsi_marshal (void)
{
  // There was a user exception, no need to marshal any parameters.
  if (this->sent_gateway_exception_)
    {
      return;
    }

  // In a collocated situation there is no outgoing CDR stream.
  // So, in case of an exception we just raise the UnknownUserException
  // and for a regular reply we use the 'collocated argument converter'.
  if (this->orb_server_request_.collocated ())
  {
    if (this->orb_server_request_.reply_status () == GIOP::USER_EXCEPTION)
      {
        throw CORBA::UnknownUserException (*this->exception_);
      }
    else if (this->orb_server_request_.operation_details ()->cac () != 0)
      {
        TAO_OutputCDR output;
        // marshal the return value if any
        if (this->retval_ != 0)
          {
            this->retval_->impl ()->marshal_value (output);
          }

        // marshal the "inout" and "out" parameters.
        if (this->params_ != 0)
          {
            this->params_->_tao_encode (
                              output,
                              CORBA::ARG_INOUT | CORBA::ARG_OUT);
          }

        TAO_InputCDR input (output);
        // set reply parameters
        this->orb_server_request_.operation_details ()->cac (
                )->dsi_convert_reply (this->orb_server_request_,
                                      input);
      }
  }
  else
  {
    if (this->orb_server_request_.reply_status () == GIOP::NO_EXCEPTION)
      {
        // In DSI, we can't rely on the skeleton to do this.
        if (this->retval_ == 0 && this->params_ == 0)
          {
          this->orb_server_request_.argument_flag (false);
          }

        this->orb_server_request_.init_reply ();

        // Send the return value, if any.
        if (this->retval_ != 0)
          {
            this->retval_->impl ()->marshal_value (
                                        *this->orb_server_request_.outgoing ());
          }

        // Send the "inout" and "out" parameters.
        if (this->params_ != 0)
          {
            this->params_->_tao_encode (
                              *this->orb_server_request_.outgoing (),
                              CORBA::ARG_INOUT | CORBA::ARG_OUT);
          }
      }
    else
      {
      // This defaults to true, but just to be safe...
      this->orb_server_request_.argument_flag (true);

        // Write the reply header to the ORB request's outgoing CDR stream.
        this->orb_server_request_.init_reply ();

        this->exception_->impl ()->marshal_value (
                                      *this->orb_server_request_.outgoing ());
      }
  }

  // in case a deferred_reply is specified (like for GIOP::LocateRequest)
  // do not send a reply here
  if (this->orb_server_request_.deferred_reply ())
  {
    this->orb_server_request_.tao_send_reply ();
  }
}

void
CORBA::ServerRequest::gateway_exception_reply (ACE_CString &raw_exception)
{
  // This defaults to 1, but just to be safe...
  this->orb_server_request_.argument_flag (true);

  // This reply path handles only user exceptions.
  this->orb_server_request_.reply_status (GIOP::USER_EXCEPTION);

  this->orb_server_request_.init_reply ();

  // We know nothing about this exception, so we marshal it as a block
  // of bytes. The outgoing stream's byte order has already been matched
  // to the original source of the reply.
  this->orb_server_request_.outgoing ()->write_octet_array (
      reinterpret_cast<const CORBA::Octet *> (raw_exception.fast_rep ()),
      static_cast<CORBA::ULong> (raw_exception.length () + ACE_CDR::MAX_ALIGNMENT)
    );

  // This will prevent the marshaling of any parameters into this reply.
  this->sent_gateway_exception_ = true;

  this->orb_server_request_.tao_send_reply ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
