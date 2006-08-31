#include "stdafx.h"
#include "PICMLBonX.h"
#include "PICMLBonX1.h"
#include "PICMLBonX2.h"
#include "PICMLBonX3.h"
#include "PICMLBonX4.h"


namespace BON
{

IMPLEMENT_ABSTRACT_BONEXTENSION( PICML::ComponentImplementation );
IMPLEMENT_BONEXTENSION( PICML::ComponentAssembly, "PICML::ComponentAssembly" );
IMPLEMENT_BONEXTENSION( PICML::MonolithicImplementation, "PICML::MonolithicImplementation" );
IMPLEMENT_BONEXTENSION( PICML::ArtifactContainer, "PICML::ArtifactContainer" );
IMPLEMENT_BONEXTENSION( PICML::BenchmarkAnalysis, "PICML::BenchmarkAnalysis" );
IMPLEMENT_BONEXTENSION( PICML::ComponentContainer, "PICML::ComponentContainer" );
IMPLEMENT_BONEXTENSION( PICML::ComponentImplementationContainer, "PICML::ComponentImplementationContainer" );
IMPLEMENT_BONEXTENSION( PICML::ComponentPropertyDescription, "PICML::ComponentPropertyDescription" );
IMPLEMENT_BONEXTENSION( PICML::DeploymentPlan, "PICML::DeploymentPlan" );
IMPLEMENT_BONEXTENSION( PICML::Domain, "PICML::Domain" );
IMPLEMENT_BONEXTENSION( PICML::MPC, "PICML::MPC" );
IMPLEMENT_BONEXTENSION( PICML::PackageConfigurationContainer, "PICML::PackageConfigurationContainer" );
IMPLEMENT_BONEXTENSION( PICML::PackageContainer, "PICML::PackageContainer" );
IMPLEMENT_BONEXTENSION( PICML::Path, "PICML::Path" );
IMPLEMENT_BONEXTENSION( PICML::Paths, "PICML::Paths" );
IMPLEMENT_BONEXTENSION( PICML::Project, "PICML::Project" );
IMPLEMENT_BONEXTENSION( PICML::Property, "PICML::Property" );
IMPLEMENT_BONEXTENSION( PICML::SatisfierProperty, "PICML::SatisfierProperty" );
IMPLEMENT_BONEXTENSION( PICML::TopLevelPackageContainer, "PICML::TopLevelPackageContainer" );
IMPLEMENT_BONEXTENSION( PICML::Worker, "PICML::Worker" );
IMPLEMENT_BONEXTENSION( PICML::WorkerLibrary, "PICML::WorkerLibrary" );
IMPLEMENT_ABSTRACT_BONEXTENSION( PICML::BenchmarkType );
IMPLEMENT_BONEXTENSION( PICML::FixedIterationBenchmarks, "PICML::FixedIterationBenchmarks" );
IMPLEMENT_BONEXTENSION( PICML::PeriodicBenchmarks, "PICML::PeriodicBenchmarks" );
IMPLEMENT_BONEXTENSION( PICML::TriggerBenchmarks, "PICML::TriggerBenchmarks" );
IMPLEMENT_ABSTRACT_BONEXTENSION( PICML::DataAnalysisBase );
IMPLEMENT_BONEXTENSION( PICML::Average, "PICML::Average" );
IMPLEMENT_BONEXTENSION( PICML::Jitter, "PICML::Jitter" );
IMPLEMENT_BONEXTENSION( PICML::Maximum, "PICML::Maximum" );
IMPLEMENT_BONEXTENSION( PICML::Minimum, "PICML::Minimum" );


} // end namespace BON

