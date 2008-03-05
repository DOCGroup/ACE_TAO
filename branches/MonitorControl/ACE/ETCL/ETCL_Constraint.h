// -*- C++ -*-

//=============================================================================
/**
 *  @file    ETCL_Constraint.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 *  @author Jeff Parsons (j.parsons@vanderbilt.edu)
 */
//=============================================================================

#ifndef ETCL_CONSTRAINT_H
#define ETCL_CONSTRAINT_H

#include /**/ "ace/pre.h"

#include "ace/SString.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/CDR_Base.h"

#include "etcl_export.h"
#include "ETCL_macros.h"

BEGIN_VERSIONED_NAMESPACE_MACRO

typedef unsigned short Literal_Type;
#define TAO_UNKNOWN 999

class ETCL_Constraint_Visitor;

class ETCL_EXPORT_MACRO ETCL_Constraint
{
public:
  /// Constructor and destructor
  ETCL_Constraint (void);
  virtual ~ETCL_Constraint (void);

  virtual int accept (ETCL_Constraint_Visitor *visitor);
};

// ****************************************************************

class ETCL_EXPORT_MACRO ETCL_Literal_Constraint
  : public ETCL_Constraint
{
public:
  ETCL_Literal_Constraint (void);

  // = Constructors for each of the various types of literals.

  explicit ETCL_Literal_Constraint (ACE_CDR::ULong uinteger);
  explicit ETCL_Literal_Constraint (ACE_CDR::Long integer);
  explicit ETCL_Literal_Constraint (ACE_CDR::Boolean boolean);
  explicit ETCL_Literal_Constraint (ACE_CDR::Double doub);
  explicit ETCL_Literal_Constraint (const char* str);

  /// Copy constructor
  ETCL_Literal_Constraint (const ETCL_Literal_Constraint& lit);

  /// Destructor.
  virtual ~ETCL_Literal_Constraint(void);

  /// Visitor accept method.
  virtual int accept (ETCL_Constraint_Visitor* visitor);

  Literal_Type expr_type (void) const;

  /// Assignment operator.
  void operator= (const ETCL_Literal_Constraint& co);

  // Conversion routines.
  operator ACE_CDR::Boolean (void) const;
  operator ACE_CDR::ULong (void) const;
  operator ACE_CDR::Long (void) const;
  operator ACE_CDR::Double (void) const;
  operator const char* (void) const;

  // Return the type represented by this MysteryOperand.

  // = Boolean operators.

  bool
  operator< (const ETCL_Literal_Constraint& rhs);

  bool
  operator<= (const ETCL_Literal_Constraint& rhs);

  bool
  operator> (const ETCL_Literal_Constraint& rhs);

  bool
  operator>= (const ETCL_Literal_Constraint& rhs);

  bool
  operator== (const ETCL_Literal_Constraint& rhs);

  bool
  operator!= (const ETCL_Literal_Constraint& rhs);

  // = Arithmetic operators.

  ETCL_Literal_Constraint
  operator+ (const ETCL_Literal_Constraint& rhs);

  ETCL_Literal_Constraint
  operator- (const ETCL_Literal_Constraint& rhs);

  ETCL_Literal_Constraint
  operator* (const ETCL_Literal_Constraint& rhs);

  ETCL_Literal_Constraint
  operator/ (const ETCL_Literal_Constraint& rhs);

  // Unary minus.
  ETCL_Literal_Constraint
  operator- (void);

  /// Ensure both operands are of the same simple numeric type.
  virtual Literal_Type
  widest_type (const ETCL_Literal_Constraint& rhs);

protected:
  /// Private copy method.
  void copy (const ETCL_Literal_Constraint& co);

  /// Union of the possible literal types.
  union
  {
    char* str_;
    ACE_CDR::ULong uinteger_;
    ACE_CDR::Long integer_;
    ACE_CDR::Boolean bool_;
    ACE_CDR::Double double_;
  } op_;

  /// The actual types of the ETCL_Literal_Constraint.
  unsigned short type_;
};

// ****************************************************************

class ETCL_EXPORT_MACRO ETCL_Identifier : public ETCL_Constraint
{
public:
  ETCL_Identifier (const char *value);

  /// Get the value
  const char *value (void) const;

