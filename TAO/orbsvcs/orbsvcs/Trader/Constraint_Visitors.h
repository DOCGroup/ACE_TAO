/* -*- C++ -*- */

// ========================================================================
// $Id$
//
// = LIBRARY
//    orbsvcs
//
// = FILENAME
//    Constraint_Visitor.h
//
// = AUTHOR
//    Seth Widoff <sbw1@cs.wustl.edu>
//
// ========================================================================

#ifndef TAO_CONSTRAINT_VISITOR_H
#define TAO_CONSTRAINT_VISITOR_H

#include "Trader.h"

  // *************************************************************
  // TAO_Constraint_Visitor
  // *************************************************************

class TAO_Constraint;
class TAO_Unary_Constraint;
class TAO_Binary_Constraint;
class TAO_Literal_Constraint;
class TAO_Property_Constraint;
class TAO_Noop_Constraint;

class TAO_Constraint_Visitor
//
// = TITLE
//     This is the base class for all visitors who wish to preform
//     some operation from the state of the expression tree. Using
//     double dispatching, subclasses of Constraint expression call
//     back to the InterpreterVisitor subclass from the accept method.
//
// = DESCRIPTION
//     Traversal of the expression tree uses the "Visitor" pattern. To
//     "visit" a node, a client invokes the "accept" method on a
//     subclass of ConstraintExpression, which, in turn, invokes the
//     appropriate method on the visitor passed to it, based on its
//     own type. So, the Constraint_Visitor has a method to deal with
//     each possible type of node in an expression tree; one for each
//     operator in the grammar.
{
public:

  virtual ~TAO_Constraint_Visitor (void) {}

  virtual int visit_constraint (TAO_Unary_Constraint* constraint) = 0;

  virtual int visit_with (TAO_Unary_Constraint* unary_with) = 0;
  virtual int visit_min (TAO_Unary_Constraint* unary_min) = 0;
  virtual int visit_max (TAO_Unary_Constraint* unary_max) = 0;
  virtual int visit_first (TAO_Noop_Constraint* noop_first) = 0;
  virtual int visit_random (TAO_Noop_Constraint* noop_random) = 0;

  virtual int visit_and (TAO_Binary_Constraint* boolean_and) = 0;
  virtual int visit_or (TAO_Binary_Constraint* boolean_or) = 0;
  virtual int visit_not (TAO_Unary_Constraint* unary_not) = 0;

  virtual int visit_exist (TAO_Unary_Constraint* unary_exist) = 0;
  virtual int visit_unary_minus (TAO_Unary_Constraint* unary_minus) = 0;

  virtual int visit_add (TAO_Binary_Constraint* boolean_add) = 0;
  virtual int visit_sub (TAO_Binary_Constraint* boolean_sub) = 0;
  virtual int visit_mult (TAO_Binary_Constraint* boolean_mult) = 0;
  virtual int visit_div (TAO_Binary_Constraint* boolean_div) = 0;

  virtual int visit_twiddle (TAO_Binary_Constraint* binary_twiddle) = 0;
  virtual int visit_in (TAO_Binary_Constraint* binary_in) = 0;

  virtual int visit_less_than (TAO_Binary_Constraint* boolean_lt) = 0;
  virtual int visit_less_than_equal (TAO_Binary_Constraint* boolean_lte) = 0;
  virtual int visit_greater_than (TAO_Binary_Constraint* boolean_gt) = 0;
  virtual int visit_greater_than_equal (TAO_Binary_Constraint* boolean_gte) = 0;
  virtual int visit_equal (TAO_Binary_Constraint* boolean_eq) = 0;
  virtual int visit_not_equal (TAO_Binary_Constraint* boolean_neq) = 0;

  virtual int visit_literal (TAO_Literal_Constraint* literal) = 0;
  virtual int visit_property (TAO_Property_Constraint* literal) = 0;
};

  // *************************************************************
  // TAO_Constraint_Validator
  // *************************************************************

#include "Constraint_Nodes.h"

