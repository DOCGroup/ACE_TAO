// $Id$


#include "Request.h"

ACE_RCSID(DynamicInterface, Request, "$Id$")


#include "ExceptionList.h"
#include "DII_Invocation.h"
#include "tao/Object.h"
#include "tao/Pluggable_Messaging_Utils.h"

#if !defined (__ACE_INLINE__)
# include "Request.inl"
#endif /* ! __ACE_INLINE__ */

  // Reference counting for DII Request object.

  CORBA::ULong
CORBA_Request::_incr_refcnt (void)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                    ace_mon,
                    this->lock_,
                    0);

  return this->refcount_++;
}

CORBA::ULong
CORBA_Request::_decr_refcnt (void)
{
  {
    ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                      ace_mon,
                      this->lock_,
                      0);

    this->refcount_--;

    if (this->refcount_ != 0)
      {
        return this->refcount_;
      }
  }

  delete this;
  return 0;
}

// The pseudo-object _nil method.
CORBA_Request_ptr
CORBA_Request::_nil (void)
{
  return (CORBA_Request_ptr)0;
}

// DII Request class implementation

CORBA_Request::CORBA_Request (CORBA::Object_ptr obj,
                              CORBA::ORB_ptr orb,
                              const CORBA::Char *op,
                              CORBA::NVList_ptr args,
                              CORBA::NamedValue_ptr result,
                              CORBA::Flags flags,
                              CORBA::ExceptionList_ptr exceptions
                              ACE_ENV_ARG_DECL_NOT_USED)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    args_ (CORBA::NVList::_duplicate (args)),
    result_ (CORBA::NamedValue::_duplicate (result)),
    flags_ (flags),
    // env_ (env),
    exceptions_ (CORBA::ExceptionList::_duplicate (exceptions)),
    contexts_ (0),
    ctx_ (CORBA::Context::_nil ()),
    refcount_ (1),
    lazy_evaluation_ (0),
    response_received_ (0),
    byte_order_ (TAO_ENCAP_BYTE_ORDER)
{
  this->target_ = CORBA::Object::_duplicate (obj);
  this->opname_ = CORBA::string_dup (op);

  if (this->exceptions_.in () == 0)
    {
      CORBA::ExceptionList *tmp = 0;
      ACE_NEW (tmp,
               CORBA::ExceptionList);

      this->exceptions_ = tmp;
    }
}

CORBA_Request::CORBA_Request (CORBA::Object_ptr obj,
                              CORBA::ORB_ptr orb,
                              const CORBA::Char *op
                              ACE_ENV_ARG_DECL_NOT_USED)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    flags_ (0),
    // env_ (env),
    contexts_ (0),
    ctx_ (CORBA::Context::_nil ()),
    refcount_ (1),
    lazy_evaluation_ (0),
    response_received_ (0),
    byte_order_ (TAO_ENCAP_BYTE_ORDER)
{
  this->target_ = CORBA::Object::_duplicate (obj);
  this->opname_ = CORBA::string_dup (op);

  CORBA::ExceptionList *tmp = 0;
  ACE_NEW (tmp,
           CORBA::ExceptionList);

  this->exceptions_ = tmp;

  ACE_NEW (this->args_,
           CORBA::NVList);

  ACE_NEW (this->result_,
           CORBA::NamedValue);
}

CORBA_Request::~CORBA_Request (void)
{
  ACE_ASSERT (refcount_ == 0);

  CORBA::release (this->target_);
  CORBA::string_free ((char*) this->opname_);
  this->opname_ = 0;
  CORBA::release (this->args_);
  CORBA::release (this->result_);
}

// The public DII interfaces:  normal and oneway calls.
//
// NOTE that using DII, programmers can get the special behaviour of
// discarding the response for normal calls.  This doesn't change the
// semantics of any OMG-IDL interface, it just streamlines control
// flow in some exotic situations.

