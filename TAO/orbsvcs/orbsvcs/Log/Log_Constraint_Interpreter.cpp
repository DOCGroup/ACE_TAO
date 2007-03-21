#include "orbsvcs/Log/Log_Constraint_Interpreter.h"

#include "orbsvcs/Log/Log_Constraint_Visitors.h"

ACE_RCSID (Log,
           Log_Constraint_Interpreter,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Log_Constraint_Interpreter::TAO_Log_Constraint_Interpreter (
    const char *constraints
  )
{
  if (TAO_ETCL_Interpreter::is_empty_string (constraints))
    {
      // Root is deleted in the TAO_Interpreter's destructor.
      ACE_NEW_THROW_EX (this->root_,
                        TAO_ETCL_Literal_Constraint ((CORBA::Boolean) 1),
                        CORBA::NO_MEMORY ());
    }
  else
  {
      // root_ is set in this base class call.
      if (TAO_ETCL_Interpreter::build_tree (constraints) != 0)
          throw DsLogAdmin::InvalidConstraint ();
    }
}

TAO_Log_Constraint_Interpreter::~TAO_Log_Constraint_Interpreter (void)
{
}

CORBA::Boolean
TAO_Log_Constraint_Interpreter::evaluate (
    TAO_Log_Constraint_Visitor &evaluator
  )
{
  CORBA::Boolean retval =
    evaluator.evaluate_constraint (this->root_);

  return retval;
}

TAO_END_VERSIONED_NAMESPACE_DECL
