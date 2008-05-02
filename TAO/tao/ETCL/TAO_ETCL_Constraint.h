// -*- C++ -*-

//=============================================================================
/**
 *  @file    TAO_ETCL_Constraint.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 *  @author Jeff Parsons (parsons@cs.wustl.edu)
 */
//=============================================================================

#ifndef TAO_ETCL_CONSTRAINT_H
#define TAO_ETCL_CONSTRAINT_H

#include "ace/ETCL/ETCL_Constraint.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/AnyTypeCode/Any_Impl.h"

#include "tao/ETCL/tao_etcl_export.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

class ETCL_Constraint_Visitor;

ACE_END_VERSIONED_NAMESPACE_DECL

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace CORBA
{
  class Any;
  typedef Any *Any_ptr;
}

class TAO_ETCL_Export TAO_ETCL_Literal_Constraint
  : public ETCL_Literal_Constraint
{
public:
  TAO_ETCL_Literal_Constraint (void);

  // = Constructors for each of the various types of literals.

  explicit TAO_ETCL_Literal_Constraint (CORBA::ULong uinteger);
  explicit TAO_ETCL_Literal_Constraint (CORBA::Long integer);
  explicit TAO_ETCL_Literal_Constraint (CORBA::Boolean boolean);
  explicit TAO_ETCL_Literal_Constraint (CORBA::Double doub);
  explicit TAO_ETCL_Literal_Constraint (const char* str);
  explicit TAO_ETCL_Literal_Constraint (CORBA::Any* any);

  /// Copy constructor
  TAO_ETCL_Literal_Constraint (const TAO_ETCL_Literal_Constraint& lit);

  /// Destructor.
  virtual ~TAO_ETCL_Literal_Constraint(void);

  /// Assignment operator.
  void operator= (const TAO_ETCL_Literal_Constraint& co);

  // Conversion routines.
  operator ACE_CDR::Boolean (void) const;
  operator ACE_CDR::ULong (void) const;
  operator ACE_CDR::Long (void) const;
  operator ACE_CDR::Double (void) const;
  operator const char* (void) const;
  operator TAO::Any_Impl* (void) const;

  // Return the type represented by this MysteryOperand.

  // = Arithmetic operators.

  TAO_ETCL_Literal_Constraint
  operator+ (const TAO_ETCL_Literal_Constraint& rhs);

  TAO_ETCL_Literal_Constraint
  operator- (const TAO_ETCL_Literal_Constraint& rhs);

  TAO_ETCL_Literal_Constraint
  operator* (const TAO_ETCL_Literal_Constraint& rhs);

  TAO_ETCL_Literal_Constraint
  operator/ (const TAO_ETCL_Literal_Constraint& rhs);

  // Unary minus.
  TAO_ETCL_Literal_Constraint
  operator- (void);

  /// Ensure both operands are of the same simple numeric type.
  virtual Literal_Type
  widest_type (const ETCL_Literal_Constraint& rhs);

  /// Determine the comparable Expression Type from the CORBA type
  static Literal_Type
  comparable_type (CORBA::TypeCode_ptr type);

private:
  /// Private copy method.
  void copy (const TAO_ETCL_Literal_Constraint& co);

  /// Possible literal type not covered by the base class.
  TAO::Any_Impl* any_;
};

// ****************************************************************

class TAO_ETCL_Export TAO_ETCL_Identifier : public ETCL_Identifier
{
public:
  TAO_ETCL_Identifier (const char *value);
};

// ****************************************************************

class TAO_ETCL_Export TAO_ETCL_Union_Value : public ETCL_Union_Value
{
public:
  TAO_ETCL_Union_Value (int sign,
                        ETCL_Constraint *integer);
  TAO_ETCL_Union_Value (ETCL_Constraint *string);
  virtual ~TAO_ETCL_Union_Value (void);

  int sign (void) const;
  TAO_ETCL_Literal_Constraint *integer (void) const;
  TAO_ETCL_Literal_Constraint *string (void) const;

  // = The Constraint methods.
  int accept (ETCL_Constraint_Visitor *visitor);

private:
  int sign_;
  TAO_ETCL_Literal_Constraint *integer_;
  TAO_ETCL_Literal_Constraint *string_;
};

class TAO_ETCL_Export TAO_ETCL_Union_Pos : public ETCL_Union_Pos
{
public:
  TAO_ETCL_Union_Pos (ETCL_Constraint *union_value,
                      ETCL_Constraint *component);
  virtual ~TAO_ETCL_Union_Pos (void);

  TAO_ETCL_Union_Value *union_value (void) const;
  ETCL_Constraint *component (void) const;

  // = The Constraint methods.
  int accept (ETCL_Constraint_Visitor *visitor);

private:
  TAO_ETCL_Union_Value *union_value_;
  ETCL_Constraint *component_;
};

class TAO_ETCL_Export TAO_ETCL_Component_Pos : public ETCL_Component_Pos
{
public:
  TAO_ETCL_Component_Pos (ETCL_Constraint *integer,
                          ETCL_Constraint *component);
  virtual ~TAO_ETCL_Component_Pos (void);

  TAO_ETCL_Literal_Constraint *integer (void) const;
  ETCL_Constraint *component (void) const;

