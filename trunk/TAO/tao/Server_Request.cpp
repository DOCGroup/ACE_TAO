// $Id$

// Implementation of the Dynamic Server Skeleton Interface

#include "tao/Server_Request.h"

#include "tao/CDR.h"
#include "tao/POAC.h"
#include "tao/Environment.h"
#include "tao/NVList.h"
#include "tao/Principal.h"
#include "tao/ORB_Core.h"
#include "tao/Timeprobe.h"
#include "tao/Any.h"

#if !defined (__ACE_INLINE__)
# include "tao/Server_Request.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(tao, Server_Request, "$Id$")

#if defined (ACE_ENABLE_TIMEPROBES)

static const char *TAO_Server_Request_Timeprobe_Description[] =
{
  "Server_Request::Server_Request - start",
  "Server_Request::Server_Request - end",
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

IIOP_ServerRequest::IIOP_ServerRequest (TAO_InputCDR &input,
                                        TAO_OutputCDR &output,
                                        TAO_ORB_Core *orb_core,
                                        CORBA::Environment &env)
  : operation_ (0),
    incoming_ (&input),
    outgoing_ (&output),
    response_expected_ (0),

#if !defined (TAO_HAS_MINIMUM_CORBA)

    params_ (0),

#endif /* TAO_HAS_MINIMUM_CORBA */

    retval_ (0),
    exception_ (0),
    exception_type_ (TAO_GIOP_NO_EXCEPTION),
    orb_core_ (orb_core),
    service_info_ (),
    request_id_ (0),
    object_key_ (),
    requesting_principal_ (0)
{
  ACE_FUNCTION_TIMEPROBE (TAO_SERVER_REQUEST_START);

  this->parse_header (env);
}

void
IIOP_ServerRequest::parse_header_std (CORBA::Environment &env)
{
  // Tear out the service context ... we currently ignore it, but it
  // should probably be passed to each ORB service as appropriate
  // (e.g. transactions, security).
  //
  // NOTE: As security support kicks in, this is a good place to
  // verify a digital signature, if that is required in this security
  // environment.  It may be required even when using IPSEC security
  // infrastructure.

  TAO_InputCDR& input = *this->incoming_;

  input >> this->service_info_;
  CORBA::Boolean hdr_status = input.good_bit ();

  // Get the rest of the request header ...

  hdr_status = hdr_status && input.read_ulong (this->request_id_);
  hdr_status = hdr_status && input.read_boolean (this->response_expected_);

  // We use ad-hoc demarshalling here: there is no need to increase
  // the reference count on the CDR message block, because this key
  // will not outlive the request (or the message block).

  CORBA::Long key_length;
  hdr_status = hdr_status && input.read_long (key_length);
  if (hdr_status)
    {
      this->object_key_.replace (key_length, key_length,
                                 (CORBA::Octet*)input.rd_ptr (),
                                 0);
      input.skip_bytes (key_length);
    }

  CORBA::Long length;
  hdr_status = hdr_status && input.read_long (length);
  if (hdr_status)
    {
      this->operation_ = input.rd_ptr ();
      hdr_status = input.skip_bytes (length);
    }

  if (hdr_status)
    {
      input >> this->requesting_principal_;
      hdr_status = input.good_bit ();
    }

  if (!hdr_status)
    env.exception (new CORBA::COMM_FAILURE (CORBA::COMPLETED_NO));

}

void
IIOP_ServerRequest::parse_header_lite (CORBA::Environment &env)
{
  TAO_InputCDR& input = *this->incoming_;

  CORBA::Boolean hdr_status = input.good_bit ();

  // Get the rest of the request header ...

  hdr_status = hdr_status && input.read_ulong (this->request_id_);
  hdr_status = hdr_status && input.read_boolean (this->response_expected_);

  // We use ad-hoc demarshalling here: there is no need to increase
  // the reference count on the CDR message block, because this key
  // will not outlive the request (or the message block).

  CORBA::Long key_length;
  hdr_status = hdr_status && input.read_long (key_length);
  if (hdr_status)
    {
      this->object_key_.replace (key_length, key_length,
                                 (CORBA::Octet*)input.rd_ptr (),
                                 0);
      input.skip_bytes (key_length);
    }

  CORBA::Long length;
  hdr_status = hdr_status && input.read_long (length);
  if (hdr_status)
    {
      this->operation_ = input.rd_ptr ();
      hdr_status = input.skip_bytes (length);
    }

  if (!hdr_status)
    env.exception (new CORBA::COMM_FAILURE (CORBA::COMPLETED_NO));
}



void
IIOP_ServerRequest::parse_header (CORBA::Environment &env)
{
  if (this->orb_core_->orb_params ()->use_lite_protocol ())
    this->parse_header_lite (env);
  else
    this->parse_header_std (env);
}

// This constructor is used, by the locate request code

IIOP_ServerRequest::IIOP_ServerRequest (CORBA::ULong &request_id,
                                        CORBA::Boolean &response_expected,
                                        TAO_ObjectKey &object_key,
                                        char* operation,
                                        TAO_OutputCDR &output,
                                        TAO_ORB_Core *orb_core,
                                        CORBA::Environment &)
  : operation_ (operation),
    incoming_ (0),
    outgoing_ (&output),
    response_expected_ (response_expected),

#if !defined (TAO_HAS_MINIMUM_CORBA)

    params_ (0),

#endif /* TAO_HAS_MINIMUM_CORBA */

    retval_ (0),
    exception_ (0),
    exception_type_ (TAO_GIOP_NO_EXCEPTION),
    orb_core_ (orb_core),
    service_info_ (0),
    request_id_ (request_id),
    object_key_ (object_key),
    requesting_principal_ (0)
{
}

IIOP_ServerRequest::~IIOP_ServerRequest (void)
{

#if !defined (TAO_HAS_MINIMUM_CORBA)

  if (this->params_)
    CORBA::release (this->params_);

#endif /* TAO_HAS_MINIMUM_CORBA */

  delete this->retval_;
  delete this->exception_;
}

CORBA::ORB_ptr
IIOP_ServerRequest::orb (void)
{
  return this->orb_core_->orb ();
}

TAO_POA *
IIOP_ServerRequest::oa (void)
{
  return this->orb_core_->root_poa ();
}

#if !defined (TAO_HAS_MINIMUM_CORBA)

// Unmarshal in/inout params, and set up to marshal the appropriate
// inout/out/return values later on.

void
IIOP_ServerRequest::arguments (CORBA::NVList_ptr &list,
                               CORBA::Environment &env)
{
  env.clear ();

  // Save params for later use when marshaling the reply.
  this->params_ = list;

  // Then unmarshal each "in" and "inout" parameter.
  for (u_int i = 0; i < list->count (); i++)
    {
      CORBA::NamedValue_ptr nv = list->item (i, env);

      // check if it is an in or inout parameter
      if (ACE_BIT_DISABLED (nv->flags (), CORBA::ARG_IN | CORBA::ARG_INOUT))
        continue;

      CORBA::Any_ptr any = nv->value ();
      CORBA::TypeCode_var tc = any->type ();

      // @@ (JP) The following code depends on the fact that 
      // TO_InputCDR does not contain chained message blocks.
      char *begin, *end;

      // This will be the start of a new message block.
      begin = this->incoming_->rd_ptr ();

      // Skip over the next aregument.
      CORBA::TypeCode::traverse_status status = 
        this->incoming_->skip (tc.in (), env);

      if (status != CORBA::TypeCode::TRAVERSE_CONTINUE)
        {
          const char* param_name = nv->name ();

          if (param_name == 0)
            param_name = "(no name given)";

          ACE_ERROR ((LM_ERROR,
                      "IIOP_ServerRequest::arguments - problem while"
                      " decoding parameter %d <%s>\n", i, param_name));
          return;
        }

      // This will be the end of the new message block.
      end = this->incoming_->rd_ptr ();

      // Allocate the new message block and set its endpoints.
      ACE_Message_Block *cdr;

      ACE_NEW (cdr,
               ACE_Message_Block (end - begin));

      cdr->rd_ptr (begin);

      cdr->wr_ptr (end);

      // Stick it into the Any. It gets duplicated there.
      any->_tao_replace (tc.in (),
                         cdr,
                         0,
                         env);  
  
      // Now we can release the original.                         
      ACE_Message_Block::release (cdr);      
    }

  // If any data is left over, it'd be context values ... else error.
  // We don't support context values, so it's always an error.

  // @@ (TAO) support for Contexts??
  if (incoming_->length () != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "IIOP_ServerRequest::arguments - "
                  "%d bytes left in buffer\n", incoming_->length ()));
      env.exception (new CORBA::BAD_PARAM (CORBA::COMPLETED_NO));
    }
}

