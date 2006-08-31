#include "stdafx.h"
#include "OCMLVisitor.h"


namespace OCML_BON
{

bool OCMLVisitor::visitAND( const AND& object )
{
	if ( !object)
		return false;

	return true;
}


bool OCMLVisitor::visitBoolean_Option( const Boolean_Option& object )
{
	if ( !object)
		return false;

	return true;
}


bool OCMLVisitor::visitEnum_Item( const Enum_Item& object )
{
	if ( !object)
		return false;

	return true;
}


bool OCMLVisitor::visitEqual( const Equal& object )
{
	if ( !object)
		return false;

	return true;
}


bool OCMLVisitor::visitFlag_Item( const Flag_Item& object )
{
	if ( !object)
		return false;

	return true;
}


bool OCMLVisitor::visitIF( const IF& object )
{
	if ( !object)
		return false;

	return true;
}


bool OCMLVisitor::visitIFF( const IFF& object )
{
	if ( !object)
		return false;

	return true;
}


bool OCMLVisitor::visitInteger_Option( const Integer_Option& object )
{
	if ( !object)
		return false;

	return true;
}


bool OCMLVisitor::visitNOT( const NOT& object )
{
	if ( !object)
		return false;

	return true;
}


bool OCMLVisitor::visitOR( const OR& object )
{
	if ( !object)
		return false;

	return true;
}


bool OCMLVisitor::visitString_Option( const String_Option& object )
{
	if ( !object)
		return false;

	return true;
}


bool OCMLVisitor::visitEnum_Option( const Enum_Option& object )
{
	if ( !object)
		return false;

	return true;
}


bool OCMLVisitor::visitFlag_Option( const Flag_Option& object )
{
	if ( !object)
		return false;

	return true;
}


bool OCMLVisitor::visitOption_Category( const Option_Category& object )
{
	if ( !object)
		return false;

	return true;
}


bool OCMLVisitor::visitRule( const Rule& object )
{
	if ( !object)
		return false;

	return true;
}


bool OCMLVisitor::visitBoolean_Equality_Association( const Boolean_Equality_Association& object )
{
	if ( !object)
		return false;

	return true;
}


bool OCMLVisitor::visitComparison_Association( const Comparison_Association& object )
{
	if ( !object)
		return false;

	return true;
}


bool OCMLVisitor::visitInteger_Equality_Association( const Integer_Equality_Association& object )
{
	if ( !object)
		return false;

	return true;
}


bool OCMLVisitor::visitLogical_Association( const Logical_Association& object )
{
	if ( !object)
		return false;

	return true;
}


bool OCMLVisitor::visitMax_Association( const Max_Association& object )
{
	if ( !object)
		return false;

	return true;
}


bool OCMLVisitor::visitMin_Association( const Min_Association& object )
{
	if ( !object)
		return false;

	return true;
}


bool OCMLVisitor::visitRange_Association( const Range_Association& object )
{
	if ( !object)
		return false;

	return true;
}


bool OCMLVisitor::visitSelect_Association( const Select_Association& object )
{
	if ( !object)
		return false;

	return true;
}


bool OCMLVisitor::visitString_Equality_Association( const String_Equality_Association& object )
{
	if ( !object)
		return false;

	return true;
}


bool OCMLVisitor::visitBoolean_Option_Reference( const Boolean_Option_Reference& object )
{
	if ( !object)
		return false;

	return true;
}


bool OCMLVisitor::visitEnum_Option_Reference( const Enum_Option_Reference& object )
{
	if ( !object)
		return false;

	return true;
}


bool OCMLVisitor::visitFlag_Option_Reference( const Flag_Option_Reference& object )
{
	if ( !object)
		return false;

	return true;
}


bool OCMLVisitor::visitInteger_Option_Reference( const Integer_Option_Reference& object )
{
	if ( !object)
		return false;

	return true;
}


bool OCMLVisitor::visitString_Option_Reference( const String_Option_Reference& object )
{
	if ( !object)
		return false;

	return true;
}


bool OCMLVisitor::visitAssociated_Option( const Associated_Option& object )
{
	if ( !object)
		return false;

	return true;
}


bool OCMLVisitor::visitAssociation( const Association& object )
{
	if ( !object)
		return false;

	return true;
}


bool OCMLVisitor::visitAtom_Option_Reference( const Atom_Option_Reference& object )
{
	if ( !object)
		return false;

	return true;
}


bool OCMLVisitor::visitDescribed_Item( const Described_Item& object )
{
	if ( !object)
		return false;

	return true;
}


bool OCMLVisitor::visitEquality_Association( const Equality_Association& object )
{
	if ( !object)
		return false;

	return true;
}


bool OCMLVisitor::visitExpression_Box( const Expression_Box& object )
{
	if ( !object)
		return false;

	return true;
}


bool OCMLVisitor::visitLogical_Expression( const Logical_Expression& object )
{
	if ( !object)
		return false;

	return true;
}


bool OCMLVisitor::visitModel_Option_Reference( const Model_Option_Reference& object )
{
	if ( !object)
		return false;

	return true;
}


bool OCMLVisitor::visitNumeric_Association( const Numeric_Association& object )
{
	if ( !object)
		return false;

	return true;
}


bool OCMLVisitor::visitOption( const Option& object )
{
	if ( !object)
		return false;

	return true;
}


bool OCMLVisitor::visitOption_Item( const Option_Item& object )
{
	if ( !object)
		return false;

	return true;
}


bool OCMLVisitor::visitOption_Reference( const Option_Reference& object )
{
	if ( !object)
		return false;

	return true;
}


void OCMLVisitor::visitObjectImpl( const BON::Object& obj )
{}


void OCMLVisitor::visitFCOImpl( const BON::FCO& fco )
{
	if ( !visitAssociated_Option( fco))
	if ( !visitAssociation( fco))
	if ( !visitAtom_Option_Reference( fco))
	if ( !visitDescribed_Item( fco))
	if ( !visitEquality_Association( fco))
	if ( !visitExpression_Box( fco))
	if ( !visitLogical_Expression( fco))
	if ( !visitModel_Option_Reference( fco))
	if ( !visitNumeric_Association( fco))
	if ( !visitOption( fco))
	if ( !visitOption_Item( fco))
	if ( !visitOption_Reference( fco))
	{
		// visiting other fco
	}
}


void OCMLVisitor::visitAtomImpl( const BON::Atom& atom )
{
	if ( !visitAND( atom))
	if ( !visitBoolean_Option( atom))
	if ( !visitEnum_Item( atom))
	if ( !visitEqual( atom))
	if ( !visitFlag_Item( atom))
	if ( !visitIF( atom))
	if ( !visitIFF( atom))
	if ( !visitInteger_Option( atom))
	if ( !visitNOT( atom))
	if ( !visitOR( atom))
	if ( !visitString_Option( atom))
	{
		// visiting other Atom
	}
}


void OCMLVisitor::visitReferenceImpl( const BON::Reference& ref )
{
	if ( !visitBoolean_Option_Reference( ref))
	if ( !visitEnum_Option_Reference( ref))
	if ( !visitFlag_Option_Reference( ref))
	if ( !visitInteger_Option_Reference( ref))
	if ( !visitString_Option_Reference( ref))
	{
		// visiting other Reference
	}
}


void OCMLVisitor::visitConnectionImpl( const BON::Connection& conn )
{
	if ( !visitBoolean_Equality_Association( conn))
	if ( !visitComparison_Association( conn))
	if ( !visitInteger_Equality_Association( conn))
	if ( !visitLogical_Association( conn))
	if ( !visitMax_Association( conn))
	if ( !visitMin_Association( conn))
	if ( !visitRange_Association( conn))
	if ( !visitSelect_Association( conn))
	if ( !visitString_Equality_Association( conn))
	{
		// visiting other Connection
	}
}


void OCMLVisitor::visitModelImpl( const BON::Model& model )
{
	if ( !visitEnum_Option( model))
	if ( !visitFlag_Option( model))
	if ( !visitOption_Category( model))
	if ( !visitRule( model))
	{
		// visiting other Model
	}
}



}; // namespace BON

