// $Id$

#ifndef DEPLOYMENTPLAN_VISITOR_H
#define DEPLOYMENTPLAN_VISITOR_H

#include <memory>
#include <stack>
#include <map>
#include "PICML/PICML.h"

// Xerces includes
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>

// Utility includes
#include "Utils/XercesString.h"
#include "Utils/Utils.h"
#include "DeploymentPlan/DeploymentPlan_Export.h"

namespace PICML
{
  using xercesc::DOMImplementation;
  using xercesc::DOMDocument;
  using xercesc::DOMElement;
  using xercesc::DOMWriter;
  using xercesc::XMLFormatTarget;
  using xercesc::LocalFileFormatTarget;

  class DeploymentPlanVisitor: public Visitor
  {
  public:
    DeploymentPlan_Export DeploymentPlanVisitor (const std::string& outputPath);
    DeploymentPlan_Export ~DeploymentPlanVisitor();

    DeploymentPlan_Export void init();
    DeploymentPlan_Export void initTarget (const std::string& fileName);
	DeploymentPlan_Export void initNodeRefName (const std::string& nodeRefName);
	DeploymentPlan_Export std::string retNodeRefName ();
	DeploymentPlan_Export void initcgName (const std::string& cgName);
	DeploymentPlan_Export std::string retcgName ();
    DeploymentPlan_Export void initDocument (const std::string& rootName);
    DeploymentPlan_Export void initRootAttributes();
    DeploymentPlan_Export void dumpDocument();

    DeploymentPlan_Export void push();
    DeploymentPlan_Export void pop();

    DeploymentPlan_Export DOMElement* createSimpleContent (const std::string& name,
                                                 const std::string& value);

    // Lord Of the Rings..
    DeploymentPlan_Export virtual void Visit_RootFolder(const RootFolder&);

    // Predefined Types
    DeploymentPlan_Export virtual void Visit_LongInteger(const LongInteger&);
	DeploymentPlan_Export virtual void Visit_Byte(const Byte&);
    DeploymentPlan_Export virtual void Visit_String(const String&);
	DeploymentPlan_Export virtual void Visit_RealNumber(const RealNumber&);
    DeploymentPlan_Export virtual void Visit_Boolean(const Boolean&);
    DeploymentPlan_Export virtual void Visit_ShortInteger(const ShortInteger&);

    DeploymentPlan_Export virtual void Visit_GenericValue(const GenericValue&){};
    DeploymentPlan_Export virtual void Visit_TypeKind(const TypeKind&){};
    DeploymentPlan_Export virtual void Visit_GenericObject(const GenericObject&){};
    DeploymentPlan_Export virtual void Visit_PredefinedTypes(const PredefinedTypes&){};
    DeploymentPlan_Export virtual void Visit_TypeEncoding(const TypeEncoding&){};
    DeploymentPlan_Export virtual void Visit_GenericValueObject(const GenericValueObject&){};

    // Implementation Artifact operations

    DeploymentPlan_Export virtual void Visit_ImplementationArtifacts(const ImplementationArtifacts&);
    DeploymentPlan_Export virtual void Visit_ArtifactContainer(const ArtifactContainer&);
    DeploymentPlan_Export virtual void Visit_ImplementationArtifact(const ImplementationArtifact&);
    DeploymentPlan_Export virtual void Visit_ArtifactDependsOn(const ArtifactDependsOn&);
    DeploymentPlan_Export virtual void Visit_ImplementationArtifactReference(const ImplementationArtifactReference&);
    DeploymentPlan_Export virtual void Visit_ArtifactExecParameter(const ArtifactExecParameter&);
    DeploymentPlan_Export virtual void Visit_Property(const Property&);
    DeploymentPlan_Export virtual void Visit_DataType(const DataType&);

    DeploymentPlan_Export virtual void Visit_ArtifactDeployRequirement(const ArtifactDeployRequirement&);
    DeploymentPlan_Export virtual void Visit_ArtifactInfoProperty(const ArtifactInfoProperty&);
    DeploymentPlan_Export virtual void Visit_ImplementationRequirement(const ImplementationRequirement&);

    // TopLevelPackage operations

