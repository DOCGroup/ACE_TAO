#include "stdafx.h"
#include "PICMLBonX.h"
#include "PICMLBonX1.h"
#include "PICMLBonX2.h"
#include "PICMLBonX3.h"
#include "PICMLBonX4.h"

namespace BON
{

IMPLEMENT_ABSTRACT_BONEXTENSION( PICML::ReqQoSBase );
IMPLEMENT_ABSTRACT_BONEXTENSION( PICML::QoSCharacteristic );
IMPLEMENT_BONEXTENSION( PICML::QoSCharRef, "PICML::QoSCharRef" );
IMPLEMENT_BONEXTENSION( PICML::ComponentPackage, "PICML::ComponentPackage" );
IMPLEMENT_BONEXTENSION( PICML::Edge, "PICML::Edge" );
IMPLEMENT_BONEXTENSION( PICML::EnumValue, "PICML::EnumValue" );
IMPLEMENT_BONEXTENSION( PICML::ExternalReferenceEndPoint, "PICML::ExternalReferenceEndPoint" );
IMPLEMENT_BONEXTENSION( PICML::ImplementationArtifact, "PICML::ImplementationArtifact" );
IMPLEMENT_BONEXTENSION( PICML::ImplementationDependency, "PICML::ImplementationDependency" );
IMPLEMENT_BONEXTENSION( PICML::Label, "PICML::Label" );
IMPLEMENT_BONEXTENSION( PICML::PackageConfiguration, "PICML::PackageConfiguration" );
IMPLEMENT_BONEXTENSION( PICML::PrivateFlag, "PICML::PrivateFlag" );
IMPLEMENT_BONEXTENSION( PICML::PublishConnector, "PICML::PublishConnector" );
IMPLEMENT_BONEXTENSION( PICML::QoSConnector, "PICML::QoSConnector" );
IMPLEMENT_BONEXTENSION( PICML::State, "PICML::State" );
IMPLEMENT_BONEXTENSION( PICML::Task, "PICML::Task" );
IMPLEMENT_BONEXTENSION( PICML::TimeProbe, "PICML::TimeProbe" );
IMPLEMENT_BONEXTENSION( PICML::TopLevelPackage, "PICML::TopLevelPackage" );
IMPLEMENT_BONEXTENSION( PICML::ArtifactDependency, "PICML::ArtifactDependency" );
IMPLEMENT_BONEXTENSION( PICML::ArtifactDependsOn, "PICML::ArtifactDependsOn" );
IMPLEMENT_BONEXTENSION( PICML::ArtifactDeployRequirement, "PICML::ArtifactDeployRequirement" );
IMPLEMENT_BONEXTENSION( PICML::ArtifactExecParameter, "PICML::ArtifactExecParameter" );
IMPLEMENT_BONEXTENSION( PICML::ArtifactInfoProperty, "PICML::ArtifactInfoProperty" );
IMPLEMENT_BONEXTENSION( PICML::AssemblyConfigProperty, "PICML::AssemblyConfigProperty" );
IMPLEMENT_BONEXTENSION( PICML::AssemblyDeployRequirement, "PICML::AssemblyDeployRequirement" );
IMPLEMENT_BONEXTENSION( PICML::AssemblyselectRequirement, "PICML::AssemblyselectRequirement" );
IMPLEMENT_BONEXTENSION( PICML::AttributeDelegate, "PICML::AttributeDelegate" );
IMPLEMENT_BONEXTENSION( PICML::AttributeMappingDelegate, "PICML::AttributeMappingDelegate" );
IMPLEMENT_BONEXTENSION( PICML::AttributeMappingValue, "PICML::AttributeMappingValue" );
IMPLEMENT_BONEXTENSION( PICML::AttributeValue, "PICML::AttributeValue" );
IMPLEMENT_BONEXTENSION( PICML::BenchmarkCharacteristics, "PICML::BenchmarkCharacteristics" );
IMPLEMENT_BONEXTENSION( PICML::Bridge2Interconnect, "PICML::Bridge2Interconnect" );
IMPLEMENT_BONEXTENSION( PICML::ComponentConfigProperty, "PICML::ComponentConfigProperty" );
IMPLEMENT_BONEXTENSION( PICML::ComponentInfoProperty, "PICML::ComponentInfoProperty" );
IMPLEMENT_BONEXTENSION( PICML::ComponentOperation, "PICML::ComponentOperation" );
IMPLEMENT_BONEXTENSION( PICML::ComponentProperty, "PICML::ComponentProperty" );
IMPLEMENT_BONEXTENSION( PICML::ConfigProperty, "PICML::ConfigProperty" );
IMPLEMENT_BONEXTENSION( PICML::CriticalPath, "PICML::CriticalPath" );
IMPLEMENT_BONEXTENSION( PICML::DstEdge, "PICML::DstEdge" );
IMPLEMENT_BONEXTENSION( PICML::EdgeProperty, "PICML::EdgeProperty" );
IMPLEMENT_BONEXTENSION( PICML::Effect, "PICML::Effect" );
IMPLEMENT_BONEXTENSION( PICML::EventSinkDelegate, "PICML::EventSinkDelegate" );
IMPLEMENT_BONEXTENSION( PICML::EventSinkPublish, "PICML::EventSinkPublish" );
IMPLEMENT_BONEXTENSION( PICML::EventSourceDelegate, "PICML::EventSourceDelegate" );
IMPLEMENT_BONEXTENSION( PICML::EventSourcePublish, "PICML::EventSourcePublish" );
IMPLEMENT_BONEXTENSION( PICML::ExtResourceConn, "PICML::ExtResourceConn" );
IMPLEMENT_BONEXTENSION( PICML::ExternalDelegate, "PICML::ExternalDelegate" );
IMPLEMENT_BONEXTENSION( PICML::FacetDelegate, "PICML::FacetDelegate" );
IMPLEMENT_BONEXTENSION( PICML::FacetInvoke, "PICML::FacetInvoke" );
IMPLEMENT_BONEXTENSION( PICML::Finish, "PICML::Finish" );
IMPLEMENT_BONEXTENSION( PICML::Implementation, "PICML::Implementation" );
IMPLEMENT_BONEXTENSION( PICML::ImplementationCapability, "PICML::ImplementationCapability" );
IMPLEMENT_BONEXTENSION( PICML::ImplementationDependsOn, "PICML::ImplementationDependsOn" );
IMPLEMENT_BONEXTENSION( PICML::Implements, "PICML::Implements" );
IMPLEMENT_BONEXTENSION( PICML::InfoProperty, "PICML::InfoProperty" );
IMPLEMENT_BONEXTENSION( PICML::Input, "PICML::Input" );
IMPLEMENT_BONEXTENSION( PICML::InstanceMapping, "PICML::InstanceMapping" );
IMPLEMENT_BONEXTENSION( PICML::Interconnect2Bridge, "PICML::Interconnect2Bridge" );
IMPLEMENT_BONEXTENSION( PICML::Interconnect2Node, "PICML::Interconnect2Node" );
IMPLEMENT_BONEXTENSION( PICML::LabelConnection, "PICML::LabelConnection" );
IMPLEMENT_BONEXTENSION( PICML::MakeMemberPrivate, "PICML::MakeMemberPrivate" );
IMPLEMENT_BONEXTENSION( PICML::ManagesComponent, "PICML::ManagesComponent" );
IMPLEMENT_BONEXTENSION( PICML::MetricConnection, "PICML::MetricConnection" );
IMPLEMENT_BONEXTENSION( PICML::MonolithDeployRequirement, "PICML::MonolithDeployRequirement" );

} // end namespace BON

