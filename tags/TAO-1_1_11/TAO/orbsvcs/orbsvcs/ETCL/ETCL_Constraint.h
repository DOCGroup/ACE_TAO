/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   orbsvcs / Extended Trader Constraint Language parser.
//
// = FILENAME
//   ECTL_Constraint
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// ============================================================================

#ifndef TAO_ETCL_CONSTRAINT_H
#define TAO_ETCL_CONSTRAINT_H

#include "orbsvcs/orbsvcs_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/SString.h"

class TAO_ETCL_Constraint_Visitor;

class TAO_ETCL_Export TAO_ETCL_Constraint
{
public:
  virtual ~TAO_ETCL_Constraint (void);
  // Constructor and destructor

  virtual void visit (TAO_ETCL_Constraint_Visitor *visitor) = 0;

  static TAO_ETCL_Constraint *parse_constraint (const char* constraint);
};

// ****************************************************************

class TAO_ETCL_Export TAO_ETCL_Boolean_Literal : public TAO_ETCL_Constraint
{
public:
  TAO_ETCL_Boolean_Literal (int value);

  int value (void) const;
  // Get the value

  // = The Constraint methods.
  void visit (TAO_ETCL_Constraint_Visitor *visitor);

private:
  int value_;
  // The value
};

// ****************************************************************

class TAO_ETCL_Export TAO_ETCL_Integer_Literal : public TAO_ETCL_Constraint
{
public:
  TAO_ETCL_Integer_Literal (long value);

  long value (void) const;
  // Get the value

  // = The Constraint methods.
  void visit (TAO_ETCL_Constraint_Visitor *visitor);

private:
  long value_;
  // The value
};

// ****************************************************************

class TAO_ETCL_Export TAO_ETCL_Float_Literal : public TAO_ETCL_Constraint
{
public:
  TAO_ETCL_Float_Literal (double value);

  double value (void) const;
  // Get the value

  // = The Constraint methods.
  void visit (TAO_ETCL_Constraint_Visitor *visitor);

private:
  double value_;
  // The value
};

// ****************************************************************

class TAO_ETCL_Export TAO_ETCL_String_Literal : public TAO_ETCL_Constraint
{
public:
  TAO_ETCL_String_Literal (const char *value);

  const char *value (void) const;
  // Get the value

  // = The Constraint methods.
  void visit (TAO_ETCL_Constraint_Visitor *visitor);

private:
  ACE_CString string_;
  // The value
};

// ****************************************************************

class TAO_ETCL_Export TAO_ETCL_Identifier : public TAO_ETCL_Constraint
{
public:
  TAO_ETCL_Identifier (const char *value);

  const char *value (void) const;
  // Get the value

  // = The Constraint methods.
  void visit (TAO_ETCL_Constraint_Visitor *visitor);

private:
  ACE_CString string_;
  // The value
};

// ****************************************************************

class TAO_ETCL_Export TAO_ETCL_Union_Value : public TAO_ETCL_Constraint
{
public:
  TAO_ETCL_Union_Value (int sign,
                        TAO_ETCL_Constraint *integer);
  TAO_ETCL_Union_Value (TAO_ETCL_Constraint *string);
  virtual ~TAO_ETCL_Union_Value (void);

  int sign (void) const;
  TAO_ETCL_Integer_Literal *integer (void) const;
  TAO_ETCL_String_Literal *string (void) const;

  // = The Constraint methods.
  void visit (TAO_ETCL_Constraint_Visitor *visitor);

private:
  int sign_;
  TAO_ETCL_Integer_Literal *integer_;
  TAO_ETCL_String_Literal *string_;
};

class TAO_ETCL_Export TAO_ETCL_Union_Pos : public TAO_ETCL_Constraint
{
public:
  TAO_ETCL_Union_Pos (TAO_ETCL_Constraint *union_value,
                      TAO_ETCL_Constraint *component);
  virtual ~TAO_ETCL_Union_Pos (void);

  TAO_ETCL_Union_Value *union_value (void) const;
  TAO_ETCL_Constraint *component (void) const;

  // = The Constraint methods.
  void visit (TAO_ETCL_Constraint_Visitor *visitor);

private:
  TAO_ETCL_Union_Value *union_value_;
  TAO_ETCL_Constraint *component_;
};

