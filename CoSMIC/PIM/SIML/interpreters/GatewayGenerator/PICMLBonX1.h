#ifndef PICMLBONX1_H
#define PICMLBONX1_H

#include "BON.h"
#include "BONImpl.h"
#include "Extensions.h"

///BUP
class GatewayGeneratorVisitor;
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
//   C  L  A  S  S   TwowayOperationImpl
//*******************************************************************
class TwowayOperationImpl :
    public HasExceptionsImpl
{
public:

  //
  // kind and role getters
  virtual std::set<InoutParameter>        getInoutParameter();
  virtual std::set<OutParameter>          getOutParameter();
  virtual std::set<ReturnType>            getReturnType();

  ///BUP
  virtual void accept (GatewayGeneratorVisitor* visitor);
  ///EUP
};


//*******************************************************************
//   C  L  A  S  S   AliasImpl
//*******************************************************************
class AliasImpl :
    virtual public BON::ReferenceImpl
    , public NoInheritableImpl
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
//   C  L  A  S  S   CollectionImpl
//*******************************************************************
class CollectionImpl :
    virtual public BON::ReferenceImpl
    , public NoInheritableImpl
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
//   C  L  A  S  S   ComponentFactoryImpl
//*******************************************************************
class ComponentFactoryImpl :
    public SupportsInterfacesImpl
    , public HasOperationsImpl
{
public:

  //
  // connectionEnd getters
  virtual std::multiset<Manageable>       getManagesComponentDsts();
  virtual std::set<ManagesComponent>      getOutManagesComponentLinks();
  //
  // kind and role getters
  virtual std::set<FactoryOperation>      getFactoryOperation();
  virtual std::set<LookupKey>             getLookupKey();
  virtual std::set<LookupOperation>       getLookupOperation();

  ///BUP
  // add your own members here
  ///EUP
};


//*******************************************************************
//   C  L  A  S  S   ObjectImpl
//*******************************************************************
class ObjectImpl :
    public PrefixableImpl
    , public HasOperationsImpl
    , public ProvideableImpl
{
public:

  //
  // attribute getters and setters
  virtual bool        isabstract() ;
  virtual bool        islocal() ;
  virtual void        setabstract( bool val);
  virtual void        setlocal( bool val);

  ///BUP
  virtual void accept (GatewayGeneratorVisitor* visitor);

  ///EUP
};


//*******************************************************************
//   C  L  A  S  S   ObjectByValueImpl
//*******************************************************************
class ObjectByValueImpl :
    public PrefixableImpl
    , public SupportsInterfacesImpl
    , public HasOperationsImpl
{
public:

  //
  // attribute getters and setters
  virtual bool        isabstract() ;
  virtual void        setabstract( bool val);
  //
  // kind and role getters
  virtual std::set<FactoryOperation>      getFactoryOperation();
  virtual std::set<MakeMemberPrivate>     getMakeMemberPrivate();
  virtual std::set<Member>                getMember();
  virtual std::set<PrivateFlag>           getPrivateFlag();

  ///BUP
  // add your own members here
  ///EUP
};


//*******************************************************************
//   C  L  A  S  S   EventImpl
//*******************************************************************
class EventImpl :
    public ObjectByValueImpl
{
public:


  ///BUP
  // add your own members here
  ///EUP
};


//*******************************************************************
//   C  L  A  S  S   ValueObjectImpl
//*******************************************************************
class ValueObjectImpl :
    public ObjectByValueImpl
{
public:


  ///BUP
  // add your own members here
  ///EUP
};


//*******************************************************************
//   C  L  A  S  S   RequirementSatisfierImpl
//*******************************************************************
class RequirementSatisfierImpl :
    virtual public BON::ModelImpl
{
public:

  //
  // attribute getters and setters
  virtual std::string getresourceType() ;
  virtual void        setresourceType( const std::string& val);
  //
  // kind and role getters
  virtual std::set<SatisfierProperty>     getSatisfierProperty();

  ///BUP
  // add your own members here
  ///EUP
};


//*******************************************************************
//   C  L  A  S  S   ElementsImpl
//*******************************************************************
class ElementsImpl :
    virtual public BON::FCOImpl
{
public:


  ///BUP
  // add your own members here
  ///EUP
};