class TAO_Constraint_Validator : public TAO_Constraint_Visitor
  //
  // = TITLE
  //     TAO_Constraint_Validator ensures that in an expression tree
  //     passed to it, the operands of each operation match the
  //     correct types.
  //
  // = DESCRIPTION
  //    TAO_Constraint_Validator uses the visitor pattern to
  //    traverse all the nodes in an expression tree, checking that
  //    for each operator node the operands are of the proper data
  //    type it they're literals, or that they exist in the service
  //    type definition _and_ have the proper type, if they're
  //    property names. The algorithm for type
  //    checking is as follows: ensure that operand expression(s)
  //    return the correct types using expr_returns* methods. If they
  //    (or it) return the correct types, call accept
  //    on each operand until all return true or one returns false,
  //    at which point we can back out of the traversal and indicate
  //    failure.
{
public:

  TAO_Constraint_Validator
    (const CosTradingRepos::ServiceTypeRepository::TypeStruct& type_struct);
  // The constructor creates a map of property names to their values
  // from the Type Description retrieved from the
  // ServiceTypeRepository. The ServiceTypeRepository throws
  // exceptions when it's displeased with the type name provided to
  // it. The map allows O(lg n) associative access, rather than the
  // O(n) sequential lookup from the CORBA data structures.

  virtual ~TAO_Constraint_Validator (void);
  // Desctructor.

  int validate (TAO_Constraint* root);
  // Validate returns 1 if the expression tree whose root is <root>
  // makes semantic sense, in that the operands for each operation
  // comply with each other and the types accepted by the operator.

  // = Visitor Methods

  virtual int visit_constraint (TAO_Unary_Constraint* constraint);

  virtual int visit_with (TAO_Unary_Constraint* unary_with);
  virtual int visit_min (TAO_Unary_Constraint* unary_min);
  virtual int visit_max (TAO_Unary_Constraint* unary_max);
  virtual int visit_first (TAO_Noop_Constraint* noop_first);
  virtual int visit_random (TAO_Noop_Constraint* noop_random);

  virtual int visit_and (TAO_Binary_Constraint* boolean_and);
  virtual int visit_or (TAO_Binary_Constraint* boolean_or);
  // The two operands must return a boolean value.

  virtual int visit_not (TAO_Unary_Constraint* unary_not);
  // The operand must return a boolean value.

  virtual int visit_exist (TAO_Unary_Constraint* unary_exist);
  // The operand must return a valid  (i.e., present in the service
  // type description) property name.

  virtual int visit_unary_minus (TAO_Unary_Constraint* unary_minus);
  // The operand must return a number to be negated.

  virtual int visit_add (TAO_Binary_Constraint* boolean_add);
  virtual int visit_sub (TAO_Binary_Constraint* boolean_sub);
  virtual int visit_mult (TAO_Binary_Constraint* boolean_mult);
  virtual int visit_div (TAO_Binary_Constraint* boolean_div);
  // Both operands must return numeric results.

  virtual int visit_twiddle (TAO_Binary_Constraint* binary_twiddle);
  // Both operands must return strings.

  virtual int visit_in (TAO_Binary_Constraint* binary_in);
  // The right operand must be a sequence of the same simple type as
  // the left operand.

  virtual int visit_less_than (TAO_Binary_Constraint* boolean_lt);
  virtual int visit_less_than_equal (TAO_Binary_Constraint* boolean_lte);
  virtual int visit_greater_than (TAO_Binary_Constraint* boolean_gt);
  virtual int visit_greater_than_equal (TAO_Binary_Constraint* boolean_gte);
  virtual int visit_equal (TAO_Binary_Constraint* boolean_eq);
  virtual int visit_not_equal (TAO_Binary_Constraint* boolean_neq);
  // The left and right operands must both be of the same simple type.

  virtual int visit_literal (TAO_Literal_Constraint* literal);
  virtual int visit_property (TAO_Property_Constraint* literal);
  // The property must be defined in the service type description.

private:

  TAO_Typecode_Table type_map_;
  // A map gleaned from the ServiceTypeStruct, which correlates
  // property names with their types.

  CORBA::TypeCode* extract_type (TAO_Constraint* expr_type,
                                TAO_Expression_Type& type);

  int expr_returns_boolean (TAO_Expression_Type expr_type);
  // expr_returns_boolean returns 1 if <expr_type>, when evaluated, will
  // return a boolean.

  int expr_returns_number (TAO_Expression_Type expr_type);
  // expr_returns_boolean returns 1 if <expr_type>, when evaluated, will
  // return a number.

  int expr_returns_string (TAO_Expression_Type expr_type);
  // expr_returns_boolean returns 1 if <expr_type>, when evaluated, will
  // return a string.

  TAO_Constraint_Validator (const TAO_Constraint_Validator&);
  TAO_Constraint_Validator& operator= (const TAO_Constraint_Validator&);
};

  // *************************************************************
  // TAO_Constraint_Evaluator
  // *************************************************************