class TAO_ETCL_Export TAO_ETCL_Component_Pos : public TAO_ETCL_Constraint
{
public:
  TAO_ETCL_Component_Pos (TAO_ETCL_Constraint *integer,
                          TAO_ETCL_Constraint *component);
  virtual ~TAO_ETCL_Component_Pos (void);

  TAO_ETCL_Integer_Literal *integer (void) const;
  TAO_ETCL_Constraint *component (void) const;

  // = The Constraint methods.
  void visit (TAO_ETCL_Constraint_Visitor *visitor);

private:
  TAO_ETCL_Integer_Literal *integer_;
  TAO_ETCL_Constraint *component_;
};

class TAO_ETCL_Export TAO_ETCL_Component_Assoc : public TAO_ETCL_Constraint
{
public:
  TAO_ETCL_Component_Assoc (TAO_ETCL_Constraint *identifier,
                            TAO_ETCL_Constraint *component);
  virtual ~TAO_ETCL_Component_Assoc (void);

  TAO_ETCL_Identifier *identifier (void) const;
  TAO_ETCL_Constraint *component (void) const;

  // = The Constraint methods.
  void visit (TAO_ETCL_Constraint_Visitor *visitor);

private:
  TAO_ETCL_Identifier *identifier_;
  TAO_ETCL_Constraint *component_;
};

class TAO_ETCL_Export TAO_ETCL_Component_Array : public TAO_ETCL_Constraint
{
public:
  TAO_ETCL_Component_Array (TAO_ETCL_Constraint *integer,
                            TAO_ETCL_Constraint *component);
  virtual ~TAO_ETCL_Component_Array (void);

  TAO_ETCL_Integer_Literal *integer (void) const;
  TAO_ETCL_Constraint *component (void) const;

  // = The Constraint methods.
  void visit (TAO_ETCL_Constraint_Visitor *visitor);

private:
  TAO_ETCL_Integer_Literal *integer_;
  TAO_ETCL_Constraint *component_;
};

class TAO_ETCL_Export TAO_ETCL_Special : public TAO_ETCL_Constraint
{
public:
  TAO_ETCL_Special (int type);
  virtual ~TAO_ETCL_Special (void);

  int type (void) const;

  // = The Constraint methods.
  void visit (TAO_ETCL_Constraint_Visitor *visitor);

private:
  int type_;
};

class TAO_ETCL_Export TAO_ETCL_Component : public TAO_ETCL_Constraint
{
public:
  TAO_ETCL_Component (TAO_ETCL_Constraint *identifier,
                      TAO_ETCL_Constraint *component);
  virtual ~TAO_ETCL_Component (void);

  TAO_ETCL_Identifier *identifier (void) const;
  TAO_ETCL_Constraint *component (void) const;

  // = The Constraint methods.
  void visit (TAO_ETCL_Constraint_Visitor *visitor);

private:
  TAO_ETCL_Identifier *identifier_;
  TAO_ETCL_Constraint *component_;
};

class TAO_ETCL_Export TAO_ETCL_Dot : public TAO_ETCL_Constraint
{
public:
  TAO_ETCL_Dot (TAO_ETCL_Constraint *component);
  virtual ~TAO_ETCL_Dot (void);

  TAO_ETCL_Constraint *component (void) const;

  // = The Constraint methods.
  void visit (TAO_ETCL_Constraint_Visitor *visitor);

private:
  TAO_ETCL_Constraint *component_;
};

class TAO_ETCL_Export TAO_ETCL_Eval : public TAO_ETCL_Constraint
{
public:
  TAO_ETCL_Eval (TAO_ETCL_Constraint *component);
  virtual ~TAO_ETCL_Eval (void);

  TAO_ETCL_Constraint *component (void) const;

  // = The Constraint methods.
  void visit (TAO_ETCL_Constraint_Visitor *visitor);

private:
  TAO_ETCL_Constraint *component_;
};

class TAO_ETCL_Export TAO_ETCL_Default : public TAO_ETCL_Constraint
{
public:
  TAO_ETCL_Default (TAO_ETCL_Constraint *component);
  virtual ~TAO_ETCL_Default (void);

  TAO_ETCL_Constraint *component (void) const;

  // = The Constraint methods.
  void visit (TAO_ETCL_Constraint_Visitor *visitor);

private:
  TAO_ETCL_Constraint *component_;
};

