#include "ORT_test_IORInterceptor.h"
#include "ObjectReferenceFactory.h"


ACE_RCSID (ORT,
           ORT_test_IORInterceptor,
           "$Id$")


ORT_test_IORInterceptor::ORT_test_IORInterceptor (void)
  : establish_count_ (0),
    components_establish_count_ (0)
{
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
  ACE_ASSERT (this->establish_count_ > 0
              && this->components_establish_count_ > 0
              && this->establish_count_ == this->components_establish_count_);
}

void
ORT_test_IORInterceptor::establish_components (
    PortableInterceptor::IORInfo_ptr /* info */
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ++this->establish_count_;

  ACE_DEBUG ((LM_DEBUG,
              "establish_components() has been invoked %d times\n",
              this->establish_count_));
}

void
ORT_test_IORInterceptor::components_established (
    PortableInterceptor::IORInfo_ptr info
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ++this->components_establish_count_;

  ACE_DEBUG ((LM_DEBUG,
              "components_established() has been invoked %d times\n",
              this->establish_count_));


  // Swap out the current ObjectReferenceFactory with our own.

  // Save a copy of the current ObjectReferenceFactory.
  PortableInterceptor::ObjectReferenceFactory_var old_orf =
    info->current_factory (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  PortableInterceptor::ObjectReferenceFactory * tmp;
  ACE_NEW_THROW_EX (tmp,
                    ObjectReferenceFactory (old_orf.in ()),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK;

  PortableInterceptor::ObjectReferenceFactory_var orf = tmp;

  info->current_factory (orf.in ()
                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
ORT_test_IORInterceptor::adapter_manager_state_changed (
    PortableInterceptor::AdapterManagerId,
    PortableInterceptor::AdapterState
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG,
              "The AdapterManager state has changed.\n"));
}

void
ORT_test_IORInterceptor:: adapter_state_changed (
    const PortableInterceptor::ObjectReferenceTemplateSeq &,
    PortableInterceptor::AdapterState
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}