// Store the result value.  There's either an exception, or a result,
// but not both of them.  Results (and exceptions) can be reported
// only after the parameter list has been provided (maybe empty).

void
IIOP_ServerRequest::set_result (const CORBA::Any &value,
                                CORBA::Environment &env)
{
  env.clear ();

  // setting a result when another result already exists or if an exception
  // exists is an error
  if (this->retval_ || this->exception_)
    env.exception (new CORBA::BAD_INV_ORDER (CORBA::COMPLETED_NO));
  else
    {
      this->retval_ = new CORBA::Any (value);
    }
}

// Store the exception value.

void
IIOP_ServerRequest::set_exception (const CORBA::Any &value,
                                   CORBA::Environment &env)
{
  if (this->retval_ || this->exception_)
    env.exception (new CORBA::BAD_INV_ORDER (CORBA::COMPLETED_NO));
  else
  {
    // Try to narrow to ForwardRequest
    PortableServer::ForwardRequest_ptr forward_request =
        PortableServer::ForwardRequest::_narrow ((CORBA::Exception *) value.value ());

    // If narrowing of exception succeeded
    if (forward_request != 0)
      {
        this->forward_location_ = forward_request->forward_reference;
        this->exception_type_ = TAO_GIOP_USER_EXCEPTION;
      }
    // Normal exception
    else
     {
       this->exception_ = new CORBA::Any (value);

       // @@ This cast is not safe, but we haven't implemented the >>=
       // and <<= operators for base exceptions (yet).
       CORBA_Exception* x = (CORBA_Exception*)value.value ();
       if (CORBA_UserException::_narrow (x) != 0)
         this->exception_type_ = TAO_GIOP_USER_EXCEPTION;
       else
        this->exception_type_ = TAO_GIOP_SYSTEM_EXCEPTION;
    }
  }
}

