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
#include "ace/Hash_Map_Manager.h"
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

    virtual ::Components::Cookie * _ciao_specify_event_service (
        const char * event_name,
        const char * publisher_name,
        const char * service_name
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException));

    virtual ::Components::Cookie * _ciao_connect_event_consumer (
        ::Components::EventConsumerBase_ptr c,
        ::Components::Cookie * ck
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException));

    virtual ::Components::Cookie * _ciao_connect_event_supplier (
        ::Components::Cookie * ck
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException));

    virtual void _ciao_disconnect_event_consumer (
      ::Components::Cookie *ck
       ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((::CORBA::SystemException,
                       ::Components::InvalidName,
                       ::Components::InvalidConnection));

    virtual void _ciao_disconnect_event_supplier (
      ::Components::Cookie *ck
       ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((::CORBA::SystemException,
                       ::Components::InvalidName,
                       ::Components::InvalidConnection));

    virtual void _ciao_push_event (::Components::EventBase *ev,
                                   ::Components::Cookie *ck
                                   ACE_ENV_ARG_DECL)
                                 ACE_THROW_SPEC ((CORBA::SystemException));

  protected:

    ::Components::Cookie * connect_rt_event_consumer (
        ::Components::EventConsumerBase_ptr c,
        ::CIAO::EventServiceInfo service_info
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException));

    ::Components::Cookie * connect_rt_event_supplier (
        ::CIAO::EventServiceInfo service_info
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException));

    void CIAO::Container::create_rt_event_channel (void);

    // Reference to the ORB
    CORBA::ORB_var orb_;

    // Reference to the POA
    PortableServer::POA_var poa_;

    // Reference to the Root POA
    PortableServer::POA_var root_poa_;

    /// Map of event types
    ACE_Hash_Map_Manager<const char *, RtecEventComm::EventType, ACE_Null_Mutex>
      ciao_event_types_map_;

    /// Map of suppliers
    ACE_Hash_Map_Manager<const char *, RtecEventComm::EventSourceID, ACE_Null_Mutex>
      ciao_publishers_map_;

    /// Reference to the RT event channel (only created if needed; nil otherwise)
    RtecEventChannelAdmin::EventChannel_var ciao_rt_event_channel_;

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
