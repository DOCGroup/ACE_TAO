// $Id$

#ifndef DEPLOYMENTPLANFRAMEWORK_VISITOR_H
#define DEPLOYMENTPLANFRAMEWORK_VISITOR_H

#include <memory>
#include <stack>
#include <map>
#include "CQML/CQML.h"

// Xerces includes
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>

// Utility includes
#include "Utils/XercesString.h"
#include "Utils/Utils.h"
#include "DeploymentPlanFramework/DeploymentPlanFramework_Export.h"
#include "DeploymentPlanFramework/Injector.h"

namespace CQML
{
  using xercesc::DOMImplementation;
  using xercesc::DOMDocument;
  using xercesc::DOMElement;
  using xercesc::DOMWriter;
  using xercesc::XMLFormatTarget;
  using xercesc::LocalFileFormatTarget;

  class DeploymentPlanFrameworkVisitor: public Visitor
  {
  public:
    DeploymentPlanFramework_Export DeploymentPlanFrameworkVisitor (const std::string& outputPath);
    DeploymentPlanFramework_Export ~DeploymentPlanFrameworkVisitor();

    DeploymentPlanFramework_Export void init();
    DeploymentPlanFramework_Export void initTarget (const std::string& fileName);
	  DeploymentPlanFramework_Export void initNodeRefName (const std::string& nodeRefName);
	  DeploymentPlanFramework_Export std::string retNodeRefName ();
	  DeploymentPlanFramework_Export void initcgName (const std::string& cgName);
	  DeploymentPlanFramework_Export std::string retcgName ();
    DeploymentPlanFramework_Export void initDocument (const std::string& rootName);
    DeploymentPlanFramework_Export void initRootAttributes();
    DeploymentPlanFramework_Export void dumpDocument();

    DeploymentPlanFramework_Export void push();
    DeploymentPlanFramework_Export void pop();

    DeploymentPlanFramework_Export DOMElement* createSimpleContent (const std::string& name,
                                                 const std::string& value);

    // Lord Of the Rings..
    DeploymentPlanFramework_Export virtual void Visit_RootFolder(const RootFolder&);

    // Predefined Types
    DeploymentPlanFramework_Export virtual void Visit_LongInteger(const LongInteger&);
  	DeploymentPlanFramework_Export virtual void Visit_Byte(const Byte&);
    DeploymentPlanFramework_Export virtual void Visit_String(const String&);
	  DeploymentPlanFramework_Export virtual void Visit_RealNumber(const RealNumber&);
    DeploymentPlanFramework_Export virtual void Visit_Boolean(const Boolean&);
    DeploymentPlanFramework_Export virtual void Visit_ShortInteger(const ShortInteger&);

    DeploymentPlanFramework_Export virtual void Visit_GenericValue(const GenericValue&){};
    DeploymentPlanFramework_Export virtual void Visit_TypeKind(const TypeKind&){};
    DeploymentPlanFramework_Export virtual void Visit_GenericObject(const GenericObject&){};
    DeploymentPlanFramework_Export virtual void Visit_PredefinedTypes(const PredefinedTypes&){};
    DeploymentPlanFramework_Export virtual void Visit_TypeEncoding(const TypeEncoding&){};
    DeploymentPlanFramework_Export virtual void Visit_GenericValueObject(const GenericValueObject&){};

    // Implementation Artifact operations

    DeploymentPlanFramework_Export virtual void Visit_ImplementationArtifacts(const ImplementationArtifacts&);
    DeploymentPlanFramework_Export virtual void Visit_ArtifactContainer(const ArtifactContainer&);
    DeploymentPlanFramework_Export virtual void Visit_ImplementationArtifact(const ImplementationArtifact&);
    DeploymentPlanFramework_Export virtual void Visit_ArtifactDependsOn(const ArtifactDependsOn&);
    DeploymentPlanFramework_Export virtual void Visit_ImplementationArtifactReference(const ImplementationArtifactReference&);
    DeploymentPlanFramework_Export virtual void Visit_ArtifactExecParameter(const ArtifactExecParameter&);
    DeploymentPlanFramework_Export virtual void Visit_Property(const Property&);
    DeploymentPlanFramework_Export virtual void Visit_DataType(const DataType&);

