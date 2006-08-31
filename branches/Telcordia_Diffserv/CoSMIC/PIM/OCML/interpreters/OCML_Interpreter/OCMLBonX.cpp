#include "stdafx.h"
#include "OCMLBonX.h"


namespace BON
{

IMPLEMENT_ABSTRACT_BONEXTENSION( OCML_BON::Described_Item );
IMPLEMENT_BONEXTENSION( OCML_BON::Option_Category, "Option_Category" );
IMPLEMENT_ABSTRACT_BONEXTENSION( OCML_BON::Option );
IMPLEMENT_ABSTRACT_BONEXTENSION( OCML_BON::Option_Item );
IMPLEMENT_BONEXTENSION( OCML_BON::Enum_Option, "Enum_Option" );
IMPLEMENT_BONEXTENSION( OCML_BON::Flag_Option, "Flag_Option" );
IMPLEMENT_BONEXTENSION( OCML_BON::Boolean_Option, "Boolean_Option" );
IMPLEMENT_BONEXTENSION( OCML_BON::Enum_Item, "Enum_Item" );
IMPLEMENT_BONEXTENSION( OCML_BON::Flag_Item, "Flag_Item" );
IMPLEMENT_BONEXTENSION( OCML_BON::Integer_Option, "Integer_Option" );
IMPLEMENT_BONEXTENSION( OCML_BON::String_Option, "String_Option" );


}; // end namespace BON

namespace OCML_BON
{
//********************************************************************************
// 
//********************************************************************************
std::string Described_ItemImpl::getDescription() 
{
	return FCOImpl::getAttribute("Description")->getStringValue();
}


//********************************************************************************
// 
//********************************************************************************
std::string Described_ItemImpl::getTitle() 
{
	return FCOImpl::getAttribute("Title")->getStringValue();
}


//********************************************************************************
// 
//********************************************************************************
void Described_ItemImpl::setDescription( const std::string& val)
{
	FCOImpl::getAttribute("Description")->setStringValue( val);
}


//********************************************************************************
// 
//********************************************************************************
void Described_ItemImpl::setTitle( const std::string& val)
{
	FCOImpl::getAttribute("Title")->setStringValue( val);
}


//********************************************************************************
// getter for role "Boolean_Option" among "Option"s and its descendants
//********************************************************************************
std::set<Boolean_Option> Option_CategoryImpl::getBoolean_Option()
{
	std::set<Boolean_Option> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Boolean_Option");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Boolean_Option elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Enum_Option" among "Option"s and its descendants
//********************************************************************************
std::set<Enum_Option> Option_CategoryImpl::getEnum_Option()
{
	std::set<Enum_Option> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Enum_Option");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Enum_Option elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Flag_Option" among "Option"s and its descendants
//********************************************************************************
std::set<Flag_Option> Option_CategoryImpl::getFlag_Option()
{
	std::set<Flag_Option> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Flag_Option");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Flag_Option elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Integer_Option" among "Option"s and its descendants
//********************************************************************************
std::set<Integer_Option> Option_CategoryImpl::getInteger_Option()
{
	std::set<Integer_Option> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Integer_Option");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Integer_Option elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// aggregated getter for role "" among "Option"s and its descendants
//********************************************************************************
std::set<Option> Option_CategoryImpl::getOption()
{
	std::set<Option> res;
	const int len = 5;
	std::set<BON::FCO> roles_vec[ len];
	roles_vec[0] = getChildFCOsAs("Boolean_Option");
	roles_vec[1] = getChildFCOsAs("Integer_Option");
	roles_vec[2] = getChildFCOsAs("String_Option");
	roles_vec[3] = getChildFCOsAs("Enum_Option");
	roles_vec[4] = getChildFCOsAs("Flag_Option");
	for( int k = 0; k < len; ++k)
		for( std::set<BON::FCO>::iterator i = roles_vec[k].begin(); i != roles_vec[k].end(); ++i)
		{
			Option elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
	return res;
}


//********************************************************************************
// getter for role "Option_Category" among "Option_Category"s
//********************************************************************************
std::set<Option_Category> Option_CategoryImpl::getOption_Category()
{
	std::set<Option_Category> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Option_Category");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Option_Category elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "String_Option" among "Option"s and its descendants
//********************************************************************************
std::set<String_Option> Option_CategoryImpl::getString_Option()
{
	std::set<String_Option> res;
	std::set<BON::FCO> roles = getChildFCOsAs("String_Option");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		String_Option elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// 
//********************************************************************************
std::string OptionImpl::getParameter_Name() 
{
	return FCOImpl::getAttribute("Parameter_Name")->getStringValue();
}


//********************************************************************************
// 
//********************************************************************************
void OptionImpl::setParameter_Name( const std::string& val)
{
	FCOImpl::getAttribute("Parameter_Name")->setStringValue( val);
}


//********************************************************************************
// 
//********************************************************************************
bool Option_ItemImpl::isItem_Default_Value() 
{
	return FCOImpl::getAttribute("Item_Default_Value")->getBooleanValue();
}


//********************************************************************************
// 
//********************************************************************************
void Option_ItemImpl::setItem_Default_Value( bool val)
{
	FCOImpl::getAttribute("Item_Default_Value")->setBooleanValue( val);
}


//********************************************************************************
// getter for role "Enum_Item" among "Enum_Item"s
//********************************************************************************
std::set<Enum_Item> Enum_OptionImpl::getEnum_Item()
{
	std::set<Enum_Item> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Enum_Item");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Enum_Item elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Flag_Item" among "Flag_Item"s
//********************************************************************************
std::set<Flag_Item> Flag_OptionImpl::getFlag_Item()
{
	std::set<Flag_Item> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Flag_Item");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Flag_Item elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// 
//********************************************************************************
bool Boolean_OptionImpl::isBoolean_Default_Value() 
{
	return FCOImpl::getAttribute("Boolean_Default_Value")->getBooleanValue();
}


//********************************************************************************
// 
//********************************************************************************
void Boolean_OptionImpl::setBoolean_Default_Value( bool val)
{
	FCOImpl::getAttribute("Boolean_Default_Value")->setBooleanValue( val);
}


//********************************************************************************
// 
//********************************************************************************
std::string Integer_OptionImpl::getInteger_Default_Value() 
{
	return FCOImpl::getAttribute("Integer_Default_Value")->getStringValue();
}


//********************************************************************************
// 
//********************************************************************************
void Integer_OptionImpl::setInteger_Default_Value( const std::string& val)
{
	FCOImpl::getAttribute("Integer_Default_Value")->setStringValue( val);
}


//********************************************************************************
// 
//********************************************************************************
std::string String_OptionImpl::getString_Default_Value() 
{
	return FCOImpl::getAttribute("String_Default_Value")->getStringValue();
}


//********************************************************************************
// 
//********************************************************************************
void String_OptionImpl::setString_Default_Value( const std::string& val)
{
	FCOImpl::getAttribute("String_Default_Value")->setStringValue( val);
}



}; // namespace OCML_BON