namespace PICML
{

//********************************************************************************
// returns dst Propertys
//********************************************************************************
std::multiset<Property> ComponentImplementationImpl::getConfigPropertyDsts()
{
	std::multiset<Property> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("PICML::ConfigProperty");
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
// returns dst Capabilitys
//********************************************************************************
std::multiset<Capability> ComponentImplementationImpl::getImplementationCapabilityDsts()
{
	std::multiset<Capability> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("PICML::ImplementationCapability");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
		{
			Capability dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// returns dst ImplementationDependencys
//********************************************************************************
std::multiset<ImplementationDependency> ComponentImplementationImpl::getImplementationDependsOnDsts()
{
	std::multiset<ImplementationDependency> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("PICML::ImplementationDependsOn");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
		{
			ImplementationDependency dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// returns dst ComponentRefs
//********************************************************************************
std::multiset<ComponentRef> ComponentImplementationImpl::getImplementsDsts()
{
	std::multiset<ComponentRef> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("PICML::Implements");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
		{
			ComponentRef dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// returns dst Propertys
//********************************************************************************
std::multiset<Property> ComponentImplementationImpl::getInfoPropertyDsts()
{
	std::multiset<Property> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("PICML::InfoProperty");
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
std::set<ConfigProperty> ComponentImplementationImpl::getOutConfigPropertyLinks()
{
	std::set<ConfigProperty> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		ConfigProperty c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
//
//********************************************************************************
std::set<ImplementationCapability> ComponentImplementationImpl::getOutImplementationCapabilityLinks()
{
	std::set<ImplementationCapability> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
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
//
//********************************************************************************
std::set<ImplementationDependsOn> ComponentImplementationImpl::getOutImplementationDependsOnLinks()
{
	std::set<ImplementationDependsOn> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		ImplementationDependsOn c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
//
//********************************************************************************
std::set<Implements> ComponentImplementationImpl::getOutImplementsLinks()
{
	std::set<Implements> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		Implements c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
//
//********************************************************************************
std::set<InfoProperty> ComponentImplementationImpl::getOutInfoPropertyLinks()
{
	std::set<InfoProperty> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		InfoProperty c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
//
//********************************************************************************
std::string ComponentImplementationImpl::getUUID()
{
	return FCOImpl::getAttribute("UUID")->getStringValue();
}


//********************************************************************************
//
//********************************************************************************
std::string ComponentImplementationImpl::getlabel()
{
	return FCOImpl::getAttribute("label")->getStringValue();
}


//********************************************************************************
//
//********************************************************************************
void ComponentImplementationImpl::setUUID( const std::string& val)
{
	FCOImpl::getAttribute("UUID")->setStringValue( val);
}


//********************************************************************************
//
//********************************************************************************
void ComponentImplementationImpl::setlabel( const std::string& val)
{
	FCOImpl::getAttribute("label")->setStringValue( val);
}


//********************************************************************************
// returns dst PathReferences
//********************************************************************************
std::multiset<PathReference> ComponentAssemblyImpl::getCriticalPathDsts()
{
	std::multiset<PathReference> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("PICML::CriticalPath");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
		{
			PathReference dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
//
//********************************************************************************
std::set<CriticalPath> ComponentAssemblyImpl::getOutCriticalPathLinks()
{
	std::set<CriticalPath> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
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
// getter for role "AssemblyConfigProperty" among "AssemblyConfigProperty"s
//********************************************************************************
std::set<AssemblyConfigProperty> ComponentAssemblyImpl::getAssemblyConfigProperty()
{
	std::set<AssemblyConfigProperty> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::AssemblyConfigProperty");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		AssemblyConfigProperty elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "AssemblyDeployRequirement" among "AssemblyDeployRequirement"s
//********************************************************************************
std::set<AssemblyDeployRequirement> ComponentAssemblyImpl::getAssemblyDeployRequirement()
{
	std::set<AssemblyDeployRequirement> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::AssemblyDeployRequirement");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		AssemblyDeployRequirement elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "AssemblyselectRequirement" among "AssemblyselectRequirement"s
//********************************************************************************
std::set<AssemblyselectRequirement> ComponentAssemblyImpl::getAssemblyselectRequirement()
{
	std::set<AssemblyselectRequirement> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::AssemblyselectRequirement");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		AssemblyselectRequirement elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "AttributeDelegate" among "AttributeDelegate"s
//********************************************************************************
std::set<AttributeDelegate> ComponentAssemblyImpl::getAttributeDelegate()
{
	std::set<AttributeDelegate> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::AttributeDelegate");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		AttributeDelegate elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "AttributeMapping" among "AttributeMapping"s
//********************************************************************************
std::set<AttributeMapping> ComponentAssemblyImpl::getAttributeMapping()
{
	std::set<AttributeMapping> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::AttributeMapping");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		AttributeMapping elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "AttributeMappingDelegate" among "AttributeMappingDelegate"s
//********************************************************************************
std::set<AttributeMappingDelegate> ComponentAssemblyImpl::getAttributeMappingDelegate()
{
	std::set<AttributeMappingDelegate> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::AttributeMappingDelegate");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		AttributeMappingDelegate elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "AttributeMappingValue" among "AttributeMappingValue"s
//********************************************************************************
std::set<AttributeMappingValue> ComponentAssemblyImpl::getAttributeMappingValue()
{
	std::set<AttributeMappingValue> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::AttributeMappingValue");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		AttributeMappingValue elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "AttributeValue" among "AttributeValue"s
//********************************************************************************
std::set<AttributeValue> ComponentAssemblyImpl::getAttributeValue()
{
	std::set<AttributeValue> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::AttributeValue");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		AttributeValue elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Component" among "Component"s
//********************************************************************************
std::set<Component> ComponentAssemblyImpl::getComponent()
{
	std::set<Component> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::Component");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Component elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "ComponentAssembly" among "ComponentAssembly"s
//********************************************************************************
std::set<ComponentAssembly> ComponentAssemblyImpl::getComponentAssembly()
{
	std::set<ComponentAssembly> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::ComponentAssembly");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		ComponentAssembly elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "ComponentAssemblyReference" among "ComponentAssemblyReference"s
//********************************************************************************
std::set<ComponentAssemblyReference> ComponentAssemblyImpl::getComponentAssemblyReference()
{
	std::set<ComponentAssemblyReference> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::ComponentAssemblyReference");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		ComponentAssemblyReference elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "ComponentPackage" among "ComponentPackage"s
//********************************************************************************
std::set<ComponentPackage> ComponentAssemblyImpl::getComponentPackage()
{
	std::set<ComponentPackage> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::ComponentPackage");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		ComponentPackage elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "ComponentPackageReference" among "ComponentPackageReference"s
//********************************************************************************
std::set<ComponentPackageReference> ComponentAssemblyImpl::getComponentPackageReference()
{
	std::set<ComponentPackageReference> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::ComponentPackageReference");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		ComponentPackageReference elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "ComponentRef" among "ComponentRef"s
//********************************************************************************
std::set<ComponentRef> ComponentAssemblyImpl::getComponentRef()
{
	std::set<ComponentRef> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::ComponentRef");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		ComponentRef elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "EventSinkDelegate" among "EventSinkDelegate"s
//********************************************************************************
std::set<EventSinkDelegate> ComponentAssemblyImpl::getEventSinkDelegate()
{
	std::set<EventSinkDelegate> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::EventSinkDelegate");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		EventSinkDelegate elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "EventSinkPublish" among "EventSinkPublish"s
//********************************************************************************
std::set<EventSinkPublish> ComponentAssemblyImpl::getEventSinkPublish()
{
	std::set<EventSinkPublish> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::EventSinkPublish");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		EventSinkPublish elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "EventSourceDelegate" among "EventSourceDelegate"s
//********************************************************************************
std::set<EventSourceDelegate> ComponentAssemblyImpl::getEventSourceDelegate()
{
	std::set<EventSourceDelegate> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::EventSourceDelegate");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		EventSourceDelegate elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "EventSourcePublish" among "EventSourcePublish"s
//********************************************************************************
std::set<EventSourcePublish> ComponentAssemblyImpl::getEventSourcePublish()
{
	std::set<EventSourcePublish> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::EventSourcePublish");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		EventSourcePublish elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "ExternalDelegate" among "ExternalDelegate"s
//********************************************************************************
std::set<ExternalDelegate> ComponentAssemblyImpl::getExternalDelegate()
{
	std::set<ExternalDelegate> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::ExternalDelegate");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		ExternalDelegate elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "ExternalReferenceEndPoint" among "ExternalReferenceEndPoint"s
//********************************************************************************
std::set<ExternalReferenceEndPoint> ComponentAssemblyImpl::getExternalReferenceEndPoint()
{
	std::set<ExternalReferenceEndPoint> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::ExternalReferenceEndPoint");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		ExternalReferenceEndPoint elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "FacetDelegate" among "FacetDelegate"s
//********************************************************************************
std::set<FacetDelegate> ComponentAssemblyImpl::getFacetDelegate()
{
	std::set<FacetDelegate> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::FacetDelegate");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		FacetDelegate elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "FacetInvoke" among "FacetInvoke"s
//********************************************************************************
std::set<FacetInvoke> ComponentAssemblyImpl::getFacetInvoke()
{
	std::set<FacetInvoke> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::FacetInvoke");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		FacetInvoke elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "InEventPort" among "InEventPort"s
//********************************************************************************
std::set<InEventPort> ComponentAssemblyImpl::getInEventPort()
{
	std::set<InEventPort> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::InEventPort");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		InEventPort elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "OutEventPort" among "OutEventPort"s
//********************************************************************************
std::set<OutEventPort> ComponentAssemblyImpl::getOutEventPort()
{
	std::set<OutEventPort> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::OutEventPort");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		OutEventPort elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "PortQoS" among "PortQoS"s
//********************************************************************************
std::set<PortQoS> ComponentAssemblyImpl::getPortQoS()
{
	std::set<PortQoS> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::PortQoS");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		PortQoS elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Property" among "Property"s
//********************************************************************************
std::set<Property> ComponentAssemblyImpl::getProperty()
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
// getter for role "ProvidedRequestPort" among "ProvidedRequestPort"s
//********************************************************************************
std::set<ProvidedRequestPort> ComponentAssemblyImpl::getProvidedRequestPort()
{
	std::set<ProvidedRequestPort> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::ProvidedRequestPort");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		ProvidedRequestPort elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "PublishConnector" among "PublishConnector"s
//********************************************************************************
std::set<PublishConnector> ComponentAssemblyImpl::getPublishConnector()
{
	std::set<PublishConnector> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::PublishConnector");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		PublishConnector elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "QoSCharRef" among "ReqQoSBase"s
//********************************************************************************
std::set<QoSCharRef> ComponentAssemblyImpl::getQoSCharRef()
{
	std::set<QoSCharRef> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::QoSCharRef");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		QoSCharRef elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "QoSConnector" among "QoSConnector"s
//********************************************************************************
std::set<QoSConnector> ComponentAssemblyImpl::getQoSConnector()
{
	std::set<QoSConnector> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::QoSConnector");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		QoSConnector elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "QoSReq" among "QoSReq"s
//********************************************************************************
std::set<QoSReq> ComponentAssemblyImpl::getQoSReq()
{
	std::set<QoSReq> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::QoSReq");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		QoSReq elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "RecepInvoke" among "RecepInvoke"s
//********************************************************************************
std::set<RecepInvoke> ComponentAssemblyImpl::getRecepInvoke()
{
	std::set<RecepInvoke> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::RecepInvoke");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		RecepInvoke elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "ReceptacleDelegate" among "ReceptacleDelegate"s
//********************************************************************************
std::set<ReceptacleDelegate> ComponentAssemblyImpl::getReceptacleDelegate()
{
	std::set<ReceptacleDelegate> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::ReceptacleDelegate");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		ReceptacleDelegate elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "RequiredRequestPort" among "RequiredRequestPort"s
//********************************************************************************
std::set<RequiredRequestPort> ComponentAssemblyImpl::getRequiredRequestPort()
{
	std::set<RequiredRequestPort> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::RequiredRequestPort");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		RequiredRequestPort elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Requirement" among "Requirement"s
//********************************************************************************
std::set<Requirement> ComponentAssemblyImpl::getRequirement()
{
	std::set<Requirement> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::Requirement");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Requirement elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "deliverTo" among "deliverTo"s
//********************************************************************************
std::set<deliverTo> ComponentAssemblyImpl::getdeliverTo()
{
	std::set<deliverTo> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::deliverTo");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		deliverTo elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "emit" among "emit"s
//********************************************************************************
std::set<emit> ComponentAssemblyImpl::getemit()
{
	std::set<emit> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::emit");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		emit elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "invoke" among "invoke"s
//********************************************************************************
std::set<invoke> ComponentAssemblyImpl::getinvoke()
{
	std::set<invoke> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::invoke");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		invoke elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "publish" among "publish"s
//********************************************************************************
std::set<publish> ComponentAssemblyImpl::getpublish()
{
	std::set<publish> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::publish");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		publish elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// returns dst ImplementationRequirements
//********************************************************************************
std::multiset<ImplementationRequirement> MonolithicImplementationImpl::getMonolithDeployRequirementDsts()
{
	std::multiset<ImplementationRequirement> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("PICML::MonolithDeployRequirement");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
		{
			ImplementationRequirement dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// returns dst Propertys
//********************************************************************************
std::multiset<Property> MonolithicImplementationImpl::getMonolithExecParameterDsts()
{
	std::multiset<Property> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("PICML::MonolithExecParameter");
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
// returns dst ImplementationArtifactReferences
//********************************************************************************
std::multiset<ImplementationArtifactReference> MonolithicImplementationImpl::getMonolithprimaryArtifactDsts()
{
	std::multiset<ImplementationArtifactReference> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("PICML::MonolithprimaryArtifact");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
		{
			ImplementationArtifactReference dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
//
//********************************************************************************
std::set<MonolithDeployRequirement> MonolithicImplementationImpl::getOutMonolithDeployRequirementLinks()
{
	std::set<MonolithDeployRequirement> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
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
//
//********************************************************************************
std::set<MonolithExecParameter> MonolithicImplementationImpl::getOutMonolithExecParameterLinks()
{
	std::set<MonolithExecParameter> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		MonolithExecParameter c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
//
//********************************************************************************
std::set<MonolithprimaryArtifact> MonolithicImplementationImpl::getOutMonolithprimaryArtifactLinks()
{
	std::set<MonolithprimaryArtifact> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
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
// getter for role "ArtifactDependency" among "ArtifactDependency"s
//********************************************************************************
std::set<ArtifactDependency> ArtifactContainerImpl::getArtifactDependency()
{
	std::set<ArtifactDependency> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::ArtifactDependency");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		ArtifactDependency elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "ArtifactDependsOn" among "ArtifactDependsOn"s
//********************************************************************************
std::set<ArtifactDependsOn> ArtifactContainerImpl::getArtifactDependsOn()
{
	std::set<ArtifactDependsOn> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::ArtifactDependsOn");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		ArtifactDependsOn elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "ArtifactDeployRequirement" among "ArtifactDeployRequirement"s
//********************************************************************************
std::set<ArtifactDeployRequirement> ArtifactContainerImpl::getArtifactDeployRequirement()
{
	std::set<ArtifactDeployRequirement> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::ArtifactDeployRequirement");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		ArtifactDeployRequirement elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "ArtifactExecParameter" among "ArtifactExecParameter"s
//********************************************************************************
std::set<ArtifactExecParameter> ArtifactContainerImpl::getArtifactExecParameter()
{
	std::set<ArtifactExecParameter> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::ArtifactExecParameter");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		ArtifactExecParameter elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "ArtifactInfoProperty" among "ArtifactInfoProperty"s
//********************************************************************************
std::set<ArtifactInfoProperty> ArtifactContainerImpl::getArtifactInfoProperty()
{
	std::set<ArtifactInfoProperty> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::ArtifactInfoProperty");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		ArtifactInfoProperty elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "ImplementationArtifact" among "ImplementationArtifact"s
//********************************************************************************
std::set<ImplementationArtifact> ArtifactContainerImpl::getImplementationArtifact()
{
	std::set<ImplementationArtifact> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::ImplementationArtifact");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		ImplementationArtifact elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "ImplementationArtifactReference" among "ImplementationArtifactReference"s
//********************************************************************************
std::set<ImplementationArtifactReference> ArtifactContainerImpl::getImplementationArtifactReference()
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
// getter for role "Property" among "Property"s
//********************************************************************************
std::set<Property> ArtifactContainerImpl::getProperty()
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
// getter for role "Requirement" among "Requirement"s
//********************************************************************************
std::set<Requirement> ArtifactContainerImpl::getRequirement()
{
	std::set<Requirement> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::Requirement");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Requirement elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Average" among "DataAnalysisBase"s and its descendants
//********************************************************************************
std::set<Average> BenchmarkAnalysisImpl::getAverage()
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
// getter for role "BenchmarkCharacteristics" among "BenchmarkCharacteristics"s
//********************************************************************************
std::set<BenchmarkCharacteristics> BenchmarkAnalysisImpl::getBenchmarkCharacteristics()
{
	std::set<BenchmarkCharacteristics> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::BenchmarkCharacteristics");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		BenchmarkCharacteristics elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// aggregated getter for role "" among "BenchmarkType"s and its descendants
//********************************************************************************
std::set<BenchmarkType> BenchmarkAnalysisImpl::getBenchmarkType()
{
	std::set<BenchmarkType> res;
	const int len = 3;
	std::set<BON::FCO> roles_vec[ len];
	roles_vec[0] = ModelImpl::getChildFCOsAs("PICML::FixedIterationBenchmarks");
	roles_vec[1] = ModelImpl::getChildFCOsAs("PICML::PeriodicBenchmarks");
	roles_vec[2] = ModelImpl::getChildFCOsAs("PICML::TriggerBenchmarks");
	for( int k = 0; k < len; ++k)
		for( std::set<BON::FCO>::iterator i = roles_vec[k].begin(); i != roles_vec[k].end(); ++i)
		{
			BenchmarkType elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
	return res;
}


//********************************************************************************
// getter for role "CompRef" among "CompRef"s
//********************************************************************************
std::set<CompRef> BenchmarkAnalysisImpl::getCompRef()
{
	std::set<CompRef> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::CompRef");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		CompRef elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "ComponentOperation" among "ComponentOperation"s
//********************************************************************************
std::set<ComponentOperation> BenchmarkAnalysisImpl::getComponentOperation()
{
	std::set<ComponentOperation> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::ComponentOperation");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		ComponentOperation elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// aggregated getter for role "" among "DataAnalysisBase"s and its descendants
//********************************************************************************
std::set<DataAnalysisBase> BenchmarkAnalysisImpl::getDataAnalysisBase()
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
// getter for role "Event" among "Event"s
//********************************************************************************
std::set<Event> BenchmarkAnalysisImpl::getEvent()
{
	std::set<Event> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::Event");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Event elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "EventRef" among "EventRef"s
//********************************************************************************
std::set<EventRef> BenchmarkAnalysisImpl::getEventRef()
{
	std::set<EventRef> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::EventRef");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		EventRef elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "FactoryOperation" among "OperationBase"s and its descendants
//********************************************************************************
std::set<FactoryOperation> BenchmarkAnalysisImpl::getFactoryOperation()
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
// getter for role "FixedIterationBenchmarks" among "BenchmarkType"s and its descendants
//********************************************************************************
std::set<FixedIterationBenchmarks> BenchmarkAnalysisImpl::getFixedIterationBenchmarks()
{
	std::set<FixedIterationBenchmarks> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::FixedIterationBenchmarks");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		FixedIterationBenchmarks elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Jitter" among "DataAnalysisBase"s and its descendants
//********************************************************************************
std::set<Jitter> BenchmarkAnalysisImpl::getJitter()
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
// getter for role "Latency" among "MetricsBase"s and its descendants
//********************************************************************************
std::set<Latency> BenchmarkAnalysisImpl::getLatency()
{
	std::set<Latency> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::Latency");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Latency elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "LookupOperation" among "OperationBase"s and its descendants
//********************************************************************************
std::set<LookupOperation> BenchmarkAnalysisImpl::getLookupOperation()
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
// getter for role "Maximum" among "DataAnalysisBase"s and its descendants
//********************************************************************************
std::set<Maximum> BenchmarkAnalysisImpl::getMaximum()
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
// getter for role "MetricConnection" among "MetricConnection"s
//********************************************************************************
std::set<MetricConnection> BenchmarkAnalysisImpl::getMetricConnection()
{
	std::set<MetricConnection> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::MetricConnection");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		MetricConnection elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// aggregated getter for role "" among "MetricsBase"s and its descendants
//********************************************************************************
std::set<MetricsBase> BenchmarkAnalysisImpl::getMetricsBase()
{
	std::set<MetricsBase> res;
	const int len = 2;
	std::set<BON::FCO> roles_vec[ len];
	roles_vec[0] = ModelImpl::getChildFCOsAs("PICML::Latency");
	roles_vec[1] = ModelImpl::getChildFCOsAs("PICML::Throughput");
	for( int k = 0; k < len; ++k)
		for( std::set<BON::FCO>::iterator i = roles_vec[k].begin(); i != roles_vec[k].end(); ++i)
		{
			MetricsBase elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
	return res;
}


//********************************************************************************
// getter for role "Minimum" among "DataAnalysisBase"s and its descendants
//********************************************************************************
std::set<Minimum> BenchmarkAnalysisImpl::getMinimum()
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
// getter for role "OnewayOperation" among "OperationBase"s and its descendants
//********************************************************************************
std::set<OnewayOperation> BenchmarkAnalysisImpl::getOnewayOperation()
{
	std::set<OnewayOperation> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::OnewayOperation");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		OnewayOperation elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// aggregated getter for role "" among "OperationBase"s and its descendants
//********************************************************************************
std::set<OperationBase> BenchmarkAnalysisImpl::getOperationBase()
{
	std::set<OperationBase> res;
	const int len = 4;
	std::set<BON::FCO> roles_vec[ len];
	roles_vec[0] = ModelImpl::getChildFCOsAs("PICML::FactoryOperation");
	roles_vec[1] = ModelImpl::getChildFCOsAs("PICML::LookupOperation");
	roles_vec[2] = ModelImpl::getChildFCOsAs("PICML::OnewayOperation");
	roles_vec[3] = ModelImpl::getChildFCOsAs("PICML::TwowayOperation");
	for( int k = 0; k < len; ++k)
		for( std::set<BON::FCO>::iterator i = roles_vec[k].begin(); i != roles_vec[k].end(); ++i)
		{
			OperationBase elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
	return res;
}


//********************************************************************************
// getter for role "OperationRef" among "OperationRef"s
//********************************************************************************
std::set<OperationRef> BenchmarkAnalysisImpl::getOperationRef()
{
	std::set<OperationRef> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::OperationRef");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		OperationRef elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "PeriodicBenchmarks" among "BenchmarkType"s and its descendants
//********************************************************************************
std::set<PeriodicBenchmarks> BenchmarkAnalysisImpl::getPeriodicBenchmarks()
{
	std::set<PeriodicBenchmarks> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::PeriodicBenchmarks");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		PeriodicBenchmarks elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Task" among "Task"s
//********************************************************************************
std::set<Task> BenchmarkAnalysisImpl::getTask()
{
	std::set<Task> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::Task");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Task elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "TaskSet" among "TaskSet"s
//********************************************************************************
std::set<TaskSet> BenchmarkAnalysisImpl::getTaskSet()
{
	std::set<TaskSet> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::TaskSet");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		TaskSet elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Throughput" among "MetricsBase"s and its descendants
//********************************************************************************
std::set<Throughput> BenchmarkAnalysisImpl::getThroughput()
{
	std::set<Throughput> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::Throughput");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Throughput elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "TimeProbe" among "TimeProbe"s
//********************************************************************************
std::set<TimeProbe> BenchmarkAnalysisImpl::getTimeProbe()
{
	std::set<TimeProbe> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::TimeProbe");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		TimeProbe elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "TimerConnection" among "TimerConnection"s
//********************************************************************************
std::set<TimerConnection> BenchmarkAnalysisImpl::getTimerConnection()
{
	std::set<TimerConnection> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::TimerConnection");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		TimerConnection elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "TimerEventSinkConn" among "TimerEventSinkConn"s
//********************************************************************************
std::set<TimerEventSinkConn> BenchmarkAnalysisImpl::getTimerEventSinkConn()
{
	std::set<TimerEventSinkConn> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::TimerEventSinkConn");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		TimerEventSinkConn elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "TriggerBenchmarks" among "BenchmarkType"s and its descendants
//********************************************************************************
std::set<TriggerBenchmarks> BenchmarkAnalysisImpl::getTriggerBenchmarks()
{
	std::set<TriggerBenchmarks> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::TriggerBenchmarks");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		TriggerBenchmarks elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "TwowayOperation" among "OperationBase"s and its descendants
//********************************************************************************
std::set<TwowayOperation> BenchmarkAnalysisImpl::getTwowayOperation()
{
	std::set<TwowayOperation> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::TwowayOperation");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		TwowayOperation elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "WorkLoadOperationConnection" among "WorkLoadOperationConnection"s
//********************************************************************************
std::set<WorkLoadOperationConnection> BenchmarkAnalysisImpl::getWorkLoadOperationConnection()
{
	std::set<WorkLoadOperationConnection> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::WorkLoadOperationConnection");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		WorkLoadOperationConnection elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "WorkloadCharacteristics" among "WorkloadCharacteristics"s
//********************************************************************************
std::set<WorkloadCharacteristics> BenchmarkAnalysisImpl::getWorkloadCharacteristics()
{
	std::set<WorkloadCharacteristics> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::WorkloadCharacteristics");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		WorkloadCharacteristics elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "ComponentConfigProperty" among "ComponentConfigProperty"s
//********************************************************************************
std::set<ComponentConfigProperty> ComponentContainerImpl::getComponentConfigProperty()
{
	std::set<ComponentConfigProperty> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::ComponentConfigProperty");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		ComponentConfigProperty elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "ComponentInfoProperty" among "ComponentInfoProperty"s
//********************************************************************************
std::set<ComponentInfoProperty> ComponentContainerImpl::getComponentInfoProperty()
{
	std::set<ComponentInfoProperty> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::ComponentInfoProperty");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		ComponentInfoProperty elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "ComponentProperty" among "ComponentProperty"s
//********************************************************************************
std::set<ComponentProperty> ComponentContainerImpl::getComponentProperty()
{
	std::set<ComponentProperty> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::ComponentProperty");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		ComponentProperty elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "ComponentPropertyDescription" among "ComponentPropertyDescription"s
//********************************************************************************
std::set<ComponentPropertyDescription> ComponentContainerImpl::getComponentPropertyDescription()
{
	std::set<ComponentPropertyDescription> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::ComponentPropertyDescription");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		ComponentPropertyDescription elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "ComponentRef" among "ComponentRef"s
//********************************************************************************
std::set<ComponentRef> ComponentContainerImpl::getComponentRef()
{
	std::set<ComponentRef> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::ComponentRef");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		ComponentRef elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Property" among "Property"s
//********************************************************************************
std::set<Property> ComponentContainerImpl::getProperty()
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
// getter for role "Capability" among "Capability"s
//********************************************************************************
std::set<Capability> ComponentImplementationContainerImpl::getCapability()
{
	std::set<Capability> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::Capability");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Capability elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "ComponentAssembly" among "ComponentImplementation"s and its descendants
//********************************************************************************
std::set<ComponentAssembly> ComponentImplementationContainerImpl::getComponentAssembly()
{
	std::set<ComponentAssembly> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::ComponentAssembly");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		ComponentAssembly elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// aggregated getter for role "" among "ComponentImplementation"s and its descendants
//********************************************************************************
std::set<ComponentImplementation> ComponentImplementationContainerImpl::getComponentImplementation()
{
	std::set<ComponentImplementation> res;
	const int len = 2;
	std::set<BON::FCO> roles_vec[ len];
	roles_vec[0] = ModelImpl::getChildFCOsAs("PICML::MonolithicImplementation");
	roles_vec[1] = ModelImpl::getChildFCOsAs("PICML::ComponentAssembly");
	for( int k = 0; k < len; ++k)
		for( std::set<BON::FCO>::iterator i = roles_vec[k].begin(); i != roles_vec[k].end(); ++i)
		{
			ComponentImplementation elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
	return res;
}


//********************************************************************************
// getter for role "ComponentRef" among "ComponentRef"s
//********************************************************************************
std::set<ComponentRef> ComponentImplementationContainerImpl::getComponentRef()
{
	std::set<ComponentRef> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::ComponentRef");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		ComponentRef elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "ConfigProperty" among "ConfigProperty"s
//********************************************************************************
std::set<ConfigProperty> ComponentImplementationContainerImpl::getConfigProperty()
{
	std::set<ConfigProperty> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::ConfigProperty");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		ConfigProperty elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "CriticalPath" among "CriticalPath"s
//********************************************************************************
std::set<CriticalPath> ComponentImplementationContainerImpl::getCriticalPath()
{
	std::set<CriticalPath> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::CriticalPath");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		CriticalPath elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "ImplementationArtifactReference" among "ImplementationArtifactReference"s
//********************************************************************************
std::set<ImplementationArtifactReference> ComponentImplementationContainerImpl::getImplementationArtifactReference()
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
// getter for role "ImplementationCapability" among "ImplementationCapability"s
//********************************************************************************
std::set<ImplementationCapability> ComponentImplementationContainerImpl::getImplementationCapability()
{
	std::set<ImplementationCapability> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::ImplementationCapability");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		ImplementationCapability elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "ImplementationDependency" among "ImplementationDependency"s
//********************************************************************************
std::set<ImplementationDependency> ComponentImplementationContainerImpl::getImplementationDependency()
{
	std::set<ImplementationDependency> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::ImplementationDependency");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		ImplementationDependency elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "ImplementationDependsOn" among "ImplementationDependsOn"s
//********************************************************************************
std::set<ImplementationDependsOn> ComponentImplementationContainerImpl::getImplementationDependsOn()
{
	std::set<ImplementationDependsOn> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::ImplementationDependsOn");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		ImplementationDependsOn elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "ImplementationRequirement" among "ImplementationRequirement"s
//********************************************************************************
std::set<ImplementationRequirement> ComponentImplementationContainerImpl::getImplementationRequirement()
{
	std::set<ImplementationRequirement> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::ImplementationRequirement");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		ImplementationRequirement elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Implements" among "Implements"s
//********************************************************************************
std::set<Implements> ComponentImplementationContainerImpl::getImplements()
{
	std::set<Implements> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::Implements");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Implements elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "InfoProperty" among "InfoProperty"s
//********************************************************************************
std::set<InfoProperty> ComponentImplementationContainerImpl::getInfoProperty()
{
	std::set<InfoProperty> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::InfoProperty");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		InfoProperty elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "MonolithDeployRequirement" among "MonolithDeployRequirement"s
//********************************************************************************
std::set<MonolithDeployRequirement> ComponentImplementationContainerImpl::getMonolithDeployRequirement()
{
	std::set<MonolithDeployRequirement> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::MonolithDeployRequirement");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		MonolithDeployRequirement elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "MonolithExecParameter" among "MonolithExecParameter"s
//********************************************************************************
std::set<MonolithExecParameter> ComponentImplementationContainerImpl::getMonolithExecParameter()
{
	std::set<MonolithExecParameter> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::MonolithExecParameter");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		MonolithExecParameter elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "MonolithicImplementation" among "ComponentImplementation"s and its descendants
//********************************************************************************
std::set<MonolithicImplementation> ComponentImplementationContainerImpl::getMonolithicImplementation()
{
	std::set<MonolithicImplementation> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::MonolithicImplementation");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		MonolithicImplementation elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "MonolithprimaryArtifact" among "MonolithprimaryArtifact"s
//********************************************************************************
std::set<MonolithprimaryArtifact> ComponentImplementationContainerImpl::getMonolithprimaryArtifact()
{
	std::set<MonolithprimaryArtifact> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::MonolithprimaryArtifact");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		MonolithprimaryArtifact elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "PathReference" among "PathReference"s
//********************************************************************************
std::set<PathReference> ComponentImplementationContainerImpl::getPathReference()
{
	std::set<PathReference> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::PathReference");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		PathReference elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Property" among "Property"s
//********************************************************************************
std::set<Property> ComponentImplementationContainerImpl::getProperty()
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
// returns src ComponentRefs
//********************************************************************************
std::multiset<ComponentRef> ComponentPropertyDescriptionImpl::getComponentPropertySrcs()
{
	std::multiset<ComponentRef> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("PICML::ComponentProperty");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			ComponentRef dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
//
//********************************************************************************
std::set<ComponentProperty> ComponentPropertyDescriptionImpl::getInComponentPropertyLinks()
{
	std::set<ComponentProperty> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		ComponentProperty c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// getter for role "DataType" among "DataType"s
//********************************************************************************
std::set<DataType> ComponentPropertyDescriptionImpl::getDataType()
{
	std::set<DataType> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::DataType");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		DataType elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
//
//********************************************************************************
std::string DeploymentPlanImpl::getUUID()
{
	return FCOImpl::getAttribute("UUID")->getStringValue();
}


//********************************************************************************
//
//********************************************************************************
std::string DeploymentPlanImpl::getlabel()
{
	return FCOImpl::getAttribute("label")->getStringValue();
}


//********************************************************************************
//
//********************************************************************************
void DeploymentPlanImpl::setUUID( const std::string& val)
{
	FCOImpl::getAttribute("UUID")->setStringValue( val);
}


//********************************************************************************
//
//********************************************************************************
void DeploymentPlanImpl::setlabel( const std::string& val)
{
	FCOImpl::getAttribute("label")->setStringValue( val);
}


//********************************************************************************
// getter for role "CollocationGroup" among "CollocationGroup"s
//********************************************************************************
std::set<CollocationGroup> DeploymentPlanImpl::getCollocationGroup()
{
	std::set<CollocationGroup> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::CollocationGroup");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		CollocationGroup elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "ComponentAssemblyReference" among "ComponentAssemblyReference"s
//********************************************************************************
std::set<ComponentAssemblyReference> DeploymentPlanImpl::getComponentAssemblyReference()
{
	std::set<ComponentAssemblyReference> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::ComponentAssemblyReference");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		ComponentAssemblyReference elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "ComponentRef" among "ComponentRef"s
//********************************************************************************
std::set<ComponentRef> DeploymentPlanImpl::getComponentRef()
{
	std::set<ComponentRef> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::ComponentRef");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		ComponentRef elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "InstanceMapping" among "InstanceMapping"s
//********************************************************************************
std::set<InstanceMapping> DeploymentPlanImpl::getInstanceMapping()
{
	std::set<InstanceMapping> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::InstanceMapping");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		InstanceMapping elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "NodeReference" among "NodeReference"s
//********************************************************************************
std::set<NodeReference> DeploymentPlanImpl::getNodeReference()
{
	std::set<NodeReference> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::NodeReference");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		NodeReference elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "SharedComponentReference" among "SharedComponentReference"s
//********************************************************************************
std::set<SharedComponentReference> DeploymentPlanImpl::getSharedComponentReference()
{
	std::set<SharedComponentReference> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::SharedComponentReference");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		SharedComponentReference elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
//
//********************************************************************************
std::string DomainImpl::getUUID()
{
	return FCOImpl::getAttribute("UUID")->getStringValue();
}


//********************************************************************************
//
//********************************************************************************
std::string DomainImpl::getlabel()
{
	return FCOImpl::getAttribute("label")->getStringValue();
}


//********************************************************************************
//
//********************************************************************************
void DomainImpl::setUUID( const std::string& val)
{
	FCOImpl::getAttribute("UUID")->setStringValue( val);
}


//********************************************************************************
//
//********************************************************************************
void DomainImpl::setlabel( const std::string& val)
{
	FCOImpl::getAttribute("label")->setStringValue( val);
}


//********************************************************************************
// getter for role "Bridge" among "Elements"s and its descendants
//********************************************************************************
std::set<Bridge> DomainImpl::getBridge()
{
	std::set<Bridge> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::Bridge");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Bridge elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Bridge2Interconnect" among "Bridge2Interconnect"s
//********************************************************************************
std::set<Bridge2Interconnect> DomainImpl::getBridge2Interconnect()
{
	std::set<Bridge2Interconnect> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::Bridge2Interconnect");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Bridge2Interconnect elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// aggregated getter for role "" among "Elements"s and its descendants
//********************************************************************************
std::set<Elements> DomainImpl::getElements()
{
	std::set<Elements> res;
	const int len = 4;
	std::set<BON::FCO> roles_vec[ len];
	roles_vec[0] = ModelImpl::getChildFCOsAs("PICML::Bridge");
	roles_vec[1] = ModelImpl::getChildFCOsAs("PICML::Interconnect");
	roles_vec[2] = ModelImpl::getChildFCOsAs("PICML::Node");
	roles_vec[3] = ModelImpl::getChildFCOsAs("PICML::SharedResource");
	for( int k = 0; k < len; ++k)
		for( std::set<BON::FCO>::iterator i = roles_vec[k].begin(); i != roles_vec[k].end(); ++i)
		{
			Elements elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
	return res;
}


//********************************************************************************
// getter for role "Interconnect" among "Elements"s and its descendants
//********************************************************************************
std::set<Interconnect> DomainImpl::getInterconnect()
{
	std::set<Interconnect> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::Interconnect");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Interconnect elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Interconnect2Bridge" among "Interconnect2Bridge"s
//********************************************************************************
std::set<Interconnect2Bridge> DomainImpl::getInterconnect2Bridge()
{
	std::set<Interconnect2Bridge> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::Interconnect2Bridge");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Interconnect2Bridge elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Interconnect2Node" among "Interconnect2Node"s
//********************************************************************************
std::set<Interconnect2Node> DomainImpl::getInterconnect2Node()
{
	std::set<Interconnect2Node> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::Interconnect2Node");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Interconnect2Node elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Node" among "Elements"s and its descendants
//********************************************************************************
std::set<Node> DomainImpl::getNode()
{
	std::set<Node> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::Node");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Node elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Node2Interconnect" among "Node2Interconnect"s
//********************************************************************************
std::set<Node2Interconnect> DomainImpl::getNode2Interconnect()
{
	std::set<Node2Interconnect> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::Node2Interconnect");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Node2Interconnect elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Property" among "Property"s
//********************************************************************************
std::set<Property> DomainImpl::getProperty()
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
// getter for role "SharedResource" among "Elements"s and its descendants
//********************************************************************************
std::set<SharedResource> DomainImpl::getSharedResource()
{
	std::set<SharedResource> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::SharedResource");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		SharedResource elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Shares" among "Shares"s
//********************************************************************************
std::set<Shares> DomainImpl::getShares()
{
	std::set<Shares> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::Shares");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Shares elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Project" among "Project"s
//********************************************************************************
std::set<Project> MPCImpl::getProject()
{
	std::set<Project> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::Project");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Project elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Workspaces" among "Workspaces"s
//********************************************************************************
std::set<Workspaces> MPCImpl::getWorkspaces()
{
	std::set<Workspaces> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::Workspaces");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Workspaces elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "ComponentPackage" among "ComponentPackage"s
//********************************************************************************
std::set<ComponentPackage> PackageConfigurationContainerImpl::getComponentPackage()
{
	std::set<ComponentPackage> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::ComponentPackage");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		ComponentPackage elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "ComponentPackageReference" among "ComponentPackageReference"s
//********************************************************************************
std::set<ComponentPackageReference> PackageConfigurationContainerImpl::getComponentPackageReference()
{
	std::set<ComponentPackageReference> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::ComponentPackageReference");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		ComponentPackageReference elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "PackageConfBasePackage" among "PackageConfBasePackage"s
//********************************************************************************
std::set<PackageConfBasePackage> PackageConfigurationContainerImpl::getPackageConfBasePackage()
{
	std::set<PackageConfBasePackage> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::PackageConfBasePackage");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		PackageConfBasePackage elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "PackageConfConfigProperty" among "PackageConfConfigProperty"s
//********************************************************************************
std::set<PackageConfConfigProperty> PackageConfigurationContainerImpl::getPackageConfConfigProperty()
{
	std::set<PackageConfConfigProperty> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::PackageConfConfigProperty");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		PackageConfConfigProperty elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "PackageConfReference" among "PackageConfReference"s
//********************************************************************************
std::set<PackageConfReference> PackageConfigurationContainerImpl::getPackageConfReference()
{
	std::set<PackageConfReference> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::PackageConfReference");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		PackageConfReference elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "PackageConfSelectRequirement" among "PackageConfSelectRequirement"s
//********************************************************************************
std::set<PackageConfSelectRequirement> PackageConfigurationContainerImpl::getPackageConfSelectRequirement()
{
	std::set<PackageConfSelectRequirement> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::PackageConfSelectRequirement");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		PackageConfSelectRequirement elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "PackageConfSpecializedConfig" among "PackageConfSpecializedConfig"s
//********************************************************************************
std::set<PackageConfSpecializedConfig> PackageConfigurationContainerImpl::getPackageConfSpecializedConfig()
{
	std::set<PackageConfSpecializedConfig> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::PackageConfSpecializedConfig");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		PackageConfSpecializedConfig elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "PackageConfiguration" among "PackageConfiguration"s
//********************************************************************************
std::set<PackageConfiguration> PackageConfigurationContainerImpl::getPackageConfiguration()
{
	std::set<PackageConfiguration> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::PackageConfiguration");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		PackageConfiguration elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Property" among "Property"s
//********************************************************************************
std::set<Property> PackageConfigurationContainerImpl::getProperty()
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
// getter for role "Requirement" among "Requirement"s
//********************************************************************************
std::set<Requirement> PackageConfigurationContainerImpl::getRequirement()
{
	std::set<Requirement> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::Requirement");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Requirement elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "ComponentImplementationReference" among "ComponentImplementationReference"s
//********************************************************************************
std::set<ComponentImplementationReference> PackageContainerImpl::getComponentImplementationReference()
{
	std::set<ComponentImplementationReference> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::ComponentImplementationReference");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		ComponentImplementationReference elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "ComponentPackage" among "ComponentPackage"s
//********************************************************************************
std::set<ComponentPackage> PackageContainerImpl::getComponentPackage()
{
	std::set<ComponentPackage> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::ComponentPackage");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		ComponentPackage elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "ComponentRef" among "ComponentRef"s
//********************************************************************************
std::set<ComponentRef> PackageContainerImpl::getComponentRef()
{
	std::set<ComponentRef> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::ComponentRef");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		ComponentRef elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Implementation" among "Implementation"s
//********************************************************************************
std::set<Implementation> PackageContainerImpl::getImplementation()
{
	std::set<Implementation> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::Implementation");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Implementation elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "PackageConfigProperty" among "PackageConfigProperty"s
//********************************************************************************
std::set<PackageConfigProperty> PackageContainerImpl::getPackageConfigProperty()
{
	std::set<PackageConfigProperty> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::PackageConfigProperty");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		PackageConfigProperty elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "PackageInfoProperty" among "PackageInfoProperty"s
//********************************************************************************
std::set<PackageInfoProperty> PackageContainerImpl::getPackageInfoProperty()
{
	std::set<PackageInfoProperty> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::PackageInfoProperty");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		PackageInfoProperty elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "PackageInterface" among "PackageInterface"s
//********************************************************************************
std::set<PackageInterface> PackageContainerImpl::getPackageInterface()
{
	std::set<PackageInterface> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::PackageInterface");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		PackageInterface elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Property" among "Property"s
//********************************************************************************
std::set<Property> PackageContainerImpl::getProperty()
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
std::set<PathProperty> PathImpl::getOutPathPropertyLinks()
{
	std::set<PathProperty> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		PathProperty c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// returns dst Propertys
//********************************************************************************
std::multiset<Property> PathImpl::getPathPropertyDsts()
{
	std::multiset<Property> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("PICML::PathProperty");
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
// getter for role "ComponentRef" among "ComponentRef"s
//********************************************************************************
std::set<ComponentRef> PathImpl::getComponentRef()
{
	std::set<ComponentRef> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::ComponentRef");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		ComponentRef elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "ConnectedComponent" among "GraphVertex"s and its descendants
//********************************************************************************
std::set<ConnectedComponent> PathImpl::getConnectedComponent()
{
	std::set<ConnectedComponent> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::ConnectedComponent");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		ConnectedComponent elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "DisplayNode" among "GraphVertex"s and its descendants
//********************************************************************************
std::set<DisplayNode> PathImpl::getDisplayNode()
{
	std::set<DisplayNode> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::DisplayNode");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		DisplayNode elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "DstEdge" among "DstEdge"s
//********************************************************************************
std::set<DstEdge> PathImpl::getDstEdge()
{
	std::set<DstEdge> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::DstEdge");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		DstEdge elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Edge" among "Edge"s
//********************************************************************************
std::set<Edge> PathImpl::getEdge()
{
	std::set<Edge> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::Edge");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Edge elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "EdgeProperty" among "EdgeProperty"s
//********************************************************************************
std::set<EdgeProperty> PathImpl::getEdgeProperty()
{
	std::set<EdgeProperty> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::EdgeProperty");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		EdgeProperty elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// aggregated getter for role "" among "GraphVertex"s and its descendants
//********************************************************************************
std::set<GraphVertex> PathImpl::getGraphVertex()
{
	std::set<GraphVertex> res;
	const int len = 6;
	std::set<BON::FCO> roles_vec[ len];
	roles_vec[0] = ModelImpl::getChildFCOsAs("PICML::DisplayNode");
	roles_vec[1] = ModelImpl::getChildFCOsAs("PICML::ConnectedComponent");
	roles_vec[2] = ModelImpl::getChildFCOsAs("PICML::InEventPort");
	roles_vec[3] = ModelImpl::getChildFCOsAs("PICML::OutEventPort");
	roles_vec[4] = ModelImpl::getChildFCOsAs("PICML::ProvidedRequestPort");
	roles_vec[5] = ModelImpl::getChildFCOsAs("PICML::RequiredRequestPort");
	for( int k = 0; k < len; ++k)
		for( std::set<BON::FCO>::iterator i = roles_vec[k].begin(); i != roles_vec[k].end(); ++i)
		{
			GraphVertex elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
	return res;
}


//********************************************************************************
// getter for role "InEventPort" among "GraphVertex"s and its descendants
//********************************************************************************
std::set<InEventPort> PathImpl::getInEventPort()
{
	std::set<InEventPort> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::InEventPort");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		InEventPort elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "OutEventPort" among "GraphVertex"s and its descendants
//********************************************************************************
std::set<OutEventPort> PathImpl::getOutEventPort()
{
	std::set<OutEventPort> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::OutEventPort");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		OutEventPort elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Property" among "Property"s
//********************************************************************************
std::set<Property> PathImpl::getProperty()
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
// getter for role "ProvidedRequestPort" among "GraphVertex"s and its descendants
//********************************************************************************
std::set<ProvidedRequestPort> PathImpl::getProvidedRequestPort()
{
	std::set<ProvidedRequestPort> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::ProvidedRequestPort");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		ProvidedRequestPort elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "RequiredRequestPort" among "GraphVertex"s and its descendants
//********************************************************************************
std::set<RequiredRequestPort> PathImpl::getRequiredRequestPort()
{
	std::set<RequiredRequestPort> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::RequiredRequestPort");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		RequiredRequestPort elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "SrcEdge" among "SrcEdge"s
//********************************************************************************
std::set<SrcEdge> PathImpl::getSrcEdge()
{
	std::set<SrcEdge> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::SrcEdge");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		SrcEdge elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Path" among "Path"s
//********************************************************************************
std::set<Path> PathsImpl::getPath()
{
	std::set<Path> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::Path");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Path elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "PathProperty" among "PathProperty"s
//********************************************************************************
std::set<PathProperty> PathsImpl::getPathProperty()
{
	std::set<PathProperty> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::PathProperty");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		PathProperty elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Property" among "Property"s
//********************************************************************************
std::set<Property> PathsImpl::getProperty()
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
// aggregated getter for role "" among "ComponentLib"s and its descendants
//********************************************************************************
std::set<ComponentLib> ProjectImpl::getComponentLib()
{
	std::set<ComponentLib> res;
	const int len = 3;
	std::set<BON::FCO> roles_vec[ len];
	roles_vec[0] = ModelImpl::getChildFCOsAs("PICML::ExecutorProject");
	roles_vec[1] = ModelImpl::getChildFCOsAs("PICML::ServantProject");
	roles_vec[2] = ModelImpl::getChildFCOsAs("PICML::StubProject");
	for( int k = 0; k < len; ++k)
		for( std::set<BON::FCO>::iterator i = roles_vec[k].begin(); i != roles_vec[k].end(); ++i)
		{
			ComponentLib elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
	return res;
}


//********************************************************************************
// getter for role "ExecutorProject" among "ComponentLib"s and its descendants
//********************************************************************************
std::set<ExecutorProject> ProjectImpl::getExecutorProject()
{
	std::set<ExecutorProject> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::ExecutorProject");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		ExecutorProject elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "ExtResourceConn" among "ExtResourceConn"s
//********************************************************************************
std::set<ExtResourceConn> ProjectImpl::getExtResourceConn()
{
	std::set<ExtResourceConn> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::ExtResourceConn");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		ExtResourceConn elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "ExternalResources" among "ExternalResources"s
//********************************************************************************
std::set<ExternalResources> ProjectImpl::getExternalResources()
{
	std::set<ExternalResources> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::ExternalResources");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		ExternalResources elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "ImplementationArtifact" among "ImplementationArtifact"s
//********************************************************************************
std::set<ImplementationArtifact> ProjectImpl::getImplementationArtifact()
{
	std::set<ImplementationArtifact> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::ImplementationArtifact");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		ImplementationArtifact elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "ServantProject" among "ComponentLib"s and its descendants
//********************************************************************************
std::set<ServantProject> ProjectImpl::getServantProject()
{
	std::set<ServantProject> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::ServantProject");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		ServantProject elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "StubProject" among "ComponentLib"s and its descendants
//********************************************************************************
std::set<StubProject> ProjectImpl::getStubProject()
{
	std::set<StubProject> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::StubProject");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		StubProject elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// returns src ImplementationArtifacts
//********************************************************************************
std::multiset<ImplementationArtifact> PropertyImpl::getArtifactExecParameterSrcs()
{
	std::multiset<ImplementationArtifact> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("PICML::ArtifactExecParameter");
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
// returns src ImplementationArtifacts
//********************************************************************************
std::multiset<ImplementationArtifact> PropertyImpl::getArtifactInfoPropertySrcs()
{
	std::multiset<ImplementationArtifact> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("PICML::ArtifactInfoProperty");
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
// returns src Components
//********************************************************************************
std::multiset<Component> PropertyImpl::getAssemblyConfigPropertySrcs()
{
	std::multiset<Component> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("PICML::AssemblyConfigProperty");
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
// returns src AttributeMappings and referenceports
//********************************************************************************
std::multiset<BON::ConnectionEnd> PropertyImpl::getAttributeMappingValueSrcs()
{
	return BON::ConnectionEndImpl::getInConnEnds("PICML::AttributeMappingValue");
}


//********************************************************************************
// returns src ReadonlyAttributes and referenceports
//********************************************************************************
std::multiset<BON::ConnectionEnd> PropertyImpl::getAttributeValueSrcs()
{
	return BON::ConnectionEndImpl::getInConnEnds("PICML::AttributeValue");
}


//********************************************************************************
// returns src ComponentRefs
//********************************************************************************
std::multiset<ComponentRef> PropertyImpl::getComponentConfigPropertySrcs()
{
	std::multiset<ComponentRef> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("PICML::ComponentConfigProperty");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			ComponentRef dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// returns src ComponentRefs
//********************************************************************************
std::multiset<ComponentRef> PropertyImpl::getComponentInfoPropertySrcs()
{
	std::multiset<ComponentRef> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("PICML::ComponentInfoProperty");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			ComponentRef dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// returns src ComponentImplementations
//********************************************************************************
std::multiset<ComponentImplementation> PropertyImpl::getConfigPropertySrcs()
{
	std::multiset<ComponentImplementation> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("PICML::ConfigProperty");
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
// returns dst Edges
//********************************************************************************
std::multiset<Edge> PropertyImpl::getEdgePropertyDsts()
{
	std::multiset<Edge> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("PICML::EdgeProperty");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
		{
			Edge dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
//
//********************************************************************************
std::set<ArtifactExecParameter> PropertyImpl::getInArtifactExecParameterLinks()
{
	std::set<ArtifactExecParameter> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		ArtifactExecParameter c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
//
//********************************************************************************
std::set<ArtifactInfoProperty> PropertyImpl::getInArtifactInfoPropertyLinks()
{
	std::set<ArtifactInfoProperty> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		ArtifactInfoProperty c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
//
//********************************************************************************
std::set<AssemblyConfigProperty> PropertyImpl::getInAssemblyConfigPropertyLinks()
{
	std::set<AssemblyConfigProperty> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		AssemblyConfigProperty c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
//
//********************************************************************************
std::set<AttributeMappingValue> PropertyImpl::getInAttributeMappingValueLinks()
{
	std::set<AttributeMappingValue> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
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
std::set<AttributeValue> PropertyImpl::getInAttributeValueLinks()
{
	std::set<AttributeValue> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		AttributeValue c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
//
//********************************************************************************
std::set<ComponentConfigProperty> PropertyImpl::getInComponentConfigPropertyLinks()
{
	std::set<ComponentConfigProperty> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		ComponentConfigProperty c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
//
//********************************************************************************
std::set<ComponentInfoProperty> PropertyImpl::getInComponentInfoPropertyLinks()
{
	std::set<ComponentInfoProperty> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		ComponentInfoProperty c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
//
//********************************************************************************
std::set<ConfigProperty> PropertyImpl::getInConfigPropertyLinks()
{
	std::set<ConfigProperty> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		ConfigProperty c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
//
//********************************************************************************
std::set<InfoProperty> PropertyImpl::getInInfoPropertyLinks()
{
	std::set<InfoProperty> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		InfoProperty c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
//
//********************************************************************************
std::set<MonolithExecParameter> PropertyImpl::getInMonolithExecParameterLinks()
{
	std::set<MonolithExecParameter> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		MonolithExecParameter c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
//
//********************************************************************************
std::set<PackageConfConfigProperty> PropertyImpl::getInPackageConfConfigPropertyLinks()
{
	std::set<PackageConfConfigProperty> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		PackageConfConfigProperty c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
//
//********************************************************************************
std::set<PackageConfigProperty> PropertyImpl::getInPackageConfigPropertyLinks()
{
	std::set<PackageConfigProperty> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		PackageConfigProperty c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
//
//********************************************************************************
std::set<PackageInfoProperty> PropertyImpl::getInPackageInfoPropertyLinks()
{
	std::set<PackageInfoProperty> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		PackageInfoProperty c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
//
//********************************************************************************
std::set<PathProperty> PropertyImpl::getInPathPropertyLinks()
{
	std::set<PathProperty> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		PathProperty c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// returns src ComponentImplementations
//********************************************************************************
std::multiset<ComponentImplementation> PropertyImpl::getInfoPropertySrcs()
{
	std::multiset<ComponentImplementation> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("PICML::InfoProperty");
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
// returns src MonolithicImplementations
//********************************************************************************
std::multiset<MonolithicImplementation> PropertyImpl::getMonolithExecParameterSrcs()
{
	std::multiset<MonolithicImplementation> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("PICML::MonolithExecParameter");
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
std::set<EdgeProperty> PropertyImpl::getOutEdgePropertyLinks()
{
	std::set<EdgeProperty> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		EdgeProperty c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// returns src PackageConfigurations
//********************************************************************************
std::multiset<PackageConfiguration> PropertyImpl::getPackageConfConfigPropertySrcs()
{
	std::multiset<PackageConfiguration> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("PICML::PackageConfConfigProperty");
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
// returns src ComponentPackages
//********************************************************************************
std::multiset<ComponentPackage> PropertyImpl::getPackageConfigPropertySrcs()
{
	std::multiset<ComponentPackage> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("PICML::PackageConfigProperty");
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
// returns src ComponentPackages
//********************************************************************************
std::multiset<ComponentPackage> PropertyImpl::getPackageInfoPropertySrcs()
{
	std::multiset<ComponentPackage> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("PICML::PackageInfoProperty");
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
// returns src Paths
//********************************************************************************
std::multiset<Path> PropertyImpl::getPathPropertySrcs()
{
	std::multiset<Path> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("PICML::PathProperty");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			Path dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
//
//********************************************************************************
std::string PropertyImpl::getDataValue()
{
	return FCOImpl::getAttribute("DataValue")->getStringValue();
}


//********************************************************************************
//
//********************************************************************************
void PropertyImpl::setDataValue( const std::string& val)
{
	FCOImpl::getAttribute("DataValue")->setStringValue( val);
}


//********************************************************************************
// getter for role "DataType" among "DataType"s
//********************************************************************************
std::set<DataType> PropertyImpl::getDataType()
{
	std::set<DataType> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::DataType");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		DataType elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
//
//********************************************************************************
std::string SatisfierPropertyImpl::getDataValue()
{
	return FCOImpl::getAttribute("DataValue")->getStringValue();
}


//********************************************************************************
//
//********************************************************************************
SatisfierPropertyImpl::SatisfierPropertyKind_Type SatisfierPropertyImpl::getSatisfierPropertyKind()
{
	std::string val = FCOImpl::getAttribute("SatisfierPropertyKind")->getStringValue();

	if ( val == "Quantity") return Quantity_SatisfierPropertyKind_Type;
	else if ( val == "Capacity") return Capacity_SatisfierPropertyKind_Type;
	else if ( val == "Minimum") return Minimum_SatisfierPropertyKind_Type;
	else if ( val == "Maximum") return Maximum_SatisfierPropertyKind_Type;
	else if ( val == "Selection") return Selection_SatisfierPropertyKind_Type;
	else if ( val == "Attribute") return Attribute_SatisfierPropertyKind_Type;
	else throw("None of the possible items");
}


//********************************************************************************
//
//********************************************************************************
bool SatisfierPropertyImpl::isdynamic()
{
	return FCOImpl::getAttribute("dynamic")->getBooleanValue();
}


//********************************************************************************
//
//********************************************************************************
void SatisfierPropertyImpl::setDataValue( const std::string& val)
{
	FCOImpl::getAttribute("DataValue")->setStringValue( val);
}


//********************************************************************************
//
//********************************************************************************
void SatisfierPropertyImpl::setSatisfierPropertyKind( SatisfierPropertyImpl::SatisfierPropertyKind_Type val)
{
	std::string str_val = "";

	if ( val == Quantity_SatisfierPropertyKind_Type) str_val = "Quantity";
	else if ( val == Capacity_SatisfierPropertyKind_Type) str_val = "Capacity";
	else if ( val == Minimum_SatisfierPropertyKind_Type) str_val = "Minimum";
	else if ( val == Maximum_SatisfierPropertyKind_Type) str_val = "Maximum";
	else if ( val == Selection_SatisfierPropertyKind_Type) str_val = "Selection";
	else if ( val == Attribute_SatisfierPropertyKind_Type) str_val = "Attribute";
	else throw("None of the possible items");

	FCOImpl::getAttribute("SatisfierPropertyKind")->setStringValue( str_val);
}


//********************************************************************************
//
//********************************************************************************
void SatisfierPropertyImpl::setdynamic( bool val)
{
	FCOImpl::getAttribute("dynamic")->setBooleanValue( val);
}


//********************************************************************************
// getter for role "DataType" among "DataType"s
//********************************************************************************
std::set<DataType> SatisfierPropertyImpl::getDataType()
{
	std::set<DataType> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::DataType");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		DataType elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "PackageConfigurationReference" among "PackageConfigurationReference"s
//********************************************************************************
std::set<PackageConfigurationReference> TopLevelPackageContainerImpl::getPackageConfigurationReference()
{
	std::set<PackageConfigurationReference> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::PackageConfigurationReference");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		PackageConfigurationReference elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "TopLevelPackage" among "TopLevelPackage"s
//********************************************************************************
std::set<TopLevelPackage> TopLevelPackageContainerImpl::getTopLevelPackage()
{
	std::set<TopLevelPackage> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::TopLevelPackage");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		TopLevelPackage elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "package" among "package"s
//********************************************************************************
std::set<package> TopLevelPackageContainerImpl::getpackage()
{
	std::set<package> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::package");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		package elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
//
//********************************************************************************
bool WorkerImpl::isAbstract()
{
	return FCOImpl::getAttribute("Abstract")->getBooleanValue();
}


//********************************************************************************
//
//********************************************************************************
void WorkerImpl::setAbstract( bool val)
{
	FCOImpl::getAttribute("Abstract")->setBooleanValue( val);
}


//********************************************************************************
// getter for role "Action" among "Action"s and its descendants
//********************************************************************************
std::set<Action> WorkerImpl::getAction()
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
std::set<Action> WorkerImpl::getAction(int dummy)
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
std::set<CompositeAction> WorkerImpl::getCompositeAction()
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
// getter for role "OutputAction" among "Action"s and its descendants
//********************************************************************************
std::set<OutputAction> WorkerImpl::getOutputAction()
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
//
//********************************************************************************
std::string WorkerLibraryImpl::getLocation()
{
	return FCOImpl::getAttribute("Location")->getStringValue();
}


//********************************************************************************
//
//********************************************************************************
void WorkerLibraryImpl::setLocation( const std::string& val)
{
	FCOImpl::getAttribute("Location")->setStringValue( val);
}


//********************************************************************************
// getter for role "WorkerFile" among "WorkerFile"s
//********************************************************************************
std::set<WorkerFile> WorkerLibraryImpl::getWorkerFile()
{
	std::set<WorkerFile> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::WorkerFile");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		WorkerFile elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// returns dst MetricsBases
//********************************************************************************
std::multiset<MetricsBase> BenchmarkTypeImpl::getBenchmarkCharacteristicsDsts()
{
	std::multiset<MetricsBase> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("PICML::BenchmarkCharacteristics");
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
std::set<BenchmarkCharacteristics> BenchmarkTypeImpl::getOutBenchmarkCharacteristicsLinks()
{
	std::set<BenchmarkCharacteristics> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
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
long FixedIterationBenchmarksImpl::getbenchmarkIterations()
{
	return FCOImpl::getAttribute("benchmarkIterations")->getIntegerValue();
}


//********************************************************************************
//
//********************************************************************************
void FixedIterationBenchmarksImpl::setbenchmarkIterations( const long val)
{
	FCOImpl::getAttribute("benchmarkIterations")->setIntegerValue( val);
}


//********************************************************************************
//
//********************************************************************************
long PeriodicBenchmarksImpl::gettimeperiod()
{
	return FCOImpl::getAttribute("timeperiod")->getIntegerValue();
}


//********************************************************************************
//
//********************************************************************************
void PeriodicBenchmarksImpl::settimeperiod( const long val)
{
	FCOImpl::getAttribute("timeperiod")->setIntegerValue( val);
}

} // namespace PICML
