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

  /// Copy constructors
  TAO_ETCL_Literal_Constraint (const TAO_ETCL_Literal_Constraint& lit);
  TAO_ETCL_Literal_Constraint (const ETCL_Literal_Constraint *lit);

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

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "TAO_ETCL_Constraint.inl"
#endif /* __ACE_INLINE__ */

#endif /* TAO_ETCL_CONSTRAINT_H */