    DeploymentPlan_Export virtual void Visit_TopLevelPackages(const TopLevelPackages&);
    DeploymentPlan_Export virtual void Visit_TopLevelPackageContainer(const TopLevelPackageContainer&);
    DeploymentPlan_Export virtual void Visit_TopLevelPackage(const TopLevelPackage&);
    DeploymentPlan_Export virtual void Visit_package(const package&);
    DeploymentPlan_Export virtual void Visit_PackageConfigurationReference(const PackageConfigurationReference&);

    // PackageConfiguration operations

    DeploymentPlan_Export virtual void Visit_PackageConfigurations(const PackageConfigurations&);
    DeploymentPlan_Export virtual void Visit_PackageConfigurationContainer(const PackageConfigurationContainer&);
    DeploymentPlan_Export virtual void Visit_PackageConfiguration(const PackageConfiguration&);
    DeploymentPlan_Export virtual void Visit_PackageConfConfigProperty(const PackageConfConfigProperty&);
    DeploymentPlan_Export virtual void Visit_PackageConfReference(const PackageConfReference&);
    DeploymentPlan_Export virtual void Visit_PackageConfSpecializedConfig(const PackageConfSpecializedConfig&);
    DeploymentPlan_Export virtual void Visit_PackageConfSelectRequirement(const PackageConfSelectRequirement&);
    DeploymentPlan_Export virtual void Visit_PackageConfBasePackage(const PackageConfBasePackage&);
    DeploymentPlan_Export virtual void Visit_ComponentPackageReference(const ComponentPackageReference&);

    DeploymentPlan_Export virtual void Visit_ComponentImplementations(const ComponentImplementations&);
	DeploymentPlan_Export virtual void Visit_ComponentImplementationContainer(const ComponentImplementationContainer&);
    DeploymentPlan_Export virtual void Visit_MonolithicImplementation(const MonolithicImplementation&);
    DeploymentPlan_Export virtual void Visit_MonolithExecParameter(const MonolithExecParameter&);
    DeploymentPlan_Export virtual void Visit_ComponentAssembly(const ComponentAssembly&);
	DeploymentPlan_Export virtual void update_connections(const ComponentAssembly&);
	DeploymentPlan_Export virtual void update_parent_connections(const ComponentAssembly&);
	DeploymentPlan_Export virtual void Visit_AssemblyConfigProperty(const AssemblyConfigProperty&);
	DeploymentPlan_Export virtual void Visit_DeploymentPlan(const DeploymentPlan&);
    DeploymentPlan_Export virtual void Visit_DeploymentPlans(const DeploymentPlans&);
	DeploymentPlan_Export virtual void Visit_MonolithprimaryArtifact(const MonolithprimaryArtifact&);
	DeploymentPlan_Export virtual void Visit_Implements(const Implements&);
	DeploymentPlan_Export virtual void Visit_ConfigProperty(const ConfigProperty&);
	DeploymentPlan_Export virtual void Visit_publish(const publish&);
	DeploymentPlan_Export virtual void Visit_PublishConnector(const PublishConnector&);
    DeploymentPlan_Export virtual void Visit_deliverTo(const deliverTo&);
    DeploymentPlan_Export virtual void Visit_emit(const emit&);
    DeploymentPlan_Export virtual void Visit_invoke(const invoke&);


	//
    DeploymentPlan_Export virtual void Visit_ComponentPackage(const ComponentPackage&){};
    DeploymentPlan_Export virtual void Visit_ComponentPackages(const ComponentPackages&){};
    DeploymentPlan_Export virtual void Visit_Implementation(const Implementation&){};
    DeploymentPlan_Export virtual void Visit_PackageContainer(const PackageContainer&){};
    DeploymentPlan_Export virtual void Visit_PackageConfigProperty(const PackageConfigProperty&){};
    DeploymentPlan_Export virtual void Visit_PackageInfoProperty(const PackageInfoProperty&){};
    DeploymentPlan_Export virtual void Visit_PackageInterface(const PackageInterface&){};
    DeploymentPlan_Export virtual void Visit_InfoProperty(const InfoProperty&){};

