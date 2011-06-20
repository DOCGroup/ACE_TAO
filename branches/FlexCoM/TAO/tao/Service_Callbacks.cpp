// $Id$

#include "tao/Service_Callbacks.h"
#include "tao/ORB_Constants.h"
#include "tao/SystemException.h"
#include "tao/Invocation_Utils.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Service_Callbacks::~TAO_Service_Callbacks (void)
{
}

CORBA::Boolean
TAO_Service_Callbacks::select_profile (const TAO_MProfile & /*mprofile*/,
                                       TAO_Profile *& /*pfile*/)
{
  return false;
}


CORBA::Boolean
TAO_Service_Callbacks::object_is_nil (CORBA::Object_ptr /* obj */)
{
  // We shouldn't be here at all. But in case if we are here,
  // something is wrong. So, we send a true for a is_nil () call
  return true;
}


TAO_Service_Callbacks::Profile_Equivalence
TAO_Service_Callbacks::is_profile_equivalent (const TAO_Profile *,
                                              const TAO_Profile *)
{
  return DONT_KNOW;
}

CORBA::ULong
TAO_Service_Callbacks::hash_ft (TAO_Profile *,
                                CORBA::ULong )
{
  return 0;
}

CORBA::Boolean
TAO_Service_Callbacks::is_permanent_forward_condition (const CORBA::Object_ptr,
                                                       const TAO_Service_Context &) const
{
  return false;
}

TAO::Invocation_Status
TAO_Service_Callbacks::raise_comm_failure (
    IOP::ServiceContextList &,
    TAO_Profile * /*profile*/)
{
  throw ::CORBA::COMM_FAILURE (
    CORBA::SystemException::_tao_minor_code (
      TAO_INVOCATION_RECV_REQUEST_MINOR_CODE,
      -1),
    CORBA::COMPLETED_MAYBE);
}

TAO::Invocation_Status
TAO_Service_Callbacks::raise_transient_failure (
    IOP::ServiceContextList &,
    TAO_Profile * /*profile*/ )
{
  throw ::CORBA::TRANSIENT (
    CORBA::SystemException::_tao_minor_code (
      TAO_INVOCATION_RECV_REQUEST_MINOR_CODE,
      -1),
    CORBA::COMPLETED_MAYBE);
}

TAO_END_VERSIONED_NAMESPACE_DECL
