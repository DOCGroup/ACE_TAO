#include "Invocation_Base.h"
#include "Stub.h"
#include "operation_details.h"
#include "RequestInfo_Util.h"

ACE_RCSID (tao,
           Invocation_Base,
           "$Id$")

namespace TAO
{
  // Need to move many methods in here to the inlined files..
  Invocation_Base::Invocation_Base (TAO_Stub *stub,
                                    TAO_Operation_Details &details)
    : details_ (details)
    , forwarded_to_ (0)
    , stub_ (stub) // @@ Do we need to increment refcount
  {
  }

  Invocation_Base::~Invocation_Base (void)
  {
  }

  TAO_ORB_Core *
  Invocation_Base::orb_core (void) const
  {
    return this->stub_->orb_core ();
  }

  void
  Invocation_Base::forwarded_reference (CORBA::Object_ptr o)
  {
    this->forwarded_to_ = CORBA::Object::_duplicate (o);
  }

  CORBA::Object_ptr
  Invocation_Base::forwarded_reference (void)
  {
    return CORBA::Object::_duplicate (this->forwarded_to_.in ());
  }

  CORBA::Object_ptr
  Invocation_Base::steal_forwarded_reference (void)
  {
    return this->forwarded_to_._retn ();
  }

  bool
  Invocation_Base::is_forwarded (void) const
  {
    return (this->forwarded_to_.in () != 0);
  }

  char *
  Invocation_Base::operation_name (void)
  {
    return ACE_const_cast (char *, this->details_.opname ());
  }

  Dynamic::ParameterList *
  Invocation_Base::arguments (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    // Generate the argument list on demand.
    Dynamic::ParameterList *parameter_list =
      TAO_RequestInfo_Util::make_parameter_list (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK_RETURN (0);

    Dynamic::ParameterList_var safe_parameter_list = parameter_list;

    if (this->details_.parameter_list (*parameter_list) == false)
      ACE_THROW_RETURN (CORBA::MARSHAL (),
                        0);

    return safe_parameter_list._retn ();
  }

  Dynamic::ExceptionList *
  Invocation_Base::exceptions (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    // Generate the argument list on demand.
    Dynamic::ExceptionList *exception_list =
      TAO_RequestInfo_Util::make_exception_list (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK_RETURN (0);

    Dynamic::ExceptionList_var safe_exception_list = exception_list;

    if (this->details_.exception_list (*exception_list) == false)
      ACE_THROW_RETURN (CORBA::MARSHAL (),
                        0);

    return safe_exception_list._retn ();
  }

  CORBA::Octet
  Invocation_Base::sync_scope (void) const
  {
    return this->details_.response_flags ();
  }

  TAO_Service_Context &
  Invocation_Base::request_service_context (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    return this->details_.request_service_context ();
  }

  TAO_Service_Context &
  Invocation_Base::reply_service_context (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    return this->details_.request_service_context ();
  }
}
