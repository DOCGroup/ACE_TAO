#include "stdafx.h"
#include "OCMLBonX.h"


namespace BON
{

IMPLEMENT_ABSTRACT_BONEXTENSION( OCML_BON::Associated_Option );
IMPLEMENT_ABSTRACT_BONEXTENSION( OCML_BON::Described_Item );
IMPLEMENT_BONEXTENSION( OCML_BON::Option_Category, "Option_Category" );
IMPLEMENT_ABSTRACT_BONEXTENSION( OCML_BON::Option );
IMPLEMENT_ABSTRACT_BONEXTENSION( OCML_BON::Option_Item );
IMPLEMENT_ABSTRACT_BONEXTENSION( OCML_BON::Option_Reference );
IMPLEMENT_BONEXTENSION( OCML_BON::Enum_Option, "Enum_Option" );
IMPLEMENT_BONEXTENSION( OCML_BON::Flag_Option, "Flag_Option" );
IMPLEMENT_ABSTRACT_BONEXTENSION( OCML_BON::Atom_Option_Reference );
IMPLEMENT_ABSTRACT_BONEXTENSION( OCML_BON::Model_Option_Reference );
IMPLEMENT_BONEXTENSION( OCML_BON::Boolean_Option, "Boolean_Option" );
IMPLEMENT_BONEXTENSION( OCML_BON::Enum_Item, "Enum_Item" );
IMPLEMENT_BONEXTENSION( OCML_BON::Flag_Item, "Flag_Item" );
IMPLEMENT_BONEXTENSION( OCML_BON::Integer_Option, "Integer_Option" );
IMPLEMENT_BONEXTENSION( OCML_BON::String_Option, "String_Option" );
IMPLEMENT_BONEXTENSION( OCML_BON::Boolean_Option_Reference, "Boolean_Option_Reference" );
IMPLEMENT_BONEXTENSION( OCML_BON::Enum_Option_Reference, "Enum_Option_Reference" );
IMPLEMENT_BONEXTENSION( OCML_BON::Flag_Option_Reference, "Flag_Option_Reference" );
IMPLEMENT_BONEXTENSION( OCML_BON::Integer_Option_Reference, "Integer_Option_Reference" );
IMPLEMENT_BONEXTENSION( OCML_BON::String_Option_Reference, "String_Option_Reference" );
IMPLEMENT_BONEXTENSION( OCML_BON::Rule, "Rule" );
IMPLEMENT_ABSTRACT_BONEXTENSION( OCML_BON::Association );
IMPLEMENT_ABSTRACT_BONEXTENSION( OCML_BON::Equality_Association );
IMPLEMENT_ABSTRACT_BONEXTENSION( OCML_BON::Numeric_Association );
IMPLEMENT_BONEXTENSION( OCML_BON::Comparison_Association, "Comparison_Association" );
IMPLEMENT_BONEXTENSION( OCML_BON::Logical_Association, "Logical_Association" );
IMPLEMENT_BONEXTENSION( OCML_BON::Select_Association, "Select_Association" );
IMPLEMENT_BONEXTENSION( OCML_BON::Boolean_Equality_Association, "Boolean_Equality_Association" );
IMPLEMENT_BONEXTENSION( OCML_BON::Integer_Equality_Association, "Integer_Equality_Association" );
IMPLEMENT_BONEXTENSION( OCML_BON::Max_Association, "Max_Association" );
IMPLEMENT_BONEXTENSION( OCML_BON::Min_Association, "Min_Association" );
IMPLEMENT_BONEXTENSION( OCML_BON::Range_Association, "Range_Association" );
IMPLEMENT_BONEXTENSION( OCML_BON::String_Equality_Association, "String_Equality_Association" );
IMPLEMENT_ABSTRACT_BONEXTENSION( OCML_BON::Expression_Box );
IMPLEMENT_ABSTRACT_BONEXTENSION( OCML_BON::Logical_Expression );
IMPLEMENT_BONEXTENSION( OCML_BON::Equal, "Equal" );
IMPLEMENT_BONEXTENSION( OCML_BON::AND, "AND" );
IMPLEMENT_BONEXTENSION( OCML_BON::IF, "IF" );
IMPLEMENT_BONEXTENSION( OCML_BON::IFF, "IFF" );
IMPLEMENT_BONEXTENSION( OCML_BON::NOT, "NOT" );
IMPLEMENT_BONEXTENSION( OCML_BON::OR, "OR" );


}; // end namespace BON