//*******************************************************************
//   C  L  A  S  S   BridgeImpl
//*******************************************************************
class BridgeImpl :
    virtual public BON::ModelImpl
    , public ElementsImpl
{
public:

  //
  // connectionEnd getters
  virtual std::multiset<Interconnect>     getBridge2InterconnectDsts();
  virtual std::set<Interconnect2Bridge>   getInInterconnect2BridgeLinks();
  virtual std::multiset<Interconnect>     getInterconnect2BridgeSrcs();
  virtual std::set<Bridge2Interconnect>   getOutBridge2InterconnectLinks();
  //
  // attribute getters and setters
  virtual std::string getlabel() ;
  virtual void        setlabel( const std::string& val);
  //
  // kind and role getters
  virtual std::set<Resource>              getResource();

  ///BUP
  // add your own members here
  ///EUP
};


//*******************************************************************
//   C  L  A  S  S   CapabilityImpl
//*******************************************************************
class CapabilityImpl :
    public RequirementSatisfierImpl
{
public:

  //
  // connectionEnd getters
  virtual std::multiset<ComponentImplementation>    getImplementationCapabilitySrcs();
  virtual std::set<ImplementationCapability>        getInImplementationCapabilityLinks();

  ///BUP
  // add your own members here
  ///EUP
};


//*******************************************************************
//   C  L  A  S  S   InterconnectImpl
//*******************************************************************
class InterconnectImpl :
    virtual public BON::ModelImpl
    , public ElementsImpl
{
public:

  //
  // connectionEnd getters
  virtual std::multiset<Bridge>           getBridge2InterconnectSrcs();
  virtual std::set<Bridge2Interconnect>   getInBridge2InterconnectLinks();
  virtual std::set<Node2Interconnect>     getInNode2InterconnectLinks();
  virtual std::multiset<Bridge>           getInterconnect2BridgeDsts();
  virtual std::multiset<Node>             getInterconnect2NodeDsts();
  virtual std::multiset<Node>             getNode2InterconnectSrcs();
  virtual std::set<Interconnect2Bridge>   getOutInterconnect2BridgeLinks();
  virtual std::set<Interconnect2Node>     getOutInterconnect2NodeLinks();
  //
  // attribute getters and setters
  virtual std::string getlabel() ;
  virtual void        setlabel( const std::string& val);
  //
  // kind and role getters
  virtual std::set<Resource>              getResource();

  ///BUP
  // add your own members here
  ///EUP
};


//*******************************************************************
//   C  L  A  S  S   NodeImpl
//*******************************************************************
class NodeImpl :
    virtual public BON::ModelImpl
    , public ElementsImpl
{
public:

  //
  // connectionEnd getters
  virtual std::set<Interconnect2Node>     getInInterconnect2NodeLinks();
  virtual std::multiset<Interconnect>     getInterconnect2NodeSrcs();
  virtual std::multiset<Interconnect>     getNode2InterconnectDsts();
  virtual std::set<Node2Interconnect>     getOutNode2InterconnectLinks();
  virtual std::set<Shares>                getOutSharesLinks();
  virtual std::multiset<SharedResource>   getSharesDsts();
  //
  // attribute getters and setters
  virtual std::string getlabel() ;
  virtual void        setlabel( const std::string& val);
  //
  // kind and role getters
  virtual std::set<Resource>              getResource();

  ///BUP
  // add your own members here
  ///EUP
};


//*******************************************************************
//   C  L  A  S  S   ResourceImpl
//*******************************************************************
class ResourceImpl :
    public RequirementSatisfierImpl
{
public:


  ///BUP
  // add your own members here
  ///EUP
};


//*******************************************************************
//   C  L  A  S  S   SharedResourceImpl
//*******************************************************************
class SharedResourceImpl :
    public ElementsImpl
    , public RequirementSatisfierImpl
{
public:

  //
  // connectionEnd getters
  virtual std::set<Shares>                getInSharesLinks();
  virtual std::multiset<Node>             getSharesSrcs();

  ///BUP
  // add your own members here
  ///EUP
};


