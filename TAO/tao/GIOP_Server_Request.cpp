// $Id$


// Implementation of the Dynamic Server Skeleton Interface  (for GIOP)

#include "tao/GIOP_Server_Request.h"

#include "tao/CDR.h"
#include "tao/POAC.h"
#include "tao/Environment.h"
#include "tao/NVList.h"
#include "tao/Principal.h"
#include "tao/ORB_Core.h"
#include "tao/ORB.h"
#include "tao/Timeprobe.h"
#include "tao/Any.h"
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
                            TAO_ORB_Core *orb_core,
                            const TAO_GIOP_Version &version)
      :mesg_base_ (mesg_base),
       incoming_ (&input),
       outgoing_ (&output),
       response_expected_ (0),
       sync_with_server_ (0),
       lazy_evaluation_ (0),

#if (TAO_HAS_MINIMUM_CORBA == 0)

       params_ (0),

#endif /* TAO_HAS_MINIMUM_CORBA */

       retval_ (0),
       exception_ (0),
       exception_type_ (TAO_GIOP_NO_EXCEPTION),
       orb_core_ (orb_core),
       version_ (version),
       service_info_ (),
       request_id_ (0),
       object_key_ (),
       requesting_principal_ (0)
{
  ACE_FUNCTION_TIMEPROBE (TAO_SERVER_REQUEST_START);
  //no-op
}

// This constructor is used, by the locate request code

TAO_GIOP_ServerRequest::
    TAO_GIOP_ServerRequest (TAO_Pluggable_Messaging *mesg_base,
                            CORBA::ULong &request_id,
                            CORBA::Boolean &response_expected,
                            TAO_ObjectKey &object_key,
                            const ACE_CString &operation,
                            TAO_OutputCDR &output,
                            TAO_ORB_Core *orb_core,
                            const TAO_GIOP_Version &version,
                            int &parse_error)
      : mesg_base_ (mesg_base),
        operation_ (operation),
        incoming_ (0),
        outgoing_ (&output),
        response_expected_ (response_expected),
        sync_with_server_ (0),
        lazy_evaluation_ (0),

#if (TAO_HAS_MINIMUM_CORBA == 0)

        params_ (0),

