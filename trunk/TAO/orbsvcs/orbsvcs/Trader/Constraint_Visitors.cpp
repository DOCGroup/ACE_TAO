// Constraint_Visitors.cpp
// $Id$

// ========================================================================
// 
// = LIBRARY 
//    orbsvcs
//  
// = FILENAME
//    Constraint_Visitors.cpp
// 
// = AUTHOR 
//    Seth Widoff <sbw1@cs.wustl.edu> 
//   
// ========================================================================

#include "Constraint_Visitors.h"
#include "Constraint_Nodes.h"
#include "Trader_T.h"

  // *************************************************************
  // TAO_Constraint_Evaluator
  // *************************************************************

TAO_Constraint_Evaluator::Operand_Queue::Operand_Queue (void)
{
}

TAO_Literal_Constraint&
TAO_Constraint_Evaluator::Operand_Queue::get_left_operand (void)
{
  TAO_Literal_Constraint* left_operand = 0;
  this->get (left_operand, 1);
  return *left_operand;
}

TAO_Literal_Constraint&
TAO_Constraint_Evaluator::Operand_Queue::get_right_operand (void)
{
  TAO_Literal_Constraint* right_operand = 0;
  this->get (right_operand);
  return *right_operand;
}

TAO_Literal_Constraint&
TAO_Constraint_Evaluator::Operand_Queue::get_operand (void)
{
  TAO_Literal_Constraint* operand = 0;
  this->get (operand);
  return *operand;
}

void
TAO_Constraint_Evaluator::Operand_Queue::dequeue_operand (void)
{
  TAO_Literal_Constraint operand;
  this->dequeue_head (operand);
}