// this method will be utilized by the DSI servant to marshal outgoing
// parameters

void
IIOP_ServerRequest::dsi_marshal (CORBA::Environment &env)
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
              (void) this->outgoing_->encode (tc.in (), retval_->value (), 0, env);
            }
          else
            {
              TAO_InputCDR cdr (retval_->_tao_get_cdr ());
              (void) this->outgoing_->append (tc.in (), &cdr, env);
            }
        }

      // ... Followed by "inout" and "out" parameters, left to right
      if (this->params_)
        {
          for (u_int i = 0;
               i < this->params_->count ();
               i++)
            {
              CORBA::NamedValue_ptr nv = this->params_->item (i, env);
              if (!(nv->flags () & (CORBA::ARG_INOUT|CORBA::ARG_OUT)))
                continue;

              CORBA::Any_ptr any = nv->value ();
              CORBA::TypeCode_var tc = any->type ();
              if (any->any_owns_data ())
                {
                  (void) this->outgoing_->encode (tc.in (), any->value (), 0, env);
                }
              else
                {
                  TAO_InputCDR cdr (any->_tao_get_cdr ());
                  (void) this->outgoing_->append (tc.in (), &cdr, env);
                }
            }
        }
    }
}

#endif /* TAO_HAS_MINIMUM_CORBA */

// Extension
void
IIOP_ServerRequest::demarshal (CORBA::Environment &orb_env,
                               // ORB related exception reporting
                               const TAO_Call_Data_Skel *info,
                               // call description
                               ...)
{
  CORBA::ULong i;
  const TAO_Param_Data_Skel *pdp;
  va_list param_vector;
  va_start (param_vector, info);

  for (i = 0, pdp = info->params;
       i < info->param_count;
       i++, pdp++)
    {
      void *ptr = va_arg (param_vector, void *);

      if ((pdp->mode == CORBA::ARG_IN)
          || (pdp->mode == CORBA::ARG_INOUT))
        {
          // Then just unmarshal the value.
          (void) incoming_->decode (pdp->tc, ptr, 0, orb_env);
        }

      if (orb_env.exception ())
        {
          orb_env.print_exception ("ServerRequest::demarshal - parameter decode failed");
          return;
        }
    }
  va_end (param_vector);
}