//*******************************************************************
//   C  L  A  S  S   ActionBaseImpl
//*******************************************************************
class ActionBaseImpl :
    virtual public BON::ModelImpl
{
public:

  //
  // connectionEnd getters
  virtual std::multiset<State>            getEffectDsts();
  virtual std::set<Transition>            getInTransitionLinks();
  virtual std::set<Effect>                getOutEffectLinks();
  virtual std::multiset<State>            getTransitionSrcs();
  //
  // attribute getters and setters
  virtual long        getRepetitions() ;
  virtual bool        isLogAction() ;
  virtual void        setLogAction( bool val);
  virtual void        setRepetitions( const long val);
  //
  // kind and role getters
  virtual std::set<Property>              getProperty();

  ///BUP
  // add your own members here
  ///EUP
};


//*******************************************************************
//   C  L  A  S  S   ActionImpl
//*******************************************************************
class ActionImpl :
    public ActionBaseImpl
{
public:


  ///BUP
  // add your own members here
  ///EUP
};


//*******************************************************************
//   C  L  A  S  S   CompositeActionImpl
//*******************************************************************
class CompositeActionImpl :
    public ActionImpl
{
public:

  //
  // kind and role getters
  virtual std::set<Action>                getAction();
  virtual std::set<Action>                getAction(int dummy);
  virtual std::set<CompositeAction>       getCompositeAction();
  virtual std::set<Effect>                getEffect();
  virtual std::set<Finish>                getFinish();
  virtual std::set<InputAction>           getInputAction();
  virtual std::set<OutputAction>          getOutputAction();
  virtual std::set<State>                 getState();
  virtual std::set<Transition>            getTransition();

  ///BUP
  // add your own members here
  ///EUP
};


//*******************************************************************
//   C  L  A  S  S   OutputActionImpl
//*******************************************************************
class OutputActionImpl :
    public ActionImpl
{
public:


  ///BUP
  // add your own members here
  ///EUP
};


//*******************************************************************
//   C  L  A  S  S   ComponentLibImpl
//*******************************************************************
class ComponentLibImpl :
    virtual public BON::FCOImpl
{
public:

  //
  // connectionEnd getters
  virtual std::multiset<ExternalResources>          getExtResourceConnDsts();
  virtual std::set<ExtResourceConn>       getOutExtResourceConnLinks();
  //
  // attribute getters and setters
  virtual std::string getORBServices() ;
  virtual std::string getlibraryexport() ;
  virtual std::string getsharedname() ;
  virtual void        setORBServices( const std::string& val);
  virtual void        setlibraryexport( const std::string& val);
  virtual void        setsharedname( const std::string& val);

  ///BUP
  // add your own members here
  ///EUP
};


//*******************************************************************
//   C  L  A  S  S   ExecutorProjectImpl
//*******************************************************************
class ExecutorProjectImpl :
    virtual public BON::ModelImpl
    , public ComponentLibImpl
{
public:

  //
  // kind and role getters
  virtual std::set<ImplementationArtifactReference> getImplementationArtifactReference();

  ///BUP
  // add your own members here
  ///EUP
};


//*******************************************************************
//   C  L  A  S  S   ServantProjectImpl
//*******************************************************************
class ServantProjectImpl :
    virtual public BON::ModelImpl
    , public ComponentLibImpl
{
public:

  //
  // kind and role getters
  virtual std::set<FileRef>               getFileRef();
  virtual std::set<ImplementationArtifactReference> getImplementationArtifactReference();

  ///BUP
  // add your own members here
  ///EUP
};


//*******************************************************************
//   C  L  A  S  S   StubProjectImpl
//*******************************************************************
class StubProjectImpl :
    virtual public BON::ModelImpl
    , public ComponentLibImpl
{
public:

  //
  // kind and role getters
  virtual std::set<FileRef>               getFileRef();
  virtual std::set<ImplementationArtifactReference> getImplementationArtifactReference();

  ///BUP
  // add your own members here
  ///EUP
};


