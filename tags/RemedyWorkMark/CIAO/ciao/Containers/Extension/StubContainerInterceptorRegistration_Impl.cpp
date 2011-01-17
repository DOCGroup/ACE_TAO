// $Id$

#include "ciao/Containers/Extension/StubContainerInterceptorRegistration_Impl.h"

namespace CIAO
{
  StubContainerInterceptorRegistration_Impl::StubContainerInterceptorRegistration_Impl (void)
  {
  }

  StubContainerInterceptorRegistration_Impl::~StubContainerInterceptorRegistration_Impl (void)
  {
  }

  ::Components::Cookie *
  StubContainerInterceptorRegistration_Impl::register_stub_interceptor (
    ::Components::ContainerPortableInterceptor::StubContainerInterceptor_ptr ci)
  {
    return this->register_interceptor(ci);
  }

  ::Components::ContainerPortableInterceptor::StubContainerInterceptor_ptr
  StubContainerInterceptorRegistration_Impl::unregister_stub_interceptor (
      ::Components::Cookie * cookie)
  {
    return this->unregister_interceptor(cookie);
  }
}
