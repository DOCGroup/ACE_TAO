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


#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/PortableServer.h"
#include "tao/PortableServer/Servant_Base.h"
#include "CCM_DeploymentS.h"
#include "CCM_ContainerC.h"
#include "ace/Hash_Map_Manager.h"

#include "CIAO_EventService_Factory.h"
#include "CIAO_EventsS.h"

namespace CIAO
{

  /**
   * @class Container
   *
   * @brief Common container interface definition.
   *
   * Perhaps we can use local interface to define these interfaces as
   * we will also get reference counting automatically.
   */
  class CIAO_CONTAINER_Export Container :
    public virtual POA_CIAO::ContainerEventService
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

    ContainerEventService_ptr
    CIAO::Container::get_container_events_ref (
        ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);

    // ContainerEventService implementation

    /// A factory method for Consumer_Config objects.
    Consumer_Config_ptr create_consumer_config (
        EventServiceType type
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException));

    /// A factory method for Supplier_Config objects.
    Supplier_Config_ptr create_supplier_config (
        EventServiceType type
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException));

    /// Connect an event sink.
    virtual void connect_event_consumer (
        Consumer_Config_ptr consumer_config
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException));

    /// Connect an event source.
    virtual void connect_event_supplier (
        Supplier_Config_ptr supplier_config
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException));

    /// Disconnect event sink.
    virtual void disconnect_event_consumer (
        const char * connection_id
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        Components::InvalidName,
        Components::InvalidConnection));

    /// Disconnect event source.
    virtual void disconnect_event_supplier (
        const char * connection_id
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        Components::InvalidName,
        Components::InvalidConnection));

    /// Push an event.
    virtual void push_event (
        Components::EventBase * ev,
        const char * connection_id
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException));

  protected:

    // Reference to the ORB
    CORBA::ORB_var orb_;

    // Reference to the POA
    PortableServer::POA_var poa_;

    /// Factory for event services.
    EventService_Factory * event_service_factory_;

    /// Mapping of components to an event service.
    ACE_Hash_Map_Manager_Ex<ACE_CString,
                            EventServiceBase *,
                            ACE_Hash<ACE_CString>,
                            ACE_Equal_To<ACE_CString>,
                            ACE_Null_Mutex> event_service_map_;

    /// Object reference to the events interface
    ContainerEventService_var events_ref_;

  };

  class CIAO_CONTAINER_Export Session_Container : public Container
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

    // Get an object reference to a component or home from the servant.
    CORBA::Object_ptr get_objref (PortableServer::Servant p
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
