// -*- C++ -*-
// $Id$

#include "tao/PI_Server/ServerRequestInfo.h"

#if (TAO_HAS_INTERCEPTORS == 1)

#include "tao/PortableServer/Root_POA.h"
#include "tao/PortableServer/Servant_Upcall.h"
#include "tao/PortableServer/Servant_Base.h"

#include "tao/TAO_Server_Request.h"
#include "tao/ORB_Core.h"
#include "tao/PolicyC.h"
#include "tao/AnyTypeCode/DynamicC.h"
#include "tao/ORB_Core.h"
#include "tao/Service_Context.h"
#include "tao/PI/RequestInfo_Util.h"
#include "tao/PI/PICurrent.h"
#include "tao/PI/PICurrent_Impl.h"
#include "tao/AnyTypeCode/ExceptionA.h"

#include "ace/OS_NS_string.h"

# if !defined (__ACE_INLINE__)
#   include "tao/PI_Server/ServerRequestInfo.inl"
# endif /* !__ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

CORBA::ULong
TAO::ServerRequestInfo::request_id (void)
{
  // The request ID returned by this method need not correspond to the
  // GIOP request ID sent with the client request.  The request ID
  // must be unique across all outstanding requests.  To avoid locking
  // overhead, the address of the TAO_ServerRequest object is used as
  // the request ID.  This guarantees that the request ID is unique.
  //
  // For 64-bit platforms, only the lower 32 bits are used.  Hopefully
  // that will be enough to ensure uniqueness.

  CORBA::ULong id = 0;

  // Note that we reinterpret_cast to an "unsigned long" instead of
  // CORBA::ULong since we need to first cast to an integer large
  // enough to hold an address to avoid compile-time warnings on some
  // 64-bit platforms.

  if (sizeof (this) == 4)       // 32 bit address
    id = static_cast <CORBA::ULong> (
                     reinterpret_cast <ptrdiff_t>
                                      (&(this->server_request_)));

  else if (sizeof (this) == 8)  // 64 bit address -- use lower 32 bits
    id = static_cast <CORBA::ULong> (
                     reinterpret_cast <ptrdiff_t>
                               (&(this->server_request_)) & 0xFFFFFFFFu);

  else
    // @@ Rather than fallback on the GIOP request ID, we should use
    //    an atomically incremented variable specific to the ORB, or
    //    perhaps specific to the process.
    id = this->server_request_.request_id ();  // Fallback

  return id;
}

char *
TAO::ServerRequestInfo::operation (void)
{
  return CORBA::string_dup (this->server_request_.operation ());
}

Dynamic::ParameterList *
TAO::ServerRequestInfo::arguments (void)
{
  if (this->args_ == 0)
    {
      throw ::CORBA::BAD_INV_ORDER (CORBA::OMGVMCID | 14, CORBA::COMPLETED_NO);
    }

  // Generate the argument list on demand.
  Dynamic::ParameterList * const parameter_list =
    TAO_RequestInfo_Util::make_parameter_list ();

  Dynamic::ParameterList_var safe_parameter_list = parameter_list;

  // Return value is always generated as first TAO::Argument in
  // skeleton.  It shouldn't be included in the parameter list.
  // Skip it.
  TAO::Argument * const * const begin = this->args_ + 1;
  TAO::Argument * const * const end   = this->args_ + this->nargs_;

  ACE_ASSERT (end - begin >= 0);

  parameter_list->length (static_cast<CORBA::ULong> (end - begin));

  CORBA::ULong p = 0;
  for (TAO::Argument * const * i = begin; i != end; ++i, ++p)
    {
      // Insert the operation parameters into the
      // Dynamic::ParameterList.
      Dynamic::Parameter& parameter = (*parameter_list)[p];
      parameter.mode = (*i)->mode ();
      // When we are in receive_request and have an out argument, then
      // don't copy it, just let the any be empty with typecode tk_null
      if ((this->server_request_.pi_reply_status () != -1) ||
          (this->server_request_.pi_reply_status () == -1 &&
           (*i)->mode () != CORBA::PARAM_OUT))
        {
          (*i)->interceptor_value (&parameter.argument);
        }
    }

  return safe_parameter_list._retn ();
}

