// $Id$

// @(#)svrrqst.cpp      1.9 95/11/04
// Copyright 1994-1995 by Sun Microsystems Inc.
// All Rights Reserved
//
// Implementation of the Dynamic Server Skeleton Interface

#include "tao/corba.h"

// {77420086-F276-11ce-9598-0000C07CA898}
DEFINE_GUID (IID_IIOP_ServerRequest,
0x77420086, 0xf276, 0x11ce, 0x95, 0x98, 0x0, 0x0, 0xc0, 0x7c, 0xa8, 0x98);

// {4B48D881-F7F0-11ce-9598-0000C07CA898}
DEFINE_GUID (IID_CORBA_ServerRequest,
0x4b48d881, 0xf7f0, 0x11ce, 0x95, 0x98, 0x0, 0x0, 0xc0, 0x7c, 0xa8, 0x98);

IIOP_ServerRequest::IIOP_ServerRequest (CDR *req,
                                        CDR *resp,
                                        CORBA::ULong reqid,
                                        CORBA::ORB_ptr the_orb,
                                        TAO_POA *the_poa)
  : incoming_ (req),
    outgoing_ (resp),
    reqid_ (reqid),
    params_ (0),
    retval_ (0),
    exception_ (0),
    ex_type_ (CORBA::NO_EXCEPTION),
    refcount_ (1),
    orb_ (the_orb),
    poa_ (the_poa)
{
}

IIOP_ServerRequest::~IIOP_ServerRequest (void)
{
  ACE_ASSERT (refcount_ == 0);

  if (params_)
    CORBA::release (params_);
  if (retval_)
    delete retval_;
  if (exception_)
    delete exception_;
}

ULONG
IIOP_ServerRequest::AddRef (void)
{
  ACE_ASSERT (refcount_ > 0);
  return refcount_++;
}

ULONG
IIOP_ServerRequest::Release (void)
{
  {
    ACE_ASSERT (this != 0);

    if (--refcount_ != 0)
      return refcount_;
  }

  delete this;
  return 0;
}

TAO_HRESULT
IIOP_ServerRequest::QueryInterface (REFIID riid,
                                    void **ppv)
{
  ACE_ASSERT (refcount_ > 0);
  *ppv = 0;

  if (IID_IIOP_ServerRequest == riid
      || IID_CORBA_ServerRequest == riid
      || IID_TAO_IUnknown == riid)
    *ppv = this;

  if (*ppv == 0)
    return ResultFromScode (TAO_E_NOINTERFACE);

 (void) AddRef ();
  return TAO_NOERROR;
}

// Unmarshal in/inout params, and set up to marshal the appropriate
// inout/out/return values later on.

void
IIOP_ServerRequest::params (CORBA::NVList_ptr list,
                            CORBA::Environment &env)
{
  env.clear ();

  // Save params for later use when marshaling the reply.
  this->params_ = list;

  // Then unmarshal each "in" and "inout" parameter.
  for (u_int i = 0; i < list->count (); i++)
    {
      CORBA::NamedValue_ptr nv = list->item (i, env);

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

      CORBA::Any_ptr any = nv->value ();
      CORBA::TypeCode_ptr tc = any->type ();

      tc->AddRef ();

      void *value;
      if (!any->value ())
        { // not preallocated
          ACE_NEW (value, char [tc->size (env)]);

          any->replace (tc, value, CORBA::B_TRUE, env);

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
        value = (void *)any->value ();

      // Then just unmarshal the value.
      (void) incoming_->decode (tc, value, 0, env);
    }

  // If any data is left over, it'd be context values ... else error.
  // We don't support context values, so it's always an error.

  if (incoming_->bytes_remaining () != 0)
    {
      dmsg1 ("params (), %d bytes remaining (error)",
             incoming_->bytes_remaining ());
      env.exception (new CORBA::BAD_PARAM (CORBA::COMPLETED_NO));
    }
}

// Store the result value.  There's either an exception, or a result,
// but not both of them.  Results (and exceptions) can be reported
// only after the parameter list has been provided (maybe empty).

void
IIOP_ServerRequest::result (CORBA::Any_ptr value,
                            CORBA::Environment &env)
{
  env.clear ();

  if (!params_ || retval_ || exception_)
    env.exception (new CORBA::BAD_INV_ORDER (CORBA::COMPLETED_NO));
  else
    retval_ = value;

  // XXX send the message now!
}

// Store the exception value.

void
IIOP_ServerRequest::exception (CORBA::ExceptionType type,
                               CORBA::Any_ptr value,
                               CORBA::Environment &env)
{
  if (!params_ || retval_ || exception_)
    env.exception (new CORBA::BAD_INV_ORDER (CORBA::COMPLETED_NO));
  else
    {
      env.clear ();
      exception_ = value;
      ex_type_ = type;
    }

  // XXX send the message now!
}

// Invocation attributes.

CORBA::String
IIOP_ServerRequest::op_name (void)
{
  return opname_;
}

CORBA::Object_ptr
IIOP_ServerRequest::target (void)
{
  // XXX implement me!!  Code from TCP_OA exists ...
  return 0;
}

CORBA::Principal_ptr
IIOP_ServerRequest::caller (void)
{
  // XXX ... return client's principal
  return 0;
}

CORBA::ORB_ptr
IIOP_ServerRequest::orb (void)
{
  return orb_;
}

TAO_POA *
IIOP_ServerRequest::oa (void)
{
  return poa_;
}

// Extension
void
IIOP_ServerRequest::demarshal (CORBA::Environment &env,  // exception reporting
                               const TAO_Call_Data_Skel *info, // call description
                               ...)                       // ... any parameters
{
  // first find out the size of the list to be created. info->count keeps track
  // of the table size. If "roundtrip" is true => one entry is for RETURN type
  // which does not go into the NVList
  CORBA::ULong list_size = info->is_roundtrip ? (info->param_count - 1)
                                              : info->param_count;
  CORBA::NVList_ptr nvlist;

  // create an NVList of the appropriate size
  this->orb ()->create_list (list_size, nvlist);

  // Now, put all "in" and "inout" parameters into the NVList
  CORBA::ULong i;

  // setup the variable argument list
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
          // populate the NVList
          (void) nvlist->add_item (0, pdp->mode, env)
            ->value ()->replace (pdp->tc, ptr, pdp->own, env);
        }
      else if (pdp->mode == CORBA::ARG_OUT)
        {
          (void) nvlist->add_item (0, pdp->mode, env);
          // don't add any value
        }
    }
  va_end (param_vector);

  // now demarshal the parameters using a call to params
  this->params (nvlist, env); // nvlist is now owned by us
}