    DeploymentPlanFramework_Export virtual void Visit_ArtifactDeployRequirement(const ArtifactDeployRequirement&);
    DeploymentPlanFramework_Export virtual void Visit_ArtifactInfoProperty(const ArtifactInfoProperty&);
    DeploymentPlanFramework_Export virtual void Visit_ImplementationRequirement(const ImplementationRequirement&);

    // TopLevelPackage operations

    DeploymentPlanFramework_Export virtual void Visit_TopLevelPackages(const TopLevelPackages&);
    DeploymentPlanFramework_Export virtual void Visit_TopLevelPackageContainer(const TopLevelPackageContainer&);
    DeploymentPlanFramework_Export virtual void Visit_TopLevelPackage(const TopLevelPackage&);
    DeploymentPlanFramework_Export virtual void Visit_package(const package&);
    DeploymentPlanFramework_Export virtual void Visit_PackageConfigurationReference(const PackageConfigurationReference&);

    // PackageConfiguration operations

    DeploymentPlanFramework_Export virtual void Visit_PackageConfigurations(const PackageConfigurations&);
    DeploymentPlanFramework_Export virtual void Visit_PackageConfigurationContainer(const PackageConfigurationContainer&);
    DeploymentPlanFramework_Export virtual void Visit_PackageConfiguration(const PackageConfiguration&);
    DeploymentPlanFramework_Export virtual void Visit_PackageConfConfigProperty(const PackageConfConfigProperty&);
    DeploymentPlanFramework_Export virtual void Visit_PackageConfReference(const PackageConfReference&);
    DeploymentPlanFramework_Export virtual void Visit_PackageConfSpecializedConfig(const PackageConfSpecializedConfig&);
    DeploymentPlanFramework_Export virtual void Visit_PackageConfSelectRequirement(const PackageConfSelectRequirement&);
    DeploymentPlanFramework_Export virtual void Visit_PackageConfBasePackage(const PackageConfBasePackage&);
    DeploymentPlanFramework_Export virtual void Visit_ComponentPackageReference(const ComponentPackageReference&);

    DeploymentPlanFramework_Export virtual void Visit_ComponentImplementations(const ComponentImplementations&);
	  DeploymentPlanFramework_Export virtual void Visit_ComponentImplementationContainer(const ComponentImplementationContainer&);
    DeploymentPlanFramework_Export virtual void Visit_MonolithicImplementation(const MonolithicImplementation&);
    DeploymentPlanFramework_Export virtual void Visit_MonolithExecParameter(const MonolithExecParameter&);
    DeploymentPlanFramework_Export virtual void Visit_ComponentAssembly(const ComponentAssembly&);
	  DeploymentPlanFramework_Export virtual void update_connections(const ComponentAssembly&);
	  DeploymentPlanFramework_Export virtual void update_parent_connections(const ComponentAssembly&);
	  DeploymentPlanFramework_Export virtual void Visit_AssemblyConfigProperty(const AssemblyConfigProperty&);
	  DeploymentPlanFramework_Export virtual void Visit_DeploymentPlan(const DeploymentPlan&);
    DeploymentPlanFramework_Export virtual void Visit_DeploymentPlans(const DeploymentPlans&);
	  DeploymentPlanFramework_Export virtual void Visit_MonolithprimaryArtifact(const MonolithprimaryArtifact&);
	  DeploymentPlanFramework_Export virtual void Visit_Implements(const Implements&);
	  DeploymentPlanFramework_Export virtual void Visit_ConfigProperty(const ConfigProperty&);
	  DeploymentPlanFramework_Export virtual void Visit_publish(const publish&);
	  DeploymentPlanFramework_Export virtual void Visit_PublishConnector(const PublishConnector&);
    DeploymentPlanFramework_Export virtual void Visit_deliverTo(const deliverTo&);
    DeploymentPlanFramework_Export virtual void Visit_emit(const emit&);
    DeploymentPlanFramework_Export virtual void Visit_invoke(const invoke&);


