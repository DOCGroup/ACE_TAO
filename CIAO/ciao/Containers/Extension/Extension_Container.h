// -*- C++ -*-

//=============================================================================
/**
 *  @file    Extension_Container.h
 *
 *  $Id$
 *
 *  Header file for CIAO's container implementations
 *
 *  @author Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef CIAO_EXTENSION_CONTAINER_H
#define CIAO_EXTENSION_CONTAINER_H

#include /**/ "ace/pre.h"

#include "ciao/Containers/Extension/Extension_Container_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/Servant_Base.h"
#include "ciao/Containers/Container_Base_T.h"
#include "ciao/Containers/Extension/Extension_ContainerC.h"
#include "ccm/CCM_HomeExecutorBaseC.h"
#include "ccm/CCM_EnterpriseComponentC.h"

#include <map>

namespace CIAO
{
  class ClientContainerInterceptorRegistration_Impl;
  class ServantContainerInterceptorRegistration_Impl;
  class ServerContainerInterceptorRegistration_Impl;
  class StubContainerInterceptorRegistration_Impl;

  typedef ::Components::HomeExecutorBase_ptr (*HomeFactory) (void);
  typedef ::PortableServer::Servant (*HomeServantFactory) (::Components::HomeExecutorBase_ptr p,
                                                           ::CIAO::Extension_Container_ptr c,
                                                           const char *ins_name);
  typedef ::Components::EnterpriseComponent_ptr (*ComponentFactory) (void);
  typedef ::PortableServer::Servant (*ComponentServantFactory) (::Components::EnterpriseComponent_ptr,
                                                                ::CIAO::Extension_Container_ptr,
                                                                const char *);

  class EXTENSION_CONTAINER_Export Extension_Container_i :
    public Container_i < ::CIAO::Extension_Container>
  {
  public:
    Extension_Container_i (CORBA::ORB_ptr o,
                           PortableServer::POA_ptr poa);

    virtual ~Extension_Container_i (void);

    /// Initialize the container with a name.
    virtual void init (const char *name);

    virtual void fini (void);

    /**
     * @brief Simply installing a home executor into the component.
     *
     * This operation install a home executor into the component.  It
     * requires the name of the DLLs to executor and the servant glue
     * code, and the entry points to the respective DLLs.  Currently,
     * we don't try to manage the lifetime of DLL objects, but we
     * should at some later point.
     *
     * @retval Home objref of the installed home.
     */
    /// Install a new home
    virtual Components::CCMHome_ptr install_home (const char *primary_artifact,
                                                  const char *entry_point,
                                                  const char *servant_artifact,
                                                  const char *servant_entrypoint,
                                                  const char *name);

    /// Install a new component
    virtual Components::CCMObject_ptr install_component (const char *primary_artifact,
                                                         const char *entry_point,
                                                         const char *servant_artifact,
                                                         const char *servant_entrypoint,
                                                         const char *name);

    // @{
    /// Inherited from extension context.
    Components::Cookie * install_service_reference (const char * service_id,
                                                    CORBA::Object_ptr objref);

    CORBA::Object_ptr uninstall_service_reference (Components::Cookie * ck);

    CORBA::Object_ptr resolve_service_reference (const char *service_id);
    // @}

    // @{
    /**
     * Getters for the Containter Portable Interceptor registration
     * Objects.
     */
    Components::ContainerPortableInterceptor::ClientContainerInterceptorRegistration_ptr
    get_client_interceptor_registration (void);

    Components::ContainerPortableInterceptor::ServantContainerInterceptorRegistration_ptr
    get_servant_interceptor_registration (void);

    Components::ContainerPortableInterceptor::ServerContainerInterceptorRegistration_ptr
    get_server_interceptor_registration (void);

    Components::ContainerPortableInterceptor::StubContainerInterceptorRegistration_ptr
    get_stub_interceptor_registration (void);
    // @}

  private:
    /// Not allowed to be
    Extension_Container_i (void);

    /// Administration of installed CCM services
    typedef std::map<const char *,
                     std::pair<Components::Cookie *, CORBA::Object_ptr>
                    > InstalledServices;
    InstalledServices installed_services_;

    // @{
    /// Caching of the COPI registration objects
    /// When a component want to register a Portable Interceptor, it'll
    /// need the registration object.
    ClientContainerInterceptorRegistration_Impl *client_copi_registration_;
    ServantContainerInterceptorRegistration_Impl *servant_copi_registration_;
    ServerContainerInterceptorRegistration_Impl *server_copi_registration_;
    StubContainerInterceptorRegistration_Impl *stub_copi_registration_;
    // @}
  };
}

#include /**/ "ace/post.h"

#endif /* CIAO_EXTENSION_CONTAINER_H */
