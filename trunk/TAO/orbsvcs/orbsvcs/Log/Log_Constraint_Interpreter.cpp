// $Id$

#include "Log_Constraint_Interpreter.h"
#include "Log_Constraint_Visitors.h"

TAO_Log_Constraint_Interpreter::
TAO_Log_Constraint_Interpreter(const char* constraints
                               ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((DsLogAdmin::InvalidConstraint,
                   CORBA::NO_MEMORY))
{
  if (TAO_Interpreter::is_empty_string (constraints))
    {
      ACE_NEW_THROW_EX (this->root_,
                        TAO_Literal_Constraint ((CORBA::Boolean) 1),
                        CORBA::NO_MEMORY ());
      ACE_CHECK;
    }
  else
    {
      if (this->build_tree (constraints) != 0)
        ACE_THROW (DsLogAdmin::InvalidConstraint ());

      // @@ pradeep: Don't know exactly how the validator works in the
      // Trader so the validater visitor is not used here.
      /*      if (validator.validate (this->root_) == -1)
              ACE_THROW (CosTrading::IllegalConstraint (constraints));*/
    }
}

TAO_Log_Constraint_Interpreter::~TAO_Log_Constraint_Interpreter (void)
{
}

CORBA::Boolean
TAO_Log_Constraint_Interpreter::evaluate (TAO_Constraint_Evaluator& evaluator)
{
  return evaluator.evaluate_constraint (this->root_);
}