	//
    DeploymentPlanFramework_Export virtual void Visit_ComponentPackage(const ComponentPackage&){};
    DeploymentPlanFramework_Export virtual void Visit_ComponentPackages(const ComponentPackages&){};
    DeploymentPlanFramework_Export virtual void Visit_Implementation(const Implementation&){};
    DeploymentPlanFramework_Export virtual void Visit_PackageContainer(const PackageContainer&){};
    DeploymentPlanFramework_Export virtual void Visit_PackageConfigProperty(const PackageConfigProperty&){};
    DeploymentPlanFramework_Export virtual void Visit_PackageInfoProperty(const PackageInfoProperty&){};
    DeploymentPlanFramework_Export virtual void Visit_PackageInterface(const PackageInterface&){};
    DeploymentPlanFramework_Export virtual void Visit_InfoProperty(const InfoProperty&){};

	//
	  DeploymentPlanFramework_Export virtual void Visit_Requirement(const Requirement&){};
    DeploymentPlanFramework_Export virtual void Visit_SatisfierProperty(const SatisfierProperty&){};
    DeploymentPlanFramework_Export virtual void Visit_ImplementationDependency(const ImplementationDependency&){};
    DeploymentPlanFramework_Export virtual void Visit_Capability(const Capability&){};
    DeploymentPlanFramework_Export virtual void Visit_AssemblyselectRequirement(const AssemblyselectRequirement&){};
    DeploymentPlanFramework_Export virtual void Visit_AssemblyDeployRequirement(const AssemblyDeployRequirement&){};
	  DeploymentPlanFramework_Export virtual void Visit_MonolithDeployRequirement(const MonolithDeployRequirement&){};
    DeploymentPlanFramework_Export virtual void Visit_ComponentImplementationReference(const ComponentImplementationReference&){};
    DeploymentPlanFramework_Export virtual void Visit_ImplementationDependsOn(const ImplementationDependsOn&){};
    DeploymentPlanFramework_Export virtual void Visit_ImplementationCapability(const ImplementationCapability&){};
    DeploymentPlanFramework_Export virtual void Visit_ComponentContainer(const ComponentContainer&){};
    DeploymentPlanFramework_Export virtual void Visit_ComponentPropertyDescription(const ComponentPropertyDescription&){};
    DeploymentPlanFramework_Export virtual void Visit_ComponentInfoProperty(const ComponentInfoProperty&){};
    DeploymentPlanFramework_Export virtual void Visit_ComponentProperty(const ComponentProperty&){};
    DeploymentPlanFramework_Export virtual void Visit_ComponentTypes(const ComponentTypes&){};
    DeploymentPlanFramework_Export virtual void Visit_ComponentConfigProperty(const ComponentConfigProperty&){};

