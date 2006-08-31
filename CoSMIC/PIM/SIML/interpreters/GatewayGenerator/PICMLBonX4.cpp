#include "stdafx.h"
#include "PICMLBonX.h"
#include "PICMLBonX1.h"
#include "PICMLBonX2.h"
#include "PICMLBonX3.h"
#include "PICMLBonX4.h"

namespace BON
{

IMPLEMENT_BONEXTENSION( PICML::MonolithExecParameter, "PICML::MonolithExecParameter" );
IMPLEMENT_BONEXTENSION( PICML::MonolithprimaryArtifact, "PICML::MonolithprimaryArtifact" );
IMPLEMENT_BONEXTENSION( PICML::MultiInput, "PICML::MultiInput" );
IMPLEMENT_BONEXTENSION( PICML::Node2Interconnect, "PICML::Node2Interconnect" );
IMPLEMENT_BONEXTENSION( PICML::PackageConfBasePackage, "PICML::PackageConfBasePackage" );
IMPLEMENT_BONEXTENSION( PICML::PackageConfConfigProperty, "PICML::PackageConfConfigProperty" );
IMPLEMENT_BONEXTENSION( PICML::PackageConfReference, "PICML::PackageConfReference" );
IMPLEMENT_BONEXTENSION( PICML::PackageConfSelectRequirement, "PICML::PackageConfSelectRequirement" );
IMPLEMENT_BONEXTENSION( PICML::PackageConfSpecializedConfig, "PICML::PackageConfSpecializedConfig" );
IMPLEMENT_BONEXTENSION( PICML::PackageConfigProperty, "PICML::PackageConfigProperty" );
IMPLEMENT_BONEXTENSION( PICML::PackageInfoProperty, "PICML::PackageInfoProperty" );
IMPLEMENT_BONEXTENSION( PICML::PackageInterface, "PICML::PackageInterface" );
IMPLEMENT_BONEXTENSION( PICML::PathProperty, "PICML::PathProperty" );
IMPLEMENT_BONEXTENSION( PICML::PortQoS, "PICML::PortQoS" );
IMPLEMENT_BONEXTENSION( PICML::QoSReq, "PICML::QoSReq" );
IMPLEMENT_BONEXTENSION( PICML::RecepInvoke, "PICML::RecepInvoke" );
IMPLEMENT_BONEXTENSION( PICML::ReceptacleDelegate, "PICML::ReceptacleDelegate" );
IMPLEMENT_BONEXTENSION( PICML::Shares, "PICML::Shares" );
IMPLEMENT_BONEXTENSION( PICML::SrcEdge, "PICML::SrcEdge" );
IMPLEMENT_BONEXTENSION( PICML::TimerConnection, "PICML::TimerConnection" );
IMPLEMENT_BONEXTENSION( PICML::TimerEventSinkConn, "PICML::TimerEventSinkConn" );
IMPLEMENT_BONEXTENSION( PICML::Transition, "PICML::Transition" );
IMPLEMENT_BONEXTENSION( PICML::WorkLoadOperationConnection, "PICML::WorkLoadOperationConnection" );
IMPLEMENT_BONEXTENSION( PICML::WorkloadCharacteristics, "PICML::WorkloadCharacteristics" );
IMPLEMENT_BONEXTENSION( PICML::deliverTo, "PICML::deliverTo" );
IMPLEMENT_BONEXTENSION( PICML::emit, "PICML::emit" );
IMPLEMENT_BONEXTENSION( PICML::invoke, "PICML::invoke" );
IMPLEMENT_BONEXTENSION( PICML::package, "PICML::package" );
IMPLEMENT_BONEXTENSION( PICML::publish, "PICML::publish" );
IMPLEMENT_BONEXTENSION( PICML::CollocationGroup, "PICML::CollocationGroup" );
IMPLEMENT_BONEXTENSION( PICML::TaskSet, "PICML::TaskSet" );
IMPLEMENT_BONEXTENSION( PICML::Workspaces, "PICML::Workspaces" );
IMPLEMENT_BONEXTENSION( PICML::AttributeMapping, "PICML::AttributeMapping" );
IMPLEMENT_BONEXTENSION( PICML::AttributeMember, "PICML::AttributeMember" );
IMPLEMENT_BONEXTENSION( PICML::CompRef, "PICML::CompRef" );
IMPLEMENT_BONEXTENSION( PICML::ComponentAssemblyReference, "PICML::ComponentAssemblyReference" );
IMPLEMENT_BONEXTENSION( PICML::ComponentImplementationReference, "PICML::ComponentImplementationReference" );
IMPLEMENT_BONEXTENSION( PICML::ComponentPackageReference, "PICML::ComponentPackageReference" );
IMPLEMENT_BONEXTENSION( PICML::DataType, "PICML::DataType" );
IMPLEMENT_BONEXTENSION( PICML::Discriminator, "PICML::Discriminator" );
IMPLEMENT_BONEXTENSION( PICML::EventRef, "PICML::EventRef" );
IMPLEMENT_BONEXTENSION( PICML::ExceptionRef, "PICML::ExceptionRef" );
IMPLEMENT_BONEXTENSION( PICML::ExternalResources, "PICML::ExternalResources" );
IMPLEMENT_BONEXTENSION( PICML::FileRef, "PICML::FileRef" );
IMPLEMENT_BONEXTENSION( PICML::GetException, "PICML::GetException" );
IMPLEMENT_BONEXTENSION( PICML::ImplementationArtifactReference, "PICML::ImplementationArtifactReference" );
IMPLEMENT_BONEXTENSION( PICML::InParameter, "PICML::InParameter" );
IMPLEMENT_BONEXTENSION( PICML::Inherits, "PICML::Inherits" );
IMPLEMENT_BONEXTENSION( PICML::InoutParameter, "PICML::InoutParameter" );
IMPLEMENT_BONEXTENSION( PICML::LookupKey, "PICML::LookupKey" );
IMPLEMENT_BONEXTENSION( PICML::Member, "PICML::Member" );
IMPLEMENT_BONEXTENSION( PICML::NodeReference, "PICML::NodeReference" );
IMPLEMENT_BONEXTENSION( PICML::OperationRef, "PICML::OperationRef" );
IMPLEMENT_BONEXTENSION( PICML::OutParameter, "PICML::OutParameter" );
IMPLEMENT_BONEXTENSION( PICML::PackageConfigurationReference, "PICML::PackageConfigurationReference" );
IMPLEMENT_BONEXTENSION( PICML::PathReference, "PICML::PathReference" );
IMPLEMENT_BONEXTENSION( PICML::ReturnType, "PICML::ReturnType" );
IMPLEMENT_BONEXTENSION( PICML::SetException, "PICML::SetException" );
IMPLEMENT_BONEXTENSION( PICML::SharedComponentReference, "PICML::SharedComponentReference" );
IMPLEMENT_BONEXTENSION( PICML::Supports, "PICML::Supports" );
IMPLEMENT_BONEXTENSION( PICML::Variable, "PICML::Variable" );
IMPLEMENT_BONEXTENSION( PICML::WorkerType, "PICML::WorkerType" );

} // end namespace BON