// Extension

void
IIOP_ServerRequest::marshal (CORBA::Environment &orb_env,
                             // ORB related exception reporting
                             //                             CORBA::Environment &skel_env,
                             // skeleton related exception reporting
                             const TAO_Call_Data_Skel *info,
                             // call description
                             ...)
{
  // what is "orb_env" and "skel_env"?
  // "skel_env" holds the exception that got raised inside the operation
  // implementation (upcall)
  //
  // "orb_env" is the exception that may have been raised due to things going
  // wrong in the entire dispatch process. These are always system exceptions.

  // check if we are inside with an exception. This may have happened
  // since the upcall could have set some exception
#if 0 /* ASG */
  if (skel_env.exception ())
    {
      // We must increase the "refcnt" on the exception, because it is
      // "owned" by both <skel_env> and (eventually) by the
      // Server_Request.
      CORBA::Exception_ptr exception = skel_env.exception ();
      exception->_incr_refcnt ();

      // The Any does not own the because ultimately it will be owned
      // by the Server_Request via the call to "set_exception"
      CORBA::Any any (skel_env.exception ()->_type (), exception);
      this->set_exception (any, orb_env);
    }
#endif

  // Setup a Reply message so that we can marshal all the outgoing parameters
  // into it. If an exception was set, then that gets marshaled into the reply
  // message and we don't do anything after that
  this->init_reply (orb_env);

#if 0 /* ASG */
  // exception? nothing to do after this
  if (orb_env.exception () || skel_env.exception ())
    return;
#endif
  TAO_CHECK_ENV_RETURN_VOID (orb_env);

  CORBA::ULong i;
  const TAO_Param_Data_Skel *pdp;
  va_list param_vector;
  va_start (param_vector, info);

  for (i = 0, pdp = info->params;
       i < info->param_count;
       i++, pdp++)
    {
      void *ptr = va_arg (param_vector, void *);

      if (pdp->mode == 0)
        {
          // check if the return type is not void
          if (pdp->tc->kind (orb_env) != CORBA::tk_void)
            {
              // Then just marshal the value.
              (void) this->outgoing_->encode (pdp->tc, ptr, 0, orb_env);
            }
        }
      else if ((pdp->mode == CORBA::ARG_INOUT)
          || (pdp->mode == CORBA::ARG_OUT))
        {
          // Then just marshal the value.
          (void) this->outgoing_->encode (pdp->tc, ptr, 0, orb_env);
        }

      if (orb_env.exception ())
        {
          orb_env.print_exception ("ServerRequest::marshal - parameter encode failed");
          return;
        }
    }
  va_end (param_vector);

}

void
IIOP_ServerRequest::init_reply (CORBA::Environment &env)
{
  // Construct a REPLY header.
  TAO_GIOP::start_message (TAO_GIOP::Reply,
			                     *this->outgoing_,
			                     this->orb_core_);

  TAO_GIOP_ServiceContextList resp_ctx;
  resp_ctx.length (0);
  *this->outgoing_ << resp_ctx;
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
                      "ServerRequest::marshal - "
                      "encoding forwarded objref failed\n"));
          return;
        }
    }

  // Any exception at all.
  else if (this->exception_)
    {
      CORBA::TypeCode_ptr except_tc;

      except_tc = this->exception_->type ();

      // Finish the GIOP Reply header, then marshal the exception.
      // XXX x->type () someday ...

      this->outgoing_->write_ulong (this->exception_type_);

      // we use the any's ACE_Message_Block
      TAO_InputCDR cdr (this->exception_->_tao_get_cdr ());
      (void) this->outgoing_->append (except_tc, &cdr, env);
    }
  else // Normal reply
    // First finish the GIOP header ...
    this->outgoing_->write_ulong (TAO_GIOP_NO_EXCEPTION);
}

CORBA::Object_ptr
IIOP_ServerRequest::forward_location (void)
// get the forward_location
{
  return CORBA::Object::_duplicate (this->forward_location_.in ());
}

CORBA::ULong
IIOP_ServerRequest::exception_type (void)
// get the exception type
{
  return this->exception_type_;
}