	//
	  DeploymentPlanFramework_Export virtual void Visit_Resource(const Resource&){};
    DeploymentPlanFramework_Export virtual void Visit_SharedResource(const SharedResource&){};
    DeploymentPlanFramework_Export virtual void Visit_NodeReference(const NodeReference&);
    DeploymentPlanFramework_Export virtual void Visit_Bridge(const Bridge&){};
    DeploymentPlanFramework_Export virtual void Visit_Node(const Node&){};
    DeploymentPlanFramework_Export virtual void Visit_Interconnect(const Interconnect&){};
    DeploymentPlanFramework_Export virtual void Visit_Domain(const Domain&){};
    DeploymentPlanFramework_Export virtual void Visit_Targets(const Targets&){};
    DeploymentPlanFramework_Export virtual void Visit_Node2Interconnect(const Node2Interconnect&){};
    DeploymentPlanFramework_Export virtual void Visit_Bridge2Interconnect(const Bridge2Interconnect&){};
    DeploymentPlanFramework_Export virtual void Visit_Shares(const Shares&){};
    DeploymentPlanFramework_Export virtual void Visit_Interconnect2Node(const Interconnect2Node&){};
    DeploymentPlanFramework_Export virtual void Visit_Interconnect2Bridge(const Interconnect2Bridge&){};
    DeploymentPlanFramework_Export virtual void Visit_InstanceMapping(const InstanceMapping&);
    DeploymentPlanFramework_Export virtual void Visit_CollocationGroup(const CollocationGroup&){};
    DeploymentPlanFramework_Export virtual void Visit_InParameter(const InParameter&){};
    DeploymentPlanFramework_Export virtual void Visit_TwowayOperation(const TwowayOperation&){};
    DeploymentPlanFramework_Export virtual void Visit_OnewayOperation(const OnewayOperation&){};
    DeploymentPlanFramework_Export virtual void Visit_FactoryOperation(const FactoryOperation&){};
    DeploymentPlanFramework_Export virtual void Visit_LookupOperation(const LookupOperation&){};
    DeploymentPlanFramework_Export virtual void Visit_InoutParameter(const InoutParameter&){};
    DeploymentPlanFramework_Export virtual void Visit_OutParameter(const OutParameter&){};
    DeploymentPlanFramework_Export virtual void Visit_ReturnType(const ReturnType&){};
    DeploymentPlanFramework_Export virtual void Visit_Exception(const Exception&){};
    DeploymentPlanFramework_Export virtual void Visit_Package(const Package&){};
    DeploymentPlanFramework_Export virtual void Visit_File(const File&){};
    DeploymentPlanFramework_Export virtual void Visit_Constant(const Constant&){};
    DeploymentPlanFramework_Export virtual void Visit_InterfaceDefinitions(const InterfaceDefinitions&){};
    DeploymentPlanFramework_Export virtual void Visit_FileRef(const FileRef&){};
    DeploymentPlanFramework_Export virtual void Visit_ExceptionRef(const ExceptionRef&){};
    DeploymentPlanFramework_Export virtual void Visit_SwitchedAggregate(const SwitchedAggregate&){};
    DeploymentPlanFramework_Export virtual void Visit_Member(const Member&){};
    DeploymentPlanFramework_Export virtual void Visit_Boxed(const Boxed&){};
    DeploymentPlanFramework_Export virtual void Visit_EnumValue(const EnumValue&){};
    DeploymentPlanFramework_Export virtual void Visit_Label(const Label&){};
    DeploymentPlanFramework_Export virtual void Visit_Aggregate(const Aggregate&){};
    DeploymentPlanFramework_Export virtual void Visit_Alias(const Alias&){};
    DeploymentPlanFramework_Export virtual void Visit_Collection(const Collection&){};
    DeploymentPlanFramework_Export virtual void Visit_Discriminator(const Discriminator&){};
    DeploymentPlanFramework_Export virtual void Visit_Enum(const Enum&){};
    DeploymentPlanFramework_Export virtual void Visit_LabelConnection(const LabelConnection&){};
    DeploymentPlanFramework_Export virtual void Visit_SetException(const SetException&){};
    DeploymentPlanFramework_Export virtual void Visit_LookupKey(const LookupKey&){};
    DeploymentPlanFramework_Export virtual void Visit_Attribute(const Attribute&){};
    DeploymentPlanFramework_Export virtual void Visit_Supports(const Supports&){};
    DeploymentPlanFramework_Export virtual void Visit_MakeMemberPrivate(const MakeMemberPrivate&){};
    DeploymentPlanFramework_Export virtual void Visit_PrivateFlag(const PrivateFlag&){};
    DeploymentPlanFramework_Export virtual void Visit_GetException(const GetException&){};
    DeploymentPlanFramework_Export virtual void Visit_Inherits(const Inherits&){};
    DeploymentPlanFramework_Export virtual void Visit_Object(const Object&){};
    DeploymentPlanFramework_Export virtual void Visit_ValueObject(const ValueObject&){};
    DeploymentPlanFramework_Export virtual void Visit_Event(const Event&){};
    DeploymentPlanFramework_Export virtual void Visit_AttributeMember(const AttributeMember&){};
    DeploymentPlanFramework_Export virtual void Visit_OutEventPort(const OutEventPort&){};
    DeploymentPlanFramework_Export virtual void Visit_ProvidedRequestPort(const ProvidedRequestPort&){};
    DeploymentPlanFramework_Export virtual void Visit_Component(const Component&){};
    DeploymentPlanFramework_Export virtual void Visit_RequiredRequestPort(const RequiredRequestPort&){};
    DeploymentPlanFramework_Export virtual void Visit_ManagesComponent(const ManagesComponent&){};
    DeploymentPlanFramework_Export virtual void Visit_InEventPort(const InEventPort&){};
    DeploymentPlanFramework_Export virtual void Visit_ComponentRef(const ComponentRef&){};
    DeploymentPlanFramework_Export virtual void Visit_ComponentFactory(const ComponentFactory&){};
    DeploymentPlanFramework_Export virtual void Visit_Object(const Udm::Object&){};
	  DeploymentPlanFramework_Export void GetReceptacleComponents (const RequiredRequestPort& receptacle,
                                    std::map<Component,std::string>& output);

