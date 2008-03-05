// -*- C++ -*-
// $Id$

// ****************************************************************

ACE_INLINE
ETCL_Literal_Constraint::ETCL_Literal_Constraint (void)
  : type_ (TAO_UNKNOWN)
{
}

// ****************************************************************

ACE_INLINE
ETCL_Identifier::ETCL_Identifier (const char *value)
  : string_ (value)
{
}

ACE_INLINE const char *
ETCL_Identifier::value (void) const
{
  return this->string_.c_str ();
}


// ****************************************************************

ACE_INLINE
ETCL_Union_Value::ETCL_Union_Value (void)
{}

ACE_INLINE
ETCL_Union_Value::ETCL_Union_Value (int /* sign */,
                                    ETCL_Constraint * /* integer */)
{}

ACE_INLINE
ETCL_Union_Value::ETCL_Union_Value (ETCL_Constraint * /* string */)
{}

// ****************************************************************

ACE_INLINE
ETCL_Union_Pos::ETCL_Union_Pos (void)
{}
    
ACE_INLINE
ETCL_Union_Pos::ETCL_Union_Pos (ETCL_Constraint * /* union_value */,
                                ETCL_Constraint * /* component */)
{}
    
// ****************************************************************

ACE_INLINE
ETCL_Component_Pos::ETCL_Component_Pos (void)
{}

ACE_INLINE
ETCL_Component_Pos::ETCL_Component_Pos (
    ETCL_Constraint * /* integer */,
    ETCL_Constraint * /* component */
  )
{}

// ****************************************************************

ACE_INLINE
ETCL_Component_Assoc::ETCL_Component_Assoc (void)
{}

ACE_INLINE
ETCL_Component_Assoc::ETCL_Component_Assoc (
    ETCL_Constraint * /* identifier */,
    ETCL_Constraint * /* component */
  )
{}

// ****************************************************************

ACE_INLINE
ETCL_Component_Array::ETCL_Component_Array (void)
{}

ACE_INLINE
ETCL_Component_Array::ETCL_Component_Array (
    ETCL_Constraint * /* integer */,
    ETCL_Constraint * /* component */
  )
{}

// ****************************************************************

ACE_INLINE
ETCL_Special::ETCL_Special (void)
{}

ACE_INLINE
ETCL_Special::ETCL_Special (int /* type */)
{}

// ****************************************************************

ACE_INLINE
ETCL_Component::ETCL_Component (void)
{}

ACE_INLINE
ETCL_Component::ETCL_Component (ETCL_Constraint * /* identifier */,
                                ETCL_Constraint * /* component */)
{}

// ****************************************************************

ACE_INLINE
ETCL_Dot::ETCL_Dot (void)
{}

ACE_INLINE
ETCL_Dot::ETCL_Dot (ETCL_Constraint * /* component */)
{}

// ****************************************************************

ACE_INLINE
ETCL_Eval::ETCL_Eval (void)
{}

ACE_INLINE
ETCL_Eval::ETCL_Eval (ETCL_Constraint * /* component */)
{}

// ****************************************************************

ACE_INLINE
ETCL_Default::ETCL_Default (void)
{}

ACE_INLINE
ETCL_Default::ETCL_Default (ETCL_Constraint * /* component */)
{}

// ****************************************************************

ACE_INLINE
ETCL_Exist::ETCL_Exist (void)
{}

ACE_INLINE
ETCL_Exist::ETCL_Exist (ETCL_Constraint * /* component */)
{}

// ****************************************************************

ACE_INLINE
ETCL_Unary_Expr::ETCL_Unary_Expr (int type,
                                  ETCL_Constraint *subexpr)
  :  type_ (type),
     subexpr_ (subexpr)
{
}

ACE_INLINE int
ETCL_Unary_Expr::type (void) const
{
  return this->type_;
}

ACE_INLINE ETCL_Constraint *
ETCL_Unary_Expr::subexpr (void) const
{
  return this->subexpr_;
}

// ****************************************************************

ACE_INLINE
ETCL_Binary_Expr::ETCL_Binary_Expr (int type,
                                    ETCL_Constraint *lhs,
                                    ETCL_Constraint *rhs)
  :  type_ (type),
     lhs_ (lhs),
     rhs_ (rhs)
{
}

ACE_INLINE int
ETCL_Binary_Expr::type (void) const
{
  return this->type_;
}

ACE_INLINE ETCL_Constraint *
ETCL_Binary_Expr::rhs (void) const
{
  return this->rhs_;
}

ACE_INLINE ETCL_Constraint *
ETCL_Binary_Expr::lhs (void) const
{
  return this->lhs_;
}

// ****************************************************************

ACE_INLINE
ETCL_Preference::ETCL_Preference (void)
{}

ACE_INLINE
ETCL_Preference::ETCL_Preference (int /* type */,
                                  ETCL_Constraint * /* subexpr */)
{}

