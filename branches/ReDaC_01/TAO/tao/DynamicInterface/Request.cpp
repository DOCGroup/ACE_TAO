// $Id$

#include "tao/DynamicInterface/Request.h"

ACE_RCSID (DynamicInterface,
           Request,
           "$Id$")

#include "tao/DynamicInterface/DII_Invocation_Adapter.h"
#include "tao/DynamicInterface/DII_Arguments.h"
#include "tao/DynamicInterface/Context.h"

#include "tao/AnyTypeCode/NVList.h"
#include "tao/Object.h"
#include "tao/Pluggable_Messaging_Utils.h"
#include "tao/AnyTypeCode/Any_Unknown_IDL_Type.h"

#include "ace/Log_Msg.h"
#include "ace/OS_NS_string.h"

#if !defined (__ACE_INLINE__)
# include "tao/DynamicInterface/Request.inl"
#endif /* ! __ACE_INLINE__ */


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Reference counting for DII Request object.

CORBA::ULong
CORBA::Request::_incr_refcnt (void)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                    ace_mon,
                    this->lock_,
                    0);

  return this->refcount_++;
}

CORBA::ULong
CORBA::Request::_decr_refcnt (void)
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

// DII Request class implementation

CORBA::Request::Request (CORBA::Object_ptr obj,
                         CORBA::ORB_ptr orb,
                         const CORBA::Char *op,
                         CORBA::NVList_ptr args,
                         CORBA::NamedValue_ptr result,
                         CORBA::Flags flags,
                         CORBA::ExceptionList_ptr exceptions
                         ACE_ENV_ARG_DECL_NOT_USED)
  : target_ (CORBA::Object::_duplicate (obj)),
    orb_ (CORBA::ORB::_duplicate (orb)),
    opname_ (CORBA::string_dup (op)),
    args_ (CORBA::NVList::_duplicate (args)),
    result_ (CORBA::NamedValue::_duplicate (result)),
    flags_ (flags),
    // env_ (env),
    exceptions_ (CORBA::ExceptionList::_duplicate (exceptions)),
    contexts_ (0),
    ctx_ (CORBA::Context::_nil ()),
    refcount_ (1),
    lazy_evaluation_ (false),
    response_received_ (0),
    byte_order_ (TAO_ENCAP_BYTE_ORDER)
{
  if (this->exceptions_.in () == 0)
    {
      CORBA::ExceptionList *tmp = 0;
      ACE_NEW (tmp,
               CORBA::ExceptionList);

      this->exceptions_ = tmp;
    }
}

CORBA::Request::Request (CORBA::Object_ptr obj,
                         CORBA::ORB_ptr orb,
                         const CORBA::Char *op
                         ACE_ENV_ARG_DECL_NOT_USED)
  : target_ (CORBA::Object::_duplicate (obj)),
    orb_ (CORBA::ORB::_duplicate (orb)),
    opname_ (CORBA::string_dup (op)),
    flags_ (0),
    // env_ (env),
    contexts_ (0),
    ctx_ (CORBA::Context::_nil ()),
    refcount_ (1),
    lazy_evaluation_ (false),
    response_received_ (0),
    byte_order_ (TAO_ENCAP_BYTE_ORDER)
{
  CORBA::ExceptionList *tmp = 0;
  ACE_NEW (tmp,
           CORBA::ExceptionList);

  this->exceptions_ = tmp;

  ACE_NEW (this->args_,
           CORBA::NVList);

  ACE_NEW (this->result_,
           CORBA::NamedValue);
}

CORBA::Request::~Request (void)
{
  ACE_ASSERT (refcount_ == 0);

  ::CORBA::release (this->target_);
  CORBA::string_free ((char*) this->opname_);
  this->opname_ = 0;
  ::CORBA::release (this->args_);
  ::CORBA::release (this->result_);
}

// The public DII interfaces:  normal and oneway calls.
//
// NOTE that using DII, programmers can get the special behaviour of
// discarding the response for normal calls.  This doesn't change the
// semantics of any OMG-IDL interface, it just streamlines control
// flow in some exotic situations.

