// Constraint_Evaluator.cpp
// $Id$

// ========================================================================
// 
// = LIBRARY 
//    orbsvcs
//  
// = FILENAME
//    Constraint_Evaluator.cpp
// 
// = AUTHOR 
//    Seth Widoff <sbw1@cs.wustl.edu> 
//   
// ========================================================================


#define get_left_operand() (this->queue_.rbegin()[1])
#define get_right_operand() this->queue_.back()
#define get_operand() this->queue_.back()

#include "Constraint_Evaluator.h"

TAO_Constraint_Evaluator::
TAO_Constraint_Evaluator(CosTrading::Offer* offer,
			 CORBA::Boolean supports_dp)
  : prop_eval_ (*offer, supports_dp)
{
  this->props_.clear();
  int length = offer->properties.length();

  // Create a map of property names to their values.
  for (int i = 0; i < length; i++)
    {
      string name = (const char*)offer->properties[i].name;
      this->props_[name] = i;
    }    
}


CORBA::Boolean
TAO_Constraint_Evaluator::evaluate_constraint(TAO_Constraint* root)
{
  CORBA::Boolean result = 0;
  while (! this->queue_.empty())
    this->queue_.pop_back();
   
  // Evaluate the offer according to the constraints in root_;
  if (root != 0)
    {
      if ((root->accept(this) == 0) &&
	  (! this->queue_.empty ()))
	{
	  result = (CORBA::Boolean) get_operand();
	  this->queue_.pop_back ();
	}
    }

  // If a property couldn't be evaluated we must return 0.
  return result;
}

int
TAO_Constraint_Evaluator::
evaluate_preference(TAO_Constraint* root,
		    TAO_Literal_Constraint& result)
{
  int return_value = -1;
  while (! this->queue_.empty())
    this->queue_.pop_back();

  // Evaluate the offer according to the constraints in root_;
  if (root != 0)
    {
      if ((root->accept(this) == 0) &&
	  (! this->queue_.empty ()))
	{
	  result = get_operand();
	  this->queue_.pop_back();
	  return_value = 0;
	}
    }
  
  return return_value;
}

int
TAO_Constraint_Evaluator::visit_constraint(TAO_Unary_Constraint* constraint)
{
  TAO_Constraint* operand = constraint->operand ();
  return operand->accept(this);
}

int
TAO_Constraint_Evaluator::visit_with(TAO_Unary_Constraint* unary_with)
{
  TAO_Constraint* operand = unary_with->operand ();
  return operand->accept(this);
}

int
TAO_Constraint_Evaluator::visit_min(TAO_Unary_Constraint* unary_min)
{
  TAO_Constraint* operand = unary_min->operand ();
  return operand->accept(this);
}

int
TAO_Constraint_Evaluator::visit_max(TAO_Unary_Constraint* unary_max)
{
  TAO_Constraint* operand = unary_max->operand ();
  return operand->accept(this);
}

int
TAO_Constraint_Evaluator::visit_random(TAO_Noop_Constraint* noop_random)
{
  TAO_Literal_Constraint random((CORBA::Long) (ACE_OS::rand ()));
  this->queue_.push_back(random);  
  return 0;
}

int
TAO_Constraint_Evaluator::visit_first(TAO_Noop_Constraint* noop_first)
{
  TAO_Literal_Constraint first((CORBA::Long) 0);
  this->queue_.push_back (first);  
  return 0;
}

int
TAO_Constraint_Evaluator::
visit_and(TAO_Binary_Constraint* boolean_and)
{
  int return_value = -1;
  CORBA::Boolean result = (CORBA::Boolean) 0;
  TAO_Constraint* left = boolean_and->left_operand(),
    *right = boolean_and->right_operand();

  // Short circuiting AND.
  
  if (left->accept(this) == 0)
    {
      result = (CORBA::Boolean) get_operand();
      this->queue_.pop_back ();
      
      if (result)
	{
	  if (right->accept(this) == 0)
	    {
	      result = (CORBA::Boolean) get_operand();
	      this->queue_.pop_back ();

	      return_value = 0;
	    }
	}
      else
	return_value = 0;
    }

  if (return_value != -1)
    this->queue_.push_back (TAO_Literal_Constraint (result));
  
  return return_value;
}

