// -*- C++ -*-
// $Id$

// ****************************************************************

ACE_INLINE
TAO_ETCL_Literal_Constraint::TAO_ETCL_Literal_Constraint (void)
  : any_ (0)
{
}

// ****************************************************************

ACE_INLINE
TAO_ETCL_Identifier::TAO_ETCL_Identifier (const char *value)
  : ETCL_Identifier (value)
{
}

// ****************************************************************

ACE_INLINE
TAO_ETCL_Union_Value::TAO_ETCL_Union_Value (int sign,
                                            ETCL_Constraint *integer)
  :  sign_ (sign),
     string_ (0)
{
  this->integer_ =
    dynamic_cast<TAO_ETCL_Literal_Constraint*> (integer);
}

ACE_INLINE
TAO_ETCL_Union_Value::TAO_ETCL_Union_Value (ETCL_Constraint *string)
  :  sign_ (0),
     integer_ (0)
{
  this->string_ =
    dynamic_cast<TAO_ETCL_Literal_Constraint*> (string);
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
TAO_ETCL_Union_Pos::TAO_ETCL_Union_Pos (ETCL_Constraint *union_value,
                                        ETCL_Constraint *component)
  :  component_ (component)
{
  this->union_value_ =
    dynamic_cast<TAO_ETCL_Union_Value*> (union_value);
}
    
ACE_INLINE TAO_ETCL_Union_Value *
TAO_ETCL_Union_Pos::union_value (void) const
{
  return this->union_value_;
}

ACE_INLINE ETCL_Constraint *
TAO_ETCL_Union_Pos::component (void) const
{
  return this->component_;
}

// ****************************************************************

ACE_INLINE
TAO_ETCL_Component_Pos::TAO_ETCL_Component_Pos (
    ETCL_Constraint *integer,
    ETCL_Constraint *component
  )
  :  component_ (component)
{
  this->integer_ =
    dynamic_cast<TAO_ETCL_Literal_Constraint*> (integer);
}

ACE_INLINE TAO_ETCL_Literal_Constraint *
TAO_ETCL_Component_Pos::integer (void) const
{
  return this->integer_;
}

ACE_INLINE ETCL_Constraint *
TAO_ETCL_Component_Pos::component (void) const
{
  return this->component_;
}

// ****************************************************************

ACE_INLINE
TAO_ETCL_Component_Assoc::TAO_ETCL_Component_Assoc (
    ETCL_Constraint *identifier,
    ETCL_Constraint *component
  )
  :  component_ (component)
{
  this->identifier_ =
    dynamic_cast<TAO_ETCL_Identifier*> (identifier);
}

ACE_INLINE TAO_ETCL_Identifier *
TAO_ETCL_Component_Assoc::identifier (void) const
{
  return this->identifier_;
}

ACE_INLINE ETCL_Constraint *
TAO_ETCL_Component_Assoc::component (void) const
{
  return this->component_;
}

// ****************************************************************

ACE_INLINE
TAO_ETCL_Component_Array::TAO_ETCL_Component_Array (
    ETCL_Constraint *integer,
    ETCL_Constraint *component
  )
  :  component_ (component)
{
  this->integer_ =
    dynamic_cast<TAO_ETCL_Literal_Constraint*> (integer);
}

ACE_INLINE TAO_ETCL_Literal_Constraint *
TAO_ETCL_Component_Array::integer (void) const
{
  return this->integer_;
}

ACE_INLINE ETCL_Constraint *
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
TAO_ETCL_Component::TAO_ETCL_Component (ETCL_Constraint *identifier,
                                        ETCL_Constraint *component)
  :  component_ (component)
{
  this->identifier_ =
    dynamic_cast<TAO_ETCL_Identifier*> (identifier);
}

ACE_INLINE TAO_ETCL_Identifier *
TAO_ETCL_Component::identifier (void) const
{
  return this->identifier_;
}

ACE_INLINE ETCL_Constraint *
TAO_ETCL_Component::component (void) const
{
  return this->component_;
}

// ****************************************************************

ACE_INLINE
TAO_ETCL_Dot::TAO_ETCL_Dot (ETCL_Constraint *component)
  :  component_ (component)
{
}

ACE_INLINE ETCL_Constraint *
TAO_ETCL_Dot::component (void) const
{
  return this->component_;
}

// ****************************************************************

ACE_INLINE
TAO_ETCL_Eval::TAO_ETCL_Eval (ETCL_Constraint *component)
  :  component_ (component)
{
}

ACE_INLINE ETCL_Constraint *
TAO_ETCL_Eval::component (void) const
{
  return this->component_;
}

// ****************************************************************

ACE_INLINE
TAO_ETCL_Default::TAO_ETCL_Default (ETCL_Constraint *component)
  :  component_ (component)
{
}

ACE_INLINE ETCL_Constraint *
TAO_ETCL_Default::component (void) const
{
  return this->component_;
}

// ****************************************************************

ACE_INLINE
TAO_ETCL_Exist::TAO_ETCL_Exist (ETCL_Constraint *component)
  :  component_ (component)
{
}

ACE_INLINE ETCL_Constraint *
TAO_ETCL_Exist::component (void) const
{
  return this->component_;
}

// ****************************************************************

ACE_INLINE
TAO_ETCL_Unary_Expr::TAO_ETCL_Unary_Expr (int type,
                                          ETCL_Constraint *subexpr)
  : ETCL_Unary_Expr (type, subexpr)
{
}

// ****************************************************************

ACE_INLINE
TAO_ETCL_Binary_Expr::TAO_ETCL_Binary_Expr (int type,
                                            ETCL_Constraint *lhs,
                                            ETCL_Constraint *rhs)
  : ETCL_Binary_Expr (type, lhs, rhs)
{
}

// ****************************************************************

ACE_INLINE
TAO_ETCL_Preference::TAO_ETCL_Preference (int type,
                                          ETCL_Constraint *subexpr)
  :  type_ (type),
     subexpr_ (subexpr)
{
}

ACE_INLINE int
TAO_ETCL_Preference::type (void) const
{
  return this->type_;
}

ACE_INLINE ETCL_Constraint *
TAO_ETCL_Preference::subexpr (void) const
{
  return this->subexpr_;
}
