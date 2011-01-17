// $Id$

#include "tao/PI/ClientRequestInfo.h"

#if TAO_HAS_INTERCEPTORS == 1

#include "tao/AnyTypeCode/Any.h"
#include "tao/AnyTypeCode/ExceptionA.h"

#include "tao/PI/PICurrent.h"
#include "tao/PI/RequestInfo_Util.h"

#include "tao/PolicyC.h"
#include "tao/PortableInterceptorC.h"
#include "tao/Invocation_Base.h"
#include "tao/operation_details.h"
#include "tao/Stub.h"
#include "tao/ORB_Core.h"
#include "tao/Profile.h"
#include "tao/debug.h"
#include "tao/Service_Context.h"
#include "tao/Exception_Data.h"

#include "ace/CORBA_macros.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_ClientRequestInfo::TAO_ClientRequestInfo (TAO::Invocation_Base *inv)
  : invocation_ (inv),
    rs_pi_current_ ()
{
  this->setup_picurrent ();
}

void
TAO_ClientRequestInfo::setup_picurrent (void)
{
  // Retrieve the thread scope current (no TSS access incurred yet).
  CORBA::Object_ptr pi_current_obj =
    this->invocation_->stub ()->orb_core ()->pi_current ();

  TAO::PICurrent *pi_current =
    dynamic_cast <TAO::PICurrent*> (pi_current_obj);

  // If the slot count is zero, then there is nothing to copy.
  // Prevent any copying (and hence TSS accesses) from occurring.
  if (pi_current != 0 && pi_current->slot_count () != 0)
    {
      // Retrieve the thread scope current.
      TAO::PICurrent_Impl *tsc = pi_current->tsc ();

      if (tsc != 0)
        {
          // Logically copy the TSC's slot table to the RSC.
          this->rs_pi_current_.take_lazy_copy (tsc);
       }
    }
}

IOP::ServiceContext *
TAO_ClientRequestInfo::get_service_context_i (
    TAO_Service_Context &service_context_list,
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

CORBA::Object_ptr
TAO_ClientRequestInfo::target (void)
{
  this->check_validity ();

  return CORBA::Object::_duplicate (this->invocation_->target ());
}

CORBA::Object_ptr
TAO_ClientRequestInfo::effective_target (void)
{
  this->check_validity ();

  return CORBA::Object::_duplicate (this->invocation_->effective_target ());
}

IOP::TaggedProfile *
TAO_ClientRequestInfo::effective_profile (void)
{
  this->check_validity ();

  IOP::TaggedProfile *tagged_profile = 0;
  ACE_NEW_THROW_EX (tagged_profile,
                    IOP::TaggedProfile,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));

  IOP::TaggedProfile_var safe_tagged_profile = tagged_profile;

  TAO_Stub *stub =
    this->invocation_->effective_target ()->_stubobj ();

  IOP::TaggedProfile *ep =
    stub->profile_in_use ()->create_tagged_profile ();

  if (ep == 0)
    {
      throw ::CORBA::BAD_PARAM (CORBA::OMGVMCID | 28, CORBA::COMPLETED_NO);
    }

  // @@BAD_PARAM exception
  tagged_profile->tag = ep->tag;
  tagged_profile->profile_data = ep->profile_data;  // Deep copy

  return safe_tagged_profile._retn ();
}

// Use at own risk. There is no way currently of extracting an
// exception from an Any. This method is in place just to be compliant
// with the spec.
CORBA::Any *
TAO_ClientRequestInfo::received_exception (void)
{
  this->check_validity ();

  if (this->invocation_->pi_reply_status () != PortableInterceptor::SYSTEM_EXCEPTION
      && this->invocation_->pi_reply_status () != PortableInterceptor::USER_EXCEPTION)
    {
      throw ::CORBA::BAD_INV_ORDER (CORBA::OMGVMCID | 14, CORBA::COMPLETED_NO);
    }

  // The spec says that if it is a user exception which can't be
  // inserted then the UNKNOWN exception needs to be thrown with minor
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

  CORBA::Exception *caught_exception =
    invocation_->caught_exception ();

  if (caught_exception != 0)
    *temp <<= *(caught_exception);

  return caught_exception_var._retn ();
}

