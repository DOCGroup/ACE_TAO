#include "stdafx.h"
#include "IDMLBonExtension.h"
#include <algorithm>
#include <string>

using std::string;


namespace BON
{
string strNamespacePref ("PICML");
IMPLEMENT_ABSTRACT_BONEXTENSION( IDML::Orderable );
IMPLEMENT_BONEXTENSION( IDML::InterfaceDefinitions, "InterfaceDefinitions" );
IMPLEMENT_BONEXTENSION( IDML::PredefinedTypes, "PredefinedTypes" );
IMPLEMENT_ABSTRACT_BONEXTENSION( IDML::Prefixable );
IMPLEMENT_ABSTRACT_BONEXTENSION( IDML::ConstantType );
IMPLEMENT_ABSTRACT_BONEXTENSION( IDML::Manageable );
IMPLEMENT_ABSTRACT_BONEXTENSION( IDML::MemberType );
IMPLEMENT_ABSTRACT_BONEXTENSION( IDML::Provideable );
IMPLEMENT_ABSTRACT_BONEXTENSION( IDML::Taggable );
IMPLEMENT_BONEXTENSION( IDML::Exception, "Exception" );
IMPLEMENT_BONEXTENSION( IDML::File, "File" );
IMPLEMENT_ABSTRACT_BONEXTENSION( IDML::OperationBase );
IMPLEMENT_BONEXTENSION( IDML::Package, "Package" );
IMPLEMENT_BONEXTENSION( IDML::ReadonlyAttribute, "ReadonlyAttribute" );
IMPLEMENT_ABSTRACT_BONEXTENSION( IDML::NamedType );
IMPLEMENT_ABSTRACT_BONEXTENSION( IDML::Port );
IMPLEMENT_ABSTRACT_BONEXTENSION( IDML::PredefinedType );
IMPLEMENT_BONEXTENSION( IDML::ComponentRef, "ComponentRef" );
IMPLEMENT_BONEXTENSION( IDML::Constant, "Constant" );
IMPLEMENT_BONEXTENSION( IDML::Attribute, "Attribute" );
IMPLEMENT_ABSTRACT_BONEXTENSION( IDML::HasExceptions );
IMPLEMENT_ABSTRACT_BONEXTENSION( IDML::Inheritable );
IMPLEMENT_BONEXTENSION( IDML::OnewayOperation, "OnewayOperation" );
IMPLEMENT_ABSTRACT_BONEXTENSION( IDML::NoInheritable );
IMPLEMENT_BONEXTENSION( IDML::Boolean, "Boolean" );
IMPLEMENT_BONEXTENSION( IDML::Byte, "Byte" );
IMPLEMENT_BONEXTENSION( IDML::GenericObject, "GenericObject" );
IMPLEMENT_BONEXTENSION( IDML::GenericValue, "GenericValue" );
IMPLEMENT_BONEXTENSION( IDML::GenericValueObject, "GenericValueObject" );
IMPLEMENT_BONEXTENSION( IDML::LongInteger, "LongInteger" );
IMPLEMENT_BONEXTENSION( IDML::RealNumber, "RealNumber" );
IMPLEMENT_BONEXTENSION( IDML::ShortInteger, "ShortInteger" );
IMPLEMENT_BONEXTENSION( IDML::String, "String" );
IMPLEMENT_BONEXTENSION( IDML::TypeEncoding, "TypeEncoding" );
IMPLEMENT_BONEXTENSION( IDML::TypeKind, "TypeKind" );
IMPLEMENT_BONEXTENSION( IDML::Boxed, "Boxed" );
IMPLEMENT_BONEXTENSION( IDML::InEventPort, "InEventPort" );
IMPLEMENT_BONEXTENSION( IDML::OutEventPort, "OutEventPort" );
IMPLEMENT_BONEXTENSION( IDML::ProvidedRequestPort, "ProvidedRequestPort" );
IMPLEMENT_BONEXTENSION( IDML::RequiredRequestPort, "RequiredRequestPort" );
IMPLEMENT_BONEXTENSION( IDML::Aggregate, "Aggregate" );
IMPLEMENT_BONEXTENSION( IDML::Enum, "Enum" );
IMPLEMENT_BONEXTENSION( IDML::FactoryOperation, "FactoryOperation" );
IMPLEMENT_ABSTRACT_BONEXTENSION( IDML::HasOperations );
IMPLEMENT_BONEXTENSION( IDML::LookupOperation, "LookupOperation" );
IMPLEMENT_ABSTRACT_BONEXTENSION( IDML::SupportsInterfaces );
IMPLEMENT_BONEXTENSION( IDML::SwitchedAggregate, "SwitchedAggregate" );
IMPLEMENT_BONEXTENSION( IDML::TwowayOperation, "TwowayOperation" );
IMPLEMENT_BONEXTENSION( IDML::Alias, "Alias" );
IMPLEMENT_BONEXTENSION( IDML::Collection, "Collection" );
IMPLEMENT_BONEXTENSION( IDML::Component, "Component" );
IMPLEMENT_BONEXTENSION( IDML::ComponentFactory, "ComponentFactory" );
IMPLEMENT_BONEXTENSION( IDML::Object, "Object" );
IMPLEMENT_ABSTRACT_BONEXTENSION( IDML::ObjectByValue );
IMPLEMENT_BONEXTENSION( IDML::Event, "Event" );
IMPLEMENT_BONEXTENSION( IDML::ValueObject, "ValueObject" );
IMPLEMENT_BONEXTENSION( IDML::EnumValue, "EnumValue" );
IMPLEMENT_BONEXTENSION( IDML::Label, "Label" );
IMPLEMENT_BONEXTENSION( IDML::PrivateFlag, "PrivateFlag" );
IMPLEMENT_BONEXTENSION( IDML::LabelConnection, "LabelConnection" );
IMPLEMENT_BONEXTENSION( IDML::MakeMemberPrivate, "MakeMemberPrivate" );
IMPLEMENT_BONEXTENSION( IDML::ManagesComponent, "ManagesComponent" );
IMPLEMENT_BONEXTENSION( IDML::AttributeMember, "AttributeMember" );
IMPLEMENT_BONEXTENSION( IDML::Discriminator, "Discriminator" );
IMPLEMENT_BONEXTENSION( IDML::ExceptionRef, "ExceptionRef" );
IMPLEMENT_BONEXTENSION( IDML::FileRef, "FileRef" );
IMPLEMENT_BONEXTENSION( IDML::GetException, "GetException" );
IMPLEMENT_BONEXTENSION( IDML::InParameter, "InParameter" );
IMPLEMENT_BONEXTENSION( IDML::Inherits, "Inherits" );
IMPLEMENT_BONEXTENSION( IDML::InoutParameter, "InoutParameter" );
IMPLEMENT_BONEXTENSION( IDML::LookupKey, "LookupKey" );
IMPLEMENT_BONEXTENSION( IDML::Member, "Member" );
IMPLEMENT_BONEXTENSION( IDML::OutParameter, "OutParameter" );
IMPLEMENT_BONEXTENSION( IDML::ReturnType, "ReturnType" );
IMPLEMENT_BONEXTENSION( IDML::SetException, "SetException" );
IMPLEMENT_BONEXTENSION( IDML::Supports, "Supports" );


}; // end namespace BON

