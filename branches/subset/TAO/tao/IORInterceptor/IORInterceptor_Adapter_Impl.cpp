// $Id$

#include "IORInterceptor_Adapter_Impl.h"
#include "tao/ORB_Core.h"

ACE_RCSID (IORInterceptor,
           IORInterceptor_Adapter_Impl,
           "$Id$")

TAO_IORInterceptor_Adapter_Impl::~TAO_IORInterceptor_Adapter_Impl (void)
{
}

void
TAO_IORInterceptor_Adapter_Impl::tao_iorinterceptor_release (
      PortableInterceptor::IORInterceptor_ptr p
    )
{
  CORBA::release (p);
}

PortableInterceptor::Interceptor_ptr 
TAO_IORInterceptor_Adapter_Impl::tao_iorinterceptor (
    TAO_IORInterceptor_List::TYPE & container,
    size_t index
  )
{
  return container[index];
}

void 
TAO_IORInterceptor_Adapter_Impl::tao_add_iorinterceptor (
    TAO_IORInterceptor_List * list,
    TAO_IORInterceptor_List::TYPE & container,
    PortableInterceptor::IORInterceptor_ptr interceptor
    ACE_ENV_ARG_DECL
  )
{
  size_t index = list->add_interceptor_helper (interceptor
                                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  container[index] =
    PortableInterceptor::IORInterceptor::_duplicate (interceptor);
}

void 
TAO_IORInterceptor_Adapter_Impl::tao_iorinterceptor_destroy (
    TAO_IORInterceptor_List::TYPE & container,
    size_t ilen
    ACE_ENV_ARG_DECL
  )
{
  container[ilen]->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
}

// *********************************************************************

// Initialization and registration of dynamic service object.

int
TAO_IORInterceptor_Adapter_Impl::Initializer (void)
{
  TAO_ORB_Core::iorinterceptor_adapter_name (
      "Concrete_IORInterceptor_Adapter"
    );

  return 
    ACE_Service_Config::process_directive (
        ace_svc_desc_TAO_IORInterceptor_Adapter_Impl
      );
}

ACE_STATIC_SVC_DEFINE (
    TAO_IORInterceptor_Adapter_Impl,
    ACE_TEXT ("Concrete_IORInterceptor_Adapter"),
    ACE_SVC_OBJ_T,
    &ACE_SVC_NAME (TAO_IORInterceptor_Adapter_Impl),
    ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
    0
  )

ACE_FACTORY_DEFINE (TAO_IORInterceptor, TAO_IORInterceptor_Adapter_Impl)

