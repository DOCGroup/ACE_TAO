// $Id$

#include "ETCL_Constraint.h"
#include "ETCL_y.h"

#if ! defined (__ACE_INLINE__)
#include "ETCL_Constraint.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ETCL, ETCL_Constraint, "$Id$")

TAO_ETCL_Constraint::~TAO_ETCL_Constraint (void)
{
}

TAO_ETCL_Constraint *
TAO_ETCL_Constraint::parse_constraint (const char* constraint)
{
}


// ****************************************************************

void
TAO_ETCL_Boolean_Literal::visit (TAO_ETCL_Constraint_Visitor *visitor)
{
  visitor->visit_boolean_literal (this);
}

void
TAO_ETCL_Integer_Literal::visit (TAO_ETCL_Constraint_Visitor *visitor)
{
  visitor->visit_integer_literal (this);
}

void
TAO_ETCL_Float_Literal::visit (TAO_ETCL_Constraint_Visitor *visitor)
{
  visitor->visit_float_literal (this);
}

void
TAO_ETCL_String_Literal::visit (TAO_ETCL_Constraint_Visitor *visitor)
{
  visitor->visit_string_literal (this);
}

void
TAO_ETCL_Identifier::visit (TAO_ETCL_Constraint_Visitor *visitor)
{
  visitor->visit_identifier (this);
}

// ****************************************************************

TAO_ETCL_Union_Value::~TAO_ETCL_Union_Value (void)
{
  delete this->string_;
  delete this->integer_;
}

void
TAO_ETCL_Union_Value::visit (TAO_ETCL_Constraint_Visitor *visitor)
{
  visitor->visit_union_value (this);
}

// ****************************************************************

TAO_ETCL_Union_Pos::~TAO_ETCL_Union_Pos (void)
{
  delete this->component_;
  delete this->union_value_;
}

void
TAO_ETCL_Union_Pos::visit (TAO_ETCL_Constraint_Visitor *visitor)
{
  visitor->visit_union_pos (this);
}

// ****************************************************************

TAO_ETCL_Component_Pos::~TAO_ETCL_Component_Pos (void)
{
  delete this->component_;
  delete this->integer_;
}

void
TAO_ETCL_Component_Pos::visit (TAO_ETCL_Constraint_Visitor *visitor)
{
  visitor->visit_component_pos (this);
}

// ****************************************************************

TAO_ETCL_Component_Assoc::~TAO_ETCL_Component_Assoc (void)
{
  delete this->component_;
  delete this->identifier_;
}

void
TAO_ETCL_Component_Assoc::visit (TAO_ETCL_Constraint_Visitor *visitor)
{
  visitor->visit_component_assoc (this);
}

// ****************************************************************

TAO_ETCL_Component_Array::~TAO_ETCL_Component_Array (void)
{
  delete this->component_;
  delete this->integer_;
}

void
TAO_ETCL_Component_Array::visit (TAO_ETCL_Constraint_Visitor *visitor)
{
  visitor->visit_component_array (this);
}

// ****************************************************************

TAO_ETCL_Special::~TAO_ETCL_Special (void)
{
}

void
TAO_ETCL_Special::visit (TAO_ETCL_Constraint_Visitor *visitor)
{
  visitor->visit_special (this);
}

// ****************************************************************

TAO_ETCL_Component::~TAO_ETCL_Component (void)
{
  delete this->component_;
  delete this->identifier_;
}

void
TAO_ETCL_Component::visit (TAO_ETCL_Constraint_Visitor *visitor)
{
  visitor->visit_component (this);
}

// ****************************************************************

TAO_ETCL_Dot::~TAO_ETCL_Dot (void)
{
  delete this->component_;
}

void
TAO_ETCL_Dot::visit (TAO_ETCL_Constraint_Visitor *visitor)
{
  visitor->visit_dot (this);
}

// ****************************************************************

TAO_ETCL_Eval::~TAO_ETCL_Eval (void)
{
  delete this->component_;
}

void
TAO_ETCL_Eval::visit (TAO_ETCL_Constraint_Visitor *visitor)
{
  visitor->visit_eval (this);
}

// ****************************************************************

TAO_ETCL_Default::~TAO_ETCL_Default (void)
{
  delete this->component_;
}

void
TAO_ETCL_Default::visit (TAO_ETCL_Constraint_Visitor *visitor)
{
  visitor->visit_default (this);
}

// ****************************************************************

TAO_ETCL_Exist::~TAO_ETCL_Exist (void)
{
  delete this->component_;
}

void
TAO_ETCL_Exist::visit (TAO_ETCL_Constraint_Visitor *visitor)
{
  visitor->visit_exist (this);
}

// ****************************************************************

TAO_ETCL_Unary_Expr::~TAO_ETCL_Unary_Expr (void)
{
  delete this->subexpr_;
}

void
TAO_ETCL_Unary_Expr::visit (TAO_ETCL_Constraint_Visitor *visitor)
{
  visitor->visit_unary_expr (this);
}

// ****************************************************************

TAO_ETCL_Binary_Expr::~TAO_ETCL_Binary_Expr (void)
{
  delete this->lhs_;
  delete this->rhs_;
}

void
TAO_ETCL_Binary_Expr::visit (TAO_ETCL_Constraint_Visitor *visitor)
{
  visitor->visit_binary_expr (this);
}

// ****************************************************************

TAO_ETCL_Preference::~TAO_ETCL_Preference (void)
{
  delete this->subexpr_;
}

void
TAO_ETCL_Preference::visit (TAO_ETCL_Constraint_Visitor *visitor)
{
  visitor->visit_preference (this);
}

// ****************************************************************

TAO_ETCL_Constraint_Visitor::~TAO_ETCL_Constraint_Visitor (void)
{
}
