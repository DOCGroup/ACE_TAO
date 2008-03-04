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
 */
//=============================================================================


#ifndef NODEAPPLICATION_IMPL_H_
#define NODEAPPLICATION_IMPL_H_

#include "ace/Map_Manager.h"

#include "ciao/CCM_KeylessCCMHomeC.h"
#include "ciao/ComponentsC.h"
#include "Cdmw/CDMW_IDLC.h"
#include "Deployment/Deployment_NodeApplicationS.h"
#include "Deployment/Deployment_DeploymentPlanC.h"
#include "Deployment/DeploymentC.h"

#include "NodeApplication_Export.h"
#include "ComponentInstallation_Impl.h"
#include "ServerActivator_Impl.h"
#include "RedirectionService/RedirectionService.h"
/*
 *     // required variables list:
    // PROCESS_DESTINATION
    // This destination is specified in .cad file in a <processcollocation><destination> tag.
    const char* DESTINATION_NAME = "Process_1@CCMApplication";
    // COMPONENT_KIND
    // The component kind is specified in .ccd file, in <componentkind> tag.
    const CdmwDeployment::ComponentKindValue kind = CdmwDeployment::SESSION;
    // UUID
    // The uuid parameter corresponds to the implementation id specified
    // in .csd file, in <implementation id=” ”> tag.
    const char* uuid = "DCE:700dc518-0110-11ce-ac8f-0800090b5d3e";
    // ENTRYPOINT
    // The entrypoint parameter correspond to the operation to be called for Home creation.
    // It is specified in .csd file, in <implementation><code><entrypoint> tag.
    const char* entrypoint = "create_HelloProviderHome";
    // SERVANT_LIFETIME
    // The servant lifetime is specified in .ccd file, in <componentkind>…<servant lifetime=” “> tag.
    CdmwDeployment::ServantLifetimeValue lifetime = CdmwDeployment::COMPONENT_LIFETIME;
    // THREADING_POLICY
    // The threading policy is specified in .ccd file, in <threading policy=” “> tag.
    const CdmwDeployment::ThreadingPolicyValue threading = CdmwDeployment::MULTITHREAD;
    // HOME_REPOSITORY_ID
    // The home’s RepositoryId is specified in .ccd file, in <homerepid repid=” “> tag.
    const char* repositoryID = "IDL:acme.com/Example/HelloProviderHome:1.0";
    // HOME_SERVANT_CLASSNAME
    // The home’s servant classname is specified in .ccd file,
    // in <homefeatures><extension class=”HOME_SERVANT_CLASSNAME“ origin=”Cdmw”> tag.
    const char* servantHomeClassName = "Cdmw.CCM.CIF.CdmwExample.SessionHelloProviderHome_impl";
    // VALUETYPE_FACTORY_DEPENDENCIES
    // The valuetype factories are specified in .csd file,
    // in <implementation><dependency><valuetypefactory> tag.
    CdmwDeployment::ValuetypeFactoryDescriptionSeq dependenciesValueFactory(1L);
    CdmwDeployment::ValuetypeFactoryDescription desc;
    desc.repid = "IDL:acme.com/Example/SaySomethingElse:1.0";
    desc.factory_entrypoint = "createSaySomethingElseFactory";
    dependenciesValueFactory[0]=desc;
    // FAULT_TOLERANCE_REPLICATION_STYLE
    // The FaultTolerance replication style is always set to WARM_PASSIVE at this time.
    const CdmwDeployment::FTReplicationStyleValue styleFaultTolerance = CdmwDeployment::WARM_PASSIVE;
    // COMPONENT_NAME
    // The component’s name is specified in .cad file, as id in <componentinstantiation id=” “> tag.
    const char* componentName = "HelloProvider";
    // USES_PORT_TIMEOUTS
    // The uses port timeouts are specified in .cad file,
    // in <componentinstantiation><extension class=”USES_PORT_TIMEOUT“ origin=”Cdmw”> tags.
    CdmwCcmCif::UsesPortTimeouts timeouts(1L);
    CdmwCcmCif::UsesPortTimeout timeout;
    timeout.uses_name = "";
    timeout.timeout = 0;
    // RECORDING
    // If incoming requests to a component are to be recorded, it is specified in .cad file,
    // in <componentinstantiation><extension class=”RECORDING“ origin=”Cdmw”>true</extension> tags.
    const bool recording = true;
    // FAULT_TOLERANCE_GROUP_REF
    // The object group reference is retrieved according to the fault tolerant group’s destination,
    // specified in .cad file, in <ftcomponentgroup><destination> tag.
    //FT::ObjectGroup_var = FT::ObjectGroup::_nil();//"HelloProviderGroupName";
    // FACET_NAME
    // If component provides some facets they specified
    // in .ccd file, in <ports><provides providesname="hello_facet"> tag
    const char* facet = "hello_facet";
  */