Dynamic::ExceptionList *
TAO::ServerRequestInfo::exceptions (void)
{
  if (this->args_ == 0)
    {
      throw ::CORBA::BAD_INV_ORDER (CORBA::OMGVMCID | 14, CORBA::COMPLETED_NO);
    }

  // Generate the exception list on demand.
  Dynamic::ExceptionList * const exception_list =
    TAO_RequestInfo_Util::make_exception_list ();

  Dynamic::ExceptionList_var safe_exception_list = exception_list;

  exception_list->length (this->nexceptions_);

  CORBA::TypeCode_ptr const * const begin = this->exceptions_;
  CORBA::TypeCode_ptr const * const end   =
    this->exceptions_ + this->nexceptions_;

  CORBA::ULong e = 0;
  for (CORBA::TypeCode_ptr const * i = begin; i != end; ++i, ++e)
    {
      CORBA::TypeCode_ptr tcp_object = *i;
      (*exception_list)[e] = tcp_object;
    }

  return safe_exception_list._retn ();
}

Dynamic::ContextList *
TAO::ServerRequestInfo::contexts (void)
{
  throw ::CORBA::BAD_INV_ORDER (CORBA::OMGVMCID | 14, CORBA::COMPLETED_NO);
}

Dynamic::RequestContext *
TAO::ServerRequestInfo::operation_context (void)
{
  throw ::CORBA::BAD_INV_ORDER (CORBA::OMGVMCID | 14, CORBA::COMPLETED_NO);
}

CORBA::Any *
TAO::ServerRequestInfo::result (void)
{
  if (this->args_ == 0)
    {
      throw ::CORBA::BAD_INV_ORDER (CORBA::OMGVMCID | 14, CORBA::COMPLETED_NO);
    }

  // Generate the result on demand.
  static CORBA::Boolean const tk_void_any = true;

  CORBA::Any * result_any =
    TAO_RequestInfo_Util::make_any (tk_void_any);

  CORBA::Any_var safe_result_any = result_any;

  // Result is always first element in TAO::Argument array.
  TAO::Argument * const r = this->args_[0];

  r->interceptor_value (result_any);

  return safe_result_any._retn ();
}

CORBA::Boolean
TAO::ServerRequestInfo::response_expected (void)
{
  return this->server_request_.response_expected ();
}

Messaging::SyncScope
TAO::ServerRequestInfo::sync_scope (void)
{
  if (this->server_request_.sync_with_server ())
    return Messaging::SYNC_WITH_SERVER;

  throw ::CORBA::BAD_INV_ORDER (CORBA::OMGVMCID | 14, CORBA::COMPLETED_NO);
}

PortableInterceptor::ReplyStatus
TAO::ServerRequestInfo::reply_status (void)
{
  if (this->server_request_.pi_reply_status () == -1)
    // A reply hasn't been received yet.
    throw ::CORBA::BAD_INV_ORDER (CORBA::OMGVMCID | 14, CORBA::COMPLETED_NO);

  return this->server_request_.pi_reply_status ();
}

CORBA::Object_ptr
TAO::ServerRequestInfo::forward_reference (void)
{
  if (this->server_request_.pi_reply_status () != PortableInterceptor::LOCATION_FORWARD)
    throw ::CORBA::BAD_INV_ORDER (CORBA::OMGVMCID | 14, CORBA::COMPLETED_NO);

  // TAO_ServerRequest::forward_location() already duplicates the
  // object reference.  There is no need to duplicate it here.
  return this->server_request_.forward_location ();
}

CORBA::Any *
TAO::ServerRequestInfo::get_slot (PortableInterceptor::SlotId id)
{
  // Retrieve the total number of assigned slots from the PICurrent.
  // No TSS access is incurred.
  CORBA::Object_ptr pi_current_obj =
    this->server_request_.orb_core ()->pi_current ();

  TAO::PICurrent *pi_current =
    dynamic_cast <TAO::PICurrent*> (pi_current_obj);

  if (pi_current == 0)
    throw ::CORBA::INTERNAL ();

  pi_current->check_validity (id);

  // Retrieve the request scope PICurrent object.
  TAO::PICurrent_Impl *rsc = this->server_request_.rs_pi_current ();

  return rsc->get_slot (id);

}

