// -*- C++ -*-

#include "ClientRequestInfo_i.h"
#include "Invocation.h"
#include "Stub.h"
#include "Tagged_Components.h"
#include "debug.h"

ACE_RCSID (TAO,
           ClientRequestInfo_i,
           "$Id$")

#if TAO_HAS_INTERCEPTORS == 1

# if !defined (__ACE_INLINE__)
#   include "ClientRequestInfo_i.inl"
# endif /* !__ACE_INLINE__ */

TAO_ClientRequestInfo_i::TAO_ClientRequestInfo_i (TAO_GIOP_Invocation *inv,
                                                  CORBA::Object_ptr target)
  : invocation_ (inv),
    target_ (target), // No need to duplicate.
    caught_exception_ (0),
    response_expected_ (1),
    reply_status_ (-1)
{
}

TAO_ClientRequestInfo_i::~TAO_ClientRequestInfo_i (void)
{
}

CORBA::Object_ptr
TAO_ClientRequestInfo_i::target (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::Object::_duplicate (this->target_);
}

CORBA::Object_ptr
TAO_ClientRequestInfo_i::effective_target (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (this->reply_status_ == PortableInterceptor::LOCATION_FORWARD)
    {
      // TAO_GIOP_Invocation::forward_reference() already duplicates
      // the reference before returning it so there is no need to
      // duplicate it here.
      return this->invocation_->forward_reference ();
    }

  return CORBA::Object::_duplicate (this->target_);
}

