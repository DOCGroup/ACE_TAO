// -*- C++ -*-
//
// $Id$

#include "ClientRequestInfo.h"

ACE_RCSID(tao, ClientRequestInfo, "$Id$")

#if (TAO_HAS_INTERCEPTORS == 1)

TAO_ClientRequestInfo::TAO_ClientRequestInfo (const char * operation,
                                              IOP::ServiceContextList
                                              &service_context_list,
                                              CORBA::Object_ptr target)
  : operation_ (operation),
    service_context_list_ (service_context_list),
    target_ (CORBA_Object::_duplicate (target))
{
}

CORBA::Object_ptr
TAO_ClientRequestInfo::target (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::Object::_duplicate (this->target_.in ());
}

CORBA::Object_ptr
TAO_ClientRequestInfo::effective_target (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::Object::_nil ();
}

IOP::TaggedProfile *
TAO_ClientRequestInfo::effective_profile (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return 0;
}

// Use at own risk. There is no way currently of extracting an
// exception from an Any. This method is in place just to be compliant
// with the spec.
CORBA::Any *
TAO_ClientRequestInfo::received_exception (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // The spec says that if it is a user exception which can't be inserted
  // then the UNKNOWN exception needs to be thrown with minor code TBD_U.
  this->any_exception_ <<= *this->caught_exception_;
  return &this->any_exception_;
}

// = TAO specific method done since there currently is no simple way
//   to extract exceptions from an Any
CORBA::Exception *
TAO_ClientRequestInfo::_received_exception (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->caught_exception_;
}

char *
TAO_ClientRequestInfo::received_exception_id (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup (this->caught_exception_->_id ());
}

IOP::TaggedComponent *
TAO_ClientRequestInfo::get_effective_component (IOP::ComponentId,
                                                CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return 0;
}

CORBA::Policy_ptr
TAO_ClientRequestInfo::get_request_policy (CORBA::PolicyType type,
                                           CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->target_->_get_policy (type,
                                     ACE_TRY_ENV);
}

void
TAO_ClientRequestInfo::add_request_service_context (
    const IOP::ServiceContext & service_context,
    CORBA::Boolean,
    CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Copy the service context into the list.
  CORBA::ULong length = this->service_context_list_.length ();
  this->service_context_list_.length (length + 1);

  this->service_context_list_[length] = service_context;
}

CORBA::ULong
TAO_ClientRequestInfo::request_id (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->request_id_;
}

char *
TAO_ClientRequestInfo::operation (CORBA::Environment &)
   ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup (this->operation_);

}

Dynamic::ParameterList *
TAO_ClientRequestInfo::arguments (CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
 return &this->parameter_list_;
}

Dynamic::ExceptionList *
TAO_ClientRequestInfo::exceptions (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return &this->exception_list_;
}

Dynamic::ContextList *
TAO_ClientRequestInfo::contexts (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
 return &this->context_list_;
}

Dynamic::RequestContext *
TAO_ClientRequestInfo::operation_context (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
 return &this->request_context_;
}

CORBA::Any *
TAO_ClientRequestInfo::result (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return 0;
}

CORBA::Boolean
TAO_ClientRequestInfo::response_expected (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return 0;
}

# if (TAO_HAS_CORBA_MESSAGING == 1)
CORBA::Short
TAO_ClientRequestInfo::sync_scope (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
 return 0;
}
#endif  /* TAO_HAS_CORBA_MESSAGING */

PortableInterceptor::ReplyStatus
TAO_ClientRequestInfo::reply_status (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return 0;
}

CORBA::Object_ptr
TAO_ClientRequestInfo::forward_reference (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return 0;
}

CORBA::Any *
TAO_ClientRequestInfo::get_slot (PortableInterceptor::SlotId,
                                 CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::InvalidSlot))
{
  return 0;
}

IOP::ServiceContext *
TAO_ClientRequestInfo::get_request_service_context (IOP::ServiceId id,
                                                    CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  for (CORBA::ULong size = 0;
       size < this->service_context_list_.length ();
       ++size)
    if (this->service_context_list_[size].context_id == id)
      {
         return &(this->service_context_list_[size]);
      }

  return 0;
}

IOP::ServiceContext *
TAO_ClientRequestInfo::get_reply_service_context (IOP::ServiceId id,
                                                  CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  for (CORBA::ULong size = 0;
       size < this->service_context_list_.length ();
       ++size)
    if (this->service_context_list_[size].context_id == id)
      {
        // Will it will get safely destroyed at the
        // TAO_Client side when the service context gets back.
        return &this->service_context_list_[size];
      }

  return 0;
}

void
TAO_ClientRequestInfo::exception (CORBA::Exception *exception)
{
  this->caught_exception_ = exception;
}

void
TAO_ClientRequestInfo::request_id (CORBA::ULong request_id)
{
  this->request_id_ = request_id;
}

#endif /* TAO_HAS_INTERCEPTORS == 1 */