void
CORBA::Request::invoke (ACE_ENV_SINGLE_ARG_DECL)
{
  const CORBA::Boolean argument_flag =
    this->args_->_lazy_has_arguments ();

  TAO::NamedValue_Argument _tao_retval (this->result_);


  TAO::NVList_Argument _tao_in_list (this->args_,
                                     this->lazy_evaluation_);

  TAO::Argument *_tao_arg_list [] = {
    &_tao_retval,
    &_tao_in_list
  };

  int number_args = 0;

  if (argument_flag)
    number_args = 2;
  else
    number_args = 1;

  TAO::DII_Invocation_Adapter _tao_call (
       this->target_,
       _tao_arg_list,
       number_args,
       this->opname_,
       static_cast<CORBA::ULong> (ACE_OS::strlen (this->opname_)),
       this->exceptions_.in (),
       this);

  _tao_call.invoke (0,
                    0
                    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // If this request was created by a gateway, then result_
  // and/or args_ are shared by a CORBA::ServerRequest, whose
  // reply must be in the same byte order as the reply we are
  // handling here. So we set the member to be accessed later.
  this->byte_order_ = _tao_retval.byte_order ();
}

void
CORBA::Request::send_oneway (ACE_ENV_SINGLE_ARG_DECL)
{
  const CORBA::Boolean argument_flag =
    this->args_->_lazy_has_arguments ();

  TAO::NamedValue_Argument _tao_retval (this->result_);


  TAO::NVList_Argument _tao_in_list (this->args_,
                                     this->lazy_evaluation_);

  TAO::Argument *_tao_arg_list [] = {
    &_tao_retval,
    &_tao_in_list
  };

  int number_args = 0;

  if (argument_flag)
    number_args = 2;
  else
    number_args = 1;

  TAO::Invocation_Adapter _tao_call (
      this->target_,
      _tao_arg_list,
      number_args,
      this->opname_,
      static_cast<CORBA::ULong> (ACE_OS::strlen (this->opname_)),
      0,
      TAO::TAO_ONEWAY_INVOCATION,
      TAO::TAO_SYNCHRONOUS_INVOCATION,
      true); // is_dii_request

  _tao_call.invoke (0,
                    0
                    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
CORBA::Request::send_deferred (ACE_ENV_SINGLE_ARG_DECL)
{
  {
    ACE_GUARD (TAO_SYNCH_MUTEX,
               ace_mon,
               this->lock_);

    this->response_received_ = 0;
  }

  const CORBA::Boolean argument_flag = this->args_->count () ? 1 : 0;

  TAO::NamedValue_Argument _tao_retval (this->result_);

  TAO::NVList_Argument _tao_in_list (this->args_,
                                     this->lazy_evaluation_);

  TAO::Argument *_tao_arg_list [] = {
    &_tao_retval,
    &_tao_in_list
  };

  int number_args = 0;

  if (argument_flag)
    number_args = 2;
  else
    number_args = 1;

  TAO::DII_Deferred_Invocation_Adapter _tao_call (
      this->target_,
      _tao_arg_list,
      number_args,
      this->opname_,
      static_cast<CORBA::ULong> (ACE_OS::strlen (this->opname_)),
      0,
      this->orb_->orb_core (),
      this);

  _tao_call.invoke (0,
                    0
                    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
CORBA::Request::get_response (ACE_ENV_SINGLE_ARG_DECL)
{
  while (!this->response_received_)
    {
      (void) this->orb_->perform_work (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
    }

  if (this->lazy_evaluation_)
    {
      this->args_->evaluate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
    }
}

CORBA::Boolean
CORBA::Request::poll_response (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                    ace_mon,
                    this->lock_,
                    0);

  if (!this->response_received_)
    {
      // If we're single-threaded, the application could starve the ORB,
      // and the response never gets received, so let the ORB do an
      // atom of work, if necessary, each time we poll.
      ACE_Time_Value tv (0, 0);
      (void) this->orb_->perform_work (&tv ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);
    }

  return this->response_received_;
}

void
CORBA::Request::handle_response (TAO_InputCDR &incoming,
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
          // We can be sure that the impl is a TAO::Unknown_IDL_Type.
          this->result_->value ()->impl ()->_tao_decode (incoming
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
    case TAO_PLUGGABLE_MESSAGE_LOCATION_FORWARD_PERM:
    default:
      // @@ (JP) Don't know what to do about any of these yet.
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) unhandled reply status\n")));
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