namespace PICML
{

//********************************************************************************
// getDst() return value is a ConnectionEnd casted to Property
//********************************************************************************
Property MonolithExecParameterImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	Property sp( ce);
	if ( sp)
		return sp;

	Property empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to MonolithicImplementation
//********************************************************************************
MonolithicImplementation MonolithExecParameterImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	MonolithicImplementation sp( ce);
	if ( sp)
		return sp;

	MonolithicImplementation empty;
	return empty;
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to ImplementationArtifactReference
//********************************************************************************
ImplementationArtifactReference MonolithprimaryArtifactImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	ImplementationArtifactReference sp( ce);
	if ( sp)
		return sp;

	ImplementationArtifactReference empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to MonolithicImplementation
//********************************************************************************
MonolithicImplementation MonolithprimaryArtifactImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	MonolithicImplementation sp( ce);
	if ( sp)
		return sp;

	MonolithicImplementation empty;
	return empty;
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to InputActionBase
//********************************************************************************
InputActionBase MultiInputImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	InputActionBase sp( ce);
	if ( sp)
		return sp;

	InputActionBase empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to MultiInputBase
//********************************************************************************
MultiInputBase MultiInputImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	MultiInputBase sp( ce);
	if ( sp)
		return sp;

	MultiInputBase empty;
	return empty;
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to Interconnect
//********************************************************************************
Interconnect Node2InterconnectImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	Interconnect sp( ce);
	if ( sp)
		return sp;

	Interconnect empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to Node
//********************************************************************************
Node Node2InterconnectImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	Node sp( ce);
	if ( sp)
		return sp;

	Node empty;
	return empty;
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to ComponentPackage
//********************************************************************************
ComponentPackage PackageConfBasePackageImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	ComponentPackage sp( ce);
	if ( sp)
		return sp;

	ComponentPackage empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to PackageConfiguration
//********************************************************************************
PackageConfiguration PackageConfBasePackageImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	PackageConfiguration sp( ce);
	if ( sp)
		return sp;

	PackageConfiguration empty;
	return empty;
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to Property
//********************************************************************************
Property PackageConfConfigPropertyImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	Property sp( ce);
	if ( sp)
		return sp;

	Property empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to PackageConfiguration
//********************************************************************************
PackageConfiguration PackageConfConfigPropertyImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	PackageConfiguration sp( ce);
	if ( sp)
		return sp;

	PackageConfiguration empty;
	return empty;
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to ComponentPackageReference
//********************************************************************************
ComponentPackageReference PackageConfReferenceImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	ComponentPackageReference sp( ce);
	if ( sp)
		return sp;

	ComponentPackageReference empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to PackageConfiguration
//********************************************************************************
PackageConfiguration PackageConfReferenceImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	PackageConfiguration sp( ce);
	if ( sp)
		return sp;

	PackageConfiguration empty;
	return empty;
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to Requirement
//********************************************************************************
Requirement PackageConfSelectRequirementImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	Requirement sp( ce);
	if ( sp)
		return sp;

	Requirement empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to PackageConfiguration
//********************************************************************************
PackageConfiguration PackageConfSelectRequirementImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	PackageConfiguration sp( ce);
	if ( sp)
		return sp;

