// -*- C++ -*-

//=============================================================================
/**
 *  @file    NodeApplication_Impl.h
 *
 *  $Id$
 *
 * @Brief  Implementation of Deployment::NodeApplication
 *
 * @author Erwin Gottlieb <eg@prismtech.com>
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 */
//=============================================================================


#ifndef NODEAPPLICATION_IMPL_H_
#define NODEAPPLICATION_IMPL_H_

#include "NodeApplication_Export.h"

#include "ace/Map_Manager.h"
#include "ace/Containers_T.h"
#include "tao/ORB.h"
#include "tao/Object.h"
#include "ccm/CCM_KeylessCCMHomeC.h"
#include "ccm/ComponentsC.h"
#include "RedirectionService/RedirectionService.h"
#include "ciao/ComponentServer/CIAO_ServerActivator_Impl.h"
//#include "Cdmw/CDMW_IDLC.h"

#include "ComponentServer/ComponentServerC.h"
#include "Deployment/Deployment_NodeApplicationS.h"
#include "Deployment/Deployment_DeploymentPlanC.h"
#include "Deployment/DeploymentC.h"
#include "Deployment/Deployment_common.h"

//#include "ComponentInstallation_Impl.h"
namespace DAnCE
{

  class NodeManager_Impl;

  class NodeApplication_Export NodeApplication_Impl : public virtual POA_Deployment::NodeApplication
  {
  public:
    NodeApplication_Impl (CORBA::ORB_ptr orb, 
                          PortableServer::POA_ptr poa,
                          const Deployment::DeploymentPlan& plan,
                          RedirectionService & redirection,
                          const ACE_CString& node_name,
                          PROPERTY_MAP &properties);

    virtual ~NodeApplication_Impl();

    virtual void finishLaunch (const Deployment::Connections & providedReference,
                               ::CORBA::Boolean start);

    virtual void start ();

    Deployment::Connections * getAllConnections();

    //TODO Exception specification should be customized
    void init_components();

  private:
    //TODO Add throw specification
    void init();

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
      eInvalid
    };
    
    struct Instance
    {
      bool configured;
      EInstanceType type;
      CORBA::ULong idd_idx;
      CORBA::ULong mdd_idx;
    };
    
    typedef ACE_Array<Instance> INSTANCES;
    
    struct Container
    {
      INSTANCES homes;
      INSTANCES components;
      Deployment::Properties properties;
    };
    
    typedef ACE_Array<Container> CONTAINERS;
    
    struct ComponentServer
    {
      CONTAINERS containers;
      Deployment::Properties properties;
    };
    
    typedef ACE_Array<ComponentServer> COMPONENTSERVERS;
    
    COMPONENTSERVERS servers_;
    
    EInstanceType get_instance_type (const Deployment::Properties& prop) const;

    void create_config_values (const Deployment::Properties& prop,
                               const ERequestType request,
                               Components::ConfigValues& cfg) const;

    void create_config_values(const Deployment::Properties& prop,
                            Components::ConfigValues& cfg) const;

    void create_container (unsigned int index);

    void create_home (unsigned int index);

    void create_component (unsigned int index);

    Components::Cookie* connect_receptacle (Components::CCMObject_ptr inst,
                                           const ACE_CString& port_name,
                                           CORBA::Object_ptr facet);

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
    const Deployment::DeploymentPlan& plan_;

    //ComponentInstallation_Impl* installation_;
    CIAO::Deployment::CIAO_ServerActivator_i* activator_;

    typedef ACE_Map_Manager<ACE_CString, ::Components::Deployment::Container_var, ACE_Null_Mutex> TContainers;
    TContainers containers_;

    typedef ACE_Map_Manager<ACE_CString, Components::KeylessCCMHome_var, ACE_Null_Mutex> THomes;
    THomes    homes_;

    typedef ACE_Map_Manager<ACE_CString, Components::CCMObject_var,  ACE_Null_Mutex> TComponents;
    TComponents components_;

    RedirectionService & redirection_;

    ACE_CString node_name_;
    
    PROPERTY_MAP &properties_;
  };
};
#endif /*NODEAPPLICATION_IMPL_H_*/