class TAO_ETCL_Export TAO_ETCL_Exist : public TAO_ETCL_Constraint
{
public:
  TAO_ETCL_Exist (TAO_ETCL_Constraint *component);
  virtual ~TAO_ETCL_Exist (void);

  TAO_ETCL_Constraint *component (void) const;

  // = The Constraint methods.
  void visit (TAO_ETCL_Constraint_Visitor *visitor);

private:
  TAO_ETCL_Constraint *component_;
};

class TAO_ETCL_Export TAO_ETCL_Unary_Expr : public TAO_ETCL_Constraint
{
public:
  TAO_ETCL_Unary_Expr (int type,
                       TAO_ETCL_Constraint *subexpr);
  virtual ~TAO_ETCL_Unary_Expr (void);

  int type (void) const;
  TAO_ETCL_Constraint *subexpr (void) const;

  // = The Constraint methods.
  void visit (TAO_ETCL_Constraint_Visitor *visitor);

private:
  int type_;
  TAO_ETCL_Constraint *subexpr_;
};

class TAO_ETCL_Export TAO_ETCL_Binary_Expr : public TAO_ETCL_Constraint
{
public:
  TAO_ETCL_Binary_Expr (int type,
                        TAO_ETCL_Constraint *lhs,
                        TAO_ETCL_Constraint *rhs);
  virtual ~TAO_ETCL_Binary_Expr (void);

  int type (void) const;
  TAO_ETCL_Constraint *rhs (void) const;
  TAO_ETCL_Constraint *lhs (void) const;

  // = The Constraint methods.
  void visit (TAO_ETCL_Constraint_Visitor *visitor);

private:
  int type_;
  TAO_ETCL_Constraint *lhs_;
  TAO_ETCL_Constraint *rhs_;
};

class TAO_ETCL_Export TAO_ETCL_Preference : public TAO_ETCL_Constraint
{
public:
  TAO_ETCL_Preference (int type,
                       TAO_ETCL_Constraint *subexpr = 0);
  virtual ~TAO_ETCL_Preference (void);

  int type (void) const;
  TAO_ETCL_Constraint *subexpr (void) const;

  // = The Constraint methods.
  void visit (TAO_ETCL_Constraint_Visitor *visitor);

private:
  int type_;
  TAO_ETCL_Constraint *subexpr_;
};

// ****************************************************************

class TAO_ETCL_Export TAO_ETCL_Constraint_Visitor
{
public:
  virtual ~TAO_ETCL_Constraint_Visitor (void);
  // Destructor

  virtual void visit_boolean_literal (TAO_ETCL_Boolean_Literal *) = 0;
  virtual void visit_integer_literal (TAO_ETCL_Integer_Literal *) = 0;
  virtual void visit_float_literal (TAO_ETCL_Float_Literal *) = 0;
  virtual void visit_string_literal (TAO_ETCL_String_Literal *) = 0;
  virtual void visit_identifier (TAO_ETCL_Identifier *) = 0;
  virtual void visit_union_value (TAO_ETCL_Union_Value *) = 0;
  virtual void visit_union_pos (TAO_ETCL_Union_Pos *) = 0;
  virtual void visit_component_pos (TAO_ETCL_Component_Pos *) = 0;
  virtual void visit_component_assoc (TAO_ETCL_Component_Assoc *) = 0;
  virtual void visit_component_array (TAO_ETCL_Component_Array *) = 0;
  virtual void visit_special (TAO_ETCL_Special *) = 0;
  virtual void visit_component (TAO_ETCL_Component *) = 0;
  virtual void visit_dot (TAO_ETCL_Dot *) = 0;
  virtual void visit_eval (TAO_ETCL_Eval *) = 0;
  virtual void visit_default (TAO_ETCL_Default *) = 0;
  virtual void visit_exist (TAO_ETCL_Exist *) = 0;
  virtual void visit_unary_expr (TAO_ETCL_Unary_Expr *) = 0;
  virtual void visit_binary_expr (TAO_ETCL_Binary_Expr *) = 0;
  virtual void visit_preference (TAO_ETCL_Preference *) = 0;
};

// ****************************************************************

union TAO_ETCL_YYSTYPE
{
  TAO_ETCL_Constraint *constraint;
};

// ****************************************************************

#if defined (__ACE_INLINE__)
#include "ETCL_Constraint.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_ETCL_CONSTRAINT_H */