int
TAO_Constraint_Evaluator::
visit_or(TAO_Binary_Constraint* boolean_or)
{
  int return_value = -1;
  CORBA::Boolean result = (CORBA::Boolean) 0;
  TAO_Constraint* left = boolean_or->left_operand (),
    *right = boolean_or->right_operand ();

  // Short-circuiting OR.
  
  if (left->accept(this) == 0)
    {
      result = (CORBA::Boolean) get_operand();
      this->queue_.pop_back ();
      
      if (result == (CORBA::Boolean) 0)
	{
	  if (right->accept (this) == 0)
	    {
	      result = (CORBA::Boolean) get_operand();
	      this->queue_.pop_back();
	      return_value = 0;
	    }
	}
      else
	return_value = 0;
    }

  if (return_value != -1)
    this->queue_.push_back (TAO_Literal_Constraint (result));
  
  return return_value;
}

int
TAO_Constraint_Evaluator::
visit_not(TAO_Unary_Constraint* unary_not)
{
  int return_value = -1;
  TAO_Constraint* operand = unary_not->operand();

  // Logical NOT.
  
  if (operand->accept(this) == 0)
    {
      CORBA::Boolean result = ! (CORBA::Boolean)get_operand();
      this->queue_.pop_back ();
      this->queue_.push_back (TAO_Literal_Constraint (result));

      return_value = 0;
    }

  return return_value;
}

int
TAO_Constraint_Evaluator::
visit_exist(TAO_Unary_Constraint* unary_exist)
{
  TAO_Property_Constraint* operand =
    (TAO_Property_Constraint*) unary_exist->operand ();
  string property_name ((const char*) operand->name ());

  // Determine if a property is defined on this offer.
  
  CORBA::Boolean result = (CORBA::Boolean)
    (this->props_.find (property_name) != this->props_.end());

  this->queue_.push_back (TAO_Literal_Constraint (result));
  
  return 0;
}

int
TAO_Constraint_Evaluator::
visit_unary_minus(TAO_Unary_Constraint* unary_minus)
{
  int return_value = -1;
  TAO_Constraint* operand = unary_minus->operand();

  if (operand->accept(this) == 0)
    {
      TAO_Literal_Constraint& result = - get_operand();
      this->queue_.pop_back ();
      this->queue_.push_back (result);

      return_value = 0;
    }

  return return_value;
}

void
TAO_Constraint_Evaluator::do_the_op (int operation)
{
  TAO_Literal_Constraint& l_op = get_left_operand();
  TAO_Literal_Constraint& r_op = get_right_operand();

  // Perform the listed bindary operation on the first two elements on 
  // the stack.
  
  TAO_Literal_Constraint& result =
    (operation <= TAO_NE)
    ?
    TAO_Literal_Constraint
    ((CORBA::Boolean)
     ((operation == TAO_GT) ? l_op > r_op :
      (operation == TAO_GE) ? l_op >= r_op :
      (operation == TAO_LT) ? l_op < r_op :
      (operation == TAO_LE) ? l_op <= r_op :
      (operation == TAO_NE) ? l_op != r_op :
      (operation == TAO_EQ) ? l_op == r_op : 0))
    :    
    ((operation == TAO_PLUS) ? l_op + r_op :
     (operation == TAO_MINUS) ? l_op - r_op :
     (operation == TAO_MULT) ? l_op * r_op :
     (operation == TAO_DIV) ? l_op / r_op :
     TAO_Literal_Constraint ());
    
  this->queue_.pop_back ();
  this->queue_.pop_back ();
  this->queue_.push_back (result);
}

int
TAO_Constraint_Evaluator::visit_bin_op (TAO_Binary_Constraint* op,
					 int operation)
{
  int return_value = -1;
  TAO_Constraint* left = op->left_operand(),
    *right = op->right_operand(); 

  // Perform an operation on the results of evaluating the left and
  // right branches of this subtree.
  if (left->accept (this) == 0)
    {
      if (right->accept (this) == 0)
	{	  
	  this->do_the_op (operation);
	  return_value = 0;
	}
      else
	this->queue_.pop_back ();
    }

  return return_value;
}

int
TAO_Constraint_Evaluator::
visit_add(TAO_Binary_Constraint* boolean_add)
{
  return this->visit_bin_op (boolean_add, TAO_PLUS);  
}

int
TAO_Constraint_Evaluator::
visit_sub(TAO_Binary_Constraint* boolean_sub)
{
  return this->visit_bin_op (boolean_sub, TAO_MINUS);
}