IOP::ServiceContext *
TAO::ServerRequestInfo::get_request_service_context (IOP::ServiceId id)
{
  TAO_Service_Context &service_context_list =
    this->server_request_.request_service_context ();

  return this->get_service_context_i (service_context_list, id);
}

IOP::ServiceContext *
TAO::ServerRequestInfo::get_reply_service_context (IOP::ServiceId id)
{
  TAO_Service_Context & service_context_list =
    this->server_request_.reply_service_context ();

  return this->get_service_context_i (service_context_list, id);
}

IOP::ServiceContext *
TAO::ServerRequestInfo::get_service_context_i (
    TAO_Service_Context & service_context_list,
    IOP::ServiceId id)
{
  IOP::ServiceContext_var service_context;

  if (service_context_list.get_context (id, service_context.out ()) != 0)
    {
      // Found.
      return service_context._retn ();
    }
  else
    {
      // Not found.
      throw ::CORBA::BAD_PARAM (CORBA::OMGVMCID | 26, CORBA::COMPLETED_NO);
    }
}

// Use at own risk. There is no way currently of extracting an
// exception from an Any. This method is in place just to be compliant
// with the spec.
CORBA::Any *
TAO::ServerRequestInfo::sending_exception (void)
{
  if (this->server_request_.pi_reply_status () != PortableInterceptor::SYSTEM_EXCEPTION
      && this->server_request_.pi_reply_status () != PortableInterceptor::USER_EXCEPTION)
    {
      throw ::CORBA::BAD_INV_ORDER (CORBA::OMGVMCID | 14, CORBA::COMPLETED_NO);
    }

  // The spec says that if it is a user exception which cannot be
  // inserted then the UNKNOWN exception should be thrown with minor
  // code 1.

  CORBA::Any * temp = 0;

  ACE_NEW_THROW_EX (temp,
                    CORBA::Any,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));

  CORBA::Any_var caught_exception_var = temp;

  if (this->server_request_.caught_exception () != 0)
    (*temp) <<= *(this->server_request_.caught_exception ());

  return caught_exception_var._retn ();
}

char *
TAO::ServerRequestInfo::server_id (void)
{
  if (this->servant_upcall_ != 0)
    return
      CORBA::string_dup (this->server_request_.orb_core ()->server_id ());

  throw ::CORBA::BAD_INV_ORDER (CORBA::OMGVMCID | 14, CORBA::COMPLETED_NO);
}

char *
TAO::ServerRequestInfo::orb_id (void)
{
  if (this->servant_upcall_ != 0)
    return
      CORBA::string_dup (this->server_request_.orb_core ()->orbid ());

  throw ::CORBA::BAD_INV_ORDER (CORBA::OMGVMCID | 14, CORBA::COMPLETED_NO);
}

PortableInterceptor::AdapterName *
TAO::ServerRequestInfo::adapter_name (void)
{
  // The adapter_name attribute defines a name for the object adapter
  // that services requests for the invoked object. In the case of the
  // POA, the adapter_name is the sequence of names from the root POA
  // to the POA that services the request. The root POA is not named
  // in this sequence.
  if (this->servant_upcall_ != 0)
    return
      this->servant_upcall_->poa ().adapter_name ();

  throw ::CORBA::BAD_INV_ORDER (CORBA::OMGVMCID | 14, CORBA::COMPLETED_NO);
}

