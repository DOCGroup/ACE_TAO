// $Id$

#include "ORT_test_IORInterceptor.h"
#include "ObjectReferenceFactory.h"

#include "tao/ORB_Constants.h"

ORT_test_IORInterceptor::ORT_test_IORInterceptor (void)
  : establish_count_ (0),
    components_establish_count_ (0)
{
}

char *
ORT_test_IORInterceptor::name (void)
{
  return CORBA::string_dup ("ORT_IORInterceptor");
}

void
ORT_test_IORInterceptor::destroy (void)
{
  ACE_ASSERT (this->establish_count_ > 0
              && this->components_establish_count_ > 0
              && this->establish_count_ == this->components_establish_count_);
}

void
ORT_test_IORInterceptor::establish_components (
    PortableInterceptor::IORInfo_ptr /* info */)
{
  ++this->establish_count_;

  ACE_DEBUG ((LM_DEBUG,
              "establish_components() has been invoked %d times\n",
              this->establish_count_));
}

void
ORT_test_IORInterceptor::components_established (
    PortableInterceptor::IORInfo_ptr info)
{
  ++this->components_establish_count_;

  ACE_DEBUG ((LM_DEBUG,
              "components_established() has been invoked %d times\n",
              this->establish_count_));


  // Swap out the current ObjectReferenceFactory with our own.

  // Save a copy of the current ObjectReferenceFactory.
  PortableInterceptor::ObjectReferenceFactory_var old_orf =
    info->current_factory ();

  PortableInterceptor::ObjectReferenceFactory * tmp;
  ACE_NEW_THROW_EX (tmp,
                    ObjectReferenceFactory (old_orf.in ()),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));

  PortableInterceptor::ObjectReferenceFactory_var orf = tmp;

  info->current_factory (orf.in ());
}

void
ORT_test_IORInterceptor::adapter_manager_state_changed (
    const char * id,
    PortableInterceptor::AdapterState)
{
  ACE_DEBUG ((LM_DEBUG,
              "The AdapterManager [id=%C] state has changed.\n", id));
}

void
ORT_test_IORInterceptor:: adapter_state_changed (
    const PortableInterceptor::ObjectReferenceTemplateSeq &,
    PortableInterceptor::AdapterState)
{
}