    DeploymentPlanFramework_Export void GetFacetComponents (const ProvidedRequestPort& facet,
                             std::map<Component,std::string>& output);

    DeploymentPlanFramework_Export void GetEventSinkComponents (const InEventPort& consumer,
                                 std::map<Component,std::string>& output);

    DeploymentPlanFramework_Export void GetEventSourceComponents (const OutEventPort& publisher,
                                   std::map<Component,std::string>& output);

    DeploymentPlanFramework_Export void GetAttributeComponents (const AttributeMapping& mapping,
                                 std::set<std::pair<std::string, std::string> >& output);

    template <typename T, typename Del, typename DelRet, typename DelEndRet>
      void GetComponents (const T& port,
                          DelRet (T::*srcDel)() const,
                          DelRet (T::*dstDel) () const,
                          DelEndRet (Del::*srcDelEnd)() const,
                          DelEndRet (Del::*dstDelEnd)() const,
                          std::map<Component, std::string>& output,
                          std::set<T>& visited);

    DeploymentPlanFramework_Export void CreateConnections (const std::map<Component, std::string>& src,
                            const std::map<Component, std::string>& dst,
							const std::string& source_kind,
							const std::string& dest_kind);

    DeploymentPlanFramework_Export void CreateConnection (const Component& srcComp,
                           const std::string& srcPortName,
                           const Component& dstComp,
                           const std::string& dstPortName,
						   const std::string& source_kind,
						   const std::string& dest_kind);
    DeploymentPlanFramework_Export void CreateAssemblyInstances (std::set<Component>& comps);
    DeploymentPlanFramework_Export void CreateAssemblyConnections (std::vector<ComponentAssembly>& assemblies);
    DeploymentPlanFramework_Export void CreateAttributeMappings (std::vector<ComponentAssembly>& assemblies);
	DeploymentPlanFramework_Export void CreatePropertyElement (std::string name, const Property& property);
	DeploymentPlanFramework_Export virtual void Visit_ReadonlyAttribute(const ReadonlyAttribute&);
    DeploymentPlanFramework_Export virtual void Visit_AttributeValue(const AttributeValue&);
    DeploymentPlanFramework_Export virtual void Visit_AttributeDelegate(const AttributeDelegate&);
    DeploymentPlanFramework_Export virtual void Visit_AttributeMapping(const AttributeMapping&);
    DeploymentPlanFramework_Export virtual void Visit_AttributeMappingValue(const AttributeMappingValue&);
    DeploymentPlanFramework_Export virtual void Visit_AttributeMappingDelegate(const AttributeMappingDelegate&);
	DeploymentPlanFramework_Export std::string ExtractName (Udm::Object obj);
	DeploymentPlanFramework_Export virtual void create_component_instance (Component& comp, std::string& comp_name);
  DeploymentPlanFramework_Export virtual void update_component_instance (std::string& comp_instance_name, std::string& nodeRefName);
	DeploymentPlanFramework_Export virtual void update_component_parents (Component& comp);
	DeploymentPlanFramework_Export virtual void update_shared_component_parents (ComponentRef& comp_ref);
	DeploymentPlanFramework_Export virtual void update_component_assembly_parents (ComponentAssembly& comp_assembly);
	DeploymentPlanFramework_Export virtual void create_assembly_config_properties (Component& comp);
	DeploymentPlanFramework_Export virtual void create_assembly_attribute_properties (Component& comp);
	DeploymentPlanFramework_Export virtual void create_component_readonly_attributes (Component& comp);
	DeploymentPlanFramework_Export virtual void update_monolith_impl (Component& comp);
	DeploymentPlanFramework_Export virtual void create_component_config_properties (MonolithicImplementation& mimpl);
	DeploymentPlanFramework_Export virtual void generate_instance_deployment_descriptions (void);
	DeploymentPlanFramework_Export virtual void generate_assembly_instance_deployment_descriptions (void);
	DeploymentPlanFramework_Export virtual void generate_artifact_descriptions (void);
  DeploymentPlanFramework_Export virtual void generate_infoproperties (const DeploymentPlan &);
	DeploymentPlanFramework_Export virtual void generate_implementation_descriptions (void);
	DeploymentPlanFramework_Export virtual void generate_child_connections (void);
	DeploymentPlanFramework_Export virtual void generate_parent_connections (void);
	DeploymentPlanFramework_Export virtual void instantiate_deployment_plan_descriptor (DeploymentPlan& dp);
	DeploymentPlanFramework_Export virtual void finalize_deployment_plan_descriptor (void);
	DeploymentPlanFramework_Export virtual void create_label_and_uuid (DeploymentPlan& dp);
	DeploymentPlanFramework_Export virtual void add_injector (const std::string& plan_name, Injector* injector);
  DeploymentPlanFramework_Export virtual void clear_private_variables (void);

