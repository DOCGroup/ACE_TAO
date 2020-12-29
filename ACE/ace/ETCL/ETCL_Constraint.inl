// -*- C++ -*-
// ****************************************************************

ACE_INLINE
ETCL_Literal_Constraint::ETCL_Literal_Constraint ()
  : type_ (ACE_ETCL_UNKNOWN)
{
}

// ****************************************************************

ACE_INLINE
ETCL_Identifier::ETCL_Identifier (const char *value)
  : string_ (value)
{
}

ACE_INLINE const char *
ETCL_Identifier::value () const
{
  return this->string_.c_str ();
}


// ****************************************************************

ACE_INLINE
ETCL_Union_Value::ETCL_Union_Value (int sign,
                                    ETCL_Constraint *integer)
  : sign_ (sign),
    string_ (0)
{
  this->integer_ =
    dynamic_cast<ETCL_Literal_Constraint*> (integer);
}

ACE_INLINE
ETCL_Union_Value::ETCL_Union_Value (ETCL_Constraint *string)
  :  sign_ (0),
     integer_ (0)
{
  this->string_ =
    dynamic_cast<ETCL_Literal_Constraint*> (string);
}

ACE_INLINE int
ETCL_Union_Value::sign () const
{
  return this->sign_;
}

ACE_INLINE ETCL_Literal_Constraint *
ETCL_Union_Value::integer () const
{
  return this->integer_;
}

ACE_INLINE ETCL_Literal_Constraint *
ETCL_Union_Value::string () const
{
  return this->string_;
}

// ****************************************************************

ACE_INLINE
ETCL_Union_Pos::ETCL_Union_Pos (ETCL_Constraint *union_value,
                                ETCL_Constraint *component)
  : component_ (component)
{
  this->union_value_ =
    dynamic_cast<ETCL_Union_Value*> (union_value);
}

ACE_INLINE ETCL_Union_Value *
ETCL_Union_Pos::union_value () const
{
  return this->union_value_;
}

ACE_INLINE ETCL_Constraint *
ETCL_Union_Pos::component () const
{
  return this->component_;
}

// ****************************************************************

ACE_INLINE
ETCL_Component_Pos::ETCL_Component_Pos (
    ETCL_Constraint *integer,
    ETCL_Constraint *component)
  : component_ (component)
{
  this->integer_ =
    dynamic_cast<ETCL_Literal_Constraint*> (integer);
}

ACE_INLINE ETCL_Literal_Constraint *
ETCL_Component_Pos::integer () const
{
  return this->integer_;
}

ACE_INLINE ETCL_Constraint *
ETCL_Component_Pos::component () const
{
  return this->component_;
}

// ****************************************************************

ACE_INLINE
ETCL_Component_Assoc::ETCL_Component_Assoc (
    ETCL_Constraint *identifier,
    ETCL_Constraint *component)
  : component_ (component)
{
  this->identifier_ =
    dynamic_cast<ETCL_Identifier*> (identifier);
}

ACE_INLINE ETCL_Identifier *
ETCL_Component_Assoc::identifier () const
{
  return this->identifier_;
}

ACE_INLINE ETCL_Constraint *
ETCL_Component_Assoc::component () const
{
  return this->component_;
}

// ****************************************************************

ACE_INLINE
ETCL_Component_Array::ETCL_Component_Array (
    ETCL_Constraint *integer,
    ETCL_Constraint *component)
  : component_ (component)
{
  this->integer_ =
    dynamic_cast<ETCL_Literal_Constraint*> (integer);
}

ACE_INLINE ETCL_Literal_Constraint *
ETCL_Component_Array::integer () const
{
  return this->integer_;
}

ACE_INLINE ETCL_Constraint *
ETCL_Component_Array::component () const
{
  return this->component_;
}

// ****************************************************************

ACE_INLINE
ETCL_Special::ETCL_Special ()
  : type_ (0)
{}

ACE_INLINE
ETCL_Special::ETCL_Special (int type)
  : type_ (type)
{}

ACE_INLINE int
ETCL_Special::type () const
{
  return this->type_;
}

// ****************************************************************

ACE_INLINE
ETCL_Component::ETCL_Component (ETCL_Constraint *identifier,
                                ETCL_Constraint *component)
  : component_ (component)
{
  this->identifier_ =
    dynamic_cast<ETCL_Identifier*> (identifier);
}

ACE_INLINE ETCL_Identifier *
ETCL_Component::identifier () const
{
  return this->identifier_;
}

ACE_INLINE ETCL_Constraint *
ETCL_Component::component () const
{
  return this->component_;
}

// ****************************************************************

ACE_INLINE
ETCL_Dot::ETCL_Dot (ETCL_Constraint *component)
  : component_ (component)
{
}

ACE_INLINE ETCL_Constraint *
ETCL_Dot::component () const
{
  return this->component_;
}

// ****************************************************************

ACE_INLINE
ETCL_Eval::ETCL_Eval (ETCL_Constraint *component)
  : component_ (component)
{
}

ACE_INLINE ETCL_Constraint *
ETCL_Eval::component () const
{
  return this->component_;
}

// ****************************************************************

ACE_INLINE
ETCL_Default::ETCL_Default (ETCL_Constraint *component)
  : component_ (component)
{
}

ACE_INLINE ETCL_Constraint *
ETCL_Default::component () const
{
  return this->component_;
}

// ****************************************************************

ACE_INLINE
ETCL_Exist::ETCL_Exist (ETCL_Constraint *component)
  : component_ (component)
{
}

ACE_INLINE ETCL_Constraint *
ETCL_Exist::component () const
{
  return this->component_;
}

// ****************************************************************

ACE_INLINE
ETCL_Unary_Expr::ETCL_Unary_Expr (int type,
                                  ETCL_Constraint *subexpr)
  :  type_ (type),
     subexpr_ (subexpr)
{}

ACE_INLINE int
ETCL_Unary_Expr::type () const
{
  return this->type_;
}

ACE_INLINE ETCL_Constraint *
ETCL_Unary_Expr::subexpr () const
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
{}

ACE_INLINE int
ETCL_Binary_Expr::type () const
{
  return this->type_;
}

ACE_INLINE ETCL_Constraint *
ETCL_Binary_Expr::rhs () const
{
  return this->rhs_;
}

ACE_INLINE ETCL_Constraint *
ETCL_Binary_Expr::lhs () const
{
  return this->lhs_;
}

// ****************************************************************

ACE_INLINE
ETCL_Preference::ETCL_Preference ()
  : type_ (0),
    subexpr_ (0)
{}

ACE_INLINE
ETCL_Preference::ETCL_Preference (int type,
                                  ETCL_Constraint *subexpr)
  : type_ (type),
    subexpr_ (subexpr)
{}

ACE_INLINE int
ETCL_Preference::type () const
{
  return this->type_;
}

ACE_INLINE ETCL_Constraint *
ETCL_Preference::subexpr () const
{
  return this->subexpr_;
}

ACE_INLINE bool
ETCL_Literal_Constraint::operator!= (const ETCL_Literal_Constraint & rhs)
{
  return !(*this == rhs);
}

ACE_INLINE bool
ETCL_Literal_Constraint::operator<= (const ETCL_Literal_Constraint & rhs)
{
  return !(*this > rhs);
}

ACE_INLINE bool
ETCL_Literal_Constraint::operator>= (const ETCL_Literal_Constraint & rhs)
{
  return !(*this < rhs);
}
