#ifndef PICMLBONX4_H
#define PICMLBONX4_H

#include "BON.h"
#include "BONImpl.h"
#include "Extensions.h"

///BUP
// add your include files/class definitions here
///EUP

namespace PICML
{

DECLARE_BONEXTENSION( BON::Folder, ComponentAnalysesImpl, ComponentAnalyses );
DECLARE_BONEXTENSION( BON::Folder, ComponentBuildImpl, ComponentBuild );
DECLARE_BONEXTENSION( BON::Folder, ComponentImplementationsImpl, ComponentImplementations );
DECLARE_BONEXTENSION( BON::Folder, ComponentPackagesImpl, ComponentPackages );
DECLARE_BONEXTENSION( BON::Folder, ComponentTypesImpl, ComponentTypes );
DECLARE_BONEXTENSION( BON::Folder, DeploymentPlansImpl, DeploymentPlans );
DECLARE_BONEXTENSION( BON::Folder, ImplementationArtifactsImpl, ImplementationArtifacts );
DECLARE_BONEXTENSION( BON::Folder, InterfaceDefinitionsImpl, InterfaceDefinitions );
DECLARE_BONEXTENSION( BON::Folder, PackageConfigurationsImpl, PackageConfigurations );
DECLARE_BONEXTENSION( BON::Folder, PathDiagramsImpl, PathDiagrams );
DECLARE_BONEXTENSION( BON::Folder, PredefinedTypesImpl, PredefinedTypes );
DECLARE_BONEXTENSION( BON::Folder, TargetsImpl, Targets );
DECLARE_BONEXTENSION( BON::Folder, TopLevelPackagesImpl, TopLevelPackages );
DECLARE_BONEXTENSION( BON::Folder, WorkerLibrariesImpl, WorkerLibraries );
DECLARE_ABSTRACT_BONEXTENSION( BON::Model, PrefixableImpl, Prefixable );
DECLARE_ABSTRACT_BONEXTENSION( BON::Model, SupportsInterfacesImpl, SupportsInterfaces );
DECLARE_ABSTRACT_BONEXTENSION( BON::FCO, CommonPortAttrsImpl, CommonPortAttrs );
DECLARE_ABSTRACT_BONEXTENSION( BON::FCO, ConstantTypeImpl, ConstantType );
DECLARE_ABSTRACT_BONEXTENSION( BON::FCO, GraphVertexImpl, GraphVertex );
DECLARE_ABSTRACT_BONEXTENSION( BON::FCO, ManageableImpl, Manageable );
DECLARE_ABSTRACT_BONEXTENSION( BON::FCO, MemberTypeImpl, MemberType );
DECLARE_ABSTRACT_BONEXTENSION( BON::FCO, MultiInputBaseImpl, MultiInputBase );
DECLARE_ABSTRACT_BONEXTENSION( BON::FCO, ProvideableImpl, Provideable );
DECLARE_ABSTRACT_BONEXTENSION( BON::FCO, TaggableImpl, Taggable );
DECLARE_BONEXTENSION2( BON::Model, GraphVertex, ConnectedComponentImpl, ConnectedComponent );
DECLARE_BONEXTENSION2( BON::Model, Taggable, ExceptionImpl, Exception );
DECLARE_BONEXTENSION( Prefixable, FileImpl, File );
DECLARE_ABSTRACT_BONEXTENSION2( BON::Model, Taggable, OperationBaseImpl, OperationBase );
DECLARE_BONEXTENSION2( Prefixable, Taggable, PackageImpl, Package );
DECLARE_BONEXTENSION2( BON::Model, Taggable, ReadonlyAttributeImpl, ReadonlyAttribute );
DECLARE_ABSTRACT_BONEXTENSION2( Taggable, MemberType, NamedTypeImpl, NamedType );
DECLARE_ABSTRACT_BONEXTENSION3( Taggable, GraphVertex, CommonPortAttrs, PortImpl, Port );
DECLARE_ABSTRACT_BONEXTENSION2( MemberType, ConstantType, PredefinedTypeImpl, PredefinedType );
DECLARE_BONEXTENSION2( BON::Atom, GraphVertex, DisplayNodeImpl, DisplayNode );
DECLARE_BONEXTENSION2( BON::Atom, MultiInputBase, EnvironmentImpl, Environment );
DECLARE_BONEXTENSION2( BON::Reference, Manageable, ComponentRefImpl, ComponentRef );
DECLARE_BONEXTENSION2( BON::Reference, Taggable, ConstantImpl, Constant );
DECLARE_BONEXTENSION( ReadonlyAttribute, AttributeImpl, Attribute );
DECLARE_BONEXTENSION3( SupportsInterfaces, Manageable, NamedType, ComponentImpl, Component );
DECLARE_ABSTRACT_BONEXTENSION( OperationBase, HasExceptionsImpl, HasExceptions );
DECLARE_ABSTRACT_BONEXTENSION2( BON::Model, NamedType, InheritableImpl, Inheritable );
DECLARE_BONEXTENSION( OperationBase, OnewayOperationImpl, OnewayOperation );
DECLARE_ABSTRACT_BONEXTENSION( NamedType, NoInheritableImpl, NoInheritable );
DECLARE_BONEXTENSION2( BON::Atom, PredefinedType, BooleanImpl, Boolean );
DECLARE_BONEXTENSION2( BON::Atom, PredefinedType, ByteImpl, Byte );
DECLARE_BONEXTENSION3( BON::Atom, Provideable, PredefinedType, GenericObjectImpl, GenericObject );
DECLARE_BONEXTENSION2( BON::Atom, PredefinedType, GenericValueImpl, GenericValue );
DECLARE_BONEXTENSION2( BON::Atom, PredefinedType, GenericValueObjectImpl, GenericValueObject );
DECLARE_BONEXTENSION2( BON::Atom, PredefinedType, LongIntegerImpl, LongInteger );
DECLARE_BONEXTENSION2( BON::Atom, PredefinedType, RealNumberImpl, RealNumber );
DECLARE_BONEXTENSION2( BON::Atom, PredefinedType, ShortIntegerImpl, ShortInteger );
DECLARE_BONEXTENSION2( BON::Atom, PredefinedType, StringImpl, String );
DECLARE_BONEXTENSION2( BON::Atom, PredefinedType, TypeEncodingImpl, TypeEncoding );
DECLARE_BONEXTENSION2( BON::Atom, PredefinedType, TypeKindImpl, TypeKind );
DECLARE_BONEXTENSION2( BON::Reference, NamedType, BoxedImpl, Boxed );
DECLARE_BONEXTENSION2( BON::Reference, Port, InEventPortImpl, InEventPort );
DECLARE_BONEXTENSION2( BON::Reference, Port, OutEventPortImpl, OutEventPort );
DECLARE_BONEXTENSION3( BON::Reference, MultiInputBase, Port, ProvidedRequestPortImpl, ProvidedRequestPort );
DECLARE_BONEXTENSION2( BON::Reference, Port, RequiredRequestPortImpl, RequiredRequestPort );
DECLARE_BONEXTENSION2( BON::Model, NoInheritable, AggregateImpl, Aggregate );
DECLARE_BONEXTENSION3( BON::Model, NoInheritable, ConstantType, EnumImpl, Enum );
DECLARE_BONEXTENSION( HasExceptions, FactoryOperationImpl, FactoryOperation );
DECLARE_ABSTRACT_BONEXTENSION( Inheritable, HasOperationsImpl, HasOperations );
DECLARE_BONEXTENSION( HasExceptions, LookupOperationImpl, LookupOperation );
DECLARE_BONEXTENSION2( BON::Model, NoInheritable, SwitchedAggregateImpl, SwitchedAggregate );
DECLARE_BONEXTENSION( HasExceptions, TwowayOperationImpl, TwowayOperation );
DECLARE_BONEXTENSION2( BON::Reference, NoInheritable, AliasImpl, Alias );
DECLARE_BONEXTENSION2( BON::Reference, NoInheritable, CollectionImpl, Collection );
DECLARE_BONEXTENSION2( SupportsInterfaces, HasOperations, ComponentFactoryImpl, ComponentFactory );
DECLARE_BONEXTENSION3( Prefixable, HasOperations, Provideable, ObjectImpl, Object );
DECLARE_ABSTRACT_BONEXTENSION3( Prefixable, SupportsInterfaces, HasOperations, ObjectByValueImpl, ObjectByValue );
DECLARE_BONEXTENSION( ObjectByValue, EventImpl, Event );
DECLARE_BONEXTENSION( ObjectByValue, ValueObjectImpl, ValueObject );
DECLARE_ABSTRACT_BONEXTENSION( BON::Model, RequirementSatisfierImpl, RequirementSatisfier );
DECLARE_ABSTRACT_BONEXTENSION( BON::FCO, ElementsImpl, Elements );
DECLARE_BONEXTENSION2( BON::Model, Elements, BridgeImpl, Bridge );
DECLARE_BONEXTENSION( RequirementSatisfier, CapabilityImpl, Capability );
DECLARE_BONEXTENSION2( BON::Model, Elements, InterconnectImpl, Interconnect );
DECLARE_BONEXTENSION2( BON::Model, Elements, NodeImpl, Node );
DECLARE_BONEXTENSION( RequirementSatisfier, ResourceImpl, Resource );
DECLARE_BONEXTENSION2( Elements, RequirementSatisfier, SharedResourceImpl, SharedResource );
DECLARE_ABSTRACT_BONEXTENSION( BON::Model, ActionBaseImpl, ActionBase );
DECLARE_BONEXTENSION( ActionBase, ActionImpl, Action );
DECLARE_BONEXTENSION( Action, CompositeActionImpl, CompositeAction );
DECLARE_BONEXTENSION( Action, OutputActionImpl, OutputAction );
DECLARE_ABSTRACT_BONEXTENSION( BON::FCO, ComponentLibImpl, ComponentLib );
DECLARE_BONEXTENSION2( BON::Model, ComponentLib, ExecutorProjectImpl, ExecutorProject );
DECLARE_BONEXTENSION2( BON::Model, ComponentLib, ServantProjectImpl, ServantProject );
DECLARE_BONEXTENSION2( BON::Model, ComponentLib, StubProjectImpl, StubProject );
DECLARE_ABSTRACT_BONEXTENSION( BON::Model, RequirementBaseImpl, RequirementBase );
DECLARE_BONEXTENSION( RequirementBase, ImplementationRequirementImpl, ImplementationRequirement );
DECLARE_BONEXTENSION( RequirementBase, RequirementImpl, Requirement );
DECLARE_ABSTRACT_BONEXTENSION( ActionBase, InputActionBaseImpl, InputActionBase );
DECLARE_BONEXTENSION( InputActionBase, InputActionImpl, InputAction );
DECLARE_BONEXTENSION( InputActionBase, PeriodicActionImpl, PeriodicAction );
DECLARE_ABSTRACT_BONEXTENSION( BON::Model, MetricsBaseImpl, MetricsBase );
DECLARE_BONEXTENSION( MetricsBase, LatencyImpl, Latency );
DECLARE_BONEXTENSION( MetricsBase, ThroughputImpl, Throughput );
DECLARE_BONEXTENSION( BON::Model, WorkerPackageImpl, WorkerPackage );
DECLARE_BONEXTENSION( WorkerPackage, WorkerFileImpl, WorkerFile );
DECLARE_ABSTRACT_BONEXTENSION( BON::FCO, ComponentImplementationImpl, ComponentImplementation );
DECLARE_BONEXTENSION2( BON::Model, ComponentImplementation, ComponentAssemblyImpl, ComponentAssembly );
DECLARE_BONEXTENSION2( BON::Atom, ComponentImplementation, MonolithicImplementationImpl, MonolithicImplementation );
DECLARE_BONEXTENSION( BON::Model, ArtifactContainerImpl, ArtifactContainer );
DECLARE_BONEXTENSION( BON::Model, BenchmarkAnalysisImpl, BenchmarkAnalysis );
DECLARE_BONEXTENSION( BON::Model, ComponentContainerImpl, ComponentContainer );
DECLARE_BONEXTENSION( BON::Model, ComponentImplementationContainerImpl, ComponentImplementationContainer );
DECLARE_BONEXTENSION( BON::Model, ComponentPropertyDescriptionImpl, ComponentPropertyDescription );
DECLARE_BONEXTENSION( BON::Model, DeploymentPlanImpl, DeploymentPlan );
DECLARE_BONEXTENSION( BON::Model, DomainImpl, Domain );
DECLARE_BONEXTENSION( BON::Model, MPCImpl, MPC );
DECLARE_BONEXTENSION( BON::Model, PackageConfigurationContainerImpl, PackageConfigurationContainer );
DECLARE_BONEXTENSION( BON::Model, PackageContainerImpl, PackageContainer );
DECLARE_BONEXTENSION( BON::Model, PathImpl, Path );
DECLARE_BONEXTENSION( BON::Model, PathsImpl, Paths );
DECLARE_BONEXTENSION( BON::Model, ProjectImpl, Project );
DECLARE_BONEXTENSION( BON::Model, PropertyImpl, Property );
DECLARE_BONEXTENSION( BON::Model, SatisfierPropertyImpl, SatisfierProperty );
DECLARE_BONEXTENSION( BON::Model, TopLevelPackageContainerImpl, TopLevelPackageContainer );
DECLARE_BONEXTENSION( BON::Model, WorkerImpl, Worker );
DECLARE_BONEXTENSION( BON::Model, WorkerLibraryImpl, WorkerLibrary );
DECLARE_ABSTRACT_BONEXTENSION( BON::FCO, BenchmarkTypeImpl, BenchmarkType );
DECLARE_BONEXTENSION2( BON::Atom, BenchmarkType, FixedIterationBenchmarksImpl, FixedIterationBenchmarks );
DECLARE_BONEXTENSION2( BON::Atom, BenchmarkType, PeriodicBenchmarksImpl, PeriodicBenchmarks );
DECLARE_BONEXTENSION2( BON::Atom, BenchmarkType, TriggerBenchmarksImpl, TriggerBenchmarks );
DECLARE_ABSTRACT_BONEXTENSION( BON::FCO, DataAnalysisBaseImpl, DataAnalysisBase );
DECLARE_BONEXTENSION2( BON::Atom, DataAnalysisBase, AverageImpl, Average );
DECLARE_BONEXTENSION2( BON::Atom, DataAnalysisBase, JitterImpl, Jitter );
DECLARE_BONEXTENSION2( BON::Atom, DataAnalysisBase, MaximumImpl, Maximum );
DECLARE_BONEXTENSION2( BON::Atom, DataAnalysisBase, MinimumImpl, Minimum );
DECLARE_ABSTRACT_BONEXTENSION( BON::FCO, ReqQoSBaseImpl, ReqQoSBase );
DECLARE_ABSTRACT_BONEXTENSION( ReqQoSBase, QoSCharacteristicImpl, QoSCharacteristic );
DECLARE_BONEXTENSION2( BON::Reference, ReqQoSBase, QoSCharRefImpl, QoSCharRef );
DECLARE_BONEXTENSION( BON::Atom, ComponentPackageImpl, ComponentPackage );
DECLARE_BONEXTENSION( BON::Atom, EdgeImpl, Edge );
DECLARE_BONEXTENSION( BON::Atom, EnumValueImpl, EnumValue );
DECLARE_BONEXTENSION( BON::Atom, ExternalReferenceEndPointImpl, ExternalReferenceEndPoint );
DECLARE_BONEXTENSION( BON::Atom, ImplementationArtifactImpl, ImplementationArtifact );
DECLARE_BONEXTENSION( BON::Atom, ImplementationDependencyImpl, ImplementationDependency );
DECLARE_BONEXTENSION( BON::Atom, LabelImpl, Label );
DECLARE_BONEXTENSION( BON::Atom, PackageConfigurationImpl, PackageConfiguration );
DECLARE_BONEXTENSION( BON::Atom, PrivateFlagImpl, PrivateFlag );
DECLARE_BONEXTENSION( BON::Atom, PublishConnectorImpl, PublishConnector );
DECLARE_BONEXTENSION( BON::Atom, QoSConnectorImpl, QoSConnector );
DECLARE_BONEXTENSION( BON::Atom, StateImpl, State );
DECLARE_BONEXTENSION( BON::Atom, TaskImpl, Task );
DECLARE_BONEXTENSION( BON::Atom, TimeProbeImpl, TimeProbe );
DECLARE_BONEXTENSION( BON::Atom, TopLevelPackageImpl, TopLevelPackage );
DECLARE_BONEXTENSION( BON::Connection, ArtifactDependencyImpl, ArtifactDependency );
DECLARE_BONEXTENSION( BON::Connection, ArtifactDependsOnImpl, ArtifactDependsOn );
DECLARE_BONEXTENSION( BON::Connection, ArtifactDeployRequirementImpl, ArtifactDeployRequirement );
DECLARE_BONEXTENSION( BON::Connection, ArtifactExecParameterImpl, ArtifactExecParameter );
DECLARE_BONEXTENSION( BON::Connection, ArtifactInfoPropertyImpl, ArtifactInfoProperty );
DECLARE_BONEXTENSION( BON::Connection, AssemblyConfigPropertyImpl, AssemblyConfigProperty );
DECLARE_BONEXTENSION( BON::Connection, AssemblyDeployRequirementImpl, AssemblyDeployRequirement );
DECLARE_BONEXTENSION( BON::Connection, AssemblyselectRequirementImpl, AssemblyselectRequirement );
DECLARE_BONEXTENSION( BON::Connection, AttributeDelegateImpl, AttributeDelegate );
DECLARE_BONEXTENSION( BON::Connection, AttributeMappingDelegateImpl, AttributeMappingDelegate );
DECLARE_BONEXTENSION( BON::Connection, AttributeMappingValueImpl, AttributeMappingValue );
DECLARE_BONEXTENSION( BON::Connection, AttributeValueImpl, AttributeValue );
DECLARE_BONEXTENSION( BON::Connection, BenchmarkCharacteristicsImpl, BenchmarkCharacteristics );
DECLARE_BONEXTENSION( BON::Connection, Bridge2InterconnectImpl, Bridge2Interconnect );
DECLARE_BONEXTENSION( BON::Connection, ComponentConfigPropertyImpl, ComponentConfigProperty );
DECLARE_BONEXTENSION( BON::Connection, ComponentInfoPropertyImpl, ComponentInfoProperty );
DECLARE_BONEXTENSION( BON::Connection, ComponentOperationImpl, ComponentOperation );
DECLARE_BONEXTENSION( BON::Connection, ComponentPropertyImpl, ComponentProperty );
DECLARE_BONEXTENSION( BON::Connection, ConfigPropertyImpl, ConfigProperty );
DECLARE_BONEXTENSION( BON::Connection, CriticalPathImpl, CriticalPath );
DECLARE_BONEXTENSION( BON::Connection, DstEdgeImpl, DstEdge );
DECLARE_BONEXTENSION( BON::Connection, EdgePropertyImpl, EdgeProperty );
DECLARE_BONEXTENSION( BON::Connection, EffectImpl, Effect );
DECLARE_BONEXTENSION( BON::Connection, EventSinkDelegateImpl, EventSinkDelegate );
DECLARE_BONEXTENSION( BON::Connection, EventSinkPublishImpl, EventSinkPublish );
DECLARE_BONEXTENSION( BON::Connection, EventSourceDelegateImpl, EventSourceDelegate );
DECLARE_BONEXTENSION( BON::Connection, EventSourcePublishImpl, EventSourcePublish );
DECLARE_BONEXTENSION( BON::Connection, ExtResourceConnImpl, ExtResourceConn );
DECLARE_BONEXTENSION( BON::Connection, ExternalDelegateImpl, ExternalDelegate );
DECLARE_BONEXTENSION( BON::Connection, FacetDelegateImpl, FacetDelegate );
DECLARE_BONEXTENSION( BON::Connection, FacetInvokeImpl, FacetInvoke );
DECLARE_BONEXTENSION( BON::Connection, FinishImpl, Finish );
DECLARE_BONEXTENSION( BON::Connection, ImplementationImpl, Implementation );
DECLARE_BONEXTENSION( BON::Connection, ImplementationCapabilityImpl, ImplementationCapability );
DECLARE_BONEXTENSION( BON::Connection, ImplementationDependsOnImpl, ImplementationDependsOn );
DECLARE_BONEXTENSION( BON::Connection, ImplementsImpl, Implements );
DECLARE_BONEXTENSION( BON::Connection, InfoPropertyImpl, InfoProperty );
DECLARE_BONEXTENSION( BON::Connection, InputImpl, Input );
DECLARE_BONEXTENSION( BON::Connection, InstanceMappingImpl, InstanceMapping );
DECLARE_BONEXTENSION( BON::Connection, Interconnect2BridgeImpl, Interconnect2Bridge );
DECLARE_BONEXTENSION( BON::Connection, Interconnect2NodeImpl, Interconnect2Node );
DECLARE_BONEXTENSION( BON::Connection, LabelConnectionImpl, LabelConnection );
DECLARE_BONEXTENSION( BON::Connection, MakeMemberPrivateImpl, MakeMemberPrivate );
DECLARE_BONEXTENSION( BON::Connection, ManagesComponentImpl, ManagesComponent );
DECLARE_BONEXTENSION( BON::Connection, MetricConnectionImpl, MetricConnection );
DECLARE_BONEXTENSION( BON::Connection, MonolithDeployRequirementImpl, MonolithDeployRequirement );
DECLARE_BONEXTENSION( BON::Connection, MonolithExecParameterImpl, MonolithExecParameter );
DECLARE_BONEXTENSION( BON::Connection, MonolithprimaryArtifactImpl, MonolithprimaryArtifact );
DECLARE_BONEXTENSION( BON::Connection, MultiInputImpl, MultiInput );
DECLARE_BONEXTENSION( BON::Connection, Node2InterconnectImpl, Node2Interconnect );
DECLARE_BONEXTENSION( BON::Connection, PackageConfBasePackageImpl, PackageConfBasePackage );
DECLARE_BONEXTENSION( BON::Connection, PackageConfConfigPropertyImpl, PackageConfConfigProperty );
DECLARE_BONEXTENSION( BON::Connection, PackageConfReferenceImpl, PackageConfReference );
DECLARE_BONEXTENSION( BON::Connection, PackageConfSelectRequirementImpl, PackageConfSelectRequirement );
DECLARE_BONEXTENSION( BON::Connection, PackageConfSpecializedConfigImpl, PackageConfSpecializedConfig );
DECLARE_BONEXTENSION( BON::Connection, PackageConfigPropertyImpl, PackageConfigProperty );
DECLARE_BONEXTENSION( BON::Connection, PackageInfoPropertyImpl, PackageInfoProperty );
DECLARE_BONEXTENSION( BON::Connection, PackageInterfaceImpl, PackageInterface );
DECLARE_BONEXTENSION( BON::Connection, PathPropertyImpl, PathProperty );
DECLARE_BONEXTENSION( BON::Connection, PortQoSImpl, PortQoS );
DECLARE_BONEXTENSION( BON::Connection, QoSReqImpl, QoSReq );
DECLARE_BONEXTENSION( BON::Connection, RecepInvokeImpl, RecepInvoke );
DECLARE_BONEXTENSION( BON::Connection, ReceptacleDelegateImpl, ReceptacleDelegate );
DECLARE_BONEXTENSION( BON::Connection, SharesImpl, Shares );
DECLARE_BONEXTENSION( BON::Connection, SrcEdgeImpl, SrcEdge );
DECLARE_BONEXTENSION( BON::Connection, TimerConnectionImpl, TimerConnection );
DECLARE_BONEXTENSION( BON::Connection, TimerEventSinkConnImpl, TimerEventSinkConn );
DECLARE_BONEXTENSION( BON::Connection, TransitionImpl, Transition );
DECLARE_BONEXTENSION( BON::Connection, WorkLoadOperationConnectionImpl, WorkLoadOperationConnection );
DECLARE_BONEXTENSION( BON::Connection, WorkloadCharacteristicsImpl, WorkloadCharacteristics );
DECLARE_BONEXTENSION( BON::Connection, deliverToImpl, deliverTo );
DECLARE_BONEXTENSION( BON::Connection, emitImpl, emit );
DECLARE_BONEXTENSION( BON::Connection, invokeImpl, invoke );
DECLARE_BONEXTENSION( BON::Connection, packageImpl, package );
DECLARE_BONEXTENSION( BON::Connection, publishImpl, publish );
DECLARE_BONEXTENSION( BON::Set, CollocationGroupImpl, CollocationGroup );
DECLARE_BONEXTENSION( BON::Set, TaskSetImpl, TaskSet );
DECLARE_BONEXTENSION( BON::Set, WorkspacesImpl, Workspaces );
DECLARE_BONEXTENSION( BON::Reference, AttributeMappingImpl, AttributeMapping );
DECLARE_BONEXTENSION( BON::Reference, AttributeMemberImpl, AttributeMember );
DECLARE_BONEXTENSION( BON::Reference, CompRefImpl, CompRef );
DECLARE_BONEXTENSION( BON::Reference, ComponentAssemblyReferenceImpl, ComponentAssemblyReference );
DECLARE_BONEXTENSION( BON::Reference, ComponentImplementationReferenceImpl, ComponentImplementationReference );
DECLARE_BONEXTENSION( BON::Reference, ComponentPackageReferenceImpl, ComponentPackageReference );
DECLARE_BONEXTENSION( BON::Reference, DataTypeImpl, DataType );
DECLARE_BONEXTENSION( BON::Reference, DiscriminatorImpl, Discriminator );
DECLARE_BONEXTENSION( BON::Reference, EventRefImpl, EventRef );
DECLARE_BONEXTENSION( BON::Reference, ExceptionRefImpl, ExceptionRef );
DECLARE_BONEXTENSION( BON::Reference, ExternalResourcesImpl, ExternalResources );
DECLARE_BONEXTENSION( BON::Reference, FileRefImpl, FileRef );
DECLARE_BONEXTENSION( BON::Reference, GetExceptionImpl, GetException );
DECLARE_BONEXTENSION( BON::Reference, ImplementationArtifactReferenceImpl, ImplementationArtifactReference );
DECLARE_BONEXTENSION( BON::Reference, InParameterImpl, InParameter );
DECLARE_BONEXTENSION( BON::Reference, InheritsImpl, Inherits );
DECLARE_BONEXTENSION( BON::Reference, InoutParameterImpl, InoutParameter );
DECLARE_BONEXTENSION( BON::Reference, LookupKeyImpl, LookupKey );
DECLARE_BONEXTENSION( BON::Reference, MemberImpl, Member );
DECLARE_BONEXTENSION( BON::Reference, NodeReferenceImpl, NodeReference );
DECLARE_BONEXTENSION( BON::Reference, OperationRefImpl, OperationRef );
DECLARE_BONEXTENSION( BON::Reference, OutParameterImpl, OutParameter );
DECLARE_BONEXTENSION( BON::Reference, PackageConfigurationReferenceImpl, PackageConfigurationReference );
DECLARE_BONEXTENSION( BON::Reference, PathReferenceImpl, PathReference );
DECLARE_BONEXTENSION( BON::Reference, ReturnTypeImpl, ReturnType );
DECLARE_BONEXTENSION( BON::Reference, SetExceptionImpl, SetException );
DECLARE_BONEXTENSION( BON::Reference, SharedComponentReferenceImpl, SharedComponentReference );
DECLARE_BONEXTENSION( BON::Reference, SupportsImpl, Supports );
DECLARE_BONEXTENSION( BON::Reference, VariableImpl, Variable );
DECLARE_BONEXTENSION( BON::Reference, WorkerTypeImpl, WorkerType );

//*******************************************************************
//   C  L  A  S  S   MonolithExecParameterImpl
//*******************************************************************
class MonolithExecParameterImpl :
	  virtual public BON::ConnectionImpl
{
public:

	//
	// connectionEnd getters
	virtual Property    getDst();
	virtual MonolithicImplementation        getSrc();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   MonolithprimaryArtifactImpl
//*******************************************************************
class MonolithprimaryArtifactImpl :
	  virtual public BON::ConnectionImpl
{
public:

	//
	// connectionEnd getters
	virtual ImplementationArtifactReference getDst();
	virtual MonolithicImplementation        getSrc();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   MultiInputImpl
//*******************************************************************
class MultiInputImpl :
	  virtual public BON::ConnectionImpl
{
public:

	//
	// connectionEnd getters
	virtual InputActionBase                 getDst();
	virtual MultiInputBase                  getSrc();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   Node2InterconnectImpl
//*******************************************************************
class Node2InterconnectImpl :
	  virtual public BON::ConnectionImpl
{
public:

	//
	// connectionEnd getters
	virtual Interconnect                    getDst();
	virtual Node        getSrc();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   PackageConfBasePackageImpl
//*******************************************************************
class PackageConfBasePackageImpl :
	  virtual public BON::ConnectionImpl
{
public:

	//
	// connectionEnd getters
	virtual ComponentPackage                getDst();
	virtual PackageConfiguration            getSrc();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   PackageConfConfigPropertyImpl
//*******************************************************************
class PackageConfConfigPropertyImpl :
	  virtual public BON::ConnectionImpl
{
public:

	//
	// connectionEnd getters
	virtual Property    getDst();
	virtual PackageConfiguration            getSrc();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   PackageConfReferenceImpl
//*******************************************************************
class PackageConfReferenceImpl :
	  virtual public BON::ConnectionImpl
{
public:

	//
	// connectionEnd getters
	virtual ComponentPackageReference       getDst();
	virtual PackageConfiguration            getSrc();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   PackageConfSelectRequirementImpl
//*******************************************************************
class PackageConfSelectRequirementImpl :
	  virtual public BON::ConnectionImpl
{
public:

	//
	// connectionEnd getters
	virtual Requirement getDst();
	virtual PackageConfiguration            getSrc();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   PackageConfSpecializedConfigImpl
//*******************************************************************
class PackageConfSpecializedConfigImpl :
	  virtual public BON::ConnectionImpl
{
public:

	//
	// connectionEnd getters
	virtual PackageConfigurationReference   getDst();
	virtual PackageConfiguration            getSrc();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   PackageConfigPropertyImpl
//*******************************************************************
class PackageConfigPropertyImpl :
	  virtual public BON::ConnectionImpl
{
public:

	//
	// connectionEnd getters
	virtual Property    getDst();
	virtual ComponentPackage                getSrc();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   PackageInfoPropertyImpl
//*******************************************************************
class PackageInfoPropertyImpl :
	  virtual public BON::ConnectionImpl
{
public:

	//
	// connectionEnd getters
	virtual Property    getDst();
	virtual ComponentPackage                getSrc();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   PackageInterfaceImpl
//*******************************************************************
class PackageInterfaceImpl :
	  virtual public BON::ConnectionImpl
{
public:

	//
	// connectionEnd getters
	virtual ComponentRef                    getDst();
	virtual ComponentPackage                getSrc();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   PathPropertyImpl
//*******************************************************************
class PathPropertyImpl :
	  virtual public BON::ConnectionImpl
{
public:

	//
	// connectionEnd getters
	virtual Property    getDst();
	virtual Path        getSrc();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   PortQoSImpl
//*******************************************************************
class PortQoSImpl :
	  virtual public BON::ConnectionImpl
{
public:

	//
	// connectionEnd getters
	virtual ReqQoSBase  getDst();
	virtual BON::ConnectionEnd              getSrc();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   QoSReqImpl
//*******************************************************************
class QoSReqImpl :
	  virtual public BON::ConnectionImpl
{
public:

	//
	// connectionEnd getters
	virtual ReqQoSBase  getDst();
	virtual QoSConnector                    getSrc();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   RecepInvokeImpl
//*******************************************************************
class RecepInvokeImpl :
	  virtual public BON::ConnectionImpl
{
public:

	//
	// connectionEnd getters
	virtual QoSConnector                    getDst();
	virtual BON::ConnectionEnd              getSrc();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   ReceptacleDelegateImpl
//*******************************************************************
class ReceptacleDelegateImpl :
	  virtual public BON::ConnectionImpl
{
public:

	//
	// connectionEnd getters
	virtual BON::ConnectionEnd              getDst();
	virtual BON::ConnectionEnd              getSrc();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   SharesImpl
//*******************************************************************
class SharesImpl :
	  virtual public BON::ConnectionImpl
{
public:

	//
	// connectionEnd getters
	virtual SharedResource                  getDst();
	virtual Node        getSrc();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   SrcEdgeImpl
//*******************************************************************
class SrcEdgeImpl :
	  virtual public BON::ConnectionImpl
{
public:

	//
	// connectionEnd getters
	virtual BON::ConnectionEnd              getDst();
	virtual Edge        getSrc();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   TimerConnectionImpl
//*******************************************************************
class TimerConnectionImpl :
	  virtual public BON::ConnectionImpl
{
public:

	//
	// connectionEnd getters
	virtual TimeProbe   getDst();
	virtual OperationRef                    getSrc();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   TimerEventSinkConnImpl
//*******************************************************************
class TimerEventSinkConnImpl :
	  virtual public BON::ConnectionImpl
{
public:

	//
	// connectionEnd getters
	virtual TimeProbe   getDst();
	virtual EventRef    getSrc();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   TransitionImpl
//*******************************************************************
class TransitionImpl :
	  virtual public BON::ConnectionImpl
{
public:

	//
	// attribute getters and setters
	virtual std::string getPrecondition() ;
	virtual void        setPrecondition( const std::string& val);
	//
	// connectionEnd getters
	virtual ActionBase  getDst();
	virtual State       getSrc();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   WorkLoadOperationConnectionImpl
//*******************************************************************
class WorkLoadOperationConnectionImpl :
	  virtual public BON::ConnectionImpl
{
public:

	//
	// connectionEnd getters
	virtual Task        getDst();
	virtual OperationRef                    getSrc();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   WorkloadCharacteristicsImpl
//*******************************************************************
class WorkloadCharacteristicsImpl :
	  virtual public BON::ConnectionImpl
{
public:

	//
	// connectionEnd getters
	virtual TaskSet     getDst();
	virtual MetricsBase getSrc();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   deliverToImpl
//*******************************************************************
class deliverToImpl :
	  virtual public BON::ConnectionImpl
{
public:

	//
	// connectionEnd getters
	virtual BON::ConnectionEnd              getDst();
	virtual PublishConnector                getSrc();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   emitImpl
//*******************************************************************
class emitImpl :
	  virtual public BON::ConnectionImpl
{
public:

	//
	// connectionEnd getters
	virtual BON::ConnectionEnd              getDst();
	virtual BON::ConnectionEnd              getSrc();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   invokeImpl
//*******************************************************************
class invokeImpl :
	  virtual public BON::ConnectionImpl
{
public:

	//
	// connectionEnd getters
	virtual BON::ConnectionEnd              getDst();
	virtual BON::ConnectionEnd              getSrc();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   packageImpl
//*******************************************************************
class packageImpl :
	  virtual public BON::ConnectionImpl
{
public:

	//
	// connectionEnd getters
	virtual PackageConfigurationReference   getDst();
	virtual TopLevelPackage                 getSrc();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   publishImpl
//*******************************************************************
class publishImpl :
	  virtual public BON::ConnectionImpl
{
public:

	//
	// connectionEnd getters
	virtual PublishConnector                getDst();
	virtual BON::ConnectionEnd              getSrc();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   CollocationGroupImpl
//*******************************************************************
class CollocationGroupImpl :
	  virtual public BON::SetImpl
{
public:

	//
	// connectionEnd getters
	virtual std::multiset<NodeReference>    getInstanceMappingDsts();
	virtual std::set<InstanceMapping>       getOutInstanceMappingLinks();
	//
	// set getters
	virtual std::set<BON::Reference>        getAllMembers();
	virtual std::set<ComponentAssemblyReference>      getComponentAssemblyReferences();
	virtual std::set<ComponentRef>          getComponentRefs();
	virtual std::set<SharedComponentReference>        getSharedComponentReferences();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   TaskSetImpl
//*******************************************************************
class TaskSetImpl :
	  virtual public BON::SetImpl
{
public:

	//
	// connectionEnd getters
	virtual std::set<WorkloadCharacteristics>         getInWorkloadCharacteristicsLinks();
	virtual std::multiset<MetricsBase>      getWorkloadCharacteristicsSrcs();
	//
	// attribute getters and setters
	virtual long        getpriority() ;
	virtual long        getrate() ;
	virtual void        setpriority( const long val);
	virtual void        setrate( const long val);
	//
	// set getters
	virtual std::set<Task>                  getTasks( int dummy);
	virtual std::set<Task>                  getTasks();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   WorkspacesImpl
//*******************************************************************
class WorkspacesImpl :
	  virtual public BON::SetImpl
{
public:

	//
	// set getters
	virtual std::set<Project>               getProjects( int dummy);
	virtual std::set<Project>               getProjects();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   AttributeMappingImpl
//*******************************************************************
class AttributeMappingImpl :
	  virtual public BON::ReferenceImpl
{
public:

	//
	// connectionEnd getters
	virtual std::multiset<BON::ConnectionEnd>         getAttributeDelegateDsts();
	virtual std::multiset<BON::ConnectionEnd>         getAttributeMappingDelegateDsts();
	virtual std::set<AttributeMappingDelegate>        getAttributeMappingDelegateLinks();
	virtual std::multiset<BON::ConnectionEnd>         getAttributeMappingDelegateSrcs();
	virtual std::multiset<Property>         getAttributeMappingValueDsts();
	virtual std::set<AttributeMappingDelegate>        getInAttributeMappingDelegateLinks();
	virtual std::set<AttributeDelegate>     getOutAttributeDelegateLinks();
	virtual std::set<AttributeMappingDelegate>        getOutAttributeMappingDelegateLinks();
	virtual std::set<AttributeMappingValue> getOutAttributeMappingValueLinks();
	//
	// ref getters
	virtual ReadonlyAttribute               getReadonlyAttribute();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   AttributeMemberImpl
//*******************************************************************
class AttributeMemberImpl :
	  virtual public BON::ReferenceImpl
{
public:

	//
	// ref getters
	virtual MemberType  getMemberType();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   CompRefImpl
//*******************************************************************
class CompRefImpl :
	  virtual public BON::ReferenceImpl
{
public:

	//
	// connectionEnd getters
	virtual std::multiset<OperationRef>     getComponentOperationSrcs();
	virtual std::set<ComponentOperation>    getInComponentOperationLinks();
	//
	// ref getters
	virtual Component   getComponent();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   ComponentAssemblyReferenceImpl
//*******************************************************************
class ComponentAssemblyReferenceImpl :
	  virtual public BON::ReferenceImpl
{
public:

	//
	// ref getters
	virtual ComponentAssembly               getComponentAssembly();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   ComponentImplementationReferenceImpl
//*******************************************************************
class ComponentImplementationReferenceImpl :
	  virtual public BON::ReferenceImpl
{
public:

	//
	// connectionEnd getters
	virtual std::multiset<ComponentPackage> getImplementationSrcs();
	virtual std::set<Implementation>        getInImplementationLinks();
	//
	// ref getters
	virtual ComponentImplementation         getComponentImplementation();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   ComponentPackageReferenceImpl
//*******************************************************************
class ComponentPackageReferenceImpl :
	  virtual public BON::ReferenceImpl
{
public:

	//
	// connectionEnd getters
	virtual std::set<PackageConfReference>  getInPackageConfReferenceLinks();
	virtual std::multiset<PackageConfiguration>       getPackageConfReferenceSrcs();
	//
	// attribute getters and setters
	virtual std::string getrequiredName() ;
	virtual std::string getrequiredType() ;
	virtual std::string getrequiredUUID() ;
	virtual void        setrequiredName( const std::string& val);
	virtual void        setrequiredType( const std::string& val);
	virtual void        setrequiredUUID( const std::string& val);
	//
	// ref getters
	virtual ComponentPackage                getComponentPackage();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   DataTypeImpl
//*******************************************************************
class DataTypeImpl :
	  virtual public BON::ReferenceImpl
{
public:

	//
	// ref getters
	virtual PredefinedType                  getPredefinedType();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   DiscriminatorImpl
//*******************************************************************
class DiscriminatorImpl :
	  virtual public BON::ReferenceImpl
{
public:

	//
	// ref getters
	virtual ConstantType                    getConstantType();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   EventRefImpl
//*******************************************************************
class EventRefImpl :
	  virtual public BON::ReferenceImpl
{
public:

	//
	// connectionEnd getters
	virtual std::set<TimerEventSinkConn>    getOutTimerEventSinkConnLinks();
	virtual std::multiset<TimeProbe>        getTimerEventSinkConnDsts();
	//
	// ref getters
	virtual Event       getEvent();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   ExceptionRefImpl
//*******************************************************************
class ExceptionRefImpl :
	  virtual public BON::ReferenceImpl
{
public:

	//
	// ref getters
	virtual Exception   getException();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   ExternalResourcesImpl
//*******************************************************************
class ExternalResourcesImpl :
	  virtual public BON::ReferenceImpl
{
public:

	//
	// connectionEnd getters
	virtual std::multiset<ComponentLib>     getExtResourceConnSrcs();
	virtual std::set<ExtResourceConn>       getInExtResourceConnLinks();
	//
	// ref getters
	virtual ImplementationArtifact          getImplementationArtifact();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   FileRefImpl
//*******************************************************************
class FileRefImpl :
	  virtual public BON::ReferenceImpl
{
public:

	//
	// ref getters
	virtual File        getFile();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   GetExceptionImpl
//*******************************************************************
class GetExceptionImpl :
	  virtual public BON::ReferenceImpl
{
public:

	//
	// ref getters
	virtual Exception   getException();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   ImplementationArtifactReferenceImpl
//*******************************************************************
class ImplementationArtifactReferenceImpl :
	  virtual public BON::ReferenceImpl
{
public:

	//
	// connectionEnd getters
	virtual std::multiset<ImplementationArtifact>     getArtifactDependsOnSrcs();
	virtual std::set<ArtifactDependsOn>     getInArtifactDependsOnLinks();
	virtual std::set<MonolithprimaryArtifact>         getInMonolithprimaryArtifactLinks();
	virtual std::multiset<MonolithicImplementation>   getMonolithprimaryArtifactSrcs();
	//
	// ref getters
	virtual ImplementationArtifact          getImplementationArtifact();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   InParameterImpl
//*******************************************************************
class InParameterImpl :
	  virtual public BON::ReferenceImpl
{
public:

	//
	// ref getters
	virtual MemberType  getMemberType();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   InheritsImpl
//*******************************************************************
class InheritsImpl :
	  virtual public BON::ReferenceImpl
{
public:

	//
	// ref getters
	virtual Inheritable getInheritable();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   InoutParameterImpl
//*******************************************************************
class InoutParameterImpl :
	  virtual public BON::ReferenceImpl
{
public:

	//
	// ref getters
	virtual MemberType  getMemberType();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   LookupKeyImpl
//*******************************************************************
class LookupKeyImpl :
	  virtual public BON::ReferenceImpl
{
public:

	//
	// ref getters
	virtual ValueObject getValueObject();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   MemberImpl
//*******************************************************************
class MemberImpl :
	  virtual public BON::ReferenceImpl
{
public:

	//
	// connectionEnd getters
	virtual std::multiset<Label>            getLabelConnectionDsts();
	virtual std::multiset<PrivateFlag>      getMakeMemberPrivateDsts();
	virtual std::set<LabelConnection>       getOutLabelConnectionLinks();
	virtual std::set<MakeMemberPrivate>     getOutMakeMemberPrivateLinks();
	//
	// ref getters
	virtual MemberType  getMemberType();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   NodeReferenceImpl
//*******************************************************************
class NodeReferenceImpl :
	  virtual public BON::ReferenceImpl
{
public:

	//
	// connectionEnd getters
	virtual std::set<InstanceMapping>       getInInstanceMappingLinks();
	virtual std::multiset<CollocationGroup> getInstanceMappingSrcs();
	//
	// ref getters
	virtual Node        getNode();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   OperationRefImpl
//*******************************************************************
class OperationRefImpl :
	  virtual public BON::ReferenceImpl
{
public:

	//
	// connectionEnd getters
	virtual std::multiset<CompRef>          getComponentOperationDsts();
	virtual std::multiset<MetricsBase>      getMetricConnectionDsts();
	virtual std::set<ComponentOperation>    getOutComponentOperationLinks();
	virtual std::set<MetricConnection>      getOutMetricConnectionLinks();
	virtual std::set<TimerConnection>       getOutTimerConnectionLinks();
	virtual std::set<WorkLoadOperationConnection>     getOutWorkLoadOperationConnectionLinks();
	virtual std::multiset<TimeProbe>        getTimerConnectionDsts();
	virtual std::multiset<Task>             getWorkLoadOperationConnectionDsts();
	//
	// ref getters
	virtual OperationBase                   getOperationBase();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   OutParameterImpl
//*******************************************************************
class OutParameterImpl :
	  virtual public BON::ReferenceImpl
{
public:

	//
	// ref getters
	virtual MemberType  getMemberType();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   PackageConfigurationReferenceImpl
//*******************************************************************
class PackageConfigurationReferenceImpl :
	  virtual public BON::ReferenceImpl
{
public:

	//
	// connectionEnd getters
	virtual std::set<PackageConfSpecializedConfig>    getInPackageConfSpecializedConfigLinks();
	virtual std::set<package>               getInpackageLinks();
	virtual std::multiset<PackageConfiguration>       getPackageConfSpecializedConfigSrcs();
	virtual std::multiset<TopLevelPackage>  getpackageSrcs();
	//
	// ref getters
	virtual PackageConfiguration            getPackageConfiguration();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   PathReferenceImpl
//*******************************************************************
class PathReferenceImpl :
	  virtual public BON::ReferenceImpl
{
public:

	//
	// connectionEnd getters
	virtual std::multiset<ComponentAssembly>          getCriticalPathSrcs();
	virtual std::set<CriticalPath>          getInCriticalPathLinks();
	//
	// ref getters
	virtual Path        getPath();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   ReturnTypeImpl
//*******************************************************************
class ReturnTypeImpl :
	  virtual public BON::ReferenceImpl
{
public:

	//
	// ref getters
	virtual MemberType  getMemberType();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   SetExceptionImpl
//*******************************************************************
class SetExceptionImpl :
	  virtual public BON::ReferenceImpl
{
public:

	//
	// ref getters
	virtual Exception   getException();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   SharedComponentReferenceImpl
//*******************************************************************
class SharedComponentReferenceImpl :
	  virtual public BON::ReferenceImpl
{
public:

	//
	// ref getters
	virtual ComponentRef                    getComponentRef();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   SupportsImpl
//*******************************************************************
class SupportsImpl :
	  virtual public BON::ReferenceImpl
{
public:

	//
	// ref getters
	virtual Object      getObject();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   VariableImpl
//*******************************************************************
class VariableImpl :
	  virtual public BON::ReferenceImpl
{
public:

	//
	// ref getters
	virtual PredefinedType                  getPredefinedType();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   WorkerTypeImpl
//*******************************************************************
class WorkerTypeImpl :
	  virtual public BON::ReferenceImpl
{
public:

	//
	// ref getters
	virtual Worker      getWorker();

	///BUP
	// add your own members here
	///EUP
};



} // namespace PICML

///BUP
// add your additional class definitions here
///EUP

#endif // PICMLBONX4_H