IOP::TaggedProfile *
TAO_ClientRequestInfo_i::effective_profile (TAO_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  IOP::TaggedProfile *tagged_profile = 0;
  ACE_NEW_THROW_EX (tagged_profile,
                    IOP::TaggedProfile,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (0);

  IOP::TaggedProfile_var safe_tagged_profile = tagged_profile;

  IOP::TaggedProfile &ep =
    this->target_->_stubobj ()->profile_in_use ()->create_tagged_profile ();

  tagged_profile->tag = ep.tag;
  tagged_profile->profile_data = ep.profile_data;  // Deep copy

  return safe_tagged_profile._retn ();
}

// Use at own risk. There is no way currently of extracting an
// exception from an Any. This method is in place just to be compliant
// with the spec.
CORBA::Any *
TAO_ClientRequestInfo_i::received_exception (TAO_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (this->reply_status_ != PortableInterceptor::SYSTEM_EXCEPTION
      && this->reply_status_ != PortableInterceptor::USER_EXCEPTION)
    {
      ACE_THROW_RETURN (CORBA::BAD_INV_ORDER (TAO_OMG_VMCID | 14,
                                              CORBA::COMPLETED_NO), 0);
    }

  // The spec says that if it is a user exception which can't be
  // inserted then the UNKNOWN exception needs to be thrown with minor
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
    *temp <<= *(this->caught_exception_);

  return caught_exception._retn ();
}

char *
TAO_ClientRequestInfo_i::received_exception_id (
    TAO_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (this->reply_status_ != PortableInterceptor::SYSTEM_EXCEPTION
      && this->reply_status_ != PortableInterceptor::USER_EXCEPTION)
    {
      ACE_THROW_RETURN (CORBA::BAD_INV_ORDER (TAO_OMG_VMCID | 14,
                                              CORBA::COMPLETED_NO),
                        0);
    }

  return CORBA::string_dup (this->caught_exception_->_id ());
}

IOP::TaggedComponent *
TAO_ClientRequestInfo_i::get_effective_component (
    IOP::ComponentId id
    TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_Tagged_Components &ecs =
    this->target_->_stubobj ()->profile_in_use ()->tagged_components ();

  IOP::MultipleComponentProfile &components = ecs.components ();

  CORBA::ULong len = components.length ();
  for (CORBA::ULong i = 0; i < len; ++i)
    {
      if (components[i].tag == id)
        {
          IOP::TaggedComponent *tagged_component = 0;

          // Only allocate a sequence if we have a tagged component
          // that matches the given IOP::ComponentId.
          ACE_NEW_THROW_EX (tagged_component,
                            IOP::TaggedComponent,
                            CORBA::NO_MEMORY (
                              CORBA::SystemException::_tao_minor_code (
                                TAO_DEFAULT_MINOR_CODE,
                                ENOMEM),
                              CORBA::COMPLETED_NO));
          ACE_CHECK_RETURN (0);

          IOP::TaggedComponent_var safe_tagged_component =
            tagged_component;

          (*tagged_component) = components[i];  // Deep copy

          return safe_tagged_component._retn ();
        }
    }

  // No tagged component was found that matched the given
  // IOP::ComponentId.
  ACE_THROW_RETURN (CORBA::BAD_PARAM (TAO_OMG_VMCID | 28,
                                      CORBA::COMPLETED_NO),
                    0);
}

IOP::TaggedComponentSeq *
TAO_ClientRequestInfo_i::get_effective_components (
    IOP::ComponentId id
    TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_Tagged_Components &ecs =
    this->target_->_stubobj ()->profile_in_use ()->tagged_components ();

  IOP::MultipleComponentProfile &components = ecs.components ();

  IOP::TaggedComponentSeq *tagged_components = 0;
  IOP::TaggedComponentSeq_var safe_tagged_components;

  CORBA::ULong len = components.length ();
  for (CORBA::ULong i = 0; i < len; ++i)
    {
      if (components[i].tag == id)
        {
          if (tagged_components == 0)
            {
              // Only allocate a sequence if we have tagged components
              // to place into the sequence.
              ACE_NEW_THROW_EX (tagged_components,
                                IOP::TaggedComponentSeq,
                                CORBA::NO_MEMORY (
                                  CORBA::SystemException::_tao_minor_code (
                                    TAO_DEFAULT_MINOR_CODE,
                                    ENOMEM),
                                  CORBA::COMPLETED_NO));
              ACE_CHECK_RETURN (0);

              safe_tagged_components = tagged_components;
            }

          CORBA::ULong old_len = safe_tagged_components->length ();
          safe_tagged_components->length (old_len + 1);

          safe_tagged_components[old_len] = components[i];  // Deep copy
        }
    }

  if (tagged_components == 0)
    {
      // No tagged component sequence was allocated, meaning no tagged
      // components were found that matched the given
      // IOP::ComponentId.
      ACE_THROW_RETURN (CORBA::BAD_PARAM (TAO_OMG_VMCID | 28,
                                          CORBA::COMPLETED_NO),
                        0);
    }

  return safe_tagged_components._retn ();
}

CORBA::Policy_ptr
TAO_ClientRequestInfo_i::get_request_policy (CORBA::PolicyType type
                                           TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // @@ Do we need to look anywhere else for the request policies?

#if TAO_HAS_CORBA_MESSAGING == 1
  return this->target_->_get_policy (type
                                      TAO_ENV_ARG_PARAMETER);
#else
  ACE_UNUSED_ARG (type);

  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOTSUP),
                      CORBA::COMPLETED_NO),
                    0);
#endif  /* TAO_HAS_CORBA_MESSAGING == 1 */
}

void
TAO_ClientRequestInfo_i::add_request_service_context (
    const IOP::ServiceContext & service_context,
    CORBA::Boolean replace
    TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Get the service context from the list
  TAO_Service_Context &service_context_list =
    this->invocation_->request_service_context ();

  if (service_context_list.set_context (service_context, replace) == 0)
    {
      ACE_THROW (CORBA::BAD_INV_ORDER (TAO_OMG_VMCID | 15,
                                       CORBA::COMPLETED_NO));
    }
}

