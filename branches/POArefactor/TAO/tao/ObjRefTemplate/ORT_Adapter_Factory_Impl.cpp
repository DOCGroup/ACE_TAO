// $Id$

#include "ORT_Adapter_Factory_Impl.h"
#include "ORT_Adapter_Impl.h"
#include "tao/PortableServer/POA.h"

ACE_RCSID (ORT,
           ORT_Adapter_Factory_Impl,
           "$Id$")

namespace TAO
{
  ORT_Adapter_Factory_Impl::~ORT_Adapter_Factory_Impl (void)
  {
  }

  ORT_Adapter *
  ORT_Adapter_Factory_Impl::create (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    ORT_Adapter_Impl * new_ort_adapter = 0;

    ACE_NEW_THROW_EX (new_ort_adapter,
                      TAO::ORT_Adapter_Impl,
                      CORBA::NO_MEMORY ());
    ACE_CHECK_RETURN (0);

    return new_ort_adapter;
  }

  void
  ORT_Adapter_Factory_Impl::destroy (ORT_Adapter * adapter
                                     ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    delete adapter;
  }

  int
  ORT_Adapter_Factory_Impl::Initializer (void)
  {
    TAO_POA::ort_adapter_factory_name ("Concrete_ORT_Adapter_Factory"
      );

    return
      ACE_Service_Config::process_directive (
          ace_svc_desc_ORT_Adapter_Factory_Impl
        );
  }

  ACE_STATIC_SVC_DEFINE (
    ORT_Adapter_Factory_Impl,
    ACE_TEXT ("Concrete_ORT_Adapter_Factory"),
    ACE_SVC_OBJ_T,
    &ACE_SVC_NAME (ORT_Adapter_Factory_Impl),
    ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
    0)

    ACE_FACTORY_DEFINE (TAO_ORT, ORT_Adapter_Factory_Impl)
}