#endif /* TAO_HAS_MINIMUM_CORBA */

        retval_ (0),
        exception_ (0),
        exception_type_ (TAO_GIOP_NO_EXCEPTION),
        orb_core_ (orb_core),
        version_ (version),
        service_info_ (),
        request_id_ (request_id),
        object_key_ (object_key),
        requesting_principal_ (0)
{
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

TAO_POA *
TAO_GIOP_ServerRequest::oa (void)
{
  return this->orb_core_->root_poa ();
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
  else
  {

#if (TAO_HAS_MINIMUM_CORBA == 0)

    const PortableServer::ForwardRequest *forward_request = 0;

    // If extraction of exception succeeded
    if ((value >>= forward_request) != 0)
      {
        this->forward_location_ = forward_request->forward_reference;
        this->exception_type_ = TAO_GIOP_LOCATION_FORWARD;
      }
    // Normal exception
    else

#endif /* TAO_HAS_MINIMUM_CORBA */

      {
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

            if (CORBA_SystemException::_narrow (x) != 0)
              {
                this->exception_type_ = TAO_GIOP_SYSTEM_EXCEPTION;
              }
          }
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
              (void) this->outgoing_->append (tc.in (), &cdr,
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
  // Construct a REPLY header.
  this->mesg_base_->write_protocol_header (TAO_PLUGGABLE_MESSAGE_REPLY,
                                           *this->outgoing_);

#if (TAO_HAS_MINIMUM_CORBA == 1)
  *this->outgoing_ << this->service_info_;
#else
  if (this->lazy_evaluation_ == 0 || this->params_ == 0)
    {
      *this->outgoing_ << this->service_info_;
    }
  else
    {
      // If lazy evaluation is enabled then we are going to insert an
      // extra node at the end of the service context list, just to
      // force the appropiate padding.
      // But first we take it out any of them..
      CORBA::ULong count = 0;
      CORBA::ULong l = this->service_info_.length ();
      CORBA::ULong i;
      for (i = 0; i != l; ++i)
        {
          if (this->service_info_[i].context_id == TAO_SVC_CONTEXT_ALIGN)
            continue;
          count++;
        }
      // Now increment it to account for the last dummy one...
      count++;

      // Now marshal the rest of the service context objects
      *this->outgoing_ << count;
      for (i = 0; i != l; ++i)
        {
          if (this->service_info_[i].context_id == TAO_SVC_CONTEXT_ALIGN)
            continue;
          *this->outgoing_ << this->service_info_[i];
        }

      // @@ Much of this code is GIOP 1.1 specific and should be
      //    re-thought once GIOP 1.2 is implemented, this is not a big
      //    deal because the code is only used in DSI gateways.
      ptr_arith_t target =
        this->params_->_tao_target_alignment ();

      ptr_arith_t current =
        ptr_arith_t(this->outgoing_->current_alignment ()) % ACE_CDR::MAX_ALIGNMENT;

      CORBA::ULong pad = 0;
      if (target == 0)
        {
          // We want to generate adequate padding to start the request
          // id on a 8 byte boundary, two cases:
          // - If the dummy tag starts on a 4 byte boundary and the
          //   dummy sequence has 0 elements then we have:
          //   4:tag 8:sequence_length 4:sequence_body 4:request_id
          //   8:payload
          // - If the dummy tag starts on an 8 byte boundary, with 4
          //   elements we get:
          //   8:tag 4:sequence_length 8:sequence_body 4:request_id
          //   8:payload
          if (current != 0 && current <= ACE_CDR::LONG_ALIGN)
            {
              pad = 4;
            }
        }
      else if (target != ACE_CDR::LONG_ALIGN)
        {
          // The situation reverses, we want to generate adequate
          // padding to start the request id on a 4 byte boundary, two
          // cases:
          // - If the dummy tag starts on a 4 byte boundary and the
          //   dummy sequence has 4 elements then we have:
          //   4:tag 8:sequence_length 4:sequence_body 8:request_id
          //   4:payload
          // - If the dummy tag starts on an 8 byte boundary, with 0
          //   elements we get:
          //   8:tag 4:sequence_length 8:sequence_body 8:request_id
          //   4:payload
          if (current > ACE_CDR::LONG_ALIGN)
            {
              pad = 4;
            }
        }
      else if (target == ACE_CDR::MAX_ALIGNMENT)
        {
          pad = 0;
        }
      else
        {
          // <target> can only have the values above
          ACE_THROW (CORBA::MARSHAL ());
        }

      *this->outgoing_ << CORBA::ULong(TAO_SVC_CONTEXT_ALIGN);
      *this->outgoing_ << pad;
      for (CORBA::ULong j = 0; j != pad; ++j)
        {
          *this->outgoing_ << ACE_OutputCDR::from_octet(0);
        }
    }
#endif /* TAO_HAS_MINIMUM_CORBA */

  this->outgoing_->write_ulong (this->request_id_);

  // Standard exceptions are caught in Connect::handle_input

  // Forward exception only.
  if (!CORBA::is_nil (this->forward_location_.in ()))
    {
      this->outgoing_->write_ulong (TAO_GIOP_LOCATION_FORWARD);

      CORBA::Object_ptr object_ptr = this->forward_location_.in ();
      if ((*this->outgoing_ << object_ptr) == 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ASYS_TEXT ("TAO_GIOP_ServerRequest::marshal - ")
                      ASYS_TEXT ("encoding forwarded objref failed\n")));
          return;
        }
    }

  // Any exception at all.
  else if (this->exception_ == 0)
    {
      // First finish the GIOP header ...
      this->outgoing_->write_ulong (TAO_GIOP_NO_EXCEPTION);
    }
  else
    {
      CORBA::TypeCode_ptr except_tc;

      except_tc = this->exception_->type ();

      // Finish the GIOP Reply header, then marshal the exception.
      // XXX x->type () someday ...

      this->outgoing_->write_ulong (this->exception_type_);

      // we use the any's ACE_Message_Block
      TAO_InputCDR cdr (this->exception_->_tao_get_cdr (),
                        this->exception_->_tao_byte_order ());
      (void) this->outgoing_->append (except_tc, &cdr, ACE_TRY_ENV);
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
TAO_GIOP_ServerRequest::send_no_exception_reply (TAO_Transport *transport)
{
  this->mesg_base_->write_protocol_header (TAO_PLUGGABLE_MESSAGE_REPLY,
                                           *this->outgoing_);

  IOP::ServiceContextList resp_ctx;
  resp_ctx.length (0);

  *this->outgoing_ << resp_ctx;
  this->outgoing_->write_ulong (this->request_id_);
  this->outgoing_->write_ulong (TAO_GIOP_NO_EXCEPTION);

  int result = this->mesg_base_->send_message (transport,
                                               *this->outgoing_);



  if (result == -1)
    {
      if (TAO_debug_level > 0)
        {
          // No exception but some kind of error, yet a response
          // is required.
          ACE_ERROR ((LM_ERROR,
                      ASYS_TEXT ("TAO: (%P|%t) %p: cannot send NO_EXCEPTION reply\n"),
                      ASYS_TEXT ("TAO_GIOP_ServerRequest::send_no_exception_reply")));
        }
    }
}
