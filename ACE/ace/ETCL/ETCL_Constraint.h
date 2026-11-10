// -*- C++ -*-

//=============================================================================
/**
 *  @file    ETCL_Constraint.h
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 *  @author Jeff Parsons (j.parsons@vanderbilt.edu)
 */
//=============================================================================

#ifndef ACE_ETCL_CONSTRAINT_H
#define ACE_ETCL_CONSTRAINT_H

#include /**/ "ace/pre.h"

#include <memory>

#include "ace/SString.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/CDR_Base.h"

#include "ace/ETCL/ace_etcl_export.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

using Literal_Type = unsigned long;

class ETCL_Constraint_Visitor;

class ACE_ETCL_Export ETCL_Constraint
{
public:
  ETCL_Constraint () = default;
  virtual ~ETCL_Constraint () = default;

  virtual int accept (ETCL_Constraint_Visitor *visitor);

protected:
  enum
  {
    ACE_ETCL_STRING,
    ACE_ETCL_DOUBLE,
    ACE_ETCL_UNSIGNED,
    ACE_ETCL_SIGNED,
    ACE_ETCL_INTEGER,
    ACE_ETCL_BOOLEAN,
    ACE_ETCL_COMPONENT,
    ACE_ETCL_UNKNOWN
  };
};

// ****************************************************************

class ACE_ETCL_Export ETCL_Literal_Constraint
  : public ETCL_Constraint
{
public:
  ETCL_Literal_Constraint ();

  explicit ETCL_Literal_Constraint (ACE_CDR::ULong uinteger);
  explicit ETCL_Literal_Constraint (ACE_CDR::Long integer);
  explicit ETCL_Literal_Constraint (ACE_CDR::Boolean boolean);
  explicit ETCL_Literal_Constraint (ACE_CDR::Double doub);
  explicit ETCL_Literal_Constraint (char const *str);

  ETCL_Literal_Constraint (ETCL_Literal_Constraint const &lit);
  ETCL_Literal_Constraint (ETCL_Literal_Constraint &&) = delete;

  ~ETCL_Literal_Constraint() override;

  /// Visitor accept method.
  int accept (ETCL_Constraint_Visitor *visitor) override;

  Literal_Type expr_type () const;

  /// Assignment operator.
  void operator= (ETCL_Literal_Constraint const &co);
  void operator= (ETCL_Literal_Constraint &&) = delete;

  // Conversion routines.
  operator ACE_CDR::Boolean () const;
  operator ACE_CDR::ULong () const;
  operator ACE_CDR::Long () const;
  operator ACE_CDR::Double () const;
  operator char const * () const;

  // Return the type represented by this MysteryOperand.

  // = Boolean operators.

  bool
  operator< (ETCL_Literal_Constraint const &rhs);

  bool
  operator<= (ETCL_Literal_Constraint const &rhs);

  bool
  operator> (ETCL_Literal_Constraint const &rhs);

  bool
  operator>= (ETCL_Literal_Constraint const &rhs);

  bool
  operator== (ETCL_Literal_Constraint const &rhs);

  bool
  operator!= (ETCL_Literal_Constraint const &rhs);

  // = Arithmetic operators.

  ETCL_Literal_Constraint
  operator+ (ETCL_Literal_Constraint const &rhs);

  ETCL_Literal_Constraint
  operator- (ETCL_Literal_Constraint const &rhs);

  ETCL_Literal_Constraint
  operator* (ETCL_Literal_Constraint const &rhs);

  ETCL_Literal_Constraint
  operator/ (ETCL_Literal_Constraint const &rhs);

  // Unary minus.
  ETCL_Literal_Constraint
  operator- ();

  /// Ensure both operands are of the same simple numeric type.
  virtual Literal_Type
  widest_type (ETCL_Literal_Constraint const &rhs);

protected:
  /// Private copy method.
  void copy (ETCL_Literal_Constraint const &co);

  /// Union of the possible literal types.
  union
  {
    char *str_; // Managed with ACE::strnew and ACE::strdelete.
    ACE_CDR::ULong uinteger_;
    ACE_CDR::Long integer_;
    ACE_CDR::Boolean bool_;
    ACE_CDR::Double double_;
  } op_ {};

  /// The actual types of the ETCL_Literal_Constraint.
  Literal_Type type_;
};

