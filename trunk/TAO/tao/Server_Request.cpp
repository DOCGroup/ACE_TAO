// $Id$

// Implementation of the Dynamic Server Skeleton Interface

#include "tao/corba.h"

#if !defined (__ACE_INLINE__)
# include "tao/Server_Request.i"
#endif /* ! __ACE_INLINE__ */

// {77420086-F276-11ce-9598-0000C07CA898}
DEFINE_GUID (IID_IIOP_ServerRequest,
0x77420086, 0xf276, 0x11ce, 0x95, 0x98, 0x0, 0x0, 0xc0, 0x7c, 0xa8, 0x98);

// {4B48D881-F7F0-11ce-9598-0000C07CA898}
DEFINE_GUID (IID_CORBA_ServerRequest,
0x4b48d881, 0xf7f0, 0x11ce, 0x95, 0x98, 0x0, 0x0, 0xc0, 0x7c, 0xa8, 0x98);

CORBA_ServerRequest *
CORBA_ServerRequest::_duplicate (CORBA_ServerRequest *req)
{
  if (req)
    {
      req->AddRef ();
      return req;
    }
  return (CORBA_ServerRequest *) 0;
}

CORBA_ServerRequest *
CORBA_ServerRequest::_nil (void)
{
  return (CORBA_ServerRequest *) 0;
}

IIOP_ServerRequest::IIOP_ServerRequest (const TAO_GIOP_RequestHeader &hdr,
					TAO_InputCDR *req,
                                        TAO_OutputCDR *resp,
                                        CORBA::ORB_ptr the_orb,
                                        TAO_POA *the_poa)
  : opname_ (CORBA::string_dup (hdr.operation)),
    incoming_ (req),
    outgoing_ (resp),
    reqid_ (hdr.request_id),
    params_ (0),
    retval_ (0),
    exception_ (0),
    refcount_ (1),
    orb_ (the_orb),
    poa_ (the_poa)
{
}

IIOP_ServerRequest::~IIOP_ServerRequest (void)
{
  if (this->params_)
    CORBA::release (this->params_);
  if (this->retval_)
    delete this->retval_;
  if (this->exception_)
    delete this->exception_;
}

ULONG
IIOP_ServerRequest::AddRef (void)
{
  ACE_ASSERT (this->refcount_ > 0);
  return this->refcount_++;
}

ULONG
IIOP_ServerRequest::Release (void)
{
  ACE_ASSERT (this != 0);

  if (--this->refcount_ != 0)
    return this->refcount_;

  delete this;
  return 0;
}

TAO_HRESULT
IIOP_ServerRequest::QueryInterface (REFIID riid,
                                    void **ppv)
{
  ACE_ASSERT (this->refcount_ > 0);
  *ppv = 0;

  if (IID_IIOP_ServerRequest == riid
      || IID_CORBA_ServerRequest == riid
      || IID_TAO_IUnknown == riid)
    *ppv = this;

  if (*ppv == 0)
    return TAO_ResultFromScode (TAO_E_NOINTERFACE);

 (void) this->AddRef ();
  return TAO_NOERROR;
}

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

      // First, make sure the memory into which we'll be unmarshaling
      // exists, and is the right size.
      //
      // NOTE: desirable to have a way to let the dynamic
      // implementation routine preallocate this data, for
      // environments where DSI is just being used in lieu of a
      // language mapped server-side API and the size is really
      // knowable in advance.
      //
      // This is exactly what the TAO IDL compiler generated skeletons do.

      CORBA::Any_ptr any = nv->value ();
      CORBA::TypeCode_ptr tc = any->type ();

      tc->AddRef ();

      void *value;
      if (!any->value ())
        { // not preallocated
          ACE_NEW (value, char [tc->size (env)]);

	  if (env.exception () != 0)
	    return;

          any->replace (tc, value, CORBA::B_TRUE, env);
	  if (env.exception () != 0)
	    return;

          // Decrement the refcount of "tc".
          //
          // The earlier AddRef is needed since Any::replace () releases
          // the typecode inside the Any.  Without the dup, the reference
          // count can go to zero, and the typecode would then be deleted.
          //
          // This Release ensures that the reference count is correct so
          // the typecode can be deleted some other time.

          tc->Release ();
        }
      else
        value = (void *)any->value (); // memory was already preallocated

      // Then just unmarshal the value.
      (void) incoming_->decode (tc, value, 0, env);
      if (env.exception () != 0)
	{
	  const char* param_name = nv->name ();
	  if (param_name == 0)
	    param_name = "(no name given)";
	  ACE_ERROR ((LM_ERROR,
		      "IIOP_ServerRequest::arguments - problem while"
		      " decoding parameter %d <%s>\n", i, param_name));
	  return;
	}
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
  if (!this->params_ || this->retval_ || this->exception_)
    env.exception (new CORBA::BAD_INV_ORDER (CORBA::COMPLETED_NO));
  else
    {
      this->retval_ = new CORBA::Any;
      this->retval_->replace (value.type (), value.value (), 1, env);
    }
}

// Store the exception value.

void
IIOP_ServerRequest::set_exception (const CORBA::Any &value,
                                   CORBA::Environment &env)
{
  if (!this->params_ || this->retval_ || this->exception_)
    env.exception (new CORBA::BAD_INV_ORDER (CORBA::COMPLETED_NO));
  else
    {
      this->exception_ = new CORBA::Any;
      this->exception_->replace (value.type (), value.value (), 1, env);

      // @@ This cast is not safe, but we haven't implemented the >>=
      // and <<= operators for base exceptions (yet).
      CORBA_Exception* x = (CORBA_Exception*)value.value ();
      if (CORBA_UserException::_narrow (x) != 0)
	this->is_user_exception_ = 1;
      else
	this->is_user_exception_ = 0;
    }
}

