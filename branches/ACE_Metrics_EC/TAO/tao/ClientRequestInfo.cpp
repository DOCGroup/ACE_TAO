// -*- C++ -*-
#include "ClientRequestInfo.h"
#include "ClientRequestInfo_i.h"

ACE_RCSID (TAO,
           ClientRequestInfo,
           "$Id$")

#if TAO_HAS_INTERCEPTORS == 1

# if !defined (__ACE_INLINE__)
#   include "ClientRequestInfo.inl"
# endif /* !__ACE_INLINE__ */

TAO_ClientRequestInfo::TAO_ClientRequestInfo (void)
  : info_ (0)
{
}

CORBA::Object_ptr
TAO_ClientRequestInfo::target (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->check_validity (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  return this->info_->target (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::Object_ptr
TAO_ClientRequestInfo::effective_target (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->check_validity (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  return this->info_->effective_target (ACE_ENV_SINGLE_ARG_PARAMETER);
}

IOP::TaggedProfile *
TAO_ClientRequestInfo::effective_profile (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->check_validity (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->info_->effective_profile (ACE_ENV_SINGLE_ARG_PARAMETER);
}

// Use at own risk. There is no way currently of extracting an
// exception from an Any. This method is in place just to be compliant
// with the spec.
CORBA::Any *
TAO_ClientRequestInfo::received_exception (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->check_validity (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->info_->received_exception (ACE_ENV_SINGLE_ARG_PARAMETER);
}

char *
TAO_ClientRequestInfo::received_exception_id (
    ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->check_validity (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->info_->received_exception_id (ACE_ENV_SINGLE_ARG_PARAMETER);
}

IOP::TaggedComponent *
TAO_ClientRequestInfo::get_effective_component (
    IOP::ComponentId id
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->check_validity (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->info_->get_effective_component (id
                                               ACE_ENV_ARG_PARAMETER);
}

IOP::TaggedComponentSeq *
TAO_ClientRequestInfo::get_effective_components (
    IOP::ComponentId id
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->check_validity (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->info_->get_effective_components (id
                                                ACE_ENV_ARG_PARAMETER);
}

CORBA::Policy_ptr
TAO_ClientRequestInfo::get_request_policy (CORBA::PolicyType type
                                           ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->check_validity (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  return this->info_->get_request_policy (type
                                          ACE_ENV_ARG_PARAMETER);
}

void
TAO_ClientRequestInfo::add_request_service_context (
    const IOP::ServiceContext & service_context,
    CORBA::Boolean replace
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->check_validity (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->info_->add_request_service_context (service_context,
                                            replace
                                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

CORBA::ULong
TAO_ClientRequestInfo::request_id (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->check_validity (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->info_->request_id (ACE_ENV_SINGLE_ARG_PARAMETER);
}

char *
TAO_ClientRequestInfo::operation (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->check_validity (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->info_->operation (ACE_ENV_SINGLE_ARG_PARAMETER);
}

Dynamic::ParameterList *
TAO_ClientRequestInfo::arguments (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->check_validity (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->info_->arguments (ACE_ENV_SINGLE_ARG_PARAMETER);
}

Dynamic::ExceptionList *
TAO_ClientRequestInfo::exceptions (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->check_validity (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->info_->exceptions (ACE_ENV_SINGLE_ARG_PARAMETER);
}

Dynamic::ContextList *
TAO_ClientRequestInfo::contexts (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->check_validity (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->info_->contexts (ACE_ENV_SINGLE_ARG_PARAMETER);
}

Dynamic::RequestContext *
TAO_ClientRequestInfo::operation_context (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->check_validity (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->info_->operation_context (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::Any *
TAO_ClientRequestInfo::result (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->check_validity (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->info_->result (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::Boolean
TAO_ClientRequestInfo::response_expected (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->check_validity (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->info_->response_expected (ACE_ENV_SINGLE_ARG_PARAMETER);
}

Messaging::SyncScope
TAO_ClientRequestInfo::sync_scope (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->check_validity (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->info_->sync_scope (ACE_ENV_SINGLE_ARG_PARAMETER);
}

PortableInterceptor::ReplyStatus
TAO_ClientRequestInfo::reply_status (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->check_validity (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (PortableInterceptor::SYSTEM_EXCEPTION);

  return this->info_->reply_status (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::Object_ptr
TAO_ClientRequestInfo::forward_reference (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->check_validity (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  return this->info_->forward_reference (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::Any *
TAO_ClientRequestInfo::get_slot (PortableInterceptor::SlotId id
                                 ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::InvalidSlot))
{
  this->check_validity (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->info_->get_slot (id
                                ACE_ENV_ARG_PARAMETER);
}

IOP::ServiceContext *
TAO_ClientRequestInfo::get_request_service_context (
    IOP::ServiceId id
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->check_validity (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->info_->get_request_service_context (id
                                                   ACE_ENV_ARG_PARAMETER);
}

IOP::ServiceContext *
TAO_ClientRequestInfo::get_reply_service_context (
    IOP::ServiceId id
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->check_validity (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->info_->get_reply_service_context (id
                                                 ACE_ENV_ARG_PARAMETER);
}


// -------------------------------------------------------------------


TAO_ClientRequestInfo_Guard::~TAO_ClientRequestInfo_Guard (void)
{
  // Restore the previous request information into the TSS object.
  // Note that no TSS access occurs here.
  if (this->info_ != 0)
    this->info_->info (this->previous_info_);
}


#endif /* TAO_HAS_INTERCEPTORS == 1 */