  private:

    DOMImplementation*  impl_;
    DOMDocument*        doc_;
    DOMElement*         root_;
    DOMElement*         curr_;
    DOMWriter*          serializer_;
    XMLFormatTarget*    target_;
    std::string         outputPath_;
    std::stack<DOMElement*> curr_stack_;
    std::string         node_ref_name_;
	  std::string         cg_name_;
	  RootFolder          root_folder_;

	// Maintain associations between PublishConnector and event publishers
    std::map<std::string, OutEventPort> publishers_;
	  std::map<std::string, MonolithicImplementation> monoimpls_;
	  std::set<std::string> selected_instances_;
	  std::set<CQML::ComponentAssembly> path_parents_;
	  std::set<CQML::ComponentAssembly> containing_assemblies_;
	  std::set<CQML::Component> assembly_components_;
	  CQML::MonolithicImplementation mimpl_;
	  std::map<std::string, std::string> deployed_instances_;
	  std::set<CQML::Component> monolith_components_;
	  std::set<CQML::Component> final_assembly_components_;

    // Maintain associations between PublishConnector and event consumers
    std::multimap<std::string, InEventPort> consumers_;

	  std::map<std::string, std::string> interfaces_;
	  std::map<std::pair<std::string, std::string>, Property> attrValues_;
	  std::map<std::string, CQML::Component> instance_monolith_components_;
	  std::map<std::string, CQML::Component> instance_assembly_components_;
	  std::map<std::string, Injector*> injectors_;
    std::map < std::string, std::pair <CQML::InstanceConnection, CQML::InstanceConnection> > connections_;
  };
}
#endif /* DEPLOYMENTPLANFRAMEWORK_VISITOR_H */