// Extension
void
IIOP_ServerRequest::marshal (CORBA::Environment &env,  // exception reporting
                             const TAO_Call_Data_Skel *info, // call description
                             ...)                       // ... any parameters
{
  // Now, put all "in" and "inout" parameters into the NVList
  CORBA::ULong i, j;

  // setup the variable argument list
  const TAO_Param_Data_Skel *pdp;
  va_list param_vector;
  va_start (param_vector, info);

  j = 0;
  for (i = 0, pdp = info->params;
       i < info->param_count;
       i++, pdp++)
    {
      void *ptr = va_arg (param_vector, void *);

      if (pdp->mode == 0) // return type
        {
          this->retval_ = new CORBA::Any (pdp->tc, ptr, pdp->own);
          continue;
        }

      if (pdp->mode == CORBA::ARG_OUT)
        {
          (void) this->params_->item (j, env)->value ()
            ->replace (pdp->tc, ptr, pdp->own, env);
          // don't add any value
        }
      j++;
    }
  va_end (param_vector);


  // setup a Reply message
  this->init_reply (env);

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
  // construct a REPLY header
  TAO_GIOP::start_message (TAO_GIOP_Reply, *this->outgoing_);
  TAO_GIOP_ServiceContextList resp_ctx;
  resp_ctx.length (0);
  this->outgoing_->encode (&TC_ServiceContextList, &resp_ctx, 0, env);
  this->outgoing_->put_ulong (this->reqid_);

  // Standard exceptions only.
  if (env.exception () != 0)
    {
      CORBA::Environment env2;
      CORBA::Exception *x = env.exception ();
      CORBA::TypeCode_ptr except_tc = x->_type ();

      this->outgoing_->put_ulong (TAO_GIOP_SYSTEM_EXCEPTION);
      (void) this->outgoing_->encode (except_tc, x, 0, env2);
    }

  // Any exception at all.
  else if (this->exception_)
    {
      CORBA::Exception *x;
      CORBA::TypeCode_ptr except_tc;

      x = (CORBA::Exception *) this->exception_->value ();
      except_tc = this->exception_->type ();

      // Finish the GIOP Reply header, then marshal the exception.
      //
      // XXX x->type () someday ...
      if (this->ex_type_ == CORBA::SYSTEM_EXCEPTION)
        this->outgoing_->put_ulong (TAO_GIOP_SYSTEM_EXCEPTION);
      else
        this->outgoing_->put_ulong (TAO_GIOP_USER_EXCEPTION);

      (void) this->outgoing_->encode (except_tc, x, 0, env);
    }
  else
    {  // Normal reply

      // First finish the GIOP header ...
      this->outgoing_->put_ulong (TAO_GIOP_NO_EXCEPTION);
    }
}
