// -*- C++ -*-
//
// $Id$

#include "ServerRequestInfo.h"
#include "POA.h"

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
  TAO_ServerRequest &server_request,
  TAO_Object_Adapter::Servant_Upcall *servant_upcall)
  : server_request_ (server_request),
    servant_upcall_ (servant_upcall),
    caught_exception_ (0),
    reply_status_ (-1)
{
}

CORBA::ULong
TAO_ServerRequestInfo::request_id (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // The request ID returned by this method need not correspond to the
  // GIOP request ID sent with the client request.  The request ID
  // must be unique across all outstanding requests.  To avoid locking
  // overhead, the address of the TAO_ServerRequest object is used as
  // the request ID.  This guarantees that the request ID is unique.
  //
  // For 64-bit platforms, only the lower 32 bits are used.  Hopefully
  // that will be enough to ensure uniqueness.

  // This is basically the same trick used in
  // TAO_GIOP_Invocation::generate_request_id().  However, no right
  // shifting of 64 bit addresses is performed since the
  // TAO_ServerRequest object is not large enough to allow that trick.

  CORBA::ULong id = 0;

  // Note that we reinterpret_cast to an "unsigned long" instead of
  // CORBA::ULong since we need to first cast to an integer large
  // enough to hold an address to avoid compile-time warnings on some
  // 64-bit platforms.

  if (sizeof (this) == 4)       // 32 bit address
    id =
      ACE_reinterpret_cast (unsigned long, &(this->server_request_));

  else if (sizeof (this) == 8)  // 64 bit address -- use lower 32 bits
    id =
      ACE_reinterpret_cast (unsigned long,
                            &(this->server_request_)) & 0xFFFFFFFFu;

  else
    // @@ Rather than fallback on the GIOP request ID, we should use
    //    an atomically incremented variable specific to the ORB, or
    //    perhaps specific to the process.
    id = this->server_request_.request_id ();  // Fallback

  return id;
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
  ACE_THROW_RETURN (CORBA::BAD_INV_ORDER (TAO_OMG_VMCID | 14,
                                          CORBA::COMPLETED_NO),
                    0);
}

Dynamic::ExceptionList *
TAO_ServerRequestInfo::exceptions (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::BAD_INV_ORDER (TAO_OMG_VMCID | 14,
                                          CORBA::COMPLETED_NO),
                    0);
}

Dynamic::ContextList *
TAO_ServerRequestInfo::contexts (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::BAD_INV_ORDER (TAO_OMG_VMCID | 14,
                                          CORBA::COMPLETED_NO),
                    0);
}

Dynamic::RequestContext *
TAO_ServerRequestInfo::operation_context (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::BAD_INV_ORDER (TAO_OMG_VMCID | 14,
                                          CORBA::COMPLETED_NO),
                    0);
}

CORBA::Any *
TAO_ServerRequestInfo::result (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::BAD_INV_ORDER (TAO_OMG_VMCID | 14,
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

  ACE_THROW_RETURN (CORBA::BAD_INV_ORDER (TAO_OMG_VMCID | 14,
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
    ACE_THROW_RETURN (CORBA::BAD_INV_ORDER (TAO_OMG_VMCID | 14,
                                            CORBA::COMPLETED_NO),
                      -1);

  return this->reply_status_;
}

CORBA::Object_ptr
TAO_ServerRequestInfo::forward_reference (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (this->reply_status_ != PortableInterceptor::LOCATION_FORWARD)
    ACE_THROW_RETURN (CORBA::BAD_INV_ORDER (TAO_OMG_VMCID | 14,
                                            CORBA::COMPLETED_NO),
                      CORBA::Object::_nil ());

  // TAO_ServerRequest::forward_location() already duplicates the
  // object reference.  There is no need to duplicate it here.
  return this->server_request_.forward_location ();
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
      ACE_THROW_RETURN (CORBA::BAD_PARAM (TAO_OMG_VMCID | 26,
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
      ACE_THROW_RETURN (CORBA::BAD_INV_ORDER (TAO_OMG_VMCID | 14,
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
  if (this->servant_upcall_ != 0)
    {
      const PortableServer::ObjectId &id =
        this->servant_upcall_->id ();

      CORBA::OctetSeq *obj_id = 0;

      ACE_NEW_THROW_EX (obj_id,
                        CORBA::OctetSeq,
                        CORBA::NO_MEMORY (
                          CORBA::SystemException::_tao_minor_code (
                            TAO_DEFAULT_MINOR_CODE,
                            ENOMEM),
                          CORBA::COMPLETED_NO));
      ACE_CHECK_RETURN (0);

      // @@ It would be nice to avoid this copy.  However, we can't be
      //    sure if the octet sequence will out live the POA from
      //    which the object ID is ultimately obtained.  In the event
      //    the octet sequence does out live the POA, a copy is indeed
      //    necessary.  Do a copy to be on the safe side.  In any
      //    case, this is still faster than the
      //    PortableServer::Current::object_id() method since no TSS
      //    access is involved.
      CORBA::ULong len = id.length ();
      obj_id->length (len);
      CORBA::Octet *buffer = obj_id->get_buffer ();
      ACE_OS_String::memcpy (buffer, id.get_buffer (), len);

      return obj_id;
    }

  ACE_THROW_RETURN (CORBA::BAD_INV_ORDER (TAO_OMG_VMCID | 14,
                                          CORBA::COMPLETED_NO),
                    0);
}

CORBA::OctetSeq *
TAO_ServerRequestInfo::adapter_id (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (this->servant_upcall_ != 0)
    return this->servant_upcall_->poa ().id (ACE_TRY_ENV);

  ACE_THROW_RETURN (CORBA::BAD_INV_ORDER (TAO_OMG_VMCID | 14,
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

  if (service_context_list.set_context (service_context, replace) == 0)
    {
      ACE_THROW (CORBA::BAD_INV_ORDER (TAO_OMG_VMCID | 15,
                                       CORBA::COMPLETED_NO));
    }
}

#endif  /* TAO_HAS_INTERCEPTORS == 1 */
