#include "stdafx.h"
#include "PICMLBonX.h"
#include "PICMLBonX1.h"
#include "PICMLBonX2.h"
#include "PICMLBonX3.h"
#include "PICMLBonX4.h"
#include "GatewayGeneratorVisitor.h"

namespace BON
{

IMPLEMENT_BONEXTENSION( PICML::TwowayOperation, "PICML::TwowayOperation" );
IMPLEMENT_BONEXTENSION( PICML::Alias, "PICML::Alias" );
IMPLEMENT_BONEXTENSION( PICML::Collection, "PICML::Collection" );
IMPLEMENT_BONEXTENSION( PICML::ComponentFactory, "PICML::ComponentFactory" );
IMPLEMENT_BONEXTENSION( PICML::Object, "PICML::Object" );
IMPLEMENT_ABSTRACT_BONEXTENSION( PICML::ObjectByValue );
IMPLEMENT_BONEXTENSION( PICML::Event, "PICML::Event" );
IMPLEMENT_BONEXTENSION( PICML::ValueObject, "PICML::ValueObject" );
IMPLEMENT_ABSTRACT_BONEXTENSION( PICML::RequirementSatisfier );
IMPLEMENT_ABSTRACT_BONEXTENSION( PICML::Elements );
IMPLEMENT_BONEXTENSION( PICML::Bridge, "PICML::Bridge" );
IMPLEMENT_BONEXTENSION( PICML::Capability, "PICML::Capability" );
IMPLEMENT_BONEXTENSION( PICML::Interconnect, "PICML::Interconnect" );
IMPLEMENT_BONEXTENSION( PICML::Node, "PICML::Node" );
IMPLEMENT_BONEXTENSION( PICML::Resource, "PICML::Resource" );
IMPLEMENT_BONEXTENSION( PICML::SharedResource, "PICML::SharedResource" );
IMPLEMENT_ABSTRACT_BONEXTENSION( PICML::ActionBase );
IMPLEMENT_BONEXTENSION( PICML::Action, "PICML::Action" );
IMPLEMENT_BONEXTENSION( PICML::CompositeAction, "PICML::CompositeAction" );
IMPLEMENT_BONEXTENSION( PICML::OutputAction, "PICML::OutputAction" );
IMPLEMENT_ABSTRACT_BONEXTENSION( PICML::ComponentLib );
IMPLEMENT_BONEXTENSION( PICML::ExecutorProject, "PICML::ExecutorProject" );
IMPLEMENT_BONEXTENSION( PICML::ServantProject, "PICML::ServantProject" );
IMPLEMENT_BONEXTENSION( PICML::StubProject, "PICML::StubProject" );
IMPLEMENT_ABSTRACT_BONEXTENSION( PICML::RequirementBase );
IMPLEMENT_BONEXTENSION( PICML::ImplementationRequirement, "PICML::ImplementationRequirement" );
IMPLEMENT_BONEXTENSION( PICML::Requirement, "PICML::Requirement" );
IMPLEMENT_ABSTRACT_BONEXTENSION( PICML::InputActionBase );
IMPLEMENT_BONEXTENSION( PICML::InputAction, "PICML::InputAction" );
IMPLEMENT_BONEXTENSION( PICML::PeriodicAction, "PICML::PeriodicAction" );
IMPLEMENT_ABSTRACT_BONEXTENSION( PICML::MetricsBase );
IMPLEMENT_BONEXTENSION( PICML::Latency, "PICML::Latency" );
IMPLEMENT_BONEXTENSION( PICML::Throughput, "PICML::Throughput" );
IMPLEMENT_BONEXTENSION( PICML::WorkerPackage, "PICML::WorkerPackage" );
IMPLEMENT_BONEXTENSION( PICML::WorkerFile, "PICML::WorkerFile" );

} // end namespace BON

