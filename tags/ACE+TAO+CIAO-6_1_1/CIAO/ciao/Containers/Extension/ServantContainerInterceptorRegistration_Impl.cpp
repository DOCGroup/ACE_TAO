// $Id$

#include "ciao/Containers/Extension/ServantContainerInterceptorRegistration_Impl.h"

namespace CIAO
{
  ServantContainerInterceptorRegistration_Impl::ServantContainerInterceptorRegistration_Impl (void)
  {
  }

  ServantContainerInterceptorRegistration_Impl::~ServantContainerInterceptorRegistration_Impl (void)
  {
  }

  ::Components::Cookie *
  ServantContainerInterceptorRegistration_Impl::register_servant_interceptor (
    ::Components::ContainerPortableInterceptor::ServantContainerInterceptor_ptr ci)
  {
    return this->register_interceptor(ci);
  }

  ::Components::ContainerPortableInterceptor::ServantContainerInterceptor_ptr
  ServantContainerInterceptorRegistration_Impl::unregister_servant_interceptor (
      ::Components::Cookie * cookie)
  {
    return this->unregister_interceptor(cookie);
  }
}