namespace IDML
{
//*******************************************************************
//
//*******************************************************************
void OrderableImpl::initialize ()
{
  fwd_declared_ = false;
}

//*******************************************************************
// // getter for kind "File"
//*******************************************************************
std::set<File> InterfaceDefinitionsImpl::getFile()
{
	std::set<File> res;
	std::set<BON::Object> kinds = getChildObjects("File");
	for( std::set<BON::Object>::iterator i = kinds.begin(); i != kinds.end(); ++i)
	{
		File elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//*******************************************************************
// // getter for kind "Boolean"
//*******************************************************************
std::set<Boolean> PredefinedTypesImpl::getBoolean()
{
	std::set<Boolean> res;
	std::set<BON::Object> kinds = getChildObjects("Boolean");
	for( std::set<BON::Object>::iterator i = kinds.begin(); i != kinds.end(); ++i)
	{
		Boolean elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//*******************************************************************
// // getter for kind "Byte"
//*******************************************************************
std::set<Byte> PredefinedTypesImpl::getByte()
{
	std::set<Byte> res;
	std::set<BON::Object> kinds = getChildObjects("Byte");
	for( std::set<BON::Object>::iterator i = kinds.begin(); i != kinds.end(); ++i)
	{
		Byte elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//*******************************************************************
// // getter for kind "GenericObject"
//*******************************************************************
std::set<GenericObject> PredefinedTypesImpl::getGenericObject()
{
	std::set<GenericObject> res;
	std::set<BON::Object> kinds = getChildObjects("GenericObject");
	for( std::set<BON::Object>::iterator i = kinds.begin(); i != kinds.end(); ++i)
	{
		GenericObject elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//*******************************************************************
// // getter for kind "GenericValue"
//*******************************************************************
std::set<GenericValue> PredefinedTypesImpl::getGenericValue()
{
	std::set<GenericValue> res;
	std::set<BON::Object> kinds = getChildObjects("GenericValue");
	for( std::set<BON::Object>::iterator i = kinds.begin(); i != kinds.end(); ++i)
	{
		GenericValue elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//*******************************************************************
// // getter for kind "GenericValueObject"
//*******************************************************************
std::set<GenericValueObject> PredefinedTypesImpl::getGenericValueObject()
{
	std::set<GenericValueObject> res;
	std::set<BON::Object> kinds = getChildObjects("GenericValueObject");
	for( std::set<BON::Object>::iterator i = kinds.begin(); i != kinds.end(); ++i)
	{
		GenericValueObject elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//*******************************************************************
// // getter for kind "LongInteger"
//*******************************************************************
std::set<LongInteger> PredefinedTypesImpl::getLongInteger()
{
	std::set<LongInteger> res;
	std::set<BON::Object> kinds = getChildObjects("LongInteger");
	for( std::set<BON::Object>::iterator i = kinds.begin(); i != kinds.end(); ++i)
	{
		LongInteger elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//*******************************************************************
// // aggregated getter for kind "PredefinedType" and its descendants
//*******************************************************************
std::set<PredefinedType> PredefinedTypesImpl::getPredefinedType()
{
	std::set<PredefinedType> res;
	const int len = 11;
	std::set<BON::Object> kinds_vec[ len];
	kinds_vec[0] = getChildObjects("Boolean");
	kinds_vec[1] = getChildObjects("Byte");
	kinds_vec[2] = getChildObjects("GenericObject");
	kinds_vec[3] = getChildObjects("GenericValue");
	kinds_vec[4] = getChildObjects("GenericValueObject");
	kinds_vec[5] = getChildObjects("LongInteger");
	kinds_vec[6] = getChildObjects("RealNumber");
	kinds_vec[7] = getChildObjects("ShortInteger");
	kinds_vec[8] = getChildObjects("String");
	kinds_vec[9] = getChildObjects("TypeEncoding");
	kinds_vec[10] = getChildObjects("TypeKind");
	for( int k = 0; k < len; ++k)
		for( std::set<BON::Object>::iterator i = kinds_vec[k].begin(); i != kinds_vec[k].end(); ++i)
		{
			PredefinedType elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
	return res;
}


//*******************************************************************
// // getter for kind "RealNumber"
//*******************************************************************
std::set<RealNumber> PredefinedTypesImpl::getRealNumber()
{
	std::set<RealNumber> res;
	std::set<BON::Object> kinds = getChildObjects("RealNumber");
	for( std::set<BON::Object>::iterator i = kinds.begin(); i != kinds.end(); ++i)
	{
		RealNumber elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//*******************************************************************
// // getter for kind "ShortInteger"
//*******************************************************************
std::set<ShortInteger> PredefinedTypesImpl::getShortInteger()
{
	std::set<ShortInteger> res;
	std::set<BON::Object> kinds = getChildObjects("ShortInteger");
	for( std::set<BON::Object>::iterator i = kinds.begin(); i != kinds.end(); ++i)
	{
		ShortInteger elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//*******************************************************************
// // getter for kind "String"
//*******************************************************************
std::set<String> PredefinedTypesImpl::getString()
{
	std::set<String> res;
	std::set<BON::Object> kinds = getChildObjects("String");
	for( std::set<BON::Object>::iterator i = kinds.begin(); i != kinds.end(); ++i)
	{
		String elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//*******************************************************************
// // getter for kind "TypeEncoding"
//*******************************************************************
std::set<TypeEncoding> PredefinedTypesImpl::getTypeEncoding()
{
	std::set<TypeEncoding> res;
	std::set<BON::Object> kinds = getChildObjects("TypeEncoding");
	for( std::set<BON::Object>::iterator i = kinds.begin(); i != kinds.end(); ++i)
	{
		TypeEncoding elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//*******************************************************************
// // getter for kind "TypeKind"
//*******************************************************************
std::set<TypeKind> PredefinedTypesImpl::getTypeKind()
{
	std::set<TypeKind> res;
	std::set<BON::Object> kinds = getChildObjects("TypeKind");
	for( std::set<BON::Object>::iterator i = kinds.begin(); i != kinds.end(); ++i)
	{
		TypeKind elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//*******************************************************************
//
//*******************************************************************
std::string PrefixableImpl::getPrefixTag()
{
	return FCOImpl::getAttribute("PrefixTag")->getStringValue();
}


//*******************************************************************
//
//*******************************************************************
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


//*******************************************************************
// returns src ComponentFactorys
//*******************************************************************
std::multiset<ComponentFactory> ManageableImpl::getManagesComponentSrcs()
{
	std::multiset<ComponentFactory> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = getInConnEnds("ManagesComponent");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			ComponentFactory dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//*******************************************************************
//
//*******************************************************************
std::string TaggableImpl::getSpecifyIdTag()
{
	return FCOImpl::getAttribute("SpecifyIdTag")->getStringValue();
}


//*******************************************************************
//
//*******************************************************************
std::string TaggableImpl::getVersionTag()
{
	return FCOImpl::getAttribute("VersionTag")->getStringValue();
}


//*******************************************************************
// getter for role "Member" among "Member"s
//*******************************************************************
std::set<Member> ExceptionImpl::getMember()
{
	std::set<Member> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Member");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Member elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//*******************************************************************
//
//*******************************************************************
bool FileImpl::include_components_idl () const
{
  return this->include_components_idl_;
}

//*******************************************************************
//
//*******************************************************************
void FileImpl::include_components_idl (bool val)
{
  this->include_components_idl_ = val;
}

//*******************************************************************
//
//*******************************************************************
bool FileImpl::include_orb_idl () const
{
  return this->include_orb_idl_;
}

//*******************************************************************
//
//*******************************************************************
void FileImpl::include_orb_idl (bool val)
{
  this->include_orb_idl_ = val;
}

//*******************************************************************
//
//*******************************************************************
void FileImpl::initialize ()
{
  this->include_components_idl_ = false;
  this->include_orb_idl_ = false;
  this->OrderableImpl::initialize ();
}

//*******************************************************************
// getter for role "Aggregate" among "NamedType"s and its descendants
//*******************************************************************
std::set<Aggregate> FileImpl::getAggregate()
{
	std::set<Aggregate> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Aggregate");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Aggregate elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//*******************************************************************
// getter for role "Alias" among "NamedType"s and its descendants
//*******************************************************************
std::set<Alias> FileImpl::getAlias()
{
	std::set<Alias> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Alias");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Alias elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//*******************************************************************
// getter for role "Boxed" among "NamedType"s and its descendants
//*******************************************************************
std::set<Boxed> FileImpl::getBoxed()
{
	std::set<Boxed> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Boxed");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Boxed elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//*******************************************************************
// getter for role "Collection" among "NamedType"s and its descendants
//*******************************************************************
std::set<Collection> FileImpl::getCollection()
{
	std::set<Collection> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Collection");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Collection elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//*******************************************************************
// getter for role "Component" among "NamedType"s and its descendants
//*******************************************************************
std::set<Component> FileImpl::getComponent()
{
	std::set<Component> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Component");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Component elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//*******************************************************************
// getter for role "ComponentFactory" among "NamedType"s and its descendants
//*******************************************************************
std::set<ComponentFactory> FileImpl::getComponentFactory()
{
	std::set<ComponentFactory> res;
	std::set<BON::FCO> roles = getChildFCOsAs("ComponentFactory");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		ComponentFactory elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//*******************************************************************
// getter for role "ComponentRef" among "ComponentRef"s
//*******************************************************************
std::set<ComponentRef> FileImpl::getComponentRef()
{
	std::set<ComponentRef> res;
	std::set<BON::FCO> roles = getChildFCOsAs("ComponentRef");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		ComponentRef elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//*******************************************************************
// getter for role "Constant" among "Constant"s
//*******************************************************************
std::set<Constant> FileImpl::getConstant()
{
	std::set<Constant> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Constant");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Constant elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//*******************************************************************
// getter for role "Enum" among "NamedType"s and its descendants
//*******************************************************************
std::set<Enum> FileImpl::getEnum()
{
	std::set<Enum> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Enum");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Enum elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//*******************************************************************
// getter for role "Event" among "NamedType"s and its descendants
//*******************************************************************
std::set<Event> FileImpl::getEvent()
{
	std::set<Event> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Event");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Event elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//*******************************************************************
// getter for role "Exception" among "Exception"s
//*******************************************************************
std::set<Exception> FileImpl::getException()
{
	std::set<Exception> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Exception");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Exception elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//*******************************************************************
// getter for role "FileRef" among "FileRef"s
//*******************************************************************
std::set<FileRef> FileImpl::getFileRef()
{
	std::set<FileRef> res;
	std::set<BON::FCO> roles = getChildFCOsAs("FileRef");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		FileRef elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//*******************************************************************
// getter for role "ManagesComponent" among "ManagesComponent"s
//*******************************************************************
std::set<ManagesComponent> FileImpl::getManagesComponent()
{
	std::set<ManagesComponent> res;
	std::set<BON::FCO> roles = getChildFCOsAs("ManagesComponent");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		ManagesComponent elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//*******************************************************************
// aggregated getter for role "" among "NamedType"s and its descendants
//*******************************************************************
std::set<NamedType> FileImpl::getNamedType()
{
	std::set<NamedType> res;
	const int len = 11;
	std::set<BON::FCO> roles_vec[ len];
	roles_vec[0] = getChildFCOsAs("Aggregate");
	roles_vec[1] = getChildFCOsAs("Component");
	roles_vec[2] = getChildFCOsAs("ComponentFactory");
	roles_vec[3] = getChildFCOsAs("Enum");
	roles_vec[4] = getChildFCOsAs("Event");
	roles_vec[5] = getChildFCOsAs("Object");
	roles_vec[6] = getChildFCOsAs("SwitchedAggregate");
	roles_vec[7] = getChildFCOsAs("ValueObject");
	roles_vec[8] = getChildFCOsAs("Alias");
	roles_vec[9] = getChildFCOsAs("Boxed");
	roles_vec[10] = getChildFCOsAs("Collection");
	for( int k = 0; k < len; ++k)
		for( std::set<BON::FCO>::iterator i = roles_vec[k].begin(); i != roles_vec[k].end(); ++i)
		{
			NamedType elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
	return res;
}


//*******************************************************************
// getter for role "Object" among "NamedType"s and its descendants
//*******************************************************************
std::set<Object> FileImpl::getObject()
{
	std::set<Object> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Object");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Object elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//*******************************************************************
// getter for role "Package" among "Package"s
//*******************************************************************
std::set<Package> FileImpl::getPackage()
{
	std::set<Package> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Package");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Package elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//*******************************************************************
// getter for role "SwitchedAggregate" among "NamedType"s and its descendants
//*******************************************************************
std::set<SwitchedAggregate> FileImpl::getSwitchedAggregate()
{
	std::set<SwitchedAggregate> res;
	std::set<BON::FCO> roles = getChildFCOsAs("SwitchedAggregate");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		SwitchedAggregate elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//*******************************************************************
// getter for role "ValueObject" among "NamedType"s and its descendants
//*******************************************************************
std::set<ValueObject> FileImpl::getValueObject()
{
	std::set<ValueObject> res;
	std::set<BON::FCO> roles = getChildFCOsAs("ValueObject");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		ValueObject elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//*******************************************************************
// getter for role "InParameter" among "InParameter"s
//*******************************************************************
std::set<InParameter> OperationBaseImpl::getInParameter()
{
	std::set<InParameter> res;
	std::set<BON::FCO> roles = getChildFCOsAs("InParameter");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		InParameter elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//*******************************************************************
// getter for role "Aggregate" among "NamedType"s and its descendants
//*******************************************************************
std::set<Aggregate> PackageImpl::getAggregate()
{
	std::set<Aggregate> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Aggregate");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Aggregate elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//*******************************************************************
// getter for role "Alias" among "NamedType"s and its descendants
//*******************************************************************
std::set<Alias> PackageImpl::getAlias()
{
	std::set<Alias> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Alias");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Alias elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//*******************************************************************
// getter for role "Boxed" among "NamedType"s and its descendants
//*******************************************************************
std::set<Boxed> PackageImpl::getBoxed()
{
	std::set<Boxed> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Boxed");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Boxed elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//*******************************************************************
// getter for role "Collection" among "NamedType"s and its descendants
//*******************************************************************
std::set<Collection> PackageImpl::getCollection()
{
	std::set<Collection> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Collection");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Collection elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//*******************************************************************
// getter for role "Component" among "NamedType"s and its descendants
//*******************************************************************
std::set<Component> PackageImpl::getComponent()
{
	std::set<Component> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Component");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Component elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//*******************************************************************
// getter for role "ComponentFactory" among "NamedType"s and its descendants
//*******************************************************************
std::set<ComponentFactory> PackageImpl::getComponentFactory()
{
	std::set<ComponentFactory> res;
	std::set<BON::FCO> roles = getChildFCOsAs("ComponentFactory");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		ComponentFactory elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//*******************************************************************
// getter for role "ComponentRef" among "ComponentRef"s
//*******************************************************************
std::set<ComponentRef> PackageImpl::getComponentRef()
{
	std::set<ComponentRef> res;
	std::set<BON::FCO> roles = getChildFCOsAs("ComponentRef");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		ComponentRef elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//*******************************************************************
// getter for role "Constant" among "Constant"s
//*******************************************************************
std::set<Constant> PackageImpl::getConstant()
{
	std::set<Constant> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Constant");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Constant elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//*******************************************************************
// getter for role "Enum" among "NamedType"s and its descendants
//*******************************************************************
std::set<Enum> PackageImpl::getEnum()
{
	std::set<Enum> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Enum");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Enum elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//*******************************************************************
// getter for role "Event" among "NamedType"s and its descendants
//*******************************************************************
std::set<Event> PackageImpl::getEvent()
{
	std::set<Event> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Event");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Event elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//*******************************************************************
// getter for role "Exception" among "Exception"s
//*******************************************************************
std::set<Exception> PackageImpl::getException()
{
	std::set<Exception> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Exception");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Exception elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//*******************************************************************
// getter for role "ManagesComponent" among "ManagesComponent"s
//*******************************************************************
std::set<ManagesComponent> PackageImpl::getManagesComponent()
{
	std::set<ManagesComponent> res;
	std::set<BON::FCO> roles = getChildFCOsAs("ManagesComponent");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		ManagesComponent elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//*******************************************************************
// aggregated getter for role "" among "NamedType"s and its descendants
//*******************************************************************
std::set<NamedType> PackageImpl::getNamedType()
{
	std::set<NamedType> res;
	const int len = 11;
	std::set<BON::FCO> roles_vec[ len];
	roles_vec[0] = getChildFCOsAs("Aggregate");
	roles_vec[1] = getChildFCOsAs("Component");
	roles_vec[2] = getChildFCOsAs("ComponentFactory");
	roles_vec[3] = getChildFCOsAs("Enum");
	roles_vec[4] = getChildFCOsAs("Event");
	roles_vec[5] = getChildFCOsAs("Object");
	roles_vec[6] = getChildFCOsAs("SwitchedAggregate");
	roles_vec[7] = getChildFCOsAs("ValueObject");
	roles_vec[8] = getChildFCOsAs("Alias");
	roles_vec[9] = getChildFCOsAs("Boxed");
	roles_vec[10] = getChildFCOsAs("Collection");
	for( int k = 0; k < len; ++k)
		for( std::set<BON::FCO>::iterator i = roles_vec[k].begin(); i != roles_vec[k].end(); ++i)
		{
			NamedType elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
	return res;
}


//*******************************************************************
// getter for role "Object" among "NamedType"s and its descendants
//*******************************************************************
std::set<Object> PackageImpl::getObject()
{
	std::set<Object> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Object");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Object elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//*******************************************************************
// getter for role "Package" among "Package"s
//*******************************************************************
std::set<Package> PackageImpl::getPackage()
{
	std::set<Package> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Package");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Package elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//*******************************************************************
// getter for role "SwitchedAggregate" among "NamedType"s and its descendants
//*******************************************************************
std::set<SwitchedAggregate> PackageImpl::getSwitchedAggregate()
{
	std::set<SwitchedAggregate> res;
	std::set<BON::FCO> roles = getChildFCOsAs("SwitchedAggregate");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		SwitchedAggregate elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//*******************************************************************
// getter for role "ValueObject" among "NamedType"s and its descendants
//*******************************************************************
std::set<ValueObject> PackageImpl::getValueObject()
{
	std::set<ValueObject> res;
	std::set<BON::FCO> roles = getChildFCOsAs("ValueObject");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		ValueObject elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//*******************************************************************
// getter for role "AttributeMember" among "AttributeMember"s
//*******************************************************************
std::set<AttributeMember> ReadonlyAttributeImpl::getAttributeMember()
{
	std::set<AttributeMember> res;
	std::set<BON::FCO> roles = getChildFCOsAs("AttributeMember");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		AttributeMember elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//*******************************************************************
// getter for role "GetException" among "GetException"s
//*******************************************************************
std::set<GetException> ReadonlyAttributeImpl::getGetException()
{
	std::set<GetException> res;
	std::set<BON::FCO> roles = getChildFCOsAs("GetException");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		GetException elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//*******************************************************************
//
//*******************************************************************
Component ComponentRefImpl::getComponent()
{
	BON::FCO r = getReferred();
	return Component(r);
}


//*******************************************************************
//
//*******************************************************************
std::string ConstantImpl::getvalue()
{
	return FCOImpl::getAttribute("value")->getStringValue();
}


//*******************************************************************
//
//*******************************************************************
ConstantType ConstantImpl::getConstantType()
{
	BON::FCO r = getReferred();
	return ConstantType(r);
}


//*******************************************************************
// getter for role "SetException" among "SetException"s
//*******************************************************************
std::set<SetException> AttributeImpl::getSetException()
{
	std::set<SetException> res;
	std::set<BON::FCO> roles = getChildFCOsAs("SetException");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		SetException elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//*******************************************************************
// getter for role "ExceptionRef" among "ExceptionRef"s
//*******************************************************************
std::set<ExceptionRef> HasExceptionsImpl::getExceptionRef()
{
	std::set<ExceptionRef> res;
	std::set<BON::FCO> roles = getChildFCOsAs("ExceptionRef");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		ExceptionRef elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//*******************************************************************
// getter for role "Attribute" among "ReadonlyAttribute"s and its descendants
//*******************************************************************
std::set<Attribute> InheritableImpl::getAttribute()
{
	std::set<Attribute> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Attribute");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Attribute elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//*******************************************************************
// getter for role "Inherits" among "Inherits"s
//*******************************************************************
std::set<Inherits> InheritableImpl::getInherits()
{
	std::set<Inherits> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Inherits");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Inherits elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//*******************************************************************
// getter for role "ReadonlyAttribute" among "ReadonlyAttribute"s and its descendants
//*******************************************************************
std::set<ReadonlyAttribute> InheritableImpl::getReadonlyAttribute()
{
	std::set<ReadonlyAttribute> res;
	std::set<BON::FCO> roles = getChildFCOsAs("ReadonlyAttribute");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		ReadonlyAttribute elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//*******************************************************************
// aggregated getter for role "" among "ReadonlyAttribute"s and its descendants
//*******************************************************************
std::set<ReadonlyAttribute> InheritableImpl::getReadonlyAttribute(int dummy)
{
	std::set<ReadonlyAttribute> res;
	const int len = 2;
	std::set<BON::FCO> roles_vec[ len];
	roles_vec[0] = getChildFCOsAs("Attribute");
	roles_vec[1] = getChildFCOsAs("ReadonlyAttribute");
	for( int k = 0; k < len; ++k)
		for( std::set<BON::FCO>::iterator i = roles_vec[k].begin(); i != roles_vec[k].end(); ++i)
		{
			ReadonlyAttribute elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
	return res;
}


//*******************************************************************
//
//*******************************************************************
MemberType BoxedImpl::getMemberType()
{
	BON::FCO r = getReferred();
	return MemberType(r);
}


//*******************************************************************
//
//*******************************************************************
Event InEventPortImpl::getEvent()
{
	BON::FCO r = getReferred();
	return Event(r);
}


//*******************************************************************
//
//*******************************************************************
bool OutEventPortImpl::issingle_destination()
{
	return FCOImpl::getAttribute("single_destination")->getBooleanValue();
}


//*******************************************************************
//
//*******************************************************************
Event OutEventPortImpl::getEvent()
{
	BON::FCO r = getReferred();
	return Event(r);
}


//*******************************************************************
//
//*******************************************************************
Provideable ProvidedRequestPortImpl::getProvideable()
{
	BON::FCO r = getReferred();
	return Provideable(r);
}


//*******************************************************************
//
//*******************************************************************
bool RequiredRequestPortImpl::ismultiple_connections()
{
	return FCOImpl::getAttribute("multiple_connections")->getBooleanValue();
}


//*******************************************************************
//
//*******************************************************************
Provideable RequiredRequestPortImpl::getProvideable()
{
	BON::FCO r = getReferred();
	return Provideable(r);
}


//*******************************************************************
// getter for role "Member" among "Member"s
//*******************************************************************
std::set<Member> AggregateImpl::getMember()
{
	std::set<Member> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Member");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Member elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//*******************************************************************
// getter for role "EnumValue" among "EnumValue"s
//*******************************************************************
std::set<EnumValue> EnumImpl::getEnumValue()
{
	std::set<EnumValue> res;
	std::set<BON::FCO> roles = getChildFCOsAs("EnumValue");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		EnumValue elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//*******************************************************************
// getter for role "Aggregate" among "NoInheritable"s and its descendants
//*******************************************************************
std::set<Aggregate> HasOperationsImpl::getAggregate()
{
	std::set<Aggregate> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Aggregate");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Aggregate elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//*******************************************************************
// getter for role "Alias" among "NoInheritable"s and its descendants
//*******************************************************************
std::set<Alias> HasOperationsImpl::getAlias()
{
	std::set<Alias> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Alias");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Alias elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//*******************************************************************
// getter for role "Collection" among "NoInheritable"s and its descendants
//*******************************************************************
std::set<Collection> HasOperationsImpl::getCollection()
{
	std::set<Collection> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Collection");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Collection elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//*******************************************************************
// getter for role "Constant" among "Constant"s
//*******************************************************************
std::set<Constant> HasOperationsImpl::getConstant()
{
	std::set<Constant> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Constant");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Constant elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//*******************************************************************
// getter for role "Enum" among "NoInheritable"s and its descendants
//*******************************************************************
std::set<Enum> HasOperationsImpl::getEnum()
{
	std::set<Enum> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Enum");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Enum elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//*******************************************************************
// getter for role "Exception" among "Exception"s
//*******************************************************************
std::set<Exception> HasOperationsImpl::getException()
{
	std::set<Exception> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Exception");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Exception elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//*******************************************************************
// aggregated getter for role "" among "NoInheritable"s and its descendants
//*******************************************************************
std::set<NoInheritable> HasOperationsImpl::getNoInheritable()
{
	std::set<NoInheritable> res;
	const int len = 5;
	std::set<BON::FCO> roles_vec[ len];
	roles_vec[0] = getChildFCOsAs("Aggregate");
	roles_vec[1] = getChildFCOsAs("Enum");
	roles_vec[2] = getChildFCOsAs("SwitchedAggregate");
	roles_vec[3] = getChildFCOsAs("Alias");
	roles_vec[4] = getChildFCOsAs("Collection");
	for( int k = 0; k < len; ++k)
		for( std::set<BON::FCO>::iterator i = roles_vec[k].begin(); i != roles_vec[k].end(); ++i)
		{
			NoInheritable elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
	return res;
}


//*******************************************************************
// getter for role "OnewayOperation" among "OnewayOperation"s
//*******************************************************************
std::set<OnewayOperation> HasOperationsImpl::getOnewayOperation()
{
	std::set<OnewayOperation> res;
	std::set<BON::FCO> roles = getChildFCOsAs("OnewayOperation");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		OnewayOperation elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//*******************************************************************
// getter for role "SwitchedAggregate" among "NoInheritable"s and its descendants
//*******************************************************************
std::set<SwitchedAggregate> HasOperationsImpl::getSwitchedAggregate()
{
	std::set<SwitchedAggregate> res;
	std::set<BON::FCO> roles = getChildFCOsAs("SwitchedAggregate");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		SwitchedAggregate elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//*******************************************************************
// getter for role "TwowayOperation" among "TwowayOperation"s
//*******************************************************************
std::set<TwowayOperation> HasOperationsImpl::getTwowayOperation()
{
	std::set<TwowayOperation> res;
	std::set<BON::FCO> roles = getChildFCOsAs("TwowayOperation");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		TwowayOperation elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//*******************************************************************
// getter for role "Supports" among "Supports"s
//*******************************************************************
std::set<Supports> SupportsInterfacesImpl::getSupports()
{
	std::set<Supports> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Supports");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Supports elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//*******************************************************************
// getter for role "Discriminator" among "Discriminator"s
//*******************************************************************
std::set<Discriminator> SwitchedAggregateImpl::getDiscriminator()
{
	std::set<Discriminator> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Discriminator");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Discriminator elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//*******************************************************************
// getter for role "Label" among "Label"s
//*******************************************************************
std::set<Label> SwitchedAggregateImpl::getLabel()
{
	std::set<Label> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Label");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Label elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//*******************************************************************
// getter for role "LabelConnection" among "LabelConnection"s
//*******************************************************************
std::set<LabelConnection> SwitchedAggregateImpl::getLabelConnection()
{
	std::set<LabelConnection> res;
	std::set<BON::FCO> roles = getChildFCOsAs("LabelConnection");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		LabelConnection elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//*******************************************************************
// getter for role "Member" among "Member"s
//*******************************************************************
std::set<Member> SwitchedAggregateImpl::getMember()
{
	std::set<Member> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Member");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Member elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//*******************************************************************
// getter for role "InoutParameter" among "InoutParameter"s
//*******************************************************************
std::set<InoutParameter> TwowayOperationImpl::getInoutParameter()
{
	std::set<InoutParameter> res;
	std::set<BON::FCO> roles = getChildFCOsAs("InoutParameter");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		InoutParameter elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//*******************************************************************
// getter for role "OutParameter" among "OutParameter"s
//*******************************************************************
std::set<OutParameter> TwowayOperationImpl::getOutParameter()
{
	std::set<OutParameter> res;
	std::set<BON::FCO> roles = getChildFCOsAs("OutParameter");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		OutParameter elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//*******************************************************************
// getter for role "ReturnType" among "ReturnType"s
//*******************************************************************
std::set<ReturnType> TwowayOperationImpl::getReturnType()
{
	std::set<ReturnType> res;
	std::set<BON::FCO> roles = getChildFCOsAs("ReturnType");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		ReturnType elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//*******************************************************************
//
//*******************************************************************
MemberType AliasImpl::getMemberType()
{
	BON::FCO r = getReferred();
	return MemberType(r);
}


//*******************************************************************
//
//*******************************************************************
MemberType CollectionImpl::getMemberType()
{
	BON::FCO r = getReferred();
	return MemberType(r);
}


//*******************************************************************
// getter for role "InEventPort" among "Port"s and its descendants
//*******************************************************************
std::set<InEventPort> ComponentImpl::getInEventPort()
{
	std::set<InEventPort> res;
	std::set<BON::FCO> roles = getChildFCOsAs("InEventPort");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		InEventPort elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//*******************************************************************
// getter for role "OutEventPort" among "Port"s and its descendants
//*******************************************************************
std::set<OutEventPort> ComponentImpl::getOutEventPort()
{
	std::set<OutEventPort> res;
	std::set<BON::FCO> roles = getChildFCOsAs("OutEventPort");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		OutEventPort elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//*******************************************************************
// aggregated getter for role "" among "Port"s and its descendants
//*******************************************************************
std::set<Port> ComponentImpl::getPort()
{
	std::set<Port> res;
	const int len = 4;
	std::set<BON::FCO> roles_vec[ len];
	roles_vec[0] = getChildFCOsAs("InEventPort");
	roles_vec[1] = getChildFCOsAs("OutEventPort");
	roles_vec[2] = getChildFCOsAs("ProvidedRequestPort");
	roles_vec[3] = getChildFCOsAs("RequiredRequestPort");
	for( int k = 0; k < len; ++k)
		for( std::set<BON::FCO>::iterator i = roles_vec[k].begin(); i != roles_vec[k].end(); ++i)
		{
			Port elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
	return res;
}


//*******************************************************************
// getter for role "ProvidedRequestPort" among "Port"s and its descendants
//*******************************************************************
std::set<ProvidedRequestPort> ComponentImpl::getProvidedRequestPort()
{
	std::set<ProvidedRequestPort> res;
	std::set<BON::FCO> roles = getChildFCOsAs("ProvidedRequestPort");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		ProvidedRequestPort elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//*******************************************************************
// getter for role "RequiredRequestPort" among "Port"s and its descendants
//*******************************************************************
std::set<RequiredRequestPort> ComponentImpl::getRequiredRequestPort()
{
	std::set<RequiredRequestPort> res;
	std::set<BON::FCO> roles = getChildFCOsAs("RequiredRequestPort");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		RequiredRequestPort elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}

//*******************************************************************
//
//*******************************************************************
bool ComponentImpl::ref_managed () const
{
  return this->ref_managed_;
}

//*******************************************************************
//
//*******************************************************************
void ComponentImpl::ref_managed (bool val)
{
  this->ref_managed_ = val;
}

//*******************************************************************
//
//*******************************************************************
Orderable ComponentImpl::base_component () const
{
  return this->base_component_;
}

//*******************************************************************
//
//*******************************************************************
void ComponentImpl::base_component (const Orderable &base)
{
  this->base_component_ = base;
}

//*******************************************************************
//
//*******************************************************************
void ComponentImpl::initialize ()
{
  this->ref_managed_ = false;
  this->OrderableImpl::initialize ();
}

//*******************************************************************
// returns dst Manageables
//*******************************************************************
std::multiset<Manageable> ComponentFactoryImpl::getManagesComponentDsts()
{
	std::multiset<Manageable> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = getOutConnEnds("ManagesComponent");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
		{
			Manageable dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//*******************************************************************
//
//*******************************************************************
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


//*******************************************************************
// getter for role "FactoryOperation" among "FactoryOperation"s
//*******************************************************************
std::set<FactoryOperation> ComponentFactoryImpl::getFactoryOperation()
{
	std::set<FactoryOperation> res;
	std::set<BON::FCO> roles = getChildFCOsAs("FactoryOperation");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		FactoryOperation elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//*******************************************************************
// getter for role "LookupKey" among "LookupKey"s
//*******************************************************************
std::set<LookupKey> ComponentFactoryImpl::getLookupKey()
{
	std::set<LookupKey> res;
	std::set<BON::FCO> roles = getChildFCOsAs("LookupKey");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		LookupKey elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//*******************************************************************
// getter for role "LookupOperation" among "LookupOperation"s
//*******************************************************************
std::set<LookupOperation> ComponentFactoryImpl::getLookupOperation()
{
	std::set<LookupOperation> res;
	std::set<BON::FCO> roles = getChildFCOsAs("LookupOperation");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		LookupOperation elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//*******************************************************************
//
//*******************************************************************
bool ObjectImpl::isabstract()
{
	return FCOImpl::getAttribute("abstract")->getBooleanValue();
}


//*******************************************************************
//
//*******************************************************************
bool ObjectImpl::islocal()
{
	return FCOImpl::getAttribute("local")->getBooleanValue();
}


//*******************************************************************
//
//*******************************************************************
bool ObjectByValueImpl::isabstract()
{
	return FCOImpl::getAttribute("abstract")->getBooleanValue();
}


//*******************************************************************
// getter for role "FactoryOperation" among "FactoryOperation"s
//*******************************************************************
std::set<FactoryOperation> ObjectByValueImpl::getFactoryOperation()
{
	std::set<FactoryOperation> res;
	std::set<BON::FCO> roles = getChildFCOsAs("FactoryOperation");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		FactoryOperation elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//*******************************************************************
// getter for role "MakeMemberPrivate" among "MakeMemberPrivate"s
//*******************************************************************
std::set<MakeMemberPrivate> ObjectByValueImpl::getMakeMemberPrivate()
{
	std::set<MakeMemberPrivate> res;
	std::set<BON::FCO> roles = getChildFCOsAs("MakeMemberPrivate");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		MakeMemberPrivate elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//*******************************************************************
// getter for role "Member" among "Member"s
//*******************************************************************
std::set<Member> ObjectByValueImpl::getMember()
{
	std::set<Member> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Member");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Member elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//*******************************************************************
// getter for role "PrivateFlag" among "PrivateFlag"s
//*******************************************************************
std::set<PrivateFlag> ObjectByValueImpl::getPrivateFlag()
{
	std::set<PrivateFlag> res;
	std::set<BON::FCO> roles = getChildFCOsAs("PrivateFlag");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		PrivateFlag elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//*******************************************************************
//
//*******************************************************************
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


//*******************************************************************
// returns src Members
//*******************************************************************
std::multiset<Member> LabelImpl::getLabelConnectionSrcs()
{
	std::multiset<Member> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = getInConnEnds("LabelConnection");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			Member dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//*******************************************************************
//
//*******************************************************************
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


//*******************************************************************
// returns src Members
//*******************************************************************
std::multiset<Member> PrivateFlagImpl::getMakeMemberPrivateSrcs()
{
	std::multiset<Member> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = getInConnEnds("MakeMemberPrivate");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			Member dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//*******************************************************************
// getDst() return value is a ConnectionEnd casted to Label
//*******************************************************************
Label LabelConnectionImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	Label sp( ce);
	if ( sp)
		return sp;

	Label empty;
	return empty;
}


//*******************************************************************
// getSrc() return value is a ConnectionEnd casted to Member
//*******************************************************************
Member LabelConnectionImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	Member sp( ce);
	if ( sp)
		return sp;

	Member empty;
	return empty;
}


//*******************************************************************
// getDst() return value is a ConnectionEnd casted to PrivateFlag
//*******************************************************************
PrivateFlag MakeMemberPrivateImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	PrivateFlag sp( ce);
	if ( sp)
		return sp;

	PrivateFlag empty;
	return empty;
}


//*******************************************************************
// getSrc() return value is a ConnectionEnd casted to Member
//*******************************************************************
Member MakeMemberPrivateImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	Member sp( ce);
	if ( sp)
		return sp;

	Member empty;
	return empty;
}


//*******************************************************************
// getDst() return value is a ConnectionEnd casted to Manageable
//*******************************************************************
Manageable ManagesComponentImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	Manageable sp( ce);
	if ( sp)
		return sp;

	Manageable empty;
	return empty;
}


//*******************************************************************
// getSrc() return value is a ConnectionEnd casted to ComponentFactory
//*******************************************************************
ComponentFactory ManagesComponentImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	ComponentFactory sp( ce);
	if ( sp)
		return sp;

	ComponentFactory empty;
	return empty;
}


//*******************************************************************
//
//*******************************************************************
MemberType AttributeMemberImpl::getMemberType()
{
	BON::FCO r = getReferred();
	return MemberType(r);
}


//*******************************************************************
//
//*******************************************************************
ConstantType DiscriminatorImpl::getConstantType()
{
	BON::FCO r = getReferred();
	return ConstantType(r);
}


//*******************************************************************
//
//*******************************************************************
Exception ExceptionRefImpl::getException()
{
	BON::FCO r = getReferred();
	return Exception(r);
}


//*******************************************************************
//
//*******************************************************************
File FileRefImpl::getFile()
{
	BON::FCO r = getReferred();
	return File(r);
}


//*******************************************************************
//
//*******************************************************************
Exception GetExceptionImpl::getException()
{
	BON::FCO r = getReferred();
	return Exception(r);
}


//*******************************************************************
//
//*******************************************************************
MemberType InParameterImpl::getMemberType()
{
	BON::FCO r = getReferred();
	return MemberType(r);
}


//*******************************************************************
//
//*******************************************************************
Inheritable InheritsImpl::getInheritable()
{
	BON::FCO r = getReferred();
	return Inheritable(r);
}


//*******************************************************************
//
//*******************************************************************
MemberType InoutParameterImpl::getMemberType()
{
	BON::FCO r = getReferred();
	return MemberType(r);
}


//*******************************************************************
//
//*******************************************************************
ValueObject LookupKeyImpl::getValueObject()
{
	BON::FCO r = getReferred();
	return ValueObject(r);
}


//*******************************************************************
// returns dst Labels
//*******************************************************************
std::multiset<Label> MemberImpl::getLabelConnectionDsts()
{
	std::multiset<Label> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = getOutConnEnds("LabelConnection");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
		{
			Label dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//*******************************************************************
// returns dst PrivateFlags
//*******************************************************************
std::multiset<PrivateFlag> MemberImpl::getMakeMemberPrivateDsts()
{
	std::multiset<PrivateFlag> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = getOutConnEnds("MakeMemberPrivate");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
		{
			PrivateFlag dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//*******************************************************************
//
//*******************************************************************
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


//*******************************************************************
//
//*******************************************************************
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


//*******************************************************************
//
//*******************************************************************
MemberType MemberImpl::getMemberType()
{
	BON::FCO r = getReferred();
	return MemberType(r);
}


//*******************************************************************
//
//*******************************************************************
MemberType OutParameterImpl::getMemberType()
{
	BON::FCO r = getReferred();
	return MemberType(r);
}


//*******************************************************************
//
//*******************************************************************
MemberType ReturnTypeImpl::getMemberType()
{
	BON::FCO r = getReferred();
	return MemberType(r);
}


//*******************************************************************
//
//*******************************************************************
Exception SetExceptionImpl::getException()
{
	BON::FCO r = getReferred();
	return Exception(r);
}


//*******************************************************************
//
//*******************************************************************
Object SupportsImpl::getObject()
{
	BON::FCO r = getReferred();
	return Object(r);
}



}; // namespace IDML
