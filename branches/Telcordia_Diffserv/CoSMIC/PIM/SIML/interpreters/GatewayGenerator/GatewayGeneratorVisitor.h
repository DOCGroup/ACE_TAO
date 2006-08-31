#ifndef GATEWAYGENERATORVISITOR_H
#define GATEWAYGENERATORVISITOR_H

#include "BON.h"
#include "BONImpl.h"
#include "Extensions.h"
#include "PICMLBonX.h"
#include "PICMLBonX1.h"
#include "PICMLBonX2.h"
#include "PICMLBonX3.h"
#include "PICMLBonX4.h"
#include "WSMLBonX.h"
#include "SIMLBonX.h"
#include <string>
#include <exception>
#include <map>
#include <fstream>

using namespace PICML;
using namespace WSML;
using namespace SIML;
using std::string;
using std::ofstream;
using std::map;
using std::set;

class GatewayGeneratorException
  : public std::exception
{};

class GatewayGeneratorVisitor
  : public BON::Visitor
{
private:
  BON::Project& project_;
  string outputFolder_;
  ofstream header_;
  ofstream source_;
  ofstream driver_;
  map<string, Object> interfaces_;
  set<TwowayOperation> operations_;
  map<TwowayOperation, string> port_;
  string targetNamespace_;
  string defName_;
  string serviceName_;
  Component comp_;

  string transform_name (const char* name);
  const char* utf8(char* t, const char* s);
  string idl_scoped_name (const BON::FCO& object);
  string wsdl_scoped_name (const BON::FCO& object);
  string basic_name (const PredefinedType& pdt);
  string get_file_name (const BON::FCO& object);
  void get_namespace(const Module& module);
  void generate_header();
  void generate_driver();
  void generate_source();
  void corba_struct_to_soap_struct (const Aggregate& agg,
                                    const string& soapName,
                                    const string& corbaName,
                                    ofstream& os);
  void soap_seq_to_corba_seq (const Collection& seq,
                              const string& soapName,
                              const string& corbaName,
                              ofstream& os);
  void corba_seq_to_soap_seq (const Collection& seq,
                              const string& soapName,
                              const string& corbaName,
                              ofstream& os);
  void soap_struct_to_corba_struct (const Aggregate& agg,
                                    const string& soapName,
                                    const string& corbaName,
                                    ofstream& os);
public:
  GatewayGeneratorVisitor (BON::Project& project, string outputFolder);
  virtual ~GatewayGeneratorVisitor();
  virtual void visitSystem( const System& object);
  virtual void visitModule( const Module& object );
  virtual void visitPortProxy( const PortProxy& object );
  virtual void visitObject( const Object& object );
  virtual void visitTwowayOperation(const TwowayOperation& object);

  virtual void visitAtomicType( const AtomicType& object ){}
  virtual void visitAttribute( const PICML::Attribute& object ){}
  virtual void visitAverage( const Average& object ){}
  virtual void visitBoolean( const Boolean& object ){}
  virtual void visitByte( const Byte& object ){}
  virtual void visitComponentPackage( const ComponentPackage& object ){}
  virtual void visitDisplayNode( const DisplayNode& object ){}
  virtual void visitEdge( const Edge& object ){}
  virtual void visitEnumValue( const EnumValue& object ){}
  virtual void visitEnvironment( const Environment& object ){}
  virtual void visitExternalReferenceEndPoint( const ExternalReferenceEndPoint& object ){}
  virtual void visitFixedIterationBenchmarks( const FixedIterationBenchmarks& object ){}
  virtual void visitGenericObject( const GenericObject& object ){}
  virtual void visitGenericValue( const GenericValue& object ){}
  virtual void visitGenericValueObject( const GenericValueObject& object ){}
  virtual void visitImplementationArtifact( const ImplementationArtifact& object ){}
  virtual void visitImplementationDependency( const ImplementationDependency& object ){}
  virtual void visitJitter( const Jitter& object ){}
  virtual void visitLabel( const Label& object ){}
  virtual void visitLongInteger( const LongInteger& object ){}
  virtual void visitMaximum( const Maximum& object ){}
  virtual void visitMinimum( const Minimum& object ){}
  virtual void visitMonolithicImplementation( const MonolithicImplementation& object ){}
  virtual void visitPackageConfiguration( const PackageConfiguration& object ){}
  virtual void visitPeriodicBenchmarks( const PeriodicBenchmarks& object ){}
  virtual void visitPrivateFlag( const PrivateFlag& object ){}
  virtual void visitPublishConnector( const PublishConnector& object ){}
  virtual void visitQoSConnector( const QoSConnector& object ){}
  virtual void visitRealNumber( const RealNumber& object ){}
  virtual void visitSOAPAddress( const SOAPAddress& object ){}
  virtual void visitSOAPBinding( const SOAPBinding& object ){}
  virtual void visitSOAPBody( const SOAPBody& object ){}
  virtual void visitSOAPFault( const SOAPFault& object ){}
  virtual void visitSOAPHeaderFault( const SOAPHeaderFault& object ){}
  virtual void visitSOAPOperation( const SOAPOperation& object ){}
  virtual void visitShortInteger( const ShortInteger& object ){}
  virtual void visitState( const State& object ){}
  virtual void visitString( const String& object ){}
  virtual void visitTask( const Task& object ){}
  virtual void visitTimeProbe( const TimeProbe& object ){}
  virtual void visitTopLevelPackage( const TopLevelPackage& object ){}
  virtual void visitTriggerBenchmarks( const TriggerBenchmarks& object ){}
  virtual void visitTypeEncoding( const TypeEncoding& object ){}
  virtual void visitTypeKind( const TypeKind& object ){}
  virtual void visitAction( const Action& object ){}
  virtual void visitActionBase( const ActionBase& object ){}
  virtual void visitAggregate( const Aggregate& object ){}
  virtual void visitArtifactContainer( const ArtifactContainer& object ){}
  virtual void visitAttribute( const WSML::Attribute& object ){}
  virtual void visitBenchmarkAnalysis( const BenchmarkAnalysis& object ){}
  virtual void visitBinding( const Binding& object ){}
  virtual void visitBindingOperation( const BindingOperation& object ){}
  virtual void visitBridge( const Bridge& object ){}
  virtual void visitCapability( const Capability& object ){}
  virtual void visitComponent( const Component& object ){}
  virtual void visitComponentAssembly( const ComponentAssembly& object ){}
  virtual void visitComponentContainer( const ComponentContainer& object ){}
  virtual void visitComponentFactory( const ComponentFactory& object ){}
  virtual void visitComponentImplementationContainer( const ComponentImplementationContainer& object ){}
  virtual void visitComponentPropertyDescription( const ComponentPropertyDescription& object ){}
  virtual void visitCompositeAction( const CompositeAction& object ){}
  virtual void visitConnectedComponent( const ConnectedComponent& object ){}
  virtual void visitDefinitions( const Definitions& object ){}
  virtual void visitDeploymentPlan( const DeploymentPlan& object ){}
  virtual void visitDocumentation( const Documentation& object ){}
  virtual void visitDomain( const Domain& object ){}
  virtual void visitElement( const Element& object ){}
  virtual void visitEnum( const Enum& object ){}
  virtual void visitEvent( const Event& object ){}
  virtual void visitException( const Exception& object ){}
  virtual void visitExecutorProject( const ExecutorProject& object ){}
  virtual void visitFactoryOperation( const FactoryOperation& object ){}
  virtual void visitFault( const Fault& object ){}
  virtual void visitFile( const File& object ){}
  virtual void visitHasExceptions( const HasExceptions& object ){}
  virtual void visitHasOperations( const HasOperations& object ){}
  virtual void visitImplementationRequirement( const ImplementationRequirement& object ){}
  virtual void visitImport( const Import& object ){}
  virtual void visitInheritable( const Inheritable& object ){}
  virtual void visitInput( const PICML::Input& object ){}
  virtual void visitInputAction( const InputAction& object ){}
  virtual void visitInputActionBase( const InputActionBase& object ){}
  virtual void visitInterconnect( const Interconnect& object ){}
  virtual void visitLatency( const Latency& object ){}
  virtual void visitLookupOperation( const LookupOperation& object ){}
  virtual void visitMPC( const MPC& object ){}
  virtual void visitMessage( const Message& object ){}
  virtual void visitMessages( const Messages& object ){}
  virtual void visitMetricsBase( const MetricsBase& object ){}
  virtual void visitNode( const Node& object ){}
  virtual void visitNotificationOperation( const NotificationOperation& object ){}
  virtual void visitObjectByValue( const ObjectByValue& object ){}
  virtual void visitOneWayOperation( const OneWayOperation& object ){}
  virtual void visitOnewayOperation( const OnewayOperation& object ){}
  virtual void visitOperation( const Operation& object ){}
  virtual void visitOperationBase( const OperationBase& object ){}
  virtual void visitOutput( const Output& object ){}
  virtual void visitOutputAction( const OutputAction& object ){}
  virtual void visitPackage( const Package& object ){}
  virtual void visitPackageConfigurationContainer( const PackageConfigurationContainer& object ){}
  virtual void visitPackageContainer( const PackageContainer& object ){}
  virtual void visitPath( const Path& object ){}
  virtual void visitPaths( const Paths& object ){}
  virtual void visitPeriodicAction( const PeriodicAction& object ){}
  virtual void visitPort( const PICML::Port& object ){}
  virtual void visitPortType( const PortType& object ){}
  virtual void visitPrefixable( const Prefixable& object ){}
  virtual void visitProject( const Project& object ){}
  virtual void visitProperty( const Property& object ){}
  virtual void visitReadonlyAttribute( const ReadonlyAttribute& object ){}
  virtual void visitRequestResponseOperation( const RequestResponseOperation& object ){}
  virtual void visitRequirement( const Requirement& object ){}
  virtual void visitRequirementBase( const RequirementBase& object ){}
  virtual void visitRequirementSatisfier( const RequirementSatisfier& object ){}
  virtual void visitResource( const Resource& object ){}
  virtual void visitSOAPHeader( const SOAPHeader& object ){}
  virtual void visitSatisfierProperty( const SatisfierProperty& object ){}
  virtual void visitSchema( const Schema& object ){}
  virtual void visitSchemaBuiltins( const SchemaBuiltins& object ){}
  virtual void visitSchemaImport( const SchemaImport& object ){}
  virtual void visitSchemaReference( const SchemaReference& object ){}
  virtual void visitServantProject( const ServantProject& object ){}
  virtual void visitService( const Service& object ){}
  virtual void visitSharedResource( const SharedResource& object ){}
  virtual void visitSolicitResponseOperation( const SolicitResponseOperation& object ){}
  virtual void visitStubProject( const StubProject& object ){}
  virtual void visitSupportsInterfaces( const SupportsInterfaces& object ){}
  virtual void visitSwitchedAggregate( const SwitchedAggregate& object ){}
  virtual void visitThroughput( const Throughput& object ){}
  virtual void visitTopLevelPackageContainer( const TopLevelPackageContainer& object ){}
  virtual void visitTypes( const Types& object ){}
  virtual void visitValueObject( const ValueObject& object ){}
  virtual void visitWorker( const Worker& object ){}
  virtual void visitWorkerFile( const WorkerFile& object ){}
  virtual void visitWorkerLibrary( const WorkerLibrary& object ){}
  virtual void visitWorkerPackage( const WorkerPackage& object ){}
  virtual void visitArtifactDependency( const ArtifactDependency& object ){}
  virtual void visitArtifactDependsOn( const ArtifactDependsOn& object ){}
  virtual void visitArtifactDeployRequirement( const ArtifactDeployRequirement& object ){}
  virtual void visitArtifactExecParameter( const ArtifactExecParameter& object ){}
  virtual void visitArtifactInfoProperty( const ArtifactInfoProperty& object ){}
  virtual void visitAssemblyConfigProperty( const AssemblyConfigProperty& object ){}
  virtual void visitAssemblyDeployRequirement( const AssemblyDeployRequirement& object ){}
  virtual void visitAssemblyselectRequirement( const AssemblyselectRequirement& object ){}
  virtual void visitAttributeDelegate( const AttributeDelegate& object ){}
  virtual void visitAttributeMappingDelegate( const AttributeMappingDelegate& object ){}
  virtual void visitAttributeMappingValue( const AttributeMappingValue& object ){}
  virtual void visitAttributeValue( const AttributeValue& object ){}
  virtual void visitBenchmarkCharacteristics( const BenchmarkCharacteristics& object ){}
  virtual void visitBindingPortType( const BindingPortType& object ){}
  virtual void visitBindsOperation( const BindsOperation& object ){}
  virtual void visitBodyBinding( const BodyBinding& object ){}
  virtual void visitBodyParts( const BodyParts& object ){}
  virtual void visitBridge2Interconnect( const Bridge2Interconnect& object ){}
  virtual void visitComponentConfigProperty( const ComponentConfigProperty& object ){}
  virtual void visitComponentInfoProperty( const ComponentInfoProperty& object ){}
  virtual void visitComponentOperation( const ComponentOperation& object ){}
  virtual void visitComponentProperty( const ComponentProperty& object ){}
  virtual void visitConfigProperty( const ConfigProperty& object ){}
  virtual void visitCriticalPath( const CriticalPath& object ){}
  virtual void visitDstEdge( const DstEdge& object ){}
  virtual void visitEdgeProperty( const EdgeProperty& object ){}
  virtual void visitEffect( const Effect& object ){}
  virtual void visitEventSinkDelegate( const EventSinkDelegate& object ){}
  virtual void visitEventSinkPublish( const EventSinkPublish& object ){}
  virtual void visitEventSourceDelegate( const EventSourceDelegate& object ){}
  virtual void visitEventSourcePublish( const EventSourcePublish& object ){}
  virtual void visitExtResourceConn( const ExtResourceConn& object ){}
  virtual void visitExternalDelegate( const ExternalDelegate& object ){}
  virtual void visitFacetDelegate( const FacetDelegate& object ){}
  virtual void visitFacetInvoke( const FacetInvoke& object ){}
  virtual void visitFaultMessage( const FaultMessage& object ){}
  virtual void visitFinish( const Finish& object ){}
  virtual void visitHeaderBinding( const HeaderBinding& object ){}
  virtual void visitHeaderMessage( const HeaderMessage& object ){}
  virtual void visitHeaderPart( const HeaderPart& object ){}
  virtual void visitImplementation( const Implementation& object ){}
  virtual void visitImplementationCapability( const ImplementationCapability& object ){}
  virtual void visitImplementationDependsOn( const ImplementationDependsOn& object ){}
  virtual void visitImplements( const Implements& object ){}
  virtual void visitInfoProperty( const InfoProperty& object ){}
  virtual void visitInput( const WSML::Input& object ){}
  virtual void visitInputMessage( const InputMessage& object ){}
  virtual void visitInstanceMapping( const InstanceMapping& object ){}
  virtual void visitInterconnect2Bridge( const Interconnect2Bridge& object ){}
  virtual void visitInterconnect2Node( const Interconnect2Node& object ){}
  virtual void visitLabelConnection( const LabelConnection& object ){}
  virtual void visitMakeMemberPrivate( const MakeMemberPrivate& object ){}
  virtual void visitManagesComponent( const ManagesComponent& object ){}
  virtual void visitMetricConnection( const MetricConnection& object ){}
  virtual void visitMonolithDeployRequirement( const MonolithDeployRequirement& object ){}
  virtual void visitMonolithExecParameter( const MonolithExecParameter& object ){}
  virtual void visitMonolithprimaryArtifact( const MonolithprimaryArtifact& object ){}
  virtual void visitMultiInput( const MultiInput& object ){}
  virtual void visitNode2Interconnect( const Node2Interconnect& object ){}
  virtual void visitOutputMessage( const OutputMessage& object ){}
  virtual void visitPackageConfBasePackage( const PackageConfBasePackage& object ){}
  virtual void visitPackageConfConfigProperty( const PackageConfConfigProperty& object ){}
  virtual void visitPackageConfReference( const PackageConfReference& object ){}
  virtual void visitPackageConfSelectRequirement( const PackageConfSelectRequirement& object ){}
  virtual void visitPackageConfSpecializedConfig( const PackageConfSpecializedConfig& object ){}
  virtual void visitPackageConfigProperty( const PackageConfigProperty& object ){}
  virtual void visitPackageInfoProperty( const PackageInfoProperty& object ){}
  virtual void visitPackageInterface( const PackageInterface& object ){}
  virtual void visitPathProperty( const PathProperty& object ){}
  virtual void visitPortBinding( const PortBinding& object ){}
  virtual void visitPortQoS( const PortQoS& object ){}
  virtual void visitQoSReq( const QoSReq& object ){}
  virtual void visitRecepInvoke( const RecepInvoke& object ){}
  virtual void visitReceptacleDelegate( const ReceptacleDelegate& object ){}
  virtual void visitSOAPFaultExtension( const SOAPFaultExtension& object ){}
  virtual void visitShares( const Shares& object ){}
  virtual void visitSrcEdge( const SrcEdge& object ){}
  virtual void visitTimerConnection( const TimerConnection& object ){}
  virtual void visitTimerEventSinkConn( const TimerEventSinkConn& object ){}
  virtual void visitTransition( const Transition& object ){}
  virtual void visitWorkLoadOperationConnection( const WorkLoadOperationConnection& object ){}
  virtual void visitWorkloadCharacteristics( const WorkloadCharacteristics& object ){}
  virtual void visitdeliverTo( const deliverTo& object ){}
  virtual void visitemit( const emit& object ){}
  virtual void visitinvoke( const invoke& object ){}
  virtual void visitpackage( const package& object ){}
  virtual void visitpublish( const publish& object ){}
  virtual void visitCollocationGroup( const CollocationGroup& object ){}
  virtual void visitTaskSet( const TaskSet& object ){}
  virtual void visitWorkspaces( const Workspaces& object ){}
  virtual void visitAlias( const Alias& object ){}
  virtual void visitAttributeMapping( const AttributeMapping& object ){}
  virtual void visitAttributeMember( const AttributeMember& object ){}
  virtual void visitBindingRef( const BindingRef& object ){}
  virtual void visitBoxed( const Boxed& object ){}
  virtual void visitCollection( const Collection& object ){}
  virtual void visitCompRef( const CompRef& object ){}
  virtual void visitComponentAssemblyReference( const ComponentAssemblyReference& object ){}
  virtual void visitComponentImplementationReference( const ComponentImplementationReference& object ){}
  virtual void visitComponentPackageReference( const ComponentPackageReference& object ){}
  virtual void visitComponentRef( const ComponentRef& object ){}
  virtual void visitConstant( const Constant& object ){}
  virtual void visitDataType( const DataType& object ){}
  virtual void visitDiscriminator( const Discriminator& object ){}
  virtual void visitEventRef( const EventRef& object ){}
  virtual void visitExceptionRef( const ExceptionRef& object ){}
  virtual void visitExternalResources( const ExternalResources& object ){}
  virtual void visitFaultRef( const FaultRef& object ){}
  virtual void visitFileRef( const FileRef& object ){}
  virtual void visitGetException( const GetException& object ){}
  virtual void visitImplementationArtifactReference( const ImplementationArtifactReference& object ){}
  virtual void visitInEventPort( const InEventPort& object ){}
  virtual void visitInParameter( const InParameter& object ){}
  virtual void visitInherits( const Inherits& object ){}
  virtual void visitInoutParameter( const InoutParameter& object ){}
  virtual void visitInputRef( const InputRef& object ){}
  virtual void visitLookupKey( const LookupKey& object ){}
  virtual void visitMember( const Member& object ){}
  virtual void visitMessageRef( const MessageRef& object ){}
  virtual void visitNodeReference( const NodeReference& object ){}
  virtual void visitOperationRef( const WSML::OperationRef& object ){}
  virtual void visitOperationRef( const PICML::OperationRef& object ){}
  virtual void visitOutEventPort( const OutEventPort& object ){}
  virtual void visitOutParameter( const OutParameter& object ){}
  virtual void visitOutputRef( const OutputRef& object ){}
  virtual void visitPackageConfigurationReference( const PackageConfigurationReference& object ){}
  virtual void visitPart( const Part& object ){}
  virtual void visitPartRef( const PartRef& object ){}
  virtual void visitPathReference( const PathReference& object ){}
  virtual void visitProvidedRequestPort( const ProvidedRequestPort& object ){}
  virtual void visitQoSCharRef( const QoSCharRef& object ){}
  virtual void visitRequiredRequestPort( const RequiredRequestPort& object ){}
  virtual void visitReturnType( const ReturnType& object ){}
  virtual void visitSetException( const SetException& object ){}
  virtual void visitSharedComponentReference( const SharedComponentReference& object ){}
  virtual void visitSupports( const Supports& object ){}
  virtual void visitVariable( const Variable& object ){}
  virtual void visitWorkerType( const WorkerType& object ){}
  virtual void visitBenchmarkType( const BenchmarkType& object ){}
  virtual void visitCommonPortAttrs( const CommonPortAttrs& object ){}
  virtual void visitComponentImplementation( const ComponentImplementation& object ){}
  virtual void visitComponentLib( const ComponentLib& object ){}
  virtual void visitConstantType( const ConstantType& object ){}
  virtual void visitDataAnalysisBase( const DataAnalysisBase& object ){}
  virtual void visitElements( const Elements& object ){}
  virtual void visitExtensibilityElement( const ExtensibilityElement& object ){}
  virtual void visitExtensibleBinding( const ExtensibleBinding& object ){}
  virtual void visitExtensibleMessage( const ExtensibleMessage& object ){}
  virtual void visitExtensibleOperation( const ExtensibleOperation& object ){}
  virtual void visitExtensiblePort( const ExtensiblePort& object ){}
  virtual void visitGraphVertex( const GraphVertex& object ){}
  virtual void visitIORef( const IORef& object ){}
  virtual void visitManageable( const Manageable& object ){}
  virtual void visitMemberType( const MemberType& object ){}
  virtual void visitMultiInputBase( const MultiInputBase& object ){}
  virtual void visitNamedType( const NamedType& object ){}
  virtual void visitNoInheritable( const NoInheritable& object ){}
  virtual void visitPort( const WSML::Port& object ){}
  virtual void visitPredefinedType( const PredefinedType& object ){}
  virtual void visitProvideable( const Provideable& object ){}
  virtual void visitQoSCharacteristic( const QoSCharacteristic& object ){}
  virtual void visitReferrable( const Referrable& object ){}
  virtual void visitReqQoSBase( const ReqQoSBase& object ){}
  virtual void visitSOAPHeaders( const SOAPHeaders& object ){}
  virtual void visitTaggable( const Taggable& object ){}
  virtual void visitComponentAnalyses( const ComponentAnalyses& object ){}
  virtual void visitComponentBuild( const ComponentBuild& object ){}
  virtual void visitComponentImplementations( const ComponentImplementations& object ){}
  virtual void visitComponentPackages( const ComponentPackages& object ){}
  virtual void visitComponentTypes( const ComponentTypes& object ){}
  virtual void visitDeploymentPlans( const DeploymentPlans& object ){}
  virtual void visitImplementationArtifacts( const ImplementationArtifacts& object ){}
  virtual void visitInterfaceDefinitions( const InterfaceDefinitions& object ){}
  virtual void visitPackageConfigurations( const PackageConfigurations& object ){}
  virtual void visitPathDiagrams( const PathDiagrams& object ){}
  virtual void visitPredefinedTypes( const PredefinedTypes& object ){}
  virtual void visitTargets( const Targets& object ){}
  virtual void visitTopLevelPackages( const TopLevelPackages& object ){}
  virtual void visitWorkerLibraries( const WorkerLibraries& object ){}
};

#endif // GATEWAYGENERATORVISITOR_H
