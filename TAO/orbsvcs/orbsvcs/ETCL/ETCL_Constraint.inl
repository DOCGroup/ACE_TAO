// -*- C++ -*-
// $Id$

// ****************************************************************

TAO_ETCL_Literal_Constraint::TAO_ETCL_Literal_Constraint (void)
  : type_ (TAO_UNKNOWN)
{
}

// ****************************************************************

ACE_INLINE
TAO_ETCL_Identifier::TAO_ETCL_Identifier (const char *value)
  :  string_ (value)
{
}

ACE_INLINE const char *
TAO_ETCL_Identifier::value (void) const
{
  return this->string_.c_str ();
}


// ****************************************************************

ACE_INLINE
TAO_ETCL_Union_Value::TAO_ETCL_Union_Value (int sign,
                                            TAO_ETCL_Constraint *integer)
  :  sign_ (sign),
     string_ (0)
{
  this->integer_ =
    ACE_dynamic_cast (TAO_ETCL_Literal_Constraint*, integer);
}

ACE_INLINE
TAO_ETCL_Union_Value::TAO_ETCL_Union_Value (TAO_ETCL_Constraint *string)
  :  sign_ (0),
     integer_ (0)
{
  this->string_ =
    ACE_dynamic_cast (TAO_ETCL_Literal_Constraint*, string);
}

ACE_INLINE int
TAO_ETCL_Union_Value::sign (void) const
{
  return this->sign_;
}

ACE_INLINE TAO_ETCL_Literal_Constraint *
TAO_ETCL_Union_Value::integer (void) const
{
  return this->integer_;
}

ACE_INLINE TAO_ETCL_Literal_Constraint *
TAO_ETCL_Union_Value::string (void) const
{
  return this->string_;
}


// ****************************************************************

ACE_INLINE
TAO_ETCL_Union_Pos::TAO_ETCL_Union_Pos (TAO_ETCL_Constraint *union_value,
                                        TAO_ETCL_Constraint *component)
  :  component_ (component)
{
  this->union_value_ =
    ACE_dynamic_cast (TAO_ETCL_Union_Value*, union_value);
}
    
ACE_INLINE TAO_ETCL_Union_Value *
TAO_ETCL_Union_Pos::union_value (void) const
{
  return this->union_value_;
}

ACE_INLINE TAO_ETCL_Constraint *
TAO_ETCL_Union_Pos::component (void) const
{
  return this->component_;
}

// ****************************************************************

ACE_INLINE
TAO_ETCL_Component_Pos::TAO_ETCL_Component_Pos (
    TAO_ETCL_Constraint *integer,
    TAO_ETCL_Constraint *component
  )
  :  component_ (component)
{
  this->integer_ =
    ACE_dynamic_cast (TAO_ETCL_Literal_Constraint*, integer);
}

ACE_INLINE TAO_ETCL_Literal_Constraint *
TAO_ETCL_Component_Pos::integer (void) const
{
  return this->integer_;
}

ACE_INLINE TAO_ETCL_Constraint *
TAO_ETCL_Component_Pos::component (void) const
{
  return this->component_;
}

// ****************************************************************

ACE_INLINE
TAO_ETCL_Component_Assoc::TAO_ETCL_Component_Assoc (
    TAO_ETCL_Constraint *identifier,
    TAO_ETCL_Constraint *component
  )
  :  component_ (component)
{
  this->identifier_ =
    ACE_dynamic_cast (TAO_ETCL_Identifier*, identifier);
}

ACE_INLINE TAO_ETCL_Identifier *
TAO_ETCL_Component_Assoc::identifier (void) const
{
  return this->identifier_;
}

ACE_INLINE TAO_ETCL_Constraint *
TAO_ETCL_Component_Assoc::component (void) const
{
  return this->component_;
}

// ****************************************************************

ACE_INLINE
TAO_ETCL_Component_Array::TAO_ETCL_Component_Array (
    TAO_ETCL_Constraint *integer,
    TAO_ETCL_Constraint *component
  )
  :  component_ (component)
{
  this->integer_ =
    ACE_dynamic_cast (TAO_ETCL_Literal_Constraint*, integer);
}

