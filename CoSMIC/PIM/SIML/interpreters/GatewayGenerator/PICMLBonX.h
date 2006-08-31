#ifndef PICMLBONX_H
#define PICMLBONX_H

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
//   C  L  A  S  S   ComponentAnalysesImpl
//*******************************************************************
class ComponentAnalysesImpl :
	  public BON::FolderImpl
{
public:
	//
	// kind and subfolder getters
	virtual std::set<BenchmarkAnalysis>     getBenchmarkAnalysis();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   ComponentBuildImpl
//*******************************************************************
class ComponentBuildImpl :
	  public BON::FolderImpl
{
public:
	//
	// kind and subfolder getters
	virtual std::set<MPC>                   getMPC();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   ComponentImplementationsImpl
//*******************************************************************
class ComponentImplementationsImpl :
	  public BON::FolderImpl
{
public:
	//
	// kind and subfolder getters
	virtual std::set<ComponentImplementationContainer> getComponentImplementationContainer();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   ComponentPackagesImpl
//*******************************************************************
class ComponentPackagesImpl :
	  public BON::FolderImpl
{
public:
	//
	// kind and subfolder getters
	virtual std::set<PackageContainer>      getPackageContainer();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   ComponentTypesImpl
//*******************************************************************
class ComponentTypesImpl :
	  public BON::FolderImpl
{
public:
	//
	// kind and subfolder getters
	virtual std::set<ComponentContainer>    getComponentContainer();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   DeploymentPlansImpl
//*******************************************************************
class DeploymentPlansImpl :
	  public BON::FolderImpl
{
public:
	//
	// kind and subfolder getters
	virtual std::set<DeploymentPlan>        getDeploymentPlan();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   ImplementationArtifactsImpl
//*******************************************************************
class ImplementationArtifactsImpl :
	  public BON::FolderImpl
{
public:
	//
	// kind and subfolder getters
	virtual std::set<ArtifactContainer>     getArtifactContainer();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   InterfaceDefinitionsImpl
//*******************************************************************
class InterfaceDefinitionsImpl :
	  public BON::FolderImpl
{
public:
	//
	// kind and subfolder getters
	virtual std::set<File>                  getFile();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   PackageConfigurationsImpl
//*******************************************************************
class PackageConfigurationsImpl :
	  public BON::FolderImpl
{
public:
	//
	// kind and subfolder getters
	virtual std::set<PackageConfigurationContainer>   getPackageConfigurationContainer();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   PathDiagramsImpl
//*******************************************************************
class PathDiagramsImpl :
	  public BON::FolderImpl
{
public:
	//
	// kind and subfolder getters
	virtual std::set<Paths>                 getPaths();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   PredefinedTypesImpl
//*******************************************************************
class PredefinedTypesImpl :
	  public BON::FolderImpl
{
public:
	//
	// kind and subfolder getters
	virtual std::set<Boolean>               getBoolean();
	virtual std::set<Byte>                  getByte();
	virtual std::set<GenericObject>         getGenericObject();
	virtual std::set<GenericValue>          getGenericValue();
	virtual std::set<GenericValueObject>    getGenericValueObject();
	virtual std::set<LongInteger>           getLongInteger();
	virtual std::set<PredefinedType>        getPredefinedType();
	virtual std::set<RealNumber>            getRealNumber();
	virtual std::set<ShortInteger>          getShortInteger();
	virtual std::set<String>                getString();
	virtual std::set<TypeEncoding>          getTypeEncoding();
	virtual std::set<TypeKind>              getTypeKind();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   TargetsImpl
//*******************************************************************
class TargetsImpl :
	  public BON::FolderImpl
{
public:
	//
	// kind and subfolder getters
	virtual std::set<Domain>                getDomain();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   TopLevelPackagesImpl
//*******************************************************************
class TopLevelPackagesImpl :
	  public BON::FolderImpl
{
public:
	//
	// kind and subfolder getters
	virtual std::set<TopLevelPackageContainer>        getTopLevelPackageContainer();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   WorkerLibrariesImpl
//*******************************************************************
class WorkerLibrariesImpl :
	  public BON::FolderImpl
{
public:
	//
	// kind and subfolder getters
	virtual std::set<WorkerLibrary>         getWorkerLibrary();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   PrefixableImpl
//*******************************************************************
class PrefixableImpl :
	  virtual public BON::ModelImpl
{
public:

	//
	// attribute getters and setters
	virtual std::string getPrefixTag() ;
	virtual void        setPrefixTag( const std::string& val);

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   SupportsInterfacesImpl
//*******************************************************************
class SupportsInterfacesImpl :
	  virtual public BON::ModelImpl
{
public:

	//
	// kind and role getters
	virtual std::set<Supports>              getSupports();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   CommonPortAttrsImpl
//*******************************************************************
class CommonPortAttrsImpl :
	  virtual public BON::FCOImpl
{
public:

	//
	// attribute getters and setters
	virtual bool        isexclusiveProvider() ;
	virtual bool        isexclusiveUser() ;
	virtual bool        isoptional() ;
	virtual void        setexclusiveProvider( bool val);
	virtual void        setexclusiveUser( bool val);
	virtual void        setoptional( bool val);

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   ConstantTypeImpl
//*******************************************************************
class ConstantTypeImpl :
	  virtual public BON::FCOImpl
{
public:


	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   GraphVertexImpl
//*******************************************************************
class GraphVertexImpl :
	  virtual public BON::FCOImpl
{
public:

	//
	// connectionEnd getters
	virtual std::multiset<Edge>             getDstEdgeDsts();
	virtual std::set<SrcEdge>               getInSrcEdgeLinks();
	virtual std::set<DstEdge>               getOutDstEdgeLinks();
	virtual std::multiset<Edge>             getSrcEdgeSrcs();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   ManageableImpl
//*******************************************************************
class ManageableImpl :
	  virtual public BON::FCOImpl
{
public:

	//
	// connectionEnd getters
	virtual std::set<ManagesComponent>      getInManagesComponentLinks();
	virtual std::multiset<ComponentFactory> getManagesComponentSrcs();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   MemberTypeImpl
//*******************************************************************
class MemberTypeImpl :
	  virtual public BON::FCOImpl
{
public:


	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   MultiInputBaseImpl
//*******************************************************************
class MultiInputBaseImpl :
	  virtual public BON::FCOImpl
{
public:

	//
	// connectionEnd getters
	virtual std::multiset<InputActionBase>  getMultiInputDsts();
	virtual std::set<MultiInput>            getOutMultiInputLinks();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   ProvideableImpl
//*******************************************************************
class ProvideableImpl :
	  virtual public BON::FCOImpl
{
public:


	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   TaggableImpl
//*******************************************************************
class TaggableImpl :
	  virtual public BON::FCOImpl
{
public:

	//
	// attribute getters and setters
	virtual std::string getSpecifyIdTag() ;
	virtual std::string getVersionTag() ;
	virtual void        setSpecifyIdTag( const std::string& val);
	virtual void        setVersionTag( const std::string& val);

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   ConnectedComponentImpl
//*******************************************************************
class ConnectedComponentImpl :
	  virtual public BON::ModelImpl
	, public GraphVertexImpl
{
public:

	//
	// kind and role getters
	virtual std::set<DstEdge>               getDstEdge();
	virtual std::set<InEventPort>           getInEventPort();
	virtual std::set<OutEventPort>          getOutEventPort();
	virtual std::set<Port>                  getPort();
	virtual std::set<ProvidedRequestPort>   getProvidedRequestPort();
	virtual std::set<RequiredRequestPort>   getRequiredRequestPort();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   ExceptionImpl
//*******************************************************************
class ExceptionImpl :
	  virtual public BON::ModelImpl
	, public TaggableImpl
{
public:

	//
	// kind and role getters
	virtual std::set<Member>                getMember();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   FileImpl
//*******************************************************************
class FileImpl :
	  public PrefixableImpl
{
public:

	//
	// kind and role getters
	virtual std::set<Aggregate>             getAggregate();
	virtual std::set<Alias>                 getAlias();
	virtual std::set<Boxed>                 getBoxed();
	virtual std::set<Collection>            getCollection();
	virtual std::set<Component>             getComponent();
	virtual std::set<ComponentFactory>      getComponentFactory();
	virtual std::set<ComponentRef>          getComponentRef();
	virtual std::set<Constant>              getConstant();
	virtual std::set<Enum>                  getEnum();
	virtual std::set<Event>                 getEvent();
	virtual std::set<Exception>             getException();
	virtual std::set<FileRef>               getFileRef();
	virtual std::set<ManagesComponent>      getManagesComponent();
	virtual std::set<NamedType>             getNamedType();
	virtual std::set<Object>                getObject();
	virtual std::set<Package>               getPackage();
	virtual std::set<SwitchedAggregate>     getSwitchedAggregate();
	virtual std::set<ValueObject>           getValueObject();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   OperationBaseImpl
//*******************************************************************
class OperationBaseImpl :
	  virtual public BON::ModelImpl
	, public TaggableImpl
{
public:

	//
	// kind and role getters
	virtual std::set<InParameter>           getInParameter();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   PackageImpl
//*******************************************************************
class PackageImpl :
	  public PrefixableImpl
	, public TaggableImpl
{
public:

	//
	// kind and role getters
	virtual std::set<Aggregate>             getAggregate();
	virtual std::set<Alias>                 getAlias();
	virtual std::set<Boxed>                 getBoxed();
	virtual std::set<Collection>            getCollection();
	virtual std::set<Component>             getComponent();
	virtual std::set<ComponentFactory>      getComponentFactory();
	virtual std::set<ComponentRef>          getComponentRef();
	virtual std::set<Constant>              getConstant();
	virtual std::set<Enum>                  getEnum();
	virtual std::set<Event>                 getEvent();
	virtual std::set<Exception>             getException();
	virtual std::set<ManagesComponent>      getManagesComponent();
	virtual std::set<NamedType>             getNamedType();
	virtual std::set<Object>                getObject();
	virtual std::set<Package>               getPackage();
	virtual std::set<SwitchedAggregate>     getSwitchedAggregate();
	virtual std::set<ValueObject>           getValueObject();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   ReadonlyAttributeImpl
//*******************************************************************
class ReadonlyAttributeImpl :
	  virtual public BON::ModelImpl
	, public TaggableImpl
{
public:

	//
	// connectionEnd getters
	virtual std::multiset<BON::ConnectionEnd>         getAttributeDelegateSrcs();
	virtual std::multiset<Property>         getAttributeValueDsts();
	virtual std::set<AttributeDelegate>     getInAttributeDelegateLinks();
	virtual std::set<AttributeValue>        getOutAttributeValueLinks();
	//
	// kind and role getters
	virtual std::set<AttributeMember>       getAttributeMember();
	virtual std::set<GetException>          getGetException();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   NamedTypeImpl
//*******************************************************************
class NamedTypeImpl :
	  public TaggableImpl
	, public MemberTypeImpl
{
public:


	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   PortImpl
//*******************************************************************
class PortImpl :
	  public TaggableImpl
	, public GraphVertexImpl
	, public CommonPortAttrsImpl
{
public:

	//
	// connectionEnd getters
	virtual std::set<PortQoS>               getOutPortQoSLinks();
	virtual std::multiset<ReqQoSBase>       getPortQoSDsts();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   PredefinedTypeImpl
//*******************************************************************
class PredefinedTypeImpl :
	  public MemberTypeImpl
	, public ConstantTypeImpl
{
public:


	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   DisplayNodeImpl
//*******************************************************************
class DisplayNodeImpl :
	  virtual public BON::AtomImpl
	, public GraphVertexImpl
{
public:


	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   EnvironmentImpl
//*******************************************************************
class EnvironmentImpl :
	  virtual public BON::AtomImpl
	, public MultiInputBaseImpl
{
public:


	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   ComponentRefImpl
//*******************************************************************
class ComponentRefImpl :
	  virtual public BON::ReferenceImpl
	, public ManageableImpl
{
public:

	//
	// connectionEnd getters
	virtual std::multiset<Property>         getComponentConfigPropertyDsts();
	virtual std::multiset<Property>         getComponentInfoPropertyDsts();
	virtual std::multiset<ComponentPropertyDescription> getComponentPropertyDsts();
	virtual std::multiset<ComponentImplementation>    getImplementsSrcs();
	virtual std::set<Implements>            getInImplementsLinks();
	virtual std::set<PackageInterface>      getInPackageInterfaceLinks();
	virtual std::set<ComponentConfigProperty>         getOutComponentConfigPropertyLinks();
	virtual std::set<ComponentInfoProperty> getOutComponentInfoPropertyLinks();
	virtual std::set<ComponentProperty>     getOutComponentPropertyLinks();
	virtual std::multiset<ComponentPackage> getPackageInterfaceSrcs();
	//
	// ref getters
	virtual Component   getComponent();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   ConstantImpl
//*******************************************************************
class ConstantImpl :
	  virtual public BON::ReferenceImpl
	, public TaggableImpl
{
public:

	//
	// attribute getters and setters
	virtual std::string getvalue() ;
	virtual void        setvalue( const std::string& val);
	//
	// ref getters
	virtual ConstantType                    getConstantType();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   AttributeImpl
//*******************************************************************
class AttributeImpl :
	  public ReadonlyAttributeImpl
{
public:

	//
	// kind and role getters
	virtual std::set<SetException>          getSetException();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   ComponentImpl
//*******************************************************************
class ComponentImpl :
	  public SupportsInterfacesImpl
	, public ManageableImpl
	, public NamedTypeImpl
{
public:

	//
	// connectionEnd getters
	virtual std::multiset<Property>         getAssemblyConfigPropertyDsts();
	virtual std::multiset<Requirement>      getAssemblyselectRequirementDsts();
	virtual std::multiset<ExternalReferenceEndPoint>  getExternalDelegateDsts();
	virtual std::set<AssemblyConfigProperty>          getOutAssemblyConfigPropertyLinks();
	virtual std::set<AssemblyselectRequirement>       getOutAssemblyselectRequirementLinks();
	virtual std::set<ExternalDelegate>      getOutExternalDelegateLinks();
	//
	// attribute getters and setters
	virtual std::string getUUID() ;
	virtual std::string getlabel() ;
	virtual void        setUUID( const std::string& val);
	virtual void        setlabel( const std::string& val);
	//
	// kind and role getters
	virtual std::set<Action>                getAction();
	virtual std::set<ActionBase>            getActionBase();
	virtual std::set<Attribute>             getAttribute();
	virtual std::set<CompositeAction>       getCompositeAction();
	virtual std::set<Effect>                getEffect();
	virtual std::set<Environment>           getEnvironment();
	virtual std::set<Finish>                getFinish();
	virtual std::set<InEventPort>           getInEventPort();
	virtual std::set<Input>                 getInput();
	virtual std::set<InputAction>           getInputAction();
	virtual std::set<MultiInput>            getMultiInput();
	virtual std::set<OutEventPort>          getOutEventPort();
	virtual std::set<OutputAction>          getOutputAction();
	virtual std::set<PeriodicAction>        getPeriodicAction();
	virtual std::set<Port>                  getPort();
	virtual std::set<ProvidedRequestPort>   getProvidedRequestPort();
	virtual std::set<ReadonlyAttribute>     getReadonlyAttribute();
	virtual std::set<ReadonlyAttribute>     getReadonlyAttribute(int dummy);
	virtual std::set<RequiredRequestPort>   getRequiredRequestPort();
	virtual std::set<State>                 getState();
	virtual std::set<Transition>            getTransition();
	virtual std::set<Variable>              getVariable();
	virtual std::set<WorkerType>            getWorkerType();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   HasExceptionsImpl
//*******************************************************************
class HasExceptionsImpl :
	  public OperationBaseImpl
{
public:

	//
	// kind and role getters
	virtual std::set<ExceptionRef>          getExceptionRef();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   InheritableImpl
//*******************************************************************
class InheritableImpl :
	  virtual public BON::ModelImpl
	, public NamedTypeImpl
{
public:

	//
	// kind and role getters
	virtual std::set<Attribute>             getAttribute();
	virtual std::set<Inherits>              getInherits();
	virtual std::set<ReadonlyAttribute>     getReadonlyAttribute();
	virtual std::set<ReadonlyAttribute>     getReadonlyAttribute(int dummy);

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   OnewayOperationImpl
//*******************************************************************
class OnewayOperationImpl :
	  public OperationBaseImpl
{
public:


	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   NoInheritableImpl
//*******************************************************************
class NoInheritableImpl :
	  public NamedTypeImpl
{
public:


	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   BooleanImpl
//*******************************************************************
class BooleanImpl :
	  virtual public BON::AtomImpl
	, public PredefinedTypeImpl
{
public:


	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   ByteImpl
//*******************************************************************
class ByteImpl :
	  virtual public BON::AtomImpl
	, public PredefinedTypeImpl
{
public:


	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   GenericObjectImpl
//*******************************************************************
class GenericObjectImpl :
	  virtual public BON::AtomImpl
	, public ProvideableImpl
	, public PredefinedTypeImpl
{
public:


	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   GenericValueImpl
//*******************************************************************
class GenericValueImpl :
	  virtual public BON::AtomImpl
	, public PredefinedTypeImpl
{
public:


	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   GenericValueObjectImpl
//*******************************************************************
class GenericValueObjectImpl :
	  virtual public BON::AtomImpl
	, public PredefinedTypeImpl
{
public:


	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   LongIntegerImpl
//*******************************************************************
class LongIntegerImpl :
	  virtual public BON::AtomImpl
	, public PredefinedTypeImpl
{
public:


	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   RealNumberImpl
//*******************************************************************
class RealNumberImpl :
	  virtual public BON::AtomImpl
	, public PredefinedTypeImpl
{
public:


	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   ShortIntegerImpl
//*******************************************************************
class ShortIntegerImpl :
	  virtual public BON::AtomImpl
	, public PredefinedTypeImpl
{
public:


	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   StringImpl
//*******************************************************************
class StringImpl :
	  virtual public BON::AtomImpl
	, public PredefinedTypeImpl
{
public:


	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   TypeEncodingImpl
//*******************************************************************
class TypeEncodingImpl :
	  virtual public BON::AtomImpl
	, public PredefinedTypeImpl
{
public:


	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   TypeKindImpl
//*******************************************************************
class TypeKindImpl :
	  virtual public BON::AtomImpl
	, public PredefinedTypeImpl
{
public:


	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   BoxedImpl
//*******************************************************************
class BoxedImpl :
	  virtual public BON::ReferenceImpl
	, public NamedTypeImpl
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
//   C  L  A  S  S   InEventPortImpl
//*******************************************************************
class InEventPortImpl :
	  virtual public BON::ReferenceImpl
	, public PortImpl
{
public:

	//
	// connectionEnd getters
	virtual std::multiset<BON::ConnectionEnd>         getEventSinkDelegateDsts();
	virtual std::set<EventSinkDelegate>     getEventSinkDelegateLinks();
	virtual std::multiset<BON::ConnectionEnd>         getEventSinkDelegateSrcs();
	virtual std::multiset<QoSConnector>     getEventSinkPublishSrcs();
	virtual std::set<EventSinkDelegate>     getInEventSinkDelegateLinks();
	virtual std::set<EventSinkPublish>      getInEventSinkPublishLinks();
	virtual std::set<deliverTo>             getIndeliverToLinks();
	virtual std::set<emit>                  getInemitLinks();
	virtual std::multiset<InputActionBase>  getInputDsts();
	virtual std::set<EventSinkDelegate>     getOutEventSinkDelegateLinks();
	virtual std::set<Input>                 getOutInputLinks();
	virtual std::multiset<PublishConnector> getdeliverToSrcs();
	virtual std::multiset<BON::ConnectionEnd>         getemitSrcs();
	//
	// ref getters
	virtual Event       getEvent();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   OutEventPortImpl
//*******************************************************************
class OutEventPortImpl :
	  virtual public BON::ReferenceImpl
	, public PortImpl
{
public:
	typedef enum
	{
		DirectConnect_out_event_port_type_Type,
		RTEventChannel_out_event_port_type_Type
	} out_event_port_type_Type;

	//
	// connectionEnd getters
	virtual std::multiset<BON::ConnectionEnd>         getEventSourceDelegateDsts();
	virtual std::set<EventSourceDelegate>   getEventSourceDelegateLinks();
	virtual std::multiset<BON::ConnectionEnd>         getEventSourceDelegateSrcs();
	virtual std::multiset<QoSConnector>     getEventSourcePublishDsts();
	virtual std::set<EventSourceDelegate>   getInEventSourceDelegateLinks();
	virtual std::set<EventSourceDelegate>   getOutEventSourceDelegateLinks();
	virtual std::set<EventSourcePublish>    getOutEventSourcePublishLinks();
	virtual std::set<emit>                  getOutemitLinks();
	virtual std::set<publish>               getOutpublishLinks();
	virtual std::multiset<BON::ConnectionEnd>         getemitDsts();
	virtual std::multiset<PublishConnector> getpublishDsts();
	//
	// attribute getters and setters
	virtual OutEventPortImpl::out_event_port_type_Type getout_event_port_type();
	virtual bool        issingle_destination() ;
	virtual void        setout_event_port_type( OutEventPortImpl::out_event_port_type_Type val);
	virtual void        setsingle_destination( bool val);
	//
	// ref getters
	virtual Event       getEvent();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   ProvidedRequestPortImpl
//*******************************************************************
class ProvidedRequestPortImpl :
	  virtual public BON::ReferenceImpl
	, public MultiInputBaseImpl
	, public PortImpl
{
public:

	//
	// connectionEnd getters
	virtual std::multiset<BON::ConnectionEnd>         getFacetDelegateDsts();
	virtual std::set<FacetDelegate>         getFacetDelegateLinks();
	virtual std::multiset<BON::ConnectionEnd>         getFacetDelegateSrcs();
	virtual std::multiset<QoSConnector>     getFacetInvokeSrcs();
	virtual std::set<FacetDelegate>         getInFacetDelegateLinks();
	virtual std::set<FacetInvoke>           getInFacetInvokeLinks();
	virtual std::set<invoke>                getIninvokeLinks();
	virtual std::set<FacetDelegate>         getOutFacetDelegateLinks();
	virtual std::multiset<BON::ConnectionEnd>         getinvokeSrcs();
	//
	// ref getters
	virtual Provideable getProvideable();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   RequiredRequestPortImpl
//*******************************************************************
class RequiredRequestPortImpl :
	  virtual public BON::ReferenceImpl
	, public PortImpl
{
public:

	//
	// connectionEnd getters
	virtual std::set<ReceptacleDelegate>    getInReceptacleDelegateLinks();
	virtual std::set<RecepInvoke>           getOutRecepInvokeLinks();
	virtual std::set<ReceptacleDelegate>    getOutReceptacleDelegateLinks();
	virtual std::set<invoke>                getOutinvokeLinks();
	virtual std::multiset<QoSConnector>     getRecepInvokeDsts();
	virtual std::multiset<BON::ConnectionEnd>         getReceptacleDelegateDsts();
	virtual std::set<ReceptacleDelegate>    getReceptacleDelegateLinks();
	virtual std::multiset<BON::ConnectionEnd>         getReceptacleDelegateSrcs();
	virtual std::multiset<BON::ConnectionEnd>         getinvokeDsts();
	//
	// attribute getters and setters
	virtual bool        ismultiple_connections() ;
	virtual void        setmultiple_connections( bool val);
	//
	// ref getters
	virtual Provideable getProvideable();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   AggregateImpl
//*******************************************************************
class AggregateImpl :
	  virtual public BON::ModelImpl
	, public NoInheritableImpl
{
public:

	//
	// kind and role getters
	virtual std::set<Member>                getMember();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   EnumImpl
//*******************************************************************
class EnumImpl :
	  virtual public BON::ModelImpl
	, public NoInheritableImpl
	, public ConstantTypeImpl
{
public:

	//
	// kind and role getters
	virtual std::set<EnumValue>             getEnumValue();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   FactoryOperationImpl
//*******************************************************************
class FactoryOperationImpl :
	  public HasExceptionsImpl
{
public:


	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   HasOperationsImpl
//*******************************************************************
class HasOperationsImpl :
	  public InheritableImpl
{
public:

	//
	// kind and role getters
	virtual std::set<Aggregate>             getAggregate();
	virtual std::set<Alias>                 getAlias();
	virtual std::set<Collection>            getCollection();
	virtual std::set<Constant>              getConstant();
	virtual std::set<Enum>                  getEnum();
	virtual std::set<Exception>             getException();
	virtual std::set<NoInheritable>         getNoInheritable();
	virtual std::set<OnewayOperation>       getOnewayOperation();
	virtual std::set<SwitchedAggregate>     getSwitchedAggregate();
	virtual std::set<TwowayOperation>       getTwowayOperation();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   LookupOperationImpl
//*******************************************************************
class LookupOperationImpl :
	  public HasExceptionsImpl
{
public:


	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   SwitchedAggregateImpl
//*******************************************************************
class SwitchedAggregateImpl :
	  virtual public BON::ModelImpl
	, public NoInheritableImpl
{
public:

	//
	// kind and role getters
	virtual std::set<Discriminator>         getDiscriminator();
	virtual std::set<Label>                 getLabel();
	virtual std::set<LabelConnection>       getLabelConnection();
	virtual std::set<Member>                getMember();

	///BUP
	// add your own members here
	///EUP
};

} // namespace PICML

///BUP
// add your additional class definitions here
///EUP

#endif // PICMLBONX_H
