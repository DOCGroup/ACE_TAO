#include "orbsvcs/Log_Macros.h"
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

TAO::SSLIOP::ORBInitializer::ORBInitializer (::Security::QOP qop)
  : qop_ (qop)
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
  // TAO-Specific way to get to the ORB Core (and thus, the ORB).
  TAO_ORBInitInfo_var tao_info =
    TAO_ORBInitInfo::_narrow (info);

  CORBA::ORB_var orb = CORBA::ORB::_duplicate(tao_info->orb_core()->orb());

  if (CORBA::is_nil(orb.in()))
    {
      throw CORBA::INTERNAL ();
    }

  // Note we do not store the SSLIOP::Current as a class member since
  // we need to avoid potential problems where the same
  // SSLIOP::Current object is shared between ORBs.  Each ORB should
  // have its own unique SSLIOP::Current object.  By obtaining the
  // SSLIOP::Current object from the resolve_initial_references()
  // mechanism, we are guaranteed that the SSLIOP::Current object is
  // specific to the ORB being initialized since a new SSLIOP::Current
  // object is registered for each ORB in this ORBInitializer's
  // pre_init() method.

  // Create two interceptors, one for collocated and one for remote calls
  size_t const slot = this->get_tss_slot_id (info);
  // Create the SSLIOP secure remote invocation server request interceptor.
  PortableInterceptor::ServerRequestInterceptor_ptr si_remote =
    PortableInterceptor::ServerRequestInterceptor::_nil ();
  ACE_NEW_THROW_EX (si_remote,
                    TAO::SSLIOP::Server_Invocation_Interceptor(
                      info,
                      this->qop_,
                      slot,
                      false), // Remote calls
                      CORBA::NO_MEMORY (CORBA::SystemException::_tao_minor_code (TAO::VMCID,
                                                                                 ENOMEM),
                                                                                 CORBA::COMPLETED_NO));
  PortableInterceptor::ServerRequestInterceptor_var si_remote_interceptor = si_remote;

  // Create the SSLIOP secure collocated invocation server request interceptor.
  PortableInterceptor::ServerRequestInterceptor_ptr si_collocated =
    PortableInterceptor::ServerRequestInterceptor::_nil ();
  ACE_NEW_THROW_EX (si_collocated,
                    TAO::SSLIOP::Server_Invocation_Interceptor(
                      info,
                      this->qop_,
                      slot,
                      true), // Collocated calls
                      CORBA::NO_MEMORY (CORBA::SystemException::_tao_minor_code (TAO::VMCID,
                                                                                 ENOMEM),
                                                                                 CORBA::COMPLETED_NO));
  PortableInterceptor::ServerRequestInterceptor_var si_collocated_interceptor = si_collocated;

  PortableInterceptor::ORBInitInfo_3_1_var info_3_1 =
    PortableInterceptor::ORBInitInfo_3_1::_narrow(info);

  if (CORBA::is_nil(info_3_1.in()))
    {
      throw CORBA::INTERNAL ();
    }

  CORBA::Any remote_proc_mode_as_any;
  CORBA::Any collocated_proc_mode_as_any;
  remote_proc_mode_as_any <<= PortableInterceptor::REMOTE_ONLY;
  collocated_proc_mode_as_any <<= PortableInterceptor::LOCAL_ONLY;
  CORBA::PolicyList policy_list_remote (1);
  CORBA::PolicyList policy_list_collocated (1);
  policy_list_remote.length (1);
  policy_list_collocated.length (1);
  policy_list_remote[0] =
    orb->create_policy (PortableInterceptor::PROCESSING_MODE_POLICY_TYPE,
                        remote_proc_mode_as_any);
  policy_list_collocated[0] =
    orb->create_policy (PortableInterceptor::PROCESSING_MODE_POLICY_TYPE,
                        collocated_proc_mode_as_any);

  // Register the SSLIOP secure invocation server request interceptor
  // with the ORB.
  info_3_1->add_server_request_interceptor_with_policy (si_remote_interceptor.in (), policy_list_remote);

  // Register the SSLIOP secure invocation server request interceptor
  // with the ORB.
  info_3_1->add_server_request_interceptor_with_policy (si_collocated_interceptor.in (), policy_list_collocated);

  policy_list_remote[0]->destroy ();
  policy_list_collocated[0]->destroy ();
  policy_list_remote[0] = CORBA::Policy::_nil ();
  policy_list_collocated[0] = CORBA::Policy::_nil ();

  // Register the SSLIOP-specific vault with the
  // PrincipalAuthenticator.
  CORBA::Object_var obj =
    info_3_1->resolve_initial_references ("SecurityLevel3:SecurityManager");

  SecurityLevel3::SecurityManager_var manager =
    SecurityLevel3::SecurityManager::_narrow (obj.in ());

  SecurityLevel3::CredentialsCurator_var curator =
    manager->credentials_curator ();

  TAO::SL3::CredentialsCurator_var tao_curator =
    TAO::SL3::CredentialsCurator::_narrow (curator.in ());

  TAO::SSLIOP::CredentialsAcquirerFactory * factory = 0;
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
      ORBSVCS_DEBUG ((LM_DEBUG,
                  "Unable to obtain TSS slot ID from "
                  "\"SecurityCurrent\" object.\n"));

      throw CORBA::INTERNAL ();
    }

  return security_current->tss_slot ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
