// $Id$

#include "ObjectReferenceTemplate_Adapter_Factory_Impl.h"
#include "ObjectReferenceTemplate_Adapter_Impl.h"
#include "tao/PortableServer/POA.h"

ACE_RCSID (ORT,
           ObjectReferenceTemplate_Adapter_Factory_Impl,
           "$Id$")

TAO_ObjectReferenceTemplate_Adapter_Factory_Impl::
  ~TAO_ObjectReferenceTemplate_Adapter_Factory_Impl (void)
{
}

TAO::ObjectReferenceTemplate_Adapter *
TAO_ObjectReferenceTemplate_Adapter_Factory_Impl::create (
    ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO::ObjectReferenceTemplate_Adapter_Impl * new_ort_adapter;
  ACE_NEW_THROW_EX (new_ort_adapter,
                    TAO::ObjectReferenceTemplate_Adapter_Impl,
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  return new_ort_adapter;
}

void
TAO_ObjectReferenceTemplate_Adapter_Factory_Impl::destroy (
    TAO::ObjectReferenceTemplate_Adapter * adapter
    ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  delete adapter;
}

// *********************************************************************

// Initialization and registration of dynamic service object.

int
TAO_ObjectReferenceTemplate_Adapter_Factory_Impl::Initializer (void)
{
  TAO_POA::ort_adapter_factory_name ("Concrete_ObjectReferenceTemplate_Adapter_Factory"
    );

  return
    ACE_Service_Config::process_directive (
        ace_svc_desc_TAO_ObjectReferenceTemplate_Adapter_Factory_Impl
      );
}

ACE_STATIC_SVC_DEFINE (
    TAO_ObjectReferenceTemplate_Adapter_Factory_Impl,
    ACE_TEXT ("Concrete_ObjectReferenceTemplate_Adapter_Factory"),
    ACE_SVC_OBJ_T,
    &ACE_SVC_NAME (TAO_ObjectReferenceTemplate_Adapter_Factory_Impl),
    ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
    0
  )

ACE_FACTORY_DEFINE (TAO_ORT, TAO_ObjectReferenceTemplate_Adapter_Factory_Impl)
