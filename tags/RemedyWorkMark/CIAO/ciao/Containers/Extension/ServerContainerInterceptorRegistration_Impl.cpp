// $Id$

#include "ciao/Containers/Extension/ServerContainerInterceptorRegistration_Impl.h"

namespace CIAO
{
  ServerContainerInterceptorRegistration_Impl::ServerContainerInterceptorRegistration_Impl (void)
  {
  }

  ServerContainerInterceptorRegistration_Impl::~ServerContainerInterceptorRegistration_Impl (void)
  {
  }

  ::Components::Cookie *
  ServerContainerInterceptorRegistration_Impl::register_server_interceptor (
    ::Components::ContainerPortableInterceptor::ServerContainerInterceptor_ptr ci)
  {
    return this->register_interceptor(ci);
  }

  ::Components::ContainerPortableInterceptor::ServerContainerInterceptor_ptr
  ServerContainerInterceptorRegistration_Impl::unregister_server_interceptor (
      ::Components::Cookie * cookie)
  {
    return this->unregister_interceptor(cookie);
  }
}