namespace OCML_BON
{
//********************************************************************************
// returns dst BON::FCOs
//********************************************************************************
std::multiset<BON::FCO> Associated_OptionImpl::getSelect_AssociationDsts()
{
	std::multiset<BON::FCO> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = getOutConnEnds("Select_Association");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
		{
			BON::FCO dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// returns src and dst BON::FCOs
//********************************************************************************
std::multiset<BON::FCO> Associated_OptionImpl::getSelect_AssociationEnds()
{
	std::multiset<BON::FCO> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = getInConnEnds("Select_Association");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			BON::FCO dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
		std::multiset<BON::ConnectionEnd> out_ends = getOutConnEnds("Select_Association");
		for ( std::multiset<BON::ConnectionEnd>::iterator cot = out_ends.begin() ; cot != out_ends.end() ; ++cot )
		{
			BON::FCO dst( *cot );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}

//********************************************************************************
// 
//********************************************************************************
std::set<Select_Association> Associated_OptionImpl::getSelect_AssociationLinks()
{
	std::set<Select_Association> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		Select_Association c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// returns src BON::FCOs
//********************************************************************************
std::multiset<BON::FCO> Associated_OptionImpl::getSelect_AssociationSrcs()
{
	std::multiset<BON::FCO> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = getInConnEnds("Select_Association");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			BON::FCO dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


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
// returns dst BON::FCOs
//********************************************************************************
std::multiset<BON::FCO> Atom_Option_ReferenceImpl::getComparison_AssociationDsts()
{
	std::multiset<BON::FCO> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = getOutConnEnds("Comparison_Association");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
		{
			BON::FCO dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// returns src and dst BON::FCOs
//********************************************************************************
std::multiset<BON::FCO> Atom_Option_ReferenceImpl::getComparison_AssociationEnds()
{
	std::multiset<BON::FCO> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = getInConnEnds("Comparison_Association");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			BON::FCO dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
		std::multiset<BON::ConnectionEnd> out_ends = getOutConnEnds("Comparison_Association");
		for ( std::multiset<BON::ConnectionEnd>::iterator cot = out_ends.begin() ; cot != out_ends.end() ; ++cot )
		{
			BON::FCO dst( *cot );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}

//********************************************************************************
// 
//********************************************************************************
std::set<Comparison_Association> Atom_Option_ReferenceImpl::getComparison_AssociationLinks()
{
	std::set<Comparison_Association> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		Comparison_Association c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// returns src BON::FCOs
//********************************************************************************
std::multiset<BON::FCO> Atom_Option_ReferenceImpl::getComparison_AssociationSrcs()
{
	std::multiset<BON::FCO> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = getInConnEnds("Comparison_Association");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			BON::FCO dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
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


//********************************************************************************
// returns dst BON::FCOs
//********************************************************************************
std::multiset<BON::FCO> Boolean_Option_ReferenceImpl::getBoolean_Equality_AssociationDsts()
{
	std::multiset<BON::FCO> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = getOutConnEnds("Boolean_Equality_Association");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
		{
			BON::FCO dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// returns src and dst BON::FCOs
//********************************************************************************
std::multiset<BON::FCO> Boolean_Option_ReferenceImpl::getBoolean_Equality_AssociationEnds()
{
	std::multiset<BON::FCO> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = getInConnEnds("Boolean_Equality_Association");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			BON::FCO dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
		std::multiset<BON::ConnectionEnd> out_ends = getOutConnEnds("Boolean_Equality_Association");
		for ( std::multiset<BON::ConnectionEnd>::iterator cot = out_ends.begin() ; cot != out_ends.end() ; ++cot )
		{
			BON::FCO dst( *cot );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}

//********************************************************************************
// 
//********************************************************************************
std::set<Boolean_Equality_Association> Boolean_Option_ReferenceImpl::getBoolean_Equality_AssociationLinks()
{
	std::set<Boolean_Equality_Association> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		Boolean_Equality_Association c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// returns src BON::FCOs
//********************************************************************************
std::multiset<BON::FCO> Boolean_Option_ReferenceImpl::getBoolean_Equality_AssociationSrcs()
{
	std::multiset<BON::FCO> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = getInConnEnds("Boolean_Equality_Association");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			BON::FCO dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// 
//********************************************************************************
Boolean_Option Boolean_Option_ReferenceImpl::getBoolean_Option()
{
	BON::FCO r = getReferred();
	return Boolean_Option(r);
}


//********************************************************************************
// 
//********************************************************************************
Enum_Option Enum_Option_ReferenceImpl::getEnum_Option()
{
	BON::FCO r = getReferred();
	return Enum_Option(r);
}


//********************************************************************************
// 
//********************************************************************************
Flag_Option Flag_Option_ReferenceImpl::getFlag_Option()
{
	BON::FCO r = getReferred();
	return Flag_Option(r);
}


//********************************************************************************
// returns dst BON::FCOs
//********************************************************************************
std::multiset<BON::FCO> Integer_Option_ReferenceImpl::getInteger_Equality_AssociationDsts()
{
	std::multiset<BON::FCO> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = getOutConnEnds("Integer_Equality_Association");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
		{
			BON::FCO dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// returns src and dst BON::FCOs
//********************************************************************************
std::multiset<BON::FCO> Integer_Option_ReferenceImpl::getInteger_Equality_AssociationEnds()
{
	std::multiset<BON::FCO> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = getInConnEnds("Integer_Equality_Association");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			BON::FCO dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
		std::multiset<BON::ConnectionEnd> out_ends = getOutConnEnds("Integer_Equality_Association");
		for ( std::multiset<BON::ConnectionEnd>::iterator cot = out_ends.begin() ; cot != out_ends.end() ; ++cot )
		{
			BON::FCO dst( *cot );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}

//********************************************************************************
// 
//********************************************************************************
std::set<Integer_Equality_Association> Integer_Option_ReferenceImpl::getInteger_Equality_AssociationLinks()
{
	std::set<Integer_Equality_Association> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		Integer_Equality_Association c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// returns src BON::FCOs
//********************************************************************************
std::multiset<BON::FCO> Integer_Option_ReferenceImpl::getInteger_Equality_AssociationSrcs()
{
	std::multiset<BON::FCO> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = getInConnEnds("Integer_Equality_Association");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			BON::FCO dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// returns dst BON::FCOs
//********************************************************************************
std::multiset<BON::FCO> Integer_Option_ReferenceImpl::getMax_AssociationDsts()
{
	std::multiset<BON::FCO> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = getOutConnEnds("Max_Association");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
		{
			BON::FCO dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// returns src and dst BON::FCOs
//********************************************************************************
std::multiset<BON::FCO> Integer_Option_ReferenceImpl::getMax_AssociationEnds()
{
	std::multiset<BON::FCO> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = getInConnEnds("Max_Association");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			BON::FCO dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
		std::multiset<BON::ConnectionEnd> out_ends = getOutConnEnds("Max_Association");
		for ( std::multiset<BON::ConnectionEnd>::iterator cot = out_ends.begin() ; cot != out_ends.end() ; ++cot )
		{
			BON::FCO dst( *cot );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}

//********************************************************************************
// 
//********************************************************************************
std::set<Max_Association> Integer_Option_ReferenceImpl::getMax_AssociationLinks()
{
	std::set<Max_Association> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		Max_Association c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// returns src BON::FCOs
//********************************************************************************
std::multiset<BON::FCO> Integer_Option_ReferenceImpl::getMax_AssociationSrcs()
{
	std::multiset<BON::FCO> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = getInConnEnds("Max_Association");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			BON::FCO dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// returns dst BON::FCOs
//********************************************************************************
std::multiset<BON::FCO> Integer_Option_ReferenceImpl::getMin_AssociationDsts()
{
	std::multiset<BON::FCO> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = getOutConnEnds("Min_Association");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
		{
			BON::FCO dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// returns src and dst BON::FCOs
//********************************************************************************
std::multiset<BON::FCO> Integer_Option_ReferenceImpl::getMin_AssociationEnds()
{
	std::multiset<BON::FCO> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = getInConnEnds("Min_Association");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			BON::FCO dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
		std::multiset<BON::ConnectionEnd> out_ends = getOutConnEnds("Min_Association");
		for ( std::multiset<BON::ConnectionEnd>::iterator cot = out_ends.begin() ; cot != out_ends.end() ; ++cot )
		{
			BON::FCO dst( *cot );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}

//********************************************************************************
// 
//********************************************************************************
std::set<Min_Association> Integer_Option_ReferenceImpl::getMin_AssociationLinks()
{
	std::set<Min_Association> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		Min_Association c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// returns src BON::FCOs
//********************************************************************************
std::multiset<BON::FCO> Integer_Option_ReferenceImpl::getMin_AssociationSrcs()
{
	std::multiset<BON::FCO> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = getInConnEnds("Min_Association");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			BON::FCO dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// returns dst BON::FCOs
//********************************************************************************
std::multiset<BON::FCO> Integer_Option_ReferenceImpl::getRange_AssociationDsts()
{
	std::multiset<BON::FCO> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = getOutConnEnds("Range_Association");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
		{
			BON::FCO dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// returns src and dst BON::FCOs
//********************************************************************************
std::multiset<BON::FCO> Integer_Option_ReferenceImpl::getRange_AssociationEnds()
{
	std::multiset<BON::FCO> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = getInConnEnds("Range_Association");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			BON::FCO dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
		std::multiset<BON::ConnectionEnd> out_ends = getOutConnEnds("Range_Association");
		for ( std::multiset<BON::ConnectionEnd>::iterator cot = out_ends.begin() ; cot != out_ends.end() ; ++cot )
		{
			BON::FCO dst( *cot );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}

//********************************************************************************
// 
//********************************************************************************
std::set<Range_Association> Integer_Option_ReferenceImpl::getRange_AssociationLinks()
{
	std::set<Range_Association> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		Range_Association c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// returns src BON::FCOs
//********************************************************************************
std::multiset<BON::FCO> Integer_Option_ReferenceImpl::getRange_AssociationSrcs()
{
	std::multiset<BON::FCO> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = getInConnEnds("Range_Association");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			BON::FCO dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// 
//********************************************************************************
Integer_Option Integer_Option_ReferenceImpl::getInteger_Option()
{
	BON::FCO r = getReferred();
	return Integer_Option(r);
}


//********************************************************************************
// returns dst BON::FCOs
//********************************************************************************
std::multiset<BON::FCO> String_Option_ReferenceImpl::getString_Equality_AssociationDsts()
{
	std::multiset<BON::FCO> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = getOutConnEnds("String_Equality_Association");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
		{
			BON::FCO dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// returns src and dst BON::FCOs
//********************************************************************************
std::multiset<BON::FCO> String_Option_ReferenceImpl::getString_Equality_AssociationEnds()
{
	std::multiset<BON::FCO> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = getInConnEnds("String_Equality_Association");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			BON::FCO dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
		std::multiset<BON::ConnectionEnd> out_ends = getOutConnEnds("String_Equality_Association");
		for ( std::multiset<BON::ConnectionEnd>::iterator cot = out_ends.begin() ; cot != out_ends.end() ; ++cot )
		{
			BON::FCO dst( *cot );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}

//********************************************************************************
// 
//********************************************************************************
std::set<String_Equality_Association> String_Option_ReferenceImpl::getString_Equality_AssociationLinks()
{
	std::set<String_Equality_Association> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		String_Equality_Association c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// returns src BON::FCOs
//********************************************************************************
std::multiset<BON::FCO> String_Option_ReferenceImpl::getString_Equality_AssociationSrcs()
{
	std::multiset<BON::FCO> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = getInConnEnds("String_Equality_Association");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			BON::FCO dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// 
//********************************************************************************
String_Option String_Option_ReferenceImpl::getString_Option()
{
	BON::FCO r = getReferred();
	return String_Option(r);
}


//********************************************************************************
// getter for role "AND" among "Expression_Box"s and its descendants
//********************************************************************************
std::set<AND> RuleImpl::getAND()
{
	std::set<AND> res;
	std::set<BON::FCO> roles = getChildFCOsAs("AND");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		AND elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// aggregated getter for role "" among "Association"s and its descendants
//********************************************************************************
std::set<Association> RuleImpl::getAssociation()
{
	std::set<Association> res;
	const int len = 9;
	std::set<BON::FCO> roles_vec[ len];
	roles_vec[0] = getChildFCOsAs("Boolean_Equality_Association");
	roles_vec[1] = getChildFCOsAs("Comparison_Association");
	roles_vec[2] = getChildFCOsAs("Integer_Equality_Association");
	roles_vec[3] = getChildFCOsAs("Logical_Association");
	roles_vec[4] = getChildFCOsAs("Max_Association");
	roles_vec[5] = getChildFCOsAs("Min_Association");
	roles_vec[6] = getChildFCOsAs("Range_Association");
	roles_vec[7] = getChildFCOsAs("Select_Association");
	roles_vec[8] = getChildFCOsAs("String_Equality_Association");
	for( int k = 0; k < len; ++k)
		for( std::set<BON::FCO>::iterator i = roles_vec[k].begin(); i != roles_vec[k].end(); ++i)
		{
			Association elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
	return res;
}


//********************************************************************************
// getter for role "Boolean_Equality_Association" among "Association"s and its descendants
//********************************************************************************
std::set<Boolean_Equality_Association> RuleImpl::getBoolean_Equality_Association()
{
	std::set<Boolean_Equality_Association> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Boolean_Equality_Association");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Boolean_Equality_Association elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Boolean_Option_Reference" among "Option_Reference"s and its descendants
//********************************************************************************
std::set<Boolean_Option_Reference> RuleImpl::getBoolean_Option_Reference()
{
	std::set<Boolean_Option_Reference> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Boolean_Option_Reference");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Boolean_Option_Reference elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Comparison_Association" among "Association"s and its descendants
//********************************************************************************
std::set<Comparison_Association> RuleImpl::getComparison_Association()
{
	std::set<Comparison_Association> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Comparison_Association");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Comparison_Association elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Enum_Option_Reference" among "Option_Reference"s and its descendants
//********************************************************************************
std::set<Enum_Option_Reference> RuleImpl::getEnum_Option_Reference()
{
	std::set<Enum_Option_Reference> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Enum_Option_Reference");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Enum_Option_Reference elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Equal" among "Expression_Box"s and its descendants
//********************************************************************************
std::set<Equal> RuleImpl::getEqual()
{
	std::set<Equal> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Equal");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Equal elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// aggregated getter for role "" among "Expression_Box"s and its descendants
//********************************************************************************
std::set<Expression_Box> RuleImpl::getExpression_Box()
{
	std::set<Expression_Box> res;
	const int len = 6;
	std::set<BON::FCO> roles_vec[ len];
	roles_vec[0] = getChildFCOsAs("AND");
	roles_vec[1] = getChildFCOsAs("Equal");
	roles_vec[2] = getChildFCOsAs("IF");
	roles_vec[3] = getChildFCOsAs("IFF");
	roles_vec[4] = getChildFCOsAs("NOT");
	roles_vec[5] = getChildFCOsAs("OR");
	for( int k = 0; k < len; ++k)
		for( std::set<BON::FCO>::iterator i = roles_vec[k].begin(); i != roles_vec[k].end(); ++i)
		{
			Expression_Box elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
	return res;
}


//********************************************************************************
// getter for role "Flag_Option_Reference" among "Option_Reference"s and its descendants
//********************************************************************************
std::set<Flag_Option_Reference> RuleImpl::getFlag_Option_Reference()
{
	std::set<Flag_Option_Reference> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Flag_Option_Reference");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Flag_Option_Reference elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "IF" among "Expression_Box"s and its descendants
//********************************************************************************
std::set<IF> RuleImpl::getIF()
{
	std::set<IF> res;
	std::set<BON::FCO> roles = getChildFCOsAs("IF");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		IF elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "IFF" among "Expression_Box"s and its descendants
//********************************************************************************
std::set<IFF> RuleImpl::getIFF()
{
	std::set<IFF> res;
	std::set<BON::FCO> roles = getChildFCOsAs("IFF");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		IFF elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Integer_Equality_Association" among "Association"s and its descendants
//********************************************************************************
std::set<Integer_Equality_Association> RuleImpl::getInteger_Equality_Association()
{
	std::set<Integer_Equality_Association> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Integer_Equality_Association");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Integer_Equality_Association elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Integer_Option_Reference" among "Option_Reference"s and its descendants
//********************************************************************************
std::set<Integer_Option_Reference> RuleImpl::getInteger_Option_Reference()
{
	std::set<Integer_Option_Reference> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Integer_Option_Reference");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Integer_Option_Reference elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Logical_Association" among "Association"s and its descendants
//********************************************************************************
std::set<Logical_Association> RuleImpl::getLogical_Association()
{
	std::set<Logical_Association> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Logical_Association");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Logical_Association elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Max_Association" among "Association"s and its descendants
//********************************************************************************
std::set<Max_Association> RuleImpl::getMax_Association()
{
	std::set<Max_Association> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Max_Association");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Max_Association elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Min_Association" among "Association"s and its descendants
//********************************************************************************
std::set<Min_Association> RuleImpl::getMin_Association()
{
	std::set<Min_Association> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Min_Association");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Min_Association elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "NOT" among "Expression_Box"s and its descendants
//********************************************************************************
std::set<NOT> RuleImpl::getNOT()
{
	std::set<NOT> res;
	std::set<BON::FCO> roles = getChildFCOsAs("NOT");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		NOT elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "OR" among "Expression_Box"s and its descendants
//********************************************************************************
std::set<OR> RuleImpl::getOR()
{
	std::set<OR> res;
	std::set<BON::FCO> roles = getChildFCOsAs("OR");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		OR elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// aggregated getter for role "" among "Option_Reference"s and its descendants
//********************************************************************************
std::set<Option_Reference> RuleImpl::getOption_Reference()
{
	std::set<Option_Reference> res;
	const int len = 5;
	std::set<BON::FCO> roles_vec[ len];
	roles_vec[0] = getChildFCOsAs("Boolean_Option_Reference");
	roles_vec[1] = getChildFCOsAs("Enum_Option_Reference");
	roles_vec[2] = getChildFCOsAs("Flag_Option_Reference");
	roles_vec[3] = getChildFCOsAs("Integer_Option_Reference");
	roles_vec[4] = getChildFCOsAs("String_Option_Reference");
	for( int k = 0; k < len; ++k)
		for( std::set<BON::FCO>::iterator i = roles_vec[k].begin(); i != roles_vec[k].end(); ++i)
		{
			Option_Reference elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
	return res;
}


//********************************************************************************
// getter for role "Range_Association" among "Association"s and its descendants
//********************************************************************************
std::set<Range_Association> RuleImpl::getRange_Association()
{
	std::set<Range_Association> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Range_Association");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Range_Association elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Select_Association" among "Association"s and its descendants
//********************************************************************************
std::set<Select_Association> RuleImpl::getSelect_Association()
{
	std::set<Select_Association> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Select_Association");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Select_Association elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "String_Equality_Association" among "Association"s and its descendants
//********************************************************************************
std::set<String_Equality_Association> RuleImpl::getString_Equality_Association()
{
	std::set<String_Equality_Association> res;
	std::set<BON::FCO> roles = getChildFCOsAs("String_Equality_Association");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		String_Equality_Association elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "String_Option_Reference" among "Option_Reference"s and its descendants
//********************************************************************************
std::set<String_Option_Reference> RuleImpl::getString_Option_Reference()
{
	std::set<String_Option_Reference> res;
	std::set<BON::FCO> roles = getChildFCOsAs("String_Option_Reference");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		String_Option_Reference elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to BON::FCO
//********************************************************************************
BON::FCO Comparison_AssociationImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	BON::FCO sp( ce);
	if ( sp)
		return sp;

	BON::FCO empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to BON::FCO
//********************************************************************************
BON::FCO Comparison_AssociationImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	BON::FCO sp( ce);
	if ( sp)
		return sp;

	BON::FCO empty;
	return empty;
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to Expression_Box
//********************************************************************************
Expression_Box Logical_AssociationImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	Expression_Box sp( ce);
	if ( sp)
		return sp;

	Expression_Box empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to Expression_Box
//********************************************************************************
Expression_Box Logical_AssociationImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	Expression_Box sp( ce);
	if ( sp)
		return sp;

	Expression_Box empty;
	return empty;
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to BON::FCO
//********************************************************************************
BON::FCO Select_AssociationImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	BON::FCO sp( ce);
	if ( sp)
		return sp;

	BON::FCO empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd
//********************************************************************************
BON::ConnectionEnd Select_AssociationImpl::getSrc()
{
	return ConnectionImpl::getSrc();
}


//********************************************************************************
// 
//********************************************************************************
bool Boolean_Equality_AssociationImpl::isBoolean_Value() 
{
	return FCOImpl::getAttribute("Boolean_Value")->getBooleanValue();
}


//********************************************************************************
// 
//********************************************************************************
void Boolean_Equality_AssociationImpl::setBoolean_Value( bool val)
{
	FCOImpl::getAttribute("Boolean_Value")->setBooleanValue( val);
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to BON::FCO
//********************************************************************************
BON::FCO Boolean_Equality_AssociationImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	BON::FCO sp( ce);
	if ( sp)
		return sp;

	BON::FCO empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to BON::FCO
//********************************************************************************
BON::FCO Boolean_Equality_AssociationImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	BON::FCO sp( ce);
	if ( sp)
		return sp;

	BON::FCO empty;
	return empty;
}


//********************************************************************************
// 
//********************************************************************************
long Integer_Equality_AssociationImpl::getInteger_Value() 
{
	return FCOImpl::getAttribute("Integer_Value")->getIntegerValue();
}


//********************************************************************************
// 
//********************************************************************************
void Integer_Equality_AssociationImpl::setInteger_Value( const long val)
{
	FCOImpl::getAttribute("Integer_Value")->setIntegerValue( val);
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to BON::FCO
//********************************************************************************
BON::FCO Integer_Equality_AssociationImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	BON::FCO sp( ce);
	if ( sp)
		return sp;

	BON::FCO empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to BON::FCO
//********************************************************************************
BON::FCO Integer_Equality_AssociationImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	BON::FCO sp( ce);
	if ( sp)
		return sp;

	BON::FCO empty;
	return empty;
}


//********************************************************************************
// 
//********************************************************************************
long Max_AssociationImpl::getMaximum() 
{
	return FCOImpl::getAttribute("Maximum")->getIntegerValue();
}


//********************************************************************************
// 
//********************************************************************************
void Max_AssociationImpl::setMaximum( const long val)
{
	FCOImpl::getAttribute("Maximum")->setIntegerValue( val);
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to BON::FCO
//********************************************************************************
BON::FCO Max_AssociationImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	BON::FCO sp( ce);
	if ( sp)
		return sp;

	BON::FCO empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to BON::FCO
//********************************************************************************
BON::FCO Max_AssociationImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	BON::FCO sp( ce);
	if ( sp)
		return sp;

	BON::FCO empty;
	return empty;
}


//********************************************************************************
// 
//********************************************************************************
long Min_AssociationImpl::getMinimum() 
{
	return FCOImpl::getAttribute("Minimum")->getIntegerValue();
}


//********************************************************************************
// 
//********************************************************************************
void Min_AssociationImpl::setMinimum( const long val)
{
	FCOImpl::getAttribute("Minimum")->setIntegerValue( val);
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to BON::FCO
//********************************************************************************
BON::FCO Min_AssociationImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	BON::FCO sp( ce);
	if ( sp)
		return sp;

	BON::FCO empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to BON::FCO
//********************************************************************************
BON::FCO Min_AssociationImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	BON::FCO sp( ce);
	if ( sp)
		return sp;

	BON::FCO empty;
	return empty;
}


//********************************************************************************
// 
//********************************************************************************
long Range_AssociationImpl::getMaximum() 
{
	return FCOImpl::getAttribute("Maximum")->getIntegerValue();
}


//********************************************************************************
// 
//********************************************************************************
long Range_AssociationImpl::getMinimum() 
{
	return FCOImpl::getAttribute("Minimum")->getIntegerValue();
}


//********************************************************************************
// 
//********************************************************************************
void Range_AssociationImpl::setMaximum( const long val)
{
	FCOImpl::getAttribute("Maximum")->setIntegerValue( val);
}


//********************************************************************************
// 
//********************************************************************************
void Range_AssociationImpl::setMinimum( const long val)
{
	FCOImpl::getAttribute("Minimum")->setIntegerValue( val);
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to BON::FCO
//********************************************************************************
BON::FCO Range_AssociationImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	BON::FCO sp( ce);
	if ( sp)
		return sp;

	BON::FCO empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to BON::FCO
//********************************************************************************
BON::FCO Range_AssociationImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	BON::FCO sp( ce);
	if ( sp)
		return sp;

	BON::FCO empty;
	return empty;
}


//********************************************************************************
// 
//********************************************************************************
std::string String_Equality_AssociationImpl::getString_Value() 
{
	return FCOImpl::getAttribute("String_Value")->getStringValue();
}


//********************************************************************************
// 
//********************************************************************************
void String_Equality_AssociationImpl::setString_Value( const std::string& val)
{
	FCOImpl::getAttribute("String_Value")->setStringValue( val);
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to BON::FCO
//********************************************************************************
BON::FCO String_Equality_AssociationImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	BON::FCO sp( ce);
	if ( sp)
		return sp;

	BON::FCO empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to BON::FCO
//********************************************************************************
BON::FCO String_Equality_AssociationImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	BON::FCO sp( ce);
	if ( sp)
		return sp;

	BON::FCO empty;
	return empty;
}


//********************************************************************************
// returns dst BON::FCOs
//********************************************************************************
std::multiset<BON::FCO> Expression_BoxImpl::getBoolean_Equality_AssociationDsts()
{
	std::multiset<BON::FCO> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = getOutConnEnds("Boolean_Equality_Association");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
		{
			BON::FCO dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// returns src and dst BON::FCOs
//********************************************************************************
std::multiset<BON::FCO> Expression_BoxImpl::getBoolean_Equality_AssociationEnds()
{
	std::multiset<BON::FCO> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = getInConnEnds("Boolean_Equality_Association");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			BON::FCO dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
		std::multiset<BON::ConnectionEnd> out_ends = getOutConnEnds("Boolean_Equality_Association");
		for ( std::multiset<BON::ConnectionEnd>::iterator cot = out_ends.begin() ; cot != out_ends.end() ; ++cot )
		{
			BON::FCO dst( *cot );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}

//********************************************************************************
// 
//********************************************************************************
std::set<Boolean_Equality_Association> Expression_BoxImpl::getBoolean_Equality_AssociationLinks()
{
	std::set<Boolean_Equality_Association> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		Boolean_Equality_Association c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// returns src BON::FCOs
//********************************************************************************
std::multiset<BON::FCO> Expression_BoxImpl::getBoolean_Equality_AssociationSrcs()
{
	std::multiset<BON::FCO> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = getInConnEnds("Boolean_Equality_Association");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			BON::FCO dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// returns dst BON::FCOs
//********************************************************************************
std::multiset<BON::FCO> Expression_BoxImpl::getInteger_Equality_AssociationDsts()
{
	std::multiset<BON::FCO> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = getOutConnEnds("Integer_Equality_Association");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
		{
			BON::FCO dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// returns src and dst BON::FCOs
//********************************************************************************
std::multiset<BON::FCO> Expression_BoxImpl::getInteger_Equality_AssociationEnds()
{
	std::multiset<BON::FCO> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = getInConnEnds("Integer_Equality_Association");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			BON::FCO dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
		std::multiset<BON::ConnectionEnd> out_ends = getOutConnEnds("Integer_Equality_Association");
		for ( std::multiset<BON::ConnectionEnd>::iterator cot = out_ends.begin() ; cot != out_ends.end() ; ++cot )
		{
			BON::FCO dst( *cot );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}

//********************************************************************************
// 
//********************************************************************************
std::set<Integer_Equality_Association> Expression_BoxImpl::getInteger_Equality_AssociationLinks()
{
	std::set<Integer_Equality_Association> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		Integer_Equality_Association c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// returns src BON::FCOs
//********************************************************************************
std::multiset<BON::FCO> Expression_BoxImpl::getInteger_Equality_AssociationSrcs()
{
	std::multiset<BON::FCO> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = getInConnEnds("Integer_Equality_Association");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			BON::FCO dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// returns dst Expression_Boxs
//********************************************************************************
std::multiset<Expression_Box> Expression_BoxImpl::getLogical_AssociationDsts()
{
	std::multiset<Expression_Box> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = getOutConnEnds("Logical_Association");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
		{
			Expression_Box dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// returns src and dst Expression_Boxs
//********************************************************************************
std::multiset<Expression_Box> Expression_BoxImpl::getLogical_AssociationEnds()
{
	std::multiset<Expression_Box> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = getInConnEnds("Logical_Association");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			Expression_Box dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
		std::multiset<BON::ConnectionEnd> out_ends = getOutConnEnds("Logical_Association");
		for ( std::multiset<BON::ConnectionEnd>::iterator cot = out_ends.begin() ; cot != out_ends.end() ; ++cot )
		{
			Expression_Box dst( *cot );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}

//********************************************************************************
// 
//********************************************************************************
std::set<Logical_Association> Expression_BoxImpl::getLogical_AssociationLinks()
{
	std::set<Logical_Association> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		Logical_Association c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// returns src Expression_Boxs
//********************************************************************************
std::multiset<Expression_Box> Expression_BoxImpl::getLogical_AssociationSrcs()
{
	std::multiset<Expression_Box> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = getInConnEnds("Logical_Association");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			Expression_Box dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// returns dst BON::FCOs
//********************************************************************************
std::multiset<BON::FCO> Expression_BoxImpl::getMax_AssociationDsts()
{
	std::multiset<BON::FCO> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = getOutConnEnds("Max_Association");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
		{
			BON::FCO dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// returns src and dst BON::FCOs
//********************************************************************************
std::multiset<BON::FCO> Expression_BoxImpl::getMax_AssociationEnds()
{
	std::multiset<BON::FCO> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = getInConnEnds("Max_Association");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			BON::FCO dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
		std::multiset<BON::ConnectionEnd> out_ends = getOutConnEnds("Max_Association");
		for ( std::multiset<BON::ConnectionEnd>::iterator cot = out_ends.begin() ; cot != out_ends.end() ; ++cot )
		{
			BON::FCO dst( *cot );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}

//********************************************************************************
// 
//********************************************************************************
std::set<Max_Association> Expression_BoxImpl::getMax_AssociationLinks()
{
	std::set<Max_Association> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		Max_Association c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// returns src BON::FCOs
//********************************************************************************
std::multiset<BON::FCO> Expression_BoxImpl::getMax_AssociationSrcs()
{
	std::multiset<BON::FCO> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = getInConnEnds("Max_Association");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			BON::FCO dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// returns dst BON::FCOs
//********************************************************************************
std::multiset<BON::FCO> Expression_BoxImpl::getMin_AssociationDsts()
{
	std::multiset<BON::FCO> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = getOutConnEnds("Min_Association");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
		{
			BON::FCO dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// returns src and dst BON::FCOs
//********************************************************************************
std::multiset<BON::FCO> Expression_BoxImpl::getMin_AssociationEnds()
{
	std::multiset<BON::FCO> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = getInConnEnds("Min_Association");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			BON::FCO dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
		std::multiset<BON::ConnectionEnd> out_ends = getOutConnEnds("Min_Association");
		for ( std::multiset<BON::ConnectionEnd>::iterator cot = out_ends.begin() ; cot != out_ends.end() ; ++cot )
		{
			BON::FCO dst( *cot );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}

//********************************************************************************
// 
//********************************************************************************
std::set<Min_Association> Expression_BoxImpl::getMin_AssociationLinks()
{
	std::set<Min_Association> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		Min_Association c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// returns src BON::FCOs
//********************************************************************************
std::multiset<BON::FCO> Expression_BoxImpl::getMin_AssociationSrcs()
{
	std::multiset<BON::FCO> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = getInConnEnds("Min_Association");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			BON::FCO dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// returns dst BON::FCOs
//********************************************************************************
std::multiset<BON::FCO> Expression_BoxImpl::getRange_AssociationDsts()
{
	std::multiset<BON::FCO> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = getOutConnEnds("Range_Association");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
		{
			BON::FCO dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// returns src and dst BON::FCOs
//********************************************************************************
std::multiset<BON::FCO> Expression_BoxImpl::getRange_AssociationEnds()
{
	std::multiset<BON::FCO> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = getInConnEnds("Range_Association");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			BON::FCO dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
		std::multiset<BON::ConnectionEnd> out_ends = getOutConnEnds("Range_Association");
		for ( std::multiset<BON::ConnectionEnd>::iterator cot = out_ends.begin() ; cot != out_ends.end() ; ++cot )
		{
			BON::FCO dst( *cot );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}

//********************************************************************************
// 
//********************************************************************************
std::set<Range_Association> Expression_BoxImpl::getRange_AssociationLinks()
{
	std::set<Range_Association> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		Range_Association c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// returns src BON::FCOs
//********************************************************************************
std::multiset<BON::FCO> Expression_BoxImpl::getRange_AssociationSrcs()
{
	std::multiset<BON::FCO> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = getInConnEnds("Range_Association");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			BON::FCO dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// returns dst BON::FCOs and referenceports
//********************************************************************************
std::multiset<BON::ConnectionEnd> Expression_BoxImpl::getSelect_AssociationDsts()
{
	return getOutConnEnds("Select_Association");
}


//********************************************************************************
// returns src and dst BON::FCOs and referenceports
//********************************************************************************
std::multiset<BON::ConnectionEnd> Expression_BoxImpl::getSelect_AssociationEnds()
{
	return getConnEnds("Select_Association");
}


//********************************************************************************
// 
//********************************************************************************
std::set<Select_Association> Expression_BoxImpl::getSelect_AssociationLinks()
{
	std::set<Select_Association> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		Select_Association c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// returns src BON::FCOs and referenceports
//********************************************************************************
std::multiset<BON::ConnectionEnd> Expression_BoxImpl::getSelect_AssociationSrcs()
{
	return getInConnEnds("Select_Association");
}


//********************************************************************************
// returns dst BON::FCOs
//********************************************************************************
std::multiset<BON::FCO> Expression_BoxImpl::getString_Equality_AssociationDsts()
{
	std::multiset<BON::FCO> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = getOutConnEnds("String_Equality_Association");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
		{
			BON::FCO dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// returns src and dst BON::FCOs
//********************************************************************************
std::multiset<BON::FCO> Expression_BoxImpl::getString_Equality_AssociationEnds()
{
	std::multiset<BON::FCO> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = getInConnEnds("String_Equality_Association");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			BON::FCO dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
		std::multiset<BON::ConnectionEnd> out_ends = getOutConnEnds("String_Equality_Association");
		for ( std::multiset<BON::ConnectionEnd>::iterator cot = out_ends.begin() ; cot != out_ends.end() ; ++cot )
		{
			BON::FCO dst( *cot );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}

//********************************************************************************
// 
//********************************************************************************
std::set<String_Equality_Association> Expression_BoxImpl::getString_Equality_AssociationLinks()
{
	std::set<String_Equality_Association> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		String_Equality_Association c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// returns src BON::FCOs
//********************************************************************************
std::multiset<BON::FCO> Expression_BoxImpl::getString_Equality_AssociationSrcs()
{
	std::multiset<BON::FCO> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = getInConnEnds("String_Equality_Association");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			BON::FCO dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// returns dst Expression_Boxs
//********************************************************************************
std::multiset<Expression_Box> Logical_ExpressionImpl::getLogical_AssociationDsts()
{
	std::multiset<Expression_Box> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = getOutConnEnds("Logical_Association");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
		{
			Expression_Box dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// returns src and dst Expression_Boxs
//********************************************************************************
std::multiset<Expression_Box> Logical_ExpressionImpl::getLogical_AssociationEnds()
{
	std::multiset<Expression_Box> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = getInConnEnds("Logical_Association");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			Expression_Box dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
		std::multiset<BON::ConnectionEnd> out_ends = getOutConnEnds("Logical_Association");
		for ( std::multiset<BON::ConnectionEnd>::iterator cot = out_ends.begin() ; cot != out_ends.end() ; ++cot )
		{
			Expression_Box dst( *cot );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}

//********************************************************************************
// 
//********************************************************************************
std::set<Logical_Association> Logical_ExpressionImpl::getLogical_AssociationLinks()
{
	std::set<Logical_Association> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		Logical_Association c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// returns src Expression_Boxs
//********************************************************************************
std::multiset<Expression_Box> Logical_ExpressionImpl::getLogical_AssociationSrcs()
{
	std::multiset<Expression_Box> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = getInConnEnds("Logical_Association");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			Expression_Box dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// returns dst BON::FCOs
//********************************************************************************
std::multiset<BON::FCO> EqualImpl::getComparison_AssociationDsts()
{
	std::multiset<BON::FCO> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = getOutConnEnds("Comparison_Association");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
		{
			BON::FCO dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// returns src and dst BON::FCOs
//********************************************************************************
std::multiset<BON::FCO> EqualImpl::getComparison_AssociationEnds()
{
	std::multiset<BON::FCO> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = getInConnEnds("Comparison_Association");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			BON::FCO dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
		std::multiset<BON::ConnectionEnd> out_ends = getOutConnEnds("Comparison_Association");
		for ( std::multiset<BON::ConnectionEnd>::iterator cot = out_ends.begin() ; cot != out_ends.end() ; ++cot )
		{
			BON::FCO dst( *cot );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}

//********************************************************************************
// 
//********************************************************************************
std::set<Comparison_Association> EqualImpl::getComparison_AssociationLinks()
{
	std::set<Comparison_Association> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		Comparison_Association c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// returns src BON::FCOs
//********************************************************************************
std::multiset<BON::FCO> EqualImpl::getComparison_AssociationSrcs()
{
	std::multiset<BON::FCO> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = getInConnEnds("Comparison_Association");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			BON::FCO dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}



}; // namespace OCML_BON

