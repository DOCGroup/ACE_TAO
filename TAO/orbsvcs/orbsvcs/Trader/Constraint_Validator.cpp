// Constraint_Validator.cpp

// ========================================================================
// $Id$
// 
// = LIBRARY 
//    orbsvcs
//  
// = FILENAME
//    Constraint_Validator.cpp
// 
// = AUTHOR 
//    Seth Widoff <sbw1@cs.wustl.edu> 
//   
// ========================================================================

#include "Constraint_Validator.h"

TAO_Constraint_Validator::
TAO_Constraint_Validator
(CosTradingRepos::ServiceTypeRepository::TypeStruct* type_struct)
{
  CosTradingRepos::ServiceTypeRepository::PropStructSeq& prop_seq =
    type_struct->props;
  int length = prop_seq.length();
  
  // Create a map of the service type properties to their types.
  for (int i = 0; i < length; i++)
    {
      CORBA::TypeCode_var corba_type = prop_seq[i].value_type;
      TAO_String_Hash_Key prop_name_str = (const char*) prop_seq[i].name;
      this->type_map_.bind (prop_name_str, corba_type.ptr ());
    }
}

int
TAO_Constraint_Validator::validate (TAO_Constraint* root)
{
  return root->accept(this);
}

int
TAO_Constraint_Validator::visit_constraint(TAO_Unary_Constraint* constraint)
{
  int return_value = -1;
  TAO_Expression_Type type;
  TAO_Constraint* operand = constraint->operand();
  this->extract_type(operand, type);
  
  if (this->expr_returns_boolean (type))
    return_value =  operand->accept (this);

  return return_value;
}

int
TAO_Constraint_Validator::visit_first(TAO_Noop_Constraint* noop)
{
  return 0;
}

int
TAO_Constraint_Validator::visit_random(TAO_Noop_Constraint* noop)
{
  return 0;
}

int
TAO_Constraint_Validator::visit_with(TAO_Unary_Constraint* unary_with)
{
  return this->visit_constraint(unary_with);
}

int
TAO_Constraint_Validator::visit_min(TAO_Unary_Constraint* unary_min)
{
  return this->visit_unary_minus(unary_min);
}

int
TAO_Constraint_Validator::visit_max(TAO_Unary_Constraint* unary_max)
{
  return this->visit_unary_minus(unary_max);
}

int 
TAO_Constraint_Validator::
visit_and(TAO_Binary_Constraint* boolean_and)
{
  int return_value = -1;
  TAO_Constraint* left = boolean_and->left_operand(),
    *right = boolean_and->right_operand();
  TAO_Expression_Type left_type, right_type;
  this->extract_type(left, left_type);
  this->extract_type(right, right_type);
  
  // Can only "and" expressions that return a boolean value
  if (this->expr_returns_boolean(left_type) &&
      this->expr_returns_boolean(right_type))
    {
      if (left->accept(this) == 0 &&
	  right->accept(this) == 0)
	return_value = 0;
    }

  return return_value;
}

int
TAO_Constraint_Validator::
visit_or(TAO_Binary_Constraint* boolean_or)
{
  // The types for or are the same as those for and.
  return this->visit_and(boolean_or);
}

int
TAO_Constraint_Validator::
visit_not(TAO_Unary_Constraint* unary_not)
{
  int return_value = -1;
  // Not can only negate an expression that returns a boolean.
  TAO_Expression_Type type;
  TAO_Constraint* operand = unary_not->operand();
  this->extract_type(operand, type);

  if (this->expr_returns_boolean(type))
    return_value = operand->accept(this);

  return return_value;
}

int
TAO_Constraint_Validator::
visit_exist(TAO_Unary_Constraint* unary_exist)
{
  // Exist simply requires that its operand be a property name
  // included in the service type.
  int return_value = -1;
  TAO_Constraint* operand = unary_exist->operand();
  TAO_Expression_Type type = operand->expr_type();
  
  if (type == TAO_IDENT)
    return_value = operand->accept(this);

  return return_value;
}

int
TAO_Constraint_Validator::
visit_unary_minus(TAO_Unary_Constraint* unary_minus)
{
  // Unary minus can only negate a numeric operand.
  int return_value = -1;
  TAO_Expression_Type type;
  TAO_Constraint* operand = unary_minus->operand();
  this->extract_type(operand, type);
    
  if (this->expr_returns_number(type))
    return_value = operand->accept(this);

  return return_value;
}

