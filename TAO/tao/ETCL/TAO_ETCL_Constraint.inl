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
  : ETCL_Union_Value (sign, integer)
{}

ACE_INLINE
TAO_ETCL_Union_Value::TAO_ETCL_Union_Value (ETCL_Constraint *string)
  : ETCL_Union_Value (string)
{}

// ****************************************************************

ACE_INLINE
TAO_ETCL_Union_Pos::TAO_ETCL_Union_Pos (ETCL_Constraint *union_value,
                                        ETCL_Constraint *component)
  : ETCL_Union_Pos (union_value, component)
{}
    
// ****************************************************************

ACE_INLINE
TAO_ETCL_Component_Pos::TAO_ETCL_Component_Pos (
    ETCL_Constraint *integer,
    ETCL_Constraint *component
  )
  : ETCL_Component_Pos (integer, component)
{}

// ****************************************************************

ACE_INLINE
TAO_ETCL_Component_Assoc::TAO_ETCL_Component_Assoc (
    ETCL_Constraint *identifier,
    ETCL_Constraint *component
  )
  : ETCL_Component_Assoc (identifier, component)
{}

// ****************************************************************

ACE_INLINE
TAO_ETCL_Component_Array::TAO_ETCL_Component_Array (
    ETCL_Constraint *integer,
    ETCL_Constraint *component
  )
  : ETCL_Component_Array (integer, component)
{}

// ****************************************************************

ACE_INLINE
TAO_ETCL_Special::TAO_ETCL_Special (int type)
  : ETCL_Special (type)
{}

// ****************************************************************

ACE_INLINE
TAO_ETCL_Component::TAO_ETCL_Component (ETCL_Constraint *identifier,
                                        ETCL_Constraint *component)
  : ETCL_Component (identifier, component)
{}

// ****************************************************************

ACE_INLINE
TAO_ETCL_Dot::TAO_ETCL_Dot (ETCL_Constraint *component)
  : ETCL_Dot (component)
{}

// ****************************************************************

ACE_INLINE
TAO_ETCL_Eval::TAO_ETCL_Eval (ETCL_Constraint *component)
  : ETCL_Eval (component)
{}

// ****************************************************************

ACE_INLINE
TAO_ETCL_Default::TAO_ETCL_Default (ETCL_Constraint *component)
  : ETCL_Default (component)
{}

// ****************************************************************

ACE_INLINE
TAO_ETCL_Exist::TAO_ETCL_Exist (ETCL_Constraint *component)
  : ETCL_Exist (component)
{}

// ****************************************************************

ACE_INLINE
TAO_ETCL_Unary_Expr::TAO_ETCL_Unary_Expr (int type,
                                          ETCL_Constraint *subexpr)
  : ETCL_Unary_Expr (type, subexpr)
{}

// ****************************************************************

ACE_INLINE
TAO_ETCL_Binary_Expr::TAO_ETCL_Binary_Expr (int type,
                                            ETCL_Constraint *lhs,
                                            ETCL_Constraint *rhs)
  : ETCL_Binary_Expr (type, lhs, rhs)
{}

// ****************************************************************

ACE_INLINE
TAO_ETCL_Preference::TAO_ETCL_Preference (int type,
                                          ETCL_Constraint *subexpr)
  : ETCL_Preference (type, subexpr)
{}
