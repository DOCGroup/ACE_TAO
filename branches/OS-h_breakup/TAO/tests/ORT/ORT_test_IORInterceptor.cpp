// $Id$

#include "ORT_test_IORInterceptor.h"

ACE_RCSID (ORT,
           Server_IORInterceptor,
           "$Id$")


ORT_test_IORInterceptor::ORT_test_IORInterceptor ()
  : establish_count_ (0),
    components_establish_count_ (0)
{
  /// Constructor
}

char *
ORT_test_IORInterceptor::name (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup ("ORT_IORInterceptor");
}

void
ORT_test_IORInterceptor::destroy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
ORT_test_IORInterceptor::establish_components (
    PortableInterceptor::IORInfo_ptr /* info */
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ++this->establish_count_;

  ACE_DEBUG ((LM_DEBUG,
              "Establish_Components is invoked %d times till now\n",
              this->establish_count_));
}

void
ORT_test_IORInterceptor::
components_established (PortableInterceptor::IORInfo_ptr /* ior_info */)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ++this->components_establish_count_;

  ACE_DEBUG ((LM_DEBUG,
              "Components Established is invoked %d times till now\n",
              this->establish_count_));
}

void
ORT_test_IORInterceptor::adapter_manager_state_changed (
    PortableInterceptor::AdapterManagerId ,
    PortableInterceptor::AdapterState)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG,
              "The adapter_manager state has changed. \n"));

  // @@ Priyanka, why do you need the below "return" statement?
  return;
}

void
ORT_test_IORInterceptor:: adapter_state_changed (
    const PortableInterceptor::ObjectReferenceTemplateSeq &,
    PortableInterceptor::AdapterState)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // @@ Priyanka, why do you need the below "return" statement?
  return;
}