char *
TAO_ClientRequestInfo::received_exception_id (void)
{
  this->check_validity ();

  CORBA::Exception *caught_exception = invocation_->caught_exception ();

  if (caught_exception == 0)
    {
      throw ::CORBA::BAD_INV_ORDER (CORBA::OMGVMCID | 14, CORBA::COMPLETED_NO);
    }

  return CORBA::string_dup (caught_exception->_rep_id ());
}

IOP::TaggedComponent *
TAO_ClientRequestInfo::get_effective_component (IOP::ComponentId id)
{
  this->check_validity ();

  TAO_Stub *stub = this->invocation_->effective_target ()->_stubobj ();

  TAO_Tagged_Components &ecs = stub->profile_in_use ()->tagged_components ();

  IOP::MultipleComponentProfile &components = ecs.components ();

  CORBA::ULong const len = components.length ();
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
                                TAO::VMCID,
                                ENOMEM),
                              CORBA::COMPLETED_NO));

          IOP::TaggedComponent_var safe_tagged_component =
            tagged_component;

          (*tagged_component) = components[i];  // Deep copy

          return safe_tagged_component._retn ();
        }
    }

  // No tagged component was found that matched the given
  // IOP::ComponentId.
  throw ::CORBA::BAD_PARAM (CORBA::OMGVMCID | 28, CORBA::COMPLETED_NO);
}

IOP::TaggedComponentSeq *
TAO_ClientRequestInfo::get_effective_components (IOP::ComponentId id)
{
  this->check_validity ();

  TAO_Stub *stub = this->invocation_->target ()->_stubobj ();

  TAO_Tagged_Components &ecs = stub->profile_in_use ()->tagged_components ();

  IOP::MultipleComponentProfile &components = ecs.components ();

  IOP::TaggedComponentSeq *tagged_components = 0;
  IOP::TaggedComponentSeq_var safe_tagged_components;

  const CORBA::ULong len = components.length ();
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
                                    TAO::VMCID,
                                    ENOMEM),
                                  CORBA::COMPLETED_NO));

              safe_tagged_components = tagged_components;
            }

          const CORBA::ULong old_len = safe_tagged_components->length ();
          safe_tagged_components->length (old_len + 1);

          safe_tagged_components[old_len] = components[i];  // Deep copy
        }
    }

  if (tagged_components == 0)
    {
      // No tagged component sequence was allocated, meaning no tagged
      // components were found that matched the given
      // IOP::ComponentId.
      throw ::CORBA::BAD_PARAM (CORBA::OMGVMCID | 28, CORBA::COMPLETED_NO);
    }

  return safe_tagged_components._retn ();
}

CORBA::Policy_ptr
TAO_ClientRequestInfo::get_request_policy (CORBA::PolicyType type)
{
  this->check_validity ();

  // @@ Do we need to look anywhere else for the request policies?

#if TAO_HAS_CORBA_MESSAGING == 1
  return this->invocation_->target ()->_get_policy (type);
#else
  ACE_UNUSED_ARG (type);

  throw ::CORBA::NO_IMPLEMENT (
    CORBA::SystemException::_tao_minor_code (
      TAO::VMCID,
      ENOTSUP),
    CORBA::COMPLETED_NO);
#endif  /* TAO_HAS_CORBA_MESSAGING == 1 */
}

void
TAO_ClientRequestInfo::add_request_service_context (
    const IOP::ServiceContext & service_context,
    CORBA::Boolean replace)
{
  this->check_validity ();

  // Get the service context from the list
  TAO_Service_Context &service_context_list =
    this->invocation_->request_service_context ();

  if (service_context_list.set_context (service_context, replace) == 0)
    {
      throw ::CORBA::BAD_INV_ORDER (CORBA::OMGVMCID | 15, CORBA::COMPLETED_NO);
    }
}