int
TAO_Constraint_Validator::
visit_add(TAO_Binary_Constraint* boolean_add)
{
  // All the mathematical operators require numeric operands.
  int return_value = -1;
  TAO_Constraint* left = boolean_add->left_operand(),
    *right = boolean_add->right_operand();
  TAO_Expression_Type left_type, right_type;
  this->extract_type(left, left_type);
  this->extract_type(right, right_type);

  if (this->expr_returns_number(left_type) &&
      this->expr_returns_number(right_type))
    {
      if (left->accept(this) == 0 &&
	  right->accept(this) == 0)
	return_value = 0;
    }

  return return_value;
}

int
TAO_Constraint_Validator::
visit_sub(TAO_Binary_Constraint* boolean_sub)
{
  return this->visit_add(boolean_sub);
}

int
TAO_Constraint_Validator::
visit_mult(TAO_Binary_Constraint* boolean_mult)
{
  return this->visit_add(boolean_mult);
}

int
TAO_Constraint_Validator::
visit_div(TAO_Binary_Constraint* boolean_div)
{
  // Div not only requires that both of its operands be numeric, but
  // also the the demoninator not be zero. However, since the
  // denominator can be an expression who's value can only be
  // determined while evaulating the constraint expression, this
  // method detects only when the demoniator is a literal whose value
  // is zero.
  int return_value = -1;
  TAO_Constraint* left = boolean_div->left_operand(),
    *right = boolean_div->right_operand();
  TAO_Expression_Type left_type, right_type;
  this->extract_type(left, left_type);
  this->extract_type(right, right_type);

  if (this->expr_returns_number(left_type) &&
      this->expr_returns_number(right_type))
    {
      // Prevent division by zero, a no no.
      int right_isnt_zero = 1;
      switch(right->expr_type())
	{
	case TAO_UNSIGNED:
	  right_isnt_zero =
	    ((CORBA::ULong) (*((TAO_Literal_Constraint*) right)) != 0);
	  break;
	case TAO_SIGNED:
	  right_isnt_zero =
	    ((CORBA::Long) (*((TAO_Literal_Constraint*) right)) != 0);
	  break;
	case TAO_DOUBLE:
	  right_isnt_zero =
	    ((CORBA::Double) (*((TAO_Literal_Constraint*) right)) != 0.0)
	  break;
	}

      if (right_isnt_zero)
	{
	  if (left->accept(this) == 0 &&
	      right->accept(this) == 0)
	    return_value = 0;
	}
    }

  return return_value;
}

int
TAO_Constraint_Validator::
visit_twiddle(TAO_Binary_Constraint* binary_twiddle)
{
  // Twiddle requires that both of its operand be strings.
  int return_value = -1;
  TAO_Constraint* left = binary_twiddle->left_operand(),
    *right = binary_twiddle->right_operand();
  TAO_Expression_Type left_type, right_type;
  this->extract_type(left, left_type);
  this->extract_type(right, right_type);
  
  if (this->expr_returns_string(left_type) &&
      this->expr_returns_string(right_type))
    {
      if (left->accept(this) == 0 &&
	  right->accept(this) == 0)
	return_value = 0;
    }

  return return_value;
}

int
TAO_Constraint_Validator::
visit_in (TAO_Binary_Constraint* binary_in)
{
  // In requires that the right operand be a sequence of a simple type
  // and that its left operand be an expression that evaluates to a
  // value of the same simple type.
  int return_value = -1;
  TAO_Constraint* left = binary_in->left_operand(),
    *right = binary_in->right_operand();
  TAO_Expression_Type left_type, right_type;
  CORBA::TypeCode* prop_type = this->extract_type(right, right_type);
  this->extract_type(left, left_type);

  if (right_type == TAO_SEQUENCE)
    {   
      int types_match = 0;
      CORBA::Environment env;
      
      if (this->expr_returns_number (left_type))
	{
	  types_match =
	    prop_type->equal (TAO_Sequences::_tc_ShortSeq, env) ||
	    prop_type->equal (TAO_Sequences::_tc_UShortSeq, env) ||
	    prop_type->equal (TAO_Sequences::_tc_LongSeq, env) ||
	    prop_type->equal (TAO_Sequences::_tc_ULongSeq, env) ||
	    prop_type->equal (TAO_Sequences::_tc_DoubleSeq, env) ||
	    prop_type->equal (TAO_Sequences::_tc_FloatSeq, env);
	}
      else if (this->expr_returns_boolean (left_type))
	types_match = prop_type->equal (TAO_Sequences::_tc_BooleanSeq, env);
      else if (this->expr_returns_string (left_type))
	types_match = prop_type->equal (TAO_Sequences::_tc_StringSeq, env);
      
      if (types_match)
	return_value = left->accept(this);
    }

  return return_value;
}