int
TAO_Constraint_Evaluator::
visit_mult(TAO_Binary_Constraint* boolean_mult)
{
  return this->visit_bin_op (boolean_mult, TAO_MULT);  
}

int
TAO_Constraint_Evaluator::
visit_div(TAO_Binary_Constraint* boolean_div)
{
  return this->visit_bin_op (boolean_div, TAO_DIV);  
}

int
TAO_Constraint_Evaluator::
visit_twiddle(TAO_Binary_Constraint* binary_twiddle)
{
  int return_value = -1;
  TAO_Constraint* left = binary_twiddle->left_operand(),
    *right = binary_twiddle->right_operand();

  // Determine if the left operand is a substring of the right.
  
  if (left->accept (this) == 0)
    {
      if (right->accept(this) == 0)
	{
	  TAO_Literal_Constraint& left_operand = get_left_operand();
	  TAO_Literal_Constraint& right_operand = get_right_operand();
	  
	  CORBA::Boolean result = (CORBA::Boolean)
	    (ACE_OS::strstr ((const char*)left_operand,
			     (const char*)right_operand) != 0);
	  
	  this->queue_.pop_back();
	  this->queue_.pop_back();
	  this->queue_.push_back (TAO_Literal_Constraint (result));
	  return_value = 0;
	}
      else
	this->queue_.pop_back ();
    }

  return return_value;
}

int
TAO_Constraint_Evaluator::
visit_in(TAO_Binary_Constraint* binary_in)
{
  int return_value = -1;
  TAO_Constraint* left = binary_in->left_operand(),
    *right = binary_in->right_operand();

  // Determine if the left operand is contained in the right.
  
  if (left->accept(this) == 0)
    {
      if (this->visit_property ((TAO_Property_Constraint*) right) == 0)
	{
	  TAO_Literal_Constraint& left_value = get_left_operand(); 
	  const CORBA::Any* any = (const CORBA::Any*) get_right_operand();

	  if (any != 0)
	    {
	      CORBA::Boolean result =
		this->sequence_does_contain ((CORBA::Any*) any, left_value);
	      
	      this->queue_.pop_back ();
	      this->queue_.pop_back ();
	      this->queue_.push_back (TAO_Literal_Constraint (result));
	      return_value = 0;
	    }
	  else
	    this->queue_.pop_back ();	  
	}
    }

  return return_value;
}

int
TAO_Constraint_Evaluator::
visit_less_than(TAO_Binary_Constraint* boolean_lt)
{
  return this->visit_bin_op (boolean_lt, TAO_LT);
}

int
TAO_Constraint_Evaluator::
visit_less_than_equal(TAO_Binary_Constraint* boolean_lte)
{
  return this->visit_bin_op (boolean_lte, TAO_LE);
}

int
TAO_Constraint_Evaluator::
visit_greater_than(TAO_Binary_Constraint* boolean_gt)
{
  return this->visit_bin_op (boolean_gt, TAO_GT);
}

int
TAO_Constraint_Evaluator::
visit_greater_than_equal(TAO_Binary_Constraint* boolean_gte)
{
  return this->visit_bin_op (boolean_gte, TAO_GE); 
}

int
TAO_Constraint_Evaluator::
visit_equal(TAO_Binary_Constraint* boolean_eq)
{
  return this->visit_bin_op (boolean_eq, TAO_EQ); 
}

int
TAO_Constraint_Evaluator::
visit_not_equal(TAO_Binary_Constraint* boolean_neq)
{
  return this->visit_bin_op (boolean_neq, TAO_NE); 
}

int
TAO_Constraint_Evaluator::
visit_literal(TAO_Literal_Constraint* literal)
{
  this->queue_.push_back (*literal);
  return 0;
}

int
TAO_Constraint_Evaluator::
visit_property(TAO_Property_Constraint* literal)
{
  int return_value = -1;
  // Handle case where property is not, in fact, mapped to a value
  string prop_name((const char*) literal->name ());
  Property_Map_Iter prop_iter = this->props_.find (prop_name);

  if (prop_iter != this->props_.end())
    {
      CORBA::Environment env;
      // Retrieve the value of the property from the Property_Evaluator
      int prop_index = (*prop_iter).second;

      CORBA::Any* value =
	this->prop_eval_.property_value (prop_index, env);

      if (value != 0)
	{
	  this->queue_.push_back (TAO_Literal_Constraint (value));
	  return_value = 0;
	}
    }

  return return_value;
}


