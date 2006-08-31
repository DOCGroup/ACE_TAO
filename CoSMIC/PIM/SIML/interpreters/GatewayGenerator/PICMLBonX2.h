#ifndef PICMLBONX2_H
#define PICMLBONX2_H

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
//   C  L  A  S  S   ComponentImplementationImpl
//*******************************************************************
class ComponentImplementationImpl :
	  virtual public BON::FCOImpl
{
public:

	//
	// connectionEnd getters
	virtual std::multiset<Property>         getConfigPropertyDsts();
	virtual std::multiset<Capability>       getImplementationCapabilityDsts();
	virtual std::multiset<ImplementationDependency>   getImplementationDependsOnDsts();
	virtual std::multiset<ComponentRef>     getImplementsDsts();
	virtual std::multiset<Property>         getInfoPropertyDsts();
	virtual std::set<ConfigProperty>        getOutConfigPropertyLinks();
	virtual std::set<ImplementationCapability>        getOutImplementationCapabilityLinks();
	virtual std::set<ImplementationDependsOn>         getOutImplementationDependsOnLinks();
	virtual std::set<Implements>            getOutImplementsLinks();
	virtual std::set<InfoProperty>          getOutInfoPropertyLinks();
	//
	// attribute getters and setters
	virtual std::string getUUID() ;
	virtual std::string getlabel() ;
	virtual void        setUUID( const std::string& val);
	virtual void        setlabel( const std::string& val);

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   ComponentAssemblyImpl
//*******************************************************************
class ComponentAssemblyImpl :
	  virtual public BON::ModelImpl
	, public ComponentImplementationImpl
{
public:

	//
	// connectionEnd getters
	virtual std::multiset<PathReference>    getCriticalPathDsts();
	virtual std::set<CriticalPath>          getOutCriticalPathLinks();
	//
	// kind and role getters
	virtual std::set<AssemblyConfigProperty>          getAssemblyConfigProperty();
	virtual std::set<AssemblyDeployRequirement>       getAssemblyDeployRequirement();
	virtual std::set<AssemblyselectRequirement>       getAssemblyselectRequirement();
	virtual std::set<AttributeDelegate>     getAttributeDelegate();
	virtual std::set<AttributeMapping>      getAttributeMapping();
	virtual std::set<AttributeMappingDelegate>        getAttributeMappingDelegate();
	virtual std::set<AttributeMappingValue> getAttributeMappingValue();
	virtual std::set<AttributeValue>        getAttributeValue();
	virtual std::set<Component>             getComponent();
	virtual std::set<ComponentAssembly>     getComponentAssembly();
	virtual std::set<ComponentAssemblyReference>      getComponentAssemblyReference();
	virtual std::set<ComponentPackage>      getComponentPackage();
	virtual std::set<ComponentPackageReference>       getComponentPackageReference();
	virtual std::set<ComponentRef>          getComponentRef();
	virtual std::set<EventSinkDelegate>     getEventSinkDelegate();
	virtual std::set<EventSinkPublish>      getEventSinkPublish();
	virtual std::set<EventSourceDelegate>   getEventSourceDelegate();
	virtual std::set<EventSourcePublish>    getEventSourcePublish();
	virtual std::set<ExternalDelegate>      getExternalDelegate();
	virtual std::set<ExternalReferenceEndPoint>       getExternalReferenceEndPoint();
	virtual std::set<FacetDelegate>         getFacetDelegate();
	virtual std::set<FacetInvoke>           getFacetInvoke();
	virtual std::set<InEventPort>           getInEventPort();
	virtual std::set<OutEventPort>          getOutEventPort();
	virtual std::set<PortQoS>               getPortQoS();
	virtual std::set<Property>              getProperty();
	virtual std::set<ProvidedRequestPort>   getProvidedRequestPort();
	virtual std::set<PublishConnector>      getPublishConnector();
	virtual std::set<QoSCharRef>            getQoSCharRef();
	virtual std::set<QoSConnector>          getQoSConnector();
	virtual std::set<QoSReq>                getQoSReq();
	virtual std::set<RecepInvoke>           getRecepInvoke();
	virtual std::set<ReceptacleDelegate>    getReceptacleDelegate();
	virtual std::set<RequiredRequestPort>   getRequiredRequestPort();
	virtual std::set<Requirement>           getRequirement();
	virtual std::set<deliverTo>             getdeliverTo();
	virtual std::set<emit>                  getemit();
	virtual std::set<invoke>                getinvoke();
	virtual std::set<publish>               getpublish();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   MonolithicImplementationImpl
//*******************************************************************
class MonolithicImplementationImpl :
	  virtual public BON::AtomImpl
	, public ComponentImplementationImpl
{
public:

	//
	// connectionEnd getters
	virtual std::multiset<ImplementationRequirement>  getMonolithDeployRequirementDsts();
	virtual std::multiset<Property>         getMonolithExecParameterDsts();
	virtual std::multiset<ImplementationArtifactReference> getMonolithprimaryArtifactDsts();
	virtual std::set<MonolithDeployRequirement>       getOutMonolithDeployRequirementLinks();
	virtual std::set<MonolithExecParameter> getOutMonolithExecParameterLinks();
	virtual std::set<MonolithprimaryArtifact>         getOutMonolithprimaryArtifactLinks();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   ArtifactContainerImpl
//*******************************************************************
class ArtifactContainerImpl :
	  virtual public BON::ModelImpl
{
public:

	//
	// kind and role getters
	virtual std::set<ArtifactDependency>    getArtifactDependency();
	virtual std::set<ArtifactDependsOn>     getArtifactDependsOn();
	virtual std::set<ArtifactDeployRequirement>       getArtifactDeployRequirement();
	virtual std::set<ArtifactExecParameter> getArtifactExecParameter();
	virtual std::set<ArtifactInfoProperty>  getArtifactInfoProperty();
	virtual std::set<ImplementationArtifact>          getImplementationArtifact();
	virtual std::set<ImplementationArtifactReference> getImplementationArtifactReference();
	virtual std::set<Property>              getProperty();
	virtual std::set<Requirement>           getRequirement();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   BenchmarkAnalysisImpl
//*******************************************************************
class BenchmarkAnalysisImpl :
	  virtual public BON::ModelImpl
{
public:

	//
	// kind and role getters
	virtual std::set<Average>               getAverage();
	virtual std::set<BenchmarkCharacteristics>        getBenchmarkCharacteristics();
	virtual std::set<BenchmarkType>         getBenchmarkType();
	virtual std::set<CompRef>               getCompRef();
	virtual std::set<ComponentOperation>    getComponentOperation();
	virtual std::set<DataAnalysisBase>      getDataAnalysisBase();
	virtual std::set<Event>                 getEvent();
	virtual std::set<EventRef>              getEventRef();
	virtual std::set<FactoryOperation>      getFactoryOperation();
	virtual std::set<FixedIterationBenchmarks>        getFixedIterationBenchmarks();
	virtual std::set<Jitter>                getJitter();
	virtual std::set<Latency>               getLatency();
	virtual std::set<LookupOperation>       getLookupOperation();
	virtual std::set<Maximum>               getMaximum();
	virtual std::set<MetricConnection>      getMetricConnection();
	virtual std::set<MetricsBase>           getMetricsBase();
	virtual std::set<Minimum>               getMinimum();
	virtual std::set<OnewayOperation>       getOnewayOperation();
	virtual std::set<OperationBase>         getOperationBase();
	virtual std::set<OperationRef>          getOperationRef();
	virtual std::set<PeriodicBenchmarks>    getPeriodicBenchmarks();
	virtual std::set<Task>                  getTask();
	virtual std::set<TaskSet>               getTaskSet();
	virtual std::set<Throughput>            getThroughput();
	virtual std::set<TimeProbe>             getTimeProbe();
	virtual std::set<TimerConnection>       getTimerConnection();
	virtual std::set<TimerEventSinkConn>    getTimerEventSinkConn();
	virtual std::set<TriggerBenchmarks>     getTriggerBenchmarks();
	virtual std::set<TwowayOperation>       getTwowayOperation();
	virtual std::set<WorkLoadOperationConnection>     getWorkLoadOperationConnection();
	virtual std::set<WorkloadCharacteristics>         getWorkloadCharacteristics();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   ComponentContainerImpl
//*******************************************************************
class ComponentContainerImpl :
	  virtual public BON::ModelImpl
{
public:

	//
	// kind and role getters
	virtual std::set<ComponentConfigProperty>         getComponentConfigProperty();
	virtual std::set<ComponentInfoProperty> getComponentInfoProperty();
	virtual std::set<ComponentProperty>     getComponentProperty();
	virtual std::set<ComponentPropertyDescription>    getComponentPropertyDescription();
	virtual std::set<ComponentRef>          getComponentRef();
	virtual std::set<Property>              getProperty();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   ComponentImplementationContainerImpl
//*******************************************************************
class ComponentImplementationContainerImpl :
	  virtual public BON::ModelImpl
{
public:

	//
	// kind and role getters
	virtual std::set<Capability>            getCapability();
	virtual std::set<ComponentAssembly>     getComponentAssembly();
	virtual std::set<ComponentImplementation>         getComponentImplementation();
	virtual std::set<ComponentRef>          getComponentRef();
	virtual std::set<ConfigProperty>        getConfigProperty();
	virtual std::set<CriticalPath>          getCriticalPath();
	virtual std::set<ImplementationArtifactReference> getImplementationArtifactReference();
	virtual std::set<ImplementationCapability>        getImplementationCapability();
	virtual std::set<ImplementationDependency>        getImplementationDependency();
	virtual std::set<ImplementationDependsOn>         getImplementationDependsOn();
	virtual std::set<ImplementationRequirement>       getImplementationRequirement();
	virtual std::set<Implements>            getImplements();
	virtual std::set<InfoProperty>          getInfoProperty();
	virtual std::set<MonolithDeployRequirement>       getMonolithDeployRequirement();
	virtual std::set<MonolithExecParameter> getMonolithExecParameter();
	virtual std::set<MonolithicImplementation>        getMonolithicImplementation();
	virtual std::set<MonolithprimaryArtifact>         getMonolithprimaryArtifact();
	virtual std::set<PathReference>         getPathReference();
	virtual std::set<Property>              getProperty();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   ComponentPropertyDescriptionImpl
//*******************************************************************
class ComponentPropertyDescriptionImpl :
	  virtual public BON::ModelImpl
{
public:

	//
	// connectionEnd getters
	virtual std::multiset<ComponentRef>     getComponentPropertySrcs();
	virtual std::set<ComponentProperty>     getInComponentPropertyLinks();
	//
	// kind and role getters
	virtual std::set<DataType>              getDataType();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   DeploymentPlanImpl
//*******************************************************************
class DeploymentPlanImpl :
	  virtual public BON::ModelImpl
{
public:

	//
	// attribute getters and setters
	virtual std::string getUUID() ;
	virtual std::string getlabel() ;
	virtual void        setUUID( const std::string& val);
	virtual void        setlabel( const std::string& val);
	//
	// kind and role getters
	virtual std::set<CollocationGroup>      getCollocationGroup();
	virtual std::set<ComponentAssemblyReference>      getComponentAssemblyReference();
	virtual std::set<ComponentRef>          getComponentRef();
	virtual std::set<InstanceMapping>       getInstanceMapping();
	virtual std::set<NodeReference>         getNodeReference();
	virtual std::set<SharedComponentReference>        getSharedComponentReference();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   DomainImpl
//*******************************************************************
class DomainImpl :
	  virtual public BON::ModelImpl
{
public:

	//
	// attribute getters and setters
	virtual std::string getUUID() ;
	virtual std::string getlabel() ;
	virtual void        setUUID( const std::string& val);
	virtual void        setlabel( const std::string& val);
	//
	// kind and role getters
	virtual std::set<Bridge>                getBridge();
	virtual std::set<Bridge2Interconnect>   getBridge2Interconnect();
	virtual std::set<Elements>              getElements();
	virtual std::set<Interconnect>          getInterconnect();
	virtual std::set<Interconnect2Bridge>   getInterconnect2Bridge();
	virtual std::set<Interconnect2Node>     getInterconnect2Node();
	virtual std::set<Node>                  getNode();
	virtual std::set<Node2Interconnect>     getNode2Interconnect();
	virtual std::set<Property>              getProperty();
	virtual std::set<SharedResource>        getSharedResource();
	virtual std::set<Shares>                getShares();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   MPCImpl
//*******************************************************************
class MPCImpl :
	  virtual public BON::ModelImpl
{
public:

	//
	// kind and role getters
	virtual std::set<Project>               getProject();
	virtual std::set<Workspaces>            getWorkspaces();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   PackageConfigurationContainerImpl
//*******************************************************************
class PackageConfigurationContainerImpl :
	  virtual public BON::ModelImpl
{
public:

	//
	// kind and role getters
	virtual std::set<ComponentPackage>      getComponentPackage();
	virtual std::set<ComponentPackageReference>       getComponentPackageReference();
	virtual std::set<PackageConfBasePackage>          getPackageConfBasePackage();
	virtual std::set<PackageConfConfigProperty>       getPackageConfConfigProperty();
	virtual std::set<PackageConfReference>  getPackageConfReference();
	virtual std::set<PackageConfSelectRequirement>    getPackageConfSelectRequirement();
	virtual std::set<PackageConfSpecializedConfig>    getPackageConfSpecializedConfig();
	virtual std::set<PackageConfiguration>  getPackageConfiguration();
	virtual std::set<Property>              getProperty();
	virtual std::set<Requirement>           getRequirement();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   PackageContainerImpl
//*******************************************************************
class PackageContainerImpl :
	  virtual public BON::ModelImpl
{
public:

	//
	// kind and role getters
	virtual std::set<ComponentImplementationReference> getComponentImplementationReference();
	virtual std::set<ComponentPackage>      getComponentPackage();
	virtual std::set<ComponentRef>          getComponentRef();
	virtual std::set<Implementation>        getImplementation();
	virtual std::set<PackageConfigProperty> getPackageConfigProperty();
	virtual std::set<PackageInfoProperty>   getPackageInfoProperty();
	virtual std::set<PackageInterface>      getPackageInterface();
	virtual std::set<Property>              getProperty();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   PathImpl
//*******************************************************************
class PathImpl :
	  virtual public BON::ModelImpl
{
public:

	//
	// connectionEnd getters
	virtual std::set<PathProperty>          getOutPathPropertyLinks();
	virtual std::multiset<Property>         getPathPropertyDsts();
	//
	// kind and role getters
	virtual std::set<ComponentRef>          getComponentRef();
	virtual std::set<ConnectedComponent>    getConnectedComponent();
	virtual std::set<DisplayNode>           getDisplayNode();
	virtual std::set<DstEdge>               getDstEdge();
	virtual std::set<Edge>                  getEdge();
	virtual std::set<EdgeProperty>          getEdgeProperty();
	virtual std::set<GraphVertex>           getGraphVertex();
	virtual std::set<InEventPort>           getInEventPort();
	virtual std::set<OutEventPort>          getOutEventPort();
	virtual std::set<Property>              getProperty();
	virtual std::set<ProvidedRequestPort>   getProvidedRequestPort();
	virtual std::set<RequiredRequestPort>   getRequiredRequestPort();
	virtual std::set<SrcEdge>               getSrcEdge();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   PathsImpl
//*******************************************************************
class PathsImpl :
	  virtual public BON::ModelImpl
{
public:

	//
	// kind and role getters
	virtual std::set<Path>                  getPath();
	virtual std::set<PathProperty>          getPathProperty();
	virtual std::set<Property>              getProperty();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   ProjectImpl
//*******************************************************************
class ProjectImpl :
	  virtual public BON::ModelImpl
{
public:

	//
	// kind and role getters
	virtual std::set<ComponentLib>          getComponentLib();
	virtual std::set<ExecutorProject>       getExecutorProject();
	virtual std::set<ExtResourceConn>       getExtResourceConn();
	virtual std::set<ExternalResources>     getExternalResources();
	virtual std::set<ImplementationArtifact>          getImplementationArtifact();
	virtual std::set<ServantProject>        getServantProject();
	virtual std::set<StubProject>           getStubProject();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   PropertyImpl
//*******************************************************************
class PropertyImpl :
	  virtual public BON::ModelImpl
{
public:

	//
	// connectionEnd getters
	virtual std::multiset<ImplementationArtifact>     getArtifactExecParameterSrcs();
	virtual std::multiset<ImplementationArtifact>     getArtifactInfoPropertySrcs();
	virtual std::multiset<Component>        getAssemblyConfigPropertySrcs();
	virtual std::multiset<BON::ConnectionEnd>         getAttributeMappingValueSrcs();
	virtual std::multiset<BON::ConnectionEnd>         getAttributeValueSrcs();
	virtual std::multiset<ComponentRef>     getComponentConfigPropertySrcs();
	virtual std::multiset<ComponentRef>     getComponentInfoPropertySrcs();
	virtual std::multiset<ComponentImplementation>    getConfigPropertySrcs();
	virtual std::multiset<Edge>             getEdgePropertyDsts();
	virtual std::set<ArtifactExecParameter> getInArtifactExecParameterLinks();
	virtual std::set<ArtifactInfoProperty>  getInArtifactInfoPropertyLinks();
	virtual std::set<AssemblyConfigProperty>          getInAssemblyConfigPropertyLinks();
	virtual std::set<AttributeMappingValue> getInAttributeMappingValueLinks();
	virtual std::set<AttributeValue>        getInAttributeValueLinks();
	virtual std::set<ComponentConfigProperty>         getInComponentConfigPropertyLinks();
	virtual std::set<ComponentInfoProperty> getInComponentInfoPropertyLinks();
	virtual std::set<ConfigProperty>        getInConfigPropertyLinks();
	virtual std::set<InfoProperty>          getInInfoPropertyLinks();
	virtual std::set<MonolithExecParameter> getInMonolithExecParameterLinks();
	virtual std::set<PackageConfConfigProperty>       getInPackageConfConfigPropertyLinks();
	virtual std::set<PackageConfigProperty> getInPackageConfigPropertyLinks();
	virtual std::set<PackageInfoProperty>   getInPackageInfoPropertyLinks();
	virtual std::set<PathProperty>          getInPathPropertyLinks();
	virtual std::multiset<ComponentImplementation>    getInfoPropertySrcs();
	virtual std::multiset<MonolithicImplementation>   getMonolithExecParameterSrcs();
	virtual std::set<EdgeProperty>          getOutEdgePropertyLinks();
	virtual std::multiset<PackageConfiguration>       getPackageConfConfigPropertySrcs();
	virtual std::multiset<ComponentPackage> getPackageConfigPropertySrcs();
	virtual std::multiset<ComponentPackage> getPackageInfoPropertySrcs();
	virtual std::multiset<Path>             getPathPropertySrcs();
	//
	// attribute getters and setters
	virtual std::string getDataValue() ;
	virtual void        setDataValue( const std::string& val);
	//
	// kind and role getters
	virtual std::set<DataType>              getDataType();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   SatisfierPropertyImpl
//*******************************************************************
class SatisfierPropertyImpl :
	  virtual public BON::ModelImpl
{
public:
	typedef enum
	{
		Quantity_SatisfierPropertyKind_Type,
		Capacity_SatisfierPropertyKind_Type,
		Minimum_SatisfierPropertyKind_Type,
		Maximum_SatisfierPropertyKind_Type,
		Selection_SatisfierPropertyKind_Type,
		Attribute_SatisfierPropertyKind_Type
	} SatisfierPropertyKind_Type;

	//
	// attribute getters and setters
	virtual std::string getDataValue() ;
	virtual SatisfierPropertyImpl::SatisfierPropertyKind_Type getSatisfierPropertyKind();
	virtual bool        isdynamic() ;
	virtual void        setDataValue( const std::string& val);
	virtual void        setSatisfierPropertyKind( SatisfierPropertyImpl::SatisfierPropertyKind_Type val);
	virtual void        setdynamic( bool val);
	//
	// kind and role getters
	virtual std::set<DataType>              getDataType();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   TopLevelPackageContainerImpl
//*******************************************************************
class TopLevelPackageContainerImpl :
	  virtual public BON::ModelImpl
{
public:

	//
	// kind and role getters
	virtual std::set<PackageConfigurationReference>   getPackageConfigurationReference();
	virtual std::set<TopLevelPackage>       getTopLevelPackage();
	virtual std::set<package>               getpackage();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   WorkerImpl
//*******************************************************************
class WorkerImpl :
	  virtual public BON::ModelImpl
{
public:

	//
	// attribute getters and setters
	virtual bool        isAbstract() ;
	virtual void        setAbstract( bool val);
	//
	// kind and role getters
	virtual std::set<Action>                getAction();
	virtual std::set<Action>                getAction(int dummy);
	virtual std::set<CompositeAction>       getCompositeAction();
	virtual std::set<OutputAction>          getOutputAction();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   WorkerLibraryImpl
//*******************************************************************
class WorkerLibraryImpl :
	  virtual public BON::ModelImpl
{
public:

	//
	// attribute getters and setters
	virtual std::string getLocation() ;
	virtual void        setLocation( const std::string& val);
	//
	// kind and role getters
	virtual std::set<WorkerFile>            getWorkerFile();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   BenchmarkTypeImpl
//*******************************************************************
class BenchmarkTypeImpl :
	  virtual public BON::FCOImpl
{
public:

	//
	// connectionEnd getters
	virtual std::multiset<MetricsBase>      getBenchmarkCharacteristicsDsts();
	virtual std::set<BenchmarkCharacteristics>        getOutBenchmarkCharacteristicsLinks();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   FixedIterationBenchmarksImpl
//*******************************************************************
class FixedIterationBenchmarksImpl :
	  virtual public BON::AtomImpl
	, public BenchmarkTypeImpl
{
public:

	//
	// attribute getters and setters
	virtual long        getbenchmarkIterations() ;
	virtual void        setbenchmarkIterations( const long val);

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   PeriodicBenchmarksImpl
//*******************************************************************
class PeriodicBenchmarksImpl :
	  virtual public BON::AtomImpl
	, public BenchmarkTypeImpl
{
public:

	//
	// attribute getters and setters
	virtual long        gettimeperiod() ;
	virtual void        settimeperiod( const long val);

	///BUP
	// add your own members here
	///EUP
};

//*******************************************************************
//   C  L  A  S  S   TriggerBenchmarksImpl
//*******************************************************************
class TriggerBenchmarksImpl :
	  virtual public BON::AtomImpl
	, public BenchmarkTypeImpl
{
public:


	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   DataAnalysisBaseImpl
//*******************************************************************
class DataAnalysisBaseImpl :
	  virtual public BON::FCOImpl
{
public:


	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   AverageImpl
//*******************************************************************
class AverageImpl :
	  virtual public BON::AtomImpl
	, public DataAnalysisBaseImpl
{
public:


	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   JitterImpl
//*******************************************************************
class JitterImpl :
	  virtual public BON::AtomImpl
	, public DataAnalysisBaseImpl
{
public:


	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   MaximumImpl
//*******************************************************************
class MaximumImpl :
	  virtual public BON::AtomImpl
	, public DataAnalysisBaseImpl
{
public:


	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   MinimumImpl
//*******************************************************************
class MinimumImpl :
	  virtual public BON::AtomImpl
	, public DataAnalysisBaseImpl
{
public:


	///BUP
	// add your own members here
	///EUP
};


} // namespace PICML

///BUP
// add your additional class definitions here
///EUP

#endif // PICMLBONX2_H