namespace PICML
{

//********************************************************************************
//
//********************************************************************************
std::set<PortQoS> ReqQoSBaseImpl::getInPortQoSLinks()
{
	std::set<PortQoS> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		PortQoS c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
//
//********************************************************************************
std::set<QoSReq> ReqQoSBaseImpl::getInQoSReqLinks()
{
	std::set<QoSReq> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		QoSReq c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// returns src Ports and referenceports
//********************************************************************************
std::multiset<BON::ConnectionEnd> ReqQoSBaseImpl::getPortQoSSrcs()
{
	return BON::ConnectionEndImpl::getInConnEnds("PICML::PortQoS");
}


//********************************************************************************
// returns src QoSConnectors
//********************************************************************************
std::multiset<QoSConnector> ReqQoSBaseImpl::getQoSReqSrcs()
{
	std::multiset<QoSConnector> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("PICML::QoSReq");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			QoSConnector dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
//
//********************************************************************************
QoSCharacteristic QoSCharRefImpl::getQoSCharacteristic()
{
	BON::FCO r = BON::ReferenceImpl::getReferred();
	return QoSCharacteristic(r);
}


//********************************************************************************
// returns dst ComponentImplementationReferences
//********************************************************************************
std::multiset<ComponentImplementationReference> ComponentPackageImpl::getImplementationDsts()
{
	std::multiset<ComponentImplementationReference> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("PICML::Implementation");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
		{
			ComponentImplementationReference dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
//
//********************************************************************************
std::set<PackageConfBasePackage> ComponentPackageImpl::getInPackageConfBasePackageLinks()
{
	std::set<PackageConfBasePackage> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		PackageConfBasePackage c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
//
//********************************************************************************
std::set<Implementation> ComponentPackageImpl::getOutImplementationLinks()
{
	std::set<Implementation> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
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
std::set<PackageConfigProperty> ComponentPackageImpl::getOutPackageConfigPropertyLinks()
{
	std::set<PackageConfigProperty> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
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
std::set<PackageInfoProperty> ComponentPackageImpl::getOutPackageInfoPropertyLinks()
{
	std::set<PackageInfoProperty> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
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
std::set<PackageInterface> ComponentPackageImpl::getOutPackageInterfaceLinks()
{
	std::set<PackageInterface> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		PackageInterface c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// returns src PackageConfigurations
//********************************************************************************
std::multiset<PackageConfiguration> ComponentPackageImpl::getPackageConfBasePackageSrcs()
{
	std::multiset<PackageConfiguration> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("PICML::PackageConfBasePackage");
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
// returns dst Propertys
//********************************************************************************
std::multiset<Property> ComponentPackageImpl::getPackageConfigPropertyDsts()
{
	std::multiset<Property> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("PICML::PackageConfigProperty");
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
// returns dst Propertys
//********************************************************************************
std::multiset<Property> ComponentPackageImpl::getPackageInfoPropertyDsts()
{
	std::multiset<Property> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("PICML::PackageInfoProperty");
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
// returns dst ComponentRefs
//********************************************************************************
std::multiset<ComponentRef> ComponentPackageImpl::getPackageInterfaceDsts()
{
	std::multiset<ComponentRef> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("PICML::PackageInterface");
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
//
//********************************************************************************
std::string ComponentPackageImpl::getUUID()
{
	return FCOImpl::getAttribute("UUID")->getStringValue();
}


//********************************************************************************
//
//********************************************************************************
std::string ComponentPackageImpl::getlabel()
{
	return FCOImpl::getAttribute("label")->getStringValue();
}


//********************************************************************************
//
//********************************************************************************
void ComponentPackageImpl::setUUID( const std::string& val)
{
	FCOImpl::getAttribute("UUID")->setStringValue( val);
}


//********************************************************************************
//
//********************************************************************************
void ComponentPackageImpl::setlabel( const std::string& val)
{
	FCOImpl::getAttribute("label")->setStringValue( val);
}


//********************************************************************************
// returns src GraphVertexs and referenceports
//********************************************************************************
std::multiset<BON::ConnectionEnd> EdgeImpl::getDstEdgeSrcs()
{
	return BON::ConnectionEndImpl::getInConnEnds("PICML::DstEdge");
}


//********************************************************************************
// returns src Propertys
//********************************************************************************
std::multiset<Property> EdgeImpl::getEdgePropertySrcs()
{
	std::multiset<Property> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("PICML::EdgeProperty");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
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
std::set<DstEdge> EdgeImpl::getInDstEdgeLinks()
{
	std::set<DstEdge> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		DstEdge c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
//
//********************************************************************************
std::set<EdgeProperty> EdgeImpl::getInEdgePropertyLinks()
{
	std::set<EdgeProperty> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
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
//
//********************************************************************************
std::set<SrcEdge> EdgeImpl::getOutSrcEdgeLinks()
{
	std::set<SrcEdge> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		SrcEdge c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// returns dst GraphVertexs and referenceports
//********************************************************************************
std::multiset<BON::ConnectionEnd> EdgeImpl::getSrcEdgeDsts()
{
	return BON::ConnectionEndImpl::getOutConnEnds("PICML::SrcEdge");
}


//********************************************************************************
// returns src Components
//********************************************************************************
std::multiset<Component> ExternalReferenceEndPointImpl::getExternalDelegateSrcs()
{
	std::multiset<Component> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("PICML::ExternalDelegate");
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
std::set<ExternalDelegate> ExternalReferenceEndPointImpl::getInExternalDelegateLinks()
{
	std::set<ExternalDelegate> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		ExternalDelegate c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
//
//********************************************************************************
std::string ExternalReferenceEndPointImpl::getlocation()
{
	return FCOImpl::getAttribute("location")->getStringValue();
}


//********************************************************************************
//
//********************************************************************************
void ExternalReferenceEndPointImpl::setlocation( const std::string& val)
{
	FCOImpl::getAttribute("location")->setStringValue( val);
}


//********************************************************************************
// returns dst ImplementationArtifacts
//********************************************************************************
std::multiset<ImplementationArtifact> ImplementationArtifactImpl::getArtifactDependencyDsts()
{
	std::multiset<ImplementationArtifact> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("PICML::ArtifactDependency");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
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
std::set<ArtifactDependency> ImplementationArtifactImpl::getArtifactDependencyLinks()
{
	std::set<ArtifactDependency> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		ArtifactDependency c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// returns src ImplementationArtifacts
//********************************************************************************
std::multiset<ImplementationArtifact> ImplementationArtifactImpl::getArtifactDependencySrcs()
{
	std::multiset<ImplementationArtifact> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("PICML::ArtifactDependency");
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
// returns dst ImplementationArtifactReferences
//********************************************************************************
std::multiset<ImplementationArtifactReference> ImplementationArtifactImpl::getArtifactDependsOnDsts()
{
	std::multiset<ImplementationArtifactReference> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("PICML::ArtifactDependsOn");
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
// returns dst Requirements
//********************************************************************************
std::multiset<Requirement> ImplementationArtifactImpl::getArtifactDeployRequirementDsts()
{
	std::multiset<Requirement> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("PICML::ArtifactDeployRequirement");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
		{
			Requirement dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// returns dst Propertys
//********************************************************************************
std::multiset<Property> ImplementationArtifactImpl::getArtifactExecParameterDsts()
{
	std::multiset<Property> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("PICML::ArtifactExecParameter");
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
// returns dst Propertys
//********************************************************************************
std::multiset<Property> ImplementationArtifactImpl::getArtifactInfoPropertyDsts()
{
	std::multiset<Property> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("PICML::ArtifactInfoProperty");
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
std::set<ArtifactDependency> ImplementationArtifactImpl::getInArtifactDependencyLinks()
{
	std::set<ArtifactDependency> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		ArtifactDependency c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
//
//********************************************************************************
std::set<ArtifactDependency> ImplementationArtifactImpl::getOutArtifactDependencyLinks()
{
	std::set<ArtifactDependency> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		ArtifactDependency c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
//
//********************************************************************************
std::set<ArtifactDependsOn> ImplementationArtifactImpl::getOutArtifactDependsOnLinks()
{
	std::set<ArtifactDependsOn> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
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
std::set<ArtifactDeployRequirement> ImplementationArtifactImpl::getOutArtifactDeployRequirementLinks()
{
	std::set<ArtifactDeployRequirement> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
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
std::set<ArtifactExecParameter> ImplementationArtifactImpl::getOutArtifactExecParameterLinks()
{
	std::set<ArtifactExecParameter> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
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
std::set<ArtifactInfoProperty> ImplementationArtifactImpl::getOutArtifactInfoPropertyLinks()
{
	std::set<ArtifactInfoProperty> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
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
std::string ImplementationArtifactImpl::getUUID()
{
	return FCOImpl::getAttribute("UUID")->getStringValue();
}


//********************************************************************************
//
//********************************************************************************
std::string ImplementationArtifactImpl::getconfiguration()
{
	return FCOImpl::getAttribute("configuration")->getStringValue();
}


//********************************************************************************
//
//********************************************************************************
std::string ImplementationArtifactImpl::getlabel()
{
	return FCOImpl::getAttribute("label")->getStringValue();
}


//********************************************************************************
//
//********************************************************************************
std::string ImplementationArtifactImpl::getlocation()
{
	return FCOImpl::getAttribute("location")->getStringValue();
}


//********************************************************************************
//
//********************************************************************************
void ImplementationArtifactImpl::setUUID( const std::string& val)
{
	FCOImpl::getAttribute("UUID")->setStringValue( val);
}


//********************************************************************************
//
//********************************************************************************
void ImplementationArtifactImpl::setconfiguration( const std::string& val)
{
	FCOImpl::getAttribute("configuration")->setStringValue( val);
}


//********************************************************************************
//
//********************************************************************************
void ImplementationArtifactImpl::setlabel( const std::string& val)
{
	FCOImpl::getAttribute("label")->setStringValue( val);
}


//********************************************************************************
//
//********************************************************************************
void ImplementationArtifactImpl::setlocation( const std::string& val)
{
	FCOImpl::getAttribute("location")->setStringValue( val);
}


//********************************************************************************
// returns src ComponentImplementations
//********************************************************************************
std::multiset<ComponentImplementation> ImplementationDependencyImpl::getImplementationDependsOnSrcs()
{
	std::multiset<ComponentImplementation> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("PICML::ImplementationDependsOn");
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
std::set<ImplementationDependsOn> ImplementationDependencyImpl::getInImplementationDependsOnLinks()
{
	std::set<ImplementationDependsOn> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
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
std::string ImplementationDependencyImpl::getrequiredType()
{
	return FCOImpl::getAttribute("requiredType")->getStringValue();
}


//********************************************************************************
//
//********************************************************************************
void ImplementationDependencyImpl::setrequiredType( const std::string& val)
{
	FCOImpl::getAttribute("requiredType")->setStringValue( val);
}


//********************************************************************************
//
//********************************************************************************
std::set<LabelConnection> LabelImpl::getInLabelConnectionLinks()
{
	std::set<LabelConnection> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
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
// returns src Members
//********************************************************************************
std::multiset<Member> LabelImpl::getLabelConnectionSrcs()
{
	std::multiset<Member> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("PICML::LabelConnection");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			Member dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
//
//********************************************************************************
std::set<PackageConfBasePackage> PackageConfigurationImpl::getOutPackageConfBasePackageLinks()
{
	std::set<PackageConfBasePackage> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		PackageConfBasePackage c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
//
//********************************************************************************
std::set<PackageConfConfigProperty> PackageConfigurationImpl::getOutPackageConfConfigPropertyLinks()
{
	std::set<PackageConfConfigProperty> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
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
std::set<PackageConfReference> PackageConfigurationImpl::getOutPackageConfReferenceLinks()
{
	std::set<PackageConfReference> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
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
//
//********************************************************************************
std::set<PackageConfSelectRequirement> PackageConfigurationImpl::getOutPackageConfSelectRequirementLinks()
{
	std::set<PackageConfSelectRequirement> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
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
//
//********************************************************************************
std::set<PackageConfSpecializedConfig> PackageConfigurationImpl::getOutPackageConfSpecializedConfigLinks()
{
	std::set<PackageConfSpecializedConfig> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
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
// returns dst ComponentPackages
//********************************************************************************
std::multiset<ComponentPackage> PackageConfigurationImpl::getPackageConfBasePackageDsts()
{
	std::multiset<ComponentPackage> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("PICML::PackageConfBasePackage");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
		{
			ComponentPackage dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// returns dst Propertys
//********************************************************************************
std::multiset<Property> PackageConfigurationImpl::getPackageConfConfigPropertyDsts()
{
	std::multiset<Property> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("PICML::PackageConfConfigProperty");
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
// returns dst ComponentPackageReferences
//********************************************************************************
std::multiset<ComponentPackageReference> PackageConfigurationImpl::getPackageConfReferenceDsts()
{
	std::multiset<ComponentPackageReference> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("PICML::PackageConfReference");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
		{
			ComponentPackageReference dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// returns dst Requirements
//********************************************************************************
std::multiset<Requirement> PackageConfigurationImpl::getPackageConfSelectRequirementDsts()
{
	std::multiset<Requirement> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("PICML::PackageConfSelectRequirement");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
		{
			Requirement dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// returns dst PackageConfigurationReferences
//********************************************************************************
std::multiset<PackageConfigurationReference> PackageConfigurationImpl::getPackageConfSpecializedConfigDsts()
{
	std::multiset<PackageConfigurationReference> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("PICML::PackageConfSpecializedConfig");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
		{
			PackageConfigurationReference dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
//
//********************************************************************************
std::string PackageConfigurationImpl::getUUID()
{
	return FCOImpl::getAttribute("UUID")->getStringValue();
}


//********************************************************************************
//
//********************************************************************************
std::string PackageConfigurationImpl::getlabel()
{
	return FCOImpl::getAttribute("label")->getStringValue();
}


//********************************************************************************
//
//********************************************************************************
void PackageConfigurationImpl::setUUID( const std::string& val)
{
	FCOImpl::getAttribute("UUID")->setStringValue( val);
}


//********************************************************************************
//
//********************************************************************************
void PackageConfigurationImpl::setlabel( const std::string& val)
{
	FCOImpl::getAttribute("label")->setStringValue( val);
}


//********************************************************************************
//
//********************************************************************************
std::set<MakeMemberPrivate> PrivateFlagImpl::getInMakeMemberPrivateLinks()
{
	std::set<MakeMemberPrivate> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
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
// returns src Members
//********************************************************************************
std::multiset<Member> PrivateFlagImpl::getMakeMemberPrivateSrcs()
{
	std::multiset<Member> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("PICML::MakeMemberPrivate");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			Member dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// returns dst Requirements
//********************************************************************************
std::multiset<Requirement> PublishConnectorImpl::getAssemblyDeployRequirementDsts()
{
	std::multiset<Requirement> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("PICML::AssemblyDeployRequirement");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
		{
			Requirement dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
//
//********************************************************************************
std::set<publish> PublishConnectorImpl::getInpublishLinks()
{
	std::set<publish> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		publish c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
//
//********************************************************************************
std::set<AssemblyDeployRequirement> PublishConnectorImpl::getOutAssemblyDeployRequirementLinks()
{
	std::set<AssemblyDeployRequirement> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
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
std::set<deliverTo> PublishConnectorImpl::getOutdeliverToLinks()
{
	std::set<deliverTo> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		deliverTo c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// returns dst InEventPorts and referenceports
//********************************************************************************
std::multiset<BON::ConnectionEnd> PublishConnectorImpl::getdeliverToDsts()
{
	return BON::ConnectionEndImpl::getOutConnEnds("PICML::deliverTo");
}


//********************************************************************************
// returns src OutEventPorts and referenceports
//********************************************************************************
std::multiset<BON::ConnectionEnd> PublishConnectorImpl::getpublishSrcs()
{
	return BON::ConnectionEndImpl::getInConnEnds("PICML::publish");
}


//********************************************************************************
// returns dst InEventPorts and referenceports
//********************************************************************************
std::multiset<BON::ConnectionEnd> QoSConnectorImpl::getEventSinkPublishDsts()
{
	return BON::ConnectionEndImpl::getOutConnEnds("PICML::EventSinkPublish");
}


//********************************************************************************
// returns src OutEventPorts and referenceports
//********************************************************************************
std::multiset<BON::ConnectionEnd> QoSConnectorImpl::getEventSourcePublishSrcs()
{
	return BON::ConnectionEndImpl::getInConnEnds("PICML::EventSourcePublish");
}


//********************************************************************************
// returns dst ProvidedRequestPorts and referenceports
//********************************************************************************
std::multiset<BON::ConnectionEnd> QoSConnectorImpl::getFacetInvokeDsts()
{
	return BON::ConnectionEndImpl::getOutConnEnds("PICML::FacetInvoke");
}


//********************************************************************************
//
//********************************************************************************
std::set<EventSourcePublish> QoSConnectorImpl::getInEventSourcePublishLinks()
{
	std::set<EventSourcePublish> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		EventSourcePublish c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
//
//********************************************************************************
std::set<RecepInvoke> QoSConnectorImpl::getInRecepInvokeLinks()
{
	std::set<RecepInvoke> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		RecepInvoke c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
//
//********************************************************************************
std::set<EventSinkPublish> QoSConnectorImpl::getOutEventSinkPublishLinks()
{
	std::set<EventSinkPublish> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		EventSinkPublish c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
//
//********************************************************************************
std::set<FacetInvoke> QoSConnectorImpl::getOutFacetInvokeLinks()
{
	std::set<FacetInvoke> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		FacetInvoke c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
//
//********************************************************************************
std::set<QoSReq> QoSConnectorImpl::getOutQoSReqLinks()
{
	std::set<QoSReq> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		QoSReq c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// returns dst ReqQoSBases
//********************************************************************************
std::multiset<ReqQoSBase> QoSConnectorImpl::getQoSReqDsts()
{
	std::multiset<ReqQoSBase> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("PICML::QoSReq");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
		{
			ReqQoSBase dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// returns src RequiredRequestPorts and referenceports
//********************************************************************************
std::multiset<BON::ConnectionEnd> QoSConnectorImpl::getRecepInvokeSrcs()
{
	return BON::ConnectionEndImpl::getInConnEnds("PICML::RecepInvoke");
}


//********************************************************************************
// returns src ActionBases
//********************************************************************************
std::multiset<ActionBase> StateImpl::getEffectSrcs()
{
	std::multiset<ActionBase> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("PICML::Effect");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			ActionBase dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// returns dst InputActionBases
//********************************************************************************
std::multiset<InputActionBase> StateImpl::getFinishDsts()
{
	std::multiset<InputActionBase> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("PICML::Finish");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
		{
			InputActionBase dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
//
//********************************************************************************
std::set<Effect> StateImpl::getInEffectLinks()
{
	std::set<Effect> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
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
//
//********************************************************************************
std::set<Finish> StateImpl::getOutFinishLinks()
{
	std::set<Finish> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
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
std::set<Transition> StateImpl::getOutTransitionLinks()
{
	std::set<Transition> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
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
// returns dst ActionBases
//********************************************************************************
std::multiset<ActionBase> StateImpl::getTransitionDsts()
{
	std::multiset<ActionBase> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("PICML::Transition");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
		{
			ActionBase dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
//
//********************************************************************************
std::set<WorkLoadOperationConnection> TaskImpl::getInWorkLoadOperationConnectionLinks()
{
	std::set<WorkLoadOperationConnection> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
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
// returns src OperationRefs
//********************************************************************************
std::multiset<OperationRef> TaskImpl::getWorkLoadOperationConnectionSrcs()
{
	std::multiset<OperationRef> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("PICML::WorkLoadOperationConnection");
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
std::set<TimerConnection> TimeProbeImpl::getInTimerConnectionLinks()
{
	std::set<TimerConnection> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
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
std::set<TimerEventSinkConn> TimeProbeImpl::getInTimerEventSinkConnLinks()
{
	std::set<TimerEventSinkConn> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
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
// returns src OperationRefs
//********************************************************************************
std::multiset<OperationRef> TimeProbeImpl::getTimerConnectionSrcs()
{
	std::multiset<OperationRef> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("PICML::TimerConnection");
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
// returns src EventRefs
//********************************************************************************
std::multiset<EventRef> TimeProbeImpl::getTimerEventSinkConnSrcs()
{
	std::multiset<EventRef> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("PICML::TimerEventSinkConn");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			EventRef dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
//
//********************************************************************************
std::set<package> TopLevelPackageImpl::getOutpackageLinks()
{
	std::set<package> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
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
// returns dst PackageConfigurationReferences
//********************************************************************************
std::multiset<PackageConfigurationReference> TopLevelPackageImpl::getpackageDsts()
{
	std::multiset<PackageConfigurationReference> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("PICML::package");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
		{
			PackageConfigurationReference dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to ImplementationArtifact
//********************************************************************************
ImplementationArtifact ArtifactDependencyImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	ImplementationArtifact sp( ce);
	if ( sp)
		return sp;

	ImplementationArtifact empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to ImplementationArtifact
//********************************************************************************
ImplementationArtifact ArtifactDependencyImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	ImplementationArtifact sp( ce);
	if ( sp)
		return sp;

	ImplementationArtifact empty;
	return empty;
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to ImplementationArtifactReference
//********************************************************************************
ImplementationArtifactReference ArtifactDependsOnImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	ImplementationArtifactReference sp( ce);
	if ( sp)
		return sp;

	ImplementationArtifactReference empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to ImplementationArtifact
//********************************************************************************
ImplementationArtifact ArtifactDependsOnImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	ImplementationArtifact sp( ce);
	if ( sp)
		return sp;

	ImplementationArtifact empty;
	return empty;
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to Requirement
//********************************************************************************
Requirement ArtifactDeployRequirementImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	Requirement sp( ce);
	if ( sp)
		return sp;

	Requirement empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to ImplementationArtifact
//********************************************************************************
ImplementationArtifact ArtifactDeployRequirementImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	ImplementationArtifact sp( ce);
	if ( sp)
		return sp;

	ImplementationArtifact empty;
	return empty;
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to Property
//********************************************************************************
Property ArtifactExecParameterImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	Property sp( ce);
	if ( sp)
		return sp;

	Property empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to ImplementationArtifact
//********************************************************************************
ImplementationArtifact ArtifactExecParameterImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	ImplementationArtifact sp( ce);
	if ( sp)
		return sp;

	ImplementationArtifact empty;
	return empty;
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to Property
//********************************************************************************
Property ArtifactInfoPropertyImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	Property sp( ce);
	if ( sp)
		return sp;

	Property empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to ImplementationArtifact
//********************************************************************************
ImplementationArtifact ArtifactInfoPropertyImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	ImplementationArtifact sp( ce);
	if ( sp)
		return sp;

	ImplementationArtifact empty;
	return empty;
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to Property
//********************************************************************************
Property AssemblyConfigPropertyImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	Property sp( ce);
	if ( sp)
		return sp;

	Property empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to Component
//********************************************************************************
Component AssemblyConfigPropertyImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	Component sp( ce);
	if ( sp)
		return sp;

	Component empty;
	return empty;
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to Requirement
//********************************************************************************
Requirement AssemblyDeployRequirementImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	Requirement sp( ce);
	if ( sp)
		return sp;

	Requirement empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to PublishConnector
//********************************************************************************
PublishConnector AssemblyDeployRequirementImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	PublishConnector sp( ce);
	if ( sp)
		return sp;

	PublishConnector empty;
	return empty;
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to Requirement
//********************************************************************************
Requirement AssemblyselectRequirementImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	Requirement sp( ce);
	if ( sp)
		return sp;

	Requirement empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to Component
//********************************************************************************
Component AssemblyselectRequirementImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	Component sp( ce);
	if ( sp)
		return sp;

	Component empty;
	return empty;
}


//********************************************************************************
// getDst() return value is a ConnectionEnd
//********************************************************************************
BON::ConnectionEnd AttributeDelegateImpl::getDst()
{
	return ConnectionImpl::getDst();
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd
//********************************************************************************
BON::ConnectionEnd AttributeDelegateImpl::getSrc()
{
	return ConnectionImpl::getSrc();
}


//********************************************************************************
// getDst() return value is a ConnectionEnd
//********************************************************************************
BON::ConnectionEnd AttributeMappingDelegateImpl::getDst()
{
	return ConnectionImpl::getDst();
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd
//********************************************************************************
BON::ConnectionEnd AttributeMappingDelegateImpl::getSrc()
{
	return ConnectionImpl::getSrc();
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to Property
//********************************************************************************
Property AttributeMappingValueImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	Property sp( ce);
	if ( sp)
		return sp;

	Property empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd
//********************************************************************************
BON::ConnectionEnd AttributeMappingValueImpl::getSrc()
{
	return ConnectionImpl::getSrc();
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to Property
//********************************************************************************
Property AttributeValueImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	Property sp( ce);
	if ( sp)
		return sp;

	Property empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd
//********************************************************************************
BON::ConnectionEnd AttributeValueImpl::getSrc()
{
	return ConnectionImpl::getSrc();
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to MetricsBase
//********************************************************************************
MetricsBase BenchmarkCharacteristicsImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	MetricsBase sp( ce);
	if ( sp)
		return sp;

	MetricsBase empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to BenchmarkType
//********************************************************************************
BenchmarkType BenchmarkCharacteristicsImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	BenchmarkType sp( ce);
	if ( sp)
		return sp;

	BenchmarkType empty;
	return empty;
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to Interconnect
//********************************************************************************
Interconnect Bridge2InterconnectImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	Interconnect sp( ce);
	if ( sp)
		return sp;

	Interconnect empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to Bridge
//********************************************************************************
Bridge Bridge2InterconnectImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	Bridge sp( ce);
	if ( sp)
		return sp;

	Bridge empty;
	return empty;
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to Property
//********************************************************************************
Property ComponentConfigPropertyImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	Property sp( ce);
	if ( sp)
		return sp;

	Property empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to ComponentRef
//********************************************************************************
ComponentRef ComponentConfigPropertyImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	ComponentRef sp( ce);
	if ( sp)
		return sp;

	ComponentRef empty;
	return empty;
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to Property
//********************************************************************************
Property ComponentInfoPropertyImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	Property sp( ce);
	if ( sp)
		return sp;

	Property empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to ComponentRef
//********************************************************************************
ComponentRef ComponentInfoPropertyImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	ComponentRef sp( ce);
	if ( sp)
		return sp;

	ComponentRef empty;
	return empty;
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to CompRef
//********************************************************************************
CompRef ComponentOperationImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	CompRef sp( ce);
	if ( sp)
		return sp;

	CompRef empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to OperationRef
//********************************************************************************
OperationRef ComponentOperationImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	OperationRef sp( ce);
	if ( sp)
		return sp;

	OperationRef empty;
	return empty;
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to ComponentPropertyDescription
//********************************************************************************
ComponentPropertyDescription ComponentPropertyImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	ComponentPropertyDescription sp( ce);
	if ( sp)
		return sp;

	ComponentPropertyDescription empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to ComponentRef
//********************************************************************************
ComponentRef ComponentPropertyImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	ComponentRef sp( ce);
	if ( sp)
		return sp;

	ComponentRef empty;
	return empty;
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to Property
//********************************************************************************
Property ConfigPropertyImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	Property sp( ce);
	if ( sp)
		return sp;

	Property empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to ComponentImplementation
//********************************************************************************
ComponentImplementation ConfigPropertyImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	ComponentImplementation sp( ce);
	if ( sp)
		return sp;

	ComponentImplementation empty;
	return empty;
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to PathReference
//********************************************************************************
PathReference CriticalPathImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	PathReference sp( ce);
	if ( sp)
		return sp;

	PathReference empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to ComponentAssembly
//********************************************************************************
ComponentAssembly CriticalPathImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	ComponentAssembly sp( ce);
	if ( sp)
		return sp;

	ComponentAssembly empty;
	return empty;
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to Edge
//********************************************************************************
Edge DstEdgeImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	Edge sp( ce);
	if ( sp)
		return sp;

	Edge empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd
//********************************************************************************
BON::ConnectionEnd DstEdgeImpl::getSrc()
{
	return ConnectionImpl::getSrc();
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to Edge
//********************************************************************************
Edge EdgePropertyImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	Edge sp( ce);
	if ( sp)
		return sp;

	Edge empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to Property
//********************************************************************************
Property EdgePropertyImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	Property sp( ce);
	if ( sp)
		return sp;

	Property empty;
	return empty;
}


//********************************************************************************
//
//********************************************************************************
std::string EffectImpl::getPostcondition()
{
	return FCOImpl::getAttribute("Postcondition")->getStringValue();
}


//********************************************************************************
//
//********************************************************************************
void EffectImpl::setPostcondition( const std::string& val)
{
	FCOImpl::getAttribute("Postcondition")->setStringValue( val);
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to State
//********************************************************************************
State EffectImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	State sp( ce);
	if ( sp)
		return sp;

	State empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to ActionBase
//********************************************************************************
ActionBase EffectImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	ActionBase sp( ce);
	if ( sp)
		return sp;

	ActionBase empty;
	return empty;
}


//********************************************************************************
// getDst() return value is a ConnectionEnd
//********************************************************************************
BON::ConnectionEnd EventSinkDelegateImpl::getDst()
{
	return ConnectionImpl::getDst();
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd
//********************************************************************************
BON::ConnectionEnd EventSinkDelegateImpl::getSrc()
{
	return ConnectionImpl::getSrc();
}


//********************************************************************************
// getDst() return value is a ConnectionEnd
//********************************************************************************
BON::ConnectionEnd EventSinkPublishImpl::getDst()
{
	return ConnectionImpl::getDst();
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to QoSConnector
//********************************************************************************
QoSConnector EventSinkPublishImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	QoSConnector sp( ce);
	if ( sp)
		return sp;

	QoSConnector empty;
	return empty;
}


//********************************************************************************
// getDst() return value is a ConnectionEnd
//********************************************************************************
BON::ConnectionEnd EventSourceDelegateImpl::getDst()
{
	return ConnectionImpl::getDst();
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd
//********************************************************************************
BON::ConnectionEnd EventSourceDelegateImpl::getSrc()
{
	return ConnectionImpl::getSrc();
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to QoSConnector
//********************************************************************************
QoSConnector EventSourcePublishImpl::getDst()
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
BON::ConnectionEnd EventSourcePublishImpl::getSrc()
{
	return ConnectionImpl::getSrc();
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to ExternalResources
//********************************************************************************
ExternalResources ExtResourceConnImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	ExternalResources sp( ce);
	if ( sp)
		return sp;

	ExternalResources empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to ComponentLib
//********************************************************************************
ComponentLib ExtResourceConnImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	ComponentLib sp( ce);
	if ( sp)
		return sp;

	ComponentLib empty;
	return empty;
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to ExternalReferenceEndPoint
//********************************************************************************
ExternalReferenceEndPoint ExternalDelegateImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	ExternalReferenceEndPoint sp( ce);
	if ( sp)
		return sp;

	ExternalReferenceEndPoint empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to Component
//********************************************************************************
Component ExternalDelegateImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	Component sp( ce);
	if ( sp)
		return sp;

	Component empty;
	return empty;
}


//********************************************************************************
// getDst() return value is a ConnectionEnd
//********************************************************************************
BON::ConnectionEnd FacetDelegateImpl::getDst()
{
	return ConnectionImpl::getDst();
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd
//********************************************************************************
BON::ConnectionEnd FacetDelegateImpl::getSrc()
{
	return ConnectionImpl::getSrc();
}


//********************************************************************************
// getDst() return value is a ConnectionEnd
//********************************************************************************
BON::ConnectionEnd FacetInvokeImpl::getDst()
{
	return ConnectionImpl::getDst();
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to QoSConnector
//********************************************************************************
QoSConnector FacetInvokeImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	QoSConnector sp( ce);
	if ( sp)
		return sp;

	QoSConnector empty;
	return empty;
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to InputActionBase
//********************************************************************************
InputActionBase FinishImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	InputActionBase sp( ce);
	if ( sp)
		return sp;

	InputActionBase empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to State
//********************************************************************************
State FinishImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	State sp( ce);
	if ( sp)
		return sp;

	State empty;
	return empty;
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to ComponentImplementationReference
//********************************************************************************
ComponentImplementationReference ImplementationImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	ComponentImplementationReference sp( ce);
	if ( sp)
		return sp;

	ComponentImplementationReference empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to ComponentPackage
//********************************************************************************
ComponentPackage ImplementationImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	ComponentPackage sp( ce);
	if ( sp)
		return sp;

	ComponentPackage empty;
	return empty;
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to Capability
//********************************************************************************
Capability ImplementationCapabilityImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	Capability sp( ce);
	if ( sp)
		return sp;

	Capability empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to ComponentImplementation
//********************************************************************************
ComponentImplementation ImplementationCapabilityImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	ComponentImplementation sp( ce);
	if ( sp)
		return sp;

	ComponentImplementation empty;
	return empty;
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to ImplementationDependency
//********************************************************************************
ImplementationDependency ImplementationDependsOnImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	ImplementationDependency sp( ce);
	if ( sp)
		return sp;

	ImplementationDependency empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to ComponentImplementation
//********************************************************************************
ComponentImplementation ImplementationDependsOnImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	ComponentImplementation sp( ce);
	if ( sp)
		return sp;

	ComponentImplementation empty;
	return empty;
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to ComponentRef
//********************************************************************************
ComponentRef ImplementsImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	ComponentRef sp( ce);
	if ( sp)
		return sp;

	ComponentRef empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to ComponentImplementation
//********************************************************************************
ComponentImplementation ImplementsImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	ComponentImplementation sp( ce);
	if ( sp)
		return sp;

	ComponentImplementation empty;
	return empty;
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to Property
//********************************************************************************
Property InfoPropertyImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	Property sp( ce);
	if ( sp)
		return sp;

	Property empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to ComponentImplementation
//********************************************************************************
ComponentImplementation InfoPropertyImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	ComponentImplementation sp( ce);
	if ( sp)
		return sp;

	ComponentImplementation empty;
	return empty;
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to InputActionBase
//********************************************************************************
InputActionBase InputImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	InputActionBase sp( ce);
	if ( sp)
		return sp;

	InputActionBase empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to InEventPort
//********************************************************************************
InEventPort InputImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	InEventPort sp( ce);
	if ( sp)
		return sp;

	InEventPort empty;
	return empty;
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to NodeReference
//********************************************************************************
NodeReference InstanceMappingImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	NodeReference sp( ce);
	if ( sp)
		return sp;

	NodeReference empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to CollocationGroup
//********************************************************************************
CollocationGroup InstanceMappingImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	CollocationGroup sp( ce);
	if ( sp)
		return sp;

	CollocationGroup empty;
	return empty;
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to Bridge
//********************************************************************************
Bridge Interconnect2BridgeImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	Bridge sp( ce);
	if ( sp)
		return sp;

	Bridge empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to Interconnect
//********************************************************************************
Interconnect Interconnect2BridgeImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	Interconnect sp( ce);
	if ( sp)
		return sp;

	Interconnect empty;
	return empty;
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to Node
//********************************************************************************
Node Interconnect2NodeImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	Node sp( ce);
	if ( sp)
		return sp;

	Node empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to Interconnect
//********************************************************************************
Interconnect Interconnect2NodeImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	Interconnect sp( ce);
	if ( sp)
		return sp;

	Interconnect empty;
	return empty;
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to Label
//********************************************************************************
Label LabelConnectionImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	Label sp( ce);
	if ( sp)
		return sp;

	Label empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to Member
//********************************************************************************
Member LabelConnectionImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	Member sp( ce);
	if ( sp)
		return sp;

	Member empty;
	return empty;
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to PrivateFlag
//********************************************************************************
PrivateFlag MakeMemberPrivateImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	PrivateFlag sp( ce);
	if ( sp)
		return sp;

	PrivateFlag empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to Member
//********************************************************************************
Member MakeMemberPrivateImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	Member sp( ce);
	if ( sp)
		return sp;

	Member empty;
	return empty;
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to Manageable
//********************************************************************************
Manageable ManagesComponentImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	Manageable sp( ce);
	if ( sp)
		return sp;

	Manageable empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to ComponentFactory
//********************************************************************************
ComponentFactory ManagesComponentImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	ComponentFactory sp( ce);
	if ( sp)
		return sp;

	ComponentFactory empty;
	return empty;
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to MetricsBase
//********************************************************************************
MetricsBase MetricConnectionImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	MetricsBase sp( ce);
	if ( sp)
		return sp;

	MetricsBase empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to OperationRef
//********************************************************************************
OperationRef MetricConnectionImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	OperationRef sp( ce);
	if ( sp)
		return sp;

	OperationRef empty;
	return empty;
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to ImplementationRequirement
//********************************************************************************
ImplementationRequirement MonolithDeployRequirementImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	ImplementationRequirement sp( ce);
	if ( sp)
		return sp;

	ImplementationRequirement empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to MonolithicImplementation
//********************************************************************************
MonolithicImplementation MonolithDeployRequirementImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	MonolithicImplementation sp( ce);
	if ( sp)
		return sp;

	MonolithicImplementation empty;
	return empty;
}


} // namespace PICML