CORBA::ULong
TAO_ClientRequestInfo_i::request_id (TAO_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // @todo We may have to worry about AMI once we support interceptors
  //       in AMI requests since the Invocation object no longer
  //       exists once an AMI request has been made.  In that case,
  //       the reply dispatcher address should be used.

  // The request ID must be unique across all outstanding requests.
  // To avoid synchronization overhead, the address of this Invocation
  // object is used as the request ID.  This guarantees that the
  // request ID is unique without being forced to acquire a lock.
  //
  // For 64-bit platforms, we right shift 8 bits and then use the
  // lower 32 bits of that shifted value.  Rather than losing the
  // upper 32 bits of significant digits by taking the lower 32 bits,
  // we only lose the upper 24 by doing the shift.  Basically, the
  // resulting request ID will comprised of bits 8 through 39.  This is
  // made possible by the fact that this Invocation object is large
  // enough to increase the likelihood that those bits (0 through 39)
  // are unique.  In particular, this->buffer_ is 512 bytes
  // (ACE_CDR::DEFAULT_BUFSIZE) long by default; implying that
  // dropping the lower 8 bits of the this Invocation object's 64 bit
  // address (i.e. 256 bytes) is not a problem.

  CORBA::ULong id = 0;

  // Note that we reinterpret_cast to an "unsigned long" instead of
  // CORBA::ULong since we need to first cast to an integer large
  // enough to hold an address to avoid compile-time warnings on some
  // 64-bit platforms.

  // 32 bit address
  if (sizeof (this) == 4)
    id = ACE_reinterpret_cast (unsigned long, this->invocation_);

  // 64 bit address -- bits 8 through 39  (see notes above!)
  // In this case, we make sure this object is large enough to safely
  // do the right shift.  This is necessary since the size of the
  // buffer that makes this object is configurable.
  else if (sizeof (this) == 8
           && sizeof (*(this->invocation_)) > 256 /* 2 << 8 */)
    id =
      (ACE_reinterpret_cast (unsigned long,
                             this->invocation_) >> 8) & 0xFFFFFFFFu;

  // 64 bit address -- lower 32 bits
  else if (sizeof (this) == 8)
    id = ACE_reinterpret_cast (unsigned long,
                               this->invocation_) & 0xFFFFFFFFu;

  // @@ The following request ID generator prevents the
  //    PortableInterceptor::ClientRequestInterceptor::send_request()
  //    interception point from occuring before the call to connect,
  //    thus preventing us from adding an optimization that itself
  //    prevents a connection from being unnecessarily performed.
  //    Thus, the ClientRequestInfo object is forced to have its own
  //    request ID generator in order to make it possible to implement
  //    the above optimization.
  //
  //    Ideally, this request ID generator should go away, especially
  //    since it adds a lock to the critical path.
  //   else    // Fallback
  //     id = this->invocation_->request_id ();

  else
    {
      if (TAO_debug_level > 0)
        ACE_ERROR ((LM_ERROR,
                    "(%P|%t) ClientRequestInfo::request_id() failed\n"
                    "(%P|%t) since its request ID generator is not\n"
                    "(%P|%t) supported on this platform.\n"));

      ACE_THROW_RETURN (CORBA::INTERNAL (), 0);
    }

  return id;
}

char *
TAO_ClientRequestInfo_i::operation (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup (this->invocation_->operation ());
}

Dynamic::ParameterList *
TAO_ClientRequestInfo_i::arguments (TAO_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::BAD_INV_ORDER (TAO_OMG_VMCID | 14,
                                          CORBA::COMPLETED_NO),
                    0);
}

Dynamic::ExceptionList *
TAO_ClientRequestInfo_i::exceptions (TAO_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::BAD_INV_ORDER (TAO_OMG_VMCID | 14,
                                          CORBA::COMPLETED_NO),
                    0);
}

Dynamic::ContextList *
TAO_ClientRequestInfo_i::contexts (TAO_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::BAD_INV_ORDER (TAO_OMG_VMCID | 14,
                                          CORBA::COMPLETED_NO),
                    0);
}

Dynamic::RequestContext *
TAO_ClientRequestInfo_i::operation_context (TAO_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::BAD_INV_ORDER (TAO_OMG_VMCID | 14,
                                          CORBA::COMPLETED_NO),
                    0);
}

CORBA::Any *
TAO_ClientRequestInfo_i::result (TAO_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::BAD_INV_ORDER (TAO_OMG_VMCID | 14,
                                          CORBA::COMPLETED_NO),
                    0);
}

CORBA::Boolean
TAO_ClientRequestInfo_i::response_expected (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->response_expected_;
}

