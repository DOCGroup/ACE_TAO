// $Id$

#include "IORInterceptor_Adapter_Factory_Impl.h"
#include "tao/ORB_Core.h"

ACE_RCSID (IORInterceptor,
           IORInterceptor_Adapter_Factory_Impl,
           "$Id$")

TAO_IORInterceptor_Adapter_Factory_Impl::~TAO_IORInterceptor_Adapter_Factory_Impl (void)
{
}

TAO_IORInterceptor_Adapter * TAO_IORInterceptor_Adapter_Factory_Impl::create (
    ACE_ENV_SINGLE_ARG_DECL_NOT_USED
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IORInterceptor_Adapter_Impl * new_iorinterceptor_adapter;
  ACE_NEW_RETURN (new_iorinterceptor_adapter,
                  TAO_IORInterceptor_Adapter_Impl,
                  0);
  return new_iorinterceptor_adapter;
}


// *********************************************************************

// Initialization and registration of dynamic service object.

int
TAO_IORInterceptor_Adapter_Factory_Impl::Initializer (void)
{
  TAO_ORB_Core::iorinterceptor_adapter_factory_name (
      "Concrete_IORInterceptor_Adapter_Factory"
    );

  return
    ACE_Service_Config::process_directive (
        ace_svc_desc_TAO_IORInterceptor_Adapter_Factory_Impl
      );
}

ACE_STATIC_SVC_DEFINE (
    TAO_IORInterceptor_Adapter_Factory_Impl,
    ACE_TEXT ("Concrete_IORInterceptor_Adapter_Factory"),
    ACE_SVC_OBJ_T,
    &ACE_SVC_NAME (TAO_IORInterceptor_Adapter_Factory_Impl),
    ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
    0
  )

ACE_FACTORY_DEFINE (TAO_IORInterceptor, TAO_IORInterceptor_Adapter_Factory_Impl)
