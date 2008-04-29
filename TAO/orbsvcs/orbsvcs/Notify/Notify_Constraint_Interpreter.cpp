// $Id$

#include "orbsvcs/Notify/Notify_Constraint_Interpreter.h"
#include "orbsvcs/Notify/Notify_Constraint_Visitors.h"

ACE_RCSID (Notify,
           NS_Constraint_Interpreter,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Notify_Constraint_Interpreter::TAO_Notify_Constraint_Interpreter (void)
{
}

TAO_Notify_Constraint_Interpreter::~TAO_Notify_Constraint_Interpreter (void)
{
}

void
TAO_Notify_Constraint_Interpreter::build_tree (
    const char *constraints
  )
{
  if (ETCL_Interpreter::is_empty_string (constraints))
    {
      // Root is deleted in the TAO_Interpreter's destructor.
      ACE_NEW_THROW_EX (this->root_,
                        TAO_ETCL_Literal_Constraint ((CORBA::Boolean) 1),
                        CORBA::NO_MEMORY ());
    }
  else
    {
      // root_ is set in this base class call.
      if (ETCL_Interpreter::build_tree (constraints) != 0)
          throw CosNotifyFilter::InvalidConstraint ();
    }
}

CORBA::Boolean
TAO_Notify_Constraint_Interpreter::evaluate (TAO_Notify_Constraint_Visitor &evaluator)
{
  return evaluator.evaluate_constraint (this->root_);
}

TAO_END_VERSIONED_NAMESPACE_DECL