#include "Trader_Utils.h"

class TAO_Constraint_Evaluator : public TAO_Constraint_Visitor
//
// = TITLE
//     TAO_Constraint_Evaluator traverse a constraint expression tree,
//     and determines whether an offer fits the constraints
//     represented by the tree
//
// = DESCRIPTION
//     Using the Visitor pattern, the TAO_Constraint_Evaluator has each
//     node of the expression tree call back to it with the method
//     designated for its type. In that method, the visitor will
//     evaluate its operands and perform the operation designated by
//     that node's type, and return the result. Note: the
//     TAO_Constraint_Evaluator assumes the tree is semantically correct,
//     that is, the validate method on TAO_Constraint_Validator return
//     true. The only possible evaluation time errors are a divide by a
//     property whose value is zero and undefined properties.
{
public:

  TAO_Constraint_Evaluator (CosTrading::Offer* offer,
                           CORBA::Boolean supports_dynamic_properties = 1);

  CORBA::Boolean evaluate_constraint (TAO_Constraint* root);
  // Evaluate returns 1 if the offer satisfies the constraints
  // represented by the the expression tree rooted at <root>, 0 if it
  // doesn't. If an error occurs during process, the constraint
  // automatically fails.

  int evaluate_preference (TAO_Constraint* root,
                          TAO_Literal_Constraint& result);
  // The result of the preference evaluation is stored in result. The
  // method returns 0 upon success, -1 upon failure.

  // = Visitor Methods

  virtual int visit_constraint (TAO_Unary_Constraint* constraint);

  virtual int visit_with (TAO_Unary_Constraint* unary_with);
  virtual int visit_min (TAO_Unary_Constraint* unary_min);
  virtual int visit_max (TAO_Unary_Constraint* unary_max);
  virtual int visit_first (TAO_Noop_Constraint* noop_first);
  virtual int visit_random (TAO_Noop_Constraint* noop_random);

  virtual int visit_and (TAO_Binary_Constraint* boolean_and);
  // Takes the logical and of the results of both operands. Note that
  // in the case where the left operand returns zero, the result is
  // immediately known.

  virtual int visit_or (TAO_Binary_Constraint* boolean_or);
  // Takes the logical or of the results of both operands. Note that
  // in the case where the left operand returns one, the result is
  // immediately known.

  virtual int visit_not (TAO_Unary_Constraint* unary_not);
  // Logically negates the value of the operand.

  virtual int visit_exist (TAO_Unary_Constraint* unary_exist);
  // The property exists if its name is bound to a value in the
  // <props_> map.

  virtual int visit_unary_minus (TAO_Unary_Constraint* unary_minus);
  // Mathematically negates the return value the operand.

  virtual int visit_add (TAO_Binary_Constraint* boolean_add);
  // Add the results of evaluating the left and right operands.

  virtual int visit_sub (TAO_Binary_Constraint* boolean_sub);
  // Subtract the results of evaluating the left and right operands.

  virtual int visit_mult (TAO_Binary_Constraint* boolean_mult);
  // Multiply the results of evaluating the left and right operands.

  virtual int visit_div (TAO_Binary_Constraint* boolean_div);
  // Divide the results of evaluating the left and right operands.

  virtual int visit_twiddle (TAO_Binary_Constraint* binary_twiddle);
  // Determines if the right operand is a substring of the left.

  virtual int visit_in (TAO_Binary_Constraint* binary_in);
  // Determines if the sequence represented by the right operand
  // contains the left operand.

  // = Compare the results of evaluating left and right operands.
  virtual int visit_less_than (TAO_Binary_Constraint* boolean_lt);
  virtual int visit_less_than_equal (TAO_Binary_Constraint* boolean_lte);
  virtual int visit_greater_than (TAO_Binary_Constraint* boolean_gt);
  virtual int visit_greater_than_equal (TAO_Binary_Constraint* boolean_gte);
  virtual int visit_equal (TAO_Binary_Constraint* boolean_eq);
  virtual int visit_not_equal (TAO_Binary_Constraint* boolean_neq);

  virtual int visit_literal (TAO_Literal_Constraint* literal);
  // Copy the value of the literal into the result container.

  virtual int visit_property (TAO_Property_Constraint* literal);
  // Copy the value of the property into the result container.

private:

  class Operand_Queue :
  public ACE_Unbounded_Queue <TAO_Literal_Constraint>
    // = TITLE
    // A queue adapter with methods to setting and getting operands
    // from the expression evaluation results.
  {
  public:

    Operand_Queue  (void);

    TAO_Literal_Constraint& get_left_operand (void);
    // In a binary operation, obtain the left operand.

    TAO_Literal_Constraint& get_right_operand (void);
    // In a binary operation, obtain the right operand.

    TAO_Literal_Constraint& get_operand (void);
    // In a unary operation, obtain the only operand.

    void dequeue_operand (void);
    // Remove an operand from the queue.
  };

  void do_the_op (int operation);
  // Method for performing a arithmetic or comparison operation.

  int visit_bin_op (TAO_Binary_Constraint* op, int operation);
  // Method for evaluating a binary operation.

  CORBA::Boolean sequence_does_contain (CORBA::Any* sequence,
                                        TAO_Literal_Constraint& element);
  // Determine if sequence contains <element>, a literal of the same
  // simple type as <sequence_type>. Return 1 in this eventuality.

  TAO_Constraint_Evaluator (const TAO_Constraint_Evaluator&);
  TAO_Constraint_Evaluator& operator= (const TAO_Constraint_Evaluator&);
  // Disallow copying.

  TAO_Lookup_Table props_;
  // The map of property names to their values for a property.

  TAO_Property_Evaluator prop_eval_;
  // Utility with which to evaluate the properties of an offer, be
  // they dyanmic or static.

  Operand_Queue queue_;
  // The result of a non_boolean operation.
};

  // *************************************************************
  // Utilities
  // *************************************************************

