// $Id$

#include "Request.h"

ACE_RCSID(DynamicInterface, Request, "$Id$")

#if (TAO_HAS_MINIMUM_CORBA == 0)

#include "ExceptionList.h"
#include "DII_Invocation.h"
#include "tao/Object.h"

#if !defined (__ACE_INLINE__)
# include "Request.inl"
#endif /* ! __ACE_INLINE__ */

// Reference counting for DII Request object.

CORBA::ULong
CORBA_Request::_incr_refcnt (void)
{
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX,
                    ace_mon,
                    this->lock_,
                    0);

  return this->refcount_++;
}

CORBA::ULong
CORBA_Request::_decr_refcnt (void)
{
  {
    ACE_GUARD_RETURN (ACE_SYNCH_MUTEX,
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
                              CORBA::ExceptionList_ptr exceptions,
                              CORBA::Environment &ACE_TRY_ENV)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    args_ (CORBA::NVList::_duplicate (args)),
    result_ (CORBA::NamedValue::_duplicate (result)),
    flags_ (flags),
    env_ (ACE_TRY_ENV),
    exceptions_ (CORBA::ExceptionList::_duplicate (exceptions)),
    contexts_ (0),
    ctx_ (CORBA::Context::_nil ()),
    refcount_ (1),
    lazy_evaluation_ (0),
    response_received_ (0)
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
                              const CORBA::Char *op,
                              CORBA::Environment &ACE_TRY_ENV)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    flags_ (0),
    env_ (ACE_TRY_ENV),
    contexts_ (0),
    ctx_ (CORBA::Context::_nil ()),
    refcount_ (1),
    lazy_evaluation_ (0),
    response_received_ (0)
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
CORBA_Request::invoke (CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::Boolean argument_flag = this->args_->_lazy_has_arguments ();

  TAO_GIOP_DII_Invocation call (this->target_->_stubobj (),
                                this->opname_,
                                ACE_OS::strlen (this->opname_),
                                argument_flag,
                                this->orb_->orb_core ());

  // Loop as needed for forwarding.
  for (;;)
    {
      call.start (ACE_TRY_ENV);
      ACE_CHECK;

      CORBA::Short flag = TAO_TWOWAY_RESPONSE_FLAG;

      call.prepare_header (ACE_static_cast (CORBA::Octet,
                                            flag),
                           ACE_TRY_ENV);
      ACE_CHECK;

      this->args_->_tao_encode (call.out_stream (),
                                this->orb_->orb_core (),
                                CORBA::ARG_IN | CORBA::ARG_INOUT,
                                ACE_TRY_ENV);
      ACE_CHECK;

      // Make the call ... blocking for the response.
      int status = call.invoke (this->exceptions_.in (),
                                ACE_TRY_ENV);
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

  // Now, get all the "return", "out", and "inout" parameters
  // from the response message body ... return parameter is
  // first, the rest are in the order defined in the IDL spec
  // (which is also the order that DII users are required to
  // use).

  if (this->result_ != 0)
    {
      this->result_->value ()->_tao_decode (call.inp_stream (),
                                            ACE_TRY_ENV);
      ACE_CHECK;
    }

  this->args_->_tao_incoming_cdr (call.inp_stream (),
                                  CORBA::ARG_OUT | CORBA::ARG_INOUT,
                                  this->lazy_evaluation_,
                                  ACE_TRY_ENV);
}

void
CORBA_Request::send_oneway (CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::Boolean argument_flag = this->args_->_lazy_has_arguments ();

  TAO_GIOP_Oneway_Invocation call (this->target_->_stubobj (),
                                   this->opname_,
                                   ACE_OS::strlen (this->opname_),
                                   argument_flag,
                                   this->orb_->orb_core ());

  // Loop as needed for forwarding.
  for (;;)
    {
      call.start (ACE_TRY_ENV);
      ACE_CHECK;

      CORBA::Octet response_flag = ACE_static_cast (CORBA::Octet,
                                                    call.sync_scope ());

      call.prepare_header (response_flag,
                           ACE_TRY_ENV);
      ACE_CHECK;

      this->args_->_tao_encode (call.out_stream (),
                                this->orb_->orb_core (),
                                CORBA::ARG_IN | CORBA::ARG_INOUT,
                                ACE_TRY_ENV);
      ACE_CHECK;

      int status = call.invoke (ACE_TRY_ENV);
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
CORBA_Request::send_deferred (CORBA::Environment &ACE_TRY_ENV)
{
  {
    ACE_GUARD (ACE_SYNCH_MUTEX,
               ace_mon,
               this->lock_);

    this->response_received_ = 0;
  }

  CORBA::Boolean argument_flag = this->args_->count () ? 1: 0;

  TAO_GIOP_DII_Deferred_Invocation call (this->target_->_stubobj (),
                                         this->orb_->orb_core (),
                                         argument_flag,
                                         this);

  // Loop as needed for forwarding.
  for (;;)
    {
      call.start (ACE_TRY_ENV);
      ACE_CHECK;

      CORBA::Short flag = TAO_TWOWAY_RESPONSE_FLAG;

      call.prepare_header (ACE_static_cast (CORBA::Octet, flag),
                           ACE_TRY_ENV);
      ACE_CHECK;

      this->args_->_tao_encode (call.out_stream (),
                                this->orb_->orb_core (),
                                CORBA::ARG_IN | CORBA::ARG_INOUT,
                                ACE_TRY_ENV);
      ACE_CHECK;

      // Make the call without blocking.
      CORBA::ULong status = call.invoke (ACE_TRY_ENV);
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
CORBA_Request::get_response (CORBA::Environment &ACE_TRY_ENV)
{
  while (!this->response_received_)
    {
      (void) this->orb_->perform_work ();
    }

  if (this->lazy_evaluation_)
    {
      this->args_->evaluate (ACE_TRY_ENV);
      ACE_CHECK;
    }
}

CORBA::Boolean
CORBA_Request::poll_response (CORBA::Environment &)
{
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX,
                    ace_mon,
                    this->lock_,
                    0);

  return this->response_received_;
}

void
CORBA_Request::handle_response (TAO_InputCDR &incoming,
                                CORBA::ULong reply_status,
                                CORBA::Environment &ACE_TRY_ENV)
{
  switch (reply_status)
  {
    case TAO_PLUGGABLE_MESSAGE_NO_EXCEPTION:
      if (this->result_ != 0)
        {
          this->result_->value ()->_tao_decode (incoming,
                                                ACE_TRY_ENV);
          ACE_CHECK;
        }

      this->args_->_tao_incoming_cdr (incoming,
                                      CORBA::ARG_OUT | CORBA::ARG_INOUT,
                                      this->lazy_evaluation_,
                                      ACE_TRY_ENV);
      ACE_CHECK;

      {
        ACE_GUARD (ACE_SYNCH_MUTEX,
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

#endif /* TAO_HAS_MINIMUM_CORBA */
