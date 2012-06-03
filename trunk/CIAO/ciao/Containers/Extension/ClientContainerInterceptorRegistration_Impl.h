/**
 *  @file   ClientContainerInterceptorRegistration_Impl.h
 *
 *  $Id$
 *
 *  @author Marcel Smit (msmit@remedy.nl)
 */
#if !defined CLIENTCONTAINERINTERCEPTORREGISTRATION_IMPL_H
#define CLIENTCONTAINERINTERCEPTORREGISTRATION_IMPL_H

#include /**/ "ace/pre.h"

#include "ciao/Containers/Extension/Extension_Container_export.h"

#include "ccm/Extension/CCM_ClientContainerInterceptorC.h"
#include "ccm/Extension/CCM_ClientContainerInterceptorRegistrationC.h"
#include "ciao/Containers/Extension/Interceptor_Registration_T.h"

namespace CIAO
{
  /**
   * @class ClientContainerInterceptorRegistration_Impl
   *
   * @brief Implementation of the CCM_ClientContainerInterceptorRegistration interface
   *
   * Implementation of the registration process of Client Container
   * Interceptors
   *
   * Part of COPI (Container Portable Interceptor)
   */
  class EXTENSION_CONTAINER_Export ClientContainerInterceptorRegistration_Impl
    : public Interceptor_Registration_T<
      ::Components::ContainerPortableInterceptor::ClientContainerInterceptorRegistration,
      ::Components::ContainerPortableInterceptor::ClientContainerInterceptor>
  {
  public:
    ClientContainerInterceptorRegistration_Impl (void);
    virtual ~ClientContainerInterceptorRegistration_Impl (void);

    /// Registers a ClientContainerInterceptor.
    /// Returns a cookie for this registration.
    virtual ::Components::Cookie *
    register_client_interceptor (
      ::Components::ContainerPortableInterceptor::ClientContainerInterceptor_ptr ci);

    /// Unregisters a ClientContainerInterceptor.
    /// Throws an InvalidRegistration exception when the
    /// ClientContainerInterceptor cannot be found.
    /// Returns the ClientContainerInterceptor
    virtual ::Components::ContainerPortableInterceptor::ClientContainerInterceptor_ptr
    unregister_client_interceptor (::Components::Cookie * cookie);
  };
}

#include /**/ "ace/post.h"

#endif /* CLIENTCONTAINERINTERCEPTORREGISTRATION_IMPL_H */
