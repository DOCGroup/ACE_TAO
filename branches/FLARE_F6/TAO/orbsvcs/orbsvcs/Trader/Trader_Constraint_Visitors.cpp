// $Id$

#include "orbsvcs/Trader/Trader_Constraint_Visitors.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Trader_Constraint_Evaluator::
TAO_Trader_Constraint_Evaluator (CosTrading::Offer* offer,
                                 CORBA::Boolean supports_dp)
  : prop_eval_ (*offer, supports_dp)
{
  this->props_.close ();
  this->props_.open ();
  int length = offer->properties.length ();

  // Create a map of property names to their values.
  for (int i = 0; i < length; i++)
    {
      CORBA::String_var name = (const char*) offer->properties[i].name;
      this->props_.bind (name, i);
    }
}

int
TAO_Trader_Constraint_Evaluator::
visit_property (TAO_Property_Constraint* literal)
{
  int return_value = -1, prop_index = 0;
  // Handle case where property is not, in fact, mapped to a value
  CORBA::String_var prop_name ((const char*) literal->name ());

  if (this->props_.find (prop_name, prop_index) == 0)
    {

      CORBA::Any *value = 0;
      // Retrieve the value of the property from the Property_Evaluator
      try
        {
          value = this->prop_eval_.property_value (prop_index);
        }
      catch (const CORBA::Exception&)
        {
          return -1;
        }

      if (value != 0)
        {
          this->queue_.enqueue_head (TAO_Literal_Constraint (value));
          return_value = 0;
        }
    }

  return return_value;
}

TAO_Trader_Constraint_Validator::
TAO_Trader_Constraint_Validator
(const CosTradingRepos::ServiceTypeRepository::TypeStruct& type_struct)
{
  const CosTradingRepos::ServiceTypeRepository::PropStructSeq& prop_seq =
    type_struct.props;
  int length = prop_seq.length ();

  // Create a map of the service type properties to their types.
  for (int i = 0; i < length; i++)
    {
      CORBA::TypeCode_ptr corba_type =
        CORBA::TypeCode::_duplicate (prop_seq[i].value_type.in ());

      CORBA::String_var prop_name_str = (const char*) prop_seq[i].name;
      this->type_map_.bind (prop_name_str, corba_type);
    }
}

TAO_Trader_Constraint_Validator::
~TAO_Trader_Constraint_Validator ()
{
}

TAO_END_VERSIONED_NAMESPACE_DECL
