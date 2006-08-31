// $Id$

#ifndef IOAUTOMATA_VISITOR_H
#define IOAUTOMATA_VISITOR_H

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
#include "PICML/XercesString.h"
#include "IOAutomata/IOAutomata_Export.h"

namespace PICML
{
  using xercesc::DOMImplementation;
  using xercesc::DOMDocument;
  using xercesc::DOMElement;
  using xercesc::DOMWriter;
  using xercesc::XMLFormatTarget;
  using xercesc::LocalFileFormatTarget;

  class IOAutomata_Export IOAutomataVisitor: public Visitor
  {
  public:
    IOAutomataVisitor (const std::string& outputPath);
    ~IOAutomataVisitor();

    // Lord Of the Rings..
    virtual void Visit_RootFolder(const RootFolder&);

    // Member functions used by IOAutomata interpreter
    virtual void Visit_InterfaceDefinitions(const InterfaceDefinitions&);
    virtual void Visit_File(const File&);
    virtual void Visit_Package(const Package&);
    virtual void Visit_Component(const Component&);
    virtual void Visit_InputAction(const InputAction&);
    virtual void Visit_Property(const Property&);
    virtual void Visit_InputState(const InputState&);
    virtual void Visit_InternalAction(const InternalAction&);
    virtual void Visit_State(const State&);
    virtual void Visit_OutputState(const OutputState&);
    virtual void Visit_OutputAction(const OutputAction&);
    virtual void Visit_TimerState (const TimerState &);
    virtual void Visit_PeriodicAction (const PeriodicAction &);
    virtual void Visit_Environment (const Environment &);
    virtual void Visit_StartupState (const StartupState &);
    virtual void Visit_Input(const Input&) {}
    virtual void Visit_InputEffect(const InputEffect&) {}

  bool check_action_sequence_end(const std::set<Finish> &finish_set);

  private:

    DOMImplementation*  impl_;
    DOMDocument*        doc_;
    DOMElement*         root_;
    DOMElement*         curr_;
    DOMWriter*          serializer_;
    XMLFormatTarget*    target_;
    std::string         outputPath_;
    const State *curr_state_;
    bool curr_state_done_;
    std::stack<DOMElement*> curr_stack_;

    void init();
    void initTarget (const std::string& fileName);
    void initDocument (const std::string& rootName);
    void initRootAttributes();
    void dumpDocument();

    void push();
    void pop();

  public:
    // Predefined Types
    virtual void Visit_Boolean(const Boolean&) { }
    virtual void Visit_Byte(const Byte&) { }
    virtual void Visit_String(const String&) { }
    virtual void Visit_RealNumber(const RealNumber&) { }
    virtual void Visit_ShortInteger(const ShortInteger&) { }
    virtual void Visit_LongInteger(const LongInteger&) { }

    virtual void Visit_GenericValue(const GenericValue&){}
    virtual void Visit_TypeKind(const TypeKind&){}
    virtual void Visit_GenericObject(const GenericObject&){};
    virtual void Visit_PredefinedTypes(const PredefinedTypes&){};
    virtual void Visit_TypeEncoding(const TypeEncoding&){};
    virtual void Visit_GenericValueObject(const GenericValueObject&){};

    // ImplementationArtifact operations

    virtual void Visit_ImplementationArtifacts(const ImplementationArtifacts&) {}
    virtual void Visit_ArtifactContainer(const ArtifactContainer&) {}
    virtual void Visit_ImplementationArtifact(const ImplementationArtifact&) {}
    virtual void Visit_ArtifactDependsOn(const ArtifactDependsOn&) {}
    virtual void Visit_ArtifactDependency(const ArtifactDependency&) {}
    virtual void Visit_ImplementationArtifactReference(const ImplementationArtifactReference&) {}
    virtual void Visit_ArtifactExecParameter(const ArtifactExecParameter&) {}
    virtual void Visit_DataType(const DataType&) {}

    virtual void Visit_ArtifactDeployRequirement(const ArtifactDeployRequirement&) {}
    virtual void Visit_ArtifactInfoProperty(const ArtifactInfoProperty&) {}
    virtual void Visit_ImplementationRequirement(const ImplementationRequirement&) {}

    // TopLevelPackage operations