	//
	DeploymentPlan_Export virtual void Visit_Requirement(const Requirement&){};
    DeploymentPlan_Export virtual void Visit_SatisfierProperty(const SatisfierProperty&){};
    DeploymentPlan_Export virtual void Visit_ImplementationDependency(const ImplementationDependency&){};
    DeploymentPlan_Export virtual void Visit_Capability(const Capability&){};
    DeploymentPlan_Export virtual void Visit_AssemblyselectRequirement(const AssemblyselectRequirement&){};
    DeploymentPlan_Export virtual void Visit_AssemblyDeployRequirement(const AssemblyDeployRequirement&){};
	DeploymentPlan_Export virtual void Visit_MonolithDeployRequirement(const MonolithDeployRequirement&){};
    DeploymentPlan_Export virtual void Visit_ComponentImplementationReference(const ComponentImplementationReference&){};
    DeploymentPlan_Export virtual void Visit_ImplementationDependsOn(const ImplementationDependsOn&){};
    DeploymentPlan_Export virtual void Visit_ImplementationCapability(const ImplementationCapability&){};
    DeploymentPlan_Export virtual void Visit_ComponentContainer(const ComponentContainer&){};
    DeploymentPlan_Export virtual void Visit_ComponentPropertyDescription(const ComponentPropertyDescription&){};
    DeploymentPlan_Export virtual void Visit_ComponentInfoProperty(const ComponentInfoProperty&){};
    DeploymentPlan_Export virtual void Visit_ComponentProperty(const ComponentProperty&){};
    DeploymentPlan_Export virtual void Visit_ComponentTypes(const ComponentTypes&){};
    DeploymentPlan_Export virtual void Visit_ComponentConfigProperty(const ComponentConfigProperty&){};