namespace PICML
{

void
ObjectImpl::accept (GatewayGeneratorVisitor* visitor)
{
  Object object (this);
  visitor->visitObject (object);
}

void
TwowayOperationImpl::accept (GatewayGeneratorVisitor* visitor)
{
  TwowayOperation operation (this);
  visitor->visitTwowayOperation (operation);
}

//********************************************************************************
// getter for role "InoutParameter" among "InoutParameter"s
//********************************************************************************
std::set<InoutParameter> TwowayOperationImpl::getInoutParameter()
{
	std::set<InoutParameter> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::InoutParameter");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		InoutParameter elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "OutParameter" among "OutParameter"s
//********************************************************************************
std::set<OutParameter> TwowayOperationImpl::getOutParameter()
{
	std::set<OutParameter> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::OutParameter");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		OutParameter elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "ReturnType" among "ReturnType"s
//********************************************************************************
std::set<ReturnType> TwowayOperationImpl::getReturnType()
{
	std::set<ReturnType> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::ReturnType");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		ReturnType elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
//
//********************************************************************************
MemberType AliasImpl::getMemberType()
{
	BON::FCO r = BON::ReferenceImpl::getReferred();
	return MemberType(r);
}


//********************************************************************************
//
//********************************************************************************
MemberType CollectionImpl::getMemberType()
{
	BON::FCO r = BON::ReferenceImpl::getReferred();
	return MemberType(r);
}


//********************************************************************************
// returns dst Manageables
//********************************************************************************
std::multiset<Manageable> ComponentFactoryImpl::getManagesComponentDsts()
{
	std::multiset<Manageable> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("PICML::ManagesComponent");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
		{
			Manageable dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
//
//********************************************************************************
std::set<ManagesComponent> ComponentFactoryImpl::getOutManagesComponentLinks()
{
	std::set<ManagesComponent> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		ManagesComponent c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// getter for role "FactoryOperation" among "FactoryOperation"s
//********************************************************************************
std::set<FactoryOperation> ComponentFactoryImpl::getFactoryOperation()
{
	std::set<FactoryOperation> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::FactoryOperation");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		FactoryOperation elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "LookupKey" among "LookupKey"s
//********************************************************************************
std::set<LookupKey> ComponentFactoryImpl::getLookupKey()
{
	std::set<LookupKey> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::LookupKey");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		LookupKey elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "LookupOperation" among "LookupOperation"s
//********************************************************************************
std::set<LookupOperation> ComponentFactoryImpl::getLookupOperation()
{
	std::set<LookupOperation> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::LookupOperation");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		LookupOperation elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
//
//********************************************************************************
bool ObjectImpl::isabstract()
{
	return FCOImpl::getAttribute("abstract")->getBooleanValue();
}


//********************************************************************************
//
//********************************************************************************
bool ObjectImpl::islocal()
{
	return FCOImpl::getAttribute("local")->getBooleanValue();
}


//********************************************************************************
//
//********************************************************************************
void ObjectImpl::setabstract( bool val)
{
	FCOImpl::getAttribute("abstract")->setBooleanValue( val);
}


//********************************************************************************
//
//********************************************************************************
void ObjectImpl::setlocal( bool val)
{
	FCOImpl::getAttribute("local")->setBooleanValue( val);
}


//********************************************************************************
//
//********************************************************************************
bool ObjectByValueImpl::isabstract()
{
	return FCOImpl::getAttribute("abstract")->getBooleanValue();
}


//********************************************************************************
//
//********************************************************************************
void ObjectByValueImpl::setabstract( bool val)
{
	FCOImpl::getAttribute("abstract")->setBooleanValue( val);
}


//********************************************************************************
// getter for role "FactoryOperation" among "FactoryOperation"s
//********************************************************************************
std::set<FactoryOperation> ObjectByValueImpl::getFactoryOperation()
{
	std::set<FactoryOperation> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::FactoryOperation");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		FactoryOperation elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "MakeMemberPrivate" among "MakeMemberPrivate"s
//********************************************************************************
std::set<MakeMemberPrivate> ObjectByValueImpl::getMakeMemberPrivate()
{
	std::set<MakeMemberPrivate> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::MakeMemberPrivate");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		MakeMemberPrivate elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Member" among "Member"s
//********************************************************************************
std::set<Member> ObjectByValueImpl::getMember()
{
	std::set<Member> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::Member");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Member elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "PrivateFlag" among "PrivateFlag"s
//********************************************************************************
std::set<PrivateFlag> ObjectByValueImpl::getPrivateFlag()
{
	std::set<PrivateFlag> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::PrivateFlag");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		PrivateFlag elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
//
//********************************************************************************
std::string RequirementSatisfierImpl::getresourceType()
{
	return FCOImpl::getAttribute("resourceType")->getStringValue();
}


//********************************************************************************
//
//********************************************************************************
void RequirementSatisfierImpl::setresourceType( const std::string& val)
{
	FCOImpl::getAttribute("resourceType")->setStringValue( val);
}


//********************************************************************************
// getter for role "SatisfierProperty" among "SatisfierProperty"s
//********************************************************************************
std::set<SatisfierProperty> RequirementSatisfierImpl::getSatisfierProperty()
{
	std::set<SatisfierProperty> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::SatisfierProperty");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		SatisfierProperty elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// returns dst Interconnects
//********************************************************************************
std::multiset<Interconnect> BridgeImpl::getBridge2InterconnectDsts()
{
	std::multiset<Interconnect> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("PICML::Bridge2Interconnect");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
		{
			Interconnect dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
//
//********************************************************************************
std::set<Interconnect2Bridge> BridgeImpl::getInInterconnect2BridgeLinks()
{
	std::set<Interconnect2Bridge> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		Interconnect2Bridge c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// returns src Interconnects
//********************************************************************************
std::multiset<Interconnect> BridgeImpl::getInterconnect2BridgeSrcs()
{
	std::multiset<Interconnect> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("PICML::Interconnect2Bridge");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			Interconnect dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
//
//********************************************************************************
std::set<Bridge2Interconnect> BridgeImpl::getOutBridge2InterconnectLinks()
{
	std::set<Bridge2Interconnect> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		Bridge2Interconnect c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
//
//********************************************************************************
std::string BridgeImpl::getlabel()
{
	return FCOImpl::getAttribute("label")->getStringValue();
}


//********************************************************************************
//
//********************************************************************************
void BridgeImpl::setlabel( const std::string& val)
{
	FCOImpl::getAttribute("label")->setStringValue( val);
}


//********************************************************************************
// getter for role "Resource" among "Resource"s
//********************************************************************************
std::set<Resource> BridgeImpl::getResource()
{
	std::set<Resource> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::Resource");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Resource elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// returns src ComponentImplementations
//********************************************************************************
std::multiset<ComponentImplementation> CapabilityImpl::getImplementationCapabilitySrcs()
{
	std::multiset<ComponentImplementation> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("PICML::ImplementationCapability");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			ComponentImplementation dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
//
//********************************************************************************
std::set<ImplementationCapability> CapabilityImpl::getInImplementationCapabilityLinks()
{
	std::set<ImplementationCapability> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		ImplementationCapability c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// returns src Bridges
//********************************************************************************
std::multiset<Bridge> InterconnectImpl::getBridge2InterconnectSrcs()
{
	std::multiset<Bridge> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("PICML::Bridge2Interconnect");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			Bridge dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
//
//********************************************************************************
std::set<Bridge2Interconnect> InterconnectImpl::getInBridge2InterconnectLinks()
{
	std::set<Bridge2Interconnect> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		Bridge2Interconnect c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
//
//********************************************************************************
std::set<Node2Interconnect> InterconnectImpl::getInNode2InterconnectLinks()
{
	std::set<Node2Interconnect> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		Node2Interconnect c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// returns dst Bridges
//********************************************************************************
std::multiset<Bridge> InterconnectImpl::getInterconnect2BridgeDsts()
{
	std::multiset<Bridge> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("PICML::Interconnect2Bridge");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
		{
			Bridge dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// returns dst Nodes
//********************************************************************************
std::multiset<Node> InterconnectImpl::getInterconnect2NodeDsts()
{
	std::multiset<Node> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("PICML::Interconnect2Node");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
		{
			Node dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// returns src Nodes
//********************************************************************************
std::multiset<Node> InterconnectImpl::getNode2InterconnectSrcs()
{
	std::multiset<Node> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("PICML::Node2Interconnect");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			Node dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
//
//********************************************************************************
std::set<Interconnect2Bridge> InterconnectImpl::getOutInterconnect2BridgeLinks()
{
	std::set<Interconnect2Bridge> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		Interconnect2Bridge c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
//
//********************************************************************************
std::set<Interconnect2Node> InterconnectImpl::getOutInterconnect2NodeLinks()
{
	std::set<Interconnect2Node> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		Interconnect2Node c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
//
//********************************************************************************
std::string InterconnectImpl::getlabel()
{
	return FCOImpl::getAttribute("label")->getStringValue();
}


//********************************************************************************
//
//********************************************************************************
void InterconnectImpl::setlabel( const std::string& val)
{
	FCOImpl::getAttribute("label")->setStringValue( val);
}


//********************************************************************************
// getter for role "Resource" among "Resource"s
//********************************************************************************
std::set<Resource> InterconnectImpl::getResource()
{
	std::set<Resource> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::Resource");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Resource elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
//
//********************************************************************************
std::set<Interconnect2Node> NodeImpl::getInInterconnect2NodeLinks()
{
	std::set<Interconnect2Node> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		Interconnect2Node c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// returns src Interconnects
//********************************************************************************
std::multiset<Interconnect> NodeImpl::getInterconnect2NodeSrcs()
{
	std::multiset<Interconnect> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("PICML::Interconnect2Node");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			Interconnect dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// returns dst Interconnects
//********************************************************************************
std::multiset<Interconnect> NodeImpl::getNode2InterconnectDsts()
{
	std::multiset<Interconnect> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("PICML::Node2Interconnect");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
		{
			Interconnect dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
//
//********************************************************************************
std::set<Node2Interconnect> NodeImpl::getOutNode2InterconnectLinks()
{
	std::set<Node2Interconnect> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		Node2Interconnect c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
//
//********************************************************************************
std::set<Shares> NodeImpl::getOutSharesLinks()
{
	std::set<Shares> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		Shares c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// returns dst SharedResources
//********************************************************************************
std::multiset<SharedResource> NodeImpl::getSharesDsts()
{
	std::multiset<SharedResource> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("PICML::Shares");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
		{
			SharedResource dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
//
//********************************************************************************
std::string NodeImpl::getlabel()
{
	return FCOImpl::getAttribute("label")->getStringValue();
}


//********************************************************************************
//
//********************************************************************************
void NodeImpl::setlabel( const std::string& val)
{
	FCOImpl::getAttribute("label")->setStringValue( val);
}


//********************************************************************************
// getter for role "Resource" among "Resource"s
//********************************************************************************
std::set<Resource> NodeImpl::getResource()
{
	std::set<Resource> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::Resource");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Resource elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
//
//********************************************************************************
std::set<Shares> SharedResourceImpl::getInSharesLinks()
{
	std::set<Shares> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		Shares c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// returns src Nodes
//********************************************************************************
std::multiset<Node> SharedResourceImpl::getSharesSrcs()
{
	std::multiset<Node> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("PICML::Shares");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			Node dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// returns dst States
//********************************************************************************
std::multiset<State> ActionBaseImpl::getEffectDsts()
{
	std::multiset<State> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("PICML::Effect");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
		{
			State dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
//
//********************************************************************************
std::set<Transition> ActionBaseImpl::getInTransitionLinks()
{
	std::set<Transition> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		Transition c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
//
//********************************************************************************
std::set<Effect> ActionBaseImpl::getOutEffectLinks()
{
	std::set<Effect> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		Effect c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// returns src States
//********************************************************************************
std::multiset<State> ActionBaseImpl::getTransitionSrcs()
{
	std::multiset<State> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("PICML::Transition");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			State dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
//
//********************************************************************************
long ActionBaseImpl::getRepetitions()
{
	return FCOImpl::getAttribute("Repetitions")->getIntegerValue();
}


//********************************************************************************
//
//********************************************************************************
bool ActionBaseImpl::isLogAction()
{
	return FCOImpl::getAttribute("LogAction")->getBooleanValue();
}


//********************************************************************************
//
//********************************************************************************
void ActionBaseImpl::setLogAction( bool val)
{
	FCOImpl::getAttribute("LogAction")->setBooleanValue( val);
}


//********************************************************************************
//
//********************************************************************************
void ActionBaseImpl::setRepetitions( const long val)
{
	FCOImpl::getAttribute("Repetitions")->setIntegerValue( val);
}


//********************************************************************************
// getter for role "Property" among "Property"s
//********************************************************************************
std::set<Property> ActionBaseImpl::getProperty()
{
	std::set<Property> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::Property");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Property elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Action" among "Action"s and its descendants
//********************************************************************************
std::set<Action> CompositeActionImpl::getAction()
{
	std::set<Action> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::Action");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Action elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// aggregated getter for role "" among "Action"s and its descendants
//********************************************************************************
std::set<Action> CompositeActionImpl::getAction(int dummy)
{
	std::set<Action> res;
	const int len = 3;
	std::set<BON::FCO> roles_vec[ len];
	roles_vec[0] = ModelImpl::getChildFCOsAs("PICML::CompositeAction");
	roles_vec[1] = ModelImpl::getChildFCOsAs("PICML::OutputAction");
	roles_vec[2] = ModelImpl::getChildFCOsAs("PICML::Action");
	for( int k = 0; k < len; ++k)
		for( std::set<BON::FCO>::iterator i = roles_vec[k].begin(); i != roles_vec[k].end(); ++i)
		{
			Action elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
	return res;
}


//********************************************************************************
// getter for role "CompositeAction" among "Action"s and its descendants
//********************************************************************************
std::set<CompositeAction> CompositeActionImpl::getCompositeAction()
{
	std::set<CompositeAction> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::CompositeAction");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		CompositeAction elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Effect" among "Effect"s
//********************************************************************************
std::set<Effect> CompositeActionImpl::getEffect()
{
	std::set<Effect> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::Effect");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Effect elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Finish" among "Finish"s
//********************************************************************************
std::set<Finish> CompositeActionImpl::getFinish()
{
	std::set<Finish> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::Finish");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Finish elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "InputAction" among "InputAction"s
//********************************************************************************
std::set<InputAction> CompositeActionImpl::getInputAction()
{
	std::set<InputAction> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::InputAction");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		InputAction elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "OutputAction" among "Action"s and its descendants
//********************************************************************************
std::set<OutputAction> CompositeActionImpl::getOutputAction()
{
	std::set<OutputAction> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::OutputAction");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		OutputAction elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "State" among "State"s
//********************************************************************************
std::set<State> CompositeActionImpl::getState()
{
	std::set<State> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::State");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		State elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Transition" among "Transition"s
//********************************************************************************
std::set<Transition> CompositeActionImpl::getTransition()
{
	std::set<Transition> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::Transition");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Transition elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// returns dst ExternalResourcess
//********************************************************************************
std::multiset<ExternalResources> ComponentLibImpl::getExtResourceConnDsts()
{
	std::multiset<ExternalResources> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("PICML::ExtResourceConn");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
		{
			ExternalResources dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
//
//********************************************************************************
std::set<ExtResourceConn> ComponentLibImpl::getOutExtResourceConnLinks()
{
	std::set<ExtResourceConn> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
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
std::string ComponentLibImpl::getORBServices()
{
	return FCOImpl::getAttribute("ORBServices")->getStringValue();
}


//********************************************************************************
//
//********************************************************************************
std::string ComponentLibImpl::getlibraryexport()
{
	return FCOImpl::getAttribute("libraryexport")->getStringValue();
}


//********************************************************************************
//
//********************************************************************************
std::string ComponentLibImpl::getsharedname()
{
	return FCOImpl::getAttribute("sharedname")->getStringValue();
}


//********************************************************************************
//
//********************************************************************************
void ComponentLibImpl::setORBServices( const std::string& val)
{
	FCOImpl::getAttribute("ORBServices")->setStringValue( val);
}


//********************************************************************************
//
//********************************************************************************
void ComponentLibImpl::setlibraryexport( const std::string& val)
{
	FCOImpl::getAttribute("libraryexport")->setStringValue( val);
}


//********************************************************************************
//
//********************************************************************************
void ComponentLibImpl::setsharedname( const std::string& val)
{
	FCOImpl::getAttribute("sharedname")->setStringValue( val);
}


//********************************************************************************
// getter for role "ImplementationArtifactReference" among "ImplementationArtifactReference"s
//********************************************************************************
std::set<ImplementationArtifactReference> ExecutorProjectImpl::getImplementationArtifactReference()
{
	std::set<ImplementationArtifactReference> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::ImplementationArtifactReference");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		ImplementationArtifactReference elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "FileRef" among "FileRef"s
//********************************************************************************
std::set<FileRef> ServantProjectImpl::getFileRef()
{
	std::set<FileRef> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::FileRef");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		FileRef elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "ImplementationArtifactReference" among "ImplementationArtifactReference"s
//********************************************************************************
std::set<ImplementationArtifactReference> ServantProjectImpl::getImplementationArtifactReference()
{
	std::set<ImplementationArtifactReference> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::ImplementationArtifactReference");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		ImplementationArtifactReference elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "FileRef" among "FileRef"s
//********************************************************************************
std::set<FileRef> StubProjectImpl::getFileRef()
{
	std::set<FileRef> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::FileRef");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		FileRef elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "ImplementationArtifactReference" among "ImplementationArtifactReference"s
//********************************************************************************
std::set<ImplementationArtifactReference> StubProjectImpl::getImplementationArtifactReference()
{
	std::set<ImplementationArtifactReference> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::ImplementationArtifactReference");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		ImplementationArtifactReference elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
//
//********************************************************************************
std::string RequirementBaseImpl::getresourceType()
{
	return FCOImpl::getAttribute("resourceType")->getStringValue();
}


//********************************************************************************
//
//********************************************************************************
void RequirementBaseImpl::setresourceType( const std::string& val)
{
	FCOImpl::getAttribute("resourceType")->setStringValue( val);
}


//********************************************************************************
// getter for role "Property" among "Property"s
//********************************************************************************
std::set<Property> RequirementBaseImpl::getProperty()
{
	std::set<Property> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::Property");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Property elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
//
//********************************************************************************
std::set<MonolithDeployRequirement> ImplementationRequirementImpl::getInMonolithDeployRequirementLinks()
{
	std::set<MonolithDeployRequirement> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		MonolithDeployRequirement c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// returns src MonolithicImplementations
//********************************************************************************
std::multiset<MonolithicImplementation> ImplementationRequirementImpl::getMonolithDeployRequirementSrcs()
{
	std::multiset<MonolithicImplementation> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("PICML::MonolithDeployRequirement");
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
ImplementationRequirementImpl::ResourceUsageKind_Type ImplementationRequirementImpl::getResourceUsageKind()
{
	std::string val = FCOImpl::getAttribute("ResourceUsageKind")->getStringValue();

	if ( val == "None") return None_ResourceUsageKind_Type;
	else if ( val == "InstanceUsesResource") return InstanceUsesResource_ResourceUsageKind_Type;
	else if ( val == "ResourceUsesInstance") return ResourceUsesInstance_ResourceUsageKind_Type;
	else if ( val == "PortUsesResource") return PortUsesResource_ResourceUsageKind_Type;
	else if ( val == "ResourceUsesPort") return ResourceUsesPort_ResourceUsageKind_Type;
	else throw("PICML::None of the possible items");
}


//********************************************************************************
//
//********************************************************************************
std::string ImplementationRequirementImpl::getcomponentPort()
{
	return FCOImpl::getAttribute("componentPort")->getStringValue();
}


//********************************************************************************
//
//********************************************************************************
std::string ImplementationRequirementImpl::getresourcePort()
{
	return FCOImpl::getAttribute("resourcePort")->getStringValue();
}


//********************************************************************************
//
//********************************************************************************
void ImplementationRequirementImpl::setResourceUsageKind( ImplementationRequirementImpl::ResourceUsageKind_Type val)
{
	std::string str_val = "";

	if ( val == None_ResourceUsageKind_Type) str_val = "None";
	else if ( val == InstanceUsesResource_ResourceUsageKind_Type) str_val = "InstanceUsesResource";
	else if ( val == ResourceUsesInstance_ResourceUsageKind_Type) str_val = "ResourceUsesInstance";
	else if ( val == PortUsesResource_ResourceUsageKind_Type) str_val = "PortUsesResource";
	else if ( val == ResourceUsesPort_ResourceUsageKind_Type) str_val = "ResourceUsesPort";
	else throw("PICML::None of the possible items");

	FCOImpl::getAttribute("ResourceUsageKind")->setStringValue( str_val);
}


//********************************************************************************
//
//********************************************************************************
void ImplementationRequirementImpl::setcomponentPort( const std::string& val)
{
	FCOImpl::getAttribute("componentPort")->setStringValue( val);
}


//********************************************************************************
//
//********************************************************************************
void ImplementationRequirementImpl::setresourcePort( const std::string& val)
{
	FCOImpl::getAttribute("resourcePort")->setStringValue( val);
}


//********************************************************************************
// returns src ImplementationArtifacts
//********************************************************************************
std::multiset<ImplementationArtifact> RequirementImpl::getArtifactDeployRequirementSrcs()
{
	std::multiset<ImplementationArtifact> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("PICML::ArtifactDeployRequirement");
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
// returns src PublishConnectors
//********************************************************************************
std::multiset<PublishConnector> RequirementImpl::getAssemblyDeployRequirementSrcs()
{
	std::multiset<PublishConnector> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("PICML::AssemblyDeployRequirement");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			PublishConnector dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// returns src Components
//********************************************************************************
std::multiset<Component> RequirementImpl::getAssemblyselectRequirementSrcs()
{
	std::multiset<Component> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("PICML::AssemblyselectRequirement");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			Component dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
//
//********************************************************************************
std::set<ArtifactDeployRequirement> RequirementImpl::getInArtifactDeployRequirementLinks()
{
	std::set<ArtifactDeployRequirement> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		ArtifactDeployRequirement c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
//
//********************************************************************************
std::set<AssemblyDeployRequirement> RequirementImpl::getInAssemblyDeployRequirementLinks()
{
	std::set<AssemblyDeployRequirement> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		AssemblyDeployRequirement c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
//
//********************************************************************************
std::set<AssemblyselectRequirement> RequirementImpl::getInAssemblyselectRequirementLinks()
{
	std::set<AssemblyselectRequirement> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		AssemblyselectRequirement c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
//
//********************************************************************************
std::set<PackageConfSelectRequirement> RequirementImpl::getInPackageConfSelectRequirementLinks()
{
	std::set<PackageConfSelectRequirement> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		PackageConfSelectRequirement c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// returns src PackageConfigurations
//********************************************************************************
std::multiset<PackageConfiguration> RequirementImpl::getPackageConfSelectRequirementSrcs()
{
	std::multiset<PackageConfiguration> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("PICML::PackageConfSelectRequirement");
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
// returns src States
//********************************************************************************
std::multiset<State> InputActionBaseImpl::getFinishSrcs()
{
	std::multiset<State> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("PICML::Finish");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			State dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
//
//********************************************************************************
std::set<Finish> InputActionBaseImpl::getInFinishLinks()
{
	std::set<Finish> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		Finish c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
//
//********************************************************************************
std::set<Input> InputActionBaseImpl::getInInputLinks()
{
	std::set<Input> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		Input c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
//
//********************************************************************************
std::set<MultiInput> InputActionBaseImpl::getInMultiInputLinks()
{
	std::set<MultiInput> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		MultiInput c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// returns src InEventPorts
//********************************************************************************
std::multiset<InEventPort> InputActionBaseImpl::getInputSrcs()
{
	std::multiset<InEventPort> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("PICML::Input");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			InEventPort dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// returns src MultiInputBases
//********************************************************************************
std::multiset<MultiInputBase> InputActionBaseImpl::getMultiInputSrcs()
{
	std::multiset<MultiInputBase> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("PICML::MultiInput");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			MultiInputBase dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// getter for role "Property" among "Property"s
//********************************************************************************
std::set<Property> InputActionBaseImpl::getProperty()
{
	std::set<Property> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::Property");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Property elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
//
//********************************************************************************
long PeriodicActionImpl::getPeriod()
{
	return FCOImpl::getAttribute("Period")->getIntegerValue();
}


//********************************************************************************
//
//********************************************************************************
double PeriodicActionImpl::getProbability()
{
	return FCOImpl::getAttribute("Probability")->getRealValue();
}


//********************************************************************************
//
//********************************************************************************
void PeriodicActionImpl::setPeriod( const long val)
{
	FCOImpl::getAttribute("Period")->setIntegerValue( val);
}


//********************************************************************************
//
//********************************************************************************
void PeriodicActionImpl::setProbability( const double val)
{
	FCOImpl::getAttribute("Probability")->setRealValue( val);
}


//********************************************************************************
// returns src BenchmarkTypes
//********************************************************************************
std::multiset<BenchmarkType> MetricsBaseImpl::getBenchmarkCharacteristicsSrcs()
{
	std::multiset<BenchmarkType> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("PICML::BenchmarkCharacteristics");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			BenchmarkType dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
//
//********************************************************************************
std::set<BenchmarkCharacteristics> MetricsBaseImpl::getInBenchmarkCharacteristicsLinks()
{
	std::set<BenchmarkCharacteristics> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		BenchmarkCharacteristics c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
//
//********************************************************************************
std::set<MetricConnection> MetricsBaseImpl::getInMetricConnectionLinks()
{
	std::set<MetricConnection> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
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
// returns src OperationRefs
//********************************************************************************
std::multiset<OperationRef> MetricsBaseImpl::getMetricConnectionSrcs()
{
	std::multiset<OperationRef> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("PICML::MetricConnection");
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
std::set<WorkloadCharacteristics> MetricsBaseImpl::getOutWorkloadCharacteristicsLinks()
{
	std::set<WorkloadCharacteristics> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
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
// returns dst TaskSets
//********************************************************************************
std::multiset<TaskSet> MetricsBaseImpl::getWorkloadCharacteristicsDsts()
{
	std::multiset<TaskSet> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("PICML::WorkloadCharacteristics");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
		{
			TaskSet dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
//
//********************************************************************************
std::string MetricsBaseImpl::getfileName()
{
	return FCOImpl::getAttribute("fileName")->getStringValue();
}


//********************************************************************************
//
//********************************************************************************
long MetricsBaseImpl::getiterations()
{
	return FCOImpl::getAttribute("iterations")->getIntegerValue();
}


//********************************************************************************
//
//********************************************************************************
long MetricsBaseImpl::getpriority()
{
	return FCOImpl::getAttribute("priority")->getIntegerValue();
}


//********************************************************************************
//
//********************************************************************************
long MetricsBaseImpl::getrate()
{
	return FCOImpl::getAttribute("rate")->getIntegerValue();
}


//********************************************************************************
//
//********************************************************************************
MetricsBaseImpl::resolution_Type MetricsBaseImpl::getresolution()
{
	std::string val = FCOImpl::getAttribute("resolution")->getStringValue();

	if ( val == "seconds") return seconds_resolution_Type;
	else if ( val == "miliseconds") return miliseconds_resolution_Type;
	else if ( val == "microseconds") return microseconds_resolution_Type;
	else throw("None of the possible items");
}


//********************************************************************************
//
//********************************************************************************
long MetricsBaseImpl::getwarmup()
{
	return FCOImpl::getAttribute("warmup")->getIntegerValue();
}


//********************************************************************************
//
//********************************************************************************
void MetricsBaseImpl::setfileName( const std::string& val)
{
	FCOImpl::getAttribute("fileName")->setStringValue( val);
}


//********************************************************************************
//
//********************************************************************************
void MetricsBaseImpl::setiterations( const long val)
{
	FCOImpl::getAttribute("iterations")->setIntegerValue( val);
}


//********************************************************************************
//
//********************************************************************************
void MetricsBaseImpl::setpriority( const long val)
{
	FCOImpl::getAttribute("priority")->setIntegerValue( val);
}


//********************************************************************************
//
//********************************************************************************
void MetricsBaseImpl::setrate( const long val)
{
	FCOImpl::getAttribute("rate")->setIntegerValue( val);
}


//********************************************************************************
//
//********************************************************************************
void MetricsBaseImpl::setresolution( MetricsBaseImpl::resolution_Type val)
{
	std::string str_val = "";

	if ( val == seconds_resolution_Type) str_val = "seconds";
	else if ( val == miliseconds_resolution_Type) str_val = "miliseconds";
	else if ( val == microseconds_resolution_Type) str_val = "microseconds";
	else throw("None of the possible items");

	FCOImpl::getAttribute("resolution")->setStringValue( str_val);
}


//********************************************************************************
//
//********************************************************************************
void MetricsBaseImpl::setwarmup( const long val)
{
	FCOImpl::getAttribute("warmup")->setIntegerValue( val);
}


//********************************************************************************
// getter for role "Average" among "DataAnalysisBase"s and its descendants
//********************************************************************************
std::set<Average> MetricsBaseImpl::getAverage()
{
	std::set<Average> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::Average");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Average elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// aggregated getter for role "" among "DataAnalysisBase"s and its descendants
//********************************************************************************
std::set<DataAnalysisBase> MetricsBaseImpl::getDataAnalysisBase()
{
	std::set<DataAnalysisBase> res;
	const int len = 4;
	std::set<BON::FCO> roles_vec[ len];
	roles_vec[0] = ModelImpl::getChildFCOsAs("PICML::Average");
	roles_vec[1] = ModelImpl::getChildFCOsAs("PICML::Jitter");
	roles_vec[2] = ModelImpl::getChildFCOsAs("PICML::Maximum");
	roles_vec[3] = ModelImpl::getChildFCOsAs("PICML::Minimum");
	for( int k = 0; k < len; ++k)
		for( std::set<BON::FCO>::iterator i = roles_vec[k].begin(); i != roles_vec[k].end(); ++i)
		{
			DataAnalysisBase elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
	return res;
}


//********************************************************************************
// getter for role "Jitter" among "DataAnalysisBase"s and its descendants
//********************************************************************************
std::set<Jitter> MetricsBaseImpl::getJitter()
{
	std::set<Jitter> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::Jitter");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Jitter elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Maximum" among "DataAnalysisBase"s and its descendants
//********************************************************************************
std::set<Maximum> MetricsBaseImpl::getMaximum()
{
	std::set<Maximum> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::Maximum");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Maximum elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Minimum" among "DataAnalysisBase"s and its descendants
//********************************************************************************
std::set<Minimum> MetricsBaseImpl::getMinimum()
{
	std::set<Minimum> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::Minimum");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Minimum elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Worker" among "Worker"s
//********************************************************************************
std::set<Worker> WorkerPackageImpl::getWorker()
{
	std::set<Worker> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::Worker");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Worker elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "WorkerPackage" among "WorkerPackage"s
//********************************************************************************
std::set<WorkerPackage> WorkerPackageImpl::getWorkerPackage()
{
	std::set<WorkerPackage> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::WorkerPackage");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		WorkerPackage elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
//
//********************************************************************************
std::string WorkerFileImpl::getLocation()
{
	return FCOImpl::getAttribute("Location")->getStringValue();
}


//********************************************************************************
//
//********************************************************************************
void WorkerFileImpl::setLocation( const std::string& val)
{
	FCOImpl::getAttribute("Location")->setStringValue( val);
}


} // namespace PICML