template <class SEQ, class OPERAND_TYPE> CORBA::Boolean
TAO_find (SEQ& sequence, const OPERAND_TYPE element)
{
  int length = sequence.length(),
    return_value = 0;

  for (int i = 0; i < length; i++)
    {
      if (sequence[i] == element)
	{	  
	  return_value = 1;
	  break;
	}
    }

  return (CORBA::Boolean) return_value;
}

#ifdef ACE_HAS_TEMPLATE_SPECIALIZATION
template<> CORBA::Boolean
TAO_find (TAO_Sequences::StringSeq& sequence, const char* element)
#else
CORBA::Boolean
TAO_find_string (TAO_Sequences::StringSeq& sequence, const char* element)
#endif /* ACE_HAS_TEMPLATE_SPECIALIZATION */
{
 int length = sequence.length(),
    return_value = 0;

  for (int i = 0; i < length; i++)
    {
      if (ACE_OS::strcmp(sequence[i], element) == 0)
	{	  
	  return_value = 1;
	  break;
	}
    }

  return (CORBA::Boolean) return_value;
}

CORBA::Boolean
TAO_Constraint_Evaluator::
sequence_does_contain(CORBA::Any* sequence,
		      TAO_Literal_Constraint& element)
{
  // Helper method to cast the void* value returned from the sequence
  // any into a sequence type locally compiled from idl. The sequence
  // wrapper uses the [] operator to locate the target element in the
  // sequence. 

  CORBA::Environment env;
  CORBA::Boolean return_value = CORBA::B_FALSE;
  CORBA::TypeCode_ptr type = sequence->type ();
  CORBA::TypeCode_ptr content = type->content_type (env);
  TAO_CHECK_ENV_RETURN (env, return_value);
  CORBA::TCKind sequence_type = content->kind (env);
  TAO_CHECK_ENV_RETURN (env, return_value);

  // What's up with this?
  if (sequence_type == CORBA::tk_sequence)
    {
      CORBA::TypeCode_ptr tmp = content->content_type (env);
      TAO_CHECK_ENV_RETURN (env, return_value);
      sequence_type = tmp->kind (env);
      TAO_CHECK_ENV_RETURN (env, return_value);
    }

  switch(sequence_type)
    {
    case CORBA::tk_short:
      {
	TAO_Sequences::ShortSeq* short_seq;
	if ((*sequence) >>= short_seq)
	  return_value = ::TAO_find (*short_seq, (CORBA::Long)element);
      }
    break;
    case CORBA::tk_ushort:
      {
	TAO_Sequences::UShortSeq* ushort_seq;
	if ((*sequence) >>= ushort_seq)
	  return_value = ::TAO_find (*ushort_seq, (CORBA::ULong)element);
      }
      break;
    case CORBA::tk_long:
      {
	TAO_Sequences::LongSeq* long_seq;
	if ((*sequence) >>= long_seq)
	  return_value = ::TAO_find (*long_seq, (CORBA::Long)element);
      }
      break;
    case CORBA::tk_ulong:
      {
	TAO_Sequences::ULongSeq* ulong_seq;
	if ((*sequence) >>= ulong_seq)
	  return_value = ::TAO_find (*ulong_seq, (CORBA::ULong)element);
      }
      break;
    case CORBA::tk_float:
      {
	TAO_Sequences::FloatSeq* float_seq;
	if ((*sequence) >>= float_seq)
	  return_value = ::TAO_find (*float_seq, (CORBA::Double)element);
      }
      break;
    case CORBA::tk_double:
      {
	TAO_Sequences::DoubleSeq* double_seq;
	if ((*sequence) >>= double_seq)
	  return_value = ::TAO_find (*double_seq, (CORBA::Double)element);
      }
      break;
    case CORBA::tk_boolean:
      {
	TAO_Sequences::BooleanSeq* bool_seq;
	if ((*sequence) >>= bool_seq)
	  return_value = ::TAO_find (*bool_seq, (CORBA::ULong)element);
      }
      break;
    case CORBA::tk_string:
      {
	TAO_Sequences::StringSeq* string_seq;
	if ((*sequence) >>= string_seq)
	  return_value = ::TAO_find_string (*string_seq, (const char*) element);
	break;
      }
    }

  return return_value;
}
