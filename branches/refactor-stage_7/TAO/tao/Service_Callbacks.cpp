#include "Service_Callbacks.h"
#include "ORB_Constants.h"
#include "Exception.h"
#include "Environment.h"

#if !defined (__ACE_INLINE__)
# include "tao/Service_Callbacks.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID (tao,
           Service_Callbacks,
           "$Id$")

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


CORBA::Boolean
TAO_Service_Callbacks::is_profile_equivalent (const TAO_Profile *,
                                              const TAO_Profile *)
{
  return 0;
}

CORBA::ULong
TAO_Service_Callbacks::hash_ft (TAO_Profile *,
                                CORBA::ULong )
{
  return 0;
}

int
TAO_Service_Callbacks::raise_comm_failure (
    TAO_GIOP_Invocation * /*invoke*/,
    TAO_Profile * /*profile*/
    ACE_ENV_ARG_DECL)
{
  ACE_THROW_RETURN (CORBA::COMM_FAILURE (
      CORBA::SystemException::_tao_minor_code (
          TAO_INVOCATION_RECV_REQUEST_MINOR_CODE,
          -1),
      CORBA::COMPLETED_MAYBE),
      2);
}

int
TAO_Service_Callbacks::raise_transient_failure (
    TAO_GIOP_Invocation * /*invoke*/,
    TAO_Profile * /*profile*/
    ACE_ENV_ARG_DECL)
{
  ACE_THROW_RETURN (CORBA::TRANSIENT (
      CORBA::SystemException::_tao_minor_code (
          TAO_INVOCATION_RECV_REQUEST_MINOR_CODE,
          -1),
      CORBA::COMPLETED_MAYBE),
      2);
}