# if TAO_HAS_CORBA_MESSAGING == 1
CORBA::Short
TAO_ClientRequestInfo_i::sync_scope (TAO_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_GIOP_Oneway_Invocation *inv =
    ACE_dynamic_cast (TAO_GIOP_Oneway_Invocation *,
                      this->invocation_);

  // The response_expected_ check is a precautionary measure for
  // platforms that do not support RTTI, i.e. where the dynamic_cast
  // above would incorrectly work.  If the response_expected flag is
  // not equal to zero then it is fairly safe to assume that the
  // invocation is not a one-way, meaning that the sync_scope() method
  // is not available.
  if (inv != 0 && this->response_expected_ == 0)
    return inv->sync_scope ();

  ACE_THROW_RETURN (CORBA::BAD_INV_ORDER (TAO_OMG_VMCID | 14,
                                          CORBA::COMPLETED_NO),
                    -1);
}
#endif  /* TAO_HAS_CORBA_MESSAGING == 1 */

PortableInterceptor::ReplyStatus
TAO_ClientRequestInfo_i::reply_status (TAO_ENV_SINGLE_ARG_DECL)
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
TAO_ClientRequestInfo_i::forward_reference (TAO_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (this->reply_status_ != PortableInterceptor::LOCATION_FORWARD)
    ACE_THROW_RETURN (CORBA::BAD_INV_ORDER (TAO_OMG_VMCID | 14,
                                            CORBA::COMPLETED_NO),
                      CORBA::Object::_nil ());

  // TAO_GIOP_Invocation::forward_reference() already duplicates the
  // reference before returning it so there is no need to duplicate it
  // here.
  return this->invocation_->forward_reference ();
}

CORBA::Any *
TAO_ClientRequestInfo_i::get_slot (PortableInterceptor::SlotId id
                                   TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::InvalidSlot))
{
  // @@ This implementation incurs a TSS access each time it is
  //    invoked.  It need not do that.  This method can be invoked by
  //    each client request interceptor multiple times.  At some point
  //    we need to add the request scope current to the Invocation
  //    object or some other object that is tied to a given
  //    invocation.  That way, only one TSS access would be
  //    introduced.
  //        -Ossama

  TAO_PICurrent *pi_current =
    this->invocation_->orb_core ()->pi_current ();

  if (pi_current == 0)
    ACE_THROW_RETURN (CORBA::INTERNAL (), 0);

  // PICurrent is read-only during a request invocation on the client
  // side.  No copying is necessary.
  return
    pi_current->get_slot (id
                           TAO_ENV_ARG_PARAMETER);
}

IOP::ServiceContext *
TAO_ClientRequestInfo_i::get_request_service_context (
    IOP::ServiceId id
    TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Get the service context from the list
  TAO_Service_Context &service_context_list =
    this->invocation_->request_service_context ();

  return this->get_service_context_i (service_context_list,
                                      id
                                       TAO_ENV_ARG_PARAMETER);
}


IOP::ServiceContext *
TAO_ClientRequestInfo_i::get_reply_service_context (
    IOP::ServiceId id
    TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Get the service context from the list
  TAO_Service_Context &service_context_list =
    this->invocation_->reply_service_context ();

  return this->get_service_context_i (service_context_list,
                                      id
                                       TAO_ENV_ARG_PARAMETER);
}

IOP::ServiceContext *
TAO_ClientRequestInfo_i::get_service_context_i (
    TAO_Service_Context &service_context_list,
    IOP::ServiceId id
    TAO_ENV_ARG_DECL)
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

void
TAO_ClientRequestInfo_i::reply_status (int invoke_status)
{
  switch (invoke_status)
    {
    case TAO_INVOKE_OK:
      this->reply_status_ = PortableInterceptor::SUCCESSFUL;
      break;
    case TAO_INVOKE_RESTART:
      if (this->invocation_->received_location_forward ())
        this->reply_status_ = PortableInterceptor::LOCATION_FORWARD;
      else
        this->reply_status_ = PortableInterceptor::TRANSPORT_RETRY;
      break;
    default:
      // We should only get here if the invocation status is
      // TAO_INVOKE_EXCEPTION, i.e. a CORBA::SystemException, so set
      // the appropriate reply status.
      this->reply_status_ = PortableInterceptor::SYSTEM_EXCEPTION;
      break;
    }
}

#endif /* TAO_HAS_INTERCEPTORS == 1 */
