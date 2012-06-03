/**
 *  @file   ServerContainerInterceptorRegistration_Impl.h
 *
 *  $Id$
 *
 *  @author Marcel Smit (msmit@remedy.nl)
 */
#if !defined SERVERCONTAINERINTERCEPTORREGISTRATION_IMPL_H
#define CCM_SERVERCONTAINERINTERCEPTORREGISTRATION_IMPL_H

#include /**/ "ace/pre.h"

#include "ciao/Containers/Extension/Extension_Container_export.h"

#include "ccm/Extension/CCM_ServerContainerInterceptorC.h"
#include "ccm/Extension/CCM_ServerContainerInterceptorRegistrationC.h"
#include "ciao/Containers/Extension/Interceptor_Registration_T.h"

namespace CIAO
{
  /**
   * @class ServerContainerInterceptorRegistration_Impl
   *
   * @brief Implementation of the CCM_ServerContainerInterceptorRegistration interface
   *
   * Implementation of the registration process of Server Container
   * Interceptors
   *
   * Part of COPI (Container Portable Interceptor)
   */
  class EXTENSION_CONTAINER_Export ServerContainerInterceptorRegistration_Impl
    : public Interceptor_Registration_T<
        ::Components::ContainerPortableInterceptor::ServerContainerInterceptorRegistration,
        ::Components::ContainerPortableInterceptor::ServerContainerInterceptor>
  {
  public:
    ServerContainerInterceptorRegistration_Impl (void);
    virtual ~ServerContainerInterceptorRegistration_Impl (void);

    /// Registers a ServerContainerInterceptor.
    /// Return a cookie for this registration.
    virtual ::Components::Cookie *
    register_server_interceptor (
      ::Components::ContainerPortableInterceptor::ServerContainerInterceptor_ptr ci);

    /// Unregisters a ServerContainerInterceptor.
    /// Throws an InvalidRegistration exception when the
    /// ServerContainerInterceptor cannot be found.
    /// Returns the ServerContainerInterceptor
    virtual ::Components::ContainerPortableInterceptor::ServerContainerInterceptor_ptr
    unregister_server_interceptor (::Components::Cookie * cookie);
  };
}

#include /**/ "ace/post.h"

#endif /* CCM_SERVERCONTAINERINTERCEPTORREGISTRATION_IMPL_H */
