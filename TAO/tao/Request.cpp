// $Id$

#include "tao/Request.h"

#if !defined (TAO_HAS_MINIMUM_CORBA)

#include "tao/Object.h"
#include "tao/Stub.h"
#include "tao/Pluggable_Messaging_Utils.h"


#if !defined (__ACE_INLINE__)
# include "tao/Request.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(tao, Request, "$Id$")

CORBA::ULong
CORBA_Request::_incr_refcnt (void)
{
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX,
                    ace_mon,
                    this->lock_,
                    0);
  return refcount_++;
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
      return this->refcount_;
  }

  delete this;
  return 0;
}

CORBA_Request*
CORBA_Request::_nil (void)
{
  return 0;
}

// Reference counting for DII Request object

// DII Request class implementation

CORBA_Request::CORBA_Request (CORBA::Object_ptr obj,
                              CORBA::ORB_ptr orb,
                              const CORBA::Char *op,
                              CORBA::NVList_ptr args,
                              CORBA::NamedValue_ptr result,
                              CORBA::Flags flags,
                              CORBA::Environment &ACE_TRY_ENV)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    args_ (CORBA::NVList::_duplicate (args)),
    result_ (CORBA::NamedValue::_duplicate (result)),
    flags_ (flags),
    env_ (ACE_TRY_ENV),
    contexts_ (0),
    ctx_ (0),
    refcount_ (1),
    lazy_evaluation_ (0),
    response_received_ (0)
{
  target_ = CORBA::Object::_duplicate (obj);
  opname_ = CORBA::string_dup (op);
}

CORBA_Request::CORBA_Request (CORBA::Object_ptr obj,
                              CORBA::ORB_ptr orb,
                              const CORBA::Char *op,
                              CORBA::Environment &ACE_TRY_ENV)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    flags_ (0),
    env_ (ACE_TRY_ENV),
    contexts_ (0),
    ctx_ (0),
    refcount_ (1),
    lazy_evaluation_ (0),
    response_received_ (0)
{
  target_ = CORBA::Object::_duplicate (obj);
  opname_ = CORBA::string_dup (op);

  ACE_NEW (args_, CORBA::NVList);
  ACE_NEW (result_, CORBA::NamedValue);
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
  TAO_Stub *stub = this->target_->_stubobj ();

  stub->do_dynamic_call ((char *) this->opname_,
                         1,
                         this->args_,
                         this->result_,
                         this->flags_,
                         this->exceptions_,
                         this->lazy_evaluation_,
                         ACE_TRY_ENV);
}

void
CORBA_Request::send_oneway (CORBA::Environment &ACE_TRY_ENV)
{
  TAO_Stub *stub = this->target_->_stubobj ();

  stub->do_dynamic_call ((char *) opname_,
                         0,
                         this->args_,
                         this->result_,
                         this->flags_,
                         this->exceptions_,
                         this->lazy_evaluation_,
                         ACE_TRY_ENV);
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

  TAO_Stub *stub = this->target_->_stubobj ();

  stub->do_deferred_call (this,
                          ACE_TRY_ENV);
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
                  ASYS_TEXT ("(%P|%t) unhandled reply status\n")));
  }
}

//  constructor.
CORBA_ORB_RequestSeq::CORBA_ORB_RequestSeq (CORBA::ULong max)
  : TAO_Unbounded_Pseudo_Sequence <CORBA_Request,CORBA_Request_var> (max)
{
  // no-op
}

CORBA_ORB_RequestSeq::CORBA_ORB_RequestSeq (const CORBA_ORB_RequestSeq &rhs)
  : TAO_Unbounded_Pseudo_Sequence <CORBA_Request,CORBA_Request_var> (rhs)
{
  // no-op
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
  // no-op
}


CORBA_ORB_RequestSeq::CORBA_ORB_RequestSeq (void)
{
  // no-op
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class TAO_Unbounded_Pseudo_Sequence<CORBA_Request,CORBA_Request_var>;
template class TAO_Pseudo_Object_Manager<CORBA_Request,CORBA_Request_var>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate TAO_Unbounded_Pseudo_Sequence<CORBA_Request,CORBA_Request_var>
#pragma instantiate TAO_Pseudo_Object_Manager<CORBA_Request,CORBA_Request_var>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#endif /* TAO_HAS_MINIMUM_CORBA */
