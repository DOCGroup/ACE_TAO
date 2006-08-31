// cpp(meta datanetwork format) source file PICML.cpp generated from diagram PICML
// generated on Mon Nov 15 23:04:54 2004

#include "PICML.h"
#include "UmlExt.h"

#include "UdmStatic.h"

// cross-package metainformation header file
#include "Cadena2PICML.h"

namespace PICML {

	Uml::Diagram umldiagram;

	Uml::Class RTEC_Proxy::meta;
	Uml::Class Include_RTEC_Filter_Group_Members_Base::meta;
	Uml::Class RTEC_Proxy_Supplier::meta;
	Uml::Class RTEC_Proxy_Consumer::meta;
	Uml::Class RT_Info::meta;
	Uml::Class RTEC_Connection::meta;
	Uml::Class RTEC_Filter_Group::meta;
	Uml::Class LogicalANDGroup::meta;
	Uml::Class Include_RTEC_Filter_Group::meta;
	Uml::Class DisjunctionGroup::meta;
	Uml::Class ConjunctionGroup::meta;
	Uml::Class Use_RTEC_Filter::meta;
	Uml::Class EventChannelConfiguration::meta;
	Uml::Class RTEC_Resource_Factory::meta;
	Uml::Class Include_RT_Info::meta;
	Uml::Class Use_RT_Info::meta;
	Uml::Class RT_Info_Param::meta;
	Uml::Class Null_RT_Info::meta;
	Uml::Class RTEC_Resource_Factory_Reference::meta;
	Uml::Class EventChannelConfig::meta;
	Uml::Class Proxy::meta;
	Uml::Class Proxy_Consumer::meta;
	Uml::Class Proxy_Supplier::meta;
	Uml::Class MetricConnection::meta;
	Uml::Class DataAnalysisBase::meta;
	Uml::Class Average::meta;
	Uml::Class Minimum::meta;
	Uml::Class Maximum::meta;
	Uml::Class Jitter::meta;
	Uml::Class TimeProbe::meta;
	Uml::Class TimerConnection::meta;
	Uml::Class Latency::meta;
	Uml::Class Throughput::meta;
	Uml::Class ComponentAnalyses::meta;
	Uml::Class BenchmarkAnalysis::meta;
	Uml::Class MetricsBase::meta;
	Uml::Class OperationRef::meta;
	Uml::Class EventRef::meta;
	Uml::Class TimerEventSinkConn::meta;
	Uml::Class TaskSet::meta;
	Uml::Class Task::meta;
	Uml::Class WorkloadCharacteristics::meta;
	Uml::Class NodeReference::meta;
	Uml::Class Interconnect::meta;
	Uml::Class Node::meta;
	Uml::Class Resource::meta;
	Uml::Class Bridge::meta;
	Uml::Class SharedResource::meta;
	Uml::Class InstanceMapping::meta;
	Uml::Class DeploymentPlans::meta;
	Uml::Class CollocationGroup::meta;
	Uml::Class DeploymentPlan::meta;
	Uml::Class invoke::meta;
	Uml::Class AssemblyselectRequirement::meta;
	Uml::Class AssemblyConfigProperty::meta;
	Uml::Class PublishConnector::meta;
	Uml::Class publish::meta;
	Uml::Class deliverTo::meta;
	Uml::Class AssemblyDeployRequirement::meta;
	Uml::Class emit::meta;
	Uml::Class ReceptacleDelegate::meta;
	Uml::Class FacetDelegate::meta;
	Uml::Class EventSourceDelegate::meta;
	Uml::Class EventSinkDelegate::meta;
	Uml::Class ExternalDelegate::meta;
	Uml::Class ComponentAssembly::meta;
	Uml::Class ExternalReferenceEndPoint::meta;
	Uml::Class ImplementationCapability::meta;
	Uml::Class ImplementationDependsOn::meta;
	Uml::Class Implements::meta;
	Uml::Class ComponentImplementations::meta;
	Uml::Class ConfigProperty::meta;
	Uml::Class InfoProperty::meta;
	Uml::Class MonolithprimaryArtifact::meta;
	Uml::Class MonolithDeployRequirement::meta;
	Uml::Class ComponentImplementationContainer::meta;
	Uml::Class MonolithExecParameter::meta;
	Uml::Class MonolithicImplementation::meta;
	Uml::Class ComponentImplementationReference::meta;
	Uml::Class ComponentImplementation::meta;
	Uml::Class PackageConfBasePackage::meta;
	Uml::Class PackageConfReference::meta;
	Uml::Class PackageConfigurations::meta;
	Uml::Class PackageConfigurationContainer::meta;
	Uml::Class PackageConfConfigProperty::meta;
	Uml::Class PackageConfSelectRequirement::meta;
	Uml::Class PackageConfSpecializedConfig::meta;
	Uml::Class PackageConfigurationReference::meta;
	Uml::Class PackageConfiguration::meta;
	Uml::Class ImplementationArtifacts::meta;
	Uml::Class ArtifactDependsOn::meta;
	Uml::Class ArtifactDeployRequirement::meta;
	Uml::Class ArtifactContainer::meta;
	Uml::Class ArtifactExecParameter::meta;
	Uml::Class ArtifactInfoProperty::meta;
	Uml::Class ImplementationArtifactReference::meta;
	Uml::Class ImplementationArtifact::meta;
	Uml::Class ComponentTypes::meta;
	Uml::Class ComponentPropertyDescription::meta;
	Uml::Class ComponentProperty::meta;
	Uml::Class ComponentContainer::meta;
	Uml::Class ComponentConfigProperty::meta;
	Uml::Class ComponentInfoProperty::meta;
	Uml::Class CommonPortAttrs::meta;
	Uml::Class TopLevelPackageContainer::meta;
	Uml::Class TopLevelPackages::meta;
	Uml::Class TopLevelPackage::meta;
	Uml::Class package::meta;
	Uml::Class ComponentPackages::meta;
	Uml::Class PackageInterface::meta;
	Uml::Class Implementation::meta;
	Uml::Class PackageContainer::meta;
	Uml::Class PackageConfigProperty::meta;
	Uml::Class PackageInfoProperty::meta;
	Uml::Class ComponentPackage::meta;
	Uml::Class ComponentPackageReference::meta;
	Uml::Class Requirement::meta;
	Uml::Class Capability::meta;
	Uml::Class DataType::meta;
	Uml::Class SatisfierProperty::meta;
	Uml::Class ImplementationDependency::meta;
	Uml::Class RequirementSatisfier::meta;
	Uml::Class RequirementBase::meta;
	Uml::Class ImplementationRequirement::meta;
	Uml::Class Property::meta;
	Uml::Class Elements::meta;
	Uml::Class Targets::meta;
	Uml::Class Shares::meta;
	Uml::Class Node2Interconnect::meta;
	Uml::Class Bridge2Interconnect::meta;
	Uml::Class Interconnect2Node::meta;
	Uml::Class Interconnect2Bridge::meta;
	Uml::Class Domain::meta;
	Uml::Class Prefixable::meta;
	Uml::Class Taggable::meta;
	Uml::Class OutEventPort::meta;
	Uml::Class ProvidedRequestPort::meta;
	Uml::Class RequiredRequestPort::meta;
	Uml::Class InEventPort::meta;
	Uml::Class Port::meta;
	Uml::Class Manageable::meta;
	Uml::Class Provideable::meta;
	Uml::Class ComponentFactory::meta;
	Uml::Class ComponentRef::meta;
	Uml::Class ManagesComponent::meta;
	Uml::Class Component::meta;
	Uml::Class OnewayOperation::meta;
	Uml::Class LookupOperation::meta;
	Uml::Class FactoryOperation::meta;
	Uml::Class OperationBase::meta;
	Uml::Class HasExceptions::meta;
	Uml::Class InParameter::meta;
	Uml::Class InoutParameter::meta;
	Uml::Class OutParameter::meta;
	Uml::Class ReturnType::meta;
	Uml::Class TwowayOperation::meta;
	Uml::Class Object::meta;
	Uml::Class Inherits::meta;
	Uml::Class Event::meta;
	Uml::Class ValueObject::meta;
	Uml::Class HasOperations::meta;
	Uml::Class SupportsInterfaces::meta;
	Uml::Class Supports::meta;
	Uml::Class ReadonlyAttribute::meta;
	Uml::Class Attribute::meta;
	Uml::Class LookupKey::meta;
	Uml::Class SetException::meta;
	Uml::Class ObjectByValue::meta;
	Uml::Class GetException::meta;
	Uml::Class PrivateFlag::meta;
	Uml::Class MakeMemberPrivate::meta;
	Uml::Class AttributeMember::meta;
	Uml::Class Inheritable::meta;
	Uml::Class EnumValue::meta;
	Uml::Class Aggregate::meta;
	Uml::Class SwitchedAggregate::meta;
	Uml::Class Label::meta;
	Uml::Class LabelConnection::meta;
	Uml::Class MemberType::meta;
	Uml::Class Discriminator::meta;
	Uml::Class Collection::meta;
	Uml::Class Alias::meta;
	Uml::Class Boxed::meta;
	Uml::Class NoInheritable::meta;
	Uml::Class Enum::meta;
	Uml::Class Member::meta;
	Uml::Class NamedType::meta;
	Uml::Class PredefinedType::meta;
	Uml::Class PredefinedTypes::meta;
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
	Uml::Class Package::meta;
	Uml::Class File::meta;
	Uml::Class Exception::meta;
	Uml::Class ExceptionRef::meta;
	Uml::Class InterfaceDefinitions::meta;
	Uml::Class ConstantType::meta;
	Uml::Class Constant::meta;
	Uml::Class FileRef::meta;
	Uml::Class ComponentBuild::meta;
	Uml::Class Project::meta;
	Uml::Class ExternalResources::meta;
	Uml::Class ExtResourceConn::meta;
	Uml::Class ComponentLib::meta;
	Uml::Class MPC::meta;
	Uml::Class StubProject::meta;
	Uml::Class ServantProject::meta;
	Uml::Class ExecutorProject::meta;
	Uml::Class Workspaces::meta;
	Uml::Class RootFolder::meta;
	Uml::Class MgaObject::meta;
	Uml::Attribute RT_Info::meta_entry_point;
	Uml::Attribute RT_Info::meta_period;
	Uml::Attribute RT_Info::meta_enabled;
	Uml::Attribute RT_Info::meta_criticality;
	Uml::Attribute RT_Info::meta_importance;
	Uml::Attribute RT_Info::meta_quantum;
	Uml::Attribute RT_Info::meta_cached_execution_time;
	Uml::Attribute RT_Info::meta_threads;
	Uml::Attribute RT_Info::meta_info_type;
	Uml::Attribute RT_Info::meta_worst_case_execution_time;
	Uml::Attribute RT_Info::meta_typical_execution_time;
	Uml::Attribute RTEC_Resource_Factory::meta_ECConsumerLock;
	Uml::Attribute RTEC_Resource_Factory::meta_ECSupplierFiltering;
	Uml::Attribute RTEC_Resource_Factory::meta_ECDispatching;
	Uml::Attribute RTEC_Resource_Factory::meta_ECTimeout;
	Uml::Attribute RTEC_Resource_Factory::meta_ECConsumerControlPeriod;
	Uml::Attribute RTEC_Resource_Factory::meta_ECSupplierLock;
	Uml::Attribute RTEC_Resource_Factory::meta_ECObserver;
	Uml::Attribute RTEC_Resource_Factory::meta_ECSupplierControl;
	Uml::Attribute RTEC_Resource_Factory::meta_ECConsumerControl;
	Uml::Attribute RTEC_Resource_Factory::meta_ECFiltering;
	Uml::Attribute RTEC_Resource_Factory::meta_ECProxyPushConsumerCollectionFlag;
	Uml::Attribute RTEC_Resource_Factory::meta_ECUseORBId;
	Uml::Attribute RTEC_Resource_Factory::meta_ECProxyPushConsumerCollectionSynch;
	Uml::Attribute RTEC_Resource_Factory::meta_ECProxyPushConsumerCollectionIterator;
	Uml::Attribute RTEC_Resource_Factory::meta_ECSupplierControlPeriod;
	Uml::Attribute RTEC_Resource_Factory::meta_ECScheduling;
	Uml::Attribute RTEC_Resource_Factory::meta_ECDispatchingThreads;
	Uml::Attribute RTEC_Resource_Factory::meta_ECProxyPushSupplierCollectionFlag;
	Uml::Attribute RTEC_Resource_Factory::meta_ECProxyPushSupplierCollectionSynch;
	Uml::Attribute RTEC_Resource_Factory::meta_ECProxyPushSupplierCollectionIterator;
	Uml::Attribute EventChannelConfig::meta_name;
	Uml::Attribute ComponentAnalyses::meta_name;
	Uml::Attribute MetricsBase::meta_warmup;
	Uml::Attribute MetricsBase::meta_fileName;
	Uml::Attribute MetricsBase::meta_iterations;
	Uml::Attribute Task::meta_rate;
	Uml::Attribute Task::meta_priority;
	Uml::Attribute Interconnect::meta_label;
	Uml::Attribute Node::meta_label;
	Uml::Attribute Resource::meta_resourceType;
	Uml::Attribute Bridge::meta_label;
	Uml::Attribute SharedResource::meta_resourceType;
	Uml::Attribute DeploymentPlans::meta_name;
	Uml::Attribute DeploymentPlan::meta_label;
	Uml::Attribute ExternalReferenceEndPoint::meta_location;
	Uml::Attribute ComponentImplementations::meta_name;
	Uml::Attribute ComponentImplementation::meta_UUID;
	Uml::Attribute ComponentImplementation::meta_label;
	Uml::Attribute PackageConfigurations::meta_name;
	Uml::Attribute PackageConfiguration::meta_UUID;
	Uml::Attribute PackageConfiguration::meta_label;
	Uml::Attribute ImplementationArtifacts::meta_name;
	Uml::Attribute ImplementationArtifact::meta_tree_file_name;
	Uml::Attribute ImplementationArtifact::meta_configuration;
	Uml::Attribute ImplementationArtifact::meta_label;
	Uml::Attribute ImplementationArtifact::meta_UUID;
	Uml::Attribute ImplementationArtifact::meta_location;
	Uml::Attribute ComponentTypes::meta_name;
	Uml::Attribute CommonPortAttrs::meta_exclusiveUser;
	Uml::Attribute CommonPortAttrs::meta_optional;
	Uml::Attribute CommonPortAttrs::meta_exclusiveProvider;
	Uml::Attribute TopLevelPackages::meta_name;
	Uml::Attribute ComponentPackages::meta_name;
	Uml::Attribute ComponentPackage::meta_UUID;
	Uml::Attribute ComponentPackage::meta_label;
	Uml::Attribute ComponentPackageReference::meta_requiredName;
	Uml::Attribute ComponentPackageReference::meta_requiredUUID;
	Uml::Attribute ComponentPackageReference::meta_requiredType;
	Uml::Attribute SatisfierProperty::meta_DataValue;
	Uml::Attribute SatisfierProperty::meta_SatisfierPropertyKind;
	Uml::Attribute ImplementationDependency::meta_requiredType;
	Uml::Attribute RequirementSatisfier::meta_resourceType;
	Uml::Attribute RequirementBase::meta_resourceType;
	Uml::Attribute ImplementationRequirement::meta_ResourceUsageKind;
	Uml::Attribute ImplementationRequirement::meta_componentPort;
	Uml::Attribute ImplementationRequirement::meta_resourcePort;
	Uml::Attribute Property::meta_DataValue;
	Uml::Attribute Targets::meta_name;
	Uml::Attribute Domain::meta_label;
	Uml::Attribute Domain::meta_UUID;
	Uml::Attribute Prefixable::meta_PrefixTag;
	Uml::Attribute Taggable::meta_SpecifyIdTag;
	Uml::Attribute Taggable::meta_VersionTag;
	Uml::Attribute OutEventPort::meta_out_event_port_type;
	Uml::Attribute OutEventPort::meta_single_destination;
	Uml::Attribute RequiredRequestPort::meta_multiple_connections;
	Uml::Attribute Component::meta_UUID;
	Uml::Attribute Component::meta_label;
	Uml::Attribute Object::meta_local;
	Uml::Attribute Object::meta_abstract;
	Uml::Attribute ObjectByValue::meta_abstract;
	Uml::Attribute PredefinedTypes::meta_name;
	Uml::Attribute InterfaceDefinitions::meta_name;
	Uml::Attribute Constant::meta_value;
	Uml::Attribute ComponentBuild::meta_name;
	Uml::Attribute ComponentLib::meta_sharedname;
	Uml::Attribute ComponentLib::meta_libraryexport;
	Uml::Attribute ComponentLib::meta_ORBServices;
	Uml::Attribute RootFolder::meta_name;
	Uml::Attribute MgaObject::meta_position;
	Uml::Attribute MgaObject::meta_name;
	Uml::AssociationRole Include_RTEC_Filter_Group_Members_Base::meta_srcInclude_RTEC_Filter_Group, Include_RTEC_Filter_Group_Members_Base::meta_srcInclude_RTEC_Filter_Group_rev;
	Uml::AssociationRole Include_RTEC_Filter_Group_Members_Base::meta_dstInclude_RTEC_Filter_Group, Include_RTEC_Filter_Group_Members_Base::meta_dstInclude_RTEC_Filter_Group_rev;
	Uml::AssociationRole RTEC_Connection::meta_dstRTEC_Connection_end_;
	Uml::AssociationRole RTEC_Connection::meta_srcRTEC_Connection_end_;
	Uml::AssociationRole RTEC_Filter_Group::meta_srcInclude_RT_Info, RTEC_Filter_Group::meta_srcInclude_RT_Info_rev;
	Uml::AssociationRole RTEC_Filter_Group::meta_dstUse_RTEC_Filter, RTEC_Filter_Group::meta_dstUse_RTEC_Filter_rev;
	Uml::AssociationRole Include_RTEC_Filter_Group::meta_dstInclude_RTEC_Filter_Group_end_;
	Uml::AssociationRole Include_RTEC_Filter_Group::meta_srcInclude_RTEC_Filter_Group_end_;
	Uml::AssociationRole Use_RTEC_Filter::meta_srcUse_RTEC_Filter_end_;
	Uml::AssociationRole Use_RTEC_Filter::meta_dstUse_RTEC_Filter_end_;
	Uml::AssociationRole RTEC_Resource_Factory::meta_referedbyRTEC_Resource_Factory_Reference;
	Uml::AssociationRole Include_RT_Info::meta_srcInclude_RT_Info_end_;
	Uml::AssociationRole Include_RT_Info::meta_dstInclude_RT_Info_end_;
	Uml::AssociationRole Use_RT_Info::meta_srcUse_RT_Info_end_;
	Uml::AssociationRole Use_RT_Info::meta_dstUse_RT_Info_end_;
	Uml::AssociationRole RT_Info_Param::meta_dstUse_RT_Info, RT_Info_Param::meta_dstUse_RT_Info_rev;
	Uml::AssociationRole RT_Info_Param::meta_dstInclude_RT_Info, RT_Info_Param::meta_dstInclude_RT_Info_rev;
	Uml::AssociationRole RT_Info_Param::meta_srcRTEC_Connection, RT_Info_Param::meta_srcRTEC_Connection_rev;
	Uml::AssociationRole RTEC_Resource_Factory_Reference::meta_srcUse_RT_Info, RTEC_Resource_Factory_Reference::meta_srcUse_RT_Info_rev;
	Uml::AssociationRole RTEC_Resource_Factory_Reference::meta_ref;
	Uml::AssociationRole MetricConnection::meta_dstMetricConnection_end_;
	Uml::AssociationRole MetricConnection::meta_srcMetricConnection_end_;
	Uml::AssociationRole TimeProbe::meta_srcTimerEventSinkConn, TimeProbe::meta_srcTimerEventSinkConn_rev;
	Uml::AssociationRole TimeProbe::meta_srcTimerConnection, TimeProbe::meta_srcTimerConnection_rev;
	Uml::AssociationRole TimerConnection::meta_dstTimerConnection_end_;
	Uml::AssociationRole TimerConnection::meta_srcTimerConnection_end_;
	Uml::AssociationRole MetricsBase::meta_srcMetricConnection, MetricsBase::meta_srcMetricConnection_rev;
	Uml::AssociationRole MetricsBase::meta_dstWorkloadCharacteristics, MetricsBase::meta_dstWorkloadCharacteristics_rev;
	Uml::AssociationRole OperationRef::meta_dstMetricConnection, OperationRef::meta_dstMetricConnection_rev;
	Uml::AssociationRole OperationRef::meta_dstTimerConnection, OperationRef::meta_dstTimerConnection_rev;
	Uml::AssociationRole OperationRef::meta_ref;
	Uml::AssociationRole EventRef::meta_dstTimerEventSinkConn, EventRef::meta_dstTimerEventSinkConn_rev;
	Uml::AssociationRole EventRef::meta_ref;
	Uml::AssociationRole TimerEventSinkConn::meta_dstTimerEventSinkConn_end_;
	Uml::AssociationRole TimerEventSinkConn::meta_srcTimerEventSinkConn_end_;
	Uml::AssociationRole TaskSet::meta_srcWorkloadCharacteristics, TaskSet::meta_srcWorkloadCharacteristics_rev;
	Uml::AssociationRole TaskSet::meta_members;
	Uml::AssociationRole Task::meta_setTaskSet;
	Uml::AssociationRole WorkloadCharacteristics::meta_srcWorkloadCharacteristics_end_;
	Uml::AssociationRole WorkloadCharacteristics::meta_dstWorkloadCharacteristics_end_;
	Uml::AssociationRole NodeReference::meta_srcInstanceMapping, NodeReference::meta_srcInstanceMapping_rev;
	Uml::AssociationRole NodeReference::meta_ref;
	Uml::AssociationRole Interconnect::meta_dstInterconnect2Node, Interconnect::meta_dstInterconnect2Node_rev;
	Uml::AssociationRole Interconnect::meta_dstInterconnect2Bridge, Interconnect::meta_dstInterconnect2Bridge_rev;
	Uml::AssociationRole Interconnect::meta_srcNode2Interconnect, Interconnect::meta_srcNode2Interconnect_rev;
	Uml::AssociationRole Interconnect::meta_srcBridge2Interconnect, Interconnect::meta_srcBridge2Interconnect_rev;
	Uml::AssociationRole Node::meta_referedbyNodeReference;
	Uml::AssociationRole Node::meta_srcInterconnect2Node, Node::meta_srcInterconnect2Node_rev;
	Uml::AssociationRole Node::meta_dstNode2Interconnect, Node::meta_dstNode2Interconnect_rev;
	Uml::AssociationRole Node::meta_dstShares, Node::meta_dstShares_rev;
	Uml::AssociationRole Bridge::meta_srcInterconnect2Bridge, Bridge::meta_srcInterconnect2Bridge_rev;
	Uml::AssociationRole Bridge::meta_dstBridge2Interconnect, Bridge::meta_dstBridge2Interconnect_rev;
	Uml::AssociationRole SharedResource::meta_srcShares, SharedResource::meta_srcShares_rev;
	Uml::AssociationRole InstanceMapping::meta_dstInstanceMapping_end_;
	Uml::AssociationRole InstanceMapping::meta_srcInstanceMapping_end_;
	Uml::AssociationRole CollocationGroup::meta_dstInstanceMapping, CollocationGroup::meta_dstInstanceMapping_rev;
	Uml::AssociationRole CollocationGroup::meta_members;
	Uml::AssociationRole invoke::meta_cadenaInvoke;
	Uml::AssociationRole invoke::meta_dstinvoke_end_;
	Uml::AssociationRole invoke::meta_srcinvoke_end_;
	Uml::AssociationRole AssemblyselectRequirement::meta_dstAssemblyselectRequirement_end_;
	Uml::AssociationRole AssemblyselectRequirement::meta_srcAssemblyselectRequirement_end_;
	Uml::AssociationRole AssemblyConfigProperty::meta_dstAssemblyConfigProperty_end_;
	Uml::AssociationRole AssemblyConfigProperty::meta_srcAssemblyConfigProperty_end_;
	Uml::AssociationRole PublishConnector::meta_dstAssemblyDeployRequirement, PublishConnector::meta_dstAssemblyDeployRequirement_rev;
	Uml::AssociationRole PublishConnector::meta_dstdeliverTo, PublishConnector::meta_dstdeliverTo_rev;
	Uml::AssociationRole PublishConnector::meta_srcpublish, PublishConnector::meta_srcpublish_rev;
	Uml::AssociationRole PublishConnector::meta_cadenaESSConnection;
	Uml::AssociationRole publish::meta_dstpublish_end_;
	Uml::AssociationRole publish::meta_srcpublish_end_;
	Uml::AssociationRole deliverTo::meta_srcdeliverTo_end_;
	Uml::AssociationRole deliverTo::meta_dstdeliverTo_end_;
	Uml::AssociationRole AssemblyDeployRequirement::meta_srcAssemblyDeployRequirement_end_;
	Uml::AssociationRole AssemblyDeployRequirement::meta_dstAssemblyDeployRequirement_end_;
	Uml::AssociationRole emit::meta_cadenaEmit;
	Uml::AssociationRole emit::meta_dstemit_end_;
	Uml::AssociationRole emit::meta_srcemit_end_;
	Uml::AssociationRole ReceptacleDelegate::meta_dstReceptacleDelegate_end_;
	Uml::AssociationRole ReceptacleDelegate::meta_srcReceptacleDelegate_end_;
	Uml::AssociationRole FacetDelegate::meta_dstFacetDelegate_end_;
	Uml::AssociationRole FacetDelegate::meta_srcFacetDelegate_end_;
	Uml::AssociationRole EventSourceDelegate::meta_dstEventSourceDelegate_end_;
	Uml::AssociationRole EventSourceDelegate::meta_srcEventSourceDelegate_end_;
	Uml::AssociationRole EventSinkDelegate::meta_dstEventSinkDelegate_end_;
	Uml::AssociationRole EventSinkDelegate::meta_srcEventSinkDelegate_end_;
	Uml::AssociationRole ExternalDelegate::meta_dstExternalDelegate_end_;
	Uml::AssociationRole ExternalDelegate::meta_srcExternalDelegate_end_;
	Uml::AssociationRole ComponentAssembly::meta_rootContainer;
	Uml::AssociationRole ComponentAssembly::meta_cadenaScenario;
	Uml::AssociationRole ExternalReferenceEndPoint::meta_srcExternalDelegate, ExternalReferenceEndPoint::meta_srcExternalDelegate_rev;
	Uml::AssociationRole ImplementationCapability::meta_srcImplementationCapability_end_;
	Uml::AssociationRole ImplementationCapability::meta_dstImplementationCapability_end_;
	Uml::AssociationRole ImplementationDependsOn::meta_srcImplementationDependsOn_end_;
	Uml::AssociationRole ImplementationDependsOn::meta_dstImplementationDependsOn_end_;
	Uml::AssociationRole Implements::meta_srcImplements_end_;
	Uml::AssociationRole Implements::meta_dstImplements_end_;
	Uml::AssociationRole ConfigProperty::meta_srcConfigProperty_end_;
	Uml::AssociationRole ConfigProperty::meta_dstConfigProperty_end_;
	Uml::AssociationRole InfoProperty::meta_srcInfoProperty_end_;
	Uml::AssociationRole InfoProperty::meta_dstInfoProperty_end_;
	Uml::AssociationRole MonolithprimaryArtifact::meta_dstMonolithprimaryArtifact_end_;
	Uml::AssociationRole MonolithprimaryArtifact::meta_srcMonolithprimaryArtifact_end_;
	Uml::AssociationRole MonolithDeployRequirement::meta_dstMonolithDeployRequirement_end_;
	Uml::AssociationRole MonolithDeployRequirement::meta_srcMonolithDeployRequirement_end_;
	Uml::AssociationRole MonolithExecParameter::meta_dstMonolithExecParameter_end_;
	Uml::AssociationRole MonolithExecParameter::meta_srcMonolithExecParameter_end_;
	Uml::AssociationRole MonolithicImplementation::meta_dstMonolithprimaryArtifact, MonolithicImplementation::meta_dstMonolithprimaryArtifact_rev;
	Uml::AssociationRole MonolithicImplementation::meta_dstMonolithDeployRequirement, MonolithicImplementation::meta_dstMonolithDeployRequirement_rev;
	Uml::AssociationRole MonolithicImplementation::meta_dstMonolithExecParameter, MonolithicImplementation::meta_dstMonolithExecParameter_rev;
	Uml::AssociationRole ComponentImplementationReference::meta_ref;
	Uml::AssociationRole ComponentImplementationReference::meta_srcImplementation, ComponentImplementationReference::meta_srcImplementation_rev;
	Uml::AssociationRole ComponentImplementation::meta_dstImplementationDependsOn, ComponentImplementation::meta_dstImplementationDependsOn_rev;
	Uml::AssociationRole ComponentImplementation::meta_dstImplementationCapability, ComponentImplementation::meta_dstImplementationCapability_rev;
	Uml::AssociationRole ComponentImplementation::meta_dstImplements, ComponentImplementation::meta_dstImplements_rev;
	Uml::AssociationRole ComponentImplementation::meta_dstConfigProperty, ComponentImplementation::meta_dstConfigProperty_rev;
	Uml::AssociationRole ComponentImplementation::meta_dstInfoProperty, ComponentImplementation::meta_dstInfoProperty_rev;
	Uml::AssociationRole ComponentImplementation::meta_referedbyComponentImplementationReference;
	Uml::AssociationRole PackageConfBasePackage::meta_dstPackageConfBasePackage_end_;
	Uml::AssociationRole PackageConfBasePackage::meta_srcPackageConfBasePackage_end_;
	Uml::AssociationRole PackageConfReference::meta_dstPackageConfReference_end_;
	Uml::AssociationRole PackageConfReference::meta_srcPackageConfReference_end_;
	Uml::AssociationRole PackageConfConfigProperty::meta_dstPackageConfConfigProperty_end_;
	Uml::AssociationRole PackageConfConfigProperty::meta_srcPackageConfConfigProperty_end_;
	Uml::AssociationRole PackageConfSelectRequirement::meta_dstPackageConfSelectRequirement_end_;
	Uml::AssociationRole PackageConfSelectRequirement::meta_srcPackageConfSelectRequirement_end_;
	Uml::AssociationRole PackageConfSpecializedConfig::meta_dstPackageConfSpecializedConfig_end_;
	Uml::AssociationRole PackageConfSpecializedConfig::meta_srcPackageConfSpecializedConfig_end_;
	Uml::AssociationRole PackageConfigurationReference::meta_srcPackageConfSpecializedConfig, PackageConfigurationReference::meta_srcPackageConfSpecializedConfig_rev;
	Uml::AssociationRole PackageConfigurationReference::meta_ref;
	Uml::AssociationRole PackageConfigurationReference::meta_srcpackage, PackageConfigurationReference::meta_srcpackage_rev;
	Uml::AssociationRole PackageConfiguration::meta_dstPackageConfBasePackage, PackageConfiguration::meta_dstPackageConfBasePackage_rev;
	Uml::AssociationRole PackageConfiguration::meta_dstPackageConfReference, PackageConfiguration::meta_dstPackageConfReference_rev;
	Uml::AssociationRole PackageConfiguration::meta_dstPackageConfConfigProperty, PackageConfiguration::meta_dstPackageConfConfigProperty_rev;
	Uml::AssociationRole PackageConfiguration::meta_dstPackageConfSelectRequirement, PackageConfiguration::meta_dstPackageConfSelectRequirement_rev;
	Uml::AssociationRole PackageConfiguration::meta_dstPackageConfSpecializedConfig, PackageConfiguration::meta_dstPackageConfSpecializedConfig_rev;
	Uml::AssociationRole PackageConfiguration::meta_referedbyPackageConfigurationReference;
	Uml::AssociationRole ArtifactDependsOn::meta_dstArtifactDependsOn_end_;
	Uml::AssociationRole ArtifactDependsOn::meta_srcArtifactDependsOn_end_;
	Uml::AssociationRole ArtifactDeployRequirement::meta_dstArtifactDeployRequirement_end_;
	Uml::AssociationRole ArtifactDeployRequirement::meta_srcArtifactDeployRequirement_end_;
	Uml::AssociationRole ArtifactExecParameter::meta_dstArtifactExecParameter_end_;
	Uml::AssociationRole ArtifactExecParameter::meta_srcArtifactExecParameter_end_;
	Uml::AssociationRole ArtifactInfoProperty::meta_dstArtifactInfoProperty_end_;
	Uml::AssociationRole ArtifactInfoProperty::meta_srcArtifactInfoProperty_end_;
	Uml::AssociationRole ImplementationArtifactReference::meta_srcMonolithprimaryArtifact, ImplementationArtifactReference::meta_srcMonolithprimaryArtifact_rev;
	Uml::AssociationRole ImplementationArtifactReference::meta_srcArtifactDependsOn, ImplementationArtifactReference::meta_srcArtifactDependsOn_rev;
	Uml::AssociationRole ImplementationArtifactReference::meta_ref;
	Uml::AssociationRole ImplementationArtifact::meta_dstArtifactDependsOn, ImplementationArtifact::meta_dstArtifactDependsOn_rev;
	Uml::AssociationRole ImplementationArtifact::meta_dstArtifactDeployRequirement, ImplementationArtifact::meta_dstArtifactDeployRequirement_rev;
	Uml::AssociationRole ImplementationArtifact::meta_dstArtifactExecParameter, ImplementationArtifact::meta_dstArtifactExecParameter_rev;
	Uml::AssociationRole ImplementationArtifact::meta_dstArtifactInfoProperty, ImplementationArtifact::meta_dstArtifactInfoProperty_rev;
	Uml::AssociationRole ImplementationArtifact::meta_referedbyImplementationArtifactReference;
	Uml::AssociationRole ImplementationArtifact::meta_referedbyExternalResources;
	Uml::AssociationRole ComponentPropertyDescription::meta_srcComponentProperty, ComponentPropertyDescription::meta_srcComponentProperty_rev;
	Uml::AssociationRole ComponentProperty::meta_dstComponentProperty_end_;
	Uml::AssociationRole ComponentProperty::meta_srcComponentProperty_end_;
	Uml::AssociationRole ComponentConfigProperty::meta_dstComponentConfigProperty_end_;
	Uml::AssociationRole ComponentConfigProperty::meta_srcComponentConfigProperty_end_;
	Uml::AssociationRole ComponentInfoProperty::meta_dstComponentInfoProperty_end_;
	Uml::AssociationRole ComponentInfoProperty::meta_srcComponentInfoProperty_end_;
	Uml::AssociationRole TopLevelPackage::meta_dstpackage, TopLevelPackage::meta_dstpackage_rev;
	Uml::AssociationRole package::meta_dstpackage_end_;
	Uml::AssociationRole package::meta_srcpackage_end_;
	Uml::AssociationRole PackageInterface::meta_dstPackageInterface_end_;
	Uml::AssociationRole PackageInterface::meta_srcPackageInterface_end_;
	Uml::AssociationRole Implementation::meta_dstImplementation_end_;
	Uml::AssociationRole Implementation::meta_srcImplementation_end_;
	Uml::AssociationRole PackageConfigProperty::meta_dstPackageConfigProperty_end_;
	Uml::AssociationRole PackageConfigProperty::meta_srcPackageConfigProperty_end_;
	Uml::AssociationRole PackageInfoProperty::meta_dstPackageInfoProperty_end_;
	Uml::AssociationRole PackageInfoProperty::meta_srcPackageInfoProperty_end_;
	Uml::AssociationRole ComponentPackage::meta_dstImplementation, ComponentPackage::meta_dstImplementation_rev;
	Uml::AssociationRole ComponentPackage::meta_srcPackageConfBasePackage, ComponentPackage::meta_srcPackageConfBasePackage_rev;
	Uml::AssociationRole ComponentPackage::meta_dstPackageInterface, ComponentPackage::meta_dstPackageInterface_rev;
	Uml::AssociationRole ComponentPackage::meta_dstPackageConfigProperty, ComponentPackage::meta_dstPackageConfigProperty_rev;
	Uml::AssociationRole ComponentPackage::meta_dstPackageInfoProperty, ComponentPackage::meta_dstPackageInfoProperty_rev;
	Uml::AssociationRole ComponentPackage::meta_referedbyComponentPackageReference;
	Uml::AssociationRole ComponentPackageReference::meta_srcPackageConfReference, ComponentPackageReference::meta_srcPackageConfReference_rev;
	Uml::AssociationRole ComponentPackageReference::meta_ref;
	Uml::AssociationRole Requirement::meta_srcAssemblyselectRequirement, Requirement::meta_srcAssemblyselectRequirement_rev;
	Uml::AssociationRole Requirement::meta_srcAssemblyDeployRequirement, Requirement::meta_srcAssemblyDeployRequirement_rev;
	Uml::AssociationRole Requirement::meta_srcPackageConfSelectRequirement, Requirement::meta_srcPackageConfSelectRequirement_rev;
	Uml::AssociationRole Requirement::meta_srcArtifactDeployRequirement, Requirement::meta_srcArtifactDeployRequirement_rev;
	Uml::AssociationRole Capability::meta_srcImplementationCapability, Capability::meta_srcImplementationCapability_rev;
	Uml::AssociationRole DataType::meta_ref;
	Uml::AssociationRole ImplementationDependency::meta_srcImplementationDependsOn, ImplementationDependency::meta_srcImplementationDependsOn_rev;
	Uml::AssociationRole ImplementationRequirement::meta_srcMonolithDeployRequirement, ImplementationRequirement::meta_srcMonolithDeployRequirement_rev;
	Uml::AssociationRole Property::meta_srcAssemblyConfigProperty, Property::meta_srcAssemblyConfigProperty_rev;
	Uml::AssociationRole Property::meta_srcConfigProperty, Property::meta_srcConfigProperty_rev;
	Uml::AssociationRole Property::meta_srcInfoProperty, Property::meta_srcInfoProperty_rev;
	Uml::AssociationRole Property::meta_srcMonolithExecParameter, Property::meta_srcMonolithExecParameter_rev;
	Uml::AssociationRole Property::meta_srcPackageConfConfigProperty, Property::meta_srcPackageConfConfigProperty_rev;
	Uml::AssociationRole Property::meta_srcArtifactExecParameter, Property::meta_srcArtifactExecParameter_rev;
	Uml::AssociationRole Property::meta_srcArtifactInfoProperty, Property::meta_srcArtifactInfoProperty_rev;
	Uml::AssociationRole Property::meta_srcComponentConfigProperty, Property::meta_srcComponentConfigProperty_rev;
	Uml::AssociationRole Property::meta_srcComponentInfoProperty, Property::meta_srcComponentInfoProperty_rev;
	Uml::AssociationRole Property::meta_srcPackageConfigProperty, Property::meta_srcPackageConfigProperty_rev;
	Uml::AssociationRole Property::meta_srcPackageInfoProperty, Property::meta_srcPackageInfoProperty_rev;
	Uml::AssociationRole Property::meta_cadenaProperty;
	Uml::AssociationRole Shares::meta_srcShares_end_;
	Uml::AssociationRole Shares::meta_dstShares_end_;
	Uml::AssociationRole Node2Interconnect::meta_dstNode2Interconnect_end_;
	Uml::AssociationRole Node2Interconnect::meta_srcNode2Interconnect_end_;
	Uml::AssociationRole Bridge2Interconnect::meta_dstBridge2Interconnect_end_;
	Uml::AssociationRole Bridge2Interconnect::meta_srcBridge2Interconnect_end_;
	Uml::AssociationRole Interconnect2Node::meta_srcInterconnect2Node_end_;
	Uml::AssociationRole Interconnect2Node::meta_dstInterconnect2Node_end_;
	Uml::AssociationRole Interconnect2Bridge::meta_srcInterconnect2Bridge_end_;
	Uml::AssociationRole Interconnect2Bridge::meta_dstInterconnect2Bridge_end_;
	Uml::AssociationRole OutEventPort::meta_dstRTEC_Connection, OutEventPort::meta_dstRTEC_Connection_rev;
	Uml::AssociationRole OutEventPort::meta_dstpublish, OutEventPort::meta_dstpublish_rev;
	Uml::AssociationRole OutEventPort::meta_dstemit, OutEventPort::meta_dstemit_rev;
	Uml::AssociationRole OutEventPort::meta_srcEventSourceDelegate, OutEventPort::meta_srcEventSourceDelegate_rev;
	Uml::AssociationRole OutEventPort::meta_dstEventSourceDelegate, OutEventPort::meta_dstEventSourceDelegate_rev;
	Uml::AssociationRole OutEventPort::meta_ref;
	Uml::AssociationRole ProvidedRequestPort::meta_srcinvoke, ProvidedRequestPort::meta_srcinvoke_rev;
	Uml::AssociationRole ProvidedRequestPort::meta_srcFacetDelegate, ProvidedRequestPort::meta_srcFacetDelegate_rev;
	Uml::AssociationRole ProvidedRequestPort::meta_dstFacetDelegate, ProvidedRequestPort::meta_dstFacetDelegate_rev;
	Uml::AssociationRole ProvidedRequestPort::meta_ref;
	Uml::AssociationRole RequiredRequestPort::meta_dstinvoke, RequiredRequestPort::meta_dstinvoke_rev;
	Uml::AssociationRole RequiredRequestPort::meta_srcReceptacleDelegate, RequiredRequestPort::meta_srcReceptacleDelegate_rev;
	Uml::AssociationRole RequiredRequestPort::meta_dstReceptacleDelegate, RequiredRequestPort::meta_dstReceptacleDelegate_rev;
	Uml::AssociationRole RequiredRequestPort::meta_ref;
	Uml::AssociationRole InEventPort::meta_srcUse_RTEC_Filter, InEventPort::meta_srcUse_RTEC_Filter_rev;
	Uml::AssociationRole InEventPort::meta_srcdeliverTo, InEventPort::meta_srcdeliverTo_rev;
	Uml::AssociationRole InEventPort::meta_srcemit, InEventPort::meta_srcemit_rev;
	Uml::AssociationRole InEventPort::meta_srcEventSinkDelegate, InEventPort::meta_srcEventSinkDelegate_rev;
	Uml::AssociationRole InEventPort::meta_dstEventSinkDelegate, InEventPort::meta_dstEventSinkDelegate_rev;
	Uml::AssociationRole InEventPort::meta_ref;
	Uml::AssociationRole Port::meta_cadenaPort;
	Uml::AssociationRole Manageable::meta_srcManagesComponent, Manageable::meta_srcManagesComponent_rev;
	Uml::AssociationRole Provideable::meta_referedbyProvidedRequestPort;
	Uml::AssociationRole Provideable::meta_referedbyRequiredRequestPort;
	Uml::AssociationRole ComponentFactory::meta_dstManagesComponent, ComponentFactory::meta_dstManagesComponent_rev;
	Uml::AssociationRole ComponentRef::meta_setCollocationGroup;
	Uml::AssociationRole ComponentRef::meta_srcImplements, ComponentRef::meta_srcImplements_rev;
	Uml::AssociationRole ComponentRef::meta_dstComponentProperty, ComponentRef::meta_dstComponentProperty_rev;
	Uml::AssociationRole ComponentRef::meta_dstComponentConfigProperty, ComponentRef::meta_dstComponentConfigProperty_rev;
	Uml::AssociationRole ComponentRef::meta_dstComponentInfoProperty, ComponentRef::meta_dstComponentInfoProperty_rev;
	Uml::AssociationRole ComponentRef::meta_srcPackageInterface, ComponentRef::meta_srcPackageInterface_rev;
	Uml::AssociationRole ComponentRef::meta_ref;
	Uml::AssociationRole ManagesComponent::meta_dstManagesComponent_end_;
	Uml::AssociationRole ManagesComponent::meta_srcManagesComponent_end_;
	Uml::AssociationRole Component::meta_dstAssemblyselectRequirement, Component::meta_dstAssemblyselectRequirement_rev;
	Uml::AssociationRole Component::meta_dstAssemblyConfigProperty, Component::meta_dstAssemblyConfigProperty_rev;
	Uml::AssociationRole Component::meta_dstExternalDelegate, Component::meta_dstExternalDelegate_rev;
	Uml::AssociationRole Component::meta_referedbyComponentRef;
	Uml::AssociationRole Component::meta_cadenaComponentInstance;
	Uml::AssociationRole OperationBase::meta_referedbyOperationRef;
	Uml::AssociationRole InParameter::meta_ref;
	Uml::AssociationRole InoutParameter::meta_ref;
	Uml::AssociationRole OutParameter::meta_ref;
	Uml::AssociationRole ReturnType::meta_ref;
	Uml::AssociationRole Object::meta_referedbySupports;
	Uml::AssociationRole Inherits::meta_ref;
	Uml::AssociationRole Event::meta_referedbyEventRef;
	Uml::AssociationRole Event::meta_referedbyOutEventPort;
	Uml::AssociationRole Event::meta_referedbyInEventPort;
	Uml::AssociationRole ValueObject::meta_referedbyLookupKey;
	Uml::AssociationRole Supports::meta_ref;
	Uml::AssociationRole LookupKey::meta_ref;
	Uml::AssociationRole SetException::meta_ref;
	Uml::AssociationRole GetException::meta_ref;
	Uml::AssociationRole PrivateFlag::meta_srcMakeMemberPrivate, PrivateFlag::meta_srcMakeMemberPrivate_rev;
	Uml::AssociationRole MakeMemberPrivate::meta_dstMakeMemberPrivate_end_;
	Uml::AssociationRole MakeMemberPrivate::meta_srcMakeMemberPrivate_end_;
	Uml::AssociationRole AttributeMember::meta_ref;
	Uml::AssociationRole Inheritable::meta_referedbyInherits;
	Uml::AssociationRole Label::meta_srcLabelConnection, Label::meta_srcLabelConnection_rev;
	Uml::AssociationRole LabelConnection::meta_dstLabelConnection_end_;
	Uml::AssociationRole LabelConnection::meta_srcLabelConnection_end_;
	Uml::AssociationRole MemberType::meta_referedbyInoutParameter;
	Uml::AssociationRole MemberType::meta_referedbyInParameter;
	Uml::AssociationRole MemberType::meta_referedbyOutParameter;
	Uml::AssociationRole MemberType::meta_referedbyReturnType;
	Uml::AssociationRole MemberType::meta_referedbyAttributeMember;
	Uml::AssociationRole MemberType::meta_referedbyCollection;
	Uml::AssociationRole MemberType::meta_referedbyAlias;
	Uml::AssociationRole MemberType::meta_referedbyBoxed;
	Uml::AssociationRole MemberType::meta_referedbyMember;
	Uml::AssociationRole Discriminator::meta_ref;
	Uml::AssociationRole Collection::meta_ref;
	Uml::AssociationRole Alias::meta_ref;
	Uml::AssociationRole Boxed::meta_ref;
	Uml::AssociationRole Member::meta_ref;
	Uml::AssociationRole Member::meta_dstMakeMemberPrivate, Member::meta_dstMakeMemberPrivate_rev;
	Uml::AssociationRole Member::meta_dstLabelConnection, Member::meta_dstLabelConnection_rev;
	Uml::AssociationRole PredefinedType::meta_referedbyDataType;
	Uml::AssociationRole File::meta_referedbyFileRef;
	Uml::AssociationRole Exception::meta_referedbySetException;
	Uml::AssociationRole Exception::meta_referedbyGetException;
	Uml::AssociationRole Exception::meta_referedbyExceptionRef;
	Uml::AssociationRole ExceptionRef::meta_ref;
	Uml::AssociationRole ConstantType::meta_referedbyDiscriminator;
	Uml::AssociationRole ConstantType::meta_referedbyConstant;
	Uml::AssociationRole Constant::meta_ref;
	Uml::AssociationRole FileRef::meta_ref;
	Uml::AssociationRole Project::meta_setWorkspace;
	Uml::AssociationRole ExternalResources::meta_ref;
	Uml::AssociationRole ExternalResources::meta_srcExtResourceConn, ExternalResources::meta_srcExtResourceConn_rev;
	Uml::AssociationRole ExtResourceConn::meta_dstExtResourceConn_end_;
	Uml::AssociationRole ExtResourceConn::meta_srcExtResourceConn_end_;
	Uml::AssociationRole ComponentLib::meta_dstExtResourceConn, ComponentLib::meta_dstExtResourceConn_rev;
	Uml::AssociationRole Workspaces::meta_members;
	Uml::CompositionParentRole RTEC_Connection::meta_EventChannelConfiguration_parent;
	Uml::CompositionParentRole RTEC_Filter_Group::meta_RTEC_Proxy_Supplier_parent;
	Uml::CompositionParentRole Include_RTEC_Filter_Group::meta_RTEC_Proxy_Supplier_parent;
	Uml::CompositionParentRole Use_RTEC_Filter::meta_EventChannelConfiguration_parent;
	Uml::CompositionParentRole EventChannelConfiguration::meta_EventChannelConfig_parent;
	Uml::CompositionParentRole RTEC_Resource_Factory::meta_EventChannelConfiguration_parent;
	Uml::CompositionParentRole Include_RT_Info::meta_RTEC_Proxy_Supplier_parent;
	Uml::CompositionParentRole Use_RT_Info::meta_RTEC_Proxy_Consumer_parent;
	Uml::CompositionParentRole Use_RT_Info::meta_EventChannelConfiguration_parent;
	Uml::CompositionParentRole RT_Info_Param::meta_RTEC_Proxy_parent;
	Uml::CompositionParentRole RTEC_Resource_Factory_Reference::meta_RTEC_Proxy_Consumer_parent;
	Uml::CompositionParentRole EventChannelConfig::meta_RootFolder_parent;
	Uml::CompositionParentRole Proxy::meta_EventChannelConfiguration_parent;
	Uml::CompositionParentRole MetricConnection::meta_BenchmarkAnalysis_parent;
	Uml::CompositionParentRole DataAnalysisBase::meta_MetricsBase_parent;
	Uml::CompositionParentRole DataAnalysisBase::meta_BenchmarkAnalysis_parent;
	Uml::CompositionParentRole TimeProbe::meta_BenchmarkAnalysis_parent;
	Uml::CompositionParentRole TimerConnection::meta_BenchmarkAnalysis_parent;
	Uml::CompositionParentRole ComponentAnalyses::meta_RootFolder_parent;
	Uml::CompositionParentRole BenchmarkAnalysis::meta_ComponentAnalyses_parent;
	Uml::CompositionParentRole MetricsBase::meta_BenchmarkAnalysis_parent;
	Uml::CompositionParentRole OperationRef::meta_BenchmarkAnalysis_parent;
	Uml::CompositionParentRole EventRef::meta_BenchmarkAnalysis_parent;
	Uml::CompositionParentRole TimerEventSinkConn::meta_BenchmarkAnalysis_parent;
	Uml::CompositionParentRole TaskSet::meta_BenchmarkAnalysis_parent;
	Uml::CompositionParentRole Task::meta_BenchmarkAnalysis_parent;
	Uml::CompositionParentRole WorkloadCharacteristics::meta_BenchmarkAnalysis_parent;
	Uml::CompositionParentRole NodeReference::meta_DeploymentPlan_parent;
	Uml::CompositionParentRole Resource::meta_Node_parent;
	Uml::CompositionParentRole Resource::meta_Interconnect_parent;
	Uml::CompositionParentRole Resource::meta_Bridge_parent;
	Uml::CompositionParentRole InstanceMapping::meta_DeploymentPlan_parent;
	Uml::CompositionParentRole DeploymentPlans::meta_RootFolder_parent;
	Uml::CompositionParentRole CollocationGroup::meta_DeploymentPlan_parent;
	Uml::CompositionParentRole DeploymentPlan::meta_DeploymentPlans_parent;
	Uml::CompositionParentRole invoke::meta_ComponentAssembly_parent;
	Uml::CompositionParentRole AssemblyselectRequirement::meta_ComponentAssembly_parent;
	Uml::CompositionParentRole AssemblyConfigProperty::meta_ComponentAssembly_parent;
	Uml::CompositionParentRole PublishConnector::meta_ComponentAssembly_parent;
	Uml::CompositionParentRole publish::meta_ComponentAssembly_parent;
	Uml::CompositionParentRole deliverTo::meta_ComponentAssembly_parent;
	Uml::CompositionParentRole AssemblyDeployRequirement::meta_ComponentAssembly_parent;
	Uml::CompositionParentRole emit::meta_ComponentAssembly_parent;
	Uml::CompositionParentRole ReceptacleDelegate::meta_ComponentAssembly_parent;
	Uml::CompositionParentRole FacetDelegate::meta_ComponentAssembly_parent;
	Uml::CompositionParentRole EventSourceDelegate::meta_ComponentAssembly_parent;
	Uml::CompositionParentRole EventSinkDelegate::meta_ComponentAssembly_parent;
	Uml::CompositionParentRole ExternalDelegate::meta_ComponentAssembly_parent;
	Uml::CompositionParentRole ComponentAssembly::meta_ComponentAssembly_parent;
	Uml::CompositionParentRole ExternalReferenceEndPoint::meta_ComponentAssembly_parent;
	Uml::CompositionParentRole ImplementationCapability::meta_ComponentImplementationContainer_parent;
	Uml::CompositionParentRole ImplementationDependsOn::meta_ComponentImplementationContainer_parent;
	Uml::CompositionParentRole Implements::meta_ComponentImplementationContainer_parent;
	Uml::CompositionParentRole ComponentImplementations::meta_RootFolder_parent;
	Uml::CompositionParentRole ConfigProperty::meta_ComponentImplementationContainer_parent;
	Uml::CompositionParentRole InfoProperty::meta_ComponentImplementationContainer_parent;
	Uml::CompositionParentRole MonolithprimaryArtifact::meta_ComponentImplementationContainer_parent;
	Uml::CompositionParentRole MonolithDeployRequirement::meta_ComponentImplementationContainer_parent;
	Uml::CompositionParentRole ComponentImplementationContainer::meta_ComponentImplementations_parent;
	Uml::CompositionParentRole MonolithExecParameter::meta_ComponentImplementationContainer_parent;
	Uml::CompositionParentRole ComponentImplementationReference::meta_PackageContainer_parent;
	Uml::CompositionParentRole ComponentImplementation::meta_ComponentImplementationContainer_parent;
	Uml::CompositionParentRole PackageConfBasePackage::meta_PackageConfigurationContainer_parent;
	Uml::CompositionParentRole PackageConfReference::meta_PackageConfigurationContainer_parent;
	Uml::CompositionParentRole PackageConfigurations::meta_RootFolder_parent;
	Uml::CompositionParentRole PackageConfigurationContainer::meta_PackageConfigurations_parent;
	Uml::CompositionParentRole PackageConfConfigProperty::meta_PackageConfigurationContainer_parent;
	Uml::CompositionParentRole PackageConfSelectRequirement::meta_PackageConfigurationContainer_parent;
	Uml::CompositionParentRole PackageConfSpecializedConfig::meta_PackageConfigurationContainer_parent;
	Uml::CompositionParentRole PackageConfigurationReference::meta_TopLevelPackageContainer_parent;
	Uml::CompositionParentRole PackageConfiguration::meta_PackageConfigurationContainer_parent;
	Uml::CompositionParentRole ImplementationArtifacts::meta_RootFolder_parent;
	Uml::CompositionParentRole ArtifactDependsOn::meta_ArtifactContainer_parent;
	Uml::CompositionParentRole ArtifactDeployRequirement::meta_ArtifactContainer_parent;
	Uml::CompositionParentRole ArtifactContainer::meta_ImplementationArtifacts_parent;
	Uml::CompositionParentRole ArtifactExecParameter::meta_ArtifactContainer_parent;
	Uml::CompositionParentRole ArtifactInfoProperty::meta_ArtifactContainer_parent;
	Uml::CompositionParentRole ImplementationArtifactReference::meta_ComponentImplementationContainer_parent;
	Uml::CompositionParentRole ImplementationArtifactReference::meta_ArtifactContainer_parent;
	Uml::CompositionParentRole ImplementationArtifactReference::meta_StubProject_parent;
	Uml::CompositionParentRole ImplementationArtifactReference::meta_ServantProject_parent;
	Uml::CompositionParentRole ImplementationArtifactReference::meta_ExecutorProject_parent;
	Uml::CompositionParentRole ImplementationArtifact::meta_ArtifactContainer_parent;
	Uml::CompositionParentRole ImplementationArtifact::meta_Project_parent;
	Uml::CompositionParentRole ComponentTypes::meta_RootFolder_parent;
	Uml::CompositionParentRole ComponentPropertyDescription::meta_ComponentContainer_parent;
	Uml::CompositionParentRole ComponentProperty::meta_ComponentContainer_parent;
	Uml::CompositionParentRole ComponentContainer::meta_ComponentTypes_parent;
	Uml::CompositionParentRole ComponentConfigProperty::meta_ComponentContainer_parent;
	Uml::CompositionParentRole ComponentInfoProperty::meta_ComponentContainer_parent;
	Uml::CompositionParentRole TopLevelPackageContainer::meta_TopLevelPackages_parent;
	Uml::CompositionParentRole TopLevelPackages::meta_RootFolder_parent;
	Uml::CompositionParentRole TopLevelPackage::meta_TopLevelPackageContainer_parent;
	Uml::CompositionParentRole package::meta_TopLevelPackageContainer_parent;
	Uml::CompositionParentRole ComponentPackages::meta_RootFolder_parent;
	Uml::CompositionParentRole PackageInterface::meta_PackageContainer_parent;
	Uml::CompositionParentRole Implementation::meta_PackageContainer_parent;
	Uml::CompositionParentRole PackageContainer::meta_ComponentPackages_parent;
	Uml::CompositionParentRole PackageConfigProperty::meta_PackageContainer_parent;
	Uml::CompositionParentRole PackageInfoProperty::meta_PackageContainer_parent;
	Uml::CompositionParentRole ComponentPackage::meta_ComponentAssembly_parent;
	Uml::CompositionParentRole ComponentPackage::meta_PackageConfigurationContainer_parent;
	Uml::CompositionParentRole ComponentPackage::meta_PackageContainer_parent;
	Uml::CompositionParentRole ComponentPackageReference::meta_ComponentAssembly_parent;
	Uml::CompositionParentRole ComponentPackageReference::meta_PackageConfigurationContainer_parent;
	Uml::CompositionParentRole Requirement::meta_ComponentAssembly_parent;
	Uml::CompositionParentRole Requirement::meta_PackageConfigurationContainer_parent;
	Uml::CompositionParentRole Requirement::meta_ArtifactContainer_parent;
	Uml::CompositionParentRole Capability::meta_ComponentImplementationContainer_parent;
	Uml::CompositionParentRole DataType::meta_SatisfierProperty_parent;
	Uml::CompositionParentRole DataType::meta_ComponentPropertyDescription_parent;
	Uml::CompositionParentRole DataType::meta_Property_parent;
	Uml::CompositionParentRole SatisfierProperty::meta_SharedResource_parent;
	Uml::CompositionParentRole SatisfierProperty::meta_Resource_parent;
	Uml::CompositionParentRole SatisfierProperty::meta_RequirementSatisfier_parent;
	Uml::CompositionParentRole ImplementationDependency::meta_ComponentImplementationContainer_parent;
	Uml::CompositionParentRole ImplementationRequirement::meta_ComponentImplementationContainer_parent;
	Uml::CompositionParentRole Property::meta_ComponentAssembly_parent;
	Uml::CompositionParentRole Property::meta_ComponentImplementationContainer_parent;
	Uml::CompositionParentRole Property::meta_PackageConfigurationContainer_parent;
	Uml::CompositionParentRole Property::meta_ArtifactContainer_parent;
	Uml::CompositionParentRole Property::meta_ComponentContainer_parent;
	Uml::CompositionParentRole Property::meta_PackageContainer_parent;
	Uml::CompositionParentRole Property::meta_RequirementBase_parent;
	Uml::CompositionParentRole Property::meta_Domain_parent;
	Uml::CompositionParentRole Elements::meta_Domain_parent;
	Uml::CompositionParentRole Targets::meta_RootFolder_parent;
	Uml::CompositionParentRole Shares::meta_Domain_parent;
	Uml::CompositionParentRole Node2Interconnect::meta_Domain_parent;
	Uml::CompositionParentRole Bridge2Interconnect::meta_Domain_parent;
	Uml::CompositionParentRole Interconnect2Node::meta_Domain_parent;
	Uml::CompositionParentRole Interconnect2Bridge::meta_Domain_parent;
	Uml::CompositionParentRole Domain::meta_Targets_parent;
	Uml::CompositionParentRole OutEventPort::meta_ComponentAssembly_parent;
	Uml::CompositionParentRole ProvidedRequestPort::meta_ComponentAssembly_parent;
	Uml::CompositionParentRole RequiredRequestPort::meta_ComponentAssembly_parent;
	Uml::CompositionParentRole InEventPort::meta_ComponentAssembly_parent;
	Uml::CompositionParentRole Port::meta_EventChannelConfiguration_parent;
	Uml::CompositionParentRole Port::meta_Component_parent;
	Uml::CompositionParentRole ComponentRef::meta_EventChannelConfiguration_parent;
	Uml::CompositionParentRole ComponentRef::meta_DeploymentPlan_parent;
	Uml::CompositionParentRole ComponentRef::meta_ComponentImplementationContainer_parent;
	Uml::CompositionParentRole ComponentRef::meta_ComponentContainer_parent;
	Uml::CompositionParentRole ComponentRef::meta_PackageContainer_parent;
	Uml::CompositionParentRole ComponentRef::meta_Package_parent;
	Uml::CompositionParentRole ComponentRef::meta_File_parent;
	Uml::CompositionParentRole ManagesComponent::meta_File_parent;
	Uml::CompositionParentRole ManagesComponent::meta_Package_parent;
	Uml::CompositionParentRole Component::meta_ComponentAssembly_parent;
	Uml::CompositionParentRole OnewayOperation::meta_HasOperations_parent;
	Uml::CompositionParentRole LookupOperation::meta_ComponentFactory_parent;
	Uml::CompositionParentRole FactoryOperation::meta_ComponentFactory_parent;
	Uml::CompositionParentRole FactoryOperation::meta_ObjectByValue_parent;
	Uml::CompositionParentRole OperationBase::meta_BenchmarkAnalysis_parent;
	Uml::CompositionParentRole InParameter::meta_OperationBase_parent;
	Uml::CompositionParentRole InoutParameter::meta_TwowayOperation_parent;
	Uml::CompositionParentRole OutParameter::meta_TwowayOperation_parent;
	Uml::CompositionParentRole ReturnType::meta_TwowayOperation_parent;
	Uml::CompositionParentRole TwowayOperation::meta_HasOperations_parent;
	Uml::CompositionParentRole Inherits::meta_Inheritable_parent;
	Uml::CompositionParentRole Event::meta_BenchmarkAnalysis_parent;
	Uml::CompositionParentRole Supports::meta_SupportsInterfaces_parent;
	Uml::CompositionParentRole ReadonlyAttribute::meta_Inheritable_parent;
	Uml::CompositionParentRole ReadonlyAttribute::meta_Component_parent;
	Uml::CompositionParentRole LookupKey::meta_ComponentFactory_parent;
	Uml::CompositionParentRole SetException::meta_Attribute_parent;
	Uml::CompositionParentRole GetException::meta_ReadonlyAttribute_parent;
	Uml::CompositionParentRole PrivateFlag::meta_ObjectByValue_parent;
	Uml::CompositionParentRole MakeMemberPrivate::meta_ObjectByValue_parent;
	Uml::CompositionParentRole AttributeMember::meta_ReadonlyAttribute_parent;
	Uml::CompositionParentRole EnumValue::meta_Enum_parent;
	Uml::CompositionParentRole Label::meta_SwitchedAggregate_parent;
	Uml::CompositionParentRole LabelConnection::meta_SwitchedAggregate_parent;
	Uml::CompositionParentRole Discriminator::meta_SwitchedAggregate_parent;
	Uml::CompositionParentRole NoInheritable::meta_HasOperations_parent;
	Uml::CompositionParentRole Member::meta_ObjectByValue_parent;
	Uml::CompositionParentRole Member::meta_SwitchedAggregate_parent;
	Uml::CompositionParentRole Member::meta_Aggregate_parent;
	Uml::CompositionParentRole Member::meta_Exception_parent;
	Uml::CompositionParentRole NamedType::meta_File_parent;
	Uml::CompositionParentRole NamedType::meta_Package_parent;
	Uml::CompositionParentRole PredefinedType::meta_PredefinedTypes_parent;
	Uml::CompositionParentRole PredefinedTypes::meta_RootFolder_parent;
	Uml::CompositionParentRole Package::meta_Package_parent;
	Uml::CompositionParentRole Package::meta_File_parent;
	Uml::CompositionParentRole File::meta_InterfaceDefinitions_parent;
	Uml::CompositionParentRole Exception::meta_HasOperations_parent;
	Uml::CompositionParentRole Exception::meta_Package_parent;
	Uml::CompositionParentRole Exception::meta_File_parent;
	Uml::CompositionParentRole ExceptionRef::meta_HasExceptions_parent;
	Uml::CompositionParentRole InterfaceDefinitions::meta_RootFolder_parent;
	Uml::CompositionParentRole Constant::meta_HasOperations_parent;
	Uml::CompositionParentRole Constant::meta_Package_parent;
	Uml::CompositionParentRole Constant::meta_File_parent;
	Uml::CompositionParentRole FileRef::meta_File_parent;
	Uml::CompositionParentRole FileRef::meta_StubProject_parent;
	Uml::CompositionParentRole ComponentBuild::meta_RootFolder_parent;
	Uml::CompositionParentRole Project::meta_MPC_parent;
	Uml::CompositionParentRole ExternalResources::meta_Project_parent;
	Uml::CompositionParentRole ExtResourceConn::meta_Project_parent;
	Uml::CompositionParentRole ComponentLib::meta_Project_parent;
	Uml::CompositionParentRole MPC::meta_ComponentBuild_parent;
	Uml::CompositionParentRole Workspaces::meta_MPC_parent;
	Uml::CompositionParentRole RootFolder::meta_RootFolder_parent;
	Uml::CompositionChildRole RTEC_Proxy::meta_RT_Info_Param_children;
	Uml::CompositionChildRole RTEC_Proxy_Supplier::meta_Include_RT_Info_children;
	Uml::CompositionChildRole RTEC_Proxy_Supplier::meta_Include_RTEC_Filter_Group_children;
	Uml::CompositionChildRole RTEC_Proxy_Supplier::meta_RTEC_Filter_Group_children;
	Uml::CompositionChildRole RTEC_Proxy_Consumer::meta_RTEC_Resource_Factory_Reference_child;
	Uml::CompositionChildRole RTEC_Proxy_Consumer::meta_Use_RT_Info_children;
	Uml::CompositionChildRole EventChannelConfiguration::meta_Use_RT_Info_children;
	Uml::CompositionChildRole EventChannelConfiguration::meta_RTEC_Resource_Factory_children;
	Uml::CompositionChildRole EventChannelConfiguration::meta_Use_RTEC_Filter_children;
	Uml::CompositionChildRole EventChannelConfiguration::meta_RTEC_Connection_children;
	Uml::CompositionChildRole EventChannelConfiguration::meta_Proxy_children;
	Uml::CompositionChildRole EventChannelConfiguration::meta_ComponentRef_children;
	Uml::CompositionChildRole EventChannelConfiguration::meta_Port_children;
	Uml::CompositionChildRole EventChannelConfig::meta_EventChannelConfiguration_children;
	Uml::CompositionChildRole ComponentAnalyses::meta_BenchmarkAnalysis_children;
	Uml::CompositionChildRole BenchmarkAnalysis::meta_TimerConnection_children;
	Uml::CompositionChildRole BenchmarkAnalysis::meta_OperationBase_children;
	Uml::CompositionChildRole BenchmarkAnalysis::meta_MetricConnection_children;
	Uml::CompositionChildRole BenchmarkAnalysis::meta_DataAnalysisBase_children;
	Uml::CompositionChildRole BenchmarkAnalysis::meta_MetricsBase_children;
	Uml::CompositionChildRole BenchmarkAnalysis::meta_TimeProbe_children;
	Uml::CompositionChildRole BenchmarkAnalysis::meta_OperationRef_children;
	Uml::CompositionChildRole BenchmarkAnalysis::meta_EventRef_children;
	Uml::CompositionChildRole BenchmarkAnalysis::meta_Event_children;
	Uml::CompositionChildRole BenchmarkAnalysis::meta_TimerEventSinkConn_children;
	Uml::CompositionChildRole BenchmarkAnalysis::meta_TaskSet_children;
	Uml::CompositionChildRole BenchmarkAnalysis::meta_Task_children;
	Uml::CompositionChildRole BenchmarkAnalysis::meta_WorkloadCharacteristics_children;
	Uml::CompositionChildRole MetricsBase::meta_DataAnalysisBase_children;
	Uml::CompositionChildRole Interconnect::meta_Resource_children;
	Uml::CompositionChildRole Node::meta_Resource_children;
	Uml::CompositionChildRole Resource::meta_SatisfierProperty_children;
	Uml::CompositionChildRole Bridge::meta_Resource_children;
	Uml::CompositionChildRole SharedResource::meta_SatisfierProperty_children;
	Uml::CompositionChildRole DeploymentPlans::meta_DeploymentPlan_children;
	Uml::CompositionChildRole DeploymentPlan::meta_InstanceMapping_children;
	Uml::CompositionChildRole DeploymentPlan::meta_NodeReference_children;
	Uml::CompositionChildRole DeploymentPlan::meta_CollocationGroup_children;
	Uml::CompositionChildRole DeploymentPlan::meta_ComponentRef_children;
	Uml::CompositionChildRole ComponentAssembly::meta_invoke_children;
	Uml::CompositionChildRole ComponentAssembly::meta_ComponentPackage_children;
	Uml::CompositionChildRole ComponentAssembly::meta_AssemblyselectRequirement_children;
	Uml::CompositionChildRole ComponentAssembly::meta_Property_children;
	Uml::CompositionChildRole ComponentAssembly::meta_ComponentPackageReference_children;
	Uml::CompositionChildRole ComponentAssembly::meta_deliverTo_children;
	Uml::CompositionChildRole ComponentAssembly::meta_AssemblyDeployRequirement_children;
	Uml::CompositionChildRole ComponentAssembly::meta_Requirement_children;
	Uml::CompositionChildRole ComponentAssembly::meta_publish_children;
	Uml::CompositionChildRole ComponentAssembly::meta_PublishConnector_children;
	Uml::CompositionChildRole ComponentAssembly::meta_emit_children;
	Uml::CompositionChildRole ComponentAssembly::meta_Component_children;
	Uml::CompositionChildRole ComponentAssembly::meta_AssemblyConfigProperty_children;
	Uml::CompositionChildRole ComponentAssembly::meta_ComponentAssembly_children;
	Uml::CompositionChildRole ComponentAssembly::meta_ProvidedRequestPort_children;
	Uml::CompositionChildRole ComponentAssembly::meta_RequiredRequestPort_children;
	Uml::CompositionChildRole ComponentAssembly::meta_InEventPort_children;
	Uml::CompositionChildRole ComponentAssembly::meta_OutEventPort_children;
	Uml::CompositionChildRole ComponentAssembly::meta_EventSinkDelegate_children;
	Uml::CompositionChildRole ComponentAssembly::meta_EventSourceDelegate_children;
	Uml::CompositionChildRole ComponentAssembly::meta_FacetDelegate_children;
	Uml::CompositionChildRole ComponentAssembly::meta_ReceptacleDelegate_children;
	Uml::CompositionChildRole ComponentAssembly::meta_ExternalReferenceEndPoint_children;
	Uml::CompositionChildRole ComponentAssembly::meta_ExternalDelegate_children;
	Uml::CompositionChildRole ComponentImplementations::meta_ComponentImplementationContainer_children;
	Uml::CompositionChildRole ComponentImplementationContainer::meta_ImplementationDependsOn_children;
	Uml::CompositionChildRole ComponentImplementationContainer::meta_ComponentImplementation_child;
	Uml::CompositionChildRole ComponentImplementationContainer::meta_Implements_children;
	Uml::CompositionChildRole ComponentImplementationContainer::meta_ComponentRef_children;
	Uml::CompositionChildRole ComponentImplementationContainer::meta_MonolithDeployRequirement_children;
	Uml::CompositionChildRole ComponentImplementationContainer::meta_MonolithprimaryArtifact_children;
	Uml::CompositionChildRole ComponentImplementationContainer::meta_MonolithExecParameter_children;
	Uml::CompositionChildRole ComponentImplementationContainer::meta_ImplementationDependency_children;
	Uml::CompositionChildRole ComponentImplementationContainer::meta_ConfigProperty_children;
	Uml::CompositionChildRole ComponentImplementationContainer::meta_ImplementationCapability_children;
	Uml::CompositionChildRole ComponentImplementationContainer::meta_InfoProperty_children;
	Uml::CompositionChildRole ComponentImplementationContainer::meta_Property_children;
	Uml::CompositionChildRole ComponentImplementationContainer::meta_Capability_children;
	Uml::CompositionChildRole ComponentImplementationContainer::meta_ImplementationRequirement_children;
	Uml::CompositionChildRole ComponentImplementationContainer::meta_ImplementationArtifactReference_children;
	Uml::CompositionChildRole PackageConfigurations::meta_PackageConfigurationContainer_children;
	Uml::CompositionChildRole PackageConfigurationContainer::meta_PackageConfReference_child;
	Uml::CompositionChildRole PackageConfigurationContainer::meta_PackageConfBasePackage_child;
	Uml::CompositionChildRole PackageConfigurationContainer::meta_ComponentPackage_child;
	Uml::CompositionChildRole PackageConfigurationContainer::meta_ComponentPackageReference_child;
	Uml::CompositionChildRole PackageConfigurationContainer::meta_PackageConfConfigProperty_children;
	Uml::CompositionChildRole PackageConfigurationContainer::meta_PackageConfSelectRequirement_children;
	Uml::CompositionChildRole PackageConfigurationContainer::meta_Requirement_children;
	Uml::CompositionChildRole PackageConfigurationContainer::meta_Property_children;
	Uml::CompositionChildRole PackageConfigurationContainer::meta_PackageConfSpecializedConfig_child;
	Uml::CompositionChildRole PackageConfigurationContainer::meta_PackageConfiguration_children;
	Uml::CompositionChildRole ImplementationArtifacts::meta_ArtifactContainer_children;
	Uml::CompositionChildRole ArtifactContainer::meta_ImplementationArtifactReference_children;
	Uml::CompositionChildRole ArtifactContainer::meta_ArtifactDependsOn_children;
	Uml::CompositionChildRole ArtifactContainer::meta_ArtifactDeployRequirement_children;
	Uml::CompositionChildRole ArtifactContainer::meta_Requirement_children;
	Uml::CompositionChildRole ArtifactContainer::meta_ArtifactExecParameter_children;
	Uml::CompositionChildRole ArtifactContainer::meta_ArtifactInfoProperty_children;
	Uml::CompositionChildRole ArtifactContainer::meta_Property_children;
	Uml::CompositionChildRole ArtifactContainer::meta_ImplementationArtifact_children;
	Uml::CompositionChildRole ComponentTypes::meta_ComponentContainer_children;
	Uml::CompositionChildRole ComponentPropertyDescription::meta_DataType_child;
	Uml::CompositionChildRole ComponentContainer::meta_ComponentConfigProperty_children;
	Uml::CompositionChildRole ComponentContainer::meta_Property_children;
	Uml::CompositionChildRole ComponentContainer::meta_ComponentProperty_children;
	Uml::CompositionChildRole ComponentContainer::meta_ComponentPropertyDescription_children;
	Uml::CompositionChildRole ComponentContainer::meta_ComponentRef_child;
	Uml::CompositionChildRole ComponentContainer::meta_ComponentInfoProperty_children;
	Uml::CompositionChildRole TopLevelPackageContainer::meta_TopLevelPackage_child;
	Uml::CompositionChildRole TopLevelPackageContainer::meta_PackageConfigurationReference_child;
	Uml::CompositionChildRole TopLevelPackageContainer::meta_package_child;
	Uml::CompositionChildRole TopLevelPackages::meta_TopLevelPackageContainer_children;
	Uml::CompositionChildRole ComponentPackages::meta_PackageContainer_children;
	Uml::CompositionChildRole PackageContainer::meta_Implementation_children;
	Uml::CompositionChildRole PackageContainer::meta_PackageInterface_child;
	Uml::CompositionChildRole PackageContainer::meta_ComponentImplementationReference_children;
	Uml::CompositionChildRole PackageContainer::meta_ComponentRef_children;
	Uml::CompositionChildRole PackageContainer::meta_PackageConfigProperty_children;
	Uml::CompositionChildRole PackageContainer::meta_Property_children;
	Uml::CompositionChildRole PackageContainer::meta_PackageInfoProperty_children;
	Uml::CompositionChildRole PackageContainer::meta_ComponentPackage_child;
	Uml::CompositionChildRole SatisfierProperty::meta_DataType_child;
	Uml::CompositionChildRole RequirementSatisfier::meta_SatisfierProperty_children;
	Uml::CompositionChildRole RequirementBase::meta_Property_children;
	Uml::CompositionChildRole Property::meta_DataType_child;
	Uml::CompositionChildRole Targets::meta_Domain_children;
	Uml::CompositionChildRole Domain::meta_Property_children;
	Uml::CompositionChildRole Domain::meta_Elements_children;
	Uml::CompositionChildRole Domain::meta_Shares_children;
	Uml::CompositionChildRole Domain::meta_Interconnect2Node_children;
	Uml::CompositionChildRole Domain::meta_Node2Interconnect_children;
	Uml::CompositionChildRole Domain::meta_Interconnect2Bridge_children;
	Uml::CompositionChildRole Domain::meta_Bridge2Interconnect_children;
	Uml::CompositionChildRole ComponentFactory::meta_LookupKey_child;
	Uml::CompositionChildRole ComponentFactory::meta_FactoryOperation_children;
	Uml::CompositionChildRole ComponentFactory::meta_LookupOperation_children;
	Uml::CompositionChildRole Component::meta_ReadonlyAttribute_children;
	Uml::CompositionChildRole Component::meta_Port_children;
	Uml::CompositionChildRole OperationBase::meta_InParameter_children;
	Uml::CompositionChildRole HasExceptions::meta_ExceptionRef_children;
	Uml::CompositionChildRole TwowayOperation::meta_OutParameter_children;
	Uml::CompositionChildRole TwowayOperation::meta_InoutParameter_children;
	Uml::CompositionChildRole TwowayOperation::meta_ReturnType_child;
	Uml::CompositionChildRole HasOperations::meta_TwowayOperation_children;
	Uml::CompositionChildRole HasOperations::meta_OnewayOperation_children;
	Uml::CompositionChildRole HasOperations::meta_Exception_children;
	Uml::CompositionChildRole HasOperations::meta_Constant_children;
	Uml::CompositionChildRole HasOperations::meta_NoInheritable_children;
	Uml::CompositionChildRole SupportsInterfaces::meta_Supports_children;
	Uml::CompositionChildRole ReadonlyAttribute::meta_AttributeMember_child;
	Uml::CompositionChildRole ReadonlyAttribute::meta_GetException_children;
	Uml::CompositionChildRole Attribute::meta_SetException_children;
	Uml::CompositionChildRole ObjectByValue::meta_PrivateFlag_children;
	Uml::CompositionChildRole ObjectByValue::meta_FactoryOperation_children;
	Uml::CompositionChildRole ObjectByValue::meta_MakeMemberPrivate_children;
	Uml::CompositionChildRole ObjectByValue::meta_Member_children;
	Uml::CompositionChildRole Inheritable::meta_ReadonlyAttribute_children;
	Uml::CompositionChildRole Inheritable::meta_Inherits_children;
	Uml::CompositionChildRole Aggregate::meta_Member_children;
	Uml::CompositionChildRole SwitchedAggregate::meta_Label_children;
	Uml::CompositionChildRole SwitchedAggregate::meta_LabelConnection_children;
	Uml::CompositionChildRole SwitchedAggregate::meta_Member_children;
	Uml::CompositionChildRole SwitchedAggregate::meta_Discriminator_child;
	Uml::CompositionChildRole Enum::meta_EnumValue_children;
	Uml::CompositionChildRole PredefinedTypes::meta_PredefinedType_children;
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
	Uml::CompositionChildRole ComponentBuild::meta_MPC_children;
	Uml::CompositionChildRole Project::meta_ExtResourceConn_children;
	Uml::CompositionChildRole Project::meta_ExternalResources_children;
	Uml::CompositionChildRole Project::meta_ComponentLib_children;
	Uml::CompositionChildRole Project::meta_ImplementationArtifact_children;
	Uml::CompositionChildRole MPC::meta_Project_children;
	Uml::CompositionChildRole MPC::meta_Workspaces_children;
	Uml::CompositionChildRole StubProject::meta_FileRef_children;
	Uml::CompositionChildRole StubProject::meta_ImplementationArtifactReference_child;
	Uml::CompositionChildRole ServantProject::meta_ImplementationArtifactReference_child;
	Uml::CompositionChildRole ExecutorProject::meta_ImplementationArtifactReference_child;
	Uml::CompositionChildRole RootFolder::meta_ComponentAnalyses_children;
	Uml::CompositionChildRole RootFolder::meta_EventChannelConfig_children;
	Uml::CompositionChildRole RootFolder::meta_DeploymentPlans_children;
	Uml::CompositionChildRole RootFolder::meta_ComponentImplementations_children;
	Uml::CompositionChildRole RootFolder::meta_PackageConfigurations_children;
	Uml::CompositionChildRole RootFolder::meta_ImplementationArtifacts_children;
	Uml::CompositionChildRole RootFolder::meta_ComponentTypes_children;
	Uml::CompositionChildRole RootFolder::meta_TopLevelPackages_children;
	Uml::CompositionChildRole RootFolder::meta_ComponentPackages_children;
	Uml::CompositionChildRole RootFolder::meta_Targets_children;
	Uml::CompositionChildRole RootFolder::meta_PredefinedTypes_children;
	Uml::CompositionChildRole RootFolder::meta_InterfaceDefinitions_children;
	Uml::CompositionChildRole RootFolder::meta_ComponentBuild_children;
	Uml::CompositionChildRole RootFolder::meta_RootFolder_children;
	void Creates()
	{
		RTEC_Proxy::meta = Uml::Class::Create(umldiagram);
		Include_RTEC_Filter_Group_Members_Base::meta = Uml::Class::Create(umldiagram);
		RTEC_Proxy_Supplier::meta = Uml::Class::Create(umldiagram);
		RTEC_Proxy_Consumer::meta = Uml::Class::Create(umldiagram);
		RT_Info::meta = Uml::Class::Create(umldiagram);
		RTEC_Connection::meta = Uml::Class::Create(umldiagram);
		RTEC_Filter_Group::meta = Uml::Class::Create(umldiagram);
		LogicalANDGroup::meta = Uml::Class::Create(umldiagram);
		Include_RTEC_Filter_Group::meta = Uml::Class::Create(umldiagram);
		DisjunctionGroup::meta = Uml::Class::Create(umldiagram);
		ConjunctionGroup::meta = Uml::Class::Create(umldiagram);
		Use_RTEC_Filter::meta = Uml::Class::Create(umldiagram);
		EventChannelConfiguration::meta = Uml::Class::Create(umldiagram);
		RTEC_Resource_Factory::meta = Uml::Class::Create(umldiagram);
		Include_RT_Info::meta = Uml::Class::Create(umldiagram);
		Use_RT_Info::meta = Uml::Class::Create(umldiagram);
		RT_Info_Param::meta = Uml::Class::Create(umldiagram);
		Null_RT_Info::meta = Uml::Class::Create(umldiagram);
		RTEC_Resource_Factory_Reference::meta = Uml::Class::Create(umldiagram);
		EventChannelConfig::meta = Uml::Class::Create(umldiagram);
		Proxy::meta = Uml::Class::Create(umldiagram);
		Proxy_Consumer::meta = Uml::Class::Create(umldiagram);
		Proxy_Supplier::meta = Uml::Class::Create(umldiagram);
		MetricConnection::meta = Uml::Class::Create(umldiagram);
		DataAnalysisBase::meta = Uml::Class::Create(umldiagram);
		Average::meta = Uml::Class::Create(umldiagram);
		Minimum::meta = Uml::Class::Create(umldiagram);
		Maximum::meta = Uml::Class::Create(umldiagram);
		Jitter::meta = Uml::Class::Create(umldiagram);
		TimeProbe::meta = Uml::Class::Create(umldiagram);
		TimerConnection::meta = Uml::Class::Create(umldiagram);
		Latency::meta = Uml::Class::Create(umldiagram);
		Throughput::meta = Uml::Class::Create(umldiagram);
		ComponentAnalyses::meta = Uml::Class::Create(umldiagram);
		BenchmarkAnalysis::meta = Uml::Class::Create(umldiagram);
		MetricsBase::meta = Uml::Class::Create(umldiagram);
		OperationRef::meta = Uml::Class::Create(umldiagram);
		EventRef::meta = Uml::Class::Create(umldiagram);
		TimerEventSinkConn::meta = Uml::Class::Create(umldiagram);
		TaskSet::meta = Uml::Class::Create(umldiagram);
		Task::meta = Uml::Class::Create(umldiagram);
		WorkloadCharacteristics::meta = Uml::Class::Create(umldiagram);
		NodeReference::meta = Uml::Class::Create(umldiagram);
		Interconnect::meta = Uml::Class::Create(umldiagram);
		Node::meta = Uml::Class::Create(umldiagram);
		Resource::meta = Uml::Class::Create(umldiagram);
		Bridge::meta = Uml::Class::Create(umldiagram);
		SharedResource::meta = Uml::Class::Create(umldiagram);
		InstanceMapping::meta = Uml::Class::Create(umldiagram);
		DeploymentPlans::meta = Uml::Class::Create(umldiagram);
		CollocationGroup::meta = Uml::Class::Create(umldiagram);
		DeploymentPlan::meta = Uml::Class::Create(umldiagram);
		invoke::meta = Uml::Class::Create(umldiagram);
		AssemblyselectRequirement::meta = Uml::Class::Create(umldiagram);
		AssemblyConfigProperty::meta = Uml::Class::Create(umldiagram);
		PublishConnector::meta = Uml::Class::Create(umldiagram);
		publish::meta = Uml::Class::Create(umldiagram);
		deliverTo::meta = Uml::Class::Create(umldiagram);
		AssemblyDeployRequirement::meta = Uml::Class::Create(umldiagram);
		emit::meta = Uml::Class::Create(umldiagram);
		ReceptacleDelegate::meta = Uml::Class::Create(umldiagram);
		FacetDelegate::meta = Uml::Class::Create(umldiagram);
		EventSourceDelegate::meta = Uml::Class::Create(umldiagram);
		EventSinkDelegate::meta = Uml::Class::Create(umldiagram);
		ExternalDelegate::meta = Uml::Class::Create(umldiagram);
		ComponentAssembly::meta = Uml::Class::Create(umldiagram);
		ExternalReferenceEndPoint::meta = Uml::Class::Create(umldiagram);
		ImplementationCapability::meta = Uml::Class::Create(umldiagram);
		ImplementationDependsOn::meta = Uml::Class::Create(umldiagram);
		Implements::meta = Uml::Class::Create(umldiagram);
		ComponentImplementations::meta = Uml::Class::Create(umldiagram);
		ConfigProperty::meta = Uml::Class::Create(umldiagram);
		InfoProperty::meta = Uml::Class::Create(umldiagram);
		MonolithprimaryArtifact::meta = Uml::Class::Create(umldiagram);
		MonolithDeployRequirement::meta = Uml::Class::Create(umldiagram);
		ComponentImplementationContainer::meta = Uml::Class::Create(umldiagram);
		MonolithExecParameter::meta = Uml::Class::Create(umldiagram);
		MonolithicImplementation::meta = Uml::Class::Create(umldiagram);
		ComponentImplementationReference::meta = Uml::Class::Create(umldiagram);
		ComponentImplementation::meta = Uml::Class::Create(umldiagram);
		PackageConfBasePackage::meta = Uml::Class::Create(umldiagram);
		PackageConfReference::meta = Uml::Class::Create(umldiagram);
		PackageConfigurations::meta = Uml::Class::Create(umldiagram);
		PackageConfigurationContainer::meta = Uml::Class::Create(umldiagram);
		PackageConfConfigProperty::meta = Uml::Class::Create(umldiagram);
		PackageConfSelectRequirement::meta = Uml::Class::Create(umldiagram);
		PackageConfSpecializedConfig::meta = Uml::Class::Create(umldiagram);
		PackageConfigurationReference::meta = Uml::Class::Create(umldiagram);
		PackageConfiguration::meta = Uml::Class::Create(umldiagram);
		ImplementationArtifacts::meta = Uml::Class::Create(umldiagram);
		ArtifactDependsOn::meta = Uml::Class::Create(umldiagram);
		ArtifactDeployRequirement::meta = Uml::Class::Create(umldiagram);
		ArtifactContainer::meta = Uml::Class::Create(umldiagram);
		ArtifactExecParameter::meta = Uml::Class::Create(umldiagram);
		ArtifactInfoProperty::meta = Uml::Class::Create(umldiagram);
		ImplementationArtifactReference::meta = Uml::Class::Create(umldiagram);
		ImplementationArtifact::meta = Uml::Class::Create(umldiagram);
		ComponentTypes::meta = Uml::Class::Create(umldiagram);
		ComponentPropertyDescription::meta = Uml::Class::Create(umldiagram);
		ComponentProperty::meta = Uml::Class::Create(umldiagram);
		ComponentContainer::meta = Uml::Class::Create(umldiagram);
		ComponentConfigProperty::meta = Uml::Class::Create(umldiagram);
		ComponentInfoProperty::meta = Uml::Class::Create(umldiagram);
		CommonPortAttrs::meta = Uml::Class::Create(umldiagram);
		TopLevelPackageContainer::meta = Uml::Class::Create(umldiagram);
		TopLevelPackages::meta = Uml::Class::Create(umldiagram);
		TopLevelPackage::meta = Uml::Class::Create(umldiagram);
		package::meta = Uml::Class::Create(umldiagram);
		ComponentPackages::meta = Uml::Class::Create(umldiagram);
		PackageInterface::meta = Uml::Class::Create(umldiagram);
		Implementation::meta = Uml::Class::Create(umldiagram);
		PackageContainer::meta = Uml::Class::Create(umldiagram);
		PackageConfigProperty::meta = Uml::Class::Create(umldiagram);
		PackageInfoProperty::meta = Uml::Class::Create(umldiagram);
		ComponentPackage::meta = Uml::Class::Create(umldiagram);
		ComponentPackageReference::meta = Uml::Class::Create(umldiagram);
		Requirement::meta = Uml::Class::Create(umldiagram);
		Capability::meta = Uml::Class::Create(umldiagram);
		DataType::meta = Uml::Class::Create(umldiagram);
		SatisfierProperty::meta = Uml::Class::Create(umldiagram);
		ImplementationDependency::meta = Uml::Class::Create(umldiagram);
		RequirementSatisfier::meta = Uml::Class::Create(umldiagram);
		RequirementBase::meta = Uml::Class::Create(umldiagram);
		ImplementationRequirement::meta = Uml::Class::Create(umldiagram);
		Property::meta = Uml::Class::Create(umldiagram);
		Elements::meta = Uml::Class::Create(umldiagram);
		Targets::meta = Uml::Class::Create(umldiagram);
		Shares::meta = Uml::Class::Create(umldiagram);
		Node2Interconnect::meta = Uml::Class::Create(umldiagram);
		Bridge2Interconnect::meta = Uml::Class::Create(umldiagram);
		Interconnect2Node::meta = Uml::Class::Create(umldiagram);
		Interconnect2Bridge::meta = Uml::Class::Create(umldiagram);
		Domain::meta = Uml::Class::Create(umldiagram);
		Prefixable::meta = Uml::Class::Create(umldiagram);
		Taggable::meta = Uml::Class::Create(umldiagram);
		OutEventPort::meta = Uml::Class::Create(umldiagram);
		ProvidedRequestPort::meta = Uml::Class::Create(umldiagram);
		RequiredRequestPort::meta = Uml::Class::Create(umldiagram);
		InEventPort::meta = Uml::Class::Create(umldiagram);
		Port::meta = Uml::Class::Create(umldiagram);
		Manageable::meta = Uml::Class::Create(umldiagram);
		Provideable::meta = Uml::Class::Create(umldiagram);
		ComponentFactory::meta = Uml::Class::Create(umldiagram);
		ComponentRef::meta = Uml::Class::Create(umldiagram);
		ManagesComponent::meta = Uml::Class::Create(umldiagram);
		Component::meta = Uml::Class::Create(umldiagram);
		OnewayOperation::meta = Uml::Class::Create(umldiagram);
		LookupOperation::meta = Uml::Class::Create(umldiagram);
		FactoryOperation::meta = Uml::Class::Create(umldiagram);
		OperationBase::meta = Uml::Class::Create(umldiagram);
		HasExceptions::meta = Uml::Class::Create(umldiagram);
		InParameter::meta = Uml::Class::Create(umldiagram);
		InoutParameter::meta = Uml::Class::Create(umldiagram);
		OutParameter::meta = Uml::Class::Create(umldiagram);
		ReturnType::meta = Uml::Class::Create(umldiagram);
		TwowayOperation::meta = Uml::Class::Create(umldiagram);
		Object::meta = Uml::Class::Create(umldiagram);
		Inherits::meta = Uml::Class::Create(umldiagram);
		Event::meta = Uml::Class::Create(umldiagram);
		ValueObject::meta = Uml::Class::Create(umldiagram);
		HasOperations::meta = Uml::Class::Create(umldiagram);
		SupportsInterfaces::meta = Uml::Class::Create(umldiagram);
		Supports::meta = Uml::Class::Create(umldiagram);
		ReadonlyAttribute::meta = Uml::Class::Create(umldiagram);
		Attribute::meta = Uml::Class::Create(umldiagram);
		LookupKey::meta = Uml::Class::Create(umldiagram);
		SetException::meta = Uml::Class::Create(umldiagram);
		ObjectByValue::meta = Uml::Class::Create(umldiagram);
		GetException::meta = Uml::Class::Create(umldiagram);
		PrivateFlag::meta = Uml::Class::Create(umldiagram);
		MakeMemberPrivate::meta = Uml::Class::Create(umldiagram);
		AttributeMember::meta = Uml::Class::Create(umldiagram);
		Inheritable::meta = Uml::Class::Create(umldiagram);
		EnumValue::meta = Uml::Class::Create(umldiagram);
		Aggregate::meta = Uml::Class::Create(umldiagram);
		SwitchedAggregate::meta = Uml::Class::Create(umldiagram);
		Label::meta = Uml::Class::Create(umldiagram);
		LabelConnection::meta = Uml::Class::Create(umldiagram);
		MemberType::meta = Uml::Class::Create(umldiagram);
		Discriminator::meta = Uml::Class::Create(umldiagram);
		Collection::meta = Uml::Class::Create(umldiagram);
		Alias::meta = Uml::Class::Create(umldiagram);
		Boxed::meta = Uml::Class::Create(umldiagram);
		NoInheritable::meta = Uml::Class::Create(umldiagram);
		Enum::meta = Uml::Class::Create(umldiagram);
		Member::meta = Uml::Class::Create(umldiagram);
		NamedType::meta = Uml::Class::Create(umldiagram);
		PredefinedType::meta = Uml::Class::Create(umldiagram);
		PredefinedTypes::meta = Uml::Class::Create(umldiagram);
		Byte::meta = Uml::Class::Create(umldiagram);
		Boolean::meta = Uml::Class::Create(umldiagram);
		ShortInteger::meta = Uml::Class::Create(umldiagram);
		LongInteger::meta = Uml::Class::Create(umldiagram);
		RealNumber::meta = Uml::Class::Create(umldiagram);
		String::meta = Uml::Class::Create(umldiagram);
		GenericObject::meta = Uml::Class::Create(umldiagram);
		GenericValueObject::meta = Uml::Class::Create(umldiagram);
		GenericValue::meta = Uml::Class::Create(umldiagram);
		TypeEncoding::meta = Uml::Class::Create(umldiagram);
		TypeKind::meta = Uml::Class::Create(umldiagram);
		Package::meta = Uml::Class::Create(umldiagram);
		File::meta = Uml::Class::Create(umldiagram);
		Exception::meta = Uml::Class::Create(umldiagram);
		ExceptionRef::meta = Uml::Class::Create(umldiagram);
		InterfaceDefinitions::meta = Uml::Class::Create(umldiagram);
		ConstantType::meta = Uml::Class::Create(umldiagram);
		Constant::meta = Uml::Class::Create(umldiagram);
		FileRef::meta = Uml::Class::Create(umldiagram);
		ComponentBuild::meta = Uml::Class::Create(umldiagram);
		Project::meta = Uml::Class::Create(umldiagram);
		ExternalResources::meta = Uml::Class::Create(umldiagram);
		ExtResourceConn::meta = Uml::Class::Create(umldiagram);
		ComponentLib::meta = Uml::Class::Create(umldiagram);
		MPC::meta = Uml::Class::Create(umldiagram);
		StubProject::meta = Uml::Class::Create(umldiagram);
		ServantProject::meta = Uml::Class::Create(umldiagram);
		ExecutorProject::meta = Uml::Class::Create(umldiagram);
		Workspaces::meta = Uml::Class::Create(umldiagram);
		RootFolder::meta = Uml::Class::Create(umldiagram);
		MgaObject::meta = Uml::Class::Create(umldiagram);
		RT_Info::meta_entry_point = Uml::Attribute::Create(RT_Info::meta);
		RT_Info::meta_period = Uml::Attribute::Create(RT_Info::meta);
		RT_Info::meta_enabled = Uml::Attribute::Create(RT_Info::meta);
		RT_Info::meta_criticality = Uml::Attribute::Create(RT_Info::meta);
		RT_Info::meta_importance = Uml::Attribute::Create(RT_Info::meta);
		RT_Info::meta_quantum = Uml::Attribute::Create(RT_Info::meta);
		RT_Info::meta_cached_execution_time = Uml::Attribute::Create(RT_Info::meta);
		RT_Info::meta_threads = Uml::Attribute::Create(RT_Info::meta);
		RT_Info::meta_info_type = Uml::Attribute::Create(RT_Info::meta);
		RT_Info::meta_worst_case_execution_time = Uml::Attribute::Create(RT_Info::meta);
		RT_Info::meta_typical_execution_time = Uml::Attribute::Create(RT_Info::meta);
		RTEC_Resource_Factory::meta_ECConsumerLock = Uml::Attribute::Create(RTEC_Resource_Factory::meta);
		RTEC_Resource_Factory::meta_ECSupplierFiltering = Uml::Attribute::Create(RTEC_Resource_Factory::meta);
		RTEC_Resource_Factory::meta_ECDispatching = Uml::Attribute::Create(RTEC_Resource_Factory::meta);
		RTEC_Resource_Factory::meta_ECTimeout = Uml::Attribute::Create(RTEC_Resource_Factory::meta);
		RTEC_Resource_Factory::meta_ECConsumerControlPeriod = Uml::Attribute::Create(RTEC_Resource_Factory::meta);
		RTEC_Resource_Factory::meta_ECSupplierLock = Uml::Attribute::Create(RTEC_Resource_Factory::meta);
		RTEC_Resource_Factory::meta_ECObserver = Uml::Attribute::Create(RTEC_Resource_Factory::meta);
		RTEC_Resource_Factory::meta_ECSupplierControl = Uml::Attribute::Create(RTEC_Resource_Factory::meta);
		RTEC_Resource_Factory::meta_ECConsumerControl = Uml::Attribute::Create(RTEC_Resource_Factory::meta);
		RTEC_Resource_Factory::meta_ECFiltering = Uml::Attribute::Create(RTEC_Resource_Factory::meta);
		RTEC_Resource_Factory::meta_ECProxyPushConsumerCollectionFlag = Uml::Attribute::Create(RTEC_Resource_Factory::meta);
		RTEC_Resource_Factory::meta_ECUseORBId = Uml::Attribute::Create(RTEC_Resource_Factory::meta);
		RTEC_Resource_Factory::meta_ECProxyPushConsumerCollectionSynch = Uml::Attribute::Create(RTEC_Resource_Factory::meta);
		RTEC_Resource_Factory::meta_ECProxyPushConsumerCollectionIterator = Uml::Attribute::Create(RTEC_Resource_Factory::meta);
		RTEC_Resource_Factory::meta_ECSupplierControlPeriod = Uml::Attribute::Create(RTEC_Resource_Factory::meta);
		RTEC_Resource_Factory::meta_ECScheduling = Uml::Attribute::Create(RTEC_Resource_Factory::meta);
		RTEC_Resource_Factory::meta_ECDispatchingThreads = Uml::Attribute::Create(RTEC_Resource_Factory::meta);
		RTEC_Resource_Factory::meta_ECProxyPushSupplierCollectionFlag = Uml::Attribute::Create(RTEC_Resource_Factory::meta);
		RTEC_Resource_Factory::meta_ECProxyPushSupplierCollectionSynch = Uml::Attribute::Create(RTEC_Resource_Factory::meta);
		RTEC_Resource_Factory::meta_ECProxyPushSupplierCollectionIterator = Uml::Attribute::Create(RTEC_Resource_Factory::meta);
		EventChannelConfig::meta_name = Uml::Attribute::Create(EventChannelConfig::meta);
		ComponentAnalyses::meta_name = Uml::Attribute::Create(ComponentAnalyses::meta);
		MetricsBase::meta_warmup = Uml::Attribute::Create(MetricsBase::meta);
		MetricsBase::meta_fileName = Uml::Attribute::Create(MetricsBase::meta);
		MetricsBase::meta_iterations = Uml::Attribute::Create(MetricsBase::meta);
		Task::meta_rate = Uml::Attribute::Create(Task::meta);
		Task::meta_priority = Uml::Attribute::Create(Task::meta);
		Interconnect::meta_label = Uml::Attribute::Create(Interconnect::meta);
		Node::meta_label = Uml::Attribute::Create(Node::meta);
		Resource::meta_resourceType = Uml::Attribute::Create(Resource::meta);
		Bridge::meta_label = Uml::Attribute::Create(Bridge::meta);
		SharedResource::meta_resourceType = Uml::Attribute::Create(SharedResource::meta);
		DeploymentPlans::meta_name = Uml::Attribute::Create(DeploymentPlans::meta);
		DeploymentPlan::meta_label = Uml::Attribute::Create(DeploymentPlan::meta);
		ExternalReferenceEndPoint::meta_location = Uml::Attribute::Create(ExternalReferenceEndPoint::meta);
		ComponentImplementations::meta_name = Uml::Attribute::Create(ComponentImplementations::meta);
		ComponentImplementation::meta_UUID = Uml::Attribute::Create(ComponentImplementation::meta);
		ComponentImplementation::meta_label = Uml::Attribute::Create(ComponentImplementation::meta);
		PackageConfigurations::meta_name = Uml::Attribute::Create(PackageConfigurations::meta);
		PackageConfiguration::meta_UUID = Uml::Attribute::Create(PackageConfiguration::meta);
		PackageConfiguration::meta_label = Uml::Attribute::Create(PackageConfiguration::meta);
		ImplementationArtifacts::meta_name = Uml::Attribute::Create(ImplementationArtifacts::meta);
		ImplementationArtifact::meta_tree_file_name = Uml::Attribute::Create(ImplementationArtifact::meta);
		ImplementationArtifact::meta_configuration = Uml::Attribute::Create(ImplementationArtifact::meta);
		ImplementationArtifact::meta_label = Uml::Attribute::Create(ImplementationArtifact::meta);
		ImplementationArtifact::meta_UUID = Uml::Attribute::Create(ImplementationArtifact::meta);
		ImplementationArtifact::meta_location = Uml::Attribute::Create(ImplementationArtifact::meta);
		ComponentTypes::meta_name = Uml::Attribute::Create(ComponentTypes::meta);
		CommonPortAttrs::meta_exclusiveUser = Uml::Attribute::Create(CommonPortAttrs::meta);
		CommonPortAttrs::meta_optional = Uml::Attribute::Create(CommonPortAttrs::meta);
		CommonPortAttrs::meta_exclusiveProvider = Uml::Attribute::Create(CommonPortAttrs::meta);
		TopLevelPackages::meta_name = Uml::Attribute::Create(TopLevelPackages::meta);
		ComponentPackages::meta_name = Uml::Attribute::Create(ComponentPackages::meta);
		ComponentPackage::meta_UUID = Uml::Attribute::Create(ComponentPackage::meta);
		ComponentPackage::meta_label = Uml::Attribute::Create(ComponentPackage::meta);
		ComponentPackageReference::meta_requiredName = Uml::Attribute::Create(ComponentPackageReference::meta);
		ComponentPackageReference::meta_requiredUUID = Uml::Attribute::Create(ComponentPackageReference::meta);
		ComponentPackageReference::meta_requiredType = Uml::Attribute::Create(ComponentPackageReference::meta);
		SatisfierProperty::meta_DataValue = Uml::Attribute::Create(SatisfierProperty::meta);
		SatisfierProperty::meta_SatisfierPropertyKind = Uml::Attribute::Create(SatisfierProperty::meta);
		ImplementationDependency::meta_requiredType = Uml::Attribute::Create(ImplementationDependency::meta);
		RequirementSatisfier::meta_resourceType = Uml::Attribute::Create(RequirementSatisfier::meta);
		RequirementBase::meta_resourceType = Uml::Attribute::Create(RequirementBase::meta);
		ImplementationRequirement::meta_ResourceUsageKind = Uml::Attribute::Create(ImplementationRequirement::meta);
		ImplementationRequirement::meta_componentPort = Uml::Attribute::Create(ImplementationRequirement::meta);
		ImplementationRequirement::meta_resourcePort = Uml::Attribute::Create(ImplementationRequirement::meta);
		Property::meta_DataValue = Uml::Attribute::Create(Property::meta);
		Targets::meta_name = Uml::Attribute::Create(Targets::meta);
		Domain::meta_label = Uml::Attribute::Create(Domain::meta);
		Domain::meta_UUID = Uml::Attribute::Create(Domain::meta);
		Prefixable::meta_PrefixTag = Uml::Attribute::Create(Prefixable::meta);
		Taggable::meta_SpecifyIdTag = Uml::Attribute::Create(Taggable::meta);
		Taggable::meta_VersionTag = Uml::Attribute::Create(Taggable::meta);
		OutEventPort::meta_out_event_port_type = Uml::Attribute::Create(OutEventPort::meta);
		OutEventPort::meta_single_destination = Uml::Attribute::Create(OutEventPort::meta);
		RequiredRequestPort::meta_multiple_connections = Uml::Attribute::Create(RequiredRequestPort::meta);
		Component::meta_UUID = Uml::Attribute::Create(Component::meta);
		Component::meta_label = Uml::Attribute::Create(Component::meta);
		Object::meta_local = Uml::Attribute::Create(Object::meta);
		Object::meta_abstract = Uml::Attribute::Create(Object::meta);
		ObjectByValue::meta_abstract = Uml::Attribute::Create(ObjectByValue::meta);
		PredefinedTypes::meta_name = Uml::Attribute::Create(PredefinedTypes::meta);
		InterfaceDefinitions::meta_name = Uml::Attribute::Create(InterfaceDefinitions::meta);
		Constant::meta_value = Uml::Attribute::Create(Constant::meta);
		ComponentBuild::meta_name = Uml::Attribute::Create(ComponentBuild::meta);
		ComponentLib::meta_sharedname = Uml::Attribute::Create(ComponentLib::meta);
		ComponentLib::meta_libraryexport = Uml::Attribute::Create(ComponentLib::meta);
		ComponentLib::meta_ORBServices = Uml::Attribute::Create(ComponentLib::meta);
		RootFolder::meta_name = Uml::Attribute::Create(RootFolder::meta);
		MgaObject::meta_position = Uml::Attribute::Create(MgaObject::meta);
		MgaObject::meta_name = Uml::Attribute::Create(MgaObject::meta);
	}
	void InitClassesAttributes()
	{
		RTEC_Proxy::meta.name() = "RTEC_Proxy";
		RTEC_Proxy::meta.isAbstract() = true;
		RTEC_Proxy::meta.stereotype() = "Model";
		Include_RTEC_Filter_Group_Members_Base::meta.name() = "Include_RTEC_Filter_Group_Members_Base";
		Include_RTEC_Filter_Group_Members_Base::meta.isAbstract() = true;
		Include_RTEC_Filter_Group_Members_Base::meta.stereotype() = "FCO";
		RTEC_Proxy_Supplier::meta.name() = "RTEC_Proxy_Supplier";
		RTEC_Proxy_Supplier::meta.isAbstract() = false;
		RTEC_Proxy_Supplier::meta.stereotype() = "Model";
		RTEC_Proxy_Consumer::meta.name() = "RTEC_Proxy_Consumer";
		RTEC_Proxy_Consumer::meta.isAbstract() = false;
		RTEC_Proxy_Consumer::meta.stereotype() = "Model";
		RT_Info::meta.name() = "RT_Info";
		RT_Info::meta.isAbstract() = false;
		RT_Info::meta.stereotype() = "Atom";
		RTEC_Connection::meta.name() = "RTEC_Connection";
		RTEC_Connection::meta.isAbstract() = false;
		RTEC_Connection::meta.stereotype() = "Connection";
		RTEC_Filter_Group::meta.name() = "RTEC_Filter_Group";
		RTEC_Filter_Group::meta.isAbstract() = true;
		RTEC_Filter_Group::meta.stereotype() = "Atom";
		LogicalANDGroup::meta.name() = "LogicalANDGroup";
		LogicalANDGroup::meta.isAbstract() = false;
		LogicalANDGroup::meta.stereotype() = "Atom";
		Include_RTEC_Filter_Group::meta.name() = "Include_RTEC_Filter_Group";
		Include_RTEC_Filter_Group::meta.isAbstract() = false;
		Include_RTEC_Filter_Group::meta.stereotype() = "Connection";
		DisjunctionGroup::meta.name() = "DisjunctionGroup";
		DisjunctionGroup::meta.isAbstract() = false;
		DisjunctionGroup::meta.stereotype() = "Atom";
		ConjunctionGroup::meta.name() = "ConjunctionGroup";
		ConjunctionGroup::meta.isAbstract() = false;
		ConjunctionGroup::meta.stereotype() = "Atom";
		Use_RTEC_Filter::meta.name() = "Use_RTEC_Filter";
		Use_RTEC_Filter::meta.isAbstract() = false;
		Use_RTEC_Filter::meta.stereotype() = "Connection";
		EventChannelConfiguration::meta.name() = "EventChannelConfiguration";
		EventChannelConfiguration::meta.isAbstract() = false;
		EventChannelConfiguration::meta.stereotype() = "Model";
		RTEC_Resource_Factory::meta.name() = "RTEC_Resource_Factory";
		RTEC_Resource_Factory::meta.isAbstract() = false;
		RTEC_Resource_Factory::meta.stereotype() = "Atom";
		Include_RT_Info::meta.name() = "Include_RT_Info";
		Include_RT_Info::meta.isAbstract() = false;
		Include_RT_Info::meta.stereotype() = "Connection";
		Use_RT_Info::meta.name() = "Use_RT_Info";
		Use_RT_Info::meta.isAbstract() = false;
		Use_RT_Info::meta.stereotype() = "Connection";
		RT_Info_Param::meta.name() = "RT_Info_Param";
		RT_Info_Param::meta.isAbstract() = true;
		RT_Info_Param::meta.stereotype() = "Atom";
		Null_RT_Info::meta.name() = "Null_RT_Info";
		Null_RT_Info::meta.isAbstract() = false;
		Null_RT_Info::meta.stereotype() = "Atom";
		RTEC_Resource_Factory_Reference::meta.name() = "RTEC_Resource_Factory_Reference";
		RTEC_Resource_Factory_Reference::meta.isAbstract() = false;
		RTEC_Resource_Factory_Reference::meta.stereotype() = "Reference";
		EventChannelConfig::meta.name() = "EventChannelConfig";
		EventChannelConfig::meta.isAbstract() = false;
		EventChannelConfig::meta.stereotype() = "Folder";
		Proxy::meta.name() = "Proxy";
		Proxy::meta.isAbstract() = true;
		Proxy::meta.stereotype() = "Model";
		Proxy_Consumer::meta.name() = "Proxy_Consumer";
		Proxy_Consumer::meta.isAbstract() = true;
		Proxy_Consumer::meta.stereotype() = "Model";
		Proxy_Supplier::meta.name() = "Proxy_Supplier";
		Proxy_Supplier::meta.isAbstract() = true;
		Proxy_Supplier::meta.stereotype() = "Model";
		MetricConnection::meta.name() = "MetricConnection";
		MetricConnection::meta.isAbstract() = false;
		MetricConnection::meta.stereotype() = "Connection";
		DataAnalysisBase::meta.name() = "DataAnalysisBase";
		DataAnalysisBase::meta.isAbstract() = true;
		DataAnalysisBase::meta.stereotype() = "FCO";
		Average::meta.name() = "Average";
		Average::meta.isAbstract() = false;
		Average::meta.stereotype() = "Atom";
		Minimum::meta.name() = "Minimum";
		Minimum::meta.isAbstract() = false;
		Minimum::meta.stereotype() = "Atom";
		Maximum::meta.name() = "Maximum";
		Maximum::meta.isAbstract() = false;
		Maximum::meta.stereotype() = "Atom";
		Jitter::meta.name() = "Jitter";
		Jitter::meta.isAbstract() = false;
		Jitter::meta.stereotype() = "Atom";
		TimeProbe::meta.name() = "TimeProbe";
		TimeProbe::meta.isAbstract() = false;
		TimeProbe::meta.stereotype() = "Atom";
		TimerConnection::meta.name() = "TimerConnection";
		TimerConnection::meta.isAbstract() = false;
		TimerConnection::meta.stereotype() = "Connection";
		Latency::meta.name() = "Latency";
		Latency::meta.isAbstract() = false;
		Latency::meta.stereotype() = "Model";
		Throughput::meta.name() = "Throughput";
		Throughput::meta.isAbstract() = false;
		Throughput::meta.stereotype() = "Model";
		ComponentAnalyses::meta.name() = "ComponentAnalyses";
		ComponentAnalyses::meta.isAbstract() = false;
		ComponentAnalyses::meta.stereotype() = "Folder";
		BenchmarkAnalysis::meta.name() = "BenchmarkAnalysis";
		BenchmarkAnalysis::meta.isAbstract() = false;
		BenchmarkAnalysis::meta.stereotype() = "Model";
		MetricsBase::meta.name() = "MetricsBase";
		MetricsBase::meta.isAbstract() = true;
		MetricsBase::meta.stereotype() = "Model";
		OperationRef::meta.name() = "OperationRef";
		OperationRef::meta.isAbstract() = false;
		OperationRef::meta.stereotype() = "Reference";
		EventRef::meta.name() = "EventRef";
		EventRef::meta.isAbstract() = false;
		EventRef::meta.stereotype() = "Reference";
		TimerEventSinkConn::meta.name() = "TimerEventSinkConn";
		TimerEventSinkConn::meta.isAbstract() = false;
		TimerEventSinkConn::meta.stereotype() = "Connection";
		TaskSet::meta.name() = "TaskSet";
		TaskSet::meta.isAbstract() = false;
		TaskSet::meta.stereotype() = "Set";
		Task::meta.name() = "Task";
		Task::meta.isAbstract() = false;
		Task::meta.stereotype() = "Atom";
		WorkloadCharacteristics::meta.name() = "WorkloadCharacteristics";
		WorkloadCharacteristics::meta.isAbstract() = false;
		WorkloadCharacteristics::meta.stereotype() = "Connection";
		NodeReference::meta.name() = "NodeReference";
		NodeReference::meta.isAbstract() = false;
		NodeReference::meta.stereotype() = "Reference";
		Interconnect::meta.name() = "Interconnect";
		Interconnect::meta.isAbstract() = false;
		Interconnect::meta.stereotype() = "Model";
		Node::meta.name() = "Node";
		Node::meta.isAbstract() = false;
		Node::meta.stereotype() = "Model";
		Resource::meta.name() = "Resource";
		Resource::meta.isAbstract() = false;
		Resource::meta.stereotype() = "Model";
		Bridge::meta.name() = "Bridge";
		Bridge::meta.isAbstract() = false;
		Bridge::meta.stereotype() = "Model";
		SharedResource::meta.name() = "SharedResource";
		SharedResource::meta.isAbstract() = false;
		SharedResource::meta.stereotype() = "Model";
		InstanceMapping::meta.name() = "InstanceMapping";
		InstanceMapping::meta.isAbstract() = false;
		InstanceMapping::meta.stereotype() = "Connection";
		DeploymentPlans::meta.name() = "DeploymentPlans";
		DeploymentPlans::meta.isAbstract() = false;
		DeploymentPlans::meta.stereotype() = "Folder";
		CollocationGroup::meta.name() = "CollocationGroup";
		CollocationGroup::meta.isAbstract() = false;
		CollocationGroup::meta.stereotype() = "Set";
		DeploymentPlan::meta.name() = "DeploymentPlan";
		DeploymentPlan::meta.isAbstract() = false;
		DeploymentPlan::meta.stereotype() = "Model";
		invoke::meta.name() = "invoke";
		invoke::meta.isAbstract() = false;
		invoke::meta.stereotype() = "Connection";
		AssemblyselectRequirement::meta.name() = "AssemblyselectRequirement";
		AssemblyselectRequirement::meta.isAbstract() = false;
		AssemblyselectRequirement::meta.stereotype() = "Connection";
		AssemblyConfigProperty::meta.name() = "AssemblyConfigProperty";
		AssemblyConfigProperty::meta.isAbstract() = false;
		AssemblyConfigProperty::meta.stereotype() = "Connection";
		PublishConnector::meta.name() = "PublishConnector";
		PublishConnector::meta.isAbstract() = false;
		PublishConnector::meta.stereotype() = "Atom";
		publish::meta.name() = "publish";
		publish::meta.isAbstract() = false;
		publish::meta.stereotype() = "Connection";
		deliverTo::meta.name() = "deliverTo";
		deliverTo::meta.isAbstract() = false;
		deliverTo::meta.stereotype() = "Connection";
		AssemblyDeployRequirement::meta.name() = "AssemblyDeployRequirement";
		AssemblyDeployRequirement::meta.isAbstract() = false;
		AssemblyDeployRequirement::meta.stereotype() = "Connection";
		emit::meta.name() = "emit";
		emit::meta.isAbstract() = false;
		emit::meta.stereotype() = "Connection";
		ReceptacleDelegate::meta.name() = "ReceptacleDelegate";
		ReceptacleDelegate::meta.isAbstract() = false;
		ReceptacleDelegate::meta.stereotype() = "Connection";
		FacetDelegate::meta.name() = "FacetDelegate";
		FacetDelegate::meta.isAbstract() = false;
		FacetDelegate::meta.stereotype() = "Connection";
		EventSourceDelegate::meta.name() = "EventSourceDelegate";
		EventSourceDelegate::meta.isAbstract() = false;
		EventSourceDelegate::meta.stereotype() = "Connection";
		EventSinkDelegate::meta.name() = "EventSinkDelegate";
		EventSinkDelegate::meta.isAbstract() = false;
		EventSinkDelegate::meta.stereotype() = "Connection";
		ExternalDelegate::meta.name() = "ExternalDelegate";
		ExternalDelegate::meta.isAbstract() = false;
		ExternalDelegate::meta.stereotype() = "Connection";
		ComponentAssembly::meta.name() = "ComponentAssembly";
		ComponentAssembly::meta.isAbstract() = false;
		ComponentAssembly::meta.stereotype() = "Model";
		ExternalReferenceEndPoint::meta.name() = "ExternalReferenceEndPoint";
		ExternalReferenceEndPoint::meta.isAbstract() = false;
		ExternalReferenceEndPoint::meta.stereotype() = "Atom";
		ImplementationCapability::meta.name() = "ImplementationCapability";
		ImplementationCapability::meta.isAbstract() = false;
		ImplementationCapability::meta.stereotype() = "Connection";
		ImplementationDependsOn::meta.name() = "ImplementationDependsOn";
		ImplementationDependsOn::meta.isAbstract() = false;
		ImplementationDependsOn::meta.stereotype() = "Connection";
		Implements::meta.name() = "Implements";
		Implements::meta.isAbstract() = false;
		Implements::meta.stereotype() = "Connection";
		ComponentImplementations::meta.name() = "ComponentImplementations";
		ComponentImplementations::meta.isAbstract() = false;
		ComponentImplementations::meta.stereotype() = "Folder";
		ConfigProperty::meta.name() = "ConfigProperty";
		ConfigProperty::meta.isAbstract() = false;
		ConfigProperty::meta.stereotype() = "Connection";
		InfoProperty::meta.name() = "InfoProperty";
		InfoProperty::meta.isAbstract() = false;
		InfoProperty::meta.stereotype() = "Connection";
		MonolithprimaryArtifact::meta.name() = "MonolithprimaryArtifact";
		MonolithprimaryArtifact::meta.isAbstract() = false;
		MonolithprimaryArtifact::meta.stereotype() = "Connection";
		MonolithDeployRequirement::meta.name() = "MonolithDeployRequirement";
		MonolithDeployRequirement::meta.isAbstract() = false;
		MonolithDeployRequirement::meta.stereotype() = "Connection";
		ComponentImplementationContainer::meta.name() = "ComponentImplementationContainer";
		ComponentImplementationContainer::meta.isAbstract() = false;
		ComponentImplementationContainer::meta.stereotype() = "Model";
		MonolithExecParameter::meta.name() = "MonolithExecParameter";
		MonolithExecParameter::meta.isAbstract() = false;
		MonolithExecParameter::meta.stereotype() = "Connection";
		MonolithicImplementation::meta.name() = "MonolithicImplementation";
		MonolithicImplementation::meta.isAbstract() = false;
		MonolithicImplementation::meta.stereotype() = "Atom";
		ComponentImplementationReference::meta.name() = "ComponentImplementationReference";
		ComponentImplementationReference::meta.isAbstract() = false;
		ComponentImplementationReference::meta.stereotype() = "Reference";
		ComponentImplementation::meta.name() = "ComponentImplementation";
		ComponentImplementation::meta.isAbstract() = true;
		ComponentImplementation::meta.stereotype() = "FCO";
		PackageConfBasePackage::meta.name() = "PackageConfBasePackage";
		PackageConfBasePackage::meta.isAbstract() = false;
		PackageConfBasePackage::meta.stereotype() = "Connection";
		PackageConfReference::meta.name() = "PackageConfReference";
		PackageConfReference::meta.isAbstract() = false;
		PackageConfReference::meta.stereotype() = "Connection";
		PackageConfigurations::meta.name() = "PackageConfigurations";
		PackageConfigurations::meta.isAbstract() = false;
		PackageConfigurations::meta.stereotype() = "Folder";
		PackageConfigurationContainer::meta.name() = "PackageConfigurationContainer";
		PackageConfigurationContainer::meta.isAbstract() = false;
		PackageConfigurationContainer::meta.stereotype() = "Model";
		PackageConfConfigProperty::meta.name() = "PackageConfConfigProperty";
		PackageConfConfigProperty::meta.isAbstract() = false;
		PackageConfConfigProperty::meta.stereotype() = "Connection";
		PackageConfSelectRequirement::meta.name() = "PackageConfSelectRequirement";
		PackageConfSelectRequirement::meta.isAbstract() = false;
		PackageConfSelectRequirement::meta.stereotype() = "Connection";
		PackageConfSpecializedConfig::meta.name() = "PackageConfSpecializedConfig";
		PackageConfSpecializedConfig::meta.isAbstract() = false;
		PackageConfSpecializedConfig::meta.stereotype() = "Connection";
		PackageConfigurationReference::meta.name() = "PackageConfigurationReference";
		PackageConfigurationReference::meta.isAbstract() = false;
		PackageConfigurationReference::meta.stereotype() = "Reference";
		PackageConfiguration::meta.name() = "PackageConfiguration";
		PackageConfiguration::meta.isAbstract() = false;
		PackageConfiguration::meta.stereotype() = "Atom";
		ImplementationArtifacts::meta.name() = "ImplementationArtifacts";
		ImplementationArtifacts::meta.isAbstract() = false;
		ImplementationArtifacts::meta.stereotype() = "Folder";
		ArtifactDependsOn::meta.name() = "ArtifactDependsOn";
		ArtifactDependsOn::meta.isAbstract() = false;
		ArtifactDependsOn::meta.stereotype() = "Connection";
		ArtifactDeployRequirement::meta.name() = "ArtifactDeployRequirement";
		ArtifactDeployRequirement::meta.isAbstract() = false;
		ArtifactDeployRequirement::meta.stereotype() = "Connection";
		ArtifactContainer::meta.name() = "ArtifactContainer";
		ArtifactContainer::meta.isAbstract() = false;
		ArtifactContainer::meta.stereotype() = "Model";
		ArtifactExecParameter::meta.name() = "ArtifactExecParameter";
		ArtifactExecParameter::meta.isAbstract() = false;
		ArtifactExecParameter::meta.stereotype() = "Connection";
		ArtifactInfoProperty::meta.name() = "ArtifactInfoProperty";
		ArtifactInfoProperty::meta.isAbstract() = false;
		ArtifactInfoProperty::meta.stereotype() = "Connection";
		ImplementationArtifactReference::meta.name() = "ImplementationArtifactReference";
		ImplementationArtifactReference::meta.isAbstract() = false;
		ImplementationArtifactReference::meta.stereotype() = "Reference";
		ImplementationArtifact::meta.name() = "ImplementationArtifact";
		ImplementationArtifact::meta.isAbstract() = false;
		ImplementationArtifact::meta.stereotype() = "Atom";
		ComponentTypes::meta.name() = "ComponentTypes";
		ComponentTypes::meta.isAbstract() = false;
		ComponentTypes::meta.stereotype() = "Folder";
		ComponentPropertyDescription::meta.name() = "ComponentPropertyDescription";
		ComponentPropertyDescription::meta.isAbstract() = false;
		ComponentPropertyDescription::meta.stereotype() = "Model";
		ComponentProperty::meta.name() = "ComponentProperty";
		ComponentProperty::meta.isAbstract() = false;
		ComponentProperty::meta.stereotype() = "Connection";
		ComponentContainer::meta.name() = "ComponentContainer";
		ComponentContainer::meta.isAbstract() = false;
		ComponentContainer::meta.stereotype() = "Model";
		ComponentConfigProperty::meta.name() = "ComponentConfigProperty";
		ComponentConfigProperty::meta.isAbstract() = false;
		ComponentConfigProperty::meta.stereotype() = "Connection";
		ComponentInfoProperty::meta.name() = "ComponentInfoProperty";
		ComponentInfoProperty::meta.isAbstract() = false;
		ComponentInfoProperty::meta.stereotype() = "Connection";
		CommonPortAttrs::meta.name() = "CommonPortAttrs";
		CommonPortAttrs::meta.isAbstract() = true;
		CommonPortAttrs::meta.stereotype() = "FCO";
		TopLevelPackageContainer::meta.name() = "TopLevelPackageContainer";
		TopLevelPackageContainer::meta.isAbstract() = false;
		TopLevelPackageContainer::meta.stereotype() = "Model";
		TopLevelPackages::meta.name() = "TopLevelPackages";
		TopLevelPackages::meta.isAbstract() = false;
		TopLevelPackages::meta.stereotype() = "Folder";
		TopLevelPackage::meta.name() = "TopLevelPackage";
		TopLevelPackage::meta.isAbstract() = false;
		TopLevelPackage::meta.stereotype() = "Atom";
		package::meta.name() = "package";
		package::meta.isAbstract() = false;
		package::meta.stereotype() = "Connection";
		ComponentPackages::meta.name() = "ComponentPackages";
		ComponentPackages::meta.isAbstract() = false;
		ComponentPackages::meta.stereotype() = "Folder";
		PackageInterface::meta.name() = "PackageInterface";
		PackageInterface::meta.isAbstract() = false;
		PackageInterface::meta.stereotype() = "Connection";
		Implementation::meta.name() = "Implementation";
		Implementation::meta.isAbstract() = false;
		Implementation::meta.stereotype() = "Connection";
		PackageContainer::meta.name() = "PackageContainer";
		PackageContainer::meta.isAbstract() = false;
		PackageContainer::meta.stereotype() = "Model";
		PackageConfigProperty::meta.name() = "PackageConfigProperty";
		PackageConfigProperty::meta.isAbstract() = false;
		PackageConfigProperty::meta.stereotype() = "Connection";
		PackageInfoProperty::meta.name() = "PackageInfoProperty";
		PackageInfoProperty::meta.isAbstract() = false;
		PackageInfoProperty::meta.stereotype() = "Connection";
		ComponentPackage::meta.name() = "ComponentPackage";
		ComponentPackage::meta.isAbstract() = false;
		ComponentPackage::meta.stereotype() = "Atom";
		ComponentPackageReference::meta.name() = "ComponentPackageReference";
		ComponentPackageReference::meta.isAbstract() = false;
		ComponentPackageReference::meta.stereotype() = "Reference";
		Requirement::meta.name() = "Requirement";
		Requirement::meta.isAbstract() = false;
		Requirement::meta.stereotype() = "Model";
		Capability::meta.name() = "Capability";
		Capability::meta.isAbstract() = false;
		Capability::meta.stereotype() = "Model";
		DataType::meta.name() = "DataType";
		DataType::meta.isAbstract() = false;
		DataType::meta.stereotype() = "Reference";
		SatisfierProperty::meta.name() = "SatisfierProperty";
		SatisfierProperty::meta.isAbstract() = false;
		SatisfierProperty::meta.stereotype() = "Model";
		ImplementationDependency::meta.name() = "ImplementationDependency";
		ImplementationDependency::meta.isAbstract() = false;
		ImplementationDependency::meta.stereotype() = "Atom";
		RequirementSatisfier::meta.name() = "RequirementSatisfier";
		RequirementSatisfier::meta.isAbstract() = true;
		RequirementSatisfier::meta.stereotype() = "Model";
		RequirementBase::meta.name() = "RequirementBase";
		RequirementBase::meta.isAbstract() = true;
		RequirementBase::meta.stereotype() = "Model";
		ImplementationRequirement::meta.name() = "ImplementationRequirement";
		ImplementationRequirement::meta.isAbstract() = false;
		ImplementationRequirement::meta.stereotype() = "Model";
		Property::meta.name() = "Property";
		Property::meta.isAbstract() = false;
		Property::meta.stereotype() = "Model";
		Elements::meta.name() = "Elements";
		Elements::meta.isAbstract() = true;
		Elements::meta.stereotype() = "FCO";
		Targets::meta.name() = "Targets";
		Targets::meta.isAbstract() = false;
		Targets::meta.stereotype() = "Folder";
		Shares::meta.name() = "Shares";
		Shares::meta.isAbstract() = false;
		Shares::meta.stereotype() = "Connection";
		Node2Interconnect::meta.name() = "Node2Interconnect";
		Node2Interconnect::meta.isAbstract() = false;
		Node2Interconnect::meta.stereotype() = "Connection";
		Bridge2Interconnect::meta.name() = "Bridge2Interconnect";
		Bridge2Interconnect::meta.isAbstract() = false;
		Bridge2Interconnect::meta.stereotype() = "Connection";
		Interconnect2Node::meta.name() = "Interconnect2Node";
		Interconnect2Node::meta.isAbstract() = false;
		Interconnect2Node::meta.stereotype() = "Connection";
		Interconnect2Bridge::meta.name() = "Interconnect2Bridge";
		Interconnect2Bridge::meta.isAbstract() = false;
		Interconnect2Bridge::meta.stereotype() = "Connection";
		Domain::meta.name() = "Domain";
		Domain::meta.isAbstract() = false;
		Domain::meta.stereotype() = "Model";
		Prefixable::meta.name() = "Prefixable";
		Prefixable::meta.isAbstract() = true;
		Prefixable::meta.stereotype() = "Model";
		Taggable::meta.name() = "Taggable";
		Taggable::meta.isAbstract() = true;
		Taggable::meta.stereotype() = "FCO";
		OutEventPort::meta.name() = "OutEventPort";
		OutEventPort::meta.isAbstract() = false;
		OutEventPort::meta.stereotype() = "Reference";
		ProvidedRequestPort::meta.name() = "ProvidedRequestPort";
		ProvidedRequestPort::meta.isAbstract() = false;
		ProvidedRequestPort::meta.stereotype() = "Reference";
		RequiredRequestPort::meta.name() = "RequiredRequestPort";
		RequiredRequestPort::meta.isAbstract() = false;
		RequiredRequestPort::meta.stereotype() = "Reference";
		InEventPort::meta.name() = "InEventPort";
		InEventPort::meta.isAbstract() = false;
		InEventPort::meta.stereotype() = "Reference";
		Port::meta.name() = "Port";
		Port::meta.isAbstract() = true;
		Port::meta.stereotype() = "FCO";
		Manageable::meta.name() = "Manageable";
		Manageable::meta.isAbstract() = true;
		Manageable::meta.stereotype() = "FCO";
		Provideable::meta.name() = "Provideable";
		Provideable::meta.isAbstract() = true;
		Provideable::meta.stereotype() = "FCO";
		ComponentFactory::meta.name() = "ComponentFactory";
		ComponentFactory::meta.isAbstract() = false;
		ComponentFactory::meta.stereotype() = "Model";
		ComponentRef::meta.name() = "ComponentRef";
		ComponentRef::meta.isAbstract() = false;
		ComponentRef::meta.stereotype() = "Reference";
		ManagesComponent::meta.name() = "ManagesComponent";
		ManagesComponent::meta.isAbstract() = false;
		ManagesComponent::meta.stereotype() = "Connection";
		Component::meta.name() = "Component";
		Component::meta.isAbstract() = false;
		Component::meta.stereotype() = "Model";
		OnewayOperation::meta.name() = "OnewayOperation";
		OnewayOperation::meta.isAbstract() = false;
		OnewayOperation::meta.stereotype() = "Model";
		LookupOperation::meta.name() = "LookupOperation";
		LookupOperation::meta.isAbstract() = false;
		LookupOperation::meta.stereotype() = "Model";
		FactoryOperation::meta.name() = "FactoryOperation";
		FactoryOperation::meta.isAbstract() = false;
		FactoryOperation::meta.stereotype() = "Model";
		OperationBase::meta.name() = "OperationBase";
		OperationBase::meta.isAbstract() = true;
		OperationBase::meta.stereotype() = "Model";
		HasExceptions::meta.name() = "HasExceptions";
		HasExceptions::meta.isAbstract() = true;
		HasExceptions::meta.stereotype() = "Model";
		InParameter::meta.name() = "InParameter";
		InParameter::meta.isAbstract() = false;
		InParameter::meta.stereotype() = "Reference";
		InoutParameter::meta.name() = "InoutParameter";
		InoutParameter::meta.isAbstract() = false;
		InoutParameter::meta.stereotype() = "Reference";
		OutParameter::meta.name() = "OutParameter";
		OutParameter::meta.isAbstract() = false;
		OutParameter::meta.stereotype() = "Reference";
		ReturnType::meta.name() = "ReturnType";
		ReturnType::meta.isAbstract() = false;
		ReturnType::meta.stereotype() = "Reference";
		TwowayOperation::meta.name() = "TwowayOperation";
		TwowayOperation::meta.isAbstract() = false;
		TwowayOperation::meta.stereotype() = "Model";
		Object::meta.name() = "Object";
		Object::meta.isAbstract() = false;
		Object::meta.stereotype() = "Model";
		Inherits::meta.name() = "Inherits";
		Inherits::meta.isAbstract() = false;
		Inherits::meta.stereotype() = "Reference";
		Event::meta.name() = "Event";
		Event::meta.isAbstract() = false;
		Event::meta.stereotype() = "Model";
		ValueObject::meta.name() = "ValueObject";
		ValueObject::meta.isAbstract() = false;
		ValueObject::meta.stereotype() = "Model";
		HasOperations::meta.name() = "HasOperations";
		HasOperations::meta.isAbstract() = true;
		HasOperations::meta.stereotype() = "Model";
		SupportsInterfaces::meta.name() = "SupportsInterfaces";
		SupportsInterfaces::meta.isAbstract() = true;
		SupportsInterfaces::meta.stereotype() = "Model";
		Supports::meta.name() = "Supports";
		Supports::meta.isAbstract() = false;
		Supports::meta.stereotype() = "Reference";
		ReadonlyAttribute::meta.name() = "ReadonlyAttribute";
		ReadonlyAttribute::meta.isAbstract() = false;
		ReadonlyAttribute::meta.stereotype() = "Model";
		Attribute::meta.name() = "Attribute";
		Attribute::meta.isAbstract() = false;
		Attribute::meta.stereotype() = "Model";
		LookupKey::meta.name() = "LookupKey";
		LookupKey::meta.isAbstract() = false;
		LookupKey::meta.stereotype() = "Reference";
		SetException::meta.name() = "SetException";
		SetException::meta.isAbstract() = false;
		SetException::meta.stereotype() = "Reference";
		ObjectByValue::meta.name() = "ObjectByValue";
		ObjectByValue::meta.isAbstract() = true;
		ObjectByValue::meta.stereotype() = "Model";
		GetException::meta.name() = "GetException";
		GetException::meta.isAbstract() = false;
		GetException::meta.stereotype() = "Reference";
		PrivateFlag::meta.name() = "PrivateFlag";
		PrivateFlag::meta.isAbstract() = false;
		PrivateFlag::meta.stereotype() = "Atom";
		MakeMemberPrivate::meta.name() = "MakeMemberPrivate";
		MakeMemberPrivate::meta.isAbstract() = false;
		MakeMemberPrivate::meta.stereotype() = "Connection";
		AttributeMember::meta.name() = "AttributeMember";
		AttributeMember::meta.isAbstract() = false;
		AttributeMember::meta.stereotype() = "Reference";
		Inheritable::meta.name() = "Inheritable";
		Inheritable::meta.isAbstract() = true;
		Inheritable::meta.stereotype() = "Model";
		EnumValue::meta.name() = "EnumValue";
		EnumValue::meta.isAbstract() = false;
		EnumValue::meta.stereotype() = "Atom";
		Aggregate::meta.name() = "Aggregate";
		Aggregate::meta.isAbstract() = false;
		Aggregate::meta.stereotype() = "Model";
		SwitchedAggregate::meta.name() = "SwitchedAggregate";
		SwitchedAggregate::meta.isAbstract() = false;
		SwitchedAggregate::meta.stereotype() = "Model";
		Label::meta.name() = "Label";
		Label::meta.isAbstract() = false;
		Label::meta.stereotype() = "Atom";
		LabelConnection::meta.name() = "LabelConnection";
		LabelConnection::meta.isAbstract() = false;
		LabelConnection::meta.stereotype() = "Connection";
		MemberType::meta.name() = "MemberType";
		MemberType::meta.isAbstract() = true;
		MemberType::meta.stereotype() = "FCO";
		Discriminator::meta.name() = "Discriminator";
		Discriminator::meta.isAbstract() = false;
		Discriminator::meta.stereotype() = "Reference";
		Collection::meta.name() = "Collection";
		Collection::meta.isAbstract() = false;
		Collection::meta.stereotype() = "Reference";
		Alias::meta.name() = "Alias";
		Alias::meta.isAbstract() = false;
		Alias::meta.stereotype() = "Reference";
		Boxed::meta.name() = "Boxed";
		Boxed::meta.isAbstract() = false;
		Boxed::meta.stereotype() = "Reference";
		NoInheritable::meta.name() = "NoInheritable";
		NoInheritable::meta.isAbstract() = true;
		NoInheritable::meta.stereotype() = "FCO";
		Enum::meta.name() = "Enum";
		Enum::meta.isAbstract() = false;
		Enum::meta.stereotype() = "Model";
		Member::meta.name() = "Member";
		Member::meta.isAbstract() = false;
		Member::meta.stereotype() = "Reference";
		NamedType::meta.name() = "NamedType";
		NamedType::meta.isAbstract() = true;
		NamedType::meta.stereotype() = "FCO";
		PredefinedType::meta.name() = "PredefinedType";
		PredefinedType::meta.isAbstract() = true;
		PredefinedType::meta.stereotype() = "FCO";
		PredefinedTypes::meta.name() = "PredefinedTypes";
		PredefinedTypes::meta.isAbstract() = false;
		PredefinedTypes::meta.stereotype() = "Folder";
		Byte::meta.name() = "Byte";
		Byte::meta.isAbstract() = false;
		Byte::meta.stereotype() = "Atom";
		Boolean::meta.name() = "Boolean";
		Boolean::meta.isAbstract() = false;
		Boolean::meta.stereotype() = "Atom";
		ShortInteger::meta.name() = "ShortInteger";
		ShortInteger::meta.isAbstract() = false;
		ShortInteger::meta.stereotype() = "Atom";
		LongInteger::meta.name() = "LongInteger";
		LongInteger::meta.isAbstract() = false;
		LongInteger::meta.stereotype() = "Atom";
		RealNumber::meta.name() = "RealNumber";
		RealNumber::meta.isAbstract() = false;
		RealNumber::meta.stereotype() = "Atom";
		String::meta.name() = "String";
		String::meta.isAbstract() = false;
		String::meta.stereotype() = "Atom";
		GenericObject::meta.name() = "GenericObject";
		GenericObject::meta.isAbstract() = false;
		GenericObject::meta.stereotype() = "Atom";
		GenericValueObject::meta.name() = "GenericValueObject";
		GenericValueObject::meta.isAbstract() = false;
		GenericValueObject::meta.stereotype() = "Atom";
		GenericValue::meta.name() = "GenericValue";
		GenericValue::meta.isAbstract() = false;
		GenericValue::meta.stereotype() = "Atom";
		TypeEncoding::meta.name() = "TypeEncoding";
		TypeEncoding::meta.isAbstract() = false;
		TypeEncoding::meta.stereotype() = "Atom";
		TypeKind::meta.name() = "TypeKind";
		TypeKind::meta.isAbstract() = false;
		TypeKind::meta.stereotype() = "Atom";
		Package::meta.name() = "Package";
		Package::meta.isAbstract() = false;
		Package::meta.stereotype() = "Model";
		File::meta.name() = "File";
		File::meta.isAbstract() = false;
		File::meta.stereotype() = "Model";
		Exception::meta.name() = "Exception";
		Exception::meta.isAbstract() = false;
		Exception::meta.stereotype() = "Model";
		ExceptionRef::meta.name() = "ExceptionRef";
		ExceptionRef::meta.isAbstract() = false;
		ExceptionRef::meta.stereotype() = "Reference";
		InterfaceDefinitions::meta.name() = "InterfaceDefinitions";
		InterfaceDefinitions::meta.isAbstract() = false;
		InterfaceDefinitions::meta.stereotype() = "Folder";
		ConstantType::meta.name() = "ConstantType";
		ConstantType::meta.isAbstract() = true;
		ConstantType::meta.stereotype() = "FCO";
		Constant::meta.name() = "Constant";
		Constant::meta.isAbstract() = false;
		Constant::meta.stereotype() = "Reference";
		FileRef::meta.name() = "FileRef";
		FileRef::meta.isAbstract() = false;
		FileRef::meta.stereotype() = "Reference";
		ComponentBuild::meta.name() = "ComponentBuild";
		ComponentBuild::meta.isAbstract() = false;
		ComponentBuild::meta.stereotype() = "Folder";
		Project::meta.name() = "Project";
		Project::meta.isAbstract() = false;
		Project::meta.stereotype() = "Model";
		ExternalResources::meta.name() = "ExternalResources";
		ExternalResources::meta.isAbstract() = false;
		ExternalResources::meta.stereotype() = "Reference";
		ExtResourceConn::meta.name() = "ExtResourceConn";
		ExtResourceConn::meta.isAbstract() = false;
		ExtResourceConn::meta.stereotype() = "Connection";
		ComponentLib::meta.name() = "ComponentLib";
		ComponentLib::meta.isAbstract() = true;
		ComponentLib::meta.stereotype() = "FCO";
		MPC::meta.name() = "MPC";
		MPC::meta.isAbstract() = false;
		MPC::meta.stereotype() = "Model";
		StubProject::meta.name() = "StubProject";
		StubProject::meta.isAbstract() = false;
		StubProject::meta.stereotype() = "Model";
		ServantProject::meta.name() = "ServantProject";
		ServantProject::meta.isAbstract() = false;
		ServantProject::meta.stereotype() = "Model";
		ExecutorProject::meta.name() = "ExecutorProject";
		ExecutorProject::meta.isAbstract() = false;
		ExecutorProject::meta.stereotype() = "Model";
		Workspaces::meta.name() = "Workspaces";
		Workspaces::meta.isAbstract() = false;
		Workspaces::meta.stereotype() = "Set";
		RootFolder::meta.name() = "RootFolder";
		RootFolder::meta.isAbstract() = false;
		RootFolder::meta.stereotype() = "Folder";
		MgaObject::meta.name() = "MgaObject";
		MgaObject::meta.isAbstract() = true;
		
		RT_Info::meta_entry_point.name() = "entry_point";
		RT_Info::meta_entry_point.type() = "String";
		RT_Info::meta_entry_point.min() = 1;
		RT_Info::meta_entry_point.max() = 1;
		RT_Info::meta_entry_point.nonpersistent() = false;
		RT_Info::meta_entry_point.registry() = false;
		RT_Info::meta_entry_point.ordered() = false;
		RT_Info::meta_entry_point.visibility() = "public";
		RT_Info::meta_entry_point.defvalue() = vector<string>();
		RT_Info::meta_period.name() = "period";
		RT_Info::meta_period.type() = "Integer";
		RT_Info::meta_period.min() = 1;
		RT_Info::meta_period.max() = 1;
		RT_Info::meta_period.nonpersistent() = false;
		RT_Info::meta_period.registry() = false;
		RT_Info::meta_period.ordered() = false;
		RT_Info::meta_period.visibility() = "public";
		RT_Info::meta_period.defvalue() = vector<string>();
		vector<string> RT_Info_enabled_dva;
		RT_Info_enabled_dva.push_back("DEPENDENCY_NON_VOLATILE");
		RT_Info::meta_enabled.name() = "enabled";
		RT_Info::meta_enabled.type() = "String";
		RT_Info::meta_enabled.min() = 1;
		RT_Info::meta_enabled.max() = 1;
		RT_Info::meta_enabled.nonpersistent() = false;
		RT_Info::meta_enabled.registry() = false;
		RT_Info::meta_enabled.ordered() = false;
		RT_Info::meta_enabled.visibility() = "public";
		RT_Info::meta_enabled.defvalue() = RT_Info_enabled_dva;
		vector<string> RT_Info_criticality_dva;
		RT_Info_criticality_dva.push_back("MEDIUM_CRITICALITY");
		RT_Info::meta_criticality.name() = "criticality";
		RT_Info::meta_criticality.type() = "String";
		RT_Info::meta_criticality.min() = 1;
		RT_Info::meta_criticality.max() = 1;
		RT_Info::meta_criticality.nonpersistent() = false;
		RT_Info::meta_criticality.registry() = false;
		RT_Info::meta_criticality.ordered() = false;
		RT_Info::meta_criticality.visibility() = "public";
		RT_Info::meta_criticality.defvalue() = RT_Info_criticality_dva;
		vector<string> RT_Info_importance_dva;
		RT_Info_importance_dva.push_back("MEDIUM_IMPORTANCE");
		RT_Info::meta_importance.name() = "importance";
		RT_Info::meta_importance.type() = "String";
		RT_Info::meta_importance.min() = 1;
		RT_Info::meta_importance.max() = 1;
		RT_Info::meta_importance.nonpersistent() = false;
		RT_Info::meta_importance.registry() = false;
		RT_Info::meta_importance.ordered() = false;
		RT_Info::meta_importance.visibility() = "public";
		RT_Info::meta_importance.defvalue() = RT_Info_importance_dva;
		RT_Info::meta_quantum.name() = "quantum";
		RT_Info::meta_quantum.type() = "Integer";
		RT_Info::meta_quantum.min() = 1;
		RT_Info::meta_quantum.max() = 1;
		RT_Info::meta_quantum.nonpersistent() = false;
		RT_Info::meta_quantum.registry() = false;
		RT_Info::meta_quantum.ordered() = false;
		RT_Info::meta_quantum.visibility() = "public";
		RT_Info::meta_quantum.defvalue() = vector<string>();
		RT_Info::meta_cached_execution_time.name() = "cached_execution_time";
		RT_Info::meta_cached_execution_time.type() = "Integer";
		RT_Info::meta_cached_execution_time.min() = 1;
		RT_Info::meta_cached_execution_time.max() = 1;
		RT_Info::meta_cached_execution_time.nonpersistent() = false;
		RT_Info::meta_cached_execution_time.registry() = false;
		RT_Info::meta_cached_execution_time.ordered() = false;
		RT_Info::meta_cached_execution_time.visibility() = "public";
		RT_Info::meta_cached_execution_time.defvalue() = vector<string>();
		vector<string> RT_Info_threads_dva;
		RT_Info_threads_dva.push_back("1");
		RT_Info::meta_threads.name() = "threads";
		RT_Info::meta_threads.type() = "Integer";
		RT_Info::meta_threads.min() = 1;
		RT_Info::meta_threads.max() = 1;
		RT_Info::meta_threads.nonpersistent() = false;
		RT_Info::meta_threads.registry() = false;
		RT_Info::meta_threads.ordered() = false;
		RT_Info::meta_threads.visibility() = "public";
		RT_Info::meta_threads.defvalue() = RT_Info_threads_dva;
		RT_Info::meta_info_type.name() = "info_type";
		RT_Info::meta_info_type.type() = "String";
		RT_Info::meta_info_type.min() = 1;
		RT_Info::meta_info_type.max() = 1;
		RT_Info::meta_info_type.nonpersistent() = false;
		RT_Info::meta_info_type.registry() = false;
		RT_Info::meta_info_type.ordered() = false;
		RT_Info::meta_info_type.visibility() = "public";
		RT_Info::meta_info_type.defvalue() = vector<string>();
		RT_Info::meta_worst_case_execution_time.name() = "worst_case_execution_time";
		RT_Info::meta_worst_case_execution_time.type() = "Integer";
		RT_Info::meta_worst_case_execution_time.min() = 1;
		RT_Info::meta_worst_case_execution_time.max() = 1;
		RT_Info::meta_worst_case_execution_time.nonpersistent() = false;
		RT_Info::meta_worst_case_execution_time.registry() = false;
		RT_Info::meta_worst_case_execution_time.ordered() = false;
		RT_Info::meta_worst_case_execution_time.visibility() = "public";
		RT_Info::meta_worst_case_execution_time.defvalue() = vector<string>();
		RT_Info::meta_typical_execution_time.name() = "typical_execution_time";
		RT_Info::meta_typical_execution_time.type() = "Integer";
		RT_Info::meta_typical_execution_time.min() = 1;
		RT_Info::meta_typical_execution_time.max() = 1;
		RT_Info::meta_typical_execution_time.nonpersistent() = false;
		RT_Info::meta_typical_execution_time.registry() = false;
		RT_Info::meta_typical_execution_time.ordered() = false;
		RT_Info::meta_typical_execution_time.visibility() = "public";
		RT_Info::meta_typical_execution_time.defvalue() = vector<string>();
		vector<string> RTEC_Resource_Factory_ECConsumerLock_dva;
		RTEC_Resource_Factory_ECConsumerLock_dva.push_back("thread (default)");
		RTEC_Resource_Factory::meta_ECConsumerLock.name() = "ECConsumerLock";
		RTEC_Resource_Factory::meta_ECConsumerLock.type() = "String";
		RTEC_Resource_Factory::meta_ECConsumerLock.min() = 1;
		RTEC_Resource_Factory::meta_ECConsumerLock.max() = 1;
		RTEC_Resource_Factory::meta_ECConsumerLock.nonpersistent() = false;
		RTEC_Resource_Factory::meta_ECConsumerLock.registry() = false;
		RTEC_Resource_Factory::meta_ECConsumerLock.ordered() = false;
		RTEC_Resource_Factory::meta_ECConsumerLock.visibility() = "public";
		RTEC_Resource_Factory::meta_ECConsumerLock.defvalue() = RTEC_Resource_Factory_ECConsumerLock_dva;
		vector<string> RTEC_Resource_Factory_ECSupplierFiltering_dva;
		RTEC_Resource_Factory_ECSupplierFiltering_dva.push_back("per-supplier (default)");
		RTEC_Resource_Factory::meta_ECSupplierFiltering.name() = "ECSupplierFiltering";
		RTEC_Resource_Factory::meta_ECSupplierFiltering.type() = "String";
		RTEC_Resource_Factory::meta_ECSupplierFiltering.min() = 1;
		RTEC_Resource_Factory::meta_ECSupplierFiltering.max() = 1;
		RTEC_Resource_Factory::meta_ECSupplierFiltering.nonpersistent() = false;
		RTEC_Resource_Factory::meta_ECSupplierFiltering.registry() = false;
		RTEC_Resource_Factory::meta_ECSupplierFiltering.ordered() = false;
		RTEC_Resource_Factory::meta_ECSupplierFiltering.visibility() = "public";
		RTEC_Resource_Factory::meta_ECSupplierFiltering.defvalue() = RTEC_Resource_Factory_ECSupplierFiltering_dva;
		vector<string> RTEC_Resource_Factory_ECDispatching_dva;
		RTEC_Resource_Factory_ECDispatching_dva.push_back("reactive (default)");
		RTEC_Resource_Factory::meta_ECDispatching.name() = "ECDispatching";
		RTEC_Resource_Factory::meta_ECDispatching.type() = "String";
		RTEC_Resource_Factory::meta_ECDispatching.min() = 1;
		RTEC_Resource_Factory::meta_ECDispatching.max() = 1;
		RTEC_Resource_Factory::meta_ECDispatching.nonpersistent() = false;
		RTEC_Resource_Factory::meta_ECDispatching.registry() = false;
		RTEC_Resource_Factory::meta_ECDispatching.ordered() = false;
		RTEC_Resource_Factory::meta_ECDispatching.visibility() = "public";
		RTEC_Resource_Factory::meta_ECDispatching.defvalue() = RTEC_Resource_Factory_ECDispatching_dva;
		vector<string> RTEC_Resource_Factory_ECTimeout_dva;
		RTEC_Resource_Factory_ECTimeout_dva.push_back("reactive (default)");
		RTEC_Resource_Factory::meta_ECTimeout.name() = "ECTimeout";
		RTEC_Resource_Factory::meta_ECTimeout.type() = "String";
		RTEC_Resource_Factory::meta_ECTimeout.min() = 1;
		RTEC_Resource_Factory::meta_ECTimeout.max() = 1;
		RTEC_Resource_Factory::meta_ECTimeout.nonpersistent() = false;
		RTEC_Resource_Factory::meta_ECTimeout.registry() = false;
		RTEC_Resource_Factory::meta_ECTimeout.ordered() = false;
		RTEC_Resource_Factory::meta_ECTimeout.visibility() = "public";
		RTEC_Resource_Factory::meta_ECTimeout.defvalue() = RTEC_Resource_Factory_ECTimeout_dva;
		vector<string> RTEC_Resource_Factory_ECConsumerControlPeriod_dva;
		RTEC_Resource_Factory_ECConsumerControlPeriod_dva.push_back("5000000");
		RTEC_Resource_Factory::meta_ECConsumerControlPeriod.name() = "ECConsumerControlPeriod";
		RTEC_Resource_Factory::meta_ECConsumerControlPeriod.type() = "Integer";
		RTEC_Resource_Factory::meta_ECConsumerControlPeriod.min() = 1;
		RTEC_Resource_Factory::meta_ECConsumerControlPeriod.max() = 1;
		RTEC_Resource_Factory::meta_ECConsumerControlPeriod.nonpersistent() = false;
		RTEC_Resource_Factory::meta_ECConsumerControlPeriod.registry() = false;
		RTEC_Resource_Factory::meta_ECConsumerControlPeriod.ordered() = false;
		RTEC_Resource_Factory::meta_ECConsumerControlPeriod.visibility() = "public";
		RTEC_Resource_Factory::meta_ECConsumerControlPeriod.defvalue() = RTEC_Resource_Factory_ECConsumerControlPeriod_dva;
		vector<string> RTEC_Resource_Factory_ECSupplierLock_dva;
		RTEC_Resource_Factory_ECSupplierLock_dva.push_back("thread (default)");
		RTEC_Resource_Factory::meta_ECSupplierLock.name() = "ECSupplierLock";
		RTEC_Resource_Factory::meta_ECSupplierLock.type() = "String";
		RTEC_Resource_Factory::meta_ECSupplierLock.min() = 1;
		RTEC_Resource_Factory::meta_ECSupplierLock.max() = 1;
		RTEC_Resource_Factory::meta_ECSupplierLock.nonpersistent() = false;
		RTEC_Resource_Factory::meta_ECSupplierLock.registry() = false;
		RTEC_Resource_Factory::meta_ECSupplierLock.ordered() = false;
		RTEC_Resource_Factory::meta_ECSupplierLock.visibility() = "public";
		RTEC_Resource_Factory::meta_ECSupplierLock.defvalue() = RTEC_Resource_Factory_ECSupplierLock_dva;
		vector<string> RTEC_Resource_Factory_ECObserver_dva;
		RTEC_Resource_Factory_ECObserver_dva.push_back("null (default)");
		RTEC_Resource_Factory::meta_ECObserver.name() = "ECObserver";
		RTEC_Resource_Factory::meta_ECObserver.type() = "String";
		RTEC_Resource_Factory::meta_ECObserver.min() = 1;
		RTEC_Resource_Factory::meta_ECObserver.max() = 1;
		RTEC_Resource_Factory::meta_ECObserver.nonpersistent() = false;
		RTEC_Resource_Factory::meta_ECObserver.registry() = false;
		RTEC_Resource_Factory::meta_ECObserver.ordered() = false;
		RTEC_Resource_Factory::meta_ECObserver.visibility() = "public";
		RTEC_Resource_Factory::meta_ECObserver.defvalue() = RTEC_Resource_Factory_ECObserver_dva;
		vector<string> RTEC_Resource_Factory_ECSupplierControl_dva;
		RTEC_Resource_Factory_ECSupplierControl_dva.push_back("null (default)");
		RTEC_Resource_Factory::meta_ECSupplierControl.name() = "ECSupplierControl";
		RTEC_Resource_Factory::meta_ECSupplierControl.type() = "String";
		RTEC_Resource_Factory::meta_ECSupplierControl.min() = 1;
		RTEC_Resource_Factory::meta_ECSupplierControl.max() = 1;
		RTEC_Resource_Factory::meta_ECSupplierControl.nonpersistent() = false;
		RTEC_Resource_Factory::meta_ECSupplierControl.registry() = false;
		RTEC_Resource_Factory::meta_ECSupplierControl.ordered() = false;
		RTEC_Resource_Factory::meta_ECSupplierControl.visibility() = "public";
		RTEC_Resource_Factory::meta_ECSupplierControl.defvalue() = RTEC_Resource_Factory_ECSupplierControl_dva;
		vector<string> RTEC_Resource_Factory_ECConsumerControl_dva;
		RTEC_Resource_Factory_ECConsumerControl_dva.push_back("null (default)");
		RTEC_Resource_Factory::meta_ECConsumerControl.name() = "ECConsumerControl";
		RTEC_Resource_Factory::meta_ECConsumerControl.type() = "String";
		RTEC_Resource_Factory::meta_ECConsumerControl.min() = 1;
		RTEC_Resource_Factory::meta_ECConsumerControl.max() = 1;
		RTEC_Resource_Factory::meta_ECConsumerControl.nonpersistent() = false;
		RTEC_Resource_Factory::meta_ECConsumerControl.registry() = false;
		RTEC_Resource_Factory::meta_ECConsumerControl.ordered() = false;
		RTEC_Resource_Factory::meta_ECConsumerControl.visibility() = "public";
		RTEC_Resource_Factory::meta_ECConsumerControl.defvalue() = RTEC_Resource_Factory_ECConsumerControl_dva;
		vector<string> RTEC_Resource_Factory_ECFiltering_dva;
		RTEC_Resource_Factory_ECFiltering_dva.push_back("basic (default)");
		RTEC_Resource_Factory::meta_ECFiltering.name() = "ECFiltering";
		RTEC_Resource_Factory::meta_ECFiltering.type() = "String";
		RTEC_Resource_Factory::meta_ECFiltering.min() = 1;
		RTEC_Resource_Factory::meta_ECFiltering.max() = 1;
		RTEC_Resource_Factory::meta_ECFiltering.nonpersistent() = false;
		RTEC_Resource_Factory::meta_ECFiltering.registry() = false;
		RTEC_Resource_Factory::meta_ECFiltering.ordered() = false;
		RTEC_Resource_Factory::meta_ECFiltering.visibility() = "public";
		RTEC_Resource_Factory::meta_ECFiltering.defvalue() = RTEC_Resource_Factory_ECFiltering_dva;
		vector<string> RTEC_Resource_Factory_ECProxyPushConsumerCollectionFlag_dva;
		RTEC_Resource_Factory_ECProxyPushConsumerCollectionFlag_dva.push_back("list (default)");
		RTEC_Resource_Factory::meta_ECProxyPushConsumerCollectionFlag.name() = "ECProxyPushConsumerCollectionFlag";
		RTEC_Resource_Factory::meta_ECProxyPushConsumerCollectionFlag.type() = "String";
		RTEC_Resource_Factory::meta_ECProxyPushConsumerCollectionFlag.min() = 1;
		RTEC_Resource_Factory::meta_ECProxyPushConsumerCollectionFlag.max() = 1;
		RTEC_Resource_Factory::meta_ECProxyPushConsumerCollectionFlag.nonpersistent() = false;
		RTEC_Resource_Factory::meta_ECProxyPushConsumerCollectionFlag.registry() = false;
		RTEC_Resource_Factory::meta_ECProxyPushConsumerCollectionFlag.ordered() = false;
		RTEC_Resource_Factory::meta_ECProxyPushConsumerCollectionFlag.visibility() = "public";
		RTEC_Resource_Factory::meta_ECProxyPushConsumerCollectionFlag.defvalue() = RTEC_Resource_Factory_ECProxyPushConsumerCollectionFlag_dva;
		RTEC_Resource_Factory::meta_ECUseORBId.name() = "ECUseORBId";
		RTEC_Resource_Factory::meta_ECUseORBId.type() = "String";
		RTEC_Resource_Factory::meta_ECUseORBId.min() = 1;
		RTEC_Resource_Factory::meta_ECUseORBId.max() = 1;
		RTEC_Resource_Factory::meta_ECUseORBId.nonpersistent() = false;
		RTEC_Resource_Factory::meta_ECUseORBId.registry() = false;
		RTEC_Resource_Factory::meta_ECUseORBId.ordered() = false;
		RTEC_Resource_Factory::meta_ECUseORBId.visibility() = "public";
		RTEC_Resource_Factory::meta_ECUseORBId.defvalue() = vector<string>();
		vector<string> RTEC_Resource_Factory_ECProxyPushConsumerCollectionSynch_dva;
		RTEC_Resource_Factory_ECProxyPushConsumerCollectionSynch_dva.push_back("mt (default)");
		RTEC_Resource_Factory::meta_ECProxyPushConsumerCollectionSynch.name() = "ECProxyPushConsumerCollectionSynch";
		RTEC_Resource_Factory::meta_ECProxyPushConsumerCollectionSynch.type() = "String";
		RTEC_Resource_Factory::meta_ECProxyPushConsumerCollectionSynch.min() = 1;
		RTEC_Resource_Factory::meta_ECProxyPushConsumerCollectionSynch.max() = 1;
		RTEC_Resource_Factory::meta_ECProxyPushConsumerCollectionSynch.nonpersistent() = false;
		RTEC_Resource_Factory::meta_ECProxyPushConsumerCollectionSynch.registry() = false;
		RTEC_Resource_Factory::meta_ECProxyPushConsumerCollectionSynch.ordered() = false;
		RTEC_Resource_Factory::meta_ECProxyPushConsumerCollectionSynch.visibility() = "public";
		RTEC_Resource_Factory::meta_ECProxyPushConsumerCollectionSynch.defvalue() = RTEC_Resource_Factory_ECProxyPushConsumerCollectionSynch_dva;
		vector<string> RTEC_Resource_Factory_ECProxyPushConsumerCollectionIterator_dva;
		RTEC_Resource_Factory_ECProxyPushConsumerCollectionIterator_dva.push_back("immediate (default)");
		RTEC_Resource_Factory::meta_ECProxyPushConsumerCollectionIterator.name() = "ECProxyPushConsumerCollectionIterator";
		RTEC_Resource_Factory::meta_ECProxyPushConsumerCollectionIterator.type() = "String";
		RTEC_Resource_Factory::meta_ECProxyPushConsumerCollectionIterator.min() = 1;
		RTEC_Resource_Factory::meta_ECProxyPushConsumerCollectionIterator.max() = 1;
		RTEC_Resource_Factory::meta_ECProxyPushConsumerCollectionIterator.nonpersistent() = false;
		RTEC_Resource_Factory::meta_ECProxyPushConsumerCollectionIterator.registry() = false;
		RTEC_Resource_Factory::meta_ECProxyPushConsumerCollectionIterator.ordered() = false;
		RTEC_Resource_Factory::meta_ECProxyPushConsumerCollectionIterator.visibility() = "public";
		RTEC_Resource_Factory::meta_ECProxyPushConsumerCollectionIterator.defvalue() = RTEC_Resource_Factory_ECProxyPushConsumerCollectionIterator_dva;
		vector<string> RTEC_Resource_Factory_ECSupplierControlPeriod_dva;
		RTEC_Resource_Factory_ECSupplierControlPeriod_dva.push_back("5000000");
		RTEC_Resource_Factory::meta_ECSupplierControlPeriod.name() = "ECSupplierControlPeriod";
		RTEC_Resource_Factory::meta_ECSupplierControlPeriod.type() = "Integer";
		RTEC_Resource_Factory::meta_ECSupplierControlPeriod.min() = 1;
		RTEC_Resource_Factory::meta_ECSupplierControlPeriod.max() = 1;
		RTEC_Resource_Factory::meta_ECSupplierControlPeriod.nonpersistent() = false;
		RTEC_Resource_Factory::meta_ECSupplierControlPeriod.registry() = false;
		RTEC_Resource_Factory::meta_ECSupplierControlPeriod.ordered() = false;
		RTEC_Resource_Factory::meta_ECSupplierControlPeriod.visibility() = "public";
		RTEC_Resource_Factory::meta_ECSupplierControlPeriod.defvalue() = RTEC_Resource_Factory_ECSupplierControlPeriod_dva;
		vector<string> RTEC_Resource_Factory_ECScheduling_dva;
		RTEC_Resource_Factory_ECScheduling_dva.push_back("null (default)");
		RTEC_Resource_Factory::meta_ECScheduling.name() = "ECScheduling";
		RTEC_Resource_Factory::meta_ECScheduling.type() = "String";
		RTEC_Resource_Factory::meta_ECScheduling.min() = 1;
		RTEC_Resource_Factory::meta_ECScheduling.max() = 1;
		RTEC_Resource_Factory::meta_ECScheduling.nonpersistent() = false;
		RTEC_Resource_Factory::meta_ECScheduling.registry() = false;
		RTEC_Resource_Factory::meta_ECScheduling.ordered() = false;
		RTEC_Resource_Factory::meta_ECScheduling.visibility() = "public";
		RTEC_Resource_Factory::meta_ECScheduling.defvalue() = RTEC_Resource_Factory_ECScheduling_dva;
		vector<string> RTEC_Resource_Factory_ECDispatchingThreads_dva;
		RTEC_Resource_Factory_ECDispatchingThreads_dva.push_back("1");
		RTEC_Resource_Factory::meta_ECDispatchingThreads.name() = "ECDispatchingThreads";
		RTEC_Resource_Factory::meta_ECDispatchingThreads.type() = "Integer";
		RTEC_Resource_Factory::meta_ECDispatchingThreads.min() = 1;
		RTEC_Resource_Factory::meta_ECDispatchingThreads.max() = 1;
		RTEC_Resource_Factory::meta_ECDispatchingThreads.nonpersistent() = false;
		RTEC_Resource_Factory::meta_ECDispatchingThreads.registry() = false;
		RTEC_Resource_Factory::meta_ECDispatchingThreads.ordered() = false;
		RTEC_Resource_Factory::meta_ECDispatchingThreads.visibility() = "public";
		RTEC_Resource_Factory::meta_ECDispatchingThreads.defvalue() = RTEC_Resource_Factory_ECDispatchingThreads_dva;
		vector<string> RTEC_Resource_Factory_ECProxyPushSupplierCollectionFlag_dva;
		RTEC_Resource_Factory_ECProxyPushSupplierCollectionFlag_dva.push_back("list (default)");
		RTEC_Resource_Factory::meta_ECProxyPushSupplierCollectionFlag.name() = "ECProxyPushSupplierCollectionFlag";
		RTEC_Resource_Factory::meta_ECProxyPushSupplierCollectionFlag.type() = "String";
		RTEC_Resource_Factory::meta_ECProxyPushSupplierCollectionFlag.min() = 1;
		RTEC_Resource_Factory::meta_ECProxyPushSupplierCollectionFlag.max() = 1;
		RTEC_Resource_Factory::meta_ECProxyPushSupplierCollectionFlag.nonpersistent() = false;
		RTEC_Resource_Factory::meta_ECProxyPushSupplierCollectionFlag.registry() = false;
		RTEC_Resource_Factory::meta_ECProxyPushSupplierCollectionFlag.ordered() = false;
		RTEC_Resource_Factory::meta_ECProxyPushSupplierCollectionFlag.visibility() = "public";
		RTEC_Resource_Factory::meta_ECProxyPushSupplierCollectionFlag.defvalue() = RTEC_Resource_Factory_ECProxyPushSupplierCollectionFlag_dva;
		vector<string> RTEC_Resource_Factory_ECProxyPushSupplierCollectionSynch_dva;
		RTEC_Resource_Factory_ECProxyPushSupplierCollectionSynch_dva.push_back("mt (default)");
		RTEC_Resource_Factory::meta_ECProxyPushSupplierCollectionSynch.name() = "ECProxyPushSupplierCollectionSynch";
		RTEC_Resource_Factory::meta_ECProxyPushSupplierCollectionSynch.type() = "String";
		RTEC_Resource_Factory::meta_ECProxyPushSupplierCollectionSynch.min() = 1;
		RTEC_Resource_Factory::meta_ECProxyPushSupplierCollectionSynch.max() = 1;
		RTEC_Resource_Factory::meta_ECProxyPushSupplierCollectionSynch.nonpersistent() = false;
		RTEC_Resource_Factory::meta_ECProxyPushSupplierCollectionSynch.registry() = false;
		RTEC_Resource_Factory::meta_ECProxyPushSupplierCollectionSynch.ordered() = false;
		RTEC_Resource_Factory::meta_ECProxyPushSupplierCollectionSynch.visibility() = "public";
		RTEC_Resource_Factory::meta_ECProxyPushSupplierCollectionSynch.defvalue() = RTEC_Resource_Factory_ECProxyPushSupplierCollectionSynch_dva;
		vector<string> RTEC_Resource_Factory_ECProxyPushSupplierCollectionIterator_dva;
		RTEC_Resource_Factory_ECProxyPushSupplierCollectionIterator_dva.push_back("immediate (default)");
		RTEC_Resource_Factory::meta_ECProxyPushSupplierCollectionIterator.name() = "ECProxyPushSupplierCollectionIterator";
		RTEC_Resource_Factory::meta_ECProxyPushSupplierCollectionIterator.type() = "String";
		RTEC_Resource_Factory::meta_ECProxyPushSupplierCollectionIterator.min() = 1;
		RTEC_Resource_Factory::meta_ECProxyPushSupplierCollectionIterator.max() = 1;
		RTEC_Resource_Factory::meta_ECProxyPushSupplierCollectionIterator.nonpersistent() = false;
		RTEC_Resource_Factory::meta_ECProxyPushSupplierCollectionIterator.registry() = false;
		RTEC_Resource_Factory::meta_ECProxyPushSupplierCollectionIterator.ordered() = false;
		RTEC_Resource_Factory::meta_ECProxyPushSupplierCollectionIterator.visibility() = "public";
		RTEC_Resource_Factory::meta_ECProxyPushSupplierCollectionIterator.defvalue() = RTEC_Resource_Factory_ECProxyPushSupplierCollectionIterator_dva;
		EventChannelConfig::meta_name.name() = "name";
		EventChannelConfig::meta_name.type() = "String";
		EventChannelConfig::meta_name.min() = 0;
		EventChannelConfig::meta_name.max() = 1;
		EventChannelConfig::meta_name.nonpersistent() = false;
		EventChannelConfig::meta_name.registry() = false;
		EventChannelConfig::meta_name.ordered() = false;
		EventChannelConfig::meta_name.visibility() = "public";
		EventChannelConfig::meta_name.defvalue() = vector<string>();
		ComponentAnalyses::meta_name.name() = "name";
		ComponentAnalyses::meta_name.type() = "String";
		ComponentAnalyses::meta_name.min() = 0;
		ComponentAnalyses::meta_name.max() = 1;
		ComponentAnalyses::meta_name.nonpersistent() = false;
		ComponentAnalyses::meta_name.registry() = false;
		ComponentAnalyses::meta_name.ordered() = false;
		ComponentAnalyses::meta_name.visibility() = "public";
		ComponentAnalyses::meta_name.defvalue() = vector<string>();
		MetricsBase::meta_warmup.name() = "warmup";
		MetricsBase::meta_warmup.type() = "Integer";
		MetricsBase::meta_warmup.min() = 1;
		MetricsBase::meta_warmup.max() = 1;
		MetricsBase::meta_warmup.nonpersistent() = false;
		MetricsBase::meta_warmup.registry() = false;
		MetricsBase::meta_warmup.ordered() = false;
		MetricsBase::meta_warmup.visibility() = "public";
		MetricsBase::meta_warmup.defvalue() = vector<string>();
		MetricsBase::meta_fileName.name() = "fileName";
		MetricsBase::meta_fileName.type() = "String";
		MetricsBase::meta_fileName.min() = 1;
		MetricsBase::meta_fileName.max() = 1;
		MetricsBase::meta_fileName.nonpersistent() = false;
		MetricsBase::meta_fileName.registry() = false;
		MetricsBase::meta_fileName.ordered() = false;
		MetricsBase::meta_fileName.visibility() = "public";
		MetricsBase::meta_fileName.defvalue() = vector<string>();
		MetricsBase::meta_iterations.name() = "iterations";
		MetricsBase::meta_iterations.type() = "Integer";
		MetricsBase::meta_iterations.min() = 1;
		MetricsBase::meta_iterations.max() = 1;
		MetricsBase::meta_iterations.nonpersistent() = false;
		MetricsBase::meta_iterations.registry() = false;
		MetricsBase::meta_iterations.ordered() = false;
		MetricsBase::meta_iterations.visibility() = "public";
		MetricsBase::meta_iterations.defvalue() = vector<string>();
		vector<string> Task_rate_dva;
		Task_rate_dva.push_back("0");
		Task::meta_rate.name() = "rate";
		Task::meta_rate.type() = "Integer";
		Task::meta_rate.min() = 1;
		Task::meta_rate.max() = 1;
		Task::meta_rate.nonpersistent() = false;
		Task::meta_rate.registry() = false;
		Task::meta_rate.ordered() = false;
		Task::meta_rate.visibility() = "public";
		Task::meta_rate.defvalue() = Task_rate_dva;
		Task::meta_priority.name() = "priority";
		Task::meta_priority.type() = "Integer";
		Task::meta_priority.min() = 1;
		Task::meta_priority.max() = 1;
		Task::meta_priority.nonpersistent() = false;
		Task::meta_priority.registry() = false;
		Task::meta_priority.ordered() = false;
		Task::meta_priority.visibility() = "public";
		Task::meta_priority.defvalue() = vector<string>();
		Interconnect::meta_label.name() = "label";
		Interconnect::meta_label.type() = "String";
		Interconnect::meta_label.min() = 1;
		Interconnect::meta_label.max() = 1;
		Interconnect::meta_label.nonpersistent() = false;
		Interconnect::meta_label.registry() = false;
		Interconnect::meta_label.ordered() = false;
		Interconnect::meta_label.visibility() = "public";
		Interconnect::meta_label.defvalue() = vector<string>();
		Node::meta_label.name() = "label";
		Node::meta_label.type() = "String";
		Node::meta_label.min() = 1;
		Node::meta_label.max() = 1;
		Node::meta_label.nonpersistent() = false;
		Node::meta_label.registry() = false;
		Node::meta_label.ordered() = false;
		Node::meta_label.visibility() = "public";
		Node::meta_label.defvalue() = vector<string>();
		Resource::meta_resourceType.name() = "resourceType";
		Resource::meta_resourceType.type() = "String";
		Resource::meta_resourceType.min() = 1;
		Resource::meta_resourceType.max() = 1;
		Resource::meta_resourceType.nonpersistent() = false;
		Resource::meta_resourceType.registry() = false;
		Resource::meta_resourceType.ordered() = false;
		Resource::meta_resourceType.visibility() = "public";
		Resource::meta_resourceType.defvalue() = vector<string>();
		Bridge::meta_label.name() = "label";
		Bridge::meta_label.type() = "String";
		Bridge::meta_label.min() = 1;
		Bridge::meta_label.max() = 1;
		Bridge::meta_label.nonpersistent() = false;
		Bridge::meta_label.registry() = false;
		Bridge::meta_label.ordered() = false;
		Bridge::meta_label.visibility() = "public";
		Bridge::meta_label.defvalue() = vector<string>();
		SharedResource::meta_resourceType.name() = "resourceType";
		SharedResource::meta_resourceType.type() = "String";
		SharedResource::meta_resourceType.min() = 1;
		SharedResource::meta_resourceType.max() = 1;
		SharedResource::meta_resourceType.nonpersistent() = false;
		SharedResource::meta_resourceType.registry() = false;
		SharedResource::meta_resourceType.ordered() = false;
		SharedResource::meta_resourceType.visibility() = "public";
		SharedResource::meta_resourceType.defvalue() = vector<string>();
		DeploymentPlans::meta_name.name() = "name";
		DeploymentPlans::meta_name.type() = "String";
		DeploymentPlans::meta_name.min() = 0;
		DeploymentPlans::meta_name.max() = 1;
		DeploymentPlans::meta_name.nonpersistent() = false;
		DeploymentPlans::meta_name.registry() = false;
		DeploymentPlans::meta_name.ordered() = false;
		DeploymentPlans::meta_name.visibility() = "public";
		DeploymentPlans::meta_name.defvalue() = vector<string>();
		DeploymentPlan::meta_label.name() = "label";
		DeploymentPlan::meta_label.type() = "String";
		DeploymentPlan::meta_label.min() = 1;
		DeploymentPlan::meta_label.max() = 1;
		DeploymentPlan::meta_label.nonpersistent() = false;
		DeploymentPlan::meta_label.registry() = false;
		DeploymentPlan::meta_label.ordered() = false;
		DeploymentPlan::meta_label.visibility() = "public";
		DeploymentPlan::meta_label.defvalue() = vector<string>();
		ExternalReferenceEndPoint::meta_location.name() = "location";
		ExternalReferenceEndPoint::meta_location.type() = "String";
		ExternalReferenceEndPoint::meta_location.min() = 1;
		ExternalReferenceEndPoint::meta_location.max() = 1;
		ExternalReferenceEndPoint::meta_location.nonpersistent() = false;
		ExternalReferenceEndPoint::meta_location.registry() = false;
		ExternalReferenceEndPoint::meta_location.ordered() = false;
		ExternalReferenceEndPoint::meta_location.visibility() = "public";
		ExternalReferenceEndPoint::meta_location.defvalue() = vector<string>();
		ComponentImplementations::meta_name.name() = "name";
		ComponentImplementations::meta_name.type() = "String";
		ComponentImplementations::meta_name.min() = 0;
		ComponentImplementations::meta_name.max() = 1;
		ComponentImplementations::meta_name.nonpersistent() = false;
		ComponentImplementations::meta_name.registry() = false;
		ComponentImplementations::meta_name.ordered() = false;
		ComponentImplementations::meta_name.visibility() = "public";
		ComponentImplementations::meta_name.defvalue() = vector<string>();
		ComponentImplementation::meta_UUID.name() = "UUID";
		ComponentImplementation::meta_UUID.type() = "String";
		ComponentImplementation::meta_UUID.min() = 1;
		ComponentImplementation::meta_UUID.max() = 1;
		ComponentImplementation::meta_UUID.nonpersistent() = false;
		ComponentImplementation::meta_UUID.registry() = false;
		ComponentImplementation::meta_UUID.ordered() = false;
		ComponentImplementation::meta_UUID.visibility() = "public";
		ComponentImplementation::meta_UUID.defvalue() = vector<string>();
		ComponentImplementation::meta_label.name() = "label";
		ComponentImplementation::meta_label.type() = "String";
		ComponentImplementation::meta_label.min() = 1;
		ComponentImplementation::meta_label.max() = 1;
		ComponentImplementation::meta_label.nonpersistent() = false;
		ComponentImplementation::meta_label.registry() = false;
		ComponentImplementation::meta_label.ordered() = false;
		ComponentImplementation::meta_label.visibility() = "public";
		ComponentImplementation::meta_label.defvalue() = vector<string>();
		PackageConfigurations::meta_name.name() = "name";
		PackageConfigurations::meta_name.type() = "String";
		PackageConfigurations::meta_name.min() = 0;
		PackageConfigurations::meta_name.max() = 1;
		PackageConfigurations::meta_name.nonpersistent() = false;
		PackageConfigurations::meta_name.registry() = false;
		PackageConfigurations::meta_name.ordered() = false;
		PackageConfigurations::meta_name.visibility() = "public";
		PackageConfigurations::meta_name.defvalue() = vector<string>();
		PackageConfiguration::meta_UUID.name() = "UUID";
		PackageConfiguration::meta_UUID.type() = "String";
		PackageConfiguration::meta_UUID.min() = 1;
		PackageConfiguration::meta_UUID.max() = 1;
		PackageConfiguration::meta_UUID.nonpersistent() = false;
		PackageConfiguration::meta_UUID.registry() = false;
		PackageConfiguration::meta_UUID.ordered() = false;
		PackageConfiguration::meta_UUID.visibility() = "public";
		PackageConfiguration::meta_UUID.defvalue() = vector<string>();
		PackageConfiguration::meta_label.name() = "label";
		PackageConfiguration::meta_label.type() = "String";
		PackageConfiguration::meta_label.min() = 1;
		PackageConfiguration::meta_label.max() = 1;
		PackageConfiguration::meta_label.nonpersistent() = false;
		PackageConfiguration::meta_label.registry() = false;
		PackageConfiguration::meta_label.ordered() = false;
		PackageConfiguration::meta_label.visibility() = "public";
		PackageConfiguration::meta_label.defvalue() = vector<string>();
		ImplementationArtifacts::meta_name.name() = "name";
		ImplementationArtifacts::meta_name.type() = "String";
		ImplementationArtifacts::meta_name.min() = 0;
		ImplementationArtifacts::meta_name.max() = 1;
		ImplementationArtifacts::meta_name.nonpersistent() = false;
		ImplementationArtifacts::meta_name.registry() = false;
		ImplementationArtifacts::meta_name.ordered() = false;
		ImplementationArtifacts::meta_name.visibility() = "public";
		ImplementationArtifacts::meta_name.defvalue() = vector<string>();
		vector<string> ImplementationArtifact_tree_file_name_dva;
		ImplementationArtifact_tree_file_name_dva.push_back("orb_tree.xml");
		ImplementationArtifact::meta_tree_file_name.name() = "tree_file_name";
		ImplementationArtifact::meta_tree_file_name.type() = "String";
		ImplementationArtifact::meta_tree_file_name.min() = 1;
		ImplementationArtifact::meta_tree_file_name.max() = 1;
		ImplementationArtifact::meta_tree_file_name.nonpersistent() = false;
		ImplementationArtifact::meta_tree_file_name.registry() = false;
		ImplementationArtifact::meta_tree_file_name.ordered() = false;
		ImplementationArtifact::meta_tree_file_name.visibility() = "public";
		ImplementationArtifact::meta_tree_file_name.defvalue() = ImplementationArtifact_tree_file_name_dva;
		ImplementationArtifact::meta_configuration.name() = "configuration";
		ImplementationArtifact::meta_configuration.type() = "String";
		ImplementationArtifact::meta_configuration.min() = 1;
		ImplementationArtifact::meta_configuration.max() = 1;
		ImplementationArtifact::meta_configuration.nonpersistent() = false;
		ImplementationArtifact::meta_configuration.registry() = false;
		ImplementationArtifact::meta_configuration.ordered() = false;
		ImplementationArtifact::meta_configuration.visibility() = "public";
		ImplementationArtifact::meta_configuration.defvalue() = vector<string>();
		ImplementationArtifact::meta_label.name() = "label";
		ImplementationArtifact::meta_label.type() = "String";
		ImplementationArtifact::meta_label.min() = 1;
		ImplementationArtifact::meta_label.max() = 1;
		ImplementationArtifact::meta_label.nonpersistent() = false;
		ImplementationArtifact::meta_label.registry() = false;
		ImplementationArtifact::meta_label.ordered() = false;
		ImplementationArtifact::meta_label.visibility() = "public";
		ImplementationArtifact::meta_label.defvalue() = vector<string>();
		ImplementationArtifact::meta_UUID.name() = "UUID";
		ImplementationArtifact::meta_UUID.type() = "String";
		ImplementationArtifact::meta_UUID.min() = 1;
		ImplementationArtifact::meta_UUID.max() = 1;
		ImplementationArtifact::meta_UUID.nonpersistent() = false;
		ImplementationArtifact::meta_UUID.registry() = false;
		ImplementationArtifact::meta_UUID.ordered() = false;
		ImplementationArtifact::meta_UUID.visibility() = "public";
		ImplementationArtifact::meta_UUID.defvalue() = vector<string>();
		ImplementationArtifact::meta_location.name() = "location";
		ImplementationArtifact::meta_location.type() = "String";
		ImplementationArtifact::meta_location.min() = 1;
		ImplementationArtifact::meta_location.max() = 1;
		ImplementationArtifact::meta_location.nonpersistent() = false;
		ImplementationArtifact::meta_location.registry() = false;
		ImplementationArtifact::meta_location.ordered() = false;
		ImplementationArtifact::meta_location.visibility() = "public";
		ImplementationArtifact::meta_location.defvalue() = vector<string>();
		ComponentTypes::meta_name.name() = "name";
		ComponentTypes::meta_name.type() = "String";
		ComponentTypes::meta_name.min() = 0;
		ComponentTypes::meta_name.max() = 1;
		ComponentTypes::meta_name.nonpersistent() = false;
		ComponentTypes::meta_name.registry() = false;
		ComponentTypes::meta_name.ordered() = false;
		ComponentTypes::meta_name.visibility() = "public";
		ComponentTypes::meta_name.defvalue() = vector<string>();
		vector<string> CommonPortAttrs_exclusiveUser_dva;
		CommonPortAttrs_exclusiveUser_dva.push_back("false");
		CommonPortAttrs::meta_exclusiveUser.name() = "exclusiveUser";
		CommonPortAttrs::meta_exclusiveUser.type() = "Boolean";
		CommonPortAttrs::meta_exclusiveUser.min() = 1;
		CommonPortAttrs::meta_exclusiveUser.max() = 1;
		CommonPortAttrs::meta_exclusiveUser.nonpersistent() = false;
		CommonPortAttrs::meta_exclusiveUser.registry() = false;
		CommonPortAttrs::meta_exclusiveUser.ordered() = false;
		CommonPortAttrs::meta_exclusiveUser.visibility() = "public";
		CommonPortAttrs::meta_exclusiveUser.defvalue() = CommonPortAttrs_exclusiveUser_dva;
		vector<string> CommonPortAttrs_optional_dva;
		CommonPortAttrs_optional_dva.push_back("false");
		CommonPortAttrs::meta_optional.name() = "optional";
		CommonPortAttrs::meta_optional.type() = "Boolean";
		CommonPortAttrs::meta_optional.min() = 1;
		CommonPortAttrs::meta_optional.max() = 1;
		CommonPortAttrs::meta_optional.nonpersistent() = false;
		CommonPortAttrs::meta_optional.registry() = false;
		CommonPortAttrs::meta_optional.ordered() = false;
		CommonPortAttrs::meta_optional.visibility() = "public";
		CommonPortAttrs::meta_optional.defvalue() = CommonPortAttrs_optional_dva;
		vector<string> CommonPortAttrs_exclusiveProvider_dva;
		CommonPortAttrs_exclusiveProvider_dva.push_back("false");
		CommonPortAttrs::meta_exclusiveProvider.name() = "exclusiveProvider";
		CommonPortAttrs::meta_exclusiveProvider.type() = "Boolean";
		CommonPortAttrs::meta_exclusiveProvider.min() = 1;
		CommonPortAttrs::meta_exclusiveProvider.max() = 1;
		CommonPortAttrs::meta_exclusiveProvider.nonpersistent() = false;
		CommonPortAttrs::meta_exclusiveProvider.registry() = false;
		CommonPortAttrs::meta_exclusiveProvider.ordered() = false;
		CommonPortAttrs::meta_exclusiveProvider.visibility() = "public";
		CommonPortAttrs::meta_exclusiveProvider.defvalue() = CommonPortAttrs_exclusiveProvider_dva;
		TopLevelPackages::meta_name.name() = "name";
		TopLevelPackages::meta_name.type() = "String";
		TopLevelPackages::meta_name.min() = 0;
		TopLevelPackages::meta_name.max() = 1;
		TopLevelPackages::meta_name.nonpersistent() = false;
		TopLevelPackages::meta_name.registry() = false;
		TopLevelPackages::meta_name.ordered() = false;
		TopLevelPackages::meta_name.visibility() = "public";
		TopLevelPackages::meta_name.defvalue() = vector<string>();
		ComponentPackages::meta_name.name() = "name";
		ComponentPackages::meta_name.type() = "String";
		ComponentPackages::meta_name.min() = 0;
		ComponentPackages::meta_name.max() = 1;
		ComponentPackages::meta_name.nonpersistent() = false;
		ComponentPackages::meta_name.registry() = false;
		ComponentPackages::meta_name.ordered() = false;
		ComponentPackages::meta_name.visibility() = "public";
		ComponentPackages::meta_name.defvalue() = vector<string>();
		ComponentPackage::meta_UUID.name() = "UUID";
		ComponentPackage::meta_UUID.type() = "String";
		ComponentPackage::meta_UUID.min() = 1;
		ComponentPackage::meta_UUID.max() = 1;
		ComponentPackage::meta_UUID.nonpersistent() = false;
		ComponentPackage::meta_UUID.registry() = false;
		ComponentPackage::meta_UUID.ordered() = false;
		ComponentPackage::meta_UUID.visibility() = "public";
		ComponentPackage::meta_UUID.defvalue() = vector<string>();
		ComponentPackage::meta_label.name() = "label";
		ComponentPackage::meta_label.type() = "String";
		ComponentPackage::meta_label.min() = 1;
		ComponentPackage::meta_label.max() = 1;
		ComponentPackage::meta_label.nonpersistent() = false;
		ComponentPackage::meta_label.registry() = false;
		ComponentPackage::meta_label.ordered() = false;
		ComponentPackage::meta_label.visibility() = "public";
		ComponentPackage::meta_label.defvalue() = vector<string>();
		ComponentPackageReference::meta_requiredName.name() = "requiredName";
		ComponentPackageReference::meta_requiredName.type() = "String";
		ComponentPackageReference::meta_requiredName.min() = 1;
		ComponentPackageReference::meta_requiredName.max() = 1;
		ComponentPackageReference::meta_requiredName.nonpersistent() = false;
		ComponentPackageReference::meta_requiredName.registry() = false;
		ComponentPackageReference::meta_requiredName.ordered() = false;
		ComponentPackageReference::meta_requiredName.visibility() = "public";
		ComponentPackageReference::meta_requiredName.defvalue() = vector<string>();
		ComponentPackageReference::meta_requiredUUID.name() = "requiredUUID";
		ComponentPackageReference::meta_requiredUUID.type() = "String";
		ComponentPackageReference::meta_requiredUUID.min() = 1;
		ComponentPackageReference::meta_requiredUUID.max() = 1;
		ComponentPackageReference::meta_requiredUUID.nonpersistent() = false;
		ComponentPackageReference::meta_requiredUUID.registry() = false;
		ComponentPackageReference::meta_requiredUUID.ordered() = false;
		ComponentPackageReference::meta_requiredUUID.visibility() = "public";
		ComponentPackageReference::meta_requiredUUID.defvalue() = vector<string>();
		ComponentPackageReference::meta_requiredType.name() = "requiredType";
		ComponentPackageReference::meta_requiredType.type() = "String";
		ComponentPackageReference::meta_requiredType.min() = 1;
		ComponentPackageReference::meta_requiredType.max() = 1;
		ComponentPackageReference::meta_requiredType.nonpersistent() = false;
		ComponentPackageReference::meta_requiredType.registry() = false;
		ComponentPackageReference::meta_requiredType.ordered() = false;
		ComponentPackageReference::meta_requiredType.visibility() = "public";
		ComponentPackageReference::meta_requiredType.defvalue() = vector<string>();
		SatisfierProperty::meta_DataValue.name() = "DataValue";
		SatisfierProperty::meta_DataValue.type() = "String";
		SatisfierProperty::meta_DataValue.min() = 1;
		SatisfierProperty::meta_DataValue.max() = 1;
		SatisfierProperty::meta_DataValue.nonpersistent() = false;
		SatisfierProperty::meta_DataValue.registry() = false;
		SatisfierProperty::meta_DataValue.ordered() = false;
		SatisfierProperty::meta_DataValue.visibility() = "public";
		SatisfierProperty::meta_DataValue.defvalue() = vector<string>();
		vector<string> SatisfierProperty_SatisfierPropertyKind_dva;
		SatisfierProperty_SatisfierPropertyKind_dva.push_back("Quantity");
		SatisfierProperty::meta_SatisfierPropertyKind.name() = "SatisfierPropertyKind";
		SatisfierProperty::meta_SatisfierPropertyKind.type() = "String";
		SatisfierProperty::meta_SatisfierPropertyKind.min() = 1;
		SatisfierProperty::meta_SatisfierPropertyKind.max() = 1;
		SatisfierProperty::meta_SatisfierPropertyKind.nonpersistent() = false;
		SatisfierProperty::meta_SatisfierPropertyKind.registry() = false;
		SatisfierProperty::meta_SatisfierPropertyKind.ordered() = false;
		SatisfierProperty::meta_SatisfierPropertyKind.visibility() = "public";
		SatisfierProperty::meta_SatisfierPropertyKind.defvalue() = SatisfierProperty_SatisfierPropertyKind_dva;
		ImplementationDependency::meta_requiredType.name() = "requiredType";
		ImplementationDependency::meta_requiredType.type() = "String";
		ImplementationDependency::meta_requiredType.min() = 1;
		ImplementationDependency::meta_requiredType.max() = 1;
		ImplementationDependency::meta_requiredType.nonpersistent() = false;
		ImplementationDependency::meta_requiredType.registry() = false;
		ImplementationDependency::meta_requiredType.ordered() = false;
		ImplementationDependency::meta_requiredType.visibility() = "public";
		ImplementationDependency::meta_requiredType.defvalue() = vector<string>();
		RequirementSatisfier::meta_resourceType.name() = "resourceType";
		RequirementSatisfier::meta_resourceType.type() = "String";
		RequirementSatisfier::meta_resourceType.min() = 1;
		RequirementSatisfier::meta_resourceType.max() = 1;
		RequirementSatisfier::meta_resourceType.nonpersistent() = false;
		RequirementSatisfier::meta_resourceType.registry() = false;
		RequirementSatisfier::meta_resourceType.ordered() = false;
		RequirementSatisfier::meta_resourceType.visibility() = "public";
		RequirementSatisfier::meta_resourceType.defvalue() = vector<string>();
		RequirementBase::meta_resourceType.name() = "resourceType";
		RequirementBase::meta_resourceType.type() = "String";
		RequirementBase::meta_resourceType.min() = 1;
		RequirementBase::meta_resourceType.max() = 1;
		RequirementBase::meta_resourceType.nonpersistent() = false;
		RequirementBase::meta_resourceType.registry() = false;
		RequirementBase::meta_resourceType.ordered() = false;
		RequirementBase::meta_resourceType.visibility() = "public";
		RequirementBase::meta_resourceType.defvalue() = vector<string>();
		vector<string> ImplementationRequirement_ResourceUsageKind_dva;
		ImplementationRequirement_ResourceUsageKind_dva.push_back("None");
		ImplementationRequirement::meta_ResourceUsageKind.name() = "ResourceUsageKind";
		ImplementationRequirement::meta_ResourceUsageKind.type() = "String";
		ImplementationRequirement::meta_ResourceUsageKind.min() = 1;
		ImplementationRequirement::meta_ResourceUsageKind.max() = 1;
		ImplementationRequirement::meta_ResourceUsageKind.nonpersistent() = false;
		ImplementationRequirement::meta_ResourceUsageKind.registry() = false;
		ImplementationRequirement::meta_ResourceUsageKind.ordered() = false;
		ImplementationRequirement::meta_ResourceUsageKind.visibility() = "public";
		ImplementationRequirement::meta_ResourceUsageKind.defvalue() = ImplementationRequirement_ResourceUsageKind_dva;
		ImplementationRequirement::meta_componentPort.name() = "componentPort";
		ImplementationRequirement::meta_componentPort.type() = "String";
		ImplementationRequirement::meta_componentPort.min() = 1;
		ImplementationRequirement::meta_componentPort.max() = 1;
		ImplementationRequirement::meta_componentPort.nonpersistent() = false;
		ImplementationRequirement::meta_componentPort.registry() = false;
		ImplementationRequirement::meta_componentPort.ordered() = false;
		ImplementationRequirement::meta_componentPort.visibility() = "public";
		ImplementationRequirement::meta_componentPort.defvalue() = vector<string>();
		ImplementationRequirement::meta_resourcePort.name() = "resourcePort";
		ImplementationRequirement::meta_resourcePort.type() = "String";
		ImplementationRequirement::meta_resourcePort.min() = 1;
		ImplementationRequirement::meta_resourcePort.max() = 1;
		ImplementationRequirement::meta_resourcePort.nonpersistent() = false;
		ImplementationRequirement::meta_resourcePort.registry() = false;
		ImplementationRequirement::meta_resourcePort.ordered() = false;
		ImplementationRequirement::meta_resourcePort.visibility() = "public";
		ImplementationRequirement::meta_resourcePort.defvalue() = vector<string>();
		Property::meta_DataValue.name() = "DataValue";
		Property::meta_DataValue.type() = "String";
		Property::meta_DataValue.min() = 1;
		Property::meta_DataValue.max() = 1;
		Property::meta_DataValue.nonpersistent() = false;
		Property::meta_DataValue.registry() = false;
		Property::meta_DataValue.ordered() = false;
		Property::meta_DataValue.visibility() = "public";
		Property::meta_DataValue.defvalue() = vector<string>();
		Targets::meta_name.name() = "name";
		Targets::meta_name.type() = "String";
		Targets::meta_name.min() = 0;
		Targets::meta_name.max() = 1;
		Targets::meta_name.nonpersistent() = false;
		Targets::meta_name.registry() = false;
		Targets::meta_name.ordered() = false;
		Targets::meta_name.visibility() = "public";
		Targets::meta_name.defvalue() = vector<string>();
		Domain::meta_label.name() = "label";
		Domain::meta_label.type() = "String";
		Domain::meta_label.min() = 1;
		Domain::meta_label.max() = 1;
		Domain::meta_label.nonpersistent() = false;
		Domain::meta_label.registry() = false;
		Domain::meta_label.ordered() = false;
		Domain::meta_label.visibility() = "public";
		Domain::meta_label.defvalue() = vector<string>();
		Domain::meta_UUID.name() = "UUID";
		Domain::meta_UUID.type() = "String";
		Domain::meta_UUID.min() = 1;
		Domain::meta_UUID.max() = 1;
		Domain::meta_UUID.nonpersistent() = false;
		Domain::meta_UUID.registry() = false;
		Domain::meta_UUID.ordered() = false;
		Domain::meta_UUID.visibility() = "public";
		Domain::meta_UUID.defvalue() = vector<string>();
		Prefixable::meta_PrefixTag.name() = "PrefixTag";
		Prefixable::meta_PrefixTag.type() = "String";
		Prefixable::meta_PrefixTag.min() = 1;
		Prefixable::meta_PrefixTag.max() = 1;
		Prefixable::meta_PrefixTag.nonpersistent() = false;
		Prefixable::meta_PrefixTag.registry() = false;
		Prefixable::meta_PrefixTag.ordered() = false;
		Prefixable::meta_PrefixTag.visibility() = "public";
		Prefixable::meta_PrefixTag.defvalue() = vector<string>();
		Taggable::meta_SpecifyIdTag.name() = "SpecifyIdTag";
		Taggable::meta_SpecifyIdTag.type() = "String";
		Taggable::meta_SpecifyIdTag.min() = 1;
		Taggable::meta_SpecifyIdTag.max() = 1;
		Taggable::meta_SpecifyIdTag.nonpersistent() = false;
		Taggable::meta_SpecifyIdTag.registry() = false;
		Taggable::meta_SpecifyIdTag.ordered() = false;
		Taggable::meta_SpecifyIdTag.visibility() = "public";
		Taggable::meta_SpecifyIdTag.defvalue() = vector<string>();
		Taggable::meta_VersionTag.name() = "VersionTag";
		Taggable::meta_VersionTag.type() = "String";
		Taggable::meta_VersionTag.min() = 1;
		Taggable::meta_VersionTag.max() = 1;
		Taggable::meta_VersionTag.nonpersistent() = false;
		Taggable::meta_VersionTag.registry() = false;
		Taggable::meta_VersionTag.ordered() = false;
		Taggable::meta_VersionTag.visibility() = "public";
		Taggable::meta_VersionTag.defvalue() = vector<string>();
		vector<string> OutEventPort_out_event_port_type_dva;
		OutEventPort_out_event_port_type_dva.push_back("Direct_Connect");
		OutEventPort::meta_out_event_port_type.name() = "out_event_port_type";
		OutEventPort::meta_out_event_port_type.type() = "String";
		OutEventPort::meta_out_event_port_type.min() = 1;
		OutEventPort::meta_out_event_port_type.max() = 1;
		OutEventPort::meta_out_event_port_type.nonpersistent() = false;
		OutEventPort::meta_out_event_port_type.registry() = false;
		OutEventPort::meta_out_event_port_type.ordered() = false;
		OutEventPort::meta_out_event_port_type.visibility() = "public";
		OutEventPort::meta_out_event_port_type.defvalue() = OutEventPort_out_event_port_type_dva;
		vector<string> OutEventPort_single_destination_dva;
		OutEventPort_single_destination_dva.push_back("false");
		OutEventPort::meta_single_destination.name() = "single_destination";
		OutEventPort::meta_single_destination.type() = "Boolean";
		OutEventPort::meta_single_destination.min() = 1;
		OutEventPort::meta_single_destination.max() = 1;
		OutEventPort::meta_single_destination.nonpersistent() = false;
		OutEventPort::meta_single_destination.registry() = false;
		OutEventPort::meta_single_destination.ordered() = false;
		OutEventPort::meta_single_destination.visibility() = "public";
		OutEventPort::meta_single_destination.defvalue() = OutEventPort_single_destination_dva;
		vector<string> RequiredRequestPort_multiple_connections_dva;
		RequiredRequestPort_multiple_connections_dva.push_back("false");
		RequiredRequestPort::meta_multiple_connections.name() = "multiple_connections";
		RequiredRequestPort::meta_multiple_connections.type() = "Boolean";
		RequiredRequestPort::meta_multiple_connections.min() = 1;
		RequiredRequestPort::meta_multiple_connections.max() = 1;
		RequiredRequestPort::meta_multiple_connections.nonpersistent() = false;
		RequiredRequestPort::meta_multiple_connections.registry() = false;
		RequiredRequestPort::meta_multiple_connections.ordered() = false;
		RequiredRequestPort::meta_multiple_connections.visibility() = "public";
		RequiredRequestPort::meta_multiple_connections.defvalue() = RequiredRequestPort_multiple_connections_dva;
		Component::meta_UUID.name() = "UUID";
		Component::meta_UUID.type() = "String";
		Component::meta_UUID.min() = 1;
		Component::meta_UUID.max() = 1;
		Component::meta_UUID.nonpersistent() = false;
		Component::meta_UUID.registry() = false;
		Component::meta_UUID.ordered() = false;
		Component::meta_UUID.visibility() = "public";
		Component::meta_UUID.defvalue() = vector<string>();
		Component::meta_label.name() = "label";
		Component::meta_label.type() = "String";
		Component::meta_label.min() = 1;
		Component::meta_label.max() = 1;
		Component::meta_label.nonpersistent() = false;
		Component::meta_label.registry() = false;
		Component::meta_label.ordered() = false;
		Component::meta_label.visibility() = "public";
		Component::meta_label.defvalue() = vector<string>();
		vector<string> Object_local_dva;
		Object_local_dva.push_back("false");
		Object::meta_local.name() = "local";
		Object::meta_local.type() = "Boolean";
		Object::meta_local.min() = 1;
		Object::meta_local.max() = 1;
		Object::meta_local.nonpersistent() = false;
		Object::meta_local.registry() = false;
		Object::meta_local.ordered() = false;
		Object::meta_local.visibility() = "public";
		Object::meta_local.defvalue() = Object_local_dva;
		vector<string> Object_abstract_dva;
		Object_abstract_dva.push_back("false");
		Object::meta_abstract.name() = "abstract";
		Object::meta_abstract.type() = "Boolean";
		Object::meta_abstract.min() = 1;
		Object::meta_abstract.max() = 1;
		Object::meta_abstract.nonpersistent() = false;
		Object::meta_abstract.registry() = false;
		Object::meta_abstract.ordered() = false;
		Object::meta_abstract.visibility() = "public";
		Object::meta_abstract.defvalue() = Object_abstract_dva;
		vector<string> ObjectByValue_abstract_dva;
		ObjectByValue_abstract_dva.push_back("false");
		ObjectByValue::meta_abstract.name() = "abstract";
		ObjectByValue::meta_abstract.type() = "Boolean";
		ObjectByValue::meta_abstract.min() = 1;
		ObjectByValue::meta_abstract.max() = 1;
		ObjectByValue::meta_abstract.nonpersistent() = false;
		ObjectByValue::meta_abstract.registry() = false;
		ObjectByValue::meta_abstract.ordered() = false;
		ObjectByValue::meta_abstract.visibility() = "public";
		ObjectByValue::meta_abstract.defvalue() = ObjectByValue_abstract_dva;
		PredefinedTypes::meta_name.name() = "name";
		PredefinedTypes::meta_name.type() = "String";
		PredefinedTypes::meta_name.min() = 0;
		PredefinedTypes::meta_name.max() = 1;
		PredefinedTypes::meta_name.nonpersistent() = false;
		PredefinedTypes::meta_name.registry() = false;
		PredefinedTypes::meta_name.ordered() = false;
		PredefinedTypes::meta_name.visibility() = "public";
		PredefinedTypes::meta_name.defvalue() = vector<string>();
		InterfaceDefinitions::meta_name.name() = "name";
		InterfaceDefinitions::meta_name.type() = "String";
		InterfaceDefinitions::meta_name.min() = 0;
		InterfaceDefinitions::meta_name.max() = 1;
		InterfaceDefinitions::meta_name.nonpersistent() = false;
		InterfaceDefinitions::meta_name.registry() = false;
		InterfaceDefinitions::meta_name.ordered() = false;
		InterfaceDefinitions::meta_name.visibility() = "public";
		InterfaceDefinitions::meta_name.defvalue() = vector<string>();
		Constant::meta_value.name() = "value";
		Constant::meta_value.type() = "String";
		Constant::meta_value.min() = 1;
		Constant::meta_value.max() = 1;
		Constant::meta_value.nonpersistent() = false;
		Constant::meta_value.registry() = false;
		Constant::meta_value.ordered() = false;
		Constant::meta_value.visibility() = "public";
		Constant::meta_value.defvalue() = vector<string>();
		ComponentBuild::meta_name.name() = "name";
		ComponentBuild::meta_name.type() = "String";
		ComponentBuild::meta_name.min() = 0;
		ComponentBuild::meta_name.max() = 1;
		ComponentBuild::meta_name.nonpersistent() = false;
		ComponentBuild::meta_name.registry() = false;
		ComponentBuild::meta_name.ordered() = false;
		ComponentBuild::meta_name.visibility() = "public";
		ComponentBuild::meta_name.defvalue() = vector<string>();
		ComponentLib::meta_sharedname.name() = "sharedname";
		ComponentLib::meta_sharedname.type() = "String";
		ComponentLib::meta_sharedname.min() = 1;
		ComponentLib::meta_sharedname.max() = 1;
		ComponentLib::meta_sharedname.nonpersistent() = false;
		ComponentLib::meta_sharedname.registry() = false;
		ComponentLib::meta_sharedname.ordered() = false;
		ComponentLib::meta_sharedname.visibility() = "public";
		ComponentLib::meta_sharedname.defvalue() = vector<string>();
		ComponentLib::meta_libraryexport.name() = "libraryexport";
		ComponentLib::meta_libraryexport.type() = "String";
		ComponentLib::meta_libraryexport.min() = 1;
		ComponentLib::meta_libraryexport.max() = 1;
		ComponentLib::meta_libraryexport.nonpersistent() = false;
		ComponentLib::meta_libraryexport.registry() = false;
		ComponentLib::meta_libraryexport.ordered() = false;
		ComponentLib::meta_libraryexport.visibility() = "public";
		ComponentLib::meta_libraryexport.defvalue() = vector<string>();
		ComponentLib::meta_ORBServices.name() = "ORBServices";
		ComponentLib::meta_ORBServices.type() = "String";
		ComponentLib::meta_ORBServices.min() = 1;
		ComponentLib::meta_ORBServices.max() = 1;
		ComponentLib::meta_ORBServices.nonpersistent() = false;
		ComponentLib::meta_ORBServices.registry() = false;
		ComponentLib::meta_ORBServices.ordered() = false;
		ComponentLib::meta_ORBServices.visibility() = "public";
		ComponentLib::meta_ORBServices.defvalue() = vector<string>();
		RootFolder::meta_name.name() = "name";
		RootFolder::meta_name.type() = "String";
		RootFolder::meta_name.min() = 0;
		RootFolder::meta_name.max() = 1;
		RootFolder::meta_name.nonpersistent() = false;
		RootFolder::meta_name.registry() = false;
		RootFolder::meta_name.ordered() = false;
		RootFolder::meta_name.visibility() = "public";
		RootFolder::meta_name.defvalue() = vector<string>();
		MgaObject::meta_position.name() = "position";
		MgaObject::meta_position.type() = "String";
		MgaObject::meta_position.min() = 0;
		MgaObject::meta_position.max() = 1;
		MgaObject::meta_position.nonpersistent() = false;
		MgaObject::meta_position.registry() = false;
		MgaObject::meta_position.ordered() = false;
		MgaObject::meta_position.visibility() = "public";
		MgaObject::meta_position.defvalue() = vector<string>();
		MgaObject::meta_name.name() = "name";
		MgaObject::meta_name.type() = "String";
		MgaObject::meta_name.min() = 0;
		MgaObject::meta_name.max() = 1;
		MgaObject::meta_name.nonpersistent() = false;
		MgaObject::meta_name.registry() = false;
		MgaObject::meta_name.ordered() = false;
		MgaObject::meta_name.visibility() = "public";
		MgaObject::meta_name.defvalue() = vector<string>();
		
	}
	void InitAssociation0()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		ass.name() = "Use_RT_Info";
		ass.assocClass() = Use_RT_Info::meta;
		RT_Info_Param::meta_dstUse_RT_Info = Uml::AssociationRole::Create(ass);
		RT_Info_Param::meta_dstUse_RT_Info.name() = "dstUse_RT_Info";
		RT_Info_Param::meta_dstUse_RT_Info.min() = 0;
		RT_Info_Param::meta_dstUse_RT_Info.max() = -1;
		RT_Info_Param::meta_dstUse_RT_Info.isNavigable() = true;
		RT_Info_Param::meta_dstUse_RT_Info.isPrimary() = false;
		RT_Info_Param::meta_dstUse_RT_Info.target() = RTEC_Resource_Factory_Reference::meta;
		Use_RT_Info::meta_dstUse_RT_Info_end_ = RTEC_Resource_Factory_Reference::meta_srcUse_RT_Info_rev = RT_Info_Param::meta_dstUse_RT_Info;
		RTEC_Resource_Factory_Reference::meta_srcUse_RT_Info = Uml::AssociationRole::Create(ass);
		RTEC_Resource_Factory_Reference::meta_srcUse_RT_Info.name() = "srcUse_RT_Info";
		RTEC_Resource_Factory_Reference::meta_srcUse_RT_Info.min() = 0;
		RTEC_Resource_Factory_Reference::meta_srcUse_RT_Info.max() = -1;
		RTEC_Resource_Factory_Reference::meta_srcUse_RT_Info.isNavigable() = true;
		RTEC_Resource_Factory_Reference::meta_srcUse_RT_Info.isPrimary() = false;
		RTEC_Resource_Factory_Reference::meta_srcUse_RT_Info.target() = RT_Info_Param::meta;
		Use_RT_Info::meta_srcUse_RT_Info_end_ = RT_Info_Param::meta_dstUse_RT_Info_rev = RTEC_Resource_Factory_Reference::meta_srcUse_RT_Info;
	};
	void InitAssociation1()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		ass.name() = "Include_RT_Info";
		ass.assocClass() = Include_RT_Info::meta;
		RT_Info_Param::meta_dstInclude_RT_Info = Uml::AssociationRole::Create(ass);
		RT_Info_Param::meta_dstInclude_RT_Info.name() = "dstInclude_RT_Info";
		RT_Info_Param::meta_dstInclude_RT_Info.min() = 0;
		RT_Info_Param::meta_dstInclude_RT_Info.max() = -1;
		RT_Info_Param::meta_dstInclude_RT_Info.isNavigable() = true;
		RT_Info_Param::meta_dstInclude_RT_Info.isPrimary() = false;
		RT_Info_Param::meta_dstInclude_RT_Info.target() = RTEC_Filter_Group::meta;
		Include_RT_Info::meta_dstInclude_RT_Info_end_ = RTEC_Filter_Group::meta_srcInclude_RT_Info_rev = RT_Info_Param::meta_dstInclude_RT_Info;
		RTEC_Filter_Group::meta_srcInclude_RT_Info = Uml::AssociationRole::Create(ass);
		RTEC_Filter_Group::meta_srcInclude_RT_Info.name() = "srcInclude_RT_Info";
		RTEC_Filter_Group::meta_srcInclude_RT_Info.min() = 0;
		RTEC_Filter_Group::meta_srcInclude_RT_Info.max() = -1;
		RTEC_Filter_Group::meta_srcInclude_RT_Info.isNavigable() = true;
		RTEC_Filter_Group::meta_srcInclude_RT_Info.isPrimary() = false;
		RTEC_Filter_Group::meta_srcInclude_RT_Info.target() = RT_Info_Param::meta;
		Include_RT_Info::meta_srcInclude_RT_Info_end_ = RT_Info_Param::meta_dstInclude_RT_Info_rev = RTEC_Filter_Group::meta_srcInclude_RT_Info;
	};
	void InitAssociation2()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		ass.name() = "RTEC_Connection";
		ass.assocClass() = RTEC_Connection::meta;
		RT_Info_Param::meta_srcRTEC_Connection = Uml::AssociationRole::Create(ass);
		RT_Info_Param::meta_srcRTEC_Connection.name() = "srcRTEC_Connection";
		RT_Info_Param::meta_srcRTEC_Connection.min() = 0;
		RT_Info_Param::meta_srcRTEC_Connection.max() = -1;
		RT_Info_Param::meta_srcRTEC_Connection.isNavigable() = true;
		RT_Info_Param::meta_srcRTEC_Connection.isPrimary() = false;
		RT_Info_Param::meta_srcRTEC_Connection.target() = OutEventPort::meta;
		RTEC_Connection::meta_srcRTEC_Connection_end_ = OutEventPort::meta_dstRTEC_Connection_rev = RT_Info_Param::meta_srcRTEC_Connection;
		OutEventPort::meta_dstRTEC_Connection = Uml::AssociationRole::Create(ass);
		OutEventPort::meta_dstRTEC_Connection.name() = "dstRTEC_Connection";
		OutEventPort::meta_dstRTEC_Connection.min() = 0;
		OutEventPort::meta_dstRTEC_Connection.max() = -1;
		OutEventPort::meta_dstRTEC_Connection.isNavigable() = true;
		OutEventPort::meta_dstRTEC_Connection.isPrimary() = false;
		OutEventPort::meta_dstRTEC_Connection.target() = RT_Info_Param::meta;
		RTEC_Connection::meta_dstRTEC_Connection_end_ = RT_Info_Param::meta_srcRTEC_Connection_rev = OutEventPort::meta_dstRTEC_Connection;
	};
	void InitAssociation3()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		ass.name() = "Include_RTEC_Filter_Group";
		ass.assocClass() = Include_RTEC_Filter_Group::meta;
		Include_RTEC_Filter_Group_Members_Base::meta_srcInclude_RTEC_Filter_Group = Uml::AssociationRole::Create(ass);
		Include_RTEC_Filter_Group_Members_Base::meta_srcInclude_RTEC_Filter_Group.name() = "srcInclude_RTEC_Filter_Group";
		Include_RTEC_Filter_Group_Members_Base::meta_srcInclude_RTEC_Filter_Group.min() = 0;
		Include_RTEC_Filter_Group_Members_Base::meta_srcInclude_RTEC_Filter_Group.max() = -1;
		Include_RTEC_Filter_Group_Members_Base::meta_srcInclude_RTEC_Filter_Group.isNavigable() = true;
		Include_RTEC_Filter_Group_Members_Base::meta_srcInclude_RTEC_Filter_Group.isPrimary() = false;
		Include_RTEC_Filter_Group_Members_Base::meta_srcInclude_RTEC_Filter_Group.target() = Include_RTEC_Filter_Group_Members_Base::meta;
		Include_RTEC_Filter_Group::meta_srcInclude_RTEC_Filter_Group_end_ = Include_RTEC_Filter_Group_Members_Base::meta_dstInclude_RTEC_Filter_Group_rev = Include_RTEC_Filter_Group_Members_Base::meta_srcInclude_RTEC_Filter_Group;
		Include_RTEC_Filter_Group_Members_Base::meta_dstInclude_RTEC_Filter_Group = Uml::AssociationRole::Create(ass);
		Include_RTEC_Filter_Group_Members_Base::meta_dstInclude_RTEC_Filter_Group.name() = "dstInclude_RTEC_Filter_Group";
		Include_RTEC_Filter_Group_Members_Base::meta_dstInclude_RTEC_Filter_Group.min() = 0;
		Include_RTEC_Filter_Group_Members_Base::meta_dstInclude_RTEC_Filter_Group.max() = -1;
		Include_RTEC_Filter_Group_Members_Base::meta_dstInclude_RTEC_Filter_Group.isNavigable() = true;
		Include_RTEC_Filter_Group_Members_Base::meta_dstInclude_RTEC_Filter_Group.isPrimary() = false;
		Include_RTEC_Filter_Group_Members_Base::meta_dstInclude_RTEC_Filter_Group.target() = Include_RTEC_Filter_Group_Members_Base::meta;
		Include_RTEC_Filter_Group::meta_dstInclude_RTEC_Filter_Group_end_ = Include_RTEC_Filter_Group_Members_Base::meta_srcInclude_RTEC_Filter_Group_rev = Include_RTEC_Filter_Group_Members_Base::meta_dstInclude_RTEC_Filter_Group;
	};
	void InitAssociation4()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		ass.name() = "Use_RTEC_Filter";
		ass.assocClass() = Use_RTEC_Filter::meta;
		RTEC_Filter_Group::meta_dstUse_RTEC_Filter = Uml::AssociationRole::Create(ass);
		RTEC_Filter_Group::meta_dstUse_RTEC_Filter.name() = "dstUse_RTEC_Filter";
		RTEC_Filter_Group::meta_dstUse_RTEC_Filter.min() = 0;
		RTEC_Filter_Group::meta_dstUse_RTEC_Filter.max() = -1;
		RTEC_Filter_Group::meta_dstUse_RTEC_Filter.isNavigable() = true;
		RTEC_Filter_Group::meta_dstUse_RTEC_Filter.isPrimary() = false;
		RTEC_Filter_Group::meta_dstUse_RTEC_Filter.target() = InEventPort::meta;
		Use_RTEC_Filter::meta_dstUse_RTEC_Filter_end_ = InEventPort::meta_srcUse_RTEC_Filter_rev = RTEC_Filter_Group::meta_dstUse_RTEC_Filter;
		InEventPort::meta_srcUse_RTEC_Filter = Uml::AssociationRole::Create(ass);
		InEventPort::meta_srcUse_RTEC_Filter.name() = "srcUse_RTEC_Filter";
		InEventPort::meta_srcUse_RTEC_Filter.min() = 0;
		InEventPort::meta_srcUse_RTEC_Filter.max() = -1;
		InEventPort::meta_srcUse_RTEC_Filter.isNavigable() = true;
		InEventPort::meta_srcUse_RTEC_Filter.isPrimary() = false;
		InEventPort::meta_srcUse_RTEC_Filter.target() = RTEC_Filter_Group::meta;
		Use_RTEC_Filter::meta_srcUse_RTEC_Filter_end_ = RTEC_Filter_Group::meta_dstUse_RTEC_Filter_rev = InEventPort::meta_srcUse_RTEC_Filter;
	};
	void InitAssociation5()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		RTEC_Resource_Factory::meta_referedbyRTEC_Resource_Factory_Reference = Uml::AssociationRole::Create(ass);
		RTEC_Resource_Factory::meta_referedbyRTEC_Resource_Factory_Reference.name() = "referedbyRTEC_Resource_Factory_Reference";
		RTEC_Resource_Factory::meta_referedbyRTEC_Resource_Factory_Reference.min() = 0;
		RTEC_Resource_Factory::meta_referedbyRTEC_Resource_Factory_Reference.max() = -1;
		RTEC_Resource_Factory::meta_referedbyRTEC_Resource_Factory_Reference.isNavigable() = true;
		RTEC_Resource_Factory::meta_referedbyRTEC_Resource_Factory_Reference.isPrimary() = false;
		RTEC_Resource_Factory::meta_referedbyRTEC_Resource_Factory_Reference.target() = RTEC_Resource_Factory_Reference::meta;
		RTEC_Resource_Factory_Reference::meta_ref = Uml::AssociationRole::Create(ass);
		RTEC_Resource_Factory_Reference::meta_ref.name() = "ref";
		RTEC_Resource_Factory_Reference::meta_ref.min() = 0;
		RTEC_Resource_Factory_Reference::meta_ref.max() = 1;
		RTEC_Resource_Factory_Reference::meta_ref.isNavigable() = true;
		RTEC_Resource_Factory_Reference::meta_ref.isPrimary() = false;
		RTEC_Resource_Factory_Reference::meta_ref.target() = RTEC_Resource_Factory::meta;
	};
	void InitAssociation6()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		ass.name() = "MetricConnection";
		ass.assocClass() = MetricConnection::meta;
		MetricsBase::meta_srcMetricConnection = Uml::AssociationRole::Create(ass);
		MetricsBase::meta_srcMetricConnection.name() = "srcMetricConnection";
		MetricsBase::meta_srcMetricConnection.min() = 1;
		MetricsBase::meta_srcMetricConnection.max() = 1;
		MetricsBase::meta_srcMetricConnection.isNavigable() = true;
		MetricsBase::meta_srcMetricConnection.isPrimary() = false;
		MetricsBase::meta_srcMetricConnection.target() = OperationRef::meta;
		MetricConnection::meta_srcMetricConnection_end_ = OperationRef::meta_dstMetricConnection_rev = MetricsBase::meta_srcMetricConnection;
		OperationRef::meta_dstMetricConnection = Uml::AssociationRole::Create(ass);
		OperationRef::meta_dstMetricConnection.name() = "dstMetricConnection";
		OperationRef::meta_dstMetricConnection.min() = 0;
		OperationRef::meta_dstMetricConnection.max() = 1;
		OperationRef::meta_dstMetricConnection.isNavigable() = true;
		OperationRef::meta_dstMetricConnection.isPrimary() = false;
		OperationRef::meta_dstMetricConnection.target() = MetricsBase::meta;
		MetricConnection::meta_dstMetricConnection_end_ = MetricsBase::meta_srcMetricConnection_rev = OperationRef::meta_dstMetricConnection;
	};
	void InitAssociation7()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		ass.name() = "TimerEventSinkConn";
		ass.assocClass() = TimerEventSinkConn::meta;
		TimeProbe::meta_srcTimerEventSinkConn = Uml::AssociationRole::Create(ass);
		TimeProbe::meta_srcTimerEventSinkConn.name() = "srcTimerEventSinkConn";
		TimeProbe::meta_srcTimerEventSinkConn.min() = 0;
		TimeProbe::meta_srcTimerEventSinkConn.max() = 1;
		TimeProbe::meta_srcTimerEventSinkConn.isNavigable() = true;
		TimeProbe::meta_srcTimerEventSinkConn.isPrimary() = false;
		TimeProbe::meta_srcTimerEventSinkConn.target() = EventRef::meta;
		TimerEventSinkConn::meta_srcTimerEventSinkConn_end_ = EventRef::meta_dstTimerEventSinkConn_rev = TimeProbe::meta_srcTimerEventSinkConn;
		EventRef::meta_dstTimerEventSinkConn = Uml::AssociationRole::Create(ass);
		EventRef::meta_dstTimerEventSinkConn.name() = "dstTimerEventSinkConn";
		EventRef::meta_dstTimerEventSinkConn.min() = 0;
		EventRef::meta_dstTimerEventSinkConn.max() = -1;
		EventRef::meta_dstTimerEventSinkConn.isNavigable() = true;
		EventRef::meta_dstTimerEventSinkConn.isPrimary() = false;
		EventRef::meta_dstTimerEventSinkConn.target() = TimeProbe::meta;
		TimerEventSinkConn::meta_dstTimerEventSinkConn_end_ = TimeProbe::meta_srcTimerEventSinkConn_rev = EventRef::meta_dstTimerEventSinkConn;
	};
	void InitAssociation8()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		ass.name() = "TimerConnection";
		ass.assocClass() = TimerConnection::meta;
		TimeProbe::meta_srcTimerConnection = Uml::AssociationRole::Create(ass);
		TimeProbe::meta_srcTimerConnection.name() = "srcTimerConnection";
		TimeProbe::meta_srcTimerConnection.min() = 0;
		TimeProbe::meta_srcTimerConnection.max() = 1;
		TimeProbe::meta_srcTimerConnection.isNavigable() = true;
		TimeProbe::meta_srcTimerConnection.isPrimary() = false;
		TimeProbe::meta_srcTimerConnection.target() = OperationRef::meta;
		TimerConnection::meta_srcTimerConnection_end_ = OperationRef::meta_dstTimerConnection_rev = TimeProbe::meta_srcTimerConnection;
		OperationRef::meta_dstTimerConnection = Uml::AssociationRole::Create(ass);
		OperationRef::meta_dstTimerConnection.name() = "dstTimerConnection";
		OperationRef::meta_dstTimerConnection.min() = 0;
		OperationRef::meta_dstTimerConnection.max() = 1;
		OperationRef::meta_dstTimerConnection.isNavigable() = true;
		OperationRef::meta_dstTimerConnection.isPrimary() = false;
		OperationRef::meta_dstTimerConnection.target() = TimeProbe::meta;
		TimerConnection::meta_dstTimerConnection_end_ = TimeProbe::meta_srcTimerConnection_rev = OperationRef::meta_dstTimerConnection;
	};
	void InitAssociation9()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		ass.name() = "WorkloadCharacteristics";
		ass.assocClass() = WorkloadCharacteristics::meta;
		MetricsBase::meta_dstWorkloadCharacteristics = Uml::AssociationRole::Create(ass);
		MetricsBase::meta_dstWorkloadCharacteristics.name() = "dstWorkloadCharacteristics";
		MetricsBase::meta_dstWorkloadCharacteristics.min() = 0;
		MetricsBase::meta_dstWorkloadCharacteristics.max() = 1;
		MetricsBase::meta_dstWorkloadCharacteristics.isNavigable() = true;
		MetricsBase::meta_dstWorkloadCharacteristics.isPrimary() = false;
		MetricsBase::meta_dstWorkloadCharacteristics.target() = TaskSet::meta;
		WorkloadCharacteristics::meta_dstWorkloadCharacteristics_end_ = TaskSet::meta_srcWorkloadCharacteristics_rev = MetricsBase::meta_dstWorkloadCharacteristics;
		TaskSet::meta_srcWorkloadCharacteristics = Uml::AssociationRole::Create(ass);
		TaskSet::meta_srcWorkloadCharacteristics.name() = "srcWorkloadCharacteristics";
		TaskSet::meta_srcWorkloadCharacteristics.min() = 1;
		TaskSet::meta_srcWorkloadCharacteristics.max() = 1;
		TaskSet::meta_srcWorkloadCharacteristics.isNavigable() = true;
		TaskSet::meta_srcWorkloadCharacteristics.isPrimary() = false;
		TaskSet::meta_srcWorkloadCharacteristics.target() = MetricsBase::meta;
		WorkloadCharacteristics::meta_srcWorkloadCharacteristics_end_ = MetricsBase::meta_dstWorkloadCharacteristics_rev = TaskSet::meta_srcWorkloadCharacteristics;
	};
	void InitAssociation10()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		OperationRef::meta_ref = Uml::AssociationRole::Create(ass);
		OperationRef::meta_ref.name() = "ref";
		OperationRef::meta_ref.min() = 0;
		OperationRef::meta_ref.max() = 1;
		OperationRef::meta_ref.isNavigable() = true;
		OperationRef::meta_ref.isPrimary() = false;
		OperationRef::meta_ref.target() = OperationBase::meta;
		OperationBase::meta_referedbyOperationRef = Uml::AssociationRole::Create(ass);
		OperationBase::meta_referedbyOperationRef.name() = "referedbyOperationRef";
		OperationBase::meta_referedbyOperationRef.min() = 0;
		OperationBase::meta_referedbyOperationRef.max() = -1;
		OperationBase::meta_referedbyOperationRef.isNavigable() = true;
		OperationBase::meta_referedbyOperationRef.isPrimary() = false;
		OperationBase::meta_referedbyOperationRef.target() = OperationRef::meta;
	};
	void InitAssociation11()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		EventRef::meta_ref = Uml::AssociationRole::Create(ass);
		EventRef::meta_ref.name() = "ref";
		EventRef::meta_ref.min() = 0;
		EventRef::meta_ref.max() = 1;
		EventRef::meta_ref.isNavigable() = true;
		EventRef::meta_ref.isPrimary() = false;
		EventRef::meta_ref.target() = Event::meta;
		Event::meta_referedbyEventRef = Uml::AssociationRole::Create(ass);
		Event::meta_referedbyEventRef.name() = "referedbyEventRef";
		Event::meta_referedbyEventRef.min() = 0;
		Event::meta_referedbyEventRef.max() = -1;
		Event::meta_referedbyEventRef.isNavigable() = true;
		Event::meta_referedbyEventRef.isPrimary() = false;
		Event::meta_referedbyEventRef.target() = EventRef::meta;
	};
	void InitAssociation12()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		TaskSet::meta_members = Uml::AssociationRole::Create(ass);
		TaskSet::meta_members.name() = "members";
		TaskSet::meta_members.min() = 0;
		TaskSet::meta_members.max() = -1;
		TaskSet::meta_members.isNavigable() = true;
		TaskSet::meta_members.isPrimary() = false;
		TaskSet::meta_members.target() = Task::meta;
		Task::meta_setTaskSet = Uml::AssociationRole::Create(ass);
		Task::meta_setTaskSet.name() = "setTaskSet";
		Task::meta_setTaskSet.min() = 0;
		Task::meta_setTaskSet.max() = -1;
		Task::meta_setTaskSet.isNavigable() = true;
		Task::meta_setTaskSet.isPrimary() = false;
		Task::meta_setTaskSet.target() = TaskSet::meta;
	};
	void InitAssociation13()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		ass.name() = "InstanceMapping";
		ass.assocClass() = InstanceMapping::meta;
		NodeReference::meta_srcInstanceMapping = Uml::AssociationRole::Create(ass);
		NodeReference::meta_srcInstanceMapping.name() = "srcInstanceMapping";
		NodeReference::meta_srcInstanceMapping.min() = 0;
		NodeReference::meta_srcInstanceMapping.max() = -1;
		NodeReference::meta_srcInstanceMapping.isNavigable() = true;
		NodeReference::meta_srcInstanceMapping.isPrimary() = false;
		NodeReference::meta_srcInstanceMapping.target() = CollocationGroup::meta;
		InstanceMapping::meta_srcInstanceMapping_end_ = CollocationGroup::meta_dstInstanceMapping_rev = NodeReference::meta_srcInstanceMapping;
		CollocationGroup::meta_dstInstanceMapping = Uml::AssociationRole::Create(ass);
		CollocationGroup::meta_dstInstanceMapping.name() = "dstInstanceMapping";
		CollocationGroup::meta_dstInstanceMapping.min() = 0;
		CollocationGroup::meta_dstInstanceMapping.max() = -1;
		CollocationGroup::meta_dstInstanceMapping.isNavigable() = true;
		CollocationGroup::meta_dstInstanceMapping.isPrimary() = false;
		CollocationGroup::meta_dstInstanceMapping.target() = NodeReference::meta;
		InstanceMapping::meta_dstInstanceMapping_end_ = NodeReference::meta_srcInstanceMapping_rev = CollocationGroup::meta_dstInstanceMapping;
	};
	void InitAssociation14()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		NodeReference::meta_ref = Uml::AssociationRole::Create(ass);
		NodeReference::meta_ref.name() = "ref";
		NodeReference::meta_ref.min() = 0;
		NodeReference::meta_ref.max() = 1;
		NodeReference::meta_ref.isNavigable() = true;
		NodeReference::meta_ref.isPrimary() = false;
		NodeReference::meta_ref.target() = Node::meta;
		Node::meta_referedbyNodeReference = Uml::AssociationRole::Create(ass);
		Node::meta_referedbyNodeReference.name() = "referedbyNodeReference";
		Node::meta_referedbyNodeReference.min() = 0;
		Node::meta_referedbyNodeReference.max() = -1;
		Node::meta_referedbyNodeReference.isNavigable() = true;
		Node::meta_referedbyNodeReference.isPrimary() = false;
		Node::meta_referedbyNodeReference.target() = NodeReference::meta;
	};
	void InitAssociation15()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		ass.name() = "Interconnect2Node";
		ass.assocClass() = Interconnect2Node::meta;
		Interconnect::meta_dstInterconnect2Node = Uml::AssociationRole::Create(ass);
		Interconnect::meta_dstInterconnect2Node.name() = "dstInterconnect2Node";
		Interconnect::meta_dstInterconnect2Node.min() = 1;
		Interconnect::meta_dstInterconnect2Node.max() = -1;
		Interconnect::meta_dstInterconnect2Node.isNavigable() = true;
		Interconnect::meta_dstInterconnect2Node.isPrimary() = false;
		Interconnect::meta_dstInterconnect2Node.target() = Node::meta;
		Interconnect2Node::meta_dstInterconnect2Node_end_ = Node::meta_srcInterconnect2Node_rev = Interconnect::meta_dstInterconnect2Node;
		Node::meta_srcInterconnect2Node = Uml::AssociationRole::Create(ass);
		Node::meta_srcInterconnect2Node.name() = "srcInterconnect2Node";
		Node::meta_srcInterconnect2Node.min() = 0;
		Node::meta_srcInterconnect2Node.max() = -1;
		Node::meta_srcInterconnect2Node.isNavigable() = true;
		Node::meta_srcInterconnect2Node.isPrimary() = false;
		Node::meta_srcInterconnect2Node.target() = Interconnect::meta;
		Interconnect2Node::meta_srcInterconnect2Node_end_ = Interconnect::meta_dstInterconnect2Node_rev = Node::meta_srcInterconnect2Node;
	};
	void InitAssociation16()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		ass.name() = "Interconnect2Bridge";
		ass.assocClass() = Interconnect2Bridge::meta;
		Interconnect::meta_dstInterconnect2Bridge = Uml::AssociationRole::Create(ass);
		Interconnect::meta_dstInterconnect2Bridge.name() = "dstInterconnect2Bridge";
		Interconnect::meta_dstInterconnect2Bridge.min() = 0;
		Interconnect::meta_dstInterconnect2Bridge.max() = -1;
		Interconnect::meta_dstInterconnect2Bridge.isNavigable() = true;
		Interconnect::meta_dstInterconnect2Bridge.isPrimary() = false;
		Interconnect::meta_dstInterconnect2Bridge.target() = Bridge::meta;
		Interconnect2Bridge::meta_dstInterconnect2Bridge_end_ = Bridge::meta_srcInterconnect2Bridge_rev = Interconnect::meta_dstInterconnect2Bridge;
		Bridge::meta_srcInterconnect2Bridge = Uml::AssociationRole::Create(ass);
		Bridge::meta_srcInterconnect2Bridge.name() = "srcInterconnect2Bridge";
		Bridge::meta_srcInterconnect2Bridge.min() = 1;
		Bridge::meta_srcInterconnect2Bridge.max() = -1;
		Bridge::meta_srcInterconnect2Bridge.isNavigable() = true;
		Bridge::meta_srcInterconnect2Bridge.isPrimary() = false;
		Bridge::meta_srcInterconnect2Bridge.target() = Interconnect::meta;
		Interconnect2Bridge::meta_srcInterconnect2Bridge_end_ = Interconnect::meta_dstInterconnect2Bridge_rev = Bridge::meta_srcInterconnect2Bridge;
	};
	void InitAssociation17()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		ass.name() = "Node2Interconnect";
		ass.assocClass() = Node2Interconnect::meta;
		Interconnect::meta_srcNode2Interconnect = Uml::AssociationRole::Create(ass);
		Interconnect::meta_srcNode2Interconnect.name() = "srcNode2Interconnect";
		Interconnect::meta_srcNode2Interconnect.min() = 1;
		Interconnect::meta_srcNode2Interconnect.max() = -1;
		Interconnect::meta_srcNode2Interconnect.isNavigable() = true;
		Interconnect::meta_srcNode2Interconnect.isPrimary() = false;
		Interconnect::meta_srcNode2Interconnect.target() = Node::meta;
		Node2Interconnect::meta_srcNode2Interconnect_end_ = Node::meta_dstNode2Interconnect_rev = Interconnect::meta_srcNode2Interconnect;
		Node::meta_dstNode2Interconnect = Uml::AssociationRole::Create(ass);
		Node::meta_dstNode2Interconnect.name() = "dstNode2Interconnect";
		Node::meta_dstNode2Interconnect.min() = 0;
		Node::meta_dstNode2Interconnect.max() = -1;
		Node::meta_dstNode2Interconnect.isNavigable() = true;
		Node::meta_dstNode2Interconnect.isPrimary() = false;
		Node::meta_dstNode2Interconnect.target() = Interconnect::meta;
		Node2Interconnect::meta_dstNode2Interconnect_end_ = Interconnect::meta_srcNode2Interconnect_rev = Node::meta_dstNode2Interconnect;
	};
	void InitAssociation18()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		ass.name() = "Bridge2Interconnect";
		ass.assocClass() = Bridge2Interconnect::meta;
		Interconnect::meta_srcBridge2Interconnect = Uml::AssociationRole::Create(ass);
		Interconnect::meta_srcBridge2Interconnect.name() = "srcBridge2Interconnect";
		Interconnect::meta_srcBridge2Interconnect.min() = 0;
		Interconnect::meta_srcBridge2Interconnect.max() = -1;
		Interconnect::meta_srcBridge2Interconnect.isNavigable() = true;
		Interconnect::meta_srcBridge2Interconnect.isPrimary() = false;
		Interconnect::meta_srcBridge2Interconnect.target() = Bridge::meta;
		Bridge2Interconnect::meta_srcBridge2Interconnect_end_ = Bridge::meta_dstBridge2Interconnect_rev = Interconnect::meta_srcBridge2Interconnect;
		Bridge::meta_dstBridge2Interconnect = Uml::AssociationRole::Create(ass);
		Bridge::meta_dstBridge2Interconnect.name() = "dstBridge2Interconnect";
		Bridge::meta_dstBridge2Interconnect.min() = 1;
		Bridge::meta_dstBridge2Interconnect.max() = -1;
		Bridge::meta_dstBridge2Interconnect.isNavigable() = true;
		Bridge::meta_dstBridge2Interconnect.isPrimary() = false;
		Bridge::meta_dstBridge2Interconnect.target() = Interconnect::meta;
		Bridge2Interconnect::meta_dstBridge2Interconnect_end_ = Interconnect::meta_srcBridge2Interconnect_rev = Bridge::meta_dstBridge2Interconnect;
	};
	void InitAssociation19()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		ass.name() = "Shares";
		ass.assocClass() = Shares::meta;
		Node::meta_dstShares = Uml::AssociationRole::Create(ass);
		Node::meta_dstShares.name() = "dstShares";
		Node::meta_dstShares.min() = 0;
		Node::meta_dstShares.max() = -1;
		Node::meta_dstShares.isNavigable() = true;
		Node::meta_dstShares.isPrimary() = false;
		Node::meta_dstShares.target() = SharedResource::meta;
		Shares::meta_dstShares_end_ = SharedResource::meta_srcShares_rev = Node::meta_dstShares;
		SharedResource::meta_srcShares = Uml::AssociationRole::Create(ass);
		SharedResource::meta_srcShares.name() = "srcShares";
		SharedResource::meta_srcShares.min() = 1;
		SharedResource::meta_srcShares.max() = -1;
		SharedResource::meta_srcShares.isNavigable() = true;
		SharedResource::meta_srcShares.isPrimary() = false;
		SharedResource::meta_srcShares.target() = Node::meta;
		Shares::meta_srcShares_end_ = Node::meta_dstShares_rev = SharedResource::meta_srcShares;
	};
	void InitAssociation20()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		CollocationGroup::meta_members = Uml::AssociationRole::Create(ass);
		CollocationGroup::meta_members.name() = "members";
		CollocationGroup::meta_members.min() = 0;
		CollocationGroup::meta_members.max() = -1;
		CollocationGroup::meta_members.isNavigable() = true;
		CollocationGroup::meta_members.isPrimary() = false;
		CollocationGroup::meta_members.target() = ComponentRef::meta;
		ComponentRef::meta_setCollocationGroup = Uml::AssociationRole::Create(ass);
		ComponentRef::meta_setCollocationGroup.name() = "setCollocationGroup";
		ComponentRef::meta_setCollocationGroup.min() = 0;
		ComponentRef::meta_setCollocationGroup.max() = -1;
		ComponentRef::meta_setCollocationGroup.isNavigable() = true;
		ComponentRef::meta_setCollocationGroup.isPrimary() = false;
		ComponentRef::meta_setCollocationGroup.target() = CollocationGroup::meta;
	};
	void InitAssociation21()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		ass.name() = "invoke";
		ass.assocClass() = invoke::meta;
		ProvidedRequestPort::meta_srcinvoke = Uml::AssociationRole::Create(ass);
		ProvidedRequestPort::meta_srcinvoke.name() = "srcinvoke";
		ProvidedRequestPort::meta_srcinvoke.min() = 0;
		ProvidedRequestPort::meta_srcinvoke.max() = -1;
		ProvidedRequestPort::meta_srcinvoke.isNavigable() = true;
		ProvidedRequestPort::meta_srcinvoke.isPrimary() = false;
		ProvidedRequestPort::meta_srcinvoke.target() = RequiredRequestPort::meta;
		invoke::meta_srcinvoke_end_ = RequiredRequestPort::meta_dstinvoke_rev = ProvidedRequestPort::meta_srcinvoke;
		RequiredRequestPort::meta_dstinvoke = Uml::AssociationRole::Create(ass);
		RequiredRequestPort::meta_dstinvoke.name() = "dstinvoke";
		RequiredRequestPort::meta_dstinvoke.min() = 0;
		RequiredRequestPort::meta_dstinvoke.max() = -1;
		RequiredRequestPort::meta_dstinvoke.isNavigable() = true;
		RequiredRequestPort::meta_dstinvoke.isPrimary() = false;
		RequiredRequestPort::meta_dstinvoke.target() = ProvidedRequestPort::meta;
		invoke::meta_dstinvoke_end_ = ProvidedRequestPort::meta_srcinvoke_rev = RequiredRequestPort::meta_dstinvoke;
	};
	void InitAssociation22()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		ass.name() = "AssemblyselectRequirement";
		ass.assocClass() = AssemblyselectRequirement::meta;
		Requirement::meta_srcAssemblyselectRequirement = Uml::AssociationRole::Create(ass);
		Requirement::meta_srcAssemblyselectRequirement.name() = "srcAssemblyselectRequirement";
		Requirement::meta_srcAssemblyselectRequirement.min() = 0;
		Requirement::meta_srcAssemblyselectRequirement.max() = -1;
		Requirement::meta_srcAssemblyselectRequirement.isNavigable() = true;
		Requirement::meta_srcAssemblyselectRequirement.isPrimary() = false;
		Requirement::meta_srcAssemblyselectRequirement.target() = Component::meta;
		AssemblyselectRequirement::meta_srcAssemblyselectRequirement_end_ = Component::meta_dstAssemblyselectRequirement_rev = Requirement::meta_srcAssemblyselectRequirement;
		Component::meta_dstAssemblyselectRequirement = Uml::AssociationRole::Create(ass);
		Component::meta_dstAssemblyselectRequirement.name() = "dstAssemblyselectRequirement";
		Component::meta_dstAssemblyselectRequirement.min() = 0;
		Component::meta_dstAssemblyselectRequirement.max() = -1;
		Component::meta_dstAssemblyselectRequirement.isNavigable() = true;
		Component::meta_dstAssemblyselectRequirement.isPrimary() = false;
		Component::meta_dstAssemblyselectRequirement.target() = Requirement::meta;
		AssemblyselectRequirement::meta_dstAssemblyselectRequirement_end_ = Requirement::meta_srcAssemblyselectRequirement_rev = Component::meta_dstAssemblyselectRequirement;
	};
	void InitAssociation23()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		ass.name() = "AssemblyConfigProperty";
		ass.assocClass() = AssemblyConfigProperty::meta;
		Property::meta_srcAssemblyConfigProperty = Uml::AssociationRole::Create(ass);
		Property::meta_srcAssemblyConfigProperty.name() = "srcAssemblyConfigProperty";
		Property::meta_srcAssemblyConfigProperty.min() = 0;
		Property::meta_srcAssemblyConfigProperty.max() = -1;
		Property::meta_srcAssemblyConfigProperty.isNavigable() = true;
		Property::meta_srcAssemblyConfigProperty.isPrimary() = false;
		Property::meta_srcAssemblyConfigProperty.target() = Component::meta;
		AssemblyConfigProperty::meta_srcAssemblyConfigProperty_end_ = Component::meta_dstAssemblyConfigProperty_rev = Property::meta_srcAssemblyConfigProperty;
		Component::meta_dstAssemblyConfigProperty = Uml::AssociationRole::Create(ass);
		Component::meta_dstAssemblyConfigProperty.name() = "dstAssemblyConfigProperty";
		Component::meta_dstAssemblyConfigProperty.min() = 0;
		Component::meta_dstAssemblyConfigProperty.max() = -1;
		Component::meta_dstAssemblyConfigProperty.isNavigable() = true;
		Component::meta_dstAssemblyConfigProperty.isPrimary() = false;
		Component::meta_dstAssemblyConfigProperty.target() = Property::meta;
		AssemblyConfigProperty::meta_dstAssemblyConfigProperty_end_ = Property::meta_srcAssemblyConfigProperty_rev = Component::meta_dstAssemblyConfigProperty;
	};
	void InitAssociation24()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		ass.name() = "AssemblyDeployRequirement";
		ass.assocClass() = AssemblyDeployRequirement::meta;
		PublishConnector::meta_dstAssemblyDeployRequirement = Uml::AssociationRole::Create(ass);
		PublishConnector::meta_dstAssemblyDeployRequirement.name() = "dstAssemblyDeployRequirement";
		PublishConnector::meta_dstAssemblyDeployRequirement.min() = 0;
		PublishConnector::meta_dstAssemblyDeployRequirement.max() = -1;
		PublishConnector::meta_dstAssemblyDeployRequirement.isNavigable() = true;
		PublishConnector::meta_dstAssemblyDeployRequirement.isPrimary() = false;
		PublishConnector::meta_dstAssemblyDeployRequirement.target() = Requirement::meta;
		AssemblyDeployRequirement::meta_dstAssemblyDeployRequirement_end_ = Requirement::meta_srcAssemblyDeployRequirement_rev = PublishConnector::meta_dstAssemblyDeployRequirement;
		Requirement::meta_srcAssemblyDeployRequirement = Uml::AssociationRole::Create(ass);
		Requirement::meta_srcAssemblyDeployRequirement.name() = "srcAssemblyDeployRequirement";
		Requirement::meta_srcAssemblyDeployRequirement.min() = 0;
		Requirement::meta_srcAssemblyDeployRequirement.max() = -1;
		Requirement::meta_srcAssemblyDeployRequirement.isNavigable() = true;
		Requirement::meta_srcAssemblyDeployRequirement.isPrimary() = false;
		Requirement::meta_srcAssemblyDeployRequirement.target() = PublishConnector::meta;
		AssemblyDeployRequirement::meta_srcAssemblyDeployRequirement_end_ = PublishConnector::meta_dstAssemblyDeployRequirement_rev = Requirement::meta_srcAssemblyDeployRequirement;
	};
	void InitAssociation25()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		ass.name() = "deliverTo";
		ass.assocClass() = deliverTo::meta;
		PublishConnector::meta_dstdeliverTo = Uml::AssociationRole::Create(ass);
		PublishConnector::meta_dstdeliverTo.name() = "dstdeliverTo";
		PublishConnector::meta_dstdeliverTo.min() = 0;
		PublishConnector::meta_dstdeliverTo.max() = -1;
		PublishConnector::meta_dstdeliverTo.isNavigable() = true;
		PublishConnector::meta_dstdeliverTo.isPrimary() = false;
		PublishConnector::meta_dstdeliverTo.target() = InEventPort::meta;
		deliverTo::meta_dstdeliverTo_end_ = InEventPort::meta_srcdeliverTo_rev = PublishConnector::meta_dstdeliverTo;
		InEventPort::meta_srcdeliverTo = Uml::AssociationRole::Create(ass);
		InEventPort::meta_srcdeliverTo.name() = "srcdeliverTo";
		InEventPort::meta_srcdeliverTo.min() = 0;
		InEventPort::meta_srcdeliverTo.max() = 1;
		InEventPort::meta_srcdeliverTo.isNavigable() = true;
		InEventPort::meta_srcdeliverTo.isPrimary() = false;
		InEventPort::meta_srcdeliverTo.target() = PublishConnector::meta;
		deliverTo::meta_srcdeliverTo_end_ = PublishConnector::meta_dstdeliverTo_rev = InEventPort::meta_srcdeliverTo;
	};
	void InitAssociation26()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		ass.name() = "publish";
		ass.assocClass() = publish::meta;
		PublishConnector::meta_srcpublish = Uml::AssociationRole::Create(ass);
		PublishConnector::meta_srcpublish.name() = "srcpublish";
		PublishConnector::meta_srcpublish.min() = 0;
		PublishConnector::meta_srcpublish.max() = 1;
		PublishConnector::meta_srcpublish.isNavigable() = true;
		PublishConnector::meta_srcpublish.isPrimary() = false;
		PublishConnector::meta_srcpublish.target() = OutEventPort::meta;
		publish::meta_srcpublish_end_ = OutEventPort::meta_dstpublish_rev = PublishConnector::meta_srcpublish;
		OutEventPort::meta_dstpublish = Uml::AssociationRole::Create(ass);
		OutEventPort::meta_dstpublish.name() = "dstpublish";
		OutEventPort::meta_dstpublish.min() = 0;
		OutEventPort::meta_dstpublish.max() = 1;
		OutEventPort::meta_dstpublish.isNavigable() = true;
		OutEventPort::meta_dstpublish.isPrimary() = false;
		OutEventPort::meta_dstpublish.target() = PublishConnector::meta;
		publish::meta_dstpublish_end_ = PublishConnector::meta_srcpublish_rev = OutEventPort::meta_dstpublish;
	};
	void InitAssociation27()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		ass.name() = "emit";
		ass.assocClass() = emit::meta;
		InEventPort::meta_srcemit = Uml::AssociationRole::Create(ass);
		InEventPort::meta_srcemit.name() = "srcemit";
		InEventPort::meta_srcemit.min() = 0;
		InEventPort::meta_srcemit.max() = 1;
		InEventPort::meta_srcemit.isNavigable() = true;
		InEventPort::meta_srcemit.isPrimary() = false;
		InEventPort::meta_srcemit.target() = OutEventPort::meta;
		emit::meta_srcemit_end_ = OutEventPort::meta_dstemit_rev = InEventPort::meta_srcemit;
		OutEventPort::meta_dstemit = Uml::AssociationRole::Create(ass);
		OutEventPort::meta_dstemit.name() = "dstemit";
		OutEventPort::meta_dstemit.min() = 0;
		OutEventPort::meta_dstemit.max() = 1;
		OutEventPort::meta_dstemit.isNavigable() = true;
		OutEventPort::meta_dstemit.isPrimary() = false;
		OutEventPort::meta_dstemit.target() = InEventPort::meta;
		emit::meta_dstemit_end_ = InEventPort::meta_srcemit_rev = OutEventPort::meta_dstemit;
	};
	void InitAssociation28()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		ass.name() = "ReceptacleDelegate";
		ass.assocClass() = ReceptacleDelegate::meta;
		RequiredRequestPort::meta_srcReceptacleDelegate = Uml::AssociationRole::Create(ass);
		RequiredRequestPort::meta_srcReceptacleDelegate.name() = "srcReceptacleDelegate";
		RequiredRequestPort::meta_srcReceptacleDelegate.min() = 0;
		RequiredRequestPort::meta_srcReceptacleDelegate.max() = -1;
		RequiredRequestPort::meta_srcReceptacleDelegate.isNavigable() = true;
		RequiredRequestPort::meta_srcReceptacleDelegate.isPrimary() = false;
		RequiredRequestPort::meta_srcReceptacleDelegate.target() = RequiredRequestPort::meta;
		ReceptacleDelegate::meta_srcReceptacleDelegate_end_ = RequiredRequestPort::meta_dstReceptacleDelegate_rev = RequiredRequestPort::meta_srcReceptacleDelegate;
		RequiredRequestPort::meta_dstReceptacleDelegate = Uml::AssociationRole::Create(ass);
		RequiredRequestPort::meta_dstReceptacleDelegate.name() = "dstReceptacleDelegate";
		RequiredRequestPort::meta_dstReceptacleDelegate.min() = 0;
		RequiredRequestPort::meta_dstReceptacleDelegate.max() = -1;
		RequiredRequestPort::meta_dstReceptacleDelegate.isNavigable() = true;
		RequiredRequestPort::meta_dstReceptacleDelegate.isPrimary() = false;
		RequiredRequestPort::meta_dstReceptacleDelegate.target() = RequiredRequestPort::meta;
		ReceptacleDelegate::meta_dstReceptacleDelegate_end_ = RequiredRequestPort::meta_srcReceptacleDelegate_rev = RequiredRequestPort::meta_dstReceptacleDelegate;
	};
	void InitAssociation29()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		ass.name() = "FacetDelegate";
		ass.assocClass() = FacetDelegate::meta;
		ProvidedRequestPort::meta_srcFacetDelegate = Uml::AssociationRole::Create(ass);
		ProvidedRequestPort::meta_srcFacetDelegate.name() = "srcFacetDelegate";
		ProvidedRequestPort::meta_srcFacetDelegate.min() = 0;
		ProvidedRequestPort::meta_srcFacetDelegate.max() = -1;
		ProvidedRequestPort::meta_srcFacetDelegate.isNavigable() = true;
		ProvidedRequestPort::meta_srcFacetDelegate.isPrimary() = false;
		ProvidedRequestPort::meta_srcFacetDelegate.target() = ProvidedRequestPort::meta;
		FacetDelegate::meta_srcFacetDelegate_end_ = ProvidedRequestPort::meta_dstFacetDelegate_rev = ProvidedRequestPort::meta_srcFacetDelegate;
		ProvidedRequestPort::meta_dstFacetDelegate = Uml::AssociationRole::Create(ass);
		ProvidedRequestPort::meta_dstFacetDelegate.name() = "dstFacetDelegate";
		ProvidedRequestPort::meta_dstFacetDelegate.min() = 0;
		ProvidedRequestPort::meta_dstFacetDelegate.max() = -1;
		ProvidedRequestPort::meta_dstFacetDelegate.isNavigable() = true;
		ProvidedRequestPort::meta_dstFacetDelegate.isPrimary() = false;
		ProvidedRequestPort::meta_dstFacetDelegate.target() = ProvidedRequestPort::meta;
		FacetDelegate::meta_dstFacetDelegate_end_ = ProvidedRequestPort::meta_srcFacetDelegate_rev = ProvidedRequestPort::meta_dstFacetDelegate;
	};
	void InitAssociation30()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		ass.name() = "EventSourceDelegate";
		ass.assocClass() = EventSourceDelegate::meta;
		OutEventPort::meta_srcEventSourceDelegate = Uml::AssociationRole::Create(ass);
		OutEventPort::meta_srcEventSourceDelegate.name() = "srcEventSourceDelegate";
		OutEventPort::meta_srcEventSourceDelegate.min() = 0;
		OutEventPort::meta_srcEventSourceDelegate.max() = -1;
		OutEventPort::meta_srcEventSourceDelegate.isNavigable() = true;
		OutEventPort::meta_srcEventSourceDelegate.isPrimary() = false;
		OutEventPort::meta_srcEventSourceDelegate.target() = OutEventPort::meta;
		EventSourceDelegate::meta_srcEventSourceDelegate_end_ = OutEventPort::meta_dstEventSourceDelegate_rev = OutEventPort::meta_srcEventSourceDelegate;
		OutEventPort::meta_dstEventSourceDelegate = Uml::AssociationRole::Create(ass);
		OutEventPort::meta_dstEventSourceDelegate.name() = "dstEventSourceDelegate";
		OutEventPort::meta_dstEventSourceDelegate.min() = 0;
		OutEventPort::meta_dstEventSourceDelegate.max() = -1;
		OutEventPort::meta_dstEventSourceDelegate.isNavigable() = true;
		OutEventPort::meta_dstEventSourceDelegate.isPrimary() = false;
		OutEventPort::meta_dstEventSourceDelegate.target() = OutEventPort::meta;
		EventSourceDelegate::meta_dstEventSourceDelegate_end_ = OutEventPort::meta_srcEventSourceDelegate_rev = OutEventPort::meta_dstEventSourceDelegate;
	};
	void InitAssociation31()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		ass.name() = "EventSinkDelegate";
		ass.assocClass() = EventSinkDelegate::meta;
		InEventPort::meta_srcEventSinkDelegate = Uml::AssociationRole::Create(ass);
		InEventPort::meta_srcEventSinkDelegate.name() = "srcEventSinkDelegate";
		InEventPort::meta_srcEventSinkDelegate.min() = 0;
		InEventPort::meta_srcEventSinkDelegate.max() = -1;
		InEventPort::meta_srcEventSinkDelegate.isNavigable() = true;
		InEventPort::meta_srcEventSinkDelegate.isPrimary() = false;
		InEventPort::meta_srcEventSinkDelegate.target() = InEventPort::meta;
		EventSinkDelegate::meta_srcEventSinkDelegate_end_ = InEventPort::meta_dstEventSinkDelegate_rev = InEventPort::meta_srcEventSinkDelegate;
		InEventPort::meta_dstEventSinkDelegate = Uml::AssociationRole::Create(ass);
		InEventPort::meta_dstEventSinkDelegate.name() = "dstEventSinkDelegate";
		InEventPort::meta_dstEventSinkDelegate.min() = 0;
		InEventPort::meta_dstEventSinkDelegate.max() = -1;
		InEventPort::meta_dstEventSinkDelegate.isNavigable() = true;
		InEventPort::meta_dstEventSinkDelegate.isPrimary() = false;
		InEventPort::meta_dstEventSinkDelegate.target() = InEventPort::meta;
		EventSinkDelegate::meta_dstEventSinkDelegate_end_ = InEventPort::meta_srcEventSinkDelegate_rev = InEventPort::meta_dstEventSinkDelegate;
	};
	void InitAssociation32()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		ass.name() = "ExternalDelegate";
		ass.assocClass() = ExternalDelegate::meta;
		ExternalReferenceEndPoint::meta_srcExternalDelegate = Uml::AssociationRole::Create(ass);
		ExternalReferenceEndPoint::meta_srcExternalDelegate.name() = "srcExternalDelegate";
		ExternalReferenceEndPoint::meta_srcExternalDelegate.min() = 0;
		ExternalReferenceEndPoint::meta_srcExternalDelegate.max() = -1;
		ExternalReferenceEndPoint::meta_srcExternalDelegate.isNavigable() = true;
		ExternalReferenceEndPoint::meta_srcExternalDelegate.isPrimary() = false;
		ExternalReferenceEndPoint::meta_srcExternalDelegate.target() = Component::meta;
		ExternalDelegate::meta_srcExternalDelegate_end_ = Component::meta_dstExternalDelegate_rev = ExternalReferenceEndPoint::meta_srcExternalDelegate;
		Component::meta_dstExternalDelegate = Uml::AssociationRole::Create(ass);
		Component::meta_dstExternalDelegate.name() = "dstExternalDelegate";
		Component::meta_dstExternalDelegate.min() = 0;
		Component::meta_dstExternalDelegate.max() = -1;
		Component::meta_dstExternalDelegate.isNavigable() = true;
		Component::meta_dstExternalDelegate.isPrimary() = false;
		Component::meta_dstExternalDelegate.target() = ExternalReferenceEndPoint::meta;
		ExternalDelegate::meta_dstExternalDelegate_end_ = ExternalReferenceEndPoint::meta_srcExternalDelegate_rev = Component::meta_dstExternalDelegate;
	};
	void InitAssociation33()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		ass.name() = "ImplementationDependsOn";
		ass.assocClass() = ImplementationDependsOn::meta;
		ComponentImplementation::meta_dstImplementationDependsOn = Uml::AssociationRole::Create(ass);
		ComponentImplementation::meta_dstImplementationDependsOn.name() = "dstImplementationDependsOn";
		ComponentImplementation::meta_dstImplementationDependsOn.min() = 0;
		ComponentImplementation::meta_dstImplementationDependsOn.max() = -1;
		ComponentImplementation::meta_dstImplementationDependsOn.isNavigable() = true;
		ComponentImplementation::meta_dstImplementationDependsOn.isPrimary() = false;
		ComponentImplementation::meta_dstImplementationDependsOn.target() = ImplementationDependency::meta;
		ImplementationDependsOn::meta_dstImplementationDependsOn_end_ = ImplementationDependency::meta_srcImplementationDependsOn_rev = ComponentImplementation::meta_dstImplementationDependsOn;
		ImplementationDependency::meta_srcImplementationDependsOn = Uml::AssociationRole::Create(ass);
		ImplementationDependency::meta_srcImplementationDependsOn.name() = "srcImplementationDependsOn";
		ImplementationDependency::meta_srcImplementationDependsOn.min() = 0;
		ImplementationDependency::meta_srcImplementationDependsOn.max() = -1;
		ImplementationDependency::meta_srcImplementationDependsOn.isNavigable() = true;
		ImplementationDependency::meta_srcImplementationDependsOn.isPrimary() = false;
		ImplementationDependency::meta_srcImplementationDependsOn.target() = ComponentImplementation::meta;
		ImplementationDependsOn::meta_srcImplementationDependsOn_end_ = ComponentImplementation::meta_dstImplementationDependsOn_rev = ImplementationDependency::meta_srcImplementationDependsOn;
	};
	void InitAssociation34()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		ass.name() = "ImplementationCapability";
		ass.assocClass() = ImplementationCapability::meta;
		ComponentImplementation::meta_dstImplementationCapability = Uml::AssociationRole::Create(ass);
		ComponentImplementation::meta_dstImplementationCapability.name() = "dstImplementationCapability";
		ComponentImplementation::meta_dstImplementationCapability.min() = 0;
		ComponentImplementation::meta_dstImplementationCapability.max() = -1;
		ComponentImplementation::meta_dstImplementationCapability.isNavigable() = true;
		ComponentImplementation::meta_dstImplementationCapability.isPrimary() = false;
		ComponentImplementation::meta_dstImplementationCapability.target() = Capability::meta;
		ImplementationCapability::meta_dstImplementationCapability_end_ = Capability::meta_srcImplementationCapability_rev = ComponentImplementation::meta_dstImplementationCapability;
		Capability::meta_srcImplementationCapability = Uml::AssociationRole::Create(ass);
		Capability::meta_srcImplementationCapability.name() = "srcImplementationCapability";
		Capability::meta_srcImplementationCapability.min() = 0;
		Capability::meta_srcImplementationCapability.max() = -1;
		Capability::meta_srcImplementationCapability.isNavigable() = true;
		Capability::meta_srcImplementationCapability.isPrimary() = false;
		Capability::meta_srcImplementationCapability.target() = ComponentImplementation::meta;
		ImplementationCapability::meta_srcImplementationCapability_end_ = ComponentImplementation::meta_dstImplementationCapability_rev = Capability::meta_srcImplementationCapability;
	};
	void InitAssociation35()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		ass.name() = "Implements";
		ass.assocClass() = Implements::meta;
		ComponentImplementation::meta_dstImplements = Uml::AssociationRole::Create(ass);
		ComponentImplementation::meta_dstImplements.name() = "dstImplements";
		ComponentImplementation::meta_dstImplements.min() = 0;
		ComponentImplementation::meta_dstImplements.max() = -1;
		ComponentImplementation::meta_dstImplements.isNavigable() = true;
		ComponentImplementation::meta_dstImplements.isPrimary() = false;
		ComponentImplementation::meta_dstImplements.target() = ComponentRef::meta;
		Implements::meta_dstImplements_end_ = ComponentRef::meta_srcImplements_rev = ComponentImplementation::meta_dstImplements;
		ComponentRef::meta_srcImplements = Uml::AssociationRole::Create(ass);
		ComponentRef::meta_srcImplements.name() = "srcImplements";
		ComponentRef::meta_srcImplements.min() = 0;
		ComponentRef::meta_srcImplements.max() = -1;
		ComponentRef::meta_srcImplements.isNavigable() = true;
		ComponentRef::meta_srcImplements.isPrimary() = false;
		ComponentRef::meta_srcImplements.target() = ComponentImplementation::meta;
		Implements::meta_srcImplements_end_ = ComponentImplementation::meta_dstImplements_rev = ComponentRef::meta_srcImplements;
	};
	void InitAssociation36()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		ass.name() = "ConfigProperty";
		ass.assocClass() = ConfigProperty::meta;
		ComponentImplementation::meta_dstConfigProperty = Uml::AssociationRole::Create(ass);
		ComponentImplementation::meta_dstConfigProperty.name() = "dstConfigProperty";
		ComponentImplementation::meta_dstConfigProperty.min() = 0;
		ComponentImplementation::meta_dstConfigProperty.max() = -1;
		ComponentImplementation::meta_dstConfigProperty.isNavigable() = true;
		ComponentImplementation::meta_dstConfigProperty.isPrimary() = false;
		ComponentImplementation::meta_dstConfigProperty.target() = Property::meta;
		ConfigProperty::meta_dstConfigProperty_end_ = Property::meta_srcConfigProperty_rev = ComponentImplementation::meta_dstConfigProperty;
		Property::meta_srcConfigProperty = Uml::AssociationRole::Create(ass);
		Property::meta_srcConfigProperty.name() = "srcConfigProperty";
		Property::meta_srcConfigProperty.min() = 0;
		Property::meta_srcConfigProperty.max() = -1;
		Property::meta_srcConfigProperty.isNavigable() = true;
		Property::meta_srcConfigProperty.isPrimary() = false;
		Property::meta_srcConfigProperty.target() = ComponentImplementation::meta;
		ConfigProperty::meta_srcConfigProperty_end_ = ComponentImplementation::meta_dstConfigProperty_rev = Property::meta_srcConfigProperty;
	};
	void InitAssociation37()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		ass.name() = "InfoProperty";
		ass.assocClass() = InfoProperty::meta;
		ComponentImplementation::meta_dstInfoProperty = Uml::AssociationRole::Create(ass);
		ComponentImplementation::meta_dstInfoProperty.name() = "dstInfoProperty";
		ComponentImplementation::meta_dstInfoProperty.min() = 0;
		ComponentImplementation::meta_dstInfoProperty.max() = -1;
		ComponentImplementation::meta_dstInfoProperty.isNavigable() = true;
		ComponentImplementation::meta_dstInfoProperty.isPrimary() = false;
		ComponentImplementation::meta_dstInfoProperty.target() = Property::meta;
		InfoProperty::meta_dstInfoProperty_end_ = Property::meta_srcInfoProperty_rev = ComponentImplementation::meta_dstInfoProperty;
		Property::meta_srcInfoProperty = Uml::AssociationRole::Create(ass);
		Property::meta_srcInfoProperty.name() = "srcInfoProperty";
		Property::meta_srcInfoProperty.min() = 0;
		Property::meta_srcInfoProperty.max() = -1;
		Property::meta_srcInfoProperty.isNavigable() = true;
		Property::meta_srcInfoProperty.isPrimary() = false;
		Property::meta_srcInfoProperty.target() = ComponentImplementation::meta;
		InfoProperty::meta_srcInfoProperty_end_ = ComponentImplementation::meta_dstInfoProperty_rev = Property::meta_srcInfoProperty;
	};
	void InitAssociation38()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		ComponentImplementation::meta_referedbyComponentImplementationReference = Uml::AssociationRole::Create(ass);
		ComponentImplementation::meta_referedbyComponentImplementationReference.name() = "referedbyComponentImplementationReference";
		ComponentImplementation::meta_referedbyComponentImplementationReference.min() = 0;
		ComponentImplementation::meta_referedbyComponentImplementationReference.max() = -1;
		ComponentImplementation::meta_referedbyComponentImplementationReference.isNavigable() = true;
		ComponentImplementation::meta_referedbyComponentImplementationReference.isPrimary() = false;
		ComponentImplementation::meta_referedbyComponentImplementationReference.target() = ComponentImplementationReference::meta;
		ComponentImplementationReference::meta_ref = Uml::AssociationRole::Create(ass);
		ComponentImplementationReference::meta_ref.name() = "ref";
		ComponentImplementationReference::meta_ref.min() = 0;
		ComponentImplementationReference::meta_ref.max() = 1;
		ComponentImplementationReference::meta_ref.isNavigable() = true;
		ComponentImplementationReference::meta_ref.isPrimary() = false;
		ComponentImplementationReference::meta_ref.target() = ComponentImplementation::meta;
	};
	void InitAssociation39()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		ass.name() = "MonolithprimaryArtifact";
		ass.assocClass() = MonolithprimaryArtifact::meta;
		ImplementationArtifactReference::meta_srcMonolithprimaryArtifact = Uml::AssociationRole::Create(ass);
		ImplementationArtifactReference::meta_srcMonolithprimaryArtifact.name() = "srcMonolithprimaryArtifact";
		ImplementationArtifactReference::meta_srcMonolithprimaryArtifact.min() = 0;
		ImplementationArtifactReference::meta_srcMonolithprimaryArtifact.max() = -1;
		ImplementationArtifactReference::meta_srcMonolithprimaryArtifact.isNavigable() = true;
		ImplementationArtifactReference::meta_srcMonolithprimaryArtifact.isPrimary() = false;
		ImplementationArtifactReference::meta_srcMonolithprimaryArtifact.target() = MonolithicImplementation::meta;
		MonolithprimaryArtifact::meta_srcMonolithprimaryArtifact_end_ = MonolithicImplementation::meta_dstMonolithprimaryArtifact_rev = ImplementationArtifactReference::meta_srcMonolithprimaryArtifact;
		MonolithicImplementation::meta_dstMonolithprimaryArtifact = Uml::AssociationRole::Create(ass);
		MonolithicImplementation::meta_dstMonolithprimaryArtifact.name() = "dstMonolithprimaryArtifact";
		MonolithicImplementation::meta_dstMonolithprimaryArtifact.min() = 0;
		MonolithicImplementation::meta_dstMonolithprimaryArtifact.max() = -1;
		MonolithicImplementation::meta_dstMonolithprimaryArtifact.isNavigable() = true;
		MonolithicImplementation::meta_dstMonolithprimaryArtifact.isPrimary() = false;
		MonolithicImplementation::meta_dstMonolithprimaryArtifact.target() = ImplementationArtifactReference::meta;
		MonolithprimaryArtifact::meta_dstMonolithprimaryArtifact_end_ = ImplementationArtifactReference::meta_srcMonolithprimaryArtifact_rev = MonolithicImplementation::meta_dstMonolithprimaryArtifact;
	};
	void InitAssociation40()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		ass.name() = "MonolithDeployRequirement";
		ass.assocClass() = MonolithDeployRequirement::meta;
		ImplementationRequirement::meta_srcMonolithDeployRequirement = Uml::AssociationRole::Create(ass);
		ImplementationRequirement::meta_srcMonolithDeployRequirement.name() = "srcMonolithDeployRequirement";
		ImplementationRequirement::meta_srcMonolithDeployRequirement.min() = 0;
		ImplementationRequirement::meta_srcMonolithDeployRequirement.max() = -1;
		ImplementationRequirement::meta_srcMonolithDeployRequirement.isNavigable() = true;
		ImplementationRequirement::meta_srcMonolithDeployRequirement.isPrimary() = false;
		ImplementationRequirement::meta_srcMonolithDeployRequirement.target() = MonolithicImplementation::meta;
		MonolithDeployRequirement::meta_srcMonolithDeployRequirement_end_ = MonolithicImplementation::meta_dstMonolithDeployRequirement_rev = ImplementationRequirement::meta_srcMonolithDeployRequirement;
		MonolithicImplementation::meta_dstMonolithDeployRequirement = Uml::AssociationRole::Create(ass);
		MonolithicImplementation::meta_dstMonolithDeployRequirement.name() = "dstMonolithDeployRequirement";
		MonolithicImplementation::meta_dstMonolithDeployRequirement.min() = 0;
		MonolithicImplementation::meta_dstMonolithDeployRequirement.max() = -1;
		MonolithicImplementation::meta_dstMonolithDeployRequirement.isNavigable() = true;
		MonolithicImplementation::meta_dstMonolithDeployRequirement.isPrimary() = false;
		MonolithicImplementation::meta_dstMonolithDeployRequirement.target() = ImplementationRequirement::meta;
		MonolithDeployRequirement::meta_dstMonolithDeployRequirement_end_ = ImplementationRequirement::meta_srcMonolithDeployRequirement_rev = MonolithicImplementation::meta_dstMonolithDeployRequirement;
	};
	void InitAssociation41()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		ass.name() = "MonolithExecParameter";
		ass.assocClass() = MonolithExecParameter::meta;
		Property::meta_srcMonolithExecParameter = Uml::AssociationRole::Create(ass);
		Property::meta_srcMonolithExecParameter.name() = "srcMonolithExecParameter";
		Property::meta_srcMonolithExecParameter.min() = 0;
		Property::meta_srcMonolithExecParameter.max() = -1;
		Property::meta_srcMonolithExecParameter.isNavigable() = true;
		Property::meta_srcMonolithExecParameter.isPrimary() = false;
		Property::meta_srcMonolithExecParameter.target() = MonolithicImplementation::meta;
		MonolithExecParameter::meta_srcMonolithExecParameter_end_ = MonolithicImplementation::meta_dstMonolithExecParameter_rev = Property::meta_srcMonolithExecParameter;
		MonolithicImplementation::meta_dstMonolithExecParameter = Uml::AssociationRole::Create(ass);
		MonolithicImplementation::meta_dstMonolithExecParameter.name() = "dstMonolithExecParameter";
		MonolithicImplementation::meta_dstMonolithExecParameter.min() = 0;
		MonolithicImplementation::meta_dstMonolithExecParameter.max() = -1;
		MonolithicImplementation::meta_dstMonolithExecParameter.isNavigable() = true;
		MonolithicImplementation::meta_dstMonolithExecParameter.isPrimary() = false;
		MonolithicImplementation::meta_dstMonolithExecParameter.target() = Property::meta;
		MonolithExecParameter::meta_dstMonolithExecParameter_end_ = Property::meta_srcMonolithExecParameter_rev = MonolithicImplementation::meta_dstMonolithExecParameter;
	};
	void InitAssociation42()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		ass.name() = "Implementation";
		ass.assocClass() = Implementation::meta;
		ComponentImplementationReference::meta_srcImplementation = Uml::AssociationRole::Create(ass);
		ComponentImplementationReference::meta_srcImplementation.name() = "srcImplementation";
		ComponentImplementationReference::meta_srcImplementation.min() = 0;
		ComponentImplementationReference::meta_srcImplementation.max() = -1;
		ComponentImplementationReference::meta_srcImplementation.isNavigable() = true;
		ComponentImplementationReference::meta_srcImplementation.isPrimary() = false;
		ComponentImplementationReference::meta_srcImplementation.target() = ComponentPackage::meta;
		Implementation::meta_srcImplementation_end_ = ComponentPackage::meta_dstImplementation_rev = ComponentImplementationReference::meta_srcImplementation;
		ComponentPackage::meta_dstImplementation = Uml::AssociationRole::Create(ass);
		ComponentPackage::meta_dstImplementation.name() = "dstImplementation";
		ComponentPackage::meta_dstImplementation.min() = 0;
		ComponentPackage::meta_dstImplementation.max() = -1;
		ComponentPackage::meta_dstImplementation.isNavigable() = true;
		ComponentPackage::meta_dstImplementation.isPrimary() = false;
		ComponentPackage::meta_dstImplementation.target() = ComponentImplementationReference::meta;
		Implementation::meta_dstImplementation_end_ = ComponentImplementationReference::meta_srcImplementation_rev = ComponentPackage::meta_dstImplementation;
	};
	void InitAssociation43()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		ass.name() = "PackageConfBasePackage";
		ass.assocClass() = PackageConfBasePackage::meta;
		ComponentPackage::meta_srcPackageConfBasePackage = Uml::AssociationRole::Create(ass);
		ComponentPackage::meta_srcPackageConfBasePackage.name() = "srcPackageConfBasePackage";
		ComponentPackage::meta_srcPackageConfBasePackage.min() = 0;
		ComponentPackage::meta_srcPackageConfBasePackage.max() = -1;
		ComponentPackage::meta_srcPackageConfBasePackage.isNavigable() = true;
		ComponentPackage::meta_srcPackageConfBasePackage.isPrimary() = false;
		ComponentPackage::meta_srcPackageConfBasePackage.target() = PackageConfiguration::meta;
		PackageConfBasePackage::meta_srcPackageConfBasePackage_end_ = PackageConfiguration::meta_dstPackageConfBasePackage_rev = ComponentPackage::meta_srcPackageConfBasePackage;
		PackageConfiguration::meta_dstPackageConfBasePackage = Uml::AssociationRole::Create(ass);
		PackageConfiguration::meta_dstPackageConfBasePackage.name() = "dstPackageConfBasePackage";
		PackageConfiguration::meta_dstPackageConfBasePackage.min() = 0;
		PackageConfiguration::meta_dstPackageConfBasePackage.max() = 1;
		PackageConfiguration::meta_dstPackageConfBasePackage.isNavigable() = true;
		PackageConfiguration::meta_dstPackageConfBasePackage.isPrimary() = false;
		PackageConfiguration::meta_dstPackageConfBasePackage.target() = ComponentPackage::meta;
		PackageConfBasePackage::meta_dstPackageConfBasePackage_end_ = ComponentPackage::meta_srcPackageConfBasePackage_rev = PackageConfiguration::meta_dstPackageConfBasePackage;
	};
	void InitAssociation44()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		ass.name() = "PackageConfReference";
		ass.assocClass() = PackageConfReference::meta;
		ComponentPackageReference::meta_srcPackageConfReference = Uml::AssociationRole::Create(ass);
		ComponentPackageReference::meta_srcPackageConfReference.name() = "srcPackageConfReference";
		ComponentPackageReference::meta_srcPackageConfReference.min() = 0;
		ComponentPackageReference::meta_srcPackageConfReference.max() = -1;
		ComponentPackageReference::meta_srcPackageConfReference.isNavigable() = true;
		ComponentPackageReference::meta_srcPackageConfReference.isPrimary() = false;
		ComponentPackageReference::meta_srcPackageConfReference.target() = PackageConfiguration::meta;
		PackageConfReference::meta_srcPackageConfReference_end_ = PackageConfiguration::meta_dstPackageConfReference_rev = ComponentPackageReference::meta_srcPackageConfReference;
		PackageConfiguration::meta_dstPackageConfReference = Uml::AssociationRole::Create(ass);
		PackageConfiguration::meta_dstPackageConfReference.name() = "dstPackageConfReference";
		PackageConfiguration::meta_dstPackageConfReference.min() = 0;
		PackageConfiguration::meta_dstPackageConfReference.max() = 1;
		PackageConfiguration::meta_dstPackageConfReference.isNavigable() = true;
		PackageConfiguration::meta_dstPackageConfReference.isPrimary() = false;
		PackageConfiguration::meta_dstPackageConfReference.target() = ComponentPackageReference::meta;
		PackageConfReference::meta_dstPackageConfReference_end_ = ComponentPackageReference::meta_srcPackageConfReference_rev = PackageConfiguration::meta_dstPackageConfReference;
	};
	void InitAssociation45()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		ass.name() = "PackageConfConfigProperty";
		ass.assocClass() = PackageConfConfigProperty::meta;
		Property::meta_srcPackageConfConfigProperty = Uml::AssociationRole::Create(ass);
		Property::meta_srcPackageConfConfigProperty.name() = "srcPackageConfConfigProperty";
		Property::meta_srcPackageConfConfigProperty.min() = 0;
		Property::meta_srcPackageConfConfigProperty.max() = -1;
		Property::meta_srcPackageConfConfigProperty.isNavigable() = true;
		Property::meta_srcPackageConfConfigProperty.isPrimary() = false;
		Property::meta_srcPackageConfConfigProperty.target() = PackageConfiguration::meta;
		PackageConfConfigProperty::meta_srcPackageConfConfigProperty_end_ = PackageConfiguration::meta_dstPackageConfConfigProperty_rev = Property::meta_srcPackageConfConfigProperty;
		PackageConfiguration::meta_dstPackageConfConfigProperty = Uml::AssociationRole::Create(ass);
		PackageConfiguration::meta_dstPackageConfConfigProperty.name() = "dstPackageConfConfigProperty";
		PackageConfiguration::meta_dstPackageConfConfigProperty.min() = 0;
		PackageConfiguration::meta_dstPackageConfConfigProperty.max() = -1;
		PackageConfiguration::meta_dstPackageConfConfigProperty.isNavigable() = true;
		PackageConfiguration::meta_dstPackageConfConfigProperty.isPrimary() = false;
		PackageConfiguration::meta_dstPackageConfConfigProperty.target() = Property::meta;
		PackageConfConfigProperty::meta_dstPackageConfConfigProperty_end_ = Property::meta_srcPackageConfConfigProperty_rev = PackageConfiguration::meta_dstPackageConfConfigProperty;
	};
	void InitAssociation46()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		ass.name() = "PackageConfSelectRequirement";
		ass.assocClass() = PackageConfSelectRequirement::meta;
		Requirement::meta_srcPackageConfSelectRequirement = Uml::AssociationRole::Create(ass);
		Requirement::meta_srcPackageConfSelectRequirement.name() = "srcPackageConfSelectRequirement";
		Requirement::meta_srcPackageConfSelectRequirement.min() = 0;
		Requirement::meta_srcPackageConfSelectRequirement.max() = -1;
		Requirement::meta_srcPackageConfSelectRequirement.isNavigable() = true;
		Requirement::meta_srcPackageConfSelectRequirement.isPrimary() = false;
		Requirement::meta_srcPackageConfSelectRequirement.target() = PackageConfiguration::meta;
		PackageConfSelectRequirement::meta_srcPackageConfSelectRequirement_end_ = PackageConfiguration::meta_dstPackageConfSelectRequirement_rev = Requirement::meta_srcPackageConfSelectRequirement;
		PackageConfiguration::meta_dstPackageConfSelectRequirement = Uml::AssociationRole::Create(ass);
		PackageConfiguration::meta_dstPackageConfSelectRequirement.name() = "dstPackageConfSelectRequirement";
		PackageConfiguration::meta_dstPackageConfSelectRequirement.min() = 0;
		PackageConfiguration::meta_dstPackageConfSelectRequirement.max() = -1;
		PackageConfiguration::meta_dstPackageConfSelectRequirement.isNavigable() = true;
		PackageConfiguration::meta_dstPackageConfSelectRequirement.isPrimary() = false;
		PackageConfiguration::meta_dstPackageConfSelectRequirement.target() = Requirement::meta;
		PackageConfSelectRequirement::meta_dstPackageConfSelectRequirement_end_ = Requirement::meta_srcPackageConfSelectRequirement_rev = PackageConfiguration::meta_dstPackageConfSelectRequirement;
	};
	void InitAssociation47()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		ass.name() = "PackageConfSpecializedConfig";
		ass.assocClass() = PackageConfSpecializedConfig::meta;
		PackageConfigurationReference::meta_srcPackageConfSpecializedConfig = Uml::AssociationRole::Create(ass);
		PackageConfigurationReference::meta_srcPackageConfSpecializedConfig.name() = "srcPackageConfSpecializedConfig";
		PackageConfigurationReference::meta_srcPackageConfSpecializedConfig.min() = 0;
		PackageConfigurationReference::meta_srcPackageConfSpecializedConfig.max() = -1;
		PackageConfigurationReference::meta_srcPackageConfSpecializedConfig.isNavigable() = true;
		PackageConfigurationReference::meta_srcPackageConfSpecializedConfig.isPrimary() = false;
		PackageConfigurationReference::meta_srcPackageConfSpecializedConfig.target() = PackageConfiguration::meta;
		PackageConfSpecializedConfig::meta_srcPackageConfSpecializedConfig_end_ = PackageConfiguration::meta_dstPackageConfSpecializedConfig_rev = PackageConfigurationReference::meta_srcPackageConfSpecializedConfig;
		PackageConfiguration::meta_dstPackageConfSpecializedConfig = Uml::AssociationRole::Create(ass);
		PackageConfiguration::meta_dstPackageConfSpecializedConfig.name() = "dstPackageConfSpecializedConfig";
		PackageConfiguration::meta_dstPackageConfSpecializedConfig.min() = 0;
		PackageConfiguration::meta_dstPackageConfSpecializedConfig.max() = 1;
		PackageConfiguration::meta_dstPackageConfSpecializedConfig.isNavigable() = true;
		PackageConfiguration::meta_dstPackageConfSpecializedConfig.isPrimary() = false;
		PackageConfiguration::meta_dstPackageConfSpecializedConfig.target() = PackageConfigurationReference::meta;
		PackageConfSpecializedConfig::meta_dstPackageConfSpecializedConfig_end_ = PackageConfigurationReference::meta_srcPackageConfSpecializedConfig_rev = PackageConfiguration::meta_dstPackageConfSpecializedConfig;
	};
	void InitAssociation48()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		PackageConfigurationReference::meta_ref = Uml::AssociationRole::Create(ass);
		PackageConfigurationReference::meta_ref.name() = "ref";
		PackageConfigurationReference::meta_ref.min() = 0;
		PackageConfigurationReference::meta_ref.max() = 1;
		PackageConfigurationReference::meta_ref.isNavigable() = true;
		PackageConfigurationReference::meta_ref.isPrimary() = false;
		PackageConfigurationReference::meta_ref.target() = PackageConfiguration::meta;
		PackageConfiguration::meta_referedbyPackageConfigurationReference = Uml::AssociationRole::Create(ass);
		PackageConfiguration::meta_referedbyPackageConfigurationReference.name() = "referedbyPackageConfigurationReference";
		PackageConfiguration::meta_referedbyPackageConfigurationReference.min() = 0;
		PackageConfiguration::meta_referedbyPackageConfigurationReference.max() = -1;
		PackageConfiguration::meta_referedbyPackageConfigurationReference.isNavigable() = true;
		PackageConfiguration::meta_referedbyPackageConfigurationReference.isPrimary() = false;
		PackageConfiguration::meta_referedbyPackageConfigurationReference.target() = PackageConfigurationReference::meta;
	};
	void InitAssociation49()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		ass.name() = "package";
		ass.assocClass() = package::meta;
		PackageConfigurationReference::meta_srcpackage = Uml::AssociationRole::Create(ass);
		PackageConfigurationReference::meta_srcpackage.name() = "srcpackage";
		PackageConfigurationReference::meta_srcpackage.min() = 1;
		PackageConfigurationReference::meta_srcpackage.max() = 1;
		PackageConfigurationReference::meta_srcpackage.isNavigable() = true;
		PackageConfigurationReference::meta_srcpackage.isPrimary() = false;
		PackageConfigurationReference::meta_srcpackage.target() = TopLevelPackage::meta;
		package::meta_srcpackage_end_ = TopLevelPackage::meta_dstpackage_rev = PackageConfigurationReference::meta_srcpackage;
		TopLevelPackage::meta_dstpackage = Uml::AssociationRole::Create(ass);
		TopLevelPackage::meta_dstpackage.name() = "dstpackage";
		TopLevelPackage::meta_dstpackage.min() = 1;
		TopLevelPackage::meta_dstpackage.max() = 1;
		TopLevelPackage::meta_dstpackage.isNavigable() = true;
		TopLevelPackage::meta_dstpackage.isPrimary() = false;
		TopLevelPackage::meta_dstpackage.target() = PackageConfigurationReference::meta;
		package::meta_dstpackage_end_ = PackageConfigurationReference::meta_srcpackage_rev = TopLevelPackage::meta_dstpackage;
	};
	void InitAssociation50()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		ass.name() = "ArtifactDependsOn";
		ass.assocClass() = ArtifactDependsOn::meta;
		ImplementationArtifactReference::meta_srcArtifactDependsOn = Uml::AssociationRole::Create(ass);
		ImplementationArtifactReference::meta_srcArtifactDependsOn.name() = "srcArtifactDependsOn";
		ImplementationArtifactReference::meta_srcArtifactDependsOn.min() = 0;
		ImplementationArtifactReference::meta_srcArtifactDependsOn.max() = -1;
		ImplementationArtifactReference::meta_srcArtifactDependsOn.isNavigable() = true;
		ImplementationArtifactReference::meta_srcArtifactDependsOn.isPrimary() = false;
		ImplementationArtifactReference::meta_srcArtifactDependsOn.target() = ImplementationArtifact::meta;
		ArtifactDependsOn::meta_srcArtifactDependsOn_end_ = ImplementationArtifact::meta_dstArtifactDependsOn_rev = ImplementationArtifactReference::meta_srcArtifactDependsOn;
		ImplementationArtifact::meta_dstArtifactDependsOn = Uml::AssociationRole::Create(ass);
		ImplementationArtifact::meta_dstArtifactDependsOn.name() = "dstArtifactDependsOn";
		ImplementationArtifact::meta_dstArtifactDependsOn.min() = 0;
		ImplementationArtifact::meta_dstArtifactDependsOn.max() = -1;
		ImplementationArtifact::meta_dstArtifactDependsOn.isNavigable() = true;
		ImplementationArtifact::meta_dstArtifactDependsOn.isPrimary() = false;
		ImplementationArtifact::meta_dstArtifactDependsOn.target() = ImplementationArtifactReference::meta;
		ArtifactDependsOn::meta_dstArtifactDependsOn_end_ = ImplementationArtifactReference::meta_srcArtifactDependsOn_rev = ImplementationArtifact::meta_dstArtifactDependsOn;
	};
	void InitAssociation51()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		ass.name() = "ArtifactDeployRequirement";
		ass.assocClass() = ArtifactDeployRequirement::meta;
		Requirement::meta_srcArtifactDeployRequirement = Uml::AssociationRole::Create(ass);
		Requirement::meta_srcArtifactDeployRequirement.name() = "srcArtifactDeployRequirement";
		Requirement::meta_srcArtifactDeployRequirement.min() = 0;
		Requirement::meta_srcArtifactDeployRequirement.max() = -1;
		Requirement::meta_srcArtifactDeployRequirement.isNavigable() = true;
		Requirement::meta_srcArtifactDeployRequirement.isPrimary() = false;
		Requirement::meta_srcArtifactDeployRequirement.target() = ImplementationArtifact::meta;
		ArtifactDeployRequirement::meta_srcArtifactDeployRequirement_end_ = ImplementationArtifact::meta_dstArtifactDeployRequirement_rev = Requirement::meta_srcArtifactDeployRequirement;
		ImplementationArtifact::meta_dstArtifactDeployRequirement = Uml::AssociationRole::Create(ass);
		ImplementationArtifact::meta_dstArtifactDeployRequirement.name() = "dstArtifactDeployRequirement";
		ImplementationArtifact::meta_dstArtifactDeployRequirement.min() = 0;
		ImplementationArtifact::meta_dstArtifactDeployRequirement.max() = -1;
		ImplementationArtifact::meta_dstArtifactDeployRequirement.isNavigable() = true;
		ImplementationArtifact::meta_dstArtifactDeployRequirement.isPrimary() = false;
		ImplementationArtifact::meta_dstArtifactDeployRequirement.target() = Requirement::meta;
		ArtifactDeployRequirement::meta_dstArtifactDeployRequirement_end_ = Requirement::meta_srcArtifactDeployRequirement_rev = ImplementationArtifact::meta_dstArtifactDeployRequirement;
	};
	void InitAssociation52()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		ass.name() = "ArtifactExecParameter";
		ass.assocClass() = ArtifactExecParameter::meta;
		Property::meta_srcArtifactExecParameter = Uml::AssociationRole::Create(ass);
		Property::meta_srcArtifactExecParameter.name() = "srcArtifactExecParameter";
		Property::meta_srcArtifactExecParameter.min() = 0;
		Property::meta_srcArtifactExecParameter.max() = -1;
		Property::meta_srcArtifactExecParameter.isNavigable() = true;
		Property::meta_srcArtifactExecParameter.isPrimary() = false;
		Property::meta_srcArtifactExecParameter.target() = ImplementationArtifact::meta;
		ArtifactExecParameter::meta_srcArtifactExecParameter_end_ = ImplementationArtifact::meta_dstArtifactExecParameter_rev = Property::meta_srcArtifactExecParameter;
		ImplementationArtifact::meta_dstArtifactExecParameter = Uml::AssociationRole::Create(ass);
		ImplementationArtifact::meta_dstArtifactExecParameter.name() = "dstArtifactExecParameter";
		ImplementationArtifact::meta_dstArtifactExecParameter.min() = 0;
		ImplementationArtifact::meta_dstArtifactExecParameter.max() = -1;
		ImplementationArtifact::meta_dstArtifactExecParameter.isNavigable() = true;
		ImplementationArtifact::meta_dstArtifactExecParameter.isPrimary() = false;
		ImplementationArtifact::meta_dstArtifactExecParameter.target() = Property::meta;
		ArtifactExecParameter::meta_dstArtifactExecParameter_end_ = Property::meta_srcArtifactExecParameter_rev = ImplementationArtifact::meta_dstArtifactExecParameter;
	};
	void InitAssociation53()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		ass.name() = "ArtifactInfoProperty";
		ass.assocClass() = ArtifactInfoProperty::meta;
		Property::meta_srcArtifactInfoProperty = Uml::AssociationRole::Create(ass);
		Property::meta_srcArtifactInfoProperty.name() = "srcArtifactInfoProperty";
		Property::meta_srcArtifactInfoProperty.min() = 0;
		Property::meta_srcArtifactInfoProperty.max() = -1;
		Property::meta_srcArtifactInfoProperty.isNavigable() = true;
		Property::meta_srcArtifactInfoProperty.isPrimary() = false;
		Property::meta_srcArtifactInfoProperty.target() = ImplementationArtifact::meta;
		ArtifactInfoProperty::meta_srcArtifactInfoProperty_end_ = ImplementationArtifact::meta_dstArtifactInfoProperty_rev = Property::meta_srcArtifactInfoProperty;
		ImplementationArtifact::meta_dstArtifactInfoProperty = Uml::AssociationRole::Create(ass);
		ImplementationArtifact::meta_dstArtifactInfoProperty.name() = "dstArtifactInfoProperty";
		ImplementationArtifact::meta_dstArtifactInfoProperty.min() = 0;
		ImplementationArtifact::meta_dstArtifactInfoProperty.max() = -1;
		ImplementationArtifact::meta_dstArtifactInfoProperty.isNavigable() = true;
		ImplementationArtifact::meta_dstArtifactInfoProperty.isPrimary() = false;
		ImplementationArtifact::meta_dstArtifactInfoProperty.target() = Property::meta;
		ArtifactInfoProperty::meta_dstArtifactInfoProperty_end_ = Property::meta_srcArtifactInfoProperty_rev = ImplementationArtifact::meta_dstArtifactInfoProperty;
	};
	void InitAssociation54()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		ImplementationArtifactReference::meta_ref = Uml::AssociationRole::Create(ass);
		ImplementationArtifactReference::meta_ref.name() = "ref";
		ImplementationArtifactReference::meta_ref.min() = 0;
		ImplementationArtifactReference::meta_ref.max() = 1;
		ImplementationArtifactReference::meta_ref.isNavigable() = true;
		ImplementationArtifactReference::meta_ref.isPrimary() = false;
		ImplementationArtifactReference::meta_ref.target() = ImplementationArtifact::meta;
		ImplementationArtifact::meta_referedbyImplementationArtifactReference = Uml::AssociationRole::Create(ass);
		ImplementationArtifact::meta_referedbyImplementationArtifactReference.name() = "referedbyImplementationArtifactReference";
		ImplementationArtifact::meta_referedbyImplementationArtifactReference.min() = 0;
		ImplementationArtifact::meta_referedbyImplementationArtifactReference.max() = -1;
		ImplementationArtifact::meta_referedbyImplementationArtifactReference.isNavigable() = true;
		ImplementationArtifact::meta_referedbyImplementationArtifactReference.isPrimary() = false;
		ImplementationArtifact::meta_referedbyImplementationArtifactReference.target() = ImplementationArtifactReference::meta;
	};
	void InitAssociation55()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		ImplementationArtifact::meta_referedbyExternalResources = Uml::AssociationRole::Create(ass);
		ImplementationArtifact::meta_referedbyExternalResources.name() = "referedbyExternalResources";
		ImplementationArtifact::meta_referedbyExternalResources.min() = 0;
		ImplementationArtifact::meta_referedbyExternalResources.max() = -1;
		ImplementationArtifact::meta_referedbyExternalResources.isNavigable() = true;
		ImplementationArtifact::meta_referedbyExternalResources.isPrimary() = false;
		ImplementationArtifact::meta_referedbyExternalResources.target() = ExternalResources::meta;
		ExternalResources::meta_ref = Uml::AssociationRole::Create(ass);
		ExternalResources::meta_ref.name() = "ref";
		ExternalResources::meta_ref.min() = 0;
		ExternalResources::meta_ref.max() = 1;
		ExternalResources::meta_ref.isNavigable() = true;
		ExternalResources::meta_ref.isPrimary() = false;
		ExternalResources::meta_ref.target() = ImplementationArtifact::meta;
	};
	void InitAssociation56()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		ass.name() = "ComponentProperty";
		ass.assocClass() = ComponentProperty::meta;
		ComponentPropertyDescription::meta_srcComponentProperty = Uml::AssociationRole::Create(ass);
		ComponentPropertyDescription::meta_srcComponentProperty.name() = "srcComponentProperty";
		ComponentPropertyDescription::meta_srcComponentProperty.min() = 0;
		ComponentPropertyDescription::meta_srcComponentProperty.max() = -1;
		ComponentPropertyDescription::meta_srcComponentProperty.isNavigable() = true;
		ComponentPropertyDescription::meta_srcComponentProperty.isPrimary() = false;
		ComponentPropertyDescription::meta_srcComponentProperty.target() = ComponentRef::meta;
		ComponentProperty::meta_srcComponentProperty_end_ = ComponentRef::meta_dstComponentProperty_rev = ComponentPropertyDescription::meta_srcComponentProperty;
		ComponentRef::meta_dstComponentProperty = Uml::AssociationRole::Create(ass);
		ComponentRef::meta_dstComponentProperty.name() = "dstComponentProperty";
		ComponentRef::meta_dstComponentProperty.min() = 0;
		ComponentRef::meta_dstComponentProperty.max() = -1;
		ComponentRef::meta_dstComponentProperty.isNavigable() = true;
		ComponentRef::meta_dstComponentProperty.isPrimary() = false;
		ComponentRef::meta_dstComponentProperty.target() = ComponentPropertyDescription::meta;
		ComponentProperty::meta_dstComponentProperty_end_ = ComponentPropertyDescription::meta_srcComponentProperty_rev = ComponentRef::meta_dstComponentProperty;
	};
	void InitAssociation57()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		ass.name() = "ComponentConfigProperty";
		ass.assocClass() = ComponentConfigProperty::meta;
		Property::meta_srcComponentConfigProperty = Uml::AssociationRole::Create(ass);
		Property::meta_srcComponentConfigProperty.name() = "srcComponentConfigProperty";
		Property::meta_srcComponentConfigProperty.min() = 0;
		Property::meta_srcComponentConfigProperty.max() = -1;
		Property::meta_srcComponentConfigProperty.isNavigable() = true;
		Property::meta_srcComponentConfigProperty.isPrimary() = false;
		Property::meta_srcComponentConfigProperty.target() = ComponentRef::meta;
		ComponentConfigProperty::meta_srcComponentConfigProperty_end_ = ComponentRef::meta_dstComponentConfigProperty_rev = Property::meta_srcComponentConfigProperty;
		ComponentRef::meta_dstComponentConfigProperty = Uml::AssociationRole::Create(ass);
		ComponentRef::meta_dstComponentConfigProperty.name() = "dstComponentConfigProperty";
		ComponentRef::meta_dstComponentConfigProperty.min() = 0;
		ComponentRef::meta_dstComponentConfigProperty.max() = -1;
		ComponentRef::meta_dstComponentConfigProperty.isNavigable() = true;
		ComponentRef::meta_dstComponentConfigProperty.isPrimary() = false;
		ComponentRef::meta_dstComponentConfigProperty.target() = Property::meta;
		ComponentConfigProperty::meta_dstComponentConfigProperty_end_ = Property::meta_srcComponentConfigProperty_rev = ComponentRef::meta_dstComponentConfigProperty;
	};
	void InitAssociation58()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		ass.name() = "ComponentInfoProperty";
		ass.assocClass() = ComponentInfoProperty::meta;
		Property::meta_srcComponentInfoProperty = Uml::AssociationRole::Create(ass);
		Property::meta_srcComponentInfoProperty.name() = "srcComponentInfoProperty";
		Property::meta_srcComponentInfoProperty.min() = 0;
		Property::meta_srcComponentInfoProperty.max() = -1;
		Property::meta_srcComponentInfoProperty.isNavigable() = true;
		Property::meta_srcComponentInfoProperty.isPrimary() = false;
		Property::meta_srcComponentInfoProperty.target() = ComponentRef::meta;
		ComponentInfoProperty::meta_srcComponentInfoProperty_end_ = ComponentRef::meta_dstComponentInfoProperty_rev = Property::meta_srcComponentInfoProperty;
		ComponentRef::meta_dstComponentInfoProperty = Uml::AssociationRole::Create(ass);
		ComponentRef::meta_dstComponentInfoProperty.name() = "dstComponentInfoProperty";
		ComponentRef::meta_dstComponentInfoProperty.min() = 0;
		ComponentRef::meta_dstComponentInfoProperty.max() = -1;
		ComponentRef::meta_dstComponentInfoProperty.isNavigable() = true;
		ComponentRef::meta_dstComponentInfoProperty.isPrimary() = false;
		ComponentRef::meta_dstComponentInfoProperty.target() = Property::meta;
		ComponentInfoProperty::meta_dstComponentInfoProperty_end_ = Property::meta_srcComponentInfoProperty_rev = ComponentRef::meta_dstComponentInfoProperty;
	};
	void InitAssociation59()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		ass.name() = "PackageInterface";
		ass.assocClass() = PackageInterface::meta;
		ComponentRef::meta_srcPackageInterface = Uml::AssociationRole::Create(ass);
		ComponentRef::meta_srcPackageInterface.name() = "srcPackageInterface";
		ComponentRef::meta_srcPackageInterface.min() = 0;
		ComponentRef::meta_srcPackageInterface.max() = 1;
		ComponentRef::meta_srcPackageInterface.isNavigable() = true;
		ComponentRef::meta_srcPackageInterface.isPrimary() = false;
		ComponentRef::meta_srcPackageInterface.target() = ComponentPackage::meta;
		PackageInterface::meta_srcPackageInterface_end_ = ComponentPackage::meta_dstPackageInterface_rev = ComponentRef::meta_srcPackageInterface;
		ComponentPackage::meta_dstPackageInterface = Uml::AssociationRole::Create(ass);
		ComponentPackage::meta_dstPackageInterface.name() = "dstPackageInterface";
		ComponentPackage::meta_dstPackageInterface.min() = 0;
		ComponentPackage::meta_dstPackageInterface.max() = 1;
		ComponentPackage::meta_dstPackageInterface.isNavigable() = true;
		ComponentPackage::meta_dstPackageInterface.isPrimary() = false;
		ComponentPackage::meta_dstPackageInterface.target() = ComponentRef::meta;
		PackageInterface::meta_dstPackageInterface_end_ = ComponentRef::meta_srcPackageInterface_rev = ComponentPackage::meta_dstPackageInterface;
	};
	void InitAssociation60()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		ass.name() = "PackageConfigProperty";
		ass.assocClass() = PackageConfigProperty::meta;
		Property::meta_srcPackageConfigProperty = Uml::AssociationRole::Create(ass);
		Property::meta_srcPackageConfigProperty.name() = "srcPackageConfigProperty";
		Property::meta_srcPackageConfigProperty.min() = 0;
		Property::meta_srcPackageConfigProperty.max() = -1;
		Property::meta_srcPackageConfigProperty.isNavigable() = true;
		Property::meta_srcPackageConfigProperty.isPrimary() = false;
		Property::meta_srcPackageConfigProperty.target() = ComponentPackage::meta;
		PackageConfigProperty::meta_srcPackageConfigProperty_end_ = ComponentPackage::meta_dstPackageConfigProperty_rev = Property::meta_srcPackageConfigProperty;
		ComponentPackage::meta_dstPackageConfigProperty = Uml::AssociationRole::Create(ass);
		ComponentPackage::meta_dstPackageConfigProperty.name() = "dstPackageConfigProperty";
		ComponentPackage::meta_dstPackageConfigProperty.min() = 0;
		ComponentPackage::meta_dstPackageConfigProperty.max() = -1;
		ComponentPackage::meta_dstPackageConfigProperty.isNavigable() = true;
		ComponentPackage::meta_dstPackageConfigProperty.isPrimary() = false;
		ComponentPackage::meta_dstPackageConfigProperty.target() = Property::meta;
		PackageConfigProperty::meta_dstPackageConfigProperty_end_ = Property::meta_srcPackageConfigProperty_rev = ComponentPackage::meta_dstPackageConfigProperty;
	};
	void InitAssociation61()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		ass.name() = "PackageInfoProperty";
		ass.assocClass() = PackageInfoProperty::meta;
		Property::meta_srcPackageInfoProperty = Uml::AssociationRole::Create(ass);
		Property::meta_srcPackageInfoProperty.name() = "srcPackageInfoProperty";
		Property::meta_srcPackageInfoProperty.min() = 0;
		Property::meta_srcPackageInfoProperty.max() = -1;
		Property::meta_srcPackageInfoProperty.isNavigable() = true;
		Property::meta_srcPackageInfoProperty.isPrimary() = false;
		Property::meta_srcPackageInfoProperty.target() = ComponentPackage::meta;
		PackageInfoProperty::meta_srcPackageInfoProperty_end_ = ComponentPackage::meta_dstPackageInfoProperty_rev = Property::meta_srcPackageInfoProperty;
		ComponentPackage::meta_dstPackageInfoProperty = Uml::AssociationRole::Create(ass);
		ComponentPackage::meta_dstPackageInfoProperty.name() = "dstPackageInfoProperty";
		ComponentPackage::meta_dstPackageInfoProperty.min() = 0;
		ComponentPackage::meta_dstPackageInfoProperty.max() = -1;
		ComponentPackage::meta_dstPackageInfoProperty.isNavigable() = true;
		ComponentPackage::meta_dstPackageInfoProperty.isPrimary() = false;
		ComponentPackage::meta_dstPackageInfoProperty.target() = Property::meta;
		PackageInfoProperty::meta_dstPackageInfoProperty_end_ = Property::meta_srcPackageInfoProperty_rev = ComponentPackage::meta_dstPackageInfoProperty;
	};
	void InitAssociation62()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		ComponentPackage::meta_referedbyComponentPackageReference = Uml::AssociationRole::Create(ass);
		ComponentPackage::meta_referedbyComponentPackageReference.name() = "referedbyComponentPackageReference";
		ComponentPackage::meta_referedbyComponentPackageReference.min() = 0;
		ComponentPackage::meta_referedbyComponentPackageReference.max() = -1;
		ComponentPackage::meta_referedbyComponentPackageReference.isNavigable() = true;
		ComponentPackage::meta_referedbyComponentPackageReference.isPrimary() = false;
		ComponentPackage::meta_referedbyComponentPackageReference.target() = ComponentPackageReference::meta;
		ComponentPackageReference::meta_ref = Uml::AssociationRole::Create(ass);
		ComponentPackageReference::meta_ref.name() = "ref";
		ComponentPackageReference::meta_ref.min() = 0;
		ComponentPackageReference::meta_ref.max() = 1;
		ComponentPackageReference::meta_ref.isNavigable() = true;
		ComponentPackageReference::meta_ref.isPrimary() = false;
		ComponentPackageReference::meta_ref.target() = ComponentPackage::meta;
	};
	void InitAssociation63()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		PredefinedType::meta_referedbyDataType = Uml::AssociationRole::Create(ass);
		PredefinedType::meta_referedbyDataType.name() = "referedbyDataType";
		PredefinedType::meta_referedbyDataType.min() = 0;
		PredefinedType::meta_referedbyDataType.max() = -1;
		PredefinedType::meta_referedbyDataType.isNavigable() = true;
		PredefinedType::meta_referedbyDataType.isPrimary() = false;
		PredefinedType::meta_referedbyDataType.target() = DataType::meta;
		DataType::meta_ref = Uml::AssociationRole::Create(ass);
		DataType::meta_ref.name() = "ref";
		DataType::meta_ref.min() = 0;
		DataType::meta_ref.max() = 1;
		DataType::meta_ref.isNavigable() = true;
		DataType::meta_ref.isPrimary() = false;
		DataType::meta_ref.target() = PredefinedType::meta;
	};
	void InitAssociation64()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		OutEventPort::meta_ref = Uml::AssociationRole::Create(ass);
		OutEventPort::meta_ref.name() = "ref";
		OutEventPort::meta_ref.min() = 0;
		OutEventPort::meta_ref.max() = 1;
		OutEventPort::meta_ref.isNavigable() = true;
		OutEventPort::meta_ref.isPrimary() = false;
		OutEventPort::meta_ref.target() = Event::meta;
		Event::meta_referedbyOutEventPort = Uml::AssociationRole::Create(ass);
		Event::meta_referedbyOutEventPort.name() = "referedbyOutEventPort";
		Event::meta_referedbyOutEventPort.min() = 0;
		Event::meta_referedbyOutEventPort.max() = -1;
		Event::meta_referedbyOutEventPort.isNavigable() = true;
		Event::meta_referedbyOutEventPort.isPrimary() = false;
		Event::meta_referedbyOutEventPort.target() = OutEventPort::meta;
	};
	void InitAssociation65()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		ProvidedRequestPort::meta_ref = Uml::AssociationRole::Create(ass);
		ProvidedRequestPort::meta_ref.name() = "ref";
		ProvidedRequestPort::meta_ref.min() = 0;
		ProvidedRequestPort::meta_ref.max() = 1;
		ProvidedRequestPort::meta_ref.isNavigable() = true;
		ProvidedRequestPort::meta_ref.isPrimary() = false;
		ProvidedRequestPort::meta_ref.target() = Provideable::meta;
		Provideable::meta_referedbyProvidedRequestPort = Uml::AssociationRole::Create(ass);
		Provideable::meta_referedbyProvidedRequestPort.name() = "referedbyProvidedRequestPort";
		Provideable::meta_referedbyProvidedRequestPort.min() = 0;
		Provideable::meta_referedbyProvidedRequestPort.max() = -1;
		Provideable::meta_referedbyProvidedRequestPort.isNavigable() = true;
		Provideable::meta_referedbyProvidedRequestPort.isPrimary() = false;
		Provideable::meta_referedbyProvidedRequestPort.target() = ProvidedRequestPort::meta;
	};
	void InitAssociation66()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		RequiredRequestPort::meta_ref = Uml::AssociationRole::Create(ass);
		RequiredRequestPort::meta_ref.name() = "ref";
		RequiredRequestPort::meta_ref.min() = 0;
		RequiredRequestPort::meta_ref.max() = 1;
		RequiredRequestPort::meta_ref.isNavigable() = true;
		RequiredRequestPort::meta_ref.isPrimary() = false;
		RequiredRequestPort::meta_ref.target() = Provideable::meta;
		Provideable::meta_referedbyRequiredRequestPort = Uml::AssociationRole::Create(ass);
		Provideable::meta_referedbyRequiredRequestPort.name() = "referedbyRequiredRequestPort";
		Provideable::meta_referedbyRequiredRequestPort.min() = 0;
		Provideable::meta_referedbyRequiredRequestPort.max() = -1;
		Provideable::meta_referedbyRequiredRequestPort.isNavigable() = true;
		Provideable::meta_referedbyRequiredRequestPort.isPrimary() = false;
		Provideable::meta_referedbyRequiredRequestPort.target() = RequiredRequestPort::meta;
	};
	void InitAssociation67()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		InEventPort::meta_ref = Uml::AssociationRole::Create(ass);
		InEventPort::meta_ref.name() = "ref";
		InEventPort::meta_ref.min() = 0;
		InEventPort::meta_ref.max() = 1;
		InEventPort::meta_ref.isNavigable() = true;
		InEventPort::meta_ref.isPrimary() = false;
		InEventPort::meta_ref.target() = Event::meta;
		Event::meta_referedbyInEventPort = Uml::AssociationRole::Create(ass);
		Event::meta_referedbyInEventPort.name() = "referedbyInEventPort";
		Event::meta_referedbyInEventPort.min() = 0;
		Event::meta_referedbyInEventPort.max() = -1;
		Event::meta_referedbyInEventPort.isNavigable() = true;
		Event::meta_referedbyInEventPort.isPrimary() = false;
		Event::meta_referedbyInEventPort.target() = InEventPort::meta;
	};
	void InitAssociation68()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		ass.name() = "ManagesComponent";
		ass.assocClass() = ManagesComponent::meta;
		Manageable::meta_srcManagesComponent = Uml::AssociationRole::Create(ass);
		Manageable::meta_srcManagesComponent.name() = "srcManagesComponent";
		Manageable::meta_srcManagesComponent.min() = 0;
		Manageable::meta_srcManagesComponent.max() = -1;
		Manageable::meta_srcManagesComponent.isNavigable() = true;
		Manageable::meta_srcManagesComponent.isPrimary() = false;
		Manageable::meta_srcManagesComponent.target() = ComponentFactory::meta;
		ManagesComponent::meta_srcManagesComponent_end_ = ComponentFactory::meta_dstManagesComponent_rev = Manageable::meta_srcManagesComponent;
		ComponentFactory::meta_dstManagesComponent = Uml::AssociationRole::Create(ass);
		ComponentFactory::meta_dstManagesComponent.name() = "dstManagesComponent";
		ComponentFactory::meta_dstManagesComponent.min() = 1;
		ComponentFactory::meta_dstManagesComponent.max() = 1;
		ComponentFactory::meta_dstManagesComponent.isNavigable() = true;
		ComponentFactory::meta_dstManagesComponent.isPrimary() = false;
		ComponentFactory::meta_dstManagesComponent.target() = Manageable::meta;
		ManagesComponent::meta_dstManagesComponent_end_ = Manageable::meta_srcManagesComponent_rev = ComponentFactory::meta_dstManagesComponent;
	};
	void InitAssociation69()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		Inheritable::meta_referedbyInherits = Uml::AssociationRole::Create(ass);
		Inheritable::meta_referedbyInherits.name() = "referedbyInherits";
		Inheritable::meta_referedbyInherits.min() = 0;
		Inheritable::meta_referedbyInherits.max() = -1;
		Inheritable::meta_referedbyInherits.isNavigable() = true;
		Inheritable::meta_referedbyInherits.isPrimary() = false;
		Inheritable::meta_referedbyInherits.target() = Inherits::meta;
		Inherits::meta_ref = Uml::AssociationRole::Create(ass);
		Inherits::meta_ref.name() = "ref";
		Inherits::meta_ref.min() = 0;
		Inherits::meta_ref.max() = 1;
		Inherits::meta_ref.isNavigable() = true;
		Inherits::meta_ref.isPrimary() = false;
		Inherits::meta_ref.target() = Inheritable::meta;
	};
	void InitAssociation70()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		MemberType::meta_referedbyInoutParameter = Uml::AssociationRole::Create(ass);
		MemberType::meta_referedbyInoutParameter.name() = "referedbyInoutParameter";
		MemberType::meta_referedbyInoutParameter.min() = 0;
		MemberType::meta_referedbyInoutParameter.max() = -1;
		MemberType::meta_referedbyInoutParameter.isNavigable() = true;
		MemberType::meta_referedbyInoutParameter.isPrimary() = false;
		MemberType::meta_referedbyInoutParameter.target() = InoutParameter::meta;
		InoutParameter::meta_ref = Uml::AssociationRole::Create(ass);
		InoutParameter::meta_ref.name() = "ref";
		InoutParameter::meta_ref.min() = 0;
		InoutParameter::meta_ref.max() = 1;
		InoutParameter::meta_ref.isNavigable() = true;
		InoutParameter::meta_ref.isPrimary() = false;
		InoutParameter::meta_ref.target() = MemberType::meta;
	};
	void InitAssociation71()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		MemberType::meta_referedbyInParameter = Uml::AssociationRole::Create(ass);
		MemberType::meta_referedbyInParameter.name() = "referedbyInParameter";
		MemberType::meta_referedbyInParameter.min() = 0;
		MemberType::meta_referedbyInParameter.max() = -1;
		MemberType::meta_referedbyInParameter.isNavigable() = true;
		MemberType::meta_referedbyInParameter.isPrimary() = false;
		MemberType::meta_referedbyInParameter.target() = InParameter::meta;
		InParameter::meta_ref = Uml::AssociationRole::Create(ass);
		InParameter::meta_ref.name() = "ref";
		InParameter::meta_ref.min() = 0;
		InParameter::meta_ref.max() = 1;
		InParameter::meta_ref.isNavigable() = true;
		InParameter::meta_ref.isPrimary() = false;
		InParameter::meta_ref.target() = MemberType::meta;
	};
	void InitAssociation72()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		MemberType::meta_referedbyOutParameter = Uml::AssociationRole::Create(ass);
		MemberType::meta_referedbyOutParameter.name() = "referedbyOutParameter";
		MemberType::meta_referedbyOutParameter.min() = 0;
		MemberType::meta_referedbyOutParameter.max() = -1;
		MemberType::meta_referedbyOutParameter.isNavigable() = true;
		MemberType::meta_referedbyOutParameter.isPrimary() = false;
		MemberType::meta_referedbyOutParameter.target() = OutParameter::meta;
		OutParameter::meta_ref = Uml::AssociationRole::Create(ass);
		OutParameter::meta_ref.name() = "ref";
		OutParameter::meta_ref.min() = 0;
		OutParameter::meta_ref.max() = 1;
		OutParameter::meta_ref.isNavigable() = true;
		OutParameter::meta_ref.isPrimary() = false;
		OutParameter::meta_ref.target() = MemberType::meta;
	};
	void InitAssociation73()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		MemberType::meta_referedbyReturnType = Uml::AssociationRole::Create(ass);
		MemberType::meta_referedbyReturnType.name() = "referedbyReturnType";
		MemberType::meta_referedbyReturnType.min() = 0;
		MemberType::meta_referedbyReturnType.max() = -1;
		MemberType::meta_referedbyReturnType.isNavigable() = true;
		MemberType::meta_referedbyReturnType.isPrimary() = false;
		MemberType::meta_referedbyReturnType.target() = ReturnType::meta;
		ReturnType::meta_ref = Uml::AssociationRole::Create(ass);
		ReturnType::meta_ref.name() = "ref";
		ReturnType::meta_ref.min() = 0;
		ReturnType::meta_ref.max() = 1;
		ReturnType::meta_ref.isNavigable() = true;
		ReturnType::meta_ref.isPrimary() = false;
		ReturnType::meta_ref.target() = MemberType::meta;
	};
	void InitAssociation74()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		MemberType::meta_referedbyAttributeMember = Uml::AssociationRole::Create(ass);
		MemberType::meta_referedbyAttributeMember.name() = "referedbyAttributeMember";
		MemberType::meta_referedbyAttributeMember.min() = 0;
		MemberType::meta_referedbyAttributeMember.max() = -1;
		MemberType::meta_referedbyAttributeMember.isNavigable() = true;
		MemberType::meta_referedbyAttributeMember.isPrimary() = false;
		MemberType::meta_referedbyAttributeMember.target() = AttributeMember::meta;
		AttributeMember::meta_ref = Uml::AssociationRole::Create(ass);
		AttributeMember::meta_ref.name() = "ref";
		AttributeMember::meta_ref.min() = 0;
		AttributeMember::meta_ref.max() = 1;
		AttributeMember::meta_ref.isNavigable() = true;
		AttributeMember::meta_ref.isPrimary() = false;
		AttributeMember::meta_ref.target() = MemberType::meta;
	};
	void InitAssociation75()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		MemberType::meta_referedbyCollection = Uml::AssociationRole::Create(ass);
		MemberType::meta_referedbyCollection.name() = "referedbyCollection";
		MemberType::meta_referedbyCollection.min() = 0;
		MemberType::meta_referedbyCollection.max() = -1;
		MemberType::meta_referedbyCollection.isNavigable() = true;
		MemberType::meta_referedbyCollection.isPrimary() = false;
		MemberType::meta_referedbyCollection.target() = Collection::meta;
		Collection::meta_ref = Uml::AssociationRole::Create(ass);
		Collection::meta_ref.name() = "ref";
		Collection::meta_ref.min() = 0;
		Collection::meta_ref.max() = 1;
		Collection::meta_ref.isNavigable() = true;
		Collection::meta_ref.isPrimary() = false;
		Collection::meta_ref.target() = MemberType::meta;
	};
	void InitAssociation76()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		MemberType::meta_referedbyAlias = Uml::AssociationRole::Create(ass);
		MemberType::meta_referedbyAlias.name() = "referedbyAlias";
		MemberType::meta_referedbyAlias.min() = 0;
		MemberType::meta_referedbyAlias.max() = -1;
		MemberType::meta_referedbyAlias.isNavigable() = true;
		MemberType::meta_referedbyAlias.isPrimary() = false;
		MemberType::meta_referedbyAlias.target() = Alias::meta;
		Alias::meta_ref = Uml::AssociationRole::Create(ass);
		Alias::meta_ref.name() = "ref";
		Alias::meta_ref.min() = 0;
		Alias::meta_ref.max() = 1;
		Alias::meta_ref.isNavigable() = true;
		Alias::meta_ref.isPrimary() = false;
		Alias::meta_ref.target() = MemberType::meta;
	};
	void InitAssociation77()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		MemberType::meta_referedbyBoxed = Uml::AssociationRole::Create(ass);
		MemberType::meta_referedbyBoxed.name() = "referedbyBoxed";
		MemberType::meta_referedbyBoxed.min() = 0;
		MemberType::meta_referedbyBoxed.max() = -1;
		MemberType::meta_referedbyBoxed.isNavigable() = true;
		MemberType::meta_referedbyBoxed.isPrimary() = false;
		MemberType::meta_referedbyBoxed.target() = Boxed::meta;
		Boxed::meta_ref = Uml::AssociationRole::Create(ass);
		Boxed::meta_ref.name() = "ref";
		Boxed::meta_ref.min() = 0;
		Boxed::meta_ref.max() = 1;
		Boxed::meta_ref.isNavigable() = true;
		Boxed::meta_ref.isPrimary() = false;
		Boxed::meta_ref.target() = MemberType::meta;
	};
	void InitAssociation78()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		MemberType::meta_referedbyMember = Uml::AssociationRole::Create(ass);
		MemberType::meta_referedbyMember.name() = "referedbyMember";
		MemberType::meta_referedbyMember.min() = 0;
		MemberType::meta_referedbyMember.max() = -1;
		MemberType::meta_referedbyMember.isNavigable() = true;
		MemberType::meta_referedbyMember.isPrimary() = false;
		MemberType::meta_referedbyMember.target() = Member::meta;
		Member::meta_ref = Uml::AssociationRole::Create(ass);
		Member::meta_ref.name() = "ref";
		Member::meta_ref.min() = 0;
		Member::meta_ref.max() = 1;
		Member::meta_ref.isNavigable() = true;
		Member::meta_ref.isPrimary() = false;
		Member::meta_ref.target() = MemberType::meta;
	};
	void InitAssociation79()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		ComponentRef::meta_ref = Uml::AssociationRole::Create(ass);
		ComponentRef::meta_ref.name() = "ref";
		ComponentRef::meta_ref.min() = 0;
		ComponentRef::meta_ref.max() = 1;
		ComponentRef::meta_ref.isNavigable() = true;
		ComponentRef::meta_ref.isPrimary() = false;
		ComponentRef::meta_ref.target() = Component::meta;
		Component::meta_referedbyComponentRef = Uml::AssociationRole::Create(ass);
		Component::meta_referedbyComponentRef.name() = "referedbyComponentRef";
		Component::meta_referedbyComponentRef.min() = 0;
		Component::meta_referedbyComponentRef.max() = -1;
		Component::meta_referedbyComponentRef.isNavigable() = true;
		Component::meta_referedbyComponentRef.isPrimary() = false;
		Component::meta_referedbyComponentRef.target() = ComponentRef::meta;
	};
	void InitAssociation80()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		Object::meta_referedbySupports = Uml::AssociationRole::Create(ass);
		Object::meta_referedbySupports.name() = "referedbySupports";
		Object::meta_referedbySupports.min() = 0;
		Object::meta_referedbySupports.max() = -1;
		Object::meta_referedbySupports.isNavigable() = true;
		Object::meta_referedbySupports.isPrimary() = false;
		Object::meta_referedbySupports.target() = Supports::meta;
		Supports::meta_ref = Uml::AssociationRole::Create(ass);
		Supports::meta_ref.name() = "ref";
		Supports::meta_ref.min() = 0;
		Supports::meta_ref.max() = 1;
		Supports::meta_ref.isNavigable() = true;
		Supports::meta_ref.isPrimary() = false;
		Supports::meta_ref.target() = Object::meta;
	};
	void InitAssociation81()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		ValueObject::meta_referedbyLookupKey = Uml::AssociationRole::Create(ass);
		ValueObject::meta_referedbyLookupKey.name() = "referedbyLookupKey";
		ValueObject::meta_referedbyLookupKey.min() = 0;
		ValueObject::meta_referedbyLookupKey.max() = -1;
		ValueObject::meta_referedbyLookupKey.isNavigable() = true;
		ValueObject::meta_referedbyLookupKey.isPrimary() = false;
		ValueObject::meta_referedbyLookupKey.target() = LookupKey::meta;
		LookupKey::meta_ref = Uml::AssociationRole::Create(ass);
		LookupKey::meta_ref.name() = "ref";
		LookupKey::meta_ref.min() = 0;
		LookupKey::meta_ref.max() = 1;
		LookupKey::meta_ref.isNavigable() = true;
		LookupKey::meta_ref.isPrimary() = false;
		LookupKey::meta_ref.target() = ValueObject::meta;
	};
	void InitAssociation82()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		Exception::meta_referedbySetException = Uml::AssociationRole::Create(ass);
		Exception::meta_referedbySetException.name() = "referedbySetException";
		Exception::meta_referedbySetException.min() = 0;
		Exception::meta_referedbySetException.max() = -1;
		Exception::meta_referedbySetException.isNavigable() = true;
		Exception::meta_referedbySetException.isPrimary() = false;
		Exception::meta_referedbySetException.target() = SetException::meta;
		SetException::meta_ref = Uml::AssociationRole::Create(ass);
		SetException::meta_ref.name() = "ref";
		SetException::meta_ref.min() = 0;
		SetException::meta_ref.max() = 1;
		SetException::meta_ref.isNavigable() = true;
		SetException::meta_ref.isPrimary() = false;
		SetException::meta_ref.target() = Exception::meta;
	};
	void InitAssociation83()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		Exception::meta_referedbyGetException = Uml::AssociationRole::Create(ass);
		Exception::meta_referedbyGetException.name() = "referedbyGetException";
		Exception::meta_referedbyGetException.min() = 0;
		Exception::meta_referedbyGetException.max() = -1;
		Exception::meta_referedbyGetException.isNavigable() = true;
		Exception::meta_referedbyGetException.isPrimary() = false;
		Exception::meta_referedbyGetException.target() = GetException::meta;
		GetException::meta_ref = Uml::AssociationRole::Create(ass);
		GetException::meta_ref.name() = "ref";
		GetException::meta_ref.min() = 0;
		GetException::meta_ref.max() = 1;
		GetException::meta_ref.isNavigable() = true;
		GetException::meta_ref.isPrimary() = false;
		GetException::meta_ref.target() = Exception::meta;
	};
	void InitAssociation84()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		ass.name() = "MakeMemberPrivate";
		ass.assocClass() = MakeMemberPrivate::meta;
		PrivateFlag::meta_srcMakeMemberPrivate = Uml::AssociationRole::Create(ass);
		PrivateFlag::meta_srcMakeMemberPrivate.name() = "srcMakeMemberPrivate";
		PrivateFlag::meta_srcMakeMemberPrivate.min() = 1;
		PrivateFlag::meta_srcMakeMemberPrivate.max() = 1;
		PrivateFlag::meta_srcMakeMemberPrivate.isNavigable() = true;
		PrivateFlag::meta_srcMakeMemberPrivate.isPrimary() = false;
		PrivateFlag::meta_srcMakeMemberPrivate.target() = Member::meta;
		MakeMemberPrivate::meta_srcMakeMemberPrivate_end_ = Member::meta_dstMakeMemberPrivate_rev = PrivateFlag::meta_srcMakeMemberPrivate;
		Member::meta_dstMakeMemberPrivate = Uml::AssociationRole::Create(ass);
		Member::meta_dstMakeMemberPrivate.name() = "dstMakeMemberPrivate";
		Member::meta_dstMakeMemberPrivate.min() = 0;
		Member::meta_dstMakeMemberPrivate.max() = 1;
		Member::meta_dstMakeMemberPrivate.isNavigable() = true;
		Member::meta_dstMakeMemberPrivate.isPrimary() = false;
		Member::meta_dstMakeMemberPrivate.target() = PrivateFlag::meta;
		MakeMemberPrivate::meta_dstMakeMemberPrivate_end_ = PrivateFlag::meta_srcMakeMemberPrivate_rev = Member::meta_dstMakeMemberPrivate;
	};
	void InitAssociation85()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		ass.name() = "LabelConnection";
		ass.assocClass() = LabelConnection::meta;
		Label::meta_srcLabelConnection = Uml::AssociationRole::Create(ass);
		Label::meta_srcLabelConnection.name() = "srcLabelConnection";
		Label::meta_srcLabelConnection.min() = 1;
		Label::meta_srcLabelConnection.max() = 1;
		Label::meta_srcLabelConnection.isNavigable() = true;
		Label::meta_srcLabelConnection.isPrimary() = false;
		Label::meta_srcLabelConnection.target() = Member::meta;
		LabelConnection::meta_srcLabelConnection_end_ = Member::meta_dstLabelConnection_rev = Label::meta_srcLabelConnection;
		Member::meta_dstLabelConnection = Uml::AssociationRole::Create(ass);
		Member::meta_dstLabelConnection.name() = "dstLabelConnection";
		Member::meta_dstLabelConnection.min() = 0;
		Member::meta_dstLabelConnection.max() = -1;
		Member::meta_dstLabelConnection.isNavigable() = true;
		Member::meta_dstLabelConnection.isPrimary() = false;
		Member::meta_dstLabelConnection.target() = Label::meta;
		LabelConnection::meta_dstLabelConnection_end_ = Label::meta_srcLabelConnection_rev = Member::meta_dstLabelConnection;
	};
	void InitAssociation86()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		ConstantType::meta_referedbyDiscriminator = Uml::AssociationRole::Create(ass);
		ConstantType::meta_referedbyDiscriminator.name() = "referedbyDiscriminator";
		ConstantType::meta_referedbyDiscriminator.min() = 0;
		ConstantType::meta_referedbyDiscriminator.max() = -1;
		ConstantType::meta_referedbyDiscriminator.isNavigable() = true;
		ConstantType::meta_referedbyDiscriminator.isPrimary() = false;
		ConstantType::meta_referedbyDiscriminator.target() = Discriminator::meta;
		Discriminator::meta_ref = Uml::AssociationRole::Create(ass);
		Discriminator::meta_ref.name() = "ref";
		Discriminator::meta_ref.min() = 0;
		Discriminator::meta_ref.max() = 1;
		Discriminator::meta_ref.isNavigable() = true;
		Discriminator::meta_ref.isPrimary() = false;
		Discriminator::meta_ref.target() = ConstantType::meta;
	};
	void InitAssociation87()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		ConstantType::meta_referedbyConstant = Uml::AssociationRole::Create(ass);
		ConstantType::meta_referedbyConstant.name() = "referedbyConstant";
		ConstantType::meta_referedbyConstant.min() = 0;
		ConstantType::meta_referedbyConstant.max() = -1;
		ConstantType::meta_referedbyConstant.isNavigable() = true;
		ConstantType::meta_referedbyConstant.isPrimary() = false;
		ConstantType::meta_referedbyConstant.target() = Constant::meta;
		Constant::meta_ref = Uml::AssociationRole::Create(ass);
		Constant::meta_ref.name() = "ref";
		Constant::meta_ref.min() = 0;
		Constant::meta_ref.max() = 1;
		Constant::meta_ref.isNavigable() = true;
		Constant::meta_ref.isPrimary() = false;
		Constant::meta_ref.target() = ConstantType::meta;
	};
	void InitAssociation88()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		File::meta_referedbyFileRef = Uml::AssociationRole::Create(ass);
		File::meta_referedbyFileRef.name() = "referedbyFileRef";
		File::meta_referedbyFileRef.min() = 0;
		File::meta_referedbyFileRef.max() = -1;
		File::meta_referedbyFileRef.isNavigable() = true;
		File::meta_referedbyFileRef.isPrimary() = false;
		File::meta_referedbyFileRef.target() = FileRef::meta;
		FileRef::meta_ref = Uml::AssociationRole::Create(ass);
		FileRef::meta_ref.name() = "ref";
		FileRef::meta_ref.min() = 0;
		FileRef::meta_ref.max() = 1;
		FileRef::meta_ref.isNavigable() = true;
		FileRef::meta_ref.isPrimary() = false;
		FileRef::meta_ref.target() = File::meta;
	};
	void InitAssociation89()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		Exception::meta_referedbyExceptionRef = Uml::AssociationRole::Create(ass);
		Exception::meta_referedbyExceptionRef.name() = "referedbyExceptionRef";
		Exception::meta_referedbyExceptionRef.min() = 0;
		Exception::meta_referedbyExceptionRef.max() = -1;
		Exception::meta_referedbyExceptionRef.isNavigable() = true;
		Exception::meta_referedbyExceptionRef.isPrimary() = false;
		Exception::meta_referedbyExceptionRef.target() = ExceptionRef::meta;
		ExceptionRef::meta_ref = Uml::AssociationRole::Create(ass);
		ExceptionRef::meta_ref.name() = "ref";
		ExceptionRef::meta_ref.min() = 0;
		ExceptionRef::meta_ref.max() = 1;
		ExceptionRef::meta_ref.isNavigable() = true;
		ExceptionRef::meta_ref.isPrimary() = false;
		ExceptionRef::meta_ref.target() = Exception::meta;
	};
	void InitAssociation90()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		Workspaces::meta_members = Uml::AssociationRole::Create(ass);
		Workspaces::meta_members.name() = "members";
		Workspaces::meta_members.min() = 0;
		Workspaces::meta_members.max() = -1;
		Workspaces::meta_members.isNavigable() = true;
		Workspaces::meta_members.isPrimary() = false;
		Workspaces::meta_members.target() = Project::meta;
		Project::meta_setWorkspace = Uml::AssociationRole::Create(ass);
		Project::meta_setWorkspace.name() = "setWorkspace";
		Project::meta_setWorkspace.min() = 0;
		Project::meta_setWorkspace.max() = -1;
		Project::meta_setWorkspace.isNavigable() = true;
		Project::meta_setWorkspace.isPrimary() = false;
		Project::meta_setWorkspace.target() = Workspaces::meta;
	};
	void InitAssociation91()
	{
		Uml::Association ass = Uml::Association::Create(umldiagram);
		ass.name() = "ExtResourceConn";
		ass.assocClass() = ExtResourceConn::meta;
		ExternalResources::meta_srcExtResourceConn = Uml::AssociationRole::Create(ass);
		ExternalResources::meta_srcExtResourceConn.name() = "srcExtResourceConn";
		ExternalResources::meta_srcExtResourceConn.min() = 0;
		ExternalResources::meta_srcExtResourceConn.max() = -1;
		ExternalResources::meta_srcExtResourceConn.isNavigable() = true;
		ExternalResources::meta_srcExtResourceConn.isPrimary() = false;
		ExternalResources::meta_srcExtResourceConn.target() = ComponentLib::meta;
		ExtResourceConn::meta_srcExtResourceConn_end_ = ComponentLib::meta_dstExtResourceConn_rev = ExternalResources::meta_srcExtResourceConn;
		ComponentLib::meta_dstExtResourceConn = Uml::AssociationRole::Create(ass);
		ComponentLib::meta_dstExtResourceConn.name() = "dstExtResourceConn";
		ComponentLib::meta_dstExtResourceConn.min() = 0;
		ComponentLib::meta_dstExtResourceConn.max() = -1;
		ComponentLib::meta_dstExtResourceConn.isNavigable() = true;
		ComponentLib::meta_dstExtResourceConn.isPrimary() = false;
		ComponentLib::meta_dstExtResourceConn.target() = ExternalResources::meta;
		ExtResourceConn::meta_dstExtResourceConn_end_ = ExternalResources::meta_srcExtResourceConn_rev = ComponentLib::meta_dstExtResourceConn;
	};
	void InitComposition0()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		RT_Info_Param::meta_RTEC_Proxy_parent = Uml::CompositionParentRole::Create(comp);
		RT_Info_Param::meta_RTEC_Proxy_parent.name() = "RTEC_Proxy_parent";
		RT_Info_Param::meta_RTEC_Proxy_parent.isNavigable() = true;
		RT_Info_Param::meta_RTEC_Proxy_parent.target() = RTEC_Proxy::meta;
		RTEC_Proxy::meta_RT_Info_Param_children = Uml::CompositionChildRole::Create(comp);
		RTEC_Proxy::meta_RT_Info_Param_children.name() = "RT_Info_Param";
		RTEC_Proxy::meta_RT_Info_Param_children.min() = 0;
		RTEC_Proxy::meta_RT_Info_Param_children.max() = -1;
		RTEC_Proxy::meta_RT_Info_Param_children.isNavigable() = true;
		RTEC_Proxy::meta_RT_Info_Param_children.target() = RT_Info_Param::meta;
	}
	void InitComposition1()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		Include_RT_Info::meta_RTEC_Proxy_Supplier_parent = Uml::CompositionParentRole::Create(comp);
		Include_RT_Info::meta_RTEC_Proxy_Supplier_parent.name() = "RTEC_Proxy_Supplier_parent";
		Include_RT_Info::meta_RTEC_Proxy_Supplier_parent.isNavigable() = true;
		Include_RT_Info::meta_RTEC_Proxy_Supplier_parent.target() = RTEC_Proxy_Supplier::meta;
		RTEC_Proxy_Supplier::meta_Include_RT_Info_children = Uml::CompositionChildRole::Create(comp);
		RTEC_Proxy_Supplier::meta_Include_RT_Info_children.name() = "Include_RT_Info";
		RTEC_Proxy_Supplier::meta_Include_RT_Info_children.min() = 0;
		RTEC_Proxy_Supplier::meta_Include_RT_Info_children.max() = -1;
		RTEC_Proxy_Supplier::meta_Include_RT_Info_children.isNavigable() = true;
		RTEC_Proxy_Supplier::meta_Include_RT_Info_children.target() = Include_RT_Info::meta;
	}
	void InitComposition2()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		Include_RTEC_Filter_Group::meta_RTEC_Proxy_Supplier_parent = Uml::CompositionParentRole::Create(comp);
		Include_RTEC_Filter_Group::meta_RTEC_Proxy_Supplier_parent.name() = "RTEC_Proxy_Supplier_parent";
		Include_RTEC_Filter_Group::meta_RTEC_Proxy_Supplier_parent.isNavigable() = true;
		Include_RTEC_Filter_Group::meta_RTEC_Proxy_Supplier_parent.target() = RTEC_Proxy_Supplier::meta;
		RTEC_Proxy_Supplier::meta_Include_RTEC_Filter_Group_children = Uml::CompositionChildRole::Create(comp);
		RTEC_Proxy_Supplier::meta_Include_RTEC_Filter_Group_children.name() = "Include_RTEC_Filter_Group";
		RTEC_Proxy_Supplier::meta_Include_RTEC_Filter_Group_children.min() = 0;
		RTEC_Proxy_Supplier::meta_Include_RTEC_Filter_Group_children.max() = -1;
		RTEC_Proxy_Supplier::meta_Include_RTEC_Filter_Group_children.isNavigable() = true;
		RTEC_Proxy_Supplier::meta_Include_RTEC_Filter_Group_children.target() = Include_RTEC_Filter_Group::meta;
	}
	void InitComposition3()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		RTEC_Filter_Group::meta_RTEC_Proxy_Supplier_parent = Uml::CompositionParentRole::Create(comp);
		RTEC_Filter_Group::meta_RTEC_Proxy_Supplier_parent.name() = "RTEC_Proxy_Supplier_parent";
		RTEC_Filter_Group::meta_RTEC_Proxy_Supplier_parent.isNavigable() = true;
		RTEC_Filter_Group::meta_RTEC_Proxy_Supplier_parent.target() = RTEC_Proxy_Supplier::meta;
		RTEC_Proxy_Supplier::meta_RTEC_Filter_Group_children = Uml::CompositionChildRole::Create(comp);
		RTEC_Proxy_Supplier::meta_RTEC_Filter_Group_children.name() = "RTEC_Filter_Group";
		RTEC_Proxy_Supplier::meta_RTEC_Filter_Group_children.min() = 0;
		RTEC_Proxy_Supplier::meta_RTEC_Filter_Group_children.max() = -1;
		RTEC_Proxy_Supplier::meta_RTEC_Filter_Group_children.isNavigable() = true;
		RTEC_Proxy_Supplier::meta_RTEC_Filter_Group_children.target() = RTEC_Filter_Group::meta;
	}
	void InitComposition4()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		Proxy::meta_EventChannelConfiguration_parent = Uml::CompositionParentRole::Create(comp);
		Proxy::meta_EventChannelConfiguration_parent.name() = "EventChannelConfiguration_parent";
		Proxy::meta_EventChannelConfiguration_parent.isNavigable() = true;
		Proxy::meta_EventChannelConfiguration_parent.target() = EventChannelConfiguration::meta;
		EventChannelConfiguration::meta_Proxy_children = Uml::CompositionChildRole::Create(comp);
		EventChannelConfiguration::meta_Proxy_children.name() = "Proxy";
		EventChannelConfiguration::meta_Proxy_children.min() = 0;
		EventChannelConfiguration::meta_Proxy_children.max() = -1;
		EventChannelConfiguration::meta_Proxy_children.isNavigable() = true;
		EventChannelConfiguration::meta_Proxy_children.target() = Proxy::meta;
	}
	void InitComposition5()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		RTEC_Resource_Factory_Reference::meta_RTEC_Proxy_Consumer_parent = Uml::CompositionParentRole::Create(comp);
		RTEC_Resource_Factory_Reference::meta_RTEC_Proxy_Consumer_parent.name() = "RTEC_Proxy_Consumer_parent";
		RTEC_Resource_Factory_Reference::meta_RTEC_Proxy_Consumer_parent.isNavigable() = true;
		RTEC_Resource_Factory_Reference::meta_RTEC_Proxy_Consumer_parent.target() = RTEC_Proxy_Consumer::meta;
		RTEC_Proxy_Consumer::meta_RTEC_Resource_Factory_Reference_child = Uml::CompositionChildRole::Create(comp);
		RTEC_Proxy_Consumer::meta_RTEC_Resource_Factory_Reference_child.name() = "RTEC_Resource_Factory_Reference";
		RTEC_Proxy_Consumer::meta_RTEC_Resource_Factory_Reference_child.min() = 1;
		RTEC_Proxy_Consumer::meta_RTEC_Resource_Factory_Reference_child.max() = 1;
		RTEC_Proxy_Consumer::meta_RTEC_Resource_Factory_Reference_child.isNavigable() = true;
		RTEC_Proxy_Consumer::meta_RTEC_Resource_Factory_Reference_child.target() = RTEC_Resource_Factory_Reference::meta;
	}
	void InitComposition6()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		Use_RT_Info::meta_RTEC_Proxy_Consumer_parent = Uml::CompositionParentRole::Create(comp);
		Use_RT_Info::meta_RTEC_Proxy_Consumer_parent.name() = "RTEC_Proxy_Consumer_parent";
		Use_RT_Info::meta_RTEC_Proxy_Consumer_parent.isNavigable() = true;
		Use_RT_Info::meta_RTEC_Proxy_Consumer_parent.target() = RTEC_Proxy_Consumer::meta;
		RTEC_Proxy_Consumer::meta_Use_RT_Info_children = Uml::CompositionChildRole::Create(comp);
		RTEC_Proxy_Consumer::meta_Use_RT_Info_children.name() = "Use_RT_Info";
		RTEC_Proxy_Consumer::meta_Use_RT_Info_children.min() = 0;
		RTEC_Proxy_Consumer::meta_Use_RT_Info_children.max() = -1;
		RTEC_Proxy_Consumer::meta_Use_RT_Info_children.isNavigable() = true;
		RTEC_Proxy_Consumer::meta_Use_RT_Info_children.target() = Use_RT_Info::meta;
	}
	void InitComposition7()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		RTEC_Connection::meta_EventChannelConfiguration_parent = Uml::CompositionParentRole::Create(comp);
		RTEC_Connection::meta_EventChannelConfiguration_parent.name() = "EventChannelConfiguration_parent";
		RTEC_Connection::meta_EventChannelConfiguration_parent.isNavigable() = true;
		RTEC_Connection::meta_EventChannelConfiguration_parent.target() = EventChannelConfiguration::meta;
		EventChannelConfiguration::meta_RTEC_Connection_children = Uml::CompositionChildRole::Create(comp);
		EventChannelConfiguration::meta_RTEC_Connection_children.name() = "RTEC_Connection";
		EventChannelConfiguration::meta_RTEC_Connection_children.min() = 0;
		EventChannelConfiguration::meta_RTEC_Connection_children.max() = -1;
		EventChannelConfiguration::meta_RTEC_Connection_children.isNavigable() = true;
		EventChannelConfiguration::meta_RTEC_Connection_children.target() = RTEC_Connection::meta;
	}
	void InitComposition8()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		Use_RTEC_Filter::meta_EventChannelConfiguration_parent = Uml::CompositionParentRole::Create(comp);
		Use_RTEC_Filter::meta_EventChannelConfiguration_parent.name() = "EventChannelConfiguration_parent";
		Use_RTEC_Filter::meta_EventChannelConfiguration_parent.isNavigable() = true;
		Use_RTEC_Filter::meta_EventChannelConfiguration_parent.target() = EventChannelConfiguration::meta;
		EventChannelConfiguration::meta_Use_RTEC_Filter_children = Uml::CompositionChildRole::Create(comp);
		EventChannelConfiguration::meta_Use_RTEC_Filter_children.name() = "Use_RTEC_Filter";
		EventChannelConfiguration::meta_Use_RTEC_Filter_children.min() = 0;
		EventChannelConfiguration::meta_Use_RTEC_Filter_children.max() = -1;
		EventChannelConfiguration::meta_Use_RTEC_Filter_children.isNavigable() = true;
		EventChannelConfiguration::meta_Use_RTEC_Filter_children.target() = Use_RTEC_Filter::meta;
	}
	void InitComposition9()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		Use_RT_Info::meta_EventChannelConfiguration_parent = Uml::CompositionParentRole::Create(comp);
		Use_RT_Info::meta_EventChannelConfiguration_parent.name() = "EventChannelConfiguration_parent";
		Use_RT_Info::meta_EventChannelConfiguration_parent.isNavigable() = true;
		Use_RT_Info::meta_EventChannelConfiguration_parent.target() = EventChannelConfiguration::meta;
		EventChannelConfiguration::meta_Use_RT_Info_children = Uml::CompositionChildRole::Create(comp);
		EventChannelConfiguration::meta_Use_RT_Info_children.name() = "Use_RT_Info";
		EventChannelConfiguration::meta_Use_RT_Info_children.min() = 0;
		EventChannelConfiguration::meta_Use_RT_Info_children.max() = -1;
		EventChannelConfiguration::meta_Use_RT_Info_children.isNavigable() = true;
		EventChannelConfiguration::meta_Use_RT_Info_children.target() = Use_RT_Info::meta;
	}
	void InitComposition10()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		RTEC_Resource_Factory::meta_EventChannelConfiguration_parent = Uml::CompositionParentRole::Create(comp);
		RTEC_Resource_Factory::meta_EventChannelConfiguration_parent.name() = "EventChannelConfiguration_parent";
		RTEC_Resource_Factory::meta_EventChannelConfiguration_parent.isNavigable() = true;
		RTEC_Resource_Factory::meta_EventChannelConfiguration_parent.target() = EventChannelConfiguration::meta;
		EventChannelConfiguration::meta_RTEC_Resource_Factory_children = Uml::CompositionChildRole::Create(comp);
		EventChannelConfiguration::meta_RTEC_Resource_Factory_children.name() = "RTEC_Resource_Factory";
		EventChannelConfiguration::meta_RTEC_Resource_Factory_children.min() = 0;
		EventChannelConfiguration::meta_RTEC_Resource_Factory_children.max() = -1;
		EventChannelConfiguration::meta_RTEC_Resource_Factory_children.isNavigable() = true;
		EventChannelConfiguration::meta_RTEC_Resource_Factory_children.target() = RTEC_Resource_Factory::meta;
	}
	void InitComposition11()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		ComponentRef::meta_EventChannelConfiguration_parent = Uml::CompositionParentRole::Create(comp);
		ComponentRef::meta_EventChannelConfiguration_parent.name() = "EventChannelConfiguration_parent";
		ComponentRef::meta_EventChannelConfiguration_parent.isNavigable() = true;
		ComponentRef::meta_EventChannelConfiguration_parent.target() = EventChannelConfiguration::meta;
		EventChannelConfiguration::meta_ComponentRef_children = Uml::CompositionChildRole::Create(comp);
		EventChannelConfiguration::meta_ComponentRef_children.name() = "ComponentRef";
		EventChannelConfiguration::meta_ComponentRef_children.min() = 0;
		EventChannelConfiguration::meta_ComponentRef_children.max() = -1;
		EventChannelConfiguration::meta_ComponentRef_children.isNavigable() = true;
		EventChannelConfiguration::meta_ComponentRef_children.target() = ComponentRef::meta;
	}
	void InitComposition12()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		Port::meta_EventChannelConfiguration_parent = Uml::CompositionParentRole::Create(comp);
		Port::meta_EventChannelConfiguration_parent.name() = "EventChannelConfiguration_parent";
		Port::meta_EventChannelConfiguration_parent.isNavigable() = true;
		Port::meta_EventChannelConfiguration_parent.target() = EventChannelConfiguration::meta;
		EventChannelConfiguration::meta_Port_children = Uml::CompositionChildRole::Create(comp);
		EventChannelConfiguration::meta_Port_children.name() = "Port";
		EventChannelConfiguration::meta_Port_children.min() = 0;
		EventChannelConfiguration::meta_Port_children.max() = -1;
		EventChannelConfiguration::meta_Port_children.isNavigable() = true;
		EventChannelConfiguration::meta_Port_children.target() = Port::meta;
	}
	void InitComposition13()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		EventChannelConfiguration::meta_EventChannelConfig_parent = Uml::CompositionParentRole::Create(comp);
		EventChannelConfiguration::meta_EventChannelConfig_parent.name() = "EventChannelConfig_parent";
		EventChannelConfiguration::meta_EventChannelConfig_parent.isNavigable() = true;
		EventChannelConfiguration::meta_EventChannelConfig_parent.target() = EventChannelConfig::meta;
		EventChannelConfig::meta_EventChannelConfiguration_children = Uml::CompositionChildRole::Create(comp);
		EventChannelConfig::meta_EventChannelConfiguration_children.name() = "EventChannelConfiguration";
		EventChannelConfig::meta_EventChannelConfiguration_children.min() = 0;
		EventChannelConfig::meta_EventChannelConfiguration_children.max() = -1;
		EventChannelConfig::meta_EventChannelConfiguration_children.isNavigable() = true;
		EventChannelConfig::meta_EventChannelConfiguration_children.target() = EventChannelConfiguration::meta;
	}
	void InitComposition14()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		EventChannelConfig::meta_RootFolder_parent = Uml::CompositionParentRole::Create(comp);
		EventChannelConfig::meta_RootFolder_parent.name() = "RootFolder_parent";
		EventChannelConfig::meta_RootFolder_parent.isNavigable() = true;
		EventChannelConfig::meta_RootFolder_parent.target() = RootFolder::meta;
		RootFolder::meta_EventChannelConfig_children = Uml::CompositionChildRole::Create(comp);
		RootFolder::meta_EventChannelConfig_children.name() = "EventChannelConfig";
		RootFolder::meta_EventChannelConfig_children.min() = 0;
		RootFolder::meta_EventChannelConfig_children.max() = -1;
		RootFolder::meta_EventChannelConfig_children.isNavigable() = true;
		RootFolder::meta_EventChannelConfig_children.target() = EventChannelConfig::meta;
	}
	void InitComposition15()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		MetricConnection::meta_BenchmarkAnalysis_parent = Uml::CompositionParentRole::Create(comp);
		MetricConnection::meta_BenchmarkAnalysis_parent.name() = "BenchmarkAnalysis_parent";
		MetricConnection::meta_BenchmarkAnalysis_parent.isNavigable() = true;
		MetricConnection::meta_BenchmarkAnalysis_parent.target() = BenchmarkAnalysis::meta;
		BenchmarkAnalysis::meta_MetricConnection_children = Uml::CompositionChildRole::Create(comp);
		BenchmarkAnalysis::meta_MetricConnection_children.name() = "MetricConnection";
		BenchmarkAnalysis::meta_MetricConnection_children.min() = 0;
		BenchmarkAnalysis::meta_MetricConnection_children.max() = -1;
		BenchmarkAnalysis::meta_MetricConnection_children.isNavigable() = true;
		BenchmarkAnalysis::meta_MetricConnection_children.target() = MetricConnection::meta;
	}
	void InitComposition16()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		DataAnalysisBase::meta_MetricsBase_parent = Uml::CompositionParentRole::Create(comp);
		DataAnalysisBase::meta_MetricsBase_parent.name() = "MetricsBase_parent";
		DataAnalysisBase::meta_MetricsBase_parent.isNavigable() = true;
		DataAnalysisBase::meta_MetricsBase_parent.target() = MetricsBase::meta;
		MetricsBase::meta_DataAnalysisBase_children = Uml::CompositionChildRole::Create(comp);
		MetricsBase::meta_DataAnalysisBase_children.name() = "DataAnalysisBase";
		MetricsBase::meta_DataAnalysisBase_children.min() = 0;
		MetricsBase::meta_DataAnalysisBase_children.max() = -1;
		MetricsBase::meta_DataAnalysisBase_children.isNavigable() = true;
		MetricsBase::meta_DataAnalysisBase_children.target() = DataAnalysisBase::meta;
	}
	void InitComposition17()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		DataAnalysisBase::meta_BenchmarkAnalysis_parent = Uml::CompositionParentRole::Create(comp);
		DataAnalysisBase::meta_BenchmarkAnalysis_parent.name() = "BenchmarkAnalysis_parent";
		DataAnalysisBase::meta_BenchmarkAnalysis_parent.isNavigable() = true;
		DataAnalysisBase::meta_BenchmarkAnalysis_parent.target() = BenchmarkAnalysis::meta;
		BenchmarkAnalysis::meta_DataAnalysisBase_children = Uml::CompositionChildRole::Create(comp);
		BenchmarkAnalysis::meta_DataAnalysisBase_children.name() = "DataAnalysisBase";
		BenchmarkAnalysis::meta_DataAnalysisBase_children.min() = 0;
		BenchmarkAnalysis::meta_DataAnalysisBase_children.max() = -1;
		BenchmarkAnalysis::meta_DataAnalysisBase_children.isNavigable() = true;
		BenchmarkAnalysis::meta_DataAnalysisBase_children.target() = DataAnalysisBase::meta;
	}
	void InitComposition18()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		TimeProbe::meta_BenchmarkAnalysis_parent = Uml::CompositionParentRole::Create(comp);
		TimeProbe::meta_BenchmarkAnalysis_parent.name() = "BenchmarkAnalysis_parent";
		TimeProbe::meta_BenchmarkAnalysis_parent.isNavigable() = true;
		TimeProbe::meta_BenchmarkAnalysis_parent.target() = BenchmarkAnalysis::meta;
		BenchmarkAnalysis::meta_TimeProbe_children = Uml::CompositionChildRole::Create(comp);
		BenchmarkAnalysis::meta_TimeProbe_children.name() = "TimeProbe";
		BenchmarkAnalysis::meta_TimeProbe_children.min() = 0;
		BenchmarkAnalysis::meta_TimeProbe_children.max() = -1;
		BenchmarkAnalysis::meta_TimeProbe_children.isNavigable() = true;
		BenchmarkAnalysis::meta_TimeProbe_children.target() = TimeProbe::meta;
	}
	void InitComposition19()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		TimerConnection::meta_BenchmarkAnalysis_parent = Uml::CompositionParentRole::Create(comp);
		TimerConnection::meta_BenchmarkAnalysis_parent.name() = "BenchmarkAnalysis_parent";
		TimerConnection::meta_BenchmarkAnalysis_parent.isNavigable() = true;
		TimerConnection::meta_BenchmarkAnalysis_parent.target() = BenchmarkAnalysis::meta;
		BenchmarkAnalysis::meta_TimerConnection_children = Uml::CompositionChildRole::Create(comp);
		BenchmarkAnalysis::meta_TimerConnection_children.name() = "TimerConnection";
		BenchmarkAnalysis::meta_TimerConnection_children.min() = 0;
		BenchmarkAnalysis::meta_TimerConnection_children.max() = -1;
		BenchmarkAnalysis::meta_TimerConnection_children.isNavigable() = true;
		BenchmarkAnalysis::meta_TimerConnection_children.target() = TimerConnection::meta;
	}
	void InitComposition20()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		MetricsBase::meta_BenchmarkAnalysis_parent = Uml::CompositionParentRole::Create(comp);
		MetricsBase::meta_BenchmarkAnalysis_parent.name() = "BenchmarkAnalysis_parent";
		MetricsBase::meta_BenchmarkAnalysis_parent.isNavigable() = true;
		MetricsBase::meta_BenchmarkAnalysis_parent.target() = BenchmarkAnalysis::meta;
		BenchmarkAnalysis::meta_MetricsBase_children = Uml::CompositionChildRole::Create(comp);
		BenchmarkAnalysis::meta_MetricsBase_children.name() = "MetricsBase";
		BenchmarkAnalysis::meta_MetricsBase_children.min() = 0;
		BenchmarkAnalysis::meta_MetricsBase_children.max() = -1;
		BenchmarkAnalysis::meta_MetricsBase_children.isNavigable() = true;
		BenchmarkAnalysis::meta_MetricsBase_children.target() = MetricsBase::meta;
	}
	void InitComposition21()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		BenchmarkAnalysis::meta_ComponentAnalyses_parent = Uml::CompositionParentRole::Create(comp);
		BenchmarkAnalysis::meta_ComponentAnalyses_parent.name() = "ComponentAnalyses_parent";
		BenchmarkAnalysis::meta_ComponentAnalyses_parent.isNavigable() = true;
		BenchmarkAnalysis::meta_ComponentAnalyses_parent.target() = ComponentAnalyses::meta;
		ComponentAnalyses::meta_BenchmarkAnalysis_children = Uml::CompositionChildRole::Create(comp);
		ComponentAnalyses::meta_BenchmarkAnalysis_children.name() = "BenchmarkAnalysis";
		ComponentAnalyses::meta_BenchmarkAnalysis_children.min() = 0;
		ComponentAnalyses::meta_BenchmarkAnalysis_children.max() = -1;
		ComponentAnalyses::meta_BenchmarkAnalysis_children.isNavigable() = true;
		ComponentAnalyses::meta_BenchmarkAnalysis_children.target() = BenchmarkAnalysis::meta;
	}
	void InitComposition22()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		ComponentAnalyses::meta_RootFolder_parent = Uml::CompositionParentRole::Create(comp);
		ComponentAnalyses::meta_RootFolder_parent.name() = "RootFolder_parent";
		ComponentAnalyses::meta_RootFolder_parent.isNavigable() = true;
		ComponentAnalyses::meta_RootFolder_parent.target() = RootFolder::meta;
		RootFolder::meta_ComponentAnalyses_children = Uml::CompositionChildRole::Create(comp);
		RootFolder::meta_ComponentAnalyses_children.name() = "ComponentAnalyses";
		RootFolder::meta_ComponentAnalyses_children.min() = 0;
		RootFolder::meta_ComponentAnalyses_children.max() = -1;
		RootFolder::meta_ComponentAnalyses_children.isNavigable() = true;
		RootFolder::meta_ComponentAnalyses_children.target() = ComponentAnalyses::meta;
	}
	void InitComposition23()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		OperationBase::meta_BenchmarkAnalysis_parent = Uml::CompositionParentRole::Create(comp);
		OperationBase::meta_BenchmarkAnalysis_parent.name() = "BenchmarkAnalysis_parent";
		OperationBase::meta_BenchmarkAnalysis_parent.isNavigable() = true;
		OperationBase::meta_BenchmarkAnalysis_parent.target() = BenchmarkAnalysis::meta;
		BenchmarkAnalysis::meta_OperationBase_children = Uml::CompositionChildRole::Create(comp);
		BenchmarkAnalysis::meta_OperationBase_children.name() = "OperationBase";
		BenchmarkAnalysis::meta_OperationBase_children.min() = 0;
		BenchmarkAnalysis::meta_OperationBase_children.max() = -1;
		BenchmarkAnalysis::meta_OperationBase_children.isNavigable() = true;
		BenchmarkAnalysis::meta_OperationBase_children.target() = OperationBase::meta;
	}
	void InitComposition24()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		OperationRef::meta_BenchmarkAnalysis_parent = Uml::CompositionParentRole::Create(comp);
		OperationRef::meta_BenchmarkAnalysis_parent.name() = "BenchmarkAnalysis_parent";
		OperationRef::meta_BenchmarkAnalysis_parent.isNavigable() = true;
		OperationRef::meta_BenchmarkAnalysis_parent.target() = BenchmarkAnalysis::meta;
		BenchmarkAnalysis::meta_OperationRef_children = Uml::CompositionChildRole::Create(comp);
		BenchmarkAnalysis::meta_OperationRef_children.name() = "OperationRef";
		BenchmarkAnalysis::meta_OperationRef_children.min() = 0;
		BenchmarkAnalysis::meta_OperationRef_children.max() = -1;
		BenchmarkAnalysis::meta_OperationRef_children.isNavigable() = true;
		BenchmarkAnalysis::meta_OperationRef_children.target() = OperationRef::meta;
	}
	void InitComposition25()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		EventRef::meta_BenchmarkAnalysis_parent = Uml::CompositionParentRole::Create(comp);
		EventRef::meta_BenchmarkAnalysis_parent.name() = "BenchmarkAnalysis_parent";
		EventRef::meta_BenchmarkAnalysis_parent.isNavigable() = true;
		EventRef::meta_BenchmarkAnalysis_parent.target() = BenchmarkAnalysis::meta;
		BenchmarkAnalysis::meta_EventRef_children = Uml::CompositionChildRole::Create(comp);
		BenchmarkAnalysis::meta_EventRef_children.name() = "EventRef";
		BenchmarkAnalysis::meta_EventRef_children.min() = 0;
		BenchmarkAnalysis::meta_EventRef_children.max() = -1;
		BenchmarkAnalysis::meta_EventRef_children.isNavigable() = true;
		BenchmarkAnalysis::meta_EventRef_children.target() = EventRef::meta;
	}
	void InitComposition26()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		Event::meta_BenchmarkAnalysis_parent = Uml::CompositionParentRole::Create(comp);
		Event::meta_BenchmarkAnalysis_parent.name() = "BenchmarkAnalysis_parent";
		Event::meta_BenchmarkAnalysis_parent.isNavigable() = true;
		Event::meta_BenchmarkAnalysis_parent.target() = BenchmarkAnalysis::meta;
		BenchmarkAnalysis::meta_Event_children = Uml::CompositionChildRole::Create(comp);
		BenchmarkAnalysis::meta_Event_children.name() = "Event";
		BenchmarkAnalysis::meta_Event_children.min() = 0;
		BenchmarkAnalysis::meta_Event_children.max() = -1;
		BenchmarkAnalysis::meta_Event_children.isNavigable() = true;
		BenchmarkAnalysis::meta_Event_children.target() = Event::meta;
	}
	void InitComposition27()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		TimerEventSinkConn::meta_BenchmarkAnalysis_parent = Uml::CompositionParentRole::Create(comp);
		TimerEventSinkConn::meta_BenchmarkAnalysis_parent.name() = "BenchmarkAnalysis_parent";
		TimerEventSinkConn::meta_BenchmarkAnalysis_parent.isNavigable() = true;
		TimerEventSinkConn::meta_BenchmarkAnalysis_parent.target() = BenchmarkAnalysis::meta;
		BenchmarkAnalysis::meta_TimerEventSinkConn_children = Uml::CompositionChildRole::Create(comp);
		BenchmarkAnalysis::meta_TimerEventSinkConn_children.name() = "TimerEventSinkConn";
		BenchmarkAnalysis::meta_TimerEventSinkConn_children.min() = 0;
		BenchmarkAnalysis::meta_TimerEventSinkConn_children.max() = -1;
		BenchmarkAnalysis::meta_TimerEventSinkConn_children.isNavigable() = true;
		BenchmarkAnalysis::meta_TimerEventSinkConn_children.target() = TimerEventSinkConn::meta;
	}
	void InitComposition28()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		TaskSet::meta_BenchmarkAnalysis_parent = Uml::CompositionParentRole::Create(comp);
		TaskSet::meta_BenchmarkAnalysis_parent.name() = "BenchmarkAnalysis_parent";
		TaskSet::meta_BenchmarkAnalysis_parent.isNavigable() = true;
		TaskSet::meta_BenchmarkAnalysis_parent.target() = BenchmarkAnalysis::meta;
		BenchmarkAnalysis::meta_TaskSet_children = Uml::CompositionChildRole::Create(comp);
		BenchmarkAnalysis::meta_TaskSet_children.name() = "TaskSet";
		BenchmarkAnalysis::meta_TaskSet_children.min() = 0;
		BenchmarkAnalysis::meta_TaskSet_children.max() = -1;
		BenchmarkAnalysis::meta_TaskSet_children.isNavigable() = true;
		BenchmarkAnalysis::meta_TaskSet_children.target() = TaskSet::meta;
	}
	void InitComposition29()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		Task::meta_BenchmarkAnalysis_parent = Uml::CompositionParentRole::Create(comp);
		Task::meta_BenchmarkAnalysis_parent.name() = "BenchmarkAnalysis_parent";
		Task::meta_BenchmarkAnalysis_parent.isNavigable() = true;
		Task::meta_BenchmarkAnalysis_parent.target() = BenchmarkAnalysis::meta;
		BenchmarkAnalysis::meta_Task_children = Uml::CompositionChildRole::Create(comp);
		BenchmarkAnalysis::meta_Task_children.name() = "Task";
		BenchmarkAnalysis::meta_Task_children.min() = 0;
		BenchmarkAnalysis::meta_Task_children.max() = -1;
		BenchmarkAnalysis::meta_Task_children.isNavigable() = true;
		BenchmarkAnalysis::meta_Task_children.target() = Task::meta;
	}
	void InitComposition30()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		WorkloadCharacteristics::meta_BenchmarkAnalysis_parent = Uml::CompositionParentRole::Create(comp);
		WorkloadCharacteristics::meta_BenchmarkAnalysis_parent.name() = "BenchmarkAnalysis_parent";
		WorkloadCharacteristics::meta_BenchmarkAnalysis_parent.isNavigable() = true;
		WorkloadCharacteristics::meta_BenchmarkAnalysis_parent.target() = BenchmarkAnalysis::meta;
		BenchmarkAnalysis::meta_WorkloadCharacteristics_children = Uml::CompositionChildRole::Create(comp);
		BenchmarkAnalysis::meta_WorkloadCharacteristics_children.name() = "WorkloadCharacteristics";
		BenchmarkAnalysis::meta_WorkloadCharacteristics_children.min() = 0;
		BenchmarkAnalysis::meta_WorkloadCharacteristics_children.max() = -1;
		BenchmarkAnalysis::meta_WorkloadCharacteristics_children.isNavigable() = true;
		BenchmarkAnalysis::meta_WorkloadCharacteristics_children.target() = WorkloadCharacteristics::meta;
	}
	void InitComposition31()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		NodeReference::meta_DeploymentPlan_parent = Uml::CompositionParentRole::Create(comp);
		NodeReference::meta_DeploymentPlan_parent.name() = "DeploymentPlan_parent";
		NodeReference::meta_DeploymentPlan_parent.isNavigable() = true;
		NodeReference::meta_DeploymentPlan_parent.target() = DeploymentPlan::meta;
		DeploymentPlan::meta_NodeReference_children = Uml::CompositionChildRole::Create(comp);
		DeploymentPlan::meta_NodeReference_children.name() = "NodeReference";
		DeploymentPlan::meta_NodeReference_children.min() = 0;
		DeploymentPlan::meta_NodeReference_children.max() = -1;
		DeploymentPlan::meta_NodeReference_children.isNavigable() = true;
		DeploymentPlan::meta_NodeReference_children.target() = NodeReference::meta;
	}
	void InitComposition32()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		Resource::meta_Interconnect_parent = Uml::CompositionParentRole::Create(comp);
		Resource::meta_Interconnect_parent.name() = "Interconnect_parent";
		Resource::meta_Interconnect_parent.isNavigable() = true;
		Resource::meta_Interconnect_parent.target() = Interconnect::meta;
		Interconnect::meta_Resource_children = Uml::CompositionChildRole::Create(comp);
		Interconnect::meta_Resource_children.name() = "Resource";
		Interconnect::meta_Resource_children.min() = 0;
		Interconnect::meta_Resource_children.max() = -1;
		Interconnect::meta_Resource_children.isNavigable() = true;
		Interconnect::meta_Resource_children.target() = Resource::meta;
	}
	void InitComposition33()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		Elements::meta_Domain_parent = Uml::CompositionParentRole::Create(comp);
		Elements::meta_Domain_parent.name() = "Domain_parent";
		Elements::meta_Domain_parent.isNavigable() = true;
		Elements::meta_Domain_parent.target() = Domain::meta;
		Domain::meta_Elements_children = Uml::CompositionChildRole::Create(comp);
		Domain::meta_Elements_children.name() = "Elements";
		Domain::meta_Elements_children.min() = 0;
		Domain::meta_Elements_children.max() = -1;
		Domain::meta_Elements_children.isNavigable() = true;
		Domain::meta_Elements_children.target() = Elements::meta;
	}
	void InitComposition34()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		Resource::meta_Node_parent = Uml::CompositionParentRole::Create(comp);
		Resource::meta_Node_parent.name() = "Node_parent";
		Resource::meta_Node_parent.isNavigable() = true;
		Resource::meta_Node_parent.target() = Node::meta;
		Node::meta_Resource_children = Uml::CompositionChildRole::Create(comp);
		Node::meta_Resource_children.name() = "Resource";
		Node::meta_Resource_children.min() = 0;
		Node::meta_Resource_children.max() = -1;
		Node::meta_Resource_children.isNavigable() = true;
		Node::meta_Resource_children.target() = Resource::meta;
	}
	void InitComposition35()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		SatisfierProperty::meta_Resource_parent = Uml::CompositionParentRole::Create(comp);
		SatisfierProperty::meta_Resource_parent.name() = "Resource_parent";
		SatisfierProperty::meta_Resource_parent.isNavigable() = true;
		SatisfierProperty::meta_Resource_parent.target() = Resource::meta;
		Resource::meta_SatisfierProperty_children = Uml::CompositionChildRole::Create(comp);
		Resource::meta_SatisfierProperty_children.name() = "SatisfierProperty";
		Resource::meta_SatisfierProperty_children.min() = 0;
		Resource::meta_SatisfierProperty_children.max() = -1;
		Resource::meta_SatisfierProperty_children.isNavigable() = true;
		Resource::meta_SatisfierProperty_children.target() = SatisfierProperty::meta;
	}
	void InitComposition36()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		Resource::meta_Bridge_parent = Uml::CompositionParentRole::Create(comp);
		Resource::meta_Bridge_parent.name() = "Bridge_parent";
		Resource::meta_Bridge_parent.isNavigable() = true;
		Resource::meta_Bridge_parent.target() = Bridge::meta;
		Bridge::meta_Resource_children = Uml::CompositionChildRole::Create(comp);
		Bridge::meta_Resource_children.name() = "Resource";
		Bridge::meta_Resource_children.min() = 0;
		Bridge::meta_Resource_children.max() = -1;
		Bridge::meta_Resource_children.isNavigable() = true;
		Bridge::meta_Resource_children.target() = Resource::meta;
	}
	void InitComposition37()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		SatisfierProperty::meta_SharedResource_parent = Uml::CompositionParentRole::Create(comp);
		SatisfierProperty::meta_SharedResource_parent.name() = "SharedResource_parent";
		SatisfierProperty::meta_SharedResource_parent.isNavigable() = true;
		SatisfierProperty::meta_SharedResource_parent.target() = SharedResource::meta;
		SharedResource::meta_SatisfierProperty_children = Uml::CompositionChildRole::Create(comp);
		SharedResource::meta_SatisfierProperty_children.name() = "SatisfierProperty";
		SharedResource::meta_SatisfierProperty_children.min() = 0;
		SharedResource::meta_SatisfierProperty_children.max() = -1;
		SharedResource::meta_SatisfierProperty_children.isNavigable() = true;
		SharedResource::meta_SatisfierProperty_children.target() = SatisfierProperty::meta;
	}
	void InitComposition38()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		InstanceMapping::meta_DeploymentPlan_parent = Uml::CompositionParentRole::Create(comp);
		InstanceMapping::meta_DeploymentPlan_parent.name() = "DeploymentPlan_parent";
		InstanceMapping::meta_DeploymentPlan_parent.isNavigable() = true;
		InstanceMapping::meta_DeploymentPlan_parent.target() = DeploymentPlan::meta;
		DeploymentPlan::meta_InstanceMapping_children = Uml::CompositionChildRole::Create(comp);
		DeploymentPlan::meta_InstanceMapping_children.name() = "InstanceMapping";
		DeploymentPlan::meta_InstanceMapping_children.min() = 0;
		DeploymentPlan::meta_InstanceMapping_children.max() = -1;
		DeploymentPlan::meta_InstanceMapping_children.isNavigable() = true;
		DeploymentPlan::meta_InstanceMapping_children.target() = InstanceMapping::meta;
	}
	void InitComposition39()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		DeploymentPlan::meta_DeploymentPlans_parent = Uml::CompositionParentRole::Create(comp);
		DeploymentPlan::meta_DeploymentPlans_parent.name() = "DeploymentPlans_parent";
		DeploymentPlan::meta_DeploymentPlans_parent.isNavigable() = true;
		DeploymentPlan::meta_DeploymentPlans_parent.target() = DeploymentPlans::meta;
		DeploymentPlans::meta_DeploymentPlan_children = Uml::CompositionChildRole::Create(comp);
		DeploymentPlans::meta_DeploymentPlan_children.name() = "DeploymentPlan";
		DeploymentPlans::meta_DeploymentPlan_children.min() = 0;
		DeploymentPlans::meta_DeploymentPlan_children.max() = -1;
		DeploymentPlans::meta_DeploymentPlan_children.isNavigable() = true;
		DeploymentPlans::meta_DeploymentPlan_children.target() = DeploymentPlan::meta;
	}
	void InitComposition40()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		DeploymentPlans::meta_RootFolder_parent = Uml::CompositionParentRole::Create(comp);
		DeploymentPlans::meta_RootFolder_parent.name() = "RootFolder_parent";
		DeploymentPlans::meta_RootFolder_parent.isNavigable() = true;
		DeploymentPlans::meta_RootFolder_parent.target() = RootFolder::meta;
		RootFolder::meta_DeploymentPlans_children = Uml::CompositionChildRole::Create(comp);
		RootFolder::meta_DeploymentPlans_children.name() = "DeploymentPlans";
		RootFolder::meta_DeploymentPlans_children.min() = 0;
		RootFolder::meta_DeploymentPlans_children.max() = -1;
		RootFolder::meta_DeploymentPlans_children.isNavigable() = true;
		RootFolder::meta_DeploymentPlans_children.target() = DeploymentPlans::meta;
	}
	void InitComposition41()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		CollocationGroup::meta_DeploymentPlan_parent = Uml::CompositionParentRole::Create(comp);
		CollocationGroup::meta_DeploymentPlan_parent.name() = "DeploymentPlan_parent";
		CollocationGroup::meta_DeploymentPlan_parent.isNavigable() = true;
		CollocationGroup::meta_DeploymentPlan_parent.target() = DeploymentPlan::meta;
		DeploymentPlan::meta_CollocationGroup_children = Uml::CompositionChildRole::Create(comp);
		DeploymentPlan::meta_CollocationGroup_children.name() = "CollocationGroup";
		DeploymentPlan::meta_CollocationGroup_children.min() = 0;
		DeploymentPlan::meta_CollocationGroup_children.max() = -1;
		DeploymentPlan::meta_CollocationGroup_children.isNavigable() = true;
		DeploymentPlan::meta_CollocationGroup_children.target() = CollocationGroup::meta;
	}
	void InitComposition42()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		ComponentRef::meta_DeploymentPlan_parent = Uml::CompositionParentRole::Create(comp);
		ComponentRef::meta_DeploymentPlan_parent.name() = "DeploymentPlan_parent";
		ComponentRef::meta_DeploymentPlan_parent.isNavigable() = true;
		ComponentRef::meta_DeploymentPlan_parent.target() = DeploymentPlan::meta;
		DeploymentPlan::meta_ComponentRef_children = Uml::CompositionChildRole::Create(comp);
		DeploymentPlan::meta_ComponentRef_children.name() = "ComponentRef";
		DeploymentPlan::meta_ComponentRef_children.min() = 0;
		DeploymentPlan::meta_ComponentRef_children.max() = -1;
		DeploymentPlan::meta_ComponentRef_children.isNavigable() = true;
		DeploymentPlan::meta_ComponentRef_children.target() = ComponentRef::meta;
	}
	void InitComposition43()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		invoke::meta_ComponentAssembly_parent = Uml::CompositionParentRole::Create(comp);
		invoke::meta_ComponentAssembly_parent.name() = "ComponentAssembly_parent";
		invoke::meta_ComponentAssembly_parent.isNavigable() = true;
		invoke::meta_ComponentAssembly_parent.target() = ComponentAssembly::meta;
		ComponentAssembly::meta_invoke_children = Uml::CompositionChildRole::Create(comp);
		ComponentAssembly::meta_invoke_children.name() = "invoke";
		ComponentAssembly::meta_invoke_children.min() = 0;
		ComponentAssembly::meta_invoke_children.max() = -1;
		ComponentAssembly::meta_invoke_children.isNavigable() = true;
		ComponentAssembly::meta_invoke_children.target() = invoke::meta;
	}
	void InitComposition44()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		AssemblyselectRequirement::meta_ComponentAssembly_parent = Uml::CompositionParentRole::Create(comp);
		AssemblyselectRequirement::meta_ComponentAssembly_parent.name() = "ComponentAssembly_parent";
		AssemblyselectRequirement::meta_ComponentAssembly_parent.isNavigable() = true;
		AssemblyselectRequirement::meta_ComponentAssembly_parent.target() = ComponentAssembly::meta;
		ComponentAssembly::meta_AssemblyselectRequirement_children = Uml::CompositionChildRole::Create(comp);
		ComponentAssembly::meta_AssemblyselectRequirement_children.name() = "AssemblyselectRequirement";
		ComponentAssembly::meta_AssemblyselectRequirement_children.min() = 0;
		ComponentAssembly::meta_AssemblyselectRequirement_children.max() = -1;
		ComponentAssembly::meta_AssemblyselectRequirement_children.isNavigable() = true;
		ComponentAssembly::meta_AssemblyselectRequirement_children.target() = AssemblyselectRequirement::meta;
	}
	void InitComposition45()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		AssemblyConfigProperty::meta_ComponentAssembly_parent = Uml::CompositionParentRole::Create(comp);
		AssemblyConfigProperty::meta_ComponentAssembly_parent.name() = "ComponentAssembly_parent";
		AssemblyConfigProperty::meta_ComponentAssembly_parent.isNavigable() = true;
		AssemblyConfigProperty::meta_ComponentAssembly_parent.target() = ComponentAssembly::meta;
		ComponentAssembly::meta_AssemblyConfigProperty_children = Uml::CompositionChildRole::Create(comp);
		ComponentAssembly::meta_AssemblyConfigProperty_children.name() = "AssemblyConfigProperty";
		ComponentAssembly::meta_AssemblyConfigProperty_children.min() = 0;
		ComponentAssembly::meta_AssemblyConfigProperty_children.max() = -1;
		ComponentAssembly::meta_AssemblyConfigProperty_children.isNavigable() = true;
		ComponentAssembly::meta_AssemblyConfigProperty_children.target() = AssemblyConfigProperty::meta;
	}
	void InitComposition46()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		PublishConnector::meta_ComponentAssembly_parent = Uml::CompositionParentRole::Create(comp);
		PublishConnector::meta_ComponentAssembly_parent.name() = "ComponentAssembly_parent";
		PublishConnector::meta_ComponentAssembly_parent.isNavigable() = true;
		PublishConnector::meta_ComponentAssembly_parent.target() = ComponentAssembly::meta;
		ComponentAssembly::meta_PublishConnector_children = Uml::CompositionChildRole::Create(comp);
		ComponentAssembly::meta_PublishConnector_children.name() = "PublishConnector";
		ComponentAssembly::meta_PublishConnector_children.min() = 0;
		ComponentAssembly::meta_PublishConnector_children.max() = -1;
		ComponentAssembly::meta_PublishConnector_children.isNavigable() = true;
		ComponentAssembly::meta_PublishConnector_children.target() = PublishConnector::meta;
	}
	void InitComposition47()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		publish::meta_ComponentAssembly_parent = Uml::CompositionParentRole::Create(comp);
		publish::meta_ComponentAssembly_parent.name() = "ComponentAssembly_parent";
		publish::meta_ComponentAssembly_parent.isNavigable() = true;
		publish::meta_ComponentAssembly_parent.target() = ComponentAssembly::meta;
		ComponentAssembly::meta_publish_children = Uml::CompositionChildRole::Create(comp);
		ComponentAssembly::meta_publish_children.name() = "publish";
		ComponentAssembly::meta_publish_children.min() = 0;
		ComponentAssembly::meta_publish_children.max() = -1;
		ComponentAssembly::meta_publish_children.isNavigable() = true;
		ComponentAssembly::meta_publish_children.target() = publish::meta;
	}
	void InitComposition48()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		deliverTo::meta_ComponentAssembly_parent = Uml::CompositionParentRole::Create(comp);
		deliverTo::meta_ComponentAssembly_parent.name() = "ComponentAssembly_parent";
		deliverTo::meta_ComponentAssembly_parent.isNavigable() = true;
		deliverTo::meta_ComponentAssembly_parent.target() = ComponentAssembly::meta;
		ComponentAssembly::meta_deliverTo_children = Uml::CompositionChildRole::Create(comp);
		ComponentAssembly::meta_deliverTo_children.name() = "deliverTo";
		ComponentAssembly::meta_deliverTo_children.min() = 0;
		ComponentAssembly::meta_deliverTo_children.max() = -1;
		ComponentAssembly::meta_deliverTo_children.isNavigable() = true;
		ComponentAssembly::meta_deliverTo_children.target() = deliverTo::meta;
	}
	void InitComposition49()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		AssemblyDeployRequirement::meta_ComponentAssembly_parent = Uml::CompositionParentRole::Create(comp);
		AssemblyDeployRequirement::meta_ComponentAssembly_parent.name() = "ComponentAssembly_parent";
		AssemblyDeployRequirement::meta_ComponentAssembly_parent.isNavigable() = true;
		AssemblyDeployRequirement::meta_ComponentAssembly_parent.target() = ComponentAssembly::meta;
		ComponentAssembly::meta_AssemblyDeployRequirement_children = Uml::CompositionChildRole::Create(comp);
		ComponentAssembly::meta_AssemblyDeployRequirement_children.name() = "AssemblyDeployRequirement";
		ComponentAssembly::meta_AssemblyDeployRequirement_children.min() = 0;
		ComponentAssembly::meta_AssemblyDeployRequirement_children.max() = -1;
		ComponentAssembly::meta_AssemblyDeployRequirement_children.isNavigable() = true;
		ComponentAssembly::meta_AssemblyDeployRequirement_children.target() = AssemblyDeployRequirement::meta;
	}
	void InitComposition50()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		emit::meta_ComponentAssembly_parent = Uml::CompositionParentRole::Create(comp);
		emit::meta_ComponentAssembly_parent.name() = "ComponentAssembly_parent";
		emit::meta_ComponentAssembly_parent.isNavigable() = true;
		emit::meta_ComponentAssembly_parent.target() = ComponentAssembly::meta;
		ComponentAssembly::meta_emit_children = Uml::CompositionChildRole::Create(comp);
		ComponentAssembly::meta_emit_children.name() = "emit";
		ComponentAssembly::meta_emit_children.min() = 0;
		ComponentAssembly::meta_emit_children.max() = -1;
		ComponentAssembly::meta_emit_children.isNavigable() = true;
		ComponentAssembly::meta_emit_children.target() = emit::meta;
	}
	void InitComposition51()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		ReceptacleDelegate::meta_ComponentAssembly_parent = Uml::CompositionParentRole::Create(comp);
		ReceptacleDelegate::meta_ComponentAssembly_parent.name() = "ComponentAssembly_parent";
		ReceptacleDelegate::meta_ComponentAssembly_parent.isNavigable() = true;
		ReceptacleDelegate::meta_ComponentAssembly_parent.target() = ComponentAssembly::meta;
		ComponentAssembly::meta_ReceptacleDelegate_children = Uml::CompositionChildRole::Create(comp);
		ComponentAssembly::meta_ReceptacleDelegate_children.name() = "ReceptacleDelegate";
		ComponentAssembly::meta_ReceptacleDelegate_children.min() = 0;
		ComponentAssembly::meta_ReceptacleDelegate_children.max() = -1;
		ComponentAssembly::meta_ReceptacleDelegate_children.isNavigable() = true;
		ComponentAssembly::meta_ReceptacleDelegate_children.target() = ReceptacleDelegate::meta;
	}
	void InitComposition52()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		FacetDelegate::meta_ComponentAssembly_parent = Uml::CompositionParentRole::Create(comp);
		FacetDelegate::meta_ComponentAssembly_parent.name() = "ComponentAssembly_parent";
		FacetDelegate::meta_ComponentAssembly_parent.isNavigable() = true;
		FacetDelegate::meta_ComponentAssembly_parent.target() = ComponentAssembly::meta;
		ComponentAssembly::meta_FacetDelegate_children = Uml::CompositionChildRole::Create(comp);
		ComponentAssembly::meta_FacetDelegate_children.name() = "FacetDelegate";
		ComponentAssembly::meta_FacetDelegate_children.min() = 0;
		ComponentAssembly::meta_FacetDelegate_children.max() = -1;
		ComponentAssembly::meta_FacetDelegate_children.isNavigable() = true;
		ComponentAssembly::meta_FacetDelegate_children.target() = FacetDelegate::meta;
	}
	void InitComposition53()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		EventSourceDelegate::meta_ComponentAssembly_parent = Uml::CompositionParentRole::Create(comp);
		EventSourceDelegate::meta_ComponentAssembly_parent.name() = "ComponentAssembly_parent";
		EventSourceDelegate::meta_ComponentAssembly_parent.isNavigable() = true;
		EventSourceDelegate::meta_ComponentAssembly_parent.target() = ComponentAssembly::meta;
		ComponentAssembly::meta_EventSourceDelegate_children = Uml::CompositionChildRole::Create(comp);
		ComponentAssembly::meta_EventSourceDelegate_children.name() = "EventSourceDelegate";
		ComponentAssembly::meta_EventSourceDelegate_children.min() = 0;
		ComponentAssembly::meta_EventSourceDelegate_children.max() = -1;
		ComponentAssembly::meta_EventSourceDelegate_children.isNavigable() = true;
		ComponentAssembly::meta_EventSourceDelegate_children.target() = EventSourceDelegate::meta;
	}
	void InitComposition54()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		EventSinkDelegate::meta_ComponentAssembly_parent = Uml::CompositionParentRole::Create(comp);
		EventSinkDelegate::meta_ComponentAssembly_parent.name() = "ComponentAssembly_parent";
		EventSinkDelegate::meta_ComponentAssembly_parent.isNavigable() = true;
		EventSinkDelegate::meta_ComponentAssembly_parent.target() = ComponentAssembly::meta;
		ComponentAssembly::meta_EventSinkDelegate_children = Uml::CompositionChildRole::Create(comp);
		ComponentAssembly::meta_EventSinkDelegate_children.name() = "EventSinkDelegate";
		ComponentAssembly::meta_EventSinkDelegate_children.min() = 0;
		ComponentAssembly::meta_EventSinkDelegate_children.max() = -1;
		ComponentAssembly::meta_EventSinkDelegate_children.isNavigable() = true;
		ComponentAssembly::meta_EventSinkDelegate_children.target() = EventSinkDelegate::meta;
	}
	void InitComposition55()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		ExternalDelegate::meta_ComponentAssembly_parent = Uml::CompositionParentRole::Create(comp);
		ExternalDelegate::meta_ComponentAssembly_parent.name() = "ComponentAssembly_parent";
		ExternalDelegate::meta_ComponentAssembly_parent.isNavigable() = true;
		ExternalDelegate::meta_ComponentAssembly_parent.target() = ComponentAssembly::meta;
		ComponentAssembly::meta_ExternalDelegate_children = Uml::CompositionChildRole::Create(comp);
		ComponentAssembly::meta_ExternalDelegate_children.name() = "ExternalDelegate";
		ComponentAssembly::meta_ExternalDelegate_children.min() = 0;
		ComponentAssembly::meta_ExternalDelegate_children.max() = -1;
		ComponentAssembly::meta_ExternalDelegate_children.isNavigable() = true;
		ComponentAssembly::meta_ExternalDelegate_children.target() = ExternalDelegate::meta;
	}
	void InitComposition56()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		ComponentPackage::meta_ComponentAssembly_parent = Uml::CompositionParentRole::Create(comp);
		ComponentPackage::meta_ComponentAssembly_parent.name() = "ComponentAssembly_parent";
		ComponentPackage::meta_ComponentAssembly_parent.isNavigable() = true;
		ComponentPackage::meta_ComponentAssembly_parent.target() = ComponentAssembly::meta;
		ComponentAssembly::meta_ComponentPackage_children = Uml::CompositionChildRole::Create(comp);
		ComponentAssembly::meta_ComponentPackage_children.name() = "ComponentPackage";
		ComponentAssembly::meta_ComponentPackage_children.min() = 0;
		ComponentAssembly::meta_ComponentPackage_children.max() = -1;
		ComponentAssembly::meta_ComponentPackage_children.isNavigable() = true;
		ComponentAssembly::meta_ComponentPackage_children.target() = ComponentPackage::meta;
	}
	void InitComposition57()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		Property::meta_ComponentAssembly_parent = Uml::CompositionParentRole::Create(comp);
		Property::meta_ComponentAssembly_parent.name() = "ComponentAssembly_parent";
		Property::meta_ComponentAssembly_parent.isNavigable() = true;
		Property::meta_ComponentAssembly_parent.target() = ComponentAssembly::meta;
		ComponentAssembly::meta_Property_children = Uml::CompositionChildRole::Create(comp);
		ComponentAssembly::meta_Property_children.name() = "Property";
		ComponentAssembly::meta_Property_children.min() = 0;
		ComponentAssembly::meta_Property_children.max() = -1;
		ComponentAssembly::meta_Property_children.isNavigable() = true;
		ComponentAssembly::meta_Property_children.target() = Property::meta;
	}
	void InitComposition58()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		ComponentPackageReference::meta_ComponentAssembly_parent = Uml::CompositionParentRole::Create(comp);
		ComponentPackageReference::meta_ComponentAssembly_parent.name() = "ComponentAssembly_parent";
		ComponentPackageReference::meta_ComponentAssembly_parent.isNavigable() = true;
		ComponentPackageReference::meta_ComponentAssembly_parent.target() = ComponentAssembly::meta;
		ComponentAssembly::meta_ComponentPackageReference_children = Uml::CompositionChildRole::Create(comp);
		ComponentAssembly::meta_ComponentPackageReference_children.name() = "ComponentPackageReference";
		ComponentAssembly::meta_ComponentPackageReference_children.min() = 0;
		ComponentAssembly::meta_ComponentPackageReference_children.max() = -1;
		ComponentAssembly::meta_ComponentPackageReference_children.isNavigable() = true;
		ComponentAssembly::meta_ComponentPackageReference_children.target() = ComponentPackageReference::meta;
	}
	void InitComposition59()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		Requirement::meta_ComponentAssembly_parent = Uml::CompositionParentRole::Create(comp);
		Requirement::meta_ComponentAssembly_parent.name() = "ComponentAssembly_parent";
		Requirement::meta_ComponentAssembly_parent.isNavigable() = true;
		Requirement::meta_ComponentAssembly_parent.target() = ComponentAssembly::meta;
		ComponentAssembly::meta_Requirement_children = Uml::CompositionChildRole::Create(comp);
		ComponentAssembly::meta_Requirement_children.name() = "Requirement";
		ComponentAssembly::meta_Requirement_children.min() = 0;
		ComponentAssembly::meta_Requirement_children.max() = -1;
		ComponentAssembly::meta_Requirement_children.isNavigable() = true;
		ComponentAssembly::meta_Requirement_children.target() = Requirement::meta;
	}
	void InitComposition60()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		Component::meta_ComponentAssembly_parent = Uml::CompositionParentRole::Create(comp);
		Component::meta_ComponentAssembly_parent.name() = "ComponentAssembly_parent";
		Component::meta_ComponentAssembly_parent.isNavigable() = true;
		Component::meta_ComponentAssembly_parent.target() = ComponentAssembly::meta;
		ComponentAssembly::meta_Component_children = Uml::CompositionChildRole::Create(comp);
		ComponentAssembly::meta_Component_children.name() = "Component";
		ComponentAssembly::meta_Component_children.min() = 1;
		ComponentAssembly::meta_Component_children.max() = -1;
		ComponentAssembly::meta_Component_children.isNavigable() = true;
		ComponentAssembly::meta_Component_children.target() = Component::meta;
	}
	void InitComposition61()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		ComponentAssembly::meta_ComponentAssembly_parent = Uml::CompositionParentRole::Create(comp);
		ComponentAssembly::meta_ComponentAssembly_parent.name() = "ComponentAssembly_parent";
		ComponentAssembly::meta_ComponentAssembly_parent.isNavigable() = true;
		ComponentAssembly::meta_ComponentAssembly_parent.target() = ComponentAssembly::meta;
		ComponentAssembly::meta_ComponentAssembly_children = Uml::CompositionChildRole::Create(comp);
		ComponentAssembly::meta_ComponentAssembly_children.name() = "ComponentAssembly";
		ComponentAssembly::meta_ComponentAssembly_children.min() = 0;
		ComponentAssembly::meta_ComponentAssembly_children.max() = -1;
		ComponentAssembly::meta_ComponentAssembly_children.isNavigable() = true;
		ComponentAssembly::meta_ComponentAssembly_children.target() = ComponentAssembly::meta;
	}
	void InitComposition62()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		ProvidedRequestPort::meta_ComponentAssembly_parent = Uml::CompositionParentRole::Create(comp);
		ProvidedRequestPort::meta_ComponentAssembly_parent.name() = "ComponentAssembly_parent";
		ProvidedRequestPort::meta_ComponentAssembly_parent.isNavigable() = true;
		ProvidedRequestPort::meta_ComponentAssembly_parent.target() = ComponentAssembly::meta;
		ComponentAssembly::meta_ProvidedRequestPort_children = Uml::CompositionChildRole::Create(comp);
		ComponentAssembly::meta_ProvidedRequestPort_children.name() = "ProvidedRequestPort";
		ComponentAssembly::meta_ProvidedRequestPort_children.min() = 0;
		ComponentAssembly::meta_ProvidedRequestPort_children.max() = -1;
		ComponentAssembly::meta_ProvidedRequestPort_children.isNavigable() = true;
		ComponentAssembly::meta_ProvidedRequestPort_children.target() = ProvidedRequestPort::meta;
	}
	void InitComposition63()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		RequiredRequestPort::meta_ComponentAssembly_parent = Uml::CompositionParentRole::Create(comp);
		RequiredRequestPort::meta_ComponentAssembly_parent.name() = "ComponentAssembly_parent";
		RequiredRequestPort::meta_ComponentAssembly_parent.isNavigable() = true;
		RequiredRequestPort::meta_ComponentAssembly_parent.target() = ComponentAssembly::meta;
		ComponentAssembly::meta_RequiredRequestPort_children = Uml::CompositionChildRole::Create(comp);
		ComponentAssembly::meta_RequiredRequestPort_children.name() = "RequiredRequestPort";
		ComponentAssembly::meta_RequiredRequestPort_children.min() = 0;
		ComponentAssembly::meta_RequiredRequestPort_children.max() = -1;
		ComponentAssembly::meta_RequiredRequestPort_children.isNavigable() = true;
		ComponentAssembly::meta_RequiredRequestPort_children.target() = RequiredRequestPort::meta;
	}
	void InitComposition64()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		InEventPort::meta_ComponentAssembly_parent = Uml::CompositionParentRole::Create(comp);
		InEventPort::meta_ComponentAssembly_parent.name() = "ComponentAssembly_parent";
		InEventPort::meta_ComponentAssembly_parent.isNavigable() = true;
		InEventPort::meta_ComponentAssembly_parent.target() = ComponentAssembly::meta;
		ComponentAssembly::meta_InEventPort_children = Uml::CompositionChildRole::Create(comp);
		ComponentAssembly::meta_InEventPort_children.name() = "InEventPort";
		ComponentAssembly::meta_InEventPort_children.min() = 0;
		ComponentAssembly::meta_InEventPort_children.max() = -1;
		ComponentAssembly::meta_InEventPort_children.isNavigable() = true;
		ComponentAssembly::meta_InEventPort_children.target() = InEventPort::meta;
	}
	void InitComposition65()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		OutEventPort::meta_ComponentAssembly_parent = Uml::CompositionParentRole::Create(comp);
		OutEventPort::meta_ComponentAssembly_parent.name() = "ComponentAssembly_parent";
		OutEventPort::meta_ComponentAssembly_parent.isNavigable() = true;
		OutEventPort::meta_ComponentAssembly_parent.target() = ComponentAssembly::meta;
		ComponentAssembly::meta_OutEventPort_children = Uml::CompositionChildRole::Create(comp);
		ComponentAssembly::meta_OutEventPort_children.name() = "OutEventPort";
		ComponentAssembly::meta_OutEventPort_children.min() = 0;
		ComponentAssembly::meta_OutEventPort_children.max() = -1;
		ComponentAssembly::meta_OutEventPort_children.isNavigable() = true;
		ComponentAssembly::meta_OutEventPort_children.target() = OutEventPort::meta;
	}
	void InitComposition66()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		ExternalReferenceEndPoint::meta_ComponentAssembly_parent = Uml::CompositionParentRole::Create(comp);
		ExternalReferenceEndPoint::meta_ComponentAssembly_parent.name() = "ComponentAssembly_parent";
		ExternalReferenceEndPoint::meta_ComponentAssembly_parent.isNavigable() = true;
		ExternalReferenceEndPoint::meta_ComponentAssembly_parent.target() = ComponentAssembly::meta;
		ComponentAssembly::meta_ExternalReferenceEndPoint_children = Uml::CompositionChildRole::Create(comp);
		ComponentAssembly::meta_ExternalReferenceEndPoint_children.name() = "ExternalReferenceEndPoint";
		ComponentAssembly::meta_ExternalReferenceEndPoint_children.min() = 0;
		ComponentAssembly::meta_ExternalReferenceEndPoint_children.max() = -1;
		ComponentAssembly::meta_ExternalReferenceEndPoint_children.isNavigable() = true;
		ComponentAssembly::meta_ExternalReferenceEndPoint_children.target() = ExternalReferenceEndPoint::meta;
	}
	void InitComposition67()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		ComponentImplementation::meta_ComponentImplementationContainer_parent = Uml::CompositionParentRole::Create(comp);
		ComponentImplementation::meta_ComponentImplementationContainer_parent.name() = "ComponentImplementationContainer_parent";
		ComponentImplementation::meta_ComponentImplementationContainer_parent.isNavigable() = true;
		ComponentImplementation::meta_ComponentImplementationContainer_parent.target() = ComponentImplementationContainer::meta;
		ComponentImplementationContainer::meta_ComponentImplementation_child = Uml::CompositionChildRole::Create(comp);
		ComponentImplementationContainer::meta_ComponentImplementation_child.name() = "ComponentImplementation";
		ComponentImplementationContainer::meta_ComponentImplementation_child.min() = 1;
		ComponentImplementationContainer::meta_ComponentImplementation_child.max() = 1;
		ComponentImplementationContainer::meta_ComponentImplementation_child.isNavigable() = true;
		ComponentImplementationContainer::meta_ComponentImplementation_child.target() = ComponentImplementation::meta;
	}
	void InitComposition68()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		ImplementationCapability::meta_ComponentImplementationContainer_parent = Uml::CompositionParentRole::Create(comp);
		ImplementationCapability::meta_ComponentImplementationContainer_parent.name() = "ComponentImplementationContainer_parent";
		ImplementationCapability::meta_ComponentImplementationContainer_parent.isNavigable() = true;
		ImplementationCapability::meta_ComponentImplementationContainer_parent.target() = ComponentImplementationContainer::meta;
		ComponentImplementationContainer::meta_ImplementationCapability_children = Uml::CompositionChildRole::Create(comp);
		ComponentImplementationContainer::meta_ImplementationCapability_children.name() = "ImplementationCapability";
		ComponentImplementationContainer::meta_ImplementationCapability_children.min() = 0;
		ComponentImplementationContainer::meta_ImplementationCapability_children.max() = -1;
		ComponentImplementationContainer::meta_ImplementationCapability_children.isNavigable() = true;
		ComponentImplementationContainer::meta_ImplementationCapability_children.target() = ImplementationCapability::meta;
	}
	void InitComposition69()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		ImplementationDependsOn::meta_ComponentImplementationContainer_parent = Uml::CompositionParentRole::Create(comp);
		ImplementationDependsOn::meta_ComponentImplementationContainer_parent.name() = "ComponentImplementationContainer_parent";
		ImplementationDependsOn::meta_ComponentImplementationContainer_parent.isNavigable() = true;
		ImplementationDependsOn::meta_ComponentImplementationContainer_parent.target() = ComponentImplementationContainer::meta;
		ComponentImplementationContainer::meta_ImplementationDependsOn_children = Uml::CompositionChildRole::Create(comp);
		ComponentImplementationContainer::meta_ImplementationDependsOn_children.name() = "ImplementationDependsOn";
		ComponentImplementationContainer::meta_ImplementationDependsOn_children.min() = 0;
		ComponentImplementationContainer::meta_ImplementationDependsOn_children.max() = -1;
		ComponentImplementationContainer::meta_ImplementationDependsOn_children.isNavigable() = true;
		ComponentImplementationContainer::meta_ImplementationDependsOn_children.target() = ImplementationDependsOn::meta;
	}
	void InitComposition70()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		Implements::meta_ComponentImplementationContainer_parent = Uml::CompositionParentRole::Create(comp);
		Implements::meta_ComponentImplementationContainer_parent.name() = "ComponentImplementationContainer_parent";
		Implements::meta_ComponentImplementationContainer_parent.isNavigable() = true;
		Implements::meta_ComponentImplementationContainer_parent.target() = ComponentImplementationContainer::meta;
		ComponentImplementationContainer::meta_Implements_children = Uml::CompositionChildRole::Create(comp);
		ComponentImplementationContainer::meta_Implements_children.name() = "Implements";
		ComponentImplementationContainer::meta_Implements_children.min() = 0;
		ComponentImplementationContainer::meta_Implements_children.max() = -1;
		ComponentImplementationContainer::meta_Implements_children.isNavigable() = true;
		ComponentImplementationContainer::meta_Implements_children.target() = Implements::meta;
	}
	void InitComposition71()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		ComponentImplementationContainer::meta_ComponentImplementations_parent = Uml::CompositionParentRole::Create(comp);
		ComponentImplementationContainer::meta_ComponentImplementations_parent.name() = "ComponentImplementations_parent";
		ComponentImplementationContainer::meta_ComponentImplementations_parent.isNavigable() = true;
		ComponentImplementationContainer::meta_ComponentImplementations_parent.target() = ComponentImplementations::meta;
		ComponentImplementations::meta_ComponentImplementationContainer_children = Uml::CompositionChildRole::Create(comp);
		ComponentImplementations::meta_ComponentImplementationContainer_children.name() = "ComponentImplementationContainer";
		ComponentImplementations::meta_ComponentImplementationContainer_children.min() = 0;
		ComponentImplementations::meta_ComponentImplementationContainer_children.max() = -1;
		ComponentImplementations::meta_ComponentImplementationContainer_children.isNavigable() = true;
		ComponentImplementations::meta_ComponentImplementationContainer_children.target() = ComponentImplementationContainer::meta;
	}
	void InitComposition72()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		ComponentImplementations::meta_RootFolder_parent = Uml::CompositionParentRole::Create(comp);
		ComponentImplementations::meta_RootFolder_parent.name() = "RootFolder_parent";
		ComponentImplementations::meta_RootFolder_parent.isNavigable() = true;
		ComponentImplementations::meta_RootFolder_parent.target() = RootFolder::meta;
		RootFolder::meta_ComponentImplementations_children = Uml::CompositionChildRole::Create(comp);
		RootFolder::meta_ComponentImplementations_children.name() = "ComponentImplementations";
		RootFolder::meta_ComponentImplementations_children.min() = 0;
		RootFolder::meta_ComponentImplementations_children.max() = -1;
		RootFolder::meta_ComponentImplementations_children.isNavigable() = true;
		RootFolder::meta_ComponentImplementations_children.target() = ComponentImplementations::meta;
	}
	void InitComposition73()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		ConfigProperty::meta_ComponentImplementationContainer_parent = Uml::CompositionParentRole::Create(comp);
		ConfigProperty::meta_ComponentImplementationContainer_parent.name() = "ComponentImplementationContainer_parent";
		ConfigProperty::meta_ComponentImplementationContainer_parent.isNavigable() = true;
		ConfigProperty::meta_ComponentImplementationContainer_parent.target() = ComponentImplementationContainer::meta;
		ComponentImplementationContainer::meta_ConfigProperty_children = Uml::CompositionChildRole::Create(comp);
		ComponentImplementationContainer::meta_ConfigProperty_children.name() = "ConfigProperty";
		ComponentImplementationContainer::meta_ConfigProperty_children.min() = 0;
		ComponentImplementationContainer::meta_ConfigProperty_children.max() = -1;
		ComponentImplementationContainer::meta_ConfigProperty_children.isNavigable() = true;
		ComponentImplementationContainer::meta_ConfigProperty_children.target() = ConfigProperty::meta;
	}
	void InitComposition74()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		InfoProperty::meta_ComponentImplementationContainer_parent = Uml::CompositionParentRole::Create(comp);
		InfoProperty::meta_ComponentImplementationContainer_parent.name() = "ComponentImplementationContainer_parent";
		InfoProperty::meta_ComponentImplementationContainer_parent.isNavigable() = true;
		InfoProperty::meta_ComponentImplementationContainer_parent.target() = ComponentImplementationContainer::meta;
		ComponentImplementationContainer::meta_InfoProperty_children = Uml::CompositionChildRole::Create(comp);
		ComponentImplementationContainer::meta_InfoProperty_children.name() = "InfoProperty";
		ComponentImplementationContainer::meta_InfoProperty_children.min() = 0;
		ComponentImplementationContainer::meta_InfoProperty_children.max() = -1;
		ComponentImplementationContainer::meta_InfoProperty_children.isNavigable() = true;
		ComponentImplementationContainer::meta_InfoProperty_children.target() = InfoProperty::meta;
	}
	void InitComposition75()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		MonolithprimaryArtifact::meta_ComponentImplementationContainer_parent = Uml::CompositionParentRole::Create(comp);
		MonolithprimaryArtifact::meta_ComponentImplementationContainer_parent.name() = "ComponentImplementationContainer_parent";
		MonolithprimaryArtifact::meta_ComponentImplementationContainer_parent.isNavigable() = true;
		MonolithprimaryArtifact::meta_ComponentImplementationContainer_parent.target() = ComponentImplementationContainer::meta;
		ComponentImplementationContainer::meta_MonolithprimaryArtifact_children = Uml::CompositionChildRole::Create(comp);
		ComponentImplementationContainer::meta_MonolithprimaryArtifact_children.name() = "MonolithprimaryArtifact";
		ComponentImplementationContainer::meta_MonolithprimaryArtifact_children.min() = 0;
		ComponentImplementationContainer::meta_MonolithprimaryArtifact_children.max() = -1;
		ComponentImplementationContainer::meta_MonolithprimaryArtifact_children.isNavigable() = true;
		ComponentImplementationContainer::meta_MonolithprimaryArtifact_children.target() = MonolithprimaryArtifact::meta;
	}
	void InitComposition76()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		MonolithDeployRequirement::meta_ComponentImplementationContainer_parent = Uml::CompositionParentRole::Create(comp);
		MonolithDeployRequirement::meta_ComponentImplementationContainer_parent.name() = "ComponentImplementationContainer_parent";
		MonolithDeployRequirement::meta_ComponentImplementationContainer_parent.isNavigable() = true;
		MonolithDeployRequirement::meta_ComponentImplementationContainer_parent.target() = ComponentImplementationContainer::meta;
		ComponentImplementationContainer::meta_MonolithDeployRequirement_children = Uml::CompositionChildRole::Create(comp);
		ComponentImplementationContainer::meta_MonolithDeployRequirement_children.name() = "MonolithDeployRequirement";
		ComponentImplementationContainer::meta_MonolithDeployRequirement_children.min() = 0;
		ComponentImplementationContainer::meta_MonolithDeployRequirement_children.max() = -1;
		ComponentImplementationContainer::meta_MonolithDeployRequirement_children.isNavigable() = true;
		ComponentImplementationContainer::meta_MonolithDeployRequirement_children.target() = MonolithDeployRequirement::meta;
	}
	void InitComposition77()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		ComponentRef::meta_ComponentImplementationContainer_parent = Uml::CompositionParentRole::Create(comp);
		ComponentRef::meta_ComponentImplementationContainer_parent.name() = "ComponentImplementationContainer_parent";
		ComponentRef::meta_ComponentImplementationContainer_parent.isNavigable() = true;
		ComponentRef::meta_ComponentImplementationContainer_parent.target() = ComponentImplementationContainer::meta;
		ComponentImplementationContainer::meta_ComponentRef_children = Uml::CompositionChildRole::Create(comp);
		ComponentImplementationContainer::meta_ComponentRef_children.name() = "ComponentRef";
		ComponentImplementationContainer::meta_ComponentRef_children.min() = 0;
		ComponentImplementationContainer::meta_ComponentRef_children.max() = -1;
		ComponentImplementationContainer::meta_ComponentRef_children.isNavigable() = true;
		ComponentImplementationContainer::meta_ComponentRef_children.target() = ComponentRef::meta;
	}
	void InitComposition78()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		MonolithExecParameter::meta_ComponentImplementationContainer_parent = Uml::CompositionParentRole::Create(comp);
		MonolithExecParameter::meta_ComponentImplementationContainer_parent.name() = "ComponentImplementationContainer_parent";
		MonolithExecParameter::meta_ComponentImplementationContainer_parent.isNavigable() = true;
		MonolithExecParameter::meta_ComponentImplementationContainer_parent.target() = ComponentImplementationContainer::meta;
		ComponentImplementationContainer::meta_MonolithExecParameter_children = Uml::CompositionChildRole::Create(comp);
		ComponentImplementationContainer::meta_MonolithExecParameter_children.name() = "MonolithExecParameter";
		ComponentImplementationContainer::meta_MonolithExecParameter_children.min() = 0;
		ComponentImplementationContainer::meta_MonolithExecParameter_children.max() = -1;
		ComponentImplementationContainer::meta_MonolithExecParameter_children.isNavigable() = true;
		ComponentImplementationContainer::meta_MonolithExecParameter_children.target() = MonolithExecParameter::meta;
	}
	void InitComposition79()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		ImplementationDependency::meta_ComponentImplementationContainer_parent = Uml::CompositionParentRole::Create(comp);
		ImplementationDependency::meta_ComponentImplementationContainer_parent.name() = "ComponentImplementationContainer_parent";
		ImplementationDependency::meta_ComponentImplementationContainer_parent.isNavigable() = true;
		ImplementationDependency::meta_ComponentImplementationContainer_parent.target() = ComponentImplementationContainer::meta;
		ComponentImplementationContainer::meta_ImplementationDependency_children = Uml::CompositionChildRole::Create(comp);
		ComponentImplementationContainer::meta_ImplementationDependency_children.name() = "ImplementationDependency";
		ComponentImplementationContainer::meta_ImplementationDependency_children.min() = 0;
		ComponentImplementationContainer::meta_ImplementationDependency_children.max() = -1;
		ComponentImplementationContainer::meta_ImplementationDependency_children.isNavigable() = true;
		ComponentImplementationContainer::meta_ImplementationDependency_children.target() = ImplementationDependency::meta;
	}
	void InitComposition80()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		Property::meta_ComponentImplementationContainer_parent = Uml::CompositionParentRole::Create(comp);
		Property::meta_ComponentImplementationContainer_parent.name() = "ComponentImplementationContainer_parent";
		Property::meta_ComponentImplementationContainer_parent.isNavigable() = true;
		Property::meta_ComponentImplementationContainer_parent.target() = ComponentImplementationContainer::meta;
		ComponentImplementationContainer::meta_Property_children = Uml::CompositionChildRole::Create(comp);
		ComponentImplementationContainer::meta_Property_children.name() = "Property";
		ComponentImplementationContainer::meta_Property_children.min() = 0;
		ComponentImplementationContainer::meta_Property_children.max() = -1;
		ComponentImplementationContainer::meta_Property_children.isNavigable() = true;
		ComponentImplementationContainer::meta_Property_children.target() = Property::meta;
	}
	void InitComposition81()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		Capability::meta_ComponentImplementationContainer_parent = Uml::CompositionParentRole::Create(comp);
		Capability::meta_ComponentImplementationContainer_parent.name() = "ComponentImplementationContainer_parent";
		Capability::meta_ComponentImplementationContainer_parent.isNavigable() = true;
		Capability::meta_ComponentImplementationContainer_parent.target() = ComponentImplementationContainer::meta;
		ComponentImplementationContainer::meta_Capability_children = Uml::CompositionChildRole::Create(comp);
		ComponentImplementationContainer::meta_Capability_children.name() = "Capability";
		ComponentImplementationContainer::meta_Capability_children.min() = 0;
		ComponentImplementationContainer::meta_Capability_children.max() = -1;
		ComponentImplementationContainer::meta_Capability_children.isNavigable() = true;
		ComponentImplementationContainer::meta_Capability_children.target() = Capability::meta;
	}
	void InitComposition82()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		ImplementationRequirement::meta_ComponentImplementationContainer_parent = Uml::CompositionParentRole::Create(comp);
		ImplementationRequirement::meta_ComponentImplementationContainer_parent.name() = "ComponentImplementationContainer_parent";
		ImplementationRequirement::meta_ComponentImplementationContainer_parent.isNavigable() = true;
		ImplementationRequirement::meta_ComponentImplementationContainer_parent.target() = ComponentImplementationContainer::meta;
		ComponentImplementationContainer::meta_ImplementationRequirement_children = Uml::CompositionChildRole::Create(comp);
		ComponentImplementationContainer::meta_ImplementationRequirement_children.name() = "ImplementationRequirement";
		ComponentImplementationContainer::meta_ImplementationRequirement_children.min() = 0;
		ComponentImplementationContainer::meta_ImplementationRequirement_children.max() = -1;
		ComponentImplementationContainer::meta_ImplementationRequirement_children.isNavigable() = true;
		ComponentImplementationContainer::meta_ImplementationRequirement_children.target() = ImplementationRequirement::meta;
	}
	void InitComposition83()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		ImplementationArtifactReference::meta_ComponentImplementationContainer_parent = Uml::CompositionParentRole::Create(comp);
		ImplementationArtifactReference::meta_ComponentImplementationContainer_parent.name() = "ComponentImplementationContainer_parent";
		ImplementationArtifactReference::meta_ComponentImplementationContainer_parent.isNavigable() = true;
		ImplementationArtifactReference::meta_ComponentImplementationContainer_parent.target() = ComponentImplementationContainer::meta;
		ComponentImplementationContainer::meta_ImplementationArtifactReference_children = Uml::CompositionChildRole::Create(comp);
		ComponentImplementationContainer::meta_ImplementationArtifactReference_children.name() = "ImplementationArtifactReference";
		ComponentImplementationContainer::meta_ImplementationArtifactReference_children.min() = 0;
		ComponentImplementationContainer::meta_ImplementationArtifactReference_children.max() = -1;
		ComponentImplementationContainer::meta_ImplementationArtifactReference_children.isNavigable() = true;
		ComponentImplementationContainer::meta_ImplementationArtifactReference_children.target() = ImplementationArtifactReference::meta;
	}
	void InitComposition84()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		ComponentImplementationReference::meta_PackageContainer_parent = Uml::CompositionParentRole::Create(comp);
		ComponentImplementationReference::meta_PackageContainer_parent.name() = "PackageContainer_parent";
		ComponentImplementationReference::meta_PackageContainer_parent.isNavigable() = true;
		ComponentImplementationReference::meta_PackageContainer_parent.target() = PackageContainer::meta;
		PackageContainer::meta_ComponentImplementationReference_children = Uml::CompositionChildRole::Create(comp);
		PackageContainer::meta_ComponentImplementationReference_children.name() = "ComponentImplementationReference";
		PackageContainer::meta_ComponentImplementationReference_children.min() = 0;
		PackageContainer::meta_ComponentImplementationReference_children.max() = -1;
		PackageContainer::meta_ComponentImplementationReference_children.isNavigable() = true;
		PackageContainer::meta_ComponentImplementationReference_children.target() = ComponentImplementationReference::meta;
	}
	void InitComposition85()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		PackageConfBasePackage::meta_PackageConfigurationContainer_parent = Uml::CompositionParentRole::Create(comp);
		PackageConfBasePackage::meta_PackageConfigurationContainer_parent.name() = "PackageConfigurationContainer_parent";
		PackageConfBasePackage::meta_PackageConfigurationContainer_parent.isNavigable() = true;
		PackageConfBasePackage::meta_PackageConfigurationContainer_parent.target() = PackageConfigurationContainer::meta;
		PackageConfigurationContainer::meta_PackageConfBasePackage_child = Uml::CompositionChildRole::Create(comp);
		PackageConfigurationContainer::meta_PackageConfBasePackage_child.name() = "PackageConfBasePackage";
		PackageConfigurationContainer::meta_PackageConfBasePackage_child.min() = 0;
		PackageConfigurationContainer::meta_PackageConfBasePackage_child.max() = 1;
		PackageConfigurationContainer::meta_PackageConfBasePackage_child.isNavigable() = true;
		PackageConfigurationContainer::meta_PackageConfBasePackage_child.target() = PackageConfBasePackage::meta;
	}
	void InitComposition86()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		PackageConfReference::meta_PackageConfigurationContainer_parent = Uml::CompositionParentRole::Create(comp);
		PackageConfReference::meta_PackageConfigurationContainer_parent.name() = "PackageConfigurationContainer_parent";
		PackageConfReference::meta_PackageConfigurationContainer_parent.isNavigable() = true;
		PackageConfReference::meta_PackageConfigurationContainer_parent.target() = PackageConfigurationContainer::meta;
		PackageConfigurationContainer::meta_PackageConfReference_child = Uml::CompositionChildRole::Create(comp);
		PackageConfigurationContainer::meta_PackageConfReference_child.name() = "PackageConfReference";
		PackageConfigurationContainer::meta_PackageConfReference_child.min() = 0;
		PackageConfigurationContainer::meta_PackageConfReference_child.max() = 1;
		PackageConfigurationContainer::meta_PackageConfReference_child.isNavigable() = true;
		PackageConfigurationContainer::meta_PackageConfReference_child.target() = PackageConfReference::meta;
	}
	void InitComposition87()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		PackageConfigurationContainer::meta_PackageConfigurations_parent = Uml::CompositionParentRole::Create(comp);
		PackageConfigurationContainer::meta_PackageConfigurations_parent.name() = "PackageConfigurations_parent";
		PackageConfigurationContainer::meta_PackageConfigurations_parent.isNavigable() = true;
		PackageConfigurationContainer::meta_PackageConfigurations_parent.target() = PackageConfigurations::meta;
		PackageConfigurations::meta_PackageConfigurationContainer_children = Uml::CompositionChildRole::Create(comp);
		PackageConfigurations::meta_PackageConfigurationContainer_children.name() = "PackageConfigurationContainer";
		PackageConfigurations::meta_PackageConfigurationContainer_children.min() = 0;
		PackageConfigurations::meta_PackageConfigurationContainer_children.max() = -1;
		PackageConfigurations::meta_PackageConfigurationContainer_children.isNavigable() = true;
		PackageConfigurations::meta_PackageConfigurationContainer_children.target() = PackageConfigurationContainer::meta;
	}
	void InitComposition88()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		PackageConfigurations::meta_RootFolder_parent = Uml::CompositionParentRole::Create(comp);
		PackageConfigurations::meta_RootFolder_parent.name() = "RootFolder_parent";
		PackageConfigurations::meta_RootFolder_parent.isNavigable() = true;
		PackageConfigurations::meta_RootFolder_parent.target() = RootFolder::meta;
		RootFolder::meta_PackageConfigurations_children = Uml::CompositionChildRole::Create(comp);
		RootFolder::meta_PackageConfigurations_children.name() = "PackageConfigurations";
		RootFolder::meta_PackageConfigurations_children.min() = 0;
		RootFolder::meta_PackageConfigurations_children.max() = -1;
		RootFolder::meta_PackageConfigurations_children.isNavigable() = true;
		RootFolder::meta_PackageConfigurations_children.target() = PackageConfigurations::meta;
	}
	void InitComposition89()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		ComponentPackage::meta_PackageConfigurationContainer_parent = Uml::CompositionParentRole::Create(comp);
		ComponentPackage::meta_PackageConfigurationContainer_parent.name() = "PackageConfigurationContainer_parent";
		ComponentPackage::meta_PackageConfigurationContainer_parent.isNavigable() = true;
		ComponentPackage::meta_PackageConfigurationContainer_parent.target() = PackageConfigurationContainer::meta;
		PackageConfigurationContainer::meta_ComponentPackage_child = Uml::CompositionChildRole::Create(comp);
		PackageConfigurationContainer::meta_ComponentPackage_child.name() = "ComponentPackage";
		PackageConfigurationContainer::meta_ComponentPackage_child.min() = 0;
		PackageConfigurationContainer::meta_ComponentPackage_child.max() = 1;
		PackageConfigurationContainer::meta_ComponentPackage_child.isNavigable() = true;
		PackageConfigurationContainer::meta_ComponentPackage_child.target() = ComponentPackage::meta;
	}
	void InitComposition90()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		ComponentPackageReference::meta_PackageConfigurationContainer_parent = Uml::CompositionParentRole::Create(comp);
		ComponentPackageReference::meta_PackageConfigurationContainer_parent.name() = "PackageConfigurationContainer_parent";
		ComponentPackageReference::meta_PackageConfigurationContainer_parent.isNavigable() = true;
		ComponentPackageReference::meta_PackageConfigurationContainer_parent.target() = PackageConfigurationContainer::meta;
		PackageConfigurationContainer::meta_ComponentPackageReference_child = Uml::CompositionChildRole::Create(comp);
		PackageConfigurationContainer::meta_ComponentPackageReference_child.name() = "ComponentPackageReference";
		PackageConfigurationContainer::meta_ComponentPackageReference_child.min() = 0;
		PackageConfigurationContainer::meta_ComponentPackageReference_child.max() = 1;
		PackageConfigurationContainer::meta_ComponentPackageReference_child.isNavigable() = true;
		PackageConfigurationContainer::meta_ComponentPackageReference_child.target() = ComponentPackageReference::meta;
	}
	void InitComposition91()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		PackageConfConfigProperty::meta_PackageConfigurationContainer_parent = Uml::CompositionParentRole::Create(comp);
		PackageConfConfigProperty::meta_PackageConfigurationContainer_parent.name() = "PackageConfigurationContainer_parent";
		PackageConfConfigProperty::meta_PackageConfigurationContainer_parent.isNavigable() = true;
		PackageConfConfigProperty::meta_PackageConfigurationContainer_parent.target() = PackageConfigurationContainer::meta;
		PackageConfigurationContainer::meta_PackageConfConfigProperty_children = Uml::CompositionChildRole::Create(comp);
		PackageConfigurationContainer::meta_PackageConfConfigProperty_children.name() = "PackageConfConfigProperty";
		PackageConfigurationContainer::meta_PackageConfConfigProperty_children.min() = 0;
		PackageConfigurationContainer::meta_PackageConfConfigProperty_children.max() = -1;
		PackageConfigurationContainer::meta_PackageConfConfigProperty_children.isNavigable() = true;
		PackageConfigurationContainer::meta_PackageConfConfigProperty_children.target() = PackageConfConfigProperty::meta;
	}
	void InitComposition92()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		PackageConfSelectRequirement::meta_PackageConfigurationContainer_parent = Uml::CompositionParentRole::Create(comp);
		PackageConfSelectRequirement::meta_PackageConfigurationContainer_parent.name() = "PackageConfigurationContainer_parent";
		PackageConfSelectRequirement::meta_PackageConfigurationContainer_parent.isNavigable() = true;
		PackageConfSelectRequirement::meta_PackageConfigurationContainer_parent.target() = PackageConfigurationContainer::meta;
		PackageConfigurationContainer::meta_PackageConfSelectRequirement_children = Uml::CompositionChildRole::Create(comp);
		PackageConfigurationContainer::meta_PackageConfSelectRequirement_children.name() = "PackageConfSelectRequirement";
		PackageConfigurationContainer::meta_PackageConfSelectRequirement_children.min() = 0;
		PackageConfigurationContainer::meta_PackageConfSelectRequirement_children.max() = -1;
		PackageConfigurationContainer::meta_PackageConfSelectRequirement_children.isNavigable() = true;
		PackageConfigurationContainer::meta_PackageConfSelectRequirement_children.target() = PackageConfSelectRequirement::meta;
	}
	void InitComposition93()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		Requirement::meta_PackageConfigurationContainer_parent = Uml::CompositionParentRole::Create(comp);
		Requirement::meta_PackageConfigurationContainer_parent.name() = "PackageConfigurationContainer_parent";
		Requirement::meta_PackageConfigurationContainer_parent.isNavigable() = true;
		Requirement::meta_PackageConfigurationContainer_parent.target() = PackageConfigurationContainer::meta;
		PackageConfigurationContainer::meta_Requirement_children = Uml::CompositionChildRole::Create(comp);
		PackageConfigurationContainer::meta_Requirement_children.name() = "Requirement";
		PackageConfigurationContainer::meta_Requirement_children.min() = 0;
		PackageConfigurationContainer::meta_Requirement_children.max() = -1;
		PackageConfigurationContainer::meta_Requirement_children.isNavigable() = true;
		PackageConfigurationContainer::meta_Requirement_children.target() = Requirement::meta;
	}
	void InitComposition94()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		Property::meta_PackageConfigurationContainer_parent = Uml::CompositionParentRole::Create(comp);
		Property::meta_PackageConfigurationContainer_parent.name() = "PackageConfigurationContainer_parent";
		Property::meta_PackageConfigurationContainer_parent.isNavigable() = true;
		Property::meta_PackageConfigurationContainer_parent.target() = PackageConfigurationContainer::meta;
		PackageConfigurationContainer::meta_Property_children = Uml::CompositionChildRole::Create(comp);
		PackageConfigurationContainer::meta_Property_children.name() = "Property";
		PackageConfigurationContainer::meta_Property_children.min() = 0;
		PackageConfigurationContainer::meta_Property_children.max() = -1;
		PackageConfigurationContainer::meta_Property_children.isNavigable() = true;
		PackageConfigurationContainer::meta_Property_children.target() = Property::meta;
	}
	void InitComposition95()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		PackageConfSpecializedConfig::meta_PackageConfigurationContainer_parent = Uml::CompositionParentRole::Create(comp);
		PackageConfSpecializedConfig::meta_PackageConfigurationContainer_parent.name() = "PackageConfigurationContainer_parent";
		PackageConfSpecializedConfig::meta_PackageConfigurationContainer_parent.isNavigable() = true;
		PackageConfSpecializedConfig::meta_PackageConfigurationContainer_parent.target() = PackageConfigurationContainer::meta;
		PackageConfigurationContainer::meta_PackageConfSpecializedConfig_child = Uml::CompositionChildRole::Create(comp);
		PackageConfigurationContainer::meta_PackageConfSpecializedConfig_child.name() = "PackageConfSpecializedConfig";
		PackageConfigurationContainer::meta_PackageConfSpecializedConfig_child.min() = 0;
		PackageConfigurationContainer::meta_PackageConfSpecializedConfig_child.max() = 1;
		PackageConfigurationContainer::meta_PackageConfSpecializedConfig_child.isNavigable() = true;
		PackageConfigurationContainer::meta_PackageConfSpecializedConfig_child.target() = PackageConfSpecializedConfig::meta;
	}
	void InitComposition96()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		PackageConfiguration::meta_PackageConfigurationContainer_parent = Uml::CompositionParentRole::Create(comp);
		PackageConfiguration::meta_PackageConfigurationContainer_parent.name() = "PackageConfigurationContainer_parent";
		PackageConfiguration::meta_PackageConfigurationContainer_parent.isNavigable() = true;
		PackageConfiguration::meta_PackageConfigurationContainer_parent.target() = PackageConfigurationContainer::meta;
		PackageConfigurationContainer::meta_PackageConfiguration_children = Uml::CompositionChildRole::Create(comp);
		PackageConfigurationContainer::meta_PackageConfiguration_children.name() = "PackageConfiguration";
		PackageConfigurationContainer::meta_PackageConfiguration_children.min() = 1;
		PackageConfigurationContainer::meta_PackageConfiguration_children.max() = -1;
		PackageConfigurationContainer::meta_PackageConfiguration_children.isNavigable() = true;
		PackageConfigurationContainer::meta_PackageConfiguration_children.target() = PackageConfiguration::meta;
	}
	void InitComposition97()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		PackageConfigurationReference::meta_TopLevelPackageContainer_parent = Uml::CompositionParentRole::Create(comp);
		PackageConfigurationReference::meta_TopLevelPackageContainer_parent.name() = "TopLevelPackageContainer_parent";
		PackageConfigurationReference::meta_TopLevelPackageContainer_parent.isNavigable() = true;
		PackageConfigurationReference::meta_TopLevelPackageContainer_parent.target() = TopLevelPackageContainer::meta;
		TopLevelPackageContainer::meta_PackageConfigurationReference_child = Uml::CompositionChildRole::Create(comp);
		TopLevelPackageContainer::meta_PackageConfigurationReference_child.name() = "PackageConfigurationReference";
		TopLevelPackageContainer::meta_PackageConfigurationReference_child.min() = 1;
		TopLevelPackageContainer::meta_PackageConfigurationReference_child.max() = 1;
		TopLevelPackageContainer::meta_PackageConfigurationReference_child.isNavigable() = true;
		TopLevelPackageContainer::meta_PackageConfigurationReference_child.target() = PackageConfigurationReference::meta;
	}
	void InitComposition98()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		ArtifactContainer::meta_ImplementationArtifacts_parent = Uml::CompositionParentRole::Create(comp);
		ArtifactContainer::meta_ImplementationArtifacts_parent.name() = "ImplementationArtifacts_parent";
		ArtifactContainer::meta_ImplementationArtifacts_parent.isNavigable() = true;
		ArtifactContainer::meta_ImplementationArtifacts_parent.target() = ImplementationArtifacts::meta;
		ImplementationArtifacts::meta_ArtifactContainer_children = Uml::CompositionChildRole::Create(comp);
		ImplementationArtifacts::meta_ArtifactContainer_children.name() = "ArtifactContainer";
		ImplementationArtifacts::meta_ArtifactContainer_children.min() = 0;
		ImplementationArtifacts::meta_ArtifactContainer_children.max() = -1;
		ImplementationArtifacts::meta_ArtifactContainer_children.isNavigable() = true;
		ImplementationArtifacts::meta_ArtifactContainer_children.target() = ArtifactContainer::meta;
	}
	void InitComposition99()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		ImplementationArtifacts::meta_RootFolder_parent = Uml::CompositionParentRole::Create(comp);
		ImplementationArtifacts::meta_RootFolder_parent.name() = "RootFolder_parent";
		ImplementationArtifacts::meta_RootFolder_parent.isNavigable() = true;
		ImplementationArtifacts::meta_RootFolder_parent.target() = RootFolder::meta;
		RootFolder::meta_ImplementationArtifacts_children = Uml::CompositionChildRole::Create(comp);
		RootFolder::meta_ImplementationArtifacts_children.name() = "ImplementationArtifacts";
		RootFolder::meta_ImplementationArtifacts_children.min() = 0;
		RootFolder::meta_ImplementationArtifacts_children.max() = -1;
		RootFolder::meta_ImplementationArtifacts_children.isNavigable() = true;
		RootFolder::meta_ImplementationArtifacts_children.target() = ImplementationArtifacts::meta;
	}
	void InitComposition100()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		ArtifactDependsOn::meta_ArtifactContainer_parent = Uml::CompositionParentRole::Create(comp);
		ArtifactDependsOn::meta_ArtifactContainer_parent.name() = "ArtifactContainer_parent";
		ArtifactDependsOn::meta_ArtifactContainer_parent.isNavigable() = true;
		ArtifactDependsOn::meta_ArtifactContainer_parent.target() = ArtifactContainer::meta;
		ArtifactContainer::meta_ArtifactDependsOn_children = Uml::CompositionChildRole::Create(comp);
		ArtifactContainer::meta_ArtifactDependsOn_children.name() = "ArtifactDependsOn";
		ArtifactContainer::meta_ArtifactDependsOn_children.min() = 0;
		ArtifactContainer::meta_ArtifactDependsOn_children.max() = -1;
		ArtifactContainer::meta_ArtifactDependsOn_children.isNavigable() = true;
		ArtifactContainer::meta_ArtifactDependsOn_children.target() = ArtifactDependsOn::meta;
	}
	void InitComposition101()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		ArtifactDeployRequirement::meta_ArtifactContainer_parent = Uml::CompositionParentRole::Create(comp);
		ArtifactDeployRequirement::meta_ArtifactContainer_parent.name() = "ArtifactContainer_parent";
		ArtifactDeployRequirement::meta_ArtifactContainer_parent.isNavigable() = true;
		ArtifactDeployRequirement::meta_ArtifactContainer_parent.target() = ArtifactContainer::meta;
		ArtifactContainer::meta_ArtifactDeployRequirement_children = Uml::CompositionChildRole::Create(comp);
		ArtifactContainer::meta_ArtifactDeployRequirement_children.name() = "ArtifactDeployRequirement";
		ArtifactContainer::meta_ArtifactDeployRequirement_children.min() = 0;
		ArtifactContainer::meta_ArtifactDeployRequirement_children.max() = -1;
		ArtifactContainer::meta_ArtifactDeployRequirement_children.isNavigable() = true;
		ArtifactContainer::meta_ArtifactDeployRequirement_children.target() = ArtifactDeployRequirement::meta;
	}
	void InitComposition102()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		ImplementationArtifactReference::meta_ArtifactContainer_parent = Uml::CompositionParentRole::Create(comp);
		ImplementationArtifactReference::meta_ArtifactContainer_parent.name() = "ArtifactContainer_parent";
		ImplementationArtifactReference::meta_ArtifactContainer_parent.isNavigable() = true;
		ImplementationArtifactReference::meta_ArtifactContainer_parent.target() = ArtifactContainer::meta;
		ArtifactContainer::meta_ImplementationArtifactReference_children = Uml::CompositionChildRole::Create(comp);
		ArtifactContainer::meta_ImplementationArtifactReference_children.name() = "ImplementationArtifactReference";
		ArtifactContainer::meta_ImplementationArtifactReference_children.min() = 0;
		ArtifactContainer::meta_ImplementationArtifactReference_children.max() = -1;
		ArtifactContainer::meta_ImplementationArtifactReference_children.isNavigable() = true;
		ArtifactContainer::meta_ImplementationArtifactReference_children.target() = ImplementationArtifactReference::meta;
	}
	void InitComposition103()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		Requirement::meta_ArtifactContainer_parent = Uml::CompositionParentRole::Create(comp);
		Requirement::meta_ArtifactContainer_parent.name() = "ArtifactContainer_parent";
		Requirement::meta_ArtifactContainer_parent.isNavigable() = true;
		Requirement::meta_ArtifactContainer_parent.target() = ArtifactContainer::meta;
		ArtifactContainer::meta_Requirement_children = Uml::CompositionChildRole::Create(comp);
		ArtifactContainer::meta_Requirement_children.name() = "Requirement";
		ArtifactContainer::meta_Requirement_children.min() = 0;
		ArtifactContainer::meta_Requirement_children.max() = -1;
		ArtifactContainer::meta_Requirement_children.isNavigable() = true;
		ArtifactContainer::meta_Requirement_children.target() = Requirement::meta;
	}
	void InitComposition104()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		ArtifactExecParameter::meta_ArtifactContainer_parent = Uml::CompositionParentRole::Create(comp);
		ArtifactExecParameter::meta_ArtifactContainer_parent.name() = "ArtifactContainer_parent";
		ArtifactExecParameter::meta_ArtifactContainer_parent.isNavigable() = true;
		ArtifactExecParameter::meta_ArtifactContainer_parent.target() = ArtifactContainer::meta;
		ArtifactContainer::meta_ArtifactExecParameter_children = Uml::CompositionChildRole::Create(comp);
		ArtifactContainer::meta_ArtifactExecParameter_children.name() = "ArtifactExecParameter";
		ArtifactContainer::meta_ArtifactExecParameter_children.min() = 0;
		ArtifactContainer::meta_ArtifactExecParameter_children.max() = -1;
		ArtifactContainer::meta_ArtifactExecParameter_children.isNavigable() = true;
		ArtifactContainer::meta_ArtifactExecParameter_children.target() = ArtifactExecParameter::meta;
	}
	void InitComposition105()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		ArtifactInfoProperty::meta_ArtifactContainer_parent = Uml::CompositionParentRole::Create(comp);
		ArtifactInfoProperty::meta_ArtifactContainer_parent.name() = "ArtifactContainer_parent";
		ArtifactInfoProperty::meta_ArtifactContainer_parent.isNavigable() = true;
		ArtifactInfoProperty::meta_ArtifactContainer_parent.target() = ArtifactContainer::meta;
		ArtifactContainer::meta_ArtifactInfoProperty_children = Uml::CompositionChildRole::Create(comp);
		ArtifactContainer::meta_ArtifactInfoProperty_children.name() = "ArtifactInfoProperty";
		ArtifactContainer::meta_ArtifactInfoProperty_children.min() = 0;
		ArtifactContainer::meta_ArtifactInfoProperty_children.max() = -1;
		ArtifactContainer::meta_ArtifactInfoProperty_children.isNavigable() = true;
		ArtifactContainer::meta_ArtifactInfoProperty_children.target() = ArtifactInfoProperty::meta;
	}
	void InitComposition106()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		Property::meta_ArtifactContainer_parent = Uml::CompositionParentRole::Create(comp);
		Property::meta_ArtifactContainer_parent.name() = "ArtifactContainer_parent";
		Property::meta_ArtifactContainer_parent.isNavigable() = true;
		Property::meta_ArtifactContainer_parent.target() = ArtifactContainer::meta;
		ArtifactContainer::meta_Property_children = Uml::CompositionChildRole::Create(comp);
		ArtifactContainer::meta_Property_children.name() = "Property";
		ArtifactContainer::meta_Property_children.min() = 0;
		ArtifactContainer::meta_Property_children.max() = -1;
		ArtifactContainer::meta_Property_children.isNavigable() = true;
		ArtifactContainer::meta_Property_children.target() = Property::meta;
	}
	void InitComposition107()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		ImplementationArtifact::meta_ArtifactContainer_parent = Uml::CompositionParentRole::Create(comp);
		ImplementationArtifact::meta_ArtifactContainer_parent.name() = "ArtifactContainer_parent";
		ImplementationArtifact::meta_ArtifactContainer_parent.isNavigable() = true;
		ImplementationArtifact::meta_ArtifactContainer_parent.target() = ArtifactContainer::meta;
		ArtifactContainer::meta_ImplementationArtifact_children = Uml::CompositionChildRole::Create(comp);
		ArtifactContainer::meta_ImplementationArtifact_children.name() = "ImplementationArtifact";
		ArtifactContainer::meta_ImplementationArtifact_children.min() = 1;
		ArtifactContainer::meta_ImplementationArtifact_children.max() = -1;
		ArtifactContainer::meta_ImplementationArtifact_children.isNavigable() = true;
		ArtifactContainer::meta_ImplementationArtifact_children.target() = ImplementationArtifact::meta;
	}
	void InitComposition108()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		ImplementationArtifactReference::meta_StubProject_parent = Uml::CompositionParentRole::Create(comp);
		ImplementationArtifactReference::meta_StubProject_parent.name() = "StubProject_parent";
		ImplementationArtifactReference::meta_StubProject_parent.isNavigable() = true;
		ImplementationArtifactReference::meta_StubProject_parent.target() = StubProject::meta;
		StubProject::meta_ImplementationArtifactReference_child = Uml::CompositionChildRole::Create(comp);
		StubProject::meta_ImplementationArtifactReference_child.name() = "ImplementationArtifactReference";
		StubProject::meta_ImplementationArtifactReference_child.min() = 1;
		StubProject::meta_ImplementationArtifactReference_child.max() = 1;
		StubProject::meta_ImplementationArtifactReference_child.isNavigable() = true;
		StubProject::meta_ImplementationArtifactReference_child.target() = ImplementationArtifactReference::meta;
	}
	void InitComposition109()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		ImplementationArtifactReference::meta_ServantProject_parent = Uml::CompositionParentRole::Create(comp);
		ImplementationArtifactReference::meta_ServantProject_parent.name() = "ServantProject_parent";
		ImplementationArtifactReference::meta_ServantProject_parent.isNavigable() = true;
		ImplementationArtifactReference::meta_ServantProject_parent.target() = ServantProject::meta;
		ServantProject::meta_ImplementationArtifactReference_child = Uml::CompositionChildRole::Create(comp);
		ServantProject::meta_ImplementationArtifactReference_child.name() = "ImplementationArtifactReference";
		ServantProject::meta_ImplementationArtifactReference_child.min() = 1;
		ServantProject::meta_ImplementationArtifactReference_child.max() = 1;
		ServantProject::meta_ImplementationArtifactReference_child.isNavigable() = true;
		ServantProject::meta_ImplementationArtifactReference_child.target() = ImplementationArtifactReference::meta;
	}
	void InitComposition110()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		ImplementationArtifactReference::meta_ExecutorProject_parent = Uml::CompositionParentRole::Create(comp);
		ImplementationArtifactReference::meta_ExecutorProject_parent.name() = "ExecutorProject_parent";
		ImplementationArtifactReference::meta_ExecutorProject_parent.isNavigable() = true;
		ImplementationArtifactReference::meta_ExecutorProject_parent.target() = ExecutorProject::meta;
		ExecutorProject::meta_ImplementationArtifactReference_child = Uml::CompositionChildRole::Create(comp);
		ExecutorProject::meta_ImplementationArtifactReference_child.name() = "ImplementationArtifactReference";
		ExecutorProject::meta_ImplementationArtifactReference_child.min() = 1;
		ExecutorProject::meta_ImplementationArtifactReference_child.max() = 1;
		ExecutorProject::meta_ImplementationArtifactReference_child.isNavigable() = true;
		ExecutorProject::meta_ImplementationArtifactReference_child.target() = ImplementationArtifactReference::meta;
	}
	void InitComposition111()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		ImplementationArtifact::meta_Project_parent = Uml::CompositionParentRole::Create(comp);
		ImplementationArtifact::meta_Project_parent.name() = "Project_parent";
		ImplementationArtifact::meta_Project_parent.isNavigable() = true;
		ImplementationArtifact::meta_Project_parent.target() = Project::meta;
		Project::meta_ImplementationArtifact_children = Uml::CompositionChildRole::Create(comp);
		Project::meta_ImplementationArtifact_children.name() = "ImplementationArtifact";
		Project::meta_ImplementationArtifact_children.min() = 0;
		Project::meta_ImplementationArtifact_children.max() = -1;
		Project::meta_ImplementationArtifact_children.isNavigable() = true;
		Project::meta_ImplementationArtifact_children.target() = ImplementationArtifact::meta;
	}
	void InitComposition112()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		ComponentContainer::meta_ComponentTypes_parent = Uml::CompositionParentRole::Create(comp);
		ComponentContainer::meta_ComponentTypes_parent.name() = "ComponentTypes_parent";
		ComponentContainer::meta_ComponentTypes_parent.isNavigable() = true;
		ComponentContainer::meta_ComponentTypes_parent.target() = ComponentTypes::meta;
		ComponentTypes::meta_ComponentContainer_children = Uml::CompositionChildRole::Create(comp);
		ComponentTypes::meta_ComponentContainer_children.name() = "ComponentContainer";
		ComponentTypes::meta_ComponentContainer_children.min() = 0;
		ComponentTypes::meta_ComponentContainer_children.max() = -1;
		ComponentTypes::meta_ComponentContainer_children.isNavigable() = true;
		ComponentTypes::meta_ComponentContainer_children.target() = ComponentContainer::meta;
	}
	void InitComposition113()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		ComponentTypes::meta_RootFolder_parent = Uml::CompositionParentRole::Create(comp);
		ComponentTypes::meta_RootFolder_parent.name() = "RootFolder_parent";
		ComponentTypes::meta_RootFolder_parent.isNavigable() = true;
		ComponentTypes::meta_RootFolder_parent.target() = RootFolder::meta;
		RootFolder::meta_ComponentTypes_children = Uml::CompositionChildRole::Create(comp);
		RootFolder::meta_ComponentTypes_children.name() = "ComponentTypes";
		RootFolder::meta_ComponentTypes_children.min() = 0;
		RootFolder::meta_ComponentTypes_children.max() = -1;
		RootFolder::meta_ComponentTypes_children.isNavigable() = true;
		RootFolder::meta_ComponentTypes_children.target() = ComponentTypes::meta;
	}
	void InitComposition114()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		DataType::meta_ComponentPropertyDescription_parent = Uml::CompositionParentRole::Create(comp);
		DataType::meta_ComponentPropertyDescription_parent.name() = "ComponentPropertyDescription_parent";
		DataType::meta_ComponentPropertyDescription_parent.isNavigable() = true;
		DataType::meta_ComponentPropertyDescription_parent.target() = ComponentPropertyDescription::meta;
		ComponentPropertyDescription::meta_DataType_child = Uml::CompositionChildRole::Create(comp);
		ComponentPropertyDescription::meta_DataType_child.name() = "DataType";
		ComponentPropertyDescription::meta_DataType_child.min() = 1;
		ComponentPropertyDescription::meta_DataType_child.max() = 1;
		ComponentPropertyDescription::meta_DataType_child.isNavigable() = true;
		ComponentPropertyDescription::meta_DataType_child.target() = DataType::meta;
	}
	void InitComposition115()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		ComponentPropertyDescription::meta_ComponentContainer_parent = Uml::CompositionParentRole::Create(comp);
		ComponentPropertyDescription::meta_ComponentContainer_parent.name() = "ComponentContainer_parent";
		ComponentPropertyDescription::meta_ComponentContainer_parent.isNavigable() = true;
		ComponentPropertyDescription::meta_ComponentContainer_parent.target() = ComponentContainer::meta;
		ComponentContainer::meta_ComponentPropertyDescription_children = Uml::CompositionChildRole::Create(comp);
		ComponentContainer::meta_ComponentPropertyDescription_children.name() = "ComponentPropertyDescription";
		ComponentContainer::meta_ComponentPropertyDescription_children.min() = 0;
		ComponentContainer::meta_ComponentPropertyDescription_children.max() = -1;
		ComponentContainer::meta_ComponentPropertyDescription_children.isNavigable() = true;
		ComponentContainer::meta_ComponentPropertyDescription_children.target() = ComponentPropertyDescription::meta;
	}
	void InitComposition116()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		ComponentProperty::meta_ComponentContainer_parent = Uml::CompositionParentRole::Create(comp);
		ComponentProperty::meta_ComponentContainer_parent.name() = "ComponentContainer_parent";
		ComponentProperty::meta_ComponentContainer_parent.isNavigable() = true;
		ComponentProperty::meta_ComponentContainer_parent.target() = ComponentContainer::meta;
		ComponentContainer::meta_ComponentProperty_children = Uml::CompositionChildRole::Create(comp);
		ComponentContainer::meta_ComponentProperty_children.name() = "ComponentProperty";
		ComponentContainer::meta_ComponentProperty_children.min() = 0;
		ComponentContainer::meta_ComponentProperty_children.max() = -1;
		ComponentContainer::meta_ComponentProperty_children.isNavigable() = true;
		ComponentContainer::meta_ComponentProperty_children.target() = ComponentProperty::meta;
	}
	void InitComposition117()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		ComponentConfigProperty::meta_ComponentContainer_parent = Uml::CompositionParentRole::Create(comp);
		ComponentConfigProperty::meta_ComponentContainer_parent.name() = "ComponentContainer_parent";
		ComponentConfigProperty::meta_ComponentContainer_parent.isNavigable() = true;
		ComponentConfigProperty::meta_ComponentContainer_parent.target() = ComponentContainer::meta;
		ComponentContainer::meta_ComponentConfigProperty_children = Uml::CompositionChildRole::Create(comp);
		ComponentContainer::meta_ComponentConfigProperty_children.name() = "ComponentConfigProperty";
		ComponentContainer::meta_ComponentConfigProperty_children.min() = 0;
		ComponentContainer::meta_ComponentConfigProperty_children.max() = -1;
		ComponentContainer::meta_ComponentConfigProperty_children.isNavigable() = true;
		ComponentContainer::meta_ComponentConfigProperty_children.target() = ComponentConfigProperty::meta;
	}
	void InitComposition118()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		Property::meta_ComponentContainer_parent = Uml::CompositionParentRole::Create(comp);
		Property::meta_ComponentContainer_parent.name() = "ComponentContainer_parent";
		Property::meta_ComponentContainer_parent.isNavigable() = true;
		Property::meta_ComponentContainer_parent.target() = ComponentContainer::meta;
		ComponentContainer::meta_Property_children = Uml::CompositionChildRole::Create(comp);
		ComponentContainer::meta_Property_children.name() = "Property";
		ComponentContainer::meta_Property_children.min() = 0;
		ComponentContainer::meta_Property_children.max() = -1;
		ComponentContainer::meta_Property_children.isNavigable() = true;
		ComponentContainer::meta_Property_children.target() = Property::meta;
	}
	void InitComposition119()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		ComponentRef::meta_ComponentContainer_parent = Uml::CompositionParentRole::Create(comp);
		ComponentRef::meta_ComponentContainer_parent.name() = "ComponentContainer_parent";
		ComponentRef::meta_ComponentContainer_parent.isNavigable() = true;
		ComponentRef::meta_ComponentContainer_parent.target() = ComponentContainer::meta;
		ComponentContainer::meta_ComponentRef_child = Uml::CompositionChildRole::Create(comp);
		ComponentContainer::meta_ComponentRef_child.name() = "ComponentRef";
		ComponentContainer::meta_ComponentRef_child.min() = 0;
		ComponentContainer::meta_ComponentRef_child.max() = 1;
		ComponentContainer::meta_ComponentRef_child.isNavigable() = true;
		ComponentContainer::meta_ComponentRef_child.target() = ComponentRef::meta;
	}
	void InitComposition120()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		ComponentInfoProperty::meta_ComponentContainer_parent = Uml::CompositionParentRole::Create(comp);
		ComponentInfoProperty::meta_ComponentContainer_parent.name() = "ComponentContainer_parent";
		ComponentInfoProperty::meta_ComponentContainer_parent.isNavigable() = true;
		ComponentInfoProperty::meta_ComponentContainer_parent.target() = ComponentContainer::meta;
		ComponentContainer::meta_ComponentInfoProperty_children = Uml::CompositionChildRole::Create(comp);
		ComponentContainer::meta_ComponentInfoProperty_children.name() = "ComponentInfoProperty";
		ComponentContainer::meta_ComponentInfoProperty_children.min() = 0;
		ComponentContainer::meta_ComponentInfoProperty_children.max() = -1;
		ComponentContainer::meta_ComponentInfoProperty_children.isNavigable() = true;
		ComponentContainer::meta_ComponentInfoProperty_children.target() = ComponentInfoProperty::meta;
	}
	void InitComposition121()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		TopLevelPackage::meta_TopLevelPackageContainer_parent = Uml::CompositionParentRole::Create(comp);
		TopLevelPackage::meta_TopLevelPackageContainer_parent.name() = "TopLevelPackageContainer_parent";
		TopLevelPackage::meta_TopLevelPackageContainer_parent.isNavigable() = true;
		TopLevelPackage::meta_TopLevelPackageContainer_parent.target() = TopLevelPackageContainer::meta;
		TopLevelPackageContainer::meta_TopLevelPackage_child = Uml::CompositionChildRole::Create(comp);
		TopLevelPackageContainer::meta_TopLevelPackage_child.name() = "TopLevelPackage";
		TopLevelPackageContainer::meta_TopLevelPackage_child.min() = 0;
		TopLevelPackageContainer::meta_TopLevelPackage_child.max() = 1;
		TopLevelPackageContainer::meta_TopLevelPackage_child.isNavigable() = true;
		TopLevelPackageContainer::meta_TopLevelPackage_child.target() = TopLevelPackage::meta;
	}
	void InitComposition122()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		package::meta_TopLevelPackageContainer_parent = Uml::CompositionParentRole::Create(comp);
		package::meta_TopLevelPackageContainer_parent.name() = "TopLevelPackageContainer_parent";
		package::meta_TopLevelPackageContainer_parent.isNavigable() = true;
		package::meta_TopLevelPackageContainer_parent.target() = TopLevelPackageContainer::meta;
		TopLevelPackageContainer::meta_package_child = Uml::CompositionChildRole::Create(comp);
		TopLevelPackageContainer::meta_package_child.name() = "package";
		TopLevelPackageContainer::meta_package_child.min() = 0;
		TopLevelPackageContainer::meta_package_child.max() = 1;
		TopLevelPackageContainer::meta_package_child.isNavigable() = true;
		TopLevelPackageContainer::meta_package_child.target() = package::meta;
	}
	void InitComposition123()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		TopLevelPackageContainer::meta_TopLevelPackages_parent = Uml::CompositionParentRole::Create(comp);
		TopLevelPackageContainer::meta_TopLevelPackages_parent.name() = "TopLevelPackages_parent";
		TopLevelPackageContainer::meta_TopLevelPackages_parent.isNavigable() = true;
		TopLevelPackageContainer::meta_TopLevelPackages_parent.target() = TopLevelPackages::meta;
		TopLevelPackages::meta_TopLevelPackageContainer_children = Uml::CompositionChildRole::Create(comp);
		TopLevelPackages::meta_TopLevelPackageContainer_children.name() = "TopLevelPackageContainer";
		TopLevelPackages::meta_TopLevelPackageContainer_children.min() = 0;
		TopLevelPackages::meta_TopLevelPackageContainer_children.max() = -1;
		TopLevelPackages::meta_TopLevelPackageContainer_children.isNavigable() = true;
		TopLevelPackages::meta_TopLevelPackageContainer_children.target() = TopLevelPackageContainer::meta;
	}
	void InitComposition124()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		TopLevelPackages::meta_RootFolder_parent = Uml::CompositionParentRole::Create(comp);
		TopLevelPackages::meta_RootFolder_parent.name() = "RootFolder_parent";
		TopLevelPackages::meta_RootFolder_parent.isNavigable() = true;
		TopLevelPackages::meta_RootFolder_parent.target() = RootFolder::meta;
		RootFolder::meta_TopLevelPackages_children = Uml::CompositionChildRole::Create(comp);
		RootFolder::meta_TopLevelPackages_children.name() = "TopLevelPackages";
		RootFolder::meta_TopLevelPackages_children.min() = 0;
		RootFolder::meta_TopLevelPackages_children.max() = -1;
		RootFolder::meta_TopLevelPackages_children.isNavigable() = true;
		RootFolder::meta_TopLevelPackages_children.target() = TopLevelPackages::meta;
	}
	void InitComposition125()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		PackageContainer::meta_ComponentPackages_parent = Uml::CompositionParentRole::Create(comp);
		PackageContainer::meta_ComponentPackages_parent.name() = "ComponentPackages_parent";
		PackageContainer::meta_ComponentPackages_parent.isNavigable() = true;
		PackageContainer::meta_ComponentPackages_parent.target() = ComponentPackages::meta;
		ComponentPackages::meta_PackageContainer_children = Uml::CompositionChildRole::Create(comp);
		ComponentPackages::meta_PackageContainer_children.name() = "PackageContainer";
		ComponentPackages::meta_PackageContainer_children.min() = 0;
		ComponentPackages::meta_PackageContainer_children.max() = -1;
		ComponentPackages::meta_PackageContainer_children.isNavigable() = true;
		ComponentPackages::meta_PackageContainer_children.target() = PackageContainer::meta;
	}
	void InitComposition126()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		ComponentPackages::meta_RootFolder_parent = Uml::CompositionParentRole::Create(comp);
		ComponentPackages::meta_RootFolder_parent.name() = "RootFolder_parent";
		ComponentPackages::meta_RootFolder_parent.isNavigable() = true;
		ComponentPackages::meta_RootFolder_parent.target() = RootFolder::meta;
		RootFolder::meta_ComponentPackages_children = Uml::CompositionChildRole::Create(comp);
		RootFolder::meta_ComponentPackages_children.name() = "ComponentPackages";
		RootFolder::meta_ComponentPackages_children.min() = 0;
		RootFolder::meta_ComponentPackages_children.max() = -1;
		RootFolder::meta_ComponentPackages_children.isNavigable() = true;
		RootFolder::meta_ComponentPackages_children.target() = ComponentPackages::meta;
	}
	void InitComposition127()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		PackageInterface::meta_PackageContainer_parent = Uml::CompositionParentRole::Create(comp);
		PackageInterface::meta_PackageContainer_parent.name() = "PackageContainer_parent";
		PackageInterface::meta_PackageContainer_parent.isNavigable() = true;
		PackageInterface::meta_PackageContainer_parent.target() = PackageContainer::meta;
		PackageContainer::meta_PackageInterface_child = Uml::CompositionChildRole::Create(comp);
		PackageContainer::meta_PackageInterface_child.name() = "PackageInterface";
		PackageContainer::meta_PackageInterface_child.min() = 0;
		PackageContainer::meta_PackageInterface_child.max() = 1;
		PackageContainer::meta_PackageInterface_child.isNavigable() = true;
		PackageContainer::meta_PackageInterface_child.target() = PackageInterface::meta;
	}
	void InitComposition128()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		Implementation::meta_PackageContainer_parent = Uml::CompositionParentRole::Create(comp);
		Implementation::meta_PackageContainer_parent.name() = "PackageContainer_parent";
		Implementation::meta_PackageContainer_parent.isNavigable() = true;
		Implementation::meta_PackageContainer_parent.target() = PackageContainer::meta;
		PackageContainer::meta_Implementation_children = Uml::CompositionChildRole::Create(comp);
		PackageContainer::meta_Implementation_children.name() = "Implementation";
		PackageContainer::meta_Implementation_children.min() = 0;
		PackageContainer::meta_Implementation_children.max() = -1;
		PackageContainer::meta_Implementation_children.isNavigable() = true;
		PackageContainer::meta_Implementation_children.target() = Implementation::meta;
	}
	void InitComposition129()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		ComponentRef::meta_PackageContainer_parent = Uml::CompositionParentRole::Create(comp);
		ComponentRef::meta_PackageContainer_parent.name() = "PackageContainer_parent";
		ComponentRef::meta_PackageContainer_parent.isNavigable() = true;
		ComponentRef::meta_PackageContainer_parent.target() = PackageContainer::meta;
		PackageContainer::meta_ComponentRef_children = Uml::CompositionChildRole::Create(comp);
		PackageContainer::meta_ComponentRef_children.name() = "ComponentRef";
		PackageContainer::meta_ComponentRef_children.min() = 0;
		PackageContainer::meta_ComponentRef_children.max() = -1;
		PackageContainer::meta_ComponentRef_children.isNavigable() = true;
		PackageContainer::meta_ComponentRef_children.target() = ComponentRef::meta;
	}
	void InitComposition130()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		PackageConfigProperty::meta_PackageContainer_parent = Uml::CompositionParentRole::Create(comp);
		PackageConfigProperty::meta_PackageContainer_parent.name() = "PackageContainer_parent";
		PackageConfigProperty::meta_PackageContainer_parent.isNavigable() = true;
		PackageConfigProperty::meta_PackageContainer_parent.target() = PackageContainer::meta;
		PackageContainer::meta_PackageConfigProperty_children = Uml::CompositionChildRole::Create(comp);
		PackageContainer::meta_PackageConfigProperty_children.name() = "PackageConfigProperty";
		PackageContainer::meta_PackageConfigProperty_children.min() = 0;
		PackageContainer::meta_PackageConfigProperty_children.max() = -1;
		PackageContainer::meta_PackageConfigProperty_children.isNavigable() = true;
		PackageContainer::meta_PackageConfigProperty_children.target() = PackageConfigProperty::meta;
	}
	void InitComposition131()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		Property::meta_PackageContainer_parent = Uml::CompositionParentRole::Create(comp);
		Property::meta_PackageContainer_parent.name() = "PackageContainer_parent";
		Property::meta_PackageContainer_parent.isNavigable() = true;
		Property::meta_PackageContainer_parent.target() = PackageContainer::meta;
		PackageContainer::meta_Property_children = Uml::CompositionChildRole::Create(comp);
		PackageContainer::meta_Property_children.name() = "Property";
		PackageContainer::meta_Property_children.min() = 0;
		PackageContainer::meta_Property_children.max() = -1;
		PackageContainer::meta_Property_children.isNavigable() = true;
		PackageContainer::meta_Property_children.target() = Property::meta;
	}
	void InitComposition132()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		PackageInfoProperty::meta_PackageContainer_parent = Uml::CompositionParentRole::Create(comp);
		PackageInfoProperty::meta_PackageContainer_parent.name() = "PackageContainer_parent";
		PackageInfoProperty::meta_PackageContainer_parent.isNavigable() = true;
		PackageInfoProperty::meta_PackageContainer_parent.target() = PackageContainer::meta;
		PackageContainer::meta_PackageInfoProperty_children = Uml::CompositionChildRole::Create(comp);
		PackageContainer::meta_PackageInfoProperty_children.name() = "PackageInfoProperty";
		PackageContainer::meta_PackageInfoProperty_children.min() = 0;
		PackageContainer::meta_PackageInfoProperty_children.max() = -1;
		PackageContainer::meta_PackageInfoProperty_children.isNavigable() = true;
		PackageContainer::meta_PackageInfoProperty_children.target() = PackageInfoProperty::meta;
	}
	void InitComposition133()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		ComponentPackage::meta_PackageContainer_parent = Uml::CompositionParentRole::Create(comp);
		ComponentPackage::meta_PackageContainer_parent.name() = "PackageContainer_parent";
		ComponentPackage::meta_PackageContainer_parent.isNavigable() = true;
		ComponentPackage::meta_PackageContainer_parent.target() = PackageContainer::meta;
		PackageContainer::meta_ComponentPackage_child = Uml::CompositionChildRole::Create(comp);
		PackageContainer::meta_ComponentPackage_child.name() = "ComponentPackage";
		PackageContainer::meta_ComponentPackage_child.min() = 0;
		PackageContainer::meta_ComponentPackage_child.max() = 1;
		PackageContainer::meta_ComponentPackage_child.isNavigable() = true;
		PackageContainer::meta_ComponentPackage_child.target() = ComponentPackage::meta;
	}
	void InitComposition134()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		Property::meta_RequirementBase_parent = Uml::CompositionParentRole::Create(comp);
		Property::meta_RequirementBase_parent.name() = "RequirementBase_parent";
		Property::meta_RequirementBase_parent.isNavigable() = true;
		Property::meta_RequirementBase_parent.target() = RequirementBase::meta;
		RequirementBase::meta_Property_children = Uml::CompositionChildRole::Create(comp);
		RequirementBase::meta_Property_children.name() = "Property";
		RequirementBase::meta_Property_children.min() = 0;
		RequirementBase::meta_Property_children.max() = -1;
		RequirementBase::meta_Property_children.isNavigable() = true;
		RequirementBase::meta_Property_children.target() = Property::meta;
	}
	void InitComposition135()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		SatisfierProperty::meta_RequirementSatisfier_parent = Uml::CompositionParentRole::Create(comp);
		SatisfierProperty::meta_RequirementSatisfier_parent.name() = "RequirementSatisfier_parent";
		SatisfierProperty::meta_RequirementSatisfier_parent.isNavigable() = true;
		SatisfierProperty::meta_RequirementSatisfier_parent.target() = RequirementSatisfier::meta;
		RequirementSatisfier::meta_SatisfierProperty_children = Uml::CompositionChildRole::Create(comp);
		RequirementSatisfier::meta_SatisfierProperty_children.name() = "SatisfierProperty";
		RequirementSatisfier::meta_SatisfierProperty_children.min() = 0;
		RequirementSatisfier::meta_SatisfierProperty_children.max() = -1;
		RequirementSatisfier::meta_SatisfierProperty_children.isNavigable() = true;
		RequirementSatisfier::meta_SatisfierProperty_children.target() = SatisfierProperty::meta;
	}
	void InitComposition136()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		DataType::meta_SatisfierProperty_parent = Uml::CompositionParentRole::Create(comp);
		DataType::meta_SatisfierProperty_parent.name() = "SatisfierProperty_parent";
		DataType::meta_SatisfierProperty_parent.isNavigable() = true;
		DataType::meta_SatisfierProperty_parent.target() = SatisfierProperty::meta;
		SatisfierProperty::meta_DataType_child = Uml::CompositionChildRole::Create(comp);
		SatisfierProperty::meta_DataType_child.name() = "DataType";
		SatisfierProperty::meta_DataType_child.min() = 1;
		SatisfierProperty::meta_DataType_child.max() = 1;
		SatisfierProperty::meta_DataType_child.isNavigable() = true;
		SatisfierProperty::meta_DataType_child.target() = DataType::meta;
	}
	void InitComposition137()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		DataType::meta_Property_parent = Uml::CompositionParentRole::Create(comp);
		DataType::meta_Property_parent.name() = "Property_parent";
		DataType::meta_Property_parent.isNavigable() = true;
		DataType::meta_Property_parent.target() = Property::meta;
		Property::meta_DataType_child = Uml::CompositionChildRole::Create(comp);
		Property::meta_DataType_child.name() = "DataType";
		Property::meta_DataType_child.min() = 1;
		Property::meta_DataType_child.max() = 1;
		Property::meta_DataType_child.isNavigable() = true;
		Property::meta_DataType_child.target() = DataType::meta;
	}
	void InitComposition138()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		Property::meta_Domain_parent = Uml::CompositionParentRole::Create(comp);
		Property::meta_Domain_parent.name() = "Domain_parent";
		Property::meta_Domain_parent.isNavigable() = true;
		Property::meta_Domain_parent.target() = Domain::meta;
		Domain::meta_Property_children = Uml::CompositionChildRole::Create(comp);
		Domain::meta_Property_children.name() = "Property";
		Domain::meta_Property_children.min() = 0;
		Domain::meta_Property_children.max() = -1;
		Domain::meta_Property_children.isNavigable() = true;
		Domain::meta_Property_children.target() = Property::meta;
	}
	void InitComposition139()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		Domain::meta_Targets_parent = Uml::CompositionParentRole::Create(comp);
		Domain::meta_Targets_parent.name() = "Targets_parent";
		Domain::meta_Targets_parent.isNavigable() = true;
		Domain::meta_Targets_parent.target() = Targets::meta;
		Targets::meta_Domain_children = Uml::CompositionChildRole::Create(comp);
		Targets::meta_Domain_children.name() = "Domain";
		Targets::meta_Domain_children.min() = 0;
		Targets::meta_Domain_children.max() = -1;
		Targets::meta_Domain_children.isNavigable() = true;
		Targets::meta_Domain_children.target() = Domain::meta;
	}
	void InitComposition140()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		Targets::meta_RootFolder_parent = Uml::CompositionParentRole::Create(comp);
		Targets::meta_RootFolder_parent.name() = "RootFolder_parent";
		Targets::meta_RootFolder_parent.isNavigable() = true;
		Targets::meta_RootFolder_parent.target() = RootFolder::meta;
		RootFolder::meta_Targets_children = Uml::CompositionChildRole::Create(comp);
		RootFolder::meta_Targets_children.name() = "Targets";
		RootFolder::meta_Targets_children.min() = 0;
		RootFolder::meta_Targets_children.max() = -1;
		RootFolder::meta_Targets_children.isNavigable() = true;
		RootFolder::meta_Targets_children.target() = Targets::meta;
	}
	void InitComposition141()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		Shares::meta_Domain_parent = Uml::CompositionParentRole::Create(comp);
		Shares::meta_Domain_parent.name() = "Domain_parent";
		Shares::meta_Domain_parent.isNavigable() = true;
		Shares::meta_Domain_parent.target() = Domain::meta;
		Domain::meta_Shares_children = Uml::CompositionChildRole::Create(comp);
		Domain::meta_Shares_children.name() = "Shares";
		Domain::meta_Shares_children.min() = 0;
		Domain::meta_Shares_children.max() = -1;
		Domain::meta_Shares_children.isNavigable() = true;
		Domain::meta_Shares_children.target() = Shares::meta;
	}
	void InitComposition142()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		Node2Interconnect::meta_Domain_parent = Uml::CompositionParentRole::Create(comp);
		Node2Interconnect::meta_Domain_parent.name() = "Domain_parent";
		Node2Interconnect::meta_Domain_parent.isNavigable() = true;
		Node2Interconnect::meta_Domain_parent.target() = Domain::meta;
		Domain::meta_Node2Interconnect_children = Uml::CompositionChildRole::Create(comp);
		Domain::meta_Node2Interconnect_children.name() = "Node2Interconnect";
		Domain::meta_Node2Interconnect_children.min() = 0;
		Domain::meta_Node2Interconnect_children.max() = -1;
		Domain::meta_Node2Interconnect_children.isNavigable() = true;
		Domain::meta_Node2Interconnect_children.target() = Node2Interconnect::meta;
	}
	void InitComposition143()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		Bridge2Interconnect::meta_Domain_parent = Uml::CompositionParentRole::Create(comp);
		Bridge2Interconnect::meta_Domain_parent.name() = "Domain_parent";
		Bridge2Interconnect::meta_Domain_parent.isNavigable() = true;
		Bridge2Interconnect::meta_Domain_parent.target() = Domain::meta;
		Domain::meta_Bridge2Interconnect_children = Uml::CompositionChildRole::Create(comp);
		Domain::meta_Bridge2Interconnect_children.name() = "Bridge2Interconnect";
		Domain::meta_Bridge2Interconnect_children.min() = 0;
		Domain::meta_Bridge2Interconnect_children.max() = -1;
		Domain::meta_Bridge2Interconnect_children.isNavigable() = true;
		Domain::meta_Bridge2Interconnect_children.target() = Bridge2Interconnect::meta;
	}
	void InitComposition144()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		Interconnect2Node::meta_Domain_parent = Uml::CompositionParentRole::Create(comp);
		Interconnect2Node::meta_Domain_parent.name() = "Domain_parent";
		Interconnect2Node::meta_Domain_parent.isNavigable() = true;
		Interconnect2Node::meta_Domain_parent.target() = Domain::meta;
		Domain::meta_Interconnect2Node_children = Uml::CompositionChildRole::Create(comp);
		Domain::meta_Interconnect2Node_children.name() = "Interconnect2Node";
		Domain::meta_Interconnect2Node_children.min() = 0;
		Domain::meta_Interconnect2Node_children.max() = -1;
		Domain::meta_Interconnect2Node_children.isNavigable() = true;
		Domain::meta_Interconnect2Node_children.target() = Interconnect2Node::meta;
	}
	void InitComposition145()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		Interconnect2Bridge::meta_Domain_parent = Uml::CompositionParentRole::Create(comp);
		Interconnect2Bridge::meta_Domain_parent.name() = "Domain_parent";
		Interconnect2Bridge::meta_Domain_parent.isNavigable() = true;
		Interconnect2Bridge::meta_Domain_parent.target() = Domain::meta;
		Domain::meta_Interconnect2Bridge_children = Uml::CompositionChildRole::Create(comp);
		Domain::meta_Interconnect2Bridge_children.name() = "Interconnect2Bridge";
		Domain::meta_Interconnect2Bridge_children.min() = 0;
		Domain::meta_Interconnect2Bridge_children.max() = -1;
		Domain::meta_Interconnect2Bridge_children.isNavigable() = true;
		Domain::meta_Interconnect2Bridge_children.target() = Interconnect2Bridge::meta;
	}
	void InitComposition146()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		Port::meta_Component_parent = Uml::CompositionParentRole::Create(comp);
		Port::meta_Component_parent.name() = "Component_parent";
		Port::meta_Component_parent.isNavigable() = true;
		Port::meta_Component_parent.target() = Component::meta;
		Component::meta_Port_children = Uml::CompositionChildRole::Create(comp);
		Component::meta_Port_children.name() = "Port";
		Component::meta_Port_children.min() = 0;
		Component::meta_Port_children.max() = -1;
		Component::meta_Port_children.isNavigable() = true;
		Component::meta_Port_children.target() = Port::meta;
	}
	void InitComposition147()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		LookupKey::meta_ComponentFactory_parent = Uml::CompositionParentRole::Create(comp);
		LookupKey::meta_ComponentFactory_parent.name() = "ComponentFactory_parent";
		LookupKey::meta_ComponentFactory_parent.isNavigable() = true;
		LookupKey::meta_ComponentFactory_parent.target() = ComponentFactory::meta;
		ComponentFactory::meta_LookupKey_child = Uml::CompositionChildRole::Create(comp);
		ComponentFactory::meta_LookupKey_child.name() = "LookupKey";
		ComponentFactory::meta_LookupKey_child.min() = 0;
		ComponentFactory::meta_LookupKey_child.max() = 1;
		ComponentFactory::meta_LookupKey_child.isNavigable() = true;
		ComponentFactory::meta_LookupKey_child.target() = LookupKey::meta;
	}
	void InitComposition148()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		FactoryOperation::meta_ComponentFactory_parent = Uml::CompositionParentRole::Create(comp);
		FactoryOperation::meta_ComponentFactory_parent.name() = "ComponentFactory_parent";
		FactoryOperation::meta_ComponentFactory_parent.isNavigable() = true;
		FactoryOperation::meta_ComponentFactory_parent.target() = ComponentFactory::meta;
		ComponentFactory::meta_FactoryOperation_children = Uml::CompositionChildRole::Create(comp);
		ComponentFactory::meta_FactoryOperation_children.name() = "FactoryOperation";
		ComponentFactory::meta_FactoryOperation_children.min() = 0;
		ComponentFactory::meta_FactoryOperation_children.max() = -1;
		ComponentFactory::meta_FactoryOperation_children.isNavigable() = true;
		ComponentFactory::meta_FactoryOperation_children.target() = FactoryOperation::meta;
	}
	void InitComposition149()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		LookupOperation::meta_ComponentFactory_parent = Uml::CompositionParentRole::Create(comp);
		LookupOperation::meta_ComponentFactory_parent.name() = "ComponentFactory_parent";
		LookupOperation::meta_ComponentFactory_parent.isNavigable() = true;
		LookupOperation::meta_ComponentFactory_parent.target() = ComponentFactory::meta;
		ComponentFactory::meta_LookupOperation_children = Uml::CompositionChildRole::Create(comp);
		ComponentFactory::meta_LookupOperation_children.name() = "LookupOperation";
		ComponentFactory::meta_LookupOperation_children.min() = 0;
		ComponentFactory::meta_LookupOperation_children.max() = -1;
		ComponentFactory::meta_LookupOperation_children.isNavigable() = true;
		ComponentFactory::meta_LookupOperation_children.target() = LookupOperation::meta;
	}
	void InitComposition150()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		TwowayOperation::meta_HasOperations_parent = Uml::CompositionParentRole::Create(comp);
		TwowayOperation::meta_HasOperations_parent.name() = "HasOperations_parent";
		TwowayOperation::meta_HasOperations_parent.isNavigable() = true;
		TwowayOperation::meta_HasOperations_parent.target() = HasOperations::meta;
		HasOperations::meta_TwowayOperation_children = Uml::CompositionChildRole::Create(comp);
		HasOperations::meta_TwowayOperation_children.name() = "TwowayOperation";
		HasOperations::meta_TwowayOperation_children.min() = 0;
		HasOperations::meta_TwowayOperation_children.max() = -1;
		HasOperations::meta_TwowayOperation_children.isNavigable() = true;
		HasOperations::meta_TwowayOperation_children.target() = TwowayOperation::meta;
	}
	void InitComposition151()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		OnewayOperation::meta_HasOperations_parent = Uml::CompositionParentRole::Create(comp);
		OnewayOperation::meta_HasOperations_parent.name() = "HasOperations_parent";
		OnewayOperation::meta_HasOperations_parent.isNavigable() = true;
		OnewayOperation::meta_HasOperations_parent.target() = HasOperations::meta;
		HasOperations::meta_OnewayOperation_children = Uml::CompositionChildRole::Create(comp);
		HasOperations::meta_OnewayOperation_children.name() = "OnewayOperation";
		HasOperations::meta_OnewayOperation_children.min() = 0;
		HasOperations::meta_OnewayOperation_children.max() = -1;
		HasOperations::meta_OnewayOperation_children.isNavigable() = true;
		HasOperations::meta_OnewayOperation_children.target() = OnewayOperation::meta;
	}
	void InitComposition152()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		Exception::meta_HasOperations_parent = Uml::CompositionParentRole::Create(comp);
		Exception::meta_HasOperations_parent.name() = "HasOperations_parent";
		Exception::meta_HasOperations_parent.isNavigable() = true;
		Exception::meta_HasOperations_parent.target() = HasOperations::meta;
		HasOperations::meta_Exception_children = Uml::CompositionChildRole::Create(comp);
		HasOperations::meta_Exception_children.name() = "Exception";
		HasOperations::meta_Exception_children.min() = 0;
		HasOperations::meta_Exception_children.max() = -1;
		HasOperations::meta_Exception_children.isNavigable() = true;
		HasOperations::meta_Exception_children.target() = Exception::meta;
	}
	void InitComposition153()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		Constant::meta_HasOperations_parent = Uml::CompositionParentRole::Create(comp);
		Constant::meta_HasOperations_parent.name() = "HasOperations_parent";
		Constant::meta_HasOperations_parent.isNavigable() = true;
		Constant::meta_HasOperations_parent.target() = HasOperations::meta;
		HasOperations::meta_Constant_children = Uml::CompositionChildRole::Create(comp);
		HasOperations::meta_Constant_children.name() = "Constant";
		HasOperations::meta_Constant_children.min() = 0;
		HasOperations::meta_Constant_children.max() = -1;
		HasOperations::meta_Constant_children.isNavigable() = true;
		HasOperations::meta_Constant_children.target() = Constant::meta;
	}
	void InitComposition154()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		NoInheritable::meta_HasOperations_parent = Uml::CompositionParentRole::Create(comp);
		NoInheritable::meta_HasOperations_parent.name() = "HasOperations_parent";
		NoInheritable::meta_HasOperations_parent.isNavigable() = true;
		NoInheritable::meta_HasOperations_parent.target() = HasOperations::meta;
		HasOperations::meta_NoInheritable_children = Uml::CompositionChildRole::Create(comp);
		HasOperations::meta_NoInheritable_children.name() = "NoInheritable";
		HasOperations::meta_NoInheritable_children.min() = 0;
		HasOperations::meta_NoInheritable_children.max() = -1;
		HasOperations::meta_NoInheritable_children.isNavigable() = true;
		HasOperations::meta_NoInheritable_children.target() = NoInheritable::meta;
	}
	void InitComposition155()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		Supports::meta_SupportsInterfaces_parent = Uml::CompositionParentRole::Create(comp);
		Supports::meta_SupportsInterfaces_parent.name() = "SupportsInterfaces_parent";
		Supports::meta_SupportsInterfaces_parent.isNavigable() = true;
		Supports::meta_SupportsInterfaces_parent.target() = SupportsInterfaces::meta;
		SupportsInterfaces::meta_Supports_children = Uml::CompositionChildRole::Create(comp);
		SupportsInterfaces::meta_Supports_children.name() = "Supports";
		SupportsInterfaces::meta_Supports_children.min() = 0;
		SupportsInterfaces::meta_Supports_children.max() = -1;
		SupportsInterfaces::meta_Supports_children.isNavigable() = true;
		SupportsInterfaces::meta_Supports_children.target() = Supports::meta;
	}
	void InitComposition156()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		ReadonlyAttribute::meta_Inheritable_parent = Uml::CompositionParentRole::Create(comp);
		ReadonlyAttribute::meta_Inheritable_parent.name() = "Inheritable_parent";
		ReadonlyAttribute::meta_Inheritable_parent.isNavigable() = true;
		ReadonlyAttribute::meta_Inheritable_parent.target() = Inheritable::meta;
		Inheritable::meta_ReadonlyAttribute_children = Uml::CompositionChildRole::Create(comp);
		Inheritable::meta_ReadonlyAttribute_children.name() = "ReadonlyAttribute";
		Inheritable::meta_ReadonlyAttribute_children.min() = 0;
		Inheritable::meta_ReadonlyAttribute_children.max() = -1;
		Inheritable::meta_ReadonlyAttribute_children.isNavigable() = true;
		Inheritable::meta_ReadonlyAttribute_children.target() = ReadonlyAttribute::meta;
	}
	void InitComposition157()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		Inherits::meta_Inheritable_parent = Uml::CompositionParentRole::Create(comp);
		Inherits::meta_Inheritable_parent.name() = "Inheritable_parent";
		Inherits::meta_Inheritable_parent.isNavigable() = true;
		Inherits::meta_Inheritable_parent.target() = Inheritable::meta;
		Inheritable::meta_Inherits_children = Uml::CompositionChildRole::Create(comp);
		Inheritable::meta_Inherits_children.name() = "Inherits";
		Inheritable::meta_Inherits_children.min() = 0;
		Inheritable::meta_Inherits_children.max() = -1;
		Inheritable::meta_Inherits_children.isNavigable() = true;
		Inheritable::meta_Inherits_children.target() = Inherits::meta;
	}
	void InitComposition158()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		NamedType::meta_File_parent = Uml::CompositionParentRole::Create(comp);
		NamedType::meta_File_parent.name() = "File_parent";
		NamedType::meta_File_parent.isNavigable() = true;
		NamedType::meta_File_parent.target() = File::meta;
		File::meta_NamedType_children = Uml::CompositionChildRole::Create(comp);
		File::meta_NamedType_children.name() = "NamedType";
		File::meta_NamedType_children.min() = 0;
		File::meta_NamedType_children.max() = -1;
		File::meta_NamedType_children.isNavigable() = true;
		File::meta_NamedType_children.target() = NamedType::meta;
	}
	void InitComposition159()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		NamedType::meta_Package_parent = Uml::CompositionParentRole::Create(comp);
		NamedType::meta_Package_parent.name() = "Package_parent";
		NamedType::meta_Package_parent.isNavigable() = true;
		NamedType::meta_Package_parent.target() = Package::meta;
		Package::meta_NamedType_children = Uml::CompositionChildRole::Create(comp);
		Package::meta_NamedType_children.name() = "NamedType";
		Package::meta_NamedType_children.min() = 0;
		Package::meta_NamedType_children.max() = -1;
		Package::meta_NamedType_children.isNavigable() = true;
		Package::meta_NamedType_children.target() = NamedType::meta;
	}
	void InitComposition160()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		ComponentRef::meta_Package_parent = Uml::CompositionParentRole::Create(comp);
		ComponentRef::meta_Package_parent.name() = "Package_parent";
		ComponentRef::meta_Package_parent.isNavigable() = true;
		ComponentRef::meta_Package_parent.target() = Package::meta;
		Package::meta_ComponentRef_children = Uml::CompositionChildRole::Create(comp);
		Package::meta_ComponentRef_children.name() = "ComponentRef";
		Package::meta_ComponentRef_children.min() = 0;
		Package::meta_ComponentRef_children.max() = -1;
		Package::meta_ComponentRef_children.isNavigable() = true;
		Package::meta_ComponentRef_children.target() = ComponentRef::meta;
	}
	void InitComposition161()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		ComponentRef::meta_File_parent = Uml::CompositionParentRole::Create(comp);
		ComponentRef::meta_File_parent.name() = "File_parent";
		ComponentRef::meta_File_parent.isNavigable() = true;
		ComponentRef::meta_File_parent.target() = File::meta;
		File::meta_ComponentRef_children = Uml::CompositionChildRole::Create(comp);
		File::meta_ComponentRef_children.name() = "ComponentRef";
		File::meta_ComponentRef_children.min() = 0;
		File::meta_ComponentRef_children.max() = -1;
		File::meta_ComponentRef_children.isNavigable() = true;
		File::meta_ComponentRef_children.target() = ComponentRef::meta;
	}
	void InitComposition162()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		ManagesComponent::meta_File_parent = Uml::CompositionParentRole::Create(comp);
		ManagesComponent::meta_File_parent.name() = "File_parent";
		ManagesComponent::meta_File_parent.isNavigable() = true;
		ManagesComponent::meta_File_parent.target() = File::meta;
		File::meta_ManagesComponent_children = Uml::CompositionChildRole::Create(comp);
		File::meta_ManagesComponent_children.name() = "ManagesComponent";
		File::meta_ManagesComponent_children.min() = 0;
		File::meta_ManagesComponent_children.max() = -1;
		File::meta_ManagesComponent_children.isNavigable() = true;
		File::meta_ManagesComponent_children.target() = ManagesComponent::meta;
	}
	void InitComposition163()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		ManagesComponent::meta_Package_parent = Uml::CompositionParentRole::Create(comp);
		ManagesComponent::meta_Package_parent.name() = "Package_parent";
		ManagesComponent::meta_Package_parent.isNavigable() = true;
		ManagesComponent::meta_Package_parent.target() = Package::meta;
		Package::meta_ManagesComponent_children = Uml::CompositionChildRole::Create(comp);
		Package::meta_ManagesComponent_children.name() = "ManagesComponent";
		Package::meta_ManagesComponent_children.min() = 0;
		Package::meta_ManagesComponent_children.max() = -1;
		Package::meta_ManagesComponent_children.isNavigable() = true;
		Package::meta_ManagesComponent_children.target() = ManagesComponent::meta;
	}
	void InitComposition164()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		ReadonlyAttribute::meta_Component_parent = Uml::CompositionParentRole::Create(comp);
		ReadonlyAttribute::meta_Component_parent.name() = "Component_parent";
		ReadonlyAttribute::meta_Component_parent.isNavigable() = true;
		ReadonlyAttribute::meta_Component_parent.target() = Component::meta;
		Component::meta_ReadonlyAttribute_children = Uml::CompositionChildRole::Create(comp);
		Component::meta_ReadonlyAttribute_children.name() = "ReadonlyAttribute";
		Component::meta_ReadonlyAttribute_children.min() = 0;
		Component::meta_ReadonlyAttribute_children.max() = -1;
		Component::meta_ReadonlyAttribute_children.isNavigable() = true;
		Component::meta_ReadonlyAttribute_children.target() = ReadonlyAttribute::meta;
	}
	void InitComposition165()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		InParameter::meta_OperationBase_parent = Uml::CompositionParentRole::Create(comp);
		InParameter::meta_OperationBase_parent.name() = "OperationBase_parent";
		InParameter::meta_OperationBase_parent.isNavigable() = true;
		InParameter::meta_OperationBase_parent.target() = OperationBase::meta;
		OperationBase::meta_InParameter_children = Uml::CompositionChildRole::Create(comp);
		OperationBase::meta_InParameter_children.name() = "InParameter";
		OperationBase::meta_InParameter_children.min() = 0;
		OperationBase::meta_InParameter_children.max() = -1;
		OperationBase::meta_InParameter_children.isNavigable() = true;
		OperationBase::meta_InParameter_children.target() = InParameter::meta;
	}
	void InitComposition166()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		ExceptionRef::meta_HasExceptions_parent = Uml::CompositionParentRole::Create(comp);
		ExceptionRef::meta_HasExceptions_parent.name() = "HasExceptions_parent";
		ExceptionRef::meta_HasExceptions_parent.isNavigable() = true;
		ExceptionRef::meta_HasExceptions_parent.target() = HasExceptions::meta;
		HasExceptions::meta_ExceptionRef_children = Uml::CompositionChildRole::Create(comp);
		HasExceptions::meta_ExceptionRef_children.name() = "ExceptionRef";
		HasExceptions::meta_ExceptionRef_children.min() = 0;
		HasExceptions::meta_ExceptionRef_children.max() = -1;
		HasExceptions::meta_ExceptionRef_children.isNavigable() = true;
		HasExceptions::meta_ExceptionRef_children.target() = ExceptionRef::meta;
	}
	void InitComposition167()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		FactoryOperation::meta_ObjectByValue_parent = Uml::CompositionParentRole::Create(comp);
		FactoryOperation::meta_ObjectByValue_parent.name() = "ObjectByValue_parent";
		FactoryOperation::meta_ObjectByValue_parent.isNavigable() = true;
		FactoryOperation::meta_ObjectByValue_parent.target() = ObjectByValue::meta;
		ObjectByValue::meta_FactoryOperation_children = Uml::CompositionChildRole::Create(comp);
		ObjectByValue::meta_FactoryOperation_children.name() = "FactoryOperation";
		ObjectByValue::meta_FactoryOperation_children.min() = 0;
		ObjectByValue::meta_FactoryOperation_children.max() = -1;
		ObjectByValue::meta_FactoryOperation_children.isNavigable() = true;
		ObjectByValue::meta_FactoryOperation_children.target() = FactoryOperation::meta;
	}
	void InitComposition168()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		InoutParameter::meta_TwowayOperation_parent = Uml::CompositionParentRole::Create(comp);
		InoutParameter::meta_TwowayOperation_parent.name() = "TwowayOperation_parent";
		InoutParameter::meta_TwowayOperation_parent.isNavigable() = true;
		InoutParameter::meta_TwowayOperation_parent.target() = TwowayOperation::meta;
		TwowayOperation::meta_InoutParameter_children = Uml::CompositionChildRole::Create(comp);
		TwowayOperation::meta_InoutParameter_children.name() = "InoutParameter";
		TwowayOperation::meta_InoutParameter_children.min() = 0;
		TwowayOperation::meta_InoutParameter_children.max() = -1;
		TwowayOperation::meta_InoutParameter_children.isNavigable() = true;
		TwowayOperation::meta_InoutParameter_children.target() = InoutParameter::meta;
	}
	void InitComposition169()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		OutParameter::meta_TwowayOperation_parent = Uml::CompositionParentRole::Create(comp);
		OutParameter::meta_TwowayOperation_parent.name() = "TwowayOperation_parent";
		OutParameter::meta_TwowayOperation_parent.isNavigable() = true;
		OutParameter::meta_TwowayOperation_parent.target() = TwowayOperation::meta;
		TwowayOperation::meta_OutParameter_children = Uml::CompositionChildRole::Create(comp);
		TwowayOperation::meta_OutParameter_children.name() = "OutParameter";
		TwowayOperation::meta_OutParameter_children.min() = 0;
		TwowayOperation::meta_OutParameter_children.max() = -1;
		TwowayOperation::meta_OutParameter_children.isNavigable() = true;
		TwowayOperation::meta_OutParameter_children.target() = OutParameter::meta;
	}
	void InitComposition170()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		ReturnType::meta_TwowayOperation_parent = Uml::CompositionParentRole::Create(comp);
		ReturnType::meta_TwowayOperation_parent.name() = "TwowayOperation_parent";
		ReturnType::meta_TwowayOperation_parent.isNavigable() = true;
		ReturnType::meta_TwowayOperation_parent.target() = TwowayOperation::meta;
		TwowayOperation::meta_ReturnType_child = Uml::CompositionChildRole::Create(comp);
		TwowayOperation::meta_ReturnType_child.name() = "ReturnType";
		TwowayOperation::meta_ReturnType_child.min() = 0;
		TwowayOperation::meta_ReturnType_child.max() = 1;
		TwowayOperation::meta_ReturnType_child.isNavigable() = true;
		TwowayOperation::meta_ReturnType_child.target() = ReturnType::meta;
	}
	void InitComposition171()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		PrivateFlag::meta_ObjectByValue_parent = Uml::CompositionParentRole::Create(comp);
		PrivateFlag::meta_ObjectByValue_parent.name() = "ObjectByValue_parent";
		PrivateFlag::meta_ObjectByValue_parent.isNavigable() = true;
		PrivateFlag::meta_ObjectByValue_parent.target() = ObjectByValue::meta;
		ObjectByValue::meta_PrivateFlag_children = Uml::CompositionChildRole::Create(comp);
		ObjectByValue::meta_PrivateFlag_children.name() = "PrivateFlag";
		ObjectByValue::meta_PrivateFlag_children.min() = 0;
		ObjectByValue::meta_PrivateFlag_children.max() = -1;
		ObjectByValue::meta_PrivateFlag_children.isNavigable() = true;
		ObjectByValue::meta_PrivateFlag_children.target() = PrivateFlag::meta;
	}
	void InitComposition172()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		MakeMemberPrivate::meta_ObjectByValue_parent = Uml::CompositionParentRole::Create(comp);
		MakeMemberPrivate::meta_ObjectByValue_parent.name() = "ObjectByValue_parent";
		MakeMemberPrivate::meta_ObjectByValue_parent.isNavigable() = true;
		MakeMemberPrivate::meta_ObjectByValue_parent.target() = ObjectByValue::meta;
		ObjectByValue::meta_MakeMemberPrivate_children = Uml::CompositionChildRole::Create(comp);
		ObjectByValue::meta_MakeMemberPrivate_children.name() = "MakeMemberPrivate";
		ObjectByValue::meta_MakeMemberPrivate_children.min() = 0;
		ObjectByValue::meta_MakeMemberPrivate_children.max() = -1;
		ObjectByValue::meta_MakeMemberPrivate_children.isNavigable() = true;
		ObjectByValue::meta_MakeMemberPrivate_children.target() = MakeMemberPrivate::meta;
	}
	void InitComposition173()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		Member::meta_ObjectByValue_parent = Uml::CompositionParentRole::Create(comp);
		Member::meta_ObjectByValue_parent.name() = "ObjectByValue_parent";
		Member::meta_ObjectByValue_parent.isNavigable() = true;
		Member::meta_ObjectByValue_parent.target() = ObjectByValue::meta;
		ObjectByValue::meta_Member_children = Uml::CompositionChildRole::Create(comp);
		ObjectByValue::meta_Member_children.name() = "Member";
		ObjectByValue::meta_Member_children.min() = 0;
		ObjectByValue::meta_Member_children.max() = -1;
		ObjectByValue::meta_Member_children.isNavigable() = true;
		ObjectByValue::meta_Member_children.target() = Member::meta;
	}
	void InitComposition174()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		AttributeMember::meta_ReadonlyAttribute_parent = Uml::CompositionParentRole::Create(comp);
		AttributeMember::meta_ReadonlyAttribute_parent.name() = "ReadonlyAttribute_parent";
		AttributeMember::meta_ReadonlyAttribute_parent.isNavigable() = true;
		AttributeMember::meta_ReadonlyAttribute_parent.target() = ReadonlyAttribute::meta;
		ReadonlyAttribute::meta_AttributeMember_child = Uml::CompositionChildRole::Create(comp);
		ReadonlyAttribute::meta_AttributeMember_child.name() = "AttributeMember";
		ReadonlyAttribute::meta_AttributeMember_child.min() = 1;
		ReadonlyAttribute::meta_AttributeMember_child.max() = 1;
		ReadonlyAttribute::meta_AttributeMember_child.isNavigable() = true;
		ReadonlyAttribute::meta_AttributeMember_child.target() = AttributeMember::meta;
	}
	void InitComposition175()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		GetException::meta_ReadonlyAttribute_parent = Uml::CompositionParentRole::Create(comp);
		GetException::meta_ReadonlyAttribute_parent.name() = "ReadonlyAttribute_parent";
		GetException::meta_ReadonlyAttribute_parent.isNavigable() = true;
		GetException::meta_ReadonlyAttribute_parent.target() = ReadonlyAttribute::meta;
		ReadonlyAttribute::meta_GetException_children = Uml::CompositionChildRole::Create(comp);
		ReadonlyAttribute::meta_GetException_children.name() = "GetException";
		ReadonlyAttribute::meta_GetException_children.min() = 0;
		ReadonlyAttribute::meta_GetException_children.max() = -1;
		ReadonlyAttribute::meta_GetException_children.isNavigable() = true;
		ReadonlyAttribute::meta_GetException_children.target() = GetException::meta;
	}
	void InitComposition176()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		SetException::meta_Attribute_parent = Uml::CompositionParentRole::Create(comp);
		SetException::meta_Attribute_parent.name() = "Attribute_parent";
		SetException::meta_Attribute_parent.isNavigable() = true;
		SetException::meta_Attribute_parent.target() = Attribute::meta;
		Attribute::meta_SetException_children = Uml::CompositionChildRole::Create(comp);
		Attribute::meta_SetException_children.name() = "SetException";
		Attribute::meta_SetException_children.min() = 0;
		Attribute::meta_SetException_children.max() = -1;
		Attribute::meta_SetException_children.isNavigable() = true;
		Attribute::meta_SetException_children.target() = SetException::meta;
	}
	void InitComposition177()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		EnumValue::meta_Enum_parent = Uml::CompositionParentRole::Create(comp);
		EnumValue::meta_Enum_parent.name() = "Enum_parent";
		EnumValue::meta_Enum_parent.isNavigable() = true;
		EnumValue::meta_Enum_parent.target() = Enum::meta;
		Enum::meta_EnumValue_children = Uml::CompositionChildRole::Create(comp);
		Enum::meta_EnumValue_children.name() = "EnumValue";
		Enum::meta_EnumValue_children.min() = 1;
		Enum::meta_EnumValue_children.max() = -1;
		Enum::meta_EnumValue_children.isNavigable() = true;
		Enum::meta_EnumValue_children.target() = EnumValue::meta;
	}
	void InitComposition178()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		Member::meta_Aggregate_parent = Uml::CompositionParentRole::Create(comp);
		Member::meta_Aggregate_parent.name() = "Aggregate_parent";
		Member::meta_Aggregate_parent.isNavigable() = true;
		Member::meta_Aggregate_parent.target() = Aggregate::meta;
		Aggregate::meta_Member_children = Uml::CompositionChildRole::Create(comp);
		Aggregate::meta_Member_children.name() = "Member";
		Aggregate::meta_Member_children.min() = 0;
		Aggregate::meta_Member_children.max() = -1;
		Aggregate::meta_Member_children.isNavigable() = true;
		Aggregate::meta_Member_children.target() = Member::meta;
	}
	void InitComposition179()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		Label::meta_SwitchedAggregate_parent = Uml::CompositionParentRole::Create(comp);
		Label::meta_SwitchedAggregate_parent.name() = "SwitchedAggregate_parent";
		Label::meta_SwitchedAggregate_parent.isNavigable() = true;
		Label::meta_SwitchedAggregate_parent.target() = SwitchedAggregate::meta;
		SwitchedAggregate::meta_Label_children = Uml::CompositionChildRole::Create(comp);
		SwitchedAggregate::meta_Label_children.name() = "Label";
		SwitchedAggregate::meta_Label_children.min() = 1;
		SwitchedAggregate::meta_Label_children.max() = -1;
		SwitchedAggregate::meta_Label_children.isNavigable() = true;
		SwitchedAggregate::meta_Label_children.target() = Label::meta;
	}
	void InitComposition180()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		LabelConnection::meta_SwitchedAggregate_parent = Uml::CompositionParentRole::Create(comp);
		LabelConnection::meta_SwitchedAggregate_parent.name() = "SwitchedAggregate_parent";
		LabelConnection::meta_SwitchedAggregate_parent.isNavigable() = true;
		LabelConnection::meta_SwitchedAggregate_parent.target() = SwitchedAggregate::meta;
		SwitchedAggregate::meta_LabelConnection_children = Uml::CompositionChildRole::Create(comp);
		SwitchedAggregate::meta_LabelConnection_children.name() = "LabelConnection";
		SwitchedAggregate::meta_LabelConnection_children.min() = 1;
		SwitchedAggregate::meta_LabelConnection_children.max() = -1;
		SwitchedAggregate::meta_LabelConnection_children.isNavigable() = true;
		SwitchedAggregate::meta_LabelConnection_children.target() = LabelConnection::meta;
	}
	void InitComposition181()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		Member::meta_SwitchedAggregate_parent = Uml::CompositionParentRole::Create(comp);
		Member::meta_SwitchedAggregate_parent.name() = "SwitchedAggregate_parent";
		Member::meta_SwitchedAggregate_parent.isNavigable() = true;
		Member::meta_SwitchedAggregate_parent.target() = SwitchedAggregate::meta;
		SwitchedAggregate::meta_Member_children = Uml::CompositionChildRole::Create(comp);
		SwitchedAggregate::meta_Member_children.name() = "Member";
		SwitchedAggregate::meta_Member_children.min() = 0;
		SwitchedAggregate::meta_Member_children.max() = -1;
		SwitchedAggregate::meta_Member_children.isNavigable() = true;
		SwitchedAggregate::meta_Member_children.target() = Member::meta;
	}
	void InitComposition182()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		Discriminator::meta_SwitchedAggregate_parent = Uml::CompositionParentRole::Create(comp);
		Discriminator::meta_SwitchedAggregate_parent.name() = "SwitchedAggregate_parent";
		Discriminator::meta_SwitchedAggregate_parent.isNavigable() = true;
		Discriminator::meta_SwitchedAggregate_parent.target() = SwitchedAggregate::meta;
		SwitchedAggregate::meta_Discriminator_child = Uml::CompositionChildRole::Create(comp);
		SwitchedAggregate::meta_Discriminator_child.name() = "Discriminator";
		SwitchedAggregate::meta_Discriminator_child.min() = 1;
		SwitchedAggregate::meta_Discriminator_child.max() = 1;
		SwitchedAggregate::meta_Discriminator_child.isNavigable() = true;
		SwitchedAggregate::meta_Discriminator_child.target() = Discriminator::meta;
	}
	void InitComposition183()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		Member::meta_Exception_parent = Uml::CompositionParentRole::Create(comp);
		Member::meta_Exception_parent.name() = "Exception_parent";
		Member::meta_Exception_parent.isNavigable() = true;
		Member::meta_Exception_parent.target() = Exception::meta;
		Exception::meta_Member_children = Uml::CompositionChildRole::Create(comp);
		Exception::meta_Member_children.name() = "Member";
		Exception::meta_Member_children.min() = 0;
		Exception::meta_Member_children.max() = -1;
		Exception::meta_Member_children.isNavigable() = true;
		Exception::meta_Member_children.target() = Member::meta;
	}
	void InitComposition184()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		PredefinedType::meta_PredefinedTypes_parent = Uml::CompositionParentRole::Create(comp);
		PredefinedType::meta_PredefinedTypes_parent.name() = "PredefinedTypes_parent";
		PredefinedType::meta_PredefinedTypes_parent.isNavigable() = true;
		PredefinedType::meta_PredefinedTypes_parent.target() = PredefinedTypes::meta;
		PredefinedTypes::meta_PredefinedType_children = Uml::CompositionChildRole::Create(comp);
		PredefinedTypes::meta_PredefinedType_children.name() = "PredefinedType";
		PredefinedTypes::meta_PredefinedType_children.min() = 0;
		PredefinedTypes::meta_PredefinedType_children.max() = -1;
		PredefinedTypes::meta_PredefinedType_children.isNavigable() = true;
		PredefinedTypes::meta_PredefinedType_children.target() = PredefinedType::meta;
	}
	void InitComposition185()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		PredefinedTypes::meta_RootFolder_parent = Uml::CompositionParentRole::Create(comp);
		PredefinedTypes::meta_RootFolder_parent.name() = "RootFolder_parent";
		PredefinedTypes::meta_RootFolder_parent.isNavigable() = true;
		PredefinedTypes::meta_RootFolder_parent.target() = RootFolder::meta;
		RootFolder::meta_PredefinedTypes_children = Uml::CompositionChildRole::Create(comp);
		RootFolder::meta_PredefinedTypes_children.name() = "PredefinedTypes";
		RootFolder::meta_PredefinedTypes_children.min() = 0;
		RootFolder::meta_PredefinedTypes_children.max() = -1;
		RootFolder::meta_PredefinedTypes_children.isNavigable() = true;
		RootFolder::meta_PredefinedTypes_children.target() = PredefinedTypes::meta;
	}
	void InitComposition186()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		Exception::meta_Package_parent = Uml::CompositionParentRole::Create(comp);
		Exception::meta_Package_parent.name() = "Package_parent";
		Exception::meta_Package_parent.isNavigable() = true;
		Exception::meta_Package_parent.target() = Package::meta;
		Package::meta_Exception_children = Uml::CompositionChildRole::Create(comp);
		Package::meta_Exception_children.name() = "Exception";
		Package::meta_Exception_children.min() = 0;
		Package::meta_Exception_children.max() = -1;
		Package::meta_Exception_children.isNavigable() = true;
		Package::meta_Exception_children.target() = Exception::meta;
	}
	void InitComposition187()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		Package::meta_Package_parent = Uml::CompositionParentRole::Create(comp);
		Package::meta_Package_parent.name() = "Package_parent";
		Package::meta_Package_parent.isNavigable() = true;
		Package::meta_Package_parent.target() = Package::meta;
		Package::meta_Package_children = Uml::CompositionChildRole::Create(comp);
		Package::meta_Package_children.name() = "Package";
		Package::meta_Package_children.min() = 0;
		Package::meta_Package_children.max() = -1;
		Package::meta_Package_children.isNavigable() = true;
		Package::meta_Package_children.target() = Package::meta;
	}
	void InitComposition188()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		Constant::meta_Package_parent = Uml::CompositionParentRole::Create(comp);
		Constant::meta_Package_parent.name() = "Package_parent";
		Constant::meta_Package_parent.isNavigable() = true;
		Constant::meta_Package_parent.target() = Package::meta;
		Package::meta_Constant_children = Uml::CompositionChildRole::Create(comp);
		Package::meta_Constant_children.name() = "Constant";
		Package::meta_Constant_children.min() = 0;
		Package::meta_Constant_children.max() = -1;
		Package::meta_Constant_children.isNavigable() = true;
		Package::meta_Constant_children.target() = Constant::meta;
	}
	void InitComposition189()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		Package::meta_File_parent = Uml::CompositionParentRole::Create(comp);
		Package::meta_File_parent.name() = "File_parent";
		Package::meta_File_parent.isNavigable() = true;
		Package::meta_File_parent.target() = File::meta;
		File::meta_Package_children = Uml::CompositionChildRole::Create(comp);
		File::meta_Package_children.name() = "Package";
		File::meta_Package_children.min() = 0;
		File::meta_Package_children.max() = -1;
		File::meta_Package_children.isNavigable() = true;
		File::meta_Package_children.target() = Package::meta;
	}
	void InitComposition190()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		Exception::meta_File_parent = Uml::CompositionParentRole::Create(comp);
		Exception::meta_File_parent.name() = "File_parent";
		Exception::meta_File_parent.isNavigable() = true;
		Exception::meta_File_parent.target() = File::meta;
		File::meta_Exception_children = Uml::CompositionChildRole::Create(comp);
		File::meta_Exception_children.name() = "Exception";
		File::meta_Exception_children.min() = 0;
		File::meta_Exception_children.max() = -1;
		File::meta_Exception_children.isNavigable() = true;
		File::meta_Exception_children.target() = Exception::meta;
	}
	void InitComposition191()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		FileRef::meta_File_parent = Uml::CompositionParentRole::Create(comp);
		FileRef::meta_File_parent.name() = "File_parent";
		FileRef::meta_File_parent.isNavigable() = true;
		FileRef::meta_File_parent.target() = File::meta;
		File::meta_FileRef_children = Uml::CompositionChildRole::Create(comp);
		File::meta_FileRef_children.name() = "FileRef";
		File::meta_FileRef_children.min() = 0;
		File::meta_FileRef_children.max() = -1;
		File::meta_FileRef_children.isNavigable() = true;
		File::meta_FileRef_children.target() = FileRef::meta;
	}
	void InitComposition192()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		Constant::meta_File_parent = Uml::CompositionParentRole::Create(comp);
		Constant::meta_File_parent.name() = "File_parent";
		Constant::meta_File_parent.isNavigable() = true;
		Constant::meta_File_parent.target() = File::meta;
		File::meta_Constant_children = Uml::CompositionChildRole::Create(comp);
		File::meta_Constant_children.name() = "Constant";
		File::meta_Constant_children.min() = 0;
		File::meta_Constant_children.max() = -1;
		File::meta_Constant_children.isNavigable() = true;
		File::meta_Constant_children.target() = Constant::meta;
	}
	void InitComposition193()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		File::meta_InterfaceDefinitions_parent = Uml::CompositionParentRole::Create(comp);
		File::meta_InterfaceDefinitions_parent.name() = "InterfaceDefinitions_parent";
		File::meta_InterfaceDefinitions_parent.isNavigable() = true;
		File::meta_InterfaceDefinitions_parent.target() = InterfaceDefinitions::meta;
		InterfaceDefinitions::meta_File_children = Uml::CompositionChildRole::Create(comp);
		InterfaceDefinitions::meta_File_children.name() = "File";
		InterfaceDefinitions::meta_File_children.min() = 1;
		InterfaceDefinitions::meta_File_children.max() = -1;
		InterfaceDefinitions::meta_File_children.isNavigable() = true;
		InterfaceDefinitions::meta_File_children.target() = File::meta;
	}
	void InitComposition194()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		InterfaceDefinitions::meta_RootFolder_parent = Uml::CompositionParentRole::Create(comp);
		InterfaceDefinitions::meta_RootFolder_parent.name() = "RootFolder_parent";
		InterfaceDefinitions::meta_RootFolder_parent.isNavigable() = true;
		InterfaceDefinitions::meta_RootFolder_parent.target() = RootFolder::meta;
		RootFolder::meta_InterfaceDefinitions_children = Uml::CompositionChildRole::Create(comp);
		RootFolder::meta_InterfaceDefinitions_children.name() = "InterfaceDefinitions";
		RootFolder::meta_InterfaceDefinitions_children.min() = 0;
		RootFolder::meta_InterfaceDefinitions_children.max() = -1;
		RootFolder::meta_InterfaceDefinitions_children.isNavigable() = true;
		RootFolder::meta_InterfaceDefinitions_children.target() = InterfaceDefinitions::meta;
	}
	void InitComposition195()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		FileRef::meta_StubProject_parent = Uml::CompositionParentRole::Create(comp);
		FileRef::meta_StubProject_parent.name() = "StubProject_parent";
		FileRef::meta_StubProject_parent.isNavigable() = true;
		FileRef::meta_StubProject_parent.target() = StubProject::meta;
		StubProject::meta_FileRef_children = Uml::CompositionChildRole::Create(comp);
		StubProject::meta_FileRef_children.name() = "FileRef";
		StubProject::meta_FileRef_children.min() = 0;
		StubProject::meta_FileRef_children.max() = -1;
		StubProject::meta_FileRef_children.isNavigable() = true;
		StubProject::meta_FileRef_children.target() = FileRef::meta;
	}
	void InitComposition196()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		MPC::meta_ComponentBuild_parent = Uml::CompositionParentRole::Create(comp);
		MPC::meta_ComponentBuild_parent.name() = "ComponentBuild_parent";
		MPC::meta_ComponentBuild_parent.isNavigable() = true;
		MPC::meta_ComponentBuild_parent.target() = ComponentBuild::meta;
		ComponentBuild::meta_MPC_children = Uml::CompositionChildRole::Create(comp);
		ComponentBuild::meta_MPC_children.name() = "MPC";
		ComponentBuild::meta_MPC_children.min() = 0;
		ComponentBuild::meta_MPC_children.max() = -1;
		ComponentBuild::meta_MPC_children.isNavigable() = true;
		ComponentBuild::meta_MPC_children.target() = MPC::meta;
	}
	void InitComposition197()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		ComponentBuild::meta_RootFolder_parent = Uml::CompositionParentRole::Create(comp);
		ComponentBuild::meta_RootFolder_parent.name() = "RootFolder_parent";
		ComponentBuild::meta_RootFolder_parent.isNavigable() = true;
		ComponentBuild::meta_RootFolder_parent.target() = RootFolder::meta;
		RootFolder::meta_ComponentBuild_children = Uml::CompositionChildRole::Create(comp);
		RootFolder::meta_ComponentBuild_children.name() = "ComponentBuild";
		RootFolder::meta_ComponentBuild_children.min() = 0;
		RootFolder::meta_ComponentBuild_children.max() = -1;
		RootFolder::meta_ComponentBuild_children.isNavigable() = true;
		RootFolder::meta_ComponentBuild_children.target() = ComponentBuild::meta;
	}
	void InitComposition198()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		ExtResourceConn::meta_Project_parent = Uml::CompositionParentRole::Create(comp);
		ExtResourceConn::meta_Project_parent.name() = "Project_parent";
		ExtResourceConn::meta_Project_parent.isNavigable() = true;
		ExtResourceConn::meta_Project_parent.target() = Project::meta;
		Project::meta_ExtResourceConn_children = Uml::CompositionChildRole::Create(comp);
		Project::meta_ExtResourceConn_children.name() = "ExtResourceConn";
		Project::meta_ExtResourceConn_children.min() = 0;
		Project::meta_ExtResourceConn_children.max() = -1;
		Project::meta_ExtResourceConn_children.isNavigable() = true;
		Project::meta_ExtResourceConn_children.target() = ExtResourceConn::meta;
	}
	void InitComposition199()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		ExternalResources::meta_Project_parent = Uml::CompositionParentRole::Create(comp);
		ExternalResources::meta_Project_parent.name() = "Project_parent";
		ExternalResources::meta_Project_parent.isNavigable() = true;
		ExternalResources::meta_Project_parent.target() = Project::meta;
		Project::meta_ExternalResources_children = Uml::CompositionChildRole::Create(comp);
		Project::meta_ExternalResources_children.name() = "ExternalResources";
		Project::meta_ExternalResources_children.min() = 0;
		Project::meta_ExternalResources_children.max() = -1;
		Project::meta_ExternalResources_children.isNavigable() = true;
		Project::meta_ExternalResources_children.target() = ExternalResources::meta;
	}
	void InitComposition200()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		ComponentLib::meta_Project_parent = Uml::CompositionParentRole::Create(comp);
		ComponentLib::meta_Project_parent.name() = "Project_parent";
		ComponentLib::meta_Project_parent.isNavigable() = true;
		ComponentLib::meta_Project_parent.target() = Project::meta;
		Project::meta_ComponentLib_children = Uml::CompositionChildRole::Create(comp);
		Project::meta_ComponentLib_children.name() = "ComponentLib";
		Project::meta_ComponentLib_children.min() = 0;
		Project::meta_ComponentLib_children.max() = -1;
		Project::meta_ComponentLib_children.isNavigable() = true;
		Project::meta_ComponentLib_children.target() = ComponentLib::meta;
	}
	void InitComposition201()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		Project::meta_MPC_parent = Uml::CompositionParentRole::Create(comp);
		Project::meta_MPC_parent.name() = "MPC_parent";
		Project::meta_MPC_parent.isNavigable() = true;
		Project::meta_MPC_parent.target() = MPC::meta;
		MPC::meta_Project_children = Uml::CompositionChildRole::Create(comp);
		MPC::meta_Project_children.name() = "Project";
		MPC::meta_Project_children.min() = 0;
		MPC::meta_Project_children.max() = -1;
		MPC::meta_Project_children.isNavigable() = true;
		MPC::meta_Project_children.target() = Project::meta;
	}
	void InitComposition202()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		Workspaces::meta_MPC_parent = Uml::CompositionParentRole::Create(comp);
		Workspaces::meta_MPC_parent.name() = "MPC_parent";
		Workspaces::meta_MPC_parent.isNavigable() = true;
		Workspaces::meta_MPC_parent.target() = MPC::meta;
		MPC::meta_Workspaces_children = Uml::CompositionChildRole::Create(comp);
		MPC::meta_Workspaces_children.name() = "Workspaces";
		MPC::meta_Workspaces_children.min() = 0;
		MPC::meta_Workspaces_children.max() = -1;
		MPC::meta_Workspaces_children.isNavigable() = true;
		MPC::meta_Workspaces_children.target() = Workspaces::meta;
	}
	void InitComposition203()
	{
		Uml::Composition comp = Uml::Composition::Create(umldiagram);
		comp.nonpersistent() =false;
		RootFolder::meta_RootFolder_parent = Uml::CompositionParentRole::Create(comp);
		RootFolder::meta_RootFolder_parent.name() = "RootFolder_parent";
		RootFolder::meta_RootFolder_parent.isNavigable() = true;
		RootFolder::meta_RootFolder_parent.target() = RootFolder::meta;
		RootFolder::meta_RootFolder_children = Uml::CompositionChildRole::Create(comp);
		RootFolder::meta_RootFolder_children.name() = "RootFolder";
		RootFolder::meta_RootFolder_children.min() = 0;
		RootFolder::meta_RootFolder_children.max() = -1;
		RootFolder::meta_RootFolder_children.isNavigable() = true;
		RootFolder::meta_RootFolder_children.target() = RootFolder::meta;
	}
	 void Initialize()
	{
		static bool first = true;
		if(!first) return;
		first = false;
		Uml::Initialize();

		Cadena2PICML::Initialize();

		ASSERT( umldiagram == Udm::null );
		UdmStatic::StaticDataNetwork * meta_dn = new UdmStatic::StaticDataNetwork(Uml::diagram);
		meta_dn->CreateNew("PICML.mem", "", Uml::Diagram::meta, Udm::CHANGES_LOST_DEFAULT);
		umldiagram = Uml::Diagram::Cast(meta_dn->GetRootObject());
		umldiagram.name() ="PICML";
		umldiagram.version() ="1.00";
		Creates();
		InitClassesAttributes();
		Uml::InitDiagram(umldiagram, "PICML", "1.00");

		InitAssociation0();
		InitAssociation1();
		InitAssociation2();
		InitAssociation3();
		InitAssociation4();
		InitAssociation5();
		InitAssociation6();
		InitAssociation7();
		InitAssociation8();
		InitAssociation9();
		InitAssociation10();
		InitAssociation11();
		InitAssociation12();
		InitAssociation13();
		InitAssociation14();
		InitAssociation15();
		InitAssociation16();
		InitAssociation17();
		InitAssociation18();
		InitAssociation19();
		InitAssociation20();
		InitAssociation21();
		InitAssociation22();
		InitAssociation23();
		InitAssociation24();
		InitAssociation25();
		InitAssociation26();
		InitAssociation27();
		InitAssociation28();
		InitAssociation29();
		InitAssociation30();
		InitAssociation31();
		InitAssociation32();
		InitAssociation33();
		InitAssociation34();
		InitAssociation35();
		InitAssociation36();
		InitAssociation37();
		InitAssociation38();
		InitAssociation39();
		InitAssociation40();
		InitAssociation41();
		InitAssociation42();
		InitAssociation43();
		InitAssociation44();
		InitAssociation45();
		InitAssociation46();
		InitAssociation47();
		InitAssociation48();
		InitAssociation49();
		InitAssociation50();
		InitAssociation51();
		InitAssociation52();
		InitAssociation53();
		InitAssociation54();
		InitAssociation55();
		InitAssociation56();
		InitAssociation57();
		InitAssociation58();
		InitAssociation59();
		InitAssociation60();
		InitAssociation61();
		InitAssociation62();
		InitAssociation63();
		InitAssociation64();
		InitAssociation65();
		InitAssociation66();
		InitAssociation67();
		InitAssociation68();
		InitAssociation69();
		InitAssociation70();
		InitAssociation71();
		InitAssociation72();
		InitAssociation73();
		InitAssociation74();
		InitAssociation75();
		InitAssociation76();
		InitAssociation77();
		InitAssociation78();
		InitAssociation79();
		InitAssociation80();
		InitAssociation81();
		InitAssociation82();
		InitAssociation83();
		InitAssociation84();
		InitAssociation85();
		InitAssociation86();
		InitAssociation87();
		InitAssociation88();
		InitAssociation89();
		InitAssociation90();
		InitAssociation91();
		invoke::meta_cadenaInvoke = Cadena2PICML::invoke_cross_ph_PICML::meta_cadenaInvoke;
		PublishConnector::meta_cadenaESSConnection = Cadena2PICML::PublishConnector_cross_ph_PICML::meta_cadenaESSConnection;
		emit::meta_cadenaEmit = Cadena2PICML::emit_cross_ph_PICML::meta_cadenaEmit;
		ComponentAssembly::meta_rootContainer = Cadena2PICML::ComponentAssembly_cross_ph_PICML::meta_rootContainer;
		ComponentAssembly::meta_cadenaScenario = Cadena2PICML::ComponentAssembly_cross_ph_PICML::meta_cadenaScenario;
		Property::meta_cadenaProperty = Cadena2PICML::Property_cross_ph_PICML::meta_cadenaProperty;
		Port::meta_cadenaPort = Cadena2PICML::Port_cross_ph_PICML::meta_cadenaPort;
		Component::meta_cadenaComponentInstance = Cadena2PICML::Component_cross_ph_PICML::meta_cadenaComponentInstance;
		InitComposition0();
		InitComposition1();
		InitComposition2();
		InitComposition3();
		InitComposition4();
		InitComposition5();
		InitComposition6();
		InitComposition7();
		InitComposition8();
		InitComposition9();
		InitComposition10();
		InitComposition11();
		InitComposition12();
		InitComposition13();
		InitComposition14();
		InitComposition15();
		InitComposition16();
		InitComposition17();
		InitComposition18();
		InitComposition19();
		InitComposition20();
		InitComposition21();
		InitComposition22();
		InitComposition23();
		InitComposition24();
		InitComposition25();
		InitComposition26();
		InitComposition27();
		InitComposition28();
		InitComposition29();
		InitComposition30();
		InitComposition31();
		InitComposition32();
		InitComposition33();
		InitComposition34();
		InitComposition35();
		InitComposition36();
		InitComposition37();
		InitComposition38();
		InitComposition39();
		InitComposition40();
		InitComposition41();
		InitComposition42();
		InitComposition43();
		InitComposition44();
		InitComposition45();
		InitComposition46();
		InitComposition47();
		InitComposition48();
		InitComposition49();
		InitComposition50();
		InitComposition51();
		InitComposition52();
		InitComposition53();
		InitComposition54();
		InitComposition55();
		InitComposition56();
		InitComposition57();
		InitComposition58();
		InitComposition59();
		InitComposition60();
		InitComposition61();
		InitComposition62();
		InitComposition63();
		InitComposition64();
		InitComposition65();
		InitComposition66();
		InitComposition67();
		InitComposition68();
		InitComposition69();
		InitComposition70();
		InitComposition71();
		InitComposition72();
		InitComposition73();
		InitComposition74();
		InitComposition75();
		InitComposition76();
		InitComposition77();
		InitComposition78();
		InitComposition79();
		InitComposition80();
		InitComposition81();
		InitComposition82();
		InitComposition83();
		InitComposition84();
		InitComposition85();
		InitComposition86();
		InitComposition87();
		InitComposition88();
		InitComposition89();
		InitComposition90();
		InitComposition91();
		InitComposition92();
		InitComposition93();
		InitComposition94();
		InitComposition95();
		InitComposition96();
		InitComposition97();
		InitComposition98();
		InitComposition99();
		InitComposition100();
		InitComposition101();
		InitComposition102();
		InitComposition103();
		InitComposition104();
		InitComposition105();
		InitComposition106();
		InitComposition107();
		InitComposition108();
		InitComposition109();
		InitComposition110();
		InitComposition111();
		InitComposition112();
		InitComposition113();
		InitComposition114();
		InitComposition115();
		InitComposition116();
		InitComposition117();
		InitComposition118();
		InitComposition119();
		InitComposition120();
		InitComposition121();
		InitComposition122();
		InitComposition123();
		InitComposition124();
		InitComposition125();
		InitComposition126();
		InitComposition127();
		InitComposition128();
		InitComposition129();
		InitComposition130();
		InitComposition131();
		InitComposition132();
		InitComposition133();
		InitComposition134();
		InitComposition135();
		InitComposition136();
		InitComposition137();
		InitComposition138();
		InitComposition139();
		InitComposition140();
		InitComposition141();
		InitComposition142();
		InitComposition143();
		InitComposition144();
		InitComposition145();
		InitComposition146();
		InitComposition147();
		InitComposition148();
		InitComposition149();
		InitComposition150();
		InitComposition151();
		InitComposition152();
		InitComposition153();
		InitComposition154();
		InitComposition155();
		InitComposition156();
		InitComposition157();
		InitComposition158();
		InitComposition159();
		InitComposition160();
		InitComposition161();
		InitComposition162();
		InitComposition163();
		InitComposition164();
		InitComposition165();
		InitComposition166();
		InitComposition167();
		InitComposition168();
		InitComposition169();
		InitComposition170();
		InitComposition171();
		InitComposition172();
		InitComposition173();
		InitComposition174();
		InitComposition175();
		InitComposition176();
		InitComposition177();
		InitComposition178();
		InitComposition179();
		InitComposition180();
		InitComposition181();
		InitComposition182();
		InitComposition183();
		InitComposition184();
		InitComposition185();
		InitComposition186();
		InitComposition187();
		InitComposition188();
		InitComposition189();
		InitComposition190();
		InitComposition191();
		InitComposition192();
		InitComposition193();
		InitComposition194();
		InitComposition195();
		InitComposition196();
		InitComposition197();
		InitComposition198();
		InitComposition199();
		InitComposition200();
		InitComposition201();
		InitComposition202();
		InitComposition203();
		RTEC_Proxy::meta.subTypes() +=  RTEC_Proxy_Supplier::meta;
		RTEC_Proxy::meta.subTypes() +=  RTEC_Proxy_Consumer::meta;
		Include_RTEC_Filter_Group_Members_Base::meta.subTypes() +=  RTEC_Filter_Group::meta;
		RTEC_Filter_Group::meta.subTypes() +=  LogicalANDGroup::meta;
		RTEC_Filter_Group::meta.subTypes() +=  DisjunctionGroup::meta;
		RTEC_Filter_Group::meta.subTypes() +=  ConjunctionGroup::meta;
		RT_Info_Param::meta.subTypes() +=  RT_Info::meta;
		RT_Info_Param::meta.subTypes() +=  Null_RT_Info::meta;
		Proxy::meta.subTypes() +=  Proxy_Consumer::meta;
		Proxy::meta.subTypes() +=  Proxy_Supplier::meta;
		Proxy_Consumer::meta.subTypes() +=  RTEC_Proxy_Consumer::meta;
		Proxy_Supplier::meta.subTypes() +=  RTEC_Proxy_Supplier::meta;
		DataAnalysisBase::meta.subTypes() +=  Average::meta;
		DataAnalysisBase::meta.subTypes() +=  Minimum::meta;
		DataAnalysisBase::meta.subTypes() +=  Maximum::meta;
		DataAnalysisBase::meta.subTypes() +=  Jitter::meta;
		MetricsBase::meta.subTypes() +=  Latency::meta;
		MetricsBase::meta.subTypes() +=  Throughput::meta;
		ComponentImplementation::meta.subTypes() +=  ComponentAssembly::meta;
		ComponentImplementation::meta.subTypes() +=  MonolithicImplementation::meta;
		CommonPortAttrs::meta.subTypes() +=  Port::meta;
		RequirementSatisfier::meta.subTypes() +=  Capability::meta;
		RequirementBase::meta.subTypes() +=  Requirement::meta;
		RequirementBase::meta.subTypes() +=  ImplementationRequirement::meta;
		Elements::meta.subTypes() +=  Interconnect::meta;
		Elements::meta.subTypes() +=  Node::meta;
		Elements::meta.subTypes() +=  Bridge::meta;
		Elements::meta.subTypes() +=  SharedResource::meta;
		Prefixable::meta.subTypes() +=  Object::meta;
		Prefixable::meta.subTypes() +=  ObjectByValue::meta;
		Prefixable::meta.subTypes() +=  Package::meta;
		Prefixable::meta.subTypes() +=  File::meta;
		Taggable::meta.subTypes() +=  Port::meta;
		Taggable::meta.subTypes() +=  OperationBase::meta;
		Taggable::meta.subTypes() +=  ReadonlyAttribute::meta;
		Taggable::meta.subTypes() +=  NamedType::meta;
		Taggable::meta.subTypes() +=  Package::meta;
		Taggable::meta.subTypes() +=  Exception::meta;
		Taggable::meta.subTypes() +=  Constant::meta;
		Port::meta.subTypes() +=  OutEventPort::meta;
		Port::meta.subTypes() +=  ProvidedRequestPort::meta;
		Port::meta.subTypes() +=  RequiredRequestPort::meta;
		Port::meta.subTypes() +=  InEventPort::meta;
		Manageable::meta.subTypes() +=  ComponentRef::meta;
		Manageable::meta.subTypes() +=  Component::meta;
		Provideable::meta.subTypes() +=  Object::meta;
		Provideable::meta.subTypes() +=  GenericObject::meta;
		OperationBase::meta.subTypes() +=  OnewayOperation::meta;
		OperationBase::meta.subTypes() +=  HasExceptions::meta;
		HasExceptions::meta.subTypes() +=  LookupOperation::meta;
		HasExceptions::meta.subTypes() +=  FactoryOperation::meta;
		HasExceptions::meta.subTypes() +=  TwowayOperation::meta;
		HasOperations::meta.subTypes() +=  ComponentFactory::meta;
		HasOperations::meta.subTypes() +=  Object::meta;
		HasOperations::meta.subTypes() +=  ObjectByValue::meta;
		SupportsInterfaces::meta.subTypes() +=  ComponentFactory::meta;
		SupportsInterfaces::meta.subTypes() +=  Component::meta;
		SupportsInterfaces::meta.subTypes() +=  ObjectByValue::meta;
		ReadonlyAttribute::meta.subTypes() +=  Attribute::meta;
		ObjectByValue::meta.subTypes() +=  Event::meta;
		ObjectByValue::meta.subTypes() +=  ValueObject::meta;
		Inheritable::meta.subTypes() +=  HasOperations::meta;
		MemberType::meta.subTypes() +=  NamedType::meta;
		MemberType::meta.subTypes() +=  PredefinedType::meta;
		NoInheritable::meta.subTypes() +=  Aggregate::meta;
		NoInheritable::meta.subTypes() +=  SwitchedAggregate::meta;
		NoInheritable::meta.subTypes() +=  Collection::meta;
		NoInheritable::meta.subTypes() +=  Alias::meta;
		NoInheritable::meta.subTypes() +=  Enum::meta;
		NamedType::meta.subTypes() +=  Component::meta;
		NamedType::meta.subTypes() +=  Inheritable::meta;
		NamedType::meta.subTypes() +=  Boxed::meta;
		NamedType::meta.subTypes() +=  NoInheritable::meta;
		PredefinedType::meta.subTypes() +=  Byte::meta;
		PredefinedType::meta.subTypes() +=  Boolean::meta;
		PredefinedType::meta.subTypes() +=  ShortInteger::meta;
		PredefinedType::meta.subTypes() +=  LongInteger::meta;
		PredefinedType::meta.subTypes() +=  RealNumber::meta;
		PredefinedType::meta.subTypes() +=  String::meta;
		PredefinedType::meta.subTypes() +=  GenericObject::meta;
		PredefinedType::meta.subTypes() +=  GenericValueObject::meta;
		PredefinedType::meta.subTypes() +=  GenericValue::meta;
		PredefinedType::meta.subTypes() +=  TypeEncoding::meta;
		PredefinedType::meta.subTypes() +=  TypeKind::meta;
		ConstantType::meta.subTypes() +=  Enum::meta;
		ConstantType::meta.subTypes() +=  PredefinedType::meta;
		ComponentLib::meta.subTypes() +=  StubProject::meta;
		ComponentLib::meta.subTypes() +=  ServantProject::meta;
		ComponentLib::meta.subTypes() +=  ExecutorProject::meta;
		MgaObject::meta.subTypes() +=  RTEC_Proxy::meta;
		MgaObject::meta.subTypes() +=  RTEC_Connection::meta;
		MgaObject::meta.subTypes() +=  RTEC_Filter_Group::meta;
		MgaObject::meta.subTypes() +=  Include_RTEC_Filter_Group::meta;
		MgaObject::meta.subTypes() +=  Use_RTEC_Filter::meta;
		MgaObject::meta.subTypes() +=  EventChannelConfiguration::meta;
		MgaObject::meta.subTypes() +=  RTEC_Resource_Factory::meta;
		MgaObject::meta.subTypes() +=  Include_RT_Info::meta;
		MgaObject::meta.subTypes() +=  Use_RT_Info::meta;
		MgaObject::meta.subTypes() +=  RT_Info_Param::meta;
		MgaObject::meta.subTypes() +=  RTEC_Resource_Factory_Reference::meta;
		MgaObject::meta.subTypes() +=  Proxy::meta;
		MgaObject::meta.subTypes() +=  MetricConnection::meta;
		MgaObject::meta.subTypes() +=  DataAnalysisBase::meta;
		MgaObject::meta.subTypes() +=  TimeProbe::meta;
		MgaObject::meta.subTypes() +=  TimerConnection::meta;
		MgaObject::meta.subTypes() +=  BenchmarkAnalysis::meta;
		MgaObject::meta.subTypes() +=  MetricsBase::meta;
		MgaObject::meta.subTypes() +=  OperationRef::meta;
		MgaObject::meta.subTypes() +=  EventRef::meta;
		MgaObject::meta.subTypes() +=  TimerEventSinkConn::meta;
		MgaObject::meta.subTypes() +=  TaskSet::meta;
		MgaObject::meta.subTypes() +=  Task::meta;
		MgaObject::meta.subTypes() +=  WorkloadCharacteristics::meta;
		MgaObject::meta.subTypes() +=  NodeReference::meta;
		MgaObject::meta.subTypes() +=  Resource::meta;
		MgaObject::meta.subTypes() +=  InstanceMapping::meta;
		MgaObject::meta.subTypes() +=  CollocationGroup::meta;
		MgaObject::meta.subTypes() +=  DeploymentPlan::meta;
		MgaObject::meta.subTypes() +=  invoke::meta;
		MgaObject::meta.subTypes() +=  AssemblyselectRequirement::meta;
		MgaObject::meta.subTypes() +=  AssemblyConfigProperty::meta;
		MgaObject::meta.subTypes() +=  PublishConnector::meta;
		MgaObject::meta.subTypes() +=  publish::meta;
		MgaObject::meta.subTypes() +=  deliverTo::meta;
		MgaObject::meta.subTypes() +=  AssemblyDeployRequirement::meta;
		MgaObject::meta.subTypes() +=  emit::meta;
		MgaObject::meta.subTypes() +=  ReceptacleDelegate::meta;
		MgaObject::meta.subTypes() +=  FacetDelegate::meta;
		MgaObject::meta.subTypes() +=  EventSourceDelegate::meta;
		MgaObject::meta.subTypes() +=  EventSinkDelegate::meta;
		MgaObject::meta.subTypes() +=  ExternalDelegate::meta;
		MgaObject::meta.subTypes() +=  ExternalReferenceEndPoint::meta;
		MgaObject::meta.subTypes() +=  ImplementationCapability::meta;
		MgaObject::meta.subTypes() +=  ImplementationDependsOn::meta;
		MgaObject::meta.subTypes() +=  Implements::meta;
		MgaObject::meta.subTypes() +=  ConfigProperty::meta;
		MgaObject::meta.subTypes() +=  InfoProperty::meta;
		MgaObject::meta.subTypes() +=  MonolithprimaryArtifact::meta;
		MgaObject::meta.subTypes() +=  MonolithDeployRequirement::meta;
		MgaObject::meta.subTypes() +=  ComponentImplementationContainer::meta;
		MgaObject::meta.subTypes() +=  MonolithExecParameter::meta;
		MgaObject::meta.subTypes() +=  ComponentImplementationReference::meta;
		MgaObject::meta.subTypes() +=  ComponentImplementation::meta;
		MgaObject::meta.subTypes() +=  PackageConfBasePackage::meta;
		MgaObject::meta.subTypes() +=  PackageConfReference::meta;
		MgaObject::meta.subTypes() +=  PackageConfigurationContainer::meta;
		MgaObject::meta.subTypes() +=  PackageConfConfigProperty::meta;
		MgaObject::meta.subTypes() +=  PackageConfSelectRequirement::meta;
		MgaObject::meta.subTypes() +=  PackageConfSpecializedConfig::meta;
		MgaObject::meta.subTypes() +=  PackageConfigurationReference::meta;
		MgaObject::meta.subTypes() +=  PackageConfiguration::meta;
		MgaObject::meta.subTypes() +=  ArtifactDependsOn::meta;
		MgaObject::meta.subTypes() +=  ArtifactDeployRequirement::meta;
		MgaObject::meta.subTypes() +=  ArtifactContainer::meta;
		MgaObject::meta.subTypes() +=  ArtifactExecParameter::meta;
		MgaObject::meta.subTypes() +=  ArtifactInfoProperty::meta;
		MgaObject::meta.subTypes() +=  ImplementationArtifactReference::meta;
		MgaObject::meta.subTypes() +=  ImplementationArtifact::meta;
		MgaObject::meta.subTypes() +=  ComponentPropertyDescription::meta;
		MgaObject::meta.subTypes() +=  ComponentProperty::meta;
		MgaObject::meta.subTypes() +=  ComponentContainer::meta;
		MgaObject::meta.subTypes() +=  ComponentConfigProperty::meta;
		MgaObject::meta.subTypes() +=  ComponentInfoProperty::meta;
		MgaObject::meta.subTypes() +=  CommonPortAttrs::meta;
		MgaObject::meta.subTypes() +=  TopLevelPackageContainer::meta;
		MgaObject::meta.subTypes() +=  TopLevelPackage::meta;
		MgaObject::meta.subTypes() +=  package::meta;
		MgaObject::meta.subTypes() +=  PackageInterface::meta;
		MgaObject::meta.subTypes() +=  Implementation::meta;
		MgaObject::meta.subTypes() +=  PackageContainer::meta;
		MgaObject::meta.subTypes() +=  PackageConfigProperty::meta;
		MgaObject::meta.subTypes() +=  PackageInfoProperty::meta;
		MgaObject::meta.subTypes() +=  ComponentPackage::meta;
		MgaObject::meta.subTypes() +=  ComponentPackageReference::meta;
		MgaObject::meta.subTypes() +=  DataType::meta;
		MgaObject::meta.subTypes() +=  SatisfierProperty::meta;
		MgaObject::meta.subTypes() +=  ImplementationDependency::meta;
		MgaObject::meta.subTypes() +=  RequirementSatisfier::meta;
		MgaObject::meta.subTypes() +=  RequirementBase::meta;
		MgaObject::meta.subTypes() +=  Property::meta;
		MgaObject::meta.subTypes() +=  Elements::meta;
		MgaObject::meta.subTypes() +=  Shares::meta;
		MgaObject::meta.subTypes() +=  Node2Interconnect::meta;
		MgaObject::meta.subTypes() +=  Bridge2Interconnect::meta;
		MgaObject::meta.subTypes() +=  Interconnect2Node::meta;
		MgaObject::meta.subTypes() +=  Interconnect2Bridge::meta;
		MgaObject::meta.subTypes() +=  Domain::meta;
		MgaObject::meta.subTypes() +=  Prefixable::meta;
		MgaObject::meta.subTypes() +=  Taggable::meta;
		MgaObject::meta.subTypes() +=  Manageable::meta;
		MgaObject::meta.subTypes() +=  Provideable::meta;
		MgaObject::meta.subTypes() +=  ManagesComponent::meta;
		MgaObject::meta.subTypes() +=  InParameter::meta;
		MgaObject::meta.subTypes() +=  InoutParameter::meta;
		MgaObject::meta.subTypes() +=  OutParameter::meta;
		MgaObject::meta.subTypes() +=  ReturnType::meta;
		MgaObject::meta.subTypes() +=  Inherits::meta;
		MgaObject::meta.subTypes() +=  SupportsInterfaces::meta;
		MgaObject::meta.subTypes() +=  Supports::meta;
		MgaObject::meta.subTypes() +=  LookupKey::meta;
		MgaObject::meta.subTypes() +=  SetException::meta;
		MgaObject::meta.subTypes() +=  GetException::meta;
		MgaObject::meta.subTypes() +=  PrivateFlag::meta;
		MgaObject::meta.subTypes() +=  MakeMemberPrivate::meta;
		MgaObject::meta.subTypes() +=  AttributeMember::meta;
		MgaObject::meta.subTypes() +=  EnumValue::meta;
		MgaObject::meta.subTypes() +=  Label::meta;
		MgaObject::meta.subTypes() +=  LabelConnection::meta;
		MgaObject::meta.subTypes() +=  MemberType::meta;
		MgaObject::meta.subTypes() +=  Discriminator::meta;
		MgaObject::meta.subTypes() +=  Member::meta;
		MgaObject::meta.subTypes() +=  ExceptionRef::meta;
		MgaObject::meta.subTypes() +=  ConstantType::meta;
		MgaObject::meta.subTypes() +=  FileRef::meta;
		MgaObject::meta.subTypes() +=  Project::meta;
		MgaObject::meta.subTypes() +=  ExternalResources::meta;
		MgaObject::meta.subTypes() +=  ExtResourceConn::meta;
		MgaObject::meta.subTypes() +=  ComponentLib::meta;
		MgaObject::meta.subTypes() +=  MPC::meta;
		MgaObject::meta.subTypes() +=  Workspaces::meta;
		
	}
	void Initialize(const Uml::Diagram & dgr)
	{
		umldiagram = dgr;
		Uml::SetClass(RTEC_Proxy::meta, umldiagram, "RTEC_Proxy");
		Uml::SetClass(Include_RTEC_Filter_Group_Members_Base::meta, umldiagram, "Include_RTEC_Filter_Group_Members_Base");
		Uml::SetClass(RTEC_Proxy_Supplier::meta, umldiagram, "RTEC_Proxy_Supplier");
		Uml::SetClass(RTEC_Proxy_Consumer::meta, umldiagram, "RTEC_Proxy_Consumer");
		Uml::SetClass(RT_Info::meta, umldiagram, "RT_Info");
		Uml::SetAttribute(RT_Info::meta_entry_point,RT_Info::meta, "entry_point");
		Uml::SetAttribute(RT_Info::meta_period,RT_Info::meta, "period");
		Uml::SetAttribute(RT_Info::meta_enabled,RT_Info::meta, "enabled");
		Uml::SetAttribute(RT_Info::meta_criticality,RT_Info::meta, "criticality");
		Uml::SetAttribute(RT_Info::meta_importance,RT_Info::meta, "importance");
		Uml::SetAttribute(RT_Info::meta_quantum,RT_Info::meta, "quantum");
		Uml::SetAttribute(RT_Info::meta_cached_execution_time,RT_Info::meta, "cached_execution_time");
		Uml::SetAttribute(RT_Info::meta_threads,RT_Info::meta, "threads");
		Uml::SetAttribute(RT_Info::meta_info_type,RT_Info::meta, "info_type");
		Uml::SetAttribute(RT_Info::meta_worst_case_execution_time,RT_Info::meta, "worst_case_execution_time");
		Uml::SetAttribute(RT_Info::meta_typical_execution_time,RT_Info::meta, "typical_execution_time");
		Uml::SetClass(RTEC_Connection::meta, umldiagram, "RTEC_Connection");
		Uml::SetClass(RTEC_Filter_Group::meta, umldiagram, "RTEC_Filter_Group");
		Uml::SetClass(LogicalANDGroup::meta, umldiagram, "LogicalANDGroup");
		Uml::SetClass(Include_RTEC_Filter_Group::meta, umldiagram, "Include_RTEC_Filter_Group");
		Uml::SetClass(DisjunctionGroup::meta, umldiagram, "DisjunctionGroup");
		Uml::SetClass(ConjunctionGroup::meta, umldiagram, "ConjunctionGroup");
		Uml::SetClass(Use_RTEC_Filter::meta, umldiagram, "Use_RTEC_Filter");
		Uml::SetClass(EventChannelConfiguration::meta, umldiagram, "EventChannelConfiguration");
		Uml::SetClass(RTEC_Resource_Factory::meta, umldiagram, "RTEC_Resource_Factory");
		Uml::SetAttribute(RTEC_Resource_Factory::meta_ECConsumerLock,RTEC_Resource_Factory::meta, "ECConsumerLock");
		Uml::SetAttribute(RTEC_Resource_Factory::meta_ECSupplierFiltering,RTEC_Resource_Factory::meta, "ECSupplierFiltering");
		Uml::SetAttribute(RTEC_Resource_Factory::meta_ECDispatching,RTEC_Resource_Factory::meta, "ECDispatching");
		Uml::SetAttribute(RTEC_Resource_Factory::meta_ECTimeout,RTEC_Resource_Factory::meta, "ECTimeout");
		Uml::SetAttribute(RTEC_Resource_Factory::meta_ECConsumerControlPeriod,RTEC_Resource_Factory::meta, "ECConsumerControlPeriod");
		Uml::SetAttribute(RTEC_Resource_Factory::meta_ECSupplierLock,RTEC_Resource_Factory::meta, "ECSupplierLock");
		Uml::SetAttribute(RTEC_Resource_Factory::meta_ECObserver,RTEC_Resource_Factory::meta, "ECObserver");
		Uml::SetAttribute(RTEC_Resource_Factory::meta_ECSupplierControl,RTEC_Resource_Factory::meta, "ECSupplierControl");
		Uml::SetAttribute(RTEC_Resource_Factory::meta_ECConsumerControl,RTEC_Resource_Factory::meta, "ECConsumerControl");
		Uml::SetAttribute(RTEC_Resource_Factory::meta_ECFiltering,RTEC_Resource_Factory::meta, "ECFiltering");
		Uml::SetAttribute(RTEC_Resource_Factory::meta_ECProxyPushConsumerCollectionFlag,RTEC_Resource_Factory::meta, "ECProxyPushConsumerCollectionFlag");
		Uml::SetAttribute(RTEC_Resource_Factory::meta_ECUseORBId,RTEC_Resource_Factory::meta, "ECUseORBId");
		Uml::SetAttribute(RTEC_Resource_Factory::meta_ECProxyPushConsumerCollectionSynch,RTEC_Resource_Factory::meta, "ECProxyPushConsumerCollectionSynch");
		Uml::SetAttribute(RTEC_Resource_Factory::meta_ECProxyPushConsumerCollectionIterator,RTEC_Resource_Factory::meta, "ECProxyPushConsumerCollectionIterator");
		Uml::SetAttribute(RTEC_Resource_Factory::meta_ECSupplierControlPeriod,RTEC_Resource_Factory::meta, "ECSupplierControlPeriod");
		Uml::SetAttribute(RTEC_Resource_Factory::meta_ECScheduling,RTEC_Resource_Factory::meta, "ECScheduling");
		Uml::SetAttribute(RTEC_Resource_Factory::meta_ECDispatchingThreads,RTEC_Resource_Factory::meta, "ECDispatchingThreads");
		Uml::SetAttribute(RTEC_Resource_Factory::meta_ECProxyPushSupplierCollectionFlag,RTEC_Resource_Factory::meta, "ECProxyPushSupplierCollectionFlag");
		Uml::SetAttribute(RTEC_Resource_Factory::meta_ECProxyPushSupplierCollectionSynch,RTEC_Resource_Factory::meta, "ECProxyPushSupplierCollectionSynch");
		Uml::SetAttribute(RTEC_Resource_Factory::meta_ECProxyPushSupplierCollectionIterator,RTEC_Resource_Factory::meta, "ECProxyPushSupplierCollectionIterator");
		Uml::SetClass(Include_RT_Info::meta, umldiagram, "Include_RT_Info");
		Uml::SetClass(Use_RT_Info::meta, umldiagram, "Use_RT_Info");
		Uml::SetClass(RT_Info_Param::meta, umldiagram, "RT_Info_Param");
		Uml::SetClass(Null_RT_Info::meta, umldiagram, "Null_RT_Info");
		Uml::SetClass(RTEC_Resource_Factory_Reference::meta, umldiagram, "RTEC_Resource_Factory_Reference");
		Uml::SetClass(EventChannelConfig::meta, umldiagram, "EventChannelConfig");
		Uml::SetAttribute(EventChannelConfig::meta_name,EventChannelConfig::meta, "name");
		Uml::SetClass(Proxy::meta, umldiagram, "Proxy");
		Uml::SetClass(Proxy_Consumer::meta, umldiagram, "Proxy_Consumer");
		Uml::SetClass(Proxy_Supplier::meta, umldiagram, "Proxy_Supplier");
		Uml::SetClass(MetricConnection::meta, umldiagram, "MetricConnection");
		Uml::SetClass(DataAnalysisBase::meta, umldiagram, "DataAnalysisBase");
		Uml::SetClass(Average::meta, umldiagram, "Average");
		Uml::SetClass(Minimum::meta, umldiagram, "Minimum");
		Uml::SetClass(Maximum::meta, umldiagram, "Maximum");
		Uml::SetClass(Jitter::meta, umldiagram, "Jitter");
		Uml::SetClass(TimeProbe::meta, umldiagram, "TimeProbe");
		Uml::SetClass(TimerConnection::meta, umldiagram, "TimerConnection");
		Uml::SetClass(Latency::meta, umldiagram, "Latency");
		Uml::SetClass(Throughput::meta, umldiagram, "Throughput");
		Uml::SetClass(ComponentAnalyses::meta, umldiagram, "ComponentAnalyses");
		Uml::SetAttribute(ComponentAnalyses::meta_name,ComponentAnalyses::meta, "name");
		Uml::SetClass(BenchmarkAnalysis::meta, umldiagram, "BenchmarkAnalysis");
		Uml::SetClass(MetricsBase::meta, umldiagram, "MetricsBase");
		Uml::SetAttribute(MetricsBase::meta_warmup,MetricsBase::meta, "warmup");
		Uml::SetAttribute(MetricsBase::meta_fileName,MetricsBase::meta, "fileName");
		Uml::SetAttribute(MetricsBase::meta_iterations,MetricsBase::meta, "iterations");
		Uml::SetClass(OperationRef::meta, umldiagram, "OperationRef");
		Uml::SetClass(EventRef::meta, umldiagram, "EventRef");
		Uml::SetClass(TimerEventSinkConn::meta, umldiagram, "TimerEventSinkConn");
		Uml::SetClass(TaskSet::meta, umldiagram, "TaskSet");
		Uml::SetClass(Task::meta, umldiagram, "Task");
		Uml::SetAttribute(Task::meta_rate,Task::meta, "rate");
		Uml::SetAttribute(Task::meta_priority,Task::meta, "priority");
		Uml::SetClass(WorkloadCharacteristics::meta, umldiagram, "WorkloadCharacteristics");
		Uml::SetClass(NodeReference::meta, umldiagram, "NodeReference");
		Uml::SetClass(Interconnect::meta, umldiagram, "Interconnect");
		Uml::SetAttribute(Interconnect::meta_label,Interconnect::meta, "label");
		Uml::SetClass(Node::meta, umldiagram, "Node");
		Uml::SetAttribute(Node::meta_label,Node::meta, "label");
		Uml::SetClass(Resource::meta, umldiagram, "Resource");
		Uml::SetAttribute(Resource::meta_resourceType,Resource::meta, "resourceType");
		Uml::SetClass(Bridge::meta, umldiagram, "Bridge");
		Uml::SetAttribute(Bridge::meta_label,Bridge::meta, "label");
		Uml::SetClass(SharedResource::meta, umldiagram, "SharedResource");
		Uml::SetAttribute(SharedResource::meta_resourceType,SharedResource::meta, "resourceType");
		Uml::SetClass(InstanceMapping::meta, umldiagram, "InstanceMapping");
		Uml::SetClass(DeploymentPlans::meta, umldiagram, "DeploymentPlans");
		Uml::SetAttribute(DeploymentPlans::meta_name,DeploymentPlans::meta, "name");
		Uml::SetClass(CollocationGroup::meta, umldiagram, "CollocationGroup");
		Uml::SetClass(DeploymentPlan::meta, umldiagram, "DeploymentPlan");
		Uml::SetAttribute(DeploymentPlan::meta_label,DeploymentPlan::meta, "label");
		Uml::SetClass(invoke::meta, umldiagram, "invoke");
		Uml::SetClass(AssemblyselectRequirement::meta, umldiagram, "AssemblyselectRequirement");
		Uml::SetClass(AssemblyConfigProperty::meta, umldiagram, "AssemblyConfigProperty");
		Uml::SetClass(PublishConnector::meta, umldiagram, "PublishConnector");
		Uml::SetClass(publish::meta, umldiagram, "publish");
		Uml::SetClass(deliverTo::meta, umldiagram, "deliverTo");
		Uml::SetClass(AssemblyDeployRequirement::meta, umldiagram, "AssemblyDeployRequirement");
		Uml::SetClass(emit::meta, umldiagram, "emit");
		Uml::SetClass(ReceptacleDelegate::meta, umldiagram, "ReceptacleDelegate");
		Uml::SetClass(FacetDelegate::meta, umldiagram, "FacetDelegate");
		Uml::SetClass(EventSourceDelegate::meta, umldiagram, "EventSourceDelegate");
		Uml::SetClass(EventSinkDelegate::meta, umldiagram, "EventSinkDelegate");
		Uml::SetClass(ExternalDelegate::meta, umldiagram, "ExternalDelegate");
		Uml::SetClass(ComponentAssembly::meta, umldiagram, "ComponentAssembly");
		Uml::SetClass(ExternalReferenceEndPoint::meta, umldiagram, "ExternalReferenceEndPoint");
		Uml::SetAttribute(ExternalReferenceEndPoint::meta_location,ExternalReferenceEndPoint::meta, "location");
		Uml::SetClass(ImplementationCapability::meta, umldiagram, "ImplementationCapability");
		Uml::SetClass(ImplementationDependsOn::meta, umldiagram, "ImplementationDependsOn");
		Uml::SetClass(Implements::meta, umldiagram, "Implements");
		Uml::SetClass(ComponentImplementations::meta, umldiagram, "ComponentImplementations");
		Uml::SetAttribute(ComponentImplementations::meta_name,ComponentImplementations::meta, "name");
		Uml::SetClass(ConfigProperty::meta, umldiagram, "ConfigProperty");
		Uml::SetClass(InfoProperty::meta, umldiagram, "InfoProperty");
		Uml::SetClass(MonolithprimaryArtifact::meta, umldiagram, "MonolithprimaryArtifact");
		Uml::SetClass(MonolithDeployRequirement::meta, umldiagram, "MonolithDeployRequirement");
		Uml::SetClass(ComponentImplementationContainer::meta, umldiagram, "ComponentImplementationContainer");
		Uml::SetClass(MonolithExecParameter::meta, umldiagram, "MonolithExecParameter");
		Uml::SetClass(MonolithicImplementation::meta, umldiagram, "MonolithicImplementation");
		Uml::SetClass(ComponentImplementationReference::meta, umldiagram, "ComponentImplementationReference");
		Uml::SetClass(ComponentImplementation::meta, umldiagram, "ComponentImplementation");
		Uml::SetAttribute(ComponentImplementation::meta_UUID,ComponentImplementation::meta, "UUID");
		Uml::SetAttribute(ComponentImplementation::meta_label,ComponentImplementation::meta, "label");
		Uml::SetClass(PackageConfBasePackage::meta, umldiagram, "PackageConfBasePackage");
		Uml::SetClass(PackageConfReference::meta, umldiagram, "PackageConfReference");
		Uml::SetClass(PackageConfigurations::meta, umldiagram, "PackageConfigurations");
		Uml::SetAttribute(PackageConfigurations::meta_name,PackageConfigurations::meta, "name");
		Uml::SetClass(PackageConfigurationContainer::meta, umldiagram, "PackageConfigurationContainer");
		Uml::SetClass(PackageConfConfigProperty::meta, umldiagram, "PackageConfConfigProperty");
		Uml::SetClass(PackageConfSelectRequirement::meta, umldiagram, "PackageConfSelectRequirement");
		Uml::SetClass(PackageConfSpecializedConfig::meta, umldiagram, "PackageConfSpecializedConfig");
		Uml::SetClass(PackageConfigurationReference::meta, umldiagram, "PackageConfigurationReference");
		Uml::SetClass(PackageConfiguration::meta, umldiagram, "PackageConfiguration");
		Uml::SetAttribute(PackageConfiguration::meta_UUID,PackageConfiguration::meta, "UUID");
		Uml::SetAttribute(PackageConfiguration::meta_label,PackageConfiguration::meta, "label");
		Uml::SetClass(ImplementationArtifacts::meta, umldiagram, "ImplementationArtifacts");
		Uml::SetAttribute(ImplementationArtifacts::meta_name,ImplementationArtifacts::meta, "name");
		Uml::SetClass(ArtifactDependsOn::meta, umldiagram, "ArtifactDependsOn");
		Uml::SetClass(ArtifactDeployRequirement::meta, umldiagram, "ArtifactDeployRequirement");
		Uml::SetClass(ArtifactContainer::meta, umldiagram, "ArtifactContainer");
		Uml::SetClass(ArtifactExecParameter::meta, umldiagram, "ArtifactExecParameter");
		Uml::SetClass(ArtifactInfoProperty::meta, umldiagram, "ArtifactInfoProperty");
		Uml::SetClass(ImplementationArtifactReference::meta, umldiagram, "ImplementationArtifactReference");
		Uml::SetClass(ImplementationArtifact::meta, umldiagram, "ImplementationArtifact");
		Uml::SetAttribute(ImplementationArtifact::meta_tree_file_name,ImplementationArtifact::meta, "tree_file_name");
		Uml::SetAttribute(ImplementationArtifact::meta_configuration,ImplementationArtifact::meta, "configuration");
		Uml::SetAttribute(ImplementationArtifact::meta_label,ImplementationArtifact::meta, "label");
		Uml::SetAttribute(ImplementationArtifact::meta_UUID,ImplementationArtifact::meta, "UUID");
		Uml::SetAttribute(ImplementationArtifact::meta_location,ImplementationArtifact::meta, "location");
		Uml::SetClass(ComponentTypes::meta, umldiagram, "ComponentTypes");
		Uml::SetAttribute(ComponentTypes::meta_name,ComponentTypes::meta, "name");
		Uml::SetClass(ComponentPropertyDescription::meta, umldiagram, "ComponentPropertyDescription");
		Uml::SetClass(ComponentProperty::meta, umldiagram, "ComponentProperty");
		Uml::SetClass(ComponentContainer::meta, umldiagram, "ComponentContainer");
		Uml::SetClass(ComponentConfigProperty::meta, umldiagram, "ComponentConfigProperty");
		Uml::SetClass(ComponentInfoProperty::meta, umldiagram, "ComponentInfoProperty");
		Uml::SetClass(CommonPortAttrs::meta, umldiagram, "CommonPortAttrs");
		Uml::SetAttribute(CommonPortAttrs::meta_exclusiveUser,CommonPortAttrs::meta, "exclusiveUser");
		Uml::SetAttribute(CommonPortAttrs::meta_optional,CommonPortAttrs::meta, "optional");
		Uml::SetAttribute(CommonPortAttrs::meta_exclusiveProvider,CommonPortAttrs::meta, "exclusiveProvider");
		Uml::SetClass(TopLevelPackageContainer::meta, umldiagram, "TopLevelPackageContainer");
		Uml::SetClass(TopLevelPackages::meta, umldiagram, "TopLevelPackages");
		Uml::SetAttribute(TopLevelPackages::meta_name,TopLevelPackages::meta, "name");
		Uml::SetClass(TopLevelPackage::meta, umldiagram, "TopLevelPackage");
		Uml::SetClass(package::meta, umldiagram, "package");
		Uml::SetClass(ComponentPackages::meta, umldiagram, "ComponentPackages");
		Uml::SetAttribute(ComponentPackages::meta_name,ComponentPackages::meta, "name");
		Uml::SetClass(PackageInterface::meta, umldiagram, "PackageInterface");
		Uml::SetClass(Implementation::meta, umldiagram, "Implementation");
		Uml::SetClass(PackageContainer::meta, umldiagram, "PackageContainer");
		Uml::SetClass(PackageConfigProperty::meta, umldiagram, "PackageConfigProperty");
		Uml::SetClass(PackageInfoProperty::meta, umldiagram, "PackageInfoProperty");
		Uml::SetClass(ComponentPackage::meta, umldiagram, "ComponentPackage");
		Uml::SetAttribute(ComponentPackage::meta_UUID,ComponentPackage::meta, "UUID");
		Uml::SetAttribute(ComponentPackage::meta_label,ComponentPackage::meta, "label");
		Uml::SetClass(ComponentPackageReference::meta, umldiagram, "ComponentPackageReference");
		Uml::SetAttribute(ComponentPackageReference::meta_requiredName,ComponentPackageReference::meta, "requiredName");
		Uml::SetAttribute(ComponentPackageReference::meta_requiredUUID,ComponentPackageReference::meta, "requiredUUID");
		Uml::SetAttribute(ComponentPackageReference::meta_requiredType,ComponentPackageReference::meta, "requiredType");
		Uml::SetClass(Requirement::meta, umldiagram, "Requirement");
		Uml::SetClass(Capability::meta, umldiagram, "Capability");
		Uml::SetClass(DataType::meta, umldiagram, "DataType");
		Uml::SetClass(SatisfierProperty::meta, umldiagram, "SatisfierProperty");
		Uml::SetAttribute(SatisfierProperty::meta_DataValue,SatisfierProperty::meta, "DataValue");
		Uml::SetAttribute(SatisfierProperty::meta_SatisfierPropertyKind,SatisfierProperty::meta, "SatisfierPropertyKind");
		Uml::SetClass(ImplementationDependency::meta, umldiagram, "ImplementationDependency");
		Uml::SetAttribute(ImplementationDependency::meta_requiredType,ImplementationDependency::meta, "requiredType");
		Uml::SetClass(RequirementSatisfier::meta, umldiagram, "RequirementSatisfier");
		Uml::SetAttribute(RequirementSatisfier::meta_resourceType,RequirementSatisfier::meta, "resourceType");
		Uml::SetClass(RequirementBase::meta, umldiagram, "RequirementBase");
		Uml::SetAttribute(RequirementBase::meta_resourceType,RequirementBase::meta, "resourceType");
		Uml::SetClass(ImplementationRequirement::meta, umldiagram, "ImplementationRequirement");
		Uml::SetAttribute(ImplementationRequirement::meta_ResourceUsageKind,ImplementationRequirement::meta, "ResourceUsageKind");
		Uml::SetAttribute(ImplementationRequirement::meta_componentPort,ImplementationRequirement::meta, "componentPort");
		Uml::SetAttribute(ImplementationRequirement::meta_resourcePort,ImplementationRequirement::meta, "resourcePort");
		Uml::SetClass(Property::meta, umldiagram, "Property");
		Uml::SetAttribute(Property::meta_DataValue,Property::meta, "DataValue");
		Uml::SetClass(Elements::meta, umldiagram, "Elements");
		Uml::SetClass(Targets::meta, umldiagram, "Targets");
		Uml::SetAttribute(Targets::meta_name,Targets::meta, "name");
		Uml::SetClass(Shares::meta, umldiagram, "Shares");
		Uml::SetClass(Node2Interconnect::meta, umldiagram, "Node2Interconnect");
		Uml::SetClass(Bridge2Interconnect::meta, umldiagram, "Bridge2Interconnect");
		Uml::SetClass(Interconnect2Node::meta, umldiagram, "Interconnect2Node");
		Uml::SetClass(Interconnect2Bridge::meta, umldiagram, "Interconnect2Bridge");
		Uml::SetClass(Domain::meta, umldiagram, "Domain");
		Uml::SetAttribute(Domain::meta_label,Domain::meta, "label");
		Uml::SetAttribute(Domain::meta_UUID,Domain::meta, "UUID");
		Uml::SetClass(Prefixable::meta, umldiagram, "Prefixable");
		Uml::SetAttribute(Prefixable::meta_PrefixTag,Prefixable::meta, "PrefixTag");
		Uml::SetClass(Taggable::meta, umldiagram, "Taggable");
		Uml::SetAttribute(Taggable::meta_SpecifyIdTag,Taggable::meta, "SpecifyIdTag");
		Uml::SetAttribute(Taggable::meta_VersionTag,Taggable::meta, "VersionTag");
		Uml::SetClass(OutEventPort::meta, umldiagram, "OutEventPort");
		Uml::SetAttribute(OutEventPort::meta_out_event_port_type,OutEventPort::meta, "out_event_port_type");
		Uml::SetAttribute(OutEventPort::meta_single_destination,OutEventPort::meta, "single_destination");
		Uml::SetClass(ProvidedRequestPort::meta, umldiagram, "ProvidedRequestPort");
		Uml::SetClass(RequiredRequestPort::meta, umldiagram, "RequiredRequestPort");
		Uml::SetAttribute(RequiredRequestPort::meta_multiple_connections,RequiredRequestPort::meta, "multiple_connections");
		Uml::SetClass(InEventPort::meta, umldiagram, "InEventPort");
		Uml::SetClass(Port::meta, umldiagram, "Port");
		Uml::SetClass(Manageable::meta, umldiagram, "Manageable");
		Uml::SetClass(Provideable::meta, umldiagram, "Provideable");
		Uml::SetClass(ComponentFactory::meta, umldiagram, "ComponentFactory");
		Uml::SetClass(ComponentRef::meta, umldiagram, "ComponentRef");
		Uml::SetClass(ManagesComponent::meta, umldiagram, "ManagesComponent");
		Uml::SetClass(Component::meta, umldiagram, "Component");
		Uml::SetAttribute(Component::meta_UUID,Component::meta, "UUID");
		Uml::SetAttribute(Component::meta_label,Component::meta, "label");
		Uml::SetClass(OnewayOperation::meta, umldiagram, "OnewayOperation");
		Uml::SetClass(LookupOperation::meta, umldiagram, "LookupOperation");
		Uml::SetClass(FactoryOperation::meta, umldiagram, "FactoryOperation");
		Uml::SetClass(OperationBase::meta, umldiagram, "OperationBase");
		Uml::SetClass(HasExceptions::meta, umldiagram, "HasExceptions");
		Uml::SetClass(InParameter::meta, umldiagram, "InParameter");
		Uml::SetClass(InoutParameter::meta, umldiagram, "InoutParameter");
		Uml::SetClass(OutParameter::meta, umldiagram, "OutParameter");
		Uml::SetClass(ReturnType::meta, umldiagram, "ReturnType");
		Uml::SetClass(TwowayOperation::meta, umldiagram, "TwowayOperation");
		Uml::SetClass(Object::meta, umldiagram, "Object");
		Uml::SetAttribute(Object::meta_local,Object::meta, "local");
		Uml::SetAttribute(Object::meta_abstract,Object::meta, "abstract");
		Uml::SetClass(Inherits::meta, umldiagram, "Inherits");
		Uml::SetClass(Event::meta, umldiagram, "Event");
		Uml::SetClass(ValueObject::meta, umldiagram, "ValueObject");
		Uml::SetClass(HasOperations::meta, umldiagram, "HasOperations");
		Uml::SetClass(SupportsInterfaces::meta, umldiagram, "SupportsInterfaces");
		Uml::SetClass(Supports::meta, umldiagram, "Supports");
		Uml::SetClass(ReadonlyAttribute::meta, umldiagram, "ReadonlyAttribute");
		Uml::SetClass(Attribute::meta, umldiagram, "Attribute");
		Uml::SetClass(LookupKey::meta, umldiagram, "LookupKey");
		Uml::SetClass(SetException::meta, umldiagram, "SetException");
		Uml::SetClass(ObjectByValue::meta, umldiagram, "ObjectByValue");
		Uml::SetAttribute(ObjectByValue::meta_abstract,ObjectByValue::meta, "abstract");
		Uml::SetClass(GetException::meta, umldiagram, "GetException");
		Uml::SetClass(PrivateFlag::meta, umldiagram, "PrivateFlag");
		Uml::SetClass(MakeMemberPrivate::meta, umldiagram, "MakeMemberPrivate");
		Uml::SetClass(AttributeMember::meta, umldiagram, "AttributeMember");
		Uml::SetClass(Inheritable::meta, umldiagram, "Inheritable");
		Uml::SetClass(EnumValue::meta, umldiagram, "EnumValue");
		Uml::SetClass(Aggregate::meta, umldiagram, "Aggregate");
		Uml::SetClass(SwitchedAggregate::meta, umldiagram, "SwitchedAggregate");
		Uml::SetClass(Label::meta, umldiagram, "Label");
		Uml::SetClass(LabelConnection::meta, umldiagram, "LabelConnection");
		Uml::SetClass(MemberType::meta, umldiagram, "MemberType");
		Uml::SetClass(Discriminator::meta, umldiagram, "Discriminator");
		Uml::SetClass(Collection::meta, umldiagram, "Collection");
		Uml::SetClass(Alias::meta, umldiagram, "Alias");
		Uml::SetClass(Boxed::meta, umldiagram, "Boxed");
		Uml::SetClass(NoInheritable::meta, umldiagram, "NoInheritable");
		Uml::SetClass(Enum::meta, umldiagram, "Enum");
		Uml::SetClass(Member::meta, umldiagram, "Member");
		Uml::SetClass(NamedType::meta, umldiagram, "NamedType");
		Uml::SetClass(PredefinedType::meta, umldiagram, "PredefinedType");
		Uml::SetClass(PredefinedTypes::meta, umldiagram, "PredefinedTypes");
		Uml::SetAttribute(PredefinedTypes::meta_name,PredefinedTypes::meta, "name");
		Uml::SetClass(Byte::meta, umldiagram, "Byte");
		Uml::SetClass(Boolean::meta, umldiagram, "Boolean");
		Uml::SetClass(ShortInteger::meta, umldiagram, "ShortInteger");
		Uml::SetClass(LongInteger::meta, umldiagram, "LongInteger");
		Uml::SetClass(RealNumber::meta, umldiagram, "RealNumber");
		Uml::SetClass(String::meta, umldiagram, "String");
		Uml::SetClass(GenericObject::meta, umldiagram, "GenericObject");
		Uml::SetClass(GenericValueObject::meta, umldiagram, "GenericValueObject");
		Uml::SetClass(GenericValue::meta, umldiagram, "GenericValue");
		Uml::SetClass(TypeEncoding::meta, umldiagram, "TypeEncoding");
		Uml::SetClass(TypeKind::meta, umldiagram, "TypeKind");
		Uml::SetClass(Package::meta, umldiagram, "Package");
		Uml::SetClass(File::meta, umldiagram, "File");
		Uml::SetClass(Exception::meta, umldiagram, "Exception");
		Uml::SetClass(ExceptionRef::meta, umldiagram, "ExceptionRef");
		Uml::SetClass(InterfaceDefinitions::meta, umldiagram, "InterfaceDefinitions");
		Uml::SetAttribute(InterfaceDefinitions::meta_name,InterfaceDefinitions::meta, "name");
		Uml::SetClass(ConstantType::meta, umldiagram, "ConstantType");
		Uml::SetClass(Constant::meta, umldiagram, "Constant");
		Uml::SetAttribute(Constant::meta_value,Constant::meta, "value");
		Uml::SetClass(FileRef::meta, umldiagram, "FileRef");
		Uml::SetClass(ComponentBuild::meta, umldiagram, "ComponentBuild");
		Uml::SetAttribute(ComponentBuild::meta_name,ComponentBuild::meta, "name");
		Uml::SetClass(Project::meta, umldiagram, "Project");
		Uml::SetClass(ExternalResources::meta, umldiagram, "ExternalResources");
		Uml::SetClass(ExtResourceConn::meta, umldiagram, "ExtResourceConn");
		Uml::SetClass(ComponentLib::meta, umldiagram, "ComponentLib");
		Uml::SetAttribute(ComponentLib::meta_sharedname,ComponentLib::meta, "sharedname");
		Uml::SetAttribute(ComponentLib::meta_libraryexport,ComponentLib::meta, "libraryexport");
		Uml::SetAttribute(ComponentLib::meta_ORBServices,ComponentLib::meta, "ORBServices");
		Uml::SetClass(MPC::meta, umldiagram, "MPC");
		Uml::SetClass(StubProject::meta, umldiagram, "StubProject");
		Uml::SetClass(ServantProject::meta, umldiagram, "ServantProject");
		Uml::SetClass(ExecutorProject::meta, umldiagram, "ExecutorProject");
		Uml::SetClass(Workspaces::meta, umldiagram, "Workspaces");
		Uml::SetClass(RootFolder::meta, umldiagram, "RootFolder");
		Uml::SetAttribute(RootFolder::meta_name,RootFolder::meta, "name");
		Uml::SetClass(MgaObject::meta, umldiagram, "MgaObject");
		Uml::SetAttribute(MgaObject::meta_position,MgaObject::meta, "position");
		Uml::SetAttribute(MgaObject::meta_name,MgaObject::meta, "name");
		//composition child roles 
		Uml::SetParentRole(RTEC_Connection::meta_EventChannelConfiguration_parent,RTEC_Connection::meta,EventChannelConfiguration::meta, "", "");
		Uml::SetParentRole(RTEC_Filter_Group::meta_RTEC_Proxy_Supplier_parent,RTEC_Filter_Group::meta,RTEC_Proxy_Supplier::meta, "", "");
		Uml::SetParentRole(Include_RTEC_Filter_Group::meta_RTEC_Proxy_Supplier_parent,Include_RTEC_Filter_Group::meta,RTEC_Proxy_Supplier::meta, "", "");
		Uml::SetParentRole(Use_RTEC_Filter::meta_EventChannelConfiguration_parent,Use_RTEC_Filter::meta,EventChannelConfiguration::meta, "", "");
		Uml::SetParentRole(EventChannelConfiguration::meta_EventChannelConfig_parent,EventChannelConfiguration::meta,EventChannelConfig::meta, "", "");
		Uml::SetParentRole(RTEC_Resource_Factory::meta_EventChannelConfiguration_parent,RTEC_Resource_Factory::meta,EventChannelConfiguration::meta, "", "");
		Uml::SetParentRole(Include_RT_Info::meta_RTEC_Proxy_Supplier_parent,Include_RT_Info::meta,RTEC_Proxy_Supplier::meta, "", "");
		Uml::SetParentRole(Use_RT_Info::meta_RTEC_Proxy_Consumer_parent,Use_RT_Info::meta,RTEC_Proxy_Consumer::meta, "", "");
		Uml::SetParentRole(Use_RT_Info::meta_EventChannelConfiguration_parent,Use_RT_Info::meta,EventChannelConfiguration::meta, "", "");
		Uml::SetParentRole(RT_Info_Param::meta_RTEC_Proxy_parent,RT_Info_Param::meta,RTEC_Proxy::meta, "", "");
		Uml::SetParentRole(RTEC_Resource_Factory_Reference::meta_RTEC_Proxy_Consumer_parent,RTEC_Resource_Factory_Reference::meta,RTEC_Proxy_Consumer::meta, "", "");
		Uml::SetParentRole(EventChannelConfig::meta_RootFolder_parent,EventChannelConfig::meta,RootFolder::meta, "", "");
		Uml::SetParentRole(Proxy::meta_EventChannelConfiguration_parent,Proxy::meta,EventChannelConfiguration::meta, "", "");
		Uml::SetParentRole(MetricConnection::meta_BenchmarkAnalysis_parent,MetricConnection::meta,BenchmarkAnalysis::meta, "", "");
		Uml::SetParentRole(DataAnalysisBase::meta_MetricsBase_parent,DataAnalysisBase::meta,MetricsBase::meta, "", "");
		Uml::SetParentRole(DataAnalysisBase::meta_BenchmarkAnalysis_parent,DataAnalysisBase::meta,BenchmarkAnalysis::meta, "", "");
		Uml::SetParentRole(TimeProbe::meta_BenchmarkAnalysis_parent,TimeProbe::meta,BenchmarkAnalysis::meta, "", "");
		Uml::SetParentRole(TimerConnection::meta_BenchmarkAnalysis_parent,TimerConnection::meta,BenchmarkAnalysis::meta, "", "");
		Uml::SetParentRole(ComponentAnalyses::meta_RootFolder_parent,ComponentAnalyses::meta,RootFolder::meta, "", "");
		Uml::SetParentRole(BenchmarkAnalysis::meta_ComponentAnalyses_parent,BenchmarkAnalysis::meta,ComponentAnalyses::meta, "", "");
		Uml::SetParentRole(MetricsBase::meta_BenchmarkAnalysis_parent,MetricsBase::meta,BenchmarkAnalysis::meta, "", "");
		Uml::SetParentRole(OperationRef::meta_BenchmarkAnalysis_parent,OperationRef::meta,BenchmarkAnalysis::meta, "", "");
		Uml::SetParentRole(EventRef::meta_BenchmarkAnalysis_parent,EventRef::meta,BenchmarkAnalysis::meta, "", "");
		Uml::SetParentRole(TimerEventSinkConn::meta_BenchmarkAnalysis_parent,TimerEventSinkConn::meta,BenchmarkAnalysis::meta, "", "");
		Uml::SetParentRole(TaskSet::meta_BenchmarkAnalysis_parent,TaskSet::meta,BenchmarkAnalysis::meta, "", "");
		Uml::SetParentRole(Task::meta_BenchmarkAnalysis_parent,Task::meta,BenchmarkAnalysis::meta, "", "");
		Uml::SetParentRole(WorkloadCharacteristics::meta_BenchmarkAnalysis_parent,WorkloadCharacteristics::meta,BenchmarkAnalysis::meta, "", "");
		Uml::SetParentRole(NodeReference::meta_DeploymentPlan_parent,NodeReference::meta,DeploymentPlan::meta, "", "");
		Uml::SetParentRole(Resource::meta_Node_parent,Resource::meta,Node::meta, "", "");
		Uml::SetParentRole(Resource::meta_Interconnect_parent,Resource::meta,Interconnect::meta, "", "");
		Uml::SetParentRole(Resource::meta_Bridge_parent,Resource::meta,Bridge::meta, "", "");
		Uml::SetParentRole(InstanceMapping::meta_DeploymentPlan_parent,InstanceMapping::meta,DeploymentPlan::meta, "", "");
		Uml::SetParentRole(DeploymentPlans::meta_RootFolder_parent,DeploymentPlans::meta,RootFolder::meta, "", "");
		Uml::SetParentRole(CollocationGroup::meta_DeploymentPlan_parent,CollocationGroup::meta,DeploymentPlan::meta, "", "");
		Uml::SetParentRole(DeploymentPlan::meta_DeploymentPlans_parent,DeploymentPlan::meta,DeploymentPlans::meta, "", "");
		Uml::SetParentRole(invoke::meta_ComponentAssembly_parent,invoke::meta,ComponentAssembly::meta, "", "");
		Uml::SetParentRole(AssemblyselectRequirement::meta_ComponentAssembly_parent,AssemblyselectRequirement::meta,ComponentAssembly::meta, "", "");
		Uml::SetParentRole(AssemblyConfigProperty::meta_ComponentAssembly_parent,AssemblyConfigProperty::meta,ComponentAssembly::meta, "", "");
		Uml::SetParentRole(PublishConnector::meta_ComponentAssembly_parent,PublishConnector::meta,ComponentAssembly::meta, "", "");
		Uml::SetParentRole(publish::meta_ComponentAssembly_parent,publish::meta,ComponentAssembly::meta, "", "");
		Uml::SetParentRole(deliverTo::meta_ComponentAssembly_parent,deliverTo::meta,ComponentAssembly::meta, "", "");
		Uml::SetParentRole(AssemblyDeployRequirement::meta_ComponentAssembly_parent,AssemblyDeployRequirement::meta,ComponentAssembly::meta, "", "");
		Uml::SetParentRole(emit::meta_ComponentAssembly_parent,emit::meta,ComponentAssembly::meta, "", "");
		Uml::SetParentRole(ReceptacleDelegate::meta_ComponentAssembly_parent,ReceptacleDelegate::meta,ComponentAssembly::meta, "", "");
		Uml::SetParentRole(FacetDelegate::meta_ComponentAssembly_parent,FacetDelegate::meta,ComponentAssembly::meta, "", "");
		Uml::SetParentRole(EventSourceDelegate::meta_ComponentAssembly_parent,EventSourceDelegate::meta,ComponentAssembly::meta, "", "");
		Uml::SetParentRole(EventSinkDelegate::meta_ComponentAssembly_parent,EventSinkDelegate::meta,ComponentAssembly::meta, "", "");
		Uml::SetParentRole(ExternalDelegate::meta_ComponentAssembly_parent,ExternalDelegate::meta,ComponentAssembly::meta, "", "");
		Uml::SetParentRole(ComponentAssembly::meta_ComponentAssembly_parent,ComponentAssembly::meta,ComponentAssembly::meta, "", "");
		Uml::SetParentRole(ExternalReferenceEndPoint::meta_ComponentAssembly_parent,ExternalReferenceEndPoint::meta,ComponentAssembly::meta, "", "");
		Uml::SetParentRole(ImplementationCapability::meta_ComponentImplementationContainer_parent,ImplementationCapability::meta,ComponentImplementationContainer::meta, "", "");
		Uml::SetParentRole(ImplementationDependsOn::meta_ComponentImplementationContainer_parent,ImplementationDependsOn::meta,ComponentImplementationContainer::meta, "", "");
		Uml::SetParentRole(Implements::meta_ComponentImplementationContainer_parent,Implements::meta,ComponentImplementationContainer::meta, "", "");
		Uml::SetParentRole(ComponentImplementations::meta_RootFolder_parent,ComponentImplementations::meta,RootFolder::meta, "", "");
		Uml::SetParentRole(ConfigProperty::meta_ComponentImplementationContainer_parent,ConfigProperty::meta,ComponentImplementationContainer::meta, "", "");
		Uml::SetParentRole(InfoProperty::meta_ComponentImplementationContainer_parent,InfoProperty::meta,ComponentImplementationContainer::meta, "", "");
		Uml::SetParentRole(MonolithprimaryArtifact::meta_ComponentImplementationContainer_parent,MonolithprimaryArtifact::meta,ComponentImplementationContainer::meta, "", "");
		Uml::SetParentRole(MonolithDeployRequirement::meta_ComponentImplementationContainer_parent,MonolithDeployRequirement::meta,ComponentImplementationContainer::meta, "", "");
		Uml::SetParentRole(ComponentImplementationContainer::meta_ComponentImplementations_parent,ComponentImplementationContainer::meta,ComponentImplementations::meta, "", "");
		Uml::SetParentRole(MonolithExecParameter::meta_ComponentImplementationContainer_parent,MonolithExecParameter::meta,ComponentImplementationContainer::meta, "", "");
		Uml::SetParentRole(ComponentImplementationReference::meta_PackageContainer_parent,ComponentImplementationReference::meta,PackageContainer::meta, "", "");
		Uml::SetParentRole(ComponentImplementation::meta_ComponentImplementationContainer_parent,ComponentImplementation::meta,ComponentImplementationContainer::meta, "", "");
		Uml::SetParentRole(PackageConfBasePackage::meta_PackageConfigurationContainer_parent,PackageConfBasePackage::meta,PackageConfigurationContainer::meta, "", "");
		Uml::SetParentRole(PackageConfReference::meta_PackageConfigurationContainer_parent,PackageConfReference::meta,PackageConfigurationContainer::meta, "", "");
		Uml::SetParentRole(PackageConfigurations::meta_RootFolder_parent,PackageConfigurations::meta,RootFolder::meta, "", "");
		Uml::SetParentRole(PackageConfigurationContainer::meta_PackageConfigurations_parent,PackageConfigurationContainer::meta,PackageConfigurations::meta, "", "");
		Uml::SetParentRole(PackageConfConfigProperty::meta_PackageConfigurationContainer_parent,PackageConfConfigProperty::meta,PackageConfigurationContainer::meta, "", "");
		Uml::SetParentRole(PackageConfSelectRequirement::meta_PackageConfigurationContainer_parent,PackageConfSelectRequirement::meta,PackageConfigurationContainer::meta, "", "");
		Uml::SetParentRole(PackageConfSpecializedConfig::meta_PackageConfigurationContainer_parent,PackageConfSpecializedConfig::meta,PackageConfigurationContainer::meta, "", "");
		Uml::SetParentRole(PackageConfigurationReference::meta_TopLevelPackageContainer_parent,PackageConfigurationReference::meta,TopLevelPackageContainer::meta, "", "");
		Uml::SetParentRole(PackageConfiguration::meta_PackageConfigurationContainer_parent,PackageConfiguration::meta,PackageConfigurationContainer::meta, "", "");
		Uml::SetParentRole(ImplementationArtifacts::meta_RootFolder_parent,ImplementationArtifacts::meta,RootFolder::meta, "", "");
		Uml::SetParentRole(ArtifactDependsOn::meta_ArtifactContainer_parent,ArtifactDependsOn::meta,ArtifactContainer::meta, "", "");
		Uml::SetParentRole(ArtifactDeployRequirement::meta_ArtifactContainer_parent,ArtifactDeployRequirement::meta,ArtifactContainer::meta, "", "");
		Uml::SetParentRole(ArtifactContainer::meta_ImplementationArtifacts_parent,ArtifactContainer::meta,ImplementationArtifacts::meta, "", "");
		Uml::SetParentRole(ArtifactExecParameter::meta_ArtifactContainer_parent,ArtifactExecParameter::meta,ArtifactContainer::meta, "", "");
		Uml::SetParentRole(ArtifactInfoProperty::meta_ArtifactContainer_parent,ArtifactInfoProperty::meta,ArtifactContainer::meta, "", "");
		Uml::SetParentRole(ImplementationArtifactReference::meta_ComponentImplementationContainer_parent,ImplementationArtifactReference::meta,ComponentImplementationContainer::meta, "", "");
		Uml::SetParentRole(ImplementationArtifactReference::meta_ArtifactContainer_parent,ImplementationArtifactReference::meta,ArtifactContainer::meta, "", "");
		Uml::SetParentRole(ImplementationArtifactReference::meta_StubProject_parent,ImplementationArtifactReference::meta,StubProject::meta, "", "");
		Uml::SetParentRole(ImplementationArtifactReference::meta_ServantProject_parent,ImplementationArtifactReference::meta,ServantProject::meta, "", "");
		Uml::SetParentRole(ImplementationArtifactReference::meta_ExecutorProject_parent,ImplementationArtifactReference::meta,ExecutorProject::meta, "", "");
		Uml::SetParentRole(ImplementationArtifact::meta_ArtifactContainer_parent,ImplementationArtifact::meta,ArtifactContainer::meta, "", "");
		Uml::SetParentRole(ImplementationArtifact::meta_Project_parent,ImplementationArtifact::meta,Project::meta, "", "");
		Uml::SetParentRole(ComponentTypes::meta_RootFolder_parent,ComponentTypes::meta,RootFolder::meta, "", "");
		Uml::SetParentRole(ComponentPropertyDescription::meta_ComponentContainer_parent,ComponentPropertyDescription::meta,ComponentContainer::meta, "", "");
		Uml::SetParentRole(ComponentProperty::meta_ComponentContainer_parent,ComponentProperty::meta,ComponentContainer::meta, "", "");
		Uml::SetParentRole(ComponentContainer::meta_ComponentTypes_parent,ComponentContainer::meta,ComponentTypes::meta, "", "");
		Uml::SetParentRole(ComponentConfigProperty::meta_ComponentContainer_parent,ComponentConfigProperty::meta,ComponentContainer::meta, "", "");
		Uml::SetParentRole(ComponentInfoProperty::meta_ComponentContainer_parent,ComponentInfoProperty::meta,ComponentContainer::meta, "", "");
		Uml::SetParentRole(TopLevelPackageContainer::meta_TopLevelPackages_parent,TopLevelPackageContainer::meta,TopLevelPackages::meta, "", "");
		Uml::SetParentRole(TopLevelPackages::meta_RootFolder_parent,TopLevelPackages::meta,RootFolder::meta, "", "");
		Uml::SetParentRole(TopLevelPackage::meta_TopLevelPackageContainer_parent,TopLevelPackage::meta,TopLevelPackageContainer::meta, "", "");
		Uml::SetParentRole(package::meta_TopLevelPackageContainer_parent,package::meta,TopLevelPackageContainer::meta, "", "");
		Uml::SetParentRole(ComponentPackages::meta_RootFolder_parent,ComponentPackages::meta,RootFolder::meta, "", "");
		Uml::SetParentRole(PackageInterface::meta_PackageContainer_parent,PackageInterface::meta,PackageContainer::meta, "", "");
		Uml::SetParentRole(Implementation::meta_PackageContainer_parent,Implementation::meta,PackageContainer::meta, "", "");
		Uml::SetParentRole(PackageContainer::meta_ComponentPackages_parent,PackageContainer::meta,ComponentPackages::meta, "", "");
		Uml::SetParentRole(PackageConfigProperty::meta_PackageContainer_parent,PackageConfigProperty::meta,PackageContainer::meta, "", "");
		Uml::SetParentRole(PackageInfoProperty::meta_PackageContainer_parent,PackageInfoProperty::meta,PackageContainer::meta, "", "");
		Uml::SetParentRole(ComponentPackage::meta_ComponentAssembly_parent,ComponentPackage::meta,ComponentAssembly::meta, "", "");
		Uml::SetParentRole(ComponentPackage::meta_PackageConfigurationContainer_parent,ComponentPackage::meta,PackageConfigurationContainer::meta, "", "");
		Uml::SetParentRole(ComponentPackage::meta_PackageContainer_parent,ComponentPackage::meta,PackageContainer::meta, "", "");
		Uml::SetParentRole(ComponentPackageReference::meta_ComponentAssembly_parent,ComponentPackageReference::meta,ComponentAssembly::meta, "", "");
		Uml::SetParentRole(ComponentPackageReference::meta_PackageConfigurationContainer_parent,ComponentPackageReference::meta,PackageConfigurationContainer::meta, "", "");
		Uml::SetParentRole(Requirement::meta_ComponentAssembly_parent,Requirement::meta,ComponentAssembly::meta, "", "");
		Uml::SetParentRole(Requirement::meta_PackageConfigurationContainer_parent,Requirement::meta,PackageConfigurationContainer::meta, "", "");
		Uml::SetParentRole(Requirement::meta_ArtifactContainer_parent,Requirement::meta,ArtifactContainer::meta, "", "");
		Uml::SetParentRole(Capability::meta_ComponentImplementationContainer_parent,Capability::meta,ComponentImplementationContainer::meta, "", "");
		Uml::SetParentRole(DataType::meta_SatisfierProperty_parent,DataType::meta,SatisfierProperty::meta, "", "");
		Uml::SetParentRole(DataType::meta_ComponentPropertyDescription_parent,DataType::meta,ComponentPropertyDescription::meta, "", "");
		Uml::SetParentRole(DataType::meta_Property_parent,DataType::meta,Property::meta, "", "");
		Uml::SetParentRole(SatisfierProperty::meta_SharedResource_parent,SatisfierProperty::meta,SharedResource::meta, "", "");
		Uml::SetParentRole(SatisfierProperty::meta_Resource_parent,SatisfierProperty::meta,Resource::meta, "", "");
		Uml::SetParentRole(SatisfierProperty::meta_RequirementSatisfier_parent,SatisfierProperty::meta,RequirementSatisfier::meta, "", "");
		Uml::SetParentRole(ImplementationDependency::meta_ComponentImplementationContainer_parent,ImplementationDependency::meta,ComponentImplementationContainer::meta, "", "");
		Uml::SetParentRole(ImplementationRequirement::meta_ComponentImplementationContainer_parent,ImplementationRequirement::meta,ComponentImplementationContainer::meta, "", "");
		Uml::SetParentRole(Property::meta_ComponentAssembly_parent,Property::meta,ComponentAssembly::meta, "", "");
		Uml::SetParentRole(Property::meta_ComponentImplementationContainer_parent,Property::meta,ComponentImplementationContainer::meta, "", "");
		Uml::SetParentRole(Property::meta_PackageConfigurationContainer_parent,Property::meta,PackageConfigurationContainer::meta, "", "");
		Uml::SetParentRole(Property::meta_ArtifactContainer_parent,Property::meta,ArtifactContainer::meta, "", "");
		Uml::SetParentRole(Property::meta_ComponentContainer_parent,Property::meta,ComponentContainer::meta, "", "");
		Uml::SetParentRole(Property::meta_PackageContainer_parent,Property::meta,PackageContainer::meta, "", "");
		Uml::SetParentRole(Property::meta_RequirementBase_parent,Property::meta,RequirementBase::meta, "", "");
		Uml::SetParentRole(Property::meta_Domain_parent,Property::meta,Domain::meta, "", "");
		Uml::SetParentRole(Elements::meta_Domain_parent,Elements::meta,Domain::meta, "", "");
		Uml::SetParentRole(Targets::meta_RootFolder_parent,Targets::meta,RootFolder::meta, "", "");
		Uml::SetParentRole(Shares::meta_Domain_parent,Shares::meta,Domain::meta, "", "");
		Uml::SetParentRole(Node2Interconnect::meta_Domain_parent,Node2Interconnect::meta,Domain::meta, "", "");
		Uml::SetParentRole(Bridge2Interconnect::meta_Domain_parent,Bridge2Interconnect::meta,Domain::meta, "", "");
		Uml::SetParentRole(Interconnect2Node::meta_Domain_parent,Interconnect2Node::meta,Domain::meta, "", "");
		Uml::SetParentRole(Interconnect2Bridge::meta_Domain_parent,Interconnect2Bridge::meta,Domain::meta, "", "");
		Uml::SetParentRole(Domain::meta_Targets_parent,Domain::meta,Targets::meta, "", "");
		Uml::SetParentRole(OutEventPort::meta_ComponentAssembly_parent,OutEventPort::meta,ComponentAssembly::meta, "", "");
		Uml::SetParentRole(ProvidedRequestPort::meta_ComponentAssembly_parent,ProvidedRequestPort::meta,ComponentAssembly::meta, "", "");
		Uml::SetParentRole(RequiredRequestPort::meta_ComponentAssembly_parent,RequiredRequestPort::meta,ComponentAssembly::meta, "", "");
		Uml::SetParentRole(InEventPort::meta_ComponentAssembly_parent,InEventPort::meta,ComponentAssembly::meta, "", "");
		Uml::SetParentRole(Port::meta_EventChannelConfiguration_parent,Port::meta,EventChannelConfiguration::meta, "", "");
		Uml::SetParentRole(Port::meta_Component_parent,Port::meta,Component::meta, "", "");
		Uml::SetParentRole(ComponentRef::meta_EventChannelConfiguration_parent,ComponentRef::meta,EventChannelConfiguration::meta, "", "");
		Uml::SetParentRole(ComponentRef::meta_DeploymentPlan_parent,ComponentRef::meta,DeploymentPlan::meta, "", "");
		Uml::SetParentRole(ComponentRef::meta_ComponentImplementationContainer_parent,ComponentRef::meta,ComponentImplementationContainer::meta, "", "");
		Uml::SetParentRole(ComponentRef::meta_ComponentContainer_parent,ComponentRef::meta,ComponentContainer::meta, "", "");
		Uml::SetParentRole(ComponentRef::meta_PackageContainer_parent,ComponentRef::meta,PackageContainer::meta, "", "");
		Uml::SetParentRole(ComponentRef::meta_Package_parent,ComponentRef::meta,Package::meta, "", "");
		Uml::SetParentRole(ComponentRef::meta_File_parent,ComponentRef::meta,File::meta, "", "");
		Uml::SetParentRole(ManagesComponent::meta_File_parent,ManagesComponent::meta,File::meta, "", "");
		Uml::SetParentRole(ManagesComponent::meta_Package_parent,ManagesComponent::meta,Package::meta, "", "");
		Uml::SetParentRole(Component::meta_ComponentAssembly_parent,Component::meta,ComponentAssembly::meta, "", "");
		Uml::SetParentRole(OnewayOperation::meta_HasOperations_parent,OnewayOperation::meta,HasOperations::meta, "", "");
		Uml::SetParentRole(LookupOperation::meta_ComponentFactory_parent,LookupOperation::meta,ComponentFactory::meta, "", "");
		Uml::SetParentRole(FactoryOperation::meta_ComponentFactory_parent,FactoryOperation::meta,ComponentFactory::meta, "", "");
		Uml::SetParentRole(FactoryOperation::meta_ObjectByValue_parent,FactoryOperation::meta,ObjectByValue::meta, "", "");
		Uml::SetParentRole(OperationBase::meta_BenchmarkAnalysis_parent,OperationBase::meta,BenchmarkAnalysis::meta, "", "");
		Uml::SetParentRole(InParameter::meta_OperationBase_parent,InParameter::meta,OperationBase::meta, "", "");
		Uml::SetParentRole(InoutParameter::meta_TwowayOperation_parent,InoutParameter::meta,TwowayOperation::meta, "", "");
		Uml::SetParentRole(OutParameter::meta_TwowayOperation_parent,OutParameter::meta,TwowayOperation::meta, "", "");
		Uml::SetParentRole(ReturnType::meta_TwowayOperation_parent,ReturnType::meta,TwowayOperation::meta, "", "");
		Uml::SetParentRole(TwowayOperation::meta_HasOperations_parent,TwowayOperation::meta,HasOperations::meta, "", "");
		Uml::SetParentRole(Inherits::meta_Inheritable_parent,Inherits::meta,Inheritable::meta, "", "");
		Uml::SetParentRole(Event::meta_BenchmarkAnalysis_parent,Event::meta,BenchmarkAnalysis::meta, "", "");
		Uml::SetParentRole(Supports::meta_SupportsInterfaces_parent,Supports::meta,SupportsInterfaces::meta, "", "");
		Uml::SetParentRole(ReadonlyAttribute::meta_Inheritable_parent,ReadonlyAttribute::meta,Inheritable::meta, "", "");
		Uml::SetParentRole(ReadonlyAttribute::meta_Component_parent,ReadonlyAttribute::meta,Component::meta, "", "");
		Uml::SetParentRole(LookupKey::meta_ComponentFactory_parent,LookupKey::meta,ComponentFactory::meta, "", "");
		Uml::SetParentRole(SetException::meta_Attribute_parent,SetException::meta,Attribute::meta, "", "");
		Uml::SetParentRole(GetException::meta_ReadonlyAttribute_parent,GetException::meta,ReadonlyAttribute::meta, "", "");
		Uml::SetParentRole(PrivateFlag::meta_ObjectByValue_parent,PrivateFlag::meta,ObjectByValue::meta, "", "");
		Uml::SetParentRole(MakeMemberPrivate::meta_ObjectByValue_parent,MakeMemberPrivate::meta,ObjectByValue::meta, "", "");
		Uml::SetParentRole(AttributeMember::meta_ReadonlyAttribute_parent,AttributeMember::meta,ReadonlyAttribute::meta, "", "");
		Uml::SetParentRole(EnumValue::meta_Enum_parent,EnumValue::meta,Enum::meta, "", "");
		Uml::SetParentRole(Label::meta_SwitchedAggregate_parent,Label::meta,SwitchedAggregate::meta, "", "");
		Uml::SetParentRole(LabelConnection::meta_SwitchedAggregate_parent,LabelConnection::meta,SwitchedAggregate::meta, "", "");
		Uml::SetParentRole(Discriminator::meta_SwitchedAggregate_parent,Discriminator::meta,SwitchedAggregate::meta, "", "");
		Uml::SetParentRole(NoInheritable::meta_HasOperations_parent,NoInheritable::meta,HasOperations::meta, "", "");
		Uml::SetParentRole(Member::meta_ObjectByValue_parent,Member::meta,ObjectByValue::meta, "", "");
		Uml::SetParentRole(Member::meta_SwitchedAggregate_parent,Member::meta,SwitchedAggregate::meta, "", "");
		Uml::SetParentRole(Member::meta_Aggregate_parent,Member::meta,Aggregate::meta, "", "");
		Uml::SetParentRole(Member::meta_Exception_parent,Member::meta,Exception::meta, "", "");
		Uml::SetParentRole(NamedType::meta_File_parent,NamedType::meta,File::meta, "", "");
		Uml::SetParentRole(NamedType::meta_Package_parent,NamedType::meta,Package::meta, "", "");
		Uml::SetParentRole(PredefinedType::meta_PredefinedTypes_parent,PredefinedType::meta,PredefinedTypes::meta, "", "");
		Uml::SetParentRole(PredefinedTypes::meta_RootFolder_parent,PredefinedTypes::meta,RootFolder::meta, "", "");
		Uml::SetParentRole(Package::meta_Package_parent,Package::meta,Package::meta, "", "");
		Uml::SetParentRole(Package::meta_File_parent,Package::meta,File::meta, "", "");
		Uml::SetParentRole(File::meta_InterfaceDefinitions_parent,File::meta,InterfaceDefinitions::meta, "", "");
		Uml::SetParentRole(Exception::meta_HasOperations_parent,Exception::meta,HasOperations::meta, "", "");
		Uml::SetParentRole(Exception::meta_Package_parent,Exception::meta,Package::meta, "", "");
		Uml::SetParentRole(Exception::meta_File_parent,Exception::meta,File::meta, "", "");
		Uml::SetParentRole(ExceptionRef::meta_HasExceptions_parent,ExceptionRef::meta,HasExceptions::meta, "", "");
		Uml::SetParentRole(InterfaceDefinitions::meta_RootFolder_parent,InterfaceDefinitions::meta,RootFolder::meta, "", "");
		Uml::SetParentRole(Constant::meta_HasOperations_parent,Constant::meta,HasOperations::meta, "", "");
		Uml::SetParentRole(Constant::meta_Package_parent,Constant::meta,Package::meta, "", "");
		Uml::SetParentRole(Constant::meta_File_parent,Constant::meta,File::meta, "", "");
		Uml::SetParentRole(FileRef::meta_File_parent,FileRef::meta,File::meta, "", "");
		Uml::SetParentRole(FileRef::meta_StubProject_parent,FileRef::meta,StubProject::meta, "", "");
		Uml::SetParentRole(ComponentBuild::meta_RootFolder_parent,ComponentBuild::meta,RootFolder::meta, "", "");
		Uml::SetParentRole(Project::meta_MPC_parent,Project::meta,MPC::meta, "", "");
		Uml::SetParentRole(ExternalResources::meta_Project_parent,ExternalResources::meta,Project::meta, "", "");
		Uml::SetParentRole(ExtResourceConn::meta_Project_parent,ExtResourceConn::meta,Project::meta, "", "");
		Uml::SetParentRole(ComponentLib::meta_Project_parent,ComponentLib::meta,Project::meta, "", "");
		Uml::SetParentRole(MPC::meta_ComponentBuild_parent,MPC::meta,ComponentBuild::meta, "", "");
		Uml::SetParentRole(Workspaces::meta_MPC_parent,Workspaces::meta,MPC::meta, "", "");
		Uml::SetParentRole(RootFolder::meta_RootFolder_parent,RootFolder::meta,RootFolder::meta, "", "");
		// composition parentroles 
		Uml::SetChildRole(RTEC_Proxy::meta_RT_Info_Param_children,RTEC_Proxy::meta,RT_Info_Param::meta, "", "");
		Uml::SetChildRole(RTEC_Proxy_Supplier::meta_Include_RT_Info_children,RTEC_Proxy_Supplier::meta,Include_RT_Info::meta, "", "");
		Uml::SetChildRole(RTEC_Proxy_Supplier::meta_Include_RTEC_Filter_Group_children,RTEC_Proxy_Supplier::meta,Include_RTEC_Filter_Group::meta, "", "");
		Uml::SetChildRole(RTEC_Proxy_Supplier::meta_RTEC_Filter_Group_children,RTEC_Proxy_Supplier::meta,RTEC_Filter_Group::meta, "", "");
		Uml::SetChildRole(RTEC_Proxy_Consumer::meta_RTEC_Resource_Factory_Reference_child,RTEC_Proxy_Consumer::meta,RTEC_Resource_Factory_Reference::meta, "", "");
		Uml::SetChildRole(RTEC_Proxy_Consumer::meta_Use_RT_Info_children,RTEC_Proxy_Consumer::meta,Use_RT_Info::meta, "", "");
		Uml::SetChildRole(EventChannelConfiguration::meta_Use_RT_Info_children,EventChannelConfiguration::meta,Use_RT_Info::meta, "", "");
		Uml::SetChildRole(EventChannelConfiguration::meta_RTEC_Resource_Factory_children,EventChannelConfiguration::meta,RTEC_Resource_Factory::meta, "", "");
		Uml::SetChildRole(EventChannelConfiguration::meta_Use_RTEC_Filter_children,EventChannelConfiguration::meta,Use_RTEC_Filter::meta, "", "");
		Uml::SetChildRole(EventChannelConfiguration::meta_RTEC_Connection_children,EventChannelConfiguration::meta,RTEC_Connection::meta, "", "");
		Uml::SetChildRole(EventChannelConfiguration::meta_Proxy_children,EventChannelConfiguration::meta,Proxy::meta, "", "");
		Uml::SetChildRole(EventChannelConfiguration::meta_ComponentRef_children,EventChannelConfiguration::meta,ComponentRef::meta, "", "");
		Uml::SetChildRole(EventChannelConfiguration::meta_Port_children,EventChannelConfiguration::meta,Port::meta, "", "");
		Uml::SetChildRole(EventChannelConfig::meta_EventChannelConfiguration_children,EventChannelConfig::meta,EventChannelConfiguration::meta, "", "");
		Uml::SetChildRole(ComponentAnalyses::meta_BenchmarkAnalysis_children,ComponentAnalyses::meta,BenchmarkAnalysis::meta, "", "");
		Uml::SetChildRole(BenchmarkAnalysis::meta_TimerConnection_children,BenchmarkAnalysis::meta,TimerConnection::meta, "", "");
		Uml::SetChildRole(BenchmarkAnalysis::meta_OperationBase_children,BenchmarkAnalysis::meta,OperationBase::meta, "", "");
		Uml::SetChildRole(BenchmarkAnalysis::meta_MetricConnection_children,BenchmarkAnalysis::meta,MetricConnection::meta, "", "");
		Uml::SetChildRole(BenchmarkAnalysis::meta_DataAnalysisBase_children,BenchmarkAnalysis::meta,DataAnalysisBase::meta, "", "");
		Uml::SetChildRole(BenchmarkAnalysis::meta_MetricsBase_children,BenchmarkAnalysis::meta,MetricsBase::meta, "", "");
		Uml::SetChildRole(BenchmarkAnalysis::meta_TimeProbe_children,BenchmarkAnalysis::meta,TimeProbe::meta, "", "");
		Uml::SetChildRole(BenchmarkAnalysis::meta_OperationRef_children,BenchmarkAnalysis::meta,OperationRef::meta, "", "");
		Uml::SetChildRole(BenchmarkAnalysis::meta_EventRef_children,BenchmarkAnalysis::meta,EventRef::meta, "", "");
		Uml::SetChildRole(BenchmarkAnalysis::meta_Event_children,BenchmarkAnalysis::meta,Event::meta, "", "");
		Uml::SetChildRole(BenchmarkAnalysis::meta_TimerEventSinkConn_children,BenchmarkAnalysis::meta,TimerEventSinkConn::meta, "", "");
		Uml::SetChildRole(BenchmarkAnalysis::meta_TaskSet_children,BenchmarkAnalysis::meta,TaskSet::meta, "", "");
		Uml::SetChildRole(BenchmarkAnalysis::meta_Task_children,BenchmarkAnalysis::meta,Task::meta, "", "");
		Uml::SetChildRole(BenchmarkAnalysis::meta_WorkloadCharacteristics_children,BenchmarkAnalysis::meta,WorkloadCharacteristics::meta, "", "");
		Uml::SetChildRole(MetricsBase::meta_DataAnalysisBase_children,MetricsBase::meta,DataAnalysisBase::meta, "", "");
		Uml::SetChildRole(Interconnect::meta_Resource_children,Interconnect::meta,Resource::meta, "", "");
		Uml::SetChildRole(Node::meta_Resource_children,Node::meta,Resource::meta, "", "");
		Uml::SetChildRole(Resource::meta_SatisfierProperty_children,Resource::meta,SatisfierProperty::meta, "", "");
		Uml::SetChildRole(Bridge::meta_Resource_children,Bridge::meta,Resource::meta, "", "");
		Uml::SetChildRole(SharedResource::meta_SatisfierProperty_children,SharedResource::meta,SatisfierProperty::meta, "", "");
		Uml::SetChildRole(DeploymentPlans::meta_DeploymentPlan_children,DeploymentPlans::meta,DeploymentPlan::meta, "", "");
		Uml::SetChildRole(DeploymentPlan::meta_InstanceMapping_children,DeploymentPlan::meta,InstanceMapping::meta, "", "");
		Uml::SetChildRole(DeploymentPlan::meta_NodeReference_children,DeploymentPlan::meta,NodeReference::meta, "", "");
		Uml::SetChildRole(DeploymentPlan::meta_CollocationGroup_children,DeploymentPlan::meta,CollocationGroup::meta, "", "");
		Uml::SetChildRole(DeploymentPlan::meta_ComponentRef_children,DeploymentPlan::meta,ComponentRef::meta, "", "");
		Uml::SetChildRole(ComponentAssembly::meta_invoke_children,ComponentAssembly::meta,invoke::meta, "", "");
		Uml::SetChildRole(ComponentAssembly::meta_ComponentPackage_children,ComponentAssembly::meta,ComponentPackage::meta, "", "");
		Uml::SetChildRole(ComponentAssembly::meta_AssemblyselectRequirement_children,ComponentAssembly::meta,AssemblyselectRequirement::meta, "", "");
		Uml::SetChildRole(ComponentAssembly::meta_Property_children,ComponentAssembly::meta,Property::meta, "", "");
		Uml::SetChildRole(ComponentAssembly::meta_ComponentPackageReference_children,ComponentAssembly::meta,ComponentPackageReference::meta, "", "");
		Uml::SetChildRole(ComponentAssembly::meta_deliverTo_children,ComponentAssembly::meta,deliverTo::meta, "", "");
		Uml::SetChildRole(ComponentAssembly::meta_AssemblyDeployRequirement_children,ComponentAssembly::meta,AssemblyDeployRequirement::meta, "", "");
		Uml::SetChildRole(ComponentAssembly::meta_Requirement_children,ComponentAssembly::meta,Requirement::meta, "", "");
		Uml::SetChildRole(ComponentAssembly::meta_publish_children,ComponentAssembly::meta,publish::meta, "", "");
		Uml::SetChildRole(ComponentAssembly::meta_PublishConnector_children,ComponentAssembly::meta,PublishConnector::meta, "", "");
		Uml::SetChildRole(ComponentAssembly::meta_emit_children,ComponentAssembly::meta,emit::meta, "", "");
		Uml::SetChildRole(ComponentAssembly::meta_Component_children,ComponentAssembly::meta,Component::meta, "", "");
		Uml::SetChildRole(ComponentAssembly::meta_AssemblyConfigProperty_children,ComponentAssembly::meta,AssemblyConfigProperty::meta, "", "");
		Uml::SetChildRole(ComponentAssembly::meta_ComponentAssembly_children,ComponentAssembly::meta,ComponentAssembly::meta, "", "");
		Uml::SetChildRole(ComponentAssembly::meta_ProvidedRequestPort_children,ComponentAssembly::meta,ProvidedRequestPort::meta, "", "");
		Uml::SetChildRole(ComponentAssembly::meta_RequiredRequestPort_children,ComponentAssembly::meta,RequiredRequestPort::meta, "", "");
		Uml::SetChildRole(ComponentAssembly::meta_InEventPort_children,ComponentAssembly::meta,InEventPort::meta, "", "");
		Uml::SetChildRole(ComponentAssembly::meta_OutEventPort_children,ComponentAssembly::meta,OutEventPort::meta, "", "");
		Uml::SetChildRole(ComponentAssembly::meta_EventSinkDelegate_children,ComponentAssembly::meta,EventSinkDelegate::meta, "", "");
		Uml::SetChildRole(ComponentAssembly::meta_EventSourceDelegate_children,ComponentAssembly::meta,EventSourceDelegate::meta, "", "");
		Uml::SetChildRole(ComponentAssembly::meta_FacetDelegate_children,ComponentAssembly::meta,FacetDelegate::meta, "", "");
		Uml::SetChildRole(ComponentAssembly::meta_ReceptacleDelegate_children,ComponentAssembly::meta,ReceptacleDelegate::meta, "", "");
		Uml::SetChildRole(ComponentAssembly::meta_ExternalReferenceEndPoint_children,ComponentAssembly::meta,ExternalReferenceEndPoint::meta, "", "");
		Uml::SetChildRole(ComponentAssembly::meta_ExternalDelegate_children,ComponentAssembly::meta,ExternalDelegate::meta, "", "");
		Uml::SetChildRole(ComponentImplementations::meta_ComponentImplementationContainer_children,ComponentImplementations::meta,ComponentImplementationContainer::meta, "", "");
		Uml::SetChildRole(ComponentImplementationContainer::meta_ImplementationDependsOn_children,ComponentImplementationContainer::meta,ImplementationDependsOn::meta, "", "");
		Uml::SetChildRole(ComponentImplementationContainer::meta_ComponentImplementation_child,ComponentImplementationContainer::meta,ComponentImplementation::meta, "", "");
		Uml::SetChildRole(ComponentImplementationContainer::meta_Implements_children,ComponentImplementationContainer::meta,Implements::meta, "", "");
		Uml::SetChildRole(ComponentImplementationContainer::meta_ComponentRef_children,ComponentImplementationContainer::meta,ComponentRef::meta, "", "");
		Uml::SetChildRole(ComponentImplementationContainer::meta_MonolithDeployRequirement_children,ComponentImplementationContainer::meta,MonolithDeployRequirement::meta, "", "");
		Uml::SetChildRole(ComponentImplementationContainer::meta_MonolithprimaryArtifact_children,ComponentImplementationContainer::meta,MonolithprimaryArtifact::meta, "", "");
		Uml::SetChildRole(ComponentImplementationContainer::meta_MonolithExecParameter_children,ComponentImplementationContainer::meta,MonolithExecParameter::meta, "", "");
		Uml::SetChildRole(ComponentImplementationContainer::meta_ImplementationDependency_children,ComponentImplementationContainer::meta,ImplementationDependency::meta, "", "");
		Uml::SetChildRole(ComponentImplementationContainer::meta_ConfigProperty_children,ComponentImplementationContainer::meta,ConfigProperty::meta, "", "");
		Uml::SetChildRole(ComponentImplementationContainer::meta_ImplementationCapability_children,ComponentImplementationContainer::meta,ImplementationCapability::meta, "", "");
		Uml::SetChildRole(ComponentImplementationContainer::meta_InfoProperty_children,ComponentImplementationContainer::meta,InfoProperty::meta, "", "");
		Uml::SetChildRole(ComponentImplementationContainer::meta_Property_children,ComponentImplementationContainer::meta,Property::meta, "", "");
		Uml::SetChildRole(ComponentImplementationContainer::meta_Capability_children,ComponentImplementationContainer::meta,Capability::meta, "", "");
		Uml::SetChildRole(ComponentImplementationContainer::meta_ImplementationRequirement_children,ComponentImplementationContainer::meta,ImplementationRequirement::meta, "", "");
		Uml::SetChildRole(ComponentImplementationContainer::meta_ImplementationArtifactReference_children,ComponentImplementationContainer::meta,ImplementationArtifactReference::meta, "", "");
		Uml::SetChildRole(PackageConfigurations::meta_PackageConfigurationContainer_children,PackageConfigurations::meta,PackageConfigurationContainer::meta, "", "");
		Uml::SetChildRole(PackageConfigurationContainer::meta_PackageConfReference_child,PackageConfigurationContainer::meta,PackageConfReference::meta, "", "");
		Uml::SetChildRole(PackageConfigurationContainer::meta_PackageConfBasePackage_child,PackageConfigurationContainer::meta,PackageConfBasePackage::meta, "", "");
		Uml::SetChildRole(PackageConfigurationContainer::meta_ComponentPackage_child,PackageConfigurationContainer::meta,ComponentPackage::meta, "", "");
		Uml::SetChildRole(PackageConfigurationContainer::meta_ComponentPackageReference_child,PackageConfigurationContainer::meta,ComponentPackageReference::meta, "", "");
		Uml::SetChildRole(PackageConfigurationContainer::meta_PackageConfConfigProperty_children,PackageConfigurationContainer::meta,PackageConfConfigProperty::meta, "", "");
		Uml::SetChildRole(PackageConfigurationContainer::meta_PackageConfSelectRequirement_children,PackageConfigurationContainer::meta,PackageConfSelectRequirement::meta, "", "");
		Uml::SetChildRole(PackageConfigurationContainer::meta_Requirement_children,PackageConfigurationContainer::meta,Requirement::meta, "", "");
		Uml::SetChildRole(PackageConfigurationContainer::meta_Property_children,PackageConfigurationContainer::meta,Property::meta, "", "");
		Uml::SetChildRole(PackageConfigurationContainer::meta_PackageConfSpecializedConfig_child,PackageConfigurationContainer::meta,PackageConfSpecializedConfig::meta, "", "");
		Uml::SetChildRole(PackageConfigurationContainer::meta_PackageConfiguration_children,PackageConfigurationContainer::meta,PackageConfiguration::meta, "", "");
		Uml::SetChildRole(ImplementationArtifacts::meta_ArtifactContainer_children,ImplementationArtifacts::meta,ArtifactContainer::meta, "", "");
		Uml::SetChildRole(ArtifactContainer::meta_ImplementationArtifactReference_children,ArtifactContainer::meta,ImplementationArtifactReference::meta, "", "");
		Uml::SetChildRole(ArtifactContainer::meta_ArtifactDependsOn_children,ArtifactContainer::meta,ArtifactDependsOn::meta, "", "");
		Uml::SetChildRole(ArtifactContainer::meta_ArtifactDeployRequirement_children,ArtifactContainer::meta,ArtifactDeployRequirement::meta, "", "");
		Uml::SetChildRole(ArtifactContainer::meta_Requirement_children,ArtifactContainer::meta,Requirement::meta, "", "");
		Uml::SetChildRole(ArtifactContainer::meta_ArtifactExecParameter_children,ArtifactContainer::meta,ArtifactExecParameter::meta, "", "");
		Uml::SetChildRole(ArtifactContainer::meta_ArtifactInfoProperty_children,ArtifactContainer::meta,ArtifactInfoProperty::meta, "", "");
		Uml::SetChildRole(ArtifactContainer::meta_Property_children,ArtifactContainer::meta,Property::meta, "", "");
		Uml::SetChildRole(ArtifactContainer::meta_ImplementationArtifact_children,ArtifactContainer::meta,ImplementationArtifact::meta, "", "");
		Uml::SetChildRole(ComponentTypes::meta_ComponentContainer_children,ComponentTypes::meta,ComponentContainer::meta, "", "");
		Uml::SetChildRole(ComponentPropertyDescription::meta_DataType_child,ComponentPropertyDescription::meta,DataType::meta, "", "");
		Uml::SetChildRole(ComponentContainer::meta_ComponentConfigProperty_children,ComponentContainer::meta,ComponentConfigProperty::meta, "", "");
		Uml::SetChildRole(ComponentContainer::meta_Property_children,ComponentContainer::meta,Property::meta, "", "");
		Uml::SetChildRole(ComponentContainer::meta_ComponentProperty_children,ComponentContainer::meta,ComponentProperty::meta, "", "");
		Uml::SetChildRole(ComponentContainer::meta_ComponentPropertyDescription_children,ComponentContainer::meta,ComponentPropertyDescription::meta, "", "");
		Uml::SetChildRole(ComponentContainer::meta_ComponentRef_child,ComponentContainer::meta,ComponentRef::meta, "", "");
		Uml::SetChildRole(ComponentContainer::meta_ComponentInfoProperty_children,ComponentContainer::meta,ComponentInfoProperty::meta, "", "");
		Uml::SetChildRole(TopLevelPackageContainer::meta_TopLevelPackage_child,TopLevelPackageContainer::meta,TopLevelPackage::meta, "", "");
		Uml::SetChildRole(TopLevelPackageContainer::meta_PackageConfigurationReference_child,TopLevelPackageContainer::meta,PackageConfigurationReference::meta, "", "");
		Uml::SetChildRole(TopLevelPackageContainer::meta_package_child,TopLevelPackageContainer::meta,package::meta, "", "");
		Uml::SetChildRole(TopLevelPackages::meta_TopLevelPackageContainer_children,TopLevelPackages::meta,TopLevelPackageContainer::meta, "", "");
		Uml::SetChildRole(ComponentPackages::meta_PackageContainer_children,ComponentPackages::meta,PackageContainer::meta, "", "");
		Uml::SetChildRole(PackageContainer::meta_Implementation_children,PackageContainer::meta,Implementation::meta, "", "");
		Uml::SetChildRole(PackageContainer::meta_PackageInterface_child,PackageContainer::meta,PackageInterface::meta, "", "");
		Uml::SetChildRole(PackageContainer::meta_ComponentImplementationReference_children,PackageContainer::meta,ComponentImplementationReference::meta, "", "");
		Uml::SetChildRole(PackageContainer::meta_ComponentRef_children,PackageContainer::meta,ComponentRef::meta, "", "");
		Uml::SetChildRole(PackageContainer::meta_PackageConfigProperty_children,PackageContainer::meta,PackageConfigProperty::meta, "", "");
		Uml::SetChildRole(PackageContainer::meta_Property_children,PackageContainer::meta,Property::meta, "", "");
		Uml::SetChildRole(PackageContainer::meta_PackageInfoProperty_children,PackageContainer::meta,PackageInfoProperty::meta, "", "");
		Uml::SetChildRole(PackageContainer::meta_ComponentPackage_child,PackageContainer::meta,ComponentPackage::meta, "", "");
		Uml::SetChildRole(SatisfierProperty::meta_DataType_child,SatisfierProperty::meta,DataType::meta, "", "");
		Uml::SetChildRole(RequirementSatisfier::meta_SatisfierProperty_children,RequirementSatisfier::meta,SatisfierProperty::meta, "", "");
		Uml::SetChildRole(RequirementBase::meta_Property_children,RequirementBase::meta,Property::meta, "", "");
		Uml::SetChildRole(Property::meta_DataType_child,Property::meta,DataType::meta, "", "");
		Uml::SetChildRole(Targets::meta_Domain_children,Targets::meta,Domain::meta, "", "");
		Uml::SetChildRole(Domain::meta_Property_children,Domain::meta,Property::meta, "", "");
		Uml::SetChildRole(Domain::meta_Elements_children,Domain::meta,Elements::meta, "", "");
		Uml::SetChildRole(Domain::meta_Shares_children,Domain::meta,Shares::meta, "", "");
		Uml::SetChildRole(Domain::meta_Interconnect2Node_children,Domain::meta,Interconnect2Node::meta, "", "");
		Uml::SetChildRole(Domain::meta_Node2Interconnect_children,Domain::meta,Node2Interconnect::meta, "", "");
		Uml::SetChildRole(Domain::meta_Interconnect2Bridge_children,Domain::meta,Interconnect2Bridge::meta, "", "");
		Uml::SetChildRole(Domain::meta_Bridge2Interconnect_children,Domain::meta,Bridge2Interconnect::meta, "", "");
		Uml::SetChildRole(ComponentFactory::meta_LookupKey_child,ComponentFactory::meta,LookupKey::meta, "", "");
		Uml::SetChildRole(ComponentFactory::meta_FactoryOperation_children,ComponentFactory::meta,FactoryOperation::meta, "", "");
		Uml::SetChildRole(ComponentFactory::meta_LookupOperation_children,ComponentFactory::meta,LookupOperation::meta, "", "");
		Uml::SetChildRole(Component::meta_ReadonlyAttribute_children,Component::meta,ReadonlyAttribute::meta, "", "");
		Uml::SetChildRole(Component::meta_Port_children,Component::meta,Port::meta, "", "");
		Uml::SetChildRole(OperationBase::meta_InParameter_children,OperationBase::meta,InParameter::meta, "", "");
		Uml::SetChildRole(HasExceptions::meta_ExceptionRef_children,HasExceptions::meta,ExceptionRef::meta, "", "");
		Uml::SetChildRole(TwowayOperation::meta_OutParameter_children,TwowayOperation::meta,OutParameter::meta, "", "");
		Uml::SetChildRole(TwowayOperation::meta_InoutParameter_children,TwowayOperation::meta,InoutParameter::meta, "", "");
		Uml::SetChildRole(TwowayOperation::meta_ReturnType_child,TwowayOperation::meta,ReturnType::meta, "", "");
		Uml::SetChildRole(HasOperations::meta_TwowayOperation_children,HasOperations::meta,TwowayOperation::meta, "", "");
		Uml::SetChildRole(HasOperations::meta_OnewayOperation_children,HasOperations::meta,OnewayOperation::meta, "", "");
		Uml::SetChildRole(HasOperations::meta_Exception_children,HasOperations::meta,Exception::meta, "", "");
		Uml::SetChildRole(HasOperations::meta_Constant_children,HasOperations::meta,Constant::meta, "", "");
		Uml::SetChildRole(HasOperations::meta_NoInheritable_children,HasOperations::meta,NoInheritable::meta, "", "");
		Uml::SetChildRole(SupportsInterfaces::meta_Supports_children,SupportsInterfaces::meta,Supports::meta, "", "");
		Uml::SetChildRole(ReadonlyAttribute::meta_AttributeMember_child,ReadonlyAttribute::meta,AttributeMember::meta, "", "");
		Uml::SetChildRole(ReadonlyAttribute::meta_GetException_children,ReadonlyAttribute::meta,GetException::meta, "", "");
		Uml::SetChildRole(Attribute::meta_SetException_children,Attribute::meta,SetException::meta, "", "");
		Uml::SetChildRole(ObjectByValue::meta_PrivateFlag_children,ObjectByValue::meta,PrivateFlag::meta, "", "");
		Uml::SetChildRole(ObjectByValue::meta_FactoryOperation_children,ObjectByValue::meta,FactoryOperation::meta, "", "");
		Uml::SetChildRole(ObjectByValue::meta_MakeMemberPrivate_children,ObjectByValue::meta,MakeMemberPrivate::meta, "", "");
		Uml::SetChildRole(ObjectByValue::meta_Member_children,ObjectByValue::meta,Member::meta, "", "");
		Uml::SetChildRole(Inheritable::meta_ReadonlyAttribute_children,Inheritable::meta,ReadonlyAttribute::meta, "", "");
		Uml::SetChildRole(Inheritable::meta_Inherits_children,Inheritable::meta,Inherits::meta, "", "");
		Uml::SetChildRole(Aggregate::meta_Member_children,Aggregate::meta,Member::meta, "", "");
		Uml::SetChildRole(SwitchedAggregate::meta_Label_children,SwitchedAggregate::meta,Label::meta, "", "");
		Uml::SetChildRole(SwitchedAggregate::meta_LabelConnection_children,SwitchedAggregate::meta,LabelConnection::meta, "", "");
		Uml::SetChildRole(SwitchedAggregate::meta_Member_children,SwitchedAggregate::meta,Member::meta, "", "");
		Uml::SetChildRole(SwitchedAggregate::meta_Discriminator_child,SwitchedAggregate::meta,Discriminator::meta, "", "");
		Uml::SetChildRole(Enum::meta_EnumValue_children,Enum::meta,EnumValue::meta, "", "");
		Uml::SetChildRole(PredefinedTypes::meta_PredefinedType_children,PredefinedTypes::meta,PredefinedType::meta, "", "");
		Uml::SetChildRole(Package::meta_Exception_children,Package::meta,Exception::meta, "", "");
		Uml::SetChildRole(Package::meta_Package_children,Package::meta,Package::meta, "", "");
		Uml::SetChildRole(Package::meta_Constant_children,Package::meta,Constant::meta, "", "");
		Uml::SetChildRole(Package::meta_NamedType_children,Package::meta,NamedType::meta, "", "");
		Uml::SetChildRole(Package::meta_ComponentRef_children,Package::meta,ComponentRef::meta, "", "");
		Uml::SetChildRole(Package::meta_ManagesComponent_children,Package::meta,ManagesComponent::meta, "", "");
		Uml::SetChildRole(File::meta_Exception_children,File::meta,Exception::meta, "", "");
		Uml::SetChildRole(File::meta_Package_children,File::meta,Package::meta, "", "");
		Uml::SetChildRole(File::meta_FileRef_children,File::meta,FileRef::meta, "", "");
		Uml::SetChildRole(File::meta_Constant_children,File::meta,Constant::meta, "", "");
		Uml::SetChildRole(File::meta_NamedType_children,File::meta,NamedType::meta, "", "");
		Uml::SetChildRole(File::meta_ComponentRef_children,File::meta,ComponentRef::meta, "", "");
		Uml::SetChildRole(File::meta_ManagesComponent_children,File::meta,ManagesComponent::meta, "", "");
		Uml::SetChildRole(Exception::meta_Member_children,Exception::meta,Member::meta, "", "");
		Uml::SetChildRole(InterfaceDefinitions::meta_File_children,InterfaceDefinitions::meta,File::meta, "", "");
		Uml::SetChildRole(ComponentBuild::meta_MPC_children,ComponentBuild::meta,MPC::meta, "", "");
		Uml::SetChildRole(Project::meta_ExtResourceConn_children,Project::meta,ExtResourceConn::meta, "", "");
		Uml::SetChildRole(Project::meta_ExternalResources_children,Project::meta,ExternalResources::meta, "", "");
		Uml::SetChildRole(Project::meta_ComponentLib_children,Project::meta,ComponentLib::meta, "", "");
		Uml::SetChildRole(Project::meta_ImplementationArtifact_children,Project::meta,ImplementationArtifact::meta, "", "");
		Uml::SetChildRole(MPC::meta_Project_children,MPC::meta,Project::meta, "", "");
		Uml::SetChildRole(MPC::meta_Workspaces_children,MPC::meta,Workspaces::meta, "", "");
		Uml::SetChildRole(StubProject::meta_FileRef_children,StubProject::meta,FileRef::meta, "", "");
		Uml::SetChildRole(StubProject::meta_ImplementationArtifactReference_child,StubProject::meta,ImplementationArtifactReference::meta, "", "");
		Uml::SetChildRole(ServantProject::meta_ImplementationArtifactReference_child,ServantProject::meta,ImplementationArtifactReference::meta, "", "");
		Uml::SetChildRole(ExecutorProject::meta_ImplementationArtifactReference_child,ExecutorProject::meta,ImplementationArtifactReference::meta, "", "");
		Uml::SetChildRole(RootFolder::meta_ComponentAnalyses_children,RootFolder::meta,ComponentAnalyses::meta, "", "");
		Uml::SetChildRole(RootFolder::meta_EventChannelConfig_children,RootFolder::meta,EventChannelConfig::meta, "", "");
		Uml::SetChildRole(RootFolder::meta_DeploymentPlans_children,RootFolder::meta,DeploymentPlans::meta, "", "");
		Uml::SetChildRole(RootFolder::meta_ComponentImplementations_children,RootFolder::meta,ComponentImplementations::meta, "", "");
		Uml::SetChildRole(RootFolder::meta_PackageConfigurations_children,RootFolder::meta,PackageConfigurations::meta, "", "");
		Uml::SetChildRole(RootFolder::meta_ImplementationArtifacts_children,RootFolder::meta,ImplementationArtifacts::meta, "", "");
		Uml::SetChildRole(RootFolder::meta_ComponentTypes_children,RootFolder::meta,ComponentTypes::meta, "", "");
		Uml::SetChildRole(RootFolder::meta_TopLevelPackages_children,RootFolder::meta,TopLevelPackages::meta, "", "");
		Uml::SetChildRole(RootFolder::meta_ComponentPackages_children,RootFolder::meta,ComponentPackages::meta, "", "");
		Uml::SetChildRole(RootFolder::meta_Targets_children,RootFolder::meta,Targets::meta, "", "");
		Uml::SetChildRole(RootFolder::meta_PredefinedTypes_children,RootFolder::meta,PredefinedTypes::meta, "", "");
		Uml::SetChildRole(RootFolder::meta_InterfaceDefinitions_children,RootFolder::meta,InterfaceDefinitions::meta, "", "");
		Uml::SetChildRole(RootFolder::meta_ComponentBuild_children,RootFolder::meta,ComponentBuild::meta, "", "");
		Uml::SetChildRole(RootFolder::meta_RootFolder_children,RootFolder::meta,RootFolder::meta, "", "");
		// Association roles 

		Uml::SetAssocRole(Include_RTEC_Filter_Group_Members_Base::meta_srcInclude_RTEC_Filter_Group,Include_RTEC_Filter_Group_Members_Base::meta,Include_RTEC_Filter_Group_Members_Base::meta, "dstInclude_RTEC_Filter_Group");
		Include_RTEC_Filter_Group_Members_Base::meta_dstInclude_RTEC_Filter_Group_rev = Include_RTEC_Filter_Group_Members_Base::meta_srcInclude_RTEC_Filter_Group;
		Uml::SetAssocRole(Include_RTEC_Filter_Group_Members_Base::meta_dstInclude_RTEC_Filter_Group,Include_RTEC_Filter_Group_Members_Base::meta,Include_RTEC_Filter_Group_Members_Base::meta, "srcInclude_RTEC_Filter_Group");
		Include_RTEC_Filter_Group_Members_Base::meta_srcInclude_RTEC_Filter_Group_rev = Include_RTEC_Filter_Group_Members_Base::meta_dstInclude_RTEC_Filter_Group;




		RTEC_Connection::meta_dstRTEC_Connection_end_ = OutEventPort::meta_dstRTEC_Connection;
		RTEC_Connection::meta_srcRTEC_Connection_end_ = RT_Info_Param::meta_srcRTEC_Connection;

		Uml::SetAssocRole(RTEC_Filter_Group::meta_srcInclude_RT_Info,RTEC_Filter_Group::meta,RT_Info_Param::meta, "dstInclude_RT_Info");
		RT_Info_Param::meta_dstInclude_RT_Info_rev = RTEC_Filter_Group::meta_srcInclude_RT_Info;
		Uml::SetAssocRole(RTEC_Filter_Group::meta_dstUse_RTEC_Filter,RTEC_Filter_Group::meta,InEventPort::meta, "srcUse_RTEC_Filter");
		InEventPort::meta_srcUse_RTEC_Filter_rev = RTEC_Filter_Group::meta_dstUse_RTEC_Filter;


		Include_RTEC_Filter_Group::meta_dstInclude_RTEC_Filter_Group_end_ = Include_RTEC_Filter_Group_Members_Base::meta_dstInclude_RTEC_Filter_Group;
		Include_RTEC_Filter_Group::meta_srcInclude_RTEC_Filter_Group_end_ = Include_RTEC_Filter_Group_Members_Base::meta_srcInclude_RTEC_Filter_Group;



		Use_RTEC_Filter::meta_srcUse_RTEC_Filter_end_ = InEventPort::meta_srcUse_RTEC_Filter;
		Use_RTEC_Filter::meta_dstUse_RTEC_Filter_end_ = RTEC_Filter_Group::meta_dstUse_RTEC_Filter;


		Uml::SetAssocRole(RTEC_Resource_Factory::meta_referedbyRTEC_Resource_Factory_Reference,RTEC_Resource_Factory::meta,RTEC_Resource_Factory_Reference::meta, "ref");

		Include_RT_Info::meta_srcInclude_RT_Info_end_ = RTEC_Filter_Group::meta_srcInclude_RT_Info;
		Include_RT_Info::meta_dstInclude_RT_Info_end_ = RT_Info_Param::meta_dstInclude_RT_Info;

		Use_RT_Info::meta_srcUse_RT_Info_end_ = RTEC_Resource_Factory_Reference::meta_srcUse_RT_Info;
		Use_RT_Info::meta_dstUse_RT_Info_end_ = RT_Info_Param::meta_dstUse_RT_Info;

		Uml::SetAssocRole(RT_Info_Param::meta_dstUse_RT_Info,RT_Info_Param::meta,RTEC_Resource_Factory_Reference::meta, "srcUse_RT_Info");
		RTEC_Resource_Factory_Reference::meta_srcUse_RT_Info_rev = RT_Info_Param::meta_dstUse_RT_Info;
		Uml::SetAssocRole(RT_Info_Param::meta_dstInclude_RT_Info,RT_Info_Param::meta,RTEC_Filter_Group::meta, "srcInclude_RT_Info");
		RTEC_Filter_Group::meta_srcInclude_RT_Info_rev = RT_Info_Param::meta_dstInclude_RT_Info;
		Uml::SetAssocRole(RT_Info_Param::meta_srcRTEC_Connection,RT_Info_Param::meta,OutEventPort::meta, "dstRTEC_Connection");
		OutEventPort::meta_dstRTEC_Connection_rev = RT_Info_Param::meta_srcRTEC_Connection;


		Uml::SetAssocRole(RTEC_Resource_Factory_Reference::meta_srcUse_RT_Info,RTEC_Resource_Factory_Reference::meta,RT_Info_Param::meta, "dstUse_RT_Info");
		RT_Info_Param::meta_dstUse_RT_Info_rev = RTEC_Resource_Factory_Reference::meta_srcUse_RT_Info;
		Uml::SetAssocRole(RTEC_Resource_Factory_Reference::meta_ref,RTEC_Resource_Factory_Reference::meta,RTEC_Resource_Factory::meta, "referedbyRTEC_Resource_Factory_Reference");





		MetricConnection::meta_dstMetricConnection_end_ = OperationRef::meta_dstMetricConnection;
		MetricConnection::meta_srcMetricConnection_end_ = MetricsBase::meta_srcMetricConnection;






		Uml::SetAssocRole(TimeProbe::meta_srcTimerEventSinkConn,TimeProbe::meta,EventRef::meta, "dstTimerEventSinkConn");
		EventRef::meta_dstTimerEventSinkConn_rev = TimeProbe::meta_srcTimerEventSinkConn;
		Uml::SetAssocRole(TimeProbe::meta_srcTimerConnection,TimeProbe::meta,OperationRef::meta, "dstTimerConnection");
		OperationRef::meta_dstTimerConnection_rev = TimeProbe::meta_srcTimerConnection;

		TimerConnection::meta_dstTimerConnection_end_ = OperationRef::meta_dstTimerConnection;
		TimerConnection::meta_srcTimerConnection_end_ = TimeProbe::meta_srcTimerConnection;





		Uml::SetAssocRole(MetricsBase::meta_srcMetricConnection,MetricsBase::meta,OperationRef::meta, "dstMetricConnection");
		OperationRef::meta_dstMetricConnection_rev = MetricsBase::meta_srcMetricConnection;
		Uml::SetAssocRole(MetricsBase::meta_dstWorkloadCharacteristics,MetricsBase::meta,TaskSet::meta, "srcWorkloadCharacteristics");
		TaskSet::meta_srcWorkloadCharacteristics_rev = MetricsBase::meta_dstWorkloadCharacteristics;

		Uml::SetAssocRole(OperationRef::meta_dstMetricConnection,OperationRef::meta,MetricsBase::meta, "srcMetricConnection");
		MetricsBase::meta_srcMetricConnection_rev = OperationRef::meta_dstMetricConnection;
		Uml::SetAssocRole(OperationRef::meta_dstTimerConnection,OperationRef::meta,TimeProbe::meta, "srcTimerConnection");
		TimeProbe::meta_srcTimerConnection_rev = OperationRef::meta_dstTimerConnection;
		Uml::SetAssocRole(OperationRef::meta_ref,OperationRef::meta,OperationBase::meta, "referedbyOperationRef");

		Uml::SetAssocRole(EventRef::meta_dstTimerEventSinkConn,EventRef::meta,TimeProbe::meta, "srcTimerEventSinkConn");
		TimeProbe::meta_srcTimerEventSinkConn_rev = EventRef::meta_dstTimerEventSinkConn;
		Uml::SetAssocRole(EventRef::meta_ref,EventRef::meta,Event::meta, "referedbyEventRef");

		TimerEventSinkConn::meta_dstTimerEventSinkConn_end_ = EventRef::meta_dstTimerEventSinkConn;
		TimerEventSinkConn::meta_srcTimerEventSinkConn_end_ = TimeProbe::meta_srcTimerEventSinkConn;

		Uml::SetAssocRole(TaskSet::meta_srcWorkloadCharacteristics,TaskSet::meta,MetricsBase::meta, "dstWorkloadCharacteristics");
		MetricsBase::meta_dstWorkloadCharacteristics_rev = TaskSet::meta_srcWorkloadCharacteristics;
		Uml::SetAssocRole(TaskSet::meta_members,TaskSet::meta,Task::meta, "setTaskSet");

		Uml::SetAssocRole(Task::meta_setTaskSet,Task::meta,TaskSet::meta, "members");

		WorkloadCharacteristics::meta_srcWorkloadCharacteristics_end_ = TaskSet::meta_srcWorkloadCharacteristics;
		WorkloadCharacteristics::meta_dstWorkloadCharacteristics_end_ = MetricsBase::meta_dstWorkloadCharacteristics;

		Uml::SetAssocRole(NodeReference::meta_srcInstanceMapping,NodeReference::meta,CollocationGroup::meta, "dstInstanceMapping");
		CollocationGroup::meta_dstInstanceMapping_rev = NodeReference::meta_srcInstanceMapping;
		Uml::SetAssocRole(NodeReference::meta_ref,NodeReference::meta,Node::meta, "referedbyNodeReference");

		Uml::SetAssocRole(Interconnect::meta_dstInterconnect2Node,Interconnect::meta,Node::meta, "srcInterconnect2Node");
		Node::meta_srcInterconnect2Node_rev = Interconnect::meta_dstInterconnect2Node;
		Uml::SetAssocRole(Interconnect::meta_dstInterconnect2Bridge,Interconnect::meta,Bridge::meta, "srcInterconnect2Bridge");
		Bridge::meta_srcInterconnect2Bridge_rev = Interconnect::meta_dstInterconnect2Bridge;
		Uml::SetAssocRole(Interconnect::meta_srcNode2Interconnect,Interconnect::meta,Node::meta, "dstNode2Interconnect");
		Node::meta_dstNode2Interconnect_rev = Interconnect::meta_srcNode2Interconnect;
		Uml::SetAssocRole(Interconnect::meta_srcBridge2Interconnect,Interconnect::meta,Bridge::meta, "dstBridge2Interconnect");
		Bridge::meta_dstBridge2Interconnect_rev = Interconnect::meta_srcBridge2Interconnect;

		Uml::SetAssocRole(Node::meta_referedbyNodeReference,Node::meta,NodeReference::meta, "ref");
		Uml::SetAssocRole(Node::meta_srcInterconnect2Node,Node::meta,Interconnect::meta, "dstInterconnect2Node");
		Interconnect::meta_dstInterconnect2Node_rev = Node::meta_srcInterconnect2Node;
		Uml::SetAssocRole(Node::meta_dstNode2Interconnect,Node::meta,Interconnect::meta, "srcNode2Interconnect");
		Interconnect::meta_srcNode2Interconnect_rev = Node::meta_dstNode2Interconnect;
		Uml::SetAssocRole(Node::meta_dstShares,Node::meta,SharedResource::meta, "srcShares");
		SharedResource::meta_srcShares_rev = Node::meta_dstShares;


		Uml::SetAssocRole(Bridge::meta_srcInterconnect2Bridge,Bridge::meta,Interconnect::meta, "dstInterconnect2Bridge");
		Interconnect::meta_dstInterconnect2Bridge_rev = Bridge::meta_srcInterconnect2Bridge;
		Uml::SetAssocRole(Bridge::meta_dstBridge2Interconnect,Bridge::meta,Interconnect::meta, "srcBridge2Interconnect");
		Interconnect::meta_srcBridge2Interconnect_rev = Bridge::meta_dstBridge2Interconnect;

		Uml::SetAssocRole(SharedResource::meta_srcShares,SharedResource::meta,Node::meta, "dstShares");
		Node::meta_dstShares_rev = SharedResource::meta_srcShares;

		InstanceMapping::meta_dstInstanceMapping_end_ = CollocationGroup::meta_dstInstanceMapping;
		InstanceMapping::meta_srcInstanceMapping_end_ = NodeReference::meta_srcInstanceMapping;


		Uml::SetAssocRole(CollocationGroup::meta_dstInstanceMapping,CollocationGroup::meta,NodeReference::meta, "srcInstanceMapping");
		NodeReference::meta_srcInstanceMapping_rev = CollocationGroup::meta_dstInstanceMapping;
		Uml::SetAssocRole(CollocationGroup::meta_members,CollocationGroup::meta,ComponentRef::meta, "setCollocationGroup");


		invoke::meta_dstinvoke_end_ = RequiredRequestPort::meta_dstinvoke;
		invoke::meta_srcinvoke_end_ = ProvidedRequestPort::meta_srcinvoke;

		AssemblyselectRequirement::meta_dstAssemblyselectRequirement_end_ = Component::meta_dstAssemblyselectRequirement;
		AssemblyselectRequirement::meta_srcAssemblyselectRequirement_end_ = Requirement::meta_srcAssemblyselectRequirement;

		AssemblyConfigProperty::meta_dstAssemblyConfigProperty_end_ = Component::meta_dstAssemblyConfigProperty;
		AssemblyConfigProperty::meta_srcAssemblyConfigProperty_end_ = Property::meta_srcAssemblyConfigProperty;

		Uml::SetAssocRole(PublishConnector::meta_dstAssemblyDeployRequirement,PublishConnector::meta,Requirement::meta, "srcAssemblyDeployRequirement");
		Requirement::meta_srcAssemblyDeployRequirement_rev = PublishConnector::meta_dstAssemblyDeployRequirement;
		Uml::SetAssocRole(PublishConnector::meta_dstdeliverTo,PublishConnector::meta,InEventPort::meta, "srcdeliverTo");
		InEventPort::meta_srcdeliverTo_rev = PublishConnector::meta_dstdeliverTo;
		Uml::SetAssocRole(PublishConnector::meta_srcpublish,PublishConnector::meta,OutEventPort::meta, "dstpublish");
		OutEventPort::meta_dstpublish_rev = PublishConnector::meta_srcpublish;

		publish::meta_dstpublish_end_ = OutEventPort::meta_dstpublish;
		publish::meta_srcpublish_end_ = PublishConnector::meta_srcpublish;

		deliverTo::meta_srcdeliverTo_end_ = InEventPort::meta_srcdeliverTo;
		deliverTo::meta_dstdeliverTo_end_ = PublishConnector::meta_dstdeliverTo;

		AssemblyDeployRequirement::meta_srcAssemblyDeployRequirement_end_ = Requirement::meta_srcAssemblyDeployRequirement;
		AssemblyDeployRequirement::meta_dstAssemblyDeployRequirement_end_ = PublishConnector::meta_dstAssemblyDeployRequirement;

		emit::meta_dstemit_end_ = OutEventPort::meta_dstemit;
		emit::meta_srcemit_end_ = InEventPort::meta_srcemit;

		ReceptacleDelegate::meta_dstReceptacleDelegate_end_ = RequiredRequestPort::meta_dstReceptacleDelegate;
		ReceptacleDelegate::meta_srcReceptacleDelegate_end_ = RequiredRequestPort::meta_srcReceptacleDelegate;

		FacetDelegate::meta_dstFacetDelegate_end_ = ProvidedRequestPort::meta_dstFacetDelegate;
		FacetDelegate::meta_srcFacetDelegate_end_ = ProvidedRequestPort::meta_srcFacetDelegate;

		EventSourceDelegate::meta_dstEventSourceDelegate_end_ = OutEventPort::meta_dstEventSourceDelegate;
		EventSourceDelegate::meta_srcEventSourceDelegate_end_ = OutEventPort::meta_srcEventSourceDelegate;

		EventSinkDelegate::meta_dstEventSinkDelegate_end_ = InEventPort::meta_dstEventSinkDelegate;
		EventSinkDelegate::meta_srcEventSinkDelegate_end_ = InEventPort::meta_srcEventSinkDelegate;

		ExternalDelegate::meta_dstExternalDelegate_end_ = Component::meta_dstExternalDelegate;
		ExternalDelegate::meta_srcExternalDelegate_end_ = ExternalReferenceEndPoint::meta_srcExternalDelegate;


		Uml::SetAssocRole(ExternalReferenceEndPoint::meta_srcExternalDelegate,ExternalReferenceEndPoint::meta,Component::meta, "dstExternalDelegate");
		Component::meta_dstExternalDelegate_rev = ExternalReferenceEndPoint::meta_srcExternalDelegate;

		ImplementationCapability::meta_srcImplementationCapability_end_ = Capability::meta_srcImplementationCapability;
		ImplementationCapability::meta_dstImplementationCapability_end_ = ComponentImplementation::meta_dstImplementationCapability;

		ImplementationDependsOn::meta_srcImplementationDependsOn_end_ = ImplementationDependency::meta_srcImplementationDependsOn;
		ImplementationDependsOn::meta_dstImplementationDependsOn_end_ = ComponentImplementation::meta_dstImplementationDependsOn;

		Implements::meta_srcImplements_end_ = ComponentRef::meta_srcImplements;
		Implements::meta_dstImplements_end_ = ComponentImplementation::meta_dstImplements;


		ConfigProperty::meta_srcConfigProperty_end_ = Property::meta_srcConfigProperty;
		ConfigProperty::meta_dstConfigProperty_end_ = ComponentImplementation::meta_dstConfigProperty;

		InfoProperty::meta_srcInfoProperty_end_ = Property::meta_srcInfoProperty;
		InfoProperty::meta_dstInfoProperty_end_ = ComponentImplementation::meta_dstInfoProperty;

		MonolithprimaryArtifact::meta_dstMonolithprimaryArtifact_end_ = MonolithicImplementation::meta_dstMonolithprimaryArtifact;
		MonolithprimaryArtifact::meta_srcMonolithprimaryArtifact_end_ = ImplementationArtifactReference::meta_srcMonolithprimaryArtifact;

		MonolithDeployRequirement::meta_dstMonolithDeployRequirement_end_ = MonolithicImplementation::meta_dstMonolithDeployRequirement;
		MonolithDeployRequirement::meta_srcMonolithDeployRequirement_end_ = ImplementationRequirement::meta_srcMonolithDeployRequirement;


		MonolithExecParameter::meta_dstMonolithExecParameter_end_ = MonolithicImplementation::meta_dstMonolithExecParameter;
		MonolithExecParameter::meta_srcMonolithExecParameter_end_ = Property::meta_srcMonolithExecParameter;

		Uml::SetAssocRole(MonolithicImplementation::meta_dstMonolithprimaryArtifact,MonolithicImplementation::meta,ImplementationArtifactReference::meta, "srcMonolithprimaryArtifact");
		ImplementationArtifactReference::meta_srcMonolithprimaryArtifact_rev = MonolithicImplementation::meta_dstMonolithprimaryArtifact;
		Uml::SetAssocRole(MonolithicImplementation::meta_dstMonolithDeployRequirement,MonolithicImplementation::meta,ImplementationRequirement::meta, "srcMonolithDeployRequirement");
		ImplementationRequirement::meta_srcMonolithDeployRequirement_rev = MonolithicImplementation::meta_dstMonolithDeployRequirement;
		Uml::SetAssocRole(MonolithicImplementation::meta_dstMonolithExecParameter,MonolithicImplementation::meta,Property::meta, "srcMonolithExecParameter");
		Property::meta_srcMonolithExecParameter_rev = MonolithicImplementation::meta_dstMonolithExecParameter;

		Uml::SetAssocRole(ComponentImplementationReference::meta_ref,ComponentImplementationReference::meta,ComponentImplementation::meta, "referedbyComponentImplementationReference");
		Uml::SetAssocRole(ComponentImplementationReference::meta_srcImplementation,ComponentImplementationReference::meta,ComponentPackage::meta, "dstImplementation");
		ComponentPackage::meta_dstImplementation_rev = ComponentImplementationReference::meta_srcImplementation;

		Uml::SetAssocRole(ComponentImplementation::meta_dstImplementationDependsOn,ComponentImplementation::meta,ImplementationDependency::meta, "srcImplementationDependsOn");
		ImplementationDependency::meta_srcImplementationDependsOn_rev = ComponentImplementation::meta_dstImplementationDependsOn;
		Uml::SetAssocRole(ComponentImplementation::meta_dstImplementationCapability,ComponentImplementation::meta,Capability::meta, "srcImplementationCapability");
		Capability::meta_srcImplementationCapability_rev = ComponentImplementation::meta_dstImplementationCapability;
		Uml::SetAssocRole(ComponentImplementation::meta_dstImplements,ComponentImplementation::meta,ComponentRef::meta, "srcImplements");
		ComponentRef::meta_srcImplements_rev = ComponentImplementation::meta_dstImplements;
		Uml::SetAssocRole(ComponentImplementation::meta_dstConfigProperty,ComponentImplementation::meta,Property::meta, "srcConfigProperty");
		Property::meta_srcConfigProperty_rev = ComponentImplementation::meta_dstConfigProperty;
		Uml::SetAssocRole(ComponentImplementation::meta_dstInfoProperty,ComponentImplementation::meta,Property::meta, "srcInfoProperty");
		Property::meta_srcInfoProperty_rev = ComponentImplementation::meta_dstInfoProperty;
		Uml::SetAssocRole(ComponentImplementation::meta_referedbyComponentImplementationReference,ComponentImplementation::meta,ComponentImplementationReference::meta, "ref");

		PackageConfBasePackage::meta_dstPackageConfBasePackage_end_ = PackageConfiguration::meta_dstPackageConfBasePackage;
		PackageConfBasePackage::meta_srcPackageConfBasePackage_end_ = ComponentPackage::meta_srcPackageConfBasePackage;

		PackageConfReference::meta_dstPackageConfReference_end_ = PackageConfiguration::meta_dstPackageConfReference;
		PackageConfReference::meta_srcPackageConfReference_end_ = ComponentPackageReference::meta_srcPackageConfReference;



		PackageConfConfigProperty::meta_dstPackageConfConfigProperty_end_ = PackageConfiguration::meta_dstPackageConfConfigProperty;
		PackageConfConfigProperty::meta_srcPackageConfConfigProperty_end_ = Property::meta_srcPackageConfConfigProperty;

		PackageConfSelectRequirement::meta_dstPackageConfSelectRequirement_end_ = PackageConfiguration::meta_dstPackageConfSelectRequirement;
		PackageConfSelectRequirement::meta_srcPackageConfSelectRequirement_end_ = Requirement::meta_srcPackageConfSelectRequirement;

		PackageConfSpecializedConfig::meta_dstPackageConfSpecializedConfig_end_ = PackageConfiguration::meta_dstPackageConfSpecializedConfig;
		PackageConfSpecializedConfig::meta_srcPackageConfSpecializedConfig_end_ = PackageConfigurationReference::meta_srcPackageConfSpecializedConfig;

		Uml::SetAssocRole(PackageConfigurationReference::meta_srcPackageConfSpecializedConfig,PackageConfigurationReference::meta,PackageConfiguration::meta, "dstPackageConfSpecializedConfig");
		PackageConfiguration::meta_dstPackageConfSpecializedConfig_rev = PackageConfigurationReference::meta_srcPackageConfSpecializedConfig;
		Uml::SetAssocRole(PackageConfigurationReference::meta_ref,PackageConfigurationReference::meta,PackageConfiguration::meta, "referedbyPackageConfigurationReference");
		Uml::SetAssocRole(PackageConfigurationReference::meta_srcpackage,PackageConfigurationReference::meta,TopLevelPackage::meta, "dstpackage");
		TopLevelPackage::meta_dstpackage_rev = PackageConfigurationReference::meta_srcpackage;

		Uml::SetAssocRole(PackageConfiguration::meta_dstPackageConfBasePackage,PackageConfiguration::meta,ComponentPackage::meta, "srcPackageConfBasePackage");
		ComponentPackage::meta_srcPackageConfBasePackage_rev = PackageConfiguration::meta_dstPackageConfBasePackage;
		Uml::SetAssocRole(PackageConfiguration::meta_dstPackageConfReference,PackageConfiguration::meta,ComponentPackageReference::meta, "srcPackageConfReference");
		ComponentPackageReference::meta_srcPackageConfReference_rev = PackageConfiguration::meta_dstPackageConfReference;
		Uml::SetAssocRole(PackageConfiguration::meta_dstPackageConfConfigProperty,PackageConfiguration::meta,Property::meta, "srcPackageConfConfigProperty");
		Property::meta_srcPackageConfConfigProperty_rev = PackageConfiguration::meta_dstPackageConfConfigProperty;
		Uml::SetAssocRole(PackageConfiguration::meta_dstPackageConfSelectRequirement,PackageConfiguration::meta,Requirement::meta, "srcPackageConfSelectRequirement");
		Requirement::meta_srcPackageConfSelectRequirement_rev = PackageConfiguration::meta_dstPackageConfSelectRequirement;
		Uml::SetAssocRole(PackageConfiguration::meta_dstPackageConfSpecializedConfig,PackageConfiguration::meta,PackageConfigurationReference::meta, "srcPackageConfSpecializedConfig");
		PackageConfigurationReference::meta_srcPackageConfSpecializedConfig_rev = PackageConfiguration::meta_dstPackageConfSpecializedConfig;
		Uml::SetAssocRole(PackageConfiguration::meta_referedbyPackageConfigurationReference,PackageConfiguration::meta,PackageConfigurationReference::meta, "ref");


		ArtifactDependsOn::meta_dstArtifactDependsOn_end_ = ImplementationArtifact::meta_dstArtifactDependsOn;
		ArtifactDependsOn::meta_srcArtifactDependsOn_end_ = ImplementationArtifactReference::meta_srcArtifactDependsOn;

		ArtifactDeployRequirement::meta_dstArtifactDeployRequirement_end_ = ImplementationArtifact::meta_dstArtifactDeployRequirement;
		ArtifactDeployRequirement::meta_srcArtifactDeployRequirement_end_ = Requirement::meta_srcArtifactDeployRequirement;


		ArtifactExecParameter::meta_dstArtifactExecParameter_end_ = ImplementationArtifact::meta_dstArtifactExecParameter;
		ArtifactExecParameter::meta_srcArtifactExecParameter_end_ = Property::meta_srcArtifactExecParameter;

		ArtifactInfoProperty::meta_dstArtifactInfoProperty_end_ = ImplementationArtifact::meta_dstArtifactInfoProperty;
		ArtifactInfoProperty::meta_srcArtifactInfoProperty_end_ = Property::meta_srcArtifactInfoProperty;

		Uml::SetAssocRole(ImplementationArtifactReference::meta_srcMonolithprimaryArtifact,ImplementationArtifactReference::meta,MonolithicImplementation::meta, "dstMonolithprimaryArtifact");
		MonolithicImplementation::meta_dstMonolithprimaryArtifact_rev = ImplementationArtifactReference::meta_srcMonolithprimaryArtifact;
		Uml::SetAssocRole(ImplementationArtifactReference::meta_srcArtifactDependsOn,ImplementationArtifactReference::meta,ImplementationArtifact::meta, "dstArtifactDependsOn");
		ImplementationArtifact::meta_dstArtifactDependsOn_rev = ImplementationArtifactReference::meta_srcArtifactDependsOn;
		Uml::SetAssocRole(ImplementationArtifactReference::meta_ref,ImplementationArtifactReference::meta,ImplementationArtifact::meta, "referedbyImplementationArtifactReference");

		Uml::SetAssocRole(ImplementationArtifact::meta_dstArtifactDependsOn,ImplementationArtifact::meta,ImplementationArtifactReference::meta, "srcArtifactDependsOn");
		ImplementationArtifactReference::meta_srcArtifactDependsOn_rev = ImplementationArtifact::meta_dstArtifactDependsOn;
		Uml::SetAssocRole(ImplementationArtifact::meta_dstArtifactDeployRequirement,ImplementationArtifact::meta,Requirement::meta, "srcArtifactDeployRequirement");
		Requirement::meta_srcArtifactDeployRequirement_rev = ImplementationArtifact::meta_dstArtifactDeployRequirement;
		Uml::SetAssocRole(ImplementationArtifact::meta_dstArtifactExecParameter,ImplementationArtifact::meta,Property::meta, "srcArtifactExecParameter");
		Property::meta_srcArtifactExecParameter_rev = ImplementationArtifact::meta_dstArtifactExecParameter;
		Uml::SetAssocRole(ImplementationArtifact::meta_dstArtifactInfoProperty,ImplementationArtifact::meta,Property::meta, "srcArtifactInfoProperty");
		Property::meta_srcArtifactInfoProperty_rev = ImplementationArtifact::meta_dstArtifactInfoProperty;
		Uml::SetAssocRole(ImplementationArtifact::meta_referedbyImplementationArtifactReference,ImplementationArtifact::meta,ImplementationArtifactReference::meta, "ref");
		Uml::SetAssocRole(ImplementationArtifact::meta_referedbyExternalResources,ImplementationArtifact::meta,ExternalResources::meta, "ref");


		Uml::SetAssocRole(ComponentPropertyDescription::meta_srcComponentProperty,ComponentPropertyDescription::meta,ComponentRef::meta, "dstComponentProperty");
		ComponentRef::meta_dstComponentProperty_rev = ComponentPropertyDescription::meta_srcComponentProperty;

		ComponentProperty::meta_dstComponentProperty_end_ = ComponentRef::meta_dstComponentProperty;
		ComponentProperty::meta_srcComponentProperty_end_ = ComponentPropertyDescription::meta_srcComponentProperty;


		ComponentConfigProperty::meta_dstComponentConfigProperty_end_ = ComponentRef::meta_dstComponentConfigProperty;
		ComponentConfigProperty::meta_srcComponentConfigProperty_end_ = Property::meta_srcComponentConfigProperty;

		ComponentInfoProperty::meta_dstComponentInfoProperty_end_ = ComponentRef::meta_dstComponentInfoProperty;
		ComponentInfoProperty::meta_srcComponentInfoProperty_end_ = Property::meta_srcComponentInfoProperty;




		Uml::SetAssocRole(TopLevelPackage::meta_dstpackage,TopLevelPackage::meta,PackageConfigurationReference::meta, "srcpackage");
		PackageConfigurationReference::meta_srcpackage_rev = TopLevelPackage::meta_dstpackage;

		package::meta_dstpackage_end_ = TopLevelPackage::meta_dstpackage;
		package::meta_srcpackage_end_ = PackageConfigurationReference::meta_srcpackage;


		PackageInterface::meta_dstPackageInterface_end_ = ComponentPackage::meta_dstPackageInterface;
		PackageInterface::meta_srcPackageInterface_end_ = ComponentRef::meta_srcPackageInterface;

		Implementation::meta_dstImplementation_end_ = ComponentPackage::meta_dstImplementation;
		Implementation::meta_srcImplementation_end_ = ComponentImplementationReference::meta_srcImplementation;


		PackageConfigProperty::meta_dstPackageConfigProperty_end_ = ComponentPackage::meta_dstPackageConfigProperty;
		PackageConfigProperty::meta_srcPackageConfigProperty_end_ = Property::meta_srcPackageConfigProperty;

		PackageInfoProperty::meta_dstPackageInfoProperty_end_ = ComponentPackage::meta_dstPackageInfoProperty;
		PackageInfoProperty::meta_srcPackageInfoProperty_end_ = Property::meta_srcPackageInfoProperty;

		Uml::SetAssocRole(ComponentPackage::meta_dstImplementation,ComponentPackage::meta,ComponentImplementationReference::meta, "srcImplementation");
		ComponentImplementationReference::meta_srcImplementation_rev = ComponentPackage::meta_dstImplementation;
		Uml::SetAssocRole(ComponentPackage::meta_srcPackageConfBasePackage,ComponentPackage::meta,PackageConfiguration::meta, "dstPackageConfBasePackage");
		PackageConfiguration::meta_dstPackageConfBasePackage_rev = ComponentPackage::meta_srcPackageConfBasePackage;
		Uml::SetAssocRole(ComponentPackage::meta_dstPackageInterface,ComponentPackage::meta,ComponentRef::meta, "srcPackageInterface");
		ComponentRef::meta_srcPackageInterface_rev = ComponentPackage::meta_dstPackageInterface;
		Uml::SetAssocRole(ComponentPackage::meta_dstPackageConfigProperty,ComponentPackage::meta,Property::meta, "srcPackageConfigProperty");
		Property::meta_srcPackageConfigProperty_rev = ComponentPackage::meta_dstPackageConfigProperty;
		Uml::SetAssocRole(ComponentPackage::meta_dstPackageInfoProperty,ComponentPackage::meta,Property::meta, "srcPackageInfoProperty");
		Property::meta_srcPackageInfoProperty_rev = ComponentPackage::meta_dstPackageInfoProperty;
		Uml::SetAssocRole(ComponentPackage::meta_referedbyComponentPackageReference,ComponentPackage::meta,ComponentPackageReference::meta, "ref");

		Uml::SetAssocRole(ComponentPackageReference::meta_srcPackageConfReference,ComponentPackageReference::meta,PackageConfiguration::meta, "dstPackageConfReference");
		PackageConfiguration::meta_dstPackageConfReference_rev = ComponentPackageReference::meta_srcPackageConfReference;
		Uml::SetAssocRole(ComponentPackageReference::meta_ref,ComponentPackageReference::meta,ComponentPackage::meta, "referedbyComponentPackageReference");

		Uml::SetAssocRole(Requirement::meta_srcAssemblyselectRequirement,Requirement::meta,Component::meta, "dstAssemblyselectRequirement");
		Component::meta_dstAssemblyselectRequirement_rev = Requirement::meta_srcAssemblyselectRequirement;
		Uml::SetAssocRole(Requirement::meta_srcAssemblyDeployRequirement,Requirement::meta,PublishConnector::meta, "dstAssemblyDeployRequirement");
		PublishConnector::meta_dstAssemblyDeployRequirement_rev = Requirement::meta_srcAssemblyDeployRequirement;
		Uml::SetAssocRole(Requirement::meta_srcPackageConfSelectRequirement,Requirement::meta,PackageConfiguration::meta, "dstPackageConfSelectRequirement");
		PackageConfiguration::meta_dstPackageConfSelectRequirement_rev = Requirement::meta_srcPackageConfSelectRequirement;
		Uml::SetAssocRole(Requirement::meta_srcArtifactDeployRequirement,Requirement::meta,ImplementationArtifact::meta, "dstArtifactDeployRequirement");
		ImplementationArtifact::meta_dstArtifactDeployRequirement_rev = Requirement::meta_srcArtifactDeployRequirement;

		Uml::SetAssocRole(Capability::meta_srcImplementationCapability,Capability::meta,ComponentImplementation::meta, "dstImplementationCapability");
		ComponentImplementation::meta_dstImplementationCapability_rev = Capability::meta_srcImplementationCapability;

		Uml::SetAssocRole(DataType::meta_ref,DataType::meta,PredefinedType::meta, "referedbyDataType");


		Uml::SetAssocRole(ImplementationDependency::meta_srcImplementationDependsOn,ImplementationDependency::meta,ComponentImplementation::meta, "dstImplementationDependsOn");
		ComponentImplementation::meta_dstImplementationDependsOn_rev = ImplementationDependency::meta_srcImplementationDependsOn;



		Uml::SetAssocRole(ImplementationRequirement::meta_srcMonolithDeployRequirement,ImplementationRequirement::meta,MonolithicImplementation::meta, "dstMonolithDeployRequirement");
		MonolithicImplementation::meta_dstMonolithDeployRequirement_rev = ImplementationRequirement::meta_srcMonolithDeployRequirement;

		Uml::SetAssocRole(Property::meta_srcAssemblyConfigProperty,Property::meta,Component::meta, "dstAssemblyConfigProperty");
		Component::meta_dstAssemblyConfigProperty_rev = Property::meta_srcAssemblyConfigProperty;
		Uml::SetAssocRole(Property::meta_srcConfigProperty,Property::meta,ComponentImplementation::meta, "dstConfigProperty");
		ComponentImplementation::meta_dstConfigProperty_rev = Property::meta_srcConfigProperty;
		Uml::SetAssocRole(Property::meta_srcInfoProperty,Property::meta,ComponentImplementation::meta, "dstInfoProperty");
		ComponentImplementation::meta_dstInfoProperty_rev = Property::meta_srcInfoProperty;
		Uml::SetAssocRole(Property::meta_srcMonolithExecParameter,Property::meta,MonolithicImplementation::meta, "dstMonolithExecParameter");
		MonolithicImplementation::meta_dstMonolithExecParameter_rev = Property::meta_srcMonolithExecParameter;
		Uml::SetAssocRole(Property::meta_srcPackageConfConfigProperty,Property::meta,PackageConfiguration::meta, "dstPackageConfConfigProperty");
		PackageConfiguration::meta_dstPackageConfConfigProperty_rev = Property::meta_srcPackageConfConfigProperty;
		Uml::SetAssocRole(Property::meta_srcArtifactExecParameter,Property::meta,ImplementationArtifact::meta, "dstArtifactExecParameter");
		ImplementationArtifact::meta_dstArtifactExecParameter_rev = Property::meta_srcArtifactExecParameter;
		Uml::SetAssocRole(Property::meta_srcArtifactInfoProperty,Property::meta,ImplementationArtifact::meta, "dstArtifactInfoProperty");
		ImplementationArtifact::meta_dstArtifactInfoProperty_rev = Property::meta_srcArtifactInfoProperty;
		Uml::SetAssocRole(Property::meta_srcComponentConfigProperty,Property::meta,ComponentRef::meta, "dstComponentConfigProperty");
		ComponentRef::meta_dstComponentConfigProperty_rev = Property::meta_srcComponentConfigProperty;
		Uml::SetAssocRole(Property::meta_srcComponentInfoProperty,Property::meta,ComponentRef::meta, "dstComponentInfoProperty");
		ComponentRef::meta_dstComponentInfoProperty_rev = Property::meta_srcComponentInfoProperty;
		Uml::SetAssocRole(Property::meta_srcPackageConfigProperty,Property::meta,ComponentPackage::meta, "dstPackageConfigProperty");
		ComponentPackage::meta_dstPackageConfigProperty_rev = Property::meta_srcPackageConfigProperty;
		Uml::SetAssocRole(Property::meta_srcPackageInfoProperty,Property::meta,ComponentPackage::meta, "dstPackageInfoProperty");
		ComponentPackage::meta_dstPackageInfoProperty_rev = Property::meta_srcPackageInfoProperty;



		Shares::meta_srcShares_end_ = SharedResource::meta_srcShares;
		Shares::meta_dstShares_end_ = Node::meta_dstShares;

		Node2Interconnect::meta_dstNode2Interconnect_end_ = Node::meta_dstNode2Interconnect;
		Node2Interconnect::meta_srcNode2Interconnect_end_ = Interconnect::meta_srcNode2Interconnect;

		Bridge2Interconnect::meta_dstBridge2Interconnect_end_ = Bridge::meta_dstBridge2Interconnect;
		Bridge2Interconnect::meta_srcBridge2Interconnect_end_ = Interconnect::meta_srcBridge2Interconnect;

		Interconnect2Node::meta_srcInterconnect2Node_end_ = Node::meta_srcInterconnect2Node;
		Interconnect2Node::meta_dstInterconnect2Node_end_ = Interconnect::meta_dstInterconnect2Node;

		Interconnect2Bridge::meta_srcInterconnect2Bridge_end_ = Bridge::meta_srcInterconnect2Bridge;
		Interconnect2Bridge::meta_dstInterconnect2Bridge_end_ = Interconnect::meta_dstInterconnect2Bridge;




		Uml::SetAssocRole(OutEventPort::meta_dstRTEC_Connection,OutEventPort::meta,RT_Info_Param::meta, "srcRTEC_Connection");
		RT_Info_Param::meta_srcRTEC_Connection_rev = OutEventPort::meta_dstRTEC_Connection;
		Uml::SetAssocRole(OutEventPort::meta_dstpublish,OutEventPort::meta,PublishConnector::meta, "srcpublish");
		PublishConnector::meta_srcpublish_rev = OutEventPort::meta_dstpublish;
		Uml::SetAssocRole(OutEventPort::meta_dstemit,OutEventPort::meta,InEventPort::meta, "srcemit");
		InEventPort::meta_srcemit_rev = OutEventPort::meta_dstemit;
		Uml::SetAssocRole(OutEventPort::meta_srcEventSourceDelegate,OutEventPort::meta,OutEventPort::meta, "dstEventSourceDelegate");
		OutEventPort::meta_dstEventSourceDelegate_rev = OutEventPort::meta_srcEventSourceDelegate;
		Uml::SetAssocRole(OutEventPort::meta_dstEventSourceDelegate,OutEventPort::meta,OutEventPort::meta, "srcEventSourceDelegate");
		OutEventPort::meta_srcEventSourceDelegate_rev = OutEventPort::meta_dstEventSourceDelegate;
		Uml::SetAssocRole(OutEventPort::meta_ref,OutEventPort::meta,Event::meta, "referedbyOutEventPort");

		Uml::SetAssocRole(ProvidedRequestPort::meta_srcinvoke,ProvidedRequestPort::meta,RequiredRequestPort::meta, "dstinvoke");
		RequiredRequestPort::meta_dstinvoke_rev = ProvidedRequestPort::meta_srcinvoke;
		Uml::SetAssocRole(ProvidedRequestPort::meta_srcFacetDelegate,ProvidedRequestPort::meta,ProvidedRequestPort::meta, "dstFacetDelegate");
		ProvidedRequestPort::meta_dstFacetDelegate_rev = ProvidedRequestPort::meta_srcFacetDelegate;
		Uml::SetAssocRole(ProvidedRequestPort::meta_dstFacetDelegate,ProvidedRequestPort::meta,ProvidedRequestPort::meta, "srcFacetDelegate");
		ProvidedRequestPort::meta_srcFacetDelegate_rev = ProvidedRequestPort::meta_dstFacetDelegate;
		Uml::SetAssocRole(ProvidedRequestPort::meta_ref,ProvidedRequestPort::meta,Provideable::meta, "referedbyProvidedRequestPort");

		Uml::SetAssocRole(RequiredRequestPort::meta_dstinvoke,RequiredRequestPort::meta,ProvidedRequestPort::meta, "srcinvoke");
		ProvidedRequestPort::meta_srcinvoke_rev = RequiredRequestPort::meta_dstinvoke;
		Uml::SetAssocRole(RequiredRequestPort::meta_srcReceptacleDelegate,RequiredRequestPort::meta,RequiredRequestPort::meta, "dstReceptacleDelegate");
		RequiredRequestPort::meta_dstReceptacleDelegate_rev = RequiredRequestPort::meta_srcReceptacleDelegate;
		Uml::SetAssocRole(RequiredRequestPort::meta_dstReceptacleDelegate,RequiredRequestPort::meta,RequiredRequestPort::meta, "srcReceptacleDelegate");
		RequiredRequestPort::meta_srcReceptacleDelegate_rev = RequiredRequestPort::meta_dstReceptacleDelegate;
		Uml::SetAssocRole(RequiredRequestPort::meta_ref,RequiredRequestPort::meta,Provideable::meta, "referedbyRequiredRequestPort");

		Uml::SetAssocRole(InEventPort::meta_srcUse_RTEC_Filter,InEventPort::meta,RTEC_Filter_Group::meta, "dstUse_RTEC_Filter");
		RTEC_Filter_Group::meta_dstUse_RTEC_Filter_rev = InEventPort::meta_srcUse_RTEC_Filter;
		Uml::SetAssocRole(InEventPort::meta_srcdeliverTo,InEventPort::meta,PublishConnector::meta, "dstdeliverTo");
		PublishConnector::meta_dstdeliverTo_rev = InEventPort::meta_srcdeliverTo;
		Uml::SetAssocRole(InEventPort::meta_srcemit,InEventPort::meta,OutEventPort::meta, "dstemit");
		OutEventPort::meta_dstemit_rev = InEventPort::meta_srcemit;
		Uml::SetAssocRole(InEventPort::meta_srcEventSinkDelegate,InEventPort::meta,InEventPort::meta, "dstEventSinkDelegate");
		InEventPort::meta_dstEventSinkDelegate_rev = InEventPort::meta_srcEventSinkDelegate;
		Uml::SetAssocRole(InEventPort::meta_dstEventSinkDelegate,InEventPort::meta,InEventPort::meta, "srcEventSinkDelegate");
		InEventPort::meta_srcEventSinkDelegate_rev = InEventPort::meta_dstEventSinkDelegate;
		Uml::SetAssocRole(InEventPort::meta_ref,InEventPort::meta,Event::meta, "referedbyInEventPort");


		Uml::SetAssocRole(Manageable::meta_srcManagesComponent,Manageable::meta,ComponentFactory::meta, "dstManagesComponent");
		ComponentFactory::meta_dstManagesComponent_rev = Manageable::meta_srcManagesComponent;

		Uml::SetAssocRole(Provideable::meta_referedbyProvidedRequestPort,Provideable::meta,ProvidedRequestPort::meta, "ref");
		Uml::SetAssocRole(Provideable::meta_referedbyRequiredRequestPort,Provideable::meta,RequiredRequestPort::meta, "ref");

		Uml::SetAssocRole(ComponentFactory::meta_dstManagesComponent,ComponentFactory::meta,Manageable::meta, "srcManagesComponent");
		Manageable::meta_srcManagesComponent_rev = ComponentFactory::meta_dstManagesComponent;

		Uml::SetAssocRole(ComponentRef::meta_setCollocationGroup,ComponentRef::meta,CollocationGroup::meta, "members");
		Uml::SetAssocRole(ComponentRef::meta_srcImplements,ComponentRef::meta,ComponentImplementation::meta, "dstImplements");
		ComponentImplementation::meta_dstImplements_rev = ComponentRef::meta_srcImplements;
		Uml::SetAssocRole(ComponentRef::meta_dstComponentProperty,ComponentRef::meta,ComponentPropertyDescription::meta, "srcComponentProperty");
		ComponentPropertyDescription::meta_srcComponentProperty_rev = ComponentRef::meta_dstComponentProperty;
		Uml::SetAssocRole(ComponentRef::meta_dstComponentConfigProperty,ComponentRef::meta,Property::meta, "srcComponentConfigProperty");
		Property::meta_srcComponentConfigProperty_rev = ComponentRef::meta_dstComponentConfigProperty;
		Uml::SetAssocRole(ComponentRef::meta_dstComponentInfoProperty,ComponentRef::meta,Property::meta, "srcComponentInfoProperty");
		Property::meta_srcComponentInfoProperty_rev = ComponentRef::meta_dstComponentInfoProperty;
		Uml::SetAssocRole(ComponentRef::meta_srcPackageInterface,ComponentRef::meta,ComponentPackage::meta, "dstPackageInterface");
		ComponentPackage::meta_dstPackageInterface_rev = ComponentRef::meta_srcPackageInterface;
		Uml::SetAssocRole(ComponentRef::meta_ref,ComponentRef::meta,Component::meta, "referedbyComponentRef");

		ManagesComponent::meta_dstManagesComponent_end_ = ComponentFactory::meta_dstManagesComponent;
		ManagesComponent::meta_srcManagesComponent_end_ = Manageable::meta_srcManagesComponent;

		Uml::SetAssocRole(Component::meta_dstAssemblyselectRequirement,Component::meta,Requirement::meta, "srcAssemblyselectRequirement");
		Requirement::meta_srcAssemblyselectRequirement_rev = Component::meta_dstAssemblyselectRequirement;
		Uml::SetAssocRole(Component::meta_dstAssemblyConfigProperty,Component::meta,Property::meta, "srcAssemblyConfigProperty");
		Property::meta_srcAssemblyConfigProperty_rev = Component::meta_dstAssemblyConfigProperty;
		Uml::SetAssocRole(Component::meta_dstExternalDelegate,Component::meta,ExternalReferenceEndPoint::meta, "srcExternalDelegate");
		ExternalReferenceEndPoint::meta_srcExternalDelegate_rev = Component::meta_dstExternalDelegate;
		Uml::SetAssocRole(Component::meta_referedbyComponentRef,Component::meta,ComponentRef::meta, "ref");




		Uml::SetAssocRole(OperationBase::meta_referedbyOperationRef,OperationBase::meta,OperationRef::meta, "ref");


		Uml::SetAssocRole(InParameter::meta_ref,InParameter::meta,MemberType::meta, "referedbyInParameter");

		Uml::SetAssocRole(InoutParameter::meta_ref,InoutParameter::meta,MemberType::meta, "referedbyInoutParameter");

		Uml::SetAssocRole(OutParameter::meta_ref,OutParameter::meta,MemberType::meta, "referedbyOutParameter");

		Uml::SetAssocRole(ReturnType::meta_ref,ReturnType::meta,MemberType::meta, "referedbyReturnType");


		Uml::SetAssocRole(Object::meta_referedbySupports,Object::meta,Supports::meta, "ref");

		Uml::SetAssocRole(Inherits::meta_ref,Inherits::meta,Inheritable::meta, "referedbyInherits");

		Uml::SetAssocRole(Event::meta_referedbyEventRef,Event::meta,EventRef::meta, "ref");
		Uml::SetAssocRole(Event::meta_referedbyOutEventPort,Event::meta,OutEventPort::meta, "ref");
		Uml::SetAssocRole(Event::meta_referedbyInEventPort,Event::meta,InEventPort::meta, "ref");

		Uml::SetAssocRole(ValueObject::meta_referedbyLookupKey,ValueObject::meta,LookupKey::meta, "ref");



		Uml::SetAssocRole(Supports::meta_ref,Supports::meta,Object::meta, "referedbySupports");



		Uml::SetAssocRole(LookupKey::meta_ref,LookupKey::meta,ValueObject::meta, "referedbyLookupKey");

		Uml::SetAssocRole(SetException::meta_ref,SetException::meta,Exception::meta, "referedbySetException");


		Uml::SetAssocRole(GetException::meta_ref,GetException::meta,Exception::meta, "referedbyGetException");

		Uml::SetAssocRole(PrivateFlag::meta_srcMakeMemberPrivate,PrivateFlag::meta,Member::meta, "dstMakeMemberPrivate");
		Member::meta_dstMakeMemberPrivate_rev = PrivateFlag::meta_srcMakeMemberPrivate;

		MakeMemberPrivate::meta_dstMakeMemberPrivate_end_ = Member::meta_dstMakeMemberPrivate;
		MakeMemberPrivate::meta_srcMakeMemberPrivate_end_ = PrivateFlag::meta_srcMakeMemberPrivate;

		Uml::SetAssocRole(AttributeMember::meta_ref,AttributeMember::meta,MemberType::meta, "referedbyAttributeMember");

		Uml::SetAssocRole(Inheritable::meta_referedbyInherits,Inheritable::meta,Inherits::meta, "ref");




		Uml::SetAssocRole(Label::meta_srcLabelConnection,Label::meta,Member::meta, "dstLabelConnection");
		Member::meta_dstLabelConnection_rev = Label::meta_srcLabelConnection;

		LabelConnection::meta_dstLabelConnection_end_ = Member::meta_dstLabelConnection;
		LabelConnection::meta_srcLabelConnection_end_ = Label::meta_srcLabelConnection;

		Uml::SetAssocRole(MemberType::meta_referedbyInoutParameter,MemberType::meta,InoutParameter::meta, "ref");
		Uml::SetAssocRole(MemberType::meta_referedbyInParameter,MemberType::meta,InParameter::meta, "ref");
		Uml::SetAssocRole(MemberType::meta_referedbyOutParameter,MemberType::meta,OutParameter::meta, "ref");
		Uml::SetAssocRole(MemberType::meta_referedbyReturnType,MemberType::meta,ReturnType::meta, "ref");
		Uml::SetAssocRole(MemberType::meta_referedbyAttributeMember,MemberType::meta,AttributeMember::meta, "ref");
		Uml::SetAssocRole(MemberType::meta_referedbyCollection,MemberType::meta,Collection::meta, "ref");
		Uml::SetAssocRole(MemberType::meta_referedbyAlias,MemberType::meta,Alias::meta, "ref");
		Uml::SetAssocRole(MemberType::meta_referedbyBoxed,MemberType::meta,Boxed::meta, "ref");
		Uml::SetAssocRole(MemberType::meta_referedbyMember,MemberType::meta,Member::meta, "ref");

		Uml::SetAssocRole(Discriminator::meta_ref,Discriminator::meta,ConstantType::meta, "referedbyDiscriminator");

		Uml::SetAssocRole(Collection::meta_ref,Collection::meta,MemberType::meta, "referedbyCollection");

		Uml::SetAssocRole(Alias::meta_ref,Alias::meta,MemberType::meta, "referedbyAlias");

		Uml::SetAssocRole(Boxed::meta_ref,Boxed::meta,MemberType::meta, "referedbyBoxed");



		Uml::SetAssocRole(Member::meta_ref,Member::meta,MemberType::meta, "referedbyMember");
		Uml::SetAssocRole(Member::meta_dstMakeMemberPrivate,Member::meta,PrivateFlag::meta, "srcMakeMemberPrivate");
		PrivateFlag::meta_srcMakeMemberPrivate_rev = Member::meta_dstMakeMemberPrivate;
		Uml::SetAssocRole(Member::meta_dstLabelConnection,Member::meta,Label::meta, "srcLabelConnection");
		Label::meta_srcLabelConnection_rev = Member::meta_dstLabelConnection;


		Uml::SetAssocRole(PredefinedType::meta_referedbyDataType,PredefinedType::meta,DataType::meta, "ref");














		Uml::SetAssocRole(File::meta_referedbyFileRef,File::meta,FileRef::meta, "ref");

		Uml::SetAssocRole(Exception::meta_referedbySetException,Exception::meta,SetException::meta, "ref");
		Uml::SetAssocRole(Exception::meta_referedbyGetException,Exception::meta,GetException::meta, "ref");
		Uml::SetAssocRole(Exception::meta_referedbyExceptionRef,Exception::meta,ExceptionRef::meta, "ref");

		Uml::SetAssocRole(ExceptionRef::meta_ref,ExceptionRef::meta,Exception::meta, "referedbyExceptionRef");


		Uml::SetAssocRole(ConstantType::meta_referedbyDiscriminator,ConstantType::meta,Discriminator::meta, "ref");
		Uml::SetAssocRole(ConstantType::meta_referedbyConstant,ConstantType::meta,Constant::meta, "ref");

		Uml::SetAssocRole(Constant::meta_ref,Constant::meta,ConstantType::meta, "referedbyConstant");

		Uml::SetAssocRole(FileRef::meta_ref,FileRef::meta,File::meta, "referedbyFileRef");


		Uml::SetAssocRole(Project::meta_setWorkspace,Project::meta,Workspaces::meta, "members");

		Uml::SetAssocRole(ExternalResources::meta_ref,ExternalResources::meta,ImplementationArtifact::meta, "referedbyExternalResources");
		Uml::SetAssocRole(ExternalResources::meta_srcExtResourceConn,ExternalResources::meta,ComponentLib::meta, "dstExtResourceConn");
		ComponentLib::meta_dstExtResourceConn_rev = ExternalResources::meta_srcExtResourceConn;

		ExtResourceConn::meta_dstExtResourceConn_end_ = ComponentLib::meta_dstExtResourceConn;
		ExtResourceConn::meta_srcExtResourceConn_end_ = ExternalResources::meta_srcExtResourceConn;

		Uml::SetAssocRole(ComponentLib::meta_dstExtResourceConn,ComponentLib::meta,ExternalResources::meta, "srcExtResourceConn");
		ExternalResources::meta_srcExtResourceConn_rev = ComponentLib::meta_dstExtResourceConn;





		Uml::SetAssocRole(Workspaces::meta_members,Workspaces::meta,Project::meta, "setWorkspace");



		
	}
	 Udm::UdmDiagram diagram = { &umldiagram, Initialize };
	static struct _regClass
	{
		_regClass()
		{
			Udm::MetaDepository::StoreDiagram("PICML", diagram);
		};
		~_regClass()
		{
			Udm::MetaDepository::RemoveDiagram("PICML");
		};
	} __regUnUsed;

}
// END PICML.cpp