// ****************************************************************

class ACE_ETCL_Export ETCL_Identifier : public ETCL_Constraint
{
public:
  explicit ETCL_Identifier (char const *value);

  /// Get the value
  char const *value () const;

  // = The Constraint methods.
  int accept (ETCL_Constraint_Visitor *visitor) override;

private:
  /// The value
  ACE_CString string_;
};

// ****************************************************************

/// Expression node constructors take ownership of child pointers of the expected node types.
class ACE_ETCL_Export ETCL_Union_Value : public ETCL_Constraint
{
public:
  ETCL_Union_Value (int sign,
                    ETCL_Constraint *integer);
  explicit ETCL_Union_Value (ETCL_Constraint *string = nullptr);
  ~ETCL_Union_Value () override;

  ETCL_Union_Value (ETCL_Union_Value const &) = delete;
  ETCL_Union_Value (ETCL_Union_Value &&) = delete;
  ETCL_Union_Value &operator= (ETCL_Union_Value const &) = delete;
  ETCL_Union_Value &operator= (ETCL_Union_Value &&) = delete;

  int sign () const;
  ETCL_Literal_Constraint *integer () const;
  ETCL_Literal_Constraint *string () const;

  int accept (ETCL_Constraint_Visitor *visitor) override;

private:
  int sign_ {};
  std::unique_ptr<ETCL_Literal_Constraint> integer_ {};
  std::unique_ptr<ETCL_Literal_Constraint> string_ {};
};

class ACE_ETCL_Export ETCL_Union_Pos : public ETCL_Constraint
{
public:
  explicit ETCL_Union_Pos (ETCL_Constraint *union_value = nullptr,
                           ETCL_Constraint *component = nullptr);
  ~ETCL_Union_Pos () override;

  ETCL_Union_Pos (ETCL_Union_Pos const &) = delete;
  ETCL_Union_Pos (ETCL_Union_Pos &&) = delete;
  ETCL_Union_Pos &operator= (ETCL_Union_Pos const &) = delete;
  ETCL_Union_Pos &operator= (ETCL_Union_Pos &&) = delete;

  ETCL_Union_Value *union_value () const;
  ETCL_Constraint *component () const;

  int accept (ETCL_Constraint_Visitor *visitor) override;

private:
  std::unique_ptr<ETCL_Union_Value> union_value_ {};
  std::unique_ptr<ETCL_Constraint> component_ {};
};

class ACE_ETCL_Export ETCL_Component_Pos : public ETCL_Constraint
{
public:
  explicit ETCL_Component_Pos (ETCL_Constraint *integer = nullptr,
                               ETCL_Constraint *component = nullptr);
  ~ETCL_Component_Pos () override;

  ETCL_Component_Pos (ETCL_Component_Pos const &) = delete;
  ETCL_Component_Pos (ETCL_Component_Pos &&) = delete;
  ETCL_Component_Pos &operator= (ETCL_Component_Pos const &) = delete;
  ETCL_Component_Pos &operator= (ETCL_Component_Pos &&) = delete;

  ETCL_Literal_Constraint *integer () const;
  ETCL_Constraint *component () const;

  int accept (ETCL_Constraint_Visitor *visitor) override;

private:
  std::unique_ptr<ETCL_Literal_Constraint> integer_ {};
  std::unique_ptr<ETCL_Constraint> component_ {};
};

class ACE_ETCL_Export ETCL_Component_Assoc : public ETCL_Constraint
{
public:
  explicit ETCL_Component_Assoc (ETCL_Constraint *identifier = nullptr,
                                 ETCL_Constraint *component = nullptr);
  ~ETCL_Component_Assoc () override;

