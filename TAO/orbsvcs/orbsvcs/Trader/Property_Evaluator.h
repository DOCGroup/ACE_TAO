/* -*- C++ -*- */

// ========================================================================
// $Id$
// 
// = LIBRARY 
//    orbsvcs
//  
// = FILENAME
//    Property_Evaluator.h
// 
// = AUTHOR 
//    Seth Widoff <sbw1@cs.wustl.edu> 
//   
// ========================================================================

#ifndef TAO_PROPERTY_EVALUATOR_H
#define TAO_PROPERTY_EVALUATOR_H

#include "stl.h"
#include "orbsvcs/CosTradingC.h"

class TAO_Property_Evaluator
//
// = TITLE
//   This class abstracts away the details of obtaining property
//   values and property types. Since the procedure for obtaining the
//   value or type of a dynamic property is disparate from the method
//   for a static property, TAO_Property_Evaluator provides methods
//   that will unify the two approaches under a single
//   interface. Since dynamic properties aren't necessarily supported
//   by a trader, this class accounts for that contingency. The use of 
//   indexed lookups allows them to occur in constant time on the
//   CORBA sequences, but requires that the client know the layout of
//   properties ahead of time. 
{
public:
  
  TAO_Property_Evaluator(const CosTrading::PropertySeq& properties,
			 CORBA::Boolean supports_dp = 1);
  
  TAO_Property_Evaluator(CosTrading::Offer& offer,
			 CORBA::Boolean supports_dp = 1);
  // Construct an instance of TAO_Property_Evaluator that operates on
  // an <offer> where the support for dynamic properties is dictated
  // by <supports_dynamic_properties>.
  
  int is_dynamic_property(int index);
  // Returns 1 if the property at index <index> is dynamic. Returns a
  // 0 when the index is out of bounds.

  CORBA::Any* property_value(int index, CORBA::Environment& _env)
    TAO_THROW_SPEC ((CosTradingDynamic::DPEvalFailure));

  // Returns value of the property whose index is <index>. If the
  // property at that index is dynamic and the trader supports dynamic 
  // properties, then the property_value method will obtain the value
  // of the dynamic property using the evalDP method on the
  // CosTradingDynamic::DynamicPropEval interface, passing on a
  // CosTradingDynamic::DPEvalFailure exception on failure. If the
  // property index is undefined, the method returns a null pointer.
  
  CORBA::TypeCode* property_type(int index);
  // Returns the type of the property whose index is <index>. If the
  // property is dynamic and the trader supports dynamic properties,
  // then the method returns the <returned_type> field of the
  // CosTradingDynamic::DynamicProp struct associated with the
  // property name. If the index is out of bounds, the method returns 
  // a null pointer (that is, 0).
  
protected:

  typedef CosTradingDynamic::DynamicProp DP_Struct;
  typedef CosTradingDynamic::DynamicPropEval DP_Eval;
  
  const CosTrading::PropertySeq& props_;
  // The offer from which the TAO_Property_Evaluator extracts property 
  // information.

  int supports_dp_;
};

class TAO_Property_Evaluator_By_Name : public TAO_Property_Evaluator
//
// = TITLE
//    This class extends the TAO_Property_Evaluator to allow lookups
//    based on the property name of interest. Since the property
//    information is contained within an integer indexed array,
//    lookups may occur in O(n) time, where n is the length of the
//    array. To make lookups by name more efficient,
//    TAO_Property_Evaluator_By_Name creates a mapping of property
//    names to integer indicies, upon which lookups are guaranteed to
//    be O(lg n).
{
public:

  TAO_Property_Evaluator_By_Name (const CosTrading::PropertySeq& properties,
				  CORBA::Environment& _env,
				  CORBA::Boolean supports_dp = 1)
    TAO_THROW_SPEC ((CosTrading::DuplicatePropertyName,
		     CosTrading::IllegalPropertyName));
  
  TAO_Property_Evaluator_By_Name(CosTrading::Offer& offer,
				 CORBA::Boolean supports_dp = 1);
  // Construct an instance of TAO_Property_Evaluator that operates on
  // an <offer> where the support for dynamic properties is dictated
  // by <supports_dynamic_properties>.

  int is_dynamic_property(const char* property_name);
  // Returns 1 if the property whose name is <property_name> is
  // defined and dynamic. If the property is undefined, this method
  // will throw a Property_Undefined exception with impunity.

  CORBA::Any* property_value(const char* property_name,
			     CORBA::Environment& _env)
    TAO_THROW_SPEC ((CosTradingDynamic::DPEvalFailure));

  // This method is identical to its counterpart in
  // TAO_Property_Evaluator, except property_value first discovers the 
  // index through a string matching lookup.
  
  CORBA::TypeCode* property_type(const char* property_name);
  // This method is identical to its counterpart in
  // TAO_Property_Evaluator, exception property_type first discovers
  // the index through a string matching lookup.

  const CosTrading::Property* get_property (const char* property_name);
  
private:
  
  typedef map<string, int, less<string> > Lookup_Table;
  typedef Lookup_Table::iterator Lookup_Table_Iter;
  // A mapping, upon which lookups will take O(lg n), of property
  // names to sequence indices.
  
  Lookup_Table table_;
  // The instance of the above mapping for the offer provided in the
  // constructor. 
};

#endif /* TAO_PROPERTY_EVALUATOR_H */
