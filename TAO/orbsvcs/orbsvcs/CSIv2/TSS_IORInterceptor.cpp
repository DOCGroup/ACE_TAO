// $Id$


#include "TSS_IORInterceptor.h"


ACE_RCSID (CSIv2,
           TSS_IORInterceptor,
           "$Id$")


TAO::TSS_IORInterceptor::TSS_IORInterceptor (void)
{
}

char *
TAO::TSS_IORInterceptor::name (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup ("TAO::TSS_IORInterceptor");
}

void
TAO::TSS_IORInterceptor::destroy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
TAO::TSS_IORInterceptor::establish_components (
    PortableInterceptor::IORInfo_ptr info
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  IOP::TaggedComponent tc;

  tc.tag = IOP::TAG_CSI_SEC_MECH_LIST;

  CSIIOP::CompoundSecMechList component;

  component.stateful = 0;  // Only stateless TSS currently supported.

  CSIIOP::CompoundSecMechanisms & mechanisms = component.mechanisms;

  // Only SSLIOP is supported at this point in time.
  mechanisms.length (1);

  CSIIOP::CompoundSecMech & mech = mechanisms[0];

  mech.target_requires =
    CSIIOP::Security::Integrity
    | Security::Confidentiality;

  // SSLIOP-specific TaggedComponent information.
  IOP::TaggedComponent & transport_mech = mech.transport_mech;
  transport_mech.tag = IOP::TAG_TLS_SEC_TRANS;

  CSIIOP::TLS_SEC_TRANS tls_component;
  tls_component.target_supports = ;
  tls_component.target_requires = ;
  tls_component_addresses = ;

  mech.as_context_mech;
  mech.sas_context_mech;

  // TAO currently only supports CSIv2 over IIOP.
  info->add_ior_component_to_profile (tc,
                                      IOP::TAG_INTERNET_IOP
                                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO::TSS_IORInterceptor::components_established (
    PortableInterceptor::IORInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
TAO::TSS_IORInterceptor::adapter_manager_state_changed (
    PortableInterceptor::AdapterManagerId,
    PortableInterceptor::AdapterState
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
TAO::TSS_IORInterceptor::adapter_state_changed (
    const PortableInterceptor::ObjectReferenceTemplateSeq &,
    PortableInterceptor::AdapterState
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}