PortableInterceptor::ObjectId *
TAO::ServerRequestInfo::object_id (void)
{
  if (this->servant_upcall_ != 0)
    {
      const PortableServer::ObjectId &id =
        this->servant_upcall_->user_id ();

      PortableInterceptor::ObjectId *tmp = 0;

      ACE_NEW_THROW_EX (tmp,
                        PortableInterceptor::ObjectId,
                        CORBA::NO_MEMORY (
                          CORBA::SystemException::_tao_minor_code (
                            TAO::VMCID,
                            ENOMEM),
                          CORBA::COMPLETED_NO));

      PortableInterceptor::ObjectId_var obj_id = tmp;

      // @@ It would be nice to avoid this copy.  However, we can't be
      //    sure if the octet sequence will out live the POA from
      //    which the object ID is ultimately obtained.  In the event
      //    the octet sequence does out live the POA, a copy is indeed
      //    necessary.  Do a copy to be on the safe side.  In any
      //    case, this is still faster than the
      //    PortableServer::Current::object_id() method since no TSS
      //    access is involved.
      CORBA::ULong const len = id.length ();
      obj_id->length (len);
      CORBA::Octet *buffer = obj_id->get_buffer ();
      ACE_OS::memcpy (buffer, id.get_buffer (), len);

      return obj_id._retn ();
    }

  throw ::CORBA::BAD_INV_ORDER (CORBA::OMGVMCID | 14, CORBA::COMPLETED_NO);
}

CORBA::OctetSeq *
TAO::ServerRequestInfo::adapter_id (void)
{
  if (this->servant_upcall_ != 0)
    return this->servant_upcall_->poa ().id ();

  throw ::CORBA::BAD_INV_ORDER (CORBA::OMGVMCID | 14, CORBA::COMPLETED_NO);
}

char *
TAO::ServerRequestInfo::target_most_derived_interface (void)
{
  PortableServer::Servant const servant =
    (this->servant_upcall_ == 0
     ? 0
     : this->servant_upcall_->servant ());

  if (servant == 0)
    {
      throw ::CORBA::NO_RESOURCES (CORBA::OMGVMCID | 1, CORBA::COMPLETED_NO);
    }

  return CORBA::string_dup (servant->_interface_repository_id ());
}

CORBA::Policy_ptr
TAO::ServerRequestInfo::get_server_policy (CORBA::PolicyType type)
{
  if (this->servant_upcall_ != 0)
    {
      CORBA::Policy_var policy =
        this->servant_upcall_->poa ().get_policy (type);

      if (!CORBA::is_nil (policy.in ()))
        {
          return policy._retn ();
        }
      else
        {
          // No policy matching the given PolicyType was found.
          throw ::CORBA::INV_POLICY (CORBA::OMGVMCID | 3, CORBA::COMPLETED_NO);
        }
    }

  // @@ Technically, we shouldn't be throwing this exception since
  //    this method should be valid in all server side request
  //    interception points.
  throw ::CORBA::BAD_INV_ORDER (CORBA::OMGVMCID | 14, CORBA::COMPLETED_NO);
}

void
TAO::ServerRequestInfo::set_slot (PortableInterceptor::SlotId id,
                                  const CORBA::Any &data)
{
  // Retrieve the total number of assigned slots from the PICurrent
  // object.  No TSS access is incurred.
  CORBA::Object_ptr pi_current_obj =
    this->server_request_.orb_core ()->pi_current ();

  TAO::PICurrent *pi_current =
    dynamic_cast <TAO::PICurrent*> (pi_current_obj);

  if (pi_current == 0)
    throw ::CORBA::INTERNAL ();

  pi_current->check_validity (id);

  // Retrieve the "request scope current" (RSC).
  TAO::PICurrent_Impl * rsc = this->server_request_.rs_pi_current ();

  rsc->set_slot (id, data);
}

CORBA::Boolean
TAO::ServerRequestInfo::target_is_a (const char * id)
{
  // Implemented in the generated skeleton.

  PortableServer::Servant const servant =
    (this->servant_upcall_ == 0
     ? 0
     : this->servant_upcall_->servant ());

  if (servant == 0)
    {
      throw ::CORBA::NO_RESOURCES (CORBA::OMGVMCID | 1, CORBA::COMPLETED_NO);
    }

  return servant->_is_a (id);
}

void
TAO::ServerRequestInfo::add_reply_service_context (
    const IOP::ServiceContext & service_context,
    CORBA::Boolean replace)
{
  // Get the service context from the list
  TAO_Service_Context &service_context_list =
    this->server_request_.reply_service_context ();

  if (service_context_list.set_context (service_context, replace) == 0)
    {
      throw ::CORBA::BAD_INV_ORDER (CORBA::OMGVMCID | 15, CORBA::COMPLETED_NO);
    }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif  /* TAO_HAS_INTERCEPTORS == 1 */