    virtual void Visit_TopLevelPackages(const TopLevelPackages&) {}
    virtual void Visit_TopLevelPackageContainer(const TopLevelPackageContainer&) {}
    virtual void Visit_TopLevelPackage(const TopLevelPackage&) {}
    virtual void Visit_PackageConfigurationReference(const PackageConfigurationReference&) {}

    // PackageConfiguration operations

    virtual void Visit_PackageConfigurations(const PackageConfigurations&) {}
    virtual void Visit_PackageConfigurationContainer(const PackageConfigurationContainer&) {}
    virtual void Visit_PackageConfiguration(const PackageConfiguration&) {}
    virtual void Visit_PackageConfConfigProperty(const PackageConfConfigProperty&) {}
    virtual void Visit_PackageConfReference(const PackageConfReference&) {}
    virtual void Visit_PackageConfSpecializedConfig(const PackageConfSpecializedConfig&) {}
    virtual void Visit_PackageConfSelectRequirement(const PackageConfSelectRequirement&) {}
    virtual void Visit_PackageConfBasePackage(const PackageConfBasePackage&) {}
    virtual void Visit_ComponentPackageReference(const ComponentPackageReference&) {}

    // ComponentPackage operations

    virtual void Visit_ComponentPackages(const ComponentPackages&) {}
    virtual void Visit_PackageContainer(const PackageContainer&) {}
    virtual void Visit_ComponentPackage(const ComponentPackage&) {}
    virtual void Visit_PackageInterface(const PackageInterface&) {}
    virtual void Visit_PackageConfigProperty(const PackageConfigProperty&) {}
    virtual void Visit_PackageInfoProperty(const PackageInfoProperty&) {}
    virtual void Visit_Implementation(const Implementation&) {}
    virtual void Visit_package(const package&) {}
    virtual void Visit_ComponentImplementationReference(const ComponentImplementationReference&) {}

    // ComponentInterface operations

    virtual void Visit_ComponentTypes(const ComponentTypes&) {}
    virtual void Visit_ComponentContainer(const ComponentContainer&) {}
    virtual void Visit_ComponentRef(const ComponentRef&) {}
    virtual void Visit_ComponentPropertyDescription(const ComponentPropertyDescription&) {}
    virtual void Visit_ComponentProperty(const ComponentProperty&) {}
    virtual void Visit_ComponentInfoProperty(const ComponentInfoProperty&) {}
    virtual void Visit_ComponentConfigProperty(const ComponentConfigProperty&) {}
    virtual void Visit_OutEventPort(const OutEventPort&) {}
    virtual void Visit_ProvidedRequestPort(const ProvidedRequestPort&) {}
    virtual void Visit_RequiredRequestPort(const RequiredRequestPort&) {}
    virtual void Visit_InEventPort(const InEventPort&);
    virtual void Visit_Supports(const Supports&) {}
    virtual void Visit_Object(const Object&) {}

    // ComponentImplementation operations

    virtual void Visit_ComponentImplementations(const ComponentImplementations&) {}
    virtual void Visit_ComponentImplementationContainer(const ComponentImplementationContainer&) {}
    virtual void Visit_MonolithicImplementation(const MonolithicImplementation&) {}
    virtual void Visit_MonolithExecParameter(const MonolithExecParameter&) {}

    virtual void Visit_Requirement(const Requirement&) {}
    virtual void Visit_SatisfierProperty(const SatisfierProperty&) {}
    virtual void Visit_ImplementationDependency(const ImplementationDependency&) {}
    virtual void Visit_Capability(const Capability&) {}
    virtual void Visit_AssemblyselectRequirement(const AssemblyselectRequirement&) {}
    virtual void Visit_AssemblyConfigProperty(const AssemblyConfigProperty&) {}
    virtual void Visit_PublishConnector(const PublishConnector&) {}
    virtual void Visit_publish(const publish&) {}
    virtual void Visit_deliverTo(const deliverTo&) {}
    virtual void Visit_AssemblyDeployRequirement(const AssemblyDeployRequirement&) {}
    virtual void Visit_ComponentAssembly(const ComponentAssembly&) {}
    virtual void Visit_emit(const emit&) {}
    virtual void Visit_invoke(const invoke&) {}

