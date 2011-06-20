// $Id$

#include "ciao/Containers/Extension/ClientContainerInterceptorRegistration_Impl.h"

namespace CIAO
{
  ClientContainerInterceptorRegistration_Impl::ClientContainerInterceptorRegistration_Impl (void)
  {
  }

  ClientContainerInterceptorRegistration_Impl::~ClientContainerInterceptorRegistration_Impl (void)
  {
  }

  ::Components::Cookie *
  ClientContainerInterceptorRegistration_Impl::register_client_interceptor (
      ::Components::ContainerPortableInterceptor::ClientContainerInterceptor_ptr ci)
  {
    return this->register_interceptor(ci);
  }

  ::Components::ContainerPortableInterceptor::ClientContainerInterceptor_ptr
  ClientContainerInterceptorRegistration_Impl::unregister_client_interceptor (
      ::Components::Cookie * cookie)
  {
    return this->unregister_interceptor(cookie);
  }
}