  // = The Constraint methods.
  int accept (ETCL_Constraint_Visitor *visitor);

private:
  TAO_ETCL_Literal_Constraint *integer_;
  ETCL_Constraint *component_;
};

class TAO_ETCL_Export TAO_ETCL_Component_Assoc : public ETCL_Component_Assoc
{
public:
  TAO_ETCL_Component_Assoc (ETCL_Constraint *identifier,
                            ETCL_Constraint *component);
  virtual ~TAO_ETCL_Component_Assoc (void);

  TAO_ETCL_Identifier *identifier (void) const;
  ETCL_Constraint *component (void) const;

  // = The Constraint methods.
  int accept (ETCL_Constraint_Visitor *visitor);

private:
  TAO_ETCL_Identifier *identifier_;
  ETCL_Constraint *component_;
};

class TAO_ETCL_Export TAO_ETCL_Component_Array : public ETCL_Component_Array
{
public:
  TAO_ETCL_Component_Array (ETCL_Constraint *integer,
                            ETCL_Constraint *component);
  virtual ~TAO_ETCL_Component_Array (void);

  TAO_ETCL_Literal_Constraint *integer (void) const;
  ETCL_Constraint *component (void) const;

  // = The Constraint methods.
  int accept (ETCL_Constraint_Visitor *visitor);

private:
  TAO_ETCL_Literal_Constraint *integer_;
  ETCL_Constraint *component_;
};

class TAO_ETCL_Export TAO_ETCL_Special : public ETCL_Special
{
public:
  TAO_ETCL_Special (int type);
  virtual ~TAO_ETCL_Special (void);

  int type (void) const;

  // = The Constraint methods.
  int accept (ETCL_Constraint_Visitor *visitor);

private:
  int type_;
};

class TAO_ETCL_Export TAO_ETCL_Component : public ETCL_Component
{
public:
  TAO_ETCL_Component (ETCL_Constraint *identifier,
                      ETCL_Constraint *component);
  virtual ~TAO_ETCL_Component (void);

  TAO_ETCL_Identifier *identifier (void) const;
  ETCL_Constraint *component (void) const;

  // = The Constraint methods.
  int accept (ETCL_Constraint_Visitor *visitor);

private:
  TAO_ETCL_Identifier *identifier_;
  ETCL_Constraint *component_;
};

class TAO_ETCL_Export TAO_ETCL_Dot : public ETCL_Dot
{
public:
  TAO_ETCL_Dot (ETCL_Constraint *component);
  virtual ~TAO_ETCL_Dot (void);

  ETCL_Constraint *component (void) const;

  // = The Constraint methods.
  int accept (ETCL_Constraint_Visitor *visitor);

private:
  ETCL_Constraint *component_;
};

class TAO_ETCL_Export TAO_ETCL_Eval : public ETCL_Eval
{
public:
  TAO_ETCL_Eval (ETCL_Constraint *component);
  virtual ~TAO_ETCL_Eval (void);

  ETCL_Constraint *component (void) const;

  // = The Constraint methods.
  int accept (ETCL_Constraint_Visitor *visitor);

private:
  ETCL_Constraint *component_;
};

class TAO_ETCL_Export TAO_ETCL_Default : public ETCL_Default
{
public:
  TAO_ETCL_Default (ETCL_Constraint *component);
  virtual ~TAO_ETCL_Default (void);

  ETCL_Constraint *component (void) const;

  // = The Constraint methods.
  int accept (ETCL_Constraint_Visitor *visitor);

private:
  ETCL_Constraint *component_;
};

class TAO_ETCL_Export TAO_ETCL_Exist : public ETCL_Exist
{
public:
  TAO_ETCL_Exist (ETCL_Constraint *component);
  virtual ~TAO_ETCL_Exist (void);

  ETCL_Constraint *component (void) const;

  // = The Constraint methods.
  int accept (ETCL_Constraint_Visitor *visitor);

private:
  ETCL_Constraint *component_;
};

class TAO_ETCL_Export TAO_ETCL_Unary_Expr : public ETCL_Unary_Expr
{
public:
  TAO_ETCL_Unary_Expr (int type,
                       ETCL_Constraint *subexpr);
  virtual ~TAO_ETCL_Unary_Expr (void);
};

class TAO_ETCL_Export TAO_ETCL_Binary_Expr : public ETCL_Binary_Expr
{
public:
  TAO_ETCL_Binary_Expr (int type,
                        ETCL_Constraint *lhs,
                        ETCL_Constraint *rhs);
  virtual ~TAO_ETCL_Binary_Expr (void);
};

class TAO_ETCL_Export TAO_ETCL_Preference : public ETCL_Preference
{
public:
  TAO_ETCL_Preference (int type,
                       ETCL_Constraint *subexpr = 0);
  virtual ~TAO_ETCL_Preference (void);

  int type (void) const;
  ETCL_Constraint *subexpr (void) const;

  // = The Constraint methods.
  int accept (ETCL_Constraint_Visitor *visitor);

private:
  int type_;
  ETCL_Constraint *subexpr_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "TAO_ETCL_Constraint.inl"
#endif /* __ACE_INLINE__ */

#endif /* TAO_ETCL_CONSTRAINT_H */