    virtual void Visit_InfoProperty(const InfoProperty&) {}
    virtual void Visit_MonolithprimaryArtifact(const MonolithprimaryArtifact&) {}
    virtual void Visit_MonolithDeployRequirement(const MonolithDeployRequirement&) {}
    virtual void Visit_ConfigProperty(const ConfigProperty&) {}
    virtual void Visit_ImplementationDependsOn(const ImplementationDependsOn&) {}
    virtual void Visit_Implements(const Implements&) {}
    virtual void Visit_ImplementationCapability(const ImplementationCapability&) {}
    // Component Attribute related operations
    virtual void Visit_ReadonlyAttribute(const ReadonlyAttribute&) {}
    virtual void Visit_AttributeValue(const AttributeValue&) {}
    virtual void Visit_AttributeDelegate(const AttributeDelegate&) {}
    virtual void Visit_AttributeMapping(const AttributeMapping&) {}
    virtual void Visit_AttributeMappingValue(const AttributeMappingValue&) {}
    virtual void Visit_AttributeMappingDelegate(const AttributeMappingDelegate&) {}

    virtual void Visit_Resource(const Resource&){}
    virtual void Visit_SharedResource(const SharedResource&){}
    virtual void Visit_NodeReference(const NodeReference&){}
    virtual void Visit_Bridge(const Bridge&){}
    virtual void Visit_Node(const Node&){};
    virtual void Visit_Interconnect(const Interconnect&){};
    virtual void Visit_Domain(const Domain&){};
    virtual void Visit_Targets(const Targets&){};
    virtual void Visit_Node2Interconnect(const Node2Interconnect&){};
    virtual void Visit_Bridge2Interconnect(const Bridge2Interconnect&){};
    virtual void Visit_Shares(const Shares&){};
    virtual void Visit_Interconnect2Node(const Interconnect2Node&){};
    virtual void Visit_Interconnect2Bridge(const Interconnect2Bridge&){};
    virtual void Visit_InstanceMapping(const InstanceMapping&){};
    virtual void Visit_DeploymentPlan(const DeploymentPlan&){};
    virtual void Visit_DeploymentPlans(const DeploymentPlans&){};
    virtual void Visit_CollocationGroup(const CollocationGroup&){};
    virtual void Visit_InParameter(const InParameter&){};
    virtual void Visit_TwowayOperation(const TwowayOperation&){};
    virtual void Visit_OnewayOperation(const OnewayOperation&){};
    virtual void Visit_FactoryOperation(const FactoryOperation&){};
    virtual void Visit_LookupOperation(const LookupOperation&){};
    virtual void Visit_InoutParameter(const InoutParameter&){};
    virtual void Visit_OutParameter(const OutParameter&){};
    virtual void Visit_ReturnType(const ReturnType&){};
    virtual void Visit_Exception(const Exception&){};
    virtual void Visit_Constant(const Constant&){};
    virtual void Visit_FileRef(const FileRef&){};
    virtual void Visit_ExceptionRef(const ExceptionRef&){};
    virtual void Visit_SwitchedAggregate(const SwitchedAggregate&){};
    virtual void Visit_Member(const Member&){};
    virtual void Visit_Boxed(const Boxed&){};
    virtual void Visit_EnumValue(const EnumValue&){};
    virtual void Visit_Label(const Label&){};
    virtual void Visit_Aggregate(const Aggregate&){};
    virtual void Visit_Alias(const Alias&){};
    virtual void Visit_Collection(const Collection&){};
    virtual void Visit_Discriminator(const Discriminator&){};
    virtual void Visit_Enum(const Enum&){};
    virtual void Visit_LabelConnection(const LabelConnection&){};
    virtual void Visit_SetException(const SetException&){};
    virtual void Visit_LookupKey(const LookupKey&){};
    virtual void Visit_Attribute(const Attribute&){};
    virtual void Visit_MakeMemberPrivate(const MakeMemberPrivate&){};
    virtual void Visit_PrivateFlag(const PrivateFlag&){};
    virtual void Visit_GetException(const GetException&){};
    virtual void Visit_Inherits(const Inherits&){};
    virtual void Visit_ValueObject(const ValueObject&){};
    virtual void Visit_Event(const Event&){};
    virtual void Visit_AttributeMember(const AttributeMember&){};
    virtual void Visit_ManagesComponent(const ManagesComponent&){};
    virtual void Visit_ComponentFactory(const ComponentFactory&){};
    virtual void Visit_Object(const Udm::Object&){};

  };
}

#endif /* IOAUTOMATA_VISITOR_H */