CORBA::ULong
TAO_ClientRequestInfo::request_id (void)
{
  this->check_validity ();

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
    id =
      static_cast<CORBA::ULong> (
        reinterpret_cast<ptrdiff_t> (this->invocation_));

  // 64 bit address -- bits 8 through 39  (see notes above!)
  // In this case, we make sure this object is large enough to safely
  // do the right shift.  This is necessary since the size of the
  // buffer that makes this object is configurable.
  else if (sizeof (this) == 8
           && sizeof (*(this->invocation_)) > 256 /* 2 << 8 */)
    id =
      (static_cast<CORBA::ULong> (
         reinterpret_cast<ptrdiff_t> (this->invocation_)) >> 8) & 0xFFFFFFFFu;

  // 64 bit address -- lower 32 bits
  else if (sizeof (this) == 8)
    id =
      static_cast<CORBA::ULong> (
        reinterpret_cast<ptrdiff_t> (this->invocation_)) & 0xFFFFFFFFu;

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

      throw ::CORBA::INTERNAL ();
    }

  return id;
}

char *
TAO_ClientRequestInfo::operation (void)
{
  this->check_validity ();

  return CORBA::string_dup (this->invocation_->operation_details ().opname  ());
}

Dynamic::ParameterList *
TAO_ClientRequestInfo::arguments (void)
{
  this->check_validity ();

  // Generate the argument list on demand.
  Dynamic::ParameterList *parameter_list =
    TAO_RequestInfo_Util::make_parameter_list ();

  Dynamic::ParameterList_var safe_parameter_list = parameter_list;

  if (this->parameter_list (*parameter_list) == false)
    throw ::CORBA::MARSHAL ();

  return safe_parameter_list._retn ();
}

bool
TAO_ClientRequestInfo::parameter_list (Dynamic::ParameterList &param_list)
{
  // Account for the return type that is in the argument list.
  param_list.length (this->invocation_->operation_details ().args_num () - 1);

  for (CORBA::ULong i = 1; i != this->invocation_->operation_details ().args_num (); ++i)
    {
      TAO::Argument *argument =
        this->invocation_->operation_details ().args ()[i];
      Dynamic::Parameter &p = param_list[i - 1];
      p.mode = argument->mode ();
      // When we are in send_request and have an out argument, then
      // don't copy it, just let the any be empty with typecode tk_null
      if ((this->invocation_->invoke_status () != TAO::TAO_INVOKE_START) ||
          (this->invocation_->invoke_status () == TAO::TAO_INVOKE_START &&
           argument->mode () != CORBA::PARAM_OUT))
        {
          argument->interceptor_value (&p.argument);
        }
    }

  return true;
}

Dynamic::ExceptionList *
TAO_ClientRequestInfo::exceptions (void)
{
  this->check_validity ();

  Dynamic::ExceptionList *exception_list =
    TAO_RequestInfo_Util::make_exception_list ();

  Dynamic::ExceptionList_var safe_exception_list = exception_list;

  if (this->exception_list (*exception_list) == false)
    throw ::CORBA::MARSHAL ();

  return safe_exception_list._retn ();
}

bool
TAO_ClientRequestInfo::exception_list (Dynamic::ExceptionList &exception_list)
{
  if (this->invocation_->operation_details ().ex_count ())
    {
      exception_list.length (this->invocation_->operation_details ().ex_count ());

      for (CORBA::ULong i = 0;
           i != this->invocation_->operation_details ().ex_count ();
           ++i)
        {
          CORBA::TypeCode_ptr tcp =
            this->invocation_->operation_details ().ex_data ()[i].tc_ptr;
          if (!CORBA::is_nil (tcp))
            {
              exception_list[i] = tcp;
            }
        }
    }
  return true;
}

Dynamic::ContextList *
TAO_ClientRequestInfo::contexts (void)
{
  this->check_validity ();

  throw ::CORBA::BAD_INV_ORDER (CORBA::OMGVMCID | 14, CORBA::COMPLETED_NO);
}

Dynamic::RequestContext *
TAO_ClientRequestInfo::operation_context (void)
{
  this->check_validity ();

  throw ::CORBA::BAD_INV_ORDER (CORBA::OMGVMCID | 14, CORBA::COMPLETED_NO);
}

