// -*- C++ -*-
//
// $Id$

#include "SSLIOP_ORBInitializer.h"

ACE_RCSID (TAO_SSLIOP,
           SSLIOP_ORBInitializer,
           "$Id$")

#include "SSLIOP_Current.h"
#include "SSLIOP_Invocation_Interceptor.h"
#include "orbsvcs/SSLIOPC.h"

#include "orbsvcs/Security/Security_Current.h"

#include "tao/Exception.h"
#include "tao/ORBInitInfo.h"

TAO_SSLIOP_ORBInitializer::TAO_SSLIOP_ORBInitializer (int no_protection)
  : no_protection_ (no_protection)
{
}

void
TAO_SSLIOP_ORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr info
    TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_ENV_ARG_DEFN;


  CORBA::String_var orb_id = info->orb_id (ACE_TRY_ENV);
  ACE_CHECK;

  size_t tss_slot = this->get_tss_slot_id (info,
                                           ACE_TRY_ENV);
  ACE_CHECK;

  // Create the SSLIOP::Current object.
  // Note that a new SSLIOP::Current object is created for each ORB.
  // It wouldn't be very used to share security context information
  // with another ORB that isn't configured with security, for
  // example.
  SSLIOP::Current_ptr current = SSLIOP::Current::_nil ();
  ACE_NEW_THROW_EX (current,
                    TAO_SSLIOP_Current (tss_slot, orb_id.in ()),
                    CORBA::NO_MEMORY (
                      CORBA_SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK;

  SSLIOP::Current_var ssliop_current = current;

  // Register the SSLIOP::Current object reference with the ORB.
  info->register_initial_reference ("SSLIOPCurrent",
                                    ssliop_current.in (),
                                    ACE_TRY_ENV);
  ACE_CHECK;
}

void
TAO_SSLIOP_ORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr info
    TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_ENV_ARG_DEFN;

  // Note we do not store the SSLIOP::Current as a class member since
  // we need to avoid potential problems where the same
  // SSLIOP::Current object is shared between ORBs.  Each ORB should
  // have its own unique SSLIOP::Current object.  By obtaining the
  // SSLIOP::Current object from the resolve_initial_references()
  // mechanism, we are guaranteed that the SSLIOP::Current object is
  // specific to the ORB being initialized since a new SSLIOP::Current
  // object is registered for each ORB in this ORBInitializer's
  // pre_init() method.

  CORBA::Object_var obj =
    info->resolve_initial_references ("SSLIOPCurrent", ACE_TRY_ENV);
  ACE_CHECK;

  SSLIOP::Current_var ssliop_current =
    SSLIOP::Current::_narrow (obj.in (), ACE_TRY_ENV);
  ACE_CHECK;

  // Create the SSLIOP secure invocation server request interceptor.
  PortableInterceptor::ServerRequestInterceptor_ptr si =
    PortableInterceptor::ServerRequestInterceptor::_nil ();
  ACE_NEW_THROW_EX (si,
                    TAO_SSLIOP_Server_Invocation_Interceptor (
                      ssliop_current.in (),
                      this->no_protection_),
                    CORBA::NO_MEMORY (
                      CORBA_SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK;

  PortableInterceptor::ServerRequestInterceptor_var si_interceptor =
    si;

  // Register the SSLIOP secure invocation server request interceptor
  // with the ORB.
  info->add_server_request_interceptor (si_interceptor.in (),
                                        ACE_TRY_ENV);
  ACE_CHECK;
}

size_t
TAO_SSLIOP_ORBInitializer::get_tss_slot_id (
  PortableInterceptor::ORBInitInfo_ptr info,
  CORBA::Environment &ACE_TRY_ENV)
{
  // Obtain the Security Service TSS slot ID from the SecurityCurrent
  // object.
  CORBA::Object_var obj =
    info->resolve_initial_references ("SecurityCurrent",
                                      ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  SecurityLevel2::Current_var current =
    SecurityLevel2::Current::_narrow (obj.in (),
                                      ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  TAO_Security_Current *security_current =
    ACE_dynamic_cast (TAO_Security_Current *,
                      current.in ());

  if (security_current == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Unable to obtain TSS slot ID from "
                  "\"SecurityCurrent\" object.\n"));

      ACE_THROW_RETURN (CORBA::INTERNAL (), 0);
    }

  return security_current->tss_slot ();
}
