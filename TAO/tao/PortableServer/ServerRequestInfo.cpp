// -*- C++ -*-
//
// $Id$

#include "ServerRequestInfo.h"

#include "tao/TAO_Server_Request.h"
#include "tao/PolicyC.h"

ACE_RCSID (TAO_PortableServer,
           ServerRequestInfo,
           "$Id$")

#if (TAO_HAS_INTERCEPTORS == 1)

# if !defined (__ACE_INLINE__)
#   include "ServerRequestInfo.inl"
# endif /* !__ACE_INLINE__ */

TAO_ServerRequestInfo::TAO_ServerRequestInfo (
  TAO_ServerRequest &server_request)
  : server_request_ (server_request),
    forward_reference_ (),
    //    poa_current_ (),
    adapter_id_ (),
    caught_exception_ (0),
    reply_status_ (-1)
{
}

CORBA::ULong
TAO_ServerRequestInfo::request_id (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->server_request_.request_id ();
}

char *
TAO_ServerRequestInfo::operation (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup (this->server_request_.operation ());
}

Dynamic::ParameterList *
TAO_ServerRequestInfo::arguments (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::BAD_INV_ORDER (TAO_OMG_VMCID | 10,
                                          CORBA::COMPLETED_NO),
                    0);
}

Dynamic::ExceptionList *
TAO_ServerRequestInfo::exceptions (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::BAD_INV_ORDER (TAO_OMG_VMCID | 10,
                                          CORBA::COMPLETED_NO),
                    0);
}

Dynamic::ContextList *
TAO_ServerRequestInfo::contexts (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::BAD_INV_ORDER (TAO_OMG_VMCID | 10,
                                          CORBA::COMPLETED_NO),
                    0);
}

Dynamic::RequestContext *
TAO_ServerRequestInfo::operation_context (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::BAD_INV_ORDER (TAO_OMG_VMCID | 10,
                                          CORBA::COMPLETED_NO),
                    0);
}

CORBA::Any *
TAO_ServerRequestInfo::result (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::BAD_INV_ORDER (TAO_OMG_VMCID | 10,
                                          CORBA::COMPLETED_NO),
                    0);
}

CORBA::Boolean
TAO_ServerRequestInfo::response_expected (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->server_request_.response_expected ();
}

# if TAO_HAS_CORBA_MESSAGING == 1
Messaging::SyncScope
TAO_ServerRequestInfo::sync_scope (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (this->server_request_.sync_with_server ())
    return Messaging::SYNC_WITH_SERVER;

  ACE_THROW_RETURN (CORBA::BAD_INV_ORDER (TAO_OMG_VMCID | 10,
                                          CORBA::COMPLETED_NO),
                    -1);
}
#endif  /* TAO_HAS_CORBA_MESSAGING */

PortableInterceptor::ReplyStatus
TAO_ServerRequestInfo::reply_status (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (this->reply_status_ == -1)
    // A reply hasn't been received yet.
    ACE_THROW_RETURN (CORBA::BAD_INV_ORDER (TAO_OMG_VMCID | 10,
                                            CORBA::COMPLETED_NO),
                      -1);

  return this->reply_status_;
}

CORBA::Object_ptr
TAO_ServerRequestInfo::forward_reference (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (this->reply_status_ != PortableInterceptor::LOCATION_FORWARD)
    ACE_THROW_RETURN (CORBA::BAD_INV_ORDER (TAO_OMG_VMCID | 10,
                                            CORBA::COMPLETED_NO),
                      CORBA::Object::_nil ());

  // We don't get the forward reference from the TAO_ServerRequest
  // object since it may not have been set there.  For example, this
  // is the case when a Servant Manager throws a
  // PortableServer::ForwardRequest exception.  As such, we only
  // return the one stored in this object since it is explicitly set
  // by the responsible request forwarding code.
  return CORBA::Object::_duplicate (this->forward_reference_.in ());
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
  TAO_Service_Context &service_context_list =
    this->server_request_.request_service_context ();

  return this->get_service_context_i (service_context_list,
                                      id,
                                      ACE_TRY_ENV);
}

