/* -*- C++ -*- */

// ========================================================================
// $Id$
//
// = LIBRARY 
//    orbsvcs
//  
// = FILENAME 
//    Constraint_Interpreter.h
// 
// = AUTHOR
//    Seth Widoff <sbw1@cs.wustl.edu>
// 
// ========================================================================

#ifndef TAO_CONSTRAINT_INTERPRETER_H
#define TAO_CONSTRAINT_INTERPRETER_H

#include <stack>
#include <deque>
#include "Interpreter.h"
#include "Constraint_Validator.h"
#include "Constraint_Evaluator.h"

class TAO_Constraint_Interpreter : public TAO_Interpreter
//
// = TITLE
//   TAO_Constraint_Interpreter will, given a constraint string whose
//   syntax and semantics comply with the trader specification for the
//   constraint language, determine if a CosTrading::Offer meets the
//   constraints. 
//
// = DESCRIPTION
//   TAO_Constraint_Interpreter will first build an expression tree
//   representing the constraint expression using Lex and Yacc. Then,
//   using a TAO_Constraint_Validator, it will validate the semantic
//   correctness of the tree. When the evaluate method is invoked with
//   an Offer, the TAO_Constraint_Interpreter will construct an
//   EvaluationVisitor, which will evaluate the tree and decide
//   whether the offer meets the constraints.
{
public:
  
  TAO_Constraint_Interpreter(CosTradingRepos::ServiceTypeRepository::TypeStruct* ts,
			     const char* constraints,
			     CORBA::Environment& env)
    TAO_THROW_SPEC (CosTrading::IllegalConstraint);
  
  TAO_Constraint_Interpreter(TAO_Constraint_Validator& validator,
			     const char* constraints,
			     CORBA::Environment& env)
    TAO_THROW_SPEC (CosTrading::IllegalConstraint);
  // This constructor builds an expression tree representing the
  // constraint specified in <constraints>, and throws an Illegal
  // Constraint exception if the constraint given has syntax errors or
  // semantic errors, such as mismatched types.
  
  ~TAO_Constraint_Interpreter(void);
  // Destructor

  CORBA::Boolean evaluate(CosTrading::Offer* offer);

  CORBA::Boolean evaluate(TAO_Constraint_Evaluator& evaluator);
  
  // Determine whether an offer fits the constraints with which the
  // tree was constructed. This method is thread safe (hopefully).
};

#endif /* TAO_CONSTRAINT_INTERPRETER_H */