	//
	DeploymentPlan_Export virtual void Visit_Resource(const Resource&){};
    DeploymentPlan_Export virtual void Visit_SharedResource(const SharedResource&){};
    DeploymentPlan_Export virtual void Visit_NodeReference(const NodeReference&);
    DeploymentPlan_Export virtual void Visit_Bridge(const Bridge&){};
    DeploymentPlan_Export virtual void Visit_Node(const Node&){};
    DeploymentPlan_Export virtual void Visit_Interconnect(const Interconnect&){};
    DeploymentPlan_Export virtual void Visit_Domain(const Domain&){};
    DeploymentPlan_Export virtual void Visit_Targets(const Targets&){};
    DeploymentPlan_Export virtual void Visit_Node2Interconnect(const Node2Interconnect&){};
    DeploymentPlan_Export virtual void Visit_Bridge2Interconnect(const Bridge2Interconnect&){};
    DeploymentPlan_Export virtual void Visit_Shares(const Shares&){};
    DeploymentPlan_Export virtual void Visit_Interconnect2Node(const Interconnect2Node&){};
    DeploymentPlan_Export virtual void Visit_Interconnect2Bridge(const Interconnect2Bridge&){};
    DeploymentPlan_Export virtual void Visit_InstanceMapping(const InstanceMapping&);
    DeploymentPlan_Export virtual void Visit_CollocationGroup(const CollocationGroup&){};
    DeploymentPlan_Export virtual void Visit_InParameter(const InParameter&){};
    DeploymentPlan_Export virtual void Visit_TwowayOperation(const TwowayOperation&){};
    DeploymentPlan_Export virtual void Visit_OnewayOperation(const OnewayOperation&){};
    DeploymentPlan_Export virtual void Visit_FactoryOperation(const FactoryOperation&){};
    DeploymentPlan_Export virtual void Visit_LookupOperation(const LookupOperation&){};
    DeploymentPlan_Export virtual void Visit_InoutParameter(const InoutParameter&){};
    DeploymentPlan_Export virtual void Visit_OutParameter(const OutParameter&){};
    DeploymentPlan_Export virtual void Visit_ReturnType(const ReturnType&){};
    DeploymentPlan_Export virtual void Visit_Exception(const Exception&){};
    DeploymentPlan_Export virtual void Visit_Package(const Package&){};
    DeploymentPlan_Export virtual void Visit_File(const File&){};
    DeploymentPlan_Export virtual void Visit_Constant(const Constant&){};
    DeploymentPlan_Export virtual void Visit_InterfaceDefinitions(const InterfaceDefinitions&){};
    DeploymentPlan_Export virtual void Visit_FileRef(const FileRef&){};
    DeploymentPlan_Export virtual void Visit_ExceptionRef(const ExceptionRef&){};
    DeploymentPlan_Export virtual void Visit_SwitchedAggregate(const SwitchedAggregate&){};
    DeploymentPlan_Export virtual void Visit_Member(const Member&){};
    DeploymentPlan_Export virtual void Visit_Boxed(const Boxed&){};
    DeploymentPlan_Export virtual void Visit_EnumValue(const EnumValue&){};
    DeploymentPlan_Export virtual void Visit_Label(const Label&){};
    DeploymentPlan_Export virtual void Visit_Aggregate(const Aggregate&){};
    DeploymentPlan_Export virtual void Visit_Alias(const Alias&){};
    DeploymentPlan_Export virtual void Visit_Collection(const Collection&){};
    DeploymentPlan_Export virtual void Visit_Discriminator(const Discriminator&){};
    DeploymentPlan_Export virtual void Visit_Enum(const Enum&){};
    DeploymentPlan_Export virtual void Visit_LabelConnection(const LabelConnection&){};
    DeploymentPlan_Export virtual void Visit_SetException(const SetException&){};
    DeploymentPlan_Export virtual void Visit_LookupKey(const LookupKey&){};
    DeploymentPlan_Export virtual void Visit_Attribute(const Attribute&){};
    DeploymentPlan_Export virtual void Visit_Supports(const Supports&){};
    DeploymentPlan_Export virtual void Visit_MakeMemberPrivate(const MakeMemberPrivate&){};
    DeploymentPlan_Export virtual void Visit_PrivateFlag(const PrivateFlag&){};
    DeploymentPlan_Export virtual void Visit_GetException(const GetException&){};
    DeploymentPlan_Export virtual void Visit_Inherits(const Inherits&){};
    DeploymentPlan_Export virtual void Visit_Object(const Object&){};
    DeploymentPlan_Export virtual void Visit_ValueObject(const ValueObject&){};
    DeploymentPlan_Export virtual void Visit_Event(const Event&){};
    DeploymentPlan_Export virtual void Visit_AttributeMember(const AttributeMember&){};
    DeploymentPlan_Export virtual void Visit_OutEventPort(const OutEventPort&){};
    DeploymentPlan_Export virtual void Visit_ProvidedRequestPort(const ProvidedRequestPort&){};
    DeploymentPlan_Export virtual void Visit_Component(const Component&){};
    DeploymentPlan_Export virtual void Visit_RequiredRequestPort(const RequiredRequestPort&){};
    DeploymentPlan_Export virtual void Visit_ManagesComponent(const ManagesComponent&){};
    DeploymentPlan_Export virtual void Visit_InEventPort(const InEventPort&){};
    DeploymentPlan_Export virtual void Visit_ComponentRef(const ComponentRef&){};
    DeploymentPlan_Export virtual void Visit_ComponentFactory(const ComponentFactory&){};
    DeploymentPlan_Export virtual void Visit_Object(const Udm::Object&){};
	DeploymentPlan_Export void GetReceptacleComponents (const RequiredRequestPort& receptacle,
                                  std::map<Component,std::string>& output);

    DeploymentPlan_Export void GetFacetComponents (const ProvidedRequestPort& facet,
                             std::map<Component,std::string>& output);

    DeploymentPlan_Export void GetEventSinkComponents (const InEventPort& consumer,
                                 std::map<Component,std::string>& output);

    DeploymentPlan_Export void GetEventSourceComponents (const OutEventPort& publisher,
                                   std::map<Component,std::string>& output);

    DeploymentPlan_Export void GetAttributeComponents (const AttributeMapping& mapping,
                                 std::set<std::pair<std::string, std::string> >& output);

    template <typename T, typename Del, typename DelRet, typename DelEndRet>
      void GetComponents (const T& port,
                          DelRet (T::*srcDel)() const,
                          DelRet (T::*dstDel) () const,
                          DelEndRet (Del::*srcDelEnd)() const,
                          DelEndRet (Del::*dstDelEnd)() const,
                          std::map<Component, std::string>& output,
                          std::set<T>& visited);

    DeploymentPlan_Export void CreateConnections (const std::map<Component, std::string>& src,
                            const std::map<Component, std::string>& dst,
							const std::string& source_kind,
							const std::string& dest_kind);