//*******************************************************************
//   C  L  A  S  S   RequirementBaseImpl
//*******************************************************************
class RequirementBaseImpl :
    virtual public BON::ModelImpl
{
public:

  //
  // attribute getters and setters
  virtual std::string getresourceType() ;
  virtual void        setresourceType( const std::string& val);
  //
  // kind and role getters
  virtual std::set<Property>              getProperty();

  ///BUP
  // add your own members here
  ///EUP
};


//*******************************************************************
//   C  L  A  S  S   ImplementationRequirementImpl
//*******************************************************************
class ImplementationRequirementImpl :
    public RequirementBaseImpl
{
public:
  typedef enum
    {
      None_ResourceUsageKind_Type,
      InstanceUsesResource_ResourceUsageKind_Type,
      ResourceUsesInstance_ResourceUsageKind_Type,
      PortUsesResource_ResourceUsageKind_Type,
      ResourceUsesPort_ResourceUsageKind_Type
    } ResourceUsageKind_Type;

  //
  // connectionEnd getters
  virtual std::set<MonolithDeployRequirement>       getInMonolithDeployRequirementLinks();
  virtual std::multiset<MonolithicImplementation>   getMonolithDeployRequirementSrcs();
  //
  // attribute getters and setters
  virtual ImplementationRequirementImpl::ResourceUsageKind_Type getResourceUsageKind();
  virtual std::string getcomponentPort() ;
  virtual std::string getresourcePort() ;
  virtual void        setResourceUsageKind( ImplementationRequirementImpl::ResourceUsageKind_Type val);
  virtual void        setcomponentPort( const std::string& val);
  virtual void        setresourcePort( const std::string& val);

  ///BUP
  // add your own members here
  ///EUP
};

//*******************************************************************
//   C  L  A  S  S   RequirementImpl
//*******************************************************************
class RequirementImpl :
    public RequirementBaseImpl
{
public:

  //
  // connectionEnd getters
  virtual std::multiset<ImplementationArtifact>     getArtifactDeployRequirementSrcs();
  virtual std::multiset<PublishConnector> getAssemblyDeployRequirementSrcs();
  virtual std::multiset<Component>        getAssemblyselectRequirementSrcs();
  virtual std::set<ArtifactDeployRequirement>       getInArtifactDeployRequirementLinks();
  virtual std::set<AssemblyDeployRequirement>       getInAssemblyDeployRequirementLinks();
  virtual std::set<AssemblyselectRequirement>       getInAssemblyselectRequirementLinks();
  virtual std::set<PackageConfSelectRequirement>    getInPackageConfSelectRequirementLinks();
  virtual std::multiset<PackageConfiguration>       getPackageConfSelectRequirementSrcs();

  ///BUP
  // add your own members here
  ///EUP
};


//*******************************************************************
//   C  L  A  S  S   InputActionBaseImpl
//*******************************************************************
class InputActionBaseImpl :
    public ActionBaseImpl
{
public:

  //
  // connectionEnd getters
  virtual std::multiset<State>            getFinishSrcs();
  virtual std::set<Finish>                getInFinishLinks();
  virtual std::set<Input>                 getInInputLinks();
  virtual std::set<MultiInput>            getInMultiInputLinks();
  virtual std::multiset<InEventPort>      getInputSrcs();
  virtual std::multiset<MultiInputBase>   getMultiInputSrcs();
  //
  // kind and role getters
  virtual std::set<Property>              getProperty();

private: // hidden members (interface inherited)
  //
  // hidden attribute getters and setters
  virtual long getRepetitions()  { throw std::string("Interface inherited kind. " + getName() + " doesn't have getRepetitions()  method."); }
  virtual bool isLogAction()  { throw std::string("Interface inherited kind. " + getName() + " doesn't have isLogAction()  method."); }
  virtual void setLogAction( bool val) { throw std::string("Interface inherited kind. " + getName() + " doesn't have setLogAction( bool val) method."); }
  virtual void setRepetitions( const long val) { throw std::string("Interface inherited kind. " + getName() + " doesn't have setRepetitions( const long val) method."); }
  //
  // hidden kind and role getters
  //	virtual std::set<Property> getProperty() { throw std::string("Interface inherited kind. " + getName() + " doesn't have getProperty() method."); }

  ///BUP
  // add your own members here
  ///EUP
};


