/* -*- C++ -*- */

// $Id$

// =========================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    DynAnyFactory.h
//
// = AUTHOR
//    Carlos O'Ryan <coryan@uci.edu>
//
// =========================================================================

#ifndef TAO_DYNANYFACTORY_H
#define TAO_DYNANYFACTORY_H
#include "ace/pre.h"

#include "DynamicAny.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (_MSC_VER)
# if (_MSC_VER >= 1200)
#  pragma warning(push)
# endif /* _MSC_VER >= 1200 */
# pragma warning (disable:4250)
#endif /* _MSC_VER */

class TAO_DynamicAny_Export TAO_DynAnyFactory : public virtual DynamicAny::DynAnyFactory, public virtual TAO_Local_RefCounted_Object
{
  // = DESCRIPTION
  //   Implements the DynamicAnyFactory interface and provides several
  //   utility functions shared by all the Dynamic Any components.
  //
public:
  TAO_DynAnyFactory (void);
  // Constructor

  static CORBA::TCKind unalias (CORBA_TypeCode_ptr tc,
                                CORBA::Environment& ACE_TRY_ENV);
  // Obtain the kind of object, after all aliasing has been removed.

  static DynamicAny::DynAny_ptr
     make_dyn_any (const CORBA_Any &any,
                   CORBA::Environment &ACE_TRY_ENV);
  // Create the correct type of DynAny object for <any>

  static DynamicAny::DynAny_ptr
     make_dyn_any (CORBA::TypeCode_ptr tc,
                   CORBA::Environment &ACE_TRY_ENV);
  // Create the correct type of DynAny object for <tc>

  // = The DynamicAnyFactory methods
  virtual DynamicAny::DynAny_ptr create_dyn_any (
      const CORBA::Any & value,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAnyFactory::InconsistentTypeCode
    ));

  virtual DynamicAny::DynAny_ptr create_dyn_any_from_type_code (
      CORBA::TypeCode_ptr type,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      DynamicAny::DynAnyFactory::InconsistentTypeCode
    ));

#if 0
  static DynamicAny::DynAny_ptr
      create_basic_dyn_any (CORBA_TypeCode_ptr tc,
                            CORBA::Environment& ACE_TRY_ENV);
  static DynamicAny::DynStruct_ptr
      create_dyn_struct (CORBA_TypeCode_ptr tc,
                         CORBA::Environment& ACE_TRY_ENV);
  static DynamicAny::DynSequence_ptr
      create_dyn_sequence (CORBA_TypeCode_ptr tc,
                           CORBA::Environment& ACE_TRY_ENV);
  static DynamicAny::DynArray_ptr
      create_dyn_array (CORBA_TypeCode_ptr tc,
                        CORBA::Environment& ACE_TRY_ENV);
  static DynamicAny::DynUnion_ptr
      create_dyn_union (CORBA_TypeCode_ptr tc,
                        CORBA::Environment& ACE_TRY_ENV);
  static DynamicAny::DynEnum_ptr
      create_dyn_enum (CORBA_TypeCode_ptr tc,
                       CORBA::Environment& ACE_TRY_ENV);
  static DynamicAny::DynAny_ptr
      create_dyn_any (CORBA_TypeCode_ptr tc,
                      CORBA::Environment& ACE_TRY_ENV);
  // Create particular types of dynamic anys.
  static DynamicAny::DynAny_ptr
      create_basic_dyn_any (const CORBA_Any &any,
                        CORBA::Environment &ACE_TRY_ENV);
  // Create a basic dynamic any (i.e. one for primitive types) using
  // just the TypeCode.
#endif /* 0 */

private:
  // Use copy() or assign() instead of these
  TAO_DynAnyFactory (const TAO_DynAnyFactory &src);
  TAO_DynAnyFactory &operator= (const TAO_DynAnyFactory &src);
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* TAO_DYNANYFACTORY_H */