    DeploymentPlan_Export void CreateConnection (const Component& srcComp,
                           const std::string& srcPortName,
                           const Component& dstComp,
                           const std::string& dstPortName,
						   const std::string& source_kind,
						   const std::string& dest_kind);
    DeploymentPlan_Export void CreateAssemblyInstances (std::set<Component>& comps);
    DeploymentPlan_Export void CreateAssemblyConnections (std::vector<ComponentAssembly>& assemblies);
    DeploymentPlan_Export void CreateAttributeMappings (std::vector<ComponentAssembly>& assemblies);
	DeploymentPlan_Export void CreatePropertyElement (std::string name, const Property& property);
	DeploymentPlan_Export virtual void Visit_ReadonlyAttribute(const ReadonlyAttribute&);
    DeploymentPlan_Export virtual void Visit_AttributeValue(const AttributeValue&);
    DeploymentPlan_Export virtual void Visit_AttributeDelegate(const AttributeDelegate&);
    DeploymentPlan_Export virtual void Visit_AttributeMapping(const AttributeMapping&);
    DeploymentPlan_Export virtual void Visit_AttributeMappingValue(const AttributeMappingValue&);
    DeploymentPlan_Export virtual void Visit_AttributeMappingDelegate(const AttributeMappingDelegate&);
	DeploymentPlan_Export std::string ExtractName (Udm::Object obj);
	DeploymentPlan_Export virtual void create_component_instance (Component& comp);
	DeploymentPlan_Export virtual void update_component_instance (Component& comp, std::string& nodeRefName);
	DeploymentPlan_Export virtual void update_component_parents (Component& comp);
	DeploymentPlan_Export virtual void update_shared_component_parents (ComponentRef& comp_ref);
	DeploymentPlan_Export virtual void update_component_assembly_parents (ComponentAssembly& comp_assembly);
	DeploymentPlan_Export virtual void create_assembly_config_properties (Component& comp);
	DeploymentPlan_Export virtual void create_assembly_attribute_properties (Component& comp);
	DeploymentPlan_Export virtual void create_component_readonly_attributes (Component& comp);
	DeploymentPlan_Export virtual void update_monolith_impl (Component& comp, std::string& comp_ref_name);
	DeploymentPlan_Export virtual void create_component_config_properties (MonolithicImplementation& mimpl);
	DeploymentPlan_Export virtual void generate_instance_deployment_descriptions (void);
	DeploymentPlan_Export virtual void generate_assembly_instance_deployment_descriptions (void);
	DeploymentPlan_Export virtual void generate_artifact_descriptions (void);
  DeploymentPlan_Export virtual void generate_infoproperties (const DeploymentPlan &);
	DeploymentPlan_Export virtual void generate_implementation_descriptions (void);
	DeploymentPlan_Export virtual void generate_child_connections (void);
	DeploymentPlan_Export virtual void generate_parent_connections (void);
	DeploymentPlan_Export virtual void instantiate_deployment_plan_descriptor (DeploymentPlan& dp);
	DeploymentPlan_Export virtual void finalize_deployment_plan_descriptor (void);
	DeploymentPlan_Export virtual void create_label_and_uuid (DeploymentPlan& dp);

  protected:
    template <typename T>
    std::string unique_id (const T &comp);

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
	std::set<PICML::ComponentAssembly> path_parents_;
	std::set<PICML::ComponentAssembly> containing_assemblies_;
	std::set<PICML::Component> assembly_components_;
	PICML::MonolithicImplementation mimpl_;
	std::map<std::string, MonolithicImplementation> selected_impls_;
	std::map<std::string, std::string> deployed_instances_;
	std::set<PICML::Component> monolith_components_;
	std::set<PICML::Component> final_assembly_components_;

    // Maintain associations between PublishConnector and event consumers
    std::multimap<std::string, InEventPort> consumers_;

	std::map<std::string, std::string> interfaces_;
	std::map<std::pair<std::string, std::string>, Property> attrValues_;
	std::map<std::pair< std::string, std::set<PICML::Component> >, std::string> cg_set_;
  };
}

#endif /* FLATPLAN_VISITOR_H */