#include "tao/DynSequence_i.h"

// Forward declaration
template <class ELEMENT_TYPE> class TAO_Element_Equal;

class TAO_Element_Equal<CORBA::Short>
{
public:
  int operator () (TAO_DynSequence_i& dyn_any,
                   CORBA::Short element) const;
  // Calls the correct method on dyn_seq to extract the element type, then
  // uses the appropriate form of equals comparison.

};

class TAO_Element_Equal<CORBA::UShort>
{
public:
  int operator () (TAO_DynSequence_i& dyn_any,
                   CORBA::UShort element) const;
  // Calls the correct method on dyn_seq to extract the element type, then
  // uses the appropriate form of equals comparison.

};

class TAO_Element_Equal<CORBA::Long>
{
public:
  int operator () (TAO_DynSequence_i& dyn_any,
                   CORBA::Long element) const;
  // Calls the correct method on dyn_seq to extract the element type, then
  // uses the appropriate form of equals comparison.

};

class TAO_Element_Equal<CORBA::ULong>
{
public:
  int operator () (TAO_DynSequence_i& dyn_any,
                   CORBA::ULong element) const;
  // Calls the correct method on dyn_seq to extract the element type, then
  // uses the appropriate form of equals comparison.

};

class TAO_Element_Equal<CORBA::Float>
{
public:
  int operator () (TAO_DynSequence_i& dyn_any,
                   CORBA::Float element) const;
  // Calls the correct method on dyn_seq to extract the element type, then
  // uses the appropriate form of equals comparison.

};

class TAO_Element_Equal<CORBA::Double>
{
public:
  int operator () (TAO_DynSequence_i& dyn_any,
                   CORBA::Double element) const;
  // Calls the correct method on dyn_seq to extract the element type, then
  // uses the appropriate form of equals comparison.

};

class TAO_Element_Equal<CORBA::Boolean>
{
public:
  int operator () (TAO_DynSequence_i& dyn_any,
                   CORBA::Boolean element) const;
  // Calls the correct method on dyn_seq to extract the element type, then
  // uses the appropriate form of equals comparison.

};

class TAO_Element_Equal<const char*>
{
public:
  int operator () (TAO_DynSequence_i& dyn_any,
                   const char* element) const;
  // Calls the correct method on dyn_seq to extract the element type, then
  // uses the appropriate form of equals comparison.

};


#endif /* CONSTRAINT_VISITORS_H */
