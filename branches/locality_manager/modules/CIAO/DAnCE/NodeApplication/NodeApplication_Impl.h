// -*- C++ -*-

//=============================================================================
/**
 *  @file    NodeApplication_Impl.h
 *
 *  $Id$
 *
 * @brief  Implementation of Deployment::NodeApplication
 *
 * @author Erwin Gottlieb <eg@prismtech.com>
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 */
//=============================================================================


#ifndef NODEAPPLICATION_IMPL_H_
#define NODEAPPLICATION_IMPL_H_

#include "NodeApplication_Export.h"

#include "ace/Containers_T.h"
#include "tao/ORB.h"
#include "tao/Object.h"
#include "ccm/CCM_KeylessCCMHomeC.h"
#include "ccm/ComponentsC.h"
#include "ciao/ComponentServer/CIAO_ServerActivator_Impl.h"
#include "orbsvcs/orbsvcs/CosNamingC.h"

#include "ccm/ComponentServer/CCM_ComponentServerC.h"
#include "Deployment/Deployment_NodeApplicationS.h"
#include "Deployment/Deployment_DeploymentPlanC.h"
#include "Deployment/DeploymentC.h"
#include "DAnCE/Deployment_common.h"

#include <map>

namespace DAnCE
{

  class NodeManager_Impl;

  class NodeApplication_Export NodeApplication_Impl :
    public virtual POA_Deployment::NodeApplication
  {
  public:
    NodeApplication_Impl (CORBA::ORB_ptr orb,
                          PortableServer::POA_ptr poa,
                          const ::Deployment::DeploymentPlan& plan,
                          const ACE_CString& node_name,
                          const PROPERTY_MAP &properties);

    virtual ~NodeApplication_Impl();

    virtual void finishLaunch (const ::Deployment::Connections & providedReference,
                               ::CORBA::Boolean start);

    virtual void start (void);

    ::Deployment::Connections * getAllConnections(void);

    void init_components(void);

    void configuration_complete_components (void);

    void passivate_components (void);

    void remove_components (void);

    enum ERequestType
    {
      eCreateComponentServer,
      eCreateContainer,
      eInstallHome,
      eCreateComponentWithConfigValues
    };

    enum EInstanceType
    {
      eHome,
      eComponent,
      eHomedComponent,
      eContainer,
      eComponentServer,
      eInvalid
    };

    enum EComponentState
    {
      eUninstalled,
      eInstalled,
      eConfigured,
      eActive,
      ePassive,
      eRemoved,
      eInvalidState
    };

  protected:
    void init(void);

    struct Container;

    struct Instance
    {
      Instance (EInstanceType type = eInvalid,
                Container *cont = 0,
                CORBA::ULong idd = 0,
                CORBA::ULong mdd = 0) :
        state (eUninstalled),
        type (type), idd_idx (idd), mdd_idx (mdd), home(0),
        container (cont)
      {
      }

      EComponentState state;
      EInstanceType type;
      CORBA::ULong idd_idx;
      CORBA::ULong mdd_idx;
      CORBA::Object_var ref;
      Instance *home;
      Container *container;
    };

    typedef ACE_Array<Instance> INSTANCES;
    typedef ACE_Array<Instance *> INSTANCE_PTRS;

    struct Container
    {
      INSTANCES homes;
      INSTANCES components;
      ::Deployment::Properties properties;
      Components::Deployment::Container_var ref;
    };

    typedef ACE_Array<Container> CONTAINERS;

    struct ComponentServer
    {
      ComponentServer (void) : properties (0)
      {
      }

      Instance instance;
      CONTAINERS containers;
      const ::Deployment::Properties *properties;
      Components::Deployment::ComponentServer_var ref;
    };

    typedef ACE_Array<ComponentServer> COMPONENTSERVERS;

    EInstanceType get_instance_type (const ::Deployment::Properties& prop) const;

    typedef std::map <std::string, int> ColocationMap;

    /// Creates colocation groups for all components in this node's deployment.
    /// Resulting map has mappings of component ids to appropriate server.
    /// As a postcondition, the size of the servers_ array is increased appropriately.
    ColocationMap create_colocation_groups (void);

    void create_component_server (size_t index);

    void create_container (size_t server, size_t container);

    void install_home (Container &cont, Instance &inst);

    void install_component (Container &cont, Instance &inst);

    void install_homed_component (Container &cont, Instance &inst);

    void store_instance_ior (Instance &inst);

    bool is_local_facet (const ::Deployment::PlanConnectionDescription& conn);

    Components::Cookie* connect_receptacle (const ::Deployment::PlanConnectionDescription& conn,
                                            Components::CCMObject_ptr facet,
                                            const ACE_CString &facet_name,
                                            CORBA::Object_ptr receptacle,
                                            const ACE_CString& receptacle_name,
                                            CIAO::Deployment::Container_ptr cont);

    Components::Cookie* connect_receptacle_ext (Components::CCMObject_ptr inst,
                                              const ACE_CString& port_name,
                                              CORBA::Object_ptr facet);

    void connect_emitter (Components::CCMObject_ptr inst,
                         const ACE_CString& port_name,
                         CORBA::Object_ptr consumer);

    void connect_emitter_ext (Components::CCMObject_ptr inst,
                            const ACE_CString& port_name,
                            CORBA::Object_ptr consumer);

    Components::Cookie* connect_publisher (Components::CCMObject_ptr inst,
                                           const ACE_CString& port_name,
                                           CORBA::Object_ptr consumer);

    CORBA::ORB_var orb_;

    PortableServer::POA_var poa_;

    const ::Deployment::DeploymentPlan& plan_;

    auto_ptr<CIAO::Deployment::CIAO_ServerActivator_i>  activator_;

    ACE_CString node_name_;

    PROPERTY_MAP properties_;

    COMPONENTSERVERS servers_;

    INSTANCE_PTRS instances_;

    CosNaming::NamingContext_var instance_nc_;
  };
}
#endif /*NODEAPPLICATION_IMPL_H_*/