TAO_Constraint_Evaluator::
TAO_Constraint_Evaluator (CosTrading::Offer* offer,
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


CORBA::Boolean
TAO_Constraint_Evaluator::evaluate_constraint (TAO_Constraint* root)
{
  CORBA::Boolean result = 0;
  this->queue_.reset ();
   
  // Evaluate the offer according to the constraints in root_;
  if (root != 0)
    {
      if ((root->accept (this) == 0) &&
	  (! this->queue_.is_empty ()))
	{
	  result = (CORBA::Boolean) this->queue_.get_operand();
	  this->queue_.dequeue_operand ();
	}
    }

  // If a property couldn't be evaluated we must return 0.
  return result;
}

int
TAO_Constraint_Evaluator::
evaluate_preference (TAO_Constraint* root,
		    TAO_Literal_Constraint& result)
{
  int return_value = -1;
  while (! this->queue_.is_empty ())
    this->queue_.dequeue_operand ();

  // Evaluate the offer according to the constraints in root_;
  if (root != 0)
    {
      if ((root->accept (this) == 0) &&
	  (! this->queue_.is_empty ()))
	{
	  result = this->queue_.get_operand ();
	  this->queue_.dequeue_operand ();
	  return_value = 0;
	}
    }
  
  return return_value;
}

int
TAO_Constraint_Evaluator::visit_constraint(TAO_Unary_Constraint* constraint)
{
  TAO_Constraint* operand = constraint->operand ();
  return operand->accept (this);
}

int
TAO_Constraint_Evaluator::visit_with (TAO_Unary_Constraint* unary_with)
{
  TAO_Constraint* operand = unary_with->operand ();
  return operand->accept (this);
}

int
TAO_Constraint_Evaluator::visit_min (TAO_Unary_Constraint* unary_min)
{
  TAO_Constraint* operand = unary_min->operand ();
  return operand->accept (this);
}

int
TAO_Constraint_Evaluator::visit_max (TAO_Unary_Constraint* unary_max)
{
  TAO_Constraint* operand = unary_max->operand ();
  return operand->accept (this);
}

int
TAO_Constraint_Evaluator::visit_random (TAO_Noop_Constraint* noop_random)
{
  TAO_Literal_Constraint random ((CORBA::Long) (ACE_OS::rand ()));
  this->queue_.enqueue_head (random);  
  return 0;
}

int
TAO_Constraint_Evaluator::visit_first (TAO_Noop_Constraint* noop_first)
{
  TAO_Literal_Constraint first ((CORBA::Long) 0);
  this->queue_.enqueue_head (first);  
  return 0;
}

int
TAO_Constraint_Evaluator::
visit_and (TAO_Binary_Constraint* boolean_and)
{
  int return_value = -1;
  CORBA::Boolean result = (CORBA::Boolean) 0;
  TAO_Constraint* left = boolean_and->left_operand (),
    *right = boolean_and->right_operand ();

  // Short circuiting AND.
  
  if (left->accept (this) == 0)
    {
      result = (CORBA::Boolean) this->queue_.get_operand ();
      this->queue_.dequeue_operand ();
      
      if (result)
	{
	  if (right->accept(this) == 0)
	    {
	      result = (CORBA::Boolean) this->queue_.get_operand();
	      this->queue_.dequeue_operand ();

	      return_value = 0;
	    }
	}
      else
	return_value = 0;
    }

  if (return_value != -1)
    this->queue_.enqueue_head (TAO_Literal_Constraint (result));
  
  return return_value;
}

int
TAO_Constraint_Evaluator::
visit_or (TAO_Binary_Constraint* boolean_or)
{
  int return_value = -1;
  CORBA::Boolean result = (CORBA::Boolean) 0;
  TAO_Constraint* left = boolean_or->left_operand (),
    *right = boolean_or->right_operand ();

  // Short-circuiting OR.
  
  if (left->accept (this) == 0)
    {
      result = (CORBA::Boolean) this->queue_.get_operand ();
      this->queue_.dequeue_operand ();
      
      if (result == (CORBA::Boolean) 0)
	{
	  if (right->accept (this) == 0)
	    {
	      result = (CORBA::Boolean) this->queue_.get_operand ();
	      this->queue_.dequeue_operand ();
	      return_value = 0;
	    }
	}
      else
	return_value = 0;
    }

  if (return_value != -1)
    this->queue_.enqueue_head (TAO_Literal_Constraint (result));
  
  return return_value;
}

int
TAO_Constraint_Evaluator::
visit_not (TAO_Unary_Constraint* unary_not)
{
  int return_value = -1;
  TAO_Constraint* operand = unary_not->operand ();

  // Logical NOT.
  
  if (operand->accept (this) == 0)
    {
      CORBA::Boolean result = ! (CORBA::Boolean)this->queue_.get_operand ();
      this->queue_.dequeue_operand ();
      this->queue_.enqueue_head (TAO_Literal_Constraint (result));

      return_value = 0;
    }

  return return_value;
}

int
TAO_Constraint_Evaluator::
visit_exist (TAO_Unary_Constraint* unary_exist)
{
  TAO_Property_Constraint* operand =
    (TAO_Property_Constraint*) unary_exist->operand ();
  TAO_String_Hash_Key property_name ((const char*) operand->name ());

  // Determine if a property is defined on this offer.
  
  CORBA::Boolean result =
    (CORBA::Boolean) (this->props_.find (property_name) == 0);
  
  this->queue_.enqueue_head (TAO_Literal_Constraint (result));
  return 0;
}

int
TAO_Constraint_Evaluator::
visit_unary_minus (TAO_Unary_Constraint* unary_minus)
{
  int return_value = -1;
  TAO_Constraint* operand = unary_minus->operand ();

  if (operand->accept (this) == 0)
    {
      TAO_Literal_Constraint result = - this->queue_.get_operand ();
      this->queue_.dequeue_operand ();
      this->queue_.enqueue_head (result);

      return_value = 0;
    }

  return return_value;
}

void
TAO_Constraint_Evaluator::do_the_op (int operation)
{
  TAO_Literal_Constraint& l_op = this->queue_.get_left_operand ();
  TAO_Literal_Constraint& r_op = this->queue_.get_right_operand ();

  // Perform the listed bindary operation on the first two elements on 
  // the stack.
  
  TAO_Literal_Constraint result =
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
    
  this->queue_.dequeue_operand ();
  this->queue_.dequeue_operand ();
  this->queue_.enqueue_head (result);
}

int
TAO_Constraint_Evaluator::visit_bin_op (TAO_Binary_Constraint* op,
                                        int operation)
{
  int return_value = -1;
  TAO_Constraint* left = op->left_operand ();
  TAO_Constraint* right = op->right_operand ();

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
	this->queue_.dequeue_operand ();
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
visit_sub (TAO_Binary_Constraint* boolean_sub)
{
  return this->visit_bin_op (boolean_sub, TAO_MINUS);
}

int
TAO_Constraint_Evaluator::
visit_mult (TAO_Binary_Constraint* boolean_mult)
{
  return this->visit_bin_op (boolean_mult, TAO_MULT);  
}

int
TAO_Constraint_Evaluator::
visit_div (TAO_Binary_Constraint* boolean_div)
{
  return this->visit_bin_op (boolean_div, TAO_DIV);  
}

int
TAO_Constraint_Evaluator::
visit_twiddle (TAO_Binary_Constraint* binary_twiddle)
{
  int return_value = -1;
  TAO_Constraint* left = binary_twiddle->left_operand (),
    *right = binary_twiddle->right_operand ();

  // Determine if the left operand is a subTAO_String_Hash_Key of the right.
  
  if (left->accept (this) == 0)
    {
      if (right->accept (this) == 0)
	{
	  TAO_Literal_Constraint& left_operand = this->queue_.get_left_operand ();
	  TAO_Literal_Constraint& right_operand = this->queue_.get_right_operand ();
	  
	  CORBA::Boolean result = (CORBA::Boolean)
	    (ACE_OS::strstr ((const char*)left_operand,
			     (const char*)right_operand) != 0);
	  
	  this->queue_.dequeue_operand ();
	  this->queue_.dequeue_operand ();
	  this->queue_.enqueue_head (TAO_Literal_Constraint (result));
	  return_value = 0;
	}
      else
	this->queue_.dequeue_operand ();
    }

  return return_value;
}

int
TAO_Constraint_Evaluator::
visit_in(TAO_Binary_Constraint* binary_in)
{
  int return_value = -1;
  TAO_Constraint* left = binary_in->left_operand (),
    *right = binary_in->right_operand ();

  // Determine if the left operand is contained in the right.
  
  if (left->accept (this) == 0)
    {
      if (this->visit_property ((TAO_Property_Constraint*) right) == 0)
	{
	  TAO_Literal_Constraint& left_value = this->queue_.get_left_operand(); 
	  const CORBA::Any* any = (const CORBA::Any*) this->queue_.get_right_operand();

	  if (any != 0)
	    {
	      CORBA::Boolean result =
		this->sequence_does_contain ((CORBA::Any*) any, left_value);
	      
	      this->queue_.dequeue_operand ();
	      this->queue_.dequeue_operand ();
	      this->queue_.enqueue_head (TAO_Literal_Constraint (result));
	      return_value = 0;
	    }
	  else
	    this->queue_.dequeue_operand ();	  
	}
    }

  return return_value;
}

int
TAO_Constraint_Evaluator::
visit_less_than (TAO_Binary_Constraint* boolean_lt)
{
  return this->visit_bin_op (boolean_lt, TAO_LT);
}

int
TAO_Constraint_Evaluator::
visit_less_than_equal (TAO_Binary_Constraint* boolean_lte)
{
  return this->visit_bin_op (boolean_lte, TAO_LE);
}

int
TAO_Constraint_Evaluator::
visit_greater_than (TAO_Binary_Constraint* boolean_gt)
{
  return this->visit_bin_op (boolean_gt, TAO_GT);
}

int
TAO_Constraint_Evaluator::
visit_greater_than_equal (TAO_Binary_Constraint* boolean_gte)
{
  return this->visit_bin_op (boolean_gte, TAO_GE); 
}

int
TAO_Constraint_Evaluator::
visit_equal (TAO_Binary_Constraint* boolean_eq)
{
  return this->visit_bin_op (boolean_eq, TAO_EQ); 
}

int
TAO_Constraint_Evaluator::
visit_not_equal (TAO_Binary_Constraint* boolean_neq)
{
  return this->visit_bin_op (boolean_neq, TAO_NE); 
}

int
TAO_Constraint_Evaluator::
visit_literal (TAO_Literal_Constraint* literal)
{
  this->queue_.enqueue_head (*literal);
  return 0;
}

int
TAO_Constraint_Evaluator::
visit_property (TAO_Property_Constraint* literal)
{
  int return_value = -1, prop_index = 0;
  // Handle case where property is not, in fact, mapped to a value
  TAO_String_Hash_Key prop_name ((const char*) literal->name ());

  if (this->props_.find (prop_name, prop_index) == 0)
    {
      CORBA::Environment env;
      // Retrieve the value of the property from the Property_Evaluator
  
      CORBA::Any* value =
	this->prop_eval_.property_value (prop_index, env);
      TAO_CHECK_ENV_RETURN (env, -1);
      
      if (value != 0)
	{
	  this->queue_.enqueue_head (TAO_Literal_Constraint (value));
	  return_value = 0;
	}
    }

  return return_value;
}


CORBA::Boolean
TAO_find_string (CosTradingSequences::StringSeq& sequence,
                 const char* element)
{
 int length = sequence.length (),
    return_value = 0;

  for (int i = 0; i < length; i++)
    {
      if (ACE_OS::strcmp (sequence[i], element) == 0)
	{	  
	  return_value = 1;
	  break;
	}
    }

  return (CORBA::Boolean) return_value;
}

CORBA::Boolean
TAO_Constraint_Evaluator::
sequence_does_contain (CORBA::Any* sequence,
		      TAO_Literal_Constraint& element)
{
  // Helper method to cast the void* value returned from the sequence
  // any into a sequence type locally compiled from idl. The sequence
  // wrapper uses the [] operator to locate the target element in the
  // sequence. 

  CORBA::Environment env;
  CORBA::Boolean return_value = CORBA::B_FALSE;
  CORBA::TypeCode_ptr type = sequence->type ();
  CORBA::TCKind sequence_type =
    TAO_Sequence_Extracter_Base::sequence_type (type, env);
  TAO_CHECK_ENV_RETURN (env, return_value);

  if (sequence_type == CORBA::tk_void)
    return return_value;
  
  switch (sequence_type)
    {
    case CORBA::tk_short:
      {
	CosTradingSequences::ShortSeq* short_seq;
        TAO_Sequence_Extracter<CosTradingSequences::ShortSeq>
          extracter (CosTradingSequences::_tc_ShortSeq);

        CORBA::Long value = element;
	if (extracter.extract (*sequence, short_seq))
	  return_value = ::TAO_find (*short_seq, value);
      }
    break;
    case CORBA::tk_ushort:
      {
	CosTradingSequences::UShortSeq* ushort_seq;
        TAO_Sequence_Extracter<CosTradingSequences::UShortSeq>
          extracter (CosTradingSequences::_tc_UShortSeq);

        CORBA::ULong value = element;
	if (extracter.extract (*sequence, ushort_seq))
	  return_value = ::TAO_find (*ushort_seq, value);
      }
      break;
    case CORBA::tk_long:
      {
	CosTradingSequences::LongSeq* long_seq;
        TAO_Sequence_Extracter<CosTradingSequences::LongSeq>
          extracter (CosTradingSequences::_tc_LongSeq);

        CORBA::Long value = element;
	if (extracter.extract (*sequence, long_seq))
	  return_value = ::TAO_find (*long_seq, value);
      }
      break;
    case CORBA::tk_ulong:
      {
	CosTradingSequences::ULongSeq* ulong_seq;
        TAO_Sequence_Extracter<CosTradingSequences::ULongSeq>
          extracter (CosTradingSequences::_tc_ULongSeq);

	if (extracter.extract (*sequence, ulong_seq))
	  return_value = ::TAO_find (*ulong_seq, (CORBA::ULong) element);
      }
      break;
    case CORBA::tk_float:
      {
	CosTradingSequences::FloatSeq* float_seq;
        TAO_Sequence_Extracter<CosTradingSequences::FloatSeq>
          extracter (CosTradingSequences::_tc_FloatSeq);

        CORBA::Double value = element;
	if (extracter.extract (*sequence, float_seq))
	  return_value = ::TAO_find (*float_seq, value);
      }
      break;
    case CORBA::tk_double:
      {
	CosTradingSequences::DoubleSeq* double_seq;
        TAO_Sequence_Extracter<CosTradingSequences::DoubleSeq>
          extracter (CosTradingSequences::_tc_DoubleSeq);

        CORBA::Double value = element;
	if (extracter.extract (*sequence, double_seq))
	  return_value = ::TAO_find (*double_seq, value);
      }
      break;
    case CORBA::tk_boolean:
      {
	CosTradingSequences::BooleanSeq* boolean_seq;
        TAO_Sequence_Extracter<CosTradingSequences::BooleanSeq>
          extracter (CosTradingSequences::_tc_BooleanSeq);

        CORBA::Boolean value = element;
	if (extracter.extract (*sequence, boolean_seq))
	  return_value = ::TAO_find (*boolean_seq, value);
      }
      break;
    case CORBA::tk_string:
      {
	CosTradingSequences::StringSeq* string_seq;
        TAO_Sequence_Extracter<CosTradingSequences::StringSeq>
          extracter (CosTradingSequences::_tc_StringSeq);

        const char* value = element;
	if (extracter.extract (*sequence, string_seq))
	  return_value = ::TAO_find_string (*string_seq, value);
      }
      break;
    default:
      break;
    }

  return return_value;
}

  // *************************************************************
  // TAO_Constraint_Evaluator
  // *************************************************************

TAO_Constraint_Validator::
TAO_Constraint_Validator
(const CosTradingRepos::ServiceTypeRepository::TypeStruct& type_struct)
{
  const CosTradingRepos::ServiceTypeRepository::PropStructSeq& prop_seq =
    type_struct.props;
  int length = prop_seq.length ();
  
  // Create a map of the service type properties to their types.
  for (int i = 0; i < length; i++)
    {
      CORBA::TypeCode_ptr corba_type =
        CORBA::TypeCode::_duplicate (prop_seq[i].value_type);

      TAO_String_Hash_Key prop_name_str = (const char*) prop_seq[i].name;
      this->type_map_.bind (prop_name_str, corba_type);
    }
}

TAO_Constraint_Validator::~TAO_Constraint_Validator (void)
{
  for (TAO_Typecode_Table::iterator type_iter (this->type_map_);
       ! type_iter.done ();
       type_iter++)
    {
      CORBA::TypeCode_ptr corba_type = (*type_iter).int_id_;
      CORBA::release (corba_type);
    }
}

int
TAO_Constraint_Validator::validate (TAO_Constraint* root)
{
  return root->accept(this);
}

int
TAO_Constraint_Validator::visit_constraint (TAO_Unary_Constraint* constraint)
{
  int return_value = -1;
  TAO_Expression_Type type;
  TAO_Constraint* operand = constraint->operand ();
  this->extract_type (operand, type);
  
  if (this->expr_returns_boolean (type))
    return_value =  operand->accept (this);

  return return_value;
}

int
TAO_Constraint_Validator::visit_first (TAO_Noop_Constraint* noop)
{
  return 0;
}

int
TAO_Constraint_Validator::visit_random (TAO_Noop_Constraint* noop)
{
  return 0;
}

int
TAO_Constraint_Validator::visit_with (TAO_Unary_Constraint* unary_with)
{
  return this->visit_constraint (unary_with);
}

int
TAO_Constraint_Validator::visit_min (TAO_Unary_Constraint* unary_min)
{
  return this->visit_unary_minus (unary_min);
}

int
TAO_Constraint_Validator::visit_max (TAO_Unary_Constraint* unary_max)
{
  return this->visit_unary_minus (unary_max);
}

int 
TAO_Constraint_Validator::
visit_and (TAO_Binary_Constraint* boolean_and)
{
  int return_value = -1;
  TAO_Constraint* left = boolean_and->left_operand (),
    *right = boolean_and->right_operand ();
  TAO_Expression_Type left_type, right_type;
  this->extract_type (left, left_type);
  this->extract_type (right, right_type);
  
  // Can only "and" expressions that return a boolean value
  if (this->expr_returns_boolean (left_type) &&
      this->expr_returns_boolean (right_type))
    {
      if (left->accept (this) == 0 &&
	  right->accept (this) == 0)
	return_value = 0;
    }

  return return_value;
}

int
TAO_Constraint_Validator::
visit_or(TAO_Binary_Constraint* boolean_or)
{
  // The types for or are the same as those for and.
  return this->visit_and (boolean_or);
}

int
TAO_Constraint_Validator::
visit_not (TAO_Unary_Constraint* unary_not)
{
  int return_value = -1;
  // Not can only negate an expression that returns a boolean.
  TAO_Expression_Type type;
  TAO_Constraint* operand = unary_not->operand ();
  this->extract_type (operand, type);

  if (this->expr_returns_boolean (type))
    return_value = operand->accept (this);

  return return_value;
}

int
TAO_Constraint_Validator::
visit_exist (TAO_Unary_Constraint* unary_exist)
{
  // Exist simply requires that its operand be a property name
  // included in the service type.
  int return_value = -1;
  TAO_Constraint* operand = unary_exist->operand ();
  TAO_Expression_Type type = operand->expr_type ();
  
  if (type == TAO_IDENT)
    return_value = operand->accept (this);

  return return_value;
}

int
TAO_Constraint_Validator::
visit_unary_minus (TAO_Unary_Constraint* unary_minus)
{
  // Unary minus can only negate a numeric operand.
  int return_value = -1;
  TAO_Expression_Type type;
  TAO_Constraint* operand = unary_minus->operand ();
  this->extract_type (operand, type);
    
  if (this->expr_returns_number (type))
    return_value = operand->accept (this);

  return return_value;
}

int
TAO_Constraint_Validator::
visit_add (TAO_Binary_Constraint* boolean_add)
{
  // All the mathematical operators require numeric operands.
  int return_value = -1;
  TAO_Constraint* left = boolean_add->left_operand (),
    *right = boolean_add->right_operand ();
  TAO_Expression_Type left_type, right_type;
  this->extract_type (left, left_type);
  this->extract_type (right, right_type);

  if (this->expr_returns_number (left_type) &&
      this->expr_returns_number (right_type))
    {
      if (left->accept (this) == 0 &&
	  right->accept (this) == 0)
	return_value = 0;
    }

  return return_value;
}

int
TAO_Constraint_Validator::
visit_sub (TAO_Binary_Constraint* boolean_sub)
{
  return this->visit_add (boolean_sub);
}

int
TAO_Constraint_Validator::
visit_mult (TAO_Binary_Constraint* boolean_mult)
{
  return this->visit_add (boolean_mult);
}

int
TAO_Constraint_Validator::
visit_div (TAO_Binary_Constraint* boolean_div)
{
  // Div not only requires that both of its operands be numeric, but
  // also the the demoninator not be zero. However, since the
  // denominator can be an expression who's value can only be
  // determined while evaulating the constraint expression, this
  // method detects only when the demoniator is a literal whose value
  // is zero.
  int return_value = -1;
  TAO_Constraint* left = boolean_div->left_operand (),
    *right = boolean_div->right_operand ();
  TAO_Expression_Type left_type, right_type;
  this->extract_type (left, left_type);
  this->extract_type (right, right_type);

  if (this->expr_returns_number (left_type) &&
      this->expr_returns_number (right_type))
    {
      // Prevent division by zero, a no no.
      int right_isnt_zero = 1;
      switch(right->expr_type ())
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
	    ((CORBA::Double) (*((TAO_Literal_Constraint*) right)) != 0.0);
	  break;
	}

      if (right_isnt_zero)
	{
	  if (left->accept (this) == 0 &&
	      right->accept (this) == 0)
	    return_value = 0;
	}
    }

  return return_value;
}

int
TAO_Constraint_Validator::
visit_twiddle (TAO_Binary_Constraint* binary_twiddle)
{
  // Twiddle requires that both of its operand be strings.
  int return_value = -1;
  TAO_Constraint* left = binary_twiddle->left_operand (),
    *right = binary_twiddle->right_operand ();
  TAO_Expression_Type left_type, right_type;
  this->extract_type (left, left_type);
  this->extract_type (right, right_type);
  
  if  (this->expr_returns_string (left_type) &&
      this->expr_returns_string (right_type))
    {
      if (left->accept (this) == 0 &&
	  right->accept (this) == 0)
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
  TAO_Expression_Type left_type, right_type;
  TAO_Constraint
    *left = binary_in->left_operand (),
    *right = binary_in->right_operand ();
  
  CORBA::TypeCode* prop_type = this->extract_type (right, right_type);
  this->extract_type (left, left_type);

  if (right_type == TAO_SEQUENCE)
    {
      CORBA::Environment env;
      CORBA::Boolean types_match = CORBA::B_FALSE;
      CORBA::TCKind seq_type =
        TAO_Sequence_Extracter_Base::sequence_type (prop_type, env);
      TAO_CHECK_ENV_RETURN (env, return_value);
      
      if (seq_type != CORBA::tk_void)
        {      
          if (this->expr_returns_number (left_type))
            {
              types_match = (seq_type == CORBA::tk_short ||
                             seq_type == CORBA::tk_ushort ||
                             seq_type == CORBA::tk_long ||
                             seq_type == CORBA::tk_ulong ||
                             seq_type == CORBA::tk_float ||
                             seq_type == CORBA::tk_double);
            }
          else if (this->expr_returns_boolean (left_type))
            types_match = (seq_type == CORBA::tk_boolean);
          else if (this->expr_returns_string (left_type))
            types_match = (seq_type == CORBA::tk_string);
          
          if (types_match)
            return_value = left->accept (this);
        }
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
  TAO_Constraint* left = boolean_lt->left_operand (),
    *right = boolean_lt->right_operand ();
  TAO_Expression_Type left_type, right_type;
  this->extract_type (left, left_type);
  this->extract_type (right, right_type);

  if ((this->expr_returns_number (left_type) &&
       this->expr_returns_number (right_type)) ||
      (this->expr_returns_string (left_type) &&
       this->expr_returns_string (right_type)))
    {
      if (left->accept (this) == 0 &&
	  right->accept (this) == 0)
	return_value = 0;
    }

  return return_value;
}

int
TAO_Constraint_Validator::
visit_less_than_equal (TAO_Binary_Constraint* boolean_lte)
{
  return this->visit_less_than (boolean_lte);
}

int
TAO_Constraint_Validator::
visit_greater_than (TAO_Binary_Constraint* boolean_gt)
{
  return this->visit_less_than (boolean_gt);
}

int
TAO_Constraint_Validator::
visit_greater_than_equal (TAO_Binary_Constraint* boolean_gte)
{
  return this->visit_less_than(boolean_gte);
}

int
TAO_Constraint_Validator::
visit_equal (TAO_Binary_Constraint* boolean_eq)
{
  int return_value = -1;
  TAO_Constraint* left = boolean_eq->left_operand (),
    *right = boolean_eq->right_operand ();
  TAO_Expression_Type left_type, right_type;
  this->extract_type (left, left_type);
  this->extract_type (right, right_type);

  if ((this->expr_returns_number (left_type) &&
       this->expr_returns_number (right_type)) ||
      (this->expr_returns_string (left_type) &&
       this->expr_returns_string (right_type)) ||
      (this->expr_returns_boolean (left_type) &&
       this->expr_returns_boolean (right_type)))
    {
      if (left->accept (this) == 0 &&
	  right->accept (this) == 0)
	return_value = 0;
    }

  return return_value;
}

int
TAO_Constraint_Validator::
visit_not_equal (TAO_Binary_Constraint* boolean_neq)
{
  return this->visit_equal (boolean_neq);
}

int
TAO_Constraint_Validator::
visit_literal (TAO_Literal_Constraint* literal)
{
  return 0;
}

int
TAO_Constraint_Validator::
visit_property (TAO_Property_Constraint* literal)
{
  return 0;
}

CORBA::TypeCode*
TAO_Constraint_Validator::extract_type (TAO_Constraint* expr,
					TAO_Expression_Type& type)
{
  CORBA::TypeCode* return_value = 0;

  type = expr->expr_type ();
  if (type == TAO_IDENT)
    {
      TAO_Property_Constraint* prop = (TAO_Property_Constraint*) expr;
      TAO_String_Hash_Key prop_name (prop->name ());

      if (this->type_map_.find (prop_name, return_value) == 0)
	type = TAO_Literal_Constraint::comparable_type (return_value);
    }

  return return_value;
}

int
TAO_Constraint_Validator::expr_returns_boolean (TAO_Expression_Type expr_type)
{
  // If the expression is a boolean operations, a boolean literal, or
  // a boolean property, return 1.
  int return_value = 0;
  
  if (expr_type <= TAO_BOOLEAN)
    return_value = 1;
  
  return return_value;
}


int
TAO_Constraint_Validator::expr_returns_number (TAO_Expression_Type expr_type)
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
TAO_Constraint_Validator::expr_returns_string (TAO_Expression_Type expr_type)
{
  // If the expression is an operation with a string return value, a
  // string literal, or a property whose type is string, return 1.
  int return_value = 0;

  if (expr_type == TAO_STRING)
    return_value = 1;

  return return_value;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Node<TAO_Literal_Constraint>;
template class ACE_Unbounded_Queue<TAO_Literal_Constraint>;
template class ACE_Unbounded_Queue_Iterator<TAO_Literal_Constraint>;
template class TAO_Sequence_Extracter<CosTradingSequences::ShortSeq>;
template class TAO_Sequence_Extracter<CosTradingSequences::UShortSeq>;
template class TAO_Sequence_Extracter<CosTradingSequences::LongSeq>;
template class TAO_Sequence_Extracter<CosTradingSequences::ULongSeq>;
template class TAO_Sequence_Extracter<CosTradingSequences::FloatSeq>;
template class TAO_Sequence_Extracter<CosTradingSequences::DoubleSeq>;
template class TAO_Sequence_Extracter<CosTradingSequences::BooleanSeq>;
template class TAO_Sequence_Extracter<CosTradingSequences::StringSeq>;
template CORBA::Boolean TAO_find (const CosTradingSequences::DoubleSeq&, const CORBA::Double);
template CORBA::Boolean TAO_find (const CosTradingSequences::FloatSeq&, const CORBA::Double);
template CORBA::Boolean TAO_find (const CosTradingSequences::BooleanSeq&, const CORBA::Boolean);
template CORBA::Boolean TAO_find (const CosTradingSequences::ULongSeq&, const CORBA::ULong);
template CORBA::Boolean TAO_find (const CosTradingSequences::LongSeq&, const CORBA::Long);
template CORBA::Boolean TAO_find (const CosTradingSequences::ShortSeq&, const CORBA::Long);
template CORBA::Boolean TAO_find (const CosTradingSequences::UShortSeq&, const CORBA::ULong);
template CORBA::Boolean TAO_find (const CosTradingSequences::StringSeq&, const char*);
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Node<TAO_Literal_Constraint>
#pragma instantiate ACE_Unbounded_Queue<TAO_Literal_Constraint>
#pragma instantiate ACE_Unbounded_Queue_Iterator<TAO_Literal_Constraint>
#pragma instantiate TAO_Sequence_Extracter<CosTradingSequences::ShortSeq>
#pragma instantiate TAO_Sequence_Extracter<CosTradingSequences::UShortSeq>
#pragma instantiate TAO_Sequence_Extracter<CosTradingSequences::LongSeq>
#pragma instantiate TAO_Sequence_Extracter<CosTradingSequences::ULongSeq>
#pragma instantiate TAO_Sequence_Extracter<CosTradingSequences::FloatSeq>
#pragma instantiate TAO_Sequence_Extracter<CosTradingSequences::DoubleSeq>
#pragma instantiate TAO_Sequence_Extracter<CosTradingSequences::BooleanSeq>
#pragma instantiate TAO_Sequence_Extracter<CosTradingSequences::StringSeq>
#pragma instantiate CORBA::Boolean TAO_find (const CosTradingSequences::DoubleSeq&, const CORBA::Double)
#pragma instantiate CORBA::Boolean TAO_find (const CosTradingSequences::FloatSeq&, const CORBA::Double)
#pragma instantiate CORBA::Boolean TAO_find (const CosTradingSequences::BooleanSeq&, const CORBA::Boolean)
#pragma instantiate CORBA::Boolean TAO_find (const CosTradingSequences::ULongSeq&, const CORBA::ULong)
#pragma instantiate CORBA::Boolean TAO_find (const CosTradingSequences::LongSeq&, const CORBA::Long)
#pragma instantiate CORBA::Boolean TAO_find (const CosTradingSequences::ShortSeq&, const CORBA::Long)
#pragma instantiate CORBA::Boolean TAO_find (const CosTradingSequences::UShortSeq&, const CORBA::ULong)
#pragma instantiate CORBA::Boolean TAO_find (const CosTradingSequences::StringSeq&, const char*)
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