ACE_INLINE TAO_ETCL_Literal_Constraint *
TAO_ETCL_Component_Array::integer (void) const
{
  return this->integer_;
}

ACE_INLINE TAO_ETCL_Constraint *
TAO_ETCL_Component_Array::component (void) const
{
  return this->component_;
}

// ****************************************************************

ACE_INLINE
TAO_ETCL_Special::TAO_ETCL_Special (int type)
  :  type_ (type)
{
}

ACE_INLINE int
TAO_ETCL_Special::type (void) const
{
  return this->type_;
}

// ****************************************************************

ACE_INLINE
TAO_ETCL_Component::TAO_ETCL_Component (TAO_ETCL_Constraint *identifier,
                                        TAO_ETCL_Constraint *component)
  :  component_ (component)
{
  this->identifier_ =
    ACE_dynamic_cast (TAO_ETCL_Identifier*, identifier);
}

ACE_INLINE TAO_ETCL_Identifier *
TAO_ETCL_Component::identifier (void) const
{
  return this->identifier_;
}

ACE_INLINE TAO_ETCL_Constraint *
TAO_ETCL_Component::component (void) const
{
  return this->component_;
}

// ****************************************************************

ACE_INLINE
TAO_ETCL_Dot::TAO_ETCL_Dot (TAO_ETCL_Constraint *component)
  :  component_ (component)
{
}

ACE_INLINE TAO_ETCL_Constraint *
TAO_ETCL_Dot::component (void) const
{
  return this->component_;
}

// ****************************************************************

ACE_INLINE
TAO_ETCL_Eval::TAO_ETCL_Eval (TAO_ETCL_Constraint *component)
  :  component_ (component)
{
}

ACE_INLINE TAO_ETCL_Constraint *
TAO_ETCL_Eval::component (void) const
{
  return this->component_;
}

// ****************************************************************

ACE_INLINE
TAO_ETCL_Default::TAO_ETCL_Default (TAO_ETCL_Constraint *component)
  :  component_ (component)
{
}

ACE_INLINE TAO_ETCL_Constraint *
TAO_ETCL_Default::component (void) const
{
  return this->component_;
}

// ****************************************************************

ACE_INLINE
TAO_ETCL_Exist::TAO_ETCL_Exist (TAO_ETCL_Constraint *component)
  :  component_ (component)
{
}

ACE_INLINE TAO_ETCL_Constraint *
TAO_ETCL_Exist::component (void) const
{
  return this->component_;
}

// ****************************************************************

ACE_INLINE
TAO_ETCL_Unary_Expr::TAO_ETCL_Unary_Expr (int type,
                                          TAO_ETCL_Constraint *subexpr)
  :  type_ (type),
     subexpr_ (subexpr)
{
}

ACE_INLINE int
TAO_ETCL_Unary_Expr::type (void) const
{
  return this->type_;
}

ACE_INLINE TAO_ETCL_Constraint *
TAO_ETCL_Unary_Expr::subexpr (void) const
{
  return this->subexpr_;
}

// ****************************************************************

ACE_INLINE
TAO_ETCL_Binary_Expr::TAO_ETCL_Binary_Expr (int type,
                                            TAO_ETCL_Constraint *lhs,
                                            TAO_ETCL_Constraint *rhs)
  :  type_ (type),
     lhs_ (lhs),
     rhs_ (rhs)
{
}

ACE_INLINE int
TAO_ETCL_Binary_Expr::type (void) const
{
  return this->type_;
}

ACE_INLINE TAO_ETCL_Constraint *
TAO_ETCL_Binary_Expr::rhs (void) const
{
  return this->rhs_;
}

ACE_INLINE TAO_ETCL_Constraint *
TAO_ETCL_Binary_Expr::lhs (void) const
{
  return this->lhs_;
}

// ****************************************************************

ACE_INLINE
TAO_ETCL_Preference::TAO_ETCL_Preference (int type,
                                          TAO_ETCL_Constraint *subexpr)
  :  type_ (type),
     subexpr_ (subexpr)
{
}

ACE_INLINE int
TAO_ETCL_Preference::type (void) const
{
  return this->type_;
}

ACE_INLINE TAO_ETCL_Constraint *
TAO_ETCL_Preference::subexpr (void) const
{
  return this->subexpr_;
}
