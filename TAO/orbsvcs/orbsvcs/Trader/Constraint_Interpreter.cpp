// Constraint_Interpreter.cpp

// ========================================================================
// $Id$
// 
// = LIBRARY 
//    orbsvcs
//  
// = FILENAME 
//    Constraint_Interpreter.cpp
// 
// = AUTHOR
//    Seth Widoff <sbw1@cs.wustl.edu>
// 
// ========================================================================


#include "Constraint_Interpreter.h"
#include <ctype.h>
#include <stdio.h>

TAO_Constraint_Interpreter::
TAO_Constraint_Interpreter(CosTradingRepos::ServiceTypeRepository::TypeStruct* ts,
			   const char* constraints,
			   CORBA::Environment& _env)
  TAO_THROW_SPEC (CosTrading::IllegalConstraint)
  : TAO_Interpreter ()
{
  TAO_Constraint_Validator type_checker(ts);

  if (TAO_Interpreter::is_empty_string (constraints))
    this->root_ = new TAO_Literal_Constraint ((CORBA::Boolean) 1);
  else
    {
      if (this->build_tree (constraints) != 0)
	TAO_THROW (CosTrading::IllegalConstraint (constraints));	  

      if (type_checker.validate (this->root_) == -1)
	TAO_THROW (CosTrading::IllegalConstraint (constraints));
    }
}

TAO_Constraint_Interpreter::
TAO_Constraint_Interpreter(TAO_Constraint_Validator& validator,
			   const char* constraints,
			   CORBA::Environment& _env)
  TAO_THROW_SPEC (CosTrading::IllegalConstraint)
{
  if (TAO_Interpreter::is_empty_string (constraints))
    this->root_ = new TAO_Literal_Constraint ((CORBA::Boolean) 1);
  else
    {
      if (this->build_tree (constraints) != 0)
	TAO_THROW (CosTrading::IllegalConstraint (constraints));	  
      
      if (validator.validate (this->root_) == -1)
	TAO_THROW (CosTrading::IllegalConstraint (constraints));
    } 
}

TAO_Constraint_Interpreter::~TAO_Constraint_Interpreter (void)
{
}

#if defined TAO_HAS_DYNAMIC_PROPERTY_BUG

CORBA::Boolean
TAO_Constraint_Interpreter::
evaluate (CosTrading::Offer* offer, CORBA::ORB_ptr orb)
{
  TAO_Constraint_Evaluator evaluator (offer, orb);
  return evaluator.evaluate_constraint (this->root_);
}

#else

CORBA::Boolean
TAO_Constraint_Interpreter::evaluate (CosTrading::Offer* offer)
{
  TAO_Constraint_Evaluator evaluator (offer);
  return evaluator.evaluate_constraint (this->root_);
}

#endif /* TAO_HAS_DYNAMIC_PROPERTY_BUG */

CORBA::Boolean
TAO_Constraint_Interpreter::evaluate(TAO_Constraint_Evaluator& evaluator)
{
  return evaluator.evaluate_constraint (this->root_);
}

