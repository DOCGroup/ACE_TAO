/* -*- C++ -*- */
// $Id$

// ========================================================================
// 
// = LIBRARY 
//    orbsvcs
//  
// = FILENAME
//    Constraint_Evaluator.h
// 
// = AUTHOR 
//    Seth Widoff <sbw1@cs.wustl.edu> 
//   
// ========================================================================

#ifndef TAO_CONSTRAINT_EVALUATOR_H
#define TAO_CONSTRAINT_EVALUATOR_H

#include "Property_Evaluator.h"
#include "Constraint_Visitor.h"
#include "Constraint_Nodes.h"

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
//     EvaluationVisitor assumes the tree is semantically correct,
//     that is, the validate method on SemanticValidatorVisitor return
//     true. The only possible evaluation time error is a divide by a
//     property whose value is zero.
{
public:

  TAO_Constraint_Evaluator(CosTrading::Offer* offer,
			   CORBA::Boolean supports_dynamic_properties = 1);

  CORBA::Boolean evaluate_constraint(TAO_Constraint* root);
  // Evaluate returns 1 if the offer satisfies the constraints
  // represented by the the expression tree rooted at <root>, 0 if it
  // doesn't. If an error occurs during process, the constraint
  // automatically fails.

  int evaluate_preference(TAO_Constraint* root,
			  TAO_Literal_Constraint& result);
  // The result of the preference evaluation is stored in result. The
  // method returns 0 upon success, -1 upon failure.
  
  // = Visitor Methods
  
  virtual int visit_constraint(TAO_Unary_Constraint* constraint);

  virtual int visit_with(TAO_Unary_Constraint* unary_with);
  virtual int visit_min(TAO_Unary_Constraint* unary_min);
  virtual int visit_max(TAO_Unary_Constraint* unary_max);
  virtual int visit_first(TAO_Noop_Constraint* noop_first);
  virtual int visit_random(TAO_Noop_Constraint* noop_random);
  
  virtual int visit_and(TAO_Binary_Constraint* boolean_and);
  // Takes the logical and of the results of both operands. Note that
  // in the case where the left operand returns zero, the result is
  // immediately known.

  virtual int visit_or(TAO_Binary_Constraint* boolean_or);
  // Takes the logical or of the results of both operands. Note that
  // in the case where the left operand returns one, the result is
  // immediately known.

  virtual int visit_not(TAO_Unary_Constraint* unary_not);
  // Logically negates the value of the operand.
  
  virtual int visit_exist(TAO_Unary_Constraint* unary_exist);
  // The property exists if its name is bound to a value in the
  // <props_> map.

  virtual int visit_unary_minus(TAO_Unary_Constraint* unary_minus);
  // Mathematically negates the return value the operand.
  
  virtual int visit_add(TAO_Binary_Constraint* boolean_add);
  // Add the results of evaluating the left and right operands.
  
  virtual int visit_sub(TAO_Binary_Constraint* boolean_sub);
  // Subtract the results of evaluating the left and right operands.
  
  virtual int visit_mult(TAO_Binary_Constraint* boolean_mult);
  // Multiply the results of evaluating the left and right operands.

  virtual int visit_div(TAO_Binary_Constraint* boolean_div);
  // Divide the results of evaluating the left and right operands.

  virtual int visit_twiddle(TAO_Binary_Constraint* binary_twiddle);
  // Determines if the right operand is a substring of the left.

  virtual int visit_in(TAO_Binary_Constraint* binary_in);
  // Determines if the sequence represented by the right operand
  // contains the left operand.

  // = Compare the results of evaluating left and right operands.
  virtual int visit_less_than(TAO_Binary_Constraint* boolean_lt);
  virtual int visit_less_than_equal(TAO_Binary_Constraint* boolean_lte);
  virtual int visit_greater_than(TAO_Binary_Constraint* boolean_gt);
  virtual int visit_greater_than_equal(TAO_Binary_Constraint* boolean_gte);
  virtual int visit_equal(TAO_Binary_Constraint* boolean_eq);
  virtual int visit_not_equal(TAO_Binary_Constraint* boolean_neq);

  virtual int visit_literal(TAO_Literal_Constraint* literal);
  // Copy the value of the literal into the result container.

  virtual int visit_property(TAO_Property_Constraint* literal);
  // Copy the value of the property into the result container.
  
private:

  class Operand_Queue :
  public ACE_Unbounded_Queue <TAO_Literal_Constraint>
  {
  public:

    Operand_Queue (void);

    TAO_Literal_Constraint& get_left_operand (void);

    TAO_Literal_Constraint& get_right_operand (void);

    TAO_Literal_Constraint& get_operand (void);

    void dequeue_operand (void);
  };
  
  typedef ACE_Hash_Map_Manager
    <
    TAO_String_Hash_Key,
    int,
    ACE_Null_Mutex
    >
    Property_Map;
  
  void do_the_op (int operation);
  int visit_bin_op (TAO_Binary_Constraint* op, int operation);
  
  CORBA::Boolean sequence_does_contain(CORBA::Any* sequence,
				       TAO_Literal_Constraint& element);
  // Determine if sequence contains <element>, a literal of the same
  // simple type as <sequence_type>. Return 1 in this eventuality.

  TAO_Literal_Constraint& left_operand(void);
  TAO_Literal_Constraint& right_operand(void);
  
  Property_Map props_;
  // The map of property names to their values for a property.

  TAO_Property_Evaluator prop_eval_;
  // Utility with which to evaluate the properties of an offer, be
  // they dyanmic or static. 

  Operand_Queue queue_;
  // The result of a non_boolean operation.
};

CORBA::Boolean TAO_find_string (TAO_Sequences::StringSeq& sequence, const char* element);

#endif /* CONSTRAINT_EVALUATOR_H */
