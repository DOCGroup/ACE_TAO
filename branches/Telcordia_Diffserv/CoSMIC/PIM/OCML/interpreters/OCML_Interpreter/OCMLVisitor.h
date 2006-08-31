#ifndef OCMLVISITOR_H
#define OCMLVISITOR_H

#include "BON.h"
#include "BONImpl.h"
#include "Extensions.h"
#include "OCMLBonX.h"

namespace OCML_BON
{
  class OCMLVisitor
    : public BON::Visitor
  {
  public :
    void visitObjectImpl( const BON::Object& object );
    void visitFCOImpl( const BON::FCO& fco );
    void visitAtomImpl( const BON::Atom& atom );
    void visitReferenceImpl( const BON::Reference& ref );
    void visitConnectionImpl( const BON::Connection& conn);
    void visitModelImpl( const BON::Model& model );
  private :
    bool visitAND( const AND& object );
    bool visitBoolean_Option( const Boolean_Option& object );
    bool visitEnum_Item( const Enum_Item& object );
    bool visitEqual( const Equal& object );
    bool visitFlag_Item( const Flag_Item& object );
    bool visitIF( const IF& object );
    bool visitIFF( const IFF& object );
    bool visitInteger_Option( const Integer_Option& object );
    bool visitNOT( const NOT& object );
    bool visitOR( const OR& object );
    bool visitString_Option( const String_Option& object );
    bool visitEnum_Option( const Enum_Option& object );
    bool visitFlag_Option( const Flag_Option& object );
    bool visitOption_Category( const Option_Category& object );
    bool visitRule( const Rule& object );
    bool visitBoolean_Equality_Association( const Boolean_Equality_Association& object );
    bool visitComparison_Association( const Comparison_Association& object );
    bool visitInteger_Equality_Association( const Integer_Equality_Association& object );
    bool visitLogical_Association( const Logical_Association& object );
    bool visitMax_Association( const Max_Association& object );
    bool visitMin_Association( const Min_Association& object );
    bool visitRange_Association( const Range_Association& object );
    bool visitSelect_Association( const Select_Association& object );
    bool visitString_Equality_Association( const String_Equality_Association& object );
    bool visitBoolean_Option_Reference( const Boolean_Option_Reference& object );
    bool visitEnum_Option_Reference( const Enum_Option_Reference& object );
    bool visitFlag_Option_Reference( const Flag_Option_Reference& object );
    bool visitInteger_Option_Reference( const Integer_Option_Reference& object );
    bool visitString_Option_Reference( const String_Option_Reference& object );
    bool visitAssociated_Option( const Associated_Option& object );
    bool visitAssociation( const Association& object );
    bool visitAtom_Option_Reference( const Atom_Option_Reference& object );
    bool visitDescribed_Item( const Described_Item& object );
    bool visitEquality_Association( const Equality_Association& object );
    bool visitExpression_Box( const Expression_Box& object );
    bool visitLogical_Expression( const Logical_Expression& object );
    bool visitModel_Option_Reference( const Model_Option_Reference& object );
    bool visitNumeric_Association( const Numeric_Association& object );
    bool visitOption( const Option& object );
    bool visitOption_Item( const Option_Item& object );
    bool visitOption_Reference( const Option_Reference& object );
  };

}; // namespace

#endif // OCMLVISITOR_H