  ETCL_Component_Assoc (ETCL_Component_Assoc const &) = delete;
  ETCL_Component_Assoc (ETCL_Component_Assoc &&) = delete;
  ETCL_Component_Assoc &operator= (ETCL_Component_Assoc const &) = delete;
  ETCL_Component_Assoc &operator= (ETCL_Component_Assoc &&) = delete;

  ETCL_Identifier *identifier () const;
  ETCL_Constraint *component () const;

  int accept (ETCL_Constraint_Visitor *visitor) override;

private:
  std::unique_ptr<ETCL_Identifier> identifier_ {};
  std::unique_ptr<ETCL_Constraint> component_ {};
};

class ACE_ETCL_Export ETCL_Component_Array : public ETCL_Constraint
{
public:
  explicit ETCL_Component_Array (ETCL_Constraint *integer = nullptr,
                                 ETCL_Constraint *component = nullptr);
  ~ETCL_Component_Array () override;

  ETCL_Component_Array (ETCL_Component_Array const &) = delete;
  ETCL_Component_Array (ETCL_Component_Array &&) = delete;
  ETCL_Component_Array &operator= (ETCL_Component_Array const &) = delete;
  ETCL_Component_Array &operator= (ETCL_Component_Array &&) = delete;

  ETCL_Literal_Constraint *integer () const;
  ETCL_Constraint *component () const;

  int accept (ETCL_Constraint_Visitor *visitor) override;

private:
  std::unique_ptr<ETCL_Literal_Constraint> integer_ {};
  std::unique_ptr<ETCL_Constraint> component_ {};
};

class ACE_ETCL_Export ETCL_Special : public ETCL_Constraint
{
public:
  ETCL_Special () = default;
  explicit ETCL_Special (int type);
  ~ETCL_Special () override = default;

  int type () const;

  int accept (ETCL_Constraint_Visitor *visitor) override;

private:
  int type_ {};
};

class ACE_ETCL_Export ETCL_Component : public ETCL_Constraint
{
public:
  explicit ETCL_Component (ETCL_Constraint *identifier = nullptr,
                           ETCL_Constraint *component = nullptr);
  ~ETCL_Component () override;

  ETCL_Component (ETCL_Component const &) = delete;
  ETCL_Component (ETCL_Component &&) = delete;
  ETCL_Component &operator= (ETCL_Component const &) = delete;
  ETCL_Component &operator= (ETCL_Component &&) = delete;

  ETCL_Identifier *identifier () const;
  ETCL_Constraint *component () const;

  int accept (ETCL_Constraint_Visitor *visitor) override;

private:
  std::unique_ptr<ETCL_Identifier> identifier_ {};
  std::unique_ptr<ETCL_Constraint> component_ {};
};

class ACE_ETCL_Export ETCL_Dot : public ETCL_Constraint
{
public:
  explicit ETCL_Dot (ETCL_Constraint *component = nullptr);
  ~ETCL_Dot () override;

  ETCL_Dot (ETCL_Dot const &) = delete;
  ETCL_Dot (ETCL_Dot &&) = delete;
  ETCL_Dot &operator= (ETCL_Dot const &) = delete;
  ETCL_Dot &operator= (ETCL_Dot &&) = delete;

  ETCL_Constraint *component () const;

  int accept (ETCL_Constraint_Visitor *visitor) override;

private:
  std::unique_ptr<ETCL_Constraint> component_ {};
};

class ACE_ETCL_Export ETCL_Eval : public ETCL_Constraint
{
public:
  explicit ETCL_Eval (ETCL_Constraint *component = nullptr);
  ~ETCL_Eval () override;

  ETCL_Eval (ETCL_Eval const &) = delete;
  ETCL_Eval (ETCL_Eval &&) = delete;
  ETCL_Eval &operator= (ETCL_Eval const &) = delete;
  ETCL_Eval &operator= (ETCL_Eval &&) = delete;

  ETCL_Constraint *component () const;

  int accept (ETCL_Constraint_Visitor *visitor) override;

private:
  std::unique_ptr<ETCL_Constraint> component_ {};
};