int
TAO_Constraint_Validator::
visit_less_than(TAO_Binary_Constraint* boolean_lt)
{
  // Comparison operations require that both operands be of the same
  // simple type.
  int return_value = -1;
  TAO_Constraint* left = boolean_lt->left_operand(),
    *right = boolean_lt->right_operand();
  TAO_Expression_Type left_type, right_type;
  this->extract_type(left, left_type);
  this->extract_type(right, right_type);

  if ((this->expr_returns_number(left_type) &&
       this->expr_returns_number(right_type)) ||
      (this->expr_returns_string(left_type) &&
       this->expr_returns_string(right_type)))
    {
      if (left->accept(this) == 0 &&
	  right->accept(this) == 0)
	return_value = 0;
    }

  return return_value;
}

int
TAO_Constraint_Validator::
visit_less_than_equal(TAO_Binary_Constraint* boolean_lte)
{
  return this->visit_less_than(boolean_lte);
}

int
TAO_Constraint_Validator::
visit_greater_than(TAO_Binary_Constraint* boolean_gt)
{
  return this->visit_less_than(boolean_gt);
}

int
TAO_Constraint_Validator::
visit_greater_than_equal(TAO_Binary_Constraint* boolean_gte)
{
  return this->visit_less_than(boolean_gte);
}

int
TAO_Constraint_Validator::
visit_equal(TAO_Binary_Constraint* boolean_eq)
{
  int return_value = -1;
  TAO_Constraint* left = boolean_eq->left_operand(),
    *right = boolean_eq->right_operand();
  TAO_Expression_Type left_type, right_type;
  this->extract_type(left, left_type);
  this->extract_type(right, right_type);

  if ((this->expr_returns_number(left_type) &&
       this->expr_returns_number(right_type)) ||
      (this->expr_returns_string(left_type) &&
       this->expr_returns_string(right_type)) ||
      (this->expr_returns_boolean(left_type) &&
       this->expr_returns_boolean(right_type)))
    {
      if (left->accept(this) == 0 &&
	  right->accept(this) == 0)
	return_value = 0;
    }

  return return_value;
}

int
TAO_Constraint_Validator::
visit_not_equal(TAO_Binary_Constraint* boolean_neq)
{
  return this->visit_equal(boolean_neq);
}

int
TAO_Constraint_Validator::
visit_literal(TAO_Literal_Constraint* literal)
{
  return 0;
}

int
TAO_Constraint_Validator::
visit_property(TAO_Property_Constraint* literal)
{
  return 0;
}

CORBA::TypeCode*
TAO_Constraint_Validator::extract_type (TAO_Constraint* expr,
					TAO_Expression_Type& type)
{
  CORBA::TypeCode* return_value = 0;

  type = expr->expr_type();
  if (type == TAO_IDENT)
    {
      TAO_Property_Constraint* prop = (TAO_Property_Constraint*) expr;
      TAO_String_Hash_Key prop_name (prop->name());

      if (this->type_map_.find (prop_name, return_value) == 0)
	type = TAO_Literal_Constraint::comparable_type (return_value);
    }

  return return_value;
}

int
TAO_Constraint_Validator::expr_returns_boolean(TAO_Expression_Type expr_type)
{
  // If the expression is a boolean operations, a boolean literal, or
  // a boolean property, return 1.
  int return_value = 0;
  
  if (expr_type <= TAO_BOOLEAN)
    return_value = 1;
  
  return return_value;
}


int
TAO_Constraint_Validator::expr_returns_number(TAO_Expression_Type expr_type)
{
  // If the expression is a number operation, a numeric literal, or a
  // numeric property, return 1.
  int return_value = 0;
  
  if ((expr_type >= TAO_PLUS && expr_type <= TAO_NUMBER) ||
      (expr_type >= TAO_UNSIGNED && expr_type <= TAO_DOUBLE))
    return_value = 1;

  return return_value;
}

int
TAO_Constraint_Validator::expr_returns_string(TAO_Expression_Type expr_type)
{
  // If the expression is an operation with a string return value, a
  // string literal, or a property whose type is string, return 1.
  int return_value = 0;

  if (expr_type == TAO_STRING)
    return_value = 1;

  return return_value;
}

