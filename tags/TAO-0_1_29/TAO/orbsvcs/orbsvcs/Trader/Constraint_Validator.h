/* -*- C++ -*- */

// ========================================================================
// $Id$
// 
// = LIBRARY 
//    orbsvcs
//  
// = FILENAME
//    Constraint_Validator.h
// 
// = AUTHOR 
//    Seth Widoff <sbw1@cs.wustl.edu> 
//   
// ========================================================================

#ifndef TAO_CONSTRAINT_VALIDATOR_H
#define TAO_CONSTRAINT_VALIDATOR_H

#include "Trader.h"
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
  //    traverse all the nodes in an expression tree, checking each
  //    for operands of the proper type. The algorithm for type
  //    checking is as follows: ensure that operand expression(s)
  //    return the correct types using exprReturns* methods, and their
  //    types. If they (or it) return the correct types, call accept
  //    on the operands until all return true, or one returns false,
  //    at which point we can back out of the traversal.
{
public:
  
  TAO_Constraint_Validator
    (CosTradingRepos::ServiceTypeRepository::TypeStruct* type_struct);
  // The constructor creates a map of property names to their values
  // from the Type Description retrieved from the
  // ServiceTypeRepository. The ServiceTypeRepository throws
  // exceptions when it's displeased with the type name provided to
  // it. The map allows O(lg n) associative access, rather than the
  // O(n) sequential lookup from the CORBA data structures.
  
  int validate(TAO_Constraint* root);
  // Validate returns 1 if the expression tree whose root is <root>
  // makes semantic sense, in that the operands for each operation
  // comply with each other and the types accepted by the operator.
  
  // = Visitor Methods

  virtual int visit_constraint(TAO_Unary_Constraint* constraint);

  virtual int visit_with(TAO_Unary_Constraint* unary_with);
  virtual int visit_min(TAO_Unary_Constraint* unary_min);
  virtual int visit_max(TAO_Unary_Constraint* unary_max);
  virtual int visit_first(TAO_Noop_Constraint* noop_first);
  virtual int visit_random(TAO_Noop_Constraint* noop_random);
  
  virtual int visit_and(TAO_Binary_Constraint* boolean_and);
  virtual int visit_or(TAO_Binary_Constraint* boolean_or);
  // The two operands must return a boolean value.
  
  virtual int visit_not(TAO_Unary_Constraint* unary_not);
  // The operand must return a boolean value.
  
  virtual int visit_exist(TAO_Unary_Constraint* unary_exist);
  // The operand must return a valid (i.e., present in the service
  // type description) property name.
  
  virtual int visit_unary_minus(TAO_Unary_Constraint* unary_minus);
  // The operand must return a number to be negated.
  
  virtual int visit_add(TAO_Binary_Constraint* boolean_add);
  virtual int visit_sub(TAO_Binary_Constraint* boolean_sub);
  virtual int visit_mult(TAO_Binary_Constraint* boolean_mult);
  virtual int visit_div(TAO_Binary_Constraint* boolean_div);
  // Both operands must return numeric results.
  
  virtual int visit_twiddle(TAO_Binary_Constraint* binary_twiddle);
  // Both operands must return strings.
  
  virtual int visit_in(TAO_Binary_Constraint* binary_in);
  // The right operand must be a sequence of the same simple type as
  // the left operand.
  
  virtual int visit_less_than(TAO_Binary_Constraint* boolean_lt);
  virtual int visit_less_than_equal(TAO_Binary_Constraint* boolean_lte);
  virtual int visit_greater_than(TAO_Binary_Constraint* boolean_gt);
  virtual int visit_greater_than_equal(TAO_Binary_Constraint* boolean_gte);
  virtual int visit_equal(TAO_Binary_Constraint* boolean_eq);
  virtual int visit_not_equal(TAO_Binary_Constraint* boolean_neq);
  // The left and right operands must both be of the same simple type.
  
  virtual int visit_literal(TAO_Literal_Constraint* literal);
  virtual int visit_property(TAO_Property_Constraint* literal);
  // The property must be defined in the service type description.
  
private:

  typedef ACE_Hash_Map_Manager
    <
    TAO_String_Hash_Key,
    CORBA::TypeCode_ptr,
    ACE_Null_Mutex
    >
    Property_Type_Map;
  
  Property_Type_Map type_map_;
  // A map gleaned from the ServiceTypeStruct, which correlates
  // property names with their types.

  CORBA::TypeCode* extract_type(TAO_Constraint* expr_type,
				TAO_Expression_Type& type);
  
  int expr_returns_boolean(TAO_Expression_Type expr_type);
  // expr_returns_boolean returns 1 if <expr_type>, when evaluated, will
  // return a boolean. 
  
  int expr_returns_number(TAO_Expression_Type expr_type);
  // expr_returns_boolean returns 1 if <expr_type>, when evaluated, will
  // return a number. 
  
  int expr_returns_string(TAO_Expression_Type expr_type);
  // expr_returns_boolean returns 1 if <expr_type>, when evaluated, will
  // return a string. 
};

#endif /* TAO_CONSTRAINT_VALIDATOR_H */
