// $Id$

#include "tao/IORInterceptor/IORInterceptor_Adapter_Impl.h"
#include "tao/IORInterceptor/IORInterceptor_Adapter_Factory_Impl.h"

#include "tao/ORB_Core.h"

ACE_RCSID (IORInterceptor,
           IORInterceptor_Adapter_Factory_Impl,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_IORInterceptor_Adapter_Factory_Impl::~
  TAO_IORInterceptor_Adapter_Factory_Impl (void)
{
}

TAO_IORInterceptor_Adapter *
TAO_IORInterceptor_Adapter_Factory_Impl::create (void)
{
  TAO_IORInterceptor_Adapter_Impl *nia = 0;
  ACE_NEW_THROW_EX (nia,
                    TAO_IORInterceptor_Adapter_Impl (),
                    CORBA::NO_MEMORY ());

  return nia;
}


// *********************************************************************

// Initialization and registration of dynamic service object.

int
TAO_IORInterceptor_Adapter_Factory_Impl::Initializer (void)
{
  TAO_ORB_Core::iorinterceptor_adapter_factory_name (
    "Concrete_IORInterceptor_Adapter_Factory");

  return
    ACE_Service_Config::process_directive (
    ace_svc_desc_TAO_IORInterceptor_Adapter_Factory_Impl);
}

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_STATIC_SVC_DEFINE (
    TAO_IORInterceptor_Adapter_Factory_Impl,
    ACE_TEXT ("Concrete_IORInterceptor_Adapter_Factory"),
    ACE_SVC_OBJ_T,
    &ACE_SVC_NAME (TAO_IORInterceptor_Adapter_Factory_Impl),
    ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
    0
  )

ACE_FACTORY_DEFINE (TAO_IORInterceptor, TAO_IORInterceptor_Adapter_Factory_Impl)

