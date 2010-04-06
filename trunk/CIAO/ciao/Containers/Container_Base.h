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
 *  @author Gan Deng <dengg@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef CIAO_CONTAINER_BASE_H
#define CIAO_CONTAINER_BASE_H

#include /**/ "ace/pre.h"

#include "ciao/Containers/Container_Base_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Hash_Map_Manager_T.h"
#include "tao/LocalObject.h"
#include "tao/PortableServer/PortableServer.h"
#include "ccm/CCM_ContainerC.h"

#include "ciao/Containers/Container_BaseC.h"

namespace CIAO
{
  namespace Deployment
  {
    class CIAO_Container_i;
  }

  /**
   * @class Container_i
   *
   * @brief Common container interface definition.
   *
   * Perhaps we can use local interface to define these interfaces as
   * we will also get reference counting automatically.
   */
  class CIAO_CONTAINER_BASE_Export Container_i :
    public virtual Container,
    public virtual ::CORBA::LocalObject
  {
  public:
    Container_i (CORBA::ORB_ptr o, PortableServer::POA_ptr root_poa);
    Container_i (CORBA::ORB_ptr o, PortableServer::POA_ptr root_poa,
                 Deployment::CIAO_Container_i *container_impl);

    virtual ~Container_i (void);

    /// Initialize the container with a name.
    virtual void init (const char *name = 0,
                       const CORBA::PolicyList *more_policies = 0) = 0;

    /// Get component's POA.
    /**
     * This operation does *NOT* increase the reference count of the
     * POA. Look at the const qualifier in the method.
     */
    virtual PortableServer::POA_ptr the_POA (void);
    virtual PortableServer::POA_ptr the_port_POA (void);

    /// Install a new home
    virtual Components::CCMHome_ptr install_home (const char *primary_artifact,
                                                  const char *entry_point,
                                                  const char *servant_artifact,
                                                  const char *servant_entrypoint,
                                                  const char *name) = 0;

    /// Uninstall a servant for component or home.
    virtual void uninstall_home (Components::CCMHome_ptr homeref) = 0;

    virtual Components::CCMObject_ptr install_component (const char *primary_artifact,
                                                         const char *entry_point,
                                                         const char *servant_artifact,
                                                         const char *servant_entrypoint,
                                                         const char *name) = 0;

    virtual void uninstall_component (Components::CCMObject_ptr compref) = 0;

    virtual CORBA::Object_ptr get_objref (PortableServer::Servant p) = 0;

    virtual CORBA::Object_ptr install_servant (PortableServer::Servant objref,
                                               Container_Types::OA_Type type,
                                               PortableServer::ObjectId_out oid) = 0;

    /// Uninstall a servant for component.
    virtual void uninstall_servant (PortableServer::Servant objref,
                                    Container_Types::OA_Type type,
                                    PortableServer::ObjectId_out oid) = 0;

    /// Get a reference to the underlying ORB.
    CORBA::ORB_ptr the_ORB (void) const;

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

    PortableServer::POA_var home_servant_poa_;

    Deployment::CIAO_Container_i *container_impl_;

  private:
    /// Not allowed to be used
    Container_i (void);
  };
}

#if defined (__ACE_INLINE__)
# include "Container_Base.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* CIAO_CONTAINER_BASE_H */