void
CORBA_Request::invoke (ACE_ENV_SINGLE_ARG_DECL)
{
  CORBA::Boolean argument_flag = this->args_->_lazy_has_arguments ();

  TAO_GIOP_DII_Invocation call (this->target_->_stubobj (),
                                this->opname_,
                                ACE_OS::strlen (this->opname_),
                                argument_flag,
                                this->orb_->orb_core (),
                                this,
                                this->byte_order_);

  // Loop as needed for forwarding.
  for (;;)
    {
      call.start (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      CORBA::Short flag = TAO_TWOWAY_RESPONSE_FLAG;

      call.prepare_header (ACE_static_cast (CORBA::Octet,
                                            flag)
                           ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      this->args_->_tao_encode (call.out_stream (),
                                this->orb_->orb_core (),
                                CORBA::ARG_IN | CORBA::ARG_INOUT
                                ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      // Make the call ... blocking for the response.
      int status = call.invoke (this->exceptions_.in ()
                                ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      if (status == TAO_INVOKE_RESTART)
        {
          continue;
        }

      if (status == TAO_INVOKE_EXCEPTION)
        {
          // Shouldn't happen.
          return;
        }

      if (status != TAO_INVOKE_OK)
        {
          ACE_THROW (CORBA::UNKNOWN (TAO_DEFAULT_MINOR_CODE,
                                     CORBA::COMPLETED_MAYBE));
        }

      // The only case left is status == TAO_INVOKE_OK, exit the
      // loop.  We cannot retry because at this point we either
      // got a reply or something with an status of
      // COMPLETED_MAYBE, thus we cannot reissue the request if we
      // are to satisfy the "at most once" semantics.
      break;
    }

  // If this request was created by a gateway, then result_
  // and/or args_ are shared by a CORBA::ServerRequest, whose
  // reply must be in the same byte order as the reply we are
  // handling here. So we set the member to be accessed later.
  this->byte_order_ = call.inp_stream ().byte_order ();

  // Now, get all the "return", "out", and "inout" parameters
  // from the response message body ... return parameter is
  // first, the rest are in the order defined in the IDL spec
  // (which is also the order that DII users are required to
  // use).

  if (this->result_ != 0)
    {
      this->result_->value ()->_tao_decode (call.inp_stream ()
                                            ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }

  this->args_->_tao_incoming_cdr (call.inp_stream (),
                                  CORBA::ARG_OUT | CORBA::ARG_INOUT,
                                  this->lazy_evaluation_
                                  ACE_ENV_ARG_PARAMETER);
}

void
CORBA_Request::send_oneway (ACE_ENV_SINGLE_ARG_DECL)
{
  CORBA::Boolean argument_flag = this->args_->_lazy_has_arguments ();

  TAO_GIOP_Oneway_Invocation call (this->target_->_stubobj (),
                                   this->opname_,
                                   ACE_OS::strlen (this->opname_),
                                   argument_flag,
                                   this->orb_->orb_core (),
                                   this->byte_order_);

  // Loop as needed for forwarding.
  for (;;)
    {
      call.start (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      CORBA::Octet response_flag = ACE_static_cast (CORBA::Octet,
                                                    call.sync_scope ());

      call.prepare_header (response_flag
                           ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      this->args_->_tao_encode (call.out_stream (),
                                this->orb_->orb_core (),
                                CORBA::ARG_IN | CORBA::ARG_INOUT
                                ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      int status = call.invoke (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      if (status == TAO_INVOKE_RESTART)
        {
          continue;
        }

      if (status == TAO_INVOKE_EXCEPTION)
        {
          // Shouldn't happen.
          return;
        }

      if (status != TAO_INVOKE_OK)
        {
          ACE_THROW (CORBA::UNKNOWN (TAO_DEFAULT_MINOR_CODE,
                                     CORBA::COMPLETED_MAYBE));
        }

      // The only case left is status == TAO_INVOKE_OK, exit the
      // loop.  We cannot retry because at this point we either
      // got a reply or something with an status of
      // COMPLETED_MAYBE, thus we cannot reissue the request if we
      // are to satisfy the "at most once" semantics.
      break;
    }
}

void
CORBA_Request::send_deferred (ACE_ENV_SINGLE_ARG_DECL)
{
  {
    ACE_GUARD (TAO_SYNCH_MUTEX,
               ace_mon,
               this->lock_);

    this->response_received_ = 0;
  }

  CORBA::Boolean argument_flag = this->args_->count () ? 1: 0;

  TAO_GIOP_DII_Deferred_Invocation call (this->target_->_stubobj (),
                                         this->orb_->orb_core (),
                                         argument_flag,
                                         this,
                                         this->byte_order_);

  // Loop as needed for forwarding.
  for (;;)
    {
      call.start (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      CORBA::Short flag = TAO_TWOWAY_RESPONSE_FLAG;

      call.prepare_header (ACE_static_cast (CORBA::Octet, flag)
                           ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      this->args_->_tao_encode (call.out_stream (),
                                this->orb_->orb_core (),
                                CORBA::ARG_IN | CORBA::ARG_INOUT
                                ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      // Make the call without blocking.
      CORBA::ULong status = call.invoke (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      if (status == TAO_INVOKE_RESTART)
        {
          continue;
        }

      if (status != TAO_INVOKE_OK)
        {
          ACE_THROW (CORBA::UNKNOWN (TAO_DEFAULT_MINOR_CODE,
                                     CORBA::COMPLETED_MAYBE));
        }

      // The only case left is status == TAO_INVOKE_OK, exit the
      // loop.  We cannot retry because at this point we either
      // got a reply or something with an status of
      // COMPLETED_MAYBE, thus we cannot reissue the request if we
      // are to satisfy the "at most once" semantics.
      break;
    }
}

void
CORBA_Request::get_response (ACE_ENV_SINGLE_ARG_DECL)
{
  while (!this->response_received_)
    {
      (void) this->orb_->perform_work ();
    }

  if (this->lazy_evaluation_)
    {
      this->args_->evaluate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
    }
}

CORBA::Boolean
CORBA_Request::poll_response (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                    ace_mon,
                    this->lock_,
                    0);

  return this->response_received_;
}

void
CORBA_Request::handle_response (TAO_InputCDR &incoming,
                                CORBA::ULong reply_status
                                ACE_ENV_ARG_DECL)
{
  // If this request was created by a gateway, then result_
  // and/or args_ are shared by a CORBA::ServerRequest, whose
  // reply must be in the same byte order as the reply we are
  // handling here. So we set the member to be accessed later.
  this->byte_order_ = incoming.byte_order ();

  switch (reply_status)
  {
    case TAO_PLUGGABLE_MESSAGE_NO_EXCEPTION:
      if (this->result_ != 0)
        {
          this->result_->value ()->_tao_decode (incoming
                                                ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;
        }

      this->args_->_tao_incoming_cdr (incoming,
                                      CORBA::ARG_OUT | CORBA::ARG_INOUT,
                                      this->lazy_evaluation_
                                      ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      {
        ACE_GUARD (TAO_SYNCH_MUTEX,
                   ace_mon,
                   this->lock_);

        this->response_received_ = 1;
      }

      break;
    case TAO_PLUGGABLE_MESSAGE_USER_EXCEPTION:
    case TAO_PLUGGABLE_MESSAGE_SYSTEM_EXCEPTION:
    case TAO_PLUGGABLE_MESSAGE_LOCATION_FORWARD:
    default:
      // @@ (JP) Don't know what to do about any of these yet.
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) unhandled reply status\n")));
  }
}

// Constructor.
CORBA_ORB_RequestSeq::CORBA_ORB_RequestSeq (CORBA::ULong max)
  : TAO_Unbounded_Pseudo_Sequence <CORBA_Request,CORBA_Request_var> (max)
{
  // No-op.
}

CORBA_ORB_RequestSeq::CORBA_ORB_RequestSeq (const CORBA_ORB_RequestSeq &rhs)
  : TAO_Unbounded_Pseudo_Sequence <CORBA_Request,CORBA_Request_var> (rhs)
{
  // No-op.
}

CORBA_ORB_RequestSeq::CORBA_ORB_RequestSeq (CORBA::ULong max,
                                            CORBA::ULong length,
                                            CORBA_Request **data,
                                            CORBA::Boolean release)
  : TAO_Unbounded_Pseudo_Sequence <CORBA_Request,CORBA_Request_var> (max,
                                                                     length,
                                                                     data,
                                                                     release)
{
  // No-op.
}


CORBA_ORB_RequestSeq::CORBA_ORB_RequestSeq (void)
{
  // No-op.
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class TAO_Unbounded_Pseudo_Sequence<CORBA_Request,CORBA_Request_var>;
template class TAO_Pseudo_Object_Manager<CORBA_Request,CORBA_Request_var>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate TAO_Unbounded_Pseudo_Sequence<CORBA_Request,CORBA_Request_var>
#pragma instantiate TAO_Pseudo_Object_Manager<CORBA_Request,CORBA_Request_var>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
