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

class TAO_Binary_Constraint;
class TAO_Unary_Constraint;
class TAO_Property_Constraint;
class TAO_Literal_Constraint;
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

  virtual int visit_constraint(TAO_Unary_Constraint* constraint) = 0;

  virtual int visit_with(TAO_Unary_Constraint* unary_with) = 0;
  virtual int visit_min(TAO_Unary_Constraint* unary_min) = 0;
  virtual int visit_max(TAO_Unary_Constraint* unary_max) = 0;
  virtual int visit_first(TAO_Noop_Constraint* noop_first) = 0;
  virtual int visit_random(TAO_Noop_Constraint* noop_random) = 0;
  
  virtual int visit_and(TAO_Binary_Constraint* boolean_and) = 0;
  virtual int visit_or(TAO_Binary_Constraint* boolean_or) = 0;
  virtual int visit_not(TAO_Unary_Constraint* unary_not) = 0;

  virtual int visit_exist(TAO_Unary_Constraint* unary_exist) = 0;
  virtual int visit_unary_minus(TAO_Unary_Constraint* unary_minus) = 0;

  virtual int visit_add(TAO_Binary_Constraint* boolean_add) = 0;
  virtual int visit_sub(TAO_Binary_Constraint* boolean_sub) = 0;
  virtual int visit_mult(TAO_Binary_Constraint* boolean_mult) = 0;
  virtual int visit_div(TAO_Binary_Constraint* boolean_div) = 0;

  virtual int visit_twiddle(TAO_Binary_Constraint* binary_twiddle) = 0;
  virtual int visit_in(TAO_Binary_Constraint* binary_in) = 0;

  virtual int visit_less_than(TAO_Binary_Constraint* boolean_lt) = 0;
  virtual int visit_less_than_equal(TAO_Binary_Constraint* boolean_lte) = 0;
  virtual int visit_greater_than(TAO_Binary_Constraint* boolean_gt) = 0;
  virtual int visit_greater_than_equal(TAO_Binary_Constraint* boolean_gte) = 0;
  virtual int visit_equal(TAO_Binary_Constraint* boolean_eq) = 0;
  virtual int visit_not_equal(TAO_Binary_Constraint* boolean_neq) = 0;

  virtual int visit_literal(TAO_Literal_Constraint* literal) = 0;
  virtual int visit_property(TAO_Property_Constraint* literal) = 0;
};

#endif /* CONSTRAINT_VISITOR_H */