IOP::ServiceContext *
TAO_ServerRequestInfo::get_reply_service_context (
    IOP::ServiceId id,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_Service_Context &service_context_list =
    this->server_request_.reply_service_context ();

  return this->get_service_context_i (service_context_list,
                                      id,
                                      ACE_TRY_ENV);
}

IOP::ServiceContext *
TAO_ServerRequestInfo::get_service_context_i (
    TAO_Service_Context &service_context_list,
    IOP::ServiceId id,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Create a new service context to be returned.  Assume
  // success.
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

  service_context->context_id = id;
  if (service_context_list.get_context (*service_context) != 0)
    {
      // Found.
      return safe_service_context._retn ();
    }
  else
    {
      // Not found.
      ACE_THROW_RETURN (CORBA::BAD_PARAM (TAO_OMG_VMCID | 23,
                                          CORBA::COMPLETED_NO),
                        0);
    }
}

// Use at own risk. There is no way currently of extracting an
// exception from an Any. This method is in place just to be compliant
// with the spec.
CORBA::Any *
TAO_ServerRequestInfo::sending_exception (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (this->reply_status_ != PortableInterceptor::SYSTEM_EXCEPTION
      && this->reply_status_ != PortableInterceptor::USER_EXCEPTION)
    {
      ACE_THROW_RETURN (CORBA::BAD_INV_ORDER (TAO_OMG_VMCID | 10,
                                              CORBA::COMPLETED_NO),
                        0);
    }

  // The spec says that if it is a user exception which cannot be
  // inserted then the UNKNOWN exception should be thrown with minor
  // code 1.

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
#if 0
  if (CORBA::is_nil (this->poa_current_.in ()))
    {
      CORBA::Object_var p =
        this->server_request_.orb_core ()->poa_current ();

      this->poa_current_ =
        PortableServer::Current::_narrow (p.in (),
                                          ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      if (CORBA::is_nil (this->poa_current_.in ()))
        ACE_THROW_RETURN (CORBA::INTERNAL (), 0);
    }

  ACE_TRY
    {
      CORBA::OctetSeq_var obj_id =
        this->poa_current_->get_object_id (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      return obj_id._retn ();
    }
  ACE_CATCH (PortableServer::Current::NoContext, exc)
    {
      // Convert the NoContext exception to a NO_RESOURCES exception.

      ACE_THROW_RETURN (CORBA::NO_RESOURCES (TAO_OMG_VMCID | 1,
                                             CORBA::COMPLETED_NO),
                        0);
    }
  ACE_ENDTRY;
#endif  /* 0 */

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
//   if (this->adapter_id_.in () == 0)
//     {
//       ACE_THROW_RETURN (CORBA::NO_RESOURCES (TAO_OMG_VMCID | 1,
//                                              CORBA::COMPLETED_NO),
//                         0);
//     }

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
  ACE_THROW_RETURN (CORBA::NO_RESOURCES (1, CORBA::COMPLETED_NO), 0);
}

CORBA::Policy_ptr
TAO_ServerRequestInfo::get_server_policy (CORBA::PolicyType,
                                          CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOTSUP),
                      CORBA::COMPLETED_NO),
                    CORBA::Policy::_nil ());
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
TAO_ServerRequestInfo::target_is_a (const char * /* id */,
                                    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_RESOURCES (TAO_OMG_VMCID | 1,
                                         CORBA::COMPLETED_NO), 0);
}

void
TAO_ServerRequestInfo::add_reply_service_context (
    const IOP::ServiceContext & service_context,
    CORBA::Boolean replace,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Get the service context from the list
  TAO_Service_Context &service_context_list =
    this->server_request_.reply_service_context ();

  if (service_context_list.set_context (service_context,replace) == 0)
    {
      ACE_THROW (CORBA::BAD_INV_ORDER (TAO_OMG_VMCID | 11,
                                       CORBA::COMPLETED_NO));
    }
}

#endif  /* TAO_HAS_INTERCEPTORS == 1 */
