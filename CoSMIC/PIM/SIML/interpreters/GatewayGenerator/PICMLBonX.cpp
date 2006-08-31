#include "stdafx.h"
#include "PICMLBonX.h"
#include "PICMLBonX1.h"
#include "PICMLBonX2.h"
#include "PICMLBonX3.h"
#include "PICMLBonX4.h"

namespace BON
{

IMPLEMENT_BONEXTENSION( PICML::ComponentAnalyses, "PICML::ComponentAnalyses" );
IMPLEMENT_BONEXTENSION( PICML::ComponentBuild, "PICML::ComponentBuild" );
IMPLEMENT_BONEXTENSION( PICML::ComponentImplementations, "PICML::ComponentImplementations" );
IMPLEMENT_BONEXTENSION( PICML::ComponentPackages, "PICML::ComponentPackages" );
IMPLEMENT_BONEXTENSION( PICML::ComponentTypes, "PICML::ComponentTypes" );
IMPLEMENT_BONEXTENSION( PICML::DeploymentPlans, "PICML::DeploymentPlans" );
IMPLEMENT_BONEXTENSION( PICML::ImplementationArtifacts, "PICML::ImplementationArtifacts" );
IMPLEMENT_BONEXTENSION( PICML::InterfaceDefinitions, "PICML::InterfaceDefinitions" );
IMPLEMENT_BONEXTENSION( PICML::PackageConfigurations, "PICML::PackageConfigurations" );
IMPLEMENT_BONEXTENSION( PICML::PathDiagrams, "PICML::PathDiagrams" );
IMPLEMENT_BONEXTENSION( PICML::PredefinedTypes, "PICML::PredefinedTypes" );
IMPLEMENT_BONEXTENSION( PICML::Targets, "PICML::Targets" );
IMPLEMENT_BONEXTENSION( PICML::TopLevelPackages, "PICML::TopLevelPackages" );
IMPLEMENT_BONEXTENSION( PICML::WorkerLibraries, "PICML::WorkerLibraries" );
IMPLEMENT_ABSTRACT_BONEXTENSION( PICML::Prefixable );
IMPLEMENT_ABSTRACT_BONEXTENSION( PICML::SupportsInterfaces );
IMPLEMENT_ABSTRACT_BONEXTENSION( PICML::CommonPortAttrs );
IMPLEMENT_ABSTRACT_BONEXTENSION( PICML::ConstantType );
IMPLEMENT_ABSTRACT_BONEXTENSION( PICML::GraphVertex );
IMPLEMENT_ABSTRACT_BONEXTENSION( PICML::Manageable );
IMPLEMENT_ABSTRACT_BONEXTENSION( PICML::MemberType );
IMPLEMENT_ABSTRACT_BONEXTENSION( PICML::MultiInputBase );
IMPLEMENT_ABSTRACT_BONEXTENSION( PICML::Provideable );
IMPLEMENT_ABSTRACT_BONEXTENSION( PICML::Taggable );
IMPLEMENT_BONEXTENSION( PICML::ConnectedComponent, "PICML::ConnectedComponent" );
IMPLEMENT_BONEXTENSION( PICML::Exception, "PICML::Exception" );
IMPLEMENT_BONEXTENSION( PICML::File, "PICML::File" );
IMPLEMENT_ABSTRACT_BONEXTENSION( PICML::OperationBase );
IMPLEMENT_BONEXTENSION( PICML::Package, "PICML::Package" );
IMPLEMENT_BONEXTENSION( PICML::ReadonlyAttribute, "PICML::ReadonlyAttribute" );
IMPLEMENT_ABSTRACT_BONEXTENSION( PICML::NamedType );
IMPLEMENT_ABSTRACT_BONEXTENSION( PICML::Port );
IMPLEMENT_ABSTRACT_BONEXTENSION( PICML::PredefinedType );
IMPLEMENT_BONEXTENSION( PICML::DisplayNode, "PICML::DisplayNode" );
IMPLEMENT_BONEXTENSION( PICML::Environment, "PICML::Environment" );
IMPLEMENT_BONEXTENSION( PICML::ComponentRef, "PICML::ComponentRef" );
IMPLEMENT_BONEXTENSION( PICML::Constant, "PICML::Constant" );
IMPLEMENT_BONEXTENSION( PICML::Attribute, "PICML::Attribute" );
IMPLEMENT_BONEXTENSION( PICML::Component, "PICML::Component" );
IMPLEMENT_ABSTRACT_BONEXTENSION( PICML::HasExceptions );
IMPLEMENT_ABSTRACT_BONEXTENSION( PICML::Inheritable );
IMPLEMENT_BONEXTENSION( PICML::OnewayOperation, "PICML::OnewayOperation" );
IMPLEMENT_ABSTRACT_BONEXTENSION( PICML::NoInheritable );
IMPLEMENT_BONEXTENSION( PICML::Boolean, "PICML::Boolean" );
IMPLEMENT_BONEXTENSION( PICML::Byte, "PICML::Byte" );
IMPLEMENT_BONEXTENSION( PICML::GenericObject, "PICML::GenericObject" );
IMPLEMENT_BONEXTENSION( PICML::GenericValue, "PICML::GenericValue" );
IMPLEMENT_BONEXTENSION( PICML::GenericValueObject, "PICML::GenericValueObject" );
IMPLEMENT_BONEXTENSION( PICML::LongInteger, "PICML::LongInteger" );
IMPLEMENT_BONEXTENSION( PICML::RealNumber, "PICML::RealNumber" );
IMPLEMENT_BONEXTENSION( PICML::ShortInteger, "PICML::ShortInteger" );
IMPLEMENT_BONEXTENSION( PICML::String, "PICML::String" );
IMPLEMENT_BONEXTENSION( PICML::TypeEncoding, "PICML::TypeEncoding" );
IMPLEMENT_BONEXTENSION( PICML::TypeKind, "PICML::TypeKind" );
IMPLEMENT_BONEXTENSION( PICML::Boxed, "PICML::Boxed" );
IMPLEMENT_BONEXTENSION( PICML::InEventPort, "PICML::InEventPort" );
IMPLEMENT_BONEXTENSION( PICML::OutEventPort, "PICML::OutEventPort" );
IMPLEMENT_BONEXTENSION( PICML::ProvidedRequestPort, "PICML::ProvidedRequestPort" );
IMPLEMENT_BONEXTENSION( PICML::RequiredRequestPort, "PICML::RequiredRequestPort" );
IMPLEMENT_BONEXTENSION( PICML::Aggregate, "PICML::Aggregate" );
IMPLEMENT_BONEXTENSION( PICML::Enum, "PICML::Enum" );
IMPLEMENT_BONEXTENSION( PICML::FactoryOperation, "PICML::FactoryOperation" );
IMPLEMENT_ABSTRACT_BONEXTENSION( PICML::HasOperations );
IMPLEMENT_BONEXTENSION( PICML::LookupOperation, "PICML::LookupOperation" );
IMPLEMENT_BONEXTENSION( PICML::SwitchedAggregate, "PICML::SwitchedAggregate" );
} // end namespace BON

