// $Id$

#include "orbsvcs/SSLIOP/SSLIOP_ORBInitializer.h"
#include "orbsvcs/SSLIOP/SSLIOP_Current.h"
#include "orbsvcs/SSLIOP/SSLIOP_Invocation_Interceptor.h"
//#include "SSLIOP_IORInterceptor.h"
#include "orbsvcs/SSLIOP/SSLIOP_CredentialsAcquirerFactory.h"

#include "orbsvcs/Security/SL3_SecurityCurrent.h"
#include "orbsvcs/Security/SL3_CredentialsCurator.h"

#include "orbsvcs/SSLIOPC.h"
#include "orbsvcs/CSIIOPC.h"

#include "tao/Exception.h"
#include "tao/PI/ORBInitInfo.h"
#include "tao/debug.h"

#include "ace/Auto_Ptr.h"


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO::SSLIOP::ORBInitializer::ORBInitializer (
  ::Security::QOP qop,
  CSIIOP::AssociationOptions csiv2_target_supports,
  CSIIOP::AssociationOptions csiv2_target_requires)
  : qop_ (qop),
    csiv2_target_supports_ (csiv2_target_supports),
    csiv2_target_requires_ (csiv2_target_requires)
{
}

void
TAO::SSLIOP::ORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr info)
{
  TAO_ORBInitInfo_var tao_info = TAO_ORBInitInfo::_narrow (info);

  if (CORBA::is_nil (tao_info.in ()))
    throw CORBA::INV_OBJREF ();

  // SSLIOP doesn't use the ORB Core until a request invocation occurs
  // so there is no problem in retrieving the ORB Core pointer in this
  // pre_init() method.
  TAO_ORB_Core *orb_core = tao_info->orb_core ();

  // Create the SSLIOP::Current object.
  // Note that a new SSLIOP::Current object is created for each ORB.
  // It wouldn't be very useful to share security context information
  // with another ORB that isn't configured with security, for
  // example.
  SSLIOP::Current_ptr current;
  ACE_NEW_THROW_EX (current,
                    TAO::SSLIOP::Current (orb_core),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));

  SSLIOP::Current_var ssliop_current = current;

  // Register the SSLIOP::Current object reference with the ORB.
  info->register_initial_reference ("SSLIOPCurrent", ssliop_current.in ());
}

void
TAO::SSLIOP::ORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr info)
{
  // Note we do not store the SSLIOP::Current as a class member since
  // we need to avoid potential problems where the same
  // SSLIOP::Current object is shared between ORBs.  Each ORB should
  // have its own unique SSLIOP::Current object.  By obtaining the
  // SSLIOP::Current object from the resolve_initial_references()
  // mechanism, we are guaranteed that the SSLIOP::Current object is
  // specific to the ORB being initialized since a new SSLIOP::Current
  // object is registered for each ORB in this ORBInitializer's
  // pre_init() method.

  // Create the SSLIOP secure invocation server request interceptor.
  PortableInterceptor::ServerRequestInterceptor_ptr si =
    PortableInterceptor::ServerRequestInterceptor::_nil ();
  ACE_NEW_THROW_EX (si,
                    TAO::SSLIOP::Server_Invocation_Interceptor(
                      info,
                      this->qop_,
                      this->get_tss_slot_id (info)),
                      CORBA::NO_MEMORY (CORBA::SystemException::_tao_minor_code (TAO::VMCID,
                                                                                 ENOMEM),
                                                                                 CORBA::COMPLETED_NO));

  PortableInterceptor::ServerRequestInterceptor_var si_interceptor =
    si;

  // Register the SSLIOP secure invocation server request interceptor
  // with the ORB.
  info->add_server_request_interceptor (si_interceptor.in ());

//   TAO_ORBInitInfo_var tao_info =
//     TAO_ORBInitInfo::_narrow (info
//);

//   if (CORBA::is_nil (tao_info.in ()))
//     ACE_THROW (CORBA::INV_OBJREF ());

//   TAO_ORB_Core * orb_core = tao_info->orb_core ();

//   // Create the SSLIOP IOR interceptor.
//   PortableInterceptor::IORInterceptor_ptr ii =
//     PortableInterceptor::IORInterceptor::_nil ();
//   ACE_NEW_THROW_EX (ii,
//                     TAO::SSLIOP::IORInterceptor (orb_core,
//                                                  this->csiv2_target_supports_,
//                                                  this->csiv2_target_requires_),
//                     CORBA::NO_MEMORY (
//                       CORBA::SystemException::_tao_minor_code (
//                         TAO::VMCID,
//                         ENOMEM),
//                       CORBA::COMPLETED_NO));

//   PortableInterceptor::IORInterceptor_var ior_interceptor =
//     ii;

//   // Register the SSLIOP IORInterceptor.
//   info->add_ior_interceptor (ior_interceptor.in ()
//);

  // Register the SSLIOP-specific vault with the
  // PrincipalAuthenticator.
  CORBA::Object_var obj =
    info->resolve_initial_references ("SecurityLevel3:SecurityManager");

  SecurityLevel3::SecurityManager_var manager =
    SecurityLevel3::SecurityManager::_narrow (obj.in ());

  SecurityLevel3::CredentialsCurator_var curator =
    manager->credentials_curator ();

  TAO::SL3::CredentialsCurator_var tao_curator =
    TAO::SL3::CredentialsCurator::_narrow (curator.in ());

  TAO::SSLIOP::CredentialsAcquirerFactory * factory;
  ACE_NEW_THROW_EX (factory,
                    TAO::SSLIOP::CredentialsAcquirerFactory,
                    CORBA::NO_MEMORY ());

  auto_ptr<TAO::SSLIOP::CredentialsAcquirerFactory> safe_factory;

  tao_curator->register_acquirer_factory ("SL3TLS", factory);

  (void) safe_factory.release ();  // CredentialsCurator now owns
                                   // CredentialsAcquirerFactory.
}

size_t
TAO::SSLIOP::ORBInitializer::get_tss_slot_id (
  PortableInterceptor::ORBInitInfo_ptr info)
{
  // Obtain the Security Service TSS slot ID from the SecurityCurrent
  // object.
  CORBA::Object_var obj =
    info->resolve_initial_references ("SecurityLevel3:SecurityCurrent");

  SecurityLevel3::SecurityCurrent_var current =
    SecurityLevel3::SecurityCurrent::_narrow (obj.in ());

  TAO::SL3::SecurityCurrent * security_current =
    dynamic_cast<TAO::SL3::SecurityCurrent *> (current.in ());

  if (security_current == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Unable to obtain TSS slot ID from "
                  "\"SecurityCurrent\" object.\n"));

      throw CORBA::INTERNAL ();
    }

  return security_current->tss_slot ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
