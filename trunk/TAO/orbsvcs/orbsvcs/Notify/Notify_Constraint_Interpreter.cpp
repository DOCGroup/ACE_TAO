// $Id$

#include "Notify_Constraint_Interpreter.h"
#include "Notify_Constraint_Visitors.h"

ACE_RCSID(Notify, NS_Constraint_Interpreter, "$Id$")

TAO_NS_Constraint_Interpreter::TAO_NS_Constraint_Interpreter (void)
{
}

TAO_NS_Constraint_Interpreter::~TAO_NS_Constraint_Interpreter (void)
{
}

void
TAO_NS_Constraint_Interpreter::build_tree (
    const char *constraints
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CosNotifyFilter::InvalidConstraint,
                   CORBA::NO_MEMORY))
{
  if (TAO_ETCL_Interpreter::is_empty_string (constraints))
    {
      // Root is deleted in the TAO_Interpreter's destructor.
      ACE_NEW_THROW_EX (this->root_,
                        TAO_ETCL_Literal_Constraint ((CORBA::Boolean) 1),
                        CORBA::NO_MEMORY ());
      ACE_CHECK;
    }
  else
    {
      // root_ is set in this base class call.
      if (TAO_ETCL_Interpreter::build_tree (constraints) != 0)
          ACE_THROW (CosNotifyFilter::InvalidConstraint ());
    }
}

CORBA::Boolean
TAO_NS_Constraint_Interpreter::evaluate (TAO_NS_Constraint_Visitor &evaluator)
{
  CORBA::Boolean retval = evaluator.evaluate_constraint (this->root_);

  return retval;
}