namespace PICML
{
//********************************************************************************
// getter for kind "BenchmarkAnalysis"
//********************************************************************************
std::set<BenchmarkAnalysis> ComponentAnalysesImpl::getBenchmarkAnalysis()
{
	std::set<BenchmarkAnalysis> res;
	std::set<BON::Object> kinds = FolderImpl::getChildObjects("PICML::BenchmarkAnalysis");
	for( std::set<BON::Object>::iterator i = kinds.begin(); i != kinds.end(); ++i)
	{
		BenchmarkAnalysis elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for kind "MPC"
//********************************************************************************
std::set<MPC> ComponentBuildImpl::getMPC()
{
	std::set<MPC> res;
	std::set<BON::Object> kinds = FolderImpl::getChildObjects("PICML::MPC");
	for( std::set<BON::Object>::iterator i = kinds.begin(); i != kinds.end(); ++i)
	{
		MPC elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for kind "ComponentImplementationContainer"
//********************************************************************************
std::set<ComponentImplementationContainer> ComponentImplementationsImpl::getComponentImplementationContainer()
{
	std::set<ComponentImplementationContainer> res;
	std::set<BON::Object> kinds = FolderImpl::getChildObjects("PICML::ComponentImplementationContainer");
	for( std::set<BON::Object>::iterator i = kinds.begin(); i != kinds.end(); ++i)
	{
		ComponentImplementationContainer elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for kind "PackageContainer"
//********************************************************************************
std::set<PackageContainer> ComponentPackagesImpl::getPackageContainer()
{
	std::set<PackageContainer> res;
	std::set<BON::Object> kinds = FolderImpl::getChildObjects("PICML::PackageContainer");
	for( std::set<BON::Object>::iterator i = kinds.begin(); i != kinds.end(); ++i)
	{
		PackageContainer elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for kind "ComponentContainer"
//********************************************************************************
std::set<ComponentContainer> ComponentTypesImpl::getComponentContainer()
{
	std::set<ComponentContainer> res;
	std::set<BON::Object> kinds = FolderImpl::getChildObjects("PICML::ComponentContainer");
	for( std::set<BON::Object>::iterator i = kinds.begin(); i != kinds.end(); ++i)
	{
		ComponentContainer elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for kind "DeploymentPlan"
//********************************************************************************
std::set<DeploymentPlan> DeploymentPlansImpl::getDeploymentPlan()
{
	std::set<DeploymentPlan> res;
	std::set<BON::Object> kinds = FolderImpl::getChildObjects("PICML::DeploymentPlan");
	for( std::set<BON::Object>::iterator i = kinds.begin(); i != kinds.end(); ++i)
	{
		DeploymentPlan elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for kind "ArtifactContainer"
//********************************************************************************
std::set<ArtifactContainer> ImplementationArtifactsImpl::getArtifactContainer()
{
	std::set<ArtifactContainer> res;
	std::set<BON::Object> kinds = FolderImpl::getChildObjects("PICML::ArtifactContainer");
	for( std::set<BON::Object>::iterator i = kinds.begin(); i != kinds.end(); ++i)
	{
		ArtifactContainer elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for kind "File"
//********************************************************************************
std::set<File> InterfaceDefinitionsImpl::getFile()
{
	std::set<File> res;
	std::set<BON::Object> kinds = FolderImpl::getChildObjects("PICML::File");
	for( std::set<BON::Object>::iterator i = kinds.begin(); i != kinds.end(); ++i)
	{
		File elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for kind "PackageConfigurationContainer"
//********************************************************************************
std::set<PackageConfigurationContainer> PackageConfigurationsImpl::getPackageConfigurationContainer()
{
	std::set<PackageConfigurationContainer> res;
	std::set<BON::Object> kinds = FolderImpl::getChildObjects("PICML::PackageConfigurationContainer");
	for( std::set<BON::Object>::iterator i = kinds.begin(); i != kinds.end(); ++i)
	{
		PackageConfigurationContainer elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for kind "Paths"
//********************************************************************************
std::set<Paths> PathDiagramsImpl::getPaths()
{
	std::set<Paths> res;
	std::set<BON::Object> kinds = FolderImpl::getChildObjects("PICML::Paths");
	for( std::set<BON::Object>::iterator i = kinds.begin(); i != kinds.end(); ++i)
	{
		Paths elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for kind "Boolean"
//********************************************************************************
std::set<Boolean> PredefinedTypesImpl::getBoolean()
{
	std::set<Boolean> res;
	std::set<BON::Object> kinds = FolderImpl::getChildObjects("PICML::Boolean");
	for( std::set<BON::Object>::iterator i = kinds.begin(); i != kinds.end(); ++i)
	{
		Boolean elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for kind "Byte"
//********************************************************************************
std::set<Byte> PredefinedTypesImpl::getByte()
{
	std::set<Byte> res;
	std::set<BON::Object> kinds = FolderImpl::getChildObjects("PICML::Byte");
	for( std::set<BON::Object>::iterator i = kinds.begin(); i != kinds.end(); ++i)
	{
		Byte elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for kind "GenericObject"
//********************************************************************************
std::set<GenericObject> PredefinedTypesImpl::getGenericObject()
{
	std::set<GenericObject> res;
	std::set<BON::Object> kinds = FolderImpl::getChildObjects("PICML::GenericObject");
	for( std::set<BON::Object>::iterator i = kinds.begin(); i != kinds.end(); ++i)
	{
		GenericObject elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for kind "GenericValue"
//********************************************************************************
std::set<GenericValue> PredefinedTypesImpl::getGenericValue()
{
	std::set<GenericValue> res;
	std::set<BON::Object> kinds = FolderImpl::getChildObjects("PICML::GenericValue");
	for( std::set<BON::Object>::iterator i = kinds.begin(); i != kinds.end(); ++i)
	{
		GenericValue elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for kind "GenericValueObject"
//********************************************************************************
std::set<GenericValueObject> PredefinedTypesImpl::getGenericValueObject()
{
	std::set<GenericValueObject> res;
	std::set<BON::Object> kinds = FolderImpl::getChildObjects("PICML::GenericValueObject");
	for( std::set<BON::Object>::iterator i = kinds.begin(); i != kinds.end(); ++i)
	{
		GenericValueObject elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for kind "LongInteger"
//********************************************************************************
std::set<LongInteger> PredefinedTypesImpl::getLongInteger()
{
	std::set<LongInteger> res;
	std::set<BON::Object> kinds = FolderImpl::getChildObjects("PICML::LongInteger");
	for( std::set<BON::Object>::iterator i = kinds.begin(); i != kinds.end(); ++i)
	{
		LongInteger elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// aggregated getter for kind "PredefinedType" and its descendants
//********************************************************************************
std::set<PredefinedType> PredefinedTypesImpl::getPredefinedType()
{
	std::set<PredefinedType> res;
	const int len = 11;
	std::set<BON::Object> kinds_vec[ len];
	kinds_vec[0] = FolderImpl::getChildObjects("PICML::Boolean");
	kinds_vec[1] = FolderImpl::getChildObjects("PICML::Byte");
	kinds_vec[2] = FolderImpl::getChildObjects("PICML::GenericObject");
	kinds_vec[3] = FolderImpl::getChildObjects("PICML::GenericValue");
	kinds_vec[4] = FolderImpl::getChildObjects("PICML::GenericValueObject");
	kinds_vec[5] = FolderImpl::getChildObjects("PICML::LongInteger");
	kinds_vec[6] = FolderImpl::getChildObjects("PICML::RealNumber");
	kinds_vec[7] = FolderImpl::getChildObjects("PICML::ShortInteger");
	kinds_vec[8] = FolderImpl::getChildObjects("PICML::String");
	kinds_vec[9] = FolderImpl::getChildObjects("PICML::TypeEncoding");
	kinds_vec[10] = FolderImpl::getChildObjects("PICML::TypeKind");
	for( int k = 0; k < len; ++k)
		for( std::set<BON::Object>::iterator i = kinds_vec[k].begin(); i != kinds_vec[k].end(); ++i)
		{
			PredefinedType elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
	return res;
}


//********************************************************************************
// getter for kind "RealNumber"
//********************************************************************************
std::set<RealNumber> PredefinedTypesImpl::getRealNumber()
{
	std::set<RealNumber> res;
	std::set<BON::Object> kinds = FolderImpl::getChildObjects("PICML::RealNumber");
	for( std::set<BON::Object>::iterator i = kinds.begin(); i != kinds.end(); ++i)
	{
		RealNumber elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for kind "ShortInteger"
//********************************************************************************
std::set<ShortInteger> PredefinedTypesImpl::getShortInteger()
{
	std::set<ShortInteger> res;
	std::set<BON::Object> kinds = FolderImpl::getChildObjects("PICML::ShortInteger");
	for( std::set<BON::Object>::iterator i = kinds.begin(); i != kinds.end(); ++i)
	{
		ShortInteger elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for kind "String"
//********************************************************************************
std::set<String> PredefinedTypesImpl::getString()
{
	std::set<String> res;
	std::set<BON::Object> kinds = FolderImpl::getChildObjects("PICML::String");
	for( std::set<BON::Object>::iterator i = kinds.begin(); i != kinds.end(); ++i)
	{
		String elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for kind "TypeEncoding"
//********************************************************************************
std::set<TypeEncoding> PredefinedTypesImpl::getTypeEncoding()
{
	std::set<TypeEncoding> res;
	std::set<BON::Object> kinds = FolderImpl::getChildObjects("PICML::TypeEncoding");
	for( std::set<BON::Object>::iterator i = kinds.begin(); i != kinds.end(); ++i)
	{
		TypeEncoding elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for kind "TypeKind"
//********************************************************************************
std::set<TypeKind> PredefinedTypesImpl::getTypeKind()
{
	std::set<TypeKind> res;
	std::set<BON::Object> kinds = FolderImpl::getChildObjects("PICML::TypeKind");
	for( std::set<BON::Object>::iterator i = kinds.begin(); i != kinds.end(); ++i)
	{
		TypeKind elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for kind "Domain"
//********************************************************************************
std::set<Domain> TargetsImpl::getDomain()
{
	std::set<Domain> res;
	std::set<BON::Object> kinds = FolderImpl::getChildObjects("PICML::Domain");
	for( std::set<BON::Object>::iterator i = kinds.begin(); i != kinds.end(); ++i)
	{
		Domain elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for kind "TopLevelPackageContainer"
//********************************************************************************
std::set<TopLevelPackageContainer> TopLevelPackagesImpl::getTopLevelPackageContainer()
{
	std::set<TopLevelPackageContainer> res;
	std::set<BON::Object> kinds = FolderImpl::getChildObjects("PICML::TopLevelPackageContainer");
	for( std::set<BON::Object>::iterator i = kinds.begin(); i != kinds.end(); ++i)
	{
		TopLevelPackageContainer elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for kind "WorkerLibrary"
//********************************************************************************
std::set<WorkerLibrary> WorkerLibrariesImpl::getWorkerLibrary()
{
	std::set<WorkerLibrary> res;
	std::set<BON::Object> kinds = FolderImpl::getChildObjects("PICML::WorkerLibrary");
	for( std::set<BON::Object>::iterator i = kinds.begin(); i != kinds.end(); ++i)
	{
		WorkerLibrary elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
//
//********************************************************************************
std::string PrefixableImpl::getPrefixTag()
{
	return FCOImpl::getAttribute("PrefixTag")->getStringValue();
}


//********************************************************************************
//
//********************************************************************************
void PrefixableImpl::setPrefixTag( const std::string& val)
{
	FCOImpl::getAttribute("PrefixTag")->setStringValue( val);
}


//********************************************************************************
// getter for role "Supports" among "Supports"s
//********************************************************************************
std::set<Supports> SupportsInterfacesImpl::getSupports()
{
	std::set<Supports> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::Supports");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Supports elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
//
//********************************************************************************
bool CommonPortAttrsImpl::isexclusiveProvider()
{
	return FCOImpl::getAttribute("exclusiveProvider")->getBooleanValue();
}


//********************************************************************************
//
//********************************************************************************
bool CommonPortAttrsImpl::isexclusiveUser()
{
	return FCOImpl::getAttribute("exclusiveUser")->getBooleanValue();
}


//********************************************************************************
//
//********************************************************************************
bool CommonPortAttrsImpl::isoptional()
{
	return FCOImpl::getAttribute("optional")->getBooleanValue();
}


//********************************************************************************
//
//********************************************************************************
void CommonPortAttrsImpl::setexclusiveProvider( bool val)
{
	FCOImpl::getAttribute("exclusiveProvider")->setBooleanValue( val);
}


//********************************************************************************
//
//********************************************************************************
void CommonPortAttrsImpl::setexclusiveUser( bool val)
{
	FCOImpl::getAttribute("exclusiveUser")->setBooleanValue( val);
}


//********************************************************************************
//
//********************************************************************************
void CommonPortAttrsImpl::setoptional( bool val)
{
	FCOImpl::getAttribute("optional")->setBooleanValue( val);
}


//********************************************************************************
// returns dst Edges
//********************************************************************************
std::multiset<Edge> GraphVertexImpl::getDstEdgeDsts()
{
	std::multiset<Edge> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("PICML::DstEdge");
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
std::set<SrcEdge> GraphVertexImpl::getInSrcEdgeLinks()
{
	std::set<SrcEdge> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
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
//
//********************************************************************************
std::set<DstEdge> GraphVertexImpl::getOutDstEdgeLinks()
{
	std::set<DstEdge> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
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
// returns src Edges
//********************************************************************************
std::multiset<Edge> GraphVertexImpl::getSrcEdgeSrcs()
{
	std::multiset<Edge> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("PICML::SrcEdge");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
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
std::set<ManagesComponent> ManageableImpl::getInManagesComponentLinks()
{
	std::set<ManagesComponent> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
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
// returns src ComponentFactorys
//********************************************************************************
std::multiset<ComponentFactory> ManageableImpl::getManagesComponentSrcs()
{
	std::multiset<ComponentFactory> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("PICML::ManagesComponent");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			ComponentFactory dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// returns dst InputActionBases
//********************************************************************************
std::multiset<InputActionBase> MultiInputBaseImpl::getMultiInputDsts()
{
	std::multiset<InputActionBase> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("PICML::MultiInput");
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
std::set<MultiInput> MultiInputBaseImpl::getOutMultiInputLinks()
{
	std::set<MultiInput> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
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
//
//********************************************************************************
std::string TaggableImpl::getSpecifyIdTag()
{
	return FCOImpl::getAttribute("SpecifyIdTag")->getStringValue();
}


//********************************************************************************
//
//********************************************************************************
std::string TaggableImpl::getVersionTag()
{
	return FCOImpl::getAttribute("VersionTag")->getStringValue();
}


//********************************************************************************
//
//********************************************************************************
void TaggableImpl::setSpecifyIdTag( const std::string& val)
{
	FCOImpl::getAttribute("SpecifyIdTag")->setStringValue( val);
}


//********************************************************************************
//
//********************************************************************************
void TaggableImpl::setVersionTag( const std::string& val)
{
	FCOImpl::getAttribute("VersionTag")->setStringValue( val);
}


//********************************************************************************
// getter for role "DstEdge" among "DstEdge"s
//********************************************************************************
std::set<DstEdge> ConnectedComponentImpl::getDstEdge()
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
// getter for role "InEventPort" among "Port"s and its descendants
//********************************************************************************
std::set<InEventPort> ConnectedComponentImpl::getInEventPort()
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
// getter for role "OutEventPort" among "Port"s and its descendants
//********************************************************************************
std::set<OutEventPort> ConnectedComponentImpl::getOutEventPort()
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
// aggregated getter for role "" among "Port"s and its descendants
//********************************************************************************
std::set<Port> ConnectedComponentImpl::getPort()
{
	std::set<Port> res;
	const int len = 4;
	std::set<BON::FCO> roles_vec[ len];
	roles_vec[0] = ModelImpl::getChildFCOsAs("PICML::InEventPort");
	roles_vec[1] = ModelImpl::getChildFCOsAs("PICML::OutEventPort");
	roles_vec[2] = ModelImpl::getChildFCOsAs("PICML::ProvidedRequestPort");
	roles_vec[3] = ModelImpl::getChildFCOsAs("PICML::RequiredRequestPort");
	for( int k = 0; k < len; ++k)
		for( std::set<BON::FCO>::iterator i = roles_vec[k].begin(); i != roles_vec[k].end(); ++i)
		{
			Port elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
	return res;
}


//********************************************************************************
// getter for role "ProvidedRequestPort" among "Port"s and its descendants
//********************************************************************************
std::set<ProvidedRequestPort> ConnectedComponentImpl::getProvidedRequestPort()
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
// getter for role "RequiredRequestPort" among "Port"s and its descendants
//********************************************************************************
std::set<RequiredRequestPort> ConnectedComponentImpl::getRequiredRequestPort()
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
// getter for role "Member" among "Member"s
//********************************************************************************
std::set<Member> ExceptionImpl::getMember()
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
// getter for role "Aggregate" among "NamedType"s and its descendants
//********************************************************************************
std::set<Aggregate> FileImpl::getAggregate()
{
	std::set<Aggregate> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::Aggregate");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Aggregate elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Alias" among "NamedType"s and its descendants
//********************************************************************************
std::set<Alias> FileImpl::getAlias()
{
	std::set<Alias> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::Alias");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Alias elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Boxed" among "NamedType"s and its descendants
//********************************************************************************
std::set<Boxed> FileImpl::getBoxed()
{
	std::set<Boxed> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::Boxed");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Boxed elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Collection" among "NamedType"s and its descendants
//********************************************************************************
std::set<Collection> FileImpl::getCollection()
{
	std::set<Collection> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::Collection");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Collection elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Component" among "NamedType"s and its descendants
//********************************************************************************
std::set<Component> FileImpl::getComponent()
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
// getter for role "ComponentFactory" among "NamedType"s and its descendants
//********************************************************************************
std::set<ComponentFactory> FileImpl::getComponentFactory()
{
	std::set<ComponentFactory> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::ComponentFactory");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		ComponentFactory elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "ComponentRef" among "ComponentRef"s
//********************************************************************************
std::set<ComponentRef> FileImpl::getComponentRef()
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
// getter for role "Constant" among "Constant"s
//********************************************************************************
std::set<Constant> FileImpl::getConstant()
{
	std::set<Constant> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::Constant");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Constant elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Enum" among "NamedType"s and its descendants
//********************************************************************************
std::set<Enum> FileImpl::getEnum()
{
	std::set<Enum> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::Enum");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Enum elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Event" among "NamedType"s and its descendants
//********************************************************************************
std::set<Event> FileImpl::getEvent()
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
// getter for role "Exception" among "Exception"s
//********************************************************************************
std::set<Exception> FileImpl::getException()
{
	std::set<Exception> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::Exception");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Exception elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "FileRef" among "FileRef"s
//********************************************************************************
std::set<FileRef> FileImpl::getFileRef()
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
// getter for role "ManagesComponent" among "ManagesComponent"s
//********************************************************************************
std::set<ManagesComponent> FileImpl::getManagesComponent()
{
	std::set<ManagesComponent> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::ManagesComponent");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		ManagesComponent elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// aggregated getter for role "" among "NamedType"s and its descendants
//********************************************************************************
std::set<NamedType> FileImpl::getNamedType()
{
	std::set<NamedType> res;
	const int len = 11;
	std::set<BON::FCO> roles_vec[ len];
	roles_vec[0] = ModelImpl::getChildFCOsAs("PICML::Aggregate");
	roles_vec[1] = ModelImpl::getChildFCOsAs("PICML::Component");
	roles_vec[2] = ModelImpl::getChildFCOsAs("PICML::ComponentFactory");
	roles_vec[3] = ModelImpl::getChildFCOsAs("PICML::Enum");
	roles_vec[4] = ModelImpl::getChildFCOsAs("PICML::Event");
	roles_vec[5] = ModelImpl::getChildFCOsAs("PICML::Object");
	roles_vec[6] = ModelImpl::getChildFCOsAs("PICML::SwitchedAggregate");
	roles_vec[7] = ModelImpl::getChildFCOsAs("PICML::ValueObject");
	roles_vec[8] = ModelImpl::getChildFCOsAs("PICML::Alias");
	roles_vec[9] = ModelImpl::getChildFCOsAs("PICML::Boxed");
	roles_vec[10] = ModelImpl::getChildFCOsAs("PICML::Collection");
	for( int k = 0; k < len; ++k)
		for( std::set<BON::FCO>::iterator i = roles_vec[k].begin(); i != roles_vec[k].end(); ++i)
		{
			NamedType elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
	return res;
}


//********************************************************************************
// getter for role "Object" among "NamedType"s and its descendants
//********************************************************************************
std::set<Object> FileImpl::getObject()
{
	std::set<Object> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::Object");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Object elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Package" among "Package"s
//********************************************************************************
std::set<Package> FileImpl::getPackage()
{
	std::set<Package> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::Package");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Package elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "SwitchedAggregate" among "NamedType"s and its descendants
//********************************************************************************
std::set<SwitchedAggregate> FileImpl::getSwitchedAggregate()
{
	std::set<SwitchedAggregate> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::SwitchedAggregate");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		SwitchedAggregate elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "ValueObject" among "NamedType"s and its descendants
//********************************************************************************
std::set<ValueObject> FileImpl::getValueObject()
{
	std::set<ValueObject> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::ValueObject");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		ValueObject elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "InParameter" among "InParameter"s
//********************************************************************************
std::set<InParameter> OperationBaseImpl::getInParameter()
{
	std::set<InParameter> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::InParameter");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		InParameter elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Aggregate" among "NamedType"s and its descendants
//********************************************************************************
std::set<Aggregate> PackageImpl::getAggregate()
{
	std::set<Aggregate> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::Aggregate");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Aggregate elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Alias" among "NamedType"s and its descendants
//********************************************************************************
std::set<Alias> PackageImpl::getAlias()
{
	std::set<Alias> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::Alias");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Alias elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Boxed" among "NamedType"s and its descendants
//********************************************************************************
std::set<Boxed> PackageImpl::getBoxed()
{
	std::set<Boxed> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::Boxed");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Boxed elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Collection" among "NamedType"s and its descendants
//********************************************************************************
std::set<Collection> PackageImpl::getCollection()
{
	std::set<Collection> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::Collection");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Collection elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Component" among "NamedType"s and its descendants
//********************************************************************************
std::set<Component> PackageImpl::getComponent()
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
// getter for role "ComponentFactory" among "NamedType"s and its descendants
//********************************************************************************
std::set<ComponentFactory> PackageImpl::getComponentFactory()
{
	std::set<ComponentFactory> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::ComponentFactory");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		ComponentFactory elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "ComponentRef" among "ComponentRef"s
//********************************************************************************
std::set<ComponentRef> PackageImpl::getComponentRef()
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
// getter for role "Constant" among "Constant"s
//********************************************************************************
std::set<Constant> PackageImpl::getConstant()
{
	std::set<Constant> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::Constant");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Constant elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Enum" among "NamedType"s and its descendants
//********************************************************************************
std::set<Enum> PackageImpl::getEnum()
{
	std::set<Enum> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::Enum");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Enum elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Event" among "NamedType"s and its descendants
//********************************************************************************
std::set<Event> PackageImpl::getEvent()
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
// getter for role "Exception" among "Exception"s
//********************************************************************************
std::set<Exception> PackageImpl::getException()
{
	std::set<Exception> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::Exception");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Exception elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "ManagesComponent" among "ManagesComponent"s
//********************************************************************************
std::set<ManagesComponent> PackageImpl::getManagesComponent()
{
	std::set<ManagesComponent> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::ManagesComponent");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		ManagesComponent elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// aggregated getter for role "" among "NamedType"s and its descendants
//********************************************************************************
std::set<NamedType> PackageImpl::getNamedType()
{
	std::set<NamedType> res;
	const int len = 11;
	std::set<BON::FCO> roles_vec[ len];
	roles_vec[0] = ModelImpl::getChildFCOsAs("PICML::Aggregate");
	roles_vec[1] = ModelImpl::getChildFCOsAs("PICML::Component");
	roles_vec[2] = ModelImpl::getChildFCOsAs("PICML::ComponentFactory");
	roles_vec[3] = ModelImpl::getChildFCOsAs("PICML::Enum");
	roles_vec[4] = ModelImpl::getChildFCOsAs("PICML::Event");
	roles_vec[5] = ModelImpl::getChildFCOsAs("PICML::Object");
	roles_vec[6] = ModelImpl::getChildFCOsAs("PICML::SwitchedAggregate");
	roles_vec[7] = ModelImpl::getChildFCOsAs("PICML::ValueObject");
	roles_vec[8] = ModelImpl::getChildFCOsAs("PICML::Alias");
	roles_vec[9] = ModelImpl::getChildFCOsAs("PICML::Boxed");
	roles_vec[10] = ModelImpl::getChildFCOsAs("PICML::Collection");
	for( int k = 0; k < len; ++k)
		for( std::set<BON::FCO>::iterator i = roles_vec[k].begin(); i != roles_vec[k].end(); ++i)
		{
			NamedType elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
	return res;
}


//********************************************************************************
// getter for role "Object" among "NamedType"s and its descendants
//********************************************************************************
std::set<Object> PackageImpl::getObject()
{
	std::set<Object> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::Object");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Object elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Package" among "Package"s
//********************************************************************************
std::set<Package> PackageImpl::getPackage()
{
	std::set<Package> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::Package");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Package elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "SwitchedAggregate" among "NamedType"s and its descendants
//********************************************************************************
std::set<SwitchedAggregate> PackageImpl::getSwitchedAggregate()
{
	std::set<SwitchedAggregate> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::SwitchedAggregate");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		SwitchedAggregate elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "ValueObject" among "NamedType"s and its descendants
//********************************************************************************
std::set<ValueObject> PackageImpl::getValueObject()
{
	std::set<ValueObject> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::ValueObject");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		ValueObject elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// returns src AttributeMappings and referenceports
//********************************************************************************
std::multiset<BON::ConnectionEnd> ReadonlyAttributeImpl::getAttributeDelegateSrcs()
{
	return BON::ConnectionEndImpl::getInConnEnds("PICML::AttributeDelegate");
}


//********************************************************************************
// returns dst Propertys
//********************************************************************************
std::multiset<Property> ReadonlyAttributeImpl::getAttributeValueDsts()
{
	std::multiset<Property> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("PICML::AttributeValue");
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
std::set<AttributeDelegate> ReadonlyAttributeImpl::getInAttributeDelegateLinks()
{
	std::set<AttributeDelegate> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
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
std::set<AttributeValue> ReadonlyAttributeImpl::getOutAttributeValueLinks()
{
	std::set<AttributeValue> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
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
// getter for role "AttributeMember" among "AttributeMember"s
//********************************************************************************
std::set<AttributeMember> ReadonlyAttributeImpl::getAttributeMember()
{
	std::set<AttributeMember> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::AttributeMember");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		AttributeMember elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "GetException" among "GetException"s
//********************************************************************************
std::set<GetException> ReadonlyAttributeImpl::getGetException()
{
	std::set<GetException> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::GetException");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		GetException elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
//
//********************************************************************************
std::set<PortQoS> PortImpl::getOutPortQoSLinks()
{
	std::set<PortQoS> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
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
// returns dst ReqQoSBases
//********************************************************************************
std::multiset<ReqQoSBase> PortImpl::getPortQoSDsts()
{
	std::multiset<ReqQoSBase> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("PICML::PortQoS");
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
// returns dst Propertys
//********************************************************************************
std::multiset<Property> ComponentRefImpl::getComponentConfigPropertyDsts()
{
	std::multiset<Property> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("PICML::ComponentConfigProperty");
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
std::multiset<Property> ComponentRefImpl::getComponentInfoPropertyDsts()
{
	std::multiset<Property> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("PICML::ComponentInfoProperty");
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
// returns dst ComponentPropertyDescriptions
//********************************************************************************
std::multiset<ComponentPropertyDescription> ComponentRefImpl::getComponentPropertyDsts()
{
	std::multiset<ComponentPropertyDescription> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("PICML::ComponentProperty");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
		{
			ComponentPropertyDescription dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// returns src ComponentImplementations
//********************************************************************************
std::multiset<ComponentImplementation> ComponentRefImpl::getImplementsSrcs()
{
	std::multiset<ComponentImplementation> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("PICML::Implements");
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
std::set<Implements> ComponentRefImpl::getInImplementsLinks()
{
	std::set<Implements> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
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
std::set<PackageInterface> ComponentRefImpl::getInPackageInterfaceLinks()
{
	std::set<PackageInterface> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
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
//
//********************************************************************************
std::set<ComponentConfigProperty> ComponentRefImpl::getOutComponentConfigPropertyLinks()
{
	std::set<ComponentConfigProperty> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
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
std::set<ComponentInfoProperty> ComponentRefImpl::getOutComponentInfoPropertyLinks()
{
	std::set<ComponentInfoProperty> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
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
std::set<ComponentProperty> ComponentRefImpl::getOutComponentPropertyLinks()
{
	std::set<ComponentProperty> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
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
// returns src ComponentPackages
//********************************************************************************
std::multiset<ComponentPackage> ComponentRefImpl::getPackageInterfaceSrcs()
{
	std::multiset<ComponentPackage> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("PICML::PackageInterface");
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
Component ComponentRefImpl::getComponent()
{
	BON::FCO r = BON::ReferenceImpl::getReferred();
	return Component(r);
}


//********************************************************************************
//
//********************************************************************************
std::string ConstantImpl::getvalue()
{
	return FCOImpl::getAttribute("value")->getStringValue();
}


//********************************************************************************
//
//********************************************************************************
void ConstantImpl::setvalue( const std::string& val)
{
	FCOImpl::getAttribute("value")->setStringValue( val);
}


//********************************************************************************
//
//********************************************************************************
ConstantType ConstantImpl::getConstantType()
{
	BON::FCO r = BON::ReferenceImpl::getReferred();
	return ConstantType(r);
}


//********************************************************************************
// getter for role "SetException" among "SetException"s
//********************************************************************************
std::set<SetException> AttributeImpl::getSetException()
{
	std::set<SetException> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::SetException");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		SetException elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// returns dst Propertys
//********************************************************************************
std::multiset<Property> ComponentImpl::getAssemblyConfigPropertyDsts()
{
	std::multiset<Property> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("PICML::AssemblyConfigProperty");
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
// returns dst Requirements
//********************************************************************************
std::multiset<Requirement> ComponentImpl::getAssemblyselectRequirementDsts()
{
	std::multiset<Requirement> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("PICML::AssemblyselectRequirement");
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
// returns dst ExternalReferenceEndPoints
//********************************************************************************
std::multiset<ExternalReferenceEndPoint> ComponentImpl::getExternalDelegateDsts()
{
	std::multiset<ExternalReferenceEndPoint> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("PICML::ExternalDelegate");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
		{
			ExternalReferenceEndPoint dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
//
//********************************************************************************
std::set<AssemblyConfigProperty> ComponentImpl::getOutAssemblyConfigPropertyLinks()
{
	std::set<AssemblyConfigProperty> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
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
std::set<AssemblyselectRequirement> ComponentImpl::getOutAssemblyselectRequirementLinks()
{
	std::set<AssemblyselectRequirement> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
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
std::set<ExternalDelegate> ComponentImpl::getOutExternalDelegateLinks()
{
	std::set<ExternalDelegate> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
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
std::string ComponentImpl::getUUID()
{
	return FCOImpl::getAttribute("UUID")->getStringValue();
}


//********************************************************************************
//
//********************************************************************************
std::string ComponentImpl::getlabel()
{
	return FCOImpl::getAttribute("label")->getStringValue();
}


//********************************************************************************
//
//********************************************************************************
void ComponentImpl::setUUID( const std::string& val)
{
	FCOImpl::getAttribute("UUID")->setStringValue( val);
}


//********************************************************************************
//
//********************************************************************************
void ComponentImpl::setlabel( const std::string& val)
{
	FCOImpl::getAttribute("label")->setStringValue( val);
}


//********************************************************************************
// getter for role "Action" among "ActionBase"s and its descendants
//********************************************************************************
std::set<Action> ComponentImpl::getAction()
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
// aggregated getter for role "" among "ActionBase"s and its descendants
//********************************************************************************
std::set<ActionBase> ComponentImpl::getActionBase()
{
	std::set<ActionBase> res;
	const int len = 5;
	std::set<BON::FCO> roles_vec[ len];
	roles_vec[0] = ModelImpl::getChildFCOsAs("PICML::Action");
	roles_vec[1] = ModelImpl::getChildFCOsAs("PICML::CompositeAction");
	roles_vec[2] = ModelImpl::getChildFCOsAs("PICML::InputAction");
	roles_vec[3] = ModelImpl::getChildFCOsAs("PICML::OutputAction");
	roles_vec[4] = ModelImpl::getChildFCOsAs("PICML::PeriodicAction");
	for( int k = 0; k < len; ++k)
		for( std::set<BON::FCO>::iterator i = roles_vec[k].begin(); i != roles_vec[k].end(); ++i)
		{
			ActionBase elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
	return res;
}


//********************************************************************************
// getter for role "Attribute" among "ReadonlyAttribute"s and its descendants
//********************************************************************************
std::set<Attribute> ComponentImpl::getAttribute()
{
	std::set<Attribute> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::Attribute");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Attribute elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "CompositeAction" among "ActionBase"s and its descendants
//********************************************************************************
std::set<CompositeAction> ComponentImpl::getCompositeAction()
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
std::set<Effect> ComponentImpl::getEffect()
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
// getter for role "Environment" among "Environment"s
//********************************************************************************
std::set<Environment> ComponentImpl::getEnvironment()
{
	std::set<Environment> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::Environment");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Environment elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Finish" among "Finish"s
//********************************************************************************
std::set<Finish> ComponentImpl::getFinish()
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
// getter for role "InEventPort" among "Port"s and its descendants
//********************************************************************************
std::set<InEventPort> ComponentImpl::getInEventPort()
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
// getter for role "Input" among "Input"s
//********************************************************************************
std::set<Input> ComponentImpl::getInput()
{
	std::set<Input> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::Input");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Input elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "InputAction" among "ActionBase"s and its descendants
//********************************************************************************
std::set<InputAction> ComponentImpl::getInputAction()
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
// getter for role "MultiInput" among "MultiInput"s
//********************************************************************************
std::set<MultiInput> ComponentImpl::getMultiInput()
{
	std::set<MultiInput> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::MultiInput");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		MultiInput elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "OutEventPort" among "Port"s and its descendants
//********************************************************************************
std::set<OutEventPort> ComponentImpl::getOutEventPort()
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
// getter for role "OutputAction" among "ActionBase"s and its descendants
//********************************************************************************
std::set<OutputAction> ComponentImpl::getOutputAction()
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
// getter for role "PeriodicAction" among "ActionBase"s and its descendants
//********************************************************************************
std::set<PeriodicAction> ComponentImpl::getPeriodicAction()
{
	std::set<PeriodicAction> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::PeriodicAction");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		PeriodicAction elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// aggregated getter for role "" among "Port"s and its descendants
//********************************************************************************
std::set<Port> ComponentImpl::getPort()
{
	std::set<Port> res;
	const int len = 4;
	std::set<BON::FCO> roles_vec[ len];
	roles_vec[0] = ModelImpl::getChildFCOsAs("PICML::InEventPort");
	roles_vec[1] = ModelImpl::getChildFCOsAs("PICML::OutEventPort");
	roles_vec[2] = ModelImpl::getChildFCOsAs("PICML::ProvidedRequestPort");
	roles_vec[3] = ModelImpl::getChildFCOsAs("PICML::RequiredRequestPort");
	for( int k = 0; k < len; ++k)
		for( std::set<BON::FCO>::iterator i = roles_vec[k].begin(); i != roles_vec[k].end(); ++i)
		{
			Port elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
	return res;
}


//********************************************************************************
// getter for role "ProvidedRequestPort" among "Port"s and its descendants
//********************************************************************************
std::set<ProvidedRequestPort> ComponentImpl::getProvidedRequestPort()
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
// getter for role "ReadonlyAttribute" among "ReadonlyAttribute"s and its descendants
//********************************************************************************
std::set<ReadonlyAttribute> ComponentImpl::getReadonlyAttribute()
{
	std::set<ReadonlyAttribute> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::ReadonlyAttribute");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		ReadonlyAttribute elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// aggregated getter for role "" among "ReadonlyAttribute"s and its descendants
//********************************************************************************
std::set<ReadonlyAttribute> ComponentImpl::getReadonlyAttribute(int dummy)
{
	std::set<ReadonlyAttribute> res;
	const int len = 2;
	std::set<BON::FCO> roles_vec[ len];
	roles_vec[0] = ModelImpl::getChildFCOsAs("PICML::Attribute");
	roles_vec[1] = ModelImpl::getChildFCOsAs("PICML::ReadonlyAttribute");
	for( int k = 0; k < len; ++k)
		for( std::set<BON::FCO>::iterator i = roles_vec[k].begin(); i != roles_vec[k].end(); ++i)
		{
			ReadonlyAttribute elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
	return res;
}


//********************************************************************************
// getter for role "RequiredRequestPort" among "Port"s and its descendants
//********************************************************************************
std::set<RequiredRequestPort> ComponentImpl::getRequiredRequestPort()
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
// getter for role "State" among "State"s
//********************************************************************************
std::set<State> ComponentImpl::getState()
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
std::set<Transition> ComponentImpl::getTransition()
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
// getter for role "Variable" among "Variable"s
//********************************************************************************
std::set<Variable> ComponentImpl::getVariable()
{
	std::set<Variable> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::Variable");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Variable elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "WorkerType" among "WorkerType"s
//********************************************************************************
std::set<WorkerType> ComponentImpl::getWorkerType()
{
	std::set<WorkerType> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::WorkerType");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		WorkerType elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "ExceptionRef" among "ExceptionRef"s
//********************************************************************************
std::set<ExceptionRef> HasExceptionsImpl::getExceptionRef()
{
	std::set<ExceptionRef> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::ExceptionRef");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		ExceptionRef elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Attribute" among "ReadonlyAttribute"s and its descendants
//********************************************************************************
std::set<Attribute> InheritableImpl::getAttribute()
{
	std::set<Attribute> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::Attribute");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Attribute elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Inherits" among "Inherits"s
//********************************************************************************
std::set<Inherits> InheritableImpl::getInherits()
{
	std::set<Inherits> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::Inherits");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Inherits elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "ReadonlyAttribute" among "ReadonlyAttribute"s and its descendants
//********************************************************************************
std::set<ReadonlyAttribute> InheritableImpl::getReadonlyAttribute()
{
	std::set<ReadonlyAttribute> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::ReadonlyAttribute");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		ReadonlyAttribute elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// aggregated getter for role "" among "ReadonlyAttribute"s and its descendants
//********************************************************************************
std::set<ReadonlyAttribute> InheritableImpl::getReadonlyAttribute(int dummy)
{
	std::set<ReadonlyAttribute> res;
	const int len = 2;
	std::set<BON::FCO> roles_vec[ len];
	roles_vec[0] = ModelImpl::getChildFCOsAs("PICML::Attribute");
	roles_vec[1] = ModelImpl::getChildFCOsAs("PICML::ReadonlyAttribute");
	for( int k = 0; k < len; ++k)
		for( std::set<BON::FCO>::iterator i = roles_vec[k].begin(); i != roles_vec[k].end(); ++i)
		{
			ReadonlyAttribute elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
	return res;
}


//********************************************************************************
//
//********************************************************************************
MemberType BoxedImpl::getMemberType()
{
	BON::FCO r = BON::ReferenceImpl::getReferred();
	return MemberType(r);
}


//********************************************************************************
// returns dst InEventPorts and referenceports
//********************************************************************************
std::multiset<BON::ConnectionEnd> InEventPortImpl::getEventSinkDelegateDsts()
{
	return BON::ConnectionEndImpl::getOutConnEnds("PICML::EventSinkDelegate");
}


//********************************************************************************
//
//********************************************************************************
std::set<EventSinkDelegate> InEventPortImpl::getEventSinkDelegateLinks()
{
	std::set<EventSinkDelegate> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		EventSinkDelegate c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// returns src InEventPorts and referenceports
//********************************************************************************
std::multiset<BON::ConnectionEnd> InEventPortImpl::getEventSinkDelegateSrcs()
{
	return BON::ConnectionEndImpl::getInConnEnds("PICML::EventSinkDelegate");
}


//********************************************************************************
// returns src QoSConnectors
//********************************************************************************
std::multiset<QoSConnector> InEventPortImpl::getEventSinkPublishSrcs()
{
	std::multiset<QoSConnector> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("PICML::EventSinkPublish");
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
std::set<EventSinkDelegate> InEventPortImpl::getInEventSinkDelegateLinks()
{
	std::set<EventSinkDelegate> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		EventSinkDelegate c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
//
//********************************************************************************
std::set<EventSinkPublish> InEventPortImpl::getInEventSinkPublishLinks()
{
	std::set<EventSinkPublish> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
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
std::set<deliverTo> InEventPortImpl::getIndeliverToLinks()
{
	std::set<deliverTo> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
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
//
//********************************************************************************
std::set<emit> InEventPortImpl::getInemitLinks()
{
	std::set<emit> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		emit c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// returns dst InputActionBases
//********************************************************************************
std::multiset<InputActionBase> InEventPortImpl::getInputDsts()
{
	std::multiset<InputActionBase> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("PICML::Input");
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
std::set<EventSinkDelegate> InEventPortImpl::getOutEventSinkDelegateLinks()
{
	std::set<EventSinkDelegate> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		EventSinkDelegate c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
//
//********************************************************************************
std::set<Input> InEventPortImpl::getOutInputLinks()
{
	std::set<Input> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
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
// returns src PublishConnectors
//********************************************************************************
std::multiset<PublishConnector> InEventPortImpl::getdeliverToSrcs()
{
	std::multiset<PublishConnector> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("PICML::deliverTo");
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
// returns src OutEventPorts and referenceports
//********************************************************************************
std::multiset<BON::ConnectionEnd> InEventPortImpl::getemitSrcs()
{
	return BON::ConnectionEndImpl::getInConnEnds("PICML::emit");
}


//********************************************************************************
//
//********************************************************************************
Event InEventPortImpl::getEvent()
{
	BON::FCO r = BON::ReferenceImpl::getReferred();
	return Event(r);
}


//********************************************************************************
// returns dst OutEventPorts and referenceports
//********************************************************************************
std::multiset<BON::ConnectionEnd> OutEventPortImpl::getEventSourceDelegateDsts()
{
	return BON::ConnectionEndImpl::getOutConnEnds("PICML::EventSourceDelegate");
}


//********************************************************************************
//
//********************************************************************************
std::set<EventSourceDelegate> OutEventPortImpl::getEventSourceDelegateLinks()
{
	std::set<EventSourceDelegate> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		EventSourceDelegate c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// returns src OutEventPorts and referenceports
//********************************************************************************
std::multiset<BON::ConnectionEnd> OutEventPortImpl::getEventSourceDelegateSrcs()
{
	return BON::ConnectionEndImpl::getInConnEnds("PICML::EventSourceDelegate");
}


//********************************************************************************
// returns dst QoSConnectors
//********************************************************************************
std::multiset<QoSConnector> OutEventPortImpl::getEventSourcePublishDsts()
{
	std::multiset<QoSConnector> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("PICML::EventSourcePublish");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
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
std::set<EventSourceDelegate> OutEventPortImpl::getInEventSourceDelegateLinks()
{
	std::set<EventSourceDelegate> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		EventSourceDelegate c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
//
//********************************************************************************
std::set<EventSourceDelegate> OutEventPortImpl::getOutEventSourceDelegateLinks()
{
	std::set<EventSourceDelegate> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		EventSourceDelegate c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
//
//********************************************************************************
std::set<EventSourcePublish> OutEventPortImpl::getOutEventSourcePublishLinks()
{
	std::set<EventSourcePublish> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
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
std::set<emit> OutEventPortImpl::getOutemitLinks()
{
	std::set<emit> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		emit c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
//
//********************************************************************************
std::set<publish> OutEventPortImpl::getOutpublishLinks()
{
	std::set<publish> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
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
// returns dst InEventPorts and referenceports
//********************************************************************************
std::multiset<BON::ConnectionEnd> OutEventPortImpl::getemitDsts()
{
	return BON::ConnectionEndImpl::getOutConnEnds("PICML::emit");
}


//********************************************************************************
// returns dst PublishConnectors
//********************************************************************************
std::multiset<PublishConnector> OutEventPortImpl::getpublishDsts()
{
	std::multiset<PublishConnector> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("PICML::publish");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
		{
			PublishConnector dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
//
//********************************************************************************
OutEventPortImpl::out_event_port_type_Type OutEventPortImpl::getout_event_port_type()
{
	std::string val = FCOImpl::getAttribute("out_event_port_type")->getStringValue();

	if ( val == "DirectConnect") return DirectConnect_out_event_port_type_Type;
	else if ( val == "RTEventChannel") return RTEventChannel_out_event_port_type_Type;
	else throw("PICML::None of the possible items");
}


//********************************************************************************
//
//********************************************************************************
bool OutEventPortImpl::issingle_destination()
{
	return FCOImpl::getAttribute("single_destination")->getBooleanValue();
}


//********************************************************************************
//
//********************************************************************************
void OutEventPortImpl::setout_event_port_type( OutEventPortImpl::out_event_port_type_Type val)
{
	std::string str_val = "";

	if ( val == DirectConnect_out_event_port_type_Type) str_val = "DirectConnect";
	else if ( val == RTEventChannel_out_event_port_type_Type) str_val = "RTEventChannel";
	else throw("PICML::None of the possible items");

	FCOImpl::getAttribute("out_event_port_type")->setStringValue( str_val);
}


//********************************************************************************
//
//********************************************************************************
void OutEventPortImpl::setsingle_destination( bool val)
{
	FCOImpl::getAttribute("single_destination")->setBooleanValue( val);
}


//********************************************************************************
//
//********************************************************************************
Event OutEventPortImpl::getEvent()
{
	BON::FCO r = BON::ReferenceImpl::getReferred();
	return Event(r);
}


//********************************************************************************
// returns dst ProvidedRequestPorts and referenceports
//********************************************************************************
std::multiset<BON::ConnectionEnd> ProvidedRequestPortImpl::getFacetDelegateDsts()
{
	return BON::ConnectionEndImpl::getOutConnEnds("PICML::FacetDelegate");
}


//********************************************************************************
//
//********************************************************************************
std::set<FacetDelegate> ProvidedRequestPortImpl::getFacetDelegateLinks()
{
	std::set<FacetDelegate> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		FacetDelegate c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// returns src ProvidedRequestPorts and referenceports
//********************************************************************************
std::multiset<BON::ConnectionEnd> ProvidedRequestPortImpl::getFacetDelegateSrcs()
{
	return BON::ConnectionEndImpl::getInConnEnds("PICML::FacetDelegate");
}


//********************************************************************************
// returns src QoSConnectors
//********************************************************************************
std::multiset<QoSConnector> ProvidedRequestPortImpl::getFacetInvokeSrcs()
{
	std::multiset<QoSConnector> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("PICML::FacetInvoke");
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
std::set<FacetDelegate> ProvidedRequestPortImpl::getInFacetDelegateLinks()
{
	std::set<FacetDelegate> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		FacetDelegate c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
//
//********************************************************************************
std::set<FacetInvoke> ProvidedRequestPortImpl::getInFacetInvokeLinks()
{
	std::set<FacetInvoke> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
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
std::set<invoke> ProvidedRequestPortImpl::getIninvokeLinks()
{
	std::set<invoke> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		invoke c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
//
//********************************************************************************
std::set<FacetDelegate> ProvidedRequestPortImpl::getOutFacetDelegateLinks()
{
	std::set<FacetDelegate> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		FacetDelegate c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// returns src RequiredRequestPorts and referenceports
//********************************************************************************
std::multiset<BON::ConnectionEnd> ProvidedRequestPortImpl::getinvokeSrcs()
{
	return BON::ConnectionEndImpl::getInConnEnds("PICML::invoke");
}


//********************************************************************************
//
//********************************************************************************
Provideable ProvidedRequestPortImpl::getProvideable()
{
	BON::FCO r = BON::ReferenceImpl::getReferred();
	return Provideable(r);
}


//********************************************************************************
//
//********************************************************************************
std::set<ReceptacleDelegate> RequiredRequestPortImpl::getInReceptacleDelegateLinks()
{
	std::set<ReceptacleDelegate> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		ReceptacleDelegate c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
//
//********************************************************************************
std::set<RecepInvoke> RequiredRequestPortImpl::getOutRecepInvokeLinks()
{
	std::set<RecepInvoke> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
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
std::set<ReceptacleDelegate> RequiredRequestPortImpl::getOutReceptacleDelegateLinks()
{
	std::set<ReceptacleDelegate> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		ReceptacleDelegate c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
//
//********************************************************************************
std::set<invoke> RequiredRequestPortImpl::getOutinvokeLinks()
{
	std::set<invoke> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		invoke c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// returns dst QoSConnectors
//********************************************************************************
std::multiset<QoSConnector> RequiredRequestPortImpl::getRecepInvokeDsts()
{
	std::multiset<QoSConnector> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("PICML::RecepInvoke");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
		{
			QoSConnector dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// returns dst RequiredRequestPorts and referenceports
//********************************************************************************
std::multiset<BON::ConnectionEnd> RequiredRequestPortImpl::getReceptacleDelegateDsts()
{
	return BON::ConnectionEndImpl::getOutConnEnds("PICML::ReceptacleDelegate");
}


//********************************************************************************
//
//********************************************************************************
std::set<ReceptacleDelegate> RequiredRequestPortImpl::getReceptacleDelegateLinks()
{
	std::set<ReceptacleDelegate> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		ReceptacleDelegate c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// returns src RequiredRequestPorts and referenceports
//********************************************************************************
std::multiset<BON::ConnectionEnd> RequiredRequestPortImpl::getReceptacleDelegateSrcs()
{
	return BON::ConnectionEndImpl::getInConnEnds("PICML::ReceptacleDelegate");
}


//********************************************************************************
// returns dst ProvidedRequestPorts and referenceports
//********************************************************************************
std::multiset<BON::ConnectionEnd> RequiredRequestPortImpl::getinvokeDsts()
{
	return BON::ConnectionEndImpl::getOutConnEnds("PICML::invoke");
}


//********************************************************************************
//
//********************************************************************************
bool RequiredRequestPortImpl::ismultiple_connections()
{
	return FCOImpl::getAttribute("multiple_connections")->getBooleanValue();
}


//********************************************************************************
//
//********************************************************************************
void RequiredRequestPortImpl::setmultiple_connections( bool val)
{
	FCOImpl::getAttribute("multiple_connections")->setBooleanValue( val);
}


//********************************************************************************
//
//********************************************************************************
Provideable RequiredRequestPortImpl::getProvideable()
{
	BON::FCO r = BON::ReferenceImpl::getReferred();
	return Provideable(r);
}


//********************************************************************************
// getter for role "Member" among "Member"s
//********************************************************************************
std::set<Member> AggregateImpl::getMember()
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
// getter for role "EnumValue" among "EnumValue"s
//********************************************************************************
std::set<EnumValue> EnumImpl::getEnumValue()
{
	std::set<EnumValue> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::EnumValue");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		EnumValue elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Aggregate" among "NoInheritable"s and its descendants
//********************************************************************************
std::set<Aggregate> HasOperationsImpl::getAggregate()
{
	std::set<Aggregate> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::Aggregate");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Aggregate elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Alias" among "NoInheritable"s and its descendants
//********************************************************************************
std::set<Alias> HasOperationsImpl::getAlias()
{
	std::set<Alias> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::Alias");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Alias elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Collection" among "NoInheritable"s and its descendants
//********************************************************************************
std::set<Collection> HasOperationsImpl::getCollection()
{
	std::set<Collection> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::Collection");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Collection elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Constant" among "Constant"s
//********************************************************************************
std::set<Constant> HasOperationsImpl::getConstant()
{
	std::set<Constant> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::Constant");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Constant elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Enum" among "NoInheritable"s and its descendants
//********************************************************************************
std::set<Enum> HasOperationsImpl::getEnum()
{
	std::set<Enum> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::Enum");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Enum elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Exception" among "Exception"s
//********************************************************************************
std::set<Exception> HasOperationsImpl::getException()
{
	std::set<Exception> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::Exception");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Exception elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// aggregated getter for role "" among "NoInheritable"s and its descendants
//********************************************************************************
std::set<NoInheritable> HasOperationsImpl::getNoInheritable()
{
	std::set<NoInheritable> res;
	const int len = 5;
	std::set<BON::FCO> roles_vec[ len];
	roles_vec[0] = ModelImpl::getChildFCOsAs("PICML::Aggregate");
	roles_vec[1] = ModelImpl::getChildFCOsAs("PICML::Enum");
	roles_vec[2] = ModelImpl::getChildFCOsAs("PICML::SwitchedAggregate");
	roles_vec[3] = ModelImpl::getChildFCOsAs("PICML::Alias");
	roles_vec[4] = ModelImpl::getChildFCOsAs("PICML::Collection");
	for( int k = 0; k < len; ++k)
		for( std::set<BON::FCO>::iterator i = roles_vec[k].begin(); i != roles_vec[k].end(); ++i)
		{
			NoInheritable elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
	return res;
}


//********************************************************************************
// getter for role "OnewayOperation" among "OnewayOperation"s
//********************************************************************************
std::set<OnewayOperation> HasOperationsImpl::getOnewayOperation()
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
// getter for role "SwitchedAggregate" among "NoInheritable"s and its descendants
//********************************************************************************
std::set<SwitchedAggregate> HasOperationsImpl::getSwitchedAggregate()
{
	std::set<SwitchedAggregate> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::SwitchedAggregate");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		SwitchedAggregate elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "TwowayOperation" among "TwowayOperation"s
//********************************************************************************
std::set<TwowayOperation> HasOperationsImpl::getTwowayOperation()
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
// getter for role "Discriminator" among "Discriminator"s
//********************************************************************************
std::set<Discriminator> SwitchedAggregateImpl::getDiscriminator()
{
	std::set<Discriminator> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::Discriminator");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Discriminator elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Label" among "Label"s
//********************************************************************************
std::set<Label> SwitchedAggregateImpl::getLabel()
{
	std::set<Label> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::Label");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Label elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "LabelConnection" among "LabelConnection"s
//********************************************************************************
std::set<LabelConnection> SwitchedAggregateImpl::getLabelConnection()
{
	std::set<LabelConnection> res;
	std::set<BON::FCO> roles = ModelImpl::getChildFCOsAs("PICML::LabelConnection");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		LabelConnection elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Member" among "Member"s
//********************************************************************************
std::set<Member> SwitchedAggregateImpl::getMember()
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

} // namespace PICML