CORBA::Any *
TAO_ClientRequestInfo::result (void)
{
  this->check_validity ();

  // Generate the result on demand.
  static const CORBA::Boolean tk_void_any = 0;
  CORBA::Any *result_any =
    TAO_RequestInfo_Util::make_any (tk_void_any);

  CORBA::Any_var safe_result_any = result_any;

  if (this->result (result_any) == false)
    throw ::CORBA::MARSHAL ();

  return safe_result_any._retn ();
}

bool
TAO_ClientRequestInfo::result (CORBA::Any *any)
{
  // Result is always first element in TAO::Argument array.
  TAO::Argument * const r = this->invocation_->operation_details ().args ()[0];

  r->interceptor_value (any);

  return true;
}

CORBA::Boolean
TAO_ClientRequestInfo::response_expected (void)
{
  this->check_validity ();

  return this->invocation_->response_expected ();
}

Messaging::SyncScope
TAO_ClientRequestInfo::sync_scope (void)
{
  this->check_validity ();

  return this->invocation_->operation_details ().response_flags ();
}

PortableInterceptor::ReplyStatus
TAO_ClientRequestInfo::reply_status (void)
{
  this->check_validity ();

  PortableInterceptor::ReplyStatus const status =
    this->invocation_->pi_reply_status();
  if (status == -1 || status == PortableInterceptor::UNKNOWN)
    {
      // A reply hasn't been received yet.
      throw ::CORBA::BAD_INV_ORDER (CORBA::OMGVMCID | 14, CORBA::COMPLETED_NO);
    }

  return status;
}

CORBA::Object_ptr
TAO_ClientRequestInfo::forward_reference (void)
{
  this->check_validity ();

  if (this->invocation_->pi_reply_status() != PortableInterceptor::LOCATION_FORWARD)
    {
      throw ::CORBA::BAD_INV_ORDER (CORBA::OMGVMCID | 14, CORBA::COMPLETED_NO);
    }

  // TAO::Invocation_Base::forward_reference() already duplicates the
  // reference before returning it so there is no need to duplicate it
  // here.
  return this->invocation_->forwarded_reference ();
}

CORBA::Any *
TAO_ClientRequestInfo::get_slot (PortableInterceptor::SlotId id)
{
  this->check_validity ();

  return this->rs_pi_current_.get_slot (id);
}

IOP::ServiceContext *
TAO_ClientRequestInfo::get_request_service_context (IOP::ServiceId id)
{
  this->check_validity ();

  // Get the service context from the list
  TAO_Service_Context &service_context_list =
    this->invocation_->request_service_context ();

  return this->get_service_context_i (service_context_list, id);
}

IOP::ServiceContext *
TAO_ClientRequestInfo::get_reply_service_context (IOP::ServiceId id)
{
  this->check_validity ();

  // Get the service context from the list
  TAO_Service_Context &service_context_list =
    this->invocation_->reply_service_context ();

  return this->get_service_context_i (service_context_list, id);
}

void
TAO_ClientRequestInfo::check_validity (void)
{
  if (this->invocation_ == 0)
    throw ::CORBA::BAD_INV_ORDER (CORBA::OMGVMCID | 14, CORBA::COMPLETED_NO);
}

void
TAO_ClientRequestInfo::tao_ft_expiration_time (TimeBase::TimeT time)
{
  this->invocation_->operation_details ().ft_expiration_time (time);
}

TimeBase::TimeT
TAO_ClientRequestInfo::tao_ft_expiration_time (void) const
{
  return this->invocation_->operation_details ().ft_expiration_time ();
}

void
TAO_ClientRequestInfo::tao_ft_retention_id (CORBA::Long request_id)
{
  this->invocation_->operation_details ().ft_retention_id (request_id) ;
}

CORBA::Long
TAO_ClientRequestInfo::tao_ft_retention_id (void) const
{
  return this->invocation_->operation_details ().ft_retention_id ();
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_INTERCEPTORS == 1 */
