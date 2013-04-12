// $Id$

#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Notify/Notify_Constraint_Interpreter.h"
#include "orbsvcs/Notify/Notify_Constraint_Visitors.h"
#include "orbsvcs/Notify/EventType.h"
#include "tao/debug.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Notify_Constraint_Interpreter::TAO_Notify_Constraint_Interpreter (void)
{
}

TAO_Notify_Constraint_Interpreter::~TAO_Notify_Constraint_Interpreter (void)
{
}

void
TAO_Notify_Constraint_Interpreter::build_tree (
    const char *constraints)
{
  if (ETCL_Interpreter::is_empty_string (constraints))
    {
      // Root is deleted in the TAO_Interpreter's destructor.
      ACE_NEW_THROW_EX (this->root_,
                        ETCL_Literal_Constraint (true),
                        CORBA::NO_MEMORY ());
    }
  else
    {
      // root_ is set in this base class call.
      if (ETCL_Interpreter::build_tree (constraints) != 0)
        {
          throw CosNotifyFilter::InvalidConstraint ();
        }
    }
}

void
TAO_Notify_Constraint_Interpreter::build_tree (
    const CosNotifyFilter::ConstraintExp& exp)
{
  ACE_CString exp_str;
  ACE_CString et_exp;

  CORBA::ULong const len = exp.event_types.length ();

  bool has_et_exp = false;

  for (CORBA::ULong ii = 0; ii < len; ++ii)
  {
    TAO_Notify_EventType et;
    bool const d = et.domain_is_wildcard (exp.event_types [ii].domain_name.in ());
    bool const t = et.type_is_wildcard (exp.event_types [ii].type_name.in ());

    if (d && t)
    {
      exp_str = "";
      break;
    }

    if (has_et_exp)
      et_exp += " or ";
    et_exp += "(";

    has_et_exp = has_et_exp || 1;

    if (! d)
    {
      et_exp += "$domain_name=='";
      et_exp += exp.event_types [ii].domain_name.in ();
      et_exp += "'";
    }

    if (! t)
    {
      if (!d)
        et_exp += " and ";

      et_exp += "$type_name=='";
      et_exp += exp.event_types [ii].type_name.in ();
      et_exp += "'";
    }

    et_exp += ")";
  }

  bool const valid_constraint = !ETCL_Interpreter::is_empty_string (exp.constraint_expr.in ());

  if (has_et_exp && valid_constraint)
  {
    exp_str = "((";
    exp_str += et_exp;
    exp_str += ") and (";
    exp_str += exp.constraint_expr.in ();
    exp_str += "))";
  }
  else if (has_et_exp)
    exp_str = et_exp;
  else if (valid_constraint)
    exp_str = exp.constraint_expr.in ();

  if (TAO_debug_level > 0)
    {
      ORBSVCS_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) Constraint: %C\n"),
        exp_str.c_str ()));
    }
  this->build_tree (exp_str.c_str ());
}

CORBA::Boolean
TAO_Notify_Constraint_Interpreter::evaluate (TAO_Notify_Constraint_Visitor &evaluator)
{
  return evaluator.evaluate_constraint (this->root_);
}

TAO_END_VERSIONED_NAMESPACE_DECL