	PackageConfiguration empty;
	return empty;
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to PackageConfigurationReference
//********************************************************************************
PackageConfigurationReference PackageConfSpecializedConfigImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	PackageConfigurationReference sp( ce);
	if ( sp)
		return sp;

	PackageConfigurationReference empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to PackageConfiguration
//********************************************************************************
PackageConfiguration PackageConfSpecializedConfigImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	PackageConfiguration sp( ce);
	if ( sp)
		return sp;

	PackageConfiguration empty;
	return empty;
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to Property
//********************************************************************************
Property PackageConfigPropertyImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	Property sp( ce);
	if ( sp)
		return sp;

	Property empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to ComponentPackage
//********************************************************************************
ComponentPackage PackageConfigPropertyImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	ComponentPackage sp( ce);
	if ( sp)
		return sp;

	ComponentPackage empty;
	return empty;
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to Property
//********************************************************************************
Property PackageInfoPropertyImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	Property sp( ce);
	if ( sp)
		return sp;

	Property empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to ComponentPackage
//********************************************************************************
ComponentPackage PackageInfoPropertyImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	ComponentPackage sp( ce);
	if ( sp)
		return sp;

	ComponentPackage empty;
	return empty;
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to ComponentRef
//********************************************************************************
ComponentRef PackageInterfaceImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	ComponentRef sp( ce);
	if ( sp)
		return sp;

	ComponentRef empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to ComponentPackage
//********************************************************************************
ComponentPackage PackageInterfaceImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	ComponentPackage sp( ce);
	if ( sp)
		return sp;

	ComponentPackage empty;
	return empty;
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to Property
//********************************************************************************
Property PathPropertyImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	Property sp( ce);
	if ( sp)
		return sp;

	Property empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to Path
//********************************************************************************
Path PathPropertyImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	Path sp( ce);
	if ( sp)
		return sp;

	Path empty;
	return empty;
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to ReqQoSBase
//********************************************************************************
ReqQoSBase PortQoSImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	ReqQoSBase sp( ce);
	if ( sp)
		return sp;

