// -*- C++ -*-
//
// $Id$

#include "ServerRequestInfo.h"

ACE_RCSID (PortableServer, ServerRequestInfo, "$Id$")

#if (TAO_HAS_INTERCEPTORS == 1)

TAO_ServerRequestInfo::TAO_ServerRequestInfo (
      const char * operation,
      IOP::ServiceContextList &service_context_list)
  : operation_ (operation),
    service_context_list_ (service_context_list),
    caught_exception_ (0),
    reply_status_ (-1)
{
}

CORBA::ULong
TAO_ServerRequestInfo::request_id (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->request_id_;
}

char *
TAO_ServerRequestInfo::operation (CORBA::Environment &)
   ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup (this->operation_);
}

Dynamic::ParameterList *
TAO_ServerRequestInfo::arguments (CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
 return &this->parameter_list_;
}

Dynamic::ExceptionList *
TAO_ServerRequestInfo::exceptions (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return &this->exception_list_;
}

Dynamic::ContextList *
TAO_ServerRequestInfo::contexts (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
 return &this->context_list_;
}

Dynamic::RequestContext *
TAO_ServerRequestInfo::operation_context (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
 return &this->request_context_;
}

CORBA::Any *
TAO_ServerRequestInfo::result (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return 0;
}

CORBA::Boolean
TAO_ServerRequestInfo::response_expected (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOTSUP),
                      CORBA::COMPLETED_NO),
                    0);
}

# if TAO_HAS_CORBA_MESSAGING == 1
Messaging::SyncScope
TAO_ServerRequestInfo::sync_scope (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
 return 0;
}
#endif  /* TAO_HAS_CORBA_MESSAGING */

PortableInterceptor::ReplyStatus
TAO_ServerRequestInfo::reply_status (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (this->reply_status_ == -1)
    // A reply hasn't been received yet.
    ACE_THROW_RETURN (CORBA::BAD_INV_ORDER (), -1);

  return this->reply_status_;
}

CORBA::Object_ptr
TAO_ServerRequestInfo::forward_reference (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (this->reply_status_ != PortableInterceptor::LOCATION_FORWARD
      || this->reply_status_ !=
           PortableInterceptor::LOCATION_FORWARD_PERMANENT)
    ACE_THROW_RETURN (CORBA::BAD_INV_ORDER (),
                      CORBA::Object::_nil ());

//   // TAO_GIOP_Invocation::forward_reference() already duplicates the
//   // reference before returning it so there is no need to duplicate it
//   // here.
//   return this->invocation_->forward_reference ();

  return CORBA::Object::_nil ();
}

CORBA::Any *
TAO_ServerRequestInfo::get_slot (PortableInterceptor::SlotId,
                                 CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::InvalidSlot))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOTSUP),
                      CORBA::COMPLETED_NO),
                    0);
}

IOP::ServiceContext *
TAO_ServerRequestInfo::get_request_service_context (
    IOP::ServiceId id,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::ULong len = this->service_context_list_.length ();

  for (CORBA::ULong i = 0; i < len; ++i)
    if (this->service_context_list_[i].context_id == id)
      {
        IOP::ServiceContext *service_context = 0;
        ACE_NEW_THROW_EX (service_context,
                          IOP::ServiceContext,
                          CORBA::NO_MEMORY (
                            CORBA::SystemException::_tao_minor_code (
                              TAO_DEFAULT_MINOR_CODE,
                              ENOMEM),
                            CORBA::COMPLETED_NO));
        ACE_CHECK_RETURN (0);

        IOP::ServiceContext_var safe_service_context = service_context;

        (*service_context) = this->service_context_list_[i];

        return safe_service_context._retn ();
      }

  ACE_THROW_RETURN (CORBA::BAD_PARAM (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        EINVAL), // @@ Need minor code from PI spec!
                      CORBA::COMPLETED_NO),
                    0);
}

