/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//    orbsvcs/ETCL
//
// = FILENAME
//    ECTL_Constraint
//
// = AUTHOR
//    Carlos O'Ryan (coryan@cs.wustl.edu)
//    Jeff Parsons (parsons@cs.wustl.edu)
//
// ============================================================================

#ifndef TAO_ETCL_CONSTRAINT_H
#define TAO_ETCL_CONSTRAINT_H

#include "etcl_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/corbafwd.h"
#include "ace/SString.h"

typedef unsigned short TAO_Literal_Type;
#define TAO_UNKNOWN 999

class TAO_ETCL_Constraint_Visitor;
class TAO_String_Manager;

class TAO_ETCL_Export TAO_ETCL_Constraint
{
public:
  TAO_ETCL_Constraint (void);
  virtual ~TAO_ETCL_Constraint (void);
  // Constructor and destructor

  virtual int accept (TAO_ETCL_Constraint_Visitor *visitor) = 0;
};

// ****************************************************************

class TAO_ETCL_Export TAO_ETCL_Literal_Constraint 
  : public TAO_ETCL_Constraint
{
public:
  TAO_ETCL_Literal_Constraint (void);

  // = Constructors for each of the various types of literals.

  TAO_ETCL_Literal_Constraint (CORBA::Any* any);
  TAO_ETCL_Literal_Constraint (CORBA::ULong uinteger);
  TAO_ETCL_Literal_Constraint (CORBA::Long integer);
  TAO_ETCL_Literal_Constraint (CORBA::Boolean boolean);
  TAO_ETCL_Literal_Constraint (CORBA::Double doub);
  TAO_ETCL_Literal_Constraint (const char* str);

  TAO_ETCL_Literal_Constraint (const TAO_ETCL_Literal_Constraint& lit);
  // Copy constructor

  ~TAO_ETCL_Literal_Constraint(void);
  // Destructor.

  int accept (TAO_ETCL_Constraint_Visitor* visitor);
  // Visitor accept method.

  TAO_Literal_Type expr_type (void) const;

  void operator= (const TAO_ETCL_Literal_Constraint& co);
  // Assignment operator.

  // Conversion routines.
  operator CORBA::Boolean (void) const;
  operator CORBA::ULong (void) const;
  operator CORBA::Long (void) const;
  operator CORBA::Double (void) const;
  operator const char* (void) const;
  operator const CORBA::Any* (void) const;

  // Return the type represented by this MysteryOperand.

  // = Boolean operators.

  CORBA::Boolean
  operator< (const TAO_ETCL_Literal_Constraint& rhs);

  CORBA::Boolean
  operator<= (const TAO_ETCL_Literal_Constraint& rhs);

  CORBA::Boolean
  operator> (const TAO_ETCL_Literal_Constraint& rhs);

  CORBA::Boolean
  operator>= (const TAO_ETCL_Literal_Constraint& rhs);

  CORBA::Boolean
  operator== (const TAO_ETCL_Literal_Constraint& rhs);

  CORBA::Boolean
  operator!= (const TAO_ETCL_Literal_Constraint& rhs);

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

  TAO_Literal_Type
  widest_type (const TAO_ETCL_Literal_Constraint& rhs);
  // Ensure both operands are of the same simple numeric type.

  static TAO_Literal_Type
  comparable_type (CORBA::TypeCode_ptr type);
  // Determine the comparable Expression Type from the CORBA type

private:
  void copy (const TAO_ETCL_Literal_Constraint& co);
  // Private copy method.

  union
  {
    char* str_;
    CORBA::Any_ptr any_;
    CORBA::ULong uinteger_;
    CORBA::Long integer_;
    CORBA::Boolean bool_;
    CORBA::Double double_;
  } op_;
  // Union of the possible literal types.

  unsigned short type_;
  // The actual types of the TAO_ETCL_Literal_Constraint.
};

// ****************************************************************

class TAO_ETCL_Export TAO_ETCL_Identifier : public TAO_ETCL_Constraint
{
public:
  TAO_ETCL_Identifier (const char *value);

  const char *value (void) const;
  // Get the value

  // = The Constraint methods.
  int accept (TAO_ETCL_Constraint_Visitor *visitor);

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
  TAO_ETCL_Literal_Constraint *integer (void) const;
  TAO_ETCL_Literal_Constraint *string (void) const;

  // = The Constraint methods.
  int accept (TAO_ETCL_Constraint_Visitor *visitor);

private:
  int sign_;
  TAO_ETCL_Literal_Constraint *integer_;
  TAO_ETCL_Literal_Constraint *string_;
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
  int accept (TAO_ETCL_Constraint_Visitor *visitor);

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

  TAO_ETCL_Literal_Constraint *integer (void) const;
  TAO_ETCL_Constraint *component (void) const;

  // = The Constraint methods.
  int accept (TAO_ETCL_Constraint_Visitor *visitor);

private:
  TAO_ETCL_Literal_Constraint *integer_;
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
  int accept (TAO_ETCL_Constraint_Visitor *visitor);

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

  TAO_ETCL_Literal_Constraint *integer (void) const;
  TAO_ETCL_Constraint *component (void) const;

  // = The Constraint methods.
  int accept (TAO_ETCL_Constraint_Visitor *visitor);

private:
  TAO_ETCL_Literal_Constraint *integer_;
  TAO_ETCL_Constraint *component_;
};

class TAO_ETCL_Export TAO_ETCL_Special : public TAO_ETCL_Constraint
{
public:
  TAO_ETCL_Special (int type);
  virtual ~TAO_ETCL_Special (void);

  int type (void) const;

  // = The Constraint methods.
  int accept (TAO_ETCL_Constraint_Visitor *visitor);

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
  int accept (TAO_ETCL_Constraint_Visitor *visitor);

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
  int accept (TAO_ETCL_Constraint_Visitor *visitor);

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
  int accept (TAO_ETCL_Constraint_Visitor *visitor);

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
  int accept (TAO_ETCL_Constraint_Visitor *visitor);

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
  int accept (TAO_ETCL_Constraint_Visitor *visitor);

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
  int accept (TAO_ETCL_Constraint_Visitor *visitor);

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
  int accept (TAO_ETCL_Constraint_Visitor *visitor);

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
  int accept (TAO_ETCL_Constraint_Visitor *visitor);

private:
  int type_;
  TAO_ETCL_Constraint *subexpr_;
};

#if defined (__ACE_INLINE__)
#include "ETCL_Constraint.inl"
#endif /* __ACE_INLINE__ */

#endif /* TAO_ETCL_CONSTRAINT_H */