class ACE_ETCL_Export ETCL_Default : public ETCL_Constraint
{
public:
  explicit ETCL_Default (ETCL_Constraint *component = nullptr);
  ~ETCL_Default () override;

  ETCL_Default (ETCL_Default const &) = delete;
  ETCL_Default (ETCL_Default &&) = delete;
  ETCL_Default &operator= (ETCL_Default const &) = delete;
  ETCL_Default &operator= (ETCL_Default &&) = delete;

  ETCL_Constraint *component () const;

  int accept (ETCL_Constraint_Visitor *visitor) override;

private:
  std::unique_ptr<ETCL_Constraint> component_ {};
};

class ACE_ETCL_Export ETCL_Exist : public ETCL_Constraint
{
public:
  explicit ETCL_Exist (ETCL_Constraint *component = nullptr);
  ~ETCL_Exist () override;

  ETCL_Exist (ETCL_Exist const &) = delete;
  ETCL_Exist (ETCL_Exist &&) = delete;
  ETCL_Exist &operator= (ETCL_Exist const &) = delete;
  ETCL_Exist &operator= (ETCL_Exist &&) = delete;

  ETCL_Constraint *component () const;

  int accept (ETCL_Constraint_Visitor *visitor) override;

private:
  std::unique_ptr<ETCL_Constraint> component_ {};
};

class ACE_ETCL_Export ETCL_Unary_Expr : public ETCL_Constraint
{
public:
  ETCL_Unary_Expr (int type,
                   ETCL_Constraint *subexpr);
  ~ETCL_Unary_Expr () override;

  ETCL_Unary_Expr (ETCL_Unary_Expr const &) = delete;
  ETCL_Unary_Expr (ETCL_Unary_Expr &&) = delete;
  ETCL_Unary_Expr &operator= (ETCL_Unary_Expr const &) = delete;
  ETCL_Unary_Expr &operator= (ETCL_Unary_Expr &&) = delete;

  int type () const;
  ETCL_Constraint *subexpr () const;

  int accept (ETCL_Constraint_Visitor *visitor) override;

private:
  int type_ {};
  std::unique_ptr<ETCL_Constraint> subexpr_ {};
};

class ACE_ETCL_Export ETCL_Binary_Expr : public ETCL_Constraint
{
public:
  ETCL_Binary_Expr (int type,
                    ETCL_Constraint *lhs,
                    ETCL_Constraint *rhs);
  ~ETCL_Binary_Expr () override;

  ETCL_Binary_Expr (ETCL_Binary_Expr const &) = delete;
  ETCL_Binary_Expr (ETCL_Binary_Expr &&) = delete;
  ETCL_Binary_Expr &operator= (ETCL_Binary_Expr const &) = delete;
  ETCL_Binary_Expr &operator= (ETCL_Binary_Expr &&) = delete;

  int type () const;
  ETCL_Constraint *rhs () const;
  ETCL_Constraint *lhs () const;

  int accept (ETCL_Constraint_Visitor *visitor) override;

private:
  int type_ {};
  std::unique_ptr<ETCL_Constraint> lhs_ {};
  std::unique_ptr<ETCL_Constraint> rhs_ {};
};

class ACE_ETCL_Export ETCL_Preference : public ETCL_Constraint
{
public:
  ETCL_Preference () = default;
  explicit ETCL_Preference (int type,
                            ETCL_Constraint *subexpr = nullptr);
  ~ETCL_Preference () override;

  ETCL_Preference (ETCL_Preference const &) = delete;
  ETCL_Preference (ETCL_Preference &&) = delete;
  ETCL_Preference &operator= (ETCL_Preference const &) = delete;
  ETCL_Preference &operator= (ETCL_Preference &&) = delete;

  int type () const;
  ETCL_Constraint *subexpr () const;

  int accept (ETCL_Constraint_Visitor *visitor) override;

private:
  int type_ {};
  std::unique_ptr<ETCL_Constraint> subexpr_ {};
};

ACE_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "ace/ETCL/ETCL_Constraint.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif // ACE_ETCL_CONSTRAINT_H
