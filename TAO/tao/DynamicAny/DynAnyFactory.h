// -*- C++ -*-
//
// $Id$

//=============================================================================
/**
 *  @file    DynAnyFactory.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan <coryan@uci.edu>
 */
//=============================================================================


#ifndef TAO_DYNANYFACTORY_H
#define TAO_DYNANYFACTORY_H
#include /**/ "ace/pre.h"

#include "tao/DynamicAny/DynamicAny.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"

#if defined (_MSC_VER)
# pragma warning(push)
# pragma warning (disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_DynAnyFactory
 *
 * Implements the DynamicAnyFactory interface and provides several
 * utility functions shared by all the Dynamic Any components.
 */
class TAO_DynamicAny_Export TAO_DynAnyFactory
  : public virtual DynamicAny::DynAnyFactory,
    public virtual TAO_Local_RefCounted_Object
{
public:
  /// Constructor.
  TAO_DynAnyFactory (void);

  // = The DynamicAnyFactory methods.
  virtual DynamicAny::DynAny_ptr create_dyn_any (const CORBA::Any & value);

  virtual DynamicAny::DynAny_ptr create_dyn_any_from_type_code (
      CORBA::TypeCode_ptr type);

  virtual DynamicAny::DynAny_ptr create_dyn_any_without_truncation (
      const CORBA::Any & value);

  virtual DynamicAny::DynAnySeq * create_multiple_dyn_anys (
      const DynamicAny::AnySeq & values,
      ::CORBA::Boolean allow_truncate);

  virtual DynamicAny::AnySeq * create_multiple_anys (
      const DynamicAny::DynAnySeq & values);

  /// TAO-specific methods.

  /// Obtain the kind of object, after all aliasing has been removed.
  static CORBA::TCKind unalias (CORBA::TypeCode_ptr tc);

  /// Same as above, but returns type code instead of TCKind. Caller
  /// must release the return value.
  static CORBA::TypeCode_ptr strip_alias (CORBA::TypeCode_ptr tc);

private:
  // Not allowed.
  TAO_DynAnyFactory (const TAO_DynAnyFactory &src);
  TAO_DynAnyFactory &operator= (const TAO_DynAnyFactory &src);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
# pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* TAO_DYNANYFACTORY_H */
