// -*- C++ -*-

//=============================================================================
/**
 *  @file    Container_Base_T.h
 *
 *  $Id$
 *
 *  Header file for CIAO's container implementations
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 *  @author Gan Deng <dengg@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef CIAO_CONTAINER_BASE_T_H
#define CIAO_CONTAINER_BASE_T_H

#include /**/ "ace/pre.h"

#include "tao/LocalObject.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/PortableServer.h"
#include "ciao/Containers/Container_BaseC.h"
#include "tao/PortableServer/Servant_Base.h"

namespace CIAO
{
  /**
   * @class Container_i
   *
   * @brief Common container interface definition.
   *
   * Perhaps we can use local interface to define these interfaces as
   * we will also get reference counting automatically.
   */

  template <typename BASE>
  class Container_i :
    public virtual BASE,
    public virtual ::CORBA::LocalObject
  {
  public:
    Container_i (CORBA::ORB_ptr o, PortableServer::POA_ptr root_poa);

    virtual ~Container_i (void);

    /// Initialize the container with a name.
    virtual void init (const char *name);

    /// Cleanup method
    virtual void fini (void);

    /// Get component's POA.
    virtual PortableServer::POA_ptr the_POA (void);
    virtual PortableServer::POA_ptr the_port_POA (void);

    /// Install a new home
    virtual Components::CCMHome_ptr install_home (const char *primary_artifact,
                                                  const char *entry_point,
                                                  const char *servant_artifact,
                                                  const char *servant_entrypoint,
                                                  const char *name) = 0;

    /// Uninstall a servant for component or home.
    virtual void uninstall_home (Components::CCMHome_ptr homeref);

    virtual Components::CCMObject_ptr install_component (const char *primary_artifact,
                                                         const char *entry_point,
                                                         const char *servant_artifact,
                                                         const char *servant_entrypoint,
                                                         const char *name) = 0;

    virtual void set_attributes (CORBA::Object_ptr compref,
                                 const ::Components::ConfigValues & values);

    /// Activate component
    virtual void activate_component (Components::CCMObject_ptr compref);

    /// Passivate a component
    virtual void passivate_component (Components::CCMObject_ptr compref);

    /// Uninstall a given component
    virtual void uninstall_component (Components::CCMObject_ptr compref);

    /// Connect a local facet
    virtual ::Components::Cookie *
    connect_local_facet (::Components::CCMObject_ptr provider,
                         const char * provider_port,
                         ::Components::CCMObject_ptr user,
                         const char * user_port);

    /// Disconnect a local facet
    virtual void
    disconnect_local_facet (::Components::Cookie * cookie,
                            ::Components::CCMObject_ptr provider,
                            const char * provider_port,
                            ::Components::CCMObject_ptr user,
                            const char * user_port);

    virtual CORBA::Object_ptr install_servant (PortableServer::Servant objref,
                                               Container_Types::OA_Type type,
                                               PortableServer::ObjectId_out oid);

    /// Uninstall a servant for component.
    virtual void uninstall_servant (PortableServer::Servant objref,
                                    Container_Types::OA_Type type,
                                    PortableServer::ObjectId_out oid);

    virtual CORBA::Object_ptr get_objref (PortableServer::Servant p);

    /// Analog of the POA method that creates an object reference from
    /// an object id string.
    virtual CORBA::Object_ptr generate_reference (const char *obj_id,
                                          const char *repo_id,
                                          Container_Types::OA_Type t);

    /// Get a reference to the underlying ORB.
    CORBA::ORB_ptr the_ORB (void) const;

    virtual CORBA::Object_ptr resolve_service_reference (const char *service_id);

  protected:
    /// Reference to the ORB
    CORBA::ORB_var orb_;

    /// POA used as root
    PortableServer::POA_var root_poa_;

    /// POA within which all the components in this container will be
    /// activated.
    PortableServer::POA_var component_poa_;

    /// POA within which all the facets and receptacles will be
    /// activated.
    /**
     * Having two POA's allows us to associate different policies that
     * are distinct from the component.
     */
    PortableServer::POA_var facet_cons_poa_;

    /// Uninstall a servant for component or home.
    void uninstall (CORBA::Object_ptr objref, Container_Types::OA_Type t);

    void prepare_installation (const char *entity,
                            const char *primary_artifact,
                            const char *entry_point,
                            const char *servant_artifact,
                            const char *servant_entrypoint,
                            const char *name,
                            int open_mode,
                            ACE_DLL &executor_dll,
                            ACE_DLL &servant_dll);

  private:
    /// Not allowed to be used
    Container_i (void);

    /// Create POA for the component.
    /**
     * This is the POA that is returned to the component applications
     * if they need one.
     */
    void create_component_POA (const char *name,
                               PortableServer::POA_ptr root);

    /// Create POA for the facets and consumers alone.
    void create_facet_consumer_POA (const char *name,
                                    PortableServer::POA_ptr root);
  };
}

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ciao/Containers/Container_Base_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("ciao/Containers/Container_Base_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"

#endif /* CIAO_CONTAINER_BASE_T_H */