  // = The Constraint methods.
  int accept (ETCL_Constraint_Visitor *visitor);

private:
  /// The value
  ACE_CString string_;
};

// ****************************************************************

class ETCL_EXPORT_MACRO ETCL_Union_Value : public ETCL_Constraint
{
public:
  ETCL_Union_Value (void);
  ETCL_Union_Value (int sign,
                    ETCL_Constraint *integer);
  ETCL_Union_Value (ETCL_Constraint *string);
};

class ETCL_EXPORT_MACRO ETCL_Union_Pos : public ETCL_Constraint
{
public:
  ETCL_Union_Pos (void);
  ETCL_Union_Pos (ETCL_Constraint *union_value,
                  ETCL_Constraint *component);
};

class ETCL_EXPORT_MACRO ETCL_Component_Pos : public ETCL_Constraint
{
public:
  ETCL_Component_Pos (void);
  ETCL_Component_Pos (ETCL_Constraint *integer,
                      ETCL_Constraint *component);
};

class ETCL_EXPORT_MACRO ETCL_Component_Assoc : public ETCL_Constraint
{
public:
  ETCL_Component_Assoc (void);
  ETCL_Component_Assoc (ETCL_Constraint *identifier,
                        ETCL_Constraint *component);
};

class ETCL_EXPORT_MACRO ETCL_Component_Array : public ETCL_Constraint
{
public:
  ETCL_Component_Array (void);
  ETCL_Component_Array (ETCL_Constraint *integer,
                        ETCL_Constraint *component);
};

class ETCL_EXPORT_MACRO ETCL_Special : public ETCL_Constraint
{
public:
  ETCL_Special (void);
  ETCL_Special (int type);
};

class ETCL_EXPORT_MACRO ETCL_Component : public ETCL_Constraint
{
public:
  ETCL_Component (void);
  ETCL_Component (ETCL_Constraint *identifier,
                  ETCL_Constraint *component);
};

class ETCL_EXPORT_MACRO ETCL_Dot : public ETCL_Constraint
{
public:
  ETCL_Dot (void);
  ETCL_Dot (ETCL_Constraint *component);
};

class ETCL_EXPORT_MACRO ETCL_Eval : public ETCL_Constraint
{
public:
  ETCL_Eval (void);
  ETCL_Eval (ETCL_Constraint *component);
};

class ETCL_EXPORT_MACRO ETCL_Default : public ETCL_Constraint
{
public:
  ETCL_Default (void);
  ETCL_Default (ETCL_Constraint *component);
};

class ETCL_EXPORT_MACRO ETCL_Exist : public ETCL_Constraint
{
public:
  ETCL_Exist (void);
  ETCL_Exist (ETCL_Constraint *component);
};

class ETCL_EXPORT_MACRO ETCL_Unary_Expr : public ETCL_Constraint
{
public:
  ETCL_Unary_Expr (int type,
                   ETCL_Constraint *subexpr);
  virtual ~ETCL_Unary_Expr (void);

  int type (void) const;
  ETCL_Constraint *subexpr (void) const;

  // = The Constraint methods.
  int accept (ETCL_Constraint_Visitor *visitor);

private:
  int type_;
  ETCL_Constraint *subexpr_;
};

class ETCL_EXPORT_MACRO ETCL_Binary_Expr : public ETCL_Constraint
{
public:
  ETCL_Binary_Expr (int type,
                    ETCL_Constraint *lhs,
                    ETCL_Constraint *rhs);
  virtual ~ETCL_Binary_Expr (void);

  int type (void) const;
  ETCL_Constraint *rhs (void) const;
  ETCL_Constraint *lhs (void) const;

  // = The Constraint methods.
  int accept (ETCL_Constraint_Visitor *visitor);

private:
  int type_;
  ETCL_Constraint *lhs_;
  ETCL_Constraint *rhs_;
};

class ETCL_EXPORT_MACRO ETCL_Preference : public ETCL_Constraint
{
public:
  ETCL_Preference (void);
  ETCL_Preference (int type,
                   ETCL_Constraint *subexpr = 0);
};

END_VERSIONED_NAMESPACE_MACRO

#if defined (__ACE_INLINE__)
#include "ETCL_Constraint.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif // ETCL_CONSTRAINT_H
