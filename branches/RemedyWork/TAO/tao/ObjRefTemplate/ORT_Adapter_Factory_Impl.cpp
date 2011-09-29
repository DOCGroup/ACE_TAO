// $Id$

#include "tao/ObjRefTemplate/ORT_Adapter_Factory_Impl.h"
#include "tao/ObjRefTemplate/ORT_Adapter_Impl.h"
#include "tao/PortableServer/Root_POA.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  ORT_Adapter *
  ORT_Adapter_Factory_Impl::create ()
  {
    ORT_Adapter_Impl * new_ort_adapter = 0;

    ACE_NEW_RETURN (new_ort_adapter,
                    TAO::ORT_Adapter_Impl,
                    0);

    return new_ort_adapter;
  }

  void
  ORT_Adapter_Factory_Impl::destroy (ORT_Adapter * adapter)
  {
    delete adapter;
  }

  int
  ORT_Adapter_Factory_Impl::Initializer (void)
  {
    TAO_Root_POA::ort_adapter_factory_name ("Concrete_ORT_Adapter_Factory");

    return ACE_Service_Config::process_directive (
          ace_svc_desc_ORT_Adapter_Factory_Impl);
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_STATIC_SVC_DEFINE (
  ORT_Adapter_Factory_Impl,
  ACE_TEXT ("Concrete_ORT_Adapter_Factory"),
  ACE_SVC_OBJ_T,
  &ACE_SVC_NAME (ORT_Adapter_Factory_Impl),
  ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
  0)

ACE_FACTORY_NAMESPACE_DEFINE (
  TAO_ORT,
  ORT_Adapter_Factory_Impl,
  TAO::ORT_Adapter_Factory_Impl)
