// $Id$

#include "Trader_Constraint_Visitors.h"

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
      TAO_String_Hash_Key name = (const char*) offer->properties[i].name;
      this->props_.bind (name, i);
    }
}

int
TAO_Trader_Constraint_Evaluator::
visit_property (TAO_Property_Constraint* literal)
{
  int return_value = -1, prop_index = 0;
  // Handle case where property is not, in fact, mapped to a value
  TAO_String_Hash_Key prop_name ((const char*) literal->name ());

  if (this->props_.find (prop_name, prop_index) == 0)
    {
      ACE_DECLARE_NEW_CORBA_ENV;

      CORBA::Any *value = 0;
      // Retrieve the value of the property from the Property_Evaluator
      ACE_TRY
        {
          value = this->prop_eval_.property_value (prop_index ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      ACE_CATCHANY
        {
          return -1;
        }
      ACE_ENDTRY;
      //      ACE_CHECK_RETURN (-1);

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

      TAO_String_Hash_Key prop_name_str = (const char*) prop_seq[i].name;
      this->type_map_.bind (prop_name_str, corba_type);
    }
}

TAO_Trader_Constraint_Validator::
~TAO_Trader_Constraint_Validator ()
{
}
