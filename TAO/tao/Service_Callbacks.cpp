//$Id$
#include "tao/Service_Callbacks.h"
#include "tao/PolicyC.h"

#if !defined (__ACE_INLINE__)
# include "tao/Service_Callbacks.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(tao, Service_Callbacks, "$Id$")


TAO_Service_Callbacks::~TAO_Service_Callbacks (void)
{
}


CORBA::Boolean
TAO_Service_Callbacks::select_profile (TAO_MProfile * /*mprofile*/,
                                       TAO_Profile *& /*pfile*/)
{
  return 0;
}

CORBA::Boolean
TAO_Service_Callbacks::reselect_profile (TAO_Stub * /*stub*/,
                                         TAO_Profile *& /*pfile*/)
{
  return 0;
}


CORBA::Boolean
TAO_Service_Callbacks::object_is_nil (CORBA::Object_ptr /* obj */)
{
  // We shouldn't be here at all. But in case if we are here,
  // something is wrong. So, we send a true for a is_nil () call
  return 1;
}

void
TAO_Service_Callbacks::reset_profile_flags (void)
{
  return;
}

CORBA::Policy_ptr
TAO_Service_Callbacks::service_create_policy (CORBA::PolicyType /*policy */,
                                              const CORBA::Any & /*val*/,
                                              CORBA::Environment & /*ACE_TRY_ENV*/)
{
  return CORBA::Policy::_nil ();
}

void
TAO_Service_Callbacks::service_context_list (TAO_Stub *& /*stub*/ ,
                                             IOP::ServiceContextList & /*service_list*/,
                                             CORBA::Environment & /*ACE_TRY_ENV*/)
{
  return;
}
