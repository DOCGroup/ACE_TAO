// -*- C++ -*-

//=============================================================================
/**
 *  @file    Container_Base.h
 *
 *  $Id$
 *
 *  Header file for CIAO's container implementations
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 */
//=============================================================================


#ifndef CIAO_CONTAINER_BASE_H
#define CIAO_CONTAINER_BASE_H
#include /**/ "ace/pre.h"

#include "tao/ORB.h"
#include "tao/PortableServer/PortableServer.h"
#include "tao/PortableServer/Servant_Base.h"
#include "CCM_ContainerC.h"
#include "CCM_DeploymentC.h"
#include "ace/Active_Map_Manager.h"
#include "CIAO_Events.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace CIAO
{

  // Forward declarations
  struct EventServiceInfo;
  class RTEventServiceSupplier_impl;
  class RTEventServiceConsumer_impl;

  /**
   * @class Container
   *
   * @brief Common container interface definition.
   *
   * Perhaps we can use local interface to define these interfaces as
   * we will also get reference counting automatically.
   */
  class CIAO_SERVER_Export Container
  {
  public:
    Container (CORBA::ORB_ptr o);

    virtual ~Container (void) = 0;

    /// Get the containing POA.  This operation does *NOT*
    /// increase the reference count of the POA.
    virtual PortableServer::POA_ptr _ciao_the_POA (void);

    /// Get a reference to the underlying ORB.
    virtual CORBA::ORB_ptr _ciao_the_ORB (void);

    /// Initialize the container with a name.
    virtual int init (const char *name = 0,
                      const CORBA::PolicyList *more_policies = 0
                      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException)) = 0;

    /// Install a new home
    virtual Components::CCMHome_ptr ciao_install_home
      (const char *exe_dll_name,
       const char *exe_entrypt,
       const char *sv_dll_name,
       const char *sv_entrypt
       ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::Deployment::UnknownImplId,
                       Components::Deployment::ImplEntryPointNotFound,
                       Components::Deployment::InstallationFailure)) = 0;

    // Uninstall a servant for component or home.
    virtual void ciao_uninstall_home (Components::CCMHome_ptr homeref
                                      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException)) = 0;

    // Events methods

    CIAO_Events::Consumer_Config_ptr _ciao_create_event_consumer_config (
        const char * service_type
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException));

    CIAO_Events::Supplier_Config_ptr _ciao_create_event_supplier_config (
        const char * service_type
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException));

    virtual void _ciao_connect_event_consumer (
        CIAO_Events::Consumer_Config_ptr consumer_config
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException));

    virtual void _ciao_connect_event_supplier (
        CIAO_Events::Supplier_Config_ptr supplier_config
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException));

    virtual void _ciao_disconnect_event_consumer (
        CIAO_Events::CONNECTION_ID connection_id
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((::CORBA::SystemException,
                       ::Components::InvalidName,
                       ::Components::InvalidConnection));

    virtual void _ciao_disconnect_event_supplier (
        CIAO_Events::CONNECTION_ID connection_id
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((::CORBA::SystemException,
                       ::Components::InvalidName,
                       ::Components::InvalidConnection));

    virtual void _ciao_push_event (::Components::EventBase *ev,
                                   CIAO_Events::CONNECTION_ID connection_id
                                   ACE_ENV_ARG_DECL)
                                 ACE_THROW_SPEC ((CORBA::SystemException));

  protected:

    // Reference to the ORB
    CORBA::ORB_var orb_;

    // Reference to the POA
    PortableServer::POA_var poa_;

    ACE_Hash_Map_Manager<CIAO_Events::CONNECTION_ID,
                         CIAO_Events::EventServiceBase *,
                         ACE_Null_Mutex> event_service_map_;

    ACE_Hash_Map_Manager<CIAO_Events::CONNECTION_ID,
                         CIAO_Events::EventServiceInfo,
                         ACE_Null_Mutex> event_info_map_;

    CIAO_Events::Events_Manager events_manager_;
  };

  class CIAO_SERVER_Export Session_Container : public Container
  {
  public:
    Session_Container (CORBA::ORB_ptr o);

    virtual ~Session_Container (void);

    /// Initialize the container with a name.
    virtual int init (const char *name = 0,
                      const CORBA::PolicyList *more_policies = 0
                      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

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
    virtual Components::CCMHome_ptr ciao_install_home
      (const char *exe_dll_name,
       const char *exe_entrypt,
       const char *sv_dll_name,
       const char *sv_entrypt
       ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::Deployment::UnknownImplId,
                       Components::Deployment::ImplEntryPointNotFound,
                       Components::Deployment::InstallationFailure));

    // Uninstall a servant for component or home.
    virtual void ciao_uninstall_home (Components::CCMHome_ptr homeref
                                      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    // Install a servant for component or home.
    CORBA::Object_ptr install_servant (PortableServer::Servant p
                                       ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    // Install a component servant.
    CORBA::Object_ptr install_component (PortableServer::Servant p,
                                         PortableServer::ObjectId_out oid
                                         ACE_ENV_ARG_DECL_WITH_DEFAULTS)

      ACE_THROW_SPEC ((CORBA::SystemException));

    // Get an object reference to a component or home from the servant.
    CORBA::Object_ptr get_objref (PortableServer::Servant p
                                  ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    // Uninstall a servant for component or home.
    void uninstall (CORBA::Object_ptr objref
                    ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    // Uninstall a servant for component or home.
    void uninstall (PortableServer::Servant svt
                    ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    // Uninstall a servant for component or home.
    void uninstall_component (CORBA::Object_ptr objref,
                              PortableServer::ObjectId_out oid
                              ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

  protected:
    long number_;

    static ACE_Atomic_Op <ACE_Thread_Mutex, long> serial_number_;
  };

  typedef ::Components::HomeExecutorBase_ptr (*HomeFactory) (void);
  typedef ::PortableServer::Servant (*ServantFactory)
    (::Components::HomeExecutorBase_ptr p,
     ::CIAO::Session_Container *c
     ACE_ENV_ARG_DECL_WITH_DEFAULTS);

}

#if defined (__ACE_INLINE__)
# include "Container_Base.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* CIAO_CONTAINER_BASE_H */
