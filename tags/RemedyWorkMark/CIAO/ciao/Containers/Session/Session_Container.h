// -*- C++ -*-

//=============================================================================
/**
 *  @file    Session_Container.h
 *
 *  $Id$
 *
 *  Header file for CIAO's container implementations
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 *  @author Gan Deng <dengg@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef CIAO_SESSION_CONTAINER_H
#define CIAO_SESSION_CONTAINER_H

#include /**/ "ace/pre.h"

#include "ciao/Containers/Session/Session_Container_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ciao/Containers/Container_Base_T.h"
#include "ciao/Containers/Session/Session_ContainerC.h"
#include "ccm/CCM_HomeExecutorBaseC.h"
#include "ccm/CCM_EnterpriseComponentC.h"

namespace CIAO
{
  typedef ::Components::HomeExecutorBase_ptr (*HomeFactory) (void);
  typedef ::PortableServer::Servant (*HomeServantFactory) (::Components::HomeExecutorBase_ptr p,
                                                           ::CIAO::Session_Container_ptr c,
                                                           const char *ins_name);
  typedef ::Components::EnterpriseComponent_ptr (*ComponentFactory) (void);
  typedef ::PortableServer::Servant (*ComponentServantFactory) (::Components::EnterpriseComponent_ptr,
                                                                ::CIAO::Session_Container_ptr,
                                                                const char *);

  typedef ACE_Hash_Map_Manager_Ex<ACE_CString,
                                  HomeFactory,
                                  ACE_Hash<ACE_CString>,
                                  ACE_Equal_To<ACE_CString>,
                                  ACE_Null_Mutex>
    HOMECREATOR_FUNCPTR_MAP;

  typedef ACE_Hash_Map_Manager_Ex<ACE_CString,
                                  HomeServantFactory,
                                  ACE_Hash<ACE_CString>,
                                  ACE_Equal_To<ACE_CString>,
                                  ACE_Null_Mutex>
  HOMESERVANTCREATOR_FUNCPTR_MAP;

  typedef ACE_Hash_Map_Manager_Ex<ACE_CString,
                                  ComponentFactory,
                                  ACE_Hash<ACE_CString>,
                                  ACE_Equal_To<ACE_CString>,
                                  ACE_Null_Mutex>
    COMPONENTCREATOR_FUNCPTR_MAP;

  typedef ACE_Hash_Map_Manager_Ex<ACE_CString,
                                  ComponentServantFactory,
                                  ACE_Hash<ACE_CString>,
                                  ACE_Equal_To<ACE_CString>,
                                  ACE_Null_Mutex>
  COMPONENTSERVANTCREATOR_FUNCPTR_MAP;


  class SESSION_CONTAINER_Export Session_Container_i :
    public Container_i < ::CIAO::Session_Container>
  {
  public:
    Session_Container_i (CORBA::ORB_ptr o,
                         PortableServer::POA_ptr poa);

    virtual ~Session_Container_i (void);

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

    virtual void uninstall_home (Components::CCMHome_ptr homeref);

    /// Install a new component
    virtual Components::CCMObject_ptr install_component (const char *primary_artifact,
                                                         const char *entry_point,
                                                         const char *servant_artifact,
                                                         const char *servant_entrypoint,
                                                         const char *name);

    /// Connect a local facet
    virtual void connect_local_facet (::Components::CCMObject_ptr provider,
                                      const char * provider_port,
                                      ::Components::CCMObject_ptr user,
                                      const char * user_port);

    /// Disconnect a local facet
    virtual void disconnect_local_facet (::Components::CCMObject_ptr provider,
                                         const char * provider_port,
                                         ::Components::CCMObject_ptr user,
                                         const char * user_port);

    /// Set attributes on a component
    virtual void set_attributes (CORBA::Object_ptr compref,
                                 const ::Components::ConfigValues & values);

    /// Activate component
    virtual void activate_component (Components::CCMObject_ptr compref);

    /// Passivate a component
    virtual void passivate_component (Components::CCMObject_ptr compref);

    /// Uninstall a component
    virtual void uninstall_component (Components::CCMObject_ptr compref);

    /// Uninstall a servant
    virtual void uninstall_servant (PortableServer::Servant objref,
                                    Container_Types::OA_Type type,
                                    PortableServer::ObjectId_out oid);

    /// Install a servant for component or home.
    virtual CORBA::Object_ptr install_servant (PortableServer::Servant p,
                                               Container_Types::OA_Type type,
                                               PortableServer::ObjectId_out oid);

    /// Get an object reference to a component or home from the servant.
    virtual CORBA::Object_ptr get_objref (PortableServer::Servant p);

    /// Uninstall a servant for component or home.
    void uninstall (CORBA::Object_ptr objref, Container_Types::OA_Type t);

    /// Uninstall a servant for component or home.
    void uninstall (PortableServer::Servant svt, Container_Types::OA_Type t);

    /// Analog of the POA method that creates an object reference from
    /// an object id string.
    CORBA::Object_ptr generate_reference (const char *obj_id,
                                          const char *repo_id,
                                          Container_Types::OA_Type t);

    /// Return the servant activator factory that activates the
    /// servants for facets and consumers.
    ::CIAO::Servant_Activator_ptr ports_servant_activator (void);

  private:
    /// Not allowed to be
    Session_Container_i (void);
  };
}

#if defined (__ACE_INLINE__)
# include "Session_Container.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* CIAO_SESSION_CONTAINER_H */