IOP::ServiceContext *
TAO_ServerRequestInfo::get_reply_service_context (
    IOP::ServiceId id,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::ULong len = this->service_context_list_.length ();

  for (CORBA::ULong i = 0; i < len; ++i)
    if (this->service_context_list_[i].context_id == id)
      {
        IOP::ServiceContext *service_context = 0;
        ACE_NEW_THROW_EX (service_context,
                          IOP::ServiceContext,
                          CORBA::NO_MEMORY (
                            CORBA::SystemException::_tao_minor_code (
                              TAO_DEFAULT_MINOR_CODE,
                              ENOMEM),
                            CORBA::COMPLETED_NO));
        ACE_CHECK_RETURN (0);

        IOP::ServiceContext_var safe_service_context = service_context;

        (*service_context) = this->service_context_list_[i];

        return safe_service_context._retn ();
      }

  ACE_THROW_RETURN (CORBA::BAD_PARAM (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        EINVAL), // @@ Need minor code from PI spec!
                      CORBA::COMPLETED_NO),
                    0);
}


// Use at own risk. There is no way currently of extracting an
// exception from an Any. This method is in place just to be compliant
// with the spec.
CORBA::Any *
TAO_ServerRequestInfo::sending_exception (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // The spec says that if it is a user exception which cant be inserted
  // then the UNKNOWN exception needs to be thrown with minor code TBD_U.

  CORBA::Any * temp = 0;

  ACE_NEW_THROW_EX (temp,
                    CORBA::Any,
                    CORBA::NO_MEMORY (
                      CORBA_SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (0);

  CORBA::Any_var caught_exception = temp;

  if (this->caught_exception_ != 0)
    (*temp) <<= *(this->caught_exception_);

  return caught_exception._retn ();
}

// = TAO specific method done since there currently is no simple way
// to extract exceptions from an Any
CORBA::Exception *
TAO_ServerRequestInfo::_sending_exception (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->caught_exception_;
}

CORBA::OctetSeq *
TAO_ServerRequestInfo::object_id (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOTSUP),
                      CORBA::COMPLETED_NO),
                    0);
}

CORBA::OctetSeq *
TAO_ServerRequestInfo::adapter_id (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOTSUP),
                      CORBA::COMPLETED_NO),
                    0);
}

char *
TAO_ServerRequestInfo::target_most_derived_interface (
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOTSUP),
                      CORBA::COMPLETED_NO),
                    0);
}

CORBA::Policy_ptr
TAO_ServerRequestInfo::get_server_policy (CORBA::PolicyType,
                                          CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return 0;
}

void
TAO_ServerRequestInfo::set_slot (PortableInterceptor::SlotId,
                                 const CORBA::Any &,
                                 CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::InvalidSlot))
{
  ACE_THROW (CORBA::NO_IMPLEMENT (
               CORBA::SystemException::_tao_minor_code (
                 TAO_DEFAULT_MINOR_CODE,
                 ENOTSUP),
               CORBA::COMPLETED_NO));
}

CORBA::Boolean
TAO_ServerRequestInfo::target_is_a (const char *,
                                    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOTSUP),
                      CORBA::COMPLETED_NO),
                    0);
}

void
TAO_ServerRequestInfo::add_reply_service_context (
    const IOP::ServiceContext & service_context,
    CORBA::Boolean replace,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Copy the service context into the list.
  CORBA::ULong len = this->service_context_list_.length ();

  // First check if a service context with the same ID exists.
  for (CORBA::ULong i = 0; i < len; ++i)
    {
      if (this->service_context_list_[i].context_id ==
          service_context.context_id)
        {
          if (replace)
            {
              this->service_context_list_[i] = service_context;
              return;
            }
          else
            // @@ Need the minor code once it becomes available.
            ACE_THROW (CORBA::BAD_INV_ORDER ());
        }
    }

  // No service context with the given ID exists so add one.
  this->service_context_list_.length (len + 1);

  this->service_context_list_[len] = service_context;
}

void
TAO_ServerRequestInfo::exception (CORBA::Exception *exception)
{
  if (CORBA::SystemException::_downcast (exception) != 0)
    this->reply_status_ = PortableInterceptor::SYSTEM_EXCEPTION;
  if (CORBA::UserException::_downcast (exception) != 0)
    this->reply_status_ = PortableInterceptor::USER_EXCEPTION;

  this->caught_exception_ = exception;
}

void
TAO_ServerRequestInfo::request_id (CORBA::ULong request_id)
{
  this->request_id_ = request_id;
}

#endif  /* TAO_HAS_INTERCEPTORS == 1 */
