// File: Property_Evaluator.cpp
// $Id$

#include "Trader.h"
#include "Property_Evaluator.h"

// Constructor

TAO_Property_Evaluator::
TAO_Property_Evaluator(const CosTrading::PropertySeq& props,
		       CORBA::Boolean supports_dp)
  : props_ (props),
    supports_dp_ (supports_dp)
{
}


TAO_Property_Evaluator::
TAO_Property_Evaluator(CosTrading::Offer& offer,
		       CORBA::Boolean supports_dp)
  : props_ (offer.properties),
    supports_dp_ (supports_dp)
{
}

int
TAO_Property_Evaluator::is_dynamic_property (int index)
{
  CORBA::Environment env;
  int return_value = 0,
    num_properties = this->props_.length();

  // Ensure index is in bounds. 
  if (index >= 0 && index < num_properties)
    {
      // Obtain the value of the property at index <index>.
      const CORBA::Any& value = this->props_[index].value;
      CORBA::TypeCode* type = value.type();
      
      if (type->equal(CosTradingDynamic::_tc_DynamicProp, env))
	return_value = 1;
    }
  
  return return_value;
}

CORBA::Any*
TAO_Property_Evaluator::property_value(int index,
				       CORBA::Environment& _env)
  TAO_THROW_SPEC ((CosTradingDynamic::DPEvalFailure))
{
  CORBA::Any* prop_val = 0;
  
  if (! this->is_dynamic_property(index))
    prop_val = (CORBA::Any *) &(this->props_[index].value);
  else if (this->supports_dp_)
    {
      // Property is defined at this point.
      DP_Eval* dp_eval;
      CosTradingDynamic::DynamicProp* dp_struct;
      const CORBA::String_var name = this->props_[index].name;
      const CORBA::Any& value = this->props_[index].value;

      // Extract the DP_Struct.
      value >>= dp_struct;
      dp_eval = dp_struct->eval_if;
	  
      if (CORBA::is_nil (dp_eval))
	TAO_THROW_RETURN (CosTradingDynamic::DPEvalFailure (), prop_val);
      else
	{
	  CORBA::TypeCode* type = dp_struct->returned_type;
	  CORBA::Any& info = dp_struct->extra_info;
	  
	  TAO_TRY
	    {
	      // Retrieve the value of the dynamic property.
	      prop_val = dp_eval->evalDP(name, type, info, TAO_TRY_ENV);
	    }
	  TAO_CATCH (CORBA::SystemException, excp)
	    {
	      TAO_THROW_RETURN
		(CosTradingDynamic::DPEvalFailure (name, type, info),
		 prop_val);
	    }
	  TAO_ENDTRY;
	}
    }

  return prop_val;
}

CORBA::TypeCode*
TAO_Property_Evaluator::property_type(int index)
{
  CORBA::TypeCode* prop_type = CORBA::TypeCode::_nil();
  
  // Determine if property is both defined and dynamic.
  if (this->is_dynamic_property (index))
    {
      // Extract type information from the DP_Struct.
      const CORBA::Any& value = this->props_[index].value;
      CosTradingDynamic::DynamicProp* dp_struct;
      value >>= dp_struct;
      
      // Grab a pointer to the returned_type description
      prop_type = CORBA::TypeCode::_duplicate(dp_struct->returned_type);
    }
  else
    // TypeCode is self-evident at this point.
    prop_type = this->props_[index].value.type();
  
  return prop_type;
}

TAO_Property_Evaluator_By_Name::
TAO_Property_Evaluator_By_Name (const CosTrading::PropertySeq& properties,
				CORBA::Environment& _env,
				CORBA::Boolean supports_dp)
  TAO_THROW_SPEC ((CosTrading::DuplicatePropertyName,
		   CosTrading::IllegalPropertyName))
    : TAO_Property_Evaluator (properties, supports_dp)
{
  string prop_name;
  int length = this->props_.length();

  for (int i = 0; i < length; i++)
    {
      const CosTrading::Property& prop = this->props_[i];

      if (! TAO_Trader_Base::is_valid_identifier_name (prop.name))
	TAO_THROW (CosTrading::IllegalPropertyName (prop.name));
      
      prop_name = prop.name;
      if (! (this->table_.insert (make_pair (prop_name, i))).second)
	TAO_THROW (CosTrading::DuplicatePropertyName (prop.name));
    }
}

TAO_Property_Evaluator_By_Name::
TAO_Property_Evaluator_By_Name(CosTrading::Offer& offer,
			       CORBA::Boolean supports_dp)
  : TAO_Property_Evaluator(offer, supports_dp)
{
  int length = this->props_.length();

  for (int i = 0; i < length; i++)
    {
      string prop_name = (const char*) this->props_[i].name;
      this->table_[prop_name] = i;
    }
}

int
TAO_Property_Evaluator_By_Name::
is_dynamic_property(const char* property_name)
{
  int predicate = 0;
  string prop_name (property_name);
  Lookup_Table_Iter lookup_iter = this->table_.find (prop_name);

  // If the property name is in the map, delegate evaluation to our
  // superclass. Otherwise, throw an exception.
  if (lookup_iter != this->table_.end())
    {
      int index = (*lookup_iter).second;
      predicate = TAO_Property_Evaluator::is_dynamic_property(index);
    }

  return predicate;
}
  
CORBA::Any*
TAO_Property_Evaluator_By_Name::property_value(const char* property_name,
					       CORBA::Environment& _env)
  TAO_THROW_SPEC ((CosTradingDynamic::DPEvalFailure))
{
  CORBA::Any* prop_value = 0;
  string prop_name(property_name);
  Lookup_Table_Iter lookup_iter = this->table_.find(prop_name);

  // If the property name is in the map, delegate evaluation to our
  // superclass. Otherwise, throw an exception.
  if (lookup_iter != this->table_.end())
    {
      int index = (*lookup_iter).second;
      prop_value = TAO_Property_Evaluator::property_value(index, _env);
    }

  return prop_value;
}
  
CORBA::TypeCode*
TAO_Property_Evaluator_By_Name::property_type(const char* property_name)
{
  string prop_name(property_name);
  CORBA::TypeCode* prop_type = CORBA::TypeCode::_nil();
  Lookup_Table_Iter lookup_iter = this->table_.find(prop_name);
  
  // If the property name is in the map, delegate evaluation to our
  // superclass. Otherwise, throw an exception.
  if (lookup_iter != this->table_.end())
    {
      int index = (*lookup_iter).second;
      prop_type = TAO_Property_Evaluator::property_type(index);
    }

  return prop_type;
}

const CosTrading::Property*
TAO_Property_Evaluator_By_Name::get_property (const char* property_name)
{
  CosTrading::Property* property = 0;
  string prop_name (property_name);
  Lookup_Table_Iter lookup_iter = this->table_.find(prop_name);

  if (lookup_iter != this->table_.end())
    property = (CosTrading::Property *) &this->props_[(*lookup_iter).second];

  return property;
}