namespace DAnCE
  {

  class NodeManager_Impl;

  class NodeApplication_Export NodeApplication_Impl : public virtual POA_Deployment::NodeApplication
    {
    public:
      NodeApplication_Impl (CORBA::ORB_ptr orb
                            , PortableServer::POA_ptr poa
                            , const Deployment::DeploymentPlan& plan
                            , RedirectionService & redirection
                            , const ACE_CString& node_name);

      virtual ~NodeApplication_Impl();

      //TODO Add throw specification
      void init()
      ACE_THROW_SPEC ( (
                         ::CORBA::SystemException
                       ));

      virtual void finishLaunch (
        const Deployment::Connections & providedReference,
        ::CORBA::Boolean start
      )
      ACE_THROW_SPEC ( (
                         ::CORBA::SystemException,
                         ::Deployment::StartError,
                         ::Deployment::InvalidConnection
                       ));

      virtual void start (
        ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ( (
                         ::CORBA::SystemException,
                         ::Deployment::StartError
                       ));

      Deployment::Connections * getAllConnections()
      ACE_THROW_SPEC ( (
                         ::CORBA::SystemException,
                         ::Deployment::InvalidProperty
                       ))
      ;

      //TODO Exception specification should be customized
      void initComponents()
      ACE_THROW_SPEC ( (
                         ::CORBA::SystemException,
                         ::Deployment::StartError,
                         ::Deployment::InvalidProperty
                       ));
    private:
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
        eComponent
      };

      EInstanceType getInstanceType (const Deployment::Properties& prop) const
      ACE_THROW_SPEC ( (
                         ::CORBA::SystemException,
                         ::Deployment::InvalidProperty
                       ));

      void createConfigValues (const Deployment::Properties& prop,
                               const ERequestType request,
                               Components::ConfigValues& cfg) const
      ACE_THROW_SPEC ( (
                         ::CORBA::SystemException,
                         ::Deployment::InvalidProperty
                       ));

      void createConfigValues(const Deployment::Properties& prop,
      Components::ConfigValues& cfg) const
  ACE_THROW_SPEC((
          ::CORBA::SystemException,
          ::Deployment::InvalidProperty
      ));

      void createContainer (unsigned int index)
      ACE_THROW_SPEC ( (
                         ::CORBA::SystemException,
                         ::Deployment::StartError,
                         ::Deployment::InvalidProperty
                       ));

      void createHome (unsigned int index)
      ACE_THROW_SPEC ( (
                         ::CORBA::SystemException,
                         ::Deployment::StartError,
                         ::Deployment::InvalidProperty
                       ));

      void createComponent (unsigned int index)
      ACE_THROW_SPEC ( (
                         ::CORBA::SystemException,
                         ::Deployment::StartError,
                         ::Deployment::InvalidProperty
                       ));

      Components::Cookie* connectReceptacle (Components::CCMObject_ptr inst,
                                             const ACE_CString& port_name,
                                             CORBA::Object_ptr facet)
      ACE_THROW_SPEC ( (
                         ::CORBA::SystemException,
                         ::Deployment::StartError,
                         ::Deployment::InvalidConnection
                       ));

      Components::Cookie* connectReceptacleExt (Components::CCMObject_ptr inst,
                                                const ACE_CString& port_name,
                                                CORBA::Object_ptr facet)
      ACE_THROW_SPEC ( (
                         ::CORBA::SystemException,
                         ::Deployment::StartError,
                         ::Deployment::InvalidConnection
                       ));

      void connectEmitter (Components::CCMObject_ptr inst,
                           const ACE_CString& port_name,
                           CORBA::Object_ptr consumer)
      ACE_THROW_SPEC ( (
                         ::CORBA::SystemException,
                         ::Deployment::StartError,
                         ::Deployment::InvalidConnection
                       ));

      void connectEmitterExt (Components::CCMObject_ptr inst,
                              const ACE_CString& port_name,
                              CORBA::Object_ptr consumer)
      ACE_THROW_SPEC ( (
                         ::CORBA::SystemException,
                         ::Deployment::StartError,
                         ::Deployment::InvalidConnection,
                         ::Components::InvalidName
                       ));

      Components::Cookie* connectPublisher (Components::CCMObject_ptr inst,
                                            const ACE_CString& port_name,
                                            CORBA::Object_ptr consumer)
      ACE_THROW_SPEC ( (
                         ::CORBA::SystemException,
                         ::Deployment::StartError,
                         ::Deployment::InvalidConnection
                       ));

      CORBA::ORB_var orb_;
      PortableServer::POA_var poa_;
      const Deployment::DeploymentPlan& plan_;

      ComponentInstallation_Impl* installation_;
      ServerActivator_Impl* activator_;

      typedef ACE_Map_Manager<ACE_CString, ::Components::Deployment::Container_var, ACE_Null_Mutex> TContainers;
      TContainers containers_;

      typedef ACE_Map_Manager<ACE_CString, Components::KeylessCCMHome_var, ACE_Null_Mutex> THomes;
      THomes    homes_;

      typedef ACE_Map_Manager<ACE_CString, Components::CCMObject_var,  ACE_Null_Mutex> TComponents;
      TComponents components_;

      RedirectionService & redirection_;

      ACE_CString node_name_;
    };
};
#endif /*NODEAPPLICATION_IMPL_H_*/