	ReqQoSBase empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd
//********************************************************************************
BON::ConnectionEnd PortQoSImpl::getSrc()
{
	return ConnectionImpl::getSrc();
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to ReqQoSBase
//********************************************************************************
ReqQoSBase QoSReqImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	ReqQoSBase sp( ce);
	if ( sp)
		return sp;

	ReqQoSBase empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to QoSConnector
//********************************************************************************
QoSConnector QoSReqImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	QoSConnector sp( ce);
	if ( sp)
		return sp;

	QoSConnector empty;
	return empty;
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to QoSConnector
//********************************************************************************
QoSConnector RecepInvokeImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	QoSConnector sp( ce);
	if ( sp)
		return sp;

	QoSConnector empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd
//********************************************************************************
BON::ConnectionEnd RecepInvokeImpl::getSrc()
{
	return ConnectionImpl::getSrc();
}


//********************************************************************************
// getDst() return value is a ConnectionEnd
//********************************************************************************
BON::ConnectionEnd ReceptacleDelegateImpl::getDst()
{
	return ConnectionImpl::getDst();
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd
//********************************************************************************
BON::ConnectionEnd ReceptacleDelegateImpl::getSrc()
{
	return ConnectionImpl::getSrc();
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to SharedResource
//********************************************************************************
SharedResource SharesImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	SharedResource sp( ce);
	if ( sp)
		return sp;

	SharedResource empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to Node
//********************************************************************************
Node SharesImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	Node sp( ce);
	if ( sp)
		return sp;

	Node empty;
	return empty;
}


//********************************************************************************
// getDst() return value is a ConnectionEnd
//********************************************************************************
BON::ConnectionEnd SrcEdgeImpl::getDst()
{
	return ConnectionImpl::getDst();
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to Edge
//********************************************************************************
Edge SrcEdgeImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	Edge sp( ce);
	if ( sp)
		return sp;

	Edge empty;
	return empty;
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to TimeProbe
//********************************************************************************
TimeProbe TimerConnectionImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	TimeProbe sp( ce);
	if ( sp)
		return sp;

	TimeProbe empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to OperationRef
//********************************************************************************
OperationRef TimerConnectionImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	OperationRef sp( ce);
	if ( sp)
		return sp;

	OperationRef empty;
	return empty;
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to TimeProbe
//********************************************************************************
TimeProbe TimerEventSinkConnImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	TimeProbe sp( ce);
	if ( sp)
		return sp;

	TimeProbe empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to EventRef
//********************************************************************************
EventRef TimerEventSinkConnImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	EventRef sp( ce);
	if ( sp)
		return sp;

	EventRef empty;
	return empty;
}


//********************************************************************************
//
//********************************************************************************
std::string TransitionImpl::getPrecondition()
{
	return FCOImpl::getAttribute("Precondition")->getStringValue();
}


//********************************************************************************
//
//********************************************************************************
void TransitionImpl::setPrecondition( const std::string& val)
{
	FCOImpl::getAttribute("Precondition")->setStringValue( val);
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to ActionBase
//********************************************************************************
ActionBase TransitionImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	ActionBase sp( ce);
	if ( sp)
		return sp;

	ActionBase empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to State
//********************************************************************************
State TransitionImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	State sp( ce);
	if ( sp)
		return sp;

	State empty;
	return empty;
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to Task
//********************************************************************************
Task WorkLoadOperationConnectionImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	Task sp( ce);
	if ( sp)
		return sp;

	Task empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to OperationRef
//********************************************************************************
OperationRef WorkLoadOperationConnectionImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	OperationRef sp( ce);
	if ( sp)
		return sp;

	OperationRef empty;
	return empty;
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to TaskSet
//********************************************************************************
TaskSet WorkloadCharacteristicsImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	TaskSet sp( ce);
	if ( sp)
		return sp;

	TaskSet empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to MetricsBase
//********************************************************************************
MetricsBase WorkloadCharacteristicsImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	MetricsBase sp( ce);
	if ( sp)
		return sp;

	MetricsBase empty;
	return empty;
}


//********************************************************************************
// getDst() return value is a ConnectionEnd
//********************************************************************************
BON::ConnectionEnd deliverToImpl::getDst()
{
	return ConnectionImpl::getDst();
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to PublishConnector
//********************************************************************************
PublishConnector deliverToImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	PublishConnector sp( ce);
	if ( sp)
		return sp;

	PublishConnector empty;
	return empty;
}


//********************************************************************************
// getDst() return value is a ConnectionEnd
//********************************************************************************
BON::ConnectionEnd emitImpl::getDst()
{
	return ConnectionImpl::getDst();
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd
//********************************************************************************
BON::ConnectionEnd emitImpl::getSrc()
{
	return ConnectionImpl::getSrc();
}


//********************************************************************************
// getDst() return value is a ConnectionEnd
//********************************************************************************
BON::ConnectionEnd invokeImpl::getDst()
{
	return ConnectionImpl::getDst();
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd
//********************************************************************************
BON::ConnectionEnd invokeImpl::getSrc()
{
	return ConnectionImpl::getSrc();
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to PackageConfigurationReference
//********************************************************************************
PackageConfigurationReference packageImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	PackageConfigurationReference sp( ce);
	if ( sp)
		return sp;

	PackageConfigurationReference empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to TopLevelPackage
//********************************************************************************
TopLevelPackage packageImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	TopLevelPackage sp( ce);
	if ( sp)
		return sp;

	TopLevelPackage empty;
	return empty;
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to PublishConnector
//********************************************************************************
PublishConnector publishImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	PublishConnector sp( ce);
	if ( sp)
		return sp;

	PublishConnector empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd
//********************************************************************************
BON::ConnectionEnd publishImpl::getSrc()
{
	return ConnectionImpl::getSrc();
}


//********************************************************************************
// returns dst NodeReferences
//********************************************************************************
std::multiset<NodeReference> CollocationGroupImpl::getInstanceMappingDsts()
{
	std::multiset<NodeReference> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("PICML::InstanceMapping");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
		{
			NodeReference dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
//
//********************************************************************************
std::set<InstanceMapping> CollocationGroupImpl::getOutInstanceMappingLinks()
{
	std::set<InstanceMapping> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		InstanceMapping c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// aggregated getter for setmembers
//********************************************************************************
std::set<BON::Reference> CollocationGroupImpl::getAllMembers()
{
	std::set<BON::Reference> res;
	std::set<BON::FCO> elems = BON::SetImpl::getMembers();
	std::set<BON::FCO>::iterator elem = elems.begin();
	for( ; elem != elems.end(); ++elem)
	{
		BON::Reference r( *elem);
		if ( r)
			res.insert( r);
	}
	return res;
}


//********************************************************************************
// specialized getter for ComponentAssemblyReference setmembers
//********************************************************************************
std::set<ComponentAssemblyReference> CollocationGroupImpl::getComponentAssemblyReferences()
{
	std::set<ComponentAssemblyReference> res;
	std::set<BON::FCO> elems = BON::SetImpl::getMembers();
	std::set<BON::FCO>::iterator elem = elems.begin();
	for( ; elem != elems.end(); ++elem)
	{
		ComponentAssemblyReference r( *elem);
		if ( r && r->getObjectMeta().name() == "ComponentAssemblyReference")
			res.insert( r);
	}
	return res;
}


//********************************************************************************
// specialized getter for ComponentRef setmembers
//********************************************************************************
std::set<ComponentRef> CollocationGroupImpl::getComponentRefs()
{
	std::set<ComponentRef> res;
	std::set<BON::FCO> elems = BON::SetImpl::getMembers();
	std::set<BON::FCO>::iterator elem = elems.begin();
	for( ; elem != elems.end(); ++elem)
	{
		ComponentRef r( *elem);
		if ( r && r->getObjectMeta().name() == "ComponentRef")
			res.insert( r);
	}
	return res;
}


//********************************************************************************
// specialized getter for SharedComponentReference setmembers
//********************************************************************************
std::set<SharedComponentReference> CollocationGroupImpl::getSharedComponentReferences()
{
	std::set<SharedComponentReference> res;
	std::set<BON::FCO> elems = BON::SetImpl::getMembers();
	std::set<BON::FCO>::iterator elem = elems.begin();
	for( ; elem != elems.end(); ++elem)
	{
		SharedComponentReference r( *elem);
		if ( r && r->getObjectMeta().name() == "SharedComponentReference")
			res.insert( r);
	}
	return res;
}


//********************************************************************************
//
//********************************************************************************
std::set<WorkloadCharacteristics> TaskSetImpl::getInWorkloadCharacteristicsLinks()
{
	std::set<WorkloadCharacteristics> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		WorkloadCharacteristics c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// returns src MetricsBases
//********************************************************************************
std::multiset<MetricsBase> TaskSetImpl::getWorkloadCharacteristicsSrcs()
{
	std::multiset<MetricsBase> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("PICML::WorkloadCharacteristics");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			MetricsBase dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
//
//********************************************************************************
long TaskSetImpl::getpriority()
{
	return FCOImpl::getAttribute("priority")->getIntegerValue();
}


//********************************************************************************
//
//********************************************************************************
long TaskSetImpl::getrate()
{
	return FCOImpl::getAttribute("rate")->getIntegerValue();
}


//********************************************************************************
//
//********************************************************************************
void TaskSetImpl::setpriority( const long val)
{
	FCOImpl::getAttribute("priority")->setIntegerValue( val);
}


//********************************************************************************
//
//********************************************************************************
void TaskSetImpl::setrate( const long val)
{
	FCOImpl::getAttribute("rate")->setIntegerValue( val);
}


//********************************************************************************
// aggregated getter for setmembers
//********************************************************************************
std::set<Task> TaskSetImpl::getTasks( int dummy)
{
	std::set<Task> res;
	std::set<BON::FCO> elems = BON::SetImpl::getMembers();
	std::set<BON::FCO>::iterator elem = elems.begin();
	for( ; elem != elems.end(); ++elem)
	{
		Task r( *elem);
		if ( r)
			res.insert( r);
	}
	return res;
}


//********************************************************************************
// specialized getter for Task setmembers
//********************************************************************************
std::set<Task> TaskSetImpl::getTasks()
{
	std::set<Task> res;
	std::set<BON::FCO> elems = BON::SetImpl::getMembers();
	std::set<BON::FCO>::iterator elem = elems.begin();
	for( ; elem != elems.end(); ++elem)
	{
		Task r( *elem);
		if ( r && r->getObjectMeta().name() == "Task")
			res.insert( r);
	}
	return res;
}


//********************************************************************************
// aggregated getter for setmembers
//********************************************************************************
std::set<Project> WorkspacesImpl::getProjects( int dummy)
{
	std::set<Project> res;
	std::set<BON::FCO> elems = BON::SetImpl::getMembers();
	std::set<BON::FCO>::iterator elem = elems.begin();
	for( ; elem != elems.end(); ++elem)
	{
		Project r( *elem);
		if ( r)
			res.insert( r);
	}
	return res;
}


//********************************************************************************
// specialized getter for Project setmembers
//********************************************************************************
std::set<Project> WorkspacesImpl::getProjects()
{
	std::set<Project> res;
	std::set<BON::FCO> elems = BON::SetImpl::getMembers();
	std::set<BON::FCO>::iterator elem = elems.begin();
	for( ; elem != elems.end(); ++elem)
	{
		Project r( *elem);
		if ( r && r->getObjectMeta().name() == "Project")
			res.insert( r);
	}
	return res;
}


//********************************************************************************
// returns dst ReadonlyAttributes and referenceports
//********************************************************************************
std::multiset<BON::ConnectionEnd> AttributeMappingImpl::getAttributeDelegateDsts()
{
	return BON::ConnectionEndImpl::getOutConnEnds("PICML::AttributeDelegate");
}


//********************************************************************************
// returns dst AttributeMappings and referenceports
//********************************************************************************
std::multiset<BON::ConnectionEnd> AttributeMappingImpl::getAttributeMappingDelegateDsts()
{
	return BON::ConnectionEndImpl::getOutConnEnds("PICML::AttributeMappingDelegate");
}


//********************************************************************************
//
//********************************************************************************
std::set<AttributeMappingDelegate> AttributeMappingImpl::getAttributeMappingDelegateLinks()
{
	std::set<AttributeMappingDelegate> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		AttributeMappingDelegate c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// returns src AttributeMappings and referenceports
//********************************************************************************
std::multiset<BON::ConnectionEnd> AttributeMappingImpl::getAttributeMappingDelegateSrcs()
{
	return BON::ConnectionEndImpl::getInConnEnds("PICML::AttributeMappingDelegate");
}


//********************************************************************************
// returns dst Propertys
//********************************************************************************
std::multiset<Property> AttributeMappingImpl::getAttributeMappingValueDsts()
{
	std::multiset<Property> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("PICML::AttributeMappingValue");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
		{
			Property dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
//
//********************************************************************************
std::set<AttributeMappingDelegate> AttributeMappingImpl::getInAttributeMappingDelegateLinks()
{
	std::set<AttributeMappingDelegate> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		AttributeMappingDelegate c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
//
//********************************************************************************
std::set<AttributeDelegate> AttributeMappingImpl::getOutAttributeDelegateLinks()
{
	std::set<AttributeDelegate> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		AttributeDelegate c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
//
//********************************************************************************
std::set<AttributeMappingDelegate> AttributeMappingImpl::getOutAttributeMappingDelegateLinks()
{
	std::set<AttributeMappingDelegate> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		AttributeMappingDelegate c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
//
//********************************************************************************
std::set<AttributeMappingValue> AttributeMappingImpl::getOutAttributeMappingValueLinks()
{
	std::set<AttributeMappingValue> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		AttributeMappingValue c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
//
//********************************************************************************
ReadonlyAttribute AttributeMappingImpl::getReadonlyAttribute()
{
	BON::FCO r = BON::ReferenceImpl::getReferred();
	return ReadonlyAttribute(r);
}


//********************************************************************************
//
//********************************************************************************
MemberType AttributeMemberImpl::getMemberType()
{
	BON::FCO r = BON::ReferenceImpl::getReferred();
	return MemberType(r);
}


//********************************************************************************
// returns src OperationRefs
//********************************************************************************
std::multiset<OperationRef> CompRefImpl::getComponentOperationSrcs()
{
	std::multiset<OperationRef> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("PICML::ComponentOperation");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			OperationRef dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
//
//********************************************************************************
std::set<ComponentOperation> CompRefImpl::getInComponentOperationLinks()
{
	std::set<ComponentOperation> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		ComponentOperation c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
//
//********************************************************************************
Component CompRefImpl::getComponent()
{
	BON::FCO r = BON::ReferenceImpl::getReferred();
	return Component(r);
}


//********************************************************************************
//
//********************************************************************************
ComponentAssembly ComponentAssemblyReferenceImpl::getComponentAssembly()
{
	BON::FCO r = BON::ReferenceImpl::getReferred();
	return ComponentAssembly(r);
}


//********************************************************************************
// returns src ComponentPackages
//********************************************************************************
std::multiset<ComponentPackage> ComponentImplementationReferenceImpl::getImplementationSrcs()
{
	std::multiset<ComponentPackage> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("PICML::Implementation");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			ComponentPackage dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
//
//********************************************************************************
std::set<Implementation> ComponentImplementationReferenceImpl::getInImplementationLinks()
{
	std::set<Implementation> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		Implementation c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
//
//********************************************************************************
ComponentImplementation ComponentImplementationReferenceImpl::getComponentImplementation()
{
	BON::FCO r = BON::ReferenceImpl::getReferred();
	return ComponentImplementation(r);
}


//********************************************************************************
//
//********************************************************************************
std::set<PackageConfReference> ComponentPackageReferenceImpl::getInPackageConfReferenceLinks()
{
	std::set<PackageConfReference> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		PackageConfReference c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// returns src PackageConfigurations
//********************************************************************************
std::multiset<PackageConfiguration> ComponentPackageReferenceImpl::getPackageConfReferenceSrcs()
{
	std::multiset<PackageConfiguration> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("PICML::PackageConfReference");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			PackageConfiguration dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
//
//********************************************************************************
std::string ComponentPackageReferenceImpl::getrequiredName()
{
	return FCOImpl::getAttribute("requiredName")->getStringValue();
}


//********************************************************************************
//
//********************************************************************************
std::string ComponentPackageReferenceImpl::getrequiredType()
{
	return FCOImpl::getAttribute("requiredType")->getStringValue();
}


//********************************************************************************
//
//********************************************************************************
std::string ComponentPackageReferenceImpl::getrequiredUUID()
{
	return FCOImpl::getAttribute("requiredUUID")->getStringValue();
}


//********************************************************************************
//
//********************************************************************************
void ComponentPackageReferenceImpl::setrequiredName( const std::string& val)
{
	FCOImpl::getAttribute("requiredName")->setStringValue( val);
}


//********************************************************************************
//
//********************************************************************************
void ComponentPackageReferenceImpl::setrequiredType( const std::string& val)
{
	FCOImpl::getAttribute("requiredType")->setStringValue( val);
}


//********************************************************************************
//
//********************************************************************************
void ComponentPackageReferenceImpl::setrequiredUUID( const std::string& val)
{
	FCOImpl::getAttribute("requiredUUID")->setStringValue( val);
}


//********************************************************************************
//
//********************************************************************************
ComponentPackage ComponentPackageReferenceImpl::getComponentPackage()
{
	BON::FCO r = BON::ReferenceImpl::getReferred();
	return ComponentPackage(r);
}


//********************************************************************************
//
//********************************************************************************
PredefinedType DataTypeImpl::getPredefinedType()
{
	BON::FCO r = BON::ReferenceImpl::getReferred();
	return PredefinedType(r);
}


//********************************************************************************
//
//********************************************************************************
ConstantType DiscriminatorImpl::getConstantType()
{
	BON::FCO r = BON::ReferenceImpl::getReferred();
	return ConstantType(r);
}


//********************************************************************************
//
//********************************************************************************
std::set<TimerEventSinkConn> EventRefImpl::getOutTimerEventSinkConnLinks()
{
	std::set<TimerEventSinkConn> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		TimerEventSinkConn c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// returns dst TimeProbes
//********************************************************************************
std::multiset<TimeProbe> EventRefImpl::getTimerEventSinkConnDsts()
{
	std::multiset<TimeProbe> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("PICML::TimerEventSinkConn");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
		{
			TimeProbe dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
//
//********************************************************************************
Event EventRefImpl::getEvent()
{
	BON::FCO r = BON::ReferenceImpl::getReferred();
	return Event(r);
}


//********************************************************************************
//
//********************************************************************************
Exception ExceptionRefImpl::getException()
{
	BON::FCO r = BON::ReferenceImpl::getReferred();
	return Exception(r);
}


//********************************************************************************
// returns src ComponentLibs
//********************************************************************************
std::multiset<ComponentLib> ExternalResourcesImpl::getExtResourceConnSrcs()
{
	std::multiset<ComponentLib> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("PICML::ExtResourceConn");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			ComponentLib dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
//
//********************************************************************************
std::set<ExtResourceConn> ExternalResourcesImpl::getInExtResourceConnLinks()
{
	std::set<ExtResourceConn> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		ExtResourceConn c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
//
//********************************************************************************
ImplementationArtifact ExternalResourcesImpl::getImplementationArtifact()
{
	BON::FCO r = BON::ReferenceImpl::getReferred();
	return ImplementationArtifact(r);
}


//********************************************************************************
//
//********************************************************************************
File FileRefImpl::getFile()
{
	BON::FCO r = BON::ReferenceImpl::getReferred();
	return File(r);
}


//********************************************************************************
//
//********************************************************************************
Exception GetExceptionImpl::getException()
{
	BON::FCO r = BON::ReferenceImpl::getReferred();
	return Exception(r);
}


//********************************************************************************
// returns src ImplementationArtifacts
//********************************************************************************
std::multiset<ImplementationArtifact> ImplementationArtifactReferenceImpl::getArtifactDependsOnSrcs()
{
	std::multiset<ImplementationArtifact> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("PICML::ArtifactDependsOn");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			ImplementationArtifact dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
//
//********************************************************************************
std::set<ArtifactDependsOn> ImplementationArtifactReferenceImpl::getInArtifactDependsOnLinks()
{
	std::set<ArtifactDependsOn> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		ArtifactDependsOn c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
//
//********************************************************************************
std::set<MonolithprimaryArtifact> ImplementationArtifactReferenceImpl::getInMonolithprimaryArtifactLinks()
{
	std::set<MonolithprimaryArtifact> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		MonolithprimaryArtifact c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// returns src MonolithicImplementations
//********************************************************************************
std::multiset<MonolithicImplementation> ImplementationArtifactReferenceImpl::getMonolithprimaryArtifactSrcs()
{
	std::multiset<MonolithicImplementation> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("PICML::MonolithprimaryArtifact");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			MonolithicImplementation dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
//
//********************************************************************************
ImplementationArtifact ImplementationArtifactReferenceImpl::getImplementationArtifact()
{
	BON::FCO r = BON::ReferenceImpl::getReferred();
	return ImplementationArtifact(r);
}


//********************************************************************************
//
//********************************************************************************
MemberType InParameterImpl::getMemberType()
{
	BON::FCO r = BON::ReferenceImpl::getReferred();
	return MemberType(r);
}


//********************************************************************************
//
//********************************************************************************
Inheritable InheritsImpl::getInheritable()
{
	BON::FCO r = BON::ReferenceImpl::getReferred();
	return Inheritable(r);
}


//********************************************************************************
//
//********************************************************************************
MemberType InoutParameterImpl::getMemberType()
{
	BON::FCO r = BON::ReferenceImpl::getReferred();
	return MemberType(r);
}


//********************************************************************************
//
//********************************************************************************
ValueObject LookupKeyImpl::getValueObject()
{
	BON::FCO r = BON::ReferenceImpl::getReferred();
	return ValueObject(r);
}


//********************************************************************************
// returns dst Labels
//********************************************************************************
std::multiset<Label> MemberImpl::getLabelConnectionDsts()
{
	std::multiset<Label> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("PICML::LabelConnection");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
		{
			Label dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// returns dst PrivateFlags
//********************************************************************************
std::multiset<PrivateFlag> MemberImpl::getMakeMemberPrivateDsts()
{
	std::multiset<PrivateFlag> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("PICML::MakeMemberPrivate");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
		{
			PrivateFlag dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
//
//********************************************************************************
std::set<LabelConnection> MemberImpl::getOutLabelConnectionLinks()
{
	std::set<LabelConnection> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		LabelConnection c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
//
//********************************************************************************
std::set<MakeMemberPrivate> MemberImpl::getOutMakeMemberPrivateLinks()
{
	std::set<MakeMemberPrivate> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		MakeMemberPrivate c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
//
//********************************************************************************
MemberType MemberImpl::getMemberType()
{
	BON::FCO r = BON::ReferenceImpl::getReferred();
	return MemberType(r);
}


//********************************************************************************
//
//********************************************************************************
std::set<InstanceMapping> NodeReferenceImpl::getInInstanceMappingLinks()
{
	std::set<InstanceMapping> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		InstanceMapping c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// returns src CollocationGroups
//********************************************************************************
std::multiset<CollocationGroup> NodeReferenceImpl::getInstanceMappingSrcs()
{
	std::multiset<CollocationGroup> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("PICML::InstanceMapping");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			CollocationGroup dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
//
//********************************************************************************
Node NodeReferenceImpl::getNode()
{
	BON::FCO r = BON::ReferenceImpl::getReferred();
	return Node(r);
}


//********************************************************************************
// returns dst CompRefs
//********************************************************************************
std::multiset<CompRef> OperationRefImpl::getComponentOperationDsts()
{
	std::multiset<CompRef> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("PICML::ComponentOperation");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
		{
			CompRef dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// returns dst MetricsBases
//********************************************************************************
std::multiset<MetricsBase> OperationRefImpl::getMetricConnectionDsts()
{
	std::multiset<MetricsBase> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("PICML::MetricConnection");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
		{
			MetricsBase dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
//
//********************************************************************************
std::set<ComponentOperation> OperationRefImpl::getOutComponentOperationLinks()
{
	std::set<ComponentOperation> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		ComponentOperation c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
//
//********************************************************************************
std::set<MetricConnection> OperationRefImpl::getOutMetricConnectionLinks()
{
	std::set<MetricConnection> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		MetricConnection c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
//
//********************************************************************************
std::set<TimerConnection> OperationRefImpl::getOutTimerConnectionLinks()
{
	std::set<TimerConnection> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		TimerConnection c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
//
//********************************************************************************
std::set<WorkLoadOperationConnection> OperationRefImpl::getOutWorkLoadOperationConnectionLinks()
{
	std::set<WorkLoadOperationConnection> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		WorkLoadOperationConnection c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// returns dst TimeProbes
//********************************************************************************
std::multiset<TimeProbe> OperationRefImpl::getTimerConnectionDsts()
{
	std::multiset<TimeProbe> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("PICML::TimerConnection");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
		{
			TimeProbe dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// returns dst Tasks
//********************************************************************************
std::multiset<Task> OperationRefImpl::getWorkLoadOperationConnectionDsts()
{
	std::multiset<Task> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("PICML::WorkLoadOperationConnection");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
		{
			Task dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
//
//********************************************************************************
OperationBase OperationRefImpl::getOperationBase()
{
	BON::FCO r = BON::ReferenceImpl::getReferred();
	return OperationBase(r);
}


//********************************************************************************
//
//********************************************************************************
MemberType OutParameterImpl::getMemberType()
{
	BON::FCO r = BON::ReferenceImpl::getReferred();
	return MemberType(r);
}


//********************************************************************************
//
//********************************************************************************
std::set<PackageConfSpecializedConfig> PackageConfigurationReferenceImpl::getInPackageConfSpecializedConfigLinks()
{
	std::set<PackageConfSpecializedConfig> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		PackageConfSpecializedConfig c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
//
//********************************************************************************
std::set<package> PackageConfigurationReferenceImpl::getInpackageLinks()
{
	std::set<package> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		package c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// returns src PackageConfigurations
//********************************************************************************
std::multiset<PackageConfiguration> PackageConfigurationReferenceImpl::getPackageConfSpecializedConfigSrcs()
{
	std::multiset<PackageConfiguration> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("PICML::PackageConfSpecializedConfig");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			PackageConfiguration dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// returns src TopLevelPackages
//********************************************************************************
std::multiset<TopLevelPackage> PackageConfigurationReferenceImpl::getpackageSrcs()
{
	std::multiset<TopLevelPackage> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("PICML::package");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			TopLevelPackage dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
//
//********************************************************************************
PackageConfiguration PackageConfigurationReferenceImpl::getPackageConfiguration()
{
	BON::FCO r = BON::ReferenceImpl::getReferred();
	return PackageConfiguration(r);
}


//********************************************************************************
// returns src ComponentAssemblys
//********************************************************************************
std::multiset<ComponentAssembly> PathReferenceImpl::getCriticalPathSrcs()
{
	std::multiset<ComponentAssembly> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("PICML::CriticalPath");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			ComponentAssembly dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
//
//********************************************************************************
std::set<CriticalPath> PathReferenceImpl::getInCriticalPathLinks()
{
	std::set<CriticalPath> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		CriticalPath c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
//
//********************************************************************************
Path PathReferenceImpl::getPath()
{
	BON::FCO r = BON::ReferenceImpl::getReferred();
	return Path(r);
}


//********************************************************************************
//
//********************************************************************************
MemberType ReturnTypeImpl::getMemberType()
{
	BON::FCO r = BON::ReferenceImpl::getReferred();
	return MemberType(r);
}


//********************************************************************************
//
//********************************************************************************
Exception SetExceptionImpl::getException()
{
	BON::FCO r = BON::ReferenceImpl::getReferred();
	return Exception(r);
}


//********************************************************************************
//
//********************************************************************************
ComponentRef SharedComponentReferenceImpl::getComponentRef()
{
	BON::FCO r = BON::ReferenceImpl::getReferred();
	return ComponentRef(r);
}


//********************************************************************************
//
//********************************************************************************
Object SupportsImpl::getObject()
{
	BON::FCO r = BON::ReferenceImpl::getReferred();
	return Object(r);
}


//********************************************************************************
//
//********************************************************************************
PredefinedType VariableImpl::getPredefinedType()
{
	BON::FCO r = BON::ReferenceImpl::getReferred();
	return PredefinedType(r);
}


//********************************************************************************
//
//********************************************************************************
Worker WorkerTypeImpl::getWorker()
{
	BON::FCO r = BON::ReferenceImpl::getReferred();
	return Worker(r);
}

} // namespace PICML