//*******************************************************************
//   C  L  A  S  S   InputActionImpl
//*******************************************************************
class InputActionImpl :
    public InputActionBaseImpl
{
public:


  ///BUP
  // add your own members here
  ///EUP
};


//*******************************************************************
//   C  L  A  S  S   PeriodicActionImpl
//*******************************************************************
class PeriodicActionImpl :
    public InputActionBaseImpl
{
public:

  //
  // attribute getters and setters
  virtual long        getPeriod() ;
  virtual double      getProbability() ;
  virtual void        setPeriod( const long val);
  virtual void        setProbability( const double val);

  ///BUP
  // add your own members here
  ///EUP
};


//*******************************************************************
//   C  L  A  S  S   MetricsBaseImpl
//*******************************************************************
class MetricsBaseImpl :
    virtual public BON::ModelImpl
{
public:
  typedef enum
    {
      seconds_resolution_Type,
      miliseconds_resolution_Type,
      microseconds_resolution_Type
    } resolution_Type;

  //
  // connectionEnd getters
  virtual std::multiset<BenchmarkType>    getBenchmarkCharacteristicsSrcs();
  virtual std::set<BenchmarkCharacteristics>        getInBenchmarkCharacteristicsLinks();
  virtual std::set<MetricConnection>      getInMetricConnectionLinks();
  virtual std::multiset<OperationRef>     getMetricConnectionSrcs();
  virtual std::set<WorkloadCharacteristics>         getOutWorkloadCharacteristicsLinks();
  virtual std::multiset<TaskSet>          getWorkloadCharacteristicsDsts();
  //
  // attribute getters and setters
  virtual std::string getfileName() ;
  virtual long        getiterations() ;
  virtual long        getpriority() ;
  virtual long        getrate() ;
  virtual MetricsBaseImpl::resolution_Type          getresolution();
  virtual long        getwarmup() ;
  virtual void        setfileName( const std::string& val);
  virtual void        setiterations( const long val);
  virtual void        setpriority( const long val);
  virtual void        setrate( const long val);
  virtual void        setresolution( MetricsBaseImpl::resolution_Type val);
  virtual void        setwarmup( const long val);
  //
  // kind and role getters
  virtual std::set<Average>               getAverage();
  virtual std::set<DataAnalysisBase>      getDataAnalysisBase();
  virtual std::set<Jitter>                getJitter();
  virtual std::set<Maximum>               getMaximum();
  virtual std::set<Minimum>               getMinimum();

  ///BUP
  // add your own members here
  ///EUP
};


//*******************************************************************
//   C  L  A  S  S   LatencyImpl
//*******************************************************************
class LatencyImpl :
    public MetricsBaseImpl
{
public:


  ///BUP
  // add your own members here
  ///EUP
};


//*******************************************************************
//   C  L  A  S  S   ThroughputImpl
//*******************************************************************
class ThroughputImpl :
    public MetricsBaseImpl
{
public:


  ///BUP
  // add your own members here
  ///EUP
};


//*******************************************************************
//   C  L  A  S  S   WorkerPackageImpl
//*******************************************************************
class WorkerPackageImpl :
    virtual public BON::ModelImpl
{
public:

  //
  // kind and role getters
  virtual std::set<Worker>                getWorker();
  virtual std::set<WorkerPackage>         getWorkerPackage();

  ///BUP
  // add your own members here
  ///EUP
};


//*******************************************************************
//   C  L  A  S  S   WorkerFileImpl
//*******************************************************************
class WorkerFileImpl :
    virtual public BON::ModelImpl
    , private WorkerPackageImpl
{
public:

  //
  // attribute getters and setters
  virtual std::string getLocation() ;
  virtual void        setLocation( const std::string& val);

public: // exposed members (implementation inherited)
  //
  // exposed kind and role getters
  using WorkerPackageImpl::getWorker;
  using WorkerPackageImpl::getWorkerPackage;

  ///BUP
  // add your own members here
  ///EUP
};

} // namespace PICML

///BUP
// add your additional class definitions here
///EUP

#endif // PICMLBONX1_H
