// $Id$

#include "orbsvcs/Trader/Constraint_Visitors.h"
#include "orbsvcs/Trader/Constraint_Nodes.h"
#include "orbsvcs/Trader/Interpreter_Utils_T.h"
#include "orbsvcs/Trader/Constraint_Tokens.h"

#include "tao/DynamicAny/DynSequence_i.h"

#include "ace/OS_NS_string.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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

TAO_Constraint_Evaluator::TAO_Constraint_Evaluator (void)
{
  // No-Op.
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
TAO_Constraint_Evaluator::visit_random (TAO_Noop_Constraint *)
{
  TAO_Literal_Constraint random (static_cast<CORBA::LongLong> (ACE_OS::rand ()));
  this->queue_.enqueue_head (random);
  return 0;
}

int
TAO_Constraint_Evaluator::visit_first (TAO_Noop_Constraint *)
{
  TAO_Literal_Constraint first (static_cast<CORBA::LongLong> (0));
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
  CORBA::String_var property_name ((const char*) operand->name ());

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

  // Determine if the left operand is a substring of the right.

  if (left->accept (this) == 0)
    {
      if (right->accept (this) == 0)
        {
          TAO_Literal_Constraint& left_operand = this->queue_.get_left_operand ();
          TAO_Literal_Constraint& right_operand = this->queue_.get_right_operand ();

          CORBA::Boolean result = (CORBA::Boolean)
            (ACE_OS::strstr ((const char*)right_operand,
                             (const char*)left_operand) != 0);

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
  ACE_UNUSED_ARG (literal);
  return -1;
  // A derived class decides how to do this.
}

/*@@ pradeep: where is this used?

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
*/

CORBA::Boolean
TAO_Constraint_Evaluator::
sequence_does_contain (CORBA::Any* sequence,
                      TAO_Literal_Constraint& element)
{
  // Helper method to cast the void* value returned from the sequence
  // any into a sequence type locally compiled from idl. The sequence
  // wrapper uses the [] operator to locate the target element in the
  // sequence.

  CORBA::Boolean return_value = 0;
  CORBA::TypeCode_var type = sequence->type ();
  CORBA::TCKind sequence_type = CORBA::tk_void;
  try
    {
      sequence_type =
        TAO_Sequence_Extracter_Base::sequence_type (type.in ());
    }
  catch (const CORBA::Exception&)
    {
      return return_value;
    }

  if (sequence_type == CORBA::tk_void)
    return return_value;

  switch (sequence_type)
    {
    case CORBA::tk_short:
      {
        CORBA::LongLong value = element;
        return_value = ::TAO_find (*sequence, static_cast<CORBA::Short> (value));
      }
    break;
    case CORBA::tk_ushort:
      {
        CORBA::ULongLong value = element;
        return_value = ::TAO_find (*sequence, static_cast<CORBA::UShort> (value));
      }
      break;
    case CORBA::tk_long:
      {
        CORBA::LongLong value = element;
        return_value = ::TAO_find (*sequence, static_cast<CORBA::Long> (value));
      }
      break;
    case CORBA::tk_ulong:
      {
        CORBA::ULongLong value = element;
        return_value = ::TAO_find (*sequence, static_cast<CORBA::ULong> (value));
      }
      break;
    case CORBA::tk_longlong:
      {
        CORBA::LongLong value = element;
        return_value = ::TAO_find (*sequence, value);
      }
      break;
    case CORBA::tk_ulonglong:
      {
        CORBA::ULongLong value = element;
        return_value = ::TAO_find (*sequence, value);
      }
      break;
    case CORBA::tk_float:
      {
        CORBA::Double value = element;
        return_value = ::TAO_find (*sequence, static_cast<CORBA::Float> (value));
      }
      break;
    case CORBA::tk_double:
      {
        CORBA::Double value = element;
        return_value = ::TAO_find (*sequence, value);
      }
      break;
    case CORBA::tk_boolean:
      {
        CORBA::Boolean value = element;
        return_value = ::TAO_find (*sequence, value);
      }
      break;
    case CORBA::tk_string:
      {
        const char* value = element;
        return_value = ::TAO_find (*sequence, value);
      }
      break;
    default:
      break;
    }

  return return_value;
}

// Explicit template specializations

int
TAO_Element_Equal<CORBA::Short>::
operator () (TAO_DynSequence_i& dyn_any,
             CORBA::Short element) const
{
  int return_value = 0;
  try
    {
      CORBA::Short value = dyn_any.get_short ();
      return_value = (value == element);
    }
  catch (const CORBA::Exception&)
    {
    }
  return return_value;
}

int
TAO_Element_Equal<CORBA::UShort>::
operator () (TAO_DynSequence_i& dyn_any,
             CORBA::UShort element) const
{
  int return_value = 0;
  try
    {
      CORBA::UShort value = dyn_any.get_ushort ();
      return_value = (value == element);
    }
  catch (const CORBA::Exception&)
    {
    }
  return return_value;
}

int
TAO_Element_Equal<CORBA::Long>::
operator () (TAO_DynSequence_i& dyn_any,
             CORBA::Long element) const
{
  int return_value = 0;
  try
    {
      CORBA::Long value = dyn_any.get_long ();
      return_value = (value == element);
    }
  catch (const CORBA::Exception&)
    {
    }
  return return_value;
}

int
TAO_Element_Equal<CORBA::ULong>::
operator () (TAO_DynSequence_i& dyn_any,
             CORBA::ULong element) const
{
  int return_value = 0;
  try
    {
      CORBA::ULong value = dyn_any.get_ulong ();
      return_value = (value == element);
    }
  catch (const CORBA::Exception&)
    {
    }
  return return_value;
}

int
TAO_Element_Equal<CORBA::LongLong>::
operator () (TAO_DynSequence_i& dyn_any,
             CORBA::LongLong element) const
{
  int return_value = 0;
  try
    {
      CORBA::LongLong value = dyn_any.get_longlong ();
      return_value = (value == element);
    }
  catch (const CORBA::Exception&)
    {
    }
  return return_value;
}

int
TAO_Element_Equal<CORBA::ULongLong>::
operator () (TAO_DynSequence_i& dyn_any,
             CORBA::ULongLong element) const
{
  int return_value = 0;
  try
    {
      CORBA::ULongLong value = dyn_any.get_ulonglong ();
      return_value = (value == element);
    }
  catch (const CORBA::Exception&)
    {
    }
  return return_value;
}

int
TAO_Element_Equal<CORBA::Float>::
operator () (TAO_DynSequence_i& dyn_any,
             CORBA::Float element) const
{
  int return_value = 0;
  try
    {
      CORBA::Float value = dyn_any.get_float ();
      return_value = ACE::is_equal (value, element);
    }
  catch (const CORBA::Exception&){}
  return return_value;
}

int
TAO_Element_Equal<CORBA::Double>::
operator () (TAO_DynSequence_i& dyn_any,
             CORBA::Double element) const
{
  int return_value = 0;
  try
    {
      CORBA::Double value = dyn_any.get_double ();
      return_value = (value == element);
    }
  catch (const CORBA::Exception&){}
  return return_value;
}

int
TAO_Element_Equal<CORBA::Boolean>::
operator () (TAO_DynSequence_i& dyn_any,
             CORBA::Boolean element) const
{
  int return_value = 0;
  try
    {
    CORBA::Boolean value = (CORBA::Boolean) dyn_any.get_short ();
      return_value = (value == element);
    }
  catch (const CORBA::Exception&){}
  return return_value;
}

int
TAO_Element_Equal<const char*>::
operator () (TAO_DynSequence_i& dyn_any,
             const char* element) const
{
  int return_value = 0;
  try
    {
      const char* value = dyn_any.get_string ();
      return_value = (ACE_OS::strcmp (value, element) == 0);
    }
  catch (const CORBA::Exception&){}
  return return_value;
}

TAO_Constraint_Validator::
TAO_Constraint_Validator (void)
{
  // No-Op.
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
TAO_Constraint_Validator::visit_first (TAO_Noop_Constraint *)
{
  return 0;
}

int
TAO_Constraint_Validator::visit_random (TAO_Noop_Constraint *)
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
            (static_cast<CORBA::ULongLong>
               (*dynamic_cast<TAO_Literal_Constraint*> (right)) != 0);
          break;
        case TAO_SIGNED:
          right_isnt_zero =
            (static_cast<CORBA::LongLong>
               (*dynamic_cast<TAO_Literal_Constraint*> (right)) != 0);
          break;
        case TAO_DOUBLE:
          right_isnt_zero =
            (static_cast<CORBA::Double>
               (*dynamic_cast<TAO_Literal_Constraint*> (right)) != 0.0);
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
      CORBA::Boolean types_match = 0;
      CORBA::TCKind seq_type = CORBA::tk_void;
      try
        {
          seq_type =
            TAO_Sequence_Extracter_Base::sequence_type (prop_type);
        }
      catch (const CORBA::Exception&)
        {
          return return_value;
        }

      if (seq_type != CORBA::tk_void)
        {
          if (this->expr_returns_number (left_type))
            {
              types_match = (seq_type == CORBA::tk_short ||
                             seq_type == CORBA::tk_ushort ||
                             seq_type == CORBA::tk_long ||
                             seq_type == CORBA::tk_ulong ||
                             seq_type == CORBA::tk_longlong ||
                             seq_type == CORBA::tk_ulonglong ||
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
visit_literal (TAO_Literal_Constraint *)
{
  return 0;
}

int
TAO_Constraint_Validator::
visit_property (TAO_Property_Constraint *)
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
      CORBA::String_var prop_name (prop->name ());

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
      (expr_type >= TAO_SIGNED && expr_type <= TAO_DOUBLE))
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

TAO_END_VERSIONED_NAMESPACE_DECL
