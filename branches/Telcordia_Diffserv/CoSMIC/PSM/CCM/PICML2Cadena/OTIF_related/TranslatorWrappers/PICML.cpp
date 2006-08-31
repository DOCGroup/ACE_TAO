// cpp source file PICML.cpp generated from diagram PICML
// generated on Thu Aug 05 20:23:46 2004

#include "PICML.h"
#include "UmlExt.h"

namespace PICML {

	Uml::Diagram umldiagram;

	Uml::Class package::meta;
	Uml::Class TopLevelPackageContainer::meta;
	Uml::Class TopLevelPackages::meta;
	Uml::Class TopLevelPackage::meta;
	Uml::Class ComponentInfoProperty::meta;
	Uml::Class ComponentContainer::meta;
	Uml::Class ComponentProperty::meta;
	Uml::Class CommonPortAttrs::meta;
	Uml::Class ComponentType::meta;
	Uml::Class ComponentTypes::meta;
	Uml::Class ComponentConfigProperty::meta;
	Uml::Class ComponentPropertyDescription::meta;
	Uml::Class ConfigProperty::meta;
	Uml::Class Implements::meta;
	Uml::Class ImplementationCapability::meta;
	Uml::Class ImplementationDependsOn::meta;
	Uml::Class InfoProperty::meta;
	Uml::Class ComponentImplementations::meta;
	Uml::Class MonolithprimaryArtifact::meta;
	Uml::Class ComponentImplementationReference::meta;
	Uml::Class MonolithExecParameter::meta;
	Uml::Class ComponentImplementationContainer::meta;
	Uml::Class MonolithDeployRequirement::meta;
	Uml::Class ComponentImplementation::meta;
	Uml::Class MonolithicImplementation::meta;
	Uml::Class DeploymentPlans::meta;
	Uml::Class DeploymentPlan::meta;
	Uml::Class CollocationGroup::meta;
	Uml::Class InstanceMapping::meta;
	Uml::Class PackageConfSpecializedConfig::meta;
	Uml::Class PackageConfigurationContainer::meta;
	Uml::Class PackageConfConfigProperty::meta;
	Uml::Class PackageConfBasePackage::meta;
	Uml::Class PackageConfigurationReference::meta;
	Uml::Class PackageConfigurations::meta;
	Uml::Class PackageConfSelectRequirement::meta;
	Uml::Class PackageConfReference::meta;
	Uml::Class PackageConfiguration::meta;
	Uml::Class PackageInterface::meta;
	Uml::Class PackageInfoProperty::meta;
	Uml::Class PackageConfigProperty::meta;
	Uml::Class ComponentPackages::meta;
	Uml::Class PackageContainer::meta;
	Uml::Class Implementation::meta;
	Uml::Class ComponentPackage::meta;
	Uml::Class ComponentPackageReference::meta;
	Uml::Class Resource::meta;
	Uml::Class Node::meta;
	Uml::Class SharedResource::meta;
	Uml::Class Interconnect::meta;
	Uml::Class NodeReference::meta;
	Uml::Class Bridge::meta;
	Uml::Class SatisfierProperty::meta;
	Uml::Class Requirement::meta;
	Uml::Class DataType::meta;
	Uml::Class ImplementationDependency::meta;
	Uml::Class Property::meta;
	Uml::Class Capability::meta;
	Uml::Class RequirementSatisfier::meta;
	Uml::Class ImplementationRequirement::meta;
	Uml::Class RequirementBase::meta;
	Uml::Class Elements::meta;
	Uml::Class Domain::meta;
	Uml::Class Interconnect2Node::meta;
	Uml::Class Bridge2Interconnect::meta;
	Uml::Class Shares::meta;
	Uml::Class Node2Interconnect::meta;
	Uml::Class Interconnect2Bridge::meta;
	Uml::Class Targets::meta;
	Uml::Class AssemblyDeployRequirement::meta;
	Uml::Class invoke::meta;
	Uml::Class ComponentAssembly::meta;
	Uml::Class AssemblyselectRequirement::meta;
	Uml::Class emit::meta;
	Uml::Class PublishConnector::meta;
	Uml::Class deliverTo::meta;
	Uml::Class publish::meta;
	Uml::Class AssemblyConfigProperty::meta;
	Uml::Class ArtifactExecParameter::meta;
	Uml::Class ImplementationArtifactReference::meta;
	Uml::Class ArtifactDependsOn::meta;
	Uml::Class ImplementationArtifact::meta;
	Uml::Class ImplementationArtifacts::meta;
	Uml::Class ArtifactDeployRequirement::meta;
	Uml::Class ArtifactInfoProperty::meta;
	Uml::Class ArtifactContainer::meta;
	Uml::Class Package::meta;
	Uml::Class File::meta;
	Uml::Class Exception::meta;
	Uml::Class ExceptionRef::meta;
	Uml::Class FileRef::meta;
	Uml::Class InterfaceDefinitions::meta;
	Uml::Class ConstantType::meta;
	Uml::Class Constant::meta;
	Uml::Class PredefinedTypes::meta;
	Uml::Class PredefinedType::meta;
	Uml::Class Byte::meta;
	Uml::Class Boolean::meta;
	Uml::Class ShortInteger::meta;
	Uml::Class LongInteger::meta;
	Uml::Class RealNumber::meta;
	Uml::Class String::meta;
	Uml::Class GenericObject::meta;
	Uml::Class GenericValueObject::meta;
	Uml::Class GenericValue::meta;
	Uml::Class TypeEncoding::meta;
	Uml::Class TypeKind::meta;
	Uml::Class Enum::meta;
	Uml::Class EnumValue::meta;
	Uml::Class Aggregate::meta;
	Uml::Class SwitchedAggregate::meta;
	Uml::Class Label::meta;
	Uml::Class LabelConnection::meta;
	Uml::Class MemberType::meta;
	Uml::Class Discriminator::meta;
	Uml::Class NamedType::meta;
	Uml::Class Collection::meta;
	Uml::Class Alias::meta;
	Uml::Class Boxed::meta;
	Uml::Class Member::meta;
	Uml::Class NoInheritable::meta;
	Uml::Class Object::meta;
	Uml::Class Inherits::meta;
	Uml::Class Event::meta;
	Uml::Class ValueObject::meta;
	Uml::Class Inheritable::meta;
	Uml::Class HasOperations::meta;
	Uml::Class SupportsInterfaces::meta;
	Uml::Class ReadonlyAttribute::meta;
	Uml::Class LookupKey::meta;
	Uml::Class SetException::meta;
	Uml::Class ObjectByValue::meta;
	Uml::Class GetException::meta;
	Uml::Class PrivateFlag::meta;
	Uml::Class MakeMemberPrivate::meta;
	Uml::Class AttributeMember::meta;
	Uml::Class Supports::meta;
	Uml::Class Attribute::meta;
	Uml::Class OperationBase::meta;
	Uml::Class FactoryOperation::meta;
	Uml::Class LookupOperation::meta;
	Uml::Class TwowayOperation::meta;
	Uml::Class OnewayOperation::meta;
	Uml::Class HasExceptions::meta;
	Uml::Class InParameter::meta;
	Uml::Class OutParameter::meta;
	Uml::Class ReturnType::meta;
	Uml::Class InoutParameter::meta;
	Uml::Class Component::meta;
	Uml::Class OutEventPort::meta;
	Uml::Class ProvidedRequestPort::meta;
	Uml::Class RequiredRequestPort::meta;
	Uml::Class InEventPort::meta;
	Uml::Class Port::meta;
	Uml::Class ComponentRef::meta;
	Uml::Class Manageable::meta;
	Uml::Class ComponentFactory::meta;
	Uml::Class ManagesComponent::meta;
	Uml::Class Provideable::meta;
	Uml::Class Prefixable::meta;
	Uml::Class Taggable::meta;
	Uml::Class RootFolder::meta;
	Uml::Class MgaObject::meta;
	Uml::Attribute TopLevelPackages::meta_name;
	Uml::Attribute CommonPortAttrs::meta_exclusiveUser;
	Uml::Attribute CommonPortAttrs::meta_optional;
	Uml::Attribute CommonPortAttrs::meta_exclusiveProvider;
	Uml::Attribute ComponentTypes::meta_name;
	Uml::Attribute ComponentImplementations::meta_name;
	Uml::Attribute ComponentImplementation::meta_UUID;
	Uml::Attribute ComponentImplementation::meta_label;
	Uml::Attribute DeploymentPlans::meta_name;
	Uml::Attribute DeploymentPlan::meta_label;
	Uml::Attribute PackageConfigurations::meta_name;
	Uml::Attribute PackageConfiguration::meta_UUID;
	Uml::Attribute PackageConfiguration::meta_label;
	Uml::Attribute ComponentPackages::meta_name;
	Uml::Attribute ComponentPackage::meta_UUID;
	Uml::Attribute ComponentPackage::meta_label;
	Uml::Attribute ComponentPackageReference::meta_requiredName;
	Uml::Attribute ComponentPackageReference::meta_requiredUUID;
	Uml::Attribute ComponentPackageReference::meta_requiredType;
	Uml::Attribute Resource::meta_resourceType;
	Uml::Attribute Node::meta_label;
	Uml::Attribute SharedResource::meta_resourceType;
	Uml::Attribute Interconnect::meta_label;
	Uml::Attribute Bridge::meta_label;
	Uml::Attribute SatisfierProperty::meta_DataValue;
	Uml::Attribute SatisfierProperty::meta_SatisfierPropertyKind;
	Uml::Attribute ImplementationDependency::meta_requiredType;
	Uml::Attribute Property::meta_DataValue;
	Uml::Attribute RequirementSatisfier::meta_resourceType;
	Uml::Attribute ImplementationRequirement::meta_ResourceUsageKind;
	Uml::Attribute ImplementationRequirement::meta_componentPort;
	Uml::Attribute ImplementationRequirement::meta_resourcePort;
	Uml::Attribute RequirementBase::meta_resourceType;
	Uml::Attribute Domain::meta_label;
	Uml::Attribute Domain::meta_UUID;
	Uml::Attribute Targets::meta_name;
	Uml::Attribute ImplementationArtifact::meta_UUID;
	Uml::Attribute ImplementationArtifact::meta_label;
	Uml::Attribute ImplementationArtifact::meta_location;
	Uml::Attribute ImplementationArtifacts::meta_name;
	Uml::Attribute InterfaceDefinitions::meta_name;
	Uml::Attribute Constant::meta_value;
	Uml::Attribute PredefinedTypes::meta_name;
	Uml::Attribute Object::meta_abstract;
	Uml::Attribute Object::meta_local;
	Uml::Attribute ObjectByValue::meta_abstract;
	Uml::Attribute Component::meta_UUID;
	Uml::Attribute Component::meta_label;
	Uml::Attribute OutEventPort::meta_single_destination;
	Uml::Attribute RequiredRequestPort::meta_multiple_connections;
	Uml::Attribute Prefixable::meta_PrefixTag;
	Uml::Attribute Taggable::meta_VersionTag;
	Uml::Attribute Taggable::meta_SpecifyIdTag;
	Uml::Attribute RootFolder::meta_name;
	Uml::Attribute MgaObject::meta_position;
	Uml::Attribute MgaObject::meta_name;
	Uml::AssociationRole package::meta_dstpackage_end_;
	Uml::AssociationRole package::meta_srcpackage_end_;
	Uml::AssociationRole TopLevelPackage::meta_dstpackage, TopLevelPackage::meta_dstpackage_rev;
	Uml::AssociationRole ComponentInfoProperty::meta_srcComponentInfoProperty_end_;
	Uml::AssociationRole ComponentInfoProperty::meta_dstComponentInfoProperty_end_;
	Uml::AssociationRole ComponentProperty::meta_srcComponentProperty_end_;
	Uml::AssociationRole ComponentProperty::meta_dstComponentProperty_end_;
	Uml::AssociationRole ComponentType::meta_dstComponentProperty, ComponentType::meta_dstComponentProperty_rev;
	Uml::AssociationRole ComponentType::meta_dstComponentConfigProperty, ComponentType::meta_dstComponentConfigProperty_rev;
	Uml::AssociationRole ComponentType::meta_dstComponentInfoProperty, ComponentType::meta_dstComponentInfoProperty_rev;
	Uml::AssociationRole ComponentType::meta_ref;
	Uml::AssociationRole ComponentType::meta_CollocationGroup;
	Uml::AssociationRole ComponentType::meta_srcImplements, ComponentType::meta_srcImplements_rev;
	Uml::AssociationRole ComponentType::meta_srcPackageInterface, ComponentType::meta_srcPackageInterface_rev;
	Uml::AssociationRole ComponentConfigProperty::meta_srcComponentConfigProperty_end_;
	Uml::AssociationRole ComponentConfigProperty::meta_dstComponentConfigProperty_end_;
	Uml::AssociationRole ComponentPropertyDescription::meta_srcComponentProperty, ComponentPropertyDescription::meta_srcComponentProperty_rev;
	Uml::AssociationRole ConfigProperty::meta_srcConfigProperty_end_;
	Uml::AssociationRole ConfigProperty::meta_dstConfigProperty_end_;
	Uml::AssociationRole Implements::meta_dstImplements_end_;
	Uml::AssociationRole Implements::meta_srcImplements_end_;
	Uml::AssociationRole ImplementationCapability::meta_srcImplementationCapability_end_;
	Uml::AssociationRole ImplementationCapability::meta_dstImplementationCapability_end_;
	Uml::AssociationRole ImplementationDependsOn::meta_srcImplementationDependsOn_end_;
	Uml::AssociationRole ImplementationDependsOn::meta_dstImplementationDependsOn_end_;
	Uml::AssociationRole InfoProperty::meta_srcInfoProperty_end_;
	Uml::AssociationRole InfoProperty::meta_dstInfoProperty_end_;
	Uml::AssociationRole MonolithprimaryArtifact::meta_srcMonolithprimaryArtifact_end_;
	Uml::AssociationRole MonolithprimaryArtifact::meta_dstMonolithprimaryArtifact_end_;
	Uml::AssociationRole ComponentImplementationReference::meta_srcImplementation, ComponentImplementationReference::meta_srcImplementation_rev;
	Uml::AssociationRole ComponentImplementationReference::meta_ref;
	Uml::AssociationRole MonolithExecParameter::meta_srcMonolithExecParameter_end_;
	Uml::AssociationRole MonolithExecParameter::meta_dstMonolithExecParameter_end_;
	Uml::AssociationRole MonolithDeployRequirement::meta_srcMonolithDeployRequirement_end_;
	Uml::AssociationRole MonolithDeployRequirement::meta_dstMonolithDeployRequirement_end_;
	Uml::AssociationRole ComponentImplementation::meta_dstImplements, ComponentImplementation::meta_dstImplements_rev;
	Uml::AssociationRole ComponentImplementation::meta_referedbyComponentImplementationReference;
	Uml::AssociationRole ComponentImplementation::meta_dstInfoProperty, ComponentImplementation::meta_dstInfoProperty_rev;
	Uml::AssociationRole ComponentImplementation::meta_dstImplementationCapability, ComponentImplementation::meta_dstImplementationCapability_rev;
	Uml::AssociationRole ComponentImplementation::meta_dstConfigProperty, ComponentImplementation::meta_dstConfigProperty_rev;
	Uml::AssociationRole ComponentImplementation::meta_dstImplementationDependsOn, ComponentImplementation::meta_dstImplementationDependsOn_rev;
	Uml::AssociationRole MonolithicImplementation::meta_dstMonolithExecParameter, MonolithicImplementation::meta_dstMonolithExecParameter_rev;
	Uml::AssociationRole MonolithicImplementation::meta_dstMonolithDeployRequirement, MonolithicImplementation::meta_dstMonolithDeployRequirement_rev;
	Uml::AssociationRole MonolithicImplementation::meta_dstMonolithprimaryArtifact, MonolithicImplementation::meta_dstMonolithprimaryArtifact_rev;
	Uml::AssociationRole CollocationGroup::meta_members;
	Uml::AssociationRole CollocationGroup::meta_dstInstanceMapping, CollocationGroup::meta_dstInstanceMapping_rev;
	Uml::AssociationRole InstanceMapping::meta_srcInstanceMapping_end_;
	Uml::AssociationRole InstanceMapping::meta_dstInstanceMapping_end_;
	Uml::AssociationRole PackageConfSpecializedConfig::meta_dstPackageConfSpecializedConfig_end_;
	Uml::AssociationRole PackageConfSpecializedConfig::meta_srcPackageConfSpecializedConfig_end_;
	Uml::AssociationRole PackageConfConfigProperty::meta_srcPackageConfConfigProperty_end_;
	Uml::AssociationRole PackageConfConfigProperty::meta_dstPackageConfConfigProperty_end_;
	Uml::AssociationRole PackageConfBasePackage::meta_srcPackageConfBasePackage_end_;
	Uml::AssociationRole PackageConfBasePackage::meta_dstPackageConfBasePackage_end_;
	Uml::AssociationRole PackageConfigurationReference::meta_srcpackage, PackageConfigurationReference::meta_srcpackage_rev;
	Uml::AssociationRole PackageConfigurationReference::meta_ref;
	Uml::AssociationRole PackageConfigurationReference::meta_srcPackageConfSpecializedConfig, PackageConfigurationReference::meta_srcPackageConfSpecializedConfig_rev;
	Uml::AssociationRole PackageConfSelectRequirement::meta_srcPackageConfSelectRequirement_end_;
	Uml::AssociationRole PackageConfSelectRequirement::meta_dstPackageConfSelectRequirement_end_;
	Uml::AssociationRole PackageConfReference::meta_srcPackageConfReference_end_;
	Uml::AssociationRole PackageConfReference::meta_dstPackageConfReference_end_;
	Uml::AssociationRole PackageConfiguration::meta_referedbyPackageConfigurationReference;
	Uml::AssociationRole PackageConfiguration::meta_dstPackageConfSpecializedConfig, PackageConfiguration::meta_dstPackageConfSpecializedConfig_rev;
	Uml::AssociationRole PackageConfiguration::meta_dstPackageConfReference, PackageConfiguration::meta_dstPackageConfReference_rev;
	Uml::AssociationRole PackageConfiguration::meta_dstPackageConfBasePackage, PackageConfiguration::meta_dstPackageConfBasePackage_rev;
	Uml::AssociationRole PackageConfiguration::meta_dstPackageConfConfigProperty, PackageConfiguration::meta_dstPackageConfConfigProperty_rev;
	Uml::AssociationRole PackageConfiguration::meta_dstPackageConfSelectRequirement, PackageConfiguration::meta_dstPackageConfSelectRequirement_rev;
	Uml::AssociationRole PackageInterface::meta_dstPackageInterface_end_;
	Uml::AssociationRole PackageInterface::meta_srcPackageInterface_end_;
	Uml::AssociationRole PackageInfoProperty::meta_srcPackageInfoProperty_end_;
	Uml::AssociationRole PackageInfoProperty::meta_dstPackageInfoProperty_end_;
	Uml::AssociationRole PackageConfigProperty::meta_srcPackageConfigProperty_end_;
	Uml::AssociationRole PackageConfigProperty::meta_dstPackageConfigProperty_end_;
	Uml::AssociationRole Implementation::meta_dstImplementation_end_;
	Uml::AssociationRole Implementation::meta_srcImplementation_end_;
	Uml::AssociationRole ComponentPackage::meta_dstPackageInterface, ComponentPackage::meta_dstPackageInterface_rev;
	Uml::AssociationRole ComponentPackage::meta_dstImplementation, ComponentPackage::meta_dstImplementation_rev;
	Uml::AssociationRole ComponentPackage::meta_srcPackageConfBasePackage, ComponentPackage::meta_srcPackageConfBasePackage_rev;
	Uml::AssociationRole ComponentPackage::meta_dstPackageInfoProperty, ComponentPackage::meta_dstPackageInfoProperty_rev;
	Uml::AssociationRole ComponentPackage::meta_dstPackageConfigProperty, ComponentPackage::meta_dstPackageConfigProperty_rev;
	Uml::AssociationRole ComponentPackage::meta_referedbyComponentPackageReference;
	Uml::AssociationRole ComponentPackageReference::meta_srcPackageConfReference, ComponentPackageReference::meta_srcPackageConfReference_rev;
	Uml::AssociationRole ComponentPackageReference::meta_ref;
	Uml::AssociationRole Node::meta_dstNode2Interconnect, Node::meta_dstNode2Interconnect_rev;
	Uml::AssociationRole Node::meta_dstShares, Node::meta_dstShares_rev;
	Uml::AssociationRole Node::meta_srcInterconnect2Node, Node::meta_srcInterconnect2Node_rev;
	Uml::AssociationRole Node::meta_referedbyNodeReference;
	Uml::AssociationRole SharedResource::meta_srcShares, SharedResource::meta_srcShares_rev;
	Uml::AssociationRole Interconnect::meta_srcNode2Interconnect, Interconnect::meta_srcNode2Interconnect_rev;
	Uml::AssociationRole Interconnect::meta_dstInterconnect2Node, Interconnect::meta_dstInterconnect2Node_rev;
	Uml::AssociationRole Interconnect::meta_dstInterconnect2Bridge, Interconnect::meta_dstInterconnect2Bridge_rev;
	Uml::AssociationRole Interconnect::meta_srcBridge2Interconnect, Interconnect::meta_srcBridge2Interconnect_rev;
	Uml::AssociationRole NodeReference::meta_srcInstanceMapping, NodeReference::meta_srcInstanceMapping_rev;
	Uml::AssociationRole NodeReference::meta_ref;
	Uml::AssociationRole Bridge::meta_srcInterconnect2Bridge, Bridge::meta_srcInterconnect2Bridge_rev;
	Uml::AssociationRole Bridge::meta_dstBridge2Interconnect, Bridge::meta_dstBridge2Interconnect_rev;
	Uml::AssociationRole Requirement::meta_srcPackageConfSelectRequirement, Requirement::meta_srcPackageConfSelectRequirement_rev;
	Uml::AssociationRole Requirement::meta_srcAssemblyDeployRequirement, Requirement::meta_srcAssemblyDeployRequirement_rev;
	Uml::AssociationRole Requirement::meta_srcArtifactDeployRequirement, Requirement::meta_srcArtifactDeployRequirement_rev;
	Uml::AssociationRole Requirement::meta_srcAssemblyselectRequirement, Requirement::meta_srcAssemblyselectRequirement_rev;
	Uml::AssociationRole DataType::meta_ref;
	Uml::AssociationRole ImplementationDependency::meta_srcImplementationDependsOn, ImplementationDependency::meta_srcImplementationDependsOn_rev;
	Uml::AssociationRole Property::meta_srcComponentConfigProperty, Property::meta_srcComponentConfigProperty_rev;
	Uml::AssociationRole Property::meta_srcComponentInfoProperty, Property::meta_srcComponentInfoProperty_rev;
	Uml::AssociationRole Property::meta_srcInfoProperty, Property::meta_srcInfoProperty_rev;
	Uml::AssociationRole Property::meta_srcConfigProperty, Property::meta_srcConfigProperty_rev;
	Uml::AssociationRole Property::meta_srcMonolithExecParameter, Property::meta_srcMonolithExecParameter_rev;
	Uml::AssociationRole Property::meta_srcPackageConfConfigProperty, Property::meta_srcPackageConfConfigProperty_rev;
	Uml::AssociationRole Property::meta_srcPackageInfoProperty, Property::meta_srcPackageInfoProperty_rev;
	Uml::AssociationRole Property::meta_srcPackageConfigProperty, Property::meta_srcPackageConfigProperty_rev;
	Uml::AssociationRole Property::meta_srcArtifactExecParameter, Property::meta_srcArtifactExecParameter_rev;
	Uml::AssociationRole Property::meta_srcArtifactInfoProperty, Property::meta_srcArtifactInfoProperty_rev;
	Uml::AssociationRole Property::meta_srcAssemblyConfigProperty, Property::meta_srcAssemblyConfigProperty_rev;
	Uml::AssociationRole Capability::meta_srcImplementationCapability, Capability::meta_srcImplementationCapability_rev;
	Uml::AssociationRole ImplementationRequirement::meta_srcMonolithDeployRequirement, ImplementationRequirement::meta_srcMonolithDeployRequirement_rev;
	Uml::AssociationRole Interconnect2Node::meta_dstInterconnect2Node_end_;
	Uml::AssociationRole Interconnect2Node::meta_srcInterconnect2Node_end_;
	Uml::AssociationRole Bridge2Interconnect::meta_dstBridge2Interconnect_end_;
	Uml::AssociationRole Bridge2Interconnect::meta_srcBridge2Interconnect_end_;
	Uml::AssociationRole Shares::meta_srcShares_end_;
	Uml::AssociationRole Shares::meta_dstShares_end_;
	Uml::AssociationRole Node2Interconnect::meta_srcNode2Interconnect_end_;
	Uml::AssociationRole Node2Interconnect::meta_dstNode2Interconnect_end_;
	Uml::AssociationRole Interconnect2Bridge::meta_srcInterconnect2Bridge_end_;
	Uml::AssociationRole Interconnect2Bridge::meta_dstInterconnect2Bridge_end_;
	Uml::AssociationRole AssemblyDeployRequirement::meta_dstAssemblyDeployRequirement_end_;
	Uml::AssociationRole AssemblyDeployRequirement::meta_srcAssemblyDeployRequirement_end_;
	Uml::AssociationRole invoke::meta_dstinvoke_end_;
	Uml::AssociationRole invoke::meta_srcinvoke_end_;
	Uml::AssociationRole AssemblyselectRequirement::meta_dstAssemblyselectRequirement_end_;
	Uml::AssociationRole AssemblyselectRequirement::meta_srcAssemblyselectRequirement_end_;
	Uml::AssociationRole emit::meta_srcemit_end_;
	Uml::AssociationRole emit::meta_dstemit_end_;
	Uml::AssociationRole PublishConnector::meta_dstAssemblyDeployRequirement, PublishConnector::meta_dstAssemblyDeployRequirement_rev;
	Uml::AssociationRole PublishConnector::meta_dstdeliverTo, PublishConnector::meta_dstdeliverTo_rev;
	Uml::AssociationRole PublishConnector::meta_srcpublish, PublishConnector::meta_srcpublish_rev;
	Uml::AssociationRole deliverTo::meta_srcdeliverTo_end_;
	Uml::AssociationRole deliverTo::meta_dstdeliverTo_end_;
	Uml::AssociationRole publish::meta_dstpublish_end_;
	Uml::AssociationRole publish::meta_srcpublish_end_;
	Uml::AssociationRole AssemblyConfigProperty::meta_dstAssemblyConfigProperty_end_;
	Uml::AssociationRole AssemblyConfigProperty::meta_srcAssemblyConfigProperty_end_;
	Uml::AssociationRole ArtifactExecParameter::meta_dstArtifactExecParameter_end_;
	Uml::AssociationRole ArtifactExecParameter::meta_srcArtifactExecParameter_end_;
	Uml::AssociationRole ImplementationArtifactReference::meta_srcMonolithprimaryArtifact, ImplementationArtifactReference::meta_srcMonolithprimaryArtifact_rev;
	Uml::AssociationRole ImplementationArtifactReference::meta_ref;
	Uml::AssociationRole ImplementationArtifactReference::meta_srcArtifactDependsOn, ImplementationArtifactReference::meta_srcArtifactDependsOn_rev;
	Uml::AssociationRole ArtifactDependsOn::meta_dstArtifactDependsOn_end_;
	Uml::AssociationRole ArtifactDependsOn::meta_srcArtifactDependsOn_end_;
	Uml::AssociationRole ImplementationArtifact::meta_dstArtifactDeployRequirement, ImplementationArtifact::meta_dstArtifactDeployRequirement_rev;
	Uml::AssociationRole ImplementationArtifact::meta_dstArtifactExecParameter, ImplementationArtifact::meta_dstArtifactExecParameter_rev;
	Uml::AssociationRole ImplementationArtifact::meta_dstArtifactInfoProperty, ImplementationArtifact::meta_dstArtifactInfoProperty_rev;
	Uml::AssociationRole ImplementationArtifact::meta_referedbyImplementationArtifactReference;
	Uml::AssociationRole ImplementationArtifact::meta_dstArtifactDependsOn, ImplementationArtifact::meta_dstArtifactDependsOn_rev;
	Uml::AssociationRole ArtifactDeployRequirement::meta_dstArtifactDeployRequirement_end_;
	Uml::AssociationRole ArtifactDeployRequirement::meta_srcArtifactDeployRequirement_end_;
	Uml::AssociationRole ArtifactInfoProperty::meta_dstArtifactInfoProperty_end_;
	Uml::AssociationRole ArtifactInfoProperty::meta_srcArtifactInfoProperty_end_;
	Uml::AssociationRole File::meta_referedbyFileRef;
	Uml::AssociationRole Exception::meta_referedbySetException;
	Uml::AssociationRole Exception::meta_referedbyExceptionRef;
	Uml::AssociationRole Exception::meta_referedbyGetException;
	Uml::AssociationRole ExceptionRef::meta_ref;
	Uml::AssociationRole FileRef::meta_ref;
	Uml::AssociationRole ConstantType::meta_referedbyDiscriminator;
	Uml::AssociationRole ConstantType::meta_referedbyConstant;
	Uml::AssociationRole Constant::meta_ref;
	Uml::AssociationRole PredefinedType::meta_referedbyDataType;
	Uml::AssociationRole Label::meta_srcLabelConnection, Label::meta_srcLabelConnection_rev;
	Uml::AssociationRole LabelConnection::meta_dstLabelConnection_end_;
	Uml::AssociationRole LabelConnection::meta_srcLabelConnection_end_;
	Uml::AssociationRole MemberType::meta_referedbyAlias;
	Uml::AssociationRole MemberType::meta_referedbyCollection;
	Uml::AssociationRole MemberType::meta_referedbyBoxed;
	Uml::AssociationRole MemberType::meta_referedbyMember;
	Uml::AssociationRole MemberType::meta_referedbyAttributeMember;
	Uml::AssociationRole MemberType::meta_referedbyInParameter;
	Uml::AssociationRole MemberType::meta_referedbyOutParameter;
	Uml::AssociationRole MemberType::meta_referedbyReturnType;
	Uml::AssociationRole MemberType::meta_referedbyInoutParameter;
	Uml::AssociationRole Discriminator::meta_ref;
	Uml::AssociationRole Collection::meta_ref;
	Uml::AssociationRole Alias::meta_ref;
	Uml::AssociationRole Boxed::meta_ref;
	Uml::AssociationRole Member::meta_dstLabelConnection, Member::meta_dstLabelConnection_rev;
	Uml::AssociationRole Member::meta_ref;
	Uml::AssociationRole Member::meta_dstMakeMemberPrivate, Member::meta_dstMakeMemberPrivate_rev;
	Uml::AssociationRole Object::meta_referedbySupports;
	Uml::AssociationRole Inherits::meta_ref;
	Uml::AssociationRole Event::meta_referedbyInEventPort;
	Uml::AssociationRole Event::meta_referedbyOutEventPort;
	Uml::AssociationRole ValueObject::meta_referedbyLookupKey;
	Uml::AssociationRole Inheritable::meta_referedbyInherits;
	Uml::AssociationRole LookupKey::meta_ref;
	Uml::AssociationRole SetException::meta_ref;
	Uml::AssociationRole GetException::meta_ref;
	Uml::AssociationRole PrivateFlag::meta_srcMakeMemberPrivate, PrivateFlag::meta_srcMakeMemberPrivate_rev;
	Uml::AssociationRole MakeMemberPrivate::meta_srcMakeMemberPrivate_end_;
	Uml::AssociationRole MakeMemberPrivate::meta_dstMakeMemberPrivate_end_;
	Uml::AssociationRole AttributeMember::meta_ref;
	Uml::AssociationRole Supports::meta_ref;
	Uml::AssociationRole InParameter::meta_ref;
	Uml::AssociationRole OutParameter::meta_ref;
	Uml::AssociationRole ReturnType::meta_ref;
	Uml::AssociationRole InoutParameter::meta_ref;
	Uml::AssociationRole Component::meta_referedbyComponentType;
	Uml::AssociationRole Component::meta_dstAssemblyselectRequirement, Component::meta_dstAssemblyselectRequirement_rev;
	Uml::AssociationRole Component::meta_dstAssemblyConfigProperty, Component::meta_dstAssemblyConfigProperty_rev;
	Uml::AssociationRole Component::meta_referedbyComponentRef;
	Uml::AssociationRole OutEventPort::meta_dstpublish, OutEventPort::meta_dstpublish_rev;
	Uml::AssociationRole OutEventPort::meta_ref;
	Uml::AssociationRole OutEventPort::meta_dstemit, OutEventPort::meta_dstemit_rev;
	Uml::AssociationRole ProvidedRequestPort::meta_srcinvoke, ProvidedRequestPort::meta_srcinvoke_rev;
	Uml::AssociationRole ProvidedRequestPort::meta_ref;
	Uml::AssociationRole RequiredRequestPort::meta_dstinvoke, RequiredRequestPort::meta_dstinvoke_rev;
	Uml::AssociationRole RequiredRequestPort::meta_ref;
	Uml::AssociationRole InEventPort::meta_srcdeliverTo, InEventPort::meta_srcdeliverTo_rev;
	Uml::AssociationRole InEventPort::meta_ref;
	Uml::AssociationRole InEventPort::meta_srcemit, InEventPort::meta_srcemit_rev;
	Uml::AssociationRole ComponentRef::meta_ref;
	Uml::AssociationRole Manageable::meta_srcManagesComponent, Manageable::meta_srcManagesComponent_rev;
	Uml::AssociationRole ComponentFactory::meta_dstManagesComponent, ComponentFactory::meta_dstManagesComponent_rev;
	Uml::AssociationRole ManagesComponent::meta_dstManagesComponent_end_;
	Uml::AssociationRole ManagesComponent::meta_srcManagesComponent_end_;
	Uml::AssociationRole Provideable::meta_referedbyProvidedRequestPort;
	Uml::AssociationRole Provideable::meta_referedbyRequiredRequestPort;
	Uml::CompositionParentRole package::meta_TopLevelPackageContainer_parent;
	Uml::CompositionParentRole TopLevelPackageContainer::meta_TopLevelPackages_parent;
	Uml::CompositionParentRole TopLevelPackages::meta_RootFolder_parent;
	Uml::CompositionParentRole TopLevelPackage::meta_TopLevelPackageContainer_parent;
	Uml::CompositionParentRole ComponentInfoProperty::meta_ComponentContainer_parent;
	Uml::CompositionParentRole ComponentContainer::meta_ComponentTypes_parent;
	Uml::CompositionParentRole ComponentProperty::meta_ComponentContainer_parent;
	Uml::CompositionParentRole ComponentType::meta_ComponentContainer_parent;
	Uml::CompositionParentRole ComponentType::meta_ComponentImplementationContainer_parent;
	Uml::CompositionParentRole ComponentType::meta_DeploymentPlan_parent;
	Uml::CompositionParentRole ComponentType::meta_PackageContainer_parent;
	Uml::CompositionParentRole ComponentTypes::meta_RootFolder_parent;
	Uml::CompositionParentRole ComponentConfigProperty::meta_ComponentContainer_parent;
	Uml::CompositionParentRole ComponentPropertyDescription::meta_ComponentContainer_parent;
	Uml::CompositionParentRole ConfigProperty::meta_ComponentImplementationContainer_parent;
	Uml::CompositionParentRole Implements::meta_ComponentImplementationContainer_parent;
	Uml::CompositionParentRole ImplementationCapability::meta_ComponentImplementationContainer_parent;
	Uml::CompositionParentRole ImplementationDependsOn::meta_ComponentImplementationContainer_parent;
	Uml::CompositionParentRole InfoProperty::meta_ComponentImplementationContainer_parent;
	Uml::CompositionParentRole ComponentImplementations::meta_RootFolder_parent;
	Uml::CompositionParentRole MonolithprimaryArtifact::meta_ComponentImplementationContainer_parent;
	Uml::CompositionParentRole ComponentImplementationReference::meta_PackageContainer_parent;
	Uml::CompositionParentRole MonolithExecParameter::meta_ComponentImplementationContainer_parent;
	Uml::CompositionParentRole ComponentImplementationContainer::meta_ComponentImplementations_parent;
	Uml::CompositionParentRole MonolithDeployRequirement::meta_ComponentImplementationContainer_parent;
	Uml::CompositionParentRole ComponentImplementation::meta_ComponentImplementationContainer_parent;
	Uml::CompositionParentRole DeploymentPlans::meta_RootFolder_parent;
	Uml::CompositionParentRole DeploymentPlan::meta_DeploymentPlans_parent;
	Uml::CompositionParentRole CollocationGroup::meta_DeploymentPlan_parent;
	Uml::CompositionParentRole InstanceMapping::meta_DeploymentPlan_parent;
	Uml::CompositionParentRole PackageConfSpecializedConfig::meta_PackageConfigurationContainer_parent;
	Uml::CompositionParentRole PackageConfigurationContainer::meta_PackageConfigurations_parent;
	Uml::CompositionParentRole PackageConfConfigProperty::meta_PackageConfigurationContainer_parent;
	Uml::CompositionParentRole PackageConfBasePackage::meta_PackageConfigurationContainer_parent;
	Uml::CompositionParentRole PackageConfigurationReference::meta_TopLevelPackageContainer_parent;
	Uml::CompositionParentRole PackageConfigurations::meta_RootFolder_parent;
	Uml::CompositionParentRole PackageConfSelectRequirement::meta_PackageConfigurationContainer_parent;
	Uml::CompositionParentRole PackageConfReference::meta_PackageConfigurationContainer_parent;
	Uml::CompositionParentRole PackageConfiguration::meta_PackageConfigurationContainer_parent;
	Uml::CompositionParentRole PackageInterface::meta_PackageContainer_parent;
	Uml::CompositionParentRole PackageInfoProperty::meta_PackageContainer_parent;
	Uml::CompositionParentRole PackageConfigProperty::meta_PackageContainer_parent;
	Uml::CompositionParentRole ComponentPackages::meta_RootFolder_parent;
	Uml::CompositionParentRole PackageContainer::meta_ComponentPackages_parent;
	Uml::CompositionParentRole Implementation::meta_PackageContainer_parent;
	Uml::CompositionParentRole ComponentPackage::meta_PackageConfigurationContainer_parent;
	Uml::CompositionParentRole ComponentPackage::meta_PackageContainer_parent;
	Uml::CompositionParentRole ComponentPackage::meta_ComponentAssembly_parent;
	Uml::CompositionParentRole ComponentPackageReference::meta_PackageConfigurationContainer_parent;
	Uml::CompositionParentRole ComponentPackageReference::meta_ComponentAssembly_parent;
	Uml::CompositionParentRole Resource::meta_Interconnect_parent;
	Uml::CompositionParentRole Resource::meta_Node_parent;
	Uml::CompositionParentRole Resource::meta_Bridge_parent;
	Uml::CompositionParentRole NodeReference::meta_DeploymentPlan_parent;
	Uml::CompositionParentRole SatisfierProperty::meta_Resource_parent;
	Uml::CompositionParentRole SatisfierProperty::meta_SharedResource_parent;
	Uml::CompositionParentRole SatisfierProperty::meta_RequirementSatisfier_parent;
	Uml::CompositionParentRole Requirement::meta_PackageConfigurationContainer_parent;
	Uml::CompositionParentRole Requirement::meta_ComponentAssembly_parent;
	Uml::CompositionParentRole Requirement::meta_ArtifactContainer_parent;
	Uml::CompositionParentRole DataType::meta_ComponentPropertyDescription_parent;
	Uml::CompositionParentRole DataType::meta_SatisfierProperty_parent;
	Uml::CompositionParentRole DataType::meta_Property_parent;
	Uml::CompositionParentRole ImplementationDependency::meta_ComponentImplementationContainer_parent;
	Uml::CompositionParentRole Property::meta_ComponentContainer_parent;
	Uml::CompositionParentRole Property::meta_ComponentImplementationContainer_parent;
	Uml::CompositionParentRole Property::meta_PackageConfigurationContainer_parent;
	Uml::CompositionParentRole Property::meta_PackageContainer_parent;
	Uml::CompositionParentRole Property::meta_RequirementBase_parent;
	Uml::CompositionParentRole Property::meta_Domain_parent;
	Uml::CompositionParentRole Property::meta_ComponentAssembly_parent;
	Uml::CompositionParentRole Property::meta_ArtifactContainer_parent;
	Uml::CompositionParentRole Capability::meta_ComponentImplementationContainer_parent;
	Uml::CompositionParentRole ImplementationRequirement::meta_ComponentImplementationContainer_parent;
	Uml::CompositionParentRole Elements::meta_Domain_parent;
	Uml::CompositionParentRole Domain::meta_Targets_parent;
	Uml::CompositionParentRole Interconnect2Node::meta_Domain_parent;
	Uml::CompositionParentRole Bridge2Interconnect::meta_Domain_parent;
	Uml::CompositionParentRole Shares::meta_Domain_parent;
	Uml::CompositionParentRole Node2Interconnect::meta_Domain_parent;
	Uml::CompositionParentRole Interconnect2Bridge::meta_Domain_parent;
	Uml::CompositionParentRole Targets::meta_RootFolder_parent;
	Uml::CompositionParentRole AssemblyDeployRequirement::meta_ComponentAssembly_parent;
	Uml::CompositionParentRole invoke::meta_ComponentAssembly_parent;
	Uml::CompositionParentRole AssemblyselectRequirement::meta_ComponentAssembly_parent;
	Uml::CompositionParentRole emit::meta_ComponentAssembly_parent;
	Uml::CompositionParentRole PublishConnector::meta_ComponentAssembly_parent;
	Uml::CompositionParentRole deliverTo::meta_ComponentAssembly_parent;
	Uml::CompositionParentRole publish::meta_ComponentAssembly_parent;
	Uml::CompositionParentRole AssemblyConfigProperty::meta_ComponentAssembly_parent;
	Uml::CompositionParentRole ArtifactExecParameter::meta_ArtifactContainer_parent;
	Uml::CompositionParentRole ImplementationArtifactReference::meta_ComponentImplementationContainer_parent;
	Uml::CompositionParentRole ImplementationArtifactReference::meta_ArtifactContainer_parent;
	Uml::CompositionParentRole ArtifactDependsOn::meta_ArtifactContainer_parent;
	Uml::CompositionParentRole ImplementationArtifact::meta_ArtifactContainer_parent;
	Uml::CompositionParentRole ImplementationArtifacts::meta_RootFolder_parent;
	Uml::CompositionParentRole ArtifactDeployRequirement::meta_ArtifactContainer_parent;
	Uml::CompositionParentRole ArtifactInfoProperty::meta_ArtifactContainer_parent;
	Uml::CompositionParentRole ArtifactContainer::meta_ImplementationArtifacts_parent;
	Uml::CompositionParentRole Package::meta_Package_parent;
	Uml::CompositionParentRole Package::meta_File_parent;
	Uml::CompositionParentRole File::meta_InterfaceDefinitions_parent;
	Uml::CompositionParentRole Exception::meta_Package_parent;
	Uml::CompositionParentRole Exception::meta_File_parent;
	Uml::CompositionParentRole Exception::meta_HasOperations_parent;
	Uml::CompositionParentRole ExceptionRef::meta_HasExceptions_parent;
	Uml::CompositionParentRole FileRef::meta_File_parent;
	Uml::CompositionParentRole InterfaceDefinitions::meta_RootFolder_parent;
	Uml::CompositionParentRole Constant::meta_Package_parent;
	Uml::CompositionParentRole Constant::meta_File_parent;
	Uml::CompositionParentRole Constant::meta_HasOperations_parent;
	Uml::CompositionParentRole PredefinedTypes::meta_RootFolder_parent;
	Uml::CompositionParentRole PredefinedType::meta_PredefinedTypes_parent;
	Uml::CompositionParentRole EnumValue::meta_Enum_parent;
	Uml::CompositionParentRole Label::meta_SwitchedAggregate_parent;
	Uml::CompositionParentRole LabelConnection::meta_SwitchedAggregate_parent;
	Uml::CompositionParentRole Discriminator::meta_SwitchedAggregate_parent;
	Uml::CompositionParentRole NamedType::meta_Package_parent;
	Uml::CompositionParentRole NamedType::meta_File_parent;
	Uml::CompositionParentRole Member::meta_Exception_parent;
	Uml::CompositionParentRole Member::meta_Aggregate_parent;
	Uml::CompositionParentRole Member::meta_SwitchedAggregate_parent;
	Uml::CompositionParentRole Member::meta_ObjectByValue_parent;
	Uml::CompositionParentRole NoInheritable::meta_HasOperations_parent;
	Uml::CompositionParentRole Inherits::meta_Inheritable_parent;
	Uml::CompositionParentRole ReadonlyAttribute::meta_Inheritable_parent;
	Uml::CompositionParentRole LookupKey::meta_ComponentFactory_parent;
	Uml::CompositionParentRole SetException::meta_Attribute_parent;
	Uml::CompositionParentRole GetException::meta_ReadonlyAttribute_parent;
	Uml::CompositionParentRole PrivateFlag::meta_ObjectByValue_parent;
	Uml::CompositionParentRole MakeMemberPrivate::meta_ObjectByValue_parent;
	Uml::CompositionParentRole AttributeMember::meta_ReadonlyAttribute_parent;
	Uml::CompositionParentRole Supports::meta_SupportsInterfaces_parent;
	Uml::CompositionParentRole FactoryOperation::meta_ObjectByValue_parent;
	Uml::CompositionParentRole FactoryOperation::meta_ComponentFactory_parent;
	Uml::CompositionParentRole LookupOperation::meta_ComponentFactory_parent;
	Uml::CompositionParentRole TwowayOperation::meta_HasOperations_parent;
	Uml::CompositionParentRole OnewayOperation::meta_HasOperations_parent;
	Uml::CompositionParentRole InParameter::meta_OperationBase_parent;
	Uml::CompositionParentRole OutParameter::meta_TwowayOperation_parent;
	Uml::CompositionParentRole ReturnType::meta_TwowayOperation_parent;
	Uml::CompositionParentRole InoutParameter::meta_TwowayOperation_parent;
	Uml::CompositionParentRole Component::meta_ComponentAssembly_parent;
	Uml::CompositionParentRole Port::meta_Component_parent;
	Uml::CompositionParentRole ComponentRef::meta_Package_parent;
	Uml::CompositionParentRole ComponentRef::meta_File_parent;
	Uml::CompositionParentRole ManagesComponent::meta_Package_parent;
	Uml::CompositionParentRole ManagesComponent::meta_File_parent;
	Uml::CompositionParentRole RootFolder::meta_RootFolder_parent;
	Uml::CompositionChildRole TopLevelPackageContainer::meta_TopLevelPackage_child;
	Uml::CompositionChildRole TopLevelPackageContainer::meta_PackageConfigurationReference_child;
	Uml::CompositionChildRole TopLevelPackageContainer::meta_package_child;
	Uml::CompositionChildRole TopLevelPackages::meta_TopLevelPackageContainer_children;
	Uml::CompositionChildRole ComponentContainer::meta_ComponentConfigProperty_children;
	Uml::CompositionChildRole ComponentContainer::meta_ComponentProperty_children;
	Uml::CompositionChildRole ComponentContainer::meta_ComponentInfoProperty_children;
	Uml::CompositionChildRole ComponentContainer::meta_Property_children;
	Uml::CompositionChildRole ComponentContainer::meta_ComponentType_child;
	Uml::CompositionChildRole ComponentContainer::meta_ComponentPropertyDescription_children;
	Uml::CompositionChildRole ComponentTypes::meta_ComponentContainer_children;
	Uml::CompositionChildRole ComponentPropertyDescription::meta_DataType_child;
	Uml::CompositionChildRole ComponentImplementations::meta_ComponentImplementationContainer_children;
	Uml::CompositionChildRole ComponentImplementationContainer::meta_ComponentType_children;
	Uml::CompositionChildRole ComponentImplementationContainer::meta_ConfigProperty_children;
	Uml::CompositionChildRole ComponentImplementationContainer::meta_MonolithprimaryArtifact_children;
	Uml::CompositionChildRole ComponentImplementationContainer::meta_Property_children;
	Uml::CompositionChildRole ComponentImplementationContainer::meta_ImplementationCapability_children;
	Uml::CompositionChildRole ComponentImplementationContainer::meta_ImplementationDependsOn_children;
	Uml::CompositionChildRole ComponentImplementationContainer::meta_Implements_children;
	Uml::CompositionChildRole ComponentImplementationContainer::meta_MonolithDeployRequirement_children;
	Uml::CompositionChildRole ComponentImplementationContainer::meta_ImplementationRequirement_children;
	Uml::CompositionChildRole ComponentImplementationContainer::meta_ImplementationArtifactReference_children;
	Uml::CompositionChildRole ComponentImplementationContainer::meta_InfoProperty_children;
	Uml::CompositionChildRole ComponentImplementationContainer::meta_Capability_children;
	Uml::CompositionChildRole ComponentImplementationContainer::meta_MonolithExecParameter_children;
	Uml::CompositionChildRole ComponentImplementationContainer::meta_ComponentImplementation_child;
	Uml::CompositionChildRole ComponentImplementationContainer::meta_ImplementationDependency_children;
	Uml::CompositionChildRole DeploymentPlans::meta_DeploymentPlan_children;
	Uml::CompositionChildRole DeploymentPlan::meta_InstanceMapping_children;
	Uml::CompositionChildRole DeploymentPlan::meta_CollocationGroup_children;
	Uml::CompositionChildRole DeploymentPlan::meta_ComponentType_children;
	Uml::CompositionChildRole DeploymentPlan::meta_NodeReference_children;
	Uml::CompositionChildRole PackageConfigurationContainer::meta_PackageConfSelectRequirement_children;
	Uml::CompositionChildRole PackageConfigurationContainer::meta_PackageConfiguration_children;
	Uml::CompositionChildRole PackageConfigurationContainer::meta_PackageConfBasePackage_child;
	Uml::CompositionChildRole PackageConfigurationContainer::meta_PackageConfReference_child;
	Uml::CompositionChildRole PackageConfigurationContainer::meta_PackageConfConfigProperty_children;
	Uml::CompositionChildRole PackageConfigurationContainer::meta_Property_children;
	Uml::CompositionChildRole PackageConfigurationContainer::meta_Requirement_children;
	Uml::CompositionChildRole PackageConfigurationContainer::meta_ComponentPackageReference_child;
	Uml::CompositionChildRole PackageConfigurationContainer::meta_PackageConfSpecializedConfig_child;
	Uml::CompositionChildRole PackageConfigurationContainer::meta_ComponentPackage_child;
	Uml::CompositionChildRole PackageConfigurations::meta_PackageConfigurationContainer_children;
	Uml::CompositionChildRole ComponentPackages::meta_PackageContainer_children;
	Uml::CompositionChildRole PackageContainer::meta_Implementation_children;
	Uml::CompositionChildRole PackageContainer::meta_PackageInterface_child;
	Uml::CompositionChildRole PackageContainer::meta_PackageConfigProperty_children;
	Uml::CompositionChildRole PackageContainer::meta_Property_children;
	Uml::CompositionChildRole PackageContainer::meta_ComponentType_children;
	Uml::CompositionChildRole PackageContainer::meta_ComponentImplementationReference_children;
	Uml::CompositionChildRole PackageContainer::meta_ComponentPackage_child;
	Uml::CompositionChildRole PackageContainer::meta_PackageInfoProperty_children;
	Uml::CompositionChildRole Resource::meta_SatisfierProperty_children;
	Uml::CompositionChildRole Node::meta_Resource_children;
	Uml::CompositionChildRole SharedResource::meta_SatisfierProperty_children;
	Uml::CompositionChildRole Interconnect::meta_Resource_children;
	Uml::CompositionChildRole Bridge::meta_Resource_children;
	Uml::CompositionChildRole SatisfierProperty::meta_DataType_child;
	Uml::CompositionChildRole Property::meta_DataType_child;
	Uml::CompositionChildRole RequirementSatisfier::meta_SatisfierProperty_children;
	Uml::CompositionChildRole RequirementBase::meta_Property_children;
	Uml::CompositionChildRole Domain::meta_Node2Interconnect_children;
	Uml::CompositionChildRole Domain::meta_Interconnect2Bridge_children;
	Uml::CompositionChildRole Domain::meta_Bridge2Interconnect_children;
	Uml::CompositionChildRole Domain::meta_Property_children;
	Uml::CompositionChildRole Domain::meta_Shares_children;
	Uml::CompositionChildRole Domain::meta_Interconnect2Node_children;
	Uml::CompositionChildRole Domain::meta_Elements_children;
	Uml::CompositionChildRole Targets::meta_Domain_children;
	Uml::CompositionChildRole ComponentAssembly::meta_PublishConnector_children;
	Uml::CompositionChildRole ComponentAssembly::meta_AssemblyConfigProperty_children;
	Uml::CompositionChildRole ComponentAssembly::meta_Requirement_children;
	Uml::CompositionChildRole ComponentAssembly::meta_ComponentPackageReference_children;
	Uml::CompositionChildRole ComponentAssembly::meta_emit_children;
	Uml::CompositionChildRole ComponentAssembly::meta_Component_children;
	Uml::CompositionChildRole ComponentAssembly::meta_AssemblyDeployRequirement_children;
	Uml::CompositionChildRole ComponentAssembly::meta_publish_children;
	Uml::CompositionChildRole ComponentAssembly::meta_AssemblyselectRequirement_children;
	Uml::CompositionChildRole ComponentAssembly::meta_ComponentPackage_children;
	Uml::CompositionChildRole ComponentAssembly::meta_invoke_children;
	Uml::CompositionChildRole ComponentAssembly::meta_deliverTo_children;
	Uml::CompositionChildRole ComponentAssembly::meta_Property_children;
	Uml::CompositionChildRole ImplementationArtifacts::meta_ArtifactContainer_children;
	Uml::CompositionChildRole ArtifactContainer::meta_Requirement_children;
	Uml::CompositionChildRole ArtifactContainer::meta_ArtifactExecParameter_children;
	Uml::CompositionChildRole ArtifactContainer::meta_Property_children;
	Uml::CompositionChildRole ArtifactContainer::meta_ArtifactDeployRequirement_children;
	Uml::CompositionChildRole ArtifactContainer::meta_ImplementationArtifactReference_children;
	Uml::CompositionChildRole ArtifactContainer::meta_ImplementationArtifact_children;
	Uml::CompositionChildRole ArtifactContainer::meta_ArtifactInfoProperty_children;
	Uml::CompositionChildRole ArtifactContainer::meta_ArtifactDependsOn_children;
	Uml::CompositionChildRole Package::meta_Exception_children;
	Uml::CompositionChildRole Package::meta_Package_children;
	Uml::CompositionChildRole Package::meta_Constant_children;
	Uml::CompositionChildRole Package::meta_NamedType_children;
	Uml::CompositionChildRole Package::meta_ComponentRef_children;
	Uml::CompositionChildRole Package::meta_ManagesComponent_children;
	Uml::CompositionChildRole File::meta_Exception_children;
	Uml::CompositionChildRole File::meta_Package_children;
	Uml::CompositionChildRole File::meta_FileRef_children;
	Uml::CompositionChildRole File::meta_Constant_children;
	Uml::CompositionChildRole File::meta_NamedType_children;
	Uml::CompositionChildRole File::meta_ComponentRef_children;
	Uml::CompositionChildRole File::meta_ManagesComponent_children;
	Uml::CompositionChildRole Exception::meta_Member_children;
	Uml::CompositionChildRole InterfaceDefinitions::meta_File_children;
	Uml::CompositionChildRole PredefinedTypes::meta_PredefinedType_children;
	Uml::CompositionChildRole Enum::meta_EnumValue_children;
	Uml::CompositionChildRole Aggregate::meta_Member_children;
	Uml::CompositionChildRole SwitchedAggregate::meta_Label_children;
	Uml::CompositionChildRole SwitchedAggregate::meta_LabelConnection_children;
	Uml::CompositionChildRole SwitchedAggregate::meta_Member_children;
	Uml::CompositionChildRole SwitchedAggregate::meta_Discriminator_child;
	Uml::CompositionChildRole Inheritable::meta_ReadonlyAttribute_children;
	Uml::CompositionChildRole Inheritable::meta_Inherits_children;
	Uml::CompositionChildRole HasOperations::meta_TwowayOperation_children;
	Uml::CompositionChildRole HasOperations::meta_OnewayOperation_children;
	Uml::CompositionChildRole HasOperations::meta_Exception_children;
	Uml::CompositionChildRole HasOperations::meta_Constant_children;
	Uml::CompositionChildRole HasOperations::meta_NoInheritable_children;
	Uml::CompositionChildRole SupportsInterfaces::meta_Supports_children;
	Uml::CompositionChildRole ReadonlyAttribute::meta_AttributeMember_child;
	Uml::CompositionChildRole ReadonlyAttribute::meta_GetException_children;
	Uml::CompositionChildRole ObjectByValue::meta_PrivateFlag_children;
	Uml::CompositionChildRole ObjectByValue::meta_FactoryOperation_children;
	Uml::CompositionChildRole ObjectByValue::meta_MakeMemberPrivate_children;
	Uml::CompositionChildRole ObjectByValue::meta_Member_children;
	Uml::CompositionChildRole Attribute::meta_SetException_children;
	Uml::CompositionChildRole OperationBase::meta_InParameter_children;
	Uml::CompositionChildRole TwowayOperation::meta_OutParameter_children;
	Uml::CompositionChildRole TwowayOperation::meta_InoutParameter_children;
	Uml::CompositionChildRole TwowayOperation::meta_ReturnType_child;
	Uml::CompositionChildRole HasExceptions::meta_ExceptionRef_children;
	Uml::CompositionChildRole Component::meta_Port_children;
	Uml::CompositionChildRole ComponentFactory::meta_LookupKey_child;
	Uml::CompositionChildRole ComponentFactory::meta_FactoryOperation_children;
	Uml::CompositionChildRole ComponentFactory::meta_LookupOperation_children;
	Uml::CompositionChildRole RootFolder::meta_ComponentTypes_children;
	Uml::CompositionChildRole RootFolder::meta_TopLevelPackages_children;
	Uml::CompositionChildRole RootFolder::meta_ComponentImplementations_children;
	Uml::CompositionChildRole RootFolder::meta_DeploymentPlans_children;
	Uml::CompositionChildRole RootFolder::meta_PackageConfigurations_children;
	Uml::CompositionChildRole RootFolder::meta_ComponentPackages_children;
	Uml::CompositionChildRole RootFolder::meta_Targets_children;
	Uml::CompositionChildRole RootFolder::meta_ImplementationArtifacts_children;
	Uml::CompositionChildRole RootFolder::meta_InterfaceDefinitions_children;
	Uml::CompositionChildRole RootFolder::meta_PredefinedTypes_children;
	Uml::CompositionChildRole RootFolder::meta_RootFolder_children;
	void Initialize()
	{
		static bool first = true;
		if(!first) return;
		first = false;
		Uml::Initialize();

		ASSERT( umldiagram == Udm::null );
		umldiagram = Uml::CreateCORBADiagram();

		package::meta = Uml::CreateCORBAClass();
		TopLevelPackageContainer::meta = Uml::CreateCORBAClass();
		TopLevelPackages::meta = Uml::CreateCORBAClass();
		TopLevelPackage::meta = Uml::CreateCORBAClass();
		ComponentInfoProperty::meta = Uml::CreateCORBAClass();
		ComponentContainer::meta = Uml::CreateCORBAClass();
		ComponentProperty::meta = Uml::CreateCORBAClass();
		CommonPortAttrs::meta = Uml::CreateCORBAClass();
		ComponentType::meta = Uml::CreateCORBAClass();
		ComponentTypes::meta = Uml::CreateCORBAClass();
		ComponentConfigProperty::meta = Uml::CreateCORBAClass();
		ComponentPropertyDescription::meta = Uml::CreateCORBAClass();
		ConfigProperty::meta = Uml::CreateCORBAClass();
		Implements::meta = Uml::CreateCORBAClass();
		ImplementationCapability::meta = Uml::CreateCORBAClass();
		ImplementationDependsOn::meta = Uml::CreateCORBAClass();
		InfoProperty::meta = Uml::CreateCORBAClass();
		ComponentImplementations::meta = Uml::CreateCORBAClass();
		MonolithprimaryArtifact::meta = Uml::CreateCORBAClass();
		ComponentImplementationReference::meta = Uml::CreateCORBAClass();
		MonolithExecParameter::meta = Uml::CreateCORBAClass();
		ComponentImplementationContainer::meta = Uml::CreateCORBAClass();
		MonolithDeployRequirement::meta = Uml::CreateCORBAClass();
		ComponentImplementation::meta = Uml::CreateCORBAClass();
		MonolithicImplementation::meta = Uml::CreateCORBAClass();
		DeploymentPlans::meta = Uml::CreateCORBAClass();
		DeploymentPlan::meta = Uml::CreateCORBAClass();
		CollocationGroup::meta = Uml::CreateCORBAClass();
		InstanceMapping::meta = Uml::CreateCORBAClass();
		PackageConfSpecializedConfig::meta = Uml::CreateCORBAClass();
		PackageConfigurationContainer::meta = Uml::CreateCORBAClass();
		PackageConfConfigProperty::meta = Uml::CreateCORBAClass();
		PackageConfBasePackage::meta = Uml::CreateCORBAClass();
		PackageConfigurationReference::meta = Uml::CreateCORBAClass();
		PackageConfigurations::meta = Uml::CreateCORBAClass();
		PackageConfSelectRequirement::meta = Uml::CreateCORBAClass();
		PackageConfReference::meta = Uml::CreateCORBAClass();
		PackageConfiguration::meta = Uml::CreateCORBAClass();
		PackageInterface::meta = Uml::CreateCORBAClass();
		PackageInfoProperty::meta = Uml::CreateCORBAClass();
		PackageConfigProperty::meta = Uml::CreateCORBAClass();
		ComponentPackages::meta = Uml::CreateCORBAClass();
		PackageContainer::meta = Uml::CreateCORBAClass();
		Implementation::meta = Uml::CreateCORBAClass();
		ComponentPackage::meta = Uml::CreateCORBAClass();
		ComponentPackageReference::meta = Uml::CreateCORBAClass();
		Resource::meta = Uml::CreateCORBAClass();
		Node::meta = Uml::CreateCORBAClass();
		SharedResource::meta = Uml::CreateCORBAClass();
		Interconnect::meta = Uml::CreateCORBAClass();
		NodeReference::meta = Uml::CreateCORBAClass();
		Bridge::meta = Uml::CreateCORBAClass();
		SatisfierProperty::meta = Uml::CreateCORBAClass();
		Requirement::meta = Uml::CreateCORBAClass();
		DataType::meta = Uml::CreateCORBAClass();
		ImplementationDependency::meta = Uml::CreateCORBAClass();
		Property::meta = Uml::CreateCORBAClass();
		Capability::meta = Uml::CreateCORBAClass();
		RequirementSatisfier::meta = Uml::CreateCORBAClass();
		ImplementationRequirement::meta = Uml::CreateCORBAClass();
		RequirementBase::meta = Uml::CreateCORBAClass();
		Elements::meta = Uml::CreateCORBAClass();
		Domain::meta = Uml::CreateCORBAClass();
		Interconnect2Node::meta = Uml::CreateCORBAClass();
		Bridge2Interconnect::meta = Uml::CreateCORBAClass();
		Shares::meta = Uml::CreateCORBAClass();
		Node2Interconnect::meta = Uml::CreateCORBAClass();
		Interconnect2Bridge::meta = Uml::CreateCORBAClass();
		Targets::meta = Uml::CreateCORBAClass();
		AssemblyDeployRequirement::meta = Uml::CreateCORBAClass();
		invoke::meta = Uml::CreateCORBAClass();
		ComponentAssembly::meta = Uml::CreateCORBAClass();
		AssemblyselectRequirement::meta = Uml::CreateCORBAClass();
		emit::meta = Uml::CreateCORBAClass();
		PublishConnector::meta = Uml::CreateCORBAClass();
		deliverTo::meta = Uml::CreateCORBAClass();
		publish::meta = Uml::CreateCORBAClass();
		AssemblyConfigProperty::meta = Uml::CreateCORBAClass();
		ArtifactExecParameter::meta = Uml::CreateCORBAClass();
		ImplementationArtifactReference::meta = Uml::CreateCORBAClass();
		ArtifactDependsOn::meta = Uml::CreateCORBAClass();
		ImplementationArtifact::meta = Uml::CreateCORBAClass();
		ImplementationArtifacts::meta = Uml::CreateCORBAClass();
		ArtifactDeployRequirement::meta = Uml::CreateCORBAClass();
		ArtifactInfoProperty::meta = Uml::CreateCORBAClass();
		ArtifactContainer::meta = Uml::CreateCORBAClass();
		Package::meta = Uml::CreateCORBAClass();
		File::meta = Uml::CreateCORBAClass();
		Exception::meta = Uml::CreateCORBAClass();
		ExceptionRef::meta = Uml::CreateCORBAClass();
		FileRef::meta = Uml::CreateCORBAClass();
		InterfaceDefinitions::meta = Uml::CreateCORBAClass();
		ConstantType::meta = Uml::CreateCORBAClass();
		Constant::meta = Uml::CreateCORBAClass();
		PredefinedTypes::meta = Uml::CreateCORBAClass();
		PredefinedType::meta = Uml::CreateCORBAClass();
		Byte::meta = Uml::CreateCORBAClass();
		Boolean::meta = Uml::CreateCORBAClass();
		ShortInteger::meta = Uml::CreateCORBAClass();
		LongInteger::meta = Uml::CreateCORBAClass();
		RealNumber::meta = Uml::CreateCORBAClass();
		String::meta = Uml::CreateCORBAClass();
		GenericObject::meta = Uml::CreateCORBAClass();
		GenericValueObject::meta = Uml::CreateCORBAClass();
		GenericValue::meta = Uml::CreateCORBAClass();
		TypeEncoding::meta = Uml::CreateCORBAClass();
		TypeKind::meta = Uml::CreateCORBAClass();
		Enum::meta = Uml::CreateCORBAClass();
		EnumValue::meta = Uml::CreateCORBAClass();
		Aggregate::meta = Uml::CreateCORBAClass();
		SwitchedAggregate::meta = Uml::CreateCORBAClass();
		Label::meta = Uml::CreateCORBAClass();
		LabelConnection::meta = Uml::CreateCORBAClass();
		MemberType::meta = Uml::CreateCORBAClass();
		Discriminator::meta = Uml::CreateCORBAClass();
		NamedType::meta = Uml::CreateCORBAClass();
		Collection::meta = Uml::CreateCORBAClass();
		Alias::meta = Uml::CreateCORBAClass();
		Boxed::meta = Uml::CreateCORBAClass();
		Member::meta = Uml::CreateCORBAClass();
		NoInheritable::meta = Uml::CreateCORBAClass();
		Object::meta = Uml::CreateCORBAClass();
		Inherits::meta = Uml::CreateCORBAClass();
		Event::meta = Uml::CreateCORBAClass();
		ValueObject::meta = Uml::CreateCORBAClass();
		Inheritable::meta = Uml::CreateCORBAClass();
		HasOperations::meta = Uml::CreateCORBAClass();
		SupportsInterfaces::meta = Uml::CreateCORBAClass();
		ReadonlyAttribute::meta = Uml::CreateCORBAClass();
		LookupKey::meta = Uml::CreateCORBAClass();
		SetException::meta = Uml::CreateCORBAClass();
		ObjectByValue::meta = Uml::CreateCORBAClass();
		GetException::meta = Uml::CreateCORBAClass();
		PrivateFlag::meta = Uml::CreateCORBAClass();
		MakeMemberPrivate::meta = Uml::CreateCORBAClass();
		AttributeMember::meta = Uml::CreateCORBAClass();
		Supports::meta = Uml::CreateCORBAClass();
		Attribute::meta = Uml::CreateCORBAClass();
		OperationBase::meta = Uml::CreateCORBAClass();
		FactoryOperation::meta = Uml::CreateCORBAClass();
		LookupOperation::meta = Uml::CreateCORBAClass();
		TwowayOperation::meta = Uml::CreateCORBAClass();
		OnewayOperation::meta = Uml::CreateCORBAClass();
		HasExceptions::meta = Uml::CreateCORBAClass();
		InParameter::meta = Uml::CreateCORBAClass();
		OutParameter::meta = Uml::CreateCORBAClass();
		ReturnType::meta = Uml::CreateCORBAClass();
		InoutParameter::meta = Uml::CreateCORBAClass();
		Component::meta = Uml::CreateCORBAClass();
		OutEventPort::meta = Uml::CreateCORBAClass();
		ProvidedRequestPort::meta = Uml::CreateCORBAClass();
		RequiredRequestPort::meta = Uml::CreateCORBAClass();
		InEventPort::meta = Uml::CreateCORBAClass();
		Port::meta = Uml::CreateCORBAClass();
		ComponentRef::meta = Uml::CreateCORBAClass();
		Manageable::meta = Uml::CreateCORBAClass();
		ComponentFactory::meta = Uml::CreateCORBAClass();
		ManagesComponent::meta = Uml::CreateCORBAClass();
		Provideable::meta = Uml::CreateCORBAClass();
		Prefixable::meta = Uml::CreateCORBAClass();
		Taggable::meta = Uml::CreateCORBAClass();
		RootFolder::meta = Uml::CreateCORBAClass();
		MgaObject::meta = Uml::CreateCORBAClass();
		TopLevelPackages::meta_name = Uml::CreateCORBAAttribute();
		CommonPortAttrs::meta_exclusiveUser = Uml::CreateCORBAAttribute();
		CommonPortAttrs::meta_optional = Uml::CreateCORBAAttribute();
		CommonPortAttrs::meta_exclusiveProvider = Uml::CreateCORBAAttribute();
		ComponentTypes::meta_name = Uml::CreateCORBAAttribute();
		ComponentImplementations::meta_name = Uml::CreateCORBAAttribute();
		ComponentImplementation::meta_UUID = Uml::CreateCORBAAttribute();
		ComponentImplementation::meta_label = Uml::CreateCORBAAttribute();
		DeploymentPlans::meta_name = Uml::CreateCORBAAttribute();
		DeploymentPlan::meta_label = Uml::CreateCORBAAttribute();
		PackageConfigurations::meta_name = Uml::CreateCORBAAttribute();
		PackageConfiguration::meta_UUID = Uml::CreateCORBAAttribute();
		PackageConfiguration::meta_label = Uml::CreateCORBAAttribute();
		ComponentPackages::meta_name = Uml::CreateCORBAAttribute();
		ComponentPackage::meta_UUID = Uml::CreateCORBAAttribute();
		ComponentPackage::meta_label = Uml::CreateCORBAAttribute();
		ComponentPackageReference::meta_requiredName = Uml::CreateCORBAAttribute();
		ComponentPackageReference::meta_requiredUUID = Uml::CreateCORBAAttribute();
		ComponentPackageReference::meta_requiredType = Uml::CreateCORBAAttribute();
		Resource::meta_resourceType = Uml::CreateCORBAAttribute();
		Node::meta_label = Uml::CreateCORBAAttribute();
		SharedResource::meta_resourceType = Uml::CreateCORBAAttribute();
		Interconnect::meta_label = Uml::CreateCORBAAttribute();
		Bridge::meta_label = Uml::CreateCORBAAttribute();
		SatisfierProperty::meta_DataValue = Uml::CreateCORBAAttribute();
		SatisfierProperty::meta_SatisfierPropertyKind = Uml::CreateCORBAAttribute();
		ImplementationDependency::meta_requiredType = Uml::CreateCORBAAttribute();
		Property::meta_DataValue = Uml::CreateCORBAAttribute();
		RequirementSatisfier::meta_resourceType = Uml::CreateCORBAAttribute();
		ImplementationRequirement::meta_ResourceUsageKind = Uml::CreateCORBAAttribute();
		ImplementationRequirement::meta_componentPort = Uml::CreateCORBAAttribute();
		ImplementationRequirement::meta_resourcePort = Uml::CreateCORBAAttribute();
		RequirementBase::meta_resourceType = Uml::CreateCORBAAttribute();
		Domain::meta_label = Uml::CreateCORBAAttribute();
		Domain::meta_UUID = Uml::CreateCORBAAttribute();
		Targets::meta_name = Uml::CreateCORBAAttribute();
		ImplementationArtifact::meta_UUID = Uml::CreateCORBAAttribute();
		ImplementationArtifact::meta_label = Uml::CreateCORBAAttribute();
		ImplementationArtifact::meta_location = Uml::CreateCORBAAttribute();
		ImplementationArtifacts::meta_name = Uml::CreateCORBAAttribute();
		InterfaceDefinitions::meta_name = Uml::CreateCORBAAttribute();
		Constant::meta_value = Uml::CreateCORBAAttribute();
		PredefinedTypes::meta_name = Uml::CreateCORBAAttribute();
		Object::meta_abstract = Uml::CreateCORBAAttribute();
		Object::meta_local = Uml::CreateCORBAAttribute();
		ObjectByValue::meta_abstract = Uml::CreateCORBAAttribute();
		Component::meta_UUID = Uml::CreateCORBAAttribute();
		Component::meta_label = Uml::CreateCORBAAttribute();
		OutEventPort::meta_single_destination = Uml::CreateCORBAAttribute();
		RequiredRequestPort::meta_multiple_connections = Uml::CreateCORBAAttribute();
		Prefixable::meta_PrefixTag = Uml::CreateCORBAAttribute();
		Taggable::meta_VersionTag = Uml::CreateCORBAAttribute();
		Taggable::meta_SpecifyIdTag = Uml::CreateCORBAAttribute();
		RootFolder::meta_name = Uml::CreateCORBAAttribute();
		MgaObject::meta_position = Uml::CreateCORBAAttribute();
		MgaObject::meta_name = Uml::CreateCORBAAttribute();
		TopLevelPackage::meta_dstpackage = Uml::CreateCORBAAssociationRole();
		ComponentType::meta_dstComponentProperty = Uml::CreateCORBAAssociationRole();
		ComponentType::meta_dstComponentConfigProperty = Uml::CreateCORBAAssociationRole();
		ComponentType::meta_dstComponentInfoProperty = Uml::CreateCORBAAssociationRole();
		ComponentType::meta_ref = Uml::CreateCORBAAssociationRole();
		ComponentType::meta_CollocationGroup = Uml::CreateCORBAAssociationRole();
		ComponentType::meta_srcImplements = Uml::CreateCORBAAssociationRole();
		ComponentType::meta_srcPackageInterface = Uml::CreateCORBAAssociationRole();
		ComponentPropertyDescription::meta_srcComponentProperty = Uml::CreateCORBAAssociationRole();
		ComponentImplementationReference::meta_srcImplementation = Uml::CreateCORBAAssociationRole();
		ComponentImplementationReference::meta_ref = Uml::CreateCORBAAssociationRole();
		ComponentImplementation::meta_dstImplements = Uml::CreateCORBAAssociationRole();
		ComponentImplementation::meta_referedbyComponentImplementationReference = Uml::CreateCORBAAssociationRole();
		ComponentImplementation::meta_dstInfoProperty = Uml::CreateCORBAAssociationRole();
		ComponentImplementation::meta_dstImplementationCapability = Uml::CreateCORBAAssociationRole();
		ComponentImplementation::meta_dstConfigProperty = Uml::CreateCORBAAssociationRole();
		ComponentImplementation::meta_dstImplementationDependsOn = Uml::CreateCORBAAssociationRole();
		MonolithicImplementation::meta_dstMonolithExecParameter = Uml::CreateCORBAAssociationRole();
		MonolithicImplementation::meta_dstMonolithDeployRequirement = Uml::CreateCORBAAssociationRole();
		MonolithicImplementation::meta_dstMonolithprimaryArtifact = Uml::CreateCORBAAssociationRole();
		CollocationGroup::meta_members = Uml::CreateCORBAAssociationRole();
		CollocationGroup::meta_dstInstanceMapping = Uml::CreateCORBAAssociationRole();
		PackageConfigurationReference::meta_srcpackage = Uml::CreateCORBAAssociationRole();
		PackageConfigurationReference::meta_ref = Uml::CreateCORBAAssociationRole();
		PackageConfigurationReference::meta_srcPackageConfSpecializedConfig = Uml::CreateCORBAAssociationRole();
		PackageConfiguration::meta_referedbyPackageConfigurationReference = Uml::CreateCORBAAssociationRole();
		PackageConfiguration::meta_dstPackageConfSpecializedConfig = Uml::CreateCORBAAssociationRole();
		PackageConfiguration::meta_dstPackageConfReference = Uml::CreateCORBAAssociationRole();
		PackageConfiguration::meta_dstPackageConfBasePackage = Uml::CreateCORBAAssociationRole();
		PackageConfiguration::meta_dstPackageConfConfigProperty = Uml::CreateCORBAAssociationRole();
		PackageConfiguration::meta_dstPackageConfSelectRequirement = Uml::CreateCORBAAssociationRole();
		ComponentPackage::meta_dstPackageInterface = Uml::CreateCORBAAssociationRole();
		ComponentPackage::meta_dstImplementation = Uml::CreateCORBAAssociationRole();
		ComponentPackage::meta_srcPackageConfBasePackage = Uml::CreateCORBAAssociationRole();
		ComponentPackage::meta_dstPackageInfoProperty = Uml::CreateCORBAAssociationRole();
		ComponentPackage::meta_dstPackageConfigProperty = Uml::CreateCORBAAssociationRole();
		ComponentPackage::meta_referedbyComponentPackageReference = Uml::CreateCORBAAssociationRole();
		ComponentPackageReference::meta_srcPackageConfReference = Uml::CreateCORBAAssociationRole();
		ComponentPackageReference::meta_ref = Uml::CreateCORBAAssociationRole();
		Node::meta_dstNode2Interconnect = Uml::CreateCORBAAssociationRole();
		Node::meta_dstShares = Uml::CreateCORBAAssociationRole();
		Node::meta_srcInterconnect2Node = Uml::CreateCORBAAssociationRole();
		Node::meta_referedbyNodeReference = Uml::CreateCORBAAssociationRole();
		SharedResource::meta_srcShares = Uml::CreateCORBAAssociationRole();
		Interconnect::meta_srcNode2Interconnect = Uml::CreateCORBAAssociationRole();
		Interconnect::meta_dstInterconnect2Node = Uml::CreateCORBAAssociationRole();
		Interconnect::meta_dstInterconnect2Bridge = Uml::CreateCORBAAssociationRole();
		Interconnect::meta_srcBridge2Interconnect = Uml::CreateCORBAAssociationRole();
		NodeReference::meta_srcInstanceMapping = Uml::CreateCORBAAssociationRole();
		NodeReference::meta_ref = Uml::CreateCORBAAssociationRole();
		Bridge::meta_srcInterconnect2Bridge = Uml::CreateCORBAAssociationRole();
		Bridge::meta_dstBridge2Interconnect = Uml::CreateCORBAAssociationRole();
		Requirement::meta_srcPackageConfSelectRequirement = Uml::CreateCORBAAssociationRole();
		Requirement::meta_srcAssemblyDeployRequirement = Uml::CreateCORBAAssociationRole();
		Requirement::meta_srcArtifactDeployRequirement = Uml::CreateCORBAAssociationRole();
		Requirement::meta_srcAssemblyselectRequirement = Uml::CreateCORBAAssociationRole();
		DataType::meta_ref = Uml::CreateCORBAAssociationRole();
		ImplementationDependency::meta_srcImplementationDependsOn = Uml::CreateCORBAAssociationRole();
		Property::meta_srcComponentConfigProperty = Uml::CreateCORBAAssociationRole();
		Property::meta_srcComponentInfoProperty = Uml::CreateCORBAAssociationRole();
		Property::meta_srcInfoProperty = Uml::CreateCORBAAssociationRole();
		Property::meta_srcConfigProperty = Uml::CreateCORBAAssociationRole();
		Property::meta_srcMonolithExecParameter = Uml::CreateCORBAAssociationRole();
		Property::meta_srcPackageConfConfigProperty = Uml::CreateCORBAAssociationRole();
		Property::meta_srcPackageInfoProperty = Uml::CreateCORBAAssociationRole();
		Property::meta_srcPackageConfigProperty = Uml::CreateCORBAAssociationRole();
		Property::meta_srcArtifactExecParameter = Uml::CreateCORBAAssociationRole();
		Property::meta_srcArtifactInfoProperty = Uml::CreateCORBAAssociationRole();
		Property::meta_srcAssemblyConfigProperty = Uml::CreateCORBAAssociationRole();
		Capability::meta_srcImplementationCapability = Uml::CreateCORBAAssociationRole();
		ImplementationRequirement::meta_srcMonolithDeployRequirement = Uml::CreateCORBAAssociationRole();
		PublishConnector::meta_dstAssemblyDeployRequirement = Uml::CreateCORBAAssociationRole();
		PublishConnector::meta_dstdeliverTo = Uml::CreateCORBAAssociationRole();
		PublishConnector::meta_srcpublish = Uml::CreateCORBAAssociationRole();
		ImplementationArtifactReference::meta_srcMonolithprimaryArtifact = Uml::CreateCORBAAssociationRole();
		ImplementationArtifactReference::meta_ref = Uml::CreateCORBAAssociationRole();
		ImplementationArtifactReference::meta_srcArtifactDependsOn = Uml::CreateCORBAAssociationRole();
		ImplementationArtifact::meta_dstArtifactDeployRequirement = Uml::CreateCORBAAssociationRole();
		ImplementationArtifact::meta_dstArtifactExecParameter = Uml::CreateCORBAAssociationRole();
		ImplementationArtifact::meta_dstArtifactInfoProperty = Uml::CreateCORBAAssociationRole();
		ImplementationArtifact::meta_referedbyImplementationArtifactReference = Uml::CreateCORBAAssociationRole();
		ImplementationArtifact::meta_dstArtifactDependsOn = Uml::CreateCORBAAssociationRole();
		File::meta_referedbyFileRef = Uml::CreateCORBAAssociationRole();
		Exception::meta_referedbySetException = Uml::CreateCORBAAssociationRole();
		Exception::meta_referedbyExceptionRef = Uml::CreateCORBAAssociationRole();
		Exception::meta_referedbyGetException = Uml::CreateCORBAAssociationRole();
		ExceptionRef::meta_ref = Uml::CreateCORBAAssociationRole();
		FileRef::meta_ref = Uml::CreateCORBAAssociationRole();
		ConstantType::meta_referedbyDiscriminator = Uml::CreateCORBAAssociationRole();
		ConstantType::meta_referedbyConstant = Uml::CreateCORBAAssociationRole();
		Constant::meta_ref = Uml::CreateCORBAAssociationRole();
		PredefinedType::meta_referedbyDataType = Uml::CreateCORBAAssociationRole();
		Label::meta_srcLabelConnection = Uml::CreateCORBAAssociationRole();
		MemberType::meta_referedbyAlias = Uml::CreateCORBAAssociationRole();
		MemberType::meta_referedbyCollection = Uml::CreateCORBAAssociationRole();
		MemberType::meta_referedbyBoxed = Uml::CreateCORBAAssociationRole();
		MemberType::meta_referedbyMember = Uml::CreateCORBAAssociationRole();
		MemberType::meta_referedbyAttributeMember = Uml::CreateCORBAAssociationRole();
		MemberType::meta_referedbyInParameter = Uml::CreateCORBAAssociationRole();
		MemberType::meta_referedbyOutParameter = Uml::CreateCORBAAssociationRole();
		MemberType::meta_referedbyReturnType = Uml::CreateCORBAAssociationRole();
		MemberType::meta_referedbyInoutParameter = Uml::CreateCORBAAssociationRole();
		Discriminator::meta_ref = Uml::CreateCORBAAssociationRole();
		Collection::meta_ref = Uml::CreateCORBAAssociationRole();
		Alias::meta_ref = Uml::CreateCORBAAssociationRole();
		Boxed::meta_ref = Uml::CreateCORBAAssociationRole();
		Member::meta_dstLabelConnection = Uml::CreateCORBAAssociationRole();
		Member::meta_ref = Uml::CreateCORBAAssociationRole();
		Member::meta_dstMakeMemberPrivate = Uml::CreateCORBAAssociationRole();
		Object::meta_referedbySupports = Uml::CreateCORBAAssociationRole();
		Inherits::meta_ref = Uml::CreateCORBAAssociationRole();
		Event::meta_referedbyInEventPort = Uml::CreateCORBAAssociationRole();
		Event::meta_referedbyOutEventPort = Uml::CreateCORBAAssociationRole();
		ValueObject::meta_referedbyLookupKey = Uml::CreateCORBAAssociationRole();
		Inheritable::meta_referedbyInherits = Uml::CreateCORBAAssociationRole();
		LookupKey::meta_ref = Uml::CreateCORBAAssociationRole();
		SetException::meta_ref = Uml::CreateCORBAAssociationRole();
		GetException::meta_ref = Uml::CreateCORBAAssociationRole();
		PrivateFlag::meta_srcMakeMemberPrivate = Uml::CreateCORBAAssociationRole();
		AttributeMember::meta_ref = Uml::CreateCORBAAssociationRole();
		Supports::meta_ref = Uml::CreateCORBAAssociationRole();
		InParameter::meta_ref = Uml::CreateCORBAAssociationRole();
		OutParameter::meta_ref = Uml::CreateCORBAAssociationRole();
		ReturnType::meta_ref = Uml::CreateCORBAAssociationRole();
		InoutParameter::meta_ref = Uml::CreateCORBAAssociationRole();
		Component::meta_referedbyComponentType = Uml::CreateCORBAAssociationRole();
		Component::meta_dstAssemblyselectRequirement = Uml::CreateCORBAAssociationRole();
		Component::meta_dstAssemblyConfigProperty = Uml::CreateCORBAAssociationRole();
		Component::meta_referedbyComponentRef = Uml::CreateCORBAAssociationRole();
		OutEventPort::meta_dstpublish = Uml::CreateCORBAAssociationRole();
		OutEventPort::meta_ref = Uml::CreateCORBAAssociationRole();
		OutEventPort::meta_dstemit = Uml::CreateCORBAAssociationRole();
		ProvidedRequestPort::meta_srcinvoke = Uml::CreateCORBAAssociationRole();
		ProvidedRequestPort::meta_ref = Uml::CreateCORBAAssociationRole();
		RequiredRequestPort::meta_dstinvoke = Uml::CreateCORBAAssociationRole();
		RequiredRequestPort::meta_ref = Uml::CreateCORBAAssociationRole();
		InEventPort::meta_srcdeliverTo = Uml::CreateCORBAAssociationRole();
		InEventPort::meta_ref = Uml::CreateCORBAAssociationRole();
		InEventPort::meta_srcemit = Uml::CreateCORBAAssociationRole();
		ComponentRef::meta_ref = Uml::CreateCORBAAssociationRole();
		Manageable::meta_srcManagesComponent = Uml::CreateCORBAAssociationRole();
		ComponentFactory::meta_dstManagesComponent = Uml::CreateCORBAAssociationRole();
		Provideable::meta_referedbyProvidedRequestPort = Uml::CreateCORBAAssociationRole();
		Provideable::meta_referedbyRequiredRequestPort = Uml::CreateCORBAAssociationRole();
		package::meta_TopLevelPackageContainer_parent = Uml::CreateCORBACompositionParentRole();
		TopLevelPackageContainer::meta_TopLevelPackages_parent = Uml::CreateCORBACompositionParentRole();
		TopLevelPackages::meta_RootFolder_parent = Uml::CreateCORBACompositionParentRole();
		TopLevelPackage::meta_TopLevelPackageContainer_parent = Uml::CreateCORBACompositionParentRole();
		ComponentInfoProperty::meta_ComponentContainer_parent = Uml::CreateCORBACompositionParentRole();
		ComponentContainer::meta_ComponentTypes_parent = Uml::CreateCORBACompositionParentRole();
		ComponentProperty::meta_ComponentContainer_parent = Uml::CreateCORBACompositionParentRole();
		ComponentType::meta_ComponentContainer_parent = Uml::CreateCORBACompositionParentRole();
		ComponentType::meta_ComponentImplementationContainer_parent = Uml::CreateCORBACompositionParentRole();
		ComponentType::meta_DeploymentPlan_parent = Uml::CreateCORBACompositionParentRole();
		ComponentType::meta_PackageContainer_parent = Uml::CreateCORBACompositionParentRole();
		ComponentTypes::meta_RootFolder_parent = Uml::CreateCORBACompositionParentRole();
		ComponentConfigProperty::meta_ComponentContainer_parent = Uml::CreateCORBACompositionParentRole();
		ComponentPropertyDescription::meta_ComponentContainer_parent = Uml::CreateCORBACompositionParentRole();
		ConfigProperty::meta_ComponentImplementationContainer_parent = Uml::CreateCORBACompositionParentRole();
		Implements::meta_ComponentImplementationContainer_parent = Uml::CreateCORBACompositionParentRole();
		ImplementationCapability::meta_ComponentImplementationContainer_parent = Uml::CreateCORBACompositionParentRole();
		ImplementationDependsOn::meta_ComponentImplementationContainer_parent = Uml::CreateCORBACompositionParentRole();
		InfoProperty::meta_ComponentImplementationContainer_parent = Uml::CreateCORBACompositionParentRole();
		ComponentImplementations::meta_RootFolder_parent = Uml::CreateCORBACompositionParentRole();
		MonolithprimaryArtifact::meta_ComponentImplementationContainer_parent = Uml::CreateCORBACompositionParentRole();
		ComponentImplementationReference::meta_PackageContainer_parent = Uml::CreateCORBACompositionParentRole();
		MonolithExecParameter::meta_ComponentImplementationContainer_parent = Uml::CreateCORBACompositionParentRole();
		ComponentImplementationContainer::meta_ComponentImplementations_parent = Uml::CreateCORBACompositionParentRole();
		MonolithDeployRequirement::meta_ComponentImplementationContainer_parent = Uml::CreateCORBACompositionParentRole();
		ComponentImplementation::meta_ComponentImplementationContainer_parent = Uml::CreateCORBACompositionParentRole();
		DeploymentPlans::meta_RootFolder_parent = Uml::CreateCORBACompositionParentRole();
		DeploymentPlan::meta_DeploymentPlans_parent = Uml::CreateCORBACompositionParentRole();
		CollocationGroup::meta_DeploymentPlan_parent = Uml::CreateCORBACompositionParentRole();
		InstanceMapping::meta_DeploymentPlan_parent = Uml::CreateCORBACompositionParentRole();
		PackageConfSpecializedConfig::meta_PackageConfigurationContainer_parent = Uml::CreateCORBACompositionParentRole();
		PackageConfigurationContainer::meta_PackageConfigurations_parent = Uml::CreateCORBACompositionParentRole();
		PackageConfConfigProperty::meta_PackageConfigurationContainer_parent = Uml::CreateCORBACompositionParentRole();
		PackageConfBasePackage::meta_PackageConfigurationContainer_parent = Uml::CreateCORBACompositionParentRole();
		PackageConfigurationReference::meta_TopLevelPackageContainer_parent = Uml::CreateCORBACompositionParentRole();
		PackageConfigurations::meta_RootFolder_parent = Uml::CreateCORBACompositionParentRole();
		PackageConfSelectRequirement::meta_PackageConfigurationContainer_parent = Uml::CreateCORBACompositionParentRole();
		PackageConfReference::meta_PackageConfigurationContainer_parent = Uml::CreateCORBACompositionParentRole();
		PackageConfiguration::meta_PackageConfigurationContainer_parent = Uml::CreateCORBACompositionParentRole();
		PackageInterface::meta_PackageContainer_parent = Uml::CreateCORBACompositionParentRole();
		PackageInfoProperty::meta_PackageContainer_parent = Uml::CreateCORBACompositionParentRole();
		PackageConfigProperty::meta_PackageContainer_parent = Uml::CreateCORBACompositionParentRole();
		ComponentPackages::meta_RootFolder_parent = Uml::CreateCORBACompositionParentRole();
		PackageContainer::meta_ComponentPackages_parent = Uml::CreateCORBACompositionParentRole();
		Implementation::meta_PackageContainer_parent = Uml::CreateCORBACompositionParentRole();
		ComponentPackage::meta_PackageConfigurationContainer_parent = Uml::CreateCORBACompositionParentRole();
		ComponentPackage::meta_PackageContainer_parent = Uml::CreateCORBACompositionParentRole();
		ComponentPackage::meta_ComponentAssembly_parent = Uml::CreateCORBACompositionParentRole();
		ComponentPackageReference::meta_PackageConfigurationContainer_parent = Uml::CreateCORBACompositionParentRole();
		ComponentPackageReference::meta_ComponentAssembly_parent = Uml::CreateCORBACompositionParentRole();
		Resource::meta_Interconnect_parent = Uml::CreateCORBACompositionParentRole();
		Resource::meta_Node_parent = Uml::CreateCORBACompositionParentRole();
		Resource::meta_Bridge_parent = Uml::CreateCORBACompositionParentRole();
		NodeReference::meta_DeploymentPlan_parent = Uml::CreateCORBACompositionParentRole();
		SatisfierProperty::meta_Resource_parent = Uml::CreateCORBACompositionParentRole();
		SatisfierProperty::meta_SharedResource_parent = Uml::CreateCORBACompositionParentRole();
		SatisfierProperty::meta_RequirementSatisfier_parent = Uml::CreateCORBACompositionParentRole();
		Requirement::meta_PackageConfigurationContainer_parent = Uml::CreateCORBACompositionParentRole();
		Requirement::meta_ComponentAssembly_parent = Uml::CreateCORBACompositionParentRole();
		Requirement::meta_ArtifactContainer_parent = Uml::CreateCORBACompositionParentRole();
		DataType::meta_ComponentPropertyDescription_parent = Uml::CreateCORBACompositionParentRole();
		DataType::meta_SatisfierProperty_parent = Uml::CreateCORBACompositionParentRole();
		DataType::meta_Property_parent = Uml::CreateCORBACompositionParentRole();
		ImplementationDependency::meta_ComponentImplementationContainer_parent = Uml::CreateCORBACompositionParentRole();
		Property::meta_ComponentContainer_parent = Uml::CreateCORBACompositionParentRole();
		Property::meta_ComponentImplementationContainer_parent = Uml::CreateCORBACompositionParentRole();
		Property::meta_PackageConfigurationContainer_parent = Uml::CreateCORBACompositionParentRole();
		Property::meta_PackageContainer_parent = Uml::CreateCORBACompositionParentRole();
		Property::meta_RequirementBase_parent = Uml::CreateCORBACompositionParentRole();
		Property::meta_Domain_parent = Uml::CreateCORBACompositionParentRole();
		Property::meta_ComponentAssembly_parent = Uml::CreateCORBACompositionParentRole();
		Property::meta_ArtifactContainer_parent = Uml::CreateCORBACompositionParentRole();
		Capability::meta_ComponentImplementationContainer_parent = Uml::CreateCORBACompositionParentRole();
		ImplementationRequirement::meta_ComponentImplementationContainer_parent = Uml::CreateCORBACompositionParentRole();
		Elements::meta_Domain_parent = Uml::CreateCORBACompositionParentRole();
		Domain::meta_Targets_parent = Uml::CreateCORBACompositionParentRole();
		Interconnect2Node::meta_Domain_parent = Uml::CreateCORBACompositionParentRole();
		Bridge2Interconnect::meta_Domain_parent = Uml::CreateCORBACompositionParentRole();
		Shares::meta_Domain_parent = Uml::CreateCORBACompositionParentRole();
		Node2Interconnect::meta_Domain_parent = Uml::CreateCORBACompositionParentRole();
		Interconnect2Bridge::meta_Domain_parent = Uml::CreateCORBACompositionParentRole();
		Targets::meta_RootFolder_parent = Uml::CreateCORBACompositionParentRole();
		AssemblyDeployRequirement::meta_ComponentAssembly_parent = Uml::CreateCORBACompositionParentRole();
		invoke::meta_ComponentAssembly_parent = Uml::CreateCORBACompositionParentRole();
		AssemblyselectRequirement::meta_ComponentAssembly_parent = Uml::CreateCORBACompositionParentRole();
		emit::meta_ComponentAssembly_parent = Uml::CreateCORBACompositionParentRole();
		PublishConnector::meta_ComponentAssembly_parent = Uml::CreateCORBACompositionParentRole();
		deliverTo::meta_ComponentAssembly_parent = Uml::CreateCORBACompositionParentRole();
		publish::meta_ComponentAssembly_parent = Uml::CreateCORBACompositionParentRole();
		AssemblyConfigProperty::meta_ComponentAssembly_parent = Uml::CreateCORBACompositionParentRole();
		ArtifactExecParameter::meta_ArtifactContainer_parent = Uml::CreateCORBACompositionParentRole();
		ImplementationArtifactReference::meta_ComponentImplementationContainer_parent = Uml::CreateCORBACompositionParentRole();
		ImplementationArtifactReference::meta_ArtifactContainer_parent = Uml::CreateCORBACompositionParentRole();
		ArtifactDependsOn::meta_ArtifactContainer_parent = Uml::CreateCORBACompositionParentRole();
		ImplementationArtifact::meta_ArtifactContainer_parent = Uml::CreateCORBACompositionParentRole();
		ImplementationArtifacts::meta_RootFolder_parent = Uml::CreateCORBACompositionParentRole();
		ArtifactDeployRequirement::meta_ArtifactContainer_parent = Uml::CreateCORBACompositionParentRole();
		ArtifactInfoProperty::meta_ArtifactContainer_parent = Uml::CreateCORBACompositionParentRole();
		ArtifactContainer::meta_ImplementationArtifacts_parent = Uml::CreateCORBACompositionParentRole();
		Package::meta_Package_parent = Uml::CreateCORBACompositionParentRole();
		Package::meta_File_parent = Uml::CreateCORBACompositionParentRole();
		File::meta_InterfaceDefinitions_parent = Uml::CreateCORBACompositionParentRole();
		Exception::meta_Package_parent = Uml::CreateCORBACompositionParentRole();
		Exception::meta_File_parent = Uml::CreateCORBACompositionParentRole();
		Exception::meta_HasOperations_parent = Uml::CreateCORBACompositionParentRole();
		ExceptionRef::meta_HasExceptions_parent = Uml::CreateCORBACompositionParentRole();
		FileRef::meta_File_parent = Uml::CreateCORBACompositionParentRole();
		InterfaceDefinitions::meta_RootFolder_parent = Uml::CreateCORBACompositionParentRole();
		Constant::meta_Package_parent = Uml::CreateCORBACompositionParentRole();
		Constant::meta_File_parent = Uml::CreateCORBACompositionParentRole();
		Constant::meta_HasOperations_parent = Uml::CreateCORBACompositionParentRole();
		PredefinedTypes::meta_RootFolder_parent = Uml::CreateCORBACompositionParentRole();
		PredefinedType::meta_PredefinedTypes_parent = Uml::CreateCORBACompositionParentRole();
		EnumValue::meta_Enum_parent = Uml::CreateCORBACompositionParentRole();
		Label::meta_SwitchedAggregate_parent = Uml::CreateCORBACompositionParentRole();
		LabelConnection::meta_SwitchedAggregate_parent = Uml::CreateCORBACompositionParentRole();
		Discriminator::meta_SwitchedAggregate_parent = Uml::CreateCORBACompositionParentRole();
		NamedType::meta_Package_parent = Uml::CreateCORBACompositionParentRole();
		NamedType::meta_File_parent = Uml::CreateCORBACompositionParentRole();
		Member::meta_Exception_parent = Uml::CreateCORBACompositionParentRole();
		Member::meta_Aggregate_parent = Uml::CreateCORBACompositionParentRole();
		Member::meta_SwitchedAggregate_parent = Uml::CreateCORBACompositionParentRole();
		Member::meta_ObjectByValue_parent = Uml::CreateCORBACompositionParentRole();
		NoInheritable::meta_HasOperations_parent = Uml::CreateCORBACompositionParentRole();
		Inherits::meta_Inheritable_parent = Uml::CreateCORBACompositionParentRole();
		ReadonlyAttribute::meta_Inheritable_parent = Uml::CreateCORBACompositionParentRole();
		LookupKey::meta_ComponentFactory_parent = Uml::CreateCORBACompositionParentRole();
		SetException::meta_Attribute_parent = Uml::CreateCORBACompositionParentRole();
		GetException::meta_ReadonlyAttribute_parent = Uml::CreateCORBACompositionParentRole();
		PrivateFlag::meta_ObjectByValue_parent = Uml::CreateCORBACompositionParentRole();
		MakeMemberPrivate::meta_ObjectByValue_parent = Uml::CreateCORBACompositionParentRole();
		AttributeMember::meta_ReadonlyAttribute_parent = Uml::CreateCORBACompositionParentRole();
		Supports::meta_SupportsInterfaces_parent = Uml::CreateCORBACompositionParentRole();
		FactoryOperation::meta_ObjectByValue_parent = Uml::CreateCORBACompositionParentRole();
		FactoryOperation::meta_ComponentFactory_parent = Uml::CreateCORBACompositionParentRole();
		LookupOperation::meta_ComponentFactory_parent = Uml::CreateCORBACompositionParentRole();
		TwowayOperation::meta_HasOperations_parent = Uml::CreateCORBACompositionParentRole();
		OnewayOperation::meta_HasOperations_parent = Uml::CreateCORBACompositionParentRole();
		InParameter::meta_OperationBase_parent = Uml::CreateCORBACompositionParentRole();
		OutParameter::meta_TwowayOperation_parent = Uml::CreateCORBACompositionParentRole();
		ReturnType::meta_TwowayOperation_parent = Uml::CreateCORBACompositionParentRole();
		InoutParameter::meta_TwowayOperation_parent = Uml::CreateCORBACompositionParentRole();
		Component::meta_ComponentAssembly_parent = Uml::CreateCORBACompositionParentRole();
		Port::meta_Component_parent = Uml::CreateCORBACompositionParentRole();
		ComponentRef::meta_Package_parent = Uml::CreateCORBACompositionParentRole();
		ComponentRef::meta_File_parent = Uml::CreateCORBACompositionParentRole();
		ManagesComponent::meta_Package_parent = Uml::CreateCORBACompositionParentRole();
		ManagesComponent::meta_File_parent = Uml::CreateCORBACompositionParentRole();
		RootFolder::meta_RootFolder_parent = Uml::CreateCORBACompositionParentRole();
		TopLevelPackageContainer::meta_TopLevelPackage_child = Uml::CreateCORBACompositionChildRole();
		TopLevelPackageContainer::meta_PackageConfigurationReference_child = Uml::CreateCORBACompositionChildRole();
		TopLevelPackageContainer::meta_package_child = Uml::CreateCORBACompositionChildRole();
		TopLevelPackages::meta_TopLevelPackageContainer_children = Uml::CreateCORBACompositionChildRole();
		ComponentContainer::meta_ComponentConfigProperty_children = Uml::CreateCORBACompositionChildRole();
		ComponentContainer::meta_ComponentProperty_children = Uml::CreateCORBACompositionChildRole();
		ComponentContainer::meta_ComponentInfoProperty_children = Uml::CreateCORBACompositionChildRole();
		ComponentContainer::meta_Property_children = Uml::CreateCORBACompositionChildRole();
		ComponentContainer::meta_ComponentType_child = Uml::CreateCORBACompositionChildRole();
		ComponentContainer::meta_ComponentPropertyDescription_children = Uml::CreateCORBACompositionChildRole();
		ComponentTypes::meta_ComponentContainer_children = Uml::CreateCORBACompositionChildRole();
		ComponentPropertyDescription::meta_DataType_child = Uml::CreateCORBACompositionChildRole();
		ComponentImplementations::meta_ComponentImplementationContainer_children = Uml::CreateCORBACompositionChildRole();
		ComponentImplementationContainer::meta_ComponentType_children = Uml::CreateCORBACompositionChildRole();
		ComponentImplementationContainer::meta_ConfigProperty_children = Uml::CreateCORBACompositionChildRole();
		ComponentImplementationContainer::meta_MonolithprimaryArtifact_children = Uml::CreateCORBACompositionChildRole();
		ComponentImplementationContainer::meta_Property_children = Uml::CreateCORBACompositionChildRole();
		ComponentImplementationContainer::meta_ImplementationCapability_children = Uml::CreateCORBACompositionChildRole();
		ComponentImplementationContainer::meta_ImplementationDependsOn_children = Uml::CreateCORBACompositionChildRole();
		ComponentImplementationContainer::meta_Implements_children = Uml::CreateCORBACompositionChildRole();
		ComponentImplementationContainer::meta_MonolithDeployRequirement_children = Uml::CreateCORBACompositionChildRole();
		ComponentImplementationContainer::meta_ImplementationRequirement_children = Uml::CreateCORBACompositionChildRole();
		ComponentImplementationContainer::meta_ImplementationArtifactReference_children = Uml::CreateCORBACompositionChildRole();
		ComponentImplementationContainer::meta_InfoProperty_children = Uml::CreateCORBACompositionChildRole();
		ComponentImplementationContainer::meta_Capability_children = Uml::CreateCORBACompositionChildRole();
		ComponentImplementationContainer::meta_MonolithExecParameter_children = Uml::CreateCORBACompositionChildRole();
		ComponentImplementationContainer::meta_ComponentImplementation_child = Uml::CreateCORBACompositionChildRole();
		ComponentImplementationContainer::meta_ImplementationDependency_children = Uml::CreateCORBACompositionChildRole();
		DeploymentPlans::meta_DeploymentPlan_children = Uml::CreateCORBACompositionChildRole();
		DeploymentPlan::meta_InstanceMapping_children = Uml::CreateCORBACompositionChildRole();
		DeploymentPlan::meta_CollocationGroup_children = Uml::CreateCORBACompositionChildRole();
		DeploymentPlan::meta_ComponentType_children = Uml::CreateCORBACompositionChildRole();
		DeploymentPlan::meta_NodeReference_children = Uml::CreateCORBACompositionChildRole();
		PackageConfigurationContainer::meta_PackageConfSelectRequirement_children = Uml::CreateCORBACompositionChildRole();
		PackageConfigurationContainer::meta_PackageConfiguration_children = Uml::CreateCORBACompositionChildRole();
		PackageConfigurationContainer::meta_PackageConfBasePackage_child = Uml::CreateCORBACompositionChildRole();
		PackageConfigurationContainer::meta_PackageConfReference_child = Uml::CreateCORBACompositionChildRole();
		PackageConfigurationContainer::meta_PackageConfConfigProperty_children = Uml::CreateCORBACompositionChildRole();
		PackageConfigurationContainer::meta_Property_children = Uml::CreateCORBACompositionChildRole();
		PackageConfigurationContainer::meta_Requirement_children = Uml::CreateCORBACompositionChildRole();
		PackageConfigurationContainer::meta_ComponentPackageReference_child = Uml::CreateCORBACompositionChildRole();
		PackageConfigurationContainer::meta_PackageConfSpecializedConfig_child = Uml::CreateCORBACompositionChildRole();
		PackageConfigurationContainer::meta_ComponentPackage_child = Uml::CreateCORBACompositionChildRole();
		PackageConfigurations::meta_PackageConfigurationContainer_children = Uml::CreateCORBACompositionChildRole();
		ComponentPackages::meta_PackageContainer_children = Uml::CreateCORBACompositionChildRole();
		PackageContainer::meta_Implementation_children = Uml::CreateCORBACompositionChildRole();
		PackageContainer::meta_PackageInterface_child = Uml::CreateCORBACompositionChildRole();
		PackageContainer::meta_PackageConfigProperty_children = Uml::CreateCORBACompositionChildRole();
		PackageContainer::meta_Property_children = Uml::CreateCORBACompositionChildRole();
		PackageContainer::meta_ComponentType_children = Uml::CreateCORBACompositionChildRole();
		PackageContainer::meta_ComponentImplementationReference_children = Uml::CreateCORBACompositionChildRole();
		PackageContainer::meta_ComponentPackage_child = Uml::CreateCORBACompositionChildRole();
		PackageContainer::meta_PackageInfoProperty_children = Uml::CreateCORBACompositionChildRole();
		Resource::meta_SatisfierProperty_children = Uml::CreateCORBACompositionChildRole();
		Node::meta_Resource_children = Uml::CreateCORBACompositionChildRole();
		SharedResource::meta_SatisfierProperty_children = Uml::CreateCORBACompositionChildRole();
		Interconnect::meta_Resource_children = Uml::CreateCORBACompositionChildRole();
		Bridge::meta_Resource_children = Uml::CreateCORBACompositionChildRole();
		SatisfierProperty::meta_DataType_child = Uml::CreateCORBACompositionChildRole();
		Property::meta_DataType_child = Uml::CreateCORBACompositionChildRole();
		RequirementSatisfier::meta_SatisfierProperty_children = Uml::CreateCORBACompositionChildRole();
		RequirementBase::meta_Property_children = Uml::CreateCORBACompositionChildRole();
		Domain::meta_Node2Interconnect_children = Uml::CreateCORBACompositionChildRole();
		Domain::meta_Interconnect2Bridge_children = Uml::CreateCORBACompositionChildRole();
		Domain::meta_Bridge2Interconnect_children = Uml::CreateCORBACompositionChildRole();
		Domain::meta_Property_children = Uml::CreateCORBACompositionChildRole();
		Domain::meta_Shares_children = Uml::CreateCORBACompositionChildRole();
		Domain::meta_Interconnect2Node_children = Uml::CreateCORBACompositionChildRole();
		Domain::meta_Elements_children = Uml::CreateCORBACompositionChildRole();
		Targets::meta_Domain_children = Uml::CreateCORBACompositionChildRole();
		ComponentAssembly::meta_PublishConnector_children = Uml::CreateCORBACompositionChildRole();
		ComponentAssembly::meta_AssemblyConfigProperty_children = Uml::CreateCORBACompositionChildRole();
		ComponentAssembly::meta_Requirement_children = Uml::CreateCORBACompositionChildRole();
		ComponentAssembly::meta_ComponentPackageReference_children = Uml::CreateCORBACompositionChildRole();
		ComponentAssembly::meta_emit_children = Uml::CreateCORBACompositionChildRole();
		ComponentAssembly::meta_Component_children = Uml::CreateCORBACompositionChildRole();
		ComponentAssembly::meta_AssemblyDeployRequirement_children = Uml::CreateCORBACompositionChildRole();
		ComponentAssembly::meta_publish_children = Uml::CreateCORBACompositionChildRole();
		ComponentAssembly::meta_AssemblyselectRequirement_children = Uml::CreateCORBACompositionChildRole();
		ComponentAssembly::meta_ComponentPackage_children = Uml::CreateCORBACompositionChildRole();
		ComponentAssembly::meta_invoke_children = Uml::CreateCORBACompositionChildRole();
		ComponentAssembly::meta_deliverTo_children = Uml::CreateCORBACompositionChildRole();
		ComponentAssembly::meta_Property_children = Uml::CreateCORBACompositionChildRole();
		ImplementationArtifacts::meta_ArtifactContainer_children = Uml::CreateCORBACompositionChildRole();
		ArtifactContainer::meta_Requirement_children = Uml::CreateCORBACompositionChildRole();
		ArtifactContainer::meta_ArtifactExecParameter_children = Uml::CreateCORBACompositionChildRole();
		ArtifactContainer::meta_Property_children = Uml::CreateCORBACompositionChildRole();
		ArtifactContainer::meta_ArtifactDeployRequirement_children = Uml::CreateCORBACompositionChildRole();
		ArtifactContainer::meta_ImplementationArtifactReference_children = Uml::CreateCORBACompositionChildRole();
		ArtifactContainer::meta_ImplementationArtifact_children = Uml::CreateCORBACompositionChildRole();
		ArtifactContainer::meta_ArtifactInfoProperty_children = Uml::CreateCORBACompositionChildRole();
		ArtifactContainer::meta_ArtifactDependsOn_children = Uml::CreateCORBACompositionChildRole();
		Package::meta_Exception_children = Uml::CreateCORBACompositionChildRole();
		Package::meta_Package_children = Uml::CreateCORBACompositionChildRole();
		Package::meta_Constant_children = Uml::CreateCORBACompositionChildRole();
		Package::meta_NamedType_children = Uml::CreateCORBACompositionChildRole();
		Package::meta_ComponentRef_children = Uml::CreateCORBACompositionChildRole();
		Package::meta_ManagesComponent_children = Uml::CreateCORBACompositionChildRole();
		File::meta_Exception_children = Uml::CreateCORBACompositionChildRole();
		File::meta_Package_children = Uml::CreateCORBACompositionChildRole();
		File::meta_FileRef_children = Uml::CreateCORBACompositionChildRole();
		File::meta_Constant_children = Uml::CreateCORBACompositionChildRole();
		File::meta_NamedType_children = Uml::CreateCORBACompositionChildRole();
		File::meta_ComponentRef_children = Uml::CreateCORBACompositionChildRole();
		File::meta_ManagesComponent_children = Uml::CreateCORBACompositionChildRole();
		Exception::meta_Member_children = Uml::CreateCORBACompositionChildRole();
		InterfaceDefinitions::meta_File_children = Uml::CreateCORBACompositionChildRole();
		PredefinedTypes::meta_PredefinedType_children = Uml::CreateCORBACompositionChildRole();
		Enum::meta_EnumValue_children = Uml::CreateCORBACompositionChildRole();
		Aggregate::meta_Member_children = Uml::CreateCORBACompositionChildRole();
		SwitchedAggregate::meta_Label_children = Uml::CreateCORBACompositionChildRole();
		SwitchedAggregate::meta_LabelConnection_children = Uml::CreateCORBACompositionChildRole();
		SwitchedAggregate::meta_Member_children = Uml::CreateCORBACompositionChildRole();
		SwitchedAggregate::meta_Discriminator_child = Uml::CreateCORBACompositionChildRole();
		Inheritable::meta_ReadonlyAttribute_children = Uml::CreateCORBACompositionChildRole();
		Inheritable::meta_Inherits_children = Uml::CreateCORBACompositionChildRole();
		HasOperations::meta_TwowayOperation_children = Uml::CreateCORBACompositionChildRole();
		HasOperations::meta_OnewayOperation_children = Uml::CreateCORBACompositionChildRole();
		HasOperations::meta_Exception_children = Uml::CreateCORBACompositionChildRole();
		HasOperations::meta_Constant_children = Uml::CreateCORBACompositionChildRole();
		HasOperations::meta_NoInheritable_children = Uml::CreateCORBACompositionChildRole();
		SupportsInterfaces::meta_Supports_children = Uml::CreateCORBACompositionChildRole();
		ReadonlyAttribute::meta_AttributeMember_child = Uml::CreateCORBACompositionChildRole();
		ReadonlyAttribute::meta_GetException_children = Uml::CreateCORBACompositionChildRole();
		ObjectByValue::meta_PrivateFlag_children = Uml::CreateCORBACompositionChildRole();
		ObjectByValue::meta_FactoryOperation_children = Uml::CreateCORBACompositionChildRole();
		ObjectByValue::meta_MakeMemberPrivate_children = Uml::CreateCORBACompositionChildRole();
		ObjectByValue::meta_Member_children = Uml::CreateCORBACompositionChildRole();
		Attribute::meta_SetException_children = Uml::CreateCORBACompositionChildRole();
		OperationBase::meta_InParameter_children = Uml::CreateCORBACompositionChildRole();
		TwowayOperation::meta_OutParameter_children = Uml::CreateCORBACompositionChildRole();
		TwowayOperation::meta_InoutParameter_children = Uml::CreateCORBACompositionChildRole();
		TwowayOperation::meta_ReturnType_child = Uml::CreateCORBACompositionChildRole();
		HasExceptions::meta_ExceptionRef_children = Uml::CreateCORBACompositionChildRole();
		Component::meta_Port_children = Uml::CreateCORBACompositionChildRole();
		ComponentFactory::meta_LookupKey_child = Uml::CreateCORBACompositionChildRole();
		ComponentFactory::meta_FactoryOperation_children = Uml::CreateCORBACompositionChildRole();
		ComponentFactory::meta_LookupOperation_children = Uml::CreateCORBACompositionChildRole();
		RootFolder::meta_ComponentTypes_children = Uml::CreateCORBACompositionChildRole();
		RootFolder::meta_TopLevelPackages_children = Uml::CreateCORBACompositionChildRole();
		RootFolder::meta_ComponentImplementations_children = Uml::CreateCORBACompositionChildRole();
		RootFolder::meta_DeploymentPlans_children = Uml::CreateCORBACompositionChildRole();
		RootFolder::meta_PackageConfigurations_children = Uml::CreateCORBACompositionChildRole();
		RootFolder::meta_ComponentPackages_children = Uml::CreateCORBACompositionChildRole();
		RootFolder::meta_Targets_children = Uml::CreateCORBACompositionChildRole();
		RootFolder::meta_ImplementationArtifacts_children = Uml::CreateCORBACompositionChildRole();
		RootFolder::meta_InterfaceDefinitions_children = Uml::CreateCORBACompositionChildRole();
		RootFolder::meta_PredefinedTypes_children = Uml::CreateCORBACompositionChildRole();
		RootFolder::meta_RootFolder_children = Uml::CreateCORBACompositionChildRole();
		Uml::InitCORBADiagram(umldiagram, "PICML");

		Uml::InitCORBAClass(package::meta, umldiagram, "package", false, "Connection");
		Uml::InitCORBAClass(TopLevelPackageContainer::meta, umldiagram, "TopLevelPackageContainer", false, "Model");
		Uml::InitCORBAClass(TopLevelPackages::meta, umldiagram, "TopLevelPackages", false, "Folder");
		Uml::InitCORBAClass(TopLevelPackage::meta, umldiagram, "TopLevelPackage", false, "Atom");
		Uml::InitCORBAClass(ComponentInfoProperty::meta, umldiagram, "ComponentInfoProperty", false, "Connection");
		Uml::InitCORBAClass(ComponentContainer::meta, umldiagram, "ComponentContainer", false, "Model");
		Uml::InitCORBAClass(ComponentProperty::meta, umldiagram, "ComponentProperty", false, "Connection");
		Uml::InitCORBAClass(CommonPortAttrs::meta, umldiagram, "CommonPortAttrs", true, "FCO");
		Uml::InitCORBAClass(ComponentType::meta, umldiagram, "ComponentType", false, "Reference");
		Uml::InitCORBAClass(ComponentTypes::meta, umldiagram, "ComponentTypes", false, "Folder");
		Uml::InitCORBAClass(ComponentConfigProperty::meta, umldiagram, "ComponentConfigProperty", false, "Connection");
		Uml::InitCORBAClass(ComponentPropertyDescription::meta, umldiagram, "ComponentPropertyDescription", false, "Model");
		Uml::InitCORBAClass(ConfigProperty::meta, umldiagram, "ConfigProperty", false, "Connection");
		Uml::InitCORBAClass(Implements::meta, umldiagram, "Implements", false, "Connection");
		Uml::InitCORBAClass(ImplementationCapability::meta, umldiagram, "ImplementationCapability", false, "Connection");
		Uml::InitCORBAClass(ImplementationDependsOn::meta, umldiagram, "ImplementationDependsOn", false, "Connection");
		Uml::InitCORBAClass(InfoProperty::meta, umldiagram, "InfoProperty", false, "Connection");
		Uml::InitCORBAClass(ComponentImplementations::meta, umldiagram, "ComponentImplementations", false, "Folder");
		Uml::InitCORBAClass(MonolithprimaryArtifact::meta, umldiagram, "MonolithprimaryArtifact", false, "Connection");
		Uml::InitCORBAClass(ComponentImplementationReference::meta, umldiagram, "ComponentImplementationReference", false, "Reference");
		Uml::InitCORBAClass(MonolithExecParameter::meta, umldiagram, "MonolithExecParameter", false, "Connection");
		Uml::InitCORBAClass(ComponentImplementationContainer::meta, umldiagram, "ComponentImplementationContainer", false, "Model");
		Uml::InitCORBAClass(MonolithDeployRequirement::meta, umldiagram, "MonolithDeployRequirement", false, "Connection");
		Uml::InitCORBAClass(ComponentImplementation::meta, umldiagram, "ComponentImplementation", true, "FCO");
		Uml::InitCORBAClass(MonolithicImplementation::meta, umldiagram, "MonolithicImplementation", false, "Atom");
		Uml::InitCORBAClass(DeploymentPlans::meta, umldiagram, "DeploymentPlans", false, "Folder");
		Uml::InitCORBAClass(DeploymentPlan::meta, umldiagram, "DeploymentPlan", false, "Model");
		Uml::InitCORBAClass(CollocationGroup::meta, umldiagram, "CollocationGroup", false, "Set");
		Uml::InitCORBAClass(InstanceMapping::meta, umldiagram, "InstanceMapping", false, "Connection");
		Uml::InitCORBAClass(PackageConfSpecializedConfig::meta, umldiagram, "PackageConfSpecializedConfig", false, "Connection");
		Uml::InitCORBAClass(PackageConfigurationContainer::meta, umldiagram, "PackageConfigurationContainer", false, "Model");
		Uml::InitCORBAClass(PackageConfConfigProperty::meta, umldiagram, "PackageConfConfigProperty", false, "Connection");
		Uml::InitCORBAClass(PackageConfBasePackage::meta, umldiagram, "PackageConfBasePackage", false, "Connection");
		Uml::InitCORBAClass(PackageConfigurationReference::meta, umldiagram, "PackageConfigurationReference", false, "Reference");
		Uml::InitCORBAClass(PackageConfigurations::meta, umldiagram, "PackageConfigurations", false, "Folder");
		Uml::InitCORBAClass(PackageConfSelectRequirement::meta, umldiagram, "PackageConfSelectRequirement", false, "Connection");
		Uml::InitCORBAClass(PackageConfReference::meta, umldiagram, "PackageConfReference", false, "Connection");
		Uml::InitCORBAClass(PackageConfiguration::meta, umldiagram, "PackageConfiguration", false, "Atom");
		Uml::InitCORBAClass(PackageInterface::meta, umldiagram, "PackageInterface", false, "Connection");
		Uml::InitCORBAClass(PackageInfoProperty::meta, umldiagram, "PackageInfoProperty", false, "Connection");
		Uml::InitCORBAClass(PackageConfigProperty::meta, umldiagram, "PackageConfigProperty", false, "Connection");
		Uml::InitCORBAClass(ComponentPackages::meta, umldiagram, "ComponentPackages", false, "Folder");
		Uml::InitCORBAClass(PackageContainer::meta, umldiagram, "PackageContainer", false, "Model");
		Uml::InitCORBAClass(Implementation::meta, umldiagram, "Implementation", false, "Connection");
		Uml::InitCORBAClass(ComponentPackage::meta, umldiagram, "ComponentPackage", false, "Atom");
		Uml::InitCORBAClass(ComponentPackageReference::meta, umldiagram, "ComponentPackageReference", false, "Reference");
		Uml::InitCORBAClass(Resource::meta, umldiagram, "Resource", false, "Model");
		Uml::InitCORBAClass(Node::meta, umldiagram, "Node", false, "Model");
		Uml::InitCORBAClass(SharedResource::meta, umldiagram, "SharedResource", false, "Model");
		Uml::InitCORBAClass(Interconnect::meta, umldiagram, "Interconnect", false, "Model");
		Uml::InitCORBAClass(NodeReference::meta, umldiagram, "NodeReference", false, "Reference");
		Uml::InitCORBAClass(Bridge::meta, umldiagram, "Bridge", false, "Model");
		Uml::InitCORBAClass(SatisfierProperty::meta, umldiagram, "SatisfierProperty", false, "Model");
		Uml::InitCORBAClass(Requirement::meta, umldiagram, "Requirement", false, "Model");
		Uml::InitCORBAClass(DataType::meta, umldiagram, "DataType", false, "Reference");
		Uml::InitCORBAClass(ImplementationDependency::meta, umldiagram, "ImplementationDependency", false, "Atom");
		Uml::InitCORBAClass(Property::meta, umldiagram, "Property", false, "Model");
		Uml::InitCORBAClass(Capability::meta, umldiagram, "Capability", false, "Model");
		Uml::InitCORBAClass(RequirementSatisfier::meta, umldiagram, "RequirementSatisfier", true, "Model");
		Uml::InitCORBAClass(ImplementationRequirement::meta, umldiagram, "ImplementationRequirement", false, "Model");
		Uml::InitCORBAClass(RequirementBase::meta, umldiagram, "RequirementBase", true, "Model");
		Uml::InitCORBAClass(Elements::meta, umldiagram, "Elements", true, "FCO");
		Uml::InitCORBAClass(Domain::meta, umldiagram, "Domain", false, "Model");
		Uml::InitCORBAClass(Interconnect2Node::meta, umldiagram, "Interconnect2Node", false, "Connection");
		Uml::InitCORBAClass(Bridge2Interconnect::meta, umldiagram, "Bridge2Interconnect", false, "Connection");
		Uml::InitCORBAClass(Shares::meta, umldiagram, "Shares", false, "Connection");
		Uml::InitCORBAClass(Node2Interconnect::meta, umldiagram, "Node2Interconnect", false, "Connection");
		Uml::InitCORBAClass(Interconnect2Bridge::meta, umldiagram, "Interconnect2Bridge", false, "Connection");
		Uml::InitCORBAClass(Targets::meta, umldiagram, "Targets", false, "Folder");
		Uml::InitCORBAClass(AssemblyDeployRequirement::meta, umldiagram, "AssemblyDeployRequirement", false, "Connection");
		Uml::InitCORBAClass(invoke::meta, umldiagram, "invoke", false, "Connection");
		Uml::InitCORBAClass(ComponentAssembly::meta, umldiagram, "ComponentAssembly", false, "Model");
		Uml::InitCORBAClass(AssemblyselectRequirement::meta, umldiagram, "AssemblyselectRequirement", false, "Connection");
		Uml::InitCORBAClass(emit::meta, umldiagram, "emit", false, "Connection");
		Uml::InitCORBAClass(PublishConnector::meta, umldiagram, "PublishConnector", false, "Atom");
		Uml::InitCORBAClass(deliverTo::meta, umldiagram, "deliverTo", false, "Connection");
		Uml::InitCORBAClass(publish::meta, umldiagram, "publish", false, "Connection");
		Uml::InitCORBAClass(AssemblyConfigProperty::meta, umldiagram, "AssemblyConfigProperty", false, "Connection");
		Uml::InitCORBAClass(ArtifactExecParameter::meta, umldiagram, "ArtifactExecParameter", false, "Connection");
		Uml::InitCORBAClass(ImplementationArtifactReference::meta, umldiagram, "ImplementationArtifactReference", false, "Reference");
		Uml::InitCORBAClass(ArtifactDependsOn::meta, umldiagram, "ArtifactDependsOn", false, "Connection");
		Uml::InitCORBAClass(ImplementationArtifact::meta, umldiagram, "ImplementationArtifact", false, "Atom");
		Uml::InitCORBAClass(ImplementationArtifacts::meta, umldiagram, "ImplementationArtifacts", false, "Folder");
		Uml::InitCORBAClass(ArtifactDeployRequirement::meta, umldiagram, "ArtifactDeployRequirement", false, "Connection");
		Uml::InitCORBAClass(ArtifactInfoProperty::meta, umldiagram, "ArtifactInfoProperty", false, "Connection");
		Uml::InitCORBAClass(ArtifactContainer::meta, umldiagram, "ArtifactContainer", false, "Model");
		Uml::InitCORBAClass(Package::meta, umldiagram, "Package", false, "Model");
		Uml::InitCORBAClass(File::meta, umldiagram, "File", false, "Model");
		Uml::InitCORBAClass(Exception::meta, umldiagram, "Exception", false, "Model");
		Uml::InitCORBAClass(ExceptionRef::meta, umldiagram, "ExceptionRef", false, "Reference");
		Uml::InitCORBAClass(FileRef::meta, umldiagram, "FileRef", false, "Reference");
		Uml::InitCORBAClass(InterfaceDefinitions::meta, umldiagram, "InterfaceDefinitions", false, "Folder");
		Uml::InitCORBAClass(ConstantType::meta, umldiagram, "ConstantType", true, "FCO");
		Uml::InitCORBAClass(Constant::meta, umldiagram, "Constant", false, "Reference");
		Uml::InitCORBAClass(PredefinedTypes::meta, umldiagram, "PredefinedTypes", false, "Folder");
		Uml::InitCORBAClass(PredefinedType::meta, umldiagram, "PredefinedType", true, "FCO");
		Uml::InitCORBAClass(Byte::meta, umldiagram, "Byte", false, "Atom");
		Uml::InitCORBAClass(Boolean::meta, umldiagram, "Boolean", false, "Atom");
		Uml::InitCORBAClass(ShortInteger::meta, umldiagram, "ShortInteger", false, "Atom");
		Uml::InitCORBAClass(LongInteger::meta, umldiagram, "LongInteger", false, "Atom");
		Uml::InitCORBAClass(RealNumber::meta, umldiagram, "RealNumber", false, "Atom");
		Uml::InitCORBAClass(String::meta, umldiagram, "String", false, "Atom");
		Uml::InitCORBAClass(GenericObject::meta, umldiagram, "GenericObject", false, "Atom");
		Uml::InitCORBAClass(GenericValueObject::meta, umldiagram, "GenericValueObject", false, "Atom");
		Uml::InitCORBAClass(GenericValue::meta, umldiagram, "GenericValue", false, "Atom");
		Uml::InitCORBAClass(TypeEncoding::meta, umldiagram, "TypeEncoding", false, "Atom");
		Uml::InitCORBAClass(TypeKind::meta, umldiagram, "TypeKind", false, "Atom");
		Uml::InitCORBAClass(Enum::meta, umldiagram, "Enum", false, "Model");
		Uml::InitCORBAClass(EnumValue::meta, umldiagram, "EnumValue", false, "Atom");
		Uml::InitCORBAClass(Aggregate::meta, umldiagram, "Aggregate", false, "Model");
		Uml::InitCORBAClass(SwitchedAggregate::meta, umldiagram, "SwitchedAggregate", false, "Model");
		Uml::InitCORBAClass(Label::meta, umldiagram, "Label", false, "Atom");
		Uml::InitCORBAClass(LabelConnection::meta, umldiagram, "LabelConnection", false, "Connection");
		Uml::InitCORBAClass(MemberType::meta, umldiagram, "MemberType", true, "FCO");
		Uml::InitCORBAClass(Discriminator::meta, umldiagram, "Discriminator", false, "Reference");
		Uml::InitCORBAClass(NamedType::meta, umldiagram, "NamedType", true, "FCO");
		Uml::InitCORBAClass(Collection::meta, umldiagram, "Collection", false, "Reference");
		Uml::InitCORBAClass(Alias::meta, umldiagram, "Alias", false, "Reference");
		Uml::InitCORBAClass(Boxed::meta, umldiagram, "Boxed", false, "Reference");
		Uml::InitCORBAClass(Member::meta, umldiagram, "Member", false, "Reference");
		Uml::InitCORBAClass(NoInheritable::meta, umldiagram, "NoInheritable", true, "FCO");
		Uml::InitCORBAClass(Object::meta, umldiagram, "Object", false, "Model");
		Uml::InitCORBAClass(Inherits::meta, umldiagram, "Inherits", false, "Reference");
		Uml::InitCORBAClass(Event::meta, umldiagram, "Event", false, "Model");
		Uml::InitCORBAClass(ValueObject::meta, umldiagram, "ValueObject", false, "Model");
		Uml::InitCORBAClass(Inheritable::meta, umldiagram, "Inheritable", true, "Model");
		Uml::InitCORBAClass(HasOperations::meta, umldiagram, "HasOperations", true, "Model");
		Uml::InitCORBAClass(SupportsInterfaces::meta, umldiagram, "SupportsInterfaces", true, "Model");
		Uml::InitCORBAClass(ReadonlyAttribute::meta, umldiagram, "ReadonlyAttribute", false, "Model");
		Uml::InitCORBAClass(LookupKey::meta, umldiagram, "LookupKey", false, "Reference");
		Uml::InitCORBAClass(SetException::meta, umldiagram, "SetException", false, "Reference");
		Uml::InitCORBAClass(ObjectByValue::meta, umldiagram, "ObjectByValue", true, "Model");
		Uml::InitCORBAClass(GetException::meta, umldiagram, "GetException", false, "Reference");
		Uml::InitCORBAClass(PrivateFlag::meta, umldiagram, "PrivateFlag", false, "Atom");
		Uml::InitCORBAClass(MakeMemberPrivate::meta, umldiagram, "MakeMemberPrivate", false, "Connection");
		Uml::InitCORBAClass(AttributeMember::meta, umldiagram, "AttributeMember", false, "Reference");
		Uml::InitCORBAClass(Supports::meta, umldiagram, "Supports", false, "Reference");
		Uml::InitCORBAClass(Attribute::meta, umldiagram, "Attribute", false, "Model");
		Uml::InitCORBAClass(OperationBase::meta, umldiagram, "OperationBase", true, "Model");
		Uml::InitCORBAClass(FactoryOperation::meta, umldiagram, "FactoryOperation", false, "Model");
		Uml::InitCORBAClass(LookupOperation::meta, umldiagram, "LookupOperation", false, "Model");
		Uml::InitCORBAClass(TwowayOperation::meta, umldiagram, "TwowayOperation", false, "Model");
		Uml::InitCORBAClass(OnewayOperation::meta, umldiagram, "OnewayOperation", false, "Model");
		Uml::InitCORBAClass(HasExceptions::meta, umldiagram, "HasExceptions", true, "Model");
		Uml::InitCORBAClass(InParameter::meta, umldiagram, "InParameter", false, "Reference");
		Uml::InitCORBAClass(OutParameter::meta, umldiagram, "OutParameter", false, "Reference");
		Uml::InitCORBAClass(ReturnType::meta, umldiagram, "ReturnType", false, "Reference");
		Uml::InitCORBAClass(InoutParameter::meta, umldiagram, "InoutParameter", false, "Reference");
		Uml::InitCORBAClass(Component::meta, umldiagram, "Component", false, "Model");
		Uml::InitCORBAClass(OutEventPort::meta, umldiagram, "OutEventPort", false, "Reference");
		Uml::InitCORBAClass(ProvidedRequestPort::meta, umldiagram, "ProvidedRequestPort", false, "Reference");
		Uml::InitCORBAClass(RequiredRequestPort::meta, umldiagram, "RequiredRequestPort", false, "Reference");
		Uml::InitCORBAClass(InEventPort::meta, umldiagram, "InEventPort", false, "Reference");
		Uml::InitCORBAClass(Port::meta, umldiagram, "Port", true, "FCO");
		Uml::InitCORBAClass(ComponentRef::meta, umldiagram, "ComponentRef", false, "Reference");
		Uml::InitCORBAClass(Manageable::meta, umldiagram, "Manageable", true, "FCO");
		Uml::InitCORBAClass(ComponentFactory::meta, umldiagram, "ComponentFactory", false, "Model");
		Uml::InitCORBAClass(ManagesComponent::meta, umldiagram, "ManagesComponent", false, "Connection");
		Uml::InitCORBAClass(Provideable::meta, umldiagram, "Provideable", true, "FCO");
		Uml::InitCORBAClass(Prefixable::meta, umldiagram, "Prefixable", true, "Model");
		Uml::InitCORBAClass(Taggable::meta, umldiagram, "Taggable", true, "FCO");
		Uml::InitCORBAClass(RootFolder::meta, umldiagram, "RootFolder", false, "Folder");
		Uml::InitCORBAClass(MgaObject::meta, umldiagram, "MgaObject", true);
		
		vector<string> TopLevelPackages_name_dva;
		Uml::InitCORBAAttribute(TopLevelPackages::meta_name, TopLevelPackages::meta, "name", "String", false, 0, 1, false, "public", TopLevelPackages_name_dva);
		vector<string> CommonPortAttrs_exclusiveUser_dva;
		CommonPortAttrs_exclusiveUser_dva.push_back("false");
		Uml::InitCORBAAttribute(CommonPortAttrs::meta_exclusiveUser, CommonPortAttrs::meta, "exclusiveUser", "Boolean", false, 1, 1, false, "public", CommonPortAttrs_exclusiveUser_dva);
		vector<string> CommonPortAttrs_optional_dva;
		CommonPortAttrs_optional_dva.push_back("false");
		Uml::InitCORBAAttribute(CommonPortAttrs::meta_optional, CommonPortAttrs::meta, "optional", "Boolean", false, 1, 1, false, "public", CommonPortAttrs_optional_dva);
		vector<string> CommonPortAttrs_exclusiveProvider_dva;
		CommonPortAttrs_exclusiveProvider_dva.push_back("false");
		Uml::InitCORBAAttribute(CommonPortAttrs::meta_exclusiveProvider, CommonPortAttrs::meta, "exclusiveProvider", "Boolean", false, 1, 1, false, "public", CommonPortAttrs_exclusiveProvider_dva);
		vector<string> ComponentTypes_name_dva;
		Uml::InitCORBAAttribute(ComponentTypes::meta_name, ComponentTypes::meta, "name", "String", false, 0, 1, false, "public", ComponentTypes_name_dva);
		vector<string> ComponentImplementations_name_dva;
		Uml::InitCORBAAttribute(ComponentImplementations::meta_name, ComponentImplementations::meta, "name", "String", false, 0, 1, false, "public", ComponentImplementations_name_dva);
		vector<string> ComponentImplementation_UUID_dva;
		Uml::InitCORBAAttribute(ComponentImplementation::meta_UUID, ComponentImplementation::meta, "UUID", "String", false, 1, 1, false, "public", ComponentImplementation_UUID_dva);
		vector<string> ComponentImplementation_label_dva;
		Uml::InitCORBAAttribute(ComponentImplementation::meta_label, ComponentImplementation::meta, "label", "String", false, 1, 1, false, "public", ComponentImplementation_label_dva);
		vector<string> DeploymentPlans_name_dva;
		Uml::InitCORBAAttribute(DeploymentPlans::meta_name, DeploymentPlans::meta, "name", "String", false, 0, 1, false, "public", DeploymentPlans_name_dva);
		vector<string> DeploymentPlan_label_dva;
		Uml::InitCORBAAttribute(DeploymentPlan::meta_label, DeploymentPlan::meta, "label", "String", false, 1, 1, false, "public", DeploymentPlan_label_dva);
		vector<string> PackageConfigurations_name_dva;
		Uml::InitCORBAAttribute(PackageConfigurations::meta_name, PackageConfigurations::meta, "name", "String", false, 0, 1, false, "public", PackageConfigurations_name_dva);
		vector<string> PackageConfiguration_UUID_dva;
		Uml::InitCORBAAttribute(PackageConfiguration::meta_UUID, PackageConfiguration::meta, "UUID", "String", false, 1, 1, false, "public", PackageConfiguration_UUID_dva);
		vector<string> PackageConfiguration_label_dva;
		Uml::InitCORBAAttribute(PackageConfiguration::meta_label, PackageConfiguration::meta, "label", "String", false, 1, 1, false, "public", PackageConfiguration_label_dva);
		vector<string> ComponentPackages_name_dva;
		Uml::InitCORBAAttribute(ComponentPackages::meta_name, ComponentPackages::meta, "name", "String", false, 0, 1, false, "public", ComponentPackages_name_dva);
		vector<string> ComponentPackage_UUID_dva;
		Uml::InitCORBAAttribute(ComponentPackage::meta_UUID, ComponentPackage::meta, "UUID", "String", false, 1, 1, false, "public", ComponentPackage_UUID_dva);
		vector<string> ComponentPackage_label_dva;
		Uml::InitCORBAAttribute(ComponentPackage::meta_label, ComponentPackage::meta, "label", "String", false, 1, 1, false, "public", ComponentPackage_label_dva);
		vector<string> ComponentPackageReference_requiredName_dva;
		Uml::InitCORBAAttribute(ComponentPackageReference::meta_requiredName, ComponentPackageReference::meta, "requiredName", "String", false, 1, 1, false, "public", ComponentPackageReference_requiredName_dva);
		vector<string> ComponentPackageReference_requiredUUID_dva;
		Uml::InitCORBAAttribute(ComponentPackageReference::meta_requiredUUID, ComponentPackageReference::meta, "requiredUUID", "String", false, 1, 1, false, "public", ComponentPackageReference_requiredUUID_dva);
		vector<string> ComponentPackageReference_requiredType_dva;
		Uml::InitCORBAAttribute(ComponentPackageReference::meta_requiredType, ComponentPackageReference::meta, "requiredType", "String", false, 1, 1, false, "public", ComponentPackageReference_requiredType_dva);
		vector<string> Resource_resourceType_dva;
		Uml::InitCORBAAttribute(Resource::meta_resourceType, Resource::meta, "resourceType", "String", false, 1, 1, false, "public", Resource_resourceType_dva);
		vector<string> Node_label_dva;
		Uml::InitCORBAAttribute(Node::meta_label, Node::meta, "label", "String", false, 1, 1, false, "public", Node_label_dva);
		vector<string> SharedResource_resourceType_dva;
		Uml::InitCORBAAttribute(SharedResource::meta_resourceType, SharedResource::meta, "resourceType", "String", false, 1, 1, false, "public", SharedResource_resourceType_dva);
		vector<string> Interconnect_label_dva;
		Uml::InitCORBAAttribute(Interconnect::meta_label, Interconnect::meta, "label", "String", false, 1, 1, false, "public", Interconnect_label_dva);
		vector<string> Bridge_label_dva;
		Uml::InitCORBAAttribute(Bridge::meta_label, Bridge::meta, "label", "String", false, 1, 1, false, "public", Bridge_label_dva);
		vector<string> SatisfierProperty_DataValue_dva;
		Uml::InitCORBAAttribute(SatisfierProperty::meta_DataValue, SatisfierProperty::meta, "DataValue", "String", false, 1, 1, false, "public", SatisfierProperty_DataValue_dva);
		vector<string> SatisfierProperty_SatisfierPropertyKind_dva;
		SatisfierProperty_SatisfierPropertyKind_dva.push_back("Quantity");
		Uml::InitCORBAAttribute(SatisfierProperty::meta_SatisfierPropertyKind, SatisfierProperty::meta, "SatisfierPropertyKind", "String", false, 1, 1, false, "public", SatisfierProperty_SatisfierPropertyKind_dva);
		vector<string> ImplementationDependency_requiredType_dva;
		Uml::InitCORBAAttribute(ImplementationDependency::meta_requiredType, ImplementationDependency::meta, "requiredType", "String", false, 1, 1, false, "public", ImplementationDependency_requiredType_dva);
		vector<string> Property_DataValue_dva;
		Uml::InitCORBAAttribute(Property::meta_DataValue, Property::meta, "DataValue", "String", false, 1, 1, false, "public", Property_DataValue_dva);
		vector<string> RequirementSatisfier_resourceType_dva;
		Uml::InitCORBAAttribute(RequirementSatisfier::meta_resourceType, RequirementSatisfier::meta, "resourceType", "String", false, 1, 1, false, "public", RequirementSatisfier_resourceType_dva);
		vector<string> ImplementationRequirement_ResourceUsageKind_dva;
		ImplementationRequirement_ResourceUsageKind_dva.push_back("None");
		Uml::InitCORBAAttribute(ImplementationRequirement::meta_ResourceUsageKind, ImplementationRequirement::meta, "ResourceUsageKind", "String", false, 1, 1, false, "public", ImplementationRequirement_ResourceUsageKind_dva);
		vector<string> ImplementationRequirement_componentPort_dva;
		Uml::InitCORBAAttribute(ImplementationRequirement::meta_componentPort, ImplementationRequirement::meta, "componentPort", "String", false, 1, 1, false, "public", ImplementationRequirement_componentPort_dva);
		vector<string> ImplementationRequirement_resourcePort_dva;
		Uml::InitCORBAAttribute(ImplementationRequirement::meta_resourcePort, ImplementationRequirement::meta, "resourcePort", "String", false, 1, 1, false, "public", ImplementationRequirement_resourcePort_dva);
		vector<string> RequirementBase_resourceType_dva;
		Uml::InitCORBAAttribute(RequirementBase::meta_resourceType, RequirementBase::meta, "resourceType", "String", false, 1, 1, false, "public", RequirementBase_resourceType_dva);
		vector<string> Domain_label_dva;
		Uml::InitCORBAAttribute(Domain::meta_label, Domain::meta, "label", "String", false, 1, 1, false, "public", Domain_label_dva);
		vector<string> Domain_UUID_dva;
		Uml::InitCORBAAttribute(Domain::meta_UUID, Domain::meta, "UUID", "String", false, 1, 1, false, "public", Domain_UUID_dva);
		vector<string> Targets_name_dva;
		Uml::InitCORBAAttribute(Targets::meta_name, Targets::meta, "name", "String", false, 0, 1, false, "public", Targets_name_dva);
		vector<string> ImplementationArtifact_UUID_dva;
		Uml::InitCORBAAttribute(ImplementationArtifact::meta_UUID, ImplementationArtifact::meta, "UUID", "String", false, 1, 1, false, "public", ImplementationArtifact_UUID_dva);
		vector<string> ImplementationArtifact_label_dva;
		Uml::InitCORBAAttribute(ImplementationArtifact::meta_label, ImplementationArtifact::meta, "label", "String", false, 1, 1, false, "public", ImplementationArtifact_label_dva);
		vector<string> ImplementationArtifact_location_dva;
		Uml::InitCORBAAttribute(ImplementationArtifact::meta_location, ImplementationArtifact::meta, "location", "String", false, 1, 1, false, "public", ImplementationArtifact_location_dva);
		vector<string> ImplementationArtifacts_name_dva;
		Uml::InitCORBAAttribute(ImplementationArtifacts::meta_name, ImplementationArtifacts::meta, "name", "String", false, 0, 1, false, "public", ImplementationArtifacts_name_dva);
		vector<string> InterfaceDefinitions_name_dva;
		Uml::InitCORBAAttribute(InterfaceDefinitions::meta_name, InterfaceDefinitions::meta, "name", "String", false, 0, 1, false, "public", InterfaceDefinitions_name_dva);
		vector<string> Constant_value_dva;
		Uml::InitCORBAAttribute(Constant::meta_value, Constant::meta, "value", "String", false, 1, 1, false, "public", Constant_value_dva);
		vector<string> PredefinedTypes_name_dva;
		Uml::InitCORBAAttribute(PredefinedTypes::meta_name, PredefinedTypes::meta, "name", "String", false, 0, 1, false, "public", PredefinedTypes_name_dva);
		vector<string> Object_abstract_dva;
		Object_abstract_dva.push_back("false");
		Uml::InitCORBAAttribute(Object::meta_abstract, Object::meta, "abstract", "Boolean", false, 1, 1, false, "public", Object_abstract_dva);
		vector<string> Object_local_dva;
		Object_local_dva.push_back("false");
		Uml::InitCORBAAttribute(Object::meta_local, Object::meta, "local", "Boolean", false, 1, 1, false, "public", Object_local_dva);
		vector<string> ObjectByValue_abstract_dva;
		ObjectByValue_abstract_dva.push_back("false");
		Uml::InitCORBAAttribute(ObjectByValue::meta_abstract, ObjectByValue::meta, "abstract", "Boolean", false, 1, 1, false, "public", ObjectByValue_abstract_dva);
		vector<string> Component_UUID_dva;
		Uml::InitCORBAAttribute(Component::meta_UUID, Component::meta, "UUID", "String", false, 1, 1, false, "public", Component_UUID_dva);
		vector<string> Component_label_dva;
		Uml::InitCORBAAttribute(Component::meta_label, Component::meta, "label", "String", false, 1, 1, false, "public", Component_label_dva);
		vector<string> OutEventPort_single_destination_dva;
		OutEventPort_single_destination_dva.push_back("false");
		Uml::InitCORBAAttribute(OutEventPort::meta_single_destination, OutEventPort::meta, "single_destination", "Boolean", false, 1, 1, false, "public", OutEventPort_single_destination_dva);
		vector<string> RequiredRequestPort_multiple_connections_dva;
		RequiredRequestPort_multiple_connections_dva.push_back("false");
		Uml::InitCORBAAttribute(RequiredRequestPort::meta_multiple_connections, RequiredRequestPort::meta, "multiple_connections", "Boolean", false, 1, 1, false, "public", RequiredRequestPort_multiple_connections_dva);
		vector<string> Prefixable_PrefixTag_dva;
		Uml::InitCORBAAttribute(Prefixable::meta_PrefixTag, Prefixable::meta, "PrefixTag", "String", false, 1, 1, false, "public", Prefixable_PrefixTag_dva);
		vector<string> Taggable_VersionTag_dva;
		Uml::InitCORBAAttribute(Taggable::meta_VersionTag, Taggable::meta, "VersionTag", "String", false, 1, 1, false, "public", Taggable_VersionTag_dva);
		vector<string> Taggable_SpecifyIdTag_dva;
		Uml::InitCORBAAttribute(Taggable::meta_SpecifyIdTag, Taggable::meta, "SpecifyIdTag", "String", false, 1, 1, false, "public", Taggable_SpecifyIdTag_dva);
		vector<string> RootFolder_name_dva;
		Uml::InitCORBAAttribute(RootFolder::meta_name, RootFolder::meta, "name", "String", false, 0, 1, false, "public", RootFolder_name_dva);
		vector<string> MgaObject_position_dva;
		Uml::InitCORBAAttribute(MgaObject::meta_position, MgaObject::meta, "position", "String", false, 0, 1, false, "public", MgaObject_position_dva);
		vector<string> MgaObject_name_dva;
		Uml::InitCORBAAttribute(MgaObject::meta_name, MgaObject::meta, "name", "String", false, 0, 1, false, "public", MgaObject_name_dva);
		
		{
			Uml::Association ass = Uml::CreateCORBAAssociation();
			Uml::InitCORBAAssociation(ass , umldiagram, "package");
			Uml::InitCORBAAssociationClass(ass, package::meta);
			Uml::InitCORBAAssociationRole(PackageConfigurationReference::meta_srcpackage, ass, "srcpackage", true, false, 1, 1, TopLevelPackage::meta);
			package::meta_srcpackage_end_ = TopLevelPackage::meta_dstpackage_rev = PackageConfigurationReference::meta_srcpackage;
			Uml::InitCORBAAssociationRole(TopLevelPackage::meta_dstpackage, ass, "dstpackage", true, false, 1, 1, PackageConfigurationReference::meta);
			package::meta_dstpackage_end_ = PackageConfigurationReference::meta_srcpackage_rev = TopLevelPackage::meta_dstpackage;
		}
		{
			Uml::Association ass = Uml::CreateCORBAAssociation();
			Uml::InitCORBAAssociation(ass , umldiagram, "ComponentProperty");
			Uml::InitCORBAAssociationClass(ass, ComponentProperty::meta);
			Uml::InitCORBAAssociationRole(ComponentType::meta_dstComponentProperty, ass, "dstComponentProperty", true, false, 0, -1, ComponentPropertyDescription::meta);
			ComponentProperty::meta_dstComponentProperty_end_ = ComponentPropertyDescription::meta_srcComponentProperty_rev = ComponentType::meta_dstComponentProperty;
			Uml::InitCORBAAssociationRole(ComponentPropertyDescription::meta_srcComponentProperty, ass, "srcComponentProperty", true, false, 0, -1, ComponentType::meta);
			ComponentProperty::meta_srcComponentProperty_end_ = ComponentType::meta_dstComponentProperty_rev = ComponentPropertyDescription::meta_srcComponentProperty;
		}
		{
			Uml::Association ass = Uml::CreateCORBAAssociation();
			Uml::InitCORBAAssociation(ass , umldiagram, "ComponentConfigProperty");
			Uml::InitCORBAAssociationClass(ass, ComponentConfigProperty::meta);
			Uml::InitCORBAAssociationRole(ComponentType::meta_dstComponentConfigProperty, ass, "dstComponentConfigProperty", true, false, 0, -1, Property::meta);
			ComponentConfigProperty::meta_dstComponentConfigProperty_end_ = Property::meta_srcComponentConfigProperty_rev = ComponentType::meta_dstComponentConfigProperty;
			Uml::InitCORBAAssociationRole(Property::meta_srcComponentConfigProperty, ass, "srcComponentConfigProperty", true, false, 0, -1, ComponentType::meta);
			ComponentConfigProperty::meta_srcComponentConfigProperty_end_ = ComponentType::meta_dstComponentConfigProperty_rev = Property::meta_srcComponentConfigProperty;
		}
		{
			Uml::Association ass = Uml::CreateCORBAAssociation();
			Uml::InitCORBAAssociation(ass , umldiagram, "ComponentInfoProperty");
			Uml::InitCORBAAssociationClass(ass, ComponentInfoProperty::meta);
			Uml::InitCORBAAssociationRole(ComponentType::meta_dstComponentInfoProperty, ass, "dstComponentInfoProperty", true, false, 0, -1, Property::meta);
			ComponentInfoProperty::meta_dstComponentInfoProperty_end_ = Property::meta_srcComponentInfoProperty_rev = ComponentType::meta_dstComponentInfoProperty;
			Uml::InitCORBAAssociationRole(Property::meta_srcComponentInfoProperty, ass, "srcComponentInfoProperty", true, false, 0, -1, ComponentType::meta);
			ComponentInfoProperty::meta_srcComponentInfoProperty_end_ = ComponentType::meta_dstComponentInfoProperty_rev = Property::meta_srcComponentInfoProperty;
		}
		{
			Uml::Association ass = Uml::CreateCORBAAssociation();
			Uml::InitCORBAAssociation(ass , umldiagram, "");
			Uml::InitCORBAAssociationRole(ComponentType::meta_ref, ass, "ref", true, false, 0, 1, Component::meta);
			Uml::InitCORBAAssociationRole(Component::meta_referedbyComponentType, ass, "referedbyComponentType", true, false, 0, -1, ComponentType::meta);
		}
		{
			Uml::Association ass = Uml::CreateCORBAAssociation();
			Uml::InitCORBAAssociation(ass , umldiagram, "");
			Uml::InitCORBAAssociationRole(ComponentType::meta_CollocationGroup, ass, "CollocationGroup", true, false, 0, -1, CollocationGroup::meta);
			Uml::InitCORBAAssociationRole(CollocationGroup::meta_members, ass, "members", true, false, 0, -1, ComponentType::meta);
		}
		{
			Uml::Association ass = Uml::CreateCORBAAssociation();
			Uml::InitCORBAAssociation(ass , umldiagram, "Implements");
			Uml::InitCORBAAssociationClass(ass, Implements::meta);
			Uml::InitCORBAAssociationRole(ComponentType::meta_srcImplements, ass, "srcImplements", true, false, 0, -1, ComponentImplementation::meta);
			Implements::meta_srcImplements_end_ = ComponentImplementation::meta_dstImplements_rev = ComponentType::meta_srcImplements;
			Uml::InitCORBAAssociationRole(ComponentImplementation::meta_dstImplements, ass, "dstImplements", true, false, 0, -1, ComponentType::meta);
			Implements::meta_dstImplements_end_ = ComponentType::meta_srcImplements_rev = ComponentImplementation::meta_dstImplements;
		}
		{
			Uml::Association ass = Uml::CreateCORBAAssociation();
			Uml::InitCORBAAssociation(ass , umldiagram, "PackageInterface");
			Uml::InitCORBAAssociationClass(ass, PackageInterface::meta);
			Uml::InitCORBAAssociationRole(ComponentType::meta_srcPackageInterface, ass, "srcPackageInterface", true, false, 0, 1, ComponentPackage::meta);
			PackageInterface::meta_srcPackageInterface_end_ = ComponentPackage::meta_dstPackageInterface_rev = ComponentType::meta_srcPackageInterface;
			Uml::InitCORBAAssociationRole(ComponentPackage::meta_dstPackageInterface, ass, "dstPackageInterface", true, false, 0, 1, ComponentType::meta);
			PackageInterface::meta_dstPackageInterface_end_ = ComponentType::meta_srcPackageInterface_rev = ComponentPackage::meta_dstPackageInterface;
		}
		{
			Uml::Association ass = Uml::CreateCORBAAssociation();
			Uml::InitCORBAAssociation(ass , umldiagram, "Implementation");
			Uml::InitCORBAAssociationClass(ass, Implementation::meta);
			Uml::InitCORBAAssociationRole(ComponentImplementationReference::meta_srcImplementation, ass, "srcImplementation", true, false, 0, -1, ComponentPackage::meta);
			Implementation::meta_srcImplementation_end_ = ComponentPackage::meta_dstImplementation_rev = ComponentImplementationReference::meta_srcImplementation;
			Uml::InitCORBAAssociationRole(ComponentPackage::meta_dstImplementation, ass, "dstImplementation", true, false, 0, -1, ComponentImplementationReference::meta);
			Implementation::meta_dstImplementation_end_ = ComponentImplementationReference::meta_srcImplementation_rev = ComponentPackage::meta_dstImplementation;
		}
		{
			Uml::Association ass = Uml::CreateCORBAAssociation();
			Uml::InitCORBAAssociation(ass , umldiagram, "");
			Uml::InitCORBAAssociationRole(ComponentImplementationReference::meta_ref, ass, "ref", true, false, 0, 1, ComponentImplementation::meta);
			Uml::InitCORBAAssociationRole(ComponentImplementation::meta_referedbyComponentImplementationReference, ass, "referedbyComponentImplementationReference", true, false, 0, -1, ComponentImplementationReference::meta);
		}
		{
			Uml::Association ass = Uml::CreateCORBAAssociation();
			Uml::InitCORBAAssociation(ass , umldiagram, "InfoProperty");
			Uml::InitCORBAAssociationClass(ass, InfoProperty::meta);
			Uml::InitCORBAAssociationRole(ComponentImplementation::meta_dstInfoProperty, ass, "dstInfoProperty", true, false, 0, -1, Property::meta);
			InfoProperty::meta_dstInfoProperty_end_ = Property::meta_srcInfoProperty_rev = ComponentImplementation::meta_dstInfoProperty;
			Uml::InitCORBAAssociationRole(Property::meta_srcInfoProperty, ass, "srcInfoProperty", true, false, 0, -1, ComponentImplementation::meta);
			InfoProperty::meta_srcInfoProperty_end_ = ComponentImplementation::meta_dstInfoProperty_rev = Property::meta_srcInfoProperty;
		}
		{
			Uml::Association ass = Uml::CreateCORBAAssociation();
			Uml::InitCORBAAssociation(ass , umldiagram, "ImplementationCapability");
			Uml::InitCORBAAssociationClass(ass, ImplementationCapability::meta);
			Uml::InitCORBAAssociationRole(ComponentImplementation::meta_dstImplementationCapability, ass, "dstImplementationCapability", true, false, 0, -1, Capability::meta);
			ImplementationCapability::meta_dstImplementationCapability_end_ = Capability::meta_srcImplementationCapability_rev = ComponentImplementation::meta_dstImplementationCapability;
			Uml::InitCORBAAssociationRole(Capability::meta_srcImplementationCapability, ass, "srcImplementationCapability", true, false, 0, -1, ComponentImplementation::meta);
			ImplementationCapability::meta_srcImplementationCapability_end_ = ComponentImplementation::meta_dstImplementationCapability_rev = Capability::meta_srcImplementationCapability;
		}
		{
			Uml::Association ass = Uml::CreateCORBAAssociation();
			Uml::InitCORBAAssociation(ass , umldiagram, "ConfigProperty");
			Uml::InitCORBAAssociationClass(ass, ConfigProperty::meta);
			Uml::InitCORBAAssociationRole(ComponentImplementation::meta_dstConfigProperty, ass, "dstConfigProperty", true, false, 0, -1, Property::meta);
			ConfigProperty::meta_dstConfigProperty_end_ = Property::meta_srcConfigProperty_rev = ComponentImplementation::meta_dstConfigProperty;
			Uml::InitCORBAAssociationRole(Property::meta_srcConfigProperty, ass, "srcConfigProperty", true, false, 0, -1, ComponentImplementation::meta);
			ConfigProperty::meta_srcConfigProperty_end_ = ComponentImplementation::meta_dstConfigProperty_rev = Property::meta_srcConfigProperty;
		}
		{
			Uml::Association ass = Uml::CreateCORBAAssociation();
			Uml::InitCORBAAssociation(ass , umldiagram, "ImplementationDependsOn");
			Uml::InitCORBAAssociationClass(ass, ImplementationDependsOn::meta);
			Uml::InitCORBAAssociationRole(ComponentImplementation::meta_dstImplementationDependsOn, ass, "dstImplementationDependsOn", true, false, 0, -1, ImplementationDependency::meta);
			ImplementationDependsOn::meta_dstImplementationDependsOn_end_ = ImplementationDependency::meta_srcImplementationDependsOn_rev = ComponentImplementation::meta_dstImplementationDependsOn;
			Uml::InitCORBAAssociationRole(ImplementationDependency::meta_srcImplementationDependsOn, ass, "srcImplementationDependsOn", true, false, 0, -1, ComponentImplementation::meta);
			ImplementationDependsOn::meta_srcImplementationDependsOn_end_ = ComponentImplementation::meta_dstImplementationDependsOn_rev = ImplementationDependency::meta_srcImplementationDependsOn;
		}
		{
			Uml::Association ass = Uml::CreateCORBAAssociation();
			Uml::InitCORBAAssociation(ass , umldiagram, "MonolithExecParameter");
			Uml::InitCORBAAssociationClass(ass, MonolithExecParameter::meta);
			Uml::InitCORBAAssociationRole(MonolithicImplementation::meta_dstMonolithExecParameter, ass, "dstMonolithExecParameter", true, false, 0, -1, Property::meta);
			MonolithExecParameter::meta_dstMonolithExecParameter_end_ = Property::meta_srcMonolithExecParameter_rev = MonolithicImplementation::meta_dstMonolithExecParameter;
			Uml::InitCORBAAssociationRole(Property::meta_srcMonolithExecParameter, ass, "srcMonolithExecParameter", true, false, 0, -1, MonolithicImplementation::meta);
			MonolithExecParameter::meta_srcMonolithExecParameter_end_ = MonolithicImplementation::meta_dstMonolithExecParameter_rev = Property::meta_srcMonolithExecParameter;
		}
		{
			Uml::Association ass = Uml::CreateCORBAAssociation();
			Uml::InitCORBAAssociation(ass , umldiagram, "MonolithDeployRequirement");
			Uml::InitCORBAAssociationClass(ass, MonolithDeployRequirement::meta);
			Uml::InitCORBAAssociationRole(MonolithicImplementation::meta_dstMonolithDeployRequirement, ass, "dstMonolithDeployRequirement", true, false, 0, -1, ImplementationRequirement::meta);
			MonolithDeployRequirement::meta_dstMonolithDeployRequirement_end_ = ImplementationRequirement::meta_srcMonolithDeployRequirement_rev = MonolithicImplementation::meta_dstMonolithDeployRequirement;
			Uml::InitCORBAAssociationRole(ImplementationRequirement::meta_srcMonolithDeployRequirement, ass, "srcMonolithDeployRequirement", true, false, 0, -1, MonolithicImplementation::meta);
			MonolithDeployRequirement::meta_srcMonolithDeployRequirement_end_ = MonolithicImplementation::meta_dstMonolithDeployRequirement_rev = ImplementationRequirement::meta_srcMonolithDeployRequirement;
		}
		{
			Uml::Association ass = Uml::CreateCORBAAssociation();
			Uml::InitCORBAAssociation(ass , umldiagram, "MonolithprimaryArtifact");
			Uml::InitCORBAAssociationClass(ass, MonolithprimaryArtifact::meta);
			Uml::InitCORBAAssociationRole(MonolithicImplementation::meta_dstMonolithprimaryArtifact, ass, "dstMonolithprimaryArtifact", true, false, 0, -1, ImplementationArtifactReference::meta);
			MonolithprimaryArtifact::meta_dstMonolithprimaryArtifact_end_ = ImplementationArtifactReference::meta_srcMonolithprimaryArtifact_rev = MonolithicImplementation::meta_dstMonolithprimaryArtifact;
			Uml::InitCORBAAssociationRole(ImplementationArtifactReference::meta_srcMonolithprimaryArtifact, ass, "srcMonolithprimaryArtifact", true, false, 0, -1, MonolithicImplementation::meta);
			MonolithprimaryArtifact::meta_srcMonolithprimaryArtifact_end_ = MonolithicImplementation::meta_dstMonolithprimaryArtifact_rev = ImplementationArtifactReference::meta_srcMonolithprimaryArtifact;
		}
		{
			Uml::Association ass = Uml::CreateCORBAAssociation();
			Uml::InitCORBAAssociation(ass , umldiagram, "InstanceMapping");
			Uml::InitCORBAAssociationClass(ass, InstanceMapping::meta);
			Uml::InitCORBAAssociationRole(CollocationGroup::meta_dstInstanceMapping, ass, "dstInstanceMapping", true, false, 0, -1, NodeReference::meta);
			InstanceMapping::meta_dstInstanceMapping_end_ = NodeReference::meta_srcInstanceMapping_rev = CollocationGroup::meta_dstInstanceMapping;
			Uml::InitCORBAAssociationRole(NodeReference::meta_srcInstanceMapping, ass, "srcInstanceMapping", true, false, 0, -1, CollocationGroup::meta);
			InstanceMapping::meta_srcInstanceMapping_end_ = CollocationGroup::meta_dstInstanceMapping_rev = NodeReference::meta_srcInstanceMapping;
		}
		{
			Uml::Association ass = Uml::CreateCORBAAssociation();
			Uml::InitCORBAAssociation(ass , umldiagram, "");
			Uml::InitCORBAAssociationRole(PackageConfigurationReference::meta_ref, ass, "ref", true, false, 0, 1, PackageConfiguration::meta);
			Uml::InitCORBAAssociationRole(PackageConfiguration::meta_referedbyPackageConfigurationReference, ass, "referedbyPackageConfigurationReference", true, false, 0, -1, PackageConfigurationReference::meta);
		}
		{
			Uml::Association ass = Uml::CreateCORBAAssociation();
			Uml::InitCORBAAssociation(ass , umldiagram, "PackageConfSpecializedConfig");
			Uml::InitCORBAAssociationClass(ass, PackageConfSpecializedConfig::meta);
			Uml::InitCORBAAssociationRole(PackageConfigurationReference::meta_srcPackageConfSpecializedConfig, ass, "srcPackageConfSpecializedConfig", true, false, 0, -1, PackageConfiguration::meta);
			PackageConfSpecializedConfig::meta_srcPackageConfSpecializedConfig_end_ = PackageConfiguration::meta_dstPackageConfSpecializedConfig_rev = PackageConfigurationReference::meta_srcPackageConfSpecializedConfig;
			Uml::InitCORBAAssociationRole(PackageConfiguration::meta_dstPackageConfSpecializedConfig, ass, "dstPackageConfSpecializedConfig", true, false, 0, 1, PackageConfigurationReference::meta);
			PackageConfSpecializedConfig::meta_dstPackageConfSpecializedConfig_end_ = PackageConfigurationReference::meta_srcPackageConfSpecializedConfig_rev = PackageConfiguration::meta_dstPackageConfSpecializedConfig;
		}
		{
			Uml::Association ass = Uml::CreateCORBAAssociation();
			Uml::InitCORBAAssociation(ass , umldiagram, "PackageConfReference");
			Uml::InitCORBAAssociationClass(ass, PackageConfReference::meta);
			Uml::InitCORBAAssociationRole(PackageConfiguration::meta_dstPackageConfReference, ass, "dstPackageConfReference", true, false, 0, 1, ComponentPackageReference::meta);
			PackageConfReference::meta_dstPackageConfReference_end_ = ComponentPackageReference::meta_srcPackageConfReference_rev = PackageConfiguration::meta_dstPackageConfReference;
			Uml::InitCORBAAssociationRole(ComponentPackageReference::meta_srcPackageConfReference, ass, "srcPackageConfReference", true, false, 0, -1, PackageConfiguration::meta);
			PackageConfReference::meta_srcPackageConfReference_end_ = PackageConfiguration::meta_dstPackageConfReference_rev = ComponentPackageReference::meta_srcPackageConfReference;
		}
		{
			Uml::Association ass = Uml::CreateCORBAAssociation();
			Uml::InitCORBAAssociation(ass , umldiagram, "PackageConfBasePackage");
			Uml::InitCORBAAssociationClass(ass, PackageConfBasePackage::meta);
			Uml::InitCORBAAssociationRole(PackageConfiguration::meta_dstPackageConfBasePackage, ass, "dstPackageConfBasePackage", true, false, 0, 1, ComponentPackage::meta);
			PackageConfBasePackage::meta_dstPackageConfBasePackage_end_ = ComponentPackage::meta_srcPackageConfBasePackage_rev = PackageConfiguration::meta_dstPackageConfBasePackage;
			Uml::InitCORBAAssociationRole(ComponentPackage::meta_srcPackageConfBasePackage, ass, "srcPackageConfBasePackage", true, false, 0, -1, PackageConfiguration::meta);
			PackageConfBasePackage::meta_srcPackageConfBasePackage_end_ = PackageConfiguration::meta_dstPackageConfBasePackage_rev = ComponentPackage::meta_srcPackageConfBasePackage;
		}
		{
			Uml::Association ass = Uml::CreateCORBAAssociation();
			Uml::InitCORBAAssociation(ass , umldiagram, "PackageConfConfigProperty");
			Uml::InitCORBAAssociationClass(ass, PackageConfConfigProperty::meta);
			Uml::InitCORBAAssociationRole(PackageConfiguration::meta_dstPackageConfConfigProperty, ass, "dstPackageConfConfigProperty", true, false, 0, -1, Property::meta);
			PackageConfConfigProperty::meta_dstPackageConfConfigProperty_end_ = Property::meta_srcPackageConfConfigProperty_rev = PackageConfiguration::meta_dstPackageConfConfigProperty;
			Uml::InitCORBAAssociationRole(Property::meta_srcPackageConfConfigProperty, ass, "srcPackageConfConfigProperty", true, false, 0, -1, PackageConfiguration::meta);
			PackageConfConfigProperty::meta_srcPackageConfConfigProperty_end_ = PackageConfiguration::meta_dstPackageConfConfigProperty_rev = Property::meta_srcPackageConfConfigProperty;
		}
		{
			Uml::Association ass = Uml::CreateCORBAAssociation();
			Uml::InitCORBAAssociation(ass , umldiagram, "PackageConfSelectRequirement");
			Uml::InitCORBAAssociationClass(ass, PackageConfSelectRequirement::meta);
			Uml::InitCORBAAssociationRole(PackageConfiguration::meta_dstPackageConfSelectRequirement, ass, "dstPackageConfSelectRequirement", true, false, 0, -1, Requirement::meta);
			PackageConfSelectRequirement::meta_dstPackageConfSelectRequirement_end_ = Requirement::meta_srcPackageConfSelectRequirement_rev = PackageConfiguration::meta_dstPackageConfSelectRequirement;
			Uml::InitCORBAAssociationRole(Requirement::meta_srcPackageConfSelectRequirement, ass, "srcPackageConfSelectRequirement", true, false, 0, -1, PackageConfiguration::meta);
			PackageConfSelectRequirement::meta_srcPackageConfSelectRequirement_end_ = PackageConfiguration::meta_dstPackageConfSelectRequirement_rev = Requirement::meta_srcPackageConfSelectRequirement;
		}
		{
			Uml::Association ass = Uml::CreateCORBAAssociation();
			Uml::InitCORBAAssociation(ass , umldiagram, "PackageInfoProperty");
			Uml::InitCORBAAssociationClass(ass, PackageInfoProperty::meta);
			Uml::InitCORBAAssociationRole(ComponentPackage::meta_dstPackageInfoProperty, ass, "dstPackageInfoProperty", true, false, 0, -1, Property::meta);
			PackageInfoProperty::meta_dstPackageInfoProperty_end_ = Property::meta_srcPackageInfoProperty_rev = ComponentPackage::meta_dstPackageInfoProperty;
			Uml::InitCORBAAssociationRole(Property::meta_srcPackageInfoProperty, ass, "srcPackageInfoProperty", true, false, 0, -1, ComponentPackage::meta);
			PackageInfoProperty::meta_srcPackageInfoProperty_end_ = ComponentPackage::meta_dstPackageInfoProperty_rev = Property::meta_srcPackageInfoProperty;
		}
		{
			Uml::Association ass = Uml::CreateCORBAAssociation();
			Uml::InitCORBAAssociation(ass , umldiagram, "PackageConfigProperty");
			Uml::InitCORBAAssociationClass(ass, PackageConfigProperty::meta);
			Uml::InitCORBAAssociationRole(ComponentPackage::meta_dstPackageConfigProperty, ass, "dstPackageConfigProperty", true, false, 0, -1, Property::meta);
			PackageConfigProperty::meta_dstPackageConfigProperty_end_ = Property::meta_srcPackageConfigProperty_rev = ComponentPackage::meta_dstPackageConfigProperty;
			Uml::InitCORBAAssociationRole(Property::meta_srcPackageConfigProperty, ass, "srcPackageConfigProperty", true, false, 0, -1, ComponentPackage::meta);
			PackageConfigProperty::meta_srcPackageConfigProperty_end_ = ComponentPackage::meta_dstPackageConfigProperty_rev = Property::meta_srcPackageConfigProperty;
		}
		{
			Uml::Association ass = Uml::CreateCORBAAssociation();
			Uml::InitCORBAAssociation(ass , umldiagram, "");
			Uml::InitCORBAAssociationRole(ComponentPackage::meta_referedbyComponentPackageReference, ass, "referedbyComponentPackageReference", true, false, 0, -1, ComponentPackageReference::meta);
			Uml::InitCORBAAssociationRole(ComponentPackageReference::meta_ref, ass, "ref", true, false, 0, 1, ComponentPackage::meta);
		}
		{
			Uml::Association ass = Uml::CreateCORBAAssociation();
			Uml::InitCORBAAssociation(ass , umldiagram, "Node2Interconnect");
			Uml::InitCORBAAssociationClass(ass, Node2Interconnect::meta);
			Uml::InitCORBAAssociationRole(Node::meta_dstNode2Interconnect, ass, "dstNode2Interconnect", true, false, 0, -1, Interconnect::meta);
			Node2Interconnect::meta_dstNode2Interconnect_end_ = Interconnect::meta_srcNode2Interconnect_rev = Node::meta_dstNode2Interconnect;
			Uml::InitCORBAAssociationRole(Interconnect::meta_srcNode2Interconnect, ass, "srcNode2Interconnect", true, false, 1, -1, Node::meta);
			Node2Interconnect::meta_srcNode2Interconnect_end_ = Node::meta_dstNode2Interconnect_rev = Interconnect::meta_srcNode2Interconnect;
		}
		{
			Uml::Association ass = Uml::CreateCORBAAssociation();
			Uml::InitCORBAAssociation(ass , umldiagram, "Shares");
			Uml::InitCORBAAssociationClass(ass, Shares::meta);
			Uml::InitCORBAAssociationRole(Node::meta_dstShares, ass, "dstShares", true, false, 0, -1, SharedResource::meta);
			Shares::meta_dstShares_end_ = SharedResource::meta_srcShares_rev = Node::meta_dstShares;
			Uml::InitCORBAAssociationRole(SharedResource::meta_srcShares, ass, "srcShares", true, false, 1, -1, Node::meta);
			Shares::meta_srcShares_end_ = Node::meta_dstShares_rev = SharedResource::meta_srcShares;
		}
		{
			Uml::Association ass = Uml::CreateCORBAAssociation();
			Uml::InitCORBAAssociation(ass , umldiagram, "Interconnect2Node");
			Uml::InitCORBAAssociationClass(ass, Interconnect2Node::meta);
			Uml::InitCORBAAssociationRole(Node::meta_srcInterconnect2Node, ass, "srcInterconnect2Node", true, false, 0, -1, Interconnect::meta);
			Interconnect2Node::meta_srcInterconnect2Node_end_ = Interconnect::meta_dstInterconnect2Node_rev = Node::meta_srcInterconnect2Node;
			Uml::InitCORBAAssociationRole(Interconnect::meta_dstInterconnect2Node, ass, "dstInterconnect2Node", true, false, 1, -1, Node::meta);
			Interconnect2Node::meta_dstInterconnect2Node_end_ = Node::meta_srcInterconnect2Node_rev = Interconnect::meta_dstInterconnect2Node;
		}
		{
			Uml::Association ass = Uml::CreateCORBAAssociation();
			Uml::InitCORBAAssociation(ass , umldiagram, "");
			Uml::InitCORBAAssociationRole(Node::meta_referedbyNodeReference, ass, "referedbyNodeReference", true, false, 0, -1, NodeReference::meta);
			Uml::InitCORBAAssociationRole(NodeReference::meta_ref, ass, "ref", true, false, 0, 1, Node::meta);
		}
		{
			Uml::Association ass = Uml::CreateCORBAAssociation();
			Uml::InitCORBAAssociation(ass , umldiagram, "Interconnect2Bridge");
			Uml::InitCORBAAssociationClass(ass, Interconnect2Bridge::meta);
			Uml::InitCORBAAssociationRole(Interconnect::meta_dstInterconnect2Bridge, ass, "dstInterconnect2Bridge", true, false, 0, -1, Bridge::meta);
			Interconnect2Bridge::meta_dstInterconnect2Bridge_end_ = Bridge::meta_srcInterconnect2Bridge_rev = Interconnect::meta_dstInterconnect2Bridge;
			Uml::InitCORBAAssociationRole(Bridge::meta_srcInterconnect2Bridge, ass, "srcInterconnect2Bridge", true, false, 1, -1, Interconnect::meta);
			Interconnect2Bridge::meta_srcInterconnect2Bridge_end_ = Interconnect::meta_dstInterconnect2Bridge_rev = Bridge::meta_srcInterconnect2Bridge;
		}
		{
			Uml::Association ass = Uml::CreateCORBAAssociation();
			Uml::InitCORBAAssociation(ass , umldiagram, "Bridge2Interconnect");
			Uml::InitCORBAAssociationClass(ass, Bridge2Interconnect::meta);
			Uml::InitCORBAAssociationRole(Interconnect::meta_srcBridge2Interconnect, ass, "srcBridge2Interconnect", true, false, 0, -1, Bridge::meta);
			Bridge2Interconnect::meta_srcBridge2Interconnect_end_ = Bridge::meta_dstBridge2Interconnect_rev = Interconnect::meta_srcBridge2Interconnect;
			Uml::InitCORBAAssociationRole(Bridge::meta_dstBridge2Interconnect, ass, "dstBridge2Interconnect", true, false, 1, -1, Interconnect::meta);
			Bridge2Interconnect::meta_dstBridge2Interconnect_end_ = Interconnect::meta_srcBridge2Interconnect_rev = Bridge::meta_dstBridge2Interconnect;
		}
		{
			Uml::Association ass = Uml::CreateCORBAAssociation();
			Uml::InitCORBAAssociation(ass , umldiagram, "AssemblyDeployRequirement");
			Uml::InitCORBAAssociationClass(ass, AssemblyDeployRequirement::meta);
			Uml::InitCORBAAssociationRole(Requirement::meta_srcAssemblyDeployRequirement, ass, "srcAssemblyDeployRequirement", true, false, 0, -1, PublishConnector::meta);
			AssemblyDeployRequirement::meta_srcAssemblyDeployRequirement_end_ = PublishConnector::meta_dstAssemblyDeployRequirement_rev = Requirement::meta_srcAssemblyDeployRequirement;
			Uml::InitCORBAAssociationRole(PublishConnector::meta_dstAssemblyDeployRequirement, ass, "dstAssemblyDeployRequirement", true, false, 0, -1, Requirement::meta);
			AssemblyDeployRequirement::meta_dstAssemblyDeployRequirement_end_ = Requirement::meta_srcAssemblyDeployRequirement_rev = PublishConnector::meta_dstAssemblyDeployRequirement;
		}
		{
			Uml::Association ass = Uml::CreateCORBAAssociation();
			Uml::InitCORBAAssociation(ass , umldiagram, "ArtifactDeployRequirement");
			Uml::InitCORBAAssociationClass(ass, ArtifactDeployRequirement::meta);
			Uml::InitCORBAAssociationRole(Requirement::meta_srcArtifactDeployRequirement, ass, "srcArtifactDeployRequirement", true, false, 0, -1, ImplementationArtifact::meta);
			ArtifactDeployRequirement::meta_srcArtifactDeployRequirement_end_ = ImplementationArtifact::meta_dstArtifactDeployRequirement_rev = Requirement::meta_srcArtifactDeployRequirement;
			Uml::InitCORBAAssociationRole(ImplementationArtifact::meta_dstArtifactDeployRequirement, ass, "dstArtifactDeployRequirement", true, false, 0, -1, Requirement::meta);
			ArtifactDeployRequirement::meta_dstArtifactDeployRequirement_end_ = Requirement::meta_srcArtifactDeployRequirement_rev = ImplementationArtifact::meta_dstArtifactDeployRequirement;
		}
		{
			Uml::Association ass = Uml::CreateCORBAAssociation();
			Uml::InitCORBAAssociation(ass , umldiagram, "AssemblyselectRequirement");
			Uml::InitCORBAAssociationClass(ass, AssemblyselectRequirement::meta);
			Uml::InitCORBAAssociationRole(Requirement::meta_srcAssemblyselectRequirement, ass, "srcAssemblyselectRequirement", true, false, 0, -1, Component::meta);
			AssemblyselectRequirement::meta_srcAssemblyselectRequirement_end_ = Component::meta_dstAssemblyselectRequirement_rev = Requirement::meta_srcAssemblyselectRequirement;
			Uml::InitCORBAAssociationRole(Component::meta_dstAssemblyselectRequirement, ass, "dstAssemblyselectRequirement", true, false, 0, -1, Requirement::meta);
			AssemblyselectRequirement::meta_dstAssemblyselectRequirement_end_ = Requirement::meta_srcAssemblyselectRequirement_rev = Component::meta_dstAssemblyselectRequirement;
		}
		{
			Uml::Association ass = Uml::CreateCORBAAssociation();
			Uml::InitCORBAAssociation(ass , umldiagram, "");
			Uml::InitCORBAAssociationRole(PredefinedType::meta_referedbyDataType, ass, "referedbyDataType", true, false, 0, -1, DataType::meta);
			Uml::InitCORBAAssociationRole(DataType::meta_ref, ass, "ref", true, false, 0, 1, PredefinedType::meta);
		}
		{
			Uml::Association ass = Uml::CreateCORBAAssociation();
			Uml::InitCORBAAssociation(ass , umldiagram, "ArtifactExecParameter");
			Uml::InitCORBAAssociationClass(ass, ArtifactExecParameter::meta);
			Uml::InitCORBAAssociationRole(Property::meta_srcArtifactExecParameter, ass, "srcArtifactExecParameter", true, false, 0, -1, ImplementationArtifact::meta);
			ArtifactExecParameter::meta_srcArtifactExecParameter_end_ = ImplementationArtifact::meta_dstArtifactExecParameter_rev = Property::meta_srcArtifactExecParameter;
			Uml::InitCORBAAssociationRole(ImplementationArtifact::meta_dstArtifactExecParameter, ass, "dstArtifactExecParameter", true, false, 0, -1, Property::meta);
			ArtifactExecParameter::meta_dstArtifactExecParameter_end_ = Property::meta_srcArtifactExecParameter_rev = ImplementationArtifact::meta_dstArtifactExecParameter;
		}
		{
			Uml::Association ass = Uml::CreateCORBAAssociation();
			Uml::InitCORBAAssociation(ass , umldiagram, "ArtifactInfoProperty");
			Uml::InitCORBAAssociationClass(ass, ArtifactInfoProperty::meta);
			Uml::InitCORBAAssociationRole(Property::meta_srcArtifactInfoProperty, ass, "srcArtifactInfoProperty", true, false, 0, -1, ImplementationArtifact::meta);
			ArtifactInfoProperty::meta_srcArtifactInfoProperty_end_ = ImplementationArtifact::meta_dstArtifactInfoProperty_rev = Property::meta_srcArtifactInfoProperty;
			Uml::InitCORBAAssociationRole(ImplementationArtifact::meta_dstArtifactInfoProperty, ass, "dstArtifactInfoProperty", true, false, 0, -1, Property::meta);
			ArtifactInfoProperty::meta_dstArtifactInfoProperty_end_ = Property::meta_srcArtifactInfoProperty_rev = ImplementationArtifact::meta_dstArtifactInfoProperty;
		}
		{
			Uml::Association ass = Uml::CreateCORBAAssociation();
			Uml::InitCORBAAssociation(ass , umldiagram, "AssemblyConfigProperty");
			Uml::InitCORBAAssociationClass(ass, AssemblyConfigProperty::meta);
			Uml::InitCORBAAssociationRole(Property::meta_srcAssemblyConfigProperty, ass, "srcAssemblyConfigProperty", true, false, 0, -1, Component::meta);
			AssemblyConfigProperty::meta_srcAssemblyConfigProperty_end_ = Component::meta_dstAssemblyConfigProperty_rev = Property::meta_srcAssemblyConfigProperty;
			Uml::InitCORBAAssociationRole(Component::meta_dstAssemblyConfigProperty, ass, "dstAssemblyConfigProperty", true, false, 0, -1, Property::meta);
			AssemblyConfigProperty::meta_dstAssemblyConfigProperty_end_ = Property::meta_srcAssemblyConfigProperty_rev = Component::meta_dstAssemblyConfigProperty;
		}
		{
			Uml::Association ass = Uml::CreateCORBAAssociation();
			Uml::InitCORBAAssociation(ass , umldiagram, "deliverTo");
			Uml::InitCORBAAssociationClass(ass, deliverTo::meta);
			Uml::InitCORBAAssociationRole(PublishConnector::meta_dstdeliverTo, ass, "dstdeliverTo", true, false, 0, -1, InEventPort::meta);
			deliverTo::meta_dstdeliverTo_end_ = InEventPort::meta_srcdeliverTo_rev = PublishConnector::meta_dstdeliverTo;
			Uml::InitCORBAAssociationRole(InEventPort::meta_srcdeliverTo, ass, "srcdeliverTo", true, false, 0, -1, PublishConnector::meta);
			deliverTo::meta_srcdeliverTo_end_ = PublishConnector::meta_dstdeliverTo_rev = InEventPort::meta_srcdeliverTo;
		}
		{
			Uml::Association ass = Uml::CreateCORBAAssociation();
			Uml::InitCORBAAssociation(ass , umldiagram, "publish");
			Uml::InitCORBAAssociationClass(ass, publish::meta);
			Uml::InitCORBAAssociationRole(PublishConnector::meta_srcpublish, ass, "srcpublish", true, false, 0, -1, OutEventPort::meta);
			publish::meta_srcpublish_end_ = OutEventPort::meta_dstpublish_rev = PublishConnector::meta_srcpublish;
			Uml::InitCORBAAssociationRole(OutEventPort::meta_dstpublish, ass, "dstpublish", true, false, 0, -1, PublishConnector::meta);
			publish::meta_dstpublish_end_ = PublishConnector::meta_srcpublish_rev = OutEventPort::meta_dstpublish;
		}
		{
			Uml::Association ass = Uml::CreateCORBAAssociation();
			Uml::InitCORBAAssociation(ass , umldiagram, "");
			Uml::InitCORBAAssociationRole(ImplementationArtifactReference::meta_ref, ass, "ref", true, false, 0, 1, ImplementationArtifact::meta);
			Uml::InitCORBAAssociationRole(ImplementationArtifact::meta_referedbyImplementationArtifactReference, ass, "referedbyImplementationArtifactReference", true, false, 0, -1, ImplementationArtifactReference::meta);
		}
		{
			Uml::Association ass = Uml::CreateCORBAAssociation();
			Uml::InitCORBAAssociation(ass , umldiagram, "ArtifactDependsOn");
			Uml::InitCORBAAssociationClass(ass, ArtifactDependsOn::meta);
			Uml::InitCORBAAssociationRole(ImplementationArtifactReference::meta_srcArtifactDependsOn, ass, "srcArtifactDependsOn", true, false, 0, -1, ImplementationArtifact::meta);
			ArtifactDependsOn::meta_srcArtifactDependsOn_end_ = ImplementationArtifact::meta_dstArtifactDependsOn_rev = ImplementationArtifactReference::meta_srcArtifactDependsOn;
			Uml::InitCORBAAssociationRole(ImplementationArtifact::meta_dstArtifactDependsOn, ass, "dstArtifactDependsOn", true, false, 0, -1, ImplementationArtifactReference::meta);
			ArtifactDependsOn::meta_dstArtifactDependsOn_end_ = ImplementationArtifactReference::meta_srcArtifactDependsOn_rev = ImplementationArtifact::meta_dstArtifactDependsOn;
		}
		{
			Uml::Association ass = Uml::CreateCORBAAssociation();
			Uml::InitCORBAAssociation(ass , umldiagram, "");
			Uml::InitCORBAAssociationRole(File::meta_referedbyFileRef, ass, "referedbyFileRef", true, false, 0, -1, FileRef::meta);
			Uml::InitCORBAAssociationRole(FileRef::meta_ref, ass, "ref", true, false, 0, 1, File::meta);
		}
		{
			Uml::Association ass = Uml::CreateCORBAAssociation();
			Uml::InitCORBAAssociation(ass , umldiagram, "");
			Uml::InitCORBAAssociationRole(Exception::meta_referedbySetException, ass, "referedbySetException", true, false, 0, -1, SetException::meta);
			Uml::InitCORBAAssociationRole(SetException::meta_ref, ass, "ref", true, false, 0, 1, Exception::meta);
		}
		{
			Uml::Association ass = Uml::CreateCORBAAssociation();
			Uml::InitCORBAAssociation(ass , umldiagram, "");
			Uml::InitCORBAAssociationRole(Exception::meta_referedbyExceptionRef, ass, "referedbyExceptionRef", true, false, 0, -1, ExceptionRef::meta);
			Uml::InitCORBAAssociationRole(ExceptionRef::meta_ref, ass, "ref", true, false, 0, 1, Exception::meta);
		}
		{
			Uml::Association ass = Uml::CreateCORBAAssociation();
			Uml::InitCORBAAssociation(ass , umldiagram, "");
			Uml::InitCORBAAssociationRole(Exception::meta_referedbyGetException, ass, "referedbyGetException", true, false, 0, -1, GetException::meta);
			Uml::InitCORBAAssociationRole(GetException::meta_ref, ass, "ref", true, false, 0, 1, Exception::meta);
		}
		{
			Uml::Association ass = Uml::CreateCORBAAssociation();
			Uml::InitCORBAAssociation(ass , umldiagram, "");
			Uml::InitCORBAAssociationRole(ConstantType::meta_referedbyDiscriminator, ass, "referedbyDiscriminator", true, false, 0, -1, Discriminator::meta);
			Uml::InitCORBAAssociationRole(Discriminator::meta_ref, ass, "ref", true, false, 0, 1, ConstantType::meta);
		}
		{
			Uml::Association ass = Uml::CreateCORBAAssociation();
			Uml::InitCORBAAssociation(ass , umldiagram, "");
			Uml::InitCORBAAssociationRole(ConstantType::meta_referedbyConstant, ass, "referedbyConstant", true, false, 0, -1, Constant::meta);
			Uml::InitCORBAAssociationRole(Constant::meta_ref, ass, "ref", true, false, 0, 1, ConstantType::meta);
		}
		{
			Uml::Association ass = Uml::CreateCORBAAssociation();
			Uml::InitCORBAAssociation(ass , umldiagram, "LabelConnection");
			Uml::InitCORBAAssociationClass(ass, LabelConnection::meta);
			Uml::InitCORBAAssociationRole(Label::meta_srcLabelConnection, ass, "srcLabelConnection", true, false, 1, 1, Member::meta);
			LabelConnection::meta_srcLabelConnection_end_ = Member::meta_dstLabelConnection_rev = Label::meta_srcLabelConnection;
			Uml::InitCORBAAssociationRole(Member::meta_dstLabelConnection, ass, "dstLabelConnection", true, false, 0, -1, Label::meta);
			LabelConnection::meta_dstLabelConnection_end_ = Label::meta_srcLabelConnection_rev = Member::meta_dstLabelConnection;
		}
		{
			Uml::Association ass = Uml::CreateCORBAAssociation();
			Uml::InitCORBAAssociation(ass , umldiagram, "");
			Uml::InitCORBAAssociationRole(MemberType::meta_referedbyAlias, ass, "referedbyAlias", true, false, 0, -1, Alias::meta);
			Uml::InitCORBAAssociationRole(Alias::meta_ref, ass, "ref", true, false, 0, 1, MemberType::meta);
		}
		{
			Uml::Association ass = Uml::CreateCORBAAssociation();
			Uml::InitCORBAAssociation(ass , umldiagram, "");
			Uml::InitCORBAAssociationRole(MemberType::meta_referedbyCollection, ass, "referedbyCollection", true, false, 0, -1, Collection::meta);
			Uml::InitCORBAAssociationRole(Collection::meta_ref, ass, "ref", true, false, 0, 1, MemberType::meta);
		}
		{
			Uml::Association ass = Uml::CreateCORBAAssociation();
			Uml::InitCORBAAssociation(ass , umldiagram, "");
			Uml::InitCORBAAssociationRole(MemberType::meta_referedbyBoxed, ass, "referedbyBoxed", true, false, 0, -1, Boxed::meta);
			Uml::InitCORBAAssociationRole(Boxed::meta_ref, ass, "ref", true, false, 0, 1, MemberType::meta);
		}
		{
			Uml::Association ass = Uml::CreateCORBAAssociation();
			Uml::InitCORBAAssociation(ass , umldiagram, "");
			Uml::InitCORBAAssociationRole(MemberType::meta_referedbyMember, ass, "referedbyMember", true, false, 0, -1, Member::meta);
			Uml::InitCORBAAssociationRole(Member::meta_ref, ass, "ref", true, false, 0, 1, MemberType::meta);
		}
		{
			Uml::Association ass = Uml::CreateCORBAAssociation();
			Uml::InitCORBAAssociation(ass , umldiagram, "");
			Uml::InitCORBAAssociationRole(MemberType::meta_referedbyAttributeMember, ass, "referedbyAttributeMember", true, false, 0, -1, AttributeMember::meta);
			Uml::InitCORBAAssociationRole(AttributeMember::meta_ref, ass, "ref", true, false, 0, 1, MemberType::meta);
		}
		{
			Uml::Association ass = Uml::CreateCORBAAssociation();
			Uml::InitCORBAAssociation(ass , umldiagram, "");
			Uml::InitCORBAAssociationRole(MemberType::meta_referedbyInParameter, ass, "referedbyInParameter", true, false, 0, -1, InParameter::meta);
			Uml::InitCORBAAssociationRole(InParameter::meta_ref, ass, "ref", true, false, 0, 1, MemberType::meta);
		}
		{
			Uml::Association ass = Uml::CreateCORBAAssociation();
			Uml::InitCORBAAssociation(ass , umldiagram, "");
			Uml::InitCORBAAssociationRole(MemberType::meta_referedbyOutParameter, ass, "referedbyOutParameter", true, false, 0, -1, OutParameter::meta);
			Uml::InitCORBAAssociationRole(OutParameter::meta_ref, ass, "ref", true, false, 0, 1, MemberType::meta);
		}
		{
			Uml::Association ass = Uml::CreateCORBAAssociation();
			Uml::InitCORBAAssociation(ass , umldiagram, "");
			Uml::InitCORBAAssociationRole(MemberType::meta_referedbyReturnType, ass, "referedbyReturnType", true, false, 0, -1, ReturnType::meta);
			Uml::InitCORBAAssociationRole(ReturnType::meta_ref, ass, "ref", true, false, 0, 1, MemberType::meta);
		}
		{
			Uml::Association ass = Uml::CreateCORBAAssociation();
			Uml::InitCORBAAssociation(ass , umldiagram, "");
			Uml::InitCORBAAssociationRole(MemberType::meta_referedbyInoutParameter, ass, "referedbyInoutParameter", true, false, 0, -1, InoutParameter::meta);
			Uml::InitCORBAAssociationRole(InoutParameter::meta_ref, ass, "ref", true, false, 0, 1, MemberType::meta);
		}
		{
			Uml::Association ass = Uml::CreateCORBAAssociation();
			Uml::InitCORBAAssociation(ass , umldiagram, "MakeMemberPrivate");
			Uml::InitCORBAAssociationClass(ass, MakeMemberPrivate::meta);
			Uml::InitCORBAAssociationRole(Member::meta_dstMakeMemberPrivate, ass, "dstMakeMemberPrivate", true, false, 0, 1, PrivateFlag::meta);
			MakeMemberPrivate::meta_dstMakeMemberPrivate_end_ = PrivateFlag::meta_srcMakeMemberPrivate_rev = Member::meta_dstMakeMemberPrivate;
			Uml::InitCORBAAssociationRole(PrivateFlag::meta_srcMakeMemberPrivate, ass, "srcMakeMemberPrivate", true, false, 1, 1, Member::meta);
			MakeMemberPrivate::meta_srcMakeMemberPrivate_end_ = Member::meta_dstMakeMemberPrivate_rev = PrivateFlag::meta_srcMakeMemberPrivate;
		}
		{
			Uml::Association ass = Uml::CreateCORBAAssociation();
			Uml::InitCORBAAssociation(ass , umldiagram, "");
			Uml::InitCORBAAssociationRole(Object::meta_referedbySupports, ass, "referedbySupports", true, false, 0, -1, Supports::meta);
			Uml::InitCORBAAssociationRole(Supports::meta_ref, ass, "ref", true, false, 0, 1, Object::meta);
		}
		{
			Uml::Association ass = Uml::CreateCORBAAssociation();
			Uml::InitCORBAAssociation(ass , umldiagram, "");
			Uml::InitCORBAAssociationRole(Inheritable::meta_referedbyInherits, ass, "referedbyInherits", true, false, 0, -1, Inherits::meta);
			Uml::InitCORBAAssociationRole(Inherits::meta_ref, ass, "ref", true, false, 0, 1, Inheritable::meta);
		}
		{
			Uml::Association ass = Uml::CreateCORBAAssociation();
			Uml::InitCORBAAssociation(ass , umldiagram, "");
			Uml::InitCORBAAssociationRole(Event::meta_referedbyInEventPort, ass, "referedbyInEventPort", true, false, 0, -1, InEventPort::meta);
			Uml::InitCORBAAssociationRole(InEventPort::meta_ref, ass, "ref", true, false, 0, 1, Event::meta);
		}
		{
			Uml::Association ass = Uml::CreateCORBAAssociation();
			Uml::InitCORBAAssociation(ass , umldiagram, "");
			Uml::InitCORBAAssociationRole(Event::meta_referedbyOutEventPort, ass, "referedbyOutEventPort", true, false, 0, -1, OutEventPort::meta);
			Uml::InitCORBAAssociationRole(OutEventPort::meta_ref, ass, "ref", true, false, 0, 1, Event::meta);
		}
		{
			Uml::Association ass = Uml::CreateCORBAAssociation();
			Uml::InitCORBAAssociation(ass , umldiagram, "");
			Uml::InitCORBAAssociationRole(ValueObject::meta_referedbyLookupKey, ass, "referedbyLookupKey", true, false, 0, -1, LookupKey::meta);
			Uml::InitCORBAAssociationRole(LookupKey::meta_ref, ass, "ref", true, false, 0, 1, ValueObject::meta);
		}
		{
			Uml::Association ass = Uml::CreateCORBAAssociation();
			Uml::InitCORBAAssociation(ass , umldiagram, "");
			Uml::InitCORBAAssociationRole(Component::meta_referedbyComponentRef, ass, "referedbyComponentRef", true, false, 0, -1, ComponentRef::meta);
			Uml::InitCORBAAssociationRole(ComponentRef::meta_ref, ass, "ref", true, false, 0, 1, Component::meta);
		}
		{
			Uml::Association ass = Uml::CreateCORBAAssociation();
			Uml::InitCORBAAssociation(ass , umldiagram, "emit");
			Uml::InitCORBAAssociationClass(ass, emit::meta);
			Uml::InitCORBAAssociationRole(OutEventPort::meta_dstemit, ass, "dstemit", true, false, 0, 1, InEventPort::meta);
			emit::meta_dstemit_end_ = InEventPort::meta_srcemit_rev = OutEventPort::meta_dstemit;
			Uml::InitCORBAAssociationRole(InEventPort::meta_srcemit, ass, "srcemit", true, false, 0, 1, OutEventPort::meta);
			emit::meta_srcemit_end_ = OutEventPort::meta_dstemit_rev = InEventPort::meta_srcemit;
		}
		{
			Uml::Association ass = Uml::CreateCORBAAssociation();
			Uml::InitCORBAAssociation(ass , umldiagram, "invoke");
			Uml::InitCORBAAssociationClass(ass, invoke::meta);
			Uml::InitCORBAAssociationRole(ProvidedRequestPort::meta_srcinvoke, ass, "srcinvoke", true, false, 0, -1, RequiredRequestPort::meta);
			invoke::meta_srcinvoke_end_ = RequiredRequestPort::meta_dstinvoke_rev = ProvidedRequestPort::meta_srcinvoke;
			Uml::InitCORBAAssociationRole(RequiredRequestPort::meta_dstinvoke, ass, "dstinvoke", true, false, 0, -1, ProvidedRequestPort::meta);
			invoke::meta_dstinvoke_end_ = ProvidedRequestPort::meta_srcinvoke_rev = RequiredRequestPort::meta_dstinvoke;
		}
		{
			Uml::Association ass = Uml::CreateCORBAAssociation();
			Uml::InitCORBAAssociation(ass , umldiagram, "");
			Uml::InitCORBAAssociationRole(ProvidedRequestPort::meta_ref, ass, "ref", true, false, 0, 1, Provideable::meta);
			Uml::InitCORBAAssociationRole(Provideable::meta_referedbyProvidedRequestPort, ass, "referedbyProvidedRequestPort", true, false, 0, -1, ProvidedRequestPort::meta);
		}
		{
			Uml::Association ass = Uml::CreateCORBAAssociation();
			Uml::InitCORBAAssociation(ass , umldiagram, "");
			Uml::InitCORBAAssociationRole(RequiredRequestPort::meta_ref, ass, "ref", true, false, 0, 1, Provideable::meta);
			Uml::InitCORBAAssociationRole(Provideable::meta_referedbyRequiredRequestPort, ass, "referedbyRequiredRequestPort", true, false, 0, -1, RequiredRequestPort::meta);
		}
		{
			Uml::Association ass = Uml::CreateCORBAAssociation();
			Uml::InitCORBAAssociation(ass , umldiagram, "ManagesComponent");
			Uml::InitCORBAAssociationClass(ass, ManagesComponent::meta);
			Uml::InitCORBAAssociationRole(Manageable::meta_srcManagesComponent, ass, "srcManagesComponent", true, false, 0, -1, ComponentFactory::meta);
			ManagesComponent::meta_srcManagesComponent_end_ = ComponentFactory::meta_dstManagesComponent_rev = Manageable::meta_srcManagesComponent;
			Uml::InitCORBAAssociationRole(ComponentFactory::meta_dstManagesComponent, ass, "dstManagesComponent", true, false, 1, 1, Manageable::meta);
			ManagesComponent::meta_dstManagesComponent_end_ = Manageable::meta_srcManagesComponent_rev = ComponentFactory::meta_dstManagesComponent;
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(TopLevelPackage::meta_TopLevelPackageContainer_parent, comp, "TopLevelPackageContainer_parent", true, TopLevelPackageContainer::meta);
			Uml::InitCORBACompositionChildRole(TopLevelPackageContainer::meta_TopLevelPackage_child, comp, "TopLevelPackage", true, 0, 1, TopLevelPackage::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(PackageConfigurationReference::meta_TopLevelPackageContainer_parent, comp, "TopLevelPackageContainer_parent", true, TopLevelPackageContainer::meta);
			Uml::InitCORBACompositionChildRole(TopLevelPackageContainer::meta_PackageConfigurationReference_child, comp, "PackageConfigurationReference", true, 1, 1, PackageConfigurationReference::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(package::meta_TopLevelPackageContainer_parent, comp, "TopLevelPackageContainer_parent", true, TopLevelPackageContainer::meta);
			Uml::InitCORBACompositionChildRole(TopLevelPackageContainer::meta_package_child, comp, "package", true, 0, 1, package::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(TopLevelPackageContainer::meta_TopLevelPackages_parent, comp, "TopLevelPackages_parent", true, TopLevelPackages::meta);
			Uml::InitCORBACompositionChildRole(TopLevelPackages::meta_TopLevelPackageContainer_children, comp, "TopLevelPackageContainer", true, 0, -1, TopLevelPackageContainer::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(ComponentConfigProperty::meta_ComponentContainer_parent, comp, "ComponentContainer_parent", true, ComponentContainer::meta);
			Uml::InitCORBACompositionChildRole(ComponentContainer::meta_ComponentConfigProperty_children, comp, "ComponentConfigProperty", true, 0, -1, ComponentConfigProperty::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(ComponentProperty::meta_ComponentContainer_parent, comp, "ComponentContainer_parent", true, ComponentContainer::meta);
			Uml::InitCORBACompositionChildRole(ComponentContainer::meta_ComponentProperty_children, comp, "ComponentProperty", true, 0, -1, ComponentProperty::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(ComponentInfoProperty::meta_ComponentContainer_parent, comp, "ComponentContainer_parent", true, ComponentContainer::meta);
			Uml::InitCORBACompositionChildRole(ComponentContainer::meta_ComponentInfoProperty_children, comp, "ComponentInfoProperty", true, 0, -1, ComponentInfoProperty::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(Property::meta_ComponentContainer_parent, comp, "ComponentContainer_parent", true, ComponentContainer::meta);
			Uml::InitCORBACompositionChildRole(ComponentContainer::meta_Property_children, comp, "Property", true, 0, -1, Property::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(ComponentType::meta_ComponentContainer_parent, comp, "ComponentContainer_parent", true, ComponentContainer::meta);
			Uml::InitCORBACompositionChildRole(ComponentContainer::meta_ComponentType_child, comp, "ComponentType", true, 0, 1, ComponentType::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(ComponentPropertyDescription::meta_ComponentContainer_parent, comp, "ComponentContainer_parent", true, ComponentContainer::meta);
			Uml::InitCORBACompositionChildRole(ComponentContainer::meta_ComponentPropertyDescription_children, comp, "ComponentPropertyDescription", true, 0, -1, ComponentPropertyDescription::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(ComponentContainer::meta_ComponentTypes_parent, comp, "ComponentTypes_parent", true, ComponentTypes::meta);
			Uml::InitCORBACompositionChildRole(ComponentTypes::meta_ComponentContainer_children, comp, "ComponentContainer", true, 0, -1, ComponentContainer::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(DataType::meta_ComponentPropertyDescription_parent, comp, "ComponentPropertyDescription_parent", true, ComponentPropertyDescription::meta);
			Uml::InitCORBACompositionChildRole(ComponentPropertyDescription::meta_DataType_child, comp, "DataType", true, 1, 1, DataType::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(ComponentImplementationContainer::meta_ComponentImplementations_parent, comp, "ComponentImplementations_parent", true, ComponentImplementations::meta);
			Uml::InitCORBACompositionChildRole(ComponentImplementations::meta_ComponentImplementationContainer_children, comp, "ComponentImplementationContainer", true, 0, -1, ComponentImplementationContainer::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(ComponentType::meta_ComponentImplementationContainer_parent, comp, "ComponentImplementationContainer_parent", true, ComponentImplementationContainer::meta);
			Uml::InitCORBACompositionChildRole(ComponentImplementationContainer::meta_ComponentType_children, comp, "ComponentType", true, 0, -1, ComponentType::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(ConfigProperty::meta_ComponentImplementationContainer_parent, comp, "ComponentImplementationContainer_parent", true, ComponentImplementationContainer::meta);
			Uml::InitCORBACompositionChildRole(ComponentImplementationContainer::meta_ConfigProperty_children, comp, "ConfigProperty", true, 0, -1, ConfigProperty::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(MonolithprimaryArtifact::meta_ComponentImplementationContainer_parent, comp, "ComponentImplementationContainer_parent", true, ComponentImplementationContainer::meta);
			Uml::InitCORBACompositionChildRole(ComponentImplementationContainer::meta_MonolithprimaryArtifact_children, comp, "MonolithprimaryArtifact", true, 0, -1, MonolithprimaryArtifact::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(Property::meta_ComponentImplementationContainer_parent, comp, "ComponentImplementationContainer_parent", true, ComponentImplementationContainer::meta);
			Uml::InitCORBACompositionChildRole(ComponentImplementationContainer::meta_Property_children, comp, "Property", true, 0, -1, Property::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(ImplementationCapability::meta_ComponentImplementationContainer_parent, comp, "ComponentImplementationContainer_parent", true, ComponentImplementationContainer::meta);
			Uml::InitCORBACompositionChildRole(ComponentImplementationContainer::meta_ImplementationCapability_children, comp, "ImplementationCapability", true, 0, -1, ImplementationCapability::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(ImplementationDependsOn::meta_ComponentImplementationContainer_parent, comp, "ComponentImplementationContainer_parent", true, ComponentImplementationContainer::meta);
			Uml::InitCORBACompositionChildRole(ComponentImplementationContainer::meta_ImplementationDependsOn_children, comp, "ImplementationDependsOn", true, 0, -1, ImplementationDependsOn::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(Implements::meta_ComponentImplementationContainer_parent, comp, "ComponentImplementationContainer_parent", true, ComponentImplementationContainer::meta);
			Uml::InitCORBACompositionChildRole(ComponentImplementationContainer::meta_Implements_children, comp, "Implements", true, 0, -1, Implements::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(MonolithDeployRequirement::meta_ComponentImplementationContainer_parent, comp, "ComponentImplementationContainer_parent", true, ComponentImplementationContainer::meta);
			Uml::InitCORBACompositionChildRole(ComponentImplementationContainer::meta_MonolithDeployRequirement_children, comp, "MonolithDeployRequirement", true, 0, -1, MonolithDeployRequirement::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(ImplementationRequirement::meta_ComponentImplementationContainer_parent, comp, "ComponentImplementationContainer_parent", true, ComponentImplementationContainer::meta);
			Uml::InitCORBACompositionChildRole(ComponentImplementationContainer::meta_ImplementationRequirement_children, comp, "ImplementationRequirement", true, 0, -1, ImplementationRequirement::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(ImplementationArtifactReference::meta_ComponentImplementationContainer_parent, comp, "ComponentImplementationContainer_parent", true, ComponentImplementationContainer::meta);
			Uml::InitCORBACompositionChildRole(ComponentImplementationContainer::meta_ImplementationArtifactReference_children, comp, "ImplementationArtifactReference", true, 0, -1, ImplementationArtifactReference::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(InfoProperty::meta_ComponentImplementationContainer_parent, comp, "ComponentImplementationContainer_parent", true, ComponentImplementationContainer::meta);
			Uml::InitCORBACompositionChildRole(ComponentImplementationContainer::meta_InfoProperty_children, comp, "InfoProperty", true, 0, -1, InfoProperty::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(Capability::meta_ComponentImplementationContainer_parent, comp, "ComponentImplementationContainer_parent", true, ComponentImplementationContainer::meta);
			Uml::InitCORBACompositionChildRole(ComponentImplementationContainer::meta_Capability_children, comp, "Capability", true, 0, -1, Capability::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(MonolithExecParameter::meta_ComponentImplementationContainer_parent, comp, "ComponentImplementationContainer_parent", true, ComponentImplementationContainer::meta);
			Uml::InitCORBACompositionChildRole(ComponentImplementationContainer::meta_MonolithExecParameter_children, comp, "MonolithExecParameter", true, 0, -1, MonolithExecParameter::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(ComponentImplementation::meta_ComponentImplementationContainer_parent, comp, "ComponentImplementationContainer_parent", true, ComponentImplementationContainer::meta);
			Uml::InitCORBACompositionChildRole(ComponentImplementationContainer::meta_ComponentImplementation_child, comp, "ComponentImplementation", true, 1, 1, ComponentImplementation::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(ImplementationDependency::meta_ComponentImplementationContainer_parent, comp, "ComponentImplementationContainer_parent", true, ComponentImplementationContainer::meta);
			Uml::InitCORBACompositionChildRole(ComponentImplementationContainer::meta_ImplementationDependency_children, comp, "ImplementationDependency", true, 0, -1, ImplementationDependency::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(DeploymentPlan::meta_DeploymentPlans_parent, comp, "DeploymentPlans_parent", true, DeploymentPlans::meta);
			Uml::InitCORBACompositionChildRole(DeploymentPlans::meta_DeploymentPlan_children, comp, "DeploymentPlan", true, 0, -1, DeploymentPlan::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(InstanceMapping::meta_DeploymentPlan_parent, comp, "DeploymentPlan_parent", true, DeploymentPlan::meta);
			Uml::InitCORBACompositionChildRole(DeploymentPlan::meta_InstanceMapping_children, comp, "InstanceMapping", true, 0, -1, InstanceMapping::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(CollocationGroup::meta_DeploymentPlan_parent, comp, "DeploymentPlan_parent", true, DeploymentPlan::meta);
			Uml::InitCORBACompositionChildRole(DeploymentPlan::meta_CollocationGroup_children, comp, "CollocationGroup", true, 0, -1, CollocationGroup::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(ComponentType::meta_DeploymentPlan_parent, comp, "DeploymentPlan_parent", true, DeploymentPlan::meta);
			Uml::InitCORBACompositionChildRole(DeploymentPlan::meta_ComponentType_children, comp, "ComponentType", true, 0, -1, ComponentType::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(NodeReference::meta_DeploymentPlan_parent, comp, "DeploymentPlan_parent", true, DeploymentPlan::meta);
			Uml::InitCORBACompositionChildRole(DeploymentPlan::meta_NodeReference_children, comp, "NodeReference", true, 0, -1, NodeReference::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(PackageConfSelectRequirement::meta_PackageConfigurationContainer_parent, comp, "PackageConfigurationContainer_parent", true, PackageConfigurationContainer::meta);
			Uml::InitCORBACompositionChildRole(PackageConfigurationContainer::meta_PackageConfSelectRequirement_children, comp, "PackageConfSelectRequirement", true, 0, -1, PackageConfSelectRequirement::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(PackageConfiguration::meta_PackageConfigurationContainer_parent, comp, "PackageConfigurationContainer_parent", true, PackageConfigurationContainer::meta);
			Uml::InitCORBACompositionChildRole(PackageConfigurationContainer::meta_PackageConfiguration_children, comp, "PackageConfiguration", true, 0, -1, PackageConfiguration::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(PackageConfBasePackage::meta_PackageConfigurationContainer_parent, comp, "PackageConfigurationContainer_parent", true, PackageConfigurationContainer::meta);
			Uml::InitCORBACompositionChildRole(PackageConfigurationContainer::meta_PackageConfBasePackage_child, comp, "PackageConfBasePackage", true, 0, 1, PackageConfBasePackage::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(PackageConfReference::meta_PackageConfigurationContainer_parent, comp, "PackageConfigurationContainer_parent", true, PackageConfigurationContainer::meta);
			Uml::InitCORBACompositionChildRole(PackageConfigurationContainer::meta_PackageConfReference_child, comp, "PackageConfReference", true, 0, 1, PackageConfReference::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(PackageConfConfigProperty::meta_PackageConfigurationContainer_parent, comp, "PackageConfigurationContainer_parent", true, PackageConfigurationContainer::meta);
			Uml::InitCORBACompositionChildRole(PackageConfigurationContainer::meta_PackageConfConfigProperty_children, comp, "PackageConfConfigProperty", true, 0, -1, PackageConfConfigProperty::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(Property::meta_PackageConfigurationContainer_parent, comp, "PackageConfigurationContainer_parent", true, PackageConfigurationContainer::meta);
			Uml::InitCORBACompositionChildRole(PackageConfigurationContainer::meta_Property_children, comp, "Property", true, 0, -1, Property::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(Requirement::meta_PackageConfigurationContainer_parent, comp, "PackageConfigurationContainer_parent", true, PackageConfigurationContainer::meta);
			Uml::InitCORBACompositionChildRole(PackageConfigurationContainer::meta_Requirement_children, comp, "Requirement", true, 0, -1, Requirement::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(ComponentPackageReference::meta_PackageConfigurationContainer_parent, comp, "PackageConfigurationContainer_parent", true, PackageConfigurationContainer::meta);
			Uml::InitCORBACompositionChildRole(PackageConfigurationContainer::meta_ComponentPackageReference_child, comp, "ComponentPackageReference", true, 0, 1, ComponentPackageReference::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(PackageConfSpecializedConfig::meta_PackageConfigurationContainer_parent, comp, "PackageConfigurationContainer_parent", true, PackageConfigurationContainer::meta);
			Uml::InitCORBACompositionChildRole(PackageConfigurationContainer::meta_PackageConfSpecializedConfig_child, comp, "PackageConfSpecializedConfig", true, 0, 1, PackageConfSpecializedConfig::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(ComponentPackage::meta_PackageConfigurationContainer_parent, comp, "PackageConfigurationContainer_parent", true, PackageConfigurationContainer::meta);
			Uml::InitCORBACompositionChildRole(PackageConfigurationContainer::meta_ComponentPackage_child, comp, "ComponentPackage", true, 0, 1, ComponentPackage::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(PackageConfigurationContainer::meta_PackageConfigurations_parent, comp, "PackageConfigurations_parent", true, PackageConfigurations::meta);
			Uml::InitCORBACompositionChildRole(PackageConfigurations::meta_PackageConfigurationContainer_children, comp, "PackageConfigurationContainer", true, 0, -1, PackageConfigurationContainer::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(PackageContainer::meta_ComponentPackages_parent, comp, "ComponentPackages_parent", true, ComponentPackages::meta);
			Uml::InitCORBACompositionChildRole(ComponentPackages::meta_PackageContainer_children, comp, "PackageContainer", true, 0, -1, PackageContainer::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(Implementation::meta_PackageContainer_parent, comp, "PackageContainer_parent", true, PackageContainer::meta);
			Uml::InitCORBACompositionChildRole(PackageContainer::meta_Implementation_children, comp, "Implementation", true, 0, -1, Implementation::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(PackageInterface::meta_PackageContainer_parent, comp, "PackageContainer_parent", true, PackageContainer::meta);
			Uml::InitCORBACompositionChildRole(PackageContainer::meta_PackageInterface_child, comp, "PackageInterface", true, 0, 1, PackageInterface::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(PackageConfigProperty::meta_PackageContainer_parent, comp, "PackageContainer_parent", true, PackageContainer::meta);
			Uml::InitCORBACompositionChildRole(PackageContainer::meta_PackageConfigProperty_children, comp, "PackageConfigProperty", true, 0, -1, PackageConfigProperty::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(Property::meta_PackageContainer_parent, comp, "PackageContainer_parent", true, PackageContainer::meta);
			Uml::InitCORBACompositionChildRole(PackageContainer::meta_Property_children, comp, "Property", true, 0, -1, Property::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(ComponentType::meta_PackageContainer_parent, comp, "PackageContainer_parent", true, PackageContainer::meta);
			Uml::InitCORBACompositionChildRole(PackageContainer::meta_ComponentType_children, comp, "ComponentType", true, 0, -1, ComponentType::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(ComponentImplementationReference::meta_PackageContainer_parent, comp, "PackageContainer_parent", true, PackageContainer::meta);
			Uml::InitCORBACompositionChildRole(PackageContainer::meta_ComponentImplementationReference_children, comp, "ComponentImplementationReference", true, 0, -1, ComponentImplementationReference::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(ComponentPackage::meta_PackageContainer_parent, comp, "PackageContainer_parent", true, PackageContainer::meta);
			Uml::InitCORBACompositionChildRole(PackageContainer::meta_ComponentPackage_child, comp, "ComponentPackage", true, 0, 1, ComponentPackage::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(PackageInfoProperty::meta_PackageContainer_parent, comp, "PackageContainer_parent", true, PackageContainer::meta);
			Uml::InitCORBACompositionChildRole(PackageContainer::meta_PackageInfoProperty_children, comp, "PackageInfoProperty", true, 0, -1, PackageInfoProperty::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(SatisfierProperty::meta_Resource_parent, comp, "Resource_parent", true, Resource::meta);
			Uml::InitCORBACompositionChildRole(Resource::meta_SatisfierProperty_children, comp, "SatisfierProperty", true, 0, -1, SatisfierProperty::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(Resource::meta_Node_parent, comp, "Node_parent", true, Node::meta);
			Uml::InitCORBACompositionChildRole(Node::meta_Resource_children, comp, "Resource", true, 0, -1, Resource::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(SatisfierProperty::meta_SharedResource_parent, comp, "SharedResource_parent", true, SharedResource::meta);
			Uml::InitCORBACompositionChildRole(SharedResource::meta_SatisfierProperty_children, comp, "SatisfierProperty", true, 0, -1, SatisfierProperty::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(Resource::meta_Interconnect_parent, comp, "Interconnect_parent", true, Interconnect::meta);
			Uml::InitCORBACompositionChildRole(Interconnect::meta_Resource_children, comp, "Resource", true, 0, -1, Resource::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(Resource::meta_Bridge_parent, comp, "Bridge_parent", true, Bridge::meta);
			Uml::InitCORBACompositionChildRole(Bridge::meta_Resource_children, comp, "Resource", true, 0, -1, Resource::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(DataType::meta_SatisfierProperty_parent, comp, "SatisfierProperty_parent", true, SatisfierProperty::meta);
			Uml::InitCORBACompositionChildRole(SatisfierProperty::meta_DataType_child, comp, "DataType", true, 1, 1, DataType::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(DataType::meta_Property_parent, comp, "Property_parent", true, Property::meta);
			Uml::InitCORBACompositionChildRole(Property::meta_DataType_child, comp, "DataType", true, 1, 1, DataType::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(SatisfierProperty::meta_RequirementSatisfier_parent, comp, "RequirementSatisfier_parent", true, RequirementSatisfier::meta);
			Uml::InitCORBACompositionChildRole(RequirementSatisfier::meta_SatisfierProperty_children, comp, "SatisfierProperty", true, 0, -1, SatisfierProperty::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(Property::meta_RequirementBase_parent, comp, "RequirementBase_parent", true, RequirementBase::meta);
			Uml::InitCORBACompositionChildRole(RequirementBase::meta_Property_children, comp, "Property", true, 0, -1, Property::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(Node2Interconnect::meta_Domain_parent, comp, "Domain_parent", true, Domain::meta);
			Uml::InitCORBACompositionChildRole(Domain::meta_Node2Interconnect_children, comp, "Node2Interconnect", true, 0, -1, Node2Interconnect::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(Interconnect2Bridge::meta_Domain_parent, comp, "Domain_parent", true, Domain::meta);
			Uml::InitCORBACompositionChildRole(Domain::meta_Interconnect2Bridge_children, comp, "Interconnect2Bridge", true, 0, -1, Interconnect2Bridge::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(Bridge2Interconnect::meta_Domain_parent, comp, "Domain_parent", true, Domain::meta);
			Uml::InitCORBACompositionChildRole(Domain::meta_Bridge2Interconnect_children, comp, "Bridge2Interconnect", true, 0, -1, Bridge2Interconnect::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(Property::meta_Domain_parent, comp, "Domain_parent", true, Domain::meta);
			Uml::InitCORBACompositionChildRole(Domain::meta_Property_children, comp, "Property", true, 0, -1, Property::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(Shares::meta_Domain_parent, comp, "Domain_parent", true, Domain::meta);
			Uml::InitCORBACompositionChildRole(Domain::meta_Shares_children, comp, "Shares", true, 0, -1, Shares::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(Interconnect2Node::meta_Domain_parent, comp, "Domain_parent", true, Domain::meta);
			Uml::InitCORBACompositionChildRole(Domain::meta_Interconnect2Node_children, comp, "Interconnect2Node", true, 0, -1, Interconnect2Node::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(Elements::meta_Domain_parent, comp, "Domain_parent", true, Domain::meta);
			Uml::InitCORBACompositionChildRole(Domain::meta_Elements_children, comp, "Elements", true, 0, -1, Elements::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(Domain::meta_Targets_parent, comp, "Targets_parent", true, Targets::meta);
			Uml::InitCORBACompositionChildRole(Targets::meta_Domain_children, comp, "Domain", true, 0, -1, Domain::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(PublishConnector::meta_ComponentAssembly_parent, comp, "ComponentAssembly_parent", true, ComponentAssembly::meta);
			Uml::InitCORBACompositionChildRole(ComponentAssembly::meta_PublishConnector_children, comp, "PublishConnector", true, 0, -1, PublishConnector::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(AssemblyConfigProperty::meta_ComponentAssembly_parent, comp, "ComponentAssembly_parent", true, ComponentAssembly::meta);
			Uml::InitCORBACompositionChildRole(ComponentAssembly::meta_AssemblyConfigProperty_children, comp, "AssemblyConfigProperty", true, 0, -1, AssemblyConfigProperty::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(Requirement::meta_ComponentAssembly_parent, comp, "ComponentAssembly_parent", true, ComponentAssembly::meta);
			Uml::InitCORBACompositionChildRole(ComponentAssembly::meta_Requirement_children, comp, "Requirement", true, 0, -1, Requirement::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(ComponentPackageReference::meta_ComponentAssembly_parent, comp, "ComponentAssembly_parent", true, ComponentAssembly::meta);
			Uml::InitCORBACompositionChildRole(ComponentAssembly::meta_ComponentPackageReference_children, comp, "ComponentPackageReference", true, 0, -1, ComponentPackageReference::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(emit::meta_ComponentAssembly_parent, comp, "ComponentAssembly_parent", true, ComponentAssembly::meta);
			Uml::InitCORBACompositionChildRole(ComponentAssembly::meta_emit_children, comp, "emit", true, 0, -1, emit::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(Component::meta_ComponentAssembly_parent, comp, "ComponentAssembly_parent", true, ComponentAssembly::meta);
			Uml::InitCORBACompositionChildRole(ComponentAssembly::meta_Component_children, comp, "Component", true, 1, -1, Component::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(AssemblyDeployRequirement::meta_ComponentAssembly_parent, comp, "ComponentAssembly_parent", true, ComponentAssembly::meta);
			Uml::InitCORBACompositionChildRole(ComponentAssembly::meta_AssemblyDeployRequirement_children, comp, "AssemblyDeployRequirement", true, 0, -1, AssemblyDeployRequirement::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(publish::meta_ComponentAssembly_parent, comp, "ComponentAssembly_parent", true, ComponentAssembly::meta);
			Uml::InitCORBACompositionChildRole(ComponentAssembly::meta_publish_children, comp, "publish", true, 0, -1, publish::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(AssemblyselectRequirement::meta_ComponentAssembly_parent, comp, "ComponentAssembly_parent", true, ComponentAssembly::meta);
			Uml::InitCORBACompositionChildRole(ComponentAssembly::meta_AssemblyselectRequirement_children, comp, "AssemblyselectRequirement", true, 0, -1, AssemblyselectRequirement::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(ComponentPackage::meta_ComponentAssembly_parent, comp, "ComponentAssembly_parent", true, ComponentAssembly::meta);
			Uml::InitCORBACompositionChildRole(ComponentAssembly::meta_ComponentPackage_children, comp, "ComponentPackage", true, 0, -1, ComponentPackage::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(invoke::meta_ComponentAssembly_parent, comp, "ComponentAssembly_parent", true, ComponentAssembly::meta);
			Uml::InitCORBACompositionChildRole(ComponentAssembly::meta_invoke_children, comp, "invoke", true, 0, -1, invoke::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(deliverTo::meta_ComponentAssembly_parent, comp, "ComponentAssembly_parent", true, ComponentAssembly::meta);
			Uml::InitCORBACompositionChildRole(ComponentAssembly::meta_deliverTo_children, comp, "deliverTo", true, 0, -1, deliverTo::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(Property::meta_ComponentAssembly_parent, comp, "ComponentAssembly_parent", true, ComponentAssembly::meta);
			Uml::InitCORBACompositionChildRole(ComponentAssembly::meta_Property_children, comp, "Property", true, 0, -1, Property::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(ArtifactContainer::meta_ImplementationArtifacts_parent, comp, "ImplementationArtifacts_parent", true, ImplementationArtifacts::meta);
			Uml::InitCORBACompositionChildRole(ImplementationArtifacts::meta_ArtifactContainer_children, comp, "ArtifactContainer", true, 0, -1, ArtifactContainer::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(Requirement::meta_ArtifactContainer_parent, comp, "ArtifactContainer_parent", true, ArtifactContainer::meta);
			Uml::InitCORBACompositionChildRole(ArtifactContainer::meta_Requirement_children, comp, "Requirement", true, 0, -1, Requirement::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(ArtifactExecParameter::meta_ArtifactContainer_parent, comp, "ArtifactContainer_parent", true, ArtifactContainer::meta);
			Uml::InitCORBACompositionChildRole(ArtifactContainer::meta_ArtifactExecParameter_children, comp, "ArtifactExecParameter", true, 0, -1, ArtifactExecParameter::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(Property::meta_ArtifactContainer_parent, comp, "ArtifactContainer_parent", true, ArtifactContainer::meta);
			Uml::InitCORBACompositionChildRole(ArtifactContainer::meta_Property_children, comp, "Property", true, 0, -1, Property::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(ArtifactDeployRequirement::meta_ArtifactContainer_parent, comp, "ArtifactContainer_parent", true, ArtifactContainer::meta);
			Uml::InitCORBACompositionChildRole(ArtifactContainer::meta_ArtifactDeployRequirement_children, comp, "ArtifactDeployRequirement", true, 0, -1, ArtifactDeployRequirement::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(ImplementationArtifactReference::meta_ArtifactContainer_parent, comp, "ArtifactContainer_parent", true, ArtifactContainer::meta);
			Uml::InitCORBACompositionChildRole(ArtifactContainer::meta_ImplementationArtifactReference_children, comp, "ImplementationArtifactReference", true, 0, -1, ImplementationArtifactReference::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(ImplementationArtifact::meta_ArtifactContainer_parent, comp, "ArtifactContainer_parent", true, ArtifactContainer::meta);
			Uml::InitCORBACompositionChildRole(ArtifactContainer::meta_ImplementationArtifact_children, comp, "ImplementationArtifact", true, 1, -1, ImplementationArtifact::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(ArtifactInfoProperty::meta_ArtifactContainer_parent, comp, "ArtifactContainer_parent", true, ArtifactContainer::meta);
			Uml::InitCORBACompositionChildRole(ArtifactContainer::meta_ArtifactInfoProperty_children, comp, "ArtifactInfoProperty", true, 0, -1, ArtifactInfoProperty::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(ArtifactDependsOn::meta_ArtifactContainer_parent, comp, "ArtifactContainer_parent", true, ArtifactContainer::meta);
			Uml::InitCORBACompositionChildRole(ArtifactContainer::meta_ArtifactDependsOn_children, comp, "ArtifactDependsOn", true, 0, -1, ArtifactDependsOn::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(Exception::meta_Package_parent, comp, "Package_parent", true, Package::meta);
			Uml::InitCORBACompositionChildRole(Package::meta_Exception_children, comp, "Exception", true, 0, -1, Exception::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(Package::meta_Package_parent, comp, "Package_parent", true, Package::meta);
			Uml::InitCORBACompositionChildRole(Package::meta_Package_children, comp, "Package", true, 0, -1, Package::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(Constant::meta_Package_parent, comp, "Package_parent", true, Package::meta);
			Uml::InitCORBACompositionChildRole(Package::meta_Constant_children, comp, "Constant", true, 0, -1, Constant::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(NamedType::meta_Package_parent, comp, "Package_parent", true, Package::meta);
			Uml::InitCORBACompositionChildRole(Package::meta_NamedType_children, comp, "NamedType", true, 0, -1, NamedType::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(ComponentRef::meta_Package_parent, comp, "Package_parent", true, Package::meta);
			Uml::InitCORBACompositionChildRole(Package::meta_ComponentRef_children, comp, "ComponentRef", true, 0, -1, ComponentRef::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(ManagesComponent::meta_Package_parent, comp, "Package_parent", true, Package::meta);
			Uml::InitCORBACompositionChildRole(Package::meta_ManagesComponent_children, comp, "ManagesComponent", true, 0, -1, ManagesComponent::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(Exception::meta_File_parent, comp, "File_parent", true, File::meta);
			Uml::InitCORBACompositionChildRole(File::meta_Exception_children, comp, "Exception", true, 0, -1, Exception::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(Package::meta_File_parent, comp, "File_parent", true, File::meta);
			Uml::InitCORBACompositionChildRole(File::meta_Package_children, comp, "Package", true, 0, -1, Package::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(FileRef::meta_File_parent, comp, "File_parent", true, File::meta);
			Uml::InitCORBACompositionChildRole(File::meta_FileRef_children, comp, "FileRef", true, 0, -1, FileRef::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(Constant::meta_File_parent, comp, "File_parent", true, File::meta);
			Uml::InitCORBACompositionChildRole(File::meta_Constant_children, comp, "Constant", true, 0, -1, Constant::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(NamedType::meta_File_parent, comp, "File_parent", true, File::meta);
			Uml::InitCORBACompositionChildRole(File::meta_NamedType_children, comp, "NamedType", true, 0, -1, NamedType::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(ComponentRef::meta_File_parent, comp, "File_parent", true, File::meta);
			Uml::InitCORBACompositionChildRole(File::meta_ComponentRef_children, comp, "ComponentRef", true, 0, -1, ComponentRef::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(ManagesComponent::meta_File_parent, comp, "File_parent", true, File::meta);
			Uml::InitCORBACompositionChildRole(File::meta_ManagesComponent_children, comp, "ManagesComponent", true, 0, -1, ManagesComponent::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(Member::meta_Exception_parent, comp, "Exception_parent", true, Exception::meta);
			Uml::InitCORBACompositionChildRole(Exception::meta_Member_children, comp, "Member", true, 0, -1, Member::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(File::meta_InterfaceDefinitions_parent, comp, "InterfaceDefinitions_parent", true, InterfaceDefinitions::meta);
			Uml::InitCORBACompositionChildRole(InterfaceDefinitions::meta_File_children, comp, "File", true, 1, -1, File::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(PredefinedType::meta_PredefinedTypes_parent, comp, "PredefinedTypes_parent", true, PredefinedTypes::meta);
			Uml::InitCORBACompositionChildRole(PredefinedTypes::meta_PredefinedType_children, comp, "PredefinedType", true, 0, -1, PredefinedType::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(EnumValue::meta_Enum_parent, comp, "Enum_parent", true, Enum::meta);
			Uml::InitCORBACompositionChildRole(Enum::meta_EnumValue_children, comp, "EnumValue", true, 1, -1, EnumValue::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(Member::meta_Aggregate_parent, comp, "Aggregate_parent", true, Aggregate::meta);
			Uml::InitCORBACompositionChildRole(Aggregate::meta_Member_children, comp, "Member", true, 0, -1, Member::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(Label::meta_SwitchedAggregate_parent, comp, "SwitchedAggregate_parent", true, SwitchedAggregate::meta);
			Uml::InitCORBACompositionChildRole(SwitchedAggregate::meta_Label_children, comp, "Label", true, 1, -1, Label::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(LabelConnection::meta_SwitchedAggregate_parent, comp, "SwitchedAggregate_parent", true, SwitchedAggregate::meta);
			Uml::InitCORBACompositionChildRole(SwitchedAggregate::meta_LabelConnection_children, comp, "LabelConnection", true, 1, -1, LabelConnection::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(Member::meta_SwitchedAggregate_parent, comp, "SwitchedAggregate_parent", true, SwitchedAggregate::meta);
			Uml::InitCORBACompositionChildRole(SwitchedAggregate::meta_Member_children, comp, "Member", true, 0, -1, Member::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(Discriminator::meta_SwitchedAggregate_parent, comp, "SwitchedAggregate_parent", true, SwitchedAggregate::meta);
			Uml::InitCORBACompositionChildRole(SwitchedAggregate::meta_Discriminator_child, comp, "Discriminator", true, 1, 1, Discriminator::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(ReadonlyAttribute::meta_Inheritable_parent, comp, "Inheritable_parent", true, Inheritable::meta);
			Uml::InitCORBACompositionChildRole(Inheritable::meta_ReadonlyAttribute_children, comp, "ReadonlyAttribute", true, 0, -1, ReadonlyAttribute::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(Inherits::meta_Inheritable_parent, comp, "Inheritable_parent", true, Inheritable::meta);
			Uml::InitCORBACompositionChildRole(Inheritable::meta_Inherits_children, comp, "Inherits", true, 0, -1, Inherits::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(TwowayOperation::meta_HasOperations_parent, comp, "HasOperations_parent", true, HasOperations::meta);
			Uml::InitCORBACompositionChildRole(HasOperations::meta_TwowayOperation_children, comp, "TwowayOperation", true, 0, -1, TwowayOperation::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(OnewayOperation::meta_HasOperations_parent, comp, "HasOperations_parent", true, HasOperations::meta);
			Uml::InitCORBACompositionChildRole(HasOperations::meta_OnewayOperation_children, comp, "OnewayOperation", true, 0, -1, OnewayOperation::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(Exception::meta_HasOperations_parent, comp, "HasOperations_parent", true, HasOperations::meta);
			Uml::InitCORBACompositionChildRole(HasOperations::meta_Exception_children, comp, "Exception", true, 0, -1, Exception::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(Constant::meta_HasOperations_parent, comp, "HasOperations_parent", true, HasOperations::meta);
			Uml::InitCORBACompositionChildRole(HasOperations::meta_Constant_children, comp, "Constant", true, 0, -1, Constant::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(NoInheritable::meta_HasOperations_parent, comp, "HasOperations_parent", true, HasOperations::meta);
			Uml::InitCORBACompositionChildRole(HasOperations::meta_NoInheritable_children, comp, "NoInheritable", true, 0, -1, NoInheritable::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(Supports::meta_SupportsInterfaces_parent, comp, "SupportsInterfaces_parent", true, SupportsInterfaces::meta);
			Uml::InitCORBACompositionChildRole(SupportsInterfaces::meta_Supports_children, comp, "Supports", true, 0, -1, Supports::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(AttributeMember::meta_ReadonlyAttribute_parent, comp, "ReadonlyAttribute_parent", true, ReadonlyAttribute::meta);
			Uml::InitCORBACompositionChildRole(ReadonlyAttribute::meta_AttributeMember_child, comp, "AttributeMember", true, 1, 1, AttributeMember::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(GetException::meta_ReadonlyAttribute_parent, comp, "ReadonlyAttribute_parent", true, ReadonlyAttribute::meta);
			Uml::InitCORBACompositionChildRole(ReadonlyAttribute::meta_GetException_children, comp, "GetException", true, 0, -1, GetException::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(PrivateFlag::meta_ObjectByValue_parent, comp, "ObjectByValue_parent", true, ObjectByValue::meta);
			Uml::InitCORBACompositionChildRole(ObjectByValue::meta_PrivateFlag_children, comp, "PrivateFlag", true, 0, -1, PrivateFlag::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(FactoryOperation::meta_ObjectByValue_parent, comp, "ObjectByValue_parent", true, ObjectByValue::meta);
			Uml::InitCORBACompositionChildRole(ObjectByValue::meta_FactoryOperation_children, comp, "FactoryOperation", true, 0, -1, FactoryOperation::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(MakeMemberPrivate::meta_ObjectByValue_parent, comp, "ObjectByValue_parent", true, ObjectByValue::meta);
			Uml::InitCORBACompositionChildRole(ObjectByValue::meta_MakeMemberPrivate_children, comp, "MakeMemberPrivate", true, 0, -1, MakeMemberPrivate::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(Member::meta_ObjectByValue_parent, comp, "ObjectByValue_parent", true, ObjectByValue::meta);
			Uml::InitCORBACompositionChildRole(ObjectByValue::meta_Member_children, comp, "Member", true, 0, -1, Member::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(SetException::meta_Attribute_parent, comp, "Attribute_parent", true, Attribute::meta);
			Uml::InitCORBACompositionChildRole(Attribute::meta_SetException_children, comp, "SetException", true, 0, -1, SetException::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(InParameter::meta_OperationBase_parent, comp, "OperationBase_parent", true, OperationBase::meta);
			Uml::InitCORBACompositionChildRole(OperationBase::meta_InParameter_children, comp, "InParameter", true, 0, -1, InParameter::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(OutParameter::meta_TwowayOperation_parent, comp, "TwowayOperation_parent", true, TwowayOperation::meta);
			Uml::InitCORBACompositionChildRole(TwowayOperation::meta_OutParameter_children, comp, "OutParameter", true, 0, -1, OutParameter::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(InoutParameter::meta_TwowayOperation_parent, comp, "TwowayOperation_parent", true, TwowayOperation::meta);
			Uml::InitCORBACompositionChildRole(TwowayOperation::meta_InoutParameter_children, comp, "InoutParameter", true, 0, -1, InoutParameter::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(ReturnType::meta_TwowayOperation_parent, comp, "TwowayOperation_parent", true, TwowayOperation::meta);
			Uml::InitCORBACompositionChildRole(TwowayOperation::meta_ReturnType_child, comp, "ReturnType", true, 0, 1, ReturnType::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(ExceptionRef::meta_HasExceptions_parent, comp, "HasExceptions_parent", true, HasExceptions::meta);
			Uml::InitCORBACompositionChildRole(HasExceptions::meta_ExceptionRef_children, comp, "ExceptionRef", true, 0, -1, ExceptionRef::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(Port::meta_Component_parent, comp, "Component_parent", true, Component::meta);
			Uml::InitCORBACompositionChildRole(Component::meta_Port_children, comp, "Port", true, 0, -1, Port::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(LookupKey::meta_ComponentFactory_parent, comp, "ComponentFactory_parent", true, ComponentFactory::meta);
			Uml::InitCORBACompositionChildRole(ComponentFactory::meta_LookupKey_child, comp, "LookupKey", true, 0, 1, LookupKey::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(FactoryOperation::meta_ComponentFactory_parent, comp, "ComponentFactory_parent", true, ComponentFactory::meta);
			Uml::InitCORBACompositionChildRole(ComponentFactory::meta_FactoryOperation_children, comp, "FactoryOperation", true, 0, -1, FactoryOperation::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(LookupOperation::meta_ComponentFactory_parent, comp, "ComponentFactory_parent", true, ComponentFactory::meta);
			Uml::InitCORBACompositionChildRole(ComponentFactory::meta_LookupOperation_children, comp, "LookupOperation", true, 0, -1, LookupOperation::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(ComponentTypes::meta_RootFolder_parent, comp, "RootFolder_parent", true, RootFolder::meta);
			Uml::InitCORBACompositionChildRole(RootFolder::meta_ComponentTypes_children, comp, "ComponentTypes", true, 0, -1, ComponentTypes::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(TopLevelPackages::meta_RootFolder_parent, comp, "RootFolder_parent", true, RootFolder::meta);
			Uml::InitCORBACompositionChildRole(RootFolder::meta_TopLevelPackages_children, comp, "TopLevelPackages", true, 0, -1, TopLevelPackages::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(ComponentImplementations::meta_RootFolder_parent, comp, "RootFolder_parent", true, RootFolder::meta);
			Uml::InitCORBACompositionChildRole(RootFolder::meta_ComponentImplementations_children, comp, "ComponentImplementations", true, 0, -1, ComponentImplementations::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(DeploymentPlans::meta_RootFolder_parent, comp, "RootFolder_parent", true, RootFolder::meta);
			Uml::InitCORBACompositionChildRole(RootFolder::meta_DeploymentPlans_children, comp, "DeploymentPlans", true, 0, -1, DeploymentPlans::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(PackageConfigurations::meta_RootFolder_parent, comp, "RootFolder_parent", true, RootFolder::meta);
			Uml::InitCORBACompositionChildRole(RootFolder::meta_PackageConfigurations_children, comp, "PackageConfigurations", true, 0, -1, PackageConfigurations::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(ComponentPackages::meta_RootFolder_parent, comp, "RootFolder_parent", true, RootFolder::meta);
			Uml::InitCORBACompositionChildRole(RootFolder::meta_ComponentPackages_children, comp, "ComponentPackages", true, 0, -1, ComponentPackages::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(Targets::meta_RootFolder_parent, comp, "RootFolder_parent", true, RootFolder::meta);
			Uml::InitCORBACompositionChildRole(RootFolder::meta_Targets_children, comp, "Targets", true, 0, -1, Targets::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(ImplementationArtifacts::meta_RootFolder_parent, comp, "RootFolder_parent", true, RootFolder::meta);
			Uml::InitCORBACompositionChildRole(RootFolder::meta_ImplementationArtifacts_children, comp, "ImplementationArtifacts", true, 0, -1, ImplementationArtifacts::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(InterfaceDefinitions::meta_RootFolder_parent, comp, "RootFolder_parent", true, RootFolder::meta);
			Uml::InitCORBACompositionChildRole(RootFolder::meta_InterfaceDefinitions_children, comp, "InterfaceDefinitions", true, 0, -1, InterfaceDefinitions::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(PredefinedTypes::meta_RootFolder_parent, comp, "RootFolder_parent", true, RootFolder::meta);
			Uml::InitCORBACompositionChildRole(RootFolder::meta_PredefinedTypes_children, comp, "PredefinedTypes", true, 0, -1, PredefinedTypes::meta);
		}
		{
			Uml::Composition comp = Uml::CreateCORBAComposition();
			Uml::InitCORBAComposition(comp , umldiagram, "");
		
			Uml::InitCORBACompositionParentRole(RootFolder::meta_RootFolder_parent, comp, "RootFolder_parent", true, RootFolder::meta);
			Uml::InitCORBACompositionChildRole(RootFolder::meta_RootFolder_children, comp, "RootFolder", true, 0, -1, RootFolder::meta);
		}
		Uml::AddCORBAInheritance(CommonPortAttrs::meta, Port::meta);
		Uml::AddCORBAInheritance(ComponentImplementation::meta, MonolithicImplementation::meta);
		Uml::AddCORBAInheritance(ComponentImplementation::meta, ComponentAssembly::meta);
		Uml::AddCORBAInheritance(RequirementSatisfier::meta, Capability::meta);
		Uml::AddCORBAInheritance(RequirementBase::meta, Requirement::meta);
		Uml::AddCORBAInheritance(RequirementBase::meta, ImplementationRequirement::meta);
		Uml::AddCORBAInheritance(Elements::meta, Node::meta);
		Uml::AddCORBAInheritance(Elements::meta, SharedResource::meta);
		Uml::AddCORBAInheritance(Elements::meta, Interconnect::meta);
		Uml::AddCORBAInheritance(Elements::meta, Bridge::meta);
		Uml::AddCORBAInheritance(ConstantType::meta, PredefinedType::meta);
		Uml::AddCORBAInheritance(ConstantType::meta, Enum::meta);
		Uml::AddCORBAInheritance(PredefinedType::meta, Byte::meta);
		Uml::AddCORBAInheritance(PredefinedType::meta, Boolean::meta);
		Uml::AddCORBAInheritance(PredefinedType::meta, ShortInteger::meta);
		Uml::AddCORBAInheritance(PredefinedType::meta, LongInteger::meta);
		Uml::AddCORBAInheritance(PredefinedType::meta, RealNumber::meta);
		Uml::AddCORBAInheritance(PredefinedType::meta, String::meta);
		Uml::AddCORBAInheritance(PredefinedType::meta, GenericObject::meta);
		Uml::AddCORBAInheritance(PredefinedType::meta, GenericValueObject::meta);
		Uml::AddCORBAInheritance(PredefinedType::meta, GenericValue::meta);
		Uml::AddCORBAInheritance(PredefinedType::meta, TypeEncoding::meta);
		Uml::AddCORBAInheritance(PredefinedType::meta, TypeKind::meta);
		Uml::AddCORBAInheritance(MemberType::meta, PredefinedType::meta);
		Uml::AddCORBAInheritance(MemberType::meta, NamedType::meta);
		Uml::AddCORBAInheritance(NamedType::meta, Boxed::meta);
		Uml::AddCORBAInheritance(NamedType::meta, NoInheritable::meta);
		Uml::AddCORBAInheritance(NamedType::meta, Inheritable::meta);
		Uml::AddCORBAInheritance(NoInheritable::meta, Enum::meta);
		Uml::AddCORBAInheritance(NoInheritable::meta, Aggregate::meta);
		Uml::AddCORBAInheritance(NoInheritable::meta, SwitchedAggregate::meta);
		Uml::AddCORBAInheritance(NoInheritable::meta, Collection::meta);
		Uml::AddCORBAInheritance(NoInheritable::meta, Alias::meta);
		Uml::AddCORBAInheritance(Inheritable::meta, HasOperations::meta);
		Uml::AddCORBAInheritance(Inheritable::meta, SupportsInterfaces::meta);
		Uml::AddCORBAInheritance(HasOperations::meta, Object::meta);
		Uml::AddCORBAInheritance(HasOperations::meta, ObjectByValue::meta);
		Uml::AddCORBAInheritance(HasOperations::meta, ComponentFactory::meta);
		Uml::AddCORBAInheritance(SupportsInterfaces::meta, ObjectByValue::meta);
		Uml::AddCORBAInheritance(SupportsInterfaces::meta, Component::meta);
		Uml::AddCORBAInheritance(SupportsInterfaces::meta, ComponentFactory::meta);
		Uml::AddCORBAInheritance(ReadonlyAttribute::meta, Attribute::meta);
		Uml::AddCORBAInheritance(ObjectByValue::meta, Event::meta);
		Uml::AddCORBAInheritance(ObjectByValue::meta, ValueObject::meta);
		Uml::AddCORBAInheritance(OperationBase::meta, OnewayOperation::meta);
		Uml::AddCORBAInheritance(OperationBase::meta, HasExceptions::meta);
		Uml::AddCORBAInheritance(HasExceptions::meta, FactoryOperation::meta);
		Uml::AddCORBAInheritance(HasExceptions::meta, LookupOperation::meta);
		Uml::AddCORBAInheritance(HasExceptions::meta, TwowayOperation::meta);
		Uml::AddCORBAInheritance(Port::meta, OutEventPort::meta);
		Uml::AddCORBAInheritance(Port::meta, ProvidedRequestPort::meta);
		Uml::AddCORBAInheritance(Port::meta, RequiredRequestPort::meta);
		Uml::AddCORBAInheritance(Port::meta, InEventPort::meta);
		Uml::AddCORBAInheritance(Manageable::meta, Component::meta);
		Uml::AddCORBAInheritance(Manageable::meta, ComponentRef::meta);
		Uml::AddCORBAInheritance(Provideable::meta, GenericObject::meta);
		Uml::AddCORBAInheritance(Provideable::meta, Object::meta);
		Uml::AddCORBAInheritance(Prefixable::meta, Package::meta);
		Uml::AddCORBAInheritance(Prefixable::meta, File::meta);
		Uml::AddCORBAInheritance(Prefixable::meta, Object::meta);
		Uml::AddCORBAInheritance(Prefixable::meta, ObjectByValue::meta);
		Uml::AddCORBAInheritance(Taggable::meta, Package::meta);
		Uml::AddCORBAInheritance(Taggable::meta, Exception::meta);
		Uml::AddCORBAInheritance(Taggable::meta, Constant::meta);
		Uml::AddCORBAInheritance(Taggable::meta, NamedType::meta);
		Uml::AddCORBAInheritance(Taggable::meta, ReadonlyAttribute::meta);
		Uml::AddCORBAInheritance(Taggable::meta, OperationBase::meta);
		Uml::AddCORBAInheritance(Taggable::meta, Port::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, package::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, TopLevelPackageContainer::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, TopLevelPackage::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, ComponentInfoProperty::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, ComponentContainer::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, ComponentProperty::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, CommonPortAttrs::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, ComponentType::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, ComponentConfigProperty::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, ComponentPropertyDescription::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, ConfigProperty::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, Implements::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, ImplementationCapability::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, ImplementationDependsOn::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, InfoProperty::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, MonolithprimaryArtifact::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, ComponentImplementationReference::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, MonolithExecParameter::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, ComponentImplementationContainer::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, MonolithDeployRequirement::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, ComponentImplementation::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, DeploymentPlan::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, CollocationGroup::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, InstanceMapping::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, PackageConfSpecializedConfig::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, PackageConfigurationContainer::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, PackageConfConfigProperty::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, PackageConfBasePackage::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, PackageConfigurationReference::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, PackageConfSelectRequirement::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, PackageConfReference::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, PackageConfiguration::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, PackageInterface::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, PackageInfoProperty::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, PackageConfigProperty::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, PackageContainer::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, Implementation::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, ComponentPackage::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, ComponentPackageReference::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, Resource::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, NodeReference::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, SatisfierProperty::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, DataType::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, ImplementationDependency::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, Property::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, RequirementSatisfier::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, RequirementBase::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, Elements::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, Domain::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, Interconnect2Node::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, Bridge2Interconnect::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, Shares::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, Node2Interconnect::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, Interconnect2Bridge::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, AssemblyDeployRequirement::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, invoke::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, AssemblyselectRequirement::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, emit::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, PublishConnector::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, deliverTo::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, publish::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, AssemblyConfigProperty::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, ArtifactExecParameter::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, ImplementationArtifactReference::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, ArtifactDependsOn::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, ImplementationArtifact::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, ArtifactDeployRequirement::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, ArtifactInfoProperty::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, ArtifactContainer::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, ExceptionRef::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, FileRef::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, ConstantType::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, EnumValue::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, Label::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, LabelConnection::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, MemberType::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, Discriminator::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, Member::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, Inherits::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, LookupKey::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, SetException::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, GetException::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, PrivateFlag::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, MakeMemberPrivate::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, AttributeMember::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, Supports::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, InParameter::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, OutParameter::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, ReturnType::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, InoutParameter::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, Manageable::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, ManagesComponent::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, Provideable::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, Prefixable::meta);
		Uml::AddCORBAInheritance(MgaObject::meta, Taggable::meta);
		
	}
	Udm::UdmDiagram diagram = { &umldiagram, Initialize };
}
// END PICML.cpp