// Extension
void
IIOP_ServerRequest::demarshal (CORBA::Environment &env,  // exception reporting
                               const TAO_Call_Data_Skel *info, // call description
                               ...)                       // ... any parameters
{
  CORBA::NVList_ptr nvlist;

  // Create an NVList
  this->orb ()->create_list (0, nvlist);

  // Now, put all "in" and "inout" parameters into the NVList.
  CORBA::ULong i;

  // Setup the variable argument list.
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
        // Populate the NVList.
        (void) nvlist->add_item (0, pdp->mode, env)->value ()->replace (pdp->tc, ptr, pdp->own, env);
      else if (pdp->mode == CORBA::ARG_OUT)
        // Don't add any value.
        (void) nvlist->add_item (0, pdp->mode, env);

    }

  va_end (param_vector);

  // Now demarshal the parameters using a call to params.
  this->arguments (nvlist, env); // nvlist is now owned by us
}

// Extension

void
IIOP_ServerRequest::marshal (CORBA::Environment &env,  // exception reporting
                             const TAO_Call_Data_Skel *info, // call description
                             ...)                       // ... any parameters
{
  CORBA::Environment env2;

  // check if we are inside with an exception. This may have happened
  // since the upcall could have set some exception
  if (env.exception ())
    {
      CORBA::Any any (env.exception ()->_type (), env.exception ()); // don't
                                                                     // own it
      this->set_exception (any, env2);
    }

  // Setup a Reply message.
  this->init_reply (env2);

  if (env2.exception () || env.exception ()) // exception, nothing to do
    return;

  // Now, put all "in" and "inout" parameters into the NVList.
  CORBA::ULong i;
  CORBA::ULong j;

  // Setup the variable argument list.
  const TAO_Param_Data_Skel *pdp;
  va_list param_vector;
  va_start (param_vector, info);

  j = 0;

  for (i = 0, pdp = info->params;
       i < info->param_count;
       i++, pdp++)
    {
      void *ptr = va_arg (param_vector, void *);

      if (pdp->mode == 0)
        {
          // check if the return type is not void
          if (pdp->tc->kind (env2) != CORBA::tk_void)
            {
              this->retval_ = new CORBA::Any (pdp->tc, ptr, pdp->own);
            }
          continue;
        }

      if (pdp->mode == CORBA::ARG_OUT)
        // don't add any value.
        (void) this->params_->item (j, env)->value ()->replace (pdp->tc, ptr, pdp->own, env);

      j++;
    }
  va_end (param_vector);

  // Normal reply.
  if (!env.exception ())
    {
      // ... then send any return value ...
      if (this->retval_)
        {
          CORBA::TypeCode_ptr tc = this->retval_->type ();
          const void *value = this->retval_->value ();

          if (value)
            (void) this->outgoing_->encode (tc, value, 0, env);
        }

      // ... Followed by "inout" and "out" parameters, left to right
      for (i = 0;
           i < this->params_->count ();
           i++)
        {
          CORBA::NamedValue_ptr nv = this->params_->item (i, env);
          CORBA::Any_ptr any;

          if (!(nv->flags () & (CORBA::ARG_INOUT|CORBA::ARG_OUT)))
            continue;

          any = nv->value ();
          CORBA::TypeCode_ptr tc = any->type ();
          const void *value = any->value ();
          (void) this->outgoing_->encode (tc, value, 0, env);
        }
    }
}

void
IIOP_ServerRequest::init_reply (CORBA::Environment &env)
{
  // Construct a REPLY header.
  TAO_GIOP::start_message (TAO_GIOP::Reply, *this->outgoing_);
  TAO_GIOP_ServiceContextList resp_ctx;
  resp_ctx.length (0);
  this->outgoing_->encode (TC_ServiceContextList,
                           &resp_ctx,
                           0,
                           env);
  this->outgoing_->write_ulong (this->reqid_);

  // Standard exceptions only.
  if (env.exception () != 0)
    {
      CORBA::Environment env2;
      CORBA::Exception *x = env.exception ();
      CORBA::TypeCode_ptr except_tc = x->_type ();

      this->outgoing_->write_ulong (TAO_GIOP_SYSTEM_EXCEPTION);
      (void) this->outgoing_->encode (except_tc, x, 0, env2);
    }

  // Any exception at all.
  else if (this->exception_)
    {
      CORBA::TypeCode_ptr except_tc;

      except_tc = this->exception_->type ();

      // Finish the GIOP Reply header, then marshal the exception.
      // XXX x->type () someday ...

      if (this->is_user_exception_)
        this->outgoing_->write_ulong (TAO_GIOP_USER_EXCEPTION);
      else
        this->outgoing_->write_ulong (TAO_GIOP_SYSTEM_EXCEPTION);

      TAO_InputCDR cdr ((ACE_Message_Block*)this->exception_->value ());
      (void) this->outgoing_->append (except_tc, &cdr, env);
    }
  else // Normal reply
    // First finish the GIOP header ...
    this->outgoing_->write_ulong (TAO_GIOP_NO_EXCEPTION);
}
